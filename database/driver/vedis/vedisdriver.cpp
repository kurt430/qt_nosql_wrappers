#include "vedisdriver.h"

extern "C"
{
#include "vedis/vedis.h"
}

using namespace krdb;

VedisDriver::VedisDriver(QObject *parent) : AbstractDriver(parent), db_(nullptr), rc_(VEDIS_OK)
{
}

VedisDriver::~VedisDriver()
{
    if (db_) close();
}

const QString VedisDriver::engineId()
{
    return QStringLiteral("vedis");
}

QString VedisDriver::engineVersionName() const
{
    return QString("%1_%2").arg(engineId()).arg(VEDIS_VERSION);
}

bool VedisDriver::open(const QString &path)
{
    if (db_) close();
    rc_ = vedis_open(&db_, path.toLocal8Bit().constData());
    return (rc_ == VEDIS_OK);
}

bool VedisDriver::close()
{
    if (db_) {
        int result = vedis_close(db_);
        db_ = nullptr;
        return (result == VEDIS_OK);
    }
    return false;
}

bool VedisDriver::put(const Slice &key, const Slice &value)
{
    rc_ = vedis_kv_store(db_, key.data_, key.size(), value.data_, value.size());
    return (rc_ == VEDIS_OK);
}

QByteArray VedisDriver::get(const Slice &key)
{
    QByteArray result;
    rc_ = vedis_kv_fetch_callback(db_, key.data_, key.size(), &output_callback_to_existing_bytearray, &result);
    return result;
}

bool VedisDriver::remove(const Slice &key)
{
    rc_ = vedis_kv_delete(db_, key.data_, key.size());
    return (rc_ == VEDIS_OK);
}

bool VedisDriver::begin()
{
    rc_ = vedis_begin(db_);
    return (rc_ == VEDIS_OK);
}

bool VedisDriver::commit()
{
    rc_ = vedis_commit(db_);
    return (rc_ == VEDIS_OK);
}

bool VedisDriver::rollback()
{
    rc_ = vedis_rollback(db_);
    return (rc_ == VEDIS_OK);
}

QString VedisDriver::errorText() const
{
    const char *buf = nullptr;
    int size = 0;
    // Extract database error log
    vedis_config(db_, VEDIS_CONFIG_ERR_LOG, &buf, &size);
    if(buf && (size > 0)){
        return QString::fromLocal8Bit(buf);
    }
    return QString();
}

int VedisDriver::output_callback_to_existing_bytearray(const void *pOutput, unsigned int nLen, void *pUserData)
{
    if (QByteArray *ba = static_cast<QByteArray *>(pUserData)) {
        ba->append(static_cast<const char *>(pOutput), static_cast<int>(nLen));
    }
    return VEDIS_OK;
}
