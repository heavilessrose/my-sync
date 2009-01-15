//  Bounds.h
//
//  A class that defines a subrange of values.
//
#ifndef BOUNDS_H
#define BOUNDS_H

#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef LONEFALL_H
  #include "lonefall.h"
#endif

/****************************************************************************/
template <class T, class UT>
class Bounds
{
  // add ranges
  friend Bounds<T, UT> operator + (const Bounds<T, UT> & r1, const Bounds<T, UT> & r2);

  protected:
    // minimum and maximum values
    T Minimum;
    T Maximum;
    UT Magnitude;

    // range-checking switch
    static Boolean SetCheck(int Mode_, Boolean Flag_);
    
  public:
    // constructors
    Bounds(T rmin, T rmax);
    Bounds(T rmax);
    Bounds(const Bounds<T, UT> & r);

    // assignment operator
    Bounds<T, UT> & operator = (const Bounds<T, UT> & r);

    // comparison operator
    Boolean operator == (const Bounds<T, UT> & r) const;
    Boolean operator != (const Bounds<T, UT> & r) const;

    // information retrieval functions
    T GetMin() const;
    T GetMax() const;
    UT GetMagnitude() const;

    // report error if 'n' is invalid
    Boolean Check(T n) const;

    // check for 'n' or range within bounds
    Boolean Includes(T n) const;
    Boolean Includes(const Bounds<T, UT> & r) const;
    Boolean Excludes(T n) const;
    Boolean Excludes(const Bounds<T, UT> & r) const;

    // range checking functions
    static void CheckOn();
    static void CheckOff();
    static Boolean GetCheck();
};

/****************************************************************************/
// constructors
template <class T, class UT>
inline Bounds<T, UT>::Bounds(const Bounds<T, UT>& r)
{
  Minimum   = r.Minimum;
  Maximum   = r.Maximum;
  Magnitude = r.Magnitude;
}

/****************************************************************************/
// constructors
template <class T, class UT>
Bounds<T, UT>::Bounds(T rmin, T rmax)
{
  if (GetCheck() && (rmax < rmin))
    THROW (LoneFallibleBase::InvalidBoundsErr());

  Minimum = rmin;
  Maximum = rmax;

  if (Minimum < 0)
  {
    if (Maximum < 0)
      Magnitude = (UT)(Minimum - Maximum);
    else
      Magnitude = (UT)(-Minimum) + (UT)Maximum;
  }
  else
    Magnitude = Maximum - Minimum;

  ++Magnitude;
}

/****************************************************************************/
template <class T, class UT>
Bounds<T, UT>::Bounds(T rmax)
{
  if (GetCheck() && (rmax <= 1))
    THROW (LoneFallibleBase::InvalidBoundsErr());
  
  Minimum   = 1;
  Maximum   = rmax;
  Magnitude = (Maximum - Minimum) + 1;
}

/****************************************************************************/
// add ranges
template <class T, class UT>
Bounds<T, UT> operator + (const Bounds<T, UT> & r1, const Bounds<T, UT> & r2)
{
  if (Bounds<T, UT>::GetCheck() && ((INT_MAX - r1.Maximum) < r2.Magnitude))
    THROW (LoneFallibleBase::AdditionOutOfRangeErr());

  return Bounds<T, UT>(r1.Minimum,r1.Maximum + r2.Magnitude);
}

/****************************************************************************/
// assignment operator
template <class T, class UT>
inline Bounds<T, UT> & Bounds<T, UT>::operator = (const Bounds<T, UT> & r)
{
  Minimum   = r.Minimum;
  Maximum   = r.Maximum;
  Magnitude = r.Magnitude;

  return *this;
}

/****************************************************************************/
// comparison operator
template <class T, class UT>
inline Boolean Bounds<T, UT>::operator == (const Bounds<T, UT> & r) const
{
  return ((Minimum == r.Minimum) && (r.Maximum == r.Maximum));
}

/****************************************************************************/
template <class T, class UT>
inline Boolean Bounds<T, UT>::operator != (const Bounds<T, UT> & r) const
{
  return ((Minimum != r.Minimum) || (r.Maximum != r.Maximum));
}

/****************************************************************************/
// information retrieval functions
template <class T, class UT>
inline T Bounds<T, UT>::GetMin() const
{
  return Minimum;
}

/****************************************************************************/
template <class T, class UT>
inline T Bounds<T, UT>::GetMax() const
{
  return Maximum;
}

/****************************************************************************/
template <class T, class UT>
inline UT Bounds<T, UT>::GetMagnitude() const
{
  return Magnitude;
}

/****************************************************************************/
// check for 'n' or range within bounds
template <class T, class UT>
inline Boolean Bounds<T, UT>::Includes(T n) const
{
  return ((n >= Minimum) && (n <= Maximum));
}

/****************************************************************************/
template <class T, class UT>
inline Boolean Bounds<T, UT>::Includes(const Bounds<T, UT> & r) const
{
  return ((Minimum <= r.Minimum) && (Maximum >= r.Maximum));
}

/****************************************************************************/
template <class T, class UT>
inline Boolean Bounds<T, UT>::Excludes(T n) const
{
  return ((n < Minimum) || (n > Maximum));
}

/****************************************************************************/
template <class T, class UT>
inline Boolean Bounds<T, UT>::Excludes(const Bounds<T, UT> & r) const
{
  return ((Minimum > r.Minimum) || (Maximum < r.Maximum));
}

/****************************************************************************/
// range checking functions
template <class T, class UT>
inline void Bounds<T, UT>::CheckOn()
{
  SetCheck(1, TRUE);
}

/****************************************************************************/
template <class T, class UT>
inline void Bounds<T, UT>::CheckOff()
{
  SetCheck(1, FALSE);
}

/****************************************************************************/
template <class T, class UT>
inline Boolean Bounds<T, UT>::GetCheck()
{
  return SetCheck(0, FALSE);
}

/****************************************************************************/
// report error if 'n' is invalid
template <class T, class UT>
inline Boolean Bounds<T, UT>::Check(T n) const
{
  return ((n < Minimum) || (n > Maximum));
}

/****************************************************************************/
// static member
template <class T, class UT>
Boolean Bounds<T, UT>::SetCheck(int Mode_, Boolean Flag_)
{
  static Boolean BoundsCheck = 0;

  if (Mode_ == 1)
    BoundsCheck = Flag_;

  return BoundsCheck;
}
/****************************************************************************/
#endif




