#ifndef LONGINT_CPP
#define LONGINT_CPP
#ifndef LONGINT_H
  #include "longint.h"
#endif

/****************************************************************************/
LongInt::LongInt(long IntVal_):
IntegerAsBitsType<LongInt, long>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(IntVal_)
{}

/****************************************************************************/
LongInt::LongInt(Ulong ULongVal_, Boolean SetToNull_):
IntegerAsBitsType<LongInt, long>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Uvalue(ULongVal_)
{
  if (SetToNull_)
    SetAsNullObject();
}

/****************************************************************************/
LongInt::LongInt(const ChrString& Str_, Boolean Signed_, Boolean Validity_):
IntegerAsBitsType<LongInt, long>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Valid(Signed_ ? LongInt::IsValue(Str_):Validity_)
{
  if (Signed_)
  {
    Byte_t Valid_ = _Valid;
    _Value = Valid_ ? Str_.strtol():0L;

    if (!Valid_)
    {
      SetAsNullObject();
      SetDataConversionError();
    }
  }
}

/****************************************************************************/
LongInt::LongInt(const ShortInt& IntObj_):
IntegerAsBitsType<LongInt, long>(IntObj_.Options()),
_Value(IsUShortInt(IntObj_) ? IntObj_.UnsignedValue():IntObj_.IntValue())
{}

/****************************************************************************/
LongInt::LongInt(const Integer& IntObj_):
IntegerAsBitsType<LongInt, long>(IntObj_.Options()),
_Value(IntObj_.IntValue())
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
LongInt::LongInt(const UShortInt& IntObj_):
IntegerAsBitsType<LongInt, long>(IntObj_.Options()),
_Uvalue(IntObj_.UnsignedValue())
{}

/****************************************************************************/
LongInt::LongInt(const UInteger& IntObj_):
IntegerAsBitsType<LongInt, long>(IntObj_.Options()),
_Uvalue(IntObj_.UnsignedValue())
{}
#endif
/****************************************************************************/
LongInt::LongInt(const LongInt& IntObj_):
IntegerAsBitsType<LongInt, long>(IntObj_),
_Value(IntObj_._Value)
{}

/****************************************************************************/
LongInt& LongInt::DerivedClass()
{
  return *this;
}

/****************************************************************************/    
const LongInt& LongInt::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
LongInt* LongInt::Make(long IntVal_)
{
  return (new LongInt(IntVal_));
}

/****************************************************************************/
LongInt* LongInt::Make(const Object& Obj_)
{
  LongIntAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& LongInt::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  LongIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromLongInt(TrgComp_)));
}

/****************************************************************************/
Object* LongInt::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new LongInt(*this));

  LongIntAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromLongInt(TrgComp_)));
}

/****************************************************************************/
LongInt& LongInt::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static LongInt _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& LongInt::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LongInt::GiveNullObject();
}

/****************************************************************************/
Object& LongInt::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LongInt::GiveNullObject();
}

/****************************************************************************/
const char* LongInt::GiveHashableData() const
{
  return ((const char*)&_Value);
}

/****************************************************************************/
long LongInt::GiveHashableDataLength() const
{
  return sizeof(_Value);
}

/****************************************************************************/
double LongInt::FloatValue() const
{
  return _Value;
}

/****************************************************************************/
Boolean LongInt::IsValue(const ChrString& Str_)
{
  return IsLong(Str_.c_str());
}

/****************************************************************************/
LongInt LongInt::ValueOf(const ChrString& Str_)
{
  return LongInt(Str_);
}

/****************************************************************************/
ChrString LongInt::ToString(const LongInt& Int_, int Radix_)
{
  char Buffer_[16];
  return ChrString(LongToStr(Int_, Buffer_, Radix_));
}

/****************************************************************************/
ChrString LongInt::ToString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(LongToStr(*this, Buffer_, Radix_));
}

/****************************************************************************/
ChrString LongInt::ToTruncString(const LongInt& Int_, int Radix_)
{
  char Buffer_[16];
  return ChrString(LongToStr(Int_, Buffer_, Radix_));
}

/****************************************************************************/
ChrString LongInt::ToTruncString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(LongToStr(*this, Buffer_, Radix_));
}

