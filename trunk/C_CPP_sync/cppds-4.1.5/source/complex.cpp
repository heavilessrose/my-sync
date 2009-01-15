#ifndef COMPLEX_CPP
#define COMPLEX_CPP
#ifndef COMPLEX_H
  #include "complex.h"
#endif

/****************************************************************************/
template <class T>
ComplexNumber<T> atan(const ComplexNumber<T> & c)
{
  ComplexNumber<T> x = log((c + ComplexNumber<T>::CNI()) / (-c + ComplexNumber<T>::CNI()));
  return ComplexNumber<T>(-x.ImagVal() * 0.5, x.RealVal() * 0.5);
}

/****************************************************************************/
template <class T>
ComplexNumber<T> asin(const ComplexNumber<T> & c)
{
  ComplexNumber<T> x = log(ComplexNumber<T>::CNI() * c + sqrt(ComplexNumber<T>::CN1() - c * c));
  return ComplexNumber<T>(x.ImagVal(), -x.RealVal());
}

/****************************************************************************/
template <class T>
ComplexNumber<T> acos(const ComplexNumber<T> & c)
{
  ComplexNumber<T> x = log(c + ComplexNumber<T>::CNI() * sqrt(ComplexNumber<T>::CN1() - c * c));
  return ComplexNumber<T>(x.ImagVal(), -x.RealVal());
}

/****************************************************************************/
template <class T>
ComplexNumber<T> pow(const ComplexNumber<T> & c, const ComplexNumber<T> & ex)
{
    ComplexNumber<T> result;

    if (ex.RealVal() == 0.0 && ex.ImagVal() == 0.0)
        return ComplexNumber<T>::CN1();

    if (c.RealVal() == 0.0 && c.ImagVal() == 0.0)
        return ComplexNumber<T>::CNZ();

    return exp(log(c) * ex);
}

/****************************************************************************/
template <class T>
ComplexNumber<T> pow(const ComplexNumber<T> & c, int ex)
{
    ComplexNumber<T> result;

    if (ex == 0)
        result = ComplexNumber<T>::CN1();
    else
    {
        result = c;

        for(int n = 1; n < ex; ++n)
            result *= result;
    }

    return result;
}

/****************************************************************************/
template <class T>
ComplexNumber<T> sqrt(const ComplexNumber<T>& c)
{
    ComplexNumber<T> result;
    long double r, i, t1, t2;

    if ((c.RealVal() == 0.0) && (c.ImagVal() == 0.0))
    {
        result.SetRealVal(0.0);
        result.SetImagVal(0.0);
    }
    else
    {
        r = fabs(c.RealVal());
        i = fabs(c.ImagVal());

        if (r >= i)
        {
            t1 = i / r;
            t2 = sqrt(r)
               * sqrt(0.5 * (1.0 + sqrt(1.0 + t1 * t1)));
        }
        else
        {
            t1 = r / i;
            t2 = sqrt(i)
               * sqrt(0.5 * (t1 + sqrt(1.0 + t1 * t1)));
        }

        if (c.RealVal() >= 0.0)
        {
            result.SetRealVal(t2);
            result.SetImagVal(c.ImagVal() / (2.0 * t2));
        }
        else
        {
            result.SetImagVal((c.ImagVal() >= 0.0) ? t2 : -t2);
            result.SetRealVal(c.ImagVal() / (2.0 * result.ImagVal()));
        }
    }

    return result;
}

