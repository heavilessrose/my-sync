#ifndef LONGDOUBLE_CPP
#define LONGDOUBLE_CPP
#ifndef LONGDOUBLE_H
  #include "ldouble.h"
#endif

const double LongDouble::vPI = M_PI;
const double LongDouble::vE = M_E;
const double LongDouble::vNAN = ::log(-1);
const double LongDouble::vINFINITY = HUGE_VAL;

/****************************************************************************/
LongDouble::LongDouble(Ldouble FltVal_):
StdFloatingPointWrapper<LongDouble, Ldouble>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(FltVal_)
{}

/****************************************************************************/
LongDouble::LongDouble(const ChrString& Str_):
StdFloatingPointWrapper<LongDouble, Ldouble>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Valid(IsValue(Str_))
{
  ChrString Temp_ = Str_;
  Temp_.ToLower();

  Byte_t Valid_ = _Valid;
  _Value = Valid_ ? Str_.strtod():
           (Temp_ == "inf" || Temp_ == "+inf") ? LongDouble::vINFINITY:
           (Temp_ == "-inf") ? -LongDouble::vINFINITY:LongDouble::vNAN;
  
  if (!Valid_)
  {
    SetAsNullObject();
    SetDataConversionError();
  }
}

/****************************************************************************/
LongDouble::LongDouble(const LongDouble& FltObj_):
StdFloatingPointWrapper<LongDouble, Ldouble>(FltObj_),
_Value(FltObj_._Value)
{}

/****************************************************************************/
LongDouble& LongDouble::DerivedClass()
{
  return *this;
}

/****************************************************************************/    
const LongDouble& LongDouble::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
LongDouble* LongDouble::Make(Ldouble FltVal_)
{
  return (new LongDouble(FltVal_));
}

/****************************************************************************/
LongDouble* LongDouble::Make(const Object& Obj_)
{
  LongDoubleAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object* LongDouble::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new LongDouble(*this));

  LongDoubleAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromLongDouble(TrgComp_)));
}

/****************************************************************************/
Object& LongDouble::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  LongDoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromLongDouble(TrgComp_)));
}

/****************************************************************************/
LongDouble& LongDouble::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static LongDouble _NULLOBJECT(0);
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& LongDouble::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LongDouble::GiveNullObject();
}

/****************************************************************************/
Object& LongDouble::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LongDouble::GiveNullObject();
}

/****************************************************************************/
const char* LongDouble::GiveHashableData() const
{
  return ToString().c_str();
}

/****************************************************************************/
long LongDouble::GiveHashableDataLength() const
{
  return ::SafeStrLen(ToString().c_str());
}

/****************************************************************************/
Ldouble LongDouble::FloatValue() const
{
  return _Value;
}

/****************************************************************************/
Boolean LongDouble::IsValue(const ChrString& Str_)
{
  return IsFloat(Str_.c_str(), 10);
}

/****************************************************************************/
LongDouble LongDouble::ValueOf(const ChrString& Str_)
{
  return LongDouble(Str_);
}

