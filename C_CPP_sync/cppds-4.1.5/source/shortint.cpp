#ifndef SHORTINT_CPP
#define SHORTINT_CPP
#ifndef SHORTINT_H
  #include "shortint.h"
#endif

/****************************************************************************/
ShortInt::ShortInt(short IntVal_):
IntegerAsBitsType<ShortInt, short>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(IntVal_)
{}

/****************************************************************************/
ShortInt::ShortInt(Ushort UIntVal_, Boolean SetToNull_):
IntegerAsBitsType<ShortInt, short>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Uvalue(UIntVal_)
{
  if (SetToNull_)
    SetAsNullObject();
}

/****************************************************************************/
ShortInt::ShortInt(const ChrString& Str_, Boolean Signed_, Boolean Validity_):
IntegerAsBitsType<ShortInt, short>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Valid(Signed_ ? ShortInt::IsValue(Str_):Validity_)
{
  if (Signed_)
  {
    Byte_t Valid_ = _Valid;
    _Value = Valid_ ? short(Str_.strtol()):0;

    if (!Valid_)
    {
      SetAsNullObject();
      SetDataConversionError();
    }
  }
}

/****************************************************************************/
ShortInt::ShortInt(const ShortInt& IntObj_):
IntegerAsBitsType<ShortInt, short>(IntObj_),
_Value(IntObj_._Value)
{}

/****************************************************************************/
ShortInt& ShortInt::DerivedClass()
{
  return *this;
}

/****************************************************************************/    
const ShortInt& ShortInt::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
ShortInt* ShortInt::Make(short IntVal_)
{
  return (new ShortInt(IntVal_));
}

/****************************************************************************/
ShortInt* ShortInt::Make(const Object& Obj_)
{
  ShortIntAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& ShortInt::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  ShortIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromShortInt(TrgComp_)));
}

/****************************************************************************/
Object* ShortInt::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new ShortInt(*this));

  ShortIntAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromShortInt(TrgComp_)));
}

/****************************************************************************/
const char* ShortInt::GiveHashableData() const
{
  return ((const char*)&_Value);
}

/****************************************************************************/
long ShortInt::GiveHashableDataLength() const
{
  return sizeof(_Value);
}

/****************************************************************************/
ShortInt& ShortInt::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static ShortInt _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& ShortInt::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ShortInt::GiveNullObject();
}

/****************************************************************************/
Object& ShortInt::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ShortInt::GiveNullObject();
}

/****************************************************************************/
double ShortInt::FloatValue() const
{
  return _Value;
}

/****************************************************************************/
Boolean ShortInt::IsValue(const ChrString& Str_)
{
  return IsShort(Str_.c_str());
}

/****************************************************************************/
ShortInt ShortInt::ValueOf(const ChrString& Str_)
{
  return ShortInt(Str_);
}

/****************************************************************************/
ChrString ShortInt::ToString(const ShortInt& Int_, int Radix_)
{
  char Buffer_[16];
  return ChrString(ShortToStr(Int_, Buffer_, Radix_));
}

/****************************************************************************/
ChrString ShortInt::ToString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(ShortToStr(*this, Buffer_, Radix_));
}

/****************************************************************************/
ChrString ShortInt::ToTruncString(const ShortInt& Int_, int Radix_)
{
  char Buffer_[16];
  return ChrString(ShortToStr(Int_, Buffer_, Radix_));
}

/****************************************************************************/
ChrString ShortInt::ToTruncString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(ShortToStr(*this, Buffer_, Radix_));
}

/****************************************************************************/
ShortInt& ShortInt::FromString(const ChrString& Str_)
{
  ShortInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
const short& ShortInt::GetData() const
{
  return _Value;
}

/****************************************************************************/
void ShortInt::SetData(const short& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
int ShortInt::ClassID() const
{
  return TypeInfo::SHORTINT_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* ShortInt::GiveObjectAcceptor() const
{
  return (new ShortIntAcceptor(this));
}

/****************************************************************************/
Boolean ShortInt::IsEqualToShortInt(const ShortInt* Ptr_) const
{
  return (_Value == Ptr_->_Value);
}

/****************************************************************************/
Boolean ShortInt::IsLesserToShortInt(const ShortInt* Ptr_) const
{
  return (_Value < Ptr_->_Value);
}

/****************************************************************************/
Boolean ShortInt::IsGreaterToShortInt(const ShortInt* Ptr_) const
{
  return (_Value > Ptr_->_Value);
}

/****************************************************************************/
Boolean ShortInt::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ShortIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToShortInt(TrgComp_);
}

/****************************************************************************/
Boolean ShortInt::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ShortIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToShortInt(TrgComp_);
}

/****************************************************************************/
Boolean ShortInt::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ShortIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToShortInt(TrgComp_);
}

