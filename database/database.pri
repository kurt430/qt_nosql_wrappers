INCLUDEPATH += $$PWD
HEADERS += $$PWD/abstractdriver.h $$PWD/driverfactory.h
SOURCES += $$PWD/driverfactory.cpp \
    $$PWD/abstractdriver.cpp

contains(KR_KEYVAL_DRIVERS, all):KR_KEYVAL_DRIVERS = lmdb vedis unqlite leveldb
for(var, $$list($$KR_KEYVAL_DRIVERS)) {
   DEFINES += KR_DB_$$upper($$var)
}
contains(KR_KEYVAL_DRIVERS, lmdb):include($$PWD/driver/lmdb/lmdb.pri)
contains(KR_KEYVAL_DRIVERS, vedis):include($$PWD/driver/vedis/vedis.pri)
contains(KR_KEYVAL_DRIVERS, unqlite):include($$PWD/driver/unqlite/unqlite.pri)
contains(KR_KEYVAL_DRIVERS, leveldb):include($$PWD/driver/leveldb/leveldb.pri)
