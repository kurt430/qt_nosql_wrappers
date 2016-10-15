#ifndef TEST_BENCHMARK_H
#define TEST_BENCHMARK_H

#include <QObject>
#include <QMap>

namespace krdb {
class AbstractDriver;
};

class TestNoSQL : public QObject
{
    Q_OBJECT

public:
    TestNoSQL(const QString& engine = QString());

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void put_1Mb_value();
    void put_10k_small_values();
    void get_10k_small_values();
    void check_kv_map();

private:
    QMap<QByteArray, QByteArray> map_;
    krdb::AbstractDriver *db_;
    QString engine_;
    QString fileName_;
};

#endif // TEST_BENCHMARK_H
