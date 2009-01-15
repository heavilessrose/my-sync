#ifndef BITSTRING_CPP
#define BITSTRING_CPP
#ifndef BITSTRING_H
  #include "bitstr.h"
#endif

static const size_t ULOBITS = BITS_BYTE * sizeof(unsigned long);
static const unsigned long UB0 = 1UL << (ULOBITS - 1);

static const BitString zeros(BitString::_Zeros);
static const BitString ones(BitString::_Ones);

/****************************************************************************/
Boolean BitString::Grow(size_t Num_, Boolean Trim_)
{
  size_t Osize_ = (_Ptr == 0) ? 0:_Res;

  if (Num_ == 0)
  {
    const size_t MIN_SIZE = _Nb;
    if (Trim_ && MIN_SIZE < Osize_)
      Tidy(1);

    _Len = 0;
    return 0;
  }
  else if (Num_ == Osize_ || Num_ < Osize_ && !Trim_)
    return 1;
  else
  {
    size_t Size_ = _Ptr == 0 && Num_ < _Res ? _Res:Num_;
    Type* S_;

    Boolean Active_ = MemMatrix::Matrix().IsNewHandlerActive();
    MemMatrix::Matrix().SetNewHandlerInactive();

#if OVERLOAD_NEW
    if ((S_ = (Type*)MemMatrix::Matrix().Reallocate(_Ptr, Bytes(Size_))) == 0)
      if ((S_ = (Type*)MemMatrix::Matrix().Reallocate(_Ptr, Bytes(Num_))) == 0)
        MemMatrix::Matrix().NoMemory();
      else
        Size_ = Num_;
#else
    if ((S_ = (Type*)realloc(_Ptr, Bytes(Size_))) == 0)
      if ((S_ = (Type*)realloc(_Ptr, Bytes(Num_))) == 0)
        MemMatrix::Matrix().NoMemory();
      else
        Size_ = Num_;
#endif

    if (Active_)
      MemMatrix::Matrix().SetNewHandlerActive();

    _Ptr = S_;

    if ((_Res = Bytes(Size_) * BITS_BYTE) == 0)
      _Res = SIZET_MAX - 1;

    return 1;
  }
}

/****************************************************************************/
void BitString::Tidy(Boolean Constructed_)
{
#if OVERLOAD_NEW
  if (Constructed_ && _Ptr != 0)
    MemMatrix::Matrix().Deallocate(_Ptr);
#else
  if (Constructed_ && _Ptr != 0)
    free(_Ptr);
#endif

  _Ptr = 0, _Src = 0, _Len = 0, _Res = 0;
}

/****************************************************************************/
BitString::BitString(Ulong ulo, size_t n):
ObjectImp()
{
  if (n == SIZET_MAX)
    Xlen();

  Tidy();

  size_t ns;
  if (ulo == 0)
    ns = 0;
  else
    for (ns = ULOBITS; ulo < UB0; --ns)
      ulo <<= 1;

  if (n < ns)
    n = ns;

  if (Grow(n))
  {
    for (int i = (n - 1) / _Nb; 0 <= i; --i)
      _Ptr[i] = 0;

    _Len = n;

    for (size_t pos = 0; ulo != 0; ulo <<= 1, ++pos)
      if (ulo & UB0)
	Set(pos);
  }
}

/****************************************************************************/
BitString::BitString(const ChrString& Str_, size_t Pos_, size_t Ns_):
ObjectImp()
{
  if (Str_.strlen() < Pos_)
    Xran();

  Tidy();

  size_t n = Str_.strlen() - Pos_;

  if (Ns_ < n)
    n = Ns_;

  if (Grow(n))
  {
    for (int i = (n - 1) / _Nb; 0 <= i; --i)
      _Ptr[i] = 0;

    _Len = n;

    for (size_t p = 0; p < n; ++p, ++Pos_)
    {
      if (Str_[Pos_] == '1')
	Set(p);
      else if (Str_[Pos_] != '0')
	Xinv();
    }
  }
}

/****************************************************************************/
BitString* BitString::Make()
{
  return (new BitString);
}

