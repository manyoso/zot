#include "zot.h"

#include <QtCore>

#define DEBUG 0

class Term;
typedef QSharedPointer<Term> TermPtr;

class Term {
public:
    enum Type { i_, k_, k1_, s_, s1_, s2_, empty_, zero_, basis_, one_ , bigleft_, left_, bigright_, right_, print_, out0_, out1_};

    Term() : m_type(Type(-1)) { }
    Term(Type t) : m_type(t) { }
    virtual ~Term() { }
    TermPtr apply(TermPtr term) const;
    TermPtr eval(TermPtr term) const;
    Type type() const { return m_type; }
    QString toString(const QString& arg = QString()) const;
    QString toStringApply(const Term* arg) const;

private:
    Type m_type;
};

struct I : Term {
    I() : Term(Term::i_) { }
    TermPtr apply(TermPtr x) const;
};

struct K : Term {
    K() : Term(Term::k_) { }
    struct K1 : Term {
        K1() : Term(Term::k1_) { }
        TermPtr apply(TermPtr /*y*/) const;
        TermPtr x;
    };
    TermPtr apply(TermPtr x) const;
};

struct S : Term {
    S() : Term(Term::s_) { }
    struct S1 : Term {
        S1() : Term(Term::s1_) { }
        struct S2 : Term {
            S2() : Term(Term::s2_) { }
            TermPtr apply(TermPtr z) const;
            TermPtr x;
            TermPtr y;
        };
        TermPtr x;
        TermPtr apply(TermPtr y) const;
    };
    TermPtr apply(TermPtr x) const;
};

struct Empty : Term {
    Empty() : Term(Term::empty_) { }
    TermPtr apply(TermPtr x) const;
};

struct Zero : Term {
    Zero() : Term(Term::zero_) { }
    struct Basis : Term {
        Basis() : Term(Term::basis_) { }
        TermPtr apply(TermPtr f) const;
    };
    TermPtr apply(TermPtr c) const;
};

struct One : Term {
    One() : Term(Term::one_) { }
    struct BigLeft : Term {
        BigLeft() : Term(Term::bigleft_) { }
            struct Left : Term {
                Left() : Term(Term::left_) { }
                    struct BigRight : Term {
                        BigRight() : Term(Term::bigright_) { }
                            struct Right : Term {
                                Right() : Term(Term::right_) { }
                                TermPtr apply(TermPtr r) const;
                                TermPtr c;
                                TermPtr l;
                            };
                        TermPtr apply(TermPtr R) const;
                        TermPtr c;
                        TermPtr l;
                    };
                TermPtr apply(TermPtr l) const;
                TermPtr c;
            };
        TermPtr apply(TermPtr L) const;
        TermPtr c;
    };
    TermPtr apply(TermPtr c) const;
};

class Print : public Term {
public:
    Print() : Term(Term::print_) { }
    TermPtr apply(TermPtr ch) const;
    QString output() const { return m_output; }

private:
    mutable QString m_output;
};

struct Out0 : Term {
    Out0() : Term(Term::out0_) { }
    TermPtr apply(TermPtr x) const;
};

struct Out1 : Term {
    Out1() : Term(Term::out1_) { }
    TermPtr apply(TermPtr x) const;
};

static TermPtr i()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new I);
    return s_instance;
}

static TermPtr k()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new K);
    return s_instance;
}

static TermPtr s()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new S);
    return s_instance;
}

static TermPtr empty()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new Empty);
    return s_instance;
}

static TermPtr zero()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new Zero);
    return s_instance;
}

static TermPtr one()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new One);
    return s_instance;
}

static TermPtr print()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new Print);
    return s_instance;
}

static TermPtr out0()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new Out0);
    return s_instance;
}

static TermPtr out1()
{
    static TermPtr s_instance;
    if (!s_instance)
        s_instance = TermPtr(new Out1);
    return s_instance;
}

QString Term::toString(const QString& argument) const
{
    switch(m_type) {
    case i_:
        return QStringLiteral("I");
    case k_:
        return QStringLiteral("K");
    case k1_:
      {
          return QString("(K%1)").arg(static_cast<const K::K1*>(this)->x->toString());
      }
    case s_:
        return QStringLiteral("S");
    case s1_:
      {
          return QString("(S%1)").arg(static_cast<const S::S1*>(this)->x->toString());
      }
    case s2_:
      {
          const S::S1::S2* s2 = static_cast<const S::S1::S2*>(this);
          return QString("(S%1%2%3)").arg(s2->x->toString()).arg(s2->y->toString()).arg(argument);
      }
    case empty_:
        return QStringLiteral("(^c.cI)");
    case zero_:
        return QStringLiteral("(^c.c(^f.fSK))");
    case basis_:
        return QStringLiteral("(^f.fSK)");
    case one_:
        return QStringLiteral("(^cL.L(^lR.R(^r.c(lr))))");
    case bigleft_:
      {
          return QString("(^L.L(^lR.R(^r.%1(lr))))").arg(static_cast<const One::BigLeft*>(this)->c->toString());
      }
    case left_:
      {
          return QString("(^lR.R(^r.%1lr))").arg(static_cast<const One::BigLeft::Left*>(this)->c->toString());
      }
    case bigright_:
      {
          const One::BigLeft::Left::BigRight* bigRight = static_cast<const One::BigLeft::Left::BigRight*>(this);
          return QString("(^R.R(^r.%1(%2r)))").arg(bigRight->c->toString()).arg(bigRight->l->toString());
      }
    case right_:
      {
          const One::BigLeft::Left::BigRight::Right* right = static_cast<const One::BigLeft::Left::BigRight::Right*>(this);
          return QString("(^r.%1(%2r))").arg(right->c->toString()).arg(right->l->toString());
      }
    case print_:
        return "(^f.fIIIK)";
    case out0_:
        return "0";
    case out1_:
        return "1";
    default:
        Q_ASSERT(false);
        return QString();
    }
}

QString CYAN() { return "\033[96m"; }
QString PURPLE() { return "\033[95m"; }
QString BLUE() { return "\033[94m"; }
QString YELLOW() { return "\033[93m"; }
QString GREEN() { return "\033[92m"; }
QString RED() { return "\033[91m"; }
QString RESET() { return "\033[0m"; }

QString Term::toStringApply(const Term* arg) const
{
    switch(m_type) {
    case i_:
    case k1_:
        return GREEN() + toString() + RED() + arg->toString() + RESET();
    case k_:
    case s_:
    case s1_:
        return QString();
    case s2_:
        return GREEN() + toString(RED() + arg->toString() + GREEN()) + RESET();
    case empty_:
    case zero_:
    case basis_:
    case one_:
    case bigleft_:
    case left_:
    case bigright_:
    case right_:
    case print_:
    case out0_:
    case out1_:
      {
          QString argument = RED() + arg->toString() + RESET();
          QString out = GREEN() + toString(argument) + RESET();
          return out + argument;
      }
    default:
        Q_ASSERT(false);
        return QString();
    }
}

class Verbose {
public:
    static Verbose* instance()
    {
        static Verbose* s_instance = 0;
        if (!s_instance)
            s_instance = new Verbose;
        return s_instance;
    }

    bool subEval() const
    { return m_subEval; }

    void setSubEval(bool subEval)
    {
        m_subEval = subEval;
        if (subEval)
            m_subEvalLevel++;
        else
            m_subEvalLevel--;
    }

    void print()
    {
        m_stream->flush();
        printf("%s", qPrintable(m_out));
        m_out.clear();
    }

    void setVerbose(bool verbose)
    { m_verbose = verbose; }

    QString prefix() const;
    int addPrefix(const QString& prefix)
    {
        m_prefix.append(prefix);
        return m_prefix.count() - 1;
    }

    void removePrefix(int i)
    {
        m_prefix.removeAt(i);
    }

    QString postfix() const;
    int addPostfix(const QString& postfix)
    {
        m_postfix.append(postfix);
        return m_postfix.count() - 1;
    }

    void removePostfix(int i)
    {
        m_postfix.removeAt(i);
    }

    void generateProgramStart();
    void generateProgramString(const QString& string, bool replace = false);
    void generateEvalString(const Term* term1, const Term* term2);
    void generateReturnString(const Term* r);
    void generateReplacementString(const Term* term1, const Term* term2);

private:
    Verbose()
    {
        m_verbose = false;
        m_subEval = false;
        m_subEvalLevel = 0;
        m_stream = new QTextStream(&m_out);
    }

    bool m_verbose;
    bool m_subEval;
    int m_subEvalLevel;
    QString m_program;
    QStringList m_prefix;
    QStringList m_postfix;
    QString m_out;
    QTextStream* m_stream;
};

void Verbose::generateProgramStart()
{
    if (!m_verbose)
        return;
    m_program += "e";
    *m_stream << PURPLE() << m_program << ":\n\t" << RESET() << empty()->toString() << "\n";
}

void Verbose::generateProgramString(const QString& string, bool replace)
{
    if (!m_verbose)
        return;
    if (replace)
        m_program = string;
    else
        m_program += string;
    *m_stream << PURPLE() << m_program << ":\n" << RESET();
    print();
}

QString Verbose::prefix() const
{
    return (m_subEval ? m_prefix.join("") : "");
}

QString Verbose::postfix() const
{
    return (m_subEval ? m_postfix.join("") : "");
}

void Verbose::generateEvalString(const Term* term1, const Term* term2)
{
    if (!m_verbose && m_subEvalLevel < 2)
        return;

    QString apply = term1->toStringApply(term2);
    if (apply.isEmpty())
        return;

    *m_stream << "\t"
        << prefix()
        << apply
        << postfix()
        << "\n";

    print();
}

void Verbose::generateReturnString(const Term* r)
{
    if (!m_verbose && m_subEvalLevel < 2)
        return;

    QString ret = r->toString();
    if (ret.isEmpty())
        return;

    *m_stream << "\t"
        << prefix()
        << ret
        << postfix()
        << "\n";
    print();
}

void Verbose::generateReplacementString(const Term* term1, const Term* term2)
{
    if (!m_verbose && m_subEvalLevel < 2)
      return;

    QString from = term1->toString();
    QString to = term2->toString();
    if (from.isEmpty() || to.isEmpty())
        return;

    *m_stream << "\t"
        << prefix()
        << YELLOW()
        << from
        << "->"
        << to
        << RESET()
        << postfix()
        << "\n";
    print();
}


class SubEval {
public:
    SubEval()
    {
        m_prefixNumber = -1;
        m_postfixNumber = -1;
        m_topLevel = !Verbose::instance()->subEval();
        if (m_topLevel)
            Verbose::instance()->setSubEval(true);
    }

    ~SubEval()
    {
        if (m_topLevel)
            Verbose::instance()->setSubEval(false);
        if (m_prefixNumber != -1)
            Verbose::instance()->removePrefix(m_prefixNumber);
        if (m_postfixNumber != -1)
            Verbose::instance()->removePostfix(m_postfixNumber);
    }

    void addPrefix(const QString& prefix)
    {
        m_prefixNumber = Verbose::instance()->addPrefix(prefix);
    }

    void addPostfix(const QString& postfix)
    {
        m_postfixNumber = Verbose::instance()->addPostfix(postfix);
    }

private:
    bool m_topLevel;
    int m_prefixNumber;
    int m_postfixNumber;
};

