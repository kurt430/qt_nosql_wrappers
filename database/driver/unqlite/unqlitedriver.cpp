#include "unqlitedriver.h"

extern "C"
{
#include "unqlite/unqlite.h"
}

using namespace krdb;

int output_callback_to_existing_bytearray(const void *pOutput, unsigned int nLen, void *pUserData)
{
    if (QByteArray *ba = static_cast<QByteArray *>(pUserData)) {
        ba->append(static_cast<const char *>(pOutput), static_cast<int>(nLen));
    }
    return UNQLITE_OK;
}

UnqliteDriver::UnqliteDriver(QObject *parent) : AbstractDriver(parent), db_(nullptr), rc_(UNQLITE_OK)
{
}

UnqliteDriver::~UnqliteDriver()
{
    if (db_) close();
}

const QString UnqliteDriver::engineId()
{
    return QStringLiteral("unqlite");
}

QString UnqliteDriver::engineVersionName() const
{
    return QString("%1_%2").arg(engineId()).arg(UNQLITE_VERSION);
}

bool UnqliteDriver::open(const QString &path)
{
    if (db_) close();
    rc_ = unqlite_open(&db_, path.toLocal8Bit().constData(), UNQLITE_OPEN_CREATE);
    return (rc_ == UNQLITE_OK);
}

bool UnqliteDriver::close()
{
    if (db_) {
        int result = unqlite_close(db_);
        db_ = nullptr;
        return (result == UNQLITE_OK);
    }
    return false;
}

bool UnqliteDriver::put(const Slice &key, const Slice &value)
{
    rc_ = unqlite_kv_store(db_, key.data_, key.size(), value.data_, value.size());
    return (rc_ == UNQLITE_OK);
}

QByteArray UnqliteDriver::get(const Slice &key)
{
    QByteArray result;
    rc_ = unqlite_kv_fetch_callback(db_, key.data_, key.size(), &output_callback_to_existing_bytearray, &result);
    return result;
}

bool UnqliteDriver::remove(const Slice &key)
{
    rc_ = unqlite_kv_delete(db_, key.data_, key.size());
    return (rc_ == UNQLITE_OK);
}

bool UnqliteDriver::begin()
{
    Q_ASSERT(db_ != nullptr);
    rc_ = unqlite_begin(db_);
    return (rc_ == UNQLITE_OK);
}

bool UnqliteDriver::commit()
{
    Q_ASSERT(db_ != nullptr);
    rc_ = unqlite_commit(db_);
    return (rc_ == UNQLITE_OK);
}

bool UnqliteDriver::rollback()
{
    Q_ASSERT(db_ != nullptr);
    rc_ = unqlite_rollback(db_);
    return (rc_ == UNQLITE_OK);
}

QString UnqliteDriver::errorText() const
{
    const char *buf = nullptr;
    int size = 0;
    // Extract database error log
    unqlite_config(db_, UNQLITE_CONFIG_ERR_LOG, &buf, &size);
    if(buf && (size > 0)){
        return QString::fromLocal8Bit(buf);
    }
    return QString();
}
