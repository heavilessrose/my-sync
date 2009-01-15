#ifndef INTEGER_CPP
#define INTEGER_CPP
#ifndef INTEGER_H
  #include "integer.h"
#endif

/****************************************************************************/
Integer::Integer(int IntVal_):
IntegerAsBitsType<Integer, int>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(IntVal_)
{}

/****************************************************************************/
Integer::Integer(Uint UIntVal_, Boolean SetNull_):
IntegerAsBitsType<Integer, int>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Uvalue(UIntVal_)
{
  if (SetNull_)
    SetAsNullObject();
}

/****************************************************************************/
Integer::Integer(const ChrString& Str_, Boolean Signed_, Boolean Validity_):
IntegerAsBitsType<Integer, int>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Valid(Signed_ ? Integer::IsValue(Str_):Validity_)
{
  if (Signed_)
  {
    Byte_t Valid_ = _Valid;
    _Value = Valid_ ? int(Str_.strtol()):0;

    if (!Valid_)
    {
      SetAsNullObject();
      SetDataConversionError();
    }
  }
}

/****************************************************************************/
Integer::Integer(const Integer& IntObj_):
IntegerAsBitsType<Integer, int>(IntObj_),
_Value(IntObj_._Value)
{}

/****************************************************************************/
Integer::Integer(const ShortInt& IntObj_):
IntegerAsBitsType<Integer, int>(IntObj_.Options()),
_Value(IntObj_.IntValue())
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
Integer::Integer(const UShortInt& IntObj_):
IntegerAsBitsType<Integer, int>(IntObj_.Options()),
_Uvalue(IntObj_.UnsignedValue())
{}
#endif

/****************************************************************************/
Integer& Integer::DerivedClass()
{
  return *this;
}

/****************************************************************************/    
const Integer& Integer::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
Integer* Integer::Make(int IntVal_)
{
  return (new Integer(IntVal_));
}

/****************************************************************************/
Integer* Integer::Make(const Object& Obj_)
{
  IntegerAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& Integer::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  INTXMARKER("start: Integer::Assign(const Object&, ObjectAcceptor*)", cout)
  IntegerAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    INTXMARKER("end(1): Integer::Assign(const Object&, ObjectAcceptor*)", cout)
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  INTXMARKER("end(2): Integer::Assign(const Object&, ObjectAcceptor*)", cout)
  return *((Object*) (ObjComp_->AssignFromInteger(TrgComp_)));
}

/****************************************************************************/
Object* Integer::Clone(ObjectAcceptor* ObjComp_) const
{
  INTXMARKER("start: Integer::Clone(ObjectAcceptor*) const", cout)

  if (ObjComp_ == NULL)
  {
    INTXMARKER("end(1): Integer::Clone(ObjectAcceptor*) const", cout)
    return (new Integer(*this));
  }

  IntegerAcceptor TrgComp_(this);

  INTXMARKER("end(2): Integer::Clone(ObjectAcceptor*) const", cout)
  return ((Object*) (ObjComp_->CreateFromInteger(TrgComp_)));
}

/****************************************************************************/
Integer& Integer::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Integer _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& Integer::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Integer::GiveNullObject();
}

/****************************************************************************/
Object& Integer::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Integer::GiveNullObject();
}

/****************************************************************************/
const char* Integer::GiveHashableData() const
{
  return ((const char*)&_Value);
}

/****************************************************************************/
long Integer::GiveHashableDataLength() const
{
  return sizeof(_Value);
}

/****************************************************************************/
double Integer::FloatValue() const
{
  return _Value;
}

/****************************************************************************/
Boolean Integer::IsValue(const ChrString& Str_)
{
  return IsInt(Str_.c_str());
}

/****************************************************************************/
Integer Integer::ValueOf(const ChrString& Str_)
{
  return Integer(Str_);
}

/****************************************************************************/
ChrString Integer::ToString(const Integer& Int_, int Radix_)
{
  char Buffer_[16];
  return ChrString(IntToStr(Int_, Buffer_, Radix_));
}

/****************************************************************************/
ChrString Integer::ToString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(IntToStr(*this, Buffer_, Radix_));
}

/****************************************************************************/
ChrString Integer::ToTruncString(const Integer& Int_, int Radix_)
{
  char Buffer_[16];
  return ChrString(IntToStr(Int_, Buffer_, Radix_));
}

/****************************************************************************/
ChrString Integer::ToTruncString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(IntToStr(*this, Buffer_, Radix_));
}

