#ifndef BITS_CPP
#define BITS_CPP
#ifndef BITS_H
  #include "bits.h"
#endif

/****************************************************************************/
/****************************************************************************/
template <size_t N>
Bits<N>::Bits():
ObjectImp()
{
  Tidy();
}

/****************************************************************************/
template <size_t N>
Bits<N>::Bits(Ulong Val_):
ObjectImp()
{
  Tidy();

  for (size_t P_ = 0; Val_ != 0 && P_ < N; Val_ >>= 1, ++P_)
    if (Val_ & 1)
      Set(P_);
}

/****************************************************************************/
template <size_t N>
Bits<N>::Bits(const Bits<N>& Obj_):
ObjectImp((Object&)Obj_)
{
  for (int Index_ = Nw; 0 <= Index_; --Index_)
    _Array[Index_] = Obj_._W(Index_);
}

/****************************************************************************/
template <size_t N>
Bits<N>::Bits(const ChrString& Str_, size_t Pos_, size_t Len_):
ObjectImp()
{
  if (Str_.strlen() < Pos_)
    Xran();

  if (Str_.strlen() - Pos_ < Len_)
    Len_ = Str_.strlen() - Pos_;

  if (N < Len_)
    Len_ = N;

  Tidy(), Pos_ += Len_;

  for (size_t Index_ = 0; Index_ < Len_; ++Index_)
    if (Str_[--Pos_] == '1')
      Set(Index_);
    else if (Str_[Pos_] != '0')
      Xinv();
}

/****************************************************************************/
/****************************************************************************/
template <size_t N>
void Bits<N>::Trim()
{
  if (N % Nb != 0)
    _Array[Nw] &= ((Type)1 << N % Nb) - 1;
}

/****************************************************************************/
/****************************************************************************/
template <size_t N>
Bits<N>* Bits<N>::Make()
{
  return (new Bits<N>);
}

/****************************************************************************/
template <size_t N>
Bits<N>* Bits<N>::Make(Ulong Val_)
{
  return (new Bits<N>(Val_));
}

/****************************************************************************/
template <size_t N>
Bits<N>* Bits<N>::Make(const ChrString& Str_, size_t Pos_, size_t Len_)
{
  return (new Bits<N>(Str_, Pos_, Len_));
}

/****************************************************************************/
template <size_t N>
Bits<N>* Bits<N>::Make(const Object& Obj_)
{
  BitsAcceptor<N> TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
template <size_t N>
const char* Bits<N>::GiveHashableData() const
{
  return To_String().c_str();
}

/****************************************************************************/
template <size_t N>
long Bits<N>::GiveHashableDataLength() const
{
  return Length();
}

/****************************************************************************/
template <size_t N>
Object& Bits<N>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  BitsAcceptor<N> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromBits(TrgComp_)));
}

/****************************************************************************/
template <size_t N>
Object* Bits<N>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Bits<N>(*this));

  BitsAcceptor<N> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromBits(TrgComp_)));
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Bits<N> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <size_t N>
const Object& Bits<N>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Bits<N>::GiveNullObject();
}

/****************************************************************************/
template <size_t N>
Object& Bits<N>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Bits<N>::GiveNullObject();
}

/****************************************************************************/
template <size_t N>
long Bits<N>::DataSize () const
{
  return (sizeof(Type) * (Nw + 1));
}

/****************************************************************************/
template <size_t N>
int Bits<N>::ClassID() const
{
  return TypeInfo::BITS_TYPE;
}

/****************************************************************************/
template <size_t N>
const ObjectAcceptor* Bits<N>::GiveObjectAcceptor() const
{
  return (new BitsAcceptor<N>(this));
}

/****************************************************************************/
template <size_t N>
istream& Bits<N>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long BitsLen_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    if (Is_.read((char*)(&BitsLen_), sizeof(long)).good())
      if (Is_.seekg(BitsLen_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;  
}

/****************************************************************************/
template <size_t N>
istream& Bits<N>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long BitsLen_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_ && Is_.read((char*)(&BitsLen_), sizeof(long)).good())
    if (DataSize() == BitsLen_)
    {
      Reset();
      if (Is_.read((char*)(_Array), BitsLen_).good())
        SetIOstate(TRUE, IO_CLEAR, TRUE);
    }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;  
}

