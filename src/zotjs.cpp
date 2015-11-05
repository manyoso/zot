#include "zotjs.h"

#include <QtCore>
#include <QScriptEngine>

// from semarch.linguistics.fas.nyu.edu/barker/Iota/zot.html
const char* zot =
"function I(x) { return x } \
\
function K(x) { \
  function K1(y) { \
    return(x) \
  } \
  return K1 \
} \
\
function S(x) { \
  function S1(y) { \
    function S2(z) { \
      return ((x (z))(y (z))) \
    } \
    return S2 \
  } \
  return S1 \
} \
\
function zero(c) { \
  function basis(f) { \
    return ((f(S))(K)) \
  } \
  return (c(basis)) \
} \
\
function one(c) { \
  function bigleft (L) { \
    function left (l) { \
      function bigright (R) { \
        function right (r) { \
          return (c(l(r))) \
        } \
        return (R(right)) \
      } \
      return bigright \
    } \
    return (L(left)) \
  } \
  return bigleft \
} \
\
function trivial(x) { \
  return x(I) \
} \
\
function zot(string) { \
  function process(position, value) { \
    if (position >= string.length) { return value } \
    if (string.charAt(position) == \"0\") { \
      return process (position + 1, value(zero)) \
    } \
    return process (position + 1, value(one)) \
  } \
  return process(0, trivial) \
} \
\
function interrogate (f) { \
  return ((((f(I))(I))(I))(K)) \
} \
\
function pr(ch) { \
  output.push(((interrogate(ch))(\"0\"))(\"1\")); \
  return pr \
} \
\
function run() { \
  output = []; \
  ((zot(%1))(K(K(K(K(K(K(I))))))))(pr); \
  return output.join(\"\") \
} \
run() \
";

ZotJS::ZotJS()
{ }

ZotJS::~ZotJS()
{ }

QString ZotJS::run(const QString& string)
{
    if (string.isEmpty())
        return string;

    QString interpreter = QString(zot).arg("\"" + string + "\"");
    QScriptEngine engine;
    QScriptValue output = engine.evaluate(interpreter);
    return output.toString();
}