TermPtr Term::eval(TermPtr term) const
{
    Verbose::instance()->generateEvalString(this, term.data());

    TermPtr r;
    switch(m_type) {
    case i_:
        return static_cast<const I*>(this)->apply(term);
    case k_:
        return static_cast<const K*>(this)->apply(term);
    case k1_:
        return static_cast<const K::K1*>(this)->apply(term);
    case s_:
        return static_cast<const S*>(this)->apply(term);
    case s1_:
        return static_cast<const S::S1*>(this)->apply(term);
    case s2_:
        return static_cast<const S::S1::S2*>(this)->apply(term);
    case empty_:
        return static_cast<const Empty*>(this)->apply(term);
    case zero_:
        return static_cast<const Zero*>(this)->apply(term);
    case basis_:
        return static_cast<const Zero::Basis*>(this)->apply(term);
    case one_:
        r = static_cast<const One*>(this)->apply(term);
        break;
    case bigleft_:
        return static_cast<const One::BigLeft*>(this)->apply(term);
    case left_:
        r = static_cast<const One::BigLeft::Left*>(this)->apply(term);
        break;
    case bigright_:
        return static_cast<const One::BigLeft::Left::BigRight*>(this)->apply(term);
    case right_:
        return static_cast<const One::BigLeft::Left::BigRight::Right*>(this)->apply(term);
    case print_:
        return static_cast<const Print*>(this)->apply(term);
    case out0_:
        r = static_cast<const Out0*>(this)->apply(term);
        break;
    case out1_:
        r = static_cast<const Out1*>(this)->apply(term);
        break;
    default:
        {
            Q_ASSERT(false);
            r = i();
            break;
        }
    }

    Verbose::instance()->generateReturnString(r.data());
    return r;
}

TermPtr I::apply(TermPtr x) const
{
    return x;
}

TermPtr K::apply(TermPtr x) const
{
    TermPtr k1(new K1);
    k1.staticCast<K1>()->x = x;
    return k1;
}

TermPtr K::K1::apply(TermPtr /*y*/) const
{
    return x;
}

TermPtr S::apply(TermPtr x) const
{
    TermPtr s1(new S1);
    s1.staticCast<S1>()->x = x;
    return s1;
}

TermPtr S::S1::apply(TermPtr y) const
{
    if (x->type() == Term::k_) {
        TermPtr s2(new S2);
        s2.staticCast<S2>()->x = x;
        s2.staticCast<S2>()->y = y;
        Verbose::instance()->generateReplacementString(s2.data(), i().data());
        return i();
    }

    TermPtr s2(new S2);
    s2.staticCast<S2>()->x = x;
    s2.staticCast<S2>()->y = y;
    return s2;
}

TermPtr S::S1::S2::apply(TermPtr z) const
{
    TermPtr first;
    TermPtr second;
    {
        SubEval eval;
        eval.addPostfix("(" + y->toString() + z->toString() + ")");
        first = x->eval(z);
    }
    {
        SubEval eval;
        eval.addPrefix(first->toString());
        second = y->eval(z);
    }
    return (first->eval(second));
}

TermPtr Empty::apply(TermPtr x) const
{
    return x->eval(i());
}

TermPtr Zero::apply(TermPtr c) const
{
    TermPtr basis(new Basis);
    return (c->eval(basis));
}

TermPtr Zero::Basis::apply(TermPtr f) const
{
    TermPtr first;
    {
        SubEval eval;
        eval.addPostfix(k()->toString());
        first = f->eval(s());
    }
    return (first->eval(k()));
}

TermPtr One::apply(TermPtr c) const
{
    TermPtr L(new BigLeft);
    L.staticCast<BigLeft>()->c = c;
    return L;
}

TermPtr One::BigLeft::apply(TermPtr L) const
{
    TermPtr left(new Left);
    left.staticCast<Left>()->c = c;
    return (L->eval(left));
}

TermPtr One::BigLeft::Left::apply(TermPtr l) const
{
    TermPtr R(new BigRight);
    R.staticCast<BigRight>()->c = c;
    R.staticCast<BigRight>()->l = l;
    return R;
}

