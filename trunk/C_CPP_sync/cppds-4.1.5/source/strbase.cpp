#ifndef STRBASE_CPP
#define STRBASE_CPP
#ifndef STRBASE_H
  #include "strbase.h"
#endif

Boolean StringData::_AutoUpdatePtrs = 0;
char StringData::_DummyData = 0;

/****************************************************************************/
StringData::StringData(Subscript Sz_):
data(AllocateStr(Sz_)),
_Size(Sz_),
_ShareCnt(1),
_Old(FALSE)
{}

/****************************************************************************/
StringData::StringData(char* Str_, Subscript Sz_):
data(Str_),
_Size(Sz_),
_ShareCnt(1),
_Old(FALSE)
{}

/****************************************************************************/
StringData::~StringData()
{
  DeallocateStr(data);
  data = NULL;
}

/****************************************************************************/
void StringData::DecShareCnt()
{
  if (_ShareCnt > 0)
    --_ShareCnt;
}

/****************************************************************************/
void StringData::IncShareCnt()
{
  ++_ShareCnt;
}

/****************************************************************************/
#if OVERLOAD_NEW
char* StringData::AllocateStr(Subscript Bytes_)
{
  return (char*)MemMatrix::Matrix().Allocate(sizeof(char) * (Bytes_ + 1));
}

/****************************************************************************/
char* StringData::ReallocateStr(char* Addr_, Subscript Bytes_)
{
  if (Addr_ && Addr_ != &_DummyData)
    return (char*)MemMatrix::Matrix().Reallocate(Addr_, sizeof(char) * (Bytes_ + 1));

  return (char*)MemMatrix::Matrix().Allocate(sizeof(char) * (Bytes_ + 1));
}

/****************************************************************************/
void StringData::DeallocateStr(char* Addr_)
{
  if (Addr_ != &_DummyData)
    MemMatrix::Matrix().Deallocate(Addr_);
}

/****************************************************************************/
#else
char* StringData::AllocateStr(Subscript Bytes_)
{
  return (new char[Bytes_ + 1]);
}

/****************************************************************************/
char* StringData::ReallocateStr(char* Addr_, Subscript Bytes_)
{
  if (Addr_ && Addr_ != &_DummyData)
    return ResizeString(Addr_, (Bytes_ + 1), STD_NEW);

  return (new char[Bytes_ + 1]);
}

/****************************************************************************/
void StringData::DeallocateStr(char* Addr_)
{
  if (Addr_ != &_DummyData)
    delete[] Addr_;
}
#endif
/****************************************************************************/
void StringData::SetAutoUpdate(Boolean Flag_)
{
  _AutoUpdatePtrs = Flag_;
}

/****************************************************************************/
char* StringData::DummyData()
{
  return &_DummyData;
}

