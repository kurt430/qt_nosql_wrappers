QMAKE_CXXFLAGS_RELEASE += -W -Wall -Wno-unused-parameter  -Wuninitialized 

QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_LFLAGS_RELEASE -= -O2

HEADERS += \
    $$PWD/liblmdb/lmdb.h \
    $$PWD/liblmdb/midl.h \
    $$PWD/liblmdb/ntdll_dynamic_load.h \
    $$PWD/lmdbdriver.h

SOURCES += \
    $$PWD/liblmdb/mdb.c \
    $$PWD/liblmdb/midl.c \
    $$PWD/lmdbdriver.cpp
