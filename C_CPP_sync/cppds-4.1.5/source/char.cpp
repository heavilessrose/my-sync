#ifndef CHAR_CPP
#define CHAR_CPP
#ifndef CHAR_H
  #include "char.h"
#endif

/****************************************************************************/
Character::Character(CharType IntVal_):
IntegerAsBitsType<Character, CharType>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(IntVal_)
{}

/****************************************************************************/
Character::Character(short IntVal_, int Radix_):
IntegerAsBitsType<Character, CharType>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(FromDigit(IntVal_, Radix_))
{}

/****************************************************************************/
Character::Character(const ChrString& Str_):
IntegerAsBitsType<Character, CharType>(TypeWrapper::BOUNDS_CHECKING_BIT),
_Value(*Str_)
{}

/****************************************************************************/
Character::Character(const Character& CharObj_):
IntegerAsBitsType<Character, CharType>(CharObj_),
_Value(CharObj_._Value)
{}

/****************************************************************************/
Character* Character::Make(CharType IntVal_)
{
  return (new Character(IntVal_));
}

/****************************************************************************/
Character* Character::Make(short IntVal_, int Radix_)
{
  return (new Character(IntVal_, Radix_));
}

/****************************************************************************/
Character* Character::Make(const Object& Obj_)
{
  CharacterAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object* Character::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Character(*this));

  CharacterAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromCharacter(TrgComp_)));
}

/****************************************************************************/
Object& Character::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  CharacterAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromCharacter(TrgComp_)));
}

/****************************************************************************/
Character& Character::DerivedClass()
{
  return *this;
}

/****************************************************************************/
const Character& Character::DerivedClass() const
{
  return *this;
}

/****************************************************************************/
Character& Character::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Character _NULLOBJECT(0);
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& Character::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Character::GiveNullObject();
}

/****************************************************************************/
Object& Character::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Character::GiveNullObject();
}

/****************************************************************************/
const char* Character::GiveHashableData() const
{
  return ((const char*)&_Value);
}

/****************************************************************************/
long Character::GiveHashableDataLength() const
{
  return sizeof(_Value);
}

/****************************************************************************/
Character Character::StaticFromString(const ChrString& Str_)
{
  return Character(Str_);
}

/****************************************************************************/
Character& Character::FromString(const ChrString& Str_)
{
  Character Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
ChrString Character::ToString(const CharType Ch_)
{
  char Buffer_[2];
  return ChrString(CharToStr(Ch_, Buffer_));
}

/****************************************************************************/
ChrString Character::ToString(int) const
{
  char Buffer_[2];
  return ChrString(CharToStr(_Value, Buffer_));
}

/****************************************************************************/
ChrString Character::ToTruncString(const CharType Ch_)
{
  char Buffer_[2];
  return ChrString(CharToStr(Ch_, Buffer_));
}

/****************************************************************************/
ChrString Character::ToTruncString(int) const
{
  char Buffer_[2];
  return ChrString(CharToStr(_Value, Buffer_));
}

/****************************************************************************/
Character Character::FromDigit(short Val_, int Radix_)
{
  if (IsBasev(Val_, Radix_))
    return Character((0 <= Val_ && Val_ <= 9) ? ('0' + Val_):
		     (10 <= Val_ && Val_ <= 35) ? ('A' + (Val_ - 10)):0);

  return 0;
}

/****************************************************************************/
short Character::ToDigit(CharType Ch_, int Radix_)
{
  if (IsBasec(Ch_, Radix_))
    return (isdigit(Ch_) ? (Ch_ - '0'):
	    isalpha(Ch_) ? (10 + (toupper(Ch_) - 'A')):0);

  return 0;
}

/****************************************************************************/
const CharType& Character::GetData() const
{
  return _Value;
}

/****************************************************************************/
void Character::SetData(const CharType& Obj_)
{
  *this = Obj_;
}

/****************************************************************************/
int Character::ClassID() const
{
  return TypeInfo::CHARACTER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* Character::GiveObjectAcceptor() const
{
  return (new CharacterAcceptor(this));
}

/****************************************************************************/
Boolean Character::IsEqualToCharacter(const Character* Ptr_) const
{
  return (_Value == Ptr_->_Value);
}

/****************************************************************************/
Boolean Character::IsLesserToCharacter(const Character* Ptr_) const
{
  return (_Value < Ptr_->_Value);
}

/****************************************************************************/
Boolean Character::IsGreaterToCharacter(const Character* Ptr_) const
{
  return (_Value > Ptr_->_Value);
}

/****************************************************************************/
Boolean Character::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  CharacterAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToCharacter(TrgComp_);
}

/****************************************************************************/
Boolean Character::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  CharacterAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToCharacter(TrgComp_);
}

/****************************************************************************/
Boolean Character::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  CharacterAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToCharacter(TrgComp_);
}

/****************************************************************************/
istream& Character::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& Character::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.read((CharType*)(&_Value), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& Character::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_)
    Os_.write((CharType*)(&_Value), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& Character::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& Character::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.iread((CharType*)(&_Value), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& Character::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.owrite((CharType*)(&_Value), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& Character::TextRead(istream& Is_, Boolean* Ok_)
{
  char Data_;

  Is_ >>Data_;
  *this = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& Character::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Value;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long Character::DataSize () const
{
  return sizeof(CharType);
}

/****************************************************************************/
Character& Character::operator = (const CharType& IntVal_)
{
  _Value = IntVal_;
  return *this;
}

/****************************************************************************/
Character& Character::operator = (const ChrString& Str_)
{
  Character Sobj_(Str_);
  return (*this = Sobj_);
}

/****************************************************************************/
Character& Character::operator = (const Character& Obj_)
{
  _Value = Obj_._Value;
  IntegerAsBitsType<Character, CharType>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
Boolean Character::AtZero() const
{
  return (_Value == 0);
}

/****************************************************************************/
Boolean Character::AtMaxValue() const
{
  return (_Value == CHAR_MAX);
}

/****************************************************************************/
Boolean Character::AtMinValue() const
{
  return (_Value == CHAR_MIN);
}

/****************************************************************************/
Boolean Character::operator ! () const
{
  return !_Value;
}

/****************************************************************************/
Character Character::operator - () const
{
  return Character(-_Value);
}

/****************************************************************************/
Character Character::operator + () const
{
  return Character(*this);
}

/****************************************************************************/
const Character& Character::operator ++ ()
{
  if (BoundsChecking() && _Value == CHAR_MAX)
    SetOverflow();

  ++_Value;
  return *this;
}

/****************************************************************************/
const Character& Character::operator ++ (int)
{
  static Character OldVal_ = _Value;	// Non-threadsafe

  if (BoundsChecking() && _Value == CHAR_MAX)
    SetOverflow();

  ++_Value;
  return OldVal_;
}

/****************************************************************************/
const Character& Character::operator -- ()
{
  if (BoundsChecking() && _Value == CHAR_MIN)
    SetUnderflow();

  --_Value;
  return *this;
}

/****************************************************************************/
const Character& Character::operator -- (int)
{
  static Character OldVal_ = _Value;	// Non-threadsafe

  if (BoundsChecking() && _Value == CHAR_MIN)
    SetUnderflow();

  --_Value;
  return OldVal_;
}

/****************************************************************************/
Character& Character::operator += (const NumOrdinalType<Character>& Obj_)
{
  if (BoundsChecking())
  {
    char Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = CharType(Obj_.DerivedClass());

    charSeriesCompare CeilCmp_ = charSeriesCompare::SumOfSeriesCmp(CHAR_MAX, Buffer_, 2, FALSE);
    charSeriesCompare FloorCmp_ = charSeriesCompare::SumOfSeriesCmp(CHAR_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value += Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Character& Character::operator -= (const NumOrdinalType<Character>& Obj_)
{
  if (BoundsChecking())
  {
    char Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = CharType(Obj_.DerivedClass());

    charSeriesCompare CeilCmp_ = charSeriesCompare::DifferenceOfSeriesCmp(CHAR_MAX, Buffer_, 2, FALSE);
    charSeriesCompare FloorCmp_ = charSeriesCompare::DifferenceOfSeriesCmp(CHAR_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value -= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Character& Character::operator *= (const NumOrdinalType<Character>& Obj_)
{
  if (BoundsChecking())
  {
    char Buffer_[2];
    Buffer_[0] = _Value;
    Buffer_[1] = CharType(Obj_.DerivedClass());

    charSeriesCompare CeilCmp_ = charSeriesCompare::ProductOfSeriesCmp(CHAR_MAX, Buffer_, 2, FALSE);
    charSeriesCompare FloorCmp_ = charSeriesCompare::ProductOfSeriesCmp(CHAR_MIN, Buffer_, 2, FALSE);

    if (CeilCmp_.Result() < 0)
      SetOverflow();

    if (FloorCmp_.Result() > 0)
      SetUnderflow();
  }

  _Value *= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Character& Character::operator /= (const NumOrdinalType<Character>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value /= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Character& Character::operator %= (const NumOrdinalType<Character>& Obj_)
{
  if (Obj_.DerivedClass().GetData() == 0)
    SetDivideByZero();
  else
    _Value %= Obj_.DerivedClass().GetData();

  return *this;
}

/****************************************************************************/
Character& Character::operator += (const CharType& IntVal_)
{
  return (*this += Character(IntVal_));
}

/****************************************************************************/    
Character& Character::operator -= (const CharType& IntVal_)
{
  return (*this -= Character(IntVal_));
}

/****************************************************************************/    
Character& Character::operator *= (const CharType& IntVal_)
{
  return (*this *= Character(IntVal_));
}

/****************************************************************************/    
Character& Character::operator /= (const CharType& IntVal_)
{
  return (*this /= Character(IntVal_));
}

/****************************************************************************/    
Character& Character::operator %= (const CharType& IntVal_)
{
  return (*this %= Character(IntVal_));
}

/****************************************************************************/
Character Character::operator ~ () const
{
  return Character(~_Value);
}

/****************************************************************************/
Character& Character::operator |= (const NumOrdinalAsBitsType<Character>& Obj_)
{
  _Value |= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Character& Character::operator &= (const NumOrdinalAsBitsType<Character>& Obj_)
{
  _Value &= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Character& Character::operator ^= (const NumOrdinalAsBitsType<Character>& Obj_)
{
  _Value ^= Obj_.DerivedClass().GetData();
  return *this;
}

/****************************************************************************/
Character& Character::operator |= (const CharType& IntVal_)
{
  return (*this |= Character(IntVal_));
}

/****************************************************************************/    
Character& Character::operator &= (const CharType& IntVal_)
{
  return (*this &= Character(IntVal_));
}

/****************************************************************************/    
Character& Character::operator ^= (const CharType& IntVal_)
{
  return (*this ^= Character(IntVal_));
}

/****************************************************************************/
Character& Character::operator <<= (size_t Shift_)
{
  _Value <<= Shift_;
  return *this;
}

/****************************************************************************/
Character& Character::operator >>= (size_t Shift_)
{
  _Value >>= Shift_;
  return *this;
}

/****************************************************************************/
Character Character::operator << (size_t Shift_) const
{
  return (Character(*this) <<= Shift_);
}

/****************************************************************************/
Character Character::operator >> (size_t Shift_) const
{
  return (Character(*this) >>= Shift_);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* Character::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void Character::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* Character::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void Character::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
Character operator + (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) += Val2_);
}

/****************************************************************************/  
Character operator - (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) -= Val2_);
}

/****************************************************************************/  
Character operator * (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) *= Val2_);
}

/****************************************************************************/  
Character operator / (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) /= Val2_);
}

/****************************************************************************/  
Character operator % (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) %= Val2_);
}

/****************************************************************************/
Character operator & (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) &= Val2_);
}

/****************************************************************************/  
Character operator | (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) |= Val2_);
}

/****************************************************************************/  
Character operator ^ (const Character& Val1_, const Character& Val2_)
{
  return (Character(Val1_) ^= Val2_);
}

/****************************************************************************/
Character operator + (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) += Val2_);
}

/****************************************************************************/  
Character operator - (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) -= Val2_);
}

/****************************************************************************/  
Character operator * (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) *= Val2_);
}

/****************************************************************************/  
Character operator / (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) /= Val2_);
}

/****************************************************************************/  
Character operator % (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) %= Val2_);
}

/****************************************************************************/  
Character operator & (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) &= Val2_);
}

/****************************************************************************/  
Character operator | (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) |= Val2_);
}

/****************************************************************************/  
Character operator ^ (CharType Val1_, const Character& Val2_)
{
  return (Character(Val1_) ^= Val2_);
}

/****************************************************************************/
Character operator + (const Character& Val1_, CharType Val2_)
{
  return (Character(Val1_) += Character(Val2_));
}

/****************************************************************************/  
Character operator - (const Character& Val1_, CharType Val2_)
{
  Character Temp1_(Val1_);
  Character Temp2_(Val2_);
  return (Temp1_ -= Temp2_);
}

/****************************************************************************/  
Character operator * (const Character& Val1_, CharType Val2_)
{
  return (Character(Val1_) *= Character(Val2_));
}

/****************************************************************************/  
Character operator / (const Character& Val1_, CharType Val2_)
{
  Character Temp1_(Val1_);
  Character Temp2_(Val2_);
  return (Temp1_ /= Temp2_);
}

/****************************************************************************/  
Character operator % (const Character& Val1_, CharType Val2_)
{
  Character Temp1_(Val1_);
  Character Temp2_(Val2_);
  return (Temp1_ %= Temp2_);
}

/****************************************************************************/  
Character operator & (const Character& Val1_, CharType Val2_)
{
  return (Character(Val1_) &= Character(Val2_));
}

/****************************************************************************/  
Character operator | (const Character& Val1_, CharType Val2_)
{
  return (Character(Val1_) |= Character(Val2_));
}

/****************************************************************************/  
Character operator ^ (const Character& Val1_, CharType Val2_)
{
  return (Character(Val1_) ^= Character(Val2_));
}

/****************************************************************************/
/****************************************************************************/
CharacterAcceptor::CharacterAcceptor(const Character* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
Boolean CharacterAcceptor::IsCharacter() const
{
  return TRUE;
}

/****************************************************************************/
Boolean CharacterAcceptor::IsTypeWrapper() const
{
  return TRUE;
}

/****************************************************************************/
void CharacterAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Character*)Ptr_;
}

/****************************************************************************/
const void* CharacterAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
void* CharacterAcceptor::CreateFromCharacter(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Character(*((const Character*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* CharacterAcceptor::CreateFromChrString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Character(*((const ChrString*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* CharacterAcceptor::AssignFromCharacter(const ObjectAcceptor& Obj_)
{
  const Character* TrgPtr_ = (const Character*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
void* CharacterAcceptor::AssignFromChrString(const ObjectAcceptor& Obj_)
{
  const ChrString* StrPtr_ = (const ChrString*)Obj_.GiveDataPtr();
  *_VarPtr = *StrPtr_;
  return _VarPtr;
}

/****************************************************************************/
Boolean CharacterAcceptor::IsEqualToCharacter(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToCharacter((const Character*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean CharacterAcceptor::IsLesserToCharacter(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToCharacter((const Character*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean CharacterAcceptor::IsGreaterToCharacter(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToCharacter((const Character*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
void* CharacterAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void CharacterAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if CHARACTER_DEBUG
int main()
{
  ChrString Str_;

  // Testing to string conversion
  Character si('f');
  Str_ = si.ToString(si);
  assert(!strcmp(Str_.c_str(), "f"));

  // Testing to lower case character conversion
  Character si2('F');
  si2 = si2.ToLowerCase(si2);

  // Testing to digit to character conversion constructor
  Character si3(0xA, 16);
  Str_ = si3.ToString(si3);
  assert(!strcmp(Str_.c_str(), "A"));

  // Testing relational operators
  assert(!(si < si3));
  assert(si > si3);
  assert(si == si2);
  assert(!(si == si3));
  assert(si != si3);

  // Testing object persistence and data storage
  ofstream fout("testing.bin");
  Write(fout, si);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, si3);
  fin.close();

  assert(si3 == 'f');

  // Testing space character validation method
  si2 = " ";
  assert(si2 == ' ');
  assert(si2.IsSpace(si2));

  // Testing Base-N digit character validation method
  assert(si3.GetData() == 'f'); assert(si3.IsBaseDigit(si3, 16));
  si3.SetData('9');
  assert(si3.GetData() == '9'); assert(si3.IsBaseDigit(si3, 10));

  // Testing to upper case conversion
  si = si.ToUpperCase(si);
  assert(si == 'F');

  // Testing to digit conversion
  int Val_ = Character::ToDigit(si3);
  assert(Val_ == 9);

  // Testing from digit to character conversion and
  // from string to character conversion
  assert(Character::FromDigit(Val_) == '9');
  assert(Character::StaticFromString("Z") == 'Z');

  cout <<"Enter a letter: ";
  cin >>si;
  cout <<si <<endl;

  Object* Ptr_ = &si.NULLOBJECT();
  assert(Ptr_->IsNullObject());

  // Testing bounds checking
  Character chk1 = 127;
  chk1 += 5;
  assert(chk1.Overflow());
  chk1.ClearError();
  chk1 = 127;
  chk1 = -chk1;
  --chk1;
  assert(!chk1.Underflow());
  --chk1;
  assert(chk1.Underflow());
  chk1.ClearError();
  chk1 = 16;
  chk1 *= chk1;
  assert(chk1.Overflow());
  chk1 = 16;
  chk1 >>= 2;
  assert(CharType(chk1) == 4);
  chk1 <<= 4;
  assert(CharType(chk1) == 64);
  chk1 = chk1 | Character(0x0F);
  assert(CharType(chk1) == 79);
  chk1 = chk1 / Character(0);
  assert(chk1.DivideByZero());
  assert(chk1.Overflow());
  assert(!chk1.NoError());

//  chk1 = 5;
//  Increment(chk1);
//  cout <<"inc(5) == " <<((int)chk1) <<endl;
//  Decrement(chk1);
//  cout <<"dec(6) == " <<((int)chk1) <<endl;

  // testing type wrapper exceptions
  try
  {
    chk1.ThrowError();
  }
  catch (const SciEngErr& ErrObj_)
  {
    cerr <<ErrObj_.message();
  }

  // Testing series comparison class on user defined type
  Character* List_[25];
  int i;

  for (i = 0; i < 10; ++i)
    List_[i] = new Character(i+1);
  List_[10] = NULL;

  Character ZeroChar(0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(1), List_, ZeroChar, CHAR_MIN, CHAR_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::SumOfSignedSeriesCmp(Character(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  assert(SeriesCompare<Character>::DifferenceOfSignedSeriesCmp(Character(-11), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() > 0);
  assert(SeriesCompare<Character>::DifferenceOfSignedSeriesCmp(Character(-55), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() == 0);
  assert(SeriesCompare<Character>::DifferenceOfSignedSeriesCmp(Character(-77), List_, ZeroChar, CHAR_MIN, CHAR_MAX).Result() < 0);
  
  delete List_[5];
  List_[5] = NULL;

  SeriesCompare<Character> Pobj_ = SeriesCompare<Character>::ProductOfSignedSeriesCmp(Character(120), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() == 0);
  Pobj_ = SeriesCompare<Character>::ProductOfSignedSeriesCmp(Character(121), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() > 0);
  Pobj_ = SeriesCompare<Character>::ProductOfSignedSeriesCmp(Character(119), List_, ZeroChar, CHAR_MIN, CHAR_MAX);
  assert(Pobj_.Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  cout <<endl <<"SUCCESS Testing Char" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