TermPtr One::BigLeft::Left::BigRight::apply(TermPtr R) const
{
    TermPtr right(new Right);
    right.staticCast<Right>()->c = c;
    right.staticCast<Right>()->l = l;
    return (R->eval(right));
}

TermPtr One::BigLeft::Left::BigRight::Right::apply(TermPtr r) const
{
    TermPtr first;
    {
        SubEval eval;
        eval.addPrefix("(" + c->toString() + ")");
        first = l->eval(r);
    }
    return (c->eval(first));
}

TermPtr Out0::apply(TermPtr x) const
{
    return x;
}

TermPtr Out1::apply(TermPtr x) const
{
    return x;
}

TermPtr Print::apply(TermPtr f) const
{
    Verbose::instance()->generateProgramString("print", true /*replace*/);
    TermPtr first;
    TermPtr second;
    TermPtr third;
    TermPtr interrogate;
    {
        SubEval eval;
        eval.addPostfix("IIK");
        first = f->eval(i());
    }
    {
        SubEval eval;
        eval.addPostfix("IK");
        second = first->eval(i());
    }
    {
        SubEval eval;
        eval.addPostfix("K");
        third = second->eval(i());
    }
    {
        SubEval eval;
        interrogate = third->eval(k());
    }
    Verbose::instance()->generateReturnString(interrogate.data());
    Verbose::instance()->generateProgramString("0", true /*replace*/);
    TermPtr interrogate0 = interrogate->eval(out0());
    Verbose::instance()->generateProgramString("1", true /*replace*/);
    TermPtr interrogate1 = interrogate0->eval(out1());
    TermPtr out = interrogate1;
    if (out->type() == Term::out0_)
        m_output += QStringLiteral("0");
    else if (out->type() == Term::out1_)
        m_output += QStringLiteral("1");
    return print();
}

TermPtr outputCombinator(TermPtr evaluate)
{
    Verbose::instance()->generateProgramString("output", true /*replace*/);
    {
        SubEval eval;
        evaluate = (evaluate)
          ->eval(k()
            ->eval(k()
              ->eval(k()
                ->eval(k()
                  ->eval(k()
                    ->eval(k()
                      ->eval(i())))))));
    }
    Verbose::instance()->generateProgramString("end", true /*replace*/);
    evaluate = evaluate->eval(print());
    return evaluate;
}

QString cppInterpreter(const QString& string)
{
    TermPtr evaluate = empty();
    Verbose::instance()->generateProgramStart();
    foreach (QChar c, string) {
        if (c == '0') {
            Verbose::instance()->generateProgramString("0");
            evaluate = evaluate->eval(zero());
        } else if (c == '1') {
            Verbose::instance()->generateProgramString("1");
            evaluate = evaluate->eval(one());
        } else
            Q_ASSERT(false);
    }

    evaluate = outputCombinator(evaluate);
    return static_cast<Print*>(print().data())->output();
}

Zot::Zot(bool verbose)
  : m_verbose(verbose)
{
    Verbose::instance()->setVerbose(verbose);
}

Zot::~Zot()
{ }

QString Zot::run(const QString& string)
{
    return cppInterpreter(string);
}

QString Zot::toSki(const QString& string)
{
    QChar backtick('1');
    QString i("100");
    QString k("1010100");
    QString s("101010100");

    QString out;
    int x = 0;
    while (x < string.length() - 1) {
        QStringRef i_ = string.midRef(x, 3);
        QStringRef k_ = string.midRef(x, 7);
        QStringRef s_ = string.midRef(x, 9);
        if (i_ == i) {
            out += "i";
            x += 3;
        } else if (k_ == k) {
            out += "k";
            x += 7;
        } else if (s_ == s) {
            out += "s";
            x += 9;
        } else if (string.at(x) == backtick) {
            out += "`";
            x += 1;
        } else {
            Q_ASSERT(false);
            return QStringLiteral("Error: from zot to ski!");
        }
    }
    return out;
}
