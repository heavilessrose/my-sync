/****************************************************************************/
// FILE : PtrVect.cpp
// PURPOSE:
// Simple bit vector class for storage of bits
/****************************************************************************/
#ifndef BITVECTOR_CPP
#define BITVECTOR_CPP
#ifndef BITVECTOR_H
  #include "bitvect.h"
#endif

/****************************************************************************/
/****************************************************************************/
int Compare(const BitVector& Val1_, const BitVector& Val2_)
{
  BitVector::SizeType Max_;
  Byte_t* Str1_ = Val1_._Status;
  Byte_t* Str2_ = Val2_._Status;
  Boolean EqualLen_ = Val1_._Length == Val2_._Length;
  Boolean V1short_ = Val1_._Length < Val2_._Length;  
  Boolean V2short_ = Val2_._Length < Val1_._Length;
  Ulong Diff_ = V1short_ ? (Val2_._Length - Val1_._Length):
                           (Val1_._Length - Val2_._Length);
    
  int Result_ = ((!Str1_ || !Str2_) ?  (Str1_ ? 1:(Str2_ ? -1:0)):10);

  if (Result_ == 10)
  {
    BitVector V1vect_ = Val1_;
    BitVector V2vect_ = Val2_;
    
    if (EqualLen_)
      Max_ = Val1_._Length;
    else if (V1short_)
    {
      V1vect_ += BitVector(Diff_);
      Str1_ = V1vect_._Status;
      Max_ = Val2_._Length;
    }
    else if (V2short_)
    {
      V2vect_ += BitVector(Diff_);
      Str2_ = V2vect_._Status;
      Max_ = Val1_._Length;
    }

    Max_ = BitVector::ByteArraySize(Max_);
    Result_ = memcmp(Str1_, Str2_, Max_);    
  }
  
  return Result_;
}

/****************************************************************************/
Boolean operator < (const BitVector& Val1_, const BitVector& Val2_)
{
  return (Compare(Val1_, Val2_) < 0);
}

/****************************************************************************/
Boolean operator > (const BitVector& Val1_, const BitVector& Val2_)
{
  return (Compare(Val1_, Val2_) > 0);
}

/****************************************************************************/
Boolean operator <= (const BitVector& Val1_, const BitVector& Val2_)
{
  return (Compare(Val1_, Val2_) <= 0);
}

/****************************************************************************/
Boolean operator >= (const BitVector& Val1_, const BitVector& Val2_)
{
  return (Compare(Val1_, Val2_) >= 0);
}

/****************************************************************************/
Boolean operator == (const BitVector& Val1_, const BitVector& Val2_)
{
  Byte_t* Str1_ = Val1_._Status;
  Byte_t* Str2_ = Val2_._Status;
  BitVector::SizeType Max_ = Val1_._Length < Val2_._Length ? Val1_._Length:Val2_._Length;
  Max_ = BitVector::ByteArraySize(Max_);
  
  int Result_ = ((!Str1_ || !Str2_) ? (Str1_ ? 1:(Str2_ ? -1:0)):memcmp(Str1_, Str2_, Max_));
  return !Result_;
}

/****************************************************************************/  
Boolean operator != (const BitVector& Val1_, const BitVector& Val2_)
{
  return !(Val1_ == Val2_);
}

/****************************************************************************/
Boolean operator == (Ulong Val1_, const BitVector& Val2_)
{
  BitVector Bv_(0, NULL, Val1_);

  Byte_t* Str1_ = Bv_._Status;
  Byte_t* Str2_ = Val2_._Status;
  BitVector::SizeType Max_ = Bv_._Length < Val2_._Length ? Bv_._Length:Val2_._Length;
  Max_ = BitVector::ByteArraySize(Max_);
  
  int Result_ = ((!Str1_ || !Str2_) ? (Str1_ ? 1:(Str2_ ? -1:0)):memcmp(Str1_, Str2_, Max_));
  return !Result_;
}

/****************************************************************************/
Boolean operator != (Ulong Val1_, const BitVector& Val2_)
{
  return !(Val1_ == Val2_);
}

/****************************************************************************/
Boolean operator == (const BitVector& Val1_, Ulong Val2_)
{
  BitVector Bv_(0, NULL, Val2_);

  Byte_t* Str1_ = Val1_._Status;
  Byte_t* Str2_ = Bv_._Status;
  BitVector::SizeType Max_ = Val1_._Length < Bv_._Length ? Val1_._Length:Bv_._Length;
  Max_ = BitVector::ByteArraySize(Max_);
  
  int Result_ = ((!Str1_ || !Str2_) ? (Str1_ ? 1:(Str2_ ? -1:0)):memcmp(Str1_, Str2_, Max_));
  return !Result_;
}

/****************************************************************************/  
Boolean operator != (const BitVector& Val1_, Ulong Val2_)
{
  return !(Val1_ == Val2_);
}

/****************************************************************************/
BitVector operator & (const BitVector& Val1_, const BitVector& Val2_)
{
  return (BitVector(Val1_) &= Val2_);
}

/****************************************************************************/
BitVector operator | (const BitVector& Val1_, const BitVector& Val2_)
{
  return (BitVector(Val1_) |= Val2_);
}

/****************************************************************************/
BitVector operator ^ (const BitVector& Val1_, const BitVector& Val2_)
{
  return (BitVector(Val1_) ^= Val2_);
}

/****************************************************************************/
BitVector operator & (Ulong Val1_, const BitVector& Val2_)
{
  BitVector Bv_(0, NULL, Val1_);
  return (Bv_ &= Val2_);
}

/****************************************************************************/  
BitVector operator | (Ulong Val1_, const BitVector& Val2_)
{
  BitVector Bv_(0, NULL, Val1_);
  return (Bv_ |= Val2_);
}

/****************************************************************************/  
BitVector operator ^ (Ulong Val1_, const BitVector& Val2_)
{
  BitVector Bv_(0, NULL, Val1_);
  return (Bv_ ^= Val2_);
}

/****************************************************************************/  
BitVector operator & (const BitVector& Val1_, Ulong Val2_)
{
  BitVector Bv_(0, NULL, Val2_);
  return (Bv_ &= Val1_);
}

/****************************************************************************/  
BitVector operator | (const BitVector& Val1_, Ulong Val2_)
{
  BitVector Bv_(0, NULL, Val2_);
  return (Bv_ |= Val1_);
}

/****************************************************************************/  
BitVector operator ^ (const BitVector& Val1_, Ulong Val2_)
{
  BitVector Bv_(0, NULL, Val2_);
  return (Bv_ ^= Val1_);
}

/****************************************************************************/
BitVector operator + (const BitVector& Val1_, const BitVector& Val2_)
{
  return (BitVector(Val1_) += Val2_);
}

/****************************************************************************/
BitVector operator + (Ulong Val1_, const BitVector& Val2_)
{
  BitVector Bv_(0, NULL, Val1_);
  return (Bv_ += Val2_);
}

/****************************************************************************/
BitVector operator + (const BitVector& Val1_, Ulong Val2_)
{
  BitVector Bv_(0, NULL, Val2_);
  return (BitVector(Val1_) += Bv_);
}

/****************************************************************************/
istream& operator >> (istream& is, BitVector& obj)
{
  return obj.TextRead(is);
}

/****************************************************************************/
ostream& operator << (ostream& os, const BitVector& obj)
{
  return obj.TextWrite(os);
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Bit vector constructor
//
// PRE:
//   SizeType Size_	: Size of bit vector in number of cells
//
// POST:
//   Creates new pointer vector object with the specified number of cells
//
BitVector::BitVector(SizeType Size_, char* Str_, Ulong Value_):
_Length(Size_ ? Size_:(sizeof(long) * 8)),
_Status(Size_ ? MemSet(new_unsigned_char_array(ByteArraySize(Size_), NULL),
                       0x00, ByteArraySize(Size_)):
                MemSet(new_unsigned_char_array(sizeof(long), NULL),
                       0x00, sizeof(long)))
{
  if (Value_)
    FromIntValue(Value_);
  else if (Str_)
    FromString(Str_);
}

/****************************************************************************/
BitVector::BitVector(const BitVector& Obj_):
_Length(Obj_._Length),
_Status(new_unsigned_char_array(ByteArraySize(Obj_._Length), Obj_._Status))
{}

/****************************************************************************/
// PURPOSE:
//   Pointer vector class destructor.
//
// POST:
//   Deletes the data item pointer array and the validity state bit array.
//
BitVector::~BitVector()
{
  ::Delete(_Status);
  _Status = NULL;
}

/****************************************************************************/
int BitVector::ClassID() const
{
  return TypeInfo::BITVECTOR_TYPE;
}

/****************************************************************************/
const char* BitVector::GiveHashableData() const
{
  return ToString();
}

/****************************************************************************/
long BitVector::GiveHashableDataLength() const
{
  return strlen(ToString());
}

/****************************************************************************/
BitVector* BitVector::Make(SizeType Size_)
{
  return (new BitVector(Size_));
}

/****************************************************************************/
BitVector* BitVector::Clone() const
{
  return (new BitVector(*this));
}

/****************************************************************************/
istream& BitVector::BinaryRead(istream& Is_)
{
  Ulong Len_;
  Boolean IoGood_ = Is_.read((char*)(&Len_), sizeof(Ulong)).good();
  SizeType Length_ = Len_;

  if (IoGood_)
  {
    if (_Length < Length_)
      Grow(Length_ - _Length);

    UnSetAllBits();
    IoGood_ = Is_.read((char*)_Status, ByteArraySize(Length_)).good();
  }
    
  return Is_;
}

/****************************************************************************/
ostream& BitVector::BinaryWrite(ostream& Os_) const
{
  Ulong Len_ = _Length;
  Boolean IoGood_ = Os_.write((char*)(&Len_), sizeof(Ulong)).good();

  if (IoGood_)
    IoGood_ = Os_.write((char*)_Status, ByteArraySize(_Length)).good();

  return Os_;
}

/****************************************************************************/
istream& BitVector::TextRead(istream& Is_)
{
    const int bufsize = 100;
    char buf[bufsize];
    char* InputStr_ = NULL;

    UnSetAllBits();
    while (Is_.width(bufsize), Is_>> buf)
    {
        InputStr_ = AppendStr(InputStr_, buf);
	int next_input_char = Is_.peek();

	if (isspace(next_input_char))
	{
	    // Read terminated by white space, not lack of room --> stop reading
	    break;
	}
    }

    if (InputStr_)
    {
      FromString(InputStr_);    
      ::Delete(InputStr_);
    }
    
    return Is_;
}

/****************************************************************************/
ostream& BitVector::TextWrite(ostream& Os_) const
{
  if (_Status && _Length)
  {
    char* Str_ = ToString();

    if (Str_)
    {
      Os_ <<Str_;
      ::Delete(Str_);
    }
  }

  return Os_;
}

/****************************************************************************/
char* BitVector::AppendStr(char* Input_, char* Buffer_)
{
  if (!Input_)
    Input_ = new_char_string(Buffer_);
  else
  {
    char* OldStr_ = Input_;
    Input_ = new_char_array(strlen(OldStr_) + strlen(Buffer_) + 1, NULL);
    strcat(strcpy(Input_, OldStr_), Buffer_);
    ::Delete(OldStr_);
  }

  return Input_;
}

/****************************************************************************/
Byte_t* BitVector::MemSet(Byte_t* Str_, int Char_, size_t Max_)
{
  return (Str_ ? (Byte_t*)(::memset(Str_, Char_, Max_)):Str_);
}

/****************************************************************************/
Byte_t* BitVector::MemMove(Byte_t* Str1_, const Byte_t* Str2_, size_t Max_)
{
  return ((Str1_ && Str2_) ? (Byte_t*)(::memmove(Str1_, Str2_, Max_)):Str1_);
}

/****************************************************************************/
char* BitVector::StrRev(char* Str_)
{
  char Temp_;
  size_t sp, ep;
  
  ep = strlen(Str_) - 1;
  sp = 0;

  while (sp < ep)
  {
    Temp_ = Str_[sp];
    Str_[sp] = Str_[ep];
    Str_[ep] = Temp_;
    
    ++sp;
    --ep;
  }

  return Str_;
}

/****************************************************************************/
char* BitVector::ToString() const
{
  char* Str_ = new_char_array(_Length + 1, NULL);
  SizeType Slen_, i, x;

  for (i = 0; i < _Length; ++i)
    if (GetBit(i))
      Str_[i] = '1';
    else
      Str_[i] = '0';

  Str_[i] = 0;
  StrRev(Str_);
  Slen_ = strlen(Str_);

  for (i = 0; i < Slen_; ++i)
    if (Str_[i] != '0')
      break;

  if (i != 0 && i < Slen_)
  {
    for (x = 0; i < Slen_; ++x, ++i)
      Str_[x] = Str_[i];

    Str_[x] = 0;
  }
  
  return Str_;
}

/****************************************************************************/
BitVector& BitVector::FromString(char* Str_)
{
  BitVector OldBv_ = *this;
  Boolean Error_ = FALSE;
  SizeType x, y, Max_;
  Max_ = strlen(Str_);

  if (Max_ > _Length)
    Grow(Max_ - _Length);
  
  for (x = 0; x < Max_; ++x)
  {
    y = Max_ - x - 1;
    if (Str_[y] == '1')
      SetBit(x);
    else if (Str_[y] == '0')
      UnSetBit(x);
    else
    {
      Error_ = TRUE;
      break;
    }
  }

  if (!Error_)
    for (;x < _Length; ++x)
      UnSetBit(x);
  else
    *this = OldBv_;

  return *this;
}

/****************************************************************************/
char* BitVector::ToHexString(Boolean WithPrefix_) const
{
  SizeType i, x, Cnt_;
  SizeType ExamVal_ = 0;
  SizeType Offset_ = WithPrefix_ ? 2:0;
  SizeType Slen_ = (_Length / 4) + ((_Length % 4) ? 1:0) + Offset_ + 1;                 
  char* Str_ = new_char_array(Slen_, NULL);
  char SymbolTable_[16] =
  {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F'
  };

  Cnt_ = Offset_;
  for (i = 0; i < _Length; ++i)
  {
    if (GetBit(i))
      ExamVal_ = (1 << (i % 4));

    if (i % 4 == 3)
    {
      Str_[Cnt_++] = SymbolTable_[ExamVal_];
      ExamVal_ = 0;
    }
  }

  Str_[Cnt_] = 0;
  StrRev(Str_);
  Slen_ = strlen(Str_);

  for (i = Offset_; i < Slen_; ++i)
    if (Str_[i] != '0')
      break;

  if (i != Offset_ && i < Slen_)
  {
    for (x = Offset_; i < Slen_; ++x, ++i)
      Str_[x] = Str_[i];

    Str_[x] = 0;
  }

  if (WithPrefix_)
  {
    Str_[0] = '0';
    Str_[1] = 'x';
  }

  return Str_;
}

/****************************************************************************/
BitVector& BitVector::FromHexString(char* Str_, Boolean WithPrefix_)
{
  BitVector OldBv_ = *this;
  Boolean Error_ = FALSE;
  SizeType x, y, z, Cnt_;
  SizeType ExamVal_ = 0;
  SizeType Max_ = strlen(Str_) - (WithPrefix_ ? 2:0);
  Cnt_ = Max_ * 4;

  if (Cnt_ > _Length)
    Grow(Cnt_ - _Length);

  for (Cnt_ = x = 0; x < Max_; ++x)
  {
    y = Max_ - x - 1;
    if (ObjRelation<char>::InRange(Str_[y], '0', '9'))
      ExamVal_ = Str_[y] - '0';
    else if (ObjRelation<char>::InRange(toupper(Str_[y]), 'A', 'F'))
      ExamVal_ = toupper(Str_[y]) - 'A' + 10;
    else
    {
      Error_ = TRUE;
      break;
    }

    for (z = 0; z < 4; ++z, ++Cnt_)
      if ((1 << z) & ExamVal_)
        SetBit(Cnt_);
      else
        UnSetBit(Cnt_);
  }

  if (!Error_)
    for (;Cnt_ < _Length; ++Cnt_)
      UnSetBit(Cnt_);
  else
    *this = OldBv_;

  return *this;
}

/****************************************************************************/
Ulong BitVector::ToIntValue(Boolean& Convertable_) const
{
  SizeType Max_ = sizeof(long) * 8;
  SizeType i, LeadBit_;

  LeadBit_ = 0;
  for (i = _Length - 1; i != SizeType(-1); --i)
    if (GetBit(i))
    {
      LeadBit_ = i;
      break;
    }

  if (Max_ < LeadBit_ + 1)
  {
    Convertable_ = FALSE;
    return 0;
  }

  Ulong Val_ = 0;
  Convertable_ = TRUE;

  for (i = 0; i <= LeadBit_; ++i)
    if (GetBit(i))
      Val_ |= (1 << i);

  return Val_;
}

/****************************************************************************/
BitVector& BitVector::FromIntValue(Ulong Val_)
{
  SizeType Max_ = sizeof(long) * 8;
  SizeType i;

  for (i = Max_ - 1; i != SizeType(-1); --i)
    if (Val_ & (1 << i))
      break;

  Max_ = i + 1;
  if (Max_ > _Length)
    Grow(Max_ - _Length);  

  for (i = 0; i < Max_; ++i)
    if ((1 << i) & Val_)
      SetBit(i);
    else
      UnSetBit(i);

  for (;i < _Length; ++i)
    UnSetBit(i);

  return *this;
}

/****************************************************************************/
BitVector& BitVector::operator = (const BitVector& Obj_)
{
  if (this != &Obj_)
  {
    Byte_t* OldStr_ = _Status;
    _Length = Obj_._Length;
    _Status = new_unsigned_char_array(ByteArraySize(Obj_._Length), Obj_._Status);
    ::Delete(OldStr_);
  }

  return *this;
}

/****************************************************************************/
BitVector& BitVector::operator += (const BitVector& Obj_)
{
  if (this != &Obj_)
  {
    Byte_t* OldStr_ = _Status;
    SizeType OldLen_ = _Length;
    SizeType ByteOldLen_ = ByteArraySize(OldLen_);
    SizeType Clen_;
    
    _Length += Obj_._Length;
    Clen_ = ByteArraySize(_Length);    
    _Status = new_unsigned_char_array(Clen_, NULL);
    MemSet(_Status, 0x00, Clen_);
    MemMove(
      MemMove(_Status, OldStr_, ByteOldLen_) + ByteOldLen_,
      Obj_._Status, ByteArraySize(Obj_._Length));

    ::Delete(OldStr_);
  }

  return *this;
}

/****************************************************************************/
BitVector& BitVector::Grow(SizeType Ext_)
{
  Byte_t* OldStr_ = _Status;  
  SizeType OldLen_ = _Length;
  SizeType Clen_;
    
  _Length += Ext_;
  Clen_ = ByteArraySize(_Length);
  _Status = new_unsigned_char_array(Clen_, NULL);
  MemSet(_Status, 0x00, Clen_);
  MemMove(_Status, OldStr_, ByteArraySize(OldLen_));
  ::Delete(OldStr_);

  return *this;
}

/****************************************************************************/
BitVector& BitVector::SetBit(SizeType BitNum_)
{
  if (BitNum_ < _Length)
    _Status[ByteIndex(BitNum_)] |= (1 << BitIndex(BitNum_));

  return *this;
}

/****************************************************************************/
BitVector& BitVector::UnSetBit(SizeType BitNum_)
{
  if (BitNum_ < _Length)
    _Status[ByteIndex(BitNum_)] &= ~(1 << BitIndex(BitNum_));

  return *this;
}

/****************************************************************************/
void BitVector::ToggleBit(SizeType BitNum_)
{
  if (GetBit(BitNum_))
    UnSetBit(BitNum_);
  else
    SetBit(BitNum_);
}

/****************************************************************************/
Byte_t BitVector::GetBit(SizeType BitNum_) const
{
  return ((BitNum_ < _Length) ?
             (_Status[ByteIndex(BitNum_)] & (1 << BitIndex(BitNum_))):0);
}

/****************************************************************************/
BitVectorRef BitVector::operator [] (SizeType BitNum_)
{
  return BitVectorRef(this, BitNum_, 0);
}

/****************************************************************************/
BitVectorRef BitVector::operator [] (SizeType BitNum_) const
{
  return BitVectorRef(this, BitNum_);
}

/****************************************************************************/
BitVector& BitVector::SetAllBits()
{
  SizeType i;
  for (i = 0; i < _Length; ++i)
    SetBit(i);

  return *this;
}

/****************************************************************************/    
BitVector& BitVector::UnSetAllBits()
{
  SizeType i;
  for (i = 0; i < _Length; ++i)
    UnSetBit(i);

  return *this;
}

/****************************************************************************/    
BitVector& BitVector::ToggleAllBits()
{
  SizeType i;
  for (i = 0; i < _Length; ++i)
    ToggleBit(i);

  return *this;
}

/****************************************************************************/
BitVector::SizeType BitVector::NextBit(SizeType Pos_, Boolean& Found_) const
{
  SizeType i;
  for (i = Pos_ + 1; i < _Length; ++i)
    if (GetBit(i))
      return i;

  Found_ = FALSE;
  return 0;
}

/****************************************************************************/    
BitVector::SizeType BitVector::PreviousBit(SizeType Pos_, Boolean& Found_) const
{
  SizeType i;
  for (i = Pos_ - 1; i != SizeType(-1); --i)
    if (GetBit(i))
      return i;

  Found_ = FALSE;
  return 0;
}

/****************************************************************************/    
Boolean BitVector::Any() const
{
  Boolean Found_;
  return (GetBit(0) || NextBit(0, Found_) != 0);
}

/****************************************************************************/    
Boolean BitVector::None() const
{
  return !Any();
}

/****************************************************************************/    
BitVector::SizeType BitVector::GetMsb(Boolean& Found_) const
{
  SizeType i;

  for (i = _Length - 1; i != SizeType(-1); --i)
    if (GetBit(i))
      return i;

  Found_ = FALSE;
  return 0;
}

/****************************************************************************/    
BitVector::SizeType BitVector::GetLsb(Boolean& Found_) const
{
  SizeType i;

  for (i = 0; i < _Length; ++i)
    if (GetBit(i))
      return i;

  Found_ = FALSE;
  return 0;
}

/****************************************************************************/    
BitVector::SizeType BitVector::NumBitsSet() const
{
  SizeType i, Cnt_;

  for (Cnt_ = i = 0; i < _Length; ++i)
    if (GetBit(i))
      ++Cnt_;

  return Cnt_;
}

/****************************************************************************/
BitVector& BitVector::Resize(SizeType Len_)
{
  SizeType i;
  BitVector Bv_(Len_);

  for (i = 0; i < Bv_.GetLength(); ++i)
    if (GetBit(i))
      Bv_.SetBit(i);

  *this = Bv_;
  return *this;
}

/****************************************************************************/
BitVector::SizeType BitVector::Trim()
{
  Boolean Found_;
  SizeType Msb_ = GetMsb(Found_);
  SizeType Sz_ = Found_ ? (Msb_ + 1):(sizeof(long) * 8);

  SizeType i;
  BitVector Bv_(Sz_);

  if (Found_)
    for (i = 0; i < Bv_.GetLength(); ++i)
      if (GetBit(i))
        Bv_.SetBit(i);

  *this = Bv_;
  return _Length;
}

/****************************************************************************/
Boolean BitVector::operator ! () const
{
  SizeType i;
  SizeType Max_ = ByteArraySize(_Length);
  
  for (i = 0; i < Max_; ++i)
    if (_Status[i])
      return FALSE;

  return TRUE;
}

/****************************************************************************/
BitVector BitVector::operator ~ () const
{
  BitVector Bv_(*this);
  return Bv_.ToggleAllBits();
}

/****************************************************************************/    
BitVector& BitVector::operator |= (const BitVector& Obj_)
{
  if (this != &Obj_)
  {
    SizeType Max_, i;
  
    if (Obj_._Length > _Length)
    {
      Grow(Obj_._Length - _Length);
      Max_ = Obj_._Length;
    }
    else
      Max_ = _Length;

    for (i = 0; i < Max_; ++i)
      if (GetBit(i) | Obj_.GetBit(i))
        SetBit(i);
  }

  return *this;
}

/****************************************************************************/    
BitVector& BitVector::operator &= (const BitVector& Obj_)
{
  if (this != &Obj_)
  {
    SizeType Max_, i;
  
    if (Obj_._Length > _Length)
    {
      Grow(Obj_._Length - _Length);
      Max_ = Obj_._Length;
    }
    else
      Max_ = _Length;

    for (i = 0; i < Max_; ++i)
      if (!(GetBit(i) & Obj_.GetBit(i)))
        UnSetBit(i);
  }

  return *this;
}

/****************************************************************************/    
BitVector& BitVector::operator ^= (const BitVector& Obj_)
{
  if (this != &Obj_)
  {
    SizeType Max_, i;
  
    if (Obj_._Length > _Length)
    {
      Grow(Obj_._Length - _Length);
      Max_ = Obj_._Length;
    }
    else
      Max_ = _Length;

    for (i = 0; i < Max_; ++i)
      if (GetBit(i) ^ Obj_.GetBit(i))
        SetBit(i);
      else
        UnSetBit(i);
  }

  return *this;
}

/****************************************************************************/    
BitVector& BitVector::operator <<= (SizeType Shift_)
{
  BitVector Bv_(_Length + Shift_);
  SizeType i;

  for (i = 0; i < _Length; ++i)
    if (GetBit(i))
      Bv_.SetBit(i + Shift_);

  *this = Bv_;
  return *this;
}

/****************************************************************************/    
BitVector& BitVector::operator >>= (SizeType Shift_)
{
  BitVector Bv_(_Length - Shift_);
  SizeType i;

  for (i = Shift_; i < _Length; ++i)
    if (GetBit(i))
      Bv_.SetBit(i - Shift_);

  *this = Bv_;
  return *this;
}

/****************************************************************************/    
BitVector BitVector::operator << (SizeType Shift_) const
{
  BitVector Bv_(*this);
  Bv_ <<= Shift_;
  return Bv_;
}

/****************************************************************************/  
BitVector BitVector::operator >> (SizeType Shift_) const
{
  BitVector Bv_(*this);
  Bv_ >>= Shift_;
  return Bv_;
}

/****************************************************************************/
#if OVERLOAD_NEW
// PURPOSE:
//   Overloaded new operator for allocating memory using custom memory
//   manager class.
//
// PRE:
//   size_t Bytes_	: The size of the memory block to be allocated.
//
// POST:
//   A new memory block is allocated and used by the newly allocated
//   TBitVector object.
//
void* BitVector::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
// PURPOSE:
//   Overloaded delete operator for releasing memory allocated with the
//   custom memory manager class.
//
// PRE:
//   void* Space_	: The memory block to be released to the memory mgr.
//
// POST:
//   The specified memory block is released to the custom memory manager.
//
void BitVector::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
// PURPOSE:
//   Overloaded new operator for allocating memory using custom memory
//   manager class.
//
// PRE:
//   size_t Bytes_	: The size of the memory block to be allocated.
//
// POST:
//   A new memory block is allocated and used by the newly allocated
//   TBitVector object.
//
#if	HAS_ARRAY_NEW
void* BitVector::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
// PURPOSE:
//   Overloaded delete operator for releasing memory allocated with the
//   custom memory manager class.
//
// PRE:
//   void* Space_	: The memory block to be released to the memory mgr.
//
// POST:
//   The specified memory block is released to the custom memory manager.
//
#if	HAS_ARRAY_NEW
void BitVector::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
BitVectorRef::BitVectorRef(const BitVector* Bv_, BitVector::SizeType BitNum_):
_Const(TRUE),
_BitNum(BitNum_),
_ConstBvp(Bv_)
{}

/****************************************************************************/
BitVectorRef::BitVectorRef(BitVector* Bv_, BitVector::SizeType BitNum_, int):
_Const(FALSE),
_BitNum(BitNum_),
_Bvp(Bv_)
{}

/****************************************************************************/
BitVectorRef& BitVectorRef::operator = (Byte_t Bit_)
{
  if (!_Const)
    if (Bit_)
      _Bvp->SetBit(_BitNum);
    else
      _Bvp->UnSetBit(_BitNum);

  return *this;
}

/****************************************************************************/
BitVectorRef::operator Byte_t () const
{
  return _ConstBvp->GetBit(_BitNum);
}

/****************************************************************************/
/****************************************************************************/
#if BITVECTOR_DEBUG
#include <iostream.h>

int main()
{
  Ulong Value_;
  BitVector pv_(20);
  BitVector pv2_(20);

  // testing basic workings of bist definition
  BitVector x50(5), x51(5), x52(5);
  x51.FromIntValue(0x0f);
  x52.FromString("10101");  
  BitVector x53(x52);
  Boolean Conv_;
  char* Str_;

  assert(x50.ToIntValue(Conv_) == 0x00);
  assert(x51.ToIntValue(Conv_) == 0x0f);
  assert(x52.ToIntValue(Conv_) == 0x15);
  assert(x53.ToIntValue(Conv_) == 0x15);

  // test arithmetic
  x50 |= x51, assert(x50.ToIntValue(Conv_) == 0x0f);
  x50 ^= x52, assert(x50.ToIntValue(Conv_) == 0x1a);
  x50 &= x51, assert(x50.ToIntValue(Conv_) == 0x0a);
  x50 <<= 2;
  Str_ = x50.ToString();
  assert(!strcmp(Str_, "101000"));
  ::Delete(Str_);
  x50 >>= 3, assert(x50.ToIntValue(Conv_) == (0x0a >> 1));  
  x50[2] = 1; assert(x50.ToIntValue(Conv_) == 0x05);
  x50[0] = 0; assert(x50.ToIntValue(Conv_) == 0x04);

  x50.Resize(5);
  x50.SetAllBits(), assert(x50.ToIntValue(Conv_) == 0x1f);
  x50[3] = 0; assert(x50.ToIntValue(Conv_) == 0x17);
  x50.UnSetAllBits(), assert(x50.ToIntValue(Conv_) == 0x00);
  x50.ToggleBit(2), assert(x50.ToIntValue(Conv_) == 0x04);
  x50.ToggleAllBits(), assert(x50.ToIntValue(Conv_) == 0x1b);

  Str_ = x50.ToString();
  assert(!strcmp(Str_, "11011"));
  ::Delete(Str_);
  assert(x50.NumBitsSet() == 4 && x52.NumBitsSet() == 3);
  assert(x50.GetLength() == 5 && x51.GetLength() == 5);
  assert(x50 == x50 && x50 != x51);
  assert(x50[1] && !x50[2]);
  assert(x50.Any() && !x50.None());
  x50.UnSetAllBits(), assert(!x50.Any() && x50.None());

  // Test friend arithmetic
  x50 = x51;
  assert((x50 << 2).ToIntValue(Conv_) == 0x3c
	&& (x50 >> 2).ToIntValue(Conv_) == 0x03);
  assert((x50 & x52) == BitVector(5, NULL, 0x05)
	&& (x50 & 0x05) == BitVector(5, NULL, 0x05)
	&& (0x05 & x50) == BitVector(5, NULL, 0x05));
  assert((x50 | x52) == BitVector(5, NULL, 0x1f)
	&& (x50 | 0x15) == (0x15 | x50));
  assert((x50 ^ x52) == BitVector(5, NULL, 0x1a)
	&& (x50 ^ 0x15) == BitVector(5, "11010")
	&& (0x15 ^ x50) == (x50 ^0x15));
  
  // Test I/O
  std::istringstream ins("1 0101 11000");
  std::ostringstream outs;
  std::string string_;

  ins >>x50; assert(x50.ToIntValue(Conv_) == 0x01);
  outs <<x50 <<" ";
  ins >>x50; assert(x50.ToIntValue(Conv_) == 0x05);
  outs <<x50 <<" ";
  ins >>x50; assert(x50.ToIntValue(Conv_) == 0x18);
  outs <<x50 <<ends;
  string_ = outs.str();
  assert(strcmp(string_.c_str(), "1 101 11000") == 0);

  // Test larger bits
  BitVector x153a(153), x153b(153);
  (x153a.SetBit(100) >>= 99) <<= 1;
  assert(x153a.ToIntValue(Conv_) == 0x04);
  x153b.SetBit(121);
  assert(x153b.Any() && !x153b.GetBit(105)
	&& x153b.GetBit(121) && x153b.NumBitsSet() == 1);
  assert(((x153a | x153b) ^ x153b) == x153a);

  cout <<"SUCCESS testing <bits>" <<endl;
  return (0);
}
#endif
/****************************************************************************/
#endif





