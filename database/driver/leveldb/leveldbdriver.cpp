#include "leveldbdriver.h"
#include "leveldb/db.h"

namespace leveldb {
class LvDbValue : public Slice
{
public:
    LvDbValue(const krdb::Slice& slice) noexcept
        : Slice(static_cast<const char *>(slice.data_), slice.size_) {}
};
}

using namespace krdb;

LevelDbDriver::LevelDbDriver(QObject *parent) : AbstractDriver(parent), db_(nullptr)
{
}

LevelDbDriver::~LevelDbDriver()
{
    delete  db_;
}

const QString LevelDbDriver::engineId()
{
    return QStringLiteral("leveldb");
}

QString LevelDbDriver::engineVersionName() const
{
    return QString("%1_%2.%3").arg(engineId()).arg(leveldb::kMajorVersion).arg(leveldb::kMinorVersion);
}

bool LevelDbDriver::open(const QString &path)
{
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, path.toStdString(), &db_);
    return status.ok();
}

bool LevelDbDriver::close()
{
    delete db_;
    db_ = nullptr;
    return true;
}

bool LevelDbDriver::put(const Slice &key, const Slice &value)
{
    const auto opt = leveldb::WriteOptions();
    leveldb::Status status = db_->Put(opt, leveldb::LvDbValue(key), leveldb::LvDbValue(value));
    return status.ok();
}

QByteArray LevelDbDriver::get(const Slice &key)
{
    std::string value;
    leveldb::Status s = db_->Get(leveldb::ReadOptions(), leveldb::LvDbValue(key), &value);
    if (s.ok()) {
        return QByteArray::fromStdString(value);
    }
    return QByteArray();
}

bool LevelDbDriver::remove(const Slice &key)
{
    const auto opt = leveldb::WriteOptions();
    leveldb::Status status = db_->Delete(opt, leveldb::LvDbValue(key));
    return status.ok();
}

bool LevelDbDriver::begin()
{
    return false;
}

bool LevelDbDriver::commit()
{
    return false;
}

bool LevelDbDriver::rollback()
{
    return false;
}

QString LevelDbDriver::errorText() const
{
    Q_ASSERT(!"Not implemented!");
    return QString();
}

