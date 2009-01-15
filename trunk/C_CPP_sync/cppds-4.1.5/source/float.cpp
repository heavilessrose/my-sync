#ifndef FLOAT_CPP
#define FLOAT_CPP
#ifndef FLOAT_H
  #include "float.h"
#endif

const double Float::vPI = M_PI;
const double Float::vE = M_E;
const double Float::vNAN = ::log(-1);
const double Float::vINFINITY = HUGE_VAL;

/****************************************************************************/
Float::Float(float FltVal_):
StdFloatingPointWrapper<Float, float>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(FltVal_)
{}

/****************************************************************************/
Float::Float(const ChrString& Str_):
StdFloatingPointWrapper<Float, float>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Valid(IsValue(Str_))
{
  ChrString Temp_ = Str_;
  Temp_.ToLower();

  Byte_t Valid_ = _Valid;
  _Value = Valid_ ? Str_.strtod():
           (Temp_ == "inf" || Temp_ == "+inf") ? Float::vINFINITY:
           (Temp_ == "-inf") ? -Float::vINFINITY:Float::vNAN;

  if (!Valid_)
  {
    SetAsNullObject();
    SetDataConversionError();    
  }
}

/****************************************************************************/
Float::Float(const Float& FltObj_):
StdFloatingPointWrapper<Float, float>(FltObj_),
_Value(FltObj_._Value)
{}

/****************************************************************************/
Float& Float::DerivedClass()
{
  return *this;
}

/****************************************************************************/    
const Float& Float::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
Float* Float::Make(float FltVal_)
{
  return (new Float(FltVal_));
}

/****************************************************************************/
Float* Float::Make(const Object& Obj_)
{
  FloatAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object* Float::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Float(*this));

  FloatAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromFloat(TrgComp_)));
}

/****************************************************************************/
Object& Float::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  FloatAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromFloat(TrgComp_)));
}

/****************************************************************************/
Float& Float::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Float _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& Float::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Float::GiveNullObject();
}

/****************************************************************************/
Object& Float::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Float::GiveNullObject();
}

/****************************************************************************/
const char* Float::GiveHashableData() const
{
  return ToString(TYPE_FLOAT_PRECISION).c_str();
}

/****************************************************************************/
long Float::GiveHashableDataLength() const
{
  return ::SafeStrLen(ToString(TYPE_FLOAT_PRECISION).c_str());
}

/****************************************************************************/
float Float::FloatValue() const
{
  return _Value;
}

/****************************************************************************/
Boolean Float::IsValue(const ChrString& Str_)
{
  return IsFloat(Str_.c_str(), 10);
}

/****************************************************************************/
Float Float::ValueOf(const ChrString& Str_)
{
  return Float(Str_);
}