/****************************************************************************/
template <class T>
double abs(const ComplexNumber<T>& c)
{
    T r, i, a, t;

    r = fabs(c.RealVal());
    i = fabs(c.ImagVal());

    if (r == 0.0)
        a = i;
    else
    {
        if (i == 0.0)
            a = r;
        else
        {
            if (r > i)
            {
                t = i / r;
                a = r * sqrt(1.0 + t * t);
            }
            else
            {
                t = r / i;
                a = i * sqrt(1.0 + t * t);
            }
        }
    }

    return a;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>::ComplexNumber():
RealType(TypeWrapper::BOUNDS_CHECKING_BIT),
_Re(0),
_Im(0)
{}

/****************************************************************************/
template <class T>
ComplexNumber<T>::ComplexNumber(const T& Real_, const T& Img_):
RealType(TypeWrapper::BOUNDS_CHECKING_BIT),
_Re(Real_),
_Im(Img_)
{}

/****************************************************************************/
template <class T>
ComplexNumber<T>::ComplexNumber(const ChrString& Str_):
RealType(TypeWrapper::BOUNDS_CHECKING_BIT)
{
  Byte_t Valid_ = IsValue(Str_);

  if (Valid_)
    *this = Str_;
  else
    _Re = _Im = ComplexNumberMember::vNAN;

  if (!Valid_)
  {
    SetAsNullObject();
    SetDataConversionError();    
  }
}

/****************************************************************************/
template <class T>
ComplexNumber<T>::ComplexNumber(const ComplexNumber& Obj_):
RealType(TypeWrapper::BOUNDS_CHECKING_BIT),
_Re(Obj_._Re),
_Im(Obj_._Im)
{}

/****************************************************************************/
template <class T>
const ComplexNumber<T>& ComplexNumber<T>::CNI()
{
  static ComplexNumber<T> Cni_(0.0, 1.0);
  return Cni_;
}

/****************************************************************************/
template <class T>
const ComplexNumber<T>& ComplexNumber<T>::CN1()
{
  static ComplexNumber<T> Cn1_(1.0, 0.0);
  return Cn1_;
}

/****************************************************************************/
template <class T>
const ComplexNumber<T>& ComplexNumber<T>::CNZ()
{
  static ComplexNumber<T> Cnz_(0.0, 0.0);
  return Cnz_;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>* ComplexNumber<T>::Make(const T& Real_, const T& Img_)
{
  return (new ComplexNumber<T>(Real_, Img_));
}

/****************************************************************************/
template <class T>
ComplexNumber<T>* ComplexNumber<T>::Make(const Object& Obj_)
{
  ComplexNumberAcceptor<T> TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
template <class T>
Object& ComplexNumber<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  ComplexNumberAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromComplexNumber(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* ComplexNumber<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new ComplexNumber<T>(*this));

  ComplexNumberAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromComplexNumber(TrgComp_)));
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsEqualToComplexNumber(const ComplexNumber<T>* Ptr_) const
{
  return (_Re == Ptr_->RealVal() &&
          _Im == Ptr_->ImagVal());
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsLesserToComplexNumber(const ComplexNumber<T>* Ptr_) const
{
  return
  (
    sqrt(Sqr(_Re) + Sqr(_Im)) < sqrt(Sqr(Ptr_->RealVal()) + Sqr(Ptr_->ImagVal()))
  );
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsGreaterToComplexNumber(const ComplexNumber<T>* Ptr_) const
{
  return
  (
    sqrt(Sqr(_Re) + Sqr(_Im)) > sqrt(Sqr(Ptr_->RealVal()) + Sqr(Ptr_->ImagVal()))
  );
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ComplexNumberAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToComplexNumber(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ComplexNumberAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToComplexNumber(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ComplexNumberAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToComplexNumber(TrgComp_);
}

/****************************************************************************/
template <class T>
istream& ComplexNumber<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
template <class T>
istream& ComplexNumber<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Re), sizeof(T)).good()))
    if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Im), sizeof(T)).good()))
      SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& ComplexNumber<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Os_.write((char*)(&_Re), sizeof(T));
    if (OstreamCheck(Os_, Ok_))
      Os_.write((char*)(&_Im), sizeof(T));
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& ComplexNumber<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
template <class T>
ByteStream& ComplexNumber<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Re), sizeof(T)).good()))
    if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Im), sizeof(T)).good()))
      SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& ComplexNumber<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Os_.owrite((char*)(&_Re), sizeof(T));
    if (OstreamCheck(((ostream&)Os_), Ok_))
      Os_.owrite((char*)(&_Im), sizeof(T));
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
istream& ComplexNumber<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  ChrString ReData_, PlusSign_, ImData_;

  Is_ >>ReData_ >>PlusSign_;

  if (PlusSign_ == "+")
    Is_ >>ImData_;
  else
    ImData_ = PlusSign_;

  T RealVal_ = ReData_.strtod();
  char* TempStr_ = new_char_string(ImData_.c_str());
  ImData_ = RemoveTrailing(TempStr_, "i");
  ::Delete(TempStr_);
  T ImagVal_ = ImData_.strtod();
    
  SetRealVal(RealVal_);
  SetImagVal(ImagVal_);

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
template <class T>
ostream& ComplexNumber<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Re <<" + " <<_Im <<"i";
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static ComplexNumber<T> _NULLOBJECT(0, 0);
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& ComplexNumber<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ComplexNumber<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& ComplexNumber<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ComplexNumber<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsValue(const ChrString& Str_)
{
  Boolean ImValid_ = FALSE;
  Boolean ReValid_ = FALSE;
  StringToken Tokenizer_;

  Tokenizer_.SetString(Str_.c_str());
  Tokenizer_.SetWhiteSpace(" \t\n\r");
  Tokenizer_.SetDelimiters("+");

  char* Buffer_ = NULL;
  Tokenizer_.FindToken(Buffer_, TRUE);
  ReValid_ = Buffer_ && IsFloat(Buffer_);    
  ::Delete(Buffer_);

  if (ReValid_)
  {
    Tokenizer_.SetWhiteSpace("i \t\n\r");
    Tokenizer_.FindToken(Buffer_, TRUE);
    ImValid_ = Buffer_ && IsFloat(Buffer_);
    ::Delete(Buffer_);
  }

  return (ReValid_ && ImValid_);
}