/****************************************************************************/
StringData* StringData::ShareInstance()
{
  IncShareCnt();
  return this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StringData::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringData::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* StringData::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringData::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
StringImp::StringImp(StringData* Data_):
ObjectImp(),
_StrData(Data_)
{}

/****************************************************************************/
StringImp::StringImp(const StringImp& Obj_, StringData* Data_):
ObjectImp((Object&)Obj_),
_StrData(Data_)
{}

/****************************************************************************/
StringImp::~StringImp()
{
  _StrData->DecShareCnt();
  if (_StrData->_ShareCnt == 0)
    delete _StrData;
  _StrData = NULL;
}

/****************************************************************************/
const char* StringImp::GiveHashableData() const
{
  return c_str();
}

/****************************************************************************/
long StringImp::GiveHashableDataLength() const
{
  return strlen();
}

/****************************************************************************/
void StringImp::AssignLetter(StringImp& Obj_)
{
  Obj_._StrData->IncShareCnt();
  _StrData->DecShareCnt();

  if (_StrData->_ShareCnt == 0)
    delete _StrData;

  _StrData = Obj_._StrData;
}

/****************************************************************************/
void StringImp::DisOwnSharedData(StringData* DisOwnedData_)
{
  if (DisOwnedData_)
  {
    DisOwnedData_->DecShareCnt();
    DisOwnedData_->_Old = TRUE;

    if (DisOwnedData_->_ShareCnt == 0)
      delete DisOwnedData_;
  }
}

/****************************************************************************/
StringData* StringImp::ShouldDisOwnData()
{
  STRMARKER("start: StringImp::DisOwnSharedData()", cout)
  StringData* OldData_ = NULL;

  if (!_StrData->IsShareSafe())
  {
    OldData_ = _StrData;
    _StrData = _StrData->NewInstance(_StrData->data, _StrData->Length(), _StrData->_Size);
  }

  STRMARKER("end: StringImp::DisOwnSharedData()", cout)
  return OldData_;
}

/****************************************************************************/
char* StringImp::ReallocateData(Subscript Bytes_)
{
  STRMARKER("start: StringImp::ReallocateData(Subscript)", cout)

  StringData* DisOwnedData_;
  Boolean InvalidData_ = _StrData->data == StringData::DummyData() || !_StrData->data;

  if ((DisOwnedData_ = ShouldDisOwnData()) || InvalidData_)
  {
    char* OldData_ = _StrData->data;
    Subscript OldLen_ = _StrData->Length();

    _StrData->data = StringData::AllocateStr(Bytes_);

    if (!InvalidData_)
      SafeMemMove(_StrData->data, OldData_, OldLen_);

    TerminateString(OldLen_);
    DisOwnSharedData(DisOwnedData_);	// Do disowning only at end
  }
  else
    _StrData->data = StringData::ReallocateStr(_StrData->data, Bytes_);

  _StrData->_Size = Bytes_;

  STRMARKER("end: StringImp::ReallocateData(Subscript)", cout)
  return _StrData->data;
}

/****************************************************************************/
void StringImp::SetStr(const char* p, Subscript pos, Subscript len)
{
  if (!p)
    return;

  Subscript p_len = strlen();
  if (pos > p_len)			// error check the start pos
    return;

  Subscript strLen = p_len - pos;
  if (len < strLen)			// error check the copy length
    strLen = len;

  if (_StrData->_Size < strLen)		// else create a new buffer
    ReallocateData(strLen);

  SafeMemMove(_StrData->data, p + pos, strLen);
  TerminateString(strLen);
}

/****************************************************************************/
void StringImp::Assignment(const StringImp& Str_)
{
    if (_StrData->data != Str_._StrData->data)
    {
	int Identical_ = (this == &Str_);
	StringImp* StrPtr_ = Identical_ ? (StringImp*)Str_.Clone():((StringImp*)&Str_);
	Subscript NewLen_ = StrPtr_->strlen();

	if (_StrData->_Size < NewLen_)
	  ReallocateData(NewLen_);

	SafeMemMove(_StrData->data, StrPtr_->_StrData->data, NewLen_);
	TerminateString(NewLen_);

	ObjectImp::operator = (*((Object*)StrPtr_));

	if (Identical_)
	  delete StrPtr_;
    }
}

/****************************************************************************/
void StringImp::Assignment(const char* Str_, Subscript Len_)
{
    if (_StrData->data != Str_)
    {
	if (_StrData->_Size < Len_)
	  ReallocateData(Len_);

	SafeMemMove(_StrData->data, Str_, Len_);
	TerminateString(Len_);
    }
}

/****************************************************************************/
void StringImp::Assignment(char Chr_)
{
    if (_StrData->_Size < 1)
      ReallocateData(1);

    _StrData->data[0] = Chr_;
    TerminateString(1);
}

/****************************************************************************/
void StringImp::ToUpper(void)
{
  for (Subscript Index_ = 0; Index_ < strlen(); Index_++)
    (*_StrData)[Index_] = toupper((*_StrData)[Index_]);
}

/****************************************************************************/
void StringImp::ToLower(void)
{
  for (Subscript Index_ = 0; Index_ < strlen(); Index_++)
    (*_StrData)[Index_] = tolower((*_StrData)[Index_]);
}

/****************************************************************************/
void StringImp::LeftJustify(Subscript FieldWidth_, char Pad_)
{
  if (_StrData->_Size < FieldWidth_)
    ReallocateData(FieldWidth_);

  Subscript Index_;
  for (Index_ = strlen(); Index_ < FieldWidth_; Index_++)
    (*_StrData)[Index_] = Pad_;

  TerminateString(Index_);
}

/****************************************************************************/
void StringImp::RightJustify(Subscript FieldWidth_, char Pad_)
{
  if (_StrData->_Size < FieldWidth_)
  {
    StrRev();
    ReallocateData(FieldWidth_);
  }
  else
    StrRev();

  Subscript Index_;
  for (Index_ = strlen(); Index_ < FieldWidth_; Index_++)
    (*_StrData)[Index_] = Pad_;

  TerminateString(Index_);
  StrRev();
}

/****************************************************************************/
void StringImp::CenterJustify(Subscript FieldWidth_, char Pad_)
{
  Subscript PadLen_ = (FieldWidth_ - strlen()) / 2;

  if (_StrData->_Size < FieldWidth_)
    ReallocateData(FieldWidth_);

  Subscript WordLen_ = strlen();
  SafeMemMove(&_StrData->data[PadLen_], _StrData->data, WordLen_);

  Subscript Index_;
  for (Index_ = 0; Index_ < PadLen_; Index_++)
    (*_StrData)[Index_] = Pad_;

  for (Index_ += WordLen_; Index_ < FieldWidth_; Index_++)
    (*_StrData)[Index_] = Pad_;

  TerminateString(Index_);
}

/****************************************************************************/
void StringImp::Insert(Subscript pos, const StringImp& Str_)
{
  int Identical_ = (this == &Str_);
  StringImp* StrPtr_ = Identical_ ? (StringImp*)Str_.Clone():((StringImp*)&Str_);

  if (pos > strlen() || pos < 0)
    return;

  if (_StrData->_Size < StrPtr_->strlen() + strlen())
    ReallocateData(StrPtr_->strlen() + strlen());

  char* Base_ = _StrData->data + pos;
  Subscript BaseLen_ = strlen() - pos;
  Subscript StopPt_ = BaseLen_ + StrPtr_->strlen();

  SafeMemMove(&Base_[StrPtr_->strlen()], Base_, BaseLen_);
  SafeMemMove(Base_, StrPtr_->c_str(), StrPtr_->strlen());
  TerminateString(pos + StopPt_);

  if (Identical_)
    delete StrPtr_;
}

/****************************************************************************/
void StringImp::Append(const StringImp& rhs, Subscript RunLen_)
{
    if (RunLen_ > rhs.strlen())
      RunLen_ = rhs.strlen();

    int Identical_ = (this == &rhs);
    StringImp* StrPtr_ = Identical_ ? (StringImp*)rhs.Clone():((StringImp*)&rhs);
    Subscript OldLen_ = strlen();

    if (_StrData->_Size < OldLen_ + RunLen_)
      ReallocateData(OldLen_ + RunLen_);

    SafeMemMove(&_StrData->data[OldLen_], StrPtr_->_StrData->data, RunLen_);
    TerminateString(OldLen_ + RunLen_);

    if (Identical_)
      delete StrPtr_;
}

/****************************************************************************/
void StringImp::OverWrite(Subscript pos, const StringImp& Str_)
{
  int Identical_ = (this == &Str_);
  StringImp* StrPtr_ = Identical_ ? (StringImp*)Str_.Clone():((StringImp*)&Str_);

  if (pos > strlen() || pos < 0)
    return;

  if (_StrData->_Size < pos + StrPtr_->strlen())
    ReallocateData(StrPtr_->strlen() + pos);

  if (strlen() - pos < StrPtr_->strlen())
    TerminateString(pos + StrPtr_->strlen());

  SafeMemMove(&_StrData->data[pos], StrPtr_->c_str(), StrPtr_->strlen());

  if (Identical_)
    delete StrPtr_;
}

/****************************************************************************/
void StringImp::Delete(Subscript pos, Subscript len)
{
  if (pos >= strlen() || pos < 0 || len <= 0)
    return;

  Subscript RemLen_ = strlen() - pos;

  if (RemLen_ <= len)
  {
    TerminateString(pos);
    return;
  }

  Subscript Remain_ = strlen() - (pos + len);
  char* Base_ = _StrData->data + pos;
  memmove(Base_, &Base_[len], Remain_);
  TerminateString(pos + Remain_);
}

/****************************************************************************/
void StringImp::Duplicate(Subscript Num_)
{
  if (_StrData->_Size < strlen() * (Num_ + 1))
    ReallocateData(strlen() * (Num_ + 1));

  char *Startp_, *Endp_;
  Startp_ = _StrData->data;
  Endp_ = &_StrData->data[strlen()];

  for (Subscript Index_ = 0, Len_ = strlen(); Index_ < Num_; Index_++)
    for (Subscript Letter_ = 0; Letter_ < Len_; Letter_++)
      *Endp_++ = *Startp_++;

  TerminateString(Endp_ - _StrData->data);
}

/****************************************************************************/
void StringImp::MakeCharSet(int(*iscsetfn)(int))
{
  char* StrSet_ = ::MakeCharSet(iscsetfn);
  Assignment(StrSet_, SafeStrLen(StrSet_));
  EraseString(StrSet_);
}

/****************************************************************************/
void StringImp::MakeCharSet(const char* Src_, size_t RunLen_)
{
  char* StrSet_ = ::MakeCharSet(Src_, RunLen_);
  Assignment(StrSet_, SafeStrLen(StrSet_));
  EraseString(StrSet_);
}

/****************************************************************************/
Boolean StringImp::InCharSet(char ch) const
{
  return ::InCharSet(ch, c_str());
}

/****************************************************************************/
/****************************************************************************/
void StringImp::LongFloatToStr(Ldouble value, int width, int prec)
{
  char Buffer_[32];
  ::LongFloatToStr(value, Buffer_, width, prec);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
}

/****************************************************************************/
void StringImp::FloatToStr(double value, int width, int prec)
{
  char Buffer_[32];
  ::FloatToStr(value, Buffer_, width, prec);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
}

/****************************************************************************/
void StringImp::UlongToStr(unsigned long value, int radix)
{
  char Buffer_[32];
  ::UlongToStr(value, Buffer_, radix);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);    
}