/****************************************************************************/
Integer& Integer::FromString(const ChrString& Str_)
{
  Integer Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
const int& Integer::GetData() const
{
  return _Value;
}

/****************************************************************************/
void Integer::SetData(const int& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
int Integer::ClassID() const
{
  INTXMARKER("start/end: Integer::ClassID() const", cout)
  return TypeInfo::INTEGER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* Integer::GiveObjectAcceptor() const
{
  return (new IntegerAcceptor(this));
}

/****************************************************************************/
Boolean Integer::IsEqualToInteger(const Integer* Ptr_) const
{
  return (_Value == Ptr_->_Value);
}

/****************************************************************************/
Boolean Integer::IsLesserToInteger(const Integer* Ptr_) const
{
  return (_Value < Ptr_->_Value);
}

/****************************************************************************/
Boolean Integer::IsGreaterToInteger(const Integer* Ptr_) const
{
  return (_Value > Ptr_->_Value);
}

/****************************************************************************/
Boolean Integer::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  IntegerAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToInteger(TrgComp_);
}

/****************************************************************************/
Boolean Integer::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  IntegerAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToInteger(TrgComp_);
}

/****************************************************************************/
Boolean Integer::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  IntegerAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToInteger(TrgComp_);
}

/****************************************************************************/
istream& Integer::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& Integer::BinaryRead(istream& Is_, Boolean* Ok_)
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
ostream& Integer::BinaryWrite(ostream& Os_, Boolean* Ok_) const
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
ByteStream& Integer::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& Integer::BinaryRead(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& Integer::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
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
istream& Integer::TextRead(istream& Is_, Boolean* Ok_)
{
  int Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& Integer::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Value;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long Integer::DataSize() const
{
  INTXMARKER("start/end: Integer::DataSize() const", cout)
  return sizeof(int);
}

/****************************************************************************/
Boolean Integer::AtZero() const
{
  return (_Value == 0);
}

/****************************************************************************/
Boolean Integer::AtMaxValue() const
{
  return (_Value == INT_MAX);
}

/****************************************************************************/
Boolean Integer::AtMinValue() const
{
  return (_Value == INT_MIN);
}

/****************************************************************************/
Integer& Integer::operator = (const int& IntVal_)
{
  _Value = IntVal_;
  return *this;
}

/****************************************************************************/
Integer& Integer::operator = (const ChrString& Str_)
{
  Integer Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
Integer& Integer::operator = (const ShortInt& Obj_)
{
  Integer Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
void Integer::AssignUShortInt(const UShortInt& Obj_)
{
  Integer Sobj_(Obj_);
  (*this = Sobj_);
}

/****************************************************************************/
Integer& Integer::operator = (const Integer& Obj_)
{
  _Value = Obj_._Value;
  IntegerAsBitsType<Integer, int>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
Boolean Integer::operator ! () const
{
  return !_Value;
}

/****************************************************************************/
Integer Integer::operator - () const
{
  return Integer(-_Value);
}

/****************************************************************************/
Integer Integer::operator + () const
{
  return Integer(*this);
}

/****************************************************************************/
const Integer& Integer::operator ++ ()
{
  if (BoundsChecking() && _Value == INT_MAX)
    SetOverflow();

  ++_Value;
  return *this;
}

/****************************************************************************/
const Integer& Integer::operator ++ (int)
{
  static Integer OldVal_ = _Value;

  if (BoundsChecking() && _Value == INT_MAX)
    SetOverflow();
  
  ++_Value;
  return OldVal_;
}

/****************************************************************************/
const Integer& Integer::operator -- ()
{
  if (BoundsChecking() && _Value == INT_MIN)
    SetUnderflow();

  --_Value;
  return *this;
}

/****************************************************************************/
const Integer& Integer::operator -- (int)
{
  static Integer OldVal_ = _Value;

  if (BoundsChecking() && _Value == INT_MIN)
    SetUnderflow();
  
  --_Value;
  return OldVal_;
}

/****************************************************************************/
Integer& Integer::operator += (const NumOrdinalType<Integer>& Obj_)
{
  if (BoundsChecking())
  {
    int Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = int(Obj_.DerivedClass());

    intSeriesCompare CeilCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MAX, Buffer_, 2, FALSE);
    intSeriesCompare FloorCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value += Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Integer& Integer::operator -= (const NumOrdinalType<Integer>& Obj_)
{
  if (BoundsChecking())
  {
    int Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = int(Obj_.DerivedClass());

    intSeriesCompare CeilCmp_ = intSeriesCompare::DifferenceOfSeriesCmp(INT_MAX, Buffer_, 2, FALSE);
    intSeriesCompare FloorCmp_ = intSeriesCompare::DifferenceOfSeriesCmp(INT_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value -= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Integer& Integer::operator *= (const NumOrdinalType<Integer>& Obj_)
{
  if (BoundsChecking())
  {
    int Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = int(Obj_.DerivedClass());

    intSeriesCompare CeilCmp_ = intSeriesCompare::ProductOfSeriesCmp(INT_MAX, Buffer_, 2, FALSE);
    intSeriesCompare FloorCmp_ = intSeriesCompare::ProductOfSeriesCmp(INT_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value *= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Integer& Integer::operator /= (const NumOrdinalType<Integer>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value /= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Integer& Integer::operator %= (const NumOrdinalType<Integer>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value %= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Integer& Integer::operator += (const int& IntVal_)
{
  return (*this += Integer(IntVal_));
}

/****************************************************************************/    
Integer& Integer::operator -= (const int& IntVal_)
{
  return (*this -= Integer(IntVal_));
}

/****************************************************************************/    
Integer& Integer::operator *= (const int& IntVal_)
{
  return (*this *= Integer(IntVal_));
}

/****************************************************************************/    
Integer& Integer::operator /= (const int& IntVal_)
{
  return (*this /= Integer(IntVal_));
}

/****************************************************************************/    
Integer& Integer::operator %= (const int& IntVal_)
{
  return (*this %= Integer(IntVal_));
}

/****************************************************************************/
Integer Integer::operator ~ () const
{
  return Integer(~_Value);
}

/****************************************************************************/
Integer& Integer::operator |= (const NumOrdinalAsBitsType<Integer>& Obj_)
{
  _Value |= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Integer& Integer::operator &= (const NumOrdinalAsBitsType<Integer>& Obj_)
{
  _Value &= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Integer& Integer::operator ^= (const NumOrdinalAsBitsType<Integer>& Obj_)
{
  _Value ^= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Integer& Integer::operator |= (const int& IntVal_)
{
  return (*this |= Integer(IntVal_));
}

/****************************************************************************/    
Integer& Integer::operator &= (const int& IntVal_)
{
  return (*this &= Integer(IntVal_));
}

/****************************************************************************/    
Integer& Integer::operator ^= (const int& IntVal_)
{
  return (*this ^= Integer(IntVal_));
}

/****************************************************************************/
Integer Integer::operator << (size_t Shift_) const
{
  return (Integer(*this) <<= Shift_);
}

/****************************************************************************/    
Integer Integer::operator >> (size_t Shift_) const
{
  return (Integer(*this) >>= Shift_);
}

/****************************************************************************/
Integer& Integer::operator <<= (size_t Shift_)
{
  _Value <<= Shift_;
  return *this;
}

/****************************************************************************/
Integer& Integer::operator >>= (size_t Shift_)
{
  _Value >>= Shift_;
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* Integer::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void Integer::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* Integer::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void Integer::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
Integer operator + (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) += Val2_);
}

/****************************************************************************/  
Integer operator - (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) -= Val2_);
}

/****************************************************************************/  
Integer operator * (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) *= Val2_);
}

/****************************************************************************/  
Integer operator / (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) /= Val2_);
}

/****************************************************************************/  
Integer operator % (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) %= Val2_);
}

/****************************************************************************/
Integer operator & (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) &= Val2_);
}

/****************************************************************************/  
Integer operator | (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) |= Val2_);
}

/****************************************************************************/  
Integer operator ^ (const Integer& Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) ^= Val2_);
}

/****************************************************************************/
Integer operator + (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) += Val2_);
}

