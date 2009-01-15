// Custom ChrString Class
//
#ifndef BYTESTR_H
  #include "bytestr.h"
#endif

/****************************************************************************/
/*************************** Key Word Token Class ***************************/
/****************************************************************************/
ByteStringToken::ByteStringToken():
_Delims(NULL),
_WhiteSpace(NULL),
_Index(0),
_Start(0)
{}

/****************************************************************************/
ByteStringToken::ByteStringToken(const ByteStringToken& Stok_):
_SearchStr(Stok_.GetString()),
_Delims(Stok_.GetDelimiters()),
_WhiteSpace(Stok_.GetWhiteSpace()),
_Index(Stok_.GetIndex()),
_Start(Stok_.GetStartIndex())
{}

/****************************************************************************/
ByteStringToken::ByteStringToken(const ByteString& Str_, size_t Start_, const char* Delims_, const char* Wspc_):
_SearchStr(Str_),
_Delims(Delims_),
_WhiteSpace(Wspc_),
_Index(Start_),
_Start(Start_)
{}

/****************************************************************************/
ByteStringToken& ByteStringToken::operator = (const ByteStringToken& Stok_)
{
  if (&Stok_ != this)
  {
    _SearchStr = Stok_.GetString();
    _Delims = Stok_.GetDelimiters();
    _WhiteSpace = Stok_.GetWhiteSpace();
    _Index = Stok_.GetIndex();
    _Start = Stok_.GetStartIndex();
  }

  return *this;
}

/****************************************************************************/
size_t ByteStringToken::FindToken(ByteString& Token_)
{
  if (!_SearchStr.strlen())
    return 0;

  const char* Delimiters_ = (_Delims == NULL) ? " ":_Delims;

  size_t NextPos_;
  size_t CopyLen_ = FindNextByteStr(_SearchStr.c_str(), _SearchStr.strlen(), Delimiters_, _Index, NextPos_);

  if (CopyLen_)
  {
    Token_.AssignStr(_SearchStr.c_str() + _Index, CopyLen_);

    if (_WhiteSpace)
    {
      RemoveByte(Token_.c_str(), CopyLen_, _WhiteSpace);
      Token_.Delete(CopyLen_, Token_.strlen() - CopyLen_);
    }

    _Index = NextPos_;
  }

  return CopyLen_;
}

/****************************************************************************/
ByteStringToken& ByteStringToken::SetStartIndex(size_t Index_)
{
  _Start = Index_;
  return *this;
}

/****************************************************************************/
ByteStringToken& ByteStringToken::ResetIndex()
{
  _Index = _Start;
  return *this;
}

/****************************************************************************/
ByteStringToken& ByteStringToken::SetDelimiters(const char* Str_)
{
  _Delims = Str_;
  return *this;
}

/****************************************************************************/
ByteStringToken& ByteStringToken::SetWhiteSpace(const char* Str_)
{
  _WhiteSpace = Str_;
  return *this;
}

/****************************************************************************/
ByteStringToken& ByteStringToken::SetString(const ByteString& Str_)
{
  _SearchStr = Str_;
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ByteStringToken::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ByteStringToken::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* ByteStringToken::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void ByteStringToken::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
ByteStringData::ByteStringData(Subscript Sz_):
StringData(Sz_),
_Length(Sz_)
{}

/****************************************************************************/
ByteStringData::ByteStringData(char* Str_, Subscript Len_, Subscript Sz_):
StringData(Str_, Sz_),
_Length(Len_)
{}

/****************************************************************************/
StringData* ByteStringData::NewInstance(Subscript Sz_)
{
  return (new ByteStringData(Sz_));
}

/****************************************************************************/
StringData* ByteStringData::NewInstance(char* Str_, Subscript Len_, Subscript Sz_)
{
  return (new ByteStringData(Str_, Len_, Sz_));
}

/****************************************************************************/
void ByteStringData::Length(Subscript Value_)
{
  _Length = Value_;
}

/****************************************************************************/
Subscript ByteStringData::Length() const
{
  return _Length;
}

/****************************************************************************/
ByteStringData* ByteStringData::Make(Subscript Sz_)
{
  return (new ByteStringData(Sz_));
}

/****************************************************************************/
ByteStringData* ByteStringData::Make(char* Str_, Subscript Len_, Subscript Sz_)
{
  return (new ByteStringData(Str_, Len_, Sz_));
}

/****************************************************************************/
/****************************************************************************/
ByteString::ByteString() :
StringImp(ByteStringData::Make(0))
{
  TerminateString(0);
}

/****************************************************************************/
ByteString::ByteString(char c, Subscript n) :
StringImp(ByteStringData::Make(StringData::AllocateStr(n), n, n))
{
  TerminateString(n);

  while (n)
  {
    --n;
    _StrData->data[n] = c;
  }
}

/****************************************************************************/
ByteString::ByteString(const char* s, Subscript n) :
StringImp(ByteStringData::Make((char*)SafeMemMove(StringData::AllocateStr(n), s, n), n, n))
{}

/****************************************************************************/
ByteString::ByteString(const StringImp& s) :
StringImp(s, ByteStringData::Make(
  (char*)SafeMemMove(StringData::AllocateStr(s.strlen()), s.c_str(), s.strlen()),
  s.strlen(), s.strlen()))
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
ByteString::ByteString(const ByteString& s):
StringImp((const StringImp&)s, ByteStringData::Make(
  (char*)SafeMemMove(StringData::AllocateStr(s.strlen()), s.c_str(), s.strlen()),
  s.strlen(), s.strlen()))
{}
#endif
/****************************************************************************/
ByteString::ByteString(ByteString& Obj_, int, int):
StringImp(Obj_._StrData->ShareInstance())
{}

/****************************************************************************/
ByteString::ByteString(char*, int, int):
StringImp(ByteStringData::Make(ChrStringData::DummyData(), 0, 0))
{
  TerminateString(0);
}

/****************************************************************************/
ByteString::ByteString(const ByteString& s1, const ByteString& s2) :
StringImp(ByteStringData::Make(
  ((char*)SafeMemMove((char*)SafeMemMove(StringData::AllocateStr(s1.strlen() + s2.strlen() + 1), s1.c_str(), s1.strlen()) + s1.strlen(), s2.c_str(), s2.strlen()) - s1.strlen()),
  (s1.strlen() + s2.strlen()), (s1.strlen() + s2.strlen())))
{}

/****************************************************************************/
ByteString* ByteString::Make()
{
  return (new ByteString());
}

/****************************************************************************/
ByteString* ByteString::Make(char Ch_, Subscript n)
{
  return (new ByteString(Ch_, n));
}

/****************************************************************************/
ByteString* ByteString::Make(const char* Str_, Subscript n)
{
  return (new ByteString(Str_, n));
}

/****************************************************************************/
ByteString* ByteString::Make(const Object& Obj_)
{
  ByteStringAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
void ByteString::TerminateString(size_t Sz_)
{
  _StrData->Length(Sz_);
}

/****************************************************************************/
Object& ByteString::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  ByteStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromByteString(TrgComp_)));
}

/****************************************************************************/
Object* ByteString::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new ByteString(*this));

  ByteStringAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromByteString(TrgComp_)));
}

/****************************************************************************/
ByteString& ByteString::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static ByteString _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& ByteString::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ByteString::GiveNullObject();
}

/****************************************************************************/
Object& ByteString::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ByteString::GiveNullObject();
}

/****************************************************************************/
void ByteString::AutoUpdatePtrs(Boolean Flag_)
{
  StringData::SetAutoUpdate(Flag_);
}

/****************************************************************************/
long ByteString::DataSize () const
{
  return strlen();
}

/****************************************************************************/
long ByteString::StorableFormWidth() const
{
  return (long(sizeof(long) +
               sizeof(long) +
	       sizeof(Byte_t) +
	       sizeof(Subscript)) +
	  DataSize());
}

/****************************************************************************/
int ByteString::ClassID() const
{
  return TypeInfo::BYTESTRING_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* ByteString::GiveObjectAcceptor() const
{
  return (new ByteStringAcceptor(this));
}

/****************************************************************************/
istream& ByteString::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Subscript StrLen_;
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    if (Is_.read((char*)(&StrLen_), sizeof(Subscript)).good())
      if (Is_.seekg(StrLen_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& ByteString::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Subscript StrLen_;
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    if (Is_.read((char*)(&StrLen_), sizeof(Subscript)).good())
    {
      if (_StrData->_Size < StrLen_)
	ReallocateData(StrLen_);

      if (Is_.read(_StrData->data, StrLen_).good())
      {
	TerminateString(StrLen_);
	SetIOstate(TRUE, IO_CLEAR, TRUE);
      }
    }
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& ByteString::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Subscript StrLen_ = DataSize();
    Os_.write((char*)(&StrLen_), sizeof(Subscript));

    if (OstreamCheck(Os_, Ok_) && _StrData->data)
      Os_.write(_StrData->data, StrLen_);
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& ByteString::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Subscript StrLen_;
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    if (Is_.iread((char*)(&StrLen_), sizeof(Subscript)).good())
      if (Is_.iseekg(StrLen_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& ByteString::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Subscript StrLen_;
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    if (Is_.iread((char*)(&StrLen_), sizeof(Subscript)).good())
    {
      if (_StrData->_Size < StrLen_)
	ReallocateData(StrLen_);

      if (Is_.iread(_StrData->data, StrLen_).good())
      {
	TerminateString(StrLen_);
	SetIOstate(TRUE, IO_CLEAR, TRUE);
      }
    }
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& ByteString::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Subscript StrLen_ = DataSize();
    Os_.owrite((char*)(&StrLen_), sizeof(Subscript));

    if (OstreamCheck(((ostream&)Os_), Ok_) && _StrData->data)
      Os_.owrite(_StrData->data, StrLen_);
  }

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& ByteString::TextRead(istream& Is_, Boolean* Ok_)
{
  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& ByteString::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
Boolean ByteString::IsEqualToByteString(const ByteString* Ptr_) const
{
  if (Ptr_)
    if (_StrData->data == Ptr_->_StrData->data)
      return TRUE;
    else if (strlen() == Ptr_->strlen())
      return (memcmp(_StrData->data, Ptr_->_StrData->data, strlen()) == 0);

    return FALSE;
}

/****************************************************************************/
Boolean ByteString::IsLesserToByteString(const ByteString* Ptr_) const
{
  if (Ptr_)
    if (_StrData->data == Ptr_->_StrData->data)
      return FALSE;
    else
    {
      int Result_;
      size_t CmpLen_ = (strlen() < Ptr_->strlen()) ? strlen():Ptr_->strlen();
      Result_ = memcmp(_StrData->data, Ptr_->_StrData->data, CmpLen_);
      
      if (Result_ < 0)
        return TRUE;
      else if (Result_ > 0)
        return FALSE;
      else
      {
        if (strlen() < Ptr_->strlen())
          return TRUE;
        else
          return FALSE;
      }
    }

    return FALSE;
}

/****************************************************************************/
Boolean ByteString::IsGreaterToByteString(const ByteString* Ptr_) const
{
  if (Ptr_)
    if (_StrData->data == Ptr_->_StrData->data)
      return FALSE;
    else
    {
      int Result_;
      size_t CmpLen_ = (strlen() < Ptr_->strlen()) ? strlen():Ptr_->strlen();
      Result_ = memcmp(_StrData->data, Ptr_->_StrData->data, CmpLen_);
      
      if (Result_ > 0)
        return TRUE;
      else if (Result_ < 0)
        return FALSE;
      else
      {
        if (strlen() > Ptr_->strlen())
          return TRUE;
        else
          return FALSE;
      }    
    }    

    return FALSE;
}

/****************************************************************************/
Boolean ByteString::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ByteStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToByteString(TrgComp_);
}

/****************************************************************************/
Boolean ByteString::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ByteStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToByteString(TrgComp_);
}

/****************************************************************************/
Boolean ByteString::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ByteStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToByteString(TrgComp_);
}

/****************************************************************************/
ByteString& ByteString::AssignStr(const char* Str_, Subscript Len_)
{
  StringImp::Assignment(Str_, Len_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::operator = (const StringImp& Str_)
{
  StringImp::Assignment(Str_);
  return *this;
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
ByteString& ByteString::operator = (const ByteString& Str_)
{
  StringImp::Assignment(Str_);
  return *this;
}
#endif
/****************************************************************************/
ByteString& ByteString::operator = (char Chr_)
{
  StringImp::Assignment(Chr_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::operator += (const ByteString& rhs)
{
  StringImp::Addition(rhs);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::operator -= (const ByteString& rhs)
{
  StringImp::Subtraction(rhs);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Delete(const ByteString& rhs)
{
  Fallible<Subscript> Index_ = findlast(rhs);

  if (Index_.valid())
    Delete(Index_, rhs.strlen());

  return *this;
}

/****************************************************************************/
ByteString operator + (const ByteString& lhs, const ByteString& rhs)
{
    return ByteString(lhs, rhs);
}

/****************************************************************************/
ByteString operator - (const ByteString& lhs, const ByteString& rhs)
{
    ByteString Temp_ = lhs;
    return (Temp_ -= rhs);
}

/****************************************************************************/
ByteString Delete(const ByteString& lhs, const ByteString& rhs)
{
  ByteString Temp_ = lhs;
  return Temp_.Delete(rhs);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ByteString::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ByteString::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* ByteString::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void ByteString::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
ByteString& ByteString::ToUpper(void)
{
  StringImp::ToUpper();
  return *this;
}

/****************************************************************************/
ByteString& ByteString::ToLower(void)
{
  StringImp::ToLower();
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Right(Subscript len)
{
  StringImp::Right(len);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Left(Subscript len)
{
  StringImp::Left(len);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Mid(Subscript pos, Subscript len)
{
  StringImp::Mid(pos, len);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::LeftJustify(Subscript FieldWidth_, char Pad_)
{
  StringImp::LeftJustify(FieldWidth_, Pad_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::RightJustify(Subscript FieldWidth_, char Pad_)
{
  StringImp::RightJustify(FieldWidth_, Pad_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::CenterJustify(Subscript FieldWidth_, char Pad_)
{
  StringImp::CenterJustify(FieldWidth_, Pad_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Insert(Subscript pos, const ByteString& Str_)
{
  StringImp::Insert(pos, Str_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Append(const ByteString& rhs, Subscript RunLen_)
{
  StringImp::Append(rhs, RunLen_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::OverWrite(Subscript pos, const ByteString& Str_)
{
  StringImp::OverWrite(pos, Str_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Delete(Subscript pos, Subscript len)
{
  StringImp::Delete(pos, len);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::SetAllBytesTo(char Pad_)
{
  for (Subscript Index_ = 0; Index_ < _StrData->Length(); ++Index_)
    _StrData->data[Index_] = Pad_;

  return *this;
}

/****************************************************************************/
Subscript ByteString::Replace(Subscript pos,
			  const ByteString& from,
			  const ByteString& to,
			  Subscript count)
{
  if (from == to)
    return 0;

  int IdenticalFrom_ = (this == &from);
  ByteString* FromPtr_ = IdenticalFrom_ ? (ByteString*)from.Clone():((ByteString*)&from);

  int IdenticalTo_ = (this == &to);
  ByteString* ToPtr_ = IdenticalTo_ ? (ByteString*)to.Clone():((ByteString*)&to);

  ByteString NextStr_(&_StrData->data[pos], strlen() - pos);
  Fallible<Subscript> Result_ = NextStr_.find(*FromPtr_);
  if (Result_.failed())
    return 0;

  Subscript Start_ = Result_;
  Subscript LenTo_ = ToPtr_->strlen();
  Subscript LenFrom_ = FromPtr_->strlen();

  if (LenFrom_ < LenTo_)
  {
    Subscript Extra_ = (strlen() - Start_) / LenFrom_;
    Extra_ *= (LenTo_ - LenFrom_);
    ReallocateData(_StrData->_Size + Extra_);
  }

  Subscript NumReplaced_ = 0;
  for (;;)
  {
    if (LenTo_ == LenFrom_)
      OverWrite(Start_, *ToPtr_);
    else
    {
      Delete(Start_, LenFrom_);
      Insert(Start_, *ToPtr_);
    }

    NumReplaced_++;
    if (NumReplaced_ == count || Start_ >= strlen())
      break;

    Result_ = findnext(Start_, *FromPtr_);
    if (Result_.valid())
      Start_ = (Subscript)Result_;
    else
      break;
  }

  if (IdenticalFrom_)
    delete FromPtr_;

  if (IdenticalTo_)
    delete ToPtr_;

  return NumReplaced_;
}

/****************************************************************************/
ByteString& ByteString::MakeCharSet(int(*iscsetfn)(int))
{
  StringImp::MakeCharSet(iscsetfn);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::MakeCharSet(const char* Src_, size_t RunLen_)
{
  StringImp::MakeCharSet(Src_, RunLen_);
  return *this;
}

/****************************************************************************/
ByteString& ByteString::Duplicate(Subscript Num_)
{
  StringImp::Duplicate(Num_);
  return *this;
}

/****************************************************************************/
/****************************************************************************/
Fallible<Subscript> ByteString::find(char find_me) const
{
    const char* foundp = (char*)memchr(_StrData->data, find_me, strlen());
    if (foundp) return Fallible<Subscript>(foundp - _StrData->data, NULL);
    else        return Fallible<Subscript>();
}

/****************************************************************************/
Fallible<Subscript> ByteString::findlast(char find_me) const
{
    ByteString TempStr_ = *this;
    TempStr_.StrRev();
    Fallible<Subscript> Result_ = TempStr_.find(find_me);

    if (Result_.valid()) return Fallible<Subscript>(strlen() - Result_ - 1, NULL);
    else 		 return Fallible<Subscript>();
}

/****************************************************************************/
Fallible<Subscript> ByteString::find(const ByteString& find_me) const
{
    Subscript Index_;
    for (Index_ = 0;
	 Index_ < strlen() && find_me.strlen() <= strlen() - Index_; Index_++)
      if (!memcmp(&_StrData->data[Index_], find_me._StrData->data, find_me.strlen()))
	return Fallible<Subscript>(Index_, NULL);

    return Fallible<Subscript>();
}

/****************************************************************************/
Fallible<Subscript> ByteString::findlast(const ByteString& s)   const
{
    ByteString NewStr_ = *this;
    ByteString find_me = s;

    NewStr_.StrRev();
    find_me.StrRev();

    char* NewData_ = NewStr_.c_str();
    Subscript Index_;

    for (Index_ = 0;
	 Index_ < strlen() && find_me.strlen() <= strlen() - Index_; Index_++)
      if (!memcmp(&NewData_[Index_], find_me._StrData->data, find_me.strlen()))
	return Fallible<Subscript>(NewStr_.strlen() - s.strlen() - Index_, NULL);

    return Fallible<Subscript>();
}

/****************************************************************************/
Fallible<Subscript> ByteString::findnext(Subscript pos, char c)   const
{
  Subscript NextPos_ = (_StrData->data[pos] == c) ? (pos + 1):pos;
  if (NextPos_ >= strlen())
    return Fallible<Subscript>();

  ByteString NextStr_(&_StrData->data[NextPos_], strlen() - NextPos_);
  Fallible<Subscript> Result_ = NextStr_.find(c);

  if (Result_.valid())
    return Fallible<Subscript>(Result_ + NextPos_, NULL);

  return Result_;
}

/****************************************************************************/
Fallible<Subscript> ByteString::findnext(Subscript pos, const ByteString& s)   const
{
  Subscript NextPos_ = !memcmp(&_StrData->data[pos], s.c_str(), s.strlen()) ? (pos + s.strlen()):pos;
  if (NextPos_ >= strlen())
    return Fallible<Subscript>();

  ByteString NextStr_(&_StrData->data[NextPos_], strlen() - NextPos_);
  Fallible<Subscript> Result_ = NextStr_.find(s);

  if (Result_.valid())
    return Fallible<Subscript>(Result_ + NextPos_, NULL);

  return Result_;
}

/****************************************************************************/
Fallible<Subscript> ByteString::findprevious(Subscript pos, char c)   const
{
  if (pos < 1)
    return Fallible<Subscript>();

  Subscript Len_ = (_StrData->data[pos] == c) ? pos:(pos + 1);
  if (Len_ >= strlen())
    return Fallible<Subscript>();

  ByteString NextStr_ = subString(0, Len_);
  return NextStr_.findlast(c);
}

/****************************************************************************/
Fallible<Subscript> ByteString::findprevious(Subscript pos, const ByteString& s)   const
{
  if (pos < s.strlen())
    return Fallible<Subscript>();

  Subscript Len_ = !memcmp(&_StrData->data[pos], s.c_str(), s.strlen()) ? pos:(pos + 1);
  if (Len_ >= strlen())
    return Fallible<Subscript>();

  ByteString NextStr_ = subString(0, Len_);
  return NextStr_.findlast(s);
}

/****************************************************************************/
Fallible<char> ByteString::operator [] (Subscript i) const
{
  return
  (
    IndexInRange(i) ?
	Fallible<char>(_StrData->data[i], NULL):
	Fallible<char>()
  );
}

/****************************************************************************/
Fallible<char> ByteString::operator [] (Subscript i)
{
  return
  (
    IndexInRange(i) ?
	Fallible<char>(_StrData->data[i], &_StrData->data[i]):
	Fallible<char>()
  );
}

/****************************************************************************/
const char* ByteString::operator () (Subscript i) const
{
  return (IndexInRange(i) ? (_StrData->data + i):NULL);
}

/****************************************************************************/
char* ByteString::operator () (Subscript i)
{
  return (IndexInRange(i) ? (_StrData->data + i):NULL);
}

/****************************************************************************/
char ByteString::operator * () const
{
  return (*this)[0];
}

/****************************************************************************/
char& ByteString::operator * ()
{
#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(__DJGPP__)
  return (*this)[0];
#else
  char& ChRef_ = (char&)((*this)[0]);
  return ChRef_;
#endif
}

/****************************************************************************/
Fallible<Subscript> ByteString::brk(const ByteString& any_of_me) const
{
    Subscript Index_;
    const char* foundp = NULL;
    for (Index_ = 0; foundp == NULL && Index_ < any_of_me.strlen(); Index_++)
      foundp = (char*)memchr(_StrData->data, any_of_me._StrData->data[Index_], strlen());

    if (foundp)return Fallible<Subscript>(foundp - _StrData->data, NULL);
    else       return Fallible<Subscript>();
}

/****************************************************************************/
Subscript ByteString::span(const ByteString& consisting_of_me) const
{
  Subscript Sindex_, Tindex_;
  for (Sindex_ = 0; Sindex_ < strlen(); Sindex_++)
    for (Tindex_ = 0;; Tindex_++)
      if (Tindex_ == consisting_of_me.strlen())
	return Sindex_;
      else if (_StrData->data[Sindex_] == consisting_of_me._StrData->data[Tindex_])
	break;

  return Sindex_;
}

/****************************************************************************/
Subscript ByteString::cspan(const ByteString& consisting_not_of_me) const
{
  Subscript Sindex_, Tindex_;
  for (Sindex_ = 0; Sindex_ < strlen(); Sindex_++)
    for (Tindex_ = 0; Tindex_ < consisting_not_of_me.strlen(); Tindex_++)
      if (_StrData->data[Sindex_] == consisting_not_of_me._StrData->data[Tindex_])
	return Sindex_;

  return Sindex_;
}

/****************************************************************************/
ByteString ByteString::subString(Subscript start) const
{
  Subscript strt =  (start <= strlen()) ? start : strlen();  // null if too far from beginning
  Subscript rn = strlen() - strt;

  if (rn)
    return ByteString(_StrData->data + strt, rn);
  return ByteString();
}

/****************************************************************************/
ByteString ByteString::subString(Subscript start, Subscript run_len) const
{
  Subscript strt =  (start < strlen()) ? start : strlen();  // null if too far from beginning
  Subscript rn = (start + run_len <= strlen()) ? run_len : strlen() - strt;

  if (rn)
    return ByteString(_StrData->data + strt, rn);
  return ByteString();
}

/****************************************************************************/
ByteString& operator << (ByteString& s, const char* p)
{
  // Write a C string into a ByteString.  This is a specialization
  // of the operator<<(ByteString, const T&) template function that bypasses
  // using ostrstream.
  return s += ByteString(p, ::strlen(p));
}

/****************************************************************************/
Fallible<double> ByteString::strtod() const
{
  ChrString NewStr_(c_str(), strlen());
  return NewStr_.strtod();
}

/****************************************************************************/
Fallible<long> ByteString::strtol() const
{
  ChrString NewStr_(c_str(), strlen());
  return NewStr_.strtol();
}

/****************************************************************************/
Fallible<unsigned long> ByteString::strtoul() const
{
  ChrString NewStr_(c_str(), strlen());
  return NewStr_.strtoul();
}

/****************************************************************************/
// String termination
StringImp& ByteString::NullTerminate()
{
  _StrData->data[strlen()] = 0;
  return *this;
}

/****************************************************************************/
ByteString& ByteString::StrRev()
{
  StringImp::StrRev();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
ByteStringAcceptor::ByteStringAcceptor(const ByteString* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
// Data transfer methods
void ByteStringAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const ByteString*)Ptr_;
}

/****************************************************************************/
const void* ByteStringAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
// ByteString class specific acceptor methods
Boolean ByteStringAcceptor::IsByteString() const
{
  return TRUE;
}

/****************************************************************************/
Boolean ByteStringAcceptor::IsStringImp() const
{
  return TRUE;
}

/****************************************************************************/
void* ByteStringAcceptor::CreateFromByteString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new ByteString(*((const StringImp*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* ByteStringAcceptor::AssignFromByteString(const ObjectAcceptor& Obj_)
{
  const StringImp* StrPtr_ = (const StringImp*)Obj_.GiveDataPtr();
  *_VarPtr = *StrPtr_;
  return _VarPtr;
}

/****************************************************************************/
void* ByteStringAcceptor::CreateFromChrString(const ObjectAcceptor& Obj_)
{
  return CreateFromByteString(Obj_);
}

/****************************************************************************/
void* ByteStringAcceptor::AssignFromChrString(const ObjectAcceptor& Obj_)
{
  return AssignFromByteString(Obj_);
}

/****************************************************************************/
Boolean ByteStringAcceptor::IsEqualToByteString(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToByteString((const ByteString*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean ByteStringAcceptor::IsLesserToByteString(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToByteString((const ByteString*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean ByteStringAcceptor::IsGreaterToByteString(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToByteString((const ByteString*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean ByteStringAcceptor::IsEqualToChrString(const ObjectAcceptor& Obj_) const
{
  ByteString Temp_(*((const StringImp*)Obj_.GiveDataPtr()));
  return _ObjPtr->IsEqualToByteString(&Temp_);
}

/****************************************************************************/
Boolean ByteStringAcceptor::IsLesserToChrString(const ObjectAcceptor& Obj_) const
{
  ByteString Temp_(*((const StringImp*)Obj_.GiveDataPtr()));
  return _ObjPtr->IsLesserToByteString(&Temp_);
}

/****************************************************************************/
Boolean ByteStringAcceptor::IsGreaterToChrString(const ObjectAcceptor& Obj_) const
{
  ByteString Temp_(*((const StringImp*)Obj_.GiveDataPtr()));
  return _ObjPtr->IsGreaterToByteString(&Temp_);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ByteStringAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ByteStringAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
ByteStringEncLetter::ByteStringEncLetter():
ByteString(NULL, 0, 0)
{}

/****************************************************************************/
ByteStringEncLetter::ByteStringEncLetter(ByteString& Obj_):
ByteString(Obj_, 0, 0)
{}

/****************************************************************************/
ByteStringEncLetter::ByteStringEncLetter(const ByteStringEncLetter& Obj_):
ByteString((ByteString&)Obj_, 0, 0)
{}

/****************************************************************************/
const ByteStringEncLetter& ByteStringEncLetter::ToUpper(void) const
{
  // Cast away const and call ByteString::ToUpper
  ByteString *const LocalThis_ = (ByteString *const)this;
  (*LocalThis_).ByteString::ToUpper();
  return *this;
}

/****************************************************************************/
const ByteStringEncLetter& ByteStringEncLetter::ToLower(void) const
{
  // Cast away const and call ByteString::ToLower
  ByteString *const LocalThis_ = (ByteString *const)this;
  (*LocalThis_).ByteString::ToLower();
  return *this;
}

/****************************************************************************/
const ByteStringEncLetter& ByteStringEncLetter::Delete(Subscript pos, Subscript len) const
{
  // Cast away const and call ByteString::Delete
  ByteString *const LocalThis_ = (ByteString *const)this;
  (*LocalThis_).ByteString::Delete(pos, len);
  return *this;
}

/****************************************************************************/
ByteString& ByteStringEncLetter::Delete(const ByteString& rhs)
{
  // Cast this to ByteString and call ByteString::Delete
  return ByteString::Delete(rhs);
}

/****************************************************************************/
const ByteStringEncLetter& ByteStringEncLetter::Delete(const ByteString& rhs) const
{
  // Cast away const and call ByteString::Delete
  ((ByteStringEncLetter *const)this)->Delete(rhs);
  return *this;
}

/****************************************************************************/
const ByteStringEncLetter& ByteStringEncLetter::Truncate(const ByteString& rhs) const
{
  // Cast away const and call ByteString::operator -= (const ByteString&)
  *((ByteStringEncLetter *const)this) -= rhs;
  return *this;
}

/****************************************************************************/
const ByteStringEncLetter& ByteStringEncLetter::StrRev() const
{
  // Cast away const and call ByteString::StrRev
  ByteString *const LocalThis_ = (ByteString *const)this;
  (*LocalThis_).ByteString::StrRev();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
ByteStringPtr::ByteStringPtr()
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ByteStringPtr::ByteStringPtr(ByteString& Obj_):
_Letter(Obj_)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ByteStringPtr::ByteStringPtr(const ByteStringPtr& Obj_):
_Letter(Obj_._Letter)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ByteStringPtr& ByteStringPtr::operator = (ByteString& Obj_)
{
  _Letter.AssignLetter(Obj_);
  _LetterPtr = &_Letter;
  return *this;
}

/****************************************************************************/
ByteStringPtr& ByteStringPtr::operator = (const ByteStringPtr& Obj_)
{
  _Letter.AssignLetter((ByteString&)Obj_._Letter);
  _LetterPtr = &_Letter;
  return *this;
}

/****************************************************************************/
ByteStringPtr& ByteStringPtr::operator -= (const ByteString& rhs)
{
  _LetterPtr->Truncate(rhs);
  return *this;
}

/****************************************************************************/
Subscript ByteStringPtr::RestrictIndex(Subscript Index_) const
{
  return _LetterPtr->RestrictIndex(Index_);
}

/****************************************************************************/
Boolean ByteStringPtr::IndexInRange(Subscript Index_) const
{
  return _LetterPtr->IndexInRange(Index_);
}

/****************************************************************************/
Fallible<char> ByteStringPtr::operator [] (Subscript i) const
{
  return (*_LetterPtr)[i];
}

/****************************************************************************/
Fallible<char> ByteStringPtr::operator [] (Subscript i)
{
  return _Letter[i];
}

/****************************************************************************/
const char* ByteStringPtr::operator () (Subscript i) const
{
  return (*_LetterPtr)(i);
}

/****************************************************************************/
char* ByteStringPtr::operator () (Subscript i)
{
  return _Letter(i);
}

/****************************************************************************/
char ByteStringPtr::operator * () const
{
  return (**_LetterPtr);
}

/****************************************************************************/
char& ByteStringPtr::operator * ()
{
  return (*_Letter);
}

/****************************************************************************/
ByteStringPtr::operator const char* () const
{
  return _LetterPtr->c_str();
}

/****************************************************************************/
ByteStringPtr::operator char* ()
{
  return _Letter.c_str();
}

/****************************************************************************/
const ByteStringEncLetter* ByteStringPtr::operator -> () const
{
  return _LetterPtr;
}

/****************************************************************************/
const ByteString& ByteStringPtr::EnclosedObject() const
{
  return _Letter;
}

/****************************************************************************/
Boolean ByteStringPtr::IsOld() const
{
  return _LetterPtr->IsOld();
}

/****************************************************************************/
/****************************************************************************/
ConstByteStringPtr::ConstByteStringPtr(const ByteString& Obj_):
_Letter((ByteString&)Obj_, 0, 0)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ConstByteStringPtr::ConstByteStringPtr(const ConstByteStringPtr& Obj_):
_Letter((ByteString&)Obj_._Letter, 0, 0)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
Subscript ConstByteStringPtr::RestrictIndex(Subscript Index_) const
{
  return _LetterPtr->RestrictIndex(Index_);
}

/****************************************************************************/
Boolean ConstByteStringPtr::IndexInRange(Subscript Index_) const
{
  return _LetterPtr->IndexInRange(Index_);
}

/****************************************************************************/
Fallible<char> ConstByteStringPtr::operator [] (Subscript i) const
{
  return (*_LetterPtr)[i];
}

/****************************************************************************/
const char* ConstByteStringPtr::operator () (Subscript i) const
{
  return (*_LetterPtr)(i);
}

/****************************************************************************/
char ConstByteStringPtr::operator * () const
{
  return (**_LetterPtr);
}

/****************************************************************************/
// conversion to C built in string type
ConstByteStringPtr::operator const char* () const
{
  return _LetterPtr->c_str();
}

/****************************************************************************/
// return pointer to enclosed ByteStringEncLetter object
const ByteString* ConstByteStringPtr::operator -> () const
{
  return _LetterPtr;
}

/****************************************************************************/
const ByteString& ConstByteStringPtr::EnclosedObject() const
{
  return _Letter;
}

/****************************************************************************/
Boolean ConstByteStringPtr::IsOld() const
{
  return _LetterPtr->IsOld();
}

/****************************************************************************/
/****************************************************************************/
#if BYTESTRING_DEBUG
#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(__DJGPP__)
  #include <conio.h>
#endif

int main()
{
#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(__DJGPP__)
  clrscr();
#endif

  ByteString Str1_;
  ByteString Str2_("Hello World", 11);
  ByteString Str3_("GoodBye World", 8);
  ByteString Str4_('x', 5);
  ByteString Str5_(Str3_);
  ConstByteStringPtr StrPtr1a_(Str1_);
  ConstByteStringPtr StrPtr2_(Str2_);
  ConstByteStringPtr StrPtr2a_(Str2_);

  ByteStringPtr StrPtr6_;
  ByteStringPtr StrPtr7_;
  ByteStringPtr StrPtr8_;
  ByteString* Str6_ = ByteString::Make();
  ByteString* Str7_ = ByteString::Make("Hello World", 11);
  ByteString* Str8_ = ByteString::Make("GoodBye World", 10);
  ByteString* Str9_ = ByteString::Make('x', 5);
  ByteString* Str10_ = ByteString::Make(*Str8_);
  StrPtr6_ = *Str6_;
  StrPtr7_ = *Str7_;
  StrPtr8_ = *Str8_;
  ByteStringPtr StrPtr9_(*Str9_);
  ByteStringPtr StrPtr10_(*Str10_);

  // testing indexing
  Subscript Lp_; Boolean Ok_; Boolean LpOk_;
  for (Lp_ = 0; (LpOk_ = Str2_[Lp_].valid()); ++Lp_)
  {
    Ok_ =
	Lp_ == 0 ? Str2_[Lp_] == 'H':
	Lp_ == 1 ? Str2_[Lp_] == 'e':
	Lp_ == 2 ||
	Lp_ == 3 ||
	Lp_ == 9 ? Str2_[Lp_] == 'l':
	Lp_ == 4 ||
	Lp_ == 7 ? Str2_[Lp_] == 'o':
	Lp_ == 5 ? Str2_[Lp_] == ' ':
	Lp_ == 6 ? Str2_[Lp_] == 'W':
	Lp_ == 8 ? Str2_[Lp_] == 'r':
	Lp_ == 10 ? Str2_[Lp_] == 'd':0;
    assert(Ok_);
  }
  assert(Lp_ == 11);
  assert(!LpOk_);
  assert(!Str2_.IndexInRange(Lp_));
  assert(Str2_.RestrictIndex(Lp_) == 10);

  // testing assignment, case conversion, justification
  Str1_ = Str2_;
  assert(StrPtr1a_.IsOld());
  Str2_.ToUpper();
  StrPtr7_->ToLower();
  Str3_.RightJustify(20);
  StrPtr8_ -= ByteString("Wo", 2);
  StrPtr8_->Delete(0, 4);
  Str8_->LeftJustify(20);
  assert(StrPtr8_.IsOld());
  StrPtr8_ = *Str8_;
  assert(!StrPtr8_.IsOld());
  Str4_.CenterJustify(20);

  // Testing duplication, insertion
  Str1_.Duplicate(3);
  *Str6_ = *Str7_;
  assert(StrPtr6_.IsOld());
  StrPtr6_ = *Str6_;
  assert(!StrPtr6_.IsOld());
  Str6_->Insert(3, ByteString("Inserted", 8));
  assert(StrPtr6_.IsOld());
  StrPtr6_ = *Str6_;
  assert(!StrPtr6_.IsOld());

#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout  <<ChrString(Str1_.c_str(), Str1_.strlen()) <<endl
	<<ChrString(Str2_.c_str(), Str2_.strlen()) <<endl
	<<ChrString(Str3_.c_str(), Str3_.strlen()) <<endl
	<<ChrString(Str4_.c_str(), Str4_.strlen()) <<endl
	<<ChrString(Str5_.c_str(), Str5_.strlen()) <<endl
	<<ChrString(Str6_->c_str(), Str6_->strlen()) <<endl
	<<ChrString(Str7_->c_str(), Str7_->strlen()) <<endl
	<<ChrString(Str8_->c_str(), Str8_->strlen()) <<endl
	<<ChrString(Str9_->c_str(), Str9_->strlen()) <<endl
	<<ChrString(Str10_->c_str(), Str10_->strlen()) <<endl;
#endif

  assert(Str1_ == "Hello WorldHello WorldHello WorldHello World");
  assert(StrPtr2_.EnclosedObject() == "HELLO WORLD");
  assert(Str3_ == "            GoodBye ");
  assert(Str4_ == "       xxxxx        ");
  assert(Str5_ == "GoodBye ");
  assert(*Str6_ == "helInsertedlo world");
  assert(*Str7_ == "hello world");
  assert(StrPtr8_.EnclosedObject() == "Bye                 ");
  assert(StrPtr9_.EnclosedObject() == "xxxxx");
  assert(*Str10_ == "GoodBye Wo");

  // testing string tokenizing
  ByteString Buffer_;
  ByteStringToken Tok_(Str1_, 0, " ", " ");
  Tok_.FindToken(Buffer_);
#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<ChrString(Buffer_.c_str(), Buffer_.strlen()) <<"-";
#endif
  assert(Buffer_ == "Hello");
  for (int Index_ = 0; Index_ < 3; Index_++)
  {
    Tok_.FindToken(Buffer_);
    assert(Buffer_ == "WorldHello");
#if BYTESTRING_SHOW_DEBUG_OUTPUT
    cout <<ChrString(Buffer_.c_str(), Buffer_.strlen()) <<"-";
#endif
  }
  Tok_.FindToken(Buffer_);
  assert(Buffer_ == "World");
#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<ChrString(Buffer_.c_str(), Buffer_.strlen()) <<endl;
#endif

  // testing deletion, overwriting, replacement
  Str2_.Delete(4, 2);
  Str7_->OverWrite(0, ByteString("Bye  ", 5));
  assert(!StrPtr7_.IsOld());
  Str1_.Replace(0, ByteString("ll", 2), ByteString("nn", 2));

#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout  <<ChrString(Str2_.c_str(), Str2_.strlen()) <<endl
	<<ChrString(Str7_->c_str(), Str7_->strlen()) <<endl
	<<ChrString(Str1_.c_str(), Str1_.strlen()) <<endl;
#endif

  assert(Str2_ == "HELLWORLD");
  assert(*Str7_ == "Bye   world");
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHenno World");

  // testing string searches
  ConstByteStringPtr StrPtr1_(Str1_);
  Fallible<Subscript> Fall1_ = StrPtr1_->find(ByteString("Bye", 3));
  Fallible<Subscript> Fall2_ = StrPtr1_->findlast(ByteString("Good", 4));
  Subscript Pos3_ = StrPtr1_->find(ByteString("World", 5));
  Subscript Pos4_ = StrPtr1_->findlast(ByteString("Henno", 5));
  Subscript Pos5_ = StrPtr1_->findnext(Pos3_, ByteString("World", 5));
  Subscript Pos6_ = StrPtr1_->findprevious(Pos4_, ByteString("Henno", 5));

  // testing BASIC emulation -- Left, Mid, Right methods
  ByteString Str11_ = Str3_.Right(5);
  ByteString Str12_ = Str8_->Left(5);
  ByteString Str13_ = Str4_.Mid(10, 10);

#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout  <<Str2_ <<endl
	<<*Str7_ <<endl
	<<Str1_ <<endl
        <<Fall1_.valid() <<endl
	<<Fall2_.valid() <<endl
	<<Pos3_ <<endl
	<<Pos4_ <<endl
	<<Pos5_ <<endl
	<<Pos6_ <<endl
	<<ChrString(Str11_.c_str(), Str11_.strlen()) <<endl
	<<ChrString(Str12_.c_str(), Str12_.strlen()) <<endl
	<<ChrString(Str13_.c_str(), Str13_.strlen()) <<endl;
#endif

  assert((int)Fall1_.valid() == 0);
  assert((int)Fall2_.valid() == 0);
  assert(Pos3_ == 6);
  assert(Pos4_ == 33);
  assert(Pos5_ == 17);
  assert(Pos6_ == 22);
  assert(Str11_ == "dBye ");
  assert(Str12_ == "Bye  ");
  assert(Str13_ == "xx        ");

  // testing subtraction
  Str1_ -= ByteString("no World", 8);
#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<Str1_ <<endl;
#endif
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHen");

  // testing file IO
  ofstream fout("test.txt", ios::out|ios::binary);
  Str1_.BinaryWrite(fout);
  fout.close();

  ifstream fin("test.txt", ios::in|ios::binary);
  Str11_.BinaryRead(fin);
  fin.close();

#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<Str11_ <<endl;
#endif
  assert(Str11_ == "Henno WorldHenno WorldHenno WorldHen");

  // testing deletion
  ByteString Str14_("JoKCsephKCWong", 14);
  Str14_.Delete(ByteString("KC", 2));
#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<Str14_ <<endl;
#endif
  assert(Str14_ == "JoKCsephWong");
  ByteString Str15_ = Delete(Str14_, ByteString("KC", 2));
#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<Str15_ <<endl;
#endif
  assert(Str15_ == "JosephWong");

  // testing addition
  Str15_ += ByteString(" This is a test of appending", 28);
#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<ChrString(Str15_.c_str(), Str15_.strlen()) <<endl;
#endif
  assert(Str15_ == "JosephWong This is a test of appending");

  // testing searches again
  Pos3_ = (Subscript)Str15_.find('s');
  Pos4_ = (Subscript)Str15_.findlast('s');
  Pos5_ = (Subscript)Str15_.findnext(Pos3_, 's');
  Pos6_ = (Subscript)Str15_.findprevious(Pos4_, 's');
  assert(Pos3_ == 2);
  assert(Pos4_ == 23);
  assert(Pos5_ == 14);
  assert(Pos6_ == 17);

  // testing appending
  Str15_ -= ByteString(" This is a test of appending", 28);
  assert(Str15_ == "JosephWong");
  assert((Str15_ + ByteString(" This is a test of appending", 28)) == "JosephWong This is a test of appending");
  Str15_.Append(Str2_, 4);
  assert(Str15_ == "JosephWongHELL");

  // brk, span and cspan methods
  Fall1_ = Str15_.brk(ByteString("eph", 3));
  assert(Fall1_.valid());
  Pos3_ = (Subscript)Fall1_;
  assert(Pos3_ == 3);
  Pos4_ = Str15_.span(ByteString("toseJy", 6));
  Pos5_ = Str15_.cspan(ByteString("Wpht", 4));

  assert(Pos4_ == 4);
  assert(Pos5_ == 4);

  // testing null object return method
  Object* Ptr_ = &Str6_->NULLOBJECT();
#if BYTESTRING_SHOW_DEBUG_OUTPUT
  cout <<"Is Null : " <<Ptr_->IsNullObject() <<endl;
#endif
  assert(Ptr_->IsNullObject());

  // testing data sharing, memory management
  delete Str6_;
  delete Str7_;
  delete Str8_;
  delete Str9_;
  delete Str10_;

#if BYTESTRING_NOTIFY_SUCCESS
  cout <<endl <<"SUCCESS Testing ByteString" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/