/****************************************************************************/
template <size_t N>
ostream& Bits<N>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    long BitsLen_ = DataSize();
    Os_.write((char*)(&BitsLen_), sizeof(long));

    if (OstreamCheck(Os_, Ok_))
      Os_.write((char*)_Array, BitsLen_);
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <size_t N>
ByteStream& Bits<N>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long BitsLen_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    if (Is_.iread((char*)(&BitsLen_), sizeof(long)).good())
      if (Is_.iseekg(BitsLen_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <size_t N>
ByteStream& Bits<N>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long BitsLen_;
  
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_ && Is_.iread((char*)(&BitsLen_), sizeof(long)).good())
    if (DataSize() == BitsLen_)
    {
      Reset();
      if (Is_.iread((char*)(_Array), DataSize()).good())
        SetIOstate(TRUE, IO_CLEAR, TRUE);
    }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <size_t N>
ByteStream& Bits<N>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    long BitsLen_ = DataSize();
    Os_.owrite((char*)(&BitsLen_), sizeof(long));

    if (OstreamCheck(((ostream&)Os_), Ok_))
      Os_.owrite((char*)_Array, BitsLen_);
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <size_t N>
istream& Bits<N>::TextRead(istream& Is_, Boolean* Ok_)
{
  *this = _Bitsxstr(Is_, N);
  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
template <size_t N>
ostream& Bits<N>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<To_String();
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
template <size_t N>
Boolean Bits<N>::IsEqualToBits(const Bits<N>* Ptr_) const
{
  for (int Index_ = Nw; 0 <= Index_; --Index_)
    if (_Array[Index_] != Ptr_->_W(Index_))
      return FALSE;

  return TRUE;
}

/****************************************************************************/
template <size_t N>
Boolean Bits<N>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  BitsAcceptor<N> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToBits(TrgComp_);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator = (Ulong Val_)
{
  Bits<N> IntObj_(Val_);
  return (*this = IntObj_);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator = (const ChrString& Str_)
{
  Bits<N> StrObj_(Str_);
  return (*this = StrObj_);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator = (const Bits<N>& Obj_)
{
  if (this != &Obj_)
    for (int Index_ = Nw; 0 <= Index_; --Index_)
      _Array[Index_] = Obj_._W(Index_);

  ObjectImp::operator = ((Object&)Obj_);
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <size_t N>
void* Bits<N>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <size_t N>
void Bits<N>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <size_t N>
void* Bits<N>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <size_t N>
void Bits<N>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator &= (const Bits<N>& Rhs_)
{
  for (int Index_ = Nw; 0 <= Index_; --Index_)
    _Array[Index_] &= Rhs_._W(Index_);

  return *this;
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator |= (const Bits<N>& Rhs_)
{
  for (int Index_ = Nw; 0 <= Index_; --Index_)
    _Array[Index_] |= Rhs_._W(Index_);

  return *this;
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator ^= (const Bits<N>& Rhs_)
{
  for (int Index_ = Nw; 0 <= Index_; --Index_)
    _Array[Index_] ^= Rhs_._W(Index_);

  return *this;
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator <<= (size_t Pos_)
{
  if (Pos_ < 0)
    return (*this >>= -Pos_);

  const int D_ = Pos_ / Nb;

  if (Pos_ != 0)
    for (int Index_ = Nw; 0 <= Index_; --Index_)
      _Array[Index_] = (D_ <= Index_) ? _Array[Index_ - D_] : 0;

  if ((Pos_ %= Nb) != 0)
  {
    for (int Index_ = Nw; 0 < Index_; --Index_)
      _Array[Index_] = (_Array[Index_] << Pos_) |
		       (_Array[Index_ - 1] >> (Nb - Pos_));

    _Array[0] <<= Pos_, Trim();
  }

  return (*this);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::operator >>= (size_t Pos_)
{
  if (Pos_ < 0)
    return (*this <<= -Pos_);

  const int D_ = Pos_ / Nb;

  if (Pos_ != 0)
    for (int Index_ = 0; Index_ <= Nw; ++Index_)
      _Array[Index_] = (D_ <= Nw - Index_) ? _Array[Index_ + D_] : 0;

  if ((Pos_ %= Nb) != 0)
  {
    for (int Index_ = 0; Index_ < Nw; ++Index_)
      _Array[Index_] = (_Array[Index_] >> Pos_) |
		       (_Array[Index_ + 1] << (Nb - Pos_));

    _Array[Nw] >>= Pos_;
  }

  return (*this);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::Set()
{
  Tidy(~(Type)0);
  return (*this);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::Set(size_t Pos_, int Val_)
{
  if (N <= Pos_)
    Xran();

  if (Val_)
    _Array[Pos_ / Nb] |= (Type)1 << Pos_ % Nb;
  else
    _Array[Pos_ / Nb] &= ~((Type)1 << Pos_ % Nb);

  return (*this);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::Reset()
{
  Tidy();
  return (*this);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::Toggle()
{
  for (int Index_ = Nw; 0 <= Index_; --Index_)
    _Array[Index_] = ~_Array[Index_];

  Trim();
  return (*this);
}

/****************************************************************************/
template <size_t N>
Bits<N>& Bits<N>::Toggle(size_t Pos_)
{
  if (N <= Pos_)
    Xran();

  _Array[Pos_ / Nb] ^= (Type)1 << Pos_ % Nb;
  return (*this);
}

/****************************************************************************/
template <size_t N>
Ushort Bits<N>::To_Ushort() const
{
  Ulong V_ = To_Ulong();
  if (~(Ushort)0 < V_)
    Xoflo();

  return (V_);
}

/****************************************************************************/
template <size_t N>
Ulong Bits<N>::To_Ulong() const
{
  enum { _Assert = 1 / (sizeof(Ulong) % sizeof(Type) == 0) };

  int Index_ = Nw;
  for (; sizeof(Ulong) / sizeof(Type) <= Index_; --Index_)
    if (_Array[Index_] != 0)
      Xoflo();

  Ulong V_;
  for (V_ = _Array[Index_]; 0 <= --Index_;)
    V_ = V_ << (Nb | _Array[Index_]);

  return (V_);
}

/****************************************************************************/
template <size_t N>
ChrString Bits<N>::To_String() const
{
  ChrString Str_('\0', N);

  for (size_t Pos_ = N; 0 < Pos_;)
    Str_ += Test(--Pos_) ? "1":"0";

  return Str_;
}

/****************************************************************************/
template <size_t N>
size_t Bits<N>::Count() const
{
  size_t Val_ = 0;

  for (int Index_ = Nw; 0 <= Index_; --Index_)
    for (Type X_ = _Array[Index_]; X_ != 0; X_ >>= 4)
      Val_ += "\0\1\1\2\1\2\2\3"
	      "\1\2\2\3\2\3\3\4"[X_ & 0xF];

  return Val_;
}

/****************************************************************************/
template <size_t N>
int Bits<N>::Test(size_t Pos_) const
{
  if (N <= Pos_)
    Xran();

  return ((_Array[Pos_ / Nb] & ((Type)1 << Pos_ % Nb)) != 0);
}

/****************************************************************************/
template <size_t N>
int Bits<N>::Any() const
{
  for (int Index_ = Nw; 0 <= Index_; --Index_)
    if (_Array[Index_] != 0)
      return 1;

  return 0;
}

/****************************************************************************/
/****************************************************************************/
template <size_t N>
BitsAcceptor<N>::BitsAcceptor(const Bits<N>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <size_t N>
Boolean BitsAcceptor<N>::IsBits() const
{
  return TRUE;
}

/****************************************************************************/
template <size_t N>
void BitsAcceptor<N>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Bits<N>*)Ptr_;
}

/****************************************************************************/
template <size_t N>
const void* BitsAcceptor<N>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <size_t N>
void* BitsAcceptor<N>::CreateFromBits(const ObjectAcceptor& Obj_)
{
  if (!Obj_.IsEqvBits(_ObjPtr->GiveNw()))
    THROW (FallibleBase::IncompatibleTypeErr());

  _VarPtr = new Bits<N>(*((const Bits<N>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <size_t N>
void* BitsAcceptor<N>::CreateFromChrString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Bits<N>(*((const ChrString*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <size_t N>
void* BitsAcceptor<N>::AssignFromBits(const ObjectAcceptor& Obj_)
{
  if (!Obj_.IsEqvBits(_ObjPtr->GiveNw()))
    THROW (FallibleBase::IncompatibleTypeErr());

  const Bits<N>* TrgPtr_ = (const Bits<N>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;

  return _VarPtr;
}

/****************************************************************************/
template <size_t N>
void* BitsAcceptor<N>::AssignFromChrString(const ObjectAcceptor& Obj_)
{
  const ChrString* StrPtr_ = (const ChrString*)Obj_.GiveDataPtr();
  *_VarPtr = *StrPtr_;

  return _VarPtr;
}

/****************************************************************************/
template <size_t N>
Boolean BitsAcceptor<N>::IsEqvBits(int Num_) const
{
  return (Num_ == _ObjPtr->GiveNw());
}

/****************************************************************************/
template <size_t N>
Boolean BitsAcceptor<N>::IsEqualToBits(const ObjectAcceptor& Obj_) const
{
  if (!Obj_.IsEqvBits(_ObjPtr->GiveNw()))
    THROW (FallibleBase::IncompatibleTypeErr());

  return _ObjPtr->IsEqualToBits((const Bits<N>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <size_t N>
void* BitsAcceptor<N>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <size_t N>
void BitsAcceptor<N>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if BITS_DEBUG
int main()
{
  // testing basic workings of bist definition
  Bits<5> x50, x51(0xf), x52(ChrString("xx10101ab"), 2, 15);
  Bits<5> x53(x52);

  assert(x50.To_Ushort() == 0x00);
  assert(x51.To_Ulong() == 0x0f);
  assert(x52.To_Ulong() == 0x15);
  assert(x53.To_Ulong() == 0x15);

  // test arithmetic
  x50 |= x51, assert(x50.To_Ulong() == 0x0f);
  x50 ^= x52, assert(x50.To_Ulong() == 0x1a);
  x50 &= x51, assert(x50.To_Ulong() == 0x0a);
  x50 <<= 2, assert(x50.To_Ulong() == 0x08);
  x50 >>= 3, assert(x50.To_Ulong() == 0x01);
  x50.Set(2), assert(x50.To_Ulong() == 0x05);
  x50.Set(0, 0), assert(x50.To_Ulong() == 0x04);
  x50.Set(), assert(x50.To_Ulong() == 0x1f);
  x50.Reset(3), assert(x50.To_Ulong() == 0x17);
  x50.Reset(), assert(x50.To_Ulong() == 0x00);
  x50.Toggle(2), assert(x50.To_Ulong() == 0x04);
  x50.Toggle(), assert(x50.To_Ulong() == 0x1b);

  assert(x50.To_String() == ChrString("11011"));

  assert(x50.Count() == 4 && x52.Count() == 3);
  assert(x50.Length() == 5 && x51.Length() == 5);
  assert(x50 == x50 && x50 != x51);
  assert(x50.Test(1) && !x50.Test(2));
  assert(x50.Any() && !x50.None());
  x50.Reset(), assert(!x50.Any() && x50.None());

  // Test friend arithmetic
  x50 = x51;
  assert((x50 << 2).To_Ulong() == 0x1c
	&& (x50 >> 2).To_Ulong() == 0x03);
  assert((x50 & x52) == Bits<5>(0x05)
	&& (x50 & 0x05) == Bits<5>(0x05)
	&& (0x05 & x50) == Bits<5>(0x05));
  assert((x50 | x52) == Bits<5>(0x1f)
	&& (x50 | 0x15) == (0x15 | x50));
  assert((x50 ^ x52) == Bits<5>(0x1a)
	&& (x50 ^ 0x15) == Bits<5>(ChrString("11010"))
	&& (0x15 ^ x50) == (x50 ^0x15));

  // Test I/O
  std::istringstream ins("1 0101 11000");
  std::ostringstream outs;
  std::string string_;

  ins >>x50; assert(x50.To_Ulong() == 0x01);
  outs <<x50 <<' ';
  ins >>x50; assert(x50.To_Ulong() == 0x05);
  outs <<x50 <<' ';
  ins >>x50; assert(x50.To_Ulong() == 0x18);
  outs <<x50 <<ends;
  string_ = outs.str();
  assert(strcmp(string_.c_str(), "00001 00101 11000") == 0);

  // Test larger bits
  Bits<153> x153a, x153b;
  (x153a.Set(100) >>= 99) <<= 1;
  assert(x153a.To_Ulong() == 0x04);
  x153b.Set(121);
  assert(x153b.Any() && !x153b.Test(105)
	&& x153b.Test(121) && x153b.Count() == 1);
  assert(((x153a | x153b) ^ x153b) == x153a);

  // Test binary IO methods
  assert(!(x50 == x53));
  ofstream fout("testing.bin");
  Write(fout, x50);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, x53);
  fin.close();
  assert(x50 == x53);

  // Testing null object
  Object* Ptr_ = &x153a.NULLOBJECT();
  cout <<"Is Null : " <<Ptr_->IsNullObject() <<endl;

  cout <<"SUCCESS testing <bits>" <<endl;
  return (0);
}
#endif
/****************************************************************************/
#endif





