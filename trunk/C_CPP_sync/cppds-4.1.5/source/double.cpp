#ifndef DOUBLE_CPP
#define DOUBLE_CPP
#ifndef DOUBLE_H
  #include "double.h"
#endif

const double Double::vPI = M_PI;
const double Double::vE = M_E;
const double Double::vNAN = ::log(-1);
const double Double::vINFINITY = HUGE_VAL;

/****************************************************************************/
Double::Double(double FltVal_):
StdFloatingPointWrapper<Double, double>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(FltVal_)
{}

/****************************************************************************/
Double::Double(const ChrString& Str_):
StdFloatingPointWrapper<Double, double>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Valid(IsValue(Str_))
{
  ChrString Temp_ = Str_;
  Temp_.ToLower();
  
  Byte_t Valid_ = _Valid;
  _Value = Valid_ ? Str_.strtod():
           (Temp_ == "inf" || Temp_ == "+inf") ? Double::vINFINITY:
           (Temp_ == "-inf") ? -Double::vINFINITY:Double::vNAN;

  if (!Valid_)
  {
    SetAsNullObject();
    SetDataConversionError();    
  }
}

/****************************************************************************/
Double::Double(const Double& FltObj_):
StdFloatingPointWrapper<Double, double>(FltObj_),
_Value(FltObj_._Value)
{}

/****************************************************************************/
Double& Double::DerivedClass()
{
  return *this;
}

/****************************************************************************/    
const Double& Double::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
Double* Double::Make(double FltVal_)
{
  return (new Double(FltVal_));
}

/****************************************************************************/
Double* Double::Make(const Object& Obj_)
{
  DoubleAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
const char* Double::GiveHashableData() const
{
  return ToString(TYPE_DOUBLE_PRECISION).c_str();
}

/****************************************************************************/
long Double::GiveHashableDataLength() const
{
  return ::SafeStrLen(ToString(TYPE_DOUBLE_PRECISION).c_str());
}

/****************************************************************************/
Object* Double::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Double(*this));

  DoubleAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromDouble(TrgComp_)));
}

/****************************************************************************/
Object& Double::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  DoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromDouble(TrgComp_)));
}

/****************************************************************************/
Double& Double::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Double _NULLOBJECT(0);
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& Double::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Double::GiveNullObject();
}

/****************************************************************************/
Object& Double::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Double::GiveNullObject();
}

/****************************************************************************/
double Double::FloatValue() const
{
  return _Value;
}

/****************************************************************************/
Boolean Double::IsValue(const ChrString& Str_)
{
  return IsFloat(Str_.c_str(), 10);
}

/****************************************************************************/
Double Double::ValueOf(const ChrString& Str_)
{
  return Double(Str_);
}

/****************************************************************************/
ChrString Double::ToString(const Double& Dbl_, int Prec_)
{
  char Buffer_[128];
  if (Dbl_.IsNan())
    return ChrString("nan");
  else if (Dbl_.IsInfinite())
  {
    if (Dbl_.FloatValue() < double(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  return ChrString(RemovePadding(FloatToStr(Dbl_, Buffer_, 127, Prec_), " "));
}

/****************************************************************************/
ChrString Double::ToString(int Prec_) const
{
  char Buffer_[128];
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (FloatValue() < double(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }

  return ChrString(RemovePadding(FloatToStr(*this, Buffer_, 127, Prec_), " "));
}

/****************************************************************************/
ChrString Double::ToTruncString(const Double& Dbl_, int Radix_)
{
  char Buffer_[128];
  if (Dbl_.IsNan())
    return ChrString("nan");
  else if (Dbl_.IsInfinite())
  {
    if (Dbl_.FloatValue() < double(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  Double Temp_ = Floor(Dbl_);
  long Lvalue_ = Temp_.IntValue();
  return ChrString(RemovePadding(LongToStr(Lvalue_, Buffer_, Radix_), " "));
}

/****************************************************************************/
ChrString Double::ToTruncString(int Radix_) const
{
  char Buffer_[128];
  if (IsNan())
    return ChrString("nan");
  else if (IsInfinite())
  {
    if (FloatValue() < double(0))
      return ChrString("-inf");
    else
      return ChrString("+inf");
  }
  
  Double Temp_ = Floor(*this);
  long Lvalue_ = Temp_.IntValue();
  return ChrString(RemovePadding(LongToStr(Lvalue_, Buffer_, Radix_), " "));
}

/****************************************************************************/
Double& Double::FromString(const ChrString& Str_)
{
  Double Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
const double& Double::GetData() const
{
  return _Value;
}

/****************************************************************************/
void Double::SetData(const double& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
Boolean Double::IsNan(double FltVal_)
{
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
  return isnan(FltVal_);
#else
  return (FltVal_ == Double::vNAN);
#endif
}

/****************************************************************************/
Boolean Double::IsInfinite(double FltVal_)
{
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
  return (isinf(FltVal_) || isinf(-FltVal_));
#else
  return (FltVal_ == Double::vINFINITY ||
          FltVal_ == -Double::vINFINITY);
#endif
}

/****************************************************************************/
Double Double::Ceiling(double FltVal_)
{
  return Double(ceil(FltVal_));
}

/****************************************************************************/
Double Double::Floor(double FltVal_)
{
  return Double(floor(FltVal_));
}

/****************************************************************************/
Double Double::Round(double FltVal_)
{
  if (FltVal_ >= 0)
    return Double(::floor(FltVal_ + .5));

  return Double(::ceil(FltVal_ - .5));
}

/****************************************************************************/
int Double::ClassID() const
{
  return TypeInfo::DOUBLE_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* Double::GiveObjectAcceptor() const
{
  return (new DoubleAcceptor(this));
}

/****************************************************************************/
Boolean Double::IsEqualToDouble(const Double* Ptr_) const
{
  return (_Value == Ptr_->_Value);
}

/****************************************************************************/
Boolean Double::IsLesserToDouble(const Double* Ptr_) const
{
  return (_Value < Ptr_->_Value);
}

/****************************************************************************/
Boolean Double::IsGreaterToDouble(const Double* Ptr_) const
{
  return (_Value > Ptr_->_Value);
}

/****************************************************************************/
Boolean Double::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToDouble(TrgComp_);
}

/****************************************************************************/
Boolean Double::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToDouble(TrgComp_);
}

/****************************************************************************/
Boolean Double::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DoubleAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToDouble(TrgComp_);
}

/****************************************************************************/
istream& Double::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& Double::BinaryRead(istream& Is_, Boolean* Ok_)
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
ostream& Double::BinaryWrite(ostream& Os_, Boolean* Ok_) const
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
ByteStream& Double::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& Double::BinaryRead(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& Double::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
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
istream& Double::TextRead(istream& Is_, Boolean* Ok_)
{
  double Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& Double::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Value;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long Double::DataSize () const
{
  return sizeof(double);
}

/****************************************************************************/
Boolean Double::IsNan() const
{
  return IsNan(*this);
}

/****************************************************************************/
Boolean Double::IsInfinite() const
{
  return IsInfinite(*this);
}

/****************************************************************************/
Double& Double::Ceiling()
{
  *this = Ceiling(*this);
  return *this;
}

/****************************************************************************/
Double& Double::Floor()
{
  *this = Floor(*this);
  return *this;
}

/****************************************************************************/
Double& Double::Round()
{
  *this = Round(*this);
  return *this;
}

/****************************************************************************/
Boolean Double::AtZero() const
{
  return (_Value == 0);
}

/****************************************************************************/
Boolean Double::operator ! () const
{
  return !_Value;
}

/****************************************************************************/
Double Double::operator - () const
{
  return Double(-_Value);
}

/****************************************************************************/
Double Double::operator + () const
{
  return Double(*this);
}

/****************************************************************************/
Double& Double::operator = (const double& IntVal_)
{
  _Value = IntVal_;
  return *this;
}

/****************************************************************************/
Double& Double::operator = (const ChrString& Str_)
{
  Double Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
Double& Double::operator = (const Double& Obj_)
{
  _Value = Obj_._Value;
  StdFloatingPointWrapper<Double, double>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
Double& Double::operator += (const NumRealType<Double>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Double::vINFINITY;
    else
      _Value += Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Double& Double::operator -= (const NumRealType<Double>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Double::vINFINITY;
    else
      _Value -= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Double& Double::operator *= (const NumRealType<Double>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Double::vINFINITY;
    else
      _Value *= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Double& Double::operator /= (const NumRealType<Double>& Obj_)
{
  if (!Obj_.DerivedClass().IsNan() && !IsNan())
    if (Obj_.DerivedClass().IsInfinite() || IsInfinite())
      _Value = Double::vINFINITY;
    else
    {
      if (Obj_.DerivedClass().GetData() == double(0))
        SetDivideByZero();
      else
        _Value /= Obj_.DerivedClass().GetData();
    }

  return *this;
}

/****************************************************************************/
Double& Double::operator += (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value += Val_;

  return *this;
}

/****************************************************************************/
Double& Double::operator -= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value -= Val_;

  return *this;
}

/****************************************************************************/
Double& Double::operator *= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value *= Val_;

  return *this;
}

/****************************************************************************/
Double& Double::operator /= (long Val_)
{
  if (!IsNan() && !IsInfinite())
    if (Val_ == long(0))
      SetDivideByZero();
    else  
      _Value /= Val_;

  return *this;
}

/****************************************************************************/
Double& Double::operator += (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value += Val_._Value;

  return *this;
}

/****************************************************************************/
Double& Double::operator -= (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value -= Val_._Value;

  return *this;
}

/****************************************************************************/
Double& Double::operator *= (UnsignedIntValue Val_)
{
  if (!IsNan() && !IsInfinite())
    _Value *= Val_._Value;

  return *this;
}

/****************************************************************************/
Double& Double::operator /= (UnsignedIntValue Val_)
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
void* Double::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void Double::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* Double::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void Double::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
Double operator + (const Double& Val1_, const Double& Val2_)
{
  return (Double(Val1_) += Val2_);
}

/****************************************************************************/  
Double operator - (const Double& Val1_, const Double& Val2_)
{
  return (Double(Val1_) -= Val2_);
}

/****************************************************************************/  
Double operator * (const Double& Val1_, const Double& Val2_)
{
  return (Double(Val1_) *= Val2_);
}

/****************************************************************************/  
Double operator / (const Double& Val1_, const Double& Val2_)
{
  return (Double(Val1_) /= Val2_);
}

/****************************************************************************/
Double operator + (long Val1_, const Double& Val2_)
{
  return (Double((float)Val1_) += Val2_);
}

/****************************************************************************/  
Double operator - (long Val1_, const Double& Val2_)
{
  return (Double((float)Val1_) -= Val2_);
}

/****************************************************************************/  
Double operator * (long Val1_, const Double& Val2_)
{
  return (Double((float)Val1_) *= Val2_);
}

/****************************************************************************/  
Double operator / (long Val1_, const Double& Val2_)
{
  return (Double((float)Val1_) /= Val2_);
}

/****************************************************************************/
Double operator + (const Double& Val1_, long Val2_)
{
  return (Double(Val1_) += Val2_);
}

/****************************************************************************/  
Double operator - (const Double& Val1_, long Val2_)
{
  return (Double(Val1_) -= Val2_);
}

/****************************************************************************/  
Double operator * (const Double& Val1_, long Val2_)
{
  return (Double(Val1_) *= Val2_);
}

/****************************************************************************/  
Double operator / (const Double& Val1_, long Val2_)
{
  return (Double(Val1_) /= Val2_);
}

/****************************************************************************/
Double operator + (UnsignedIntValue Val1_, const Double& Val2_)
{
  return (Double((float)(Val1_._Value)) += Val2_);
}

/****************************************************************************/  
Double operator - (UnsignedIntValue Val1_, const Double& Val2_)
{
  return (Double((float)(Val1_._Value)) -= Val2_);
}

/****************************************************************************/  
Double operator * (UnsignedIntValue Val1_, const Double& Val2_)
{
  return (Double((float)(Val1_._Value)) *= Val2_);
}

/****************************************************************************/  
Double operator / (UnsignedIntValue Val1_, const Double& Val2_)
{
  return (Double((float)(Val1_._Value)) /= Val2_);
}

/****************************************************************************/
Double operator + (const Double& Val1_, UnsignedIntValue Val2_)
{
  return (Double(Val1_) += Val2_);
}

/****************************************************************************/  
Double operator - (const Double& Val1_, UnsignedIntValue Val2_)
{
  return (Double(Val1_) -= Val2_);
}

/****************************************************************************/  
Double operator * (const Double& Val1_, UnsignedIntValue Val2_)
{
  return (Double(Val1_) *= Val2_);
}

/****************************************************************************/  
Double operator / (const Double& Val1_, UnsignedIntValue Val2_)
{
  return (Double(Val1_) /= Val2_);
}

/****************************************************************************/
/****************************************************************************/
Boolean DoubleAcceptor::IsDouble() const
{
  return TRUE;
}

/****************************************************************************/
Boolean DoubleAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(DoubleAcceptor::DoubleAcceptor, Double)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(DoubleAcceptor::AcceptDataPtr, Double)
OBJACCEPTOR_GIVEDATA_TOPDEFN(DoubleAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(DoubleAcceptor::CreateFromDouble, Double, Double)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(DoubleAcceptor::AssignFromDouble, Double)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(DoubleAcceptor::IsLesserToDouble, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(DoubleAcceptor::IsEqualToDouble, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(DoubleAcceptor::IsGreaterToDouble, _ObjPtr->IsGreaterToDouble, Double)
MEMORYOPS_DEFN(DoubleAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(DoubleAcceptor::CreateFromChrString, Double, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(DoubleAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(DoubleAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToDouble, Double, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(DoubleAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToDouble, Double, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(DoubleAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToDouble, Double, ChrString)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::CreateFromLongNumber, Double)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::AssignFromLongNumber)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsLesserToLongNumber, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsEqualToLongNumber, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsGreaterToLongNumber, _ObjPtr->IsGreaterToDouble, Double)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::CreateFromULongInt, Double)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::AssignFromULongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsLesserToULongInt, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsEqualToULongInt, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsGreaterToULongInt, _ObjPtr->IsGreaterToDouble, Double)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::CreateFromUInteger, Double)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::AssignFromUInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsLesserToUInteger, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsEqualToUInteger, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsGreaterToUInteger, _ObjPtr->IsGreaterToDouble, Double)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::CreateFromUShortInt, Double)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::AssignFromUShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsLesserToUShortInt, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsEqualToUShortInt, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsGreaterToUShortInt, _ObjPtr->IsGreaterToDouble, Double)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::CreateFromLongInt, Double)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::AssignFromLongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsLesserToLongInt, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsEqualToLongInt, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsGreaterToLongInt, _ObjPtr->IsGreaterToDouble, Double)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::CreateFromInteger, Double)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::AssignFromInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsLesserToInteger, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsEqualToInteger, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsGreaterToInteger, _ObjPtr->IsGreaterToDouble, Double)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::CreateFromShortInt, Double)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::AssignFromShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsLesserToShortInt, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsEqualToShortInt, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(DoubleAcceptor::IsGreaterToShortInt, _ObjPtr->IsGreaterToDouble, Double)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::CreateFromFloat, Double, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::IsLesserToFloat, _ObjPtr->IsLesserToDouble, Double, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::IsEqualToFloat, _ObjPtr->IsEqualToDouble, Double, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::IsGreaterToFloat, _ObjPtr->IsGreaterToDouble, Double, FloatTypesPrecision::FLOAT)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::CreateFromLongDouble, Double, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::IsLesserToLongDouble, _ObjPtr->IsLesserToDouble, Double, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::IsEqualToLongDouble, _ObjPtr->IsEqualToDouble, Double, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(DoubleAcceptor::IsGreaterToLongDouble, _ObjPtr->IsGreaterToDouble, Double, FloatTypesPrecision::LONGDOUBLE)

OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN(DoubleAcceptor::CreateFromRational, Double)
OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN(DoubleAcceptor::AssignFromRational)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(DoubleAcceptor::IsLesserToRational, _ObjPtr->IsLesserToDouble, Double)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(DoubleAcceptor::IsEqualToRational, _ObjPtr->IsEqualToDouble, Double)
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(DoubleAcceptor::IsGreaterToRational, _ObjPtr->IsGreaterToDouble, Double)

/****************************************************************************/
/****************************************************************************/
#if DOUBLE_DEBUG
int main()
{
  ChrString Str_;

  Double si(315.15123);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315.1512", 8));

  Double si2(315.15123);
  Double si3(-210.57123);
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

  assert((si3 * Double(100000L)).IntValue() == 31515123);

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

  cout <<endl <<"SUCCESS Testing Double" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





