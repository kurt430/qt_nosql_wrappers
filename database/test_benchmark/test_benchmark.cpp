#include <QString>
#include <QByteArray>
#include <QtTest>
#include <QFileInfo>

#include "abstractdriver.h"
#include "driverfactory.h"

#include "test_benchmark.h"

TestNoSQL::TestNoSQL(const QString& engine) : db_(nullptr), engine_(engine)
{
    const QByteArray recFill(64, ' ');
    for(int i=0; i<100; ++i) {
        for(int k=0; k<100; ++k) {
            map_.insert(QString("key:%1:%2").arg(i).arg(k).toLocal8Bit(), QString("value:%1:%2").arg(i).arg(k).toLocal8Bit() + recFill);
        }
    }
    if (engine_.isEmpty()) engine_ = krdb::DriverFactory::avaiable().first();
}

void TestNoSQL::initTestCase()
{
    db_ =  krdb::DriverFactory::create(engine_, this);
    QVERIFY(db_);
    QString name = db_->engineVersionName();
    qInfo() << "ENGINE:" << name;
    fileName_ = name.replace('.', '_');
    QVERIFY(db_->open(fileName_));
}

qint64 fileStorageSize(const QString &path)
{
    QFileInfo fi(path);
    qint64 storageSize = 0;
    if (fi.isFile()) {
        storageSize = fi.size();
    } else if (fi.isDir()) {
        QDir dir(path);
        const auto list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for(const auto& child : list) {
            if (child.isDir()) {
                storageSize += fileStorageSize(child.absoluteFilePath());
            } else if (child.isFile()) {
                storageSize += child.size();
            }
        }
    }
    return storageSize;
}

void TestNoSQL::cleanupTestCase()
{
    db_->close();
    delete db_;
    db_ = nullptr;
    qInfo() << "Database file:" << fileName_ << "size:" << fileStorageSize(fileName_) << "bytes";
}

void TestNoSQL::put_1Mb_value()
{
    const krdb::Slice key("huge_value");
    QByteArray value(1024*1024, 0);
    for (int i=0; i<value.size(); ++i) value[i] = char(i);
    bool valid = true;
    QBENCHMARK {
        valid = db_->put(key, value);
        QVERIFY(valid);
        if (!valid) break;
    }
    if (!valid) {
        qDebug() << db_->errorText();
    }    
}

void TestNoSQL::put_10k_small_values()
{
    auto it = map_.constBegin();
    bool valid = true;
    QBENCHMARK {
        while (it != map_.constEnd()) {
            valid = db_->put(it.key(), it.value());
            QVERIFY(valid);
            if (!valid) break;
            ++it;
        }
        if (!valid) break;
    }
    if (!valid) {
        qDebug() << db_->errorText();
    }
}

void TestNoSQL::get_10k_small_values()
{
    auto it = map_.constBegin();
    QBENCHMARK {
        while (it != map_.constEnd()) {
            db_->get(it.key());
            ++it;
        }
    }
}

void TestNoSQL::check_kv_map()
{
    auto it = map_.constBegin();
    while (it != map_.constEnd()) {
        const QByteArray &expected  = it.value();
        const QByteArray actual = db_->get(it.key());
        QCOMPARE(actual, expected);
        ++it;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_SET_MAIN_SOURCE_PATH

    int retCode = 1;
    const auto engines = krdb::DriverFactory::avaiable();
    for (const auto& engine : engines) {
        TestNoSQL *testCase = new TestNoSQL(engine);
        retCode = QTest::qExec(testCase, argc, argv); \
        delete testCase;
        if (retCode != 0) break;
    }

    return retCode;
}