/****************************************************************************/
void StringImp::LongToStr(long value, int radix)
{
  char Buffer_[32];
  ::LongToStr(value, Buffer_, radix);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);    
}

/****************************************************************************/
void StringImp::UshortToStr(unsigned short value, int radix)
{
  char Buffer_[32];
  ::UshortToStr(value, Buffer_, radix);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);    
}

/****************************************************************************/
void StringImp::ShortToStr(short value, int radix)
{
  char Buffer_[32];
  ::ShortToStr(value, Buffer_, radix);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);    
}

/****************************************************************************/
void StringImp::UintToStr(unsigned int value, int radix)
{
  char Buffer_[32];
  ::UintToStr(value, Buffer_, radix);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);    
}

/****************************************************************************/
void StringImp::IntToStr(int value, int radix)
{
  char Buffer_[32];
  ::IntToStr(value, Buffer_, radix);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);    
}

/****************************************************************************/
void StringImp::CharToStr(unsigned char ch)
{
  char Buffer_[5];
  ::CharToStr(ch, Buffer_);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);    
}

/****************************************************************************/
void StringImp::AddPosSign()
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  Buffer_ = ::AddPosSign(Buffer_);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
    
  MemMatrix::Matrix().Deallocate(Buffer_);
}

/****************************************************************************/
void StringImp::AddRadixPrefix(int radix)
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  Buffer_ = ::AddRadixPrefix(Buffer_, radix);
  Subscript len = ::strlen(Buffer_);
  
  if (len)  
    Assignment(Buffer_, len);
  else
    Assignment(0);
    
  MemMatrix::Matrix().Deallocate(Buffer_);
}

