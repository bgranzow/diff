#ifndef EXPR_H
#define EXPR_H

#include <cmath>

namespace diff {

/* Curiously Recurring Template Pattern:
 * basically, inherit from CRTPExpr<Yourself>
 * this is used to distinguish the Expr classes
 * from other classes, so that our operator overloads
 * do not take over all the math operators in a program
 */
template <class T>
class CRTPExpr {
  public:
    T const& self() const
    {
      return static_cast<T const&>(*this);
    }
};

class ConstExpr : public CRTPExpr<ConstExpr> {
  private:
    double const v_;
  public:
    ConstExpr(double const& v):
      v_(v)
    {
    }
    double val() const
    {
      return v_;
    }
    double dx(unsigned i) const
    {
      return 0;
    }
};

template <class T>
class ValExpr : public CRTPExpr<ValExpr<T> > {
  private:
    T v_; /* not a reference, store the AD value */
  public:
    enum { degree = T::degree };
    ValExpr()
    {
    }
    ValExpr(T const& v):
      v_(v)
    {
    }
    ValExpr(double v):
      v_(v)
    {
    }
    template <class R>
    ValExpr(CRTPExpr<R> const& r)
    {
      assign(r);
    }
    template <class R>
    ValExpr<T>& operator=(CRTPExpr<R> const& r)
    {
      assign(r);
      return *this;
    }
    void diff(unsigned int idx)
    {
      for (int i=0; i < ValExpr<T>::degree; ++i)
        this->dx(i) = 0.0;
      this->dx(idx) = 1.0;
    }
    double val() const
    {
      return v_.val();
    }
    double dx(unsigned i) const
    {
      return v_.dx(i);
    }
    double& val()
    {
      return v_.val();
    }
    double& dx(unsigned i)
    {
      return v_.dx(i);
    }
  private:
    /* this triggers the cascade of evaluations */
    template <class R>
    void assign(CRTPExpr<R> const& r)
    {
      this->val() = r.self().val();
      for (unsigned i = 0; i < ValExpr<T>::degree; ++i)
        this->dx(i) = r.self().dx(i);
    }
};

template <class L, class R>
class AddExpr : public CRTPExpr<AddExpr<L, R> > {
  private:
    L const& l_;
    R const& r_;
  public:
    AddExpr(L const& l, R const& r):
      l_(l),
      r_(r)
    {
    }
    double val() const
    {
      return l_.val() + r_.val();
    }
    double dx(unsigned i) const
    {
      return l_.dx(i) + r_.dx(i);
    }
};

template <class A, class B>
AddExpr<A, B> operator+(CRTPExpr<A> const& a, CRTPExpr<B> const& b)
{
  return AddExpr<A, B>(a.self(), b.self());
}

template <class L, class R>
class SubExpr : public CRTPExpr<SubExpr<L, R> > {
  private:
    L const& l_;
    R const& r_;
  public:
    SubExpr(L const& l, R const& r):
      l_(l),
      r_(r)
    {
    }
    double val() const
    {
      return l_.val() - r_.val();
    }
    double dx(unsigned i) const
    {
      return l_.dx(i) - r_.dx(i);
    }
};

template <class A, class B>
SubExpr<A, B> operator-(CRTPExpr<A> const& a, CRTPExpr<B> const& b)
{
  return SubExpr<A, B>(a.self(), b.self());
}

template <class L, class R>
class MulExpr : public CRTPExpr<MulExpr<L, R> > {
  private:
    L const& l_;
    R const& r_;
  public:
    MulExpr(L const& l, R const& r):
      l_(l),
      r_(r)
    {
    }
    double val() const
    {
      return l_.val() * r_.val();
    }
    double dx(unsigned i) const
    {
      /* note that this might redudantly evaluate the val()s ... */
      return l_.dx(i) * r_.val() + l_.val() * r_.dx(i);
    }
};

template <class A, class B>
MulExpr<A, B> operator*(CRTPExpr<A> const& a, CRTPExpr<B> const& b)
{
  return MulExpr<A, B>(a.self(), b.self());
}

template <class L, class R>
class DivExpr : public CRTPExpr<DivExpr<L, R> > {
  private:
    L const& l_;
    R const& r_;
  public:
    DivExpr(L const& l, R const& r):
      l_(l),
      r_(r)
    {
    }
    double val() const
    {
      return l_.val() / r_.val();
    }
    double dx(unsigned i) const
    {
      return (l_.dx(i) * r_.val() - l_.val() * r_.dx(i)) / (r_.val() * r_.val());
    }
};

template <class A, class B>
DivExpr<A, B> operator/(CRTPExpr<A> const& a, CRTPExpr<B> const& b)
{
  return DivExpr<A, B>(a.self(), b.self());
}

template <class T>
class ExpExpr : public CRTPExpr<ExpExpr<T> > {
  private:
    T const& v_;
  public:
    ExpExpr(T const& v):
      v_(v)
    {
    }
    double val() const
    {
      return std::exp(v_.val());
    }
    double dx(unsigned i) const
    {
      return v_.dx(i) * std::exp(v_.val());
    }
};

template <class T>
ExpExpr<T> exp(CRTPExpr<T> const& a)
{
  return ExpExpr<T>(a.self());
}

template <class T>
class LogExpr : public CRTPExpr<LogExpr<T> > {
  private:
    T const& v_;
  public:
    LogExpr(T const& v):
      v_(v)
    {
    }
    double val() const
    {
      return std::log(v_.val());
    }
    double dx(unsigned i) const
    {
      return v_.dx(i) / v_.val();
    }
};

template <class L, class R>
class PowExpr : public CRTPExpr<PowExpr<L, R> > {
  private:
    L const& l_;
    R const& r_;
  public:
    PowExpr(L const& l, R const& r):
      l_(l),
      r_(r)
    {
    }
    double val() const
    {
      return std::pow(l_.val(), r_.val());
    }
    double dx(unsigned i) const
    {
      return r_.dx(i) * std::log(l_.val()) * std::pow(l_.val(), r_.val()) +
             r_.val() * l_.dx(i) * std::pow(l_.val(), r_.val() - 1.0);
    }
};

template <class A, class B>
PowExpr<A, B> pow(CRTPExpr<A> const& a, CRTPExpr<B> const& b)
{
  return PowExpr<A, B>(a.self(), b.self());
}

}

#endif
