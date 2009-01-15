// Custom ChrString Class
//
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif

/****************************************************************************/
int CompString(const ChrString& lhs, const ChrString& rhs)
{
  return
  (
    (lhs < rhs) ? -1:
    (lhs > rhs) ? 1:0
  );
}

/****************************************************************************/
/****************************************************************************/
ChrStringData::ChrStringData(Subscript Sz_):
StringData(Sz_)
{}

/****************************************************************************/
ChrStringData::ChrStringData(char* Str_, Subscript Len_, Subscript Sz_):
StringData(Str_, Sz_)
{}

/****************************************************************************/
StringData* ChrStringData::NewInstance(Subscript Sz_)
{
  return Make(Sz_);
}

/****************************************************************************/
StringData* ChrStringData::NewInstance(char* Str_, Subscript Len_, Subscript Sz_)
{
  return Make(Str_, Len_, Sz_);
}

/****************************************************************************/
void ChrStringData::Length(Subscript)
{}

/****************************************************************************/
Subscript ChrStringData::Length() const
{
  return SafeStrLen(data);
}

/****************************************************************************/
ChrStringData* ChrStringData::Make(Subscript Sz_)
{
  return (new ChrStringData(Sz_));
}

/****************************************************************************/
ChrStringData* ChrStringData::Make(char* Str_, Subscript Len_, Subscript Sz_)
{
  return (new ChrStringData(Str_, Len_, Sz_));
}

/****************************************************************************/
/****************************************************************************/
ChrString::ChrString():
StringImp(ChrStringData::Make(0))
{
  TerminateString(0);
}

/****************************************************************************/
ChrString::ChrString(const char* s):
StringImp(ChrStringData::Make(
  SafeStrCpy(StringData::AllocateStr(SafeStrLen(s)), s), 0,
  SafeStrLen(s)))
{}

/****************************************************************************/
ChrString::ChrString(char c, Subscript n):
StringImp(ChrStringData::Make(StringData::AllocateStr(n), 0, n))
{
  TerminateString(n);

  while (n)
  {
    --n;
    _StrData->data[n] = c;
  }
}

/****************************************************************************/
ChrString::ChrString(const char* s, Subscript n) :
StringImp(ChrStringData::Make(SafeStrnCpy(StringData::AllocateStr(n), s, n), 0, n))
{
  TerminateString(n);
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
ChrString::ChrString(const ChrString& s):
StringImp((const StringImp&)s, ChrStringData::Make(
  (char*)SafeMemMove(StringData::AllocateStr(s.strlen()), s.c_str(), s.strlen()),
  0, s.strlen()))
{
  TerminateString(s.strlen());
}
#endif
/****************************************************************************/
ChrString::ChrString(const StringImp& s) :
StringImp(s, ChrStringData::Make(
  (char*)SafeMemMove(StringData::AllocateStr(s.strlen()), s.c_str(), s.strlen()),
  0, s.strlen()))
{
  TerminateString(s.strlen());
}

/****************************************************************************/
ChrString::ChrString(ChrString& Obj_, int, int):
StringImp(Obj_._StrData->ShareInstance())
{}

/****************************************************************************/
ChrString::ChrString(char*, int, int):
StringImp(ChrStringData::Make(ChrStringData::DummyData(), 0, 0))
{
  TerminateString(0);
}

/****************************************************************************/
ChrString::ChrString(const ChrString& s1, const ChrString& s2) :
StringImp(ChrStringData::Make(
  SafeStrCat(SafeStrCpy(StringData::AllocateStr(s1.strlen() + s2.strlen()), s1.c_str()), s2.c_str()),
  0, (s1.strlen() + s2.strlen())))
{}

/****************************************************************************/
ChrString* ChrString::Make()
{
  return (new ChrString());
}

/****************************************************************************/
ChrString* ChrString::Make(char Ch_, Subscript n)
{
  return (new ChrString(Ch_, n));
}

/****************************************************************************/
ChrString* ChrString::Make(const char* Str_)
{
  return (new ChrString(Str_));
}

/****************************************************************************/
ChrString* ChrString::Make(const char* Str_, Subscript n)
{
  return (new ChrString(Str_, n));
}

/****************************************************************************/
ChrString* ChrString::Make(const Object& Obj_)
{
  ChrStringAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
void ChrString::TerminateString(size_t Sz_)
{
  _StrData->data[Sz_] = 0;
}

/****************************************************************************/
Object& ChrString::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  ChrStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromChrString(TrgComp_)));
}

/****************************************************************************/
Object* ChrString::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new ChrString(*this));

  ChrStringAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromChrString(TrgComp_)));
}

/****************************************************************************/
ChrString& ChrString::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static ChrString _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& ChrString::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ChrString::GiveNullObject();
}

/****************************************************************************/
Object& ChrString::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ChrString::GiveNullObject();
}

/****************************************************************************/
void ChrString::AutoUpdatePtrs(Boolean Flag_)
{
  StringData::SetAutoUpdate(Flag_);
}

/****************************************************************************/
long ChrString::DataSize () const
{
  return strlen();
}

/****************************************************************************/
long ChrString::StorableFormWidth () const
{
  return (long(sizeof(long) +
               sizeof(long) +
	       sizeof(Byte_t) +
	       sizeof(Subscript)) +
	  DataSize());
}

/****************************************************************************/
int ChrString::ClassID() const
{
  return TypeInfo::STRING_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* ChrString::GiveObjectAcceptor() const
{
  return (new ChrStringAcceptor(this));
}

/****************************************************************************/
istream& ChrString::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  Subscript StrLen_;
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
istream& ChrString::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  Subscript StrLen_;
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
ostream& ChrString::BinaryWrite(ostream& Os_, Boolean* Ok_) const
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
ByteStream& ChrString::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  Subscript StrLen_;
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
ByteStream& ChrString::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  Subscript StrLen_;
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
ByteStream& ChrString::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
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
istream& ChrString::TextRead(istream& Is_, Boolean* Ok_)
{
    const int bufsize = 100;
    char buf[bufsize];

    *this = ChrString();
    while (Is_.width(bufsize), Is_>> buf)
    {
	*this += buf;
	int next_input_char = Is_.peek();

	if (isspace(next_input_char))
	{
	    // Read terminated by white space, not lack of room --> stop reading
	    break;
	}
    }

    return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& ChrString::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  if (_StrData->data)
    Os_ <<c_str();

  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
Boolean ChrString::IsEqualToChrString(const ChrString* Ptr_) const
{
  if (Ptr_)
    if (_StrData->data == Ptr_->_StrData->data)
      return TRUE;
    else if (strlen() == Ptr_->strlen())
      return strcmp(_StrData->data, Ptr_->_StrData->data) == 0;

  return FALSE;
}

/****************************************************************************/
Boolean ChrString::IsLesserToChrString(const ChrString* Ptr_) const
{
  if (Ptr_)
    if (_StrData->data == Ptr_->_StrData->data)
      return FALSE;
    else
      return strcmp(_StrData->data, Ptr_->_StrData->data) < 0;

  return FALSE;
}

/****************************************************************************/
Boolean ChrString::IsGreaterToChrString(const ChrString* Ptr_) const
{
  if (Ptr_)
    if (_StrData->data == Ptr_->_StrData->data)
      return FALSE;
    else
      return strcmp(_StrData->data, Ptr_->_StrData->data) > 0;

  return FALSE;
}

/****************************************************************************/
Boolean ChrString::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ChrStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToChrString(TrgComp_);
}

/****************************************************************************/
Boolean ChrString::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ChrStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToChrString(TrgComp_);
}

/****************************************************************************/
Boolean ChrString::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ChrStringAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToChrString(TrgComp_);
}

/****************************************************************************/
ChrString& ChrString::operator = (const StringImp& Str_)
{
  StringImp::Assignment(Str_);
  return *this;
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
ChrString& ChrString::operator = (const ChrString& Str_)
{
  StringImp::Assignment(Str_);
  return *this;
}
#endif
/****************************************************************************/
ChrString& ChrString::operator = (const char* Str_)
{
  StringImp::Assignment(Str_, SafeStrLen(Str_));
  return *this;
}

/****************************************************************************/
ChrString& ChrString::operator = (char Chr_)
{
  StringImp::Assignment(Chr_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::operator += (const ChrString& rhs)
{
  StringImp::Addition(rhs);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::operator -= (const ChrString& rhs)
{
  StringImp::Subtraction(rhs);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Delete(const ChrString& rhs)
{
  Fallible<Subscript> Index_ = findlast(rhs);

  if (Index_.valid())
    Delete(Index_, rhs.strlen());

  return *this;
}

/****************************************************************************/
ChrString operator + (const ChrString& lhs, const ChrString& rhs)
{
  return ChrString(lhs, rhs);
}

/****************************************************************************/
ChrString operator - (const ChrString& lhs, const ChrString& rhs)
{
  ChrString Temp_ = lhs;
  return (Temp_ -= rhs);
}

/****************************************************************************/
ChrString Delete(const ChrString& lhs, const ChrString& rhs)
{
  ChrString Temp_ = lhs;
  return Temp_.Delete(rhs);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ChrString::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ChrString::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* ChrString::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void ChrString::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
ChrString& ChrString::ToUpper(void)
{
  StringImp::ToUpper();
  return *this;
}

/****************************************************************************/
ChrString& ChrString::ToLower(void)
{
  StringImp::ToLower();
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Right(Subscript len)
{
  StringImp::Right(len);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Left(Subscript len)
{
  StringImp::Left(len);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Mid(Subscript pos, Subscript len)
{
  StringImp::Mid(pos, len);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::LeftJustify(Subscript FieldWidth_, char Pad_)
{
  StringImp::LeftJustify(FieldWidth_, Pad_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::RightJustify(Subscript FieldWidth_, char Pad_)
{
  StringImp::RightJustify(FieldWidth_, Pad_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::CenterJustify(Subscript FieldWidth_, char Pad_)
{
  StringImp::CenterJustify(FieldWidth_, Pad_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Insert(Subscript pos, const ChrString& Str_)
{
  StringImp::Insert(pos, Str_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Append(const ChrString& rhs, Subscript RunLen_)
{
  StringImp::Append(rhs, RunLen_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::OverWrite(Subscript pos, const ChrString& Str_)
{
  StringImp::OverWrite(pos, Str_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Delete(Subscript pos, Subscript len)
{
  StringImp::Delete(pos, len);
  return *this;
}

/****************************************************************************/
Subscript ChrString::Replace(Subscript pos,
			  const ChrString& from,
			  const ChrString& to,
			  Subscript count)
{
  if (from == to)
    return 0;

  int IdenticalFrom_ = (this == &from);
  ChrString* FromPtr_ = IdenticalFrom_ ? (ChrString*)from.Clone():((ChrString*)&from);

  int IdenticalTo_ = (this == &to);
  ChrString* ToPtr_ = IdenticalTo_ ? (ChrString*)to.Clone():((ChrString*)&to);

  ChrString NextStr_ = &_StrData->data[pos];
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
	  #if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
        Start_ = Result_.operator Subscript& ();
	  #else
		Start_ = Result_;
	  #endif
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
ChrString& ChrString::MakeCharSet(int(*iscsetfn)(int))
{
  StringImp::MakeCharSet(iscsetfn);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::MakeCharSet(const char* Src_, size_t RunLen_)
{
  StringImp::MakeCharSet(Src_, RunLen_);
  return *this;
}

/****************************************************************************/
ChrString& ChrString::Duplicate(Subscript Num_)
{
  StringImp::Duplicate(Num_);
  return *this;
}

/****************************************************************************/
/****************************************************************************/
Fallible<Subscript> ChrString::find(char find_me) const
{
    const char* foundp = NULL;

    if (_StrData->data)
      foundp = strchr(_StrData->data, find_me);

    if (foundp) return Fallible<Subscript>(foundp - _StrData->data, NULL);
    else        return Fallible<Subscript>();

}

/****************************************************************************/
Fallible<Subscript> ChrString::findlast(char find_me) const
{
    const char* foundp = NULL;

    if (_StrData->data)
      foundp = strrchr(_StrData->data, find_me);

    if (foundp) return Fallible<Subscript>(foundp - _StrData->data, NULL);
    else        return Fallible<Subscript>();
}

/****************************************************************************/
Fallible<Subscript> ChrString::find(const ChrString& find_me) const
{
    const char* foundp = NULL;

    if (_StrData->data)
      foundp = strstr(_StrData->data, find_me._StrData->data);

    if (foundp) return Fallible<Subscript>(foundp - _StrData->data, NULL);
    else        return Fallible<Subscript>();
}

/****************************************************************************/
Fallible<Subscript> ChrString::findlast(const ChrString& s)   const
{
    ChrString NewStr_ = *this;
    ChrString find_me = s;

    NewStr_.StrRev();
    find_me.StrRev();

    char* NewData_ = NewStr_.c_str();
    const char* foundp = strstr(NewData_, find_me._StrData->data);

    if (foundp)
      return Fallible<Subscript>(NewStr_.strlen() - s.strlen() - (foundp - NewData_), NULL);
    else
      return Fallible<Subscript>();
}

/****************************************************************************/
Fallible<Subscript> ChrString::findnext(Subscript pos, char c)   const
{
  Subscript NextPos_ = (_StrData->data[pos] == c) ? (pos + 1):pos;
  if (NextPos_ >= strlen())
    return Fallible<Subscript>();

  ChrString NextStr_ = &_StrData->data[NextPos_];
  Fallible<Subscript> Result_ = NextStr_.find(c);

  if (Result_.valid())
    return Fallible<Subscript>(Result_ + NextPos_, NULL);

  return Result_;
}

/****************************************************************************/
Fallible<Subscript> ChrString::findnext(Subscript pos, const ChrString& s)   const
{
  Subscript NextPos_ = !strncmp(&_StrData->data[pos], s.c_str(), s.strlen()) ? (pos + s.strlen()):pos;
  if (NextPos_ >= strlen())
    return Fallible<Subscript>();

  ChrString NextStr_ = &_StrData->data[NextPos_];
  Fallible<Subscript> Result_ = NextStr_.find(s);

  if (Result_.valid())
    return Fallible<Subscript>(Result_ + NextPos_, NULL);

  return Result_;
}

/****************************************************************************/
Fallible<Subscript> ChrString::findprevious(Subscript pos, char c)   const
{
  if (pos < 1)
    return Fallible<Subscript>();

  Subscript Len_ = (_StrData->data[pos] == c) ? pos:(pos + 1);
  if (Len_ >= strlen())
    return Fallible<Subscript>();

  ChrString NextStr_ = subString(0, Len_);
  return NextStr_.findlast(c);
}

/****************************************************************************/
Fallible<Subscript> ChrString::findprevious(Subscript pos, const ChrString& s)   const
{
  if (pos < s.strlen())
    return Fallible<Subscript>();

  Subscript Len_ = !strncmp(&_StrData->data[pos], s.c_str(), s.strlen()) ? pos:(pos + 1);
  if (Len_ >= strlen())
    return Fallible<Subscript>();

  ChrString NextStr_ = subString(0, Len_);
  return NextStr_.findlast(s);
}

/****************************************************************************/
Fallible<char> ChrString::operator [] (Subscript i) const
{
  return
  (
    IndexInRange(i) ?
	Fallible<char>(_StrData->data[i], NULL):
	Fallible<char>()
  );
}

/****************************************************************************/
Fallible<char> ChrString::operator [] (Subscript i)
{
  return
  (
    IndexInRange(i) ?
	Fallible<char>(_StrData->data[i], &_StrData->data[i]):
	Fallible<char>()
  );
}

/****************************************************************************/
const char* ChrString::operator () (Subscript i) const
{
  return (IndexInRange(i) ? (_StrData->data + i):NULL);
}

/****************************************************************************/
char* ChrString::operator () (Subscript i)
{
  return (IndexInRange(i) ? (_StrData->data + i):NULL);
}

/****************************************************************************/
char ChrString::operator * () const
{
  return (*this)[0];
}

/****************************************************************************/
char& ChrString::operator * ()
{
#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(__DJGPP__)
  return (*this)[0];
#else
  char& ChRef_ = (char&)((*this)[0]);
  return ChRef_;
#endif
}

/****************************************************************************/
Fallible<Subscript> ChrString::brk(const ChrString& any_of_me) const
{
    const char* foundp = NULL;

    if (_StrData->data)
      foundp = strpbrk(_StrData->data, any_of_me._StrData->data);

    if (foundp) return Fallible<Subscript>(foundp - _StrData->data, NULL);
    else        return Fallible<Subscript>();
}

/****************************************************************************/
Subscript ChrString::span(const ChrString& consisting_of_me) const
{
  if (_StrData->data)
    return strspn(_StrData->data, consisting_of_me._StrData->data);
  return 0;
}

/****************************************************************************/
Subscript ChrString::cspan(const ChrString& consisting_not_of_me) const
{
  if (_StrData->data)
    return strcspn(_StrData->data, consisting_not_of_me._StrData->data);
  return 0;
}

/****************************************************************************/
ChrString ChrString::subString(Subscript start) const
{
  Subscript strt =  (start <= strlen()) ? start : strlen();  // null if too far from beginning
  return ChrString(_StrData->data + strt);
}

/****************************************************************************/
ChrString ChrString::subString(Subscript start, Subscript run_len) const
{
  Subscript strt =  (start < strlen()) ? start : strlen();  // null if too far from beginning
  Subscript rn = ( start + run_len <= strlen() ) ? run_len : strlen();
  return ChrString(_StrData->data + strt, rn);
}

/****************************************************************************/
ChrString& operator << (ChrString& s, const char* p)
{
  // Write a C string into a ChrString.  This is a specialization
  // of the operator<<(ChrString, const T&) template function that bypasses
  // using ostrstream.
  return s += ChrString(p);
}

/****************************************************************************/
Fallible<double> ChrString::strtod() const
{
    errno = 0; // Clear error indication
    char* endp;
    double result = ::strtod(_StrData->data, &endp);
    if (errno || *endp) return Fallible<double>();
    else                return Fallible<double>(result, NULL);
}

/****************************************************************************/
Fallible<long> ChrString::strtol() const
{
  errno = 0;  // Clear error indication
  char* endp;
  long result = ::strtol(_StrData->data, &endp, 0);
  if (errno || *endp) return Fallible<long>();
  else             return Fallible<long>(result, NULL);
}

/****************************************************************************/
Fallible<unsigned long> ChrString::strtoul() const
{
  errno = 0;  // Clear error indication
  char* endp;
  Ulong result = ::strtoul(_StrData->data, &endp, 0);
  if (errno || *endp) return Fallible<unsigned long>();
  else             return Fallible<unsigned long>(result, NULL);
}

/****************************************************************************/
// String termination
StringImp& ChrString::NullTerminate()
{
  return *this;
}

/****************************************************************************/
ChrString& ChrString::StrRev()
{
  StringImp::StrRev();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
ChrStringAcceptor::ChrStringAcceptor(const ChrString* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
// Data transfer methods
void ChrStringAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const ChrString*)Ptr_;
}

/****************************************************************************/
const void* ChrStringAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
// ChrString class specific acceptor methods
Boolean ChrStringAcceptor::IsString() const
{
  return TRUE;
}

/****************************************************************************/
// ChrString class specific acceptor methods
Boolean ChrStringAcceptor::IsStringImp() const
{
  return TRUE;
}

/****************************************************************************/
void* ChrStringAcceptor::CreateFromChrString(const ObjectAcceptor& Obj_)
{
  _VarPtr = new ChrString(*((const StringImp*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* ChrStringAcceptor::AssignFromChrString(const ObjectAcceptor& Obj_)
{
  const StringImp* StrPtr_ = (const StringImp*)Obj_.GiveDataPtr();
  *_VarPtr = *StrPtr_;
  return _VarPtr;
}

/****************************************************************************/
void* ChrStringAcceptor::CreateFromByteString(const ObjectAcceptor& Obj_)
{
  return CreateFromChrString(Obj_);
}

/****************************************************************************/
void* ChrStringAcceptor::AssignFromByteString(const ObjectAcceptor& Obj_)
{
  return AssignFromChrString(Obj_);
}

/****************************************************************************/
Boolean ChrStringAcceptor::IsEqualToChrString(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToChrString((const ChrString*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean ChrStringAcceptor::IsLesserToChrString(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToChrString((const ChrString*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean ChrStringAcceptor::IsGreaterToChrString(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToChrString((const ChrString*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean ChrStringAcceptor::IsEqualToByteString(const ObjectAcceptor& Obj_) const
{
  ChrString Temp_(*((const StringImp*)Obj_.GiveDataPtr()));
  return _ObjPtr->IsEqualToChrString(&Temp_);
}

/****************************************************************************/
Boolean ChrStringAcceptor::IsLesserToByteString(const ObjectAcceptor& Obj_) const
{
  ChrString Temp_(*((const StringImp*)Obj_.GiveDataPtr()));
  return _ObjPtr->IsLesserToChrString(&Temp_);
}

/****************************************************************************/
Boolean ChrStringAcceptor::IsGreaterToByteString(const ObjectAcceptor& Obj_) const
{
  ChrString Temp_(*((const StringImp*)Obj_.GiveDataPtr()));
  return _ObjPtr->IsGreaterToChrString(&Temp_);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ChrStringAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ChrStringAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif

/****************************************************************************/
/****************************************************************************/
ChrStringEncLetter::ChrStringEncLetter():
ChrString(NULL, 0, 0)
{}

/****************************************************************************/
ChrStringEncLetter::ChrStringEncLetter(ChrString& Obj_):
ChrString(Obj_, 0, 0)
{}

/****************************************************************************/
ChrStringEncLetter::ChrStringEncLetter(const ChrStringEncLetter& Obj_):
ChrString((ChrString&)Obj_, 0, 0)
{}

/****************************************************************************/
const ChrStringEncLetter& ChrStringEncLetter::ToUpper(void) const
{
  // Cast away const and call ChrString::ToUpper
  ChrString *const LocalThis_ = (ChrString *const)this;
  (*LocalThis_).ChrString::ToUpper();
  return *this;
}

/****************************************************************************/
const ChrStringEncLetter& ChrStringEncLetter::ToLower(void) const
{
  // Cast away const and call ChrString::ToLower
  ChrString *const LocalThis_ = (ChrString *const)this;
  (*LocalThis_).ChrString::ToLower();
  return *this;
}

/****************************************************************************/
const ChrStringEncLetter& ChrStringEncLetter::Delete(Subscript pos, Subscript len) const
{
  // Cast away const and call ChrString::Delete
  ChrString *const LocalThis_ = (ChrString *const)this;
  (*LocalThis_).ChrString::Delete(pos, len);
  return *this;
}

/****************************************************************************/
ChrString& ChrStringEncLetter::Delete(const ChrString& rhs)
{
  // Cast this to ChrString and call ChrString::Delete
  return ChrString::Delete(rhs);
}

/****************************************************************************/
const ChrStringEncLetter& ChrStringEncLetter::Delete(const ChrString& rhs) const
{
  // Cast away const and call ChrString::Delete
  ((ChrStringEncLetter *const)this)->Delete(rhs);
  return *this;
}

/****************************************************************************/
const ChrStringEncLetter& ChrStringEncLetter::Truncate(const ChrString& rhs) const
{
  // Cast away const and call ChrString::operator -= (const ChrString&)
  *((ChrStringEncLetter *const)this) -= rhs;
  return *this;
}

/****************************************************************************/
const ChrStringEncLetter& ChrStringEncLetter::StrRev() const
{
  // Cast away const and call ChrString::StrRev
  ChrString *const LocalThis_ = (ChrString *const)this;
  (*LocalThis_).ChrString::StrRev();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
ChrStringPtr::ChrStringPtr()
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ChrStringPtr::ChrStringPtr(ChrString& Obj_):
_Letter(Obj_)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ChrStringPtr::ChrStringPtr(const ChrStringPtr& Obj_):
_Letter(Obj_._Letter)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ChrStringPtr& ChrStringPtr::operator = (ChrString& Obj_)
{
  _Letter.AssignLetter(Obj_);
  _LetterPtr = &_Letter;
  return *this;
}

/****************************************************************************/
ChrStringPtr& ChrStringPtr::operator = (const ChrStringPtr& Obj_)
{
  _Letter.AssignLetter((ChrString&)Obj_._Letter);
  _LetterPtr = &_Letter;
  return *this;
}

/****************************************************************************/
ChrStringPtr& ChrStringPtr::operator -= (const ChrString& rhs)
{
  _LetterPtr->Truncate(rhs);
  return *this;
}

/****************************************************************************/
Subscript ChrStringPtr::RestrictIndex(Subscript Index_) const
{
  return _LetterPtr->RestrictIndex(Index_);
}

/****************************************************************************/
Boolean ChrStringPtr::IndexInRange(Subscript Index_) const
{
  return _LetterPtr->IndexInRange(Index_);
}

/****************************************************************************/
Fallible<char> ChrStringPtr::operator [] (Subscript i) const
{
  return (*_LetterPtr)[i];
}

/****************************************************************************/
Fallible<char> ChrStringPtr::operator [] (Subscript i)
{
  return _Letter[i];
}

/****************************************************************************/
const char* ChrStringPtr::operator () (Subscript i) const
{
  return (*_LetterPtr)(i);
}

/****************************************************************************/
char* ChrStringPtr::operator () (Subscript i)
{
  return _Letter(i);
}

/****************************************************************************/
char ChrStringPtr::operator * () const
{
  return (**_LetterPtr);
}

/****************************************************************************/
char& ChrStringPtr::operator * ()
{
  return (*_Letter);
}

/****************************************************************************/
ChrStringPtr::operator const char* () const
{
  return _LetterPtr->c_str();
}

/****************************************************************************/
ChrStringPtr::operator char* ()
{
  return _Letter.c_str();
}

/****************************************************************************/
const ChrStringEncLetter* ChrStringPtr::operator -> () const
{
  return _LetterPtr;
}

/****************************************************************************/
const ChrString& ChrStringPtr::EnclosedObject() const
{
  return _Letter;
}

/****************************************************************************/
Boolean ChrStringPtr::IsOld() const
{
  return _LetterPtr->IsOld();
}

/****************************************************************************/
/****************************************************************************/
ConstChrStringPtr::ConstChrStringPtr(const ChrString& Obj_):
_Letter((ChrString&)Obj_, 0, 0)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
ConstChrStringPtr::ConstChrStringPtr(const ConstChrStringPtr& Obj_):
_Letter((ChrString&)Obj_._Letter, 0, 0)
{
  _LetterPtr = &_Letter;
}

/****************************************************************************/
Subscript ConstChrStringPtr::RestrictIndex(Subscript Index_) const
{
  return _LetterPtr->RestrictIndex(Index_);
}

/****************************************************************************/
Boolean ConstChrStringPtr::IndexInRange(Subscript Index_) const
{
  return _LetterPtr->IndexInRange(Index_);
}

/****************************************************************************/
Fallible<char> ConstChrStringPtr::operator [] (Subscript i) const
{
  return (*_LetterPtr)[i];
}

/****************************************************************************/
const char* ConstChrStringPtr::operator () (Subscript i) const
{
  return (*_LetterPtr)(i);
}

/****************************************************************************/
char ConstChrStringPtr::operator * () const
{
  return (**_LetterPtr);
}

/****************************************************************************/
// conversion to C built in string type
ConstChrStringPtr::operator const char* () const
{
  return _LetterPtr->c_str();
}

/****************************************************************************/
// return pointer to enclosed ChrStringEncLetter object
const ChrString* ConstChrStringPtr::operator -> () const
{
  return _LetterPtr;
}

/****************************************************************************/
const ChrString& ConstChrStringPtr::EnclosedObject() const
{
  return _Letter;
}

/****************************************************************************/
Boolean ConstChrStringPtr::IsOld() const
{
  return _LetterPtr->IsOld();
}

/****************************************************************************/
/****************************************************************************/
#if STRING_DEBUG
#if defined(__DJGPP__) | defined(__BORLANDC__) | defined(__TURBOC__)
  #include <conio.h>
#endif
int main()
{
#if defined(__DJGPP__) | defined(__BORLANDC__) | defined(__TURBOC__)
  clrscr();
#endif

  ChrString Str1b_;
  ChrString Str1a_;

  // test initial appends
  Str1a_ += "hello";
  assert(!SafeStrCmp(Str1a_.c_str(), "hello"));

  // testing base convsersions
  Str1b_.LongToStr(42, 2);
  Str1a_ = Str1b_;
  assert(!SafeStrCmp(Str1b_.c_str(), "101010"));
  Str1b_.BinToHex();
  assert(!SafeStrCmp(Str1b_.c_str(), "0x2A"));
  Str1b_ = Str1a_;  
  Str1b_.BinToCmp2Bin(4);
  assert(!SafeStrCmp(Str1b_.c_str(), "00000000000000000000000000101010"));
  Str1b_.Cmp2BinToBin();
  assert(!SafeStrCmp(Str1b_.c_str(), "101010"));

  Str1b_.LongToStr(147, 2);
  Str1a_ = Str1b_;
  assert(!SafeStrCmp(Str1b_.c_str(), "10010011"));
  Str1b_.BinToHex();
  assert(!SafeStrCmp(Str1b_.c_str(), "0x93"));
  Str1b_ = Str1a_;  
  Str1b_.BinToCmp2Bin(4);
  assert(!SafeStrCmp(Str1b_.c_str(), "00000000000000000000000010010011"));
  Str1b_.Cmp2BinToBin();
  assert(!SafeStrCmp(Str1b_.c_str(), "10010011"));

  Str1b_.LongToStr(255, 2);
  Str1a_ = Str1b_;
  assert(!SafeStrCmp(Str1b_.c_str(), "11111111"));
  Str1b_.BinToHex();
  assert(!SafeStrCmp(Str1b_.c_str(), "0xFF"));
  Str1b_ = Str1a_;  
  Str1b_.BinToCmp2Bin(4);
  assert(!SafeStrCmp(Str1b_.c_str(), "00000000000000000000000011111111"));
  Str1b_.Cmp2BinToBin();
  assert(!SafeStrCmp(Str1b_.c_str(), "11111111"));

  Str1b_.LongToStr(-152, 2);
  Str1a_ = Str1b_;
  assert(!SafeStrCmp(Str1b_.c_str(), "-10011000"));
  Str1b_.BinToHex();
  assert(!SafeStrCmp(Str1b_.c_str(), "-0x98"));
  Str1b_ = Str1a_;  
  Str1b_.BinToCmp2Bin(4);
  assert(!SafeStrCmp(Str1b_.c_str(), "11111111111111111111111101101000"));
  Str1b_.Cmp2BinToBin();
  assert(!SafeStrCmp(Str1b_.c_str(), "-10011000"));

  Str1b_.LongToStr(-255, 2);
  Str1a_ = Str1b_;  
  assert(!SafeStrCmp(Str1b_.c_str(), "-11111111"));
  Str1b_.BinToHex();
  assert(!SafeStrCmp(Str1b_.c_str(), "-0xFF"));
  Str1b_ = Str1a_;    
  Str1b_.BinToCmp2Bin(4);
  assert(!SafeStrCmp(Str1b_.c_str(), "11111111111111111111111100000001"));
  Str1b_.Cmp2BinToBin();
  assert(!SafeStrCmp(Str1b_.c_str(), "-11111111"));
  
  // starting initialization
  ChrString Str1_;
  ChrString Str2_("Hello World");
  ChrString Str3_("GoodBye World", 8);
  ChrString Str4_('x', 5);
  ChrString Str5_(Str3_);
  ConstChrStringPtr StrPtr1a_(Str1_);
  ConstChrStringPtr StrPtr2_(Str2_);
  ConstChrStringPtr StrPtr2a_(Str2_);

  ChrStringPtr StrPtr6_;
  ChrStringPtr StrPtr7_;
  ChrStringPtr StrPtr8_;
  ChrString* Str6_ = ChrString::Make();
  ChrString* Str7_ = ChrString::Make("Hello World");
  ChrString* Str8_ = ChrString::Make("GoodBye World", 10);
  ChrString* Str9_ = ChrString::Make('x', 5);
  ChrString* Str10_ = ChrString::Make(*Str8_);
  StrPtr6_ = *Str6_;
  StrPtr7_ = *Str7_;
  StrPtr8_ = *Str8_;
  ChrStringPtr StrPtr9_(*Str9_);
  ChrStringPtr StrPtr10_(*Str10_);

  // testing indexing
  Subscript Lp_; Boolean Ok_; Boolean LpOk_;
  for (Lp_ = 0; (LpOk_ = Str2_[Lp_].valid()); ++Lp_)
  {
    Ok_ =
	Lp_ == 0 ? *Str2_ == 'H':
	Lp_ == 1 ? *Str2_(Lp_) == 'e':
	Lp_ == 2 ||
	Lp_ == 3 ||
	Lp_ == 9 ? Str2_[Lp_] == 'l':
	Lp_ == 4 ||
	Lp_ == 7 ? Str2_[Lp_] == 'o':
	Lp_ == 5 ? Str2_[Lp_] == ' ':
	Lp_ == 6 ? Str2_[Lp_] == 'W':
	Lp_ == 8 ? Str2_[Lp_] == 'r':
	Lp_ == 10 ? Str2_[Lp_] == 'd':
	Lp_ == 11 ? Str2_[Lp_] == 0:0;
    assert(Ok_);
  }
  assert(Lp_ == 12);
  assert(!LpOk_);
  assert(!Str2_.IndexInRange(Lp_));
  assert(Str2_.RestrictIndex(Lp_) == 11);

  // Testing assignment, case conversion, justification
  Str1_ = Str2_;
  assert(StrPtr1a_.IsOld());
  Str2_.ToUpper();
  StrPtr7_->ToLower();
  Str3_.RightJustify(20);
  StrPtr8_ -= "Wo";
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
  Str6_->Insert(3, "Inserted");
  assert(StrPtr6_.IsOld());
  StrPtr6_ = *Str6_;
  assert(!StrPtr6_.IsOld());

#if STRING_SHOW_DEBUG_OUTPUT
  cout  <<Str1_ <<endl
	<<StrPtr2_.EnclosedObject() <<endl
	<<Str3_ <<endl
	<<Str4_ <<endl
	<<Str5_ <<endl
	<<*Str6_ <<endl
	<<*Str7_ <<endl
	<<StrPtr8_.EnclosedObject() <<endl
	<<StrPtr9_.EnclosedObject() <<endl
	<<*Str10_ <<endl;
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
  char* Buffer_ = new_char_array(20, NULL);
  StringToken Tok_(Str1_.c_str(), 0, " ", " ");
  Tok_.FindToken(Buffer_);
#if STRING_SHOW_DEBUG_OUTPUT
  cout <<Buffer_ <<"-";
#endif
  assert(!strcmp(Buffer_, "Hello"));
  for (int Index_ = 0; Index_ < 3; Index_++)
  {
    Tok_.FindToken(Buffer_);
    assert(!strcmp(Buffer_, "WorldHello"));
#if STRING_SHOW_DEBUG_OUTPUT
    cout <<Buffer_ <<"-";
#endif
  }
  Tok_.FindToken(Buffer_);
  assert(!strcmp(Buffer_, "World"));
#if STRING_SHOW_DEBUG_OUTPUT
  cout <<Buffer_ <<endl;
#endif
  ::DeleteArray(Buffer_);

  // testing deletion, overwriting, replacement
  Str2_.Delete(4, 2);
  Str7_->OverWrite(0, "Bye  ");
  assert(!StrPtr7_.IsOld());
  Str1_.Replace(0, "ll", "nn");

#if STRING_SHOW_DEBUG_OUTPUT
  cout  <<Str2_ <<endl
	<<*Str7_ <<endl
	<<Str1_ <<endl;
#endif

  assert(Str2_ == "HELLWORLD");
  assert(*Str7_ == "Bye   world");
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHenno World");

  // testing string searches
  ConstChrStringPtr StrPtr1_(Str1_);
  Fallible<Subscript> Fall1_ = StrPtr1_->find("Bye");
  Fallible<Subscript> Fall2_ = StrPtr1_->findlast("Good");
  Subscript Pos3_ = StrPtr1_->find("World");
  Subscript Pos4_ = StrPtr1_->findlast("Henno");
  Subscript Pos5_ = StrPtr1_->findnext(Pos3_, "World");
  Subscript Pos6_ = StrPtr1_->findprevious(Pos4_, "Henno");

  // testing BASIC emulation -- Left, Mid, Right methods
  ChrString Str11_ = Str3_.Right(5);
  ChrString Str12_ = Str8_->Left(5);
  ChrString Str13_ = Str4_.Mid(10, 10);

#if STRING_SHOW_DEBUG_OUTPUT
  cout  <<Str2_ <<endl
	<<*Str7_ <<endl
	<<Str1_ <<endl
	<<Fall1_.valid() <<endl
	<<Fall2_.valid() <<endl
	<<Pos3_ <<endl
	<<Pos4_ <<endl
	<<Pos5_ <<endl
	<<Pos6_ <<endl
	<<Str11_ <<endl
	<<Str12_ <<endl
	<<Str13_ <<endl;
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
  Str1_ -= "no World";
#if STRING_SHOW_DEBUG_OUTPUT
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

#if STRING_SHOW_DEBUG_OUTPUT
  cout <<Str11_ <<endl;
#endif
  assert(Str11_ == "Henno WorldHenno WorldHenno WorldHen");

  // testing deletion
  ChrString Str14_ = "JoKCsephKCWong";
  Str14_.Delete("KC");
#if STRING_SHOW_DEBUG_OUTPUT
  cout <<Str14_ <<endl;
#endif
  assert(Str14_ == "JoKCsephWong");
  ChrString Str15_ = Delete(Str14_, "KC");
#if STRING_SHOW_DEBUG_OUTPUT
  cout <<Str15_ <<endl;
#endif
  assert(Str15_ == "JosephWong");

  // testing addition
  Str15_ += " This is a test of appending";
#if STRING_SHOW_DEBUG_OUTPUT
  cout <<Str15_.c_str() <<endl;
#endif
  assert(Str15_ == "JosephWong This is a test of appending");

  // testing searches again
  Pos3_ = Str15_.find('s');
  Pos4_ = Str15_.findlast('s');
  Pos5_ = Str15_.findnext(Pos3_, 's');
  Pos6_ = Str15_.findprevious(Pos4_, 's');
  assert(Pos3_ == 2);
  assert(Pos4_ == 23);
  assert(Pos5_ == 14);
  assert(Pos6_ == 17);

  // testing appending
  Str15_ -= " This is a test of appending";
  assert(Str15_ == "JosephWong");
  assert((Str15_ + " This is a test of appending") == "JosephWong This is a test of appending");
  Str15_.Append(Str2_, 4);
  assert(Str15_ == "JosephWongHELL");

  // brk, span and cspan methods
  Fall1_ = Str15_.brk("eph");
  assert(Fall1_.valid());
  Pos3_ = Fall1_;
  assert(Pos3_ == 3);
  Pos4_ = Str15_.span("toseJy");
  Pos5_ = Str15_.cspan("Wpht");

  assert(Pos4_ == 4);
  assert(Pos5_ == 4);

  // testing null object return method
  Object* Ptr_ = &Str6_->NULLOBJECT();
#if STRING_SHOW_DEBUG_OUTPUT
  cout <<"Is Null : " <<Ptr_->IsNullObject() <<endl;
#endif
  assert(Ptr_->IsNullObject());

  // testing data sharing, memory management
  delete Str6_;
  delete Str7_;
  delete Str8_;
  delete Str9_;
  delete Str10_;

#if STRING_NOTIFY_SUCCESS
  cout <<endl <<"SUCCESS Testing ChrString" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/