/****************************************************************************/  
Integer operator - (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) -= Val2_);
}

/****************************************************************************/  
Integer operator * (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) *= Val2_);
}

/****************************************************************************/  
Integer operator / (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) /= Val2_);
}

/****************************************************************************/  
Integer operator % (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) %= Val2_);
}

/****************************************************************************/  
Integer operator & (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) &= Val2_);
}

/****************************************************************************/  
Integer operator | (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) |= Val2_);
}

/****************************************************************************/  
Integer operator ^ (int Val1_, const Integer& Val2_)
{
  return (Integer(Val1_) ^= Val2_);
}

/****************************************************************************/
Integer operator + (const Integer& Val1_, int Val2_)
{
  return (Integer(Val1_) += Integer(Val2_));
}

/****************************************************************************/  
Integer operator - (const Integer& Val1_, int Val2_)
{
  Integer Temp1_(Val1_);
  Integer Temp2_(Val2_);
  return (Temp1_ -= Temp2_);
}

/****************************************************************************/  
Integer operator * (const Integer& Val1_, int Val2_)
{
  return (Integer(Val1_) *= Integer(Val2_));
}

/****************************************************************************/  
Integer operator / (const Integer& Val1_, int Val2_)
{
  Integer Temp1_(Val1_);
  Integer Temp2_(Val2_);
  return (Temp1_ /= Temp2_);
}

