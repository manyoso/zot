#include <QtCore>

#include "ski.h"
#include "zot.h"
#include "zotjs.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("zot");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption fileOption("file", "Specify a zot binary file to run.", "file");
    parser.addOption(fileOption);

    QCommandLineOption programOption("program", "Specify a zot binary program to run.", "program");
    parser.addOption(programOption);

    QCommandLineOption inputOption("input", "Specify a zot binary input to run.", "input");
    parser.addOption(inputOption);

    QCommandLineOption interpreterOption("interpreter", "Specify a zot interpreter to use (c++|js). Default is c++.", "interpreter", "c++");
    parser.addOption(interpreterOption);

    QCommandLineOption skiOption("ski", "Specify to translate from SKI calculus.");
    parser.addOption(skiOption);

    QCommandLineOption translateOption("translate", "Translate program (zotToSki, zotToSkiTicks, skiToZot, skiToSkiTicks, skiTicksToZot, skiTicksToSki).", "translate");
    parser.addOption(translateOption);

    QCommandLineOption verboseOption("verbose", "Verbose execution evaluation for c++ interpreter.");
    parser.addOption(verboseOption);

    parser.process(*QCoreApplication::instance());

    bool isFile = parser.isSet(fileOption);
    bool isProgram = parser.isSet(programOption);
    bool isInput = parser.isSet(inputOption);
    bool isSki = parser.isSet(skiOption);
    bool isVerbose = parser.isSet(verboseOption);

    if ((isFile && isProgram) || (!isFile && !isProgram))
        parser.showHelp(-1);

    QString program;

    if (isFile) {
        QString fileName = parser.value(fileOption);
        QFile file(fileName);
        if (!file.exists()) {
            qDebug() << "Error: file does not exist: " << fileName;
            exit(-1);
        }

        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Error: could not open file for reading: " << fileName;
            exit(-1);
        }

        QFileInfo info(file);
        isSki = info.suffix() == "ski";
        program = file.readAll();
    } else if (isProgram) {
        program = parser.value(programOption);
    }

    // Remove all whitespace
    program = program.simplified();
    program.replace(" ", "");

    if (isSki && !parser.isSet(translateOption))
        program = Ski::toZot(program);

    if (isInput)
        program.append(parser.value(inputOption));

    // Remove all whitespace from input too
    program = program.simplified();
    program.replace(" ", "");

    if (parser.isSet(translateOption)) {
        QString translate = parser.value(translateOption);
        QString out;
        if (translate == "zotToSki") {
            out = Ski::fromSkiTicks(Zot::toSki(program));
        } else if (translate =="zotToSkiTicks") {
            out = Zot::toSki(program);
        } else if (translate =="skiToZot") {
            out = Ski::toZot(program);
        } else if (translate =="skiToSkiTicks") {
            out = Ski::toSkiTicks(program);
        } else if (translate =="skiTicksToZot") {
            out = Ski::toZot(program);
        } else if (translate =="skiTicksToSki") {
            out = Ski::fromSkiTicks(program);
        } else {
            parser.showHelp(-1);
        }
        printf("%s\n", qPrintable(out));
        return EXIT_SUCCESS;
    }

    QString interpreter = parser.value(interpreterOption);
    if (interpreter == "c++") {
        Zot zot(isVerbose);
        QString out = zot.run(program);
        if (!out.isEmpty())
            printf("%s\n", qPrintable(out));
    } else if (interpreter == "js") {
        ZotJS zot;
        QString out = zot.run(program);
        if (!out.isEmpty())
            printf("%s\n", qPrintable(out));
    } else {
        parser.showHelp(-1);
    }

    return EXIT_SUCCESS;
}