/****************************************************************************/
ChrString LongDouble::ToString(const LongDouble& Dbl_, int Prec_)
{
  char Buffer_[128];
  if (Dbl_.IsNan())
    return ChrString("nan");
  else if (Dbl_.IsInfinite())
  {
    if (Dbl_.FloatValue() < Ldouble(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  return ChrString(RemovePadding(LongFloatToStr(Dbl_, Buffer_, 127, Prec_), " "));
}

/****************************************************************************/
ChrString LongDouble::ToString(int Prec_) const
{
  char Buffer_[128];
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (FloatValue() < Ldouble(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  return ChrString(RemovePadding(LongFloatToStr(*this, Buffer_, 127, Prec_), " "));
}

/****************************************************************************/
ChrString LongDouble::ToTruncString(const LongDouble& Dbl_, int Radix_)
{
  char Buffer_[128];
  if (Dbl_.IsNan())
    return ChrString("nan");
  else if (Dbl_.IsInfinite())
  {
    if (Dbl_.FloatValue() < Ldouble(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  LongDouble Temp_ = Floor(Dbl_);
  long Lvalue_ = Temp_.IntValue();
  return ChrString(RemovePadding(LongToStr(Lvalue_, Buffer_, Radix_), " "));
}

/****************************************************************************/
ChrString LongDouble::ToTruncString(int Radix_) const
{
  char Buffer_[128];
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (FloatValue() < Ldouble(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  LongDouble Temp_ = Floor(*this);
  long Lvalue_ = Temp_.IntValue();
  return ChrString(RemovePadding(LongToStr(Lvalue_, Buffer_, Radix_), " "));
}

/****************************************************************************/
LongDouble& LongDouble::FromString(const ChrString& Str_)
{
  LongDouble Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
const Ldouble& LongDouble::GetData() const
{
  return _Value;
}

/****************************************************************************/
void LongDouble::SetData(const Ldouble& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
Boolean LongDouble::IsNan(Ldouble FltVal_)
{
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
  return isnan(FltVal_);
#else
  return (FltVal_ == LongDouble::vNAN);
#endif
}

/****************************************************************************/
Boolean LongDouble::IsInfinite(Ldouble FltVal_)
{
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
  return (isinf(FltVal_) || isinf(-FltVal_));
#else
  return (FltVal_ == LongDouble::vINFINITY ||
          FltVal_ == -LongDouble::vINFINITY);
#endif
}

/****************************************************************************/
LongDouble LongDouble::Ceiling(Ldouble FltVal_)
{
  return LongDouble(ceil(FltVal_));
}

/****************************************************************************/
LongDouble LongDouble::Floor(Ldouble FltVal_)
{
  return LongDouble(floor(FltVal_));
}

/****************************************************************************/
LongDouble LongDouble::Round(Ldouble FltVal_)
{
  if (FltVal_ >= 0)
    return LongDouble(::floor(FltVal_ + .5));

  return LongDouble(::ceil(FltVal_ - .5));
}

/****************************************************************************/
int LongDouble::ClassID() const
{
  return TypeInfo::LONGDOUBLE_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* LongDouble::GiveObjectAcceptor() const
{
  return (new LongDoubleAcceptor(this));
}

/****************************************************************************/
Boolean LongDouble::IsEqualToLongDouble(const LongDouble* Ptr_) const
{
  return (_Value == Ptr_->_Value);
}

/****************************************************************************/
Boolean LongDouble::IsLesserToLongDouble(const LongDouble* Ptr_) const
{
  return (_Value < Ptr_->_Value);
}

/****************************************************************************/
Boolean LongDouble::IsGreaterToLongDouble(const LongDouble* Ptr_) const
{
  return (_Value > Ptr_->_Value);
}

/****************************************************************************/
Boolean LongDouble::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongDoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToLongDouble(TrgComp_);
}

/****************************************************************************/
Boolean LongDouble::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongDoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToLongDouble(TrgComp_);
}

/****************************************************************************/
Boolean LongDouble::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongDoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToLongDouble(TrgComp_);
}

/****************************************************************************/
istream& LongDouble::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& LongDouble::BinaryRead(istream& Is_, Boolean* Ok_)
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
ostream& LongDouble::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.write((char*)(&_Value), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& LongDouble::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& LongDouble::BinaryRead(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& LongDouble::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.owrite((char*)(&_Value), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& LongDouble::TextRead(istream& Is_, Boolean* Ok_)
{
  Ldouble Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& LongDouble::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Value;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long LongDouble::DataSize () const
{
  return sizeof(Ldouble);
}

/****************************************************************************/
Boolean LongDouble::IsNan() const
{
  return IsNan(*this);
}

/****************************************************************************/
Boolean LongDouble::IsInfinite() const
{
  return IsInfinite(*this);
}

/****************************************************************************/
LongDouble& LongDouble::Ceiling()
{
  *this = Ceiling(*this);
  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::Floor()
{
  *this = Floor(*this);
  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::Round()
{
  *this = Round(*this);
  return *this;
}

/****************************************************************************/
Boolean LongDouble::AtZero() const
{
  return (_Value == 0);
}

/****************************************************************************/
Boolean LongDouble::operator ! () const
{
  return !_Value;
}

/****************************************************************************/
LongDouble LongDouble::operator - () const
{
  return LongDouble(-_Value);
}

/****************************************************************************/
LongDouble LongDouble::operator + () const
{
  return LongDouble(*this);
}

/****************************************************************************/
LongDouble& LongDouble::operator = (const Ldouble& Value_)
{
  _Value = Value_;
  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator = (const ChrString& Str_)
{
  LongDouble Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongDouble& LongDouble::operator = (const LongDouble& Obj_)
{
  _Value = Obj_._Value;
  StdFloatingPointWrapper<LongDouble, Ldouble>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator += (const NumRealType<LongDouble>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = LongDouble::vINFINITY;
    else
      _Value += Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator -= (const NumRealType<LongDouble>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = LongDouble::vINFINITY;
    else
      _Value -= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator *= (const NumRealType<LongDouble>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = LongDouble::vINFINITY;
    else
      _Value *= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator /= (const NumRealType<LongDouble>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = LongDouble::vINFINITY;
    else
    {
      if (Obj_.DerivedClass().GetData() == Ldouble(0))
        SetDivideByZero();
      else
        _Value /= Obj_.DerivedClass().GetData();
    }    

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator += (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value += Val_;

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator -= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value -= Val_;

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator *= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value *= Val_;

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator /= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    if (Val_ == long(0))
      SetDivideByZero();
    else  
      _Value /= Val_;  

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator += (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value += Val_._Value;

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator -= (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value -= Val_._Value;

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator *= (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value *= Val_._Value;

  return *this;
}

/****************************************************************************/
LongDouble& LongDouble::operator /= (UnsignedIntValue Val_)
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
void* LongDouble::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void LongDouble::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* LongDouble::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void LongDouble::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
LongDouble operator + (const LongDouble& Val1_, const LongDouble& Val2_)
{
  return (LongDouble(Val1_) += Val2_);
}

/****************************************************************************/  
LongDouble operator - (const LongDouble& Val1_, const LongDouble& Val2_)
{
  return (LongDouble(Val1_) -= Val2_);
}

/****************************************************************************/  
LongDouble operator * (const LongDouble& Val1_, const LongDouble& Val2_)
{
  return (LongDouble(Val1_) *= Val2_);
}

/****************************************************************************/  
LongDouble operator / (const LongDouble& Val1_, const LongDouble& Val2_)
{
  return (LongDouble(Val1_) /= Val2_);
}

/****************************************************************************/
LongDouble operator + (long Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)Val1_) += Val2_);
}

/****************************************************************************/  
LongDouble operator - (long Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)Val1_) -= Val2_);
}

/****************************************************************************/  
LongDouble operator * (long Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)Val1_) *= Val2_);
}

/****************************************************************************/  
LongDouble operator / (long Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)Val1_) /= Val2_);
}

/****************************************************************************/
LongDouble operator + (const LongDouble& Val1_, long Val2_)
{
  return (LongDouble(Val1_) += Val2_);
}

/****************************************************************************/  
LongDouble operator - (const LongDouble& Val1_, long Val2_)
{
  return (LongDouble(Val1_) -= Val2_);
}

/****************************************************************************/  
LongDouble operator * (const LongDouble& Val1_, long Val2_)
{
  return (LongDouble(Val1_) *= Val2_);
}

/****************************************************************************/  
LongDouble operator / (const LongDouble& Val1_, long Val2_)
{
  return (LongDouble(Val1_) /= Val2_);
}

/****************************************************************************/
LongDouble operator + (UnsignedIntValue Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)(Val1_._Value)) += Val2_);
}

/****************************************************************************/  
LongDouble operator - (UnsignedIntValue Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)(Val1_._Value)) -= Val2_);
}

/****************************************************************************/  
LongDouble operator * (UnsignedIntValue Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)(Val1_._Value)) *= Val2_);
}

/****************************************************************************/  
LongDouble operator / (UnsignedIntValue Val1_, const LongDouble& Val2_)
{
  return (LongDouble((float)(Val1_._Value)) /= Val2_);
}

/****************************************************************************/
LongDouble operator + (const LongDouble& Val1_, UnsignedIntValue Val2_)
{
  return (LongDouble(Val1_) += Val2_);
}

/****************************************************************************/  
LongDouble operator - (const LongDouble& Val1_, UnsignedIntValue Val2_)
{
  return (LongDouble(Val1_) -= Val2_);
}

/****************************************************************************/  
LongDouble operator * (const LongDouble& Val1_, UnsignedIntValue Val2_)
{
  return (LongDouble(Val1_) *= Val2_);
}

/****************************************************************************/  
LongDouble operator / (const LongDouble& Val1_, UnsignedIntValue Val2_)
{
  return (LongDouble(Val1_) /= Val2_);
}

/****************************************************************************/
/****************************************************************************/
Boolean LongDoubleAcceptor::IsLongDouble() const
{
  return TRUE;
}

/****************************************************************************/
Boolean LongDoubleAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(LongDoubleAcceptor::LongDoubleAcceptor, LongDouble)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(LongDoubleAcceptor::AcceptDataPtr, LongDouble)
OBJACCEPTOR_GIVEDATA_TOPDEFN(LongDoubleAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongDoubleAcceptor::CreateFromLongDouble, LongDouble, LongDouble)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongDoubleAcceptor::AssignFromLongDouble, LongDouble)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongDoubleAcceptor::IsLesserToLongDouble, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongDoubleAcceptor::IsEqualToLongDouble, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongDoubleAcceptor::IsGreaterToLongDouble, _ObjPtr->IsGreaterToLongDouble, LongDouble)
MEMORYOPS_DEFN(LongDoubleAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(LongDoubleAcceptor::CreateFromChrString, LongDouble, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongDoubleAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongDoubleAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToLongDouble, LongDouble, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongDoubleAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToLongDouble, LongDouble, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongDoubleAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToLongDouble, LongDouble, ChrString)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::CreateFromLongNumber, LongDouble)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::AssignFromLongNumber)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsLesserToLongNumber, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsEqualToLongNumber, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsGreaterToLongNumber, _ObjPtr->IsGreaterToLongDouble, LongDouble)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::CreateFromULongInt, LongDouble)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::AssignFromULongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsLesserToULongInt, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsEqualToULongInt, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsGreaterToULongInt, _ObjPtr->IsGreaterToLongDouble, LongDouble)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::CreateFromUInteger, LongDouble)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::AssignFromUInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsLesserToUInteger, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsEqualToUInteger, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsGreaterToUInteger, _ObjPtr->IsGreaterToLongDouble, LongDouble)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::CreateFromUShortInt, LongDouble)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::AssignFromUShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsLesserToUShortInt, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsEqualToUShortInt, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsGreaterToUShortInt, _ObjPtr->IsGreaterToLongDouble, LongDouble)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::CreateFromLongInt, LongDouble)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::AssignFromLongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsLesserToLongInt, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsEqualToLongInt, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsGreaterToLongInt, _ObjPtr->IsGreaterToLongDouble, LongDouble)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::CreateFromInteger, LongDouble)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::AssignFromInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsLesserToInteger, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsEqualToInteger, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsGreaterToInteger, _ObjPtr->IsGreaterToLongDouble, LongDouble)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::CreateFromShortInt, LongDouble)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::AssignFromShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsLesserToShortInt, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsEqualToShortInt, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongDoubleAcceptor::IsGreaterToShortInt, _ObjPtr->IsGreaterToLongDouble, LongDouble)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::CreateFromFloat, LongDouble, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::IsLesserToFloat, _ObjPtr->IsLesserToLongDouble, LongDouble, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::IsEqualToFloat, _ObjPtr->IsEqualToLongDouble, LongDouble, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::IsGreaterToFloat, _ObjPtr->IsGreaterToLongDouble, LongDouble, FloatTypesPrecision::FLOAT)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::CreateFromDouble, LongDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::IsLesserToDouble, _ObjPtr->IsLesserToLongDouble, LongDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::IsEqualToDouble, _ObjPtr->IsEqualToLongDouble, LongDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongDoubleAcceptor::IsGreaterToDouble, _ObjPtr->IsGreaterToLongDouble, LongDouble, FloatTypesPrecision::DOUBLE)

OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN(LongDoubleAcceptor::CreateFromRational, LongDouble)
OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN(LongDoubleAcceptor::AssignFromRational)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(LongDoubleAcceptor::IsLesserToRational, _ObjPtr->IsLesserToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(LongDoubleAcceptor::IsEqualToRational, _ObjPtr->IsEqualToLongDouble, LongDouble)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(LongDoubleAcceptor::IsGreaterToRational, _ObjPtr->IsGreaterToLongDouble, LongDouble)
/****************************************************************************/
/****************************************************************************/
#if LONGDOUBLE_DEBUG
int main()
{
  ChrString Str_;

  LongDouble si(315.15123);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315.1512", 8));

  LongDouble si2(315.15123);
  LongDouble si3(-210.57123);
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

  assert((si3 * LongDouble(10000)).IntValue() == 3151512);

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

  cout <<endl <<"SUCCESS Testing LongDouble" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





