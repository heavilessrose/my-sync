#ifndef MIXEDNUMWRAPPER_CPP
#define MIXEDNUMWRAPPER_CPP
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif

/****************************************************************************/
template <class DERIVED>
MixedNumType<DERIVED>::MixedNumType(int Options_):
OrdinalType(Options_),
_ExtErrorStatus(0)
{}

/****************************************************************************/
template <class DERIVED>
MixedNumType<DERIVED>::MixedNumType(const TypeWrapper& Obj_):
OrdinalType(Obj_),
_ExtErrorStatus(Obj_.GetExtErrorStatusFlag())
{}

/****************************************************************************/
template <class DERIVED>
void MixedNumType<DERIVED>::SetExtErrorStatusFlag(unsigned Value_)
{
  _ExtErrorStatus = Value_;
}

/****************************************************************************/
template <class DERIVED>
unsigned MixedNumType<DERIVED>::GetExtErrorStatusFlag() const
{
  return _ExtErrorStatus;
}

/****************************************************************************/
template <class DERIVED, class NESTEDTYPE>
void MixedNumberType<DERIVED, NESTEDTYPE>::SetExtErrorStatusFlag(unsigned Value_)
{
  MixedNumType<DERIVED>::SetExtErrorStatusFlag(Value_);
}

/****************************************************************************/
template <class DERIVED, class NESTEDTYPE>
unsigned MixedNumberType<DERIVED, NESTEDTYPE>::GetExtErrorStatusFlag() const
{
  return MixedNumType<DERIVED>::GetExtErrorStatusFlag();
}

/****************************************************************************/
#endif