/****************************************************************************/  
Integer operator % (const Integer& Val1_, int Val2_)
{
  Integer Temp1_(Val1_);
  Integer Temp2_(Val2_);
  return (Temp1_ %= Temp2_);
}

/****************************************************************************/  
Integer operator & (const Integer& Val1_, int Val2_)
{
  return (Integer(Val1_) &= Integer(Val2_));
}

/****************************************************************************/  
Integer operator | (const Integer& Val1_, int Val2_)
{
  return (Integer(Val1_) |= Integer(Val2_));
}

/****************************************************************************/  
Integer operator ^ (const Integer& Val1_, int Val2_)
{
  return (Integer(Val1_) ^= Integer(Val2_));
}

/****************************************************************************/
/****************************************************************************/
Boolean IntegerAcceptor::IsInteger() const
{
  return TRUE;
}

/****************************************************************************/
Boolean IntegerAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(IntegerAcceptor::IntegerAcceptor, Integer)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(IntegerAcceptor::AcceptDataPtr, Integer)
OBJACCEPTOR_GIVEDATA_TOPDEFN(IntegerAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(IntegerAcceptor::CreateFromInteger, Integer, Integer)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(IntegerAcceptor::AssignFromInteger, Integer)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(IntegerAcceptor::IsLesserToInteger, _ObjPtr->IsLesserToInteger, Integer)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(IntegerAcceptor::IsEqualToInteger, _ObjPtr->IsEqualToInteger, Integer)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(IntegerAcceptor::IsGreaterToInteger, _ObjPtr->IsGreaterToInteger, Integer)
MEMORYOPS_DEFN(IntegerAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(IntegerAcceptor::CreateFromShortInt, Integer, ShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(IntegerAcceptor::AssignFromShortInt, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsLesserToShortInt, _ObjPtr->IsLesserToInteger, Integer, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsEqualToShortInt, _ObjPtr->IsEqualToInteger, Integer, ShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsGreaterToShortInt, _ObjPtr->IsGreaterToInteger, Integer, ShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(IntegerAcceptor::CreateFromUShortInt, Integer, UShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(IntegerAcceptor::AssignFromUShortInt, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsLesserToUShortInt, _ObjPtr->IsLesserToInteger, Integer, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsEqualToUShortInt, _ObjPtr->IsEqualToInteger, Integer, UShortInt)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsGreaterToUShortInt, _ObjPtr->IsGreaterToInteger, Integer, UShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(IntegerAcceptor::CreateFromChrString, Integer, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(IntegerAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToInteger, Integer, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToInteger, Integer, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(IntegerAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToInteger, Integer, ChrString)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::CreateFromLongNumber, Integer)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::AssignFromLongNumber)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsLesserToLongNumber, _ObjPtr->IsLesserToInteger, Integer)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsEqualToLongNumber, _ObjPtr->IsEqualToInteger, Integer)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsGreaterToLongNumber, _ObjPtr->IsGreaterToInteger, Integer)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::CreateFromULongInt, Integer)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::AssignFromULongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsLesserToULongInt, _ObjPtr->IsLesserToInteger, Integer)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsEqualToULongInt, _ObjPtr->IsEqualToInteger, Integer)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsGreaterToULongInt, _ObjPtr->IsGreaterToInteger, Integer)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::CreateFromLongInt, Integer)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::AssignFromLongInt)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsLesserToLongInt, _ObjPtr->IsLesserToInteger, Integer)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsEqualToLongInt, _ObjPtr->IsEqualToInteger, Integer)
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(IntegerAcceptor::IsGreaterToLongInt, _ObjPtr->IsGreaterToInteger, Integer)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::CreateFromFloat, Integer, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsLesserToFloat, _ObjPtr->IsLesserToInteger, Integer, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsEqualToFloat, _ObjPtr->IsEqualToInteger, Integer, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsGreaterToFloat, _ObjPtr->IsGreaterToInteger, Integer, FloatTypesPrecision::FLOAT)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::CreateFromDouble, Integer, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsLesserToDouble, _ObjPtr->IsLesserToInteger, Integer, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsEqualToDouble, _ObjPtr->IsEqualToInteger, Integer, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsGreaterToDouble, _ObjPtr->IsGreaterToInteger, Integer, FloatTypesPrecision::DOUBLE)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::CreateFromLongDouble, Integer, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsLesserToLongDouble, _ObjPtr->IsLesserToInteger, Integer, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsEqualToLongDouble, _ObjPtr->IsEqualToInteger, Integer, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(IntegerAcceptor::IsGreaterToLongDouble, _ObjPtr->IsGreaterToInteger, Integer, FloatTypesPrecision::LONGDOUBLE)

