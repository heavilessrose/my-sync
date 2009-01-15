/*
Modified
Example fallible class from the book Scientific and Engineering Programming
in C++: An Introduction with Advanced Techniques and Examples,
*/
#include "fallible.h"
#include "chrstring.h"

ChrString FallibleBase::DivideByZeroErr::message() const
{
    return "ERROR: Divide By Zero";
}

ChrString FallibleBase::TypeOverflowErr::message() const
{
    return "ERROR: Overflowed Data Type Limits";
}

ChrString FallibleBase::TypeUnderflowErr::message() const
{
    return "ERROR: Underflowed Data Type Limits";
}

ChrString FallibleBase::TypeConversionErr::message() const
{
    return "ERROR: Type Conversion Error";
}

ChrString FallibleBase::IOFailureErr::message() const
{
    return "ERROR: IO Failure Error";
}

ChrString FallibleBase::UsedInInvalidStateErr::message() const
{
    return "ERROR: Fallible object used in invalid state.";
}

ChrString FallibleBase::IncompatibleTypeErr::message() const
{
    return "ERROR: Incompatible type passed as argument to function.";
}

ChrString FallibleBase::BitsInvalidArgumentErr::message() const
{
    return "ERROR: Invalid Bits<N> char";
}

ChrString FallibleBase::BitsOverFlowErr::message() const
{
    return "ERROR: Bits<N> conversion overflow";
}

ChrString FallibleBase::BitsOutOfRangeErr::message() const
{
    return "ERROR: Invalid Bits<N> position";
}

ChrString FallibleBase::BitStringInvalidArgumentErr::message() const
{
  return "ERROR: Invalid bitstring char";
}

ChrString FallibleBase::BitStringLengthErr::message() const
{
  return "ERROR: Bitstring too long";
}

ChrString FallibleBase::BitStringOutOfRangeErr::message() const
{
  return "ERROR: Invalid bitstring position";
}

ChrString FallibleBase::ArrayLengthErr::message() const
{
  return "ERROR: Array too long";
}

ChrString FallibleBase::ArrayIndexRangeErr::message() const
{
  return "ERROR: Array index is out of range";
}

ChrString FallibleBase::ArrayInvalidArgumentErr::message() const
{
  return "ERROR: Invalid Array Pointer";
}

ChrString FallibleBase::DynArrayInvalidArgumentErr::message() const
{
  return "ERROR: Invalid Argument to Dynamic Array";
}

ChrString FallibleBase::DynArrayLengthErr::message() const
{
  return "ERROR: Dynamic Array Too Long";
}

ChrString FallibleBase::DynArrayOutOfRangeErr::message() const
{
  return "ERROR: Dynamic Array Index is Out Of Range";
}

ChrString FallibleBase::DerefNullPointerErr::message() const
{
  return "ERROR: Attempt To Dereference Null Pointer";
}

ChrString FallibleBase::DerefNullLetterObjectErr::message() const
{
  return "ERROR: Attempt To Dereference Null Letter Object";
}

ChrString FallibleBase::OutOfMemoryErr::message() const
{
  return "ERROR: Out Of Memory";
}

ChrString FallibleBase::FullHashTableErr::message() const
{
  return "ERROR: Attempt To Insert Into Full Hash Table";
}

ChrString FallibleBase::IncompatableArraysErr::message() const
{
  return "ERROR: Incompatible Arrays";
}

ChrString FallibleBase::FuzzyRangeErr::message() const
{
  return "ERROR: Fuzzy logic value out of valid range";
}

ChrString FallibleBase::MatrixAllocationErr::message() const
{
  return "ERROR: Can't allocate memory for matrix";
}

ChrString FallibleBase::IncompatibleMatrixErr::message() const
{
  return "ERROR: Operation involves incompatible matrices";
}

ChrString FallibleBase::InvalidIndexErr::message() const
{
  return "ERROR: Out-of-range index for matrix";
}

ChrString FallibleBase::SingularMatrixErr::message() const
{
  return "ERROR: Can't invert singular matrix";
}

ChrString FallibleBase::MatrixTooBigErr::message() const
{
  return "ERROR: Requested matrix too big";
}

ChrString FallibleBase::ZeroDimensionErr::message() const
{
  return "ERROR: Matrix row and column must be > 0";
}

ChrString FallibleBase::ZeroDivisionErr::message() const
{
  return "ERROR: Matrix division by zero";
}

ChrString FallibleBase::PolynomialAllocationErr::message() const
{
  return "ERROR: Can't allocate memory for polynomial";
}

ChrString FallibleBase::PolynomialRangeErr::message() const
{
  return "ERROR: Term out of range in polynomial";
}

ChrString FallibleBase::PolynomialTooSmallErr::message() const
{
  return "ERROR: Polynomial must have > 2 terms";
}

ChrString FallibleBase::IncompatiblePolynomialErr::message() const
{
  return "ERROR: Operation w/incompatible polynomials";
}

ChrString FallibleBase::PolynomialOverflowErr::message() const
{
  return "ERROR: Overflow in polynomial FFT";
}

ChrString FallibleBase::FileOpenErr::message() const
{
  return "ERROR: Cannot Open File";
}

ChrString FallibleBase::DateRangeErr::message() const
{
  return "ERROR: Date value out of range";
}

ChrString FallibleBase::InvalidMonthErr::message() const
{
  return "ERROR: Invalid Month Value";
}

ChrString FallibleBase::InvalidYearErr::message() const
{
  return "ERROR: Invalid Year Value";
}

ChrString FallibleBase::InvalidDayErr::message() const
{
  return "ERROR: Invalid Day Value";
}

ChrString FallibleBase::InvalidDateErr::message() const
{
  return "ERROR: Invalid Date Value";
}

ChrString FallibleBase::InvalidTimeErr::message() const
{
  return "ERROR: Invalid Time Value";
}

ChrString FallibleBase::RootNotBracketedErr::message() const
{
  return "ERROR: Root must be bracketed in root finding function";
}

ChrString FallibleBase::RootFindMaxIterationsExceededErr::message() const
{
  return "ERROR: Maximum number of iterations exceeded in rtsafe";
}

ChrString FallibleBase::NoComplexRootsErr::message() const
{
  return "ERROR: Function cannot calculate complex roots";
}

void FallibleBase::throwErr() const
{
    THROW (FallibleBase::UsedInInvalidStateErr());
}

void CatchEx(const SciEngErr& ExCls_)
{
  XSHOWFATAL(ExCls_.message().c_str(), ExCls_.Stream());
}

#if FALLIBLE_DEBUG
int main()
{
  Fallible<int> Fint(5, NULL);
  int i1 = Fint;

  cout <<i1 <<endl;

  i1 = 10;
  Fallible<int> Fint2;
  int i2 = Fint2.elseDefaultTo(i1);

  cout <<i2 <<endl;

  i2 *= 2;
  const Fallible<int> Fint3;
  int i3 = Fint3.elseDefaultTo(i2);

  cout <<i3 <<endl;

  Fallible<int> Fint4;
  int i4 = Fint4;

  return 0;
}
#endif




