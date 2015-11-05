#include "ski.h"

QString Ski::toZot(const QString& string)
{
    QString input = string;
    if (input.contains("("))
        input = toSkiTicks(string);

    QString out;
    foreach (QChar ch, input) {
        if (ch == '`')
            out += QStringLiteral("1");
        else if (ch == 'i')
            out += QStringLiteral("100");
        else if (ch == 'k')
            out += QStringLiteral("1010100");
        else if (ch == 's')
            out += QStringLiteral("101010100");
        else {
            QString error = QString("Error: from ski to zot! ch=`%1`").arg(ch);
            Q_ASSERT_X(false, "translate", qPrintable(error));
            return error;
        }
    }
    return out;
}

QString Ski::toSkiTicks(const QString& string)
{
    QString out;
    foreach (QChar ch, string) {
        if (ch == '(')
            out += QStringLiteral("`");
        else if (ch == ')')
            ; // no-op
        else if (ch == 'i')
            out += QStringLiteral("i");
        else if (ch == 'k')
            out += QStringLiteral("k");
        else if (ch == 's')
            out += QStringLiteral("s");
        else {
            QString error = QString("Error: from ski to ski ticks! ch=`%1`").arg(ch);
            Q_ASSERT_X(false, "translate", qPrintable(error));
            return error;
        }
    }
    return out;
}

QString Ski::fromSkiTicks(const QString& /*string*/)
{
    QString out;
//     QString term;
//     QStack<QChar> terms;
//     for (int i = 0; i < string.length(); i++) {
//         QChar ch = string.at(i);
//         QChar next = (i + 1 < string.length() ? string.at(i + 1) : QChar());
//         if (ch == '`') {
//             term += QStringLiteral("(");
//             terms.push(term);
//             term = QString();
//             continue;
//         } else if (ch == 'i')
//             term += QStringLiteral("i");
//         else if (ch == 'k')
//             term += QStringLiteral("k");
//         else if (ch == 's')
//             term += QStringLiteral("s");
//         else {
//             QString error = QString("Error: from ski ticks to ski! ch=`%1`").arg(ch);
//             Q_ASSERT_X(false, "translate", qPrintable(error));
//             return error;
//         }
//     }
    return out;
}
