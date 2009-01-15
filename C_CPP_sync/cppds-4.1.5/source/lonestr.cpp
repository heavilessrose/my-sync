// Custom String Class
//
#ifndef LONESTRING_H
  #include "lonestr.h"
#endif

/****************************************************************************/
int CompLoneString(const LoneString& obj1_, const LoneString& obj2_)
{
  return
  (
    (obj1_ < obj2_) ? -1:
    (obj1_ > obj2_) ? 1:0
  );
}

/****************************************************************************/
LoneString* Clone(const LoneString& obj)
{
  return obj.Clone();
}

/****************************************************************************/
/****************************************************************************/
LoneString* LoneString::Make()
{
  return (new LoneString());
}

/****************************************************************************/
LoneString* LoneString::Make(char Ch_, Subscript n)
{
  return (new LoneString(Ch_, n));
}

/****************************************************************************/
LoneString* LoneString::Make(const char* Str_)
{
  return (new LoneString(Str_));
}

/****************************************************************************/
LoneString* LoneString::Make(const char* Str_, Subscript n)
{
  return (new LoneString(Str_, n));
}

/****************************************************************************/
LoneString* LoneString::Make(const LoneString& Obj_)
{
  return Obj_.Clone();
}

/****************************************************************************/
LoneString* LoneString::Clone() const
{
  return (new LoneString(*this));
}

/****************************************************************************/
LoneString& LoneString::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static LoneString _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
Boolean LoneString::IsNullObject() const
{
  return (_IsNull != FALSE);
}

/****************************************************************************/
const LoneString& LoneString::SetAsNullObject() const
{
  LoneString *const LocalThis_ = (LoneString *const)this;
  LocalThis_->_IsNull = TRUE;
  return *this;
}

/****************************************************************************/
LoneString& LoneString::SetAsNullObject()
{
  _IsNull = TRUE;
  return *this;
}

/****************************************************************************/
const LoneString& LoneString::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LoneString::GiveNullObject();
}

/****************************************************************************/
LoneString& LoneString::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return LoneString::GiveNullObject();
}

/****************************************************************************/
long LoneString::DataSize () const
{
  return strlen();
}

/****************************************************************************/
long LoneString::StorableFormWidth () const
{
  return (long(sizeof(long) +
               sizeof(long) +
	       sizeof(Byte_t) +
	       sizeof(Subscript)) +
	  DataSize());
}

/****************************************************************************/
int LoneString::ClassID() const
{
  return TypeInfo::LONESTRING_TYPE;
}

/****************************************************************************/
const char* LoneString::ClassName() const
{
  return "LoneString";
}

/****************************************************************************/
// ----- Saving and restoration in binary form ----
Boolean LoneString::ReadObjectData(istream& Is_, Boolean Ignore_)
{
  Boolean StreamOk_;
  Byte_t IsNull_;
  long IDvalue_;

  Is_.read((char*)(&IDvalue_), sizeof(long));

  if ((StreamOk_ = Is_.good()) && CheckClassType(IDvalue_))
  {
    Is_.read((char*)(&IsNull_), sizeof(Byte_t));
    if (IsNull_ && !Ignore_)
      SetAsNullObject();
  }
  else
  {
    if (StreamOk_)
      Is_.seekg(-long(sizeof(long)), ios::cur);

    return FALSE;
  }

  return Is_.good();
}

/****************************************************************************/
Boolean LoneString::WriteObjectData(ostream& Os_) const
{
  Byte_t IsNull_ = IsNullObject();
  long IDvalue_ = ClassID();
  Os_.write((char*)(&IDvalue_), sizeof(long));

  if (Os_.good())
    Os_.write((char*)(&IsNull_), sizeof(Byte_t));

  return Os_.good();
}

/****************************************************************************/
istream& LoneString::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  int Set_ = FALSE;
  Subscript StrLen_;
  
  Boolean Dummy_;
  if (!Ok_)
    Ok_ = &Dummy_;

  if (ReadObjectData(Is_, TRUE))
  {
    if (Is_.read((char*)(&StrLen_), sizeof(Subscript)).good())
      if (Is_.seekg(StrLen_, ios::cur).good())
      {
	*Ok_ = TRUE;
	Set_ = TRUE;
      }
  }

  if (!Set_)
    *Ok_ = FALSE;

  return Is_;
}