/****************************************************************************/
ChrString Float::ToString(const Float& Flt_, int Prec_)
{
  char Buffer_[64];
  if (Flt_.IsNan())
    return ChrString("nan");
  else if (Flt_.IsInfinite())
  {
    if (Flt_.FloatValue() < float(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  return ChrString(RemovePadding(FloatToStr(Flt_, Buffer_, 63, Prec_), " "));
}

/****************************************************************************/
ChrString Float::ToString(int Prec_) const
{
  char Buffer_[64];
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (FloatValue() < float(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  return ChrString(RemovePadding(FloatToStr(*this, Buffer_, 63, Prec_), " "));
}

/****************************************************************************/
ChrString Float::ToTruncString(const Float& Flt_, int Radix_)
{
  char Buffer_[64];
  if (Flt_.IsNan())
    return ChrString("nan");
  else if (Flt_.IsInfinite())
  {
    if (Flt_.FloatValue() < float(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  Float Temp_ = Floor(Flt_);
  long Lvalue_ = Temp_.IntValue();
  return ChrString(RemovePadding(LongToStr(Lvalue_, Buffer_, Radix_), " "));
}

/****************************************************************************/
ChrString Float::ToTruncString(int Radix_) const
{
  char Buffer_[64];
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (FloatValue() < float(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  Float Temp_ = Floor(*this);
  long Lvalue_ = Temp_.IntValue();
  return ChrString(RemovePadding(LongToStr(Lvalue_, Buffer_, Radix_), " "));
}

/****************************************************************************/
Float& Float::FromString(const ChrString& Str_)
{
  Float Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
const float& Float::GetData() const
{
  return _Value;
}

/****************************************************************************/
void Float::SetData(const float& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
Boolean Float::IsNan(float FltVal_)
{
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
  return isnan(FltVal_);
#else
  return (FltVal_ == Float::vNAN);
#endif
}

/****************************************************************************/
Boolean Float::IsInfinite(float FltVal_)
{
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
  return (isinf(FltVal_) || isinf(-FltVal_));
#else
  return (FltVal_ == Float::vINFINITY ||
          FltVal_ == -Float::vINFINITY);
#endif
}

/****************************************************************************/
Float Float::Ceiling(float FltVal_)
{
  return Float(ceil(FltVal_));
}

/****************************************************************************/
Float Float::Floor(float FltVal_)
{
  return Float(floor(FltVal_));
}

/****************************************************************************/
Float Float::Round(float FltVal_)
{
  if (FltVal_ >= 0)
    return Float(::floor(FltVal_ + .5));

  return Float(::ceil(FltVal_ - .5));
}

/****************************************************************************/
int Float::ClassID() const
{
  return TypeInfo::FLOAT_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* Float::GiveObjectAcceptor() const
{
  return (new FloatAcceptor(this));
}

/****************************************************************************/
Boolean Float::IsEqualToFloat(const Float* Ptr_) const
{
  return (_Value == Ptr_->_Value);
}

/****************************************************************************/
Boolean Float::IsLesserToFloat(const Float* Ptr_) const
{
  return (_Value < Ptr_->_Value);
}

/****************************************************************************/
Boolean Float::IsGreaterToFloat(const Float* Ptr_) const
{
  return (_Value > Ptr_->_Value);
}

/****************************************************************************/
Boolean Float::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  FloatAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToFloat(TrgComp_);
}

/****************************************************************************/
Boolean Float::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  FloatAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToFloat(TrgComp_);
}

/****************************************************************************/
Boolean Float::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  FloatAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToFloat(TrgComp_);
}

/****************************************************************************/
istream& Float::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.seekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& Float::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.read((char*)(&_Value), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& Float::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.write((char*)(&_Value), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& Float::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.iseekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& Float::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.iread((char*)(&_Value), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& Float::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.owrite((char*)(&_Value), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& Float::TextRead(istream& Is_, Boolean* Ok_)
{
  float Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& Float::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Value;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long Float::DataSize() const
{
  return sizeof(float);
}

/****************************************************************************/
Boolean Float::IsNan() const
{
  return IsNan(*this);
}

/****************************************************************************/
Boolean Float::IsInfinite() const
{
  return IsInfinite(*this);
}

/****************************************************************************/
Float& Float::Ceiling()
{
  *this = Ceiling(*this);
  return *this;
}

/****************************************************************************/
Float& Float::Floor()
{
  *this = Floor(*this);
  return *this;
}

/****************************************************************************/
Float& Float::Round()
{
  *this = Round(*this);
  return *this;
}

/****************************************************************************/
Boolean Float::AtZero() const
{
  return (_Value == 0);
}

/****************************************************************************/
Boolean Float::operator ! () const
{
  return !_Value;
}

/****************************************************************************/
Float Float::operator - () const
{
  return Float(-_Value);
}

/****************************************************************************/
Float Float::operator + () const
{
  return Float(*this);
}

/****************************************************************************/
Float& Float::operator = (const float& IntVal_)
{
  _Value = IntVal_;
  return *this;
}

/****************************************************************************/
Float& Float::operator = (const ChrString& Str_)
{
  Float Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
Float& Float::operator = (const Float& Obj_)
{
  _Value = Obj_._Value;
  StdFloatingPointWrapper<Float, float>::operator = (Obj_);  
  return *this;
}

/****************************************************************************/
Float& Float::operator += (const NumRealType<Float>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Float::vINFINITY;
    else
      _Value += Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Float& Float::operator -= (const NumRealType<Float>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Float::vINFINITY;
    else
      _Value -= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Float& Float::operator *= (const NumRealType<Float>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Float::vINFINITY;
    else
      _Value *= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Float& Float::operator /= (const NumRealType<Float>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Float::vINFINITY;
    else
    {
      if (Obj_.DerivedClass().GetData() == float(0))
        SetDivideByZero();
      else
        _Value /= Obj_.DerivedClass().GetData();
    }    

  return *this;
}

/****************************************************************************/
Float& Float::operator += (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value += Val_;

  return *this;
}

/****************************************************************************/
Float& Float::operator -= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value -= Val_;

  return *this;
}

/****************************************************************************/
Float& Float::operator *= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value *= Val_;

  return *this;
}

/****************************************************************************/
Float& Float::operator /= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    if (Val_ == long(0))
      SetDivideByZero();
    else  
      _Value /= Val_;  

  return *this;
}

/****************************************************************************/
Float& Float::operator += (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value += Val_._Value;

  return *this;
}

/****************************************************************************/
Float& Float::operator -= (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value -= Val_._Value;

  return *this;
}

/****************************************************************************/
Float& Float::operator *= (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value *= Val_._Value;

  return *this;
}

/****************************************************************************/
Float& Float::operator /= (UnsignedIntValue Val_)
{
  UnsignedIntValue ZeroVal_(0);

  if (!IsNan() && !IsInfinite())
    if (Val_._Value == ZeroVal_._Value)
      SetDivideByZero();
    else    
      _Value /= Val_._Value;  

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* Float::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void Float::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* Float::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void Float::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
Float operator + (const Float& Val1_, const Float& Val2_)
{
  return (Float(Val1_) += Val2_);
}

/****************************************************************************/  
Float operator - (const Float& Val1_, const Float& Val2_)
{
  return (Float(Val1_) -= Val2_);
}

/****************************************************************************/  
Float operator * (const Float& Val1_, const Float& Val2_)
{
  return (Float(Val1_) *= Val2_);
}

/****************************************************************************/  
Float operator / (const Float& Val1_, const Float& Val2_)
{
  return (Float(Val1_) /= Val2_);
}

/****************************************************************************/
Float operator + (long Val1_, const Float& Val2_)
{
  return (Float((float)Val1_) += Val2_);
}

/****************************************************************************/  
Float operator - (long Val1_, const Float& Val2_)
{
  return (Float((float)Val1_) -= Val2_);
}

/****************************************************************************/  
Float operator * (long Val1_, const Float& Val2_)
{
  return (Float((float)Val1_) *= Val2_);
}

/****************************************************************************/  
Float operator / (long Val1_, const Float& Val2_)
{
  return (Float((float)Val1_) /= Val2_);
}

/****************************************************************************/
Float operator + (const Float& Val1_, long Val2_)
{
  return (Float(Val1_) += Val2_);
}

/****************************************************************************/  
Float operator - (const Float& Val1_, long Val2_)
{
  return (Float(Val1_) -= Val2_);
}

/****************************************************************************/  
Float operator * (const Float& Val1_, long Val2_)
{
  return (Float(Val1_) *= Val2_);
}

/****************************************************************************/  
Float operator / (const Float& Val1_, long Val2_)
{
  return (Float(Val1_) /= Val2_);
}

/****************************************************************************/
Float operator + (UnsignedIntValue Val1_, const Float& Val2_)
{
  return (Float((float)(Val1_._Value)) += Val2_);
}

/****************************************************************************/  
Float operator - (UnsignedIntValue Val1_, const Float& Val2_)
{
  return (Float((float)(Val1_._Value)) -= Val2_);
}

/****************************************************************************/  
Float operator * (UnsignedIntValue Val1_, const Float& Val2_)
{
  return (Float((float)(Val1_._Value)) *= Val2_);
}

/****************************************************************************/  
Float operator / (UnsignedIntValue Val1_, const Float& Val2_)
{
  return (Float((float)(Val1_._Value)) /= Val2_);
}

/****************************************************************************/
Float operator + (const Float& Val1_, UnsignedIntValue Val2_)
{
  return (Float(Val1_) += Val2_);
}

/****************************************************************************/  
Float operator - (const Float& Val1_, UnsignedIntValue Val2_)
{
  return (Float(Val1_) -= Val2_);
}

/****************************************************************************/  
Float operator * (const Float& Val1_, UnsignedIntValue Val2_)
{
  return (Float(Val1_) *= Val2_);
}

/****************************************************************************/  
Float operator / (const Float& Val1_, UnsignedIntValue Val2_)
{
  return (Float(Val1_) /= Val2_);
}

/****************************************************************************/
/****************************************************************************/
Boolean FloatAcceptor::IsFloat() const
{
  return TRUE;
}

/****************************************************************************/
Boolean FloatAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(FloatAcceptor::FloatAcceptor, Float)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(FloatAcceptor::AcceptDataPtr, Float)
OBJACCEPTOR_GIVEDATA_TOPDEFN(FloatAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(FloatAcceptor::CreateFromFloat, Float, Float)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(FloatAcceptor::AssignFromFloat, Float)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(FloatAcceptor::IsLesserToFloat, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(FloatAcceptor::IsEqualToFloat, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(FloatAcceptor::IsGreaterToFloat, _ObjPtr->IsGreaterToFloat, Float)
MEMORYOPS_DEFN(FloatAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(FloatAcceptor::CreateFromChrString, Float, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(FloatAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(FloatAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToFloat, Float, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(FloatAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToFloat, Float, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(FloatAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToFloat, Float, ChrString)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::CreateFromLongNumber, Float)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::AssignFromLongNumber)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsLesserToLongNumber, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsEqualToLongNumber, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsGreaterToLongNumber, _ObjPtr->IsGreaterToFloat, Float)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::CreateFromULongInt, Float)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::AssignFromULongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsLesserToULongInt, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsEqualToULongInt, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsGreaterToULongInt, _ObjPtr->IsGreaterToFloat, Float)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::CreateFromUInteger, Float)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::AssignFromUInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsLesserToUInteger, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsEqualToUInteger, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsGreaterToUInteger, _ObjPtr->IsGreaterToFloat, Float)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::CreateFromUShortInt, Float)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::AssignFromUShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsLesserToUShortInt, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsEqualToUShortInt, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsGreaterToUShortInt, _ObjPtr->IsGreaterToFloat, Float)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::CreateFromLongInt, Float)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::AssignFromLongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsLesserToLongInt, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsEqualToLongInt, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsGreaterToLongInt, _ObjPtr->IsGreaterToFloat, Float)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::CreateFromInteger, Float)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::AssignFromInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsLesserToInteger, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsEqualToInteger, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsGreaterToInteger, _ObjPtr->IsGreaterToFloat, Float)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::CreateFromShortInt, Float)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::AssignFromShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsLesserToShortInt, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsEqualToShortInt, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(FloatAcceptor::IsGreaterToShortInt, _ObjPtr->IsGreaterToFloat, Float)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(FloatAcceptor::CreateFromDouble, Float, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(FloatAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(FloatAcceptor::IsLesserToDouble, _ObjPtr->IsLesserToFloat, Float, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(FloatAcceptor::IsEqualToDouble, _ObjPtr->IsEqualToFloat, Float, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(FloatAcceptor::IsGreaterToDouble, _ObjPtr->IsGreaterToFloat, Float, FloatTypesPrecision::DOUBLE)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(FloatAcceptor::CreateFromLongDouble, Float, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(FloatAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(FloatAcceptor::IsLesserToLongDouble, _ObjPtr->IsLesserToFloat, Float, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(FloatAcceptor::IsEqualToLongDouble, _ObjPtr->IsEqualToFloat, Float, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(FloatAcceptor::IsGreaterToLongDouble, _ObjPtr->IsGreaterToFloat, Float, FloatTypesPrecision::LONGDOUBLE)

OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN(FloatAcceptor::CreateFromRational, Float)
OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN(FloatAcceptor::AssignFromRational)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(FloatAcceptor::IsLesserToRational, _ObjPtr->IsLesserToFloat, Float)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(FloatAcceptor::IsEqualToRational, _ObjPtr->IsEqualToFloat, Float)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(FloatAcceptor::IsGreaterToRational, _ObjPtr->IsGreaterToFloat, Float)

/****************************************************************************/
/****************************************************************************/
#if FLOAT_DEBUG
int main()
{
  ChrString Str_;

  Float si(315.15123);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315.1512", 8));

  Float si2(315.15123);
  Float si3(-210.57123);
  Str_ = si3.ToString();
  assert(!strncmp(Str_.c_str(), "-210.5712", 9));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  ofstream fout("testing.bin");
  Write(fout, si);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, si3);
  fin.close();

  assert((si3 * Float(10000)).IntValue() == 3151512);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());  
  ChrString strobj_ = "568.123";
  si2.Assign(strobj_);
  cout <<si2 <<endl;

  si2 += 100;
  cout <<si2 <<endl;  
  si2 -= UnsignedIntValue(200);
  cout <<si2 <<endl;
  si2 *= 2;
  cout <<si2 <<endl;  
  si2 /= UnsignedIntValue(2);
  cout <<si2 <<endl;

  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;

  Rational<long> r1, r2, r3;

  r1(1l,2l);
  r2(2l,3l);

  r3 = r1 + r2;
  si.Assign(r1);
  si2.Assign(r2);
  si3.Assign(r3);

  cout <<si <<endl;
  cout <<si2 <<endl;
  cout <<si3 <<endl;

  cout <<endl <<"SUCCESS Testing Float" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





