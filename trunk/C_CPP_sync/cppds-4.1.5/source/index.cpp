//  Index.cpp
//
//  Defines a data type used for indexing arrays
//
#ifndef INDEX_CPP
#define INDEX_CPP
#ifndef INDEX_H
  #include "index.h"
#endif

/****************************************************************************/
// constructors
Index::Index(const Range & r, int i):
Range(r)
{
  if (GetCheck() && ((i < Minimum) || (i > Maximum)))
    THROW (LoneFallibleBase::InvalidBoundsErr());

  Value = i;
}

/****************************************************************************/
// conversions
Index::operator size_t () const
{
  unsigned long result;

  if (Minimum < 0)
  {
    result = (size_t)(-Minimum);

    if (Value < 0)
      result -= (size_t)(-Value);
    else
      result += (size_t)Value;
  }
  else
    result = (unsigned long)Value - (unsigned long)Minimum;

  return (size_t)result;
}

/****************************************************************************/
//---------------------
// assignment operators
//---------------------
Index & Index::operator = (int i)
{
  if (GetCheck() && ((i < Minimum) || (i > Maximum)))
    THROW (LoneFallibleBase::InvalidBoundsErr());

  Value = i;
  return *this;
}

/****************************************************************************/
Index & Index::operator = (const Index & i)
{
  this->Range::operator = (i);
  Value = i.Value;
  return *this;
}

/****************************************************************************/
//---------------
// math operators
//---------------
Index Index::operator + (int i)
{
  if (GetCheck())
  {
    if (i < 0)
    {
      if ((Value - Minimum) < -i)
        THROW (LoneFallibleBase::BoundsUnderFlowErr());
    }
    else
    {
      if ((Maximum - Value) < i)
        THROW (LoneFallibleBase::BoundsOverFlowErr());
    }
  }

  Index result(*this);
  result.Value += i;
  return result;
}

/****************************************************************************/
Index Index::operator - (int i)
{
  if (GetCheck())
  {
    if (i > 0)
    {
      if ((Value - Minimum) < i)
        THROW (LoneFallibleBase::BoundsUnderFlowErr());
    }
    else
    {
      if ((Maximum - Value) < -i)
        THROW (LoneFallibleBase::BoundsOverFlowErr());
    }
  }

  Index result(*this);
  result.Value -= i;
  return result;
}

/****************************************************************************/
//------------------------
// increment and decrement
//------------------------
Index & Index::operator ++ ()
{
  if (GetCheck() && (Value == Maximum))
    THROW (LoneFallibleBase::BoundsOverFlowErr());

  ++Value;
  return *this;
}

/****************************************************************************/
Index & Index::operator -- ()
{
  if (GetCheck() && (Value == Minimum))
    THROW (LoneFallibleBase::BoundsUnderFlowErr());

  --Value;
  return *this;
}

/****************************************************************************/
#endif