/****************************************************************************/
BitString* BitString::Make(Ulong Val_, size_t n)
{
  return (new BitString(Val_, n));
}

/****************************************************************************/
BitString* BitString::Make(const ChrString& Str_, size_t Pos_, size_t Len_)
{
  return (new BitString(Str_, Pos_, Len_));
}

/****************************************************************************/
BitString* BitString::Make(const BitString& Str_, size_t Pos_, size_t Len_)
{
  return (new BitString(Str_, Pos_, Len_));
}

/****************************************************************************/
BitString* BitString::Make(_Source S_)
{
  return (new BitString(S_));
}

/****************************************************************************/
BitString* BitString::Make(const Object& Obj_)
{
  BitStringAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
const char* BitString::GiveHashableData() const
{
  return To_String().c_str();
}

/****************************************************************************/
long BitString::GiveHashableDataLength() const
{
  return Length();
}

/****************************************************************************/
Object& BitString::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  BitStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromBitString(TrgComp_)));
}

/****************************************************************************/
Object* BitString::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new BitString(*this));

  BitStringAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromBitString(TrgComp_)));
}

/****************************************************************************/
BitString& BitString::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static BitString _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& BitString::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return BitString::GiveNullObject();
}

/****************************************************************************/
Object& BitString::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return BitString::GiveNullObject();
}

/****************************************************************************/
long BitString::DataSize () const
{
  return (sizeof(Type) * _Len);
}