/****************************************************************************/
void StringImp::RemoveLeading(const char* chset)
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  Buffer_ = ::RemoveLeading(Buffer_, chset);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
    
  MemMatrix::Matrix().Deallocate(Buffer_);
}

/****************************************************************************/
void StringImp::RemoveTrailing(const char* chset)
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;  
  Buffer_ = ::RemoveTrailing(Buffer_, chset);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
    
  MemMatrix::Matrix().Deallocate(Buffer_);
}

/****************************************************************************/
void StringImp::RemovePadding(const char* chset)
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  Buffer_ = ::RemovePadding(Buffer_, chset);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
    
  MemMatrix::Matrix().Deallocate(Buffer_);
}

/****************************************************************************/
void StringImp::RemoveChar(const char* chset)
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  Buffer_ = ::RemoveChar(Buffer_, chset);
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
    
  MemMatrix::Matrix().Deallocate(Buffer_);
}

/****************************************************************************/
int StringImp::IsIntBin() const
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::IsIntBin(Buffer_);
  MemMatrix::Matrix().Deallocate(Buffer_);  
  return RetVal_;
}

/****************************************************************************/
int StringImp::IsIntOct() const
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::IsIntOct(Buffer_);
  MemMatrix::Matrix().Deallocate(Buffer_);
  return RetVal_;
}

/****************************************************************************/
int StringImp::IsIntDec() const
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::IsIntDec(Buffer_);
  MemMatrix::Matrix().Deallocate(Buffer_);
  return RetVal_;
}

/****************************************************************************/
int StringImp::IsIntHex() const
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::IsIntHex(Buffer_);
  MemMatrix::Matrix().Deallocate(Buffer_);
  return RetVal_;
}

/****************************************************************************/
int StringImp::BinToHex()
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::BinToHex(Buffer_) != NULL;
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
  
  MemMatrix::Matrix().Deallocate(Buffer_);  
  return RetVal_;  
}

/****************************************************************************/
int StringImp::BinToCmp2Bin(int NumBytes_)
{
  int BytesNeeded_ = BytesInCmp2Bin();
  if (BytesNeeded_ > NumBytes_)
    NumBytes_ = BytesNeeded_;
  
  Subscript OldLen_ = strlen();
  Subscript NewLen_ = 8 * NumBytes_;
  if (OldLen_ < NewLen_)
    OldLen_ = NewLen_;
  
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::BinToCmp2Bin(Buffer_, NumBytes_) != NULL;
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
  
  MemMatrix::Matrix().Deallocate(Buffer_);
  return RetVal_;
}

/****************************************************************************/
int StringImp::Cmp2BinToBin()
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::Cmp2BinToBin(Buffer_) != NULL;
  Subscript len = ::strlen(Buffer_);
  
  if (len)
    Assignment(Buffer_, len);
  else
    Assignment(0);
  
  MemMatrix::Matrix().Deallocate(Buffer_);
  return RetVal_;
}

/****************************************************************************/
int StringImp::BytesInCmp2Bin() const
{
  Subscript OldLen_ = strlen();
  char* Buffer_ = (char*)MemMatrix::Matrix().Allocate(OldLen_ + 5);
  strncpy(Buffer_, _StrData->data, OldLen_);
  Buffer_[OldLen_] = 0;
  int RetVal_ = ::BytesInCmp2Bin(Buffer_);
  MemMatrix::Matrix().Deallocate(Buffer_);
  return RetVal_;
}

/****************************************************************************/
/****************************************************************************/
void StringImp::Addition(const StringImp& rhs)
{
    int Identical_ = (this == &rhs);
    StringImp* StrPtr_ = Identical_ ? (StringImp*)rhs.Clone():((StringImp*)&rhs);
    Subscript NewLen_ = strlen() + StrPtr_->strlen();

    if (_StrData->_Size < NewLen_)
      ReallocateData(NewLen_);

    SafeMemMove(_StrData->data + strlen(), StrPtr_->_StrData->data, StrPtr_->strlen());
    TerminateString(NewLen_);

    if (Identical_)
      delete StrPtr_;
}

/****************************************************************************/
void StringImp::Subtraction(const StringImp& rhs)
{
  int Identical_ = (this == &rhs);
  StringImp* StrPtr_ = Identical_ ? (StringImp*)rhs.Clone():((StringImp*)&rhs);

  if (strlen() >= StrPtr_->strlen())
  {
    Subscript Index_ = strlen();
    Index_ -= StrPtr_->strlen();

    if (!memcmp(&_StrData->data[Index_], StrPtr_->_StrData->data, StrPtr_->strlen()))
      TerminateString(Index_);
  }

  if (Identical_)
    delete StrPtr_;
}

/****************************************************************************/
Boolean StringImp::IndexInRange(Subscript Index_, size_t ExcludeNull_) const
{
  return (0 <= Index_ && Index_ <= strlen() - ExcludeNull_);
}

/****************************************************************************/
Subscript StringImp::RestrictIndex(Subscript Index_, size_t ExcludeNull_) const
{
  return
  (
    (Index_ < 0) ? 0:
    (Index_ > strlen() - ExcludeNull_) ? (strlen() - ExcludeNull_):
    Index_
  );
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StringImp::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringImp::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* StringImp::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringImp::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif




