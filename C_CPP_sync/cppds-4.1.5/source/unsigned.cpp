#ifndef UNSIGNED_CPP
#define UNSIGNED_CPP
#ifndef UNSIGNED_H
  #include "unsigned.h"
#endif

/****************************************************************************/
UInteger::UInteger(Uint IntVal_):
Integer(IntVal_, 0)
{}

/****************************************************************************/
UInteger::UInteger(const ChrString& Str_):
Integer(Str_, FALSE, UInteger::IsValue(Str_))
{
  Byte_t Valid_ = _Valid;
  _Uvalue = Valid_ ? Uint(Str_.strtoul()):0U;

  if (!Valid_)
  {
    SetAsNullObject();
    SetDataConversionError();
  }
}

/****************************************************************************/
UInteger::UInteger(const Integer& IntObj_):
Integer(IntObj_)
{}

/****************************************************************************/
UInteger::UInteger(const ShortInt& IntObj_):
Integer(IntObj_)
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
UInteger::UInteger(const UInteger& IntObj_):
Integer(IntObj_)
{}

/****************************************************************************/
UInteger::UInteger(const UShortInt& IntObj_):
Integer(IntObj_)
{}
#endif
/****************************************************************************/
UInteger* UInteger::Make(Uint IntVal_)
{
  return (new UInteger(IntVal_));
}

/****************************************************************************/
UInteger* UInteger::Make(const Object& Obj_)
{
  UIntegerAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return ((UInteger*) TrgComp_._VarPtr);
}

/****************************************************************************/
Object* UInteger::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return ((Object*) (new UInteger(*this)));

  UIntegerAcceptor TrgComp_(this);
  return ((Object*) ObjComp_->CreateFromUInteger(TrgComp_));
}

/****************************************************************************/
Object& UInteger::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  UIntegerAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromUInteger(TrgComp_)));
}

/****************************************************************************/
UInteger& UInteger::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static UInteger _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& UInteger::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return UInteger::GiveNullObject();
}

/****************************************************************************/
Object& UInteger::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return UInteger::GiveNullObject();
}

/****************************************************************************/
double UInteger::FloatValue() const
{
  return _Uvalue;
}

/****************************************************************************/
Boolean UInteger::IsValue(const ChrString& Str_)
{
  return IsUint(Str_.c_str());
}

/****************************************************************************/
UInteger UInteger::ValueOf(const ChrString& Str_)
{
  return UInteger(Str_);
}

/****************************************************************************/
ChrString UInteger::ToString(const Integer& Int_, int Radix_)
{
  char Buffer_[16];

  UInteger UInt_(Int_);
  return ChrString(UintToStr(Uint(UInt_), Buffer_, Radix_));
}

/****************************************************************************/
ChrString UInteger::ToString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(UintToStr(Uint(*this), Buffer_, Radix_));
}

/****************************************************************************/
ChrString UInteger::ToTruncString(const Integer& Int_, int Radix_)
{
  char Buffer_[16];

  UInteger UInt_(Int_);
  return ChrString(UintToStr(Uint(UInt_), Buffer_, Radix_));
}

/****************************************************************************/
ChrString UInteger::ToTruncString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(UintToStr(Uint(*this), Buffer_, Radix_));
}