/****************************************************************************/
int BitString::ClassID() const
{
  return TypeInfo::BITSTRING_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* BitString::GiveObjectAcceptor() const
{
  return (new BitStringAcceptor(this));
}

/****************************************************************************/
istream& BitString::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long StrLen_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    if (Is_.read((char*)(&StrLen_), sizeof(long)).good())
      if (Is_.seekg(StrLen_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& BitString::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long StrLen_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
    if (Is_.read((char*)(&StrLen_), sizeof(long)).good())
    {
      if (_Len < StrLen_)
	Resize(StrLen_);

      Reset();
      if (Is_.read((char*)_Ptr, StrLen_).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& BitString::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    long StrLen_ = DataSize();
    Os_.write((char*)(&StrLen_), sizeof(long));

    if (OstreamCheck(Os_, Ok_) && _Ptr)
      Os_.write((char*)_Ptr, StrLen_);
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& BitString::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long StrLen_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    if (Is_.iread((char*)(&StrLen_), sizeof(long)).good())
      if (Is_.iseekg(StrLen_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& BitString::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long StrLen_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    if (Is_.iread((char*)(&StrLen_), sizeof(long)).good())
    {
      if (_Len < StrLen_)
	Resize(StrLen_);

      Reset();
      if (Is_.iread((char*)_Ptr, StrLen_).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& BitString::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    long StrLen_ = DataSize();
    Os_.owrite((char*)(&StrLen_), sizeof(long));

    if (OstreamCheck(((ostream&)Os_), Ok_) && _Ptr)
      Os_.owrite((char*)_Ptr, StrLen_);
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& BitString::TextRead(istream& Is_, Boolean* Ok_)
{
  *this = _Bitsxstr(Is_, SIZET_MAX - 1);
  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& BitString::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<To_String();
  return ObjectImp::TextWrite(Os_, Ok_);;
}

/****************************************************************************/
Boolean BitString::IsEqualToBitString(const BitString* Str_) const
{
  if (!Str_ || _Len != Str_->Length())
    return 0;
  else if (_Len == 0)
    return 1;
  else
  {
    for (int i = (_Len - 1) / _Nb; 0 <= i; --i)
      if (_Ptr[i] != Str_->_W(i))
	return 0;

    return 1;
  }
}

/****************************************************************************/
Boolean BitString::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  BitStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToBitString(TrgComp_);
}

/****************************************************************************/
BitString& BitString::operator = (const ChrString& Str_)
{
  BitString StrObj_(Str_);
  return (*this = StrObj_);
}

/****************************************************************************/
BitString& BitString::operator = (_Source S_)
{
  BitString SrcObj_(S_);
  return (*this = SrcObj_);
}

/****************************************************************************/
BitString& BitString::operator = (const BitString& Rhs_)
{
  AssignSubstr(Rhs_);
  ObjectImp::operator = ((Object&)Rhs_);
  return *this;
}

/****************************************************************************/
BitString& BitString::operator &= (const BitString& Str_)
{
  size_t m, n;

  if (Str_.Length() < _Len)
    m = Str_.Length(), n = _Len;
  else
    m = _Len, n = Str_.Length();

  if (Grow(n))
  {
    int mw = (m == 0) ? -1:((m - 1) / _Nb);
    int nw = (n - 1) / _Nb;

    for (int i = 0; i <= mw; ++i)
      _Ptr[i] &= Str_._W(i);

    while (++mw <= nw)
      _Ptr[mw] = 0;

    _Len = n;
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::operator |= (const BitString& Str_)
{
  size_t m, n;

  if (Str_.Length() < _Len)
    m = Str_.Length(), n = _Len;
  else
    m = _Len, n = Str_.Length();

  if (Grow(n))
  {
    int mw = (m == 0) ? -1:((m - 1) / _Nb);
    int nw = (n - 1) / _Nb;

    for (int i = 0; i <= mw; ++i)
      _Ptr[i] |= Str_._W(i);

    if (_Len < n)
      while (++mw <= nw)
	_Ptr[mw] = Str_._W(mw);

    _Len = n;
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::operator ^= (const BitString& Str_)
{
  size_t m, n;

  if (Str_.Length() < _Len)
    m = Str_.Length(), n = _Len;
  else
    m = _Len, n = Str_.Length();

  if (Grow(n))
  {
    int mw = (m == 0) ? -1:((m - 1) / _Nb);
    int nw = (n - 1) / _Nb;

    for (int i = 0; i <= mw; ++i)
      _Ptr[i] ^= Str_._W(i);

    if (_Len < n)
      while (++mw <= nw)
	_Ptr[mw] = Str_._W(mw);

    _Len = n;
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::operator <<= (size_t Pos_)
{
  if (Pos_ < 0)
    *this >>= -Pos_;
  else if (0 < _Len && 0 < Pos_)
    Copylr(*this, 0, Pos_, _Len);

  return *this;
}

/****************************************************************************/
BitString& BitString::operator >>= (size_t Pos_)
{
  if (Pos_ < 0)
    *this <<= -Pos_;
  else if (0 < _Len && 0 < Pos_)
  {
    Copyrl(*this, Pos_, 0, _Len);
    Copylr(zeros, 0, Pos_, Pos_);
  }

  return *this;
}

/****************************************************************************/
void BitString::Copylr(const BitString& Str_, size_t p0, size_t Pos_, size_t n)
{
  if (0 < n)
  {
    const int nb = n % _Nb;
    const int nl = (p0 == 0) ? -1:((p0 - 1) / _Nb);
    const int np = Pos_ / _Nb;
    const int nw = (n - 1) / _Nb;
    const int off = np - nl - 1;

    Type mask, save;

    if (nb != 0)
      mask = ~(Type)0 >> nb, save = _Ptr[nw];

    Pos_ %= _Nb;

    if ((p0 %= _Nb) == 0)
      for (int i = nl; ++i <= nw; )
	_Ptr[i] = Str_._X(i + off, Pos_);
    else
    {
      _Ptr[nl] = _Ptr[nl] & ~(~(Type)0 >> p0) | Str_._X(np, Pos_) >> p0;

      for (int i = nl; ++i <= nw; )
	_Ptr[i] = Str_._X(i + off, Pos_) << (_Nb - p0) |
		  Str_._X(i + off + 1, Pos_) >> p0;
    }

    if (nb != 0)
      _Ptr[nw] = _Ptr[nw] & ~mask | save & mask;
  }
}

/****************************************************************************/
void BitString::Copyrl(const BitString& Str_, size_t p0, size_t Pos_, size_t n)
{
  if (0 < n)
  {
    const int nb = n % _Nb;
    const int nl = (p0 == 0) ? -1:((p0 - 1) / _Nb);
    const int np = Pos_ / _Nb;
    const int nw = (n - 1) / _Nb;
    const int off = np - nl - 1;

    Type mask, save;

    if (nb != 0)
      mask = ~(Type)0 >> nb, save = _Ptr[nw];

    Pos_ %= _Nb;

    if ((p0 %= _Nb) == 0)
      for (int i = nw; nl < i; --i)
	_Ptr[i] = Str_._X(i + off, Pos_);
    else
    {
      for (int i = nw; nl < i; --i)
	_Ptr[i] = Str_._X(i + off, Pos_) << (_Nb - p0) |
		  Str_._X(i + off + 1, Pos_) >> p0;

      _Ptr[nl] = _Ptr[nl] & ~(~(Type)0 >> p0) | Str_._X(np, Pos_) >> p0;
    }

    if (nb != 0)
      _Ptr[nw] = _Ptr[nw] & ~mask | save & mask;
  }
}

/****************************************************************************/
BitString& BitString::Append(const BitString& Str_, size_t Pos_, size_t ns)
{
  if (Str_.Length() < Pos_)
    Xran();

  size_t n = Str_.Length() - Pos_;

  if (n < ns)
    ns = n;

  if (SIZET_MAX - _Len <= ns)
    Xlen();

  if (0 < ns)
  {
    n = _Len + ns;
    Grow(n);
    Copylr(Str_, _Len, Pos_, n);
    Setl(n);
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::AssignSubstr(const BitString& Str_, size_t Pos_, size_t ns)
{
  if (Str_.Length() < Pos_)
    Xran();

  size_t n = Str_.Length() - Pos_;

  if (ns < n)
    n = ns;

  if (this == &Str_)
    Remove(Pos_ + n), Remove(0, Pos_);
  else if (Grow(n, 1))
  {
    Copylr(Str_, 0, Pos_, n);
    Setl(n);
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::Insert(size_t p0, const BitString& Str_, size_t Pos_, size_t ns)
{
  if (_Len < p0 || Str_.Length() < Pos_)
    Xran();

  size_t n = Str_.Length() - Pos_;

  if (n < ns)
    ns = n;

  if (SIZET_MAX - _Len <= ns)
    Xlen();

  if (0 < ns && Grow(n = _Len + ns))
  {
    Copyrl(*this, p0 + ns, p0, n);
    Copylr(Str_, p0, Pos_, p0 + ns);
    Setl(n);
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::Remove(size_t p0, size_t nr)
{
  if (_Len < p0)
    Xran();

  if (_Len - p0 < nr)
    nr = _Len - p0;

  if (0 < nr)
  {
    const int n = _Len - nr;

    Copylr(*this, p0, p0 + nr, n);

    if (Grow(n))
      Setl(n);
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::Replace(size_t p0, size_t n0,
			      const BitString& Str_, size_t Pos_, size_t ns)
{
  if (_Len < p0 || Str_.Length() < Pos_)
    Xran();

  size_t n = Str_.Length() - Pos_;

  if (n < ns)
    ns = n;

  if (SIZET_MAX - ns <= _Len - n0)
    Xlen();

  n = _Len + ns - n0;

  if (ns  < n0)
    Copylr(*this, p0 + ns, p0 + n0, n);

  if ((0 < ns || 0 < n0) && Grow(n))
  {
    if (n0 < ns)
      Copyrl(*this, p0 + ns, p0 + n0, n);

    Copylr(Str_, p0, Pos_, p0 + ns);
    Setl(n);
  }

  return *this;
}

/****************************************************************************/
size_t BitString::Resize(size_t n, Boolean Val_)
{
  const size_t Len_ = _Len;

  if (n == SIZET_MAX)
    Xlen();

  if (Grow(n, 1))
  {
    if (_Len < n)
      Copylr(Val_ != 0 ? ones:zeros, _Len, 0, n);

    Setl(n);
  }

  return Len_;
}

/****************************************************************************/
BitString& BitString::Set()
{
  if (0 < _Len)
  {
    for (int i = (_Len - 1) / _Nb; 0 <= i; --i)
      _Ptr[i] = ~(Type)0;

    Setl(_Len);
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::Set(size_t p0, Boolean Val_)
{
  if (_Len < p0)
    Xran();

  if (_Len == p0)
  {
    Grow(_Len + 1);
    Setl(_Len + 1);
  }

  if (Val_ != 0)
    _Ptr[p0 / _Nb] |= (Type)1 << (_Nb - 1 - p0 % _Nb);
  else
    _Ptr[p0 / _Nb] &= ~((Type)1 << (_Nb - 1 - p0 % _Nb));

  return *this;
}

/****************************************************************************/
BitString& BitString::Reset()
{
  if (0 < _Len)
    for (int i = (_Len - 1) / _Nb; 0 <= i; --i)
      _Ptr[i] = 0;

  return *this;
}

/****************************************************************************/
BitString& BitString::Toggle()
{
  if (0 < _Len)
  {
    for (int i = (_Len - 1) / _Nb; 0 <= i; --i)
      _Ptr[i] ^= ~(Type)0;

    Setl(_Len);
  }

  return *this;
}

/****************************************************************************/
BitString& BitString::Toggle(size_t p0)
{
  if (_Len < p0)
    Xran();

  _Ptr[p0 / _Nb] ^= (Type)1 << (_Nb - 1 - p0 % _Nb);
  return *this;
}

/****************************************************************************/
size_t BitString::Count() const
{
  size_t Val_ = 0;

  if (0 < _Len)
  {
    for (int i = (_Len - 1) / _Nb; 0 <= i; --i)
      for (Type x = _Ptr[i]; x != 0; x >>= 4)
	Val_ += "\0\1\1\2\1\2\2\3"
		"\1\2\2\3\2\3\3\4"[x & 0xF];
  }

  return Val_;
}

/****************************************************************************/
ChrString BitString::To_String() const
{
  ChrString Str_('\0', _Len);

  for (size_t Pos_ = 0; Pos_ < _Len; Pos_++)
    Str_ += Test(Pos_) ? "1":"0";

  return Str_;
}

/****************************************************************************/
size_t BitString::Find(Boolean Val_, size_t p0, size_t n) const
{
  static const Type MSB = (Type)1 << (_Nb - 1);
  size_t pf =
	(_Len <= p0) 	? 0:
	(_Len - p0 < n) ? _Len:
			  p0 + n;

  if (p0 < pf)
  {
    const int nw = pf / _Nb;
    int i = p0 / _Nb;
    size_t Pos_ = i * _Nb;

    if (Val_)
      for (;i <= nw; ++i, Pos_ += _Nb)
      {
	Type x = _Ptr[i];

	for (size_t j = Pos_; x != 0; x <<= 1, ++j)
	  if (x & MSB && p0 <= j && j < pf)
	    return j;
      }
    else
      for (;i <= nw; ++i, Pos_ += _Nb)
      {
	Type x = _Ptr[i];

	for (size_t j = Pos_; ~x != 0; x = x << 1 | 1, ++j)
	  if (!(x & MSB) && p0 <= j && j < pf)
	    return j;
      }
  }

  return SIZET_MAX;
}

/****************************************************************************/
size_t BitString::Rfind(Boolean Val_, size_t p0, size_t n) const
{
  static const Type MSB = (Type)1 << (_Nb - 1);
  size_t pf =
	(_Len <= p0) 	? 0:
	(_Len - p0 < n) ? _Len:
			  p0 + n;

  if (p0 < pf)
  {
    const int nw = p0 / _Nb;
    int i = pf / _Nb;
    size_t Pos_ = i * _Nb + _Nb - 1;

    if (Val_)
      for (;nw <= i; --i, Pos_ -= _Nb)
      {
	Type x = _Ptr[i];

	for (size_t j = Pos_; x != 0; x >>= 1, --j)
	  if (x & 1 && p0 <= j && j < pf)
	    return j;
      }
    else
      for (;nw <= i; --i, Pos_ -= _Nb)
      {
	Type x = _Ptr[i];

	for (size_t j = Pos_; ~x != 0; x = x >> 1 | MSB, --j)
	  if (!(x & 1) && p0 <= j && j < pf)
	    return j;
      }
  }

  return SIZET_MAX;
}

/****************************************************************************/
Boolean BitString::operator == (const BitString& Str_) const
{
  if (_Len != Str_.Length())
    return 0;
  else if (_Len == 0)
    return 1;
  else
  {
    for (int i = (_Len - 1) / _Nb; 0 <= i; --i)
      if (_Ptr[i] != Str_._W(i))
	return 0;

    return 1;
  }
}

/****************************************************************************/
Boolean BitString::Test(size_t p0) const
{
  if (_Len <= p0)
    Xran();

  return ((_Ptr[p0 / _Nb] & (Type)1 << (_Nb - 1 - p0 % _Nb)) != 0);
}

/****************************************************************************/
Boolean BitString::Any() const
{
  if (_Len == 0)
    return 0;
  else
  {
    for (int i = (_Len - 1) / _Nb; 0 <= i; --i)
      if (_Ptr[i] != 0)
	return 1;

    return 0;
  }
}

/****************************************************************************/
#if OVERLOAD_NEW
void* BitString::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void BitString::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* BitString::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void BitString::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
BitStringAcceptor::BitStringAcceptor(const BitString* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
// Data transfer methods
void BitStringAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const BitString*)Ptr_;
}

/****************************************************************************/
const void* BitStringAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
// BitString class specific acceptor methods
Boolean BitStringAcceptor::IsBitString() const
{
  return TRUE;
}

/****************************************************************************/
void* BitStringAcceptor::CreateFromBitString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new BitString(*((const BitString*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* BitStringAcceptor::CreateFromChrString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new BitString(*((const ChrString*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* BitStringAcceptor::AssignFromBitString(const ObjectAcceptor& Obj_)
{
  const BitString* TrgPtr_ = (const BitString*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
void* BitStringAcceptor::AssignFromChrString(const ObjectAcceptor& Obj_)
{
  const ChrString* StrPtr_ = (const ChrString*)Obj_.GiveDataPtr();
  *_VarPtr = *StrPtr_;
  return _VarPtr;
}

/****************************************************************************/
Boolean BitStringAcceptor::IsEqualToBitString(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToBitString((const BitString*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
void* BitStringAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void BitStringAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if BITSTRING_DEBUG
int main()
{
  // Test basic workings of BitString definitions
  ChrString Str_("11110101");
  BitString x0, x1(0x1230, 1), x2(x1), x3(x1, 1, 8);
  BitString x4(Str_), x5(Str_, 4, 3);
  assert(x0.Length() == 0 && x0.To_String() == ChrString(""));
  assert(x1.Length() == 13 &&
	 x1.To_String() == ChrString("1001000110000"));
  assert(x2.Count() == 4 &&
	 x2.To_String() == ChrString("1001000110000"));
  assert(x3.To_String() == ChrString("00100011"));
  assert(x4.To_String() == ChrString("11110101"));
  assert(x5.To_String() == ChrString("010"));

  // Test assigns and appends
  x0 = x1, assert(x0.To_String() == ChrString("1001000110000"));
  x0.AssignSubstr(x3), assert(x0.To_String() == ChrString("00100011"));
  x0.AssignSubstr(x3, 1), assert(x0.To_String() == ChrString("0100011"));
  x0.AssignSubstr(x3, 2, 3), assert(x0.To_String() == ChrString("100"));
  x0 += x5, assert(x0.To_String() == ChrString("100010"));
  x0.Append(x5), assert(x0.To_String() == ChrString("100010010"));
  x0.Append(x5, 1), assert(x0.To_String() == ChrString("10001001010"));
  x0.Append(x5, 2, 3);
  assert(x0.To_String() == ChrString("100010010100"));
  x0.AssignSubstr(x0, 3, 4), assert(x0.To_String() == ChrString("0100"));
  assert((x1 + x1 + x1).To_String() ==
	 ChrString("1001000110000" "1001000110000" "1001000110000"));

  // Test Logic
  x0 = x3;
  x0 |= x5, assert(x0.To_String() == ChrString("01100011"));
  assert((x3 | x5).To_String() == ChrString("01100011"));
  assert((x3 | Str_).To_String() == ChrString("11110111"));
  x0 &= x4, assert(x0.To_String() == ChrString("01100001"));
  assert((x3 & x4).To_String() == ChrString("00100001"));
  x0 ^= x2, assert(x0.To_String() == ChrString("1111000010000"));
  assert((x4 ^ x5).To_String() == ChrString("10110101"));
  x0 <<= 2, assert(x0.To_String() == ChrString("1100001000000"));
  assert((x4 << 3).To_String() == ChrString("10101000"));
  x0 >>= 4, assert(x0.To_String() == ChrString("0000110000100"));
  assert((x4 >> 2).To_String() == ChrString("00111101"));
  assert((~x3).To_String() == ChrString("11011100"));
  assert(((x1 + x1 + x1) ^ x1) == (x1 + x1 + x1) >> 13);

  // test inserts, removes, replaces
  x0 = x5;
  x0.Insert(1, x5), assert(x0.To_String() == ChrString("001010"));
  x0.Insert(1, x5, 1), assert(x0.To_String() == ChrString("01001010"));
  x0.Insert(1, x5, 1, 1);
  assert(x0.To_String() == ChrString("011001010"));
  x0.Remove(7), assert(x0.To_String() == ChrString("0110010"));
  x0.Remove(2, 3), assert(x0.To_String() == ChrString("0110"));
  x0.Replace(2, 1, x5), assert(x0.To_String() == ChrString("010100"));
  x0.Replace(2, 1, x5, 1);
  assert(x0.To_String() == ChrString("0110100"));
  x0.Replace(2, 3, x5, 1, 1);
  assert(x0.To_String() == ChrString("01100"));
  x0.Remove(), assert(x0.To_String() == ChrString(""));

  // Test bit twiddling
  x0 = x5;
  x0.Set(2), assert(x0.To_String() == ChrString("011"));
  x0.Set(1, 0), assert(x0.To_String() == ChrString("001"));
  x0.Set(), assert(x0.To_String() == ChrString("111"));
  x0.Reset(0), assert(x0.To_String() == ChrString("011"));
  x0.Reset(), assert(x0.To_String() == ChrString("000"));
  x0.Toggle(2), assert(x0.To_String() == ChrString("001"));
  x0.Toggle(), assert(x0.To_String() == ChrString("110"));
  assert(x0.Trim() == 2 && x0.To_String() == ChrString("11"));
  assert(x0.Resize(4) == 2 && x0.To_String() == ChrString("1100"));
  assert(x0.Resize(3) == 4 && x0.To_String() == ChrString("110"));
  assert(x0.Resize(4, 1) == 3 && x0.To_String() == ChrString("1101"));

  // Test finds
  x0 = x3;
  assert(x0.To_String() == ChrString("00100011"));
  assert(x0.Find(0) == 0 && x0.Find(1) == 2);
  assert(x0.Find(0, 2) == 3 && x0.Find(1, 3) == 6);
  assert(x0.Find(0, 2, 1) == SIZET_MAX && x0.Find(1, 1, 2) == 2);

  // Test tests
  x0 = x5;
  assert(x0.Any() && !x0.None());
  assert(!x0.Test(0) && x0.Test(1));
  assert(x0 == x5 && x0 != x3);
  x0.Reset(), assert(!x0.Any() && x0.None());
  x0.Resize(145), x0.Toggle(101);
  assert(!x0.Test(100) && x0.Test(101));

  // Test binary IO methods
/*
  assert(!(x0 == x3));
  ofstream fout("testing.bin");
  Write(fout, x0);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, x3);
  fin.close();
  assert(x0 == x3);
*/

  Object* Ptr_ = &x0.NULLOBJECT();
  cout <<"Is Null : " <<Ptr_->IsNullObject() <<endl;
  cout <<"SUCCESS testing <BitString>" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