/****************************************************************************/
template <class T>
ComplexNumber<T> ComplexNumber<T>::ValueOf(const ChrString& Str_)
{
  return ComplexNumber<T>(Str_);
}

/****************************************************************************/
template <class T>
const char* ComplexNumber<T>::GiveHashableData() const
{
  return ToString().c_str();
}

/****************************************************************************/
template <class T>
long ComplexNumber<T>::GiveHashableDataLength() const
{
  return ::SafeStrLen(ToString().c_str());
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator += (const ComplexNumber& Rhs_)
{
  _Re += Rhs_.RealVal();
  _Im += Rhs_.ImagVal();
  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator -= (const ComplexNumber& Rhs_)
{
  _Re -= Rhs_.RealVal();
  _Im -= Rhs_.ImagVal();
  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator *= (const ComplexNumber& Rhs_)
{
  T NewRe_ = (_Re * Rhs_.RealVal()) - (_Im * Rhs_.ImagVal());
  T NewIm_ = (_Re * Rhs_.ImagVal()) + (_Im * Rhs_.RealVal());
  
  _Re = NewRe_;
  _Im = NewIm_;

  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator /= (const ComplexNumber& Rhs_)
{
  ComplexNumber CompConj_(Rhs_);
  
  CompConj_._Im = -CompConj_._Im;
  *this *= CompConj_;
  T Denom_ = Sqr(Rhs_._Re) + Sqr(Rhs_._Im);

  if (Denom_ == T(0))
    SetDivideByZero();
  else
  {
    _Re /= Denom_;
    _Im /= Denom_;
  }

  return *this;
}

/****************************************************************************/
template <class T>    
ComplexNumber<T>& ComplexNumber<T>::operator += (UnsignedIntValue Rhs_)
{
  _Re += Rhs_._Value;
  return *this;
}

/****************************************************************************/
template <class T>    
ComplexNumber<T>& ComplexNumber<T>::operator -= (UnsignedIntValue Rhs_)
{
  _Re -= Rhs_._Value;
  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator *= (UnsignedIntValue Rhs_)
{
  _Re *= Rhs_._Value;
  _Im *= Rhs_._Value;
  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator /= (UnsignedIntValue Rhs_)
{
  _Re /= Rhs_._Value;
  _Im /= Rhs_._Value;
  return *this;
}

/****************************************************************************/
template <class T>    
ComplexNumber<T>& ComplexNumber<T>::operator += (long Rhs_)
{
  _Re += Rhs_;
  return *this;
}

/****************************************************************************/
template <class T>    
ComplexNumber<T>& ComplexNumber<T>::operator -= (long Rhs_)
{
  _Re -= Rhs_;
  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator *= (long Rhs_)
{
  _Re *= Rhs_;
  _Im *= Rhs_;
  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator /= (long Rhs_)
{
  _Re /= Rhs_;
  _Im /= Rhs_;
  return *this;
}

/****************************************************************************/
template <class T>    
int ComplexNumber<T>::GetTypePrec() const
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

/****************************************************************************/
template <class T>    
ComplexNumber<T>& ComplexNumber<T>::Ceiling()
{
  _Re = ceil(_Re);
  _Im = ceil(_Im);
  return *this;
}

/****************************************************************************/
template <class T>    
ComplexNumber<T>& ComplexNumber<T>::Floor()
{
  _Re = floor(_Re);
  _Im = floor(_Im);
  return *this;
}

/****************************************************************************/
template <class T>    
ComplexNumber<T>& ComplexNumber<T>::Round()
{
  if (_Re >= 0)
    _Re = floor(_Re + .5);
  else
    _Re = ceil(_Re - .5);

  if (_Im >= 0)
    _Im = floor(_Im + .5);
  else
    _Im = ceil(_Im - .5);
  
  return *this;
}

/****************************************************************************/
template <class T>
ChrString ComplexNumber<T>::ToString(const ComplexNumber<T>& Dbl_)
{
  return Dbl_.ToString();
}

/****************************************************************************/
template <class T>
ChrString ComplexNumber<T>::ToTruncString(const ComplexNumber<T>& Dbl_)
{
  return Dbl_.ToTruncString();
}

/****************************************************************************/
template <class T>
ChrString ComplexNumber<T>::ToString(int) const
{
  char Buffer_[128];
  int Prec_ = GetTypePrec();
  
  ChrString ReStr_(RemovePadding(FloatToStr(_Re, Buffer_, 127, Prec_), " "));
  ChrString ImStr_(RemovePadding(FloatToStr(_Im, Buffer_, 127, Prec_), " "));
  ChrString RetStr_ = ReStr_ + ChrString(" + ") + ImStr_ + "i";
  return RetStr_;
}

/****************************************************************************/
template <class T>
ChrString ComplexNumber<T>::ToTruncString(int) const
{
  char Buffer_[128];
  ComplexNumber<T> Temp_(*this);
  long Lvalue_;
  int Prec_ = GetTypePrec();  

  Temp_.Floor();
  Lvalue_ = (long)Temp_.RealVal();
  ChrString ReStr_(RemovePadding(FloatToStr(Lvalue_, Buffer_, 127, Prec_), " "));
  Lvalue_ = (long)Temp_.ImagVal();
  ChrString ImStr_(RemovePadding(FloatToStr(Lvalue_, Buffer_, 127, Prec_), " "));
  ChrString RetStr_ = ReStr_ + ChrString(" + ") + ImStr_ + "i";
  return RetStr_;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::FromString(const ChrString& Str_)
{
  ComplexNumber<T> Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
template <class T>
long ComplexNumber<T>::DataSize () const
{
  return (sizeof(T) * 2);
}

/****************************************************************************/
template <class T>
int ComplexNumber<T>::ClassID() const
{
  return TypeInfo::COMPLEXNUMBER_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* ComplexNumber<T>::GiveObjectAcceptor() const
{
  return (new ComplexNumberAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsNan() const
{
  return
  (
    _Re == ComplexNumber<T>::vNAN ||
    _Im == ComplexNumber<T>::vNAN
  );
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::IsInfinite() const
{
  return
  (
    _Re == ComplexNumber<T>::vINFINITY ||
    _Im == ComplexNumber<T>::vINFINITY
  );
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::AtZero() const
{
  return (!_Im && !_Re);
}

/****************************************************************************/
template <class T>
Boolean ComplexNumber<T>::operator ! () const
{
  return (!_Im && !_Re);
}

/****************************************************************************/
template <class T>
ComplexNumber<T> ComplexNumber<T>::operator - () const
{
  ComplexNumber Cn_(*this);
  Cn_._Re = -Cn_._Re;
  Cn_._Im = -Cn_._Im;
  return Cn_;
}

/****************************************************************************/
template <class T>
ComplexNumber<T> ComplexNumber<T>::operator + () const
{
  ComplexNumber Cn_(*this);
  return Cn_;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator = (const ComplexNumber& Obj_)
{
  if (this != &Obj_)
  {
    _Im = Obj_._Im;
    _Re = Obj_._Re;
  }

  return *this;
}

/****************************************************************************/
template <class T>
ComplexNumber<T>& ComplexNumber<T>::operator = (const ChrString& Str_)
{
  Boolean Valid_ = IsValue(Str_);
  ChrString Token_;
  StringToken Tokenizer_;
  
  if (Valid_)
  {
    Tokenizer_.SetString(Str_.c_str());
    Tokenizer_.SetWhiteSpace(" \t\n\r");
    Tokenizer_.SetDelimiters("+");

    char* Buffer_ = NULL;
    Tokenizer_.FindToken(Buffer_, TRUE);
    Valid_ = Buffer_ && IsFloat(Buffer_);
    
    if (Valid_)
    {
      Token_ = Buffer_;
	  #if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
        _Re = (double&)Token_.strtod();
	  #else
        _Re = Token_.strtod();
	  #endif
    }
    else
      _Re = -HUGE_VAL;

    ::Delete(Buffer_);

    if (Valid_)
    {
      Tokenizer_.SetWhiteSpace("i \t\n\r");
      Tokenizer_.FindToken(Buffer_, TRUE);
      Valid_ = Buffer_ && IsFloat(Buffer_);

      if (Valid_)
      {
        Token_ = Buffer_;
  	    #if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
          _Im = (double&)Token_.strtod();
	    #else
          _Im = Token_.strtod();
	    #endif
      }
      else
        _Im = -HUGE_VAL;

      ::Delete(Buffer_);
    }
  }

  if (!Valid_)
  {
    _Re = _Im = ComplexNumberMember::vNAN;
    SetAsNullObject();
    SetDataConversionError();
  }

  return *this;
}

/****************************************************************************/
template <class T>
void ComplexNumber<T>::SetRealVal(const T& Obj_)
{
  _Re = Obj_;
}

/****************************************************************************/
template <class T>
void ComplexNumber<T>::SetImagVal(const T& Obj_)
{
  _Im = Obj_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ComplexNumber<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ComplexNumber<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* ComplexNumber<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ComplexNumber<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
ComplexNumberAcceptor<T>::ComplexNumberAcceptor(const ComplexNumber<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
Boolean ComplexNumberAcceptor<T>::IsComplexNumber() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean ComplexNumberAcceptor<T>::IsTypeWrapper() const
{
  return FALSE;
}

/****************************************************************************/
template <class T>
void ComplexNumberAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const ComplexNumber<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* ComplexNumberAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
void* ComplexNumberAcceptor<T>::CreateFromComplexNumber(const ObjectAcceptor& Obj_)
{
  _VarPtr = new ComplexNumber<T>(*((const ComplexNumber<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* ComplexNumberAcceptor<T>::CreateFromChrString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new ComplexNumber<T>(*((const ChrString*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* ComplexNumberAcceptor<T>::AssignFromComplexNumber(const ObjectAcceptor& Obj_)
{
  const ComplexNumber<T>* TrgPtr_ = (const ComplexNumber<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* ComplexNumberAcceptor<T>::AssignFromChrString(const ObjectAcceptor& Obj_)
{
  const ChrString* StrPtr_ = (const ChrString*)Obj_.GiveDataPtr();
  *_VarPtr = *StrPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean ComplexNumberAcceptor<T>::IsEqualToComplexNumber(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToComplexNumber((const ComplexNumber<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean ComplexNumberAcceptor<T>::IsLesserToComplexNumber(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToComplexNumber((const ComplexNumber<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean ComplexNumberAcceptor<T>::IsGreaterToComplexNumber(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToComplexNumber((const ComplexNumber<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ComplexNumberAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ComplexNumberAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if COMPLEX_DEBUG
void TestComplex(ostream & buffer)
{
    buffer << "Testing Complex\r\n"
              "===============\r\n\r\n";

    ComplexNumber<double> one(1.0, 0);
    ComplexNumber<double> a (2.0, 3.0);
    ComplexNumber<double> b (4.0, 5.0);
    ComplexNumber<double> c, d;

    buffer << "      a = " << a << "\r\n";
    buffer << "      b = " << b << "\r\n";

    buffer << "\r\n";
    d = a + one;
    buffer << "a + one = " << d << "\r\n";

    d = a - one;
    buffer << "a - one = " << d << "\r\n";

    d = a * one;
    buffer << "a * one = " << d << "\r\n";

    d = a / one;
    buffer << "a / one = " << d << "\r\n";

    d = a + 2.0;
    buffer << "a + 2.0 = " << d << "\r\n";
    
    d = a - 2.0;
    buffer << "a - 2.0 = " << d << "\r\n";
    
    d = a * 2.0;
    buffer << "a * 2.0 = " << d << "\r\n";
    
    d = a / 2.0;
    buffer << "a / 2.0 = " << d << "\r\n";
    
    d = 2.0 + a;
    buffer << "2.0 + a = " << d << "\r\n";
    
    d = ComplexNumber<double>(2.0, 0) - a;
    buffer << "2.0 - a = " << d << "\r\n";
    
    d = ComplexNumber<double>(2.0, 0) * a;
    buffer << "2.0 * a = " << d << "\r\n";
    
    d = ComplexNumber<double>(2.0, 0) / a;
    buffer << "2.0 / a = " << d << "\r\n";
    
    d = d * a;
    buffer << "  d * a = " << d << "\r\n\r\n";

    d = a + b;
    buffer << "  a + b = " << d << "\r\n";
    
    d = a - b;
    buffer << "  a - b = " << d << "\r\n";
    
    d = a * b;
    buffer << "  a * b = " << d << "\r\n";
    
    d = a / b;
    buffer << "  a / b = " << d << "\r\n";

    d = d * b;
    buffer << "  d * b = " << d << "\r\n\r\n";

    a = ComplexNumber<double>(2.0,3.0);
    b = ComplexNumber<double>(4.0,5.0);

    c = a; c += b;
    buffer << " a += b = " << c << "\r\n";

    c = a; c -= b;
    buffer << " a -= b = " << c << "\r\n";

    c = a; c *= b;
    buffer << " a *= b = " << c << "\r\n";

    c = a; c /= b;
    buffer << " a /= b = " << c << "\r\n";

    c = a; c += 2.0;
    buffer << " a += 2 = " << c << "\r\n";

    c = a; c -= 2.0;
    buffer << " a -= 2 = " << c << "\r\n";

    c = a; c *= 2.0;
    buffer << " a *= 2 = " << c << "\r\n";

    c = a; c /= 2.0;
    buffer << " a /= 2 = " << c << "\r\n\r\n";

    d = -a;
    buffer << "     -a = " << d << "\r\n";
    
    d = conj(a);
    buffer << "conj(a) = " << d << "\r\n";

    d = ComplexNumber<double>(norm(a));
    buffer << "norm(a) = " << d << "\r\n";

    d = ComplexNumber<double>(abs(a));
    buffer << " abs(a) = " << d << "\r\n";

    d = ComplexNumber<double>(arg(a));
    buffer << " arg(a) = " << d << "\r\n\r\n";

    // trigonometry tests
    d = tan(a);
    buffer << " tan(a) = " << d << "\r\n";

    d = sin(a);
    buffer << " sin(a) = " << d << "\r\n";

    c = cos(a);
    buffer << " cos(a) = " << c << "\r\n";

    c = c * c + d * d;
    buffer << "sin * sin + cos * cos = "
           << c << "\r\n\r\n";

    d = ComplexNumber<double>(N_DPI3, N_DPI);
    buffer << "      d = " << d << "\r\n";

    c = tan(d);
    buffer << " tan(d) = " << c << "\r\n";

    c = atan(c);
    buffer << "atan(c) = " << c << "\r\n\r\n";

    double dt = atan(1.0);
    ComplexNumber <double> ct = atan(ComplexNumber<double>(1.0,0.0));

    buffer << "     dt = " << dt << "\r\n";
    buffer << "     ct = " << ct << "\r\n\r\n";

    d = ComplexNumber<double>(N_DPI3,N_DPI);
    buffer << "      d = " << d << "\r\n";

    c = sin(d);
    buffer << " sin(d) = " << c << "\r\n";

    c = asin(c);
    buffer << "asin(c) = " << c << "\r\n\r\n";

    d = ComplexNumber<double>(N_DPI3,N_DPI);
    buffer << "      d = " << d << "\r\n";
    
    c = cos(d);
    buffer << " cos(d) = " << c << "\r\n";
    
    c = acos(c);
    buffer << "acos(c) = " << c << "\r\n";

    double          qd = tanh(N_DPI);
    ComplexNumber<double> qc = tanh(ComplexNumber<double>(N_DPI));

    buffer << "\r\n" << qd << " should equal " << qc << "\r\n";

    qd = atanh(qd);
    qc = atanh(qc);
    buffer << qd << " should equal " << qc << "\r\n";

    ComplexNumber<double> qx(0.5,0.3);
    buffer << "qx = " << qx << "\r\n";

    qc = tanh(qx);
    buffer << "qc = " << qc << "\r\n";
    
    qc = atanh(qc);
    buffer << "qc = " << qc << "\r\n";

    a = ComplexNumber<double>(0.5,0.3);
    buffer << "\r\n      a  = " << a << "\r\n";

    b = tanh(a);
    buffer << " tanh(a) = " << b << "\r\n";
    
    c = sinh(a);
    buffer << " sinh(a) = " << c << "\r\n";

    d = cosh(a);
    buffer << " cosh(a) = " << d << "\r\n";

    b = atanh(b);
    buffer << "atanh(b) = " << b << "\r\n";

    c = asinh(c);
    buffer << "asinh(c) = " << c << "\r\n";

    d = acosh(d);
    buffer << "acosh(d) = " << d << "\r\n\r\n";

    // logarithm tests
    a = ComplexNumber<double>(4.0);
    buffer << "      a = " << a << "\r\n";

    b = ComplexNumber<double>(-1.0);
    buffer << "      b = " << b << "\r\n";

    c = ComplexNumber<double>(3.14,15.92);
    buffer << "      c = " << c << "\r\n";

    d = sqrt(a);
    buffer << "sqrt(a) = " << d << "\r\n";

    d = sqrt(b);
    buffer << "sqrt(b) = " << d << "\r\n";

    d = sqrt(c);
    buffer << "sqrt(c) = " << d << "\r\n";

    d *= d;
    buffer << "  d * d = " << d << "\r\n";

    d = pow(a,0.5);
    buffer << "pow(a,0.5) = " << d << "\r\n";

    d = log(c);
    buffer << " log(a) = " << d << "\r\n";

    d = exp(d);
    buffer << " exp(d) = " << d << "\r\n";

    d = exp(ComplexNumber<double>(1.0));
    buffer << " exp(1) = " << d << "\r\n";

    d = log(d);
    buffer << " log(e) = " << d << "\r\n";

    d = log10(ComplexNumber<double>(10.0));
    buffer << "   log10(10) = " << d << "\r\n";

    d = log10(ComplexNumber<double>(217.3));
    buffer << "log10(217.3) = " << d << "\r\n";


    a = ComplexNumber<double>(N_DPI,N_DE); // value with lotsa digits
    b = SigDig(a,8);
    c = SigDig(a,2);

    buffer << setprecision(15);
    buffer << "      a = " << a << "\r\n";
    buffer << "      b = " << b << "\r\n";
    buffer << "      c = " << c << "\r\n";
    buffer << setprecision(6) << "\r\n";

    // conversion tests
//    ComplexNumber<float> fa(a);
//    ComplexNumber<long double> la(fa);
//    buffer << "     fa = " << fa << "\r\n";
//    buffer << "     la = " << la << "\r\n";
}


int main()
{
  ComplexNumber<double> Cn0;
  ComplexNumber<double> Cn1(1, -1);
  ComplexNumber<double> Cn2(4, 7);
  ComplexNumber<double> Cn3(-1, 3);
  ComplexNumber<double> Cn4(2, -5);
  ComplexNumber<double> Cn5(2, 5);

  Cn0 = Cn1 + Cn2;
  assert(Cn0.RealVal() == 5);
  assert(Cn0.ImagVal() == 6);  
  Cn0 = Cn1 - Cn2;
  assert(Cn0.RealVal() == -3);
  assert(Cn0.ImagVal() == -8);
  
  Cn0 = Cn3 * Cn4;
  assert(Cn0.RealVal() == 13);
  assert(Cn0.ImagVal() == 11);
  Cn0 = Cn3 / Cn5;
  assert(Cn0.RealVal() == 13.0/29);
  assert(Cn0.ImagVal() == 11.0/29);

///
  ChrString Str_;

  ComplexNumber<double> si(315, 247);
  Str_ = si.ToString();
  assert(!strncmp(Str_.c_str(), "315 + 247i", 10));

  ComplexNumber<double> si2(315, 247);
  ComplexNumber<double> si3(-210, 171);
  Str_ = si3.ToString();
  assert(!strncmp(Str_.c_str(), "-210 + 171i", 11));

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

  ComplexNumber<double> si4;
  si4 = si3 * 1000;  
  assert(si4.RealVal() == 315000);
  assert(si4.ImagVal() == 247000);

  si2 = "abc";
  assert(si2.IsNullObject());
  assert(si2.DataConversionError());
  ChrString strobj_ = "568 + 123i";
  si2.Assign(strobj_);
  cout <<si2 <<endl;

  si2 += 100;
  cout <<si2 <<endl;
  si2 -= 200;
  cout <<si2 <<endl;
  si2 *= 2;
  cout <<si2 <<endl;
  si2 /= 2;
  cout <<si2 <<endl;

#if COMPLEX_TESTINPUT
  cout <<"Enter a number: ";
  cin >>si;
  cout <<si <<endl;
#endif

  TestComplex(cout);
  cout <<endl <<"SUCCESS Testing <ComplexNumber>" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif




