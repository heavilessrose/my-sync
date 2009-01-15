#ifndef ULONGINT_CPP
#define ULONGINT_CPP
#ifndef ULONGINT_H
  #include "ulongint.h"
#endif

/****************************************************************************/
ULongInt::ULongInt(Ulong IntVal_):
LongInt(IntVal_, 0)
{}

/****************************************************************************/
ULongInt::ULongInt(const ChrString& Str_):
LongInt(Str_, FALSE, ULongInt::IsValue(Str_))
{
  Byte_t Valid_ = _Valid;
  _Uvalue = Valid_ ? Str_.strtoul():0UL;

  if (!Valid_)
  {
    SetAsNullObject();
    SetDataConversionError();
  }    
}

/****************************************************************************/
ULongInt::ULongInt(const ShortInt& IntObj_):
LongInt(IntObj_)
{}

/****************************************************************************/
ULongInt::ULongInt(const LongInt& IntObj_):
LongInt(IntObj_)
{}

/****************************************************************************/
ULongInt::ULongInt(const Integer& IntObj_):
LongInt(IntObj_)
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
ULongInt::ULongInt(const UShortInt& IntObj_):
LongInt(IntObj_)
{}
/****************************************************************************/
ULongInt::ULongInt(const ULongInt& IntObj_):
LongInt(IntObj_)
{}
/****************************************************************************/
ULongInt::ULongInt(const UInteger& IntObj_):
LongInt(IntObj_)
{}
#endif
/****************************************************************************/
ULongInt* ULongInt::Make(Ulong IntVal_)
{
  return (new ULongInt(IntVal_));
}

/****************************************************************************/
ULongInt* ULongInt::Make(const Object& Obj_)
{
  ULongIntAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return ((ULongInt*) TrgComp_._VarPtr);
}

/****************************************************************************/
Object* ULongInt::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return ((Object*) (new ULongInt(*this)));

  ULongIntAcceptor TrgComp_(this);
  return ((Object*) ObjComp_->CreateFromULongInt(TrgComp_));
}

/****************************************************************************/
Object& ULongInt::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  ULongIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromULongInt(TrgComp_)));
}

/****************************************************************************/
double ULongInt::FloatValue() const
{
  return _Uvalue;
}

/****************************************************************************/
Boolean ULongInt::IsValue(const ChrString& Str_)
{
  return IsUlong(Str_.c_str());
}

/****************************************************************************/
ULongInt ULongInt::ValueOf(const ChrString& Str_)
{
  return ULongInt(Str_);
}

/****************************************************************************/
ChrString ULongInt::ToString(const LongInt& Int_, int Radix_)
{
  char Buffer_[16];

  ULongInt UInt_(Int_);
  return ChrString(UlongToStr(Ulong(UInt_), Buffer_, Radix_));
}

/****************************************************************************/
ChrString ULongInt::ToString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(UlongToStr(Ulong(*this), Buffer_, Radix_));
}

/****************************************************************************/
ChrString ULongInt::ToTruncString(const LongInt& Int_, int Radix_)
{
  char Buffer_[16];

  ULongInt UInt_(Int_);
  return ChrString(UlongToStr(Ulong(UInt_), Buffer_, Radix_));
}

/****************************************************************************/
ChrString ULongInt::ToTruncString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(UlongToStr(Ulong(*this), Buffer_, Radix_));
}

/****************************************************************************/
LongInt& ULongInt::FromString(const ChrString& Str_)
{
  ULongInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
int ULongInt::ClassID() const
{
  return TypeInfo::ULONGINT_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* ULongInt::GiveObjectAcceptor() const
{
  return (new ULongIntAcceptor(this));
}

/****************************************************************************/
ULongInt& ULongInt::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static ULongInt _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& ULongInt::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ULongInt::GiveNullObject();
}

/****************************************************************************/
Object& ULongInt::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ULongInt::GiveNullObject();
}

/****************************************************************************/
Boolean ULongInt::IsLesserToLongInt(const LongInt* Ptr_) const
{
  return (_Uvalue < Ptr_->UnsignedValue());
}

/****************************************************************************/
Boolean ULongInt::IsGreaterToLongInt(const LongInt* Ptr_) const
{
  return (_Uvalue > Ptr_->UnsignedValue());
}

/****************************************************************************/
istream& ULongInt::TextRead(istream& Is_, Boolean* Ok_)
{
  Ulong Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& ULongInt::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Uvalue;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
Boolean ULongInt::AtMaxValue() const
{
  return (_Uvalue == ULONG_MAX);
}

/****************************************************************************/
Boolean ULongInt::AtMinValue() const
{
  return (_Uvalue == 0);
}

/****************************************************************************/
LongInt& ULongInt::operator = (const long& IntVal_)
{
  _Uvalue = Ulong(IntVal_);
  return *this;
}

/****************************************************************************/
LongInt& ULongInt::operator = (const ChrString& Str_)
{
  ULongInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& ULongInt::operator = (const UShortInt& Obj_)
{
  ULongInt Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& ULongInt::operator = (const ShortInt& Obj_)
{
  ULongInt Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& ULongInt::operator = (const UInteger& Obj_)
{
  ULongInt Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& ULongInt::operator = (const Integer& Obj_)
{
  ULongInt Sobj_(Obj_);
  return (*this = Sobj_);
}

/****************************************************************************/
LongInt& ULongInt::operator = (const ULongInt& Obj_)
{
  LongInt::operator = (Obj_);
  return *this;
}

/****************************************************************************/
LongInt& ULongInt::operator = (const LongInt& Obj_)
{
  LongInt::operator = (Obj_);
  return *this;
}

/****************************************************************************/
ULongInt ULongInt::operator + () const
{
  return ULongInt(*this);
}

/****************************************************************************/
ULongInt ULongInt::operator ~ () const
{
  return ULongInt(~_Uvalue);
}

/****************************************************************************/
const LongInt& ULongInt::operator ++ ()
{
  if (BoundsChecking() && _Uvalue == ULONG_MAX)
    SetOverflow();

  ++_Uvalue;
  return *this;
}

/****************************************************************************/
const LongInt& ULongInt::operator ++ (int)
{
  static ULongInt OldVal_ = _Uvalue;

  if (BoundsChecking() && _Uvalue == ULONG_MAX)
    SetOverflow();

  ++_Uvalue;
  return OldVal_;
}

/****************************************************************************/
const LongInt& ULongInt::operator -- ()
{
  if (BoundsChecking() && _Uvalue == 0)
    SetUnderflow();

  --_Uvalue;
  return *this;
}

/****************************************************************************/
const LongInt& ULongInt::operator -- (int)
{
  static ULongInt OldVal_ = _Uvalue;

  if (BoundsChecking() && _Uvalue == 0)
    SetUnderflow();
  
  --_Uvalue;
  return OldVal_;
}

/****************************************************************************/
LongInt& ULongInt::operator += (const NumOrdinalType<LongInt>& Obj_)
{
  if (BoundsChecking())
  {
    Ulong Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UlongSeriesCompare CeilCmp_ = UlongSeriesCompare::SumOfSeriesCmp(ULONG_MAX, Buffer_, 2, FALSE);
    UlongSeriesCompare FloorCmp_ = UlongSeriesCompare::SumOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue += Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
LongInt& ULongInt::operator -= (const NumOrdinalType<LongInt>& Obj_)
{
  if (BoundsChecking())
  {
    Ulong Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UlongSeriesCompare CeilCmp_ = UlongSeriesCompare::DifferenceOfSeriesCmp(ULONG_MAX, Buffer_, 2, FALSE);
    UlongSeriesCompare FloorCmp_ = UlongSeriesCompare::DifferenceOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue -= Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
LongInt& ULongInt::operator *= (const NumOrdinalType<LongInt>& Obj_)
{
  if (BoundsChecking())
  {
    Ulong Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UlongSeriesCompare CeilCmp_ = UlongSeriesCompare::ProductOfSeriesCmp(ULONG_MAX, Buffer_, 2, FALSE);
    UlongSeriesCompare FloorCmp_ = UlongSeriesCompare::ProductOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue *= Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
LongInt& ULongInt::operator /= (const NumOrdinalType<LongInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Uvalue /= Obj_.DerivedClass().UnsignedValue();

  return *this;
}

/****************************************************************************/
LongInt& ULongInt::operator %= (const NumOrdinalType<LongInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Uvalue %= Obj_.DerivedClass().UnsignedValue();

  return *this;
}

/****************************************************************************/
LongInt& ULongInt::operator += (const long& IntVal_)
{
  return (*this += ULongInt(IntVal_));
}

/****************************************************************************/    
LongInt& ULongInt::operator -= (const long& IntVal_)
{
  return (*this -= ULongInt(IntVal_));
}

/****************************************************************************/    
LongInt& ULongInt::operator *= (const long& IntVal_)
{
  return (*this *= ULongInt(IntVal_));
}

/****************************************************************************/    
LongInt& ULongInt::operator /= (const long& IntVal_)
{
  return (*this /= ULongInt(IntVal_));
}

/****************************************************************************/    
LongInt& ULongInt::operator %= (const long& IntVal_)
{
  return (*this %= ULongInt(IntVal_));
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ULongInt::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ULongInt::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* ULongInt::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void ULongInt::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
ULongInt operator + (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) += Val2_);
}

/****************************************************************************/  
ULongInt operator - (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) -= Val2_);
}

/****************************************************************************/  
ULongInt operator * (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) *= Val2_);
}

/****************************************************************************/  
ULongInt operator / (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) /= Val2_);
}

/****************************************************************************/  
ULongInt operator % (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) %= Val2_);
}

/****************************************************************************/
ULongInt operator & (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) &= Val2_);
}

/****************************************************************************/  
ULongInt operator | (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) |= Val2_);
}

/****************************************************************************/  
ULongInt operator ^ (const ULongInt& Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) ^= Val2_);
}

/****************************************************************************/
ULongInt operator + (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) += Val2_);
}

/****************************************************************************/  
ULongInt operator - (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) -= Val2_);
}

/****************************************************************************/  
ULongInt operator * (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) *= Val2_);
}

/****************************************************************************/  
ULongInt operator / (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) /= Val2_);
}

/****************************************************************************/  
ULongInt operator % (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) %= Val2_);
}

/****************************************************************************/  
ULongInt operator & (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) &= Val2_);
}

/****************************************************************************/  
ULongInt operator | (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) |= Val2_);
}

/****************************************************************************/  
ULongInt operator ^ (Ulong Val1_, const ULongInt& Val2_)
{
  return (ULongInt(Val1_) ^= Val2_);
}

/****************************************************************************/
ULongInt operator + (const ULongInt& Val1_, Ulong Val2_)
{
  return (ULongInt(Val1_) += ULongInt(Val2_));
}

/****************************************************************************/  
ULongInt operator - (const ULongInt& Val1_, Ulong Val2_)
{
  ULongInt Temp1_(Val1_);
  ULongInt Temp2_(Val2_);
  return (Temp1_ -= Temp2_);
}

/****************************************************************************/  
ULongInt operator * (const ULongInt& Val1_, Ulong Val2_)
{
  return (ULongInt(Val1_) *= ULongInt(Val2_));
}

/****************************************************************************/  
ULongInt operator / (const ULongInt& Val1_, Ulong Val2_)
{
  ULongInt Temp1_(Val1_);
  ULongInt Temp2_(Val2_);
  return (Temp1_ /= Temp2_);
}

/****************************************************************************/  
ULongInt operator % (const ULongInt& Val1_, Ulong Val2_)
{
  ULongInt Temp1_(Val1_);
  ULongInt Temp2_(Val2_);
  return (Temp1_ %= Temp2_);
}

/****************************************************************************/  
ULongInt operator & (const ULongInt& Val1_, Ulong Val2_)
{
  return (ULongInt(Val1_) &= ULongInt(Val2_));
}

/****************************************************************************/  
ULongInt operator | (const ULongInt& Val1_, Ulong Val2_)
{
  return (ULongInt(Val1_) |= ULongInt(Val2_));
}

/****************************************************************************/  
ULongInt operator ^ (const ULongInt& Val1_, Ulong Val2_)
{
  return (ULongInt(Val1_) ^= ULongInt(Val2_));
}

/****************************************************************************/
/****************************************************************************/
ULongIntAcceptor::ULongIntAcceptor(const ULongInt* Ptr_):
LongIntAcceptor(Ptr_)
{}

/****************************************************************************/
Boolean ULongIntAcceptor::IsULongInt() const
{
  return TRUE;
}

/****************************************************************************/
Boolean ULongIntAcceptor::IsLongInt() const
{
  return TRUE;
}

/****************************************************************************/
void* ULongIntAcceptor::AssignFromUInteger(const ObjectAcceptor& Obj_)
{
  const UInteger* TrgPtr_ = (const UInteger*)Obj_.GiveDataPtr();
  _VarPtr->AssignUInteger(*TrgPtr_);
  return _VarPtr;
}

/****************************************************************************/
OBJACCEPTOR_CREATEFNC_TOPDEFN(ULongIntAcceptor::CreateFromChrString, ULongInt, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ULongIntAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_CREATEFNC_TOPDEFN(ULongIntAcceptor::CreateFromLongInt, ULongInt, LongInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ULongIntAcceptor::AssignFromLongInt, LongInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(ULongIntAcceptor::CreateFromShortInt, ULongInt, ShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ULongIntAcceptor::AssignFromShortInt, ShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(ULongIntAcceptor::CreateFromInteger, ULongInt, Integer)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ULongIntAcceptor::AssignFromInteger, Integer)
OBJACCEPTOR_CREATEFNC_TOPDEFN(ULongIntAcceptor::CreateFromULongInt, ULongInt, ULongInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ULongIntAcceptor::AssignFromULongInt, ULongInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(ULongIntAcceptor::CreateFromUShortInt, ULongInt, UShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(ULongIntAcceptor::AssignFromUShortInt, UShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(ULongIntAcceptor::CreateFromUInteger, ULongInt, UInteger)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(ULongIntAcceptor::CreateFromLongNumber, ULongInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(ULongIntAcceptor::AssignFromLongNumber)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(ULongIntAcceptor::CreateFromFloat, ULongInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(ULongIntAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(ULongIntAcceptor::CreateFromDouble, ULongInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(ULongIntAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(ULongIntAcceptor::CreateFromLongDouble, ULongInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(ULongIntAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)

MEMORYOPS_DEFN(ULongIntAcceptor)
/****************************************************************************/
/****************************************************************************/
#if ULONGINT_DEBUG
int main()
{
  ChrString Str_;

  ULongInt si(61515012);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "61515012"));

  ULongInt si2(61515012);
  ULongInt si3(41057012);
  Str_ = si3.ToString();
  assert(!strcmp(Str_.c_str(), "41057012"));  

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

  assert(si3 == ULongInt(61515012));

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  
  UInteger uintv("56800012");
  si2.Assign(uintv);
  assert(si2 == ULongInt(56800012));

  UShortInt usintv("65534");
  si2.Assign(usintv);
  assert(si2 == ULongInt(65534));

  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;

  // Testing bounds checking
  ULongInt chk1 = ULONG_MAX;
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
  assert(Ulong(chk1) == 16384);
  chk1 <<= 4;
  assert(Ulong(chk1) == 262144);
  chk1 |= ULongInt(0x0F);
  assert(Ulong(chk1) == 262159);
  chk1 = ULongInt(chk1) / ULongInt(0);
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
  ULongInt* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new ULongInt(i+1);
  List_[10] = NULL;

  ULongInt ZeroChar(0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(11), List_, ZeroChar, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(55), List_, ZeroChar, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(77), List_, ZeroChar, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<ULongInt>::SumOfUnsignedSeriesCmp(ULongInt(1), List_, ZeroChar, CHAR_MAX, TRUE).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<ULongInt> Pobj_ = SeriesCompare<ULongInt>::ProductOfUnsignedSeriesCmp(ULongInt(120), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<ULongInt>::ProductOfUnsignedSeriesCmp(ULongInt(121), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<ULongInt>::ProductOfUnsignedSeriesCmp(ULongInt(119), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing ULongInt" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





