QMAKE_CFLAGS += -fpermissive
QMAKE_CXXFLAGS += -fpermissive

QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_LFLAGS_RELEASE -= -O2

HEADERS += $$PWD/unqlite/unqlite.h $$PWD/unqlitedriver.h
SOURCES += $$PWD/unqlite/unqlite.c $$PWD/unqlitedriver.cpp
