#ifndef TYPEWRAPPER_CPP
#define TYPEWRAPPER_CPP
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif

/****************************************************************************/
OrdinalType::OrdinalType(int Options_):
TypeWrapperImp(Options_)
{}

/****************************************************************************/
OrdinalType::OrdinalType(const TypeWrapper& Obj_):
TypeWrapperImp(Obj_)
{}

/****************************************************************************/
Boolean OrdinalType::IsSummableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean OrdinalType::IsMultipliableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean OrdinalType::IsOrdinalType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean OrdinalType::IsInvertableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean OrdinalType::IsLogicalType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean OrdinalType::IsBitwiseableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean OrdinalType::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean OrdinalType::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean OrdinalType::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
/****************************************************************************/
RealType::RealType(int Options_):
TypeWrapperImp(Options_)
{}

/****************************************************************************/
RealType::RealType(const TypeWrapper& Obj_):
TypeWrapperImp(Obj_)
{}

/****************************************************************************/
// TypeWrapper class virtual methods
Boolean RealType::IsOrdinalType() const
{
  return FALSE;
}

/****************************************************************************/
/****************************************************************************/
TypeWrapperImp::TypeWrapperImp(int Options_):
ObjectImp(),
_ErrorStatus(Options_ & ALL_OPTIONS)
{}

/****************************************************************************/
TypeWrapperImp::TypeWrapperImp(const TypeWrapper& Obj_):
ObjectImp(Obj_._EnclosedObject()),
_ErrorStatus((Obj_.IsStatusSet(BOUNDS_CHECKING_BIT) ? BOUNDS_CHECKING_BIT:0) |
             (Obj_.IsStatusSet(TERMINATE_ON_ERROR_BIT) ? TERMINATE_ON_ERROR_BIT:0))
{}

/****************************************************************************/
Boolean TypeWrapperImp::IsSummableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsMultipliableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsOrdinalType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsInvertableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsLogicalType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsBitwiseableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean TypeWrapperImp::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
Object& TypeWrapperImp::_EnclosedObject()
{
  return EnclosedObject();
}

/****************************************************************************/    
const Object& TypeWrapperImp::_EnclosedObject() const
{
  return EnclosedObject();
}

/****************************************************************************/
void TypeWrapperImp::SetDivideByZero()
{
  _ErrorStatus |= DIVIDE_BY_ZERO_ERROR;

  if (_ErrorStatus & TERMINATE_ON_ERROR_BIT)
    THROW (FallibleBase::DivideByZeroErr());
}

/****************************************************************************/
void TypeWrapperImp::SetUnderflow()
{
  _ErrorStatus |= UNDERFLOW_ERROR;

  if (_ErrorStatus & TERMINATE_ON_ERROR_BIT)
    THROW (FallibleBase::TypeUnderflowErr());
}

/****************************************************************************/
void TypeWrapperImp::SetOverflow()
{
  _ErrorStatus |= OVERFLOW_ERROR;

  if (_ErrorStatus & TERMINATE_ON_ERROR_BIT)
    THROW (FallibleBase::TypeOverflowErr());
}

/****************************************************************************/
void TypeWrapperImp::SetDataConversionError()
{
  _ErrorStatus |= DATA_CONVERSION_ERROR;

  if (_ErrorStatus & TERMINATE_ON_ERROR_BIT)
    THROW (FallibleBase::TypeConversionErr());
}

/****************************************************************************/
void TypeWrapperImp::SetNoComplexRoots()
{
  SetExtErrorStatus(NOCOMPLEXROOTS_ERROR);

  if (_ErrorStatus & TERMINATE_ON_ERROR_BIT)
    THROW (FallibleBase::NoComplexRootsErr());  
}

/****************************************************************************/
void TypeWrapperImp::SetFindRootFunctionError()
{
  SetExtErrorStatus(FINDROOTFUNCTION_ERROR);

  if (_ErrorStatus & TERMINATE_ON_ERROR_BIT)
    THROW (FallibleBase::RootNotBracketedErr());  
}

/****************************************************************************/
void TypeWrapperImp::SetRootNotFoundError()
{
  SetExtErrorStatus(ROOTNOTFOUND_ERROR);

  if (_ErrorStatus & TERMINATE_ON_ERROR_BIT)
    THROW (FallibleBase::RootFindMaxIterationsExceededErr());  
}

