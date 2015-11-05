#ifndef zot_h
#define zot_h

#include <QtCore>

class Zot {
public:
    Zot(bool verbose);
    ~Zot();

    QString run(const QString& string);
    static QString toSki(const QString& string);

private:
    bool m_verbose;
};

#endif // zot_h