/****************************************************************************/
istream& ShortInt::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& ShortInt::BinaryRead(istream& Is_, Boolean* Ok_)
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
ostream& ShortInt::BinaryWrite(ostream& Os_, Boolean* Ok_) const
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
ByteStream& ShortInt::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& ShortInt::BinaryRead(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& ShortInt::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
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
istream& ShortInt::TextRead(istream& Is_, Boolean* Ok_)
{
  short Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& ShortInt::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Value;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long ShortInt::DataSize() const
{
  return sizeof(short);
}

/****************************************************************************/
Boolean ShortInt::AtZero() const
{
  return (_Value == 0);
}

/****************************************************************************/
Boolean ShortInt::AtMaxValue() const
{
  return (_Value == SHRT_MAX);
}

/****************************************************************************/
Boolean ShortInt::AtMinValue() const
{
  return (_Value == SHRT_MIN);
}

/****************************************************************************/
ShortInt& ShortInt::operator = (const short& IntVal_)
{
  _Value = IntVal_;
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator = (const ChrString& Str_)
{
  ShortInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
ShortInt& ShortInt::operator = (const ShortInt& Obj_)
{
  _Value = Obj_._Value;  
  IntegerAsBitsType<ShortInt, short>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
Boolean ShortInt::operator ! () const
{
  return !_Value;
}

/****************************************************************************/
ShortInt ShortInt::operator - () const
{
  return ShortInt(-_Value);
}

/****************************************************************************/
ShortInt ShortInt::operator + () const
{
  return ShortInt(*this);
}

/****************************************************************************/
const ShortInt& ShortInt::operator ++ ()
{
  if (BoundsChecking() && _Value == SHRT_MAX)
    SetOverflow();

  ++_Value;
  return *this;
}

/****************************************************************************/
const ShortInt& ShortInt::operator ++ (int)
{
  static ShortInt OldVal_ = _Value;

  if (BoundsChecking() && _Value == SHRT_MAX)
    SetOverflow();
  
  ++_Value;
  return OldVal_;
}

/****************************************************************************/
const ShortInt& ShortInt::operator -- ()
{
  if (BoundsChecking() && _Value == SHRT_MIN)
    SetUnderflow();

  --_Value;
  return *this;
}

/****************************************************************************/
const ShortInt& ShortInt::operator -- (int)
{
  static ShortInt OldVal_ = _Value;

  if (BoundsChecking() && _Value == SHRT_MIN)
    SetUnderflow();
  
  --_Value;
  return OldVal_;
}

/****************************************************************************/
ShortInt& ShortInt::operator += (const NumOrdinalType<ShortInt>& Obj_)
{
  if (BoundsChecking())
  {
    short Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = short(Obj_.DerivedClass());

    shortSeriesCompare CeilCmp_ = shortSeriesCompare::SumOfSeriesCmp(SHRT_MAX, Buffer_, 2, FALSE);
    shortSeriesCompare FloorCmp_ = shortSeriesCompare::SumOfSeriesCmp(SHRT_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value += Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator -= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (BoundsChecking())
  {
    short Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = short(Obj_.DerivedClass());

    shortSeriesCompare CeilCmp_ = shortSeriesCompare::DifferenceOfSeriesCmp(SHRT_MAX, Buffer_, 2, FALSE);
    shortSeriesCompare FloorCmp_ = shortSeriesCompare::DifferenceOfSeriesCmp(SHRT_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value -= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator *= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (BoundsChecking())
  {
    short Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = short(Obj_.DerivedClass());

    shortSeriesCompare CeilCmp_ = shortSeriesCompare::ProductOfSeriesCmp(SHRT_MAX, Buffer_, 2, FALSE);
    shortSeriesCompare FloorCmp_ = shortSeriesCompare::ProductOfSeriesCmp(SHRT_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value *= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator /= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value /= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator %= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value %= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator += (const short& IntVal_)
{
  return (*this += ShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& ShortInt::operator -= (const short& IntVal_)
{
  return (*this -= ShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& ShortInt::operator *= (const short& IntVal_)
{
  return (*this *= ShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& ShortInt::operator /= (const short& IntVal_)
{
  return (*this /= ShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& ShortInt::operator %= (const short& IntVal_)
{
  return (*this %= ShortInt(IntVal_));
}

/****************************************************************************/
ShortInt ShortInt::operator ~ () const
{
  return ShortInt(~_Value);
}

/****************************************************************************/
ShortInt& ShortInt::operator |= (const NumOrdinalAsBitsType<ShortInt>& Obj_)
{
  _Value |= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator &= (const NumOrdinalAsBitsType<ShortInt>& Obj_)
{
  _Value &= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator ^= (const NumOrdinalAsBitsType<ShortInt>& Obj_)
{
  _Value ^= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator |= (const short& IntVal_)
{
  return (*this |= ShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& ShortInt::operator &= (const short& IntVal_)
{
  return (*this &= ShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& ShortInt::operator ^= (const short& IntVal_)
{
  return (*this ^= ShortInt(IntVal_));
}

/****************************************************************************/
ShortInt& ShortInt::operator <<= (size_t Shift_)
{
  _Value <<= Shift_;
  return *this;
}

/****************************************************************************/
ShortInt& ShortInt::operator >>= (size_t Shift_)
{
  _Value >>= Shift_;
  return *this;
}

/****************************************************************************/
ShortInt ShortInt::operator << (size_t Shift_) const
{
  return (ShortInt(*this) <<= Shift_);
}

/****************************************************************************/    
ShortInt ShortInt::operator >> (size_t Shift_) const
{
  return (ShortInt(*this) >>= Shift_);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ShortInt::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ShortInt::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* ShortInt::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void ShortInt::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
ShortInt operator + (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) += Val2_);
}

/****************************************************************************/  
ShortInt operator - (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) -= Val2_);
}

/****************************************************************************/  
ShortInt operator * (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) *= Val2_);
}

/****************************************************************************/  
ShortInt operator / (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) /= Val2_);
}

/****************************************************************************/  
ShortInt operator % (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) %= Val2_);
}

/****************************************************************************/
ShortInt operator & (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) &= Val2_);
}

/****************************************************************************/  
ShortInt operator | (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) |= Val2_);
}

/****************************************************************************/  
ShortInt operator ^ (const ShortInt& Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) ^= Val2_);
}

/****************************************************************************/
ShortInt operator + (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) += Val2_);
}

/****************************************************************************/  
ShortInt operator - (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) -= Val2_);
}

/****************************************************************************/  
ShortInt operator * (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) *= Val2_);
}

/****************************************************************************/  
ShortInt operator / (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) /= Val2_);
}

/****************************************************************************/  
ShortInt operator % (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) %= Val2_);
}

/****************************************************************************/  
ShortInt operator & (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) &= Val2_);
}