/****************************************************************************/
istream& LoneString::BinaryRead(istream& Is_, Boolean* Ok_)
{
  int Set_ = FALSE;
  Subscript StrLen_;

  Boolean Dummy_;
  if (!Ok_)
    Ok_ = &Dummy_;  

  if (ReadObjectData(Is_, FALSE))
  {
    if (Is_.read((char*)(&StrLen_), sizeof(Subscript)).good())
    {
      if (_Size < StrLen_)
      {
	data = ReallocateStr(data, StrLen_ + 1);
	_Size = StrLen_;
      }

      if (Is_.read(data, StrLen_).good())
      {
	data[StrLen_] = 0;
	*Ok_ = TRUE;
	Set_ = TRUE;
      }
    }
  }

  if (!Set_)
    *Ok_ = FALSE;

  return Is_;
}

/****************************************************************************/
ostream& LoneString::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  int Set_ = FALSE;

  Boolean Dummy_;
  if (!Ok_)
    Ok_ = &Dummy_;
  
  if (WriteObjectData(Os_))
  {
    Subscript StrLen_ = DataSize();
    Os_.write((char*)(&StrLen_), sizeof(Subscript));

    if (Os_.write(data, StrLen_).good())
    {
      *Ok_ = TRUE;
      Set_ = TRUE;      
    }
  }

  if (!Set_)
    *Ok_ = FALSE;

  return Os_;
}

/****************************************************************************/
istream& LoneString::TextRead(istream& Is_, Boolean* Ok_)
{
    const int bufsize = 100;
    char buf[bufsize];
    *this = LoneString();

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

    if (Ok_)
      *Ok_ = Is_.good();
      
    return Is_;
}

/****************************************************************************/
ostream& LoneString::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<c_str();

  if (Ok_)
    *Ok_ = Os_.good();
    
  return Os_;
}

/****************************************************************************/
Boolean LoneString::IsEqualString(const LoneString* Ptr_) const
{
    return strcmp(data, Ptr_->data) == 0;
}

/****************************************************************************/
Boolean LoneString::IsLesserString(const LoneString* Ptr_) const
{
    return strcmp(data, Ptr_->data) < 0;
}

/****************************************************************************/
Boolean LoneString::IsGreaterString(const LoneString* Ptr_) const
{
    return strcmp(data, Ptr_->data) > 0;
}

/****************************************************************************/
Boolean LoneString::IsEqual(const LoneString& Obj_) const
{
  return !strcmp(c_str(), Obj_.c_str());
}

/****************************************************************************/
Boolean LoneString::IsLesser(const LoneString& Obj_) const
{
  return (strcmp(c_str(), Obj_.c_str()) < 0);
}

/****************************************************************************/
Boolean LoneString::IsGreater(const LoneString& Obj_) const
{
  return (strcmp(c_str(), Obj_.c_str()) > 0);
}

/****************************************************************************/
LoneString::LoneString() :
data(AllocateStr(1)),
_Size(0)
{
    data[0] = 0;
}

/****************************************************************************/
LoneString::LoneString(const char* s) :
data(SafeStrCpy(AllocateStr(SafeStrLen(s) + 1), s) ),
_Size(SafeStrLen(s))
{}

/****************************************************************************/
LoneString::LoneString(char c, Subscript n) :
data(AllocateStr(n + 1)),
_Size(n)
{
  data[n] = '\0'; // Null terminate

  while (n)
  {
    --n;
    data[n] = c;
  }
}

/****************************************************************************/
LoneString::LoneString(const char* s, Subscript n) :
data(SafeStrnCpy(AllocateStr(n + 1), s, n)),
_Size(n)
{
    data[n] = '\0';  // Null terminate
}

/****************************************************************************/
LoneString::LoneString(const LoneString& s) :
data(SafeStrCpy(AllocateStr(s.strlen() + 1), s.data) ),
_Size(s.strlen())
{}

/****************************************************************************/
LoneString& LoneString::operator = (const LoneString& Str_)
{
    if (data != Str_.data)
    {
	int Identical_ = (this == &Str_);
	LoneString* StrPtr_ = Identical_ ? (LoneString*)Str_.Clone():((LoneString*)&Str_);

	if (_Size < StrPtr_->strlen())
	{
	  data = SafeStrCpy(ReallocateStr(data, StrPtr_->strlen() + 1), StrPtr_->data);
	  _Size = StrPtr_->strlen();
	}
	else
	  SafeStrCpy(data, StrPtr_->data);

	if (Identical_)
	  delete StrPtr_;
    }

    return *this;
}

/****************************************************************************/
LoneString& LoneString::operator = (const char* Str_)
{
    if (data != Str_)
    {
	if (_Size < SafeStrLen(Str_))
	{
	  data = SafeStrCpy(ReallocateStr(data, SafeStrLen(Str_) + 1), Str_);
	  _Size = SafeStrLen(Str_);
	}
	else
	  SafeStrCpy(data, Str_);
    }

    return *this;
}

/****************************************************************************/
LoneString& LoneString::operator = (char Chr_)
{
    if (_Size < 1)
    {
      DeallocateStr(data);
      data = AllocateStr(2);
      _Size = 1;
    }

    data[0] = Chr_;
    data[1] = '\0';
    return *this;
}

/****************************************************************************/
LoneString& LoneString::operator += (const LoneString& rhs)
{
    int Identical_ = (this == &rhs);
    LoneString* StrPtr_ = Identical_ ? (LoneString*)rhs.Clone():((LoneString*)&rhs);

    if (_Size < strlen() + StrPtr_->strlen())
    {
      _Size = strlen() + StrPtr_->strlen();
      data = SafeStrCat(ReallocateStr(data, this->strlen() + StrPtr_->strlen() + 1), StrPtr_->data);
    }
    else
      SafeStrCat(data, StrPtr_->data);

    if (Identical_)
      delete StrPtr_;

    return *this;
}

/****************************************************************************/
LoneString& LoneString::operator -= (const LoneString& rhs)
{
  int Identical_ = (this == &rhs);
  LoneString* StrPtr_ = Identical_ ? (LoneString*)rhs.Clone():((LoneString*)&rhs);

  if (strlen() >= StrPtr_->strlen())
  {
    Subscript Index_ = strlen();
    Index_ -= StrPtr_->strlen();

    if (!strcmp(&data[Index_], StrPtr_->data))
      data[Index_] = 0;
  }

  if (Identical_)
    delete StrPtr_;

  return *this;
}

/****************************************************************************/
LoneString& LoneString::Delete(const LoneString& rhs)
{
  LoneFallible<Subscript> Index_ = findlast(rhs);

  if (Index_.valid())
    Delete(Index_, rhs.strlen());

  return *this;
}

/****************************************************************************/
LoneString::LoneString(const LoneString& s1, const LoneString& s2) :
data( SafeStrCat(SafeStrCpy(AllocateStr(s1.strlen() + s2.strlen() + 1), s1.data), s2.data) ),
_Size(s1.strlen() + s2.strlen())
{}

/****************************************************************************/
LoneString operator + (const LoneString& lhs, const LoneString& rhs)
{
    return LoneString(lhs, rhs);
}

/****************************************************************************/
LoneString operator - (const LoneString& lhs, const LoneString& rhs)
{
    LoneString Temp_ = lhs;
    return (Temp_ -= rhs);
}

/****************************************************************************/
LoneString Delete(const LoneString& lhs, const LoneString& rhs)
{
  LoneString Temp_ = lhs;
  return Temp_.Delete(rhs);
}

/****************************************************************************/
LoneString& LoneString::ToUpper(void)
{
  for (Subscript Index_ = 0; Index_ < strlen(); Index_++)
    data[Index_] = toupper(data[Index_]);

  return *this;
}

/****************************************************************************/
LoneString& LoneString::ToLower(void)
{
  for (Subscript Index_ = 0; Index_ < strlen(); Index_++)
    data[Index_] = tolower(data[Index_]);

  return *this;
}

/****************************************************************************/
LoneString& LoneString::StrRev()
{
  StrReverse(data);
  return *this;
}

/****************************************************************************/
LoneString& LoneString::Right(Subscript len)
{
  SetStr(data, strlen() - len, len);
  return *this;
}

/****************************************************************************/
LoneString& LoneString::Left(Subscript len)
{
  SetStr(data, 0, len);
  return *this;
}

/****************************************************************************/
LoneString& LoneString::Mid(Subscript pos, Subscript len)
{
  SetStr(data, pos, len);
  return *this;
}

/****************************************************************************/
void LoneString::SetStr(const char* p, Subscript pos, Subscript len)
{
  if (!p)
    return;

  Subscript p_len = ::strlen(p);

  if (pos > p_len)			// error check the start pos
    return;

  Subscript strLen = p_len - pos;
  if (len < strLen)			// error check the copy length
    strLen = len;

  if (_Size >= strLen)		// if the buffer is large enough...
    memmove(data, p + pos, strLen);	// copy the subLoneString
  else					// else create a new buffer
  {
    data = ReallocateStr(data, strLen + 1);
    memmove(data, p + pos, strLen);		// copy the subLoneString
    _Size = strLen;
  }

  data[strLen] = 0;			// add NULL termination
}

/****************************************************************************/
LoneString& LoneString::LeftJustify(Subscript FieldWidth_, char Pad_)
{
  if (_Size < FieldWidth_)
  {
    data = ReallocateStr(data, FieldWidth_ + 1);
    _Size = FieldWidth_;
  }

  Subscript Index_;
  for (Index_ = strlen(); Index_ < FieldWidth_; Index_++)
    data[Index_] = Pad_;
  data[Index_] = 0;

  return *this;
}

/****************************************************************************/
LoneString& LoneString::RightJustify(Subscript FieldWidth_, char Pad_)
{
  if (_Size < FieldWidth_)
  {
    StrRev();

    data = ReallocateStr(data, FieldWidth_ + 1);
    _Size = FieldWidth_;
  }
  else
    StrRev();

  Subscript Index_;
  for (Index_ = strlen(); Index_ < FieldWidth_; Index_++)
    data[Index_] = Pad_;
  data[Index_] = 0;

  return StrRev();
}

/****************************************************************************/
LoneString& LoneString::CenterJustify(Subscript FieldWidth_, char Pad_)
{
  Subscript PadLen_ = (FieldWidth_ - strlen()) / 2;

  if (_Size < FieldWidth_)
  {
    data = ReallocateStr(data, FieldWidth_ + 1);
    _Size = FieldWidth_;
  }

  Subscript WordLen_ = strlen();
  memmove(&data[PadLen_], data, WordLen_);
  (data + PadLen_)[WordLen_] = 0;

  Subscript Index_;
  for (Index_ = 0; Index_ < PadLen_; Index_++)
    data[Index_] = Pad_;

  for (Index_ = strlen(); Index_ < FieldWidth_; Index_++)
    data[Index_] = Pad_;
  data[Index_] = 0;

  return *this;
}

/****************************************************************************/
LoneString& LoneString::Insert(Subscript pos, const LoneString& Str_)
{
  int Identical_ = (this == &Str_);
  LoneString* StrPtr_ = Identical_ ? (LoneString*)Str_.Clone():((LoneString*)&Str_);

  if (pos > strlen() || pos < 0)
    return *this;

  if (_Size < StrPtr_->strlen() + strlen())
  {
    _Size = StrPtr_->strlen() + strlen();
    data = ReallocateStr(data, _Size + 1);
  }

  char* Base_ = data + pos;
  Subscript StopPt_ = ::strlen(Base_) + StrPtr_->strlen();

  memmove(&Base_[StrPtr_->strlen()], Base_, ::strlen(Base_));
  memmove(Base_, StrPtr_->c_str(), StrPtr_->strlen());
  Base_[StopPt_] = 0;

  if (Identical_)
    delete StrPtr_;

  return *this;
}

/****************************************************************************/
LoneString& LoneString::Append(const LoneString& rhs, Subscript RunLen_)
{
    if (RunLen_ > rhs.strlen())
      RunLen_ = rhs.strlen();

    int Identical_ = (this == &rhs);
    LoneString* StrPtr_ = Identical_ ? (LoneString*)rhs.Clone():((LoneString*)&rhs);

    if (_Size < strlen() + RunLen_)
    {
      Subscript OldLen_ = strlen();
      _Size = OldLen_ + RunLen_;
      data = ReallocateStr(data, _Size + 1);
      SafeStrnCpy(&data[OldLen_], StrPtr_->data, RunLen_);
      data[OldLen_ + RunLen_] = 0;
    }
    else
    {
      Subscript OldLen_ = strlen();
      SafeStrnCpy(&data[OldLen_], StrPtr_->data, RunLen_);
      data[OldLen_ + RunLen_] = 0;
    }

    if (Identical_)
      delete StrPtr_;

    return *this;
}

/****************************************************************************/
LoneString& LoneString::OverWrite(Subscript pos, const LoneString& Str_)
{
  int Identical_ = (this == &Str_);
  LoneString* StrPtr_ = Identical_ ? (LoneString*)Str_.Clone():((LoneString*)&Str_);

  if (pos > strlen() || pos < 0)
    return *this;

  if (_Size < pos + StrPtr_->strlen())
  {
    _Size = StrPtr_->strlen() + pos;
    data = ReallocateStr(data, _Size + 1);
  }

  if (::strlen(&data[pos]) < StrPtr_->strlen())
    data[pos + StrPtr_->strlen()] = 0;

  memmove(&data[pos], StrPtr_->c_str(), StrPtr_->strlen());

  if (Identical_)
    delete StrPtr_;

  return *this;
}

/****************************************************************************/
LoneString& LoneString::Delete(Subscript pos, Subscript len)
{
  if (pos >= strlen() || pos < 0 || len <= 0)
    return *this;

  Subscript RemLen_ = ::strlen(&data[pos]);
  if (RemLen_ <= len)
  {
    data[pos] = 0;
    return *this;
  }

  if (!len)
    return *this;

  char* Base_ = data + pos;
  memmove(Base_, &Base_[len], ::strlen(&Base_[len]) + 1);

  return *this;
}

/****************************************************************************/
Subscript LoneString::Replace(Subscript pos,
			  const LoneString& from,
			  const LoneString& to,
			  Subscript count)
{
  if (from == to)
    return 0;

  int IdenticalFrom_ = (this == &from);
  LoneString* FromPtr_ = IdenticalFrom_ ? (LoneString*)from.Clone():((LoneString*)&from);

  int IdenticalTo_ = (this == &to);
  LoneString* ToPtr_ = IdenticalTo_ ? (LoneString*)to.Clone():((LoneString*)&to);

  LoneString NextStr_ = &data[pos];
  LoneFallible<Subscript> Result_ = NextStr_.find(*FromPtr_);
  if (Result_.failed())
    return 0;

  Subscript Start_ = Result_;
  Subscript LenTo_ = ToPtr_->strlen();
  Subscript LenFrom_ = FromPtr_->strlen();

  if (LenFrom_ < LenTo_)
  {
    Subscript Extra_ = ::strlen(&data[Start_]) / LenFrom_;
    Extra_ *= (LenTo_ - LenFrom_);

    _Size += Extra_;
    data = ReallocateStr(data, _Size + 1);
  }

  Subscript NumReplaced_;
  for (NumReplaced_ = 0;;)
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
LoneString& LoneString::Duplicate(Subscript Num_)
{
  if (_Size < strlen() * (Num_ + 1))
  {
    _Size = strlen() * (Num_ + 1);
    data = ReallocateStr(data, _Size + 1);
  }

  char *Startp_, *Endp_;
  Startp_ = data;
  Endp_ = &data[strlen()];

  for (Subscript Index_ = 0, Len_ = strlen(); Index_ < Num_; Index_++)
    for (Subscript Letter_ = 0; Letter_ < Len_; Letter_++)
      *Endp_++ = *Startp_++;
  *Endp_ = 0;

  return *this;
}

/****************************************************************************/
/****************************************************************************/
LoneString::~LoneString()
{
  DeallocateStr(data);
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::find(char find_me) const
{
    const char* foundp = strchr(data, find_me);
    if (foundp) return LoneFallible<Subscript>(foundp - data, NULL);
    else        return LoneFallible<Subscript>();
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::findlast(char find_me) const
{
    const char* foundp = strrchr(data, find_me);
    if (foundp) return LoneFallible<Subscript>(foundp - data, NULL);
    else        return LoneFallible<Subscript>();
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::find(const LoneString& find_me) const
{
    const char*foundp = strstr(data, find_me.data);
    if (foundp)return LoneFallible<Subscript>(foundp - data, NULL);
    else       return LoneFallible<Subscript>();
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::findlast(const LoneString& s)   const
{
    LoneString NewStr_ = *this;
    LoneString find_me = s;

    NewStr_.StrRev();
    find_me.StrRev();

    char* NewData_ = NewStr_.c_str();
    const char* foundp = strstr(NewData_, find_me.data);

    if (foundp)
      return LoneFallible<Subscript>(NewStr_.strlen() - s.strlen() - (foundp - NewData_), NULL);
    else
      return LoneFallible<Subscript>();
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::findnext(Subscript pos, char c)   const
{
  Subscript NextPos_ = (data[pos] == c) ? (pos + 1):pos;
  if (NextPos_ >= strlen())
    return LoneFallible<Subscript>();

  LoneString NextStr_ = &data[NextPos_];
  LoneFallible<Subscript> Result_ = NextStr_.find(c);

  if (Result_.valid())
    return LoneFallible<Subscript>(Result_ + NextPos_, NULL);

  return Result_;
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::findnext(Subscript pos, const LoneString& s)   const
{
  Subscript NextPos_ = !strncmp(&data[pos], s.c_str(), s.strlen()) ? (pos + s.strlen()):pos;
  if (NextPos_ >= strlen())
    return LoneFallible<Subscript>();

  LoneString NextStr_ = &data[NextPos_];
  LoneFallible<Subscript> Result_ = NextStr_.find(s);

  if (Result_.valid())
    return LoneFallible<Subscript>(Result_ + NextPos_, NULL);

  return Result_;
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::findprevious(Subscript pos, char c)   const
{
  if (pos < 1)
    return LoneFallible<Subscript>();

  Subscript Len_ = (data[pos] == c) ? pos:(pos + 1);
  if (Len_ >= strlen())
    return LoneFallible<Subscript>();

  LoneString NextStr_ = subString(0, Len_);
  return NextStr_.findlast(c);
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::findprevious(Subscript pos, const LoneString& s)   const
{
  if (pos < s.strlen())
    return LoneFallible<Subscript>();

  Subscript Len_ = !strncmp(&data[pos], s.c_str(), s.strlen()) ? pos:(pos + 1);
  if (Len_ >= strlen())
    return LoneFallible<Subscript>();

  LoneString NextStr_ = subString(0, Len_);
  return NextStr_.findlast(s);
}

/****************************************************************************/
Boolean LoneString::IndexInRange(Subscript Index_) const
{
  return (0 <= Index_ && Index_ <= strlen());
}

/****************************************************************************/
Subscript LoneString::RestrictIndex(Subscript Index_) const
{
  return
  (
    (Index_ < 0) ? 0:
    (Index_ > strlen()) ? strlen():
    Index_
  );
}

/****************************************************************************/
LoneFallible<char> LoneString::operator [] (Subscript i) const
{
  return
  (
    IndexInRange(i) ?
	LoneFallible<char>(data[i], NULL):
	LoneFallible<char>()
  );
}

/****************************************************************************/
LoneFallible<char> LoneString::operator [] (Subscript i)
{
  return
  (
    IndexInRange(i) ?
	LoneFallible<char>(data[i], &data[i]):
	LoneFallible<char>()
  );
}

/****************************************************************************/
const char* LoneString::operator () (Subscript i) const
{
  return (IndexInRange(i) ? (data + i):NULL);
}

/****************************************************************************/
char* LoneString::operator () (Subscript i)
{
  return (IndexInRange(i) ? (data + i):NULL);
}

/****************************************************************************/
char LoneString::operator * () const
{
  return (*this)[0];
}

/****************************************************************************/
char& LoneString::operator * ()
{
#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(__DJGPP__)
  return (*this)[0];
#else
  char& ChRef_ = (char&)((*this)[0]);
  return ChRef_;
#endif
}

/****************************************************************************/
LoneFallible<Subscript> LoneString::brk(const LoneString& any_of_me) const
{
    const char*foundp = strpbrk(data, any_of_me.data);
    if (foundp)return LoneFallible<Subscript>(foundp - data, NULL);
    else       return LoneFallible<Subscript>();
}

/****************************************************************************/
Subscript LoneString::span(const LoneString& consisting_of_me) const
{
    return strspn(data, consisting_of_me.data);
}

/****************************************************************************/
Subscript LoneString::cspan(const LoneString& consisting_not_of_me) const
{
    return strcspn(data, consisting_not_of_me.data);
}

/****************************************************************************/
LoneString LoneString::subString(Subscript start) const
{
  Subscript strt =  (start <= strlen()) ? start : strlen();  // null if too far from beginning
  return LoneString(data + strt);
}

/****************************************************************************/
LoneString LoneString::subString(Subscript start, Subscript run_len) const
{
  Subscript strt =  (start < strlen()) ? start : strlen();  // null if too far from beginning
  Subscript rn = ( start + run_len <= strlen() ) ? run_len : strlen();
  return LoneString(data + strt, rn  );
}

/****************************************************************************/
LoneString& operator << (LoneString& s, const char* p)
{
  // Write a C LoneString into a LoneString.  This is a specialization
  // of the operator<<(LoneString, const T&) template function that bypasses
  // using ostrstream.
  return s += LoneString(p);
}

/****************************************************************************/
LoneFallible<double> LoneString::strtod() const
{
    errno = 0; // Clear error indication
    char* endp;
    double result = ::strtod(data, &endp);

    if (errno || *endp) return LoneFallible<double>();
    else                return LoneFallible<double>(result, NULL);
}

/****************************************************************************/
LoneFallible<long> LoneString::strtol() const
{
  errno = 0;  // Clear error indication
  char* endp;
  long result = ::strtol(data, &endp, 0);
  if (errno || *endp) return LoneFallible<long>();
  else             return LoneFallible<long>(result, NULL);
}

/****************************************************************************/
LoneFallible<unsigned long> LoneString::strtoul() const
{
  errno = 0;  // Clear error indication
  char* endp;
  unsigned long result = ::strtoul(data, &endp, 0);
  if (errno || *endp) return LoneFallible<unsigned long>();
  else             return LoneFallible<unsigned long>(result, NULL);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* LoneString::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void LoneString::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* LoneString::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void LoneString::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if LONESTRING_DEBUG
#if defined(__TURBOC__)
  #include <conio.h>
#endif
int main()
{
#if defined(__TURBOC__)
  clrscr();
#endif

  LoneString Str1_;
  LoneString Str2_("Hello World");
  LoneString Str3_("GoodBye World", 8);
  LoneString Str4_('x', 5);
  LoneString Str5_(Str3_);

  LoneString* Str6_ = LoneString::Make();
  LoneString* Str7_ = LoneString::Make("Hello World");
  LoneString* Str8_ = LoneString::Make("GoodBye World", 8);
  LoneString* Str9_ = LoneString::Make('x', 5);
  LoneString* Str10_ = LoneString::Make(*Str8_);

  Str1_ = Str2_;
  Str2_.ToUpper();
  Str7_->ToLower();
  Str3_.RightJustify(20);
  Str8_->Delete(0, 4);
  Str8_->LeftJustify(20);
  Str4_.CenterJustify(20);

  Str1_.Duplicate(3);
  *Str6_ = *Str7_;
  Str6_->Insert(3, "Inserted");

  cout  <<Str1_ <<endl
	<<Str2_ <<endl
	<<Str3_ <<endl
	<<Str4_ <<endl
	<<Str5_ <<endl
	<<*Str6_ <<endl
	<<*Str7_ <<endl
	<<*Str8_ <<endl
	<<*Str9_ <<endl
	<<*Str10_ <<endl;

  assert(WhatIs(Str1_) == TypeInfo::LONESTRING_TYPE);
  assert(Str1_ == "Hello WorldHello WorldHello WorldHello World");
  assert(Str2_ == "HELLO WORLD");
  assert(Str3_ == "            GoodBye ");
  assert(Str4_ == "       xxxxx        ");
  assert(Str5_ == "GoodBye ");
  assert(*Str6_ == "helInsertedlo world");
  assert(*Str7_ == "hello world");
  assert(*Str8_ == "Bye                 ");
  assert(*Str9_ == "xxxxx");
  assert(*Str10_ == "GoodBye ");

  char Buffer_[20];
  StringToken Tok_(Str1_.c_str(), 0, " ", " ");
  Tok_.FindToken(Buffer_);
  cout <<Buffer_ <<"-";
  assert(!strcmp(Buffer_, "Hello"));
  for (int Index_ = 0; Index_ < 3; Index_++)
  {
    Tok_.FindToken(Buffer_);
    assert(!strcmp(Buffer_, "WorldHello"));
    cout <<Buffer_ <<"-";
  }
  Tok_.FindToken(Buffer_);
  assert(!strcmp(Buffer_, "World"));
  cout <<Buffer_ <<endl;

  Str2_.Delete(4, 2);
  Str7_->OverWrite(0, "Bye  ");
  Str1_.Replace(0, "ll", "nn");

  cout  <<Str2_ <<endl
	<<*Str7_ <<endl
	<<Str1_ <<endl;

  assert(Str2_ == "HELLWORLD");
  assert(*Str7_ == "Bye   world");
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHenno World");

  LoneFallible<Subscript> Fall1_ = Str1_.find("Bye");
  LoneFallible<Subscript> Fall2_ = Str1_.findlast("Good");
  Subscript Pos3_ = Str1_.find("World");
  Subscript Pos4_ = Str1_.findlast("Henno");
  Subscript Pos5_ = Str1_.findnext(Pos3_, "World");
  Subscript Pos6_ = Str1_.findprevious(Pos4_, "Henno");

  LoneString Str11_ = Str3_.Right(5);
  LoneString Str12_ = Str8_->Left(5);
  LoneString Str13_ = Str4_.Mid(10, 10);

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

  assert((int)Fall1_.valid() == 0);
  assert((int)Fall2_.valid() == 0);
  assert(Pos3_ == 6);
  assert(Pos4_ == 33);
  assert(Pos5_ == 17);
  assert(Pos6_ == 22);
  assert(Str11_ == "dBye ");
  assert(Str12_ == "Bye  ");
  assert(Str13_ == "xx        ");

  Str1_ -= "no World";
  cout <<Str1_ <<endl;
  assert(Str1_ == "Henno WorldHenno WorldHenno WorldHen");

  ofstream fout("test.txt", ios::out|ios::binary);
  Str1_.BinaryWrite(fout);
  fout.close();

  ifstream fin("test.txt", ios::in|ios::binary);
  Str11_.BinaryRead(fin);
  fin.close();

  cout <<Str11_ <<endl;
  assert(Str11_ == "Henno WorldHenno WorldHenno WorldHen");

  LoneString Str14_ = "JoKCsephKCWong";
  Str14_.Delete("KC");
  cout <<Str14_ <<endl;
  assert(Str14_ == "JoKCsephWong");
  LoneString Str15_ = Delete(Str14_, "KC");
  cout <<Str15_ <<endl;
  assert(Str15_ == "JosephWong");

  Str15_ += " This is a test of appending";
  cout <<Str15_.c_str() <<endl;
  assert(Str15_ == "JosephWong This is a test of appending");

  Pos3_ = Str15_.find('s');
  Pos4_ = Str15_.findlast('s');
  Pos5_ = Str15_.findnext(Pos3_, 's');
  Pos6_ = Str15_.findprevious(Pos4_, 's');
  assert(Pos3_ == 2);
  assert(Pos4_ == 23);
  assert(Pos5_ == 14);
  assert(Pos6_ == 17);

  Str15_ -= " This is a test of appending";
  assert(Str15_ == "JosephWong");
  assert((Str15_ + " This is a test of appending") == "JosephWong This is a test of appending");
  Str15_.Append(Str2_, 4);
  assert(Str15_ == "JosephWongHELL");

  LoneFallible<Subscript> Fall3_ = Str15_.brk("eph");
  assert(Fall3_.valid());
  Pos3_ = Fall3_;
  assert(Pos3_ == 3);
  Pos4_ = Str15_.span("toseJy");
  Pos5_ = Str15_.cspan("Wpht");

  assert(Pos4_ == 4);
  assert(Pos5_ == 4);

  delete Str6_;
  delete Str7_;
  delete Str8_;
  delete Str9_;
  delete Str10_;

  cout <<endl <<"SUCCESS Testing LoneString" <<endl;
  return 0;
}
#endif
/****************************************************************************/




