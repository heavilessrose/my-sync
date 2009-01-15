#include "lonefall.h"

const char* LoneFallibleBase::IOFailureErr::message() const
{
    return "ERROR: IO Failure Error";
}

const char* LoneFallibleBase::UsedInInvalidStateErr::message() const
{
    return "ERROR: Fallible object used in invalid state.";
}

const char* LoneFallibleBase::IncompatibleTypeErr::message() const
{
    return "ERROR: Incompatible type passed as argument to function.";
}

const char* LoneFallibleBase::BitsInvalidArgumentErr::message() const
{
    return "ERROR: Invalid Bits<N> char";
}

const char* LoneFallibleBase::BitsOverFlowErr::message() const
{
    return "ERROR: Bits<N> conversion overflow";
}

const char* LoneFallibleBase::BitsOutOfRangeErr::message() const
{
    return "ERROR: Invalid Bits<N> position";
}

const char* LoneFallibleBase::BitStringInvalidArgumentErr::message() const
{
  return "ERROR: Invalid bitstring char";
}

const char* LoneFallibleBase::BitStringLengthErr::message() const
{
  return "ERROR: Bitstring too long";
}

const char* LoneFallibleBase::BitStringOutOfRangeErr::message() const
{
  return "ERROR: Invalid bitstring position";
}

const char* LoneFallibleBase::ArrayLengthErr::message() const
{
  return "ERROR: Array too long";
}

const char* LoneFallibleBase::ArrayIndexRangeErr::message() const
{
  return "ERROR: Array index is out of range";
}

const char* LoneFallibleBase::ArrayInvalidArgumentErr::message() const
{
  return "ERROR: Invalid Array Pointer";
}

const char* LoneFallibleBase::DerefNullPointerErr::message() const
{
  return "ERROR: Attempt To Dereference Null Pointer";
}

const char* LoneFallibleBase::NotSameBasePointerErr::message() const
{
  return "ERROR: Attempt To Compare Arrays With Different Base Pointers";
}

const char* LoneFallibleBase::OutOfMemoryErr::message() const
{
  return "ERROR: Out Of Memory";
}

const char* LoneFallibleBase::IncompatibleHtmlTagsErr::message() const
{
  return "ERROR: Incompatible HTML Tags";
}

const char* LoneFallibleBase::NotImplementedErr::message() const
{
  return "ERROR: Method or function not implemented";
}

const char* LoneFallibleBase::InvalidBoundsErr::message() const
{
  return "ERROR: Invalid range on bounds";
}

const char* LoneFallibleBase::BoundsUnderFlowErr::message() const
{
  return "ERROR: Value under flow bounds";
}

const char* LoneFallibleBase::BoundsOverFlowErr::message() const
{
  return "ERROR: Value over flow bounds";
}

const char* LoneFallibleBase::AdditionOutOfRangeErr::message() const
{
  return "ERROR: Result of addition operation is out of range";
}

void LoneFallibleBase::throwErr() const
{
    THROW (LoneFallibleBase::UsedInInvalidStateErr());
}

void CatchEx(const LoneErr& ExCls_)
{
  XSHOWFATAL(ExCls_.message(), ExCls_.Stream());
}


#if FALLIBLE_DEBUG
void main()
{
  Fallible<int> Fint(5, NULL);
  int i1 = Fint;

  cout <<i1 <<endl;

  Fallible<int> Fint2;
  int i2 = Fint2;

  cout <<i2 <<endl;
}
#endif