/****************************************************************************/
LongInt& LongInt::FromString(const ChrString& Str_)
{
  LongInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
const long& LongInt::GetData() const
{
  return _Value;
}

/****************************************************************************/
void LongInt::SetData(const long& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
int LongInt::ClassID() const
{
  return TypeInfo::LONGINT_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* LongInt::GiveObjectAcceptor() const
{
  return (new LongIntAcceptor(this));
}

/****************************************************************************/
Boolean LongInt::IsEqualToLongInt(const LongInt* Ptr_) const
{
  return (_Value == Ptr_->_Value);
}

/****************************************************************************/
Boolean LongInt::IsLesserToLongInt(const LongInt* Ptr_) const
{
  return (_Value < Ptr_->_Value);
}

/****************************************************************************/
Boolean LongInt::IsGreaterToLongInt(const LongInt* Ptr_) const
{
  return (_Value > Ptr_->_Value);
}

/****************************************************************************/
Boolean LongInt::IsUShortInt(const ShortInt& Obj_)
{
  return (Obj_.ClassID() != ShortInt::StaticID());
}

/****************************************************************************/
Boolean LongInt::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToLongInt(TrgComp_);
}

/****************************************************************************/
Boolean LongInt::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToLongInt(TrgComp_);
}

/****************************************************************************/
Boolean LongInt::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  LongIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToLongInt(TrgComp_);
}