/****************************************************************************/  
ShortInt operator | (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) |= Val2_);
}

/****************************************************************************/  
ShortInt operator ^ (short Val1_, const ShortInt& Val2_)
{
  return (ShortInt(Val1_) ^= Val2_);
}

/****************************************************************************/
ShortInt operator + (const ShortInt& Val1_, short Val2_)
{
  return (ShortInt(Val1_) += ShortInt(Val2_));
}

/****************************************************************************/  
ShortInt operator - (const ShortInt& Val1_, short Val2_)
{
  ShortInt Temp1_(Val1_);
  ShortInt Temp2_(Val2_);
  return (Temp1_ -= Temp2_);
}

/****************************************************************************/  
ShortInt operator * (const ShortInt& Val1_, short Val2_)
{
  return (ShortInt(Val1_) *= ShortInt(Val2_));
}

/****************************************************************************/  
ShortInt operator / (const ShortInt& Val1_, short Val2_)
{
  ShortInt Temp1_(Val1_);
  ShortInt Temp2_(Val2_);
  return (Temp1_ /= Temp2_);
}

/****************************************************************************/  
ShortInt operator % (const ShortInt& Val1_, short Val2_)
{
  ShortInt Temp1_(Val1_);
  ShortInt Temp2_(Val2_);
  return (Temp1_ %= Temp2_);
}

/****************************************************************************/  
ShortInt operator & (const ShortInt& Val1_, short Val2_)
{
  return (ShortInt(Val1_) &= ShortInt(Val2_));
}

/****************************************************************************/  
ShortInt operator | (const ShortInt& Val1_, short Val2_)
{
  return (ShortInt(Val1_) |= ShortInt(Val2_));
}

/****************************************************************************/  
ShortInt operator ^ (const ShortInt& Val1_, short Val2_)
{
  return (ShortInt(Val1_) ^= ShortInt(Val2_));
}

/****************************************************************************/
/****************************************************************************/
Boolean ShortIntAcceptor::IsShortInt() const
{
  return TRUE;
}

