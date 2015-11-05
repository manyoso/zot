include($$PWD/../zot.pri)

TEMPLATE = app
TARGET = zot
DESTDIR = $$OUTPUT_DIR/bin

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main_zot.cpp

include($$PWD/zot.pri)
