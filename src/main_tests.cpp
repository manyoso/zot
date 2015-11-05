#include <QtCore>
#include <QtTest/QtTest>

#include "testzot.h"

#define _NAME_ "zottests"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(_NAME_);

    int rc = 0;
    TestZot test;
    rc = QTest::qExec(&test, argc, argv) == 0 ? rc : -1;

    return rc;
}