/****************************************************************************/
Boolean ShortIntAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(ShortIntAcceptor::ShortIntAcceptor, ShortInt)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(ShortIntAcceptor::AcceptDataPtr, ShortInt)
OBJACCEPTOR_GIVEDATA_TOPDEFN(ShortIntAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(ShortIntAcceptor::CreateFromShortInt, ShortInt, ShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ShortIntAcceptor::AssignFromShortInt, ShortInt)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(ShortIntAcceptor::IsLesserToShortInt, _ObjPtr->IsLesserToShortInt, ShortInt)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(ShortIntAcceptor::IsEqualToShortInt, _ObjPtr->IsEqualToShortInt, ShortInt)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(ShortIntAcceptor::IsGreaterToShortInt, _ObjPtr->IsGreaterToShortInt, ShortInt)
MEMORYOPS_DEFN(ShortIntAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(ShortIntAcceptor::CreateFromChrString, ShortInt, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ShortIntAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(ShortIntAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToShortInt, ShortInt, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(ShortIntAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToShortInt, ShortInt, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(ShortIntAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToShortInt, ShortInt, ChrString)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::CreateFromLongNumber, ShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::AssignFromLongNumber)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsLesserToLongNumber, _ObjPtr->IsLesserToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsEqualToLongNumber, _ObjPtr->IsEqualToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsGreaterToLongNumber, _ObjPtr->IsGreaterToShortInt, ShortInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::CreateFromULongInt, ShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::AssignFromULongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsLesserToULongInt, _ObjPtr->IsLesserToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsEqualToULongInt, _ObjPtr->IsEqualToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsGreaterToULongInt, _ObjPtr->IsGreaterToShortInt, ShortInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::CreateFromUInteger, ShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::AssignFromUInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsLesserToUInteger, _ObjPtr->IsLesserToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsEqualToUInteger, _ObjPtr->IsEqualToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsGreaterToUInteger, _ObjPtr->IsGreaterToShortInt, ShortInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::CreateFromLongInt, ShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::AssignFromLongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsLesserToLongInt, _ObjPtr->IsLesserToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsEqualToLongInt, _ObjPtr->IsEqualToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsGreaterToLongInt, _ObjPtr->IsGreaterToShortInt, ShortInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::CreateFromInteger, ShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::AssignFromInteger)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsLesserToInteger, _ObjPtr->IsLesserToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsEqualToInteger, _ObjPtr->IsEqualToShortInt, ShortInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(ShortIntAcceptor::IsGreaterToInteger, _ObjPtr->IsGreaterToShortInt, ShortInt)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::CreateFromFloat, ShortInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsLesserToFloat, _ObjPtr->IsLesserToShortInt, ShortInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsEqualToFloat, _ObjPtr->IsEqualToShortInt, ShortInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsGreaterToFloat, _ObjPtr->IsGreaterToShortInt, ShortInt, FloatTypesPrecision::FLOAT)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::CreateFromDouble, ShortInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsLesserToDouble, _ObjPtr->IsLesserToShortInt, ShortInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsEqualToDouble, _ObjPtr->IsEqualToShortInt, ShortInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsGreaterToDouble, _ObjPtr->IsGreaterToShortInt, ShortInt, FloatTypesPrecision::DOUBLE)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::CreateFromLongDouble, ShortInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsLesserToLongDouble, _ObjPtr->IsLesserToShortInt, ShortInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsEqualToLongDouble, _ObjPtr->IsEqualToShortInt, ShortInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(ShortIntAcceptor::IsGreaterToLongDouble, _ObjPtr->IsGreaterToShortInt, ShortInt, FloatTypesPrecision::LONGDOUBLE)

/****************************************************************************/
/****************************************************************************/
#if SHORTINT_DEBUG1
int ShortIntAction::_Init = 0;

void* ShortIntRunnable::Run(void* Ptr_)
{
	ShortIntData* _DataPtr = (ShortIntData*)Ptr_;
	_Action.Execute(_DataPtr);
	return NULL;
}

void ShortIntAction::Execute(ShortIntData* DataPtr_)
{
	EnterCriticalSection(GiveMutex());

	ShortInt* _Data;
	if (DataPtr_)	
	  if (DataPtr_->_Type == ShortIntData::SHORT_VALUE)
		_Data = ShortInt::Make(DataPtr_->_ValData);
	  else
		_Data = ShortInt::Make(*DataPtr_->_ObjData);

	size_t i; 
	for (i = 1; i < 10; i *= 2)
	{
	  *_Data += i;
	  cout <<(*_Data) <<endl;
	}

	LeaveCriticalSection(GiveMutex());
}
#endif

#if SHORTINT_DEBUG
int main()
{
#if SHORTINT_DEBUG1
  ShortIntRunnable sir1;
//  ShortIntRunnable sir2;
  ShortIntRunnable sir3;
  ShortIntRunnable sir4;

  ShortIntData sid1;
//  ShortIntData sid2;
  ShortIntData sid3;
//  ChrString str2("2000");
  ShortInt si3("3000");
  ShortIntData sid4;
  ShortInt si4("4000");

  sid1._Type = ShortIntData::SHORT_VALUE;
  sid1._ValData = 1000;
//  sid2._Type = ShortIntData::STRING_OBJECT;
//  sid2._ObjData = &str2;
  sid3._Type = ShortIntData::SHORTINT_OBJECT;
  sid3._ObjData = &si3;
  sid4._Type = ShortIntData::SHORTINT_OBJECT;
  sid4._ObjData = &si4;

  sir1.Start(&sid1);
//  sir2.Start(&sid2);
  sir3.Start(&sid3);
  sir4.Start(&sid4);

  sir1.Wait();
//  sir2.Wait();
  sir3.Wait();
  sir4.Wait();
#endif

#if SHORTINT_DEBUG2
  ChrString Str_;

  ShortInt si(31515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "31515"));

  ShortInt si2(31515);
  ShortInt si3(-21057);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "-21057"));

  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);
  
