#ifndef USHORTINT_CPP
#define USHORTINT_CPP
#ifndef USHORTINT_H
  #include "ushortint.h"
#endif

/****************************************************************************/
UShortInt::UShortInt(Ushort IntVal_):
ShortInt(IntVal_, 0)
{}

/****************************************************************************/
UShortInt::UShortInt(const ChrString& Str_):
ShortInt(Str_, FALSE, UShortInt::IsValue(Str_))
{
  Byte_t Valid_ = _Valid;
  _Uvalue = Valid_ ? Ushort(Str_.strtoul()):0;

  if (!Valid_)
  {
    SetAsNullObject();
    SetDataConversionError();
  }
}

/****************************************************************************/
UShortInt::UShortInt(const ShortInt& IntObj_):
ShortInt(IntObj_)
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
UShortInt::UShortInt(const UShortInt& IntObj_):
ShortInt(IntObj_)
{}
#endif
/****************************************************************************/
UShortInt* UShortInt::Make(Ushort IntVal_)
{
  return (new UShortInt(IntVal_));
}

/****************************************************************************/
UShortInt* UShortInt::Make(const Object& Obj_)
{
  UShortIntAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return ((UShortInt*) TrgComp_._VarPtr);
}

/****************************************************************************/
Object* UShortInt::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return ((Object*) (new UShortInt(*this)));

  UShortIntAcceptor TrgComp_(this);
  return ((Object*) ObjComp_->CreateFromUShortInt(TrgComp_));
}

/****************************************************************************/
Object& UShortInt::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  UShortIntAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromUShortInt(TrgComp_)));
}

/****************************************************************************/
UShortInt& UShortInt::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static UShortInt _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& UShortInt::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return UShortInt::GiveNullObject();
}

/****************************************************************************/
Object& UShortInt::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return UShortInt::GiveNullObject();
}

/****************************************************************************/
double UShortInt::FloatValue() const
{
  return _Uvalue;
}

/****************************************************************************/
Boolean UShortInt::IsValue(const ChrString& Str_)
{
  return IsUshort(Str_.c_str());
}

/****************************************************************************/
UShortInt UShortInt::ValueOf(const ChrString& Str_)
{
  return UShortInt(Str_);
}

/****************************************************************************/
ChrString UShortInt::ToString(const ShortInt& Int_, int Radix_)
{
  char Buffer_[16];

  UShortInt UInt_(Int_);
  return ChrString(UshortToStr(Ushort(UInt_), Buffer_, Radix_));
}

/****************************************************************************/
ChrString UShortInt::ToString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(UshortToStr(Ushort(*this), Buffer_, Radix_));
}

/****************************************************************************/
ChrString UShortInt::ToTruncString(const ShortInt& Int_, int Radix_)
{
  char Buffer_[16];

  UShortInt UInt_(Int_);
  return ChrString(UshortToStr(Ushort(UInt_), Buffer_, Radix_));
}

/****************************************************************************/
ChrString UShortInt::ToTruncString(int Radix_) const
{
  char Buffer_[16];
  return ChrString(UshortToStr(Ushort(*this), Buffer_, Radix_));
}

/****************************************************************************/
ShortInt& UShortInt::FromString(const ChrString& Str_)
{
  UShortInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
int UShortInt::ClassID() const
{
  return TypeInfo::USHORTINT_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* UShortInt::GiveObjectAcceptor() const
{
  return (new UShortIntAcceptor(this));
}

/****************************************************************************/
Boolean UShortInt::IsLesserToShortInt(const ShortInt* Ptr_) const
{
  return (_Uvalue < Ptr_->UnsignedValue());
}

/****************************************************************************/
Boolean UShortInt::IsGreaterToShortInt(const ShortInt* Ptr_) const
{
  return (_Uvalue > Ptr_->UnsignedValue());
}

/****************************************************************************/
istream& UShortInt::TextRead(istream& Is_, Boolean* Ok_)
{
  Ushort Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& UShortInt::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Uvalue;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
Boolean UShortInt::AtMaxValue() const
{
  return (_Uvalue == USHRT_MAX);
}

/****************************************************************************/
Boolean UShortInt::AtMinValue() const
{
  return (_Uvalue == 0);
}

/****************************************************************************/
ShortInt& UShortInt::operator = (const short& IntVal_)
{
  _Uvalue = Ushort(IntVal_);
  return *this;
}

/****************************************************************************/
ShortInt& UShortInt::operator = (const ChrString& Str_)
{
  UShortInt Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
ShortInt& UShortInt::operator = (const ShortInt& Obj_)
{
  ShortInt::operator = (Obj_);
  return *this;
}

/****************************************************************************/
ShortInt& UShortInt::operator = (const UShortInt& Obj_)
{
  ShortInt::operator = (Obj_);
  return *this;
}

/****************************************************************************/
UShortInt UShortInt::operator + () const
{
  return UShortInt(*this);
}

/****************************************************************************/
UShortInt UShortInt::operator ~ () const
{
  return UShortInt(~_Uvalue);
}

/****************************************************************************/
const ShortInt& UShortInt::operator ++ ()
{
  if (BoundsChecking() && _Uvalue == USHRT_MAX)
    SetOverflow();

  ++_Uvalue;
  return *this;
}

/****************************************************************************/
const ShortInt& UShortInt::operator ++ (int)
{
  static UShortInt OldVal_ = _Uvalue;

  if (BoundsChecking() && _Uvalue == USHRT_MAX)
    SetOverflow();

  ++_Uvalue;
  return OldVal_;
}

/****************************************************************************/
const ShortInt& UShortInt::operator -- ()
{
  if (BoundsChecking() && _Uvalue == 0)
    SetUnderflow();

  --_Uvalue;
  return *this;
}

/****************************************************************************/
const ShortInt& UShortInt::operator -- (int)
{
  static UShortInt OldVal_ = _Uvalue;

  if (BoundsChecking() && _Uvalue == 0)
    SetUnderflow();
  
  --_Uvalue;
  return OldVal_;
}

/****************************************************************************/
ShortInt& UShortInt::operator += (const NumOrdinalType<ShortInt>& Obj_)
{
  if (BoundsChecking())
  {
    Ushort Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UshortSeriesCompare CeilCmp_ = UshortSeriesCompare::SumOfSeriesCmp(USHRT_MAX, Buffer_, 2, FALSE);
    UshortSeriesCompare FloorCmp_ = UshortSeriesCompare::SumOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue += Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
ShortInt& UShortInt::operator -= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (BoundsChecking())
  {
    Ushort Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UshortSeriesCompare CeilCmp_ = UshortSeriesCompare::DifferenceOfSeriesCmp(USHRT_MAX, Buffer_, 2, FALSE);
    UshortSeriesCompare FloorCmp_ = UshortSeriesCompare::DifferenceOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue -= Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
ShortInt& UShortInt::operator *= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (BoundsChecking())
  {
    Ushort Buffer_[2];
    Buffer_[0] = _Uvalue;
    Buffer_[1] = Obj_.DerivedClass().UnsignedValue();

    UshortSeriesCompare CeilCmp_ = UshortSeriesCompare::ProductOfSeriesCmp(USHRT_MAX, Buffer_, 2, FALSE);
    UshortSeriesCompare FloorCmp_ = UshortSeriesCompare::ProductOfSeriesCmp(0, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Uvalue *= Obj_.DerivedClass().UnsignedValue();
  return *this;
}

/****************************************************************************/
ShortInt& UShortInt::operator /= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Uvalue /= Obj_.DerivedClass().UnsignedValue();

  return *this;
}

/****************************************************************************/
ShortInt& UShortInt::operator %= (const NumOrdinalType<ShortInt>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Uvalue %= Obj_.DerivedClass().UnsignedValue();

  return *this;
}

/****************************************************************************/
ShortInt& UShortInt::operator += (const short& IntVal_)
{
  return (*this += UShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& UShortInt::operator -= (const short& IntVal_)
{
  return (*this -= UShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& UShortInt::operator *= (const short& IntVal_)
{
  return (*this *= UShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& UShortInt::operator /= (const short& IntVal_)
{
  return (*this /= UShortInt(IntVal_));
}

/****************************************************************************/    
ShortInt& UShortInt::operator %= (const short& IntVal_)
{
  return (*this %= UShortInt(IntVal_));
}

/****************************************************************************/
#if OVERLOAD_NEW
void* UShortInt::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void UShortInt::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* UShortInt::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void UShortInt::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
UShortInt operator + (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) += Val2_);
}

/****************************************************************************/  
UShortInt operator - (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) -= Val2_);
}

/****************************************************************************/  
UShortInt operator * (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) *= Val2_);
}

/****************************************************************************/  
UShortInt operator / (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) /= Val2_);
}

/****************************************************************************/  
UShortInt operator % (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) %= Val2_);
}

/****************************************************************************/
UShortInt operator & (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) &= Val2_);
}

/****************************************************************************/  
UShortInt operator | (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) |= Val2_);
}

/****************************************************************************/  
UShortInt operator ^ (const UShortInt& Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) ^= Val2_);
}

/****************************************************************************/
UShortInt operator + (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) += Val2_);
}

/****************************************************************************/  
UShortInt operator - (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) -= Val2_);
}

/****************************************************************************/  
UShortInt operator * (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) *= Val2_);
}

/****************************************************************************/  
UShortInt operator / (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) /= Val2_);
}

/****************************************************************************/  
UShortInt operator % (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) %= Val2_);
}

/****************************************************************************/  
UShortInt operator & (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) &= Val2_);
}

/****************************************************************************/  
UShortInt operator | (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) |= Val2_);
}

/****************************************************************************/  
UShortInt operator ^ (Ushort Val1_, const UShortInt& Val2_)
{
  return (UShortInt(Val1_) ^= Val2_);
}

/****************************************************************************/
UShortInt operator + (const UShortInt& Val1_, Ushort Val2_)
{
  return (UShortInt(Val1_) += UShortInt(Val2_));
}

/****************************************************************************/  
UShortInt operator - (const UShortInt& Val1_, Ushort Val2_)
{
  UShortInt Temp1_(Val1_);
  UShortInt Temp2_(Val2_);
  return (Temp1_ -= Temp2_);
}

/****************************************************************************/  
UShortInt operator * (const UShortInt& Val1_, Ushort Val2_)
{
  return (UShortInt(Val1_) *= UShortInt(Val2_));
}

/****************************************************************************/  
UShortInt operator / (const UShortInt& Val1_, Ushort Val2_)
{
  UShortInt Temp1_(Val1_);
  UShortInt Temp2_(Val2_);
  return (Temp1_ /= Temp2_);
}

/****************************************************************************/  
UShortInt operator % (const UShortInt& Val1_, Ushort Val2_)
{
  UShortInt Temp1_(Val1_);
  UShortInt Temp2_(Val2_);
  return (Temp1_ %= Temp2_);
}

/****************************************************************************/  
UShortInt operator & (const UShortInt& Val1_, Ushort Val2_)
{
  return (UShortInt(Val1_) &= UShortInt(Val2_));
}

/****************************************************************************/  
UShortInt operator | (const UShortInt& Val1_, Ushort Val2_)
{
  return (UShortInt(Val1_) |= UShortInt(Val2_));
}

/****************************************************************************/  
UShortInt operator ^ (const UShortInt& Val1_, Ushort Val2_)
{
  return (UShortInt(Val1_) ^= UShortInt(Val2_));
}

/****************************************************************************/
/****************************************************************************/
UShortIntAcceptor::UShortIntAcceptor(const UShortInt* Ptr_):
ShortIntAcceptor(Ptr_)
{}

/****************************************************************************/
Boolean UShortIntAcceptor::IsUShortInt() const
{
  return TRUE;
}

/****************************************************************************/
Boolean UShortIntAcceptor::IsShortInt() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CREATEFNC_TOPDEFN(UShortIntAcceptor::CreateFromChrString, UShortInt, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(UShortIntAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_CREATEFNC_TOPDEFN(UShortIntAcceptor::CreateFromShortInt, UShortInt, ShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(UShortIntAcceptor::AssignFromShortInt, ShortInt)
OBJACCEPTOR_CREATEFNC_TOPDEFN(UShortIntAcceptor::CreateFromUShortInt, UShortInt, UShortInt)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(UShortIntAcceptor::AssignFromUShortInt, UShortInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::CreateFromLongNumber, UShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::AssignFromLongNumber)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::CreateFromLongInt, UShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::AssignFromLongInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::CreateFromInteger, UShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::AssignFromInteger)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::CreateFromULongInt, UShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::AssignFromULongInt)
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::CreateFromUInteger, UShortInt)
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(UShortIntAcceptor::AssignFromUInteger)

OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(UShortIntAcceptor::CreateFromFloat, UShortInt, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(UShortIntAcceptor::AssignFromFloat, FloatTypesPrecision::FLOAT)
OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(UShortIntAcceptor::CreateFromDouble, UShortInt, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(UShortIntAcceptor::AssignFromDouble, FloatTypesPrecision::DOUBLE)
OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(UShortIntAcceptor::CreateFromLongDouble, UShortInt, FloatTypesPrecision::LONGDOUBLE)
OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(UShortIntAcceptor::AssignFromLongDouble, FloatTypesPrecision::LONGDOUBLE)

MEMORYOPS_DEFN(UShortIntAcceptor)

/****************************************************************************/
/****************************************************************************/
#if USHORTINT_DEBUG
int main()
{
  ChrString Str_;

  UShortInt si(61515);
  Str_ = si.ToString();
  assert(!strcmp(Str_.c_str(), "61515"));

  UShortInt si2(61515);
  UShortInt si3(41057);
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

  assert(Ushort(si3) == 61515);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  si2 = "56800";
  assert(Ushort(si2) == 56800);

  si2 = "56800111";
  assert(Ushort(si2) == 0);

  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;

  // Testing bounds checking
  UShortInt chk1 = 65535;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 256;
  chk1 *= chk1;
  assert(chk1.Overflow());
  chk1 = 256;
  chk1 >>= 2;
  assert(Ushort(chk1) == 64);
  chk1 <<= 4;
  assert(Ushort(chk1) == 1024);
  chk1 |= UShortInt(0x0F);
  assert(Ushort(chk1) == 1039);
  chk1 = chk1 / UShortInt(0);
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
  UShortInt* List_[25];
  short i;

  for (i = 0; i < 10; ++i)
    List_[i] = new UShortInt(i+1);
  List_[10] = NULL;

  UShortInt ZeroChar(0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(11), List_, ZeroChar, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(55), List_, ZeroChar, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(77), List_, ZeroChar, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<UShortInt>::SumOfUnsignedSeriesCmp(UShortInt(1), List_, ZeroChar, CHAR_MAX, TRUE).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<UShortInt> Pobj_ = SeriesCompare<UShortInt>::ProductOfUnsignedSeriesCmp(UShortInt(120), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<UShortInt>::ProductOfUnsignedSeriesCmp(UShortInt(121), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<UShortInt>::ProductOfUnsignedSeriesCmp(UShortInt(119), List_, ZeroChar, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];  

  cout <<endl <<"SUCCESS Testing UShortInt" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





