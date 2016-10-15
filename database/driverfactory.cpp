#include "driverfactory.h"
#include "abstractdriver.h"

#ifdef KR_DB_LMDB
#include "driver/lmdb/lmdbdriver.h"
#endif

#ifdef KR_DB_VEDIS
#include "driver/vedis/vedisdriver.h"
#endif

#ifdef KR_DB_UNQLITE
#include "driver/unqlite/unqlitedriver.h"
#endif

#ifdef KR_DB_LEVELDB
#include "driver/leveldb/leveldbdriver.h"
#endif

using namespace krdb;

QList<QString> DriverFactory::avaiable()
{
    QList<QString> result;
#ifdef KR_DB_LEVELDB
    result << LevelDbDriver::engineId();
#endif
#ifdef KR_DB_UNQLITE
    result << UnqliteDriver::engineId();
#endif
#ifdef KR_DB_VEDIS
    result << VedisDriver::engineId();
#endif
#ifdef KR_DB_LMDB
    result << LmDbDriver::engineId();
#endif
    return result;
}

AbstractDriver *DriverFactory::create(const QString &text, QObject *parent)
{
#ifdef KR_DB_LMDB
    if (text == LmDbDriver::engineId()) return new LmDbDriver(parent);
#endif
#ifdef KR_DB_VEDIS
    if (text == VedisDriver::engineId()) return new VedisDriver(parent);
#endif
#ifdef KR_DB_UNQLITE
    if (text == UnqliteDriver::engineId()) return new UnqliteDriver(parent);
#endif
#ifdef KR_DB_LEVELDB
    if (text == LevelDbDriver::engineId()) return new LevelDbDriver(parent);
#endif
    return nullptr;
}