/****************************************************************************/
istream& LongInt::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& LongInt::BinaryRead(istream& Is_, Boolean* Ok_)
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
ostream& LongInt::BinaryWrite(ostream& Os_, Boolean* Ok_) const
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
ByteStream& LongInt::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& LongInt::BinaryRead(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& LongInt::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
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
istream& LongInt::TextRead(istream& Is_, Boolean* Ok_)
{
  long Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& LongInt::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Value;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long LongInt::DataSize () const
{
  return sizeof(long);
}

/****************************************************************************/
Boolean LongInt::AtZero() const
{
  return (_Value == 0);
}

/****************************************************************************/
Boolean LongInt::AtMaxValue() const
{
  return (_Value == LONG_MAX);
}

/****************************************************************************/
Boolean LongInt::AtMinValue() const
{
  return (_Value == LONG_MIN);
}

/****************************************************************************/
LongInt& LongInt::operator = (const long& IntVal_)
{
  _Value = IntVal_;
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator = (const ChrString& Str_)
{
  LongInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& LongInt::operator = (const ShortInt& Obj_)
{
  LongInt Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& LongInt::operator = (const UShortInt& Obj_)
{
  LongInt Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& LongInt::operator = (const Integer& Obj_)
{
  LongInt Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
void LongInt::AssignUInteger(const UInteger& Obj_)
{
  LongInt Sobj_(Obj_);
  (*this = Sobj_);
}

/****************************************************************************/
LongInt& LongInt::operator = (const LongInt& Obj_)
{
  _Value = Obj_._Value;
  IntegerAsBitsType<LongInt, long>::operator = (Obj_);  
  return *this;
}

/****************************************************************************/
Boolean LongInt::operator ! () const
{
  return !_Value;
}

/****************************************************************************/
LongInt LongInt::operator - () const
{
  return LongInt(-_Value);
}

/****************************************************************************/
LongInt LongInt::operator + () const
{
  return LongInt(*this);
}

/****************************************************************************/
const LongInt& LongInt::operator ++ ()
{
  if (BoundsChecking() && _Value == LONG_MAX)
    SetOverflow();

  ++_Value;
  return *this;
}

/****************************************************************************/
const LongInt& LongInt::operator ++ (int)
{
  static LongInt OldVal_ = _Value;

  if (BoundsChecking() && _Value == LONG_MAX)
    SetOverflow();
  
  ++_Value;
  return OldVal_;
}

/****************************************************************************/
const LongInt& LongInt::operator -- ()
{
  if (BoundsChecking() && _Value == LONG_MIN)
    SetUnderflow();

  --_Value;
  return *this;
}

/****************************************************************************/
const LongInt& LongInt::operator -- (int)
{
  static LongInt OldVal_ = _Value;

  if (BoundsChecking() && _Value == LONG_MIN)
    SetUnderflow();
  
  --_Value;
  return OldVal_;
}

/****************************************************************************/
LongInt& LongInt::operator += (const NumOrdinalType<LongInt>& Obj_)
{
  if (BoundsChecking())
  {
    long Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = long(Obj_.DerivedClass());

    longSeriesCompare CeilCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MAX, Buffer_, 2, FALSE);
    longSeriesCompare FloorCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value += Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator -= (const NumOrdinalType<LongInt>& Obj_)
{
  if (BoundsChecking())
  {
    long Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = long(Obj_.DerivedClass());

    longSeriesCompare CeilCmp_ = longSeriesCompare::DifferenceOfSeriesCmp(LONG_MAX, Buffer_, 2, FALSE);
    longSeriesCompare FloorCmp_ = longSeriesCompare::DifferenceOfSeriesCmp(LONG_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value -= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator *= (const NumOrdinalType<LongInt>& Obj_)
{
  if (BoundsChecking())
  {
    long Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = long(Obj_.DerivedClass());

    longSeriesCompare CeilCmp_ = longSeriesCompare::ProductOfSeriesCmp(LONG_MAX, Buffer_, 2, FALSE);
    longSeriesCompare FloorCmp_ = longSeriesCompare::ProductOfSeriesCmp(LONG_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value *= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator /= (const NumOrdinalType<LongInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value /= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator %= (const NumOrdinalType<LongInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value %= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator += (const long& IntVal_)
{
  return (*this += LongInt(IntVal_));
}

/****************************************************************************/    
LongInt& LongInt::operator -= (const long& IntVal_)
{
  return (*this -= LongInt(IntVal_));
}

/****************************************************************************/    
LongInt& LongInt::operator *= (const long& IntVal_)
{
  return (*this *= LongInt(IntVal_));
}

/****************************************************************************/    
LongInt& LongInt::operator /= (const long& IntVal_)
{
  return (*this /= LongInt(IntVal_));
}

/****************************************************************************/    
LongInt& LongInt::operator %= (const long& IntVal_)
{
  return (*this %= LongInt(IntVal_));
}

/****************************************************************************/
LongInt LongInt::operator ~ () const
{
  return LongInt(~_Value);
}

/****************************************************************************/
LongInt& LongInt::operator |= (const NumOrdinalAsBitsType<LongInt>& Obj_)
{
  _Value |= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator &= (const NumOrdinalAsBitsType<LongInt>& Obj_)
{
  _Value &= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator ^= (const NumOrdinalAsBitsType<LongInt>& Obj_)
{
  _Value ^= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator |= (const long& IntVal_)
{
  return (*this |= LongInt(IntVal_));
}

/****************************************************************************/    
LongInt& LongInt::operator &= (const long& IntVal_)
{
  return (*this &= LongInt(IntVal_));
}

/****************************************************************************/    
LongInt& LongInt::operator ^= (const long& IntVal_)
{
  return (*this ^= LongInt(IntVal_));
}

/****************************************************************************/
LongInt& LongInt::operator <<= (size_t Shift_)
{
  _Value <<= Shift_;
  return *this;
}

/****************************************************************************/
LongInt& LongInt::operator >>= (size_t Shift_)
{
  _Value >>= Shift_;
  return *this;
}

/****************************************************************************/
LongInt LongInt::operator << (size_t Shift_) const
{
  return (LongInt(*this) <<= Shift_);
}

/****************************************************************************/    
LongInt LongInt::operator >> (size_t Shift_) const
{
  return (LongInt(*this) >>= Shift_);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* LongInt::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void LongInt::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* LongInt::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void LongInt::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
LongInt operator + (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) += Val2_);
}

/****************************************************************************/  
LongInt operator - (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) -= Val2_);
}

/****************************************************************************/  
LongInt operator * (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) *= Val2_);
}

/****************************************************************************/  
LongInt operator / (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) /= Val2_);
}

/****************************************************************************/  
LongInt operator % (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) %= Val2_);
}

/****************************************************************************/
LongInt operator & (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) &= Val2_);
}

/****************************************************************************/  
LongInt operator | (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) |= Val2_);
}

/****************************************************************************/  
LongInt operator ^ (const LongInt& Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) ^= Val2_);
}

/****************************************************************************/
LongInt operator + (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) += Val2_);
}

/****************************************************************************/  
LongInt operator - (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) -= Val2_);
}

/****************************************************************************/  
LongInt operator * (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) *= Val2_);
}

/****************************************************************************/  
LongInt operator / (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) /= Val2_);
}

/****************************************************************************/  
LongInt operator % (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) %= Val2_);
}

/****************************************************************************/  
LongInt operator & (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) &= Val2_);
}

/****************************************************************************/  
LongInt operator | (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) |= Val2_);
}

