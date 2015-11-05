include($$PWD/../zot.pri)
include($$PWD/../src/zot.pri)

TEMPLATE = app
TARGET = zottests
DESTDIR = $$OUTPUT_DIR/bin
QT += testlib

DEPENDPATH += .
INCLUDEPATH += .

HEADERS += testzot.h

SOURCES += main_tests.cpp \
           testzot.cpp