/****************************************************************************/
Integer& UInteger::FromString(const ChrString& Str_)
{
  UInteger Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
int UInteger::ClassID() const
{
  return TypeInfo::UINTEGER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* UInteger::GiveObjectAcceptor() const
{
  return (new UIntegerAcceptor(this));
}

/****************************************************************************/
Boolean UInteger::IsLesserToInteger(const Integer* Ptr_) const
{
  return (_Uvalue < Ptr_->UnsignedValue());
}

/****************************************************************************/
Boolean UInteger::IsGreaterToInteger(const Integer* Ptr_) const
{
  return (_Uvalue > Ptr_->UnsignedValue());
}

/****************************************************************************/
istream& UInteger::TextRead(istream& Is_, Boolean* Ok_)
{
  Uint Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& UInteger::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Uvalue;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
Boolean UInteger::AtMaxValue() const
{
  return (_Uvalue == UINT_MAX);
}

/****************************************************************************/
Boolean UInteger::AtMinValue() const
{
  return (_Uvalue == 0);
}

/****************************************************************************/
Integer& UInteger::operator = (const int& IntVal_)
{
  _Uvalue = Uint(IntVal_);
  return *this;
}

/****************************************************************************/
Integer& UInteger::operator = (const ChrString& Str_)
{
  UInteger Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
Integer& UInteger::operator = (const ShortInt& Obj_)
{
  // debug
  cout <<Obj_ <<endl;

  UInteger Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
Integer& UInteger::operator = (const UShortInt& Obj_)
{
  UInteger Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
Integer& UInteger::operator = (const Integer& Obj_)
{
  Integer::operator = (Obj_);
  return *this;
}

/****************************************************************************/
Integer& UInteger::operator = (const UInteger& Obj_)
{
  Integer::operator = (Obj_);
  return *this;
}

/****************************************************************************/
UInteger UInteger::operator + () const
{
  return UInteger(*this);
}

/****************************************************************************/
UInteger UInteger::operator ~ () const
{
  return UInteger(~_Uvalue);
}

/****************************************************************************/
const Integer& UInteger::operator ++ ()
{
  if (BoundsChecking() && _Uvalue == UINT_MAX)
    SetOverflow();

  ++_Uvalue;
  return *this;
}

/****************************************************************************/
const Integer& UInteger::operator ++ (int)
{
  static UInteger OldVal_ = _Uvalue;

  if (BoundsChecking() && _Uvalue == UINT_MAX)
    SetOverflow();
  
  ++_Uvalue;
  return OldVal_;
}

/****************************************************************************/
const Integer& UInteger::operator -- ()
{
  if (BoundsChecking() && _Uvalue == 0)
    SetUnderflow();

  --_Uvalue;
  return *this;
}

/****************************************************************************/
const Integer& UInteger::operator -- (int)
{
  static UInteger OldVal_ = _Uvalue;

  if (BoundsChecking() && _Uvalue == 0)
    SetUnderflow();
  
  --_Uvalue;
  return OldVal_;
}

/****************************************************************************/
Integer& UInteger::operator += (const NumOrdinalType<Integer>& Obj_)
{
  if (BoundsChecking())
  {
    Uint Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UintSeriesCompare CeilCmp_ = UintSeriesCompare::SumOfSeriesCmp(UINT_MAX, Buffer_, 2, FALSE);
    UintSeriesCompare FloorCmp_ = UintSeriesCompare::SumOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue += Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
Integer& UInteger::operator -= (const NumOrdinalType<Integer>& Obj_)
{
  if (BoundsChecking())
  {
    Uint Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UintSeriesCompare CeilCmp_ = UintSeriesCompare::DifferenceOfSeriesCmp(UINT_MAX, Buffer_, 2, FALSE);
    UintSeriesCompare FloorCmp_ = UintSeriesCompare::DifferenceOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue -= Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
Integer& UInteger::operator *= (const NumOrdinalType<Integer>& Obj_)
{
  if (BoundsChecking())
  {
    Uint Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UintSeriesCompare CeilCmp_ = UintSeriesCompare::ProductOfSeriesCmp(UINT_MAX, Buffer_, 2, FALSE);
    UintSeriesCompare FloorCmp_ = UintSeriesCompare::ProductOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue *= Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
Integer& UInteger::operator /= (const NumOrdinalType<Integer>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Uvalue /= Obj_.DerivedClass().UnsignedValue();

  return *this;
}

/****************************************************************************/
Integer& UInteger::operator %= (const NumOrdinalType<Integer>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Uvalue %= Obj_.DerivedClass().UnsignedValue();

  return *this;
}

/****************************************************************************/
Integer& UInteger::operator += (const int& IntVal_)
{
  return (*this += UInteger(IntVal_));
}

/****************************************************************************/    
Integer& UInteger::operator -= (const int& IntVal_)
{
  return (*this -= UInteger(IntVal_));
}

/****************************************************************************/    
Integer& UInteger::operator *= (const int& IntVal_)
{
  return (*this *= UInteger(IntVal_));
}

/****************************************************************************/    
Integer& UInteger::operator /= (const int& IntVal_)
{
  return (*this /= UInteger(IntVal_));
}

/****************************************************************************/    
Integer& UInteger::operator %= (const int& IntVal_)
{
  return (*this %= UInteger(IntVal_));
}

/****************************************************************************/
#if OVERLOAD_NEW
void* UInteger::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void UInteger::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* UInteger::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void UInteger::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
UInteger operator + (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) += Val2_);
}

/****************************************************************************/  
UInteger operator - (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) -= Val2_);
}

/****************************************************************************/  
UInteger operator * (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) *= Val2_);
}

/****************************************************************************/  
UInteger operator / (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) /= Val2_);
}

/****************************************************************************/  
UInteger operator % (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) %= Val2_);
}

/****************************************************************************/
UInteger operator & (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) &= Val2_);
}