/****************************************************************************/
TypeWrapper& TypeWrapperImp::TerminateOnError(Boolean Flag_)
{
  if (Flag_)
    _ErrorStatus |= TERMINATE_ON_ERROR_BIT;
  else
    _ErrorStatus &= ~TERMINATE_ON_ERROR_BIT;

  return *this;
}

/****************************************************************************/
TypeWrapper& TypeWrapperImp::CheckBounds(Boolean Flag_)
{
  if (Flag_)
    _ErrorStatus |= BOUNDS_CHECKING_BIT;
  else
    _ErrorStatus &= ~BOUNDS_CHECKING_BIT;

  return *this;
}

/****************************************************************************/
void TypeWrapperImp::ClearError(int ErrBits_)
{
  _ErrorStatus &= ~(ErrBits_ & ALL_STDERRORS);
  ClearExtErrorStatus(ErrBits_);
}

/****************************************************************************/
void TypeWrapperImp::ThrowError()
{
  if (_ErrorStatus & UNDERFLOW_ERROR)
    THROW (FallibleBase::TypeUnderflowErr());
    
  if (_ErrorStatus & OVERFLOW_ERROR)
    THROW (FallibleBase::TypeOverflowErr());

  if (_ErrorStatus & DIVIDE_BY_ZERO_ERROR)
    THROW (FallibleBase::DivideByZeroErr());

  if (_ErrorStatus & DATA_CONVERSION_ERROR)
    THROW (FallibleBase::TypeConversionErr());

  if (IsExtStatusSet(NOCOMPLEXROOTS_ERROR))
    THROW (FallibleBase::NoComplexRootsErr());

  if (IsExtStatusSet(FINDROOTFUNCTION_ERROR))
    THROW (FallibleBase::RootNotBracketedErr());
    
  if (IsExtStatusSet(ROOTNOTFOUND_ERROR))
    THROW (FallibleBase::RootFindMaxIterationsExceededErr());
}

/****************************************************************************/
Boolean TypeWrapperImp::IsOptionSet(int BitFlag_) const
{
  return (_ErrorStatus & ALL_OPTIONS & BitFlag_);
}

/****************************************************************************/
Boolean TypeWrapperImp::IsStatusSet(int BitFlag_) const
{
  return ((_ErrorStatus & ALL_STDERRORS & BitFlag_) | IsExtStatusSet(BitFlag_));
}

/****************************************************************************/
int TypeWrapperImp::ErrorStatus() const
{
  return ((_ErrorStatus & ALL_STDERRORS) | ExtErrorStatus());
}

/****************************************************************************/
int TypeWrapperImp::Options() const
{
  return (_ErrorStatus & ALL_OPTIONS);
}

/****************************************************************************/
void TypeWrapperImp::SetExtErrorStatus(unsigned BitFlag_)
{
  SetExtErrorStatusFlag(GetExtErrorStatusFlag() | (BitFlag_ & ALL_EXTERRORS));
}

/****************************************************************************/
void TypeWrapperImp::ClearExtErrorStatus(int ErrBits_)
{
  SetExtErrorStatusFlag(GetExtErrorStatusFlag() & ~(ErrBits_ & ALL_EXTERRORS));
}

/****************************************************************************/
Boolean TypeWrapperImp::IsExtStatusSet(int BitFlag_) const
{
  return (GetExtErrorStatusFlag() & BitFlag_ & ALL_EXTERRORS);
}

/****************************************************************************/
int TypeWrapperImp::ExtErrorStatus() const
{
  return (GetExtErrorStatusFlag() & ALL_EXTERRORS);
}

/****************************************************************************/
unsigned TypeWrapperImp::GetExtErrorStatusFlag() const
{
  static unsigned Dum_ = 0;
  return Dum_;
}

/****************************************************************************/
void TypeWrapperImp::SetExtErrorStatusFlag(unsigned Value_)
{}

/****************************************************************************/
TypeWrapperImp& TypeWrapperImp::operator = (const TypeWrapperImp& Obj_)
{
  _ErrorStatus = (Obj_.ErrorStatus() & ALL_STDERRORS);
  _ErrorStatus |= Obj_.Options();
  SetExtErrorStatusFlag(Obj_.GetExtErrorStatusFlag());
  ObjectImp::operator = (Obj_._EnclosedObject());
  return *this;
}

/****************************************************************************/
/****************************************************************************/
#endif