/****************************************************************************/
/****************************************************************************/
#if INTEGER_DEBUG1
int IntegerAction::_Init = 0;

void* IntegerRunnable::Run(void* Ptr_)
{
	IntegerData* _DataPtr = (IntegerData*)Ptr_;
	_Action.Execute(_DataPtr);
	return NULL;
}

void IntegerAction::Execute(IntegerData* DataPtr_)
{
	EnterCriticalSection(GiveMutex());

	Integer* _Data;
	if (DataPtr_)	
	  if (DataPtr_->_Type == IntegerData::SHORT_VALUE)
		_Data = Integer::Make(DataPtr_->_ValData);
	  else
		_Data = Integer::Make(*DataPtr_->_ObjData);

	size_t i; 
	for (i = 1; i < 10; i *= 2)
	{
	  *_Data += i;
	  cout <<(*_Data) <<endl;
	}

	LeaveCriticalSection(GiveMutex());
}
#endif

#if INTEGER_DEBUG
int main()
{
#if INTEGER_DEBUG1
  IntegerRunnable sir1;
//  IntegerRunnable sir2;
  IntegerRunnable sir3;
  IntegerRunnable sir4;

  IntegerData sid1;
//  IntegerData sid2;
  IntegerData sid3;
//  ChrString str2("2000");
  Integer si3("3000");
  IntegerData sid4;
  Integer si4("4000");

  sid1._Type = IntegerData::INT_VALUE;
  sid1._ValData = 1000;
//  sid2._Type = IntegerData::STRING_OBJECT;
//  sid2._ObjData = &str2;
  sid3._Type = IntegerData::INTEGER_OBJECT;
  sid3._ObjData = &si3;
  sid4._Type = IntegerData::INTEGER_OBJECT;
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

#if INTEGER_DEBUG2
  ChrString Str_;

  Integer si(31515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "31515"));

  Integer si2(31515);
  Integer si3(-21057);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "-21057"));

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

  assert(si3 == 31515);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  Integer sint1("-32090");
  si2.Assign(sint1);
  assert(si2 = -32090);

  si2 = "34000";
  assert(si2 == 34000);

  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;

  // Testing bounds checking
  Integer chk1 = INT_MAX;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = INT_MAX;
  chk1 = -chk1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 65536;
  chk1 = chk1 * 32768;
  assert(chk1.Overflow());
  chk1 = 65536;
  chk1 >>= 2;
  assert(int(chk1) == 16384);
  chk1 <<= 4;
  assert(int(chk1) == 262144);
  chk1 |= Integer(0x0F);
  assert(int(chk1) == 262159);
  chk1 = chk1 / 0;
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

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
  Integer* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new Integer(i+1);
  List_[10] = NULL;

  Integer ZeroChar(0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::SumOfSignedSeriesCmp(Integer(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Integer>::DifferenceOfSignedSeriesCmp(Integer(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Integer>::DifferenceOfSignedSeriesCmp(Integer(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Integer>::DifferenceOfSignedSeriesCmp(Integer(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<Integer> Pobj_ = SeriesCompare<Integer>::ProductOfSignedSeriesCmp(Integer(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<Integer>::ProductOfSignedSeriesCmp(Integer(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<Integer>::ProductOfSignedSeriesCmp(Integer(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing Integer" <<endl;
#endif

#if INTEGER_DEBUG3
  Integer si1(2001);

  cout <<AssociatedIndex(si1, 2000) <<endl;
  ShowInfo((ostream&)cout, si1);
  cout <<endl <<SizeOfData(si1) <<endl;
  cout <<FormWidthOfObject(si1) <<endl;
  cout <<WhatIs(si1) <<endl;
  Integer* siobjp = (Integer*)Clone(si1);
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

  Integer si2;
  ifstream fin("testing.bin");
  Read(fin, si2);
  fin.close();

  assert(si2 == 1001);
  cout <<si2 <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif





