#include "lmdbdriver.h"

#include <QDebug>

extern "C"
{
#include "liblmdb/lmdb.h"
}

using namespace krdb;

namespace krdb {
class LmdbValue
{
protected:
    MDB_val v_;
public:
    LmdbValue() noexcept = default;
    LmdbValue(const Slice& slice) noexcept
        : v_{slice.size_, const_cast<void*>(slice.data_)} {}
    QByteArray toByteArray() const noexcept { return QByteArray(static_cast<const char *>(v_.mv_data), static_cast<int>(v_.mv_size));}
    operator MDB_val*() noexcept { return &v_; }
    operator const MDB_val*() const noexcept { return &v_; }
};
}

static_assert(std::is_pod<LmdbValue>::value, "Value must be a POD type");
static_assert(sizeof(LmdbValue) == sizeof(MDB_val), "sizeof(Value) != sizeof(MDB_val)");

LmDbDriver::LmDbDriver(QObject *parent) : AbstractDriver(parent)
{
}

LmDbDriver::~LmDbDriver()
{
    close();
}

const QString LmDbDriver::engineId()
{
    return QStringLiteral("lmdb");
}

QString LmDbDriver::engineVersionName() const
{
    return QString("%1_%2.%3.%4").arg(engineId()).arg(MDB_VERSION_MAJOR).arg(MDB_VERSION_MINOR).arg(MDB_VERSION_PATCH);
}

bool LmDbDriver::open(const QString &path)
{
    if (env_) close();
    rc_ = mdb_env_create(&env_);
    mdb_env_set_maxreaders(env_, 1);
    mdb_env_set_mapsize(env_, (1UL * 1024UL * 1024UL * 1024UL));
    if (rc_ == 0) {
        rc_ = mdb_env_open(env_, path.toLocal8Bit().constData(), MDB_NOSUBDIR, 0664);
        if (rc_ != 0) {
            qDebug() << "ERROR:" << errorText();
            close();
        }
    }
    return (rc_ == 0);
}

bool LmDbDriver::close()
{
    if (transaction_) closeTransaction(true);
    if (env_) {
        mdb_env_close(env_);
        env_ = nullptr;
        return true;
    }
    return false;
}

bool LmDbDriver::put(const Slice &key, const Slice &val)
{
    prepareActiveTransaction(false); // write transaction
    rc_ = mdb_put(transaction_, dbi_, LmdbValue(key), LmdbValue(val), 0);
    return (rc_ == 0);
}

QByteArray LmDbDriver::get(const Slice &key)
{
    prepareActiveTransaction(true); // read transaction
    LmdbValue mdata;
    rc_ = mdb_get(transaction_, dbi_, LmdbValue(key), mdata);
    if (rc_ == 0) {
        return mdata.toByteArray();
    }
    return QByteArray();
}

bool LmDbDriver::remove(const Slice &key)
{
    prepareActiveTransaction(false); // write transaction
    rc_ = mdb_del(transaction_, dbi_, LmdbValue(key), NULL);
    return (rc_ == 0);
}

bool LmDbDriver::begin()
{
    Q_ASSERT(env_);
    return prepareActiveTransaction(false);
}

bool LmDbDriver::commit()
{
    if (transaction_) {
        closeTransaction(trxnReadOnly_);
        return true;
    }
    return false;
}

bool LmDbDriver::rollback()
{
    if (transaction_) {
        closeTransaction(true);
        return true;
    }
    return false;
}

QString LmDbDriver::errorText() const
{
    const char *txt = mdb_strerror(rc_);
    return QString::fromLocal8Bit(txt);
}

void LmDbDriver::closeTransaction(bool abort)
{
    if (abort) {
        mdb_txn_abort(transaction_);
    } else {
        mdb_txn_commit(transaction_);
    }
    transaction_ = nullptr;
}

bool LmDbDriver::prepareActiveTransaction(bool readOnly)
{
    if (transaction_) {
        // already have active transaction
        if (trxnReadOnly_ != readOnly) {
            // transaction mode changed - close current transaction
            closeTransaction(trxnReadOnly_);
        }
    }

    if (transaction_ == nullptr) {
        trxnReadOnly_ = readOnly;
        uint txn_flags = (trxnReadOnly_) ? MDB_RDONLY : 0u;
        rc_ = mdb_txn_begin(env_, NULL, txn_flags, &transaction_);
        if (dbi_ == 0) {
            uint dbi_flags = 0; //MDB_INTEGERKEY
            rc_ = mdb_dbi_open(transaction_, NULL, dbi_flags, &dbi_);
        }
    }
    return (transaction_ != nullptr);
}
