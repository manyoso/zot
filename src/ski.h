#ifndef ski_h
#define ski_h

#include <QtCore>

class Ski {
public:
    static QString toZot(const QString& string);
    static QString toSkiTicks(const QString& string);
    static QString fromSkiTicks(const QString& string);
};

#endif // ski_h
