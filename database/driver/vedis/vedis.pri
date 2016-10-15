QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_LFLAGS_RELEASE -= -O2

HEADERS += $$PWD/vedis/vedis.h $$PWD/vedisdriver.h
SOURCES += $$PWD/vedis/vedis.c $$PWD/vedisdriver.cpp

