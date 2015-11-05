#ifndef zotjs_h
#define zotjs_h

#include <QtCore>

class ZotJS {
public:
    ZotJS();
    ~ZotJS();

    QString run(const QString& string);
};

#endif // zotjs_h
