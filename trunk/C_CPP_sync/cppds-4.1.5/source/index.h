//  index.h
//
//  Defines a data type used for indexing arrays
//
#ifndef INDEX_H
#define INDEX_H

#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef BOUNDS_H
  #include "bounds.h"
#endif

/****************************************************************************/
typedef Bounds<int, unsigned> Range;

class Index : public Range
{
    protected:
        // current index value
        int Value;

    public:
        // constructors
        Index(int imin, int imax);
        Index(const Range & r);
        Index(const Range & r, int i);
        Index(const Index & i);

        // conversions
        operator int() const;    // returns exact value
        operator size_t() const; // returns zero-based value

        // value checks
        Boolean IsMin() const;
        Boolean IsMax() const;

        // explicit assignments
        void SetMin();
        void SetMax();

        // assignment operators
        Index & operator = (int n);
        Index & operator = (const Index & i);

        // math operators
        Index operator + (const Index & i);
        Index operator - (const Index & i);
        Index operator + (int i);
        Index operator - (int i);

        // shorthand operators
        Index & operator += (const Index & i);
        Index & operator -= (const Index & i);
        Index & operator += (int n);
        Index & operator -= (int n);

        // increment and decrement
        Index & operator ++ ();
        Index & operator -- ();

        // comparison operators
        Boolean operator >  (const Index & i) const;
        Boolean operator >= (const Index & i) const;
        Boolean operator == (const Index & i) const;
        Boolean operator != (const Index & i) const;
        Boolean operator <= (const Index & i) const;
        Boolean operator <  (const Index & i) const;
        Boolean operator >  (int i) const;
        Boolean operator >= (int i) const;
        Boolean operator == (int i) const;
        Boolean operator != (int i) const;
        Boolean operator <= (int i) const;
        Boolean operator <  (int i) const;
};

/****************************************************************************/
// constructors
inline Index::Index(int imin, int imax):
Range(imin, imax)
{
  Value = Minimum;
}

/****************************************************************************/
inline Index::Index(const Range & r):
Range(r)
{
  Value = Minimum;
}

/****************************************************************************/
inline Index::Index(const Index & i):
Range(i)
{
  Value = i.Value;
}

/****************************************************************************/
// conversions
inline Index::operator int () const
{
  return Value;
}

/****************************************************************************/
// value checks
inline Boolean Index::IsMin() const
{
  return (Value == Minimum);
}

/****************************************************************************/
inline Boolean Index::IsMax() const
{
  return (Value == Maximum);
}

/****************************************************************************/
// explicit assignments
inline void Index::SetMin()
{
  Value = Minimum;
}

/****************************************************************************/
inline void Index::SetMax()
{
  Value = Maximum;
}

/****************************************************************************/
// math operators
inline Index Index::operator + (const Index & i)
{
  return *this + i.Value;
}

/****************************************************************************/
inline Index Index::operator - (const Index & i)
{
  return *this - i.Value;
}

/****************************************************************************/
// shorthand operators
inline Index & Index::operator += (const Index & i)
{
  return *this += i.Value;
}

/****************************************************************************/
inline Index & Index::operator -= (const Index & i)
{
  return *this -= i.Value;
}

/****************************************************************************/
inline Index & Index::operator += (int i)
{
  return *this = *this + i;
}

/****************************************************************************/
inline Index & Index::operator -= (int i)
{
  return *this = *this - i;
}

/****************************************************************************/
// comparison operators
inline Boolean Index::operator > (const Index & i) const
{
  return (Value > i.Value);
}

/****************************************************************************/
inline Boolean Index::operator >= (const Index & i) const
{
  return (Value >= i.Value);
}

/****************************************************************************/
inline Boolean Index::operator == (const Index & i) const
{
  return (Value == i.Value);
}

/****************************************************************************/
inline Boolean Index::operator != (const Index & i) const
{
  return (Value != i.Value);
}

/****************************************************************************/
inline Boolean Index::operator <= (const Index & i) const
{
  return (Value <= i.Value);
}

/****************************************************************************/
inline Boolean Index::operator < (const Index & i) const
{
  return (Value < i.Value);
}

/****************************************************************************/
inline Boolean Index::operator > (int i) const
{
  return (Value > i);
}

/****************************************************************************/
inline Boolean Index::operator >= (int i) const
{
  return (Value >= i);
}

/****************************************************************************/
inline Boolean Index::operator == (int i) const
{
  return (Value == i);
}

/****************************************************************************/
inline Boolean Index::operator != (int i) const
{
  return (Value != i);
}

/****************************************************************************/
inline Boolean Index::operator <= (int i) const
{
  return (Value <= i);
}

/****************************************************************************/
inline Boolean Index::operator < (int i) const
{
  return (Value < i);
}

/****************************************************************************/
#endif




