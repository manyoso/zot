#include <QtCore>

class LogFile {
public:
    static LogFile* instance()
    {
        static LogFile* s_instance = 0;
        if (!s_instance)
            s_instance = new LogFile;
        return s_instance;
    }

    void log(const QString& message)
    {
        Q_ASSERT(m_stream);
        *m_stream << message << "\n";
        m_stream->flush();
    }

    void close()
    {
        m_stream->flush();
        m_file->close();
        delete m_stream;
        delete m_file;
        m_stream = 0;
        m_file = 0;
    }

private:
    LogFile()
    {
        QDir bin(QCoreApplication::applicationDirPath());
        m_file = new QFile(bin.path() + QDir::separator() + "zotsearch.log");
        if (m_file->open(QFile::WriteOnly | QFile::Truncate))
            m_stream = new QTextStream(m_file);
        Q_ASSERT(m_stream);
    }

    QTextStream* m_stream;
    QFile* m_file;
};

void output(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s\n", localMsg.constData());
        LogFile::instance()->log(msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

void printBinary(const QString& string, int iterations)
{
    if (!iterations) {
        QString program = string;
        QString log = QString();
        while (true) {
          QDir bin(QCoreApplication::applicationDirPath());
          QProcess zot;
          zot.setProgram(bin.path() + QDir::separator() + "zot");

          QStringList args = QStringList()
              << "--program"
              << program;

          zot.setArguments(args);
          zot.start();
          zot.waitForFinished();
          QString out = zot.readAll();
          out = out.simplified();
          out.replace(" ", "");
          if (!out.isEmpty()) {
              if (out == program) {
                  log += log.isEmpty() ? "quine" : ", quine";
                  log += log.isEmpty() ? out : ", " + out;
                  break;
              }
              log += log.isEmpty() ? out : ", " + out;
              program = out;
          } else {
            break;
          }
        }

        if (!log.isEmpty())
            qDebug() << qPrintable(string) << ":" << qPrintable(log);
    } else {
        printBinary(string + "0", iterations - 1);
        printBinary(string + "1", iterations - 1);
    }
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("zotsearch");
    QCoreApplication::setApplicationVersion("0.1");
    qInstallMessageHandler(output);

    int i = 0;
    while (true)
        printBinary(QString(), i++);

    return EXIT_SUCCESS;
}