/*
  ofstream fout("testing.bin");
  Write(fout, si);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, si3);
  fin.close();  
  assert(si3 == si);
*/

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2 = "-32090";
  assert(si2 == -32090);

  si2 = "34000";
  assert(!(si2 == 34000));
  assert(si2 == 0);

  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;

  // Testing bounds checking
  ShortInt chk1 = 32767;
  chk1 += ShortInt(5);
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 32767;
  chk1 = -chk1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 256;
  chk1 = chk1 * ShortInt(128);
  
  assert(chk1.Overflow());
  chk1 = 256;
  chk1 >>= 2;
  assert(short(chk1) == 64);
  chk1 <<= 4;
  assert(short(chk1) == 1024);
  chk1 |= 0x0F;
  assert(short(chk1) == 1039);
  chk1 /= ShortInt(0);
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

  chk1.ClearError();
  try
  {
    chk1.ThrowError();
  }
  catch (const SciEngErr& ErrObj_)
  {
    cerr <<ErrObj_.message();
  }

  // Testing series comparison class on user defined type
  ShortInt* List_[25];
  short i;

  for (i = 0; i < 10; ++i)
    List_[i] = new ShortInt(i+1);
  List_[10] = NULL;

  ShortInt ZeroChar(0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::SumOfSignedSeriesCmp(ShortInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ShortInt>::DifferenceOfSignedSeriesCmp(ShortInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<ShortInt>::DifferenceOfSignedSeriesCmp(ShortInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<ShortInt>::DifferenceOfSignedSeriesCmp(ShortInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<ShortInt> Pobj_ = SeriesCompare<ShortInt>::ProductOfSignedSeriesCmp(ShortInt(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<ShortInt>::ProductOfSignedSeriesCmp(ShortInt(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<ShortInt>::ProductOfSignedSeriesCmp(ShortInt(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];  

  cout <<endl <<"SUCCESS Testing ShortInt" <<endl;
#endif

#if SHORTINT_DEBUG3
  ShortInt si1(2001);

  cout <<AssociatedIndex(si1, 2000) <<endl;
  ShowInfo((ostream&)cout, si1);
  cout <<endl <<SizeOfData(si1) <<endl;
  cout <<FormWidthOfObject(si1) <<endl;
  cout <<WhatIs(si1) <<endl;
  ShortInt* siobjp = (ShortInt*)Clone(si1);
  assert(*siobjp == 2001);    
  cout <<*siobjp <<endl;
  *siobjp = 3001;
  Assign(si1, *siobjp);
  NonConstAssign(si1, *siobjp);
  assert(si1 == 3001);  
  cout <<si1 <<endl;
  si1 = 4001;
  *siobjp = 1001;
  Swap(*siobjp, si1);
  cout <<si1 <<endl;
  cout <<*siobjp <<endl;
  assert(si1 == 1001);
  assert(*siobjp == 4001);
  
  ofstream fout("testing.bin");
  Write(fout, si1);
  fout.close();

  ShortInt si2;
  ifstream fin("testing.bin");
  fin.clear();
  Read(fin, si2);
  fin.close();
  
  cout <<si2 <<endl;
//  assert(si2 == 1001);
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif





