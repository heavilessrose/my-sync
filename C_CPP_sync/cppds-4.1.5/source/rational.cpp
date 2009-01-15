///////////////////////////////////////////////////////////////
//  MATHEMATICAL LIBRARY
//      rational.cxx
//
//      A class for handling Rational numbers (fractions).
//
//      Copyright 1992 by Scott Robert Ladd
//      All Rights Reserved
///////////////////////////////////////////////////////////////

#ifndef RATIONAL_CPP
#define RATIONAL_CPP
#ifndef RATIONAL_H
  #include "rational.h"
#endif

//==========================================================
template <class T>
Rational<T>::Rational():
RationalRealType(TypeWrapper::BOUNDS_CHECKING_BIT),
Numer(0),
Denom(0)
{}

//==========================================================
template <class T>
Rational<T>::Rational(const ChrString& Str_):
RationalRealType(TypeWrapper::BOUNDS_CHECKING_BIT),
Numer(0),
Denom(0)
{
  ConvertFromString(Str_);
}

//==========================================================
template <class T>
Rational<T>::Rational(const Rational<T>& r):
RationalRealType(TypeWrapper::BOUNDS_CHECKING_BIT),
Numer(r.Numer),
Denom(r.Denom)
{
  Reduce();
}

//==========================================================
template <class T>
Rational<T>::Rational(T num, T den):
RationalRealType(TypeWrapper::BOUNDS_CHECKING_BIT),
Numer((den == 0) ? 0:num),
Denom(den)
{
  Reduce();
}

//==========================================================
template <class T>
int Rational<T>::GetTypePrec() const
{
  T Dum_;
  int Type_ = WhatIs(Dum_);
  int Prec_ = 63;
  
  if (Type_ == TypeInfo::BUILTIN_FLOAT)
    Prec_ = 31;
  else if (Type_ == TypeInfo::BUILTIN_DOUBLE)
    Prec_ = 63;
  else if (Type_ == TypeInfo::BUILTIN_LONG_DOUBLE)
    Prec_ = 79;

  return Prec_;
}

//==========================================================
template <class T>
void Rational<T>::ConvertFromString(const ChrString& Str_)
{
  T NumVal_, DenVal_;
  ChrString NumData_, DenData_;
  
  Boolean IntType_ = IsIntegerType(&NumVal_);
  char* Buffer_ = new_char_string(Str_.c_str());
  NumData_ = strtok(Buffer_, "/");
  DenData_ = strtok(NULL, "/");

#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
  if (IntType_)
  {
    NumVal_ = (long)NumData_.strtol();
    DenVal_ = (long)DenData_.strtol();
  }
  else
  {
    NumVal_ = (double)NumData_.strtod();
    DenVal_ = (double)DenData_.strtod();
  }
#else
  if (IntType_)
  {
    NumVal_ = NumData_.strtol();
    DenVal_ = DenData_.strtol();
  }
  else
  {
    NumVal_ = NumData_.strtod();
    DenVal_ = DenData_.strtod();
  }
#endif

  AssignRational(NumVal_, DenVal_);
  ::DeleteArray(Buffer_);
}

//==========================================================
template <class T>
Rational<T>* Rational<T>::Make()
{
  return (new Rational<T>());
}

//==========================================================
template <class T>
Rational<T>* Rational<T>::Make(const T& num, const T& den)
{
  return (new Rational<T>(num, den));
}

//==========================================================
template <class T>
Rational<T>* Rational<T>::Make(const Object& Obj_)
{
  RationalAcceptor<T> TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

//==========================================================
template <class T>
Object& Rational<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  RationalAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromRational(TrgComp_)));
}

//==========================================================
template <class T>
Object* Rational<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Rational<T>(*this));

  RationalAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromRational(TrgComp_)));
}

//==========================================================
template <class T>
Rational<T>& Rational<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Rational<T> _NULLOBJECT(0, 0);
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//==========================================================
template <class T>
const Object& Rational<T>::NULLOBJECT() const
{
  return GiveNullObject();
}

//==========================================================
template <class T>
Object& Rational<T>::NULLOBJECT()
{
  return GiveNullObject();
}

//==========================================================
template <class T>
Boolean Rational<T>::IsValue(const ChrString& Str_)
{
  T Example_;
  Boolean IntType_ = IsIntegerType(&Example_);
  Boolean NumValid_ = FALSE;
  Boolean DenValid_ = FALSE;
  ChrString Token_;
  StringToken Tokenizer_;

  Tokenizer_.SetString(Str_.c_str());
  Tokenizer_.SetWhiteSpace(" \t\n\r");
  Tokenizer_.SetDelimiters("/");

  char* Buffer_ = NULL;
  Tokenizer_.FindToken(Buffer_, TRUE);
  NumValid_ = Buffer_ &&
              (IntType_ ? (IsSignedNum(Buffer_) || IsUnsignedNum(Buffer_)):
                          IsFloat(Buffer_));
  ::Delete(Buffer_);

  if (NumValid_)
  {
    Tokenizer_.FindToken(Buffer_, TRUE);
    DenValid_ = Buffer_ &&
                (IntType_ ? (IsSignedNum(Buffer_) || IsUnsignedNum(Buffer_)):
                            IsFloat(Buffer_));    
    ::Delete(Buffer_);
  }

  return (NumValid_ && DenValid_);
}

//==========================================================
template <class T>
Rational<T> Rational<T>::ValueOf(const ChrString& Str_)
{
  return Rational<T>(Str_);
}

//==========================================================
template <class T>
ChrString Rational<T>::ToString(const Rational<T>& Dbl_)
{
  return Dbl_.ToString();
}

//==========================================================
template <class T>
ChrString Rational<T>::ToTruncString(const Rational<T>& Dbl_)
{
  return Dbl_.ToTruncString();
}

//==========================================================
template <class T>
ChrString Rational<T>::ToString(int Radix_) const
{
  T Example_;
  Boolean IntType_ = IsIntegerType(&Example_);
  char Buffer_[128];
  int Prec_ = GetTypePrec();
  ChrString NumStr_;
  ChrString DenStr_;

  if (IntType_)
  {
    NumStr_ = RemovePadding(LongToStr(Numer, Buffer_), " ");
    DenStr_ = RemovePadding(LongToStr(Denom, Buffer_), " ");
  }
  else
  {
    NumStr_ = RemovePadding(FloatToStr(Numer, Buffer_, 127, Prec_), " ");
    DenStr_ = RemovePadding(FloatToStr(Denom, Buffer_, 127, Prec_), " ");
  }
  
  ChrString RetStr_ = NumStr_ + ChrString(" / ") + DenStr_;
  return RetStr_;
}

//==========================================================
template <class T>
ChrString Rational<T>::ToTruncString(int Radix_) const
{
  T Example_;
  Boolean IntType_ = IsIntegerType(&Example_);
  char Buffer_[128];
  Rational<T> Temp_(*this);
  long Lvalue1_, Lvalue2_;
  int Prec_ = GetTypePrec();
  ChrString NumStr_;
  ChrString DenStr_;

  Lvalue1_ = (long)Temp_.GetNum();
  Lvalue2_ = (long)Temp_.GetDen();

  if (IntType_)
  {
    NumStr_ = (RemovePadding(LongToStr(Lvalue1_, Buffer_), " "));
    DenStr_ = (RemovePadding(LongToStr(Lvalue2_, Buffer_), " "));
  }
  else
  {
    NumStr_ = (RemovePadding(FloatToStr(Lvalue1_, Buffer_, 127, Prec_), " "));
    DenStr_ = (RemovePadding(FloatToStr(Lvalue2_, Buffer_, 127, Prec_), " "));
  }

  ChrString RetStr_ = NumStr_ + ChrString(" / ") + DenStr_;
  return RetStr_;
}

//==========================================================
template <class T>
long Rational<T>::DataSize() const
{
  return (sizeof(T) * 2);
}

//==========================================================
template <class T>
int Rational<T>::ClassID() const
{
  return TypeInfo::RATIONAL_TYPE;
}

//==========================================================
template <class T>
Rational<T>& Rational<T>::FromString(const ChrString& Str_)
{
  ConvertFromString(Str_);
  return *this;
}

//==========================================================
template <class T>
Boolean Rational<T>::AtZero() const
{
  double result;
  
  if (Denom == 0)
  {
    if (Numer == 0)
      result = 0.0;
    else
      result = HUGE_VAL;
  }
  else
    result = (double)Numer / (double)Denom;
    
  return (!result);
}

//==========================================================
template <class T>
const ObjectAcceptor* Rational<T>::GiveObjectAcceptor() const
{
  return (new RationalAcceptor<T>(this));
}

//==========================================================
template <class T>
istream& Rational<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
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

//==========================================================
template <class T>
istream& Rational<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.read((char*)(&Numer), sizeof(T)).good()))
    if (*Ok_ && (*Ok_ = Is_.read((char*)(&Denom), sizeof(T)).good()))
      SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

//==========================================================
template <class T>
ostream& Rational<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Os_.write((char*)(&Numer), sizeof(T));
    if (OstreamCheck(Os_, Ok_))
      Os_.write((char*)(&Denom), sizeof(T));
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

//==========================================================
template <class T>
ByteStream& Rational<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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

//==========================================================
template <class T>
ByteStream& Rational<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.iread((char*)(&Numer), sizeof(T)).good()))
    if (*Ok_ && (*Ok_ = Is_.iread((char*)(&Denom), sizeof(T)).good()))
      SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

//==========================================================
template <class T>
ByteStream& Rational<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Os_.owrite((char*)(&Numer), sizeof(T));
    if (OstreamCheck(((ostream&)Os_), Ok_))
      Os_.owrite((char*)(&Denom), sizeof(T));
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

//==========================================================
template <class T>
istream& Rational<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  T NumVal_, DenVal_;
  Boolean IntType_ = IsIntegerType(&NumVal_);
  ChrString NumData_, DivSign_, DenData_;

  Is_ >>NumData_ >>DivSign_;

  if (DivSign_ == "/")
    Is_ >>DenData_;
  else
    DenData_ = DivSign_;

#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
  if (IntType_)
  {
    NumVal_ = (long)NumData_.strtol();
    DenVal_ = (long)DenData_.strtol();
  }
  else
  {
    NumVal_ = (double)NumData_.strtod();
    DenVal_ = (double)DenData_.strtod();
  }
#else
  if (IntType_)
  {
    NumVal_ = NumData_.strtol();
    DenVal_ = DenData_.strtol();
  }
  else
  {
    NumVal_ = NumData_.strtod();
    DenVal_ = DenData_.strtod();
  }
#endif

  AssignRational(NumVal_, DenVal_);  
  return ObjectImp::TextRead(Is_, Ok_);
}

//==========================================================
template <class T>
ostream& Rational<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<Numer <<" + " <<Denom;
  return ObjectImp::TextWrite(Os_, Ok_);
}

//==========================================================
template <class T>
Boolean Rational<T>::IsEqualToRational(const Rational<T>* Ptr_) const
{
  return (Numer == Ptr_->GetNum() &&
          Denom == Ptr_->GetDen());
}

//==========================================================
template <class T>
Boolean Rational<T>::IsLesserToRational(const Rational<T>* Ptr_) const
{
  T Multiple_ = LCM(Denom, Ptr_->GetDen());

  return
  (
    Numer * (Multiple_ / Denom) <
    Ptr_->GetNum() * (Multiple_ / Ptr_->GetDen())
  );
}

//==========================================================
template <class T>
Boolean Rational<T>::IsGreaterToRational(const Rational<T>* Ptr_) const
{
  T Multiple_ = LCM(Denom, Ptr_->GetDen());

  return
  (
    Numer * (Multiple_ / Denom) >
    Ptr_->GetNum() * (Multiple_ / Ptr_->GetDen())
  );
}

//==========================================================
template <class T>
Boolean Rational<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  RationalAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToRational(TrgComp_);
}

//==========================================================
template <class T>
Boolean Rational<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  RationalAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToRational(TrgComp_);
}

//==========================================================
template <class T>
Boolean Rational<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  RationalAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToRational(TrgComp_);
}

//==========================================================
template <class T>
Rational<T>& Rational<T>::Ceiling()
{
  double Val_ = (double)(*this);
  Numer = T(ceil(Val_));
  Denom = 1;
  return *this;
}

//==========================================================
template <class T>
Rational<T>& Rational<T>::Floor()
{
  double Val_ = (double)(*this);
  Numer = T(floor(Val_));
  Denom = 1;
  return *this;
}

//==========================================================
template <class T>
Rational<T>& Rational<T>::Round()
{
  double Val_ = (double)(*this);
  
  if (Val_ >= 0)
    Numer = T(ceil(Val_ + .5));
  else
    Numer = T(floor(Val_ - .5));
    
  Denom = 1;
  return *this;
}

//==========================================================
template <class T>
Boolean Rational<T>::operator ! () const
{
  return (GetNum() == T(0));
}

//==========================================================
template <class T>
Rational<T> Rational<T>::operator - () const
{
  Rational<T> result = *this;
  result.Numer = -result.Numer;
  return result;
}

//==========================================================
template <class T>
Rational<T> Rational<T>::operator + () const
{
  return Rational<T>(Numer, Denom);
}

//==========================================================
template <class T>
Rational<T>& Rational<T>::operator = (const ChrString& Str_)
{
  ConvertFromString(Str_);
  return *this;
}

//==========================================================
template <class T>
Rational<T>& Rational<T>::operator = (const Rational<T> & r)
{
  Numer = r.Numer;
  Denom = r.Denom;
  return *this;
}

//==========================================================
template <class T>
Rational<T> Rational<T>::AssignRational(T num, T den)
{
  if (den == T(0))
    num = T(0);

  Numer = num;
  Denom = den;

  Reduce();

  return *this;
}

//==========================================================
template <class T>
Rational<T> & Rational<T>::operator () (T num, T den)
{
  if (den == 0)
    num = 0;

  Numer = num;
  Denom = den;

  Reduce();

  return *this;
}

//==========================================================
template <class T>
Rational<T> Rational<T>::operator += (const Rational<T> & r)
{
  *this = *this + r;
  return *this;
}

//==========================================================
template <class T>
Rational<T> Rational<T>::operator -= (const Rational<T> & r)
{
  *this = *this - r;
  return *this;
}

//==========================================================
template <class T>
Rational<T> Rational<T>::operator *= (const Rational<T> & r)
{
  *this = *this * r;
  return *this;
}

//==========================================================
template <class T>
Rational<T> Rational<T>::operator /= (const Rational<T> & r)
{
  *this = *this / r;
  return *this;
}

//==========================================================
template <class T>
Rational<T>::operator double() const
{
  double result;
  
  if (Denom == 0)
  {
    if (Numer == 0)
      result = 0.0;
    else
      result = HUGE_VAL;
  }
  else
    result = (double)Numer / (double)Denom;
    
  return result;
}

//==========================================================
template <class T>
double Rational<T>::FloatValue() const
{
  return ((double)*this);
}

//==========================================================
template <class T>
T Rational<T>::GetNum() const
{
  return Numer;
}

//==========================================================
template <class T>
T Rational<T>::GetDen() const
{
  return Denom;
}

//==========================================================
template <class T>
void Rational<T>::Reduce()
{
  // check for zero numerator
  if (Numer == 0)
  {
    Denom = 0;
    return;
  }
  else
    if (Denom == 0)
    {
      Numer = 0;
      return;
    }
    
  if ((Numer > 0) && (Denom < 0))
  {
    Numer = -Numer;
    Denom = -Denom;
  }
  else
  {
    if ((Numer < 0) && (Denom < 0))
    {
      Numer = -Numer;
      Denom = -Denom;
    }
  }

  T div = GCD(Numer,Denom);
  
  Numer /= div;
  Denom /= div;
}

//==========================================================
template <class T>
T Rational<T>::LCM(T x, T y)
{
  x = labs(x);
  y = labs(y);

  if (x == y)
    return x;
  else
  {
    if (x < y)
      return (y / GCD(x,y)) * x;
    else
      return (x / GCD(y,x)) * y;
  }
}

//==========================================================
template <class T>
T Rational<T>::GCD(T x, T y)
{
  x = labs(x);
  y = labs(y);

  T temp;

  while (y != 0)
  {
    temp = x % y;
    x = y;
    y = temp;
  }
  
  return x;
}

//==========================================================
#if OVERLOAD_NEW
template <class T>
void* Rational<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//==========================================================
template <class T>
void Rational<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//==========================================================
#if HAS_ARRAY_NEW
template <class T>
void* Rational<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//==========================================================
template <class T>
void Rational<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
//==========================================================
/****************************************************************************/
template <class T>
Boolean RationalAcceptor<T>::IsRational() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean RationalAcceptor<T>::IsTypeWrapper() const
{
  return FALSE;
}

/****************************************************************************/
template <class T>
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(RationalAcceptor<T>::RationalAcceptor, Rational<T>)
template <class T>
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(RationalAcceptor<T>::AcceptDataPtr, Rational<T>)
template <class T>
OBJACCEPTOR_GIVEDATA_TOPDEFN(RationalAcceptor<T>::GiveDataPtr)
template <class T>
OBJACCEPTOR_CREATEFNC_TOPDEFN(RationalAcceptor<T>::CreateFromRational, Rational<T>, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(RationalAcceptor<T>::AssignFromRational, Rational<T>)
template <class T>
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(RationalAcceptor<T>::IsLesserToRational, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(RationalAcceptor<T>::IsEqualToRational, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(RationalAcceptor<T>::IsGreaterToRational, _ObjPtr->IsGreaterToRational, Rational<T>)
MEMORYOPS_TEMPLATE_DEFN(RationalAcceptor)

template <class T>
OBJACCEPTOR_CREATEFNC_TOPDEFN(RationalAcceptor<T>::CreateFromChrString, Rational<T>, ChrString)
template <class T>
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(RationalAcceptor<T>::AssignFromChrString, ChrString)
template <class T>
OBJACCEPTOR_COMPFNC_TOPDEFN(RationalAcceptor<T>::IsLesserToChrString, _ObjPtr->IsLesserToRational, Rational<T>, ChrString)
template <class T>
OBJACCEPTOR_COMPFNC_TOPDEFN(RationalAcceptor<T>::IsEqualToChrString, _ObjPtr->IsEqualToRational, Rational<T>, ChrString)
template <class T>
OBJACCEPTOR_COMPFNC_TOPDEFN(RationalAcceptor<T>::IsGreaterToChrString, _ObjPtr->IsGreaterToRational, Rational<T>, ChrString)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromLongNumber, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromLongNumber)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToLongNumber, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToLongNumber, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToLongNumber, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromULongInt, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromULongInt)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToULongInt, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToULongInt, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToULongInt, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromUInteger, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromUInteger)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToUInteger, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToUInteger, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToUInteger, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromUShortInt, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromUShortInt)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToUShortInt, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToUShortInt, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToUShortInt, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromLongInt, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromLongInt)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToLongInt, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToLongInt, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToLongInt, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromInteger, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromInteger)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToInteger, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToInteger, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToInteger, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromShortInt, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromShortInt)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToShortInt, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToShortInt, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToShortInt, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromDouble, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromDouble)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToDouble, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToDouble, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToDouble, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromLongDouble, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromLongDouble)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToLongDouble, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToLongDouble, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToLongDouble, _ObjPtr->IsGreaterToRational, Rational<T>)
template <class T>
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::CreateFromFloat, Rational<T>)
template <class T>
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::AssignFromFloat)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsLesserToFloat, _ObjPtr->IsLesserToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsEqualToFloat, _ObjPtr->IsEqualToRational, Rational<T>)
template <class T>
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(RationalAcceptor<T>::IsGreaterToFloat, _ObjPtr->IsGreaterToRational, Rational<T>)
/****************************************************************************/
#endif





