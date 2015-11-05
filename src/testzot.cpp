#include <QtCore>

#include "testzot.h"

void TestZot::testZot()
{
    QDir bin(QCoreApplication::applicationDirPath());
    QVERIFY(bin.exists());

    QDir examples(bin.path() + "/../../examples");
    QVERIFY(examples.exists());

    QProcess zot;
    zot.setProgram(bin.path() + QDir::separator() + "zot");

    // reverser
    QStringList args = QStringList()
      << "--file"
      << examples.path() + "/reverser.zot"
      << "--input"
      << "111000";

    zot.setArguments(args);
    zot.start();
    QVERIFY(zot.waitForFinished());
    QCOMPARE(zot.exitStatus(), QProcess::NormalExit);
    QCOMPARE(zot.exitCode(), 0);
    QCOMPARE(zot.state(), QProcess::NotRunning);
    QCOMPARE(zot.readAll(), QByteArray("000111\n"));
}