/****************************************************************************/  
UInteger operator | (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) |= Val2_);
}

/****************************************************************************/  
UInteger operator ^ (const UInteger& Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) ^= Val2_);
}

/****************************************************************************/
UInteger operator + (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) += Val2_);
}

/****************************************************************************/  
UInteger operator - (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) -= Val2_);
}

/****************************************************************************/  
UInteger operator * (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) *= Val2_);
}

/****************************************************************************/  
UInteger operator / (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) /= Val2_);
}

/****************************************************************************/  
UInteger operator % (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) %= Val2_);
}

/****************************************************************************/  
UInteger operator & (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) &= Val2_);
}

/****************************************************************************/  
UInteger operator | (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) |= Val2_);
}

/****************************************************************************/  
UInteger operator ^ (Uint Val1_, const UInteger& Val2_)
{
  return (UInteger(Val1_) ^= Val2_);
}

/****************************************************************************/
UInteger operator + (const UInteger& Val1_, Uint Val2_)
{
  return (UInteger(Val1_) += UInteger(Val2_));
}

/****************************************************************************/  
UInteger operator - (const UInteger& Val1_, Uint Val2_)
{
  UInteger Temp1_(Val1_);
  UInteger Temp2_(Val2_);
  return (Temp1_ -= Temp2_);
}

/****************************************************************************/  
UInteger operator * (const UInteger& Val1_, Uint Val2_)
{
  return (UInteger(Val1_) *= UInteger(Val2_));
}

/****************************************************************************/  
UInteger operator / (const UInteger& Val1_, Uint Val2_)
{
  UInteger Temp1_(Val1_);
  UInteger Temp2_(Val2_);
  return (Temp1_ /= Temp2_);
}

/****************************************************************************/  
UInteger operator % (const UInteger& Val1_, Uint Val2_)
{
  UInteger Temp1_(Val1_);
  UInteger Temp2_(Val2_);
  return (Temp1_ %= Temp2_);
}

/****************************************************************************/  
UInteger operator & (const UInteger& Val1_, Uint Val2_)
{
  return (UInteger(Val1_) &= UInteger(Val2_));
}

/****************************************************************************/  
UInteger operator | (const UInteger& Val1_, Uint Val2_)
{
  return (UInteger(Val1_) |= UInteger(Val2_));
}

/****************************************************************************/  
UInteger operator ^ (const UInteger& Val1_, Uint Val2_)
{
  return (UInteger(Val1_) ^= UInteger(Val2_));
}

/****************************************************************************/
/****************************************************************************/
UIntegerAcceptor::UIntegerAcceptor(const UInteger* Ptr_):
IntegerAcceptor(Ptr_)
{}

/****************************************************************************/
Boolean UIntegerAcceptor::IsUInteger() const
{
  return TRUE;
}

/****************************************************************************/
Boolean UIntegerAcceptor::IsInteger() const
{
  return TRUE;
}

/****************************************************************************/
void* UIntegerAcceptor::AssignFromUShortInt(const ObjectAcceptor& Obj_)
{
  const UShortInt* TrgPtr_ = (const UShortInt*)Obj_.GiveDataPtr();
  _VarPtr->AssignUShortInt(*TrgPtr_);
  return _VarPtr;
}

/****************************************************************************/
OBJACCEPTOR_CREATEFNC_TOPDEFN(UIntegerAcceptor::CreateFromChrString, UInteger, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(UIntegerAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_CREATEFNC_TOPDEFN(UIntegerAcceptor::CreateFromShortInt, UInteger, ShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(UIntegerAcceptor::AssignFromShortInt, ShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(UIntegerAcceptor::CreateFromInteger, UInteger, Integer)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(UIntegerAcceptor::AssignFromInteger, Integer)
OBJACCEPTOR_CREATEFNC_TOPDEFN(UIntegerAcceptor::CreateFromUShortInt, UInteger, UShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(UIntegerAcceptor::CreateFromUInteger, UInteger, UInteger)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(UIntegerAcceptor::AssignFromUInteger, UInteger)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UIntegerAcceptor::CreateFromLongNumber, UInteger)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UIntegerAcceptor::AssignFromLongNumber)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UIntegerAcceptor::CreateFromLongInt, UInteger)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UIntegerAcceptor::AssignFromLongInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UIntegerAcceptor::CreateFromULongInt, UInteger)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UIntegerAcceptor::AssignFromULongInt)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(UIntegerAcceptor::CreateFromFloat, UInteger, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(UIntegerAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(UIntegerAcceptor::CreateFromDouble, UInteger, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(UIntegerAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(UIntegerAcceptor::CreateFromLongDouble, UInteger, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(UIntegerAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)

MEMORYOPS_DEFN(UIntegerAcceptor)
/****************************************************************************/
/****************************************************************************/
#if UNSIGNED_DEBUG
int main()
{
  ChrString Str_;

  UInteger si(61515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "61515"));

  UInteger si2(61515);
  UInteger si3(41057);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "41057"));

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

  assert(si3 == UInteger(61515));

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2.ClearError();
  UShortInt usint("56800");
  cout <<usint <<endl;
  si2.Assign(usint);
  cout <<si2 <<endl;  
  assert(si2 == UInteger(56800));

  si2 = "56800111";
  assert(si2 = 56800111);

  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;

  // Testing bounds checking
  UInteger chk1 = UINT_MAX;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 65536;
  chk1 *= chk1;
  assert(chk1.Overflow());
  chk1 = 65536;
  chk1 >>= 2;
  assert(int(chk1) == 16384);
  chk1 <<= 4;
  assert(int(chk1) == 262144);
  chk1 = chk1 | UInteger(0x0F);
  assert(int(chk1) == 262159);
  chk1 = chk1 / UInteger(0);
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
  UInteger* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new UInteger(i+1);
  List_[10] = NULL;

  UInteger ZeroChar(0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(11), List_, ZeroChar, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(55), List_, ZeroChar, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(77), List_, ZeroChar, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<UInteger>::SumOfUnsignedSeriesCmp(UInteger(1), List_, ZeroChar, CHAR_MAX, TRUE).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<UInteger> Pobj_ = SeriesCompare<UInteger>::ProductOfUnsignedSeriesCmp(UInteger(120), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<UInteger>::ProductOfUnsignedSeriesCmp(UInteger(121), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<UInteger>::ProductOfUnsignedSeriesCmp(UInteger(119), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing UInteger" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