/****************************************************************************/  
LongInt operator ^ (long Val1_, const LongInt& Val2_)
{
  return (LongInt(Val1_) ^= Val2_);
}

/****************************************************************************/
LongInt operator + (const LongInt& Val1_, long Val2_)
{
  return (LongInt(Val1_) += LongInt(Val2_));
}

/****************************************************************************/  
LongInt operator - (const LongInt& Val1_, long Val2_)
{
  LongInt Temp1_(Val1_);
  LongInt Temp2_(Val2_);
  return (Temp1_ -= Temp2_);
}

/****************************************************************************/  
LongInt operator * (const LongInt& Val1_, long Val2_)
{
  return (LongInt(Val1_) *= LongInt(Val2_));
}

/****************************************************************************/  
LongInt operator / (const LongInt& Val1_, long Val2_)
{
  LongInt Temp1_(Val1_);
  LongInt Temp2_(Val2_);
  return (Temp1_ /= Temp2_);
}

/****************************************************************************/  
LongInt operator % (const LongInt& Val1_, long Val2_)
{
  LongInt Temp1_(Val1_);
  LongInt Temp2_(Val2_);
  return (Temp1_ %= Temp2_);
}

/****************************************************************************/  
LongInt operator & (const LongInt& Val1_, long Val2_)
{
  return (LongInt(Val1_) &= LongInt(Val2_));
}

/****************************************************************************/  
LongInt operator | (const LongInt& Val1_, long Val2_)
{
  return (LongInt(Val1_) |= LongInt(Val2_));
}

/****************************************************************************/  
LongInt operator ^ (const LongInt& Val1_, long Val2_)
{
  return (LongInt(Val1_) ^= LongInt(Val2_));
}

/****************************************************************************/
/****************************************************************************/
Boolean LongIntAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
Boolean LongIntAcceptor::IsLongInt() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(LongIntAcceptor::LongIntAcceptor, LongInt)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(LongIntAcceptor::AcceptDataPtr, LongInt)
OBJACCEPTOR_GIVEDATA_TOPDEFN(LongIntAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongIntAcceptor::CreateFromLongInt, LongInt, LongInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongIntAcceptor::AssignFromLongInt, LongInt)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongIntAcceptor::IsLesserToLongInt, _ObjPtr->IsLesserToLongInt, LongInt)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongIntAcceptor::IsEqualToLongInt, _ObjPtr->IsEqualToLongInt, LongInt)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(LongIntAcceptor::IsGreaterToLongInt, _ObjPtr->IsGreaterToLongInt, LongInt)
MEMORYOPS_DEFN(LongIntAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(LongIntAcceptor::CreateFromShortInt, LongInt, ShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongIntAcceptor::AssignFromShortInt, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsLesserToShortInt, _ObjPtr->IsLesserToLongInt, LongInt, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsEqualToShortInt, _ObjPtr->IsEqualToLongInt, LongInt, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsGreaterToShortInt, _ObjPtr->IsGreaterToLongInt, LongInt, ShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongIntAcceptor::CreateFromInteger, LongInt, Integer)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongIntAcceptor::AssignFromInteger, Integer)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsLesserToInteger, _ObjPtr->IsLesserToLongInt, LongInt, Integer)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsEqualToInteger, _ObjPtr->IsEqualToLongInt, LongInt, Integer)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsGreaterToInteger, _ObjPtr->IsGreaterToLongInt, LongInt, Integer)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongIntAcceptor::CreateFromUShortInt, LongInt, UShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongIntAcceptor::AssignFromUShortInt, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsLesserToUShortInt, _ObjPtr->IsLesserToLongInt, LongInt, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsEqualToUShortInt, _ObjPtr->IsEqualToLongInt, LongInt, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsGreaterToUShortInt, _ObjPtr->IsGreaterToLongInt, LongInt, UShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongIntAcceptor::CreateFromUInteger, LongInt, UInteger)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongIntAcceptor::AssignFromUInteger, UInteger)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsLesserToUInteger, _ObjPtr->IsLesserToLongInt, LongInt, UInteger)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsEqualToUInteger, _ObjPtr->IsEqualToLongInt, LongInt, UInteger)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsGreaterToUInteger, _ObjPtr->IsGreaterToLongInt, LongInt, UInteger)
OBJACCEPTOR_CREATEFNC_TOPDEFN(LongIntAcceptor::CreateFromChrString, LongInt, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(LongIntAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToLongInt, LongInt, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToLongInt, LongInt, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(LongIntAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToLongInt, LongInt, ChrString)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(LongIntAcceptor::CreateFromLongNumber, LongInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(LongIntAcceptor::AssignFromLongNumber)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongIntAcceptor::IsLesserToLongNumber, _ObjPtr->IsLesserToLongInt, LongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongIntAcceptor::IsEqualToLongNumber, _ObjPtr->IsEqualToLongInt, LongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(LongIntAcceptor::IsGreaterToLongNumber, _ObjPtr->IsGreaterToLongInt, LongInt)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::CreateFromFloat, LongInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsLesserToFloat, _ObjPtr->IsLesserToLongInt, LongInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsEqualToFloat, _ObjPtr->IsEqualToLongInt, LongInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsGreaterToFloat, _ObjPtr->IsGreaterToLongInt, LongInt, FloatTypesPrecision::FLOAT)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::CreateFromDouble, LongInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsLesserToDouble, _ObjPtr->IsLesserToLongInt, LongInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsEqualToDouble, _ObjPtr->IsEqualToLongInt, LongInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsGreaterToDouble, _ObjPtr->IsGreaterToLongInt, LongInt, FloatTypesPrecision::DOUBLE)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::CreateFromLongDouble, LongInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsLesserToLongDouble, _ObjPtr->IsLesserToLongInt, LongInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsEqualToLongDouble, _ObjPtr->IsEqualToLongInt, LongInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(LongIntAcceptor::IsGreaterToLongDouble, _ObjPtr->IsGreaterToLongInt, LongInt, FloatTypesPrecision::LONGDOUBLE)

/****************************************************************************/
/****************************************************************************/
#if LONGINT_DEBUG
int main()
{
  ChrString Str_;

  LongInt si(31515012);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "31515012"));

  LongInt si2(31515012);
  LongInt si3(-21057012);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "-21057012"));  

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

  assert(si3 == 31515012);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2 = "568012";
  assert(si2 == 568012);

  ShortInt shi1_ = 21243;
  UShortInt ushi1_ = 41511;

  si2 = shi1_;
  assert(si2 == 21243);
  si2 = ushi1_;
  assert(si2 == 41511);

  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;

  // Testing bounds checking
  int OvFl_, UnFl_, DivZ_, NoErr_;
  LongInt chk1 = LONG_MAX;
  chk1 += 5;
  OvFl_ = chk1.Overflow();
  assert(OvFl_);
  chk1.ClearError();
  chk1 = LONG_MAX;
  chk1 = -chk1;
  --chk1;
  UnFl_ = chk1.Underflow();
  assert(!UnFl_);
  --chk1;
  UnFl_ = chk1.Underflow();
  assert(UnFl_);
  chk1.ClearError();
  chk1 = 65536;
  chk1 *= 32769;
  OvFl_ = chk1.Overflow();  
  assert(OvFl_);
  chk1 = 65536;
  chk1 >>= 2;
  assert(long(chk1) == 16384);
  chk1 <<= 4;
  assert(long(chk1) == 262144);
  chk1 |= LongInt(0x0F);
  assert(long(chk1) == 262159);
  chk1 /= LongInt(0);
  DivZ_ = chk1.DivideByZero();
  assert(DivZ_);
  OvFl_ = chk1.Overflow();
  assert(OvFl_);
  NoErr_ = chk1.NoError();
  assert(!NoErr_);

//  chk1.ClearError();
  try
  {
    chk1.ThrowError();
  }
  catch (const SciEngErr& ErrObj_)
  {
    cerr <<ErrObj_.message();
  }

  // Testing series comparison class on user defined type
  LongInt* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new LongInt(i+1);
  List_[10] = NULL;

  LongInt ZeroChar(0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::SumOfSignedSeriesCmp(LongInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<LongInt>::DifferenceOfSignedSeriesCmp(LongInt(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<LongInt>::DifferenceOfSignedSeriesCmp(LongInt(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<LongInt>::DifferenceOfSignedSeriesCmp(LongInt(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<LongInt> Pobj_ = SeriesCompare<LongInt>::ProductOfSignedSeriesCmp(LongInt(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<LongInt>::ProductOfSignedSeriesCmp(LongInt(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<LongInt>::ProductOfSignedSeriesCmp(LongInt(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing LongInt" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif






