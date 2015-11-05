include($$PWD/../zot.pri)

TEMPLATE = app
TARGET = zotsearch
DESTDIR = $$OUTPUT_DIR/bin

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main_zotsearch.cpp
