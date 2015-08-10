#ifndef DIFF_H
#define DIFF_H

namespace diff {

/** \brief forward automatic differentiation variable */
template <unsigned int N>
class AD
{
  protected:
    /** \brief the variable value */
    double x_;
    /** \brief the derivative array */
    double dx_[N];
  public:
    /** \brief default constructor */
    AD():x_(0.) {zero();}
    /** \brief construct from a double */
    AD(double x):x_(x) {zero();}
    /** \brief copy constructor */
    AD(AD<N> const& other) {copy(other);}
    /** \brief get the size of the derivative array */
    unsigned int size() const {return N;}
    /** \brief set as the ith variable of N */
    void diff(unsigned int i)
    {
      zero();
      dx_[i] = 1.;
    }
    /** \brief get the value of the variable (mutable) */
    double& val() {return x_;}
    /** \brief get the value of the variable (immutable) */
    const double& val() const {return x_;}
    /** \brief get the ith derivative value (mutable) */
    double& dx(unsigned int i) {return dx_[i];}
    /** \brief get the ith derivative value (immutable) */
    const double& dx(unsigned int i) const {return dx_[i];}
    /** \brief assigment to a double */
    AD<N>& operator=(double other)
    {
      x_ = other;
      zero();
      return *this;
    }
    /** \brief assignment to another AD variable */
    AD<N>& operator=(AD<N> const& other)
    {
      copy(other);
      return *this;
    }
    /** \brief addition assignment with a double */
    AD<N>& operator+=(double other)
    {
      x_ += other;
      return *this;
    }
    /** \brief addition assignment with another AD variable */
    AD<N>& operator+=(AD<N> const& other)
    {
      x_ += other.x_;
      for (unsigned int i=0; i < N; ++i)
        dx_[i] += other.dx_[i];
      return *this;
    }
    /** \brief subtraction assignment with a double */
    AD<N>& operator-=(double other)
    {
      x_ -= other;
      return *this;
    }
    /** \brief subtraction assignment with another AD variable */
    AD<N>& operator-=(AD<N> const& other)
    {
      x_ -= other.x_;
      for (unsigned int i=0; i < N; ++i)
        dx_[i] -= other.dx_[i];
      return *this;
    }
    /** \brief multiplication assignment with a double */
    AD<N>& operator*=(double other)
    {
      x_ *= other;
      for (unsigned int i=0; i < N; ++i)
        dx_[i] *= other;
      return *this;
    }
    /** \brief multiplication assignment with another AD variable */
    AD<N>& operator*=(AD<N> const& other)
    {
      x_ *= other.x_;
      for (unsigned int i=0; i < N; ++i)
        dx_[i] = dx_[i]*other.x_ + x_*other.dx_[i];
    }
    /** \brief division assignment with a double */
    AD<N>& operator/=(double other)
    {
      x_ /= other;
      for (unsigned int i=0; i < N; ++i)
        dx_[i] /= other;
      return *this;
    }
    /** \brief division assignment with another AD variable */
    AD<N>& operator/=(AD<N> const& other)
    {
      x_ /= other.x_;
      for (unsigned int i=0; i < N; ++i)
        dx_[i] = (dx_[i]*other.x_ - x_*other.dx_[i]) / (other.x_*other.x_);
    }
  private:
    void zero()
    {
      for (int i=0; i < N; ++i)
        dx_[i] = 0.;
    }
    void copy(AD<N> const& other)
    {
      x_ = other.x_;
      for (int i=0; i < N; ++i)
        dx_[i] = other.dx_[i];
    }
};

/***********************
  * BINARY OPERATIONS *
************************/

/** \brief binary addition between a double and an AD variable */
template <unsigned int N>
AD<N> operator+(double L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = R.dx(i);
  tmp.val() = L + R.val();
  return tmp;
}

/** \brief binary addition between an AD variable and a double */
template <unsigned int N>
AD<N> operator+(AD<N> const& L, double R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L.dx(i);
  tmp.val() = L.val() + R;
  return tmp;
}

/** \brief binary addition between two AD variables */
template <unsigned int N>
AD<N> operator+(AD<N> const& L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L.dx(i) + R.dx(i);
  tmp.val() = L.val() + R.val();
  return tmp;
}

/** \brief binary subtraction between a double and an AD variable */
template <unsigned int N>
AD<N> operator-(double L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = -R.dx(i);
  tmp.val() = L - R.val();
  return tmp;
}

/** \brief binary subtraction between an AD variable and a double */
template <unsigned int N>
AD<N> operator-(AD<N> const& L, double R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L.dx(i);
  tmp.val() = L.val() - R;
  return tmp;
}

/** \brief binary subtraction between two AD variables */
template <unsigned int N>
AD<N> operator-(AD<N> const& L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L.dx(i) - R.dx(i);
  tmp.val() = L.val() - R.val();
  return tmp;
}

/** \brief binary multiplication between a double and an AD variable */
template <unsigned int N>
AD<N> operator*(double L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L * R.dx(i);
  tmp.val() = L * R.val();
  return tmp;
}

/** \brief binary multiplication between an AD variable and a double */
template <unsigned int N>
AD<N> operator*(AD<N> const& L, double R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L.dx(i) * R;
  tmp.val() = L.val() * R;
  return tmp;
}

/** \brief binary multiplication between two AD variables */
template <unsigned int N>
AD<N> operator*(AD<N> const& L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L.dx(i) * R.val() + L.val() * R.dx(i);
  tmp.val() = L.val() * R.val();
  return tmp;
}

/** \brief binary division between a double and an AD variable */
template <unsigned int N>
AD<N> operator/(double L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = ( -L*R.dx(i) ) / (R.val()*R.val());
  tmp.val() = L / R.val();
  return tmp;
}

/** \brief binary division between an AD variable and a double */
template <unsigned int N>
AD<N> operator/(AD<N> const& L, double R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = L.dx(i) / R;
  tmp.val() = L.val() / R;
  return tmp;
}

/** \brief binary division between two AD variables */
template <unsigned int N>
AD<N> operator/(AD<N> const& L, AD<N> const& R)
{
  AD<N> tmp;
  for (unsigned int i=0; i < N; ++i)
    tmp.dx(i) = (L.dx(i) * R.val() - L.val() * R.dx(i) ) / (R.val() * R.val());
  tmp.val() = L.val() / R.val();
  return tmp;
}

}

#endif
