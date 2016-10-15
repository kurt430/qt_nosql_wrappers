QT       += testlib
QT       -= gui
TARGET    = test_bechmark
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE  = app
SOURCES  += test_benchmark.cpp
HEADERS  += test_benchmark.h

KR_KEYVAL_DRIVERS = all
include(../database.pri)
