#ifndef SLOTTEDFILE_CPP
#define SLOTTEDFILE_CPP
#ifndef SLOTTEDFILE_H
  #include "slotfile.h"
#endif

/****************************************************************************/
SlottedFileAcceptor::SlottedFileAcceptor(const SlottedFile* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
void SlottedFileAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const SlottedFile*)Ptr_;
}

/****************************************************************************/
const void* SlottedFileAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
Boolean SlottedFileAcceptor::IsFile() const
{
  return TRUE;
}

/****************************************************************************/
Boolean SlottedFileAcceptor::IsSlottedFile() const
{
  return TRUE;
}

/****************************************************************************/
void* SlottedFileAcceptor::CreateFromSlottedFile(const ObjectAcceptor& Obj_)
{
  _VarPtr = new SlottedFile(*((const SlottedFile*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* SlottedFileAcceptor::AssignFromSlottedFile(const ObjectAcceptor& Obj_)
{
  const SlottedFile* TrgPtr_ = (const SlottedFile*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* SlottedFileAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void SlottedFileAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
SlottedFileBrowser::SlottedFileBrowser(ByteStream* Stream_):
StreamBrowser(Stream_)
{}

/****************************************************************************/
ByteStream& SlottedFileBrowser::SeekGet(long RecNum_)
{
  if (_Browser.GetMode() & ios::in)
    _Stream->iseekg(_Browser.StorableFormWidth() * RecNum_);
  return *_Stream;
}

/****************************************************************************/
ByteStream& SlottedFileBrowser::SeekPut(long RecNum_)
{
  _Stream->oseekp(_Browser.StorableFormWidth() * RecNum_);
  return *_Stream;
}

/****************************************************************************/
ByteStream& SlottedFileBrowser::SeekGet(long Incr_, int SeekDir_)
{
  if (_Browser.GetMode() & ios::in)
    _Stream->iseekg(_Browser.StorableFormWidth() * Incr_, SeekDir_);
  return *_Stream;
}

/****************************************************************************/
ByteStream& SlottedFileBrowser::SeekPut(long Incr_, int SeekDir_)
{
  _Stream->oseekp(_Browser.StorableFormWidth() * Incr_, SeekDir_);
  return *_Stream;
}

/****************************************************************************/
long SlottedFileBrowser::TellGet()
{
  if (_Browser.GetMode() & ios::in)
    return (_Stream->itellg() / _Browser.StorableFormWidth());
  return 0;
}

/****************************************************************************/
long SlottedFileBrowser::TellPut()
{
  return (_Stream->otellp() / _Browser.StorableFormWidth());
}

/****************************************************************************/
ByteStream& SlottedFileBrowser::ReadRecord()
{
  if (_Browser.GetMode() & ios::in)
    return _Browser.Read(*_Stream);
  return *_Stream;
}

/****************************************************************************/
ByteStream& SlottedFileBrowser::PeekRecord()
{
  if (_Browser.GetMode() & ios::in)
  {
    _Browser.Read(*_Stream);
    if (_Stream->Verified())
      SeekGet(-1, ios::cur);
  }

  return *_Stream;
}

/****************************************************************************/
ByteStream& SlottedFileBrowser::WriteRecord()
{
  return _Browser.Write(*_Stream);
}

/****************************************************************************/
ByteStream& SlottedFileBrowser::AppendRecord()
{
  SeekPut(0, ios::end);
  return WriteRecord();
}

/****************************************************************************/
Boolean SlottedFileBrowser::Verified() const
{
  return _Browser.Verified();
}

/****************************************************************************/
long SlottedFileBrowser::SizePut()
{
  long StartPos_, CurPos_, Diff_;

  CurPos_ = TellPut();
  SeekPut(0);
  StartPos_ = TellPut();
  SeekPut(0, ios::end);
  Diff_ = TellPut() - StartPos_;
  SeekPut(CurPos_);

  return Diff_;
}

/****************************************************************************/
long SlottedFileBrowser::SizeGet()
{
  if (_Browser.GetMode() & ios::in)
  {
    long StartPos_, CurPos_, Diff_;

    CurPos_ = TellGet();
    SeekGet(0);
    StartPos_ = TellGet();
    SeekGet(0, ios::end);
    Diff_ = TellGet() - StartPos_;
    SeekGet(CurPos_);

    return Diff_;
  }

  return 0;
}

/****************************************************************************/
ByteStream* SlottedFileBrowser::MakeByteStreamBrowser()
{
  return (new SlottedFileBrowser(_Stream));
}

/****************************************************************************/
void SlottedFileBrowser::SetByteStreamBrowser(ByteStream* Browser_)
{
  _Stream->SetByteStreamBrowser(Browser_);
}

/****************************************************************************/
ByteStream* SlottedFileBrowser::GetByteStreamBrowser() const
{
  return _Stream->GetByteStreamBrowser();
}

/****************************************************************************/
ByteStreamIterator* SlottedFileBrowser::NewByteStreamIterator()
{
  return (new ByteStreamIterator(_Stream, CurrentIOMode()));
}

/****************************************************************************/
/****************************************************************************/
SlottedFileInfo::SlottedFileInfo(const SlottedFileInfo& Finfo_):
_SlotSize(Finfo_._SlotSize),
_ObjectRecord(Finfo_._ObjectRecord ? Finfo_._ObjectRecord->Clone():NULL),
_SlotsAllocated(Finfo_._SlotsAllocated),
_HeaderSize(Finfo_._HeaderSize)
{}

/****************************************************************************/
SlottedFileInfo::SlottedFileInfo(const Object& Record_,
				 long Allocated_, long HeaderSize_):
_SlotSize(Record_.StorableFormWidth()),
_ObjectRecord(Record_.NULLOBJECT().Clone()),
_SlotsAllocated(Allocated_),
_HeaderSize(HeaderSize_)
{}

/****************************************************************************/
SlottedFileInfo::~SlottedFileInfo()
{
  delete _ObjectRecord;
  _ObjectRecord = NULL;
}

/****************************************************************************/
SlottedFileInfo* SlottedFileInfo::Make(const SlottedFileInfo& Finfo_)
{
  return (new SlottedFileInfo(Finfo_));
}

/****************************************************************************/
SlottedFileInfo* SlottedFileInfo::Make(const Object& Record_,
				       long Allocated_, long HeaderSize_)
{
  return (new SlottedFileInfo(Record_, Allocated_, HeaderSize_));
}

/****************************************************************************/
SlottedFileInfo& SlottedFileInfo::operator = (const SlottedFileInfo& Finfo_)
{
  if (this != &Finfo_)
  {
    _SlotSize = Finfo_._SlotSize;
    delete _ObjectRecord;    
    _ObjectRecord = (Finfo_._ObjectRecord ? Finfo_._ObjectRecord->Clone():NULL);
    _SlotsAllocated = Finfo_._SlotsAllocated;
    _HeaderSize = Finfo_._HeaderSize;
  }
  
  return *this;
}

/****************************************************************************/
SlottedFileInfo* SlottedFileInfo::AllocateSlots(long NewSlots_)
{
  _SlotsAllocated += NewSlots_;
  return this;
}

/****************************************************************************/
SlottedFileInfo* SlottedFileInfo::SetHeaderSize(long Size_)
{
  _HeaderSize = Size_;
  return this;
}

/****************************************************************************/
istream& SlottedFileInfo::ReadFileInfo(istream& Is_)
{
  if (_ObjectRecord)
  {
    Boolean Ok_;

    Is_.read((char*)(&_SlotSize), sizeof(long));
    _ObjectRecord->BinaryRead(Is_, &Ok_);

    if (Ok_)
    {
      Is_.read((char*)(&_SlotsAllocated), sizeof(long));
      Is_.read((char*)(&_HeaderSize), sizeof(long));
    }
  }

  return Is_;
}

/****************************************************************************/
ostream& SlottedFileInfo::WriteFileInfo(ostream& Os_) const
{
  if (_ObjectRecord)
  {
    Boolean Ok_;

    Os_.write((char*)(&_SlotSize), sizeof(long));
    _ObjectRecord->BinaryWrite(Os_, &Ok_);

    if (Ok_)
    {
      Os_.write((char*)(&_SlotsAllocated), sizeof(long));
      Os_.write((char*)(&_HeaderSize), sizeof(long));
    }
  }

  return Os_;
}

/****************************************************************************/
Boolean SlottedFileInfo::operator == (SlottedFileInfo& Info_)
{
  if (Info_._ObjectRecord && _ObjectRecord)
    return
    (
      Info_._SlotSize == _SlotSize &&
      Info_._ObjectRecord->IsNullObject() &&
      _ObjectRecord->IsNullObject() &&
      Info_._SlotsAllocated == _SlotsAllocated &&
      Info_._HeaderSize == _HeaderSize
    );

  return
  (
    Info_._SlotSize == _SlotSize &&
    Info_._SlotsAllocated == _SlotsAllocated &&
    Info_._HeaderSize == _HeaderSize
  );
}

/****************************************************************************/
#if OVERLOAD_NEW
void* SlottedFileInfo::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void SlottedFileInfo::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* SlottedFileInfo::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void SlottedFileInfo::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
SlottedFile::SlottedFile(SlottedFileInfo* Info_):
_Info(Info_),
_Status(SLOTTEDFILE_NOTREADY),
_StoreToHeader(FALSE)
{
  MakeByteStreamBrowser();
}

/****************************************************************************/
SlottedFile::SlottedFile(const File& Finfo_, SlottedFileInfo* Info_):
File(Finfo_),
_Info(Info_),
_Status(SLOTTEDFILE_NOTREADY),
_StoreToHeader(FALSE)
{
  MakeByteStreamBrowser();
}

/****************************************************************************/
SlottedFile::SlottedFile(const SlottedFile& Finfo_):
File(Finfo_),
_Info(Finfo_._Info ? SlottedFileInfo::Make(*Finfo_._Info):NULL),
_Status(SLOTTEDFILE_NOTREADY),
_StoreToHeader(Finfo_._StoreToHeader)
{
  MakeByteStreamBrowser();
}

/****************************************************************************/
SlottedFile::~SlottedFile()
{
  delete _Info;
  _Info = NULL;
}

/****************************************************************************/
/****************************************************************************/
ByteStream* SlottedFile::MakeByteStreamBrowser()
{
  if (ClassID() == StaticID())
    GetReader()->SetByteStreamBrowser(new SlottedFileBrowser((ByteStream*)this));
  else
    GetReader()->SetByteStreamBrowser(NULL);

  return GetReader()->GetByteStreamBrowser();
}

/****************************************************************************/
void SlottedFile::SetByteStreamBrowser(ByteStream* Browser_)
{
  if (Browser_)
    GetReader()->SetByteStreamBrowser(Browser_);
  else
    GetReader()->SetByteStreamBrowser(NULL);
}

/****************************************************************************/
ByteStream* SlottedFile::GetByteStreamBrowser() const
{
  return GetReader()->GetByteStreamBrowser();
}

/****************************************************************************/
istream& SlottedFile::iseekg(long Pos_)
{
  SLFMARKER("start: SlottedFile::iseekg(long)")

  if (Pos_ < 0)
    Pos_ = 0;

  SLFTRACE(RecordSection())
  SLFTRACE(Pos_)

  istream& Strm_ = GetByteStreamBrowser()->iseekg(RecordSection() + Pos_);
  IstreamCheck(Strm_);

  SLFMARKER("end: SlottedFile::iseekg(long)")
  return Strm_;
}

/****************************************************************************/
istream& SlottedFile::iseekg(long Pos_, int Dir_)
{
  SLFMARKER("start: SlottedFile::iseekg(long, ios::seek_dir)")

  if (Dir_ == ios::beg)
    return iseekg(Pos_);

  if (Pos_ < 0)
  {
    if (Dir_ == ios::end)
      iseekg(0, ios::end);

    long AbsCurPos_ = itellg();
    SLFTRACE(AbsCurPos_)

    if (Pos_ + AbsCurPos_ < 0L)
      Pos_ = -AbsCurPos_;
  }
  else if (Pos_ > 0 && Dir_ == ios::end)
    Pos_ = 0;

  SLFTRACE(Pos_)
  SLFTRACE(itellg())

  istream& Strm_ = GetByteStreamBrowser()->iseekg(Pos_, Dir_);
  IstreamCheck(Strm_);

  SLFMARKER("end: SlottedFile::iseekg(long, ios::seek_dir)")
  return Strm_;
}

/****************************************************************************/
long SlottedFile::itellg()
{
  long Pos_ = GetByteStreamBrowser()->itellg() - RecordSection();
  IstreamCheck(GetInStream(FALSE));
  return Pos_;
}

/****************************************************************************/
ostream& SlottedFile::oseekp(long Pos_)
{
  SLFMARKER("start: SlottedFile::oseekp(long)")

  SLFTRACE(RecordSection())
  SLFTRACE(Pos_)

  if (Pos_ < 0)
    Pos_ = 0;

  ostream& Strm_ = GetByteStreamBrowser()->oseekp(RecordSection() + Pos_);
  OstreamCheck(Strm_);

  SLFMARKER("end: SlottedFile::oseekp(long)")
  return Strm_;
}

/****************************************************************************/
ostream& SlottedFile::oseekp(long Pos_, int Dir_)
{
  SLFMARKER("start: SlottedFile::oseekp(long, ios::seek_dir)")

  if (Dir_ == ios::beg)
    return oseekp(Pos_);

  if (Pos_ < 0)
  {
    if (Dir_ == ios::end)
      oseekp(0, ios::end);

    long AbsCurPos_ = otellp();
    SLFTRACE(AbsCurPos_)

    if (Pos_ + AbsCurPos_ < 0L)
      Pos_ = -AbsCurPos_;
  }
  else if (Pos_ > 0 && Dir_ == ios::end)
    Pos_ = 0;

  SLFTRACE(Pos_)
  SLFTRACE(otellp())

  ostream& Strm_ = GetByteStreamBrowser()->oseekp(Pos_, Dir_);
  OstreamCheck(Strm_);

  SLFMARKER("end: SlottedFile::oseekp(long, ios::seek_dir)")
  return Strm_;
}

/****************************************************************************/
long SlottedFile::otellp()
{
  long Pos_ = GetByteStreamBrowser()->otellp() - RecordSection();
  OstreamCheck(GetOutStream(FALSE));
  return Pos_;
}

/****************************************************************************/
ByteStream& SlottedFile::SeekGet(long RecNum_)
{
  if (_Status == SLOTTEDFILE_READY &&
      GetByteStreamBrowser() && IsValidSlot(RecNum_))
    return GetByteStreamBrowser()->SeekGet(RecNum_);

  return *this;
}

/****************************************************************************/
ByteStream& SlottedFile::SeekPut(long RecNum_)
{
  if (_Status == SLOTTEDFILE_READY &&
      GetByteStreamBrowser() && IsValidSlot(RecNum_))
    return GetByteStreamBrowser()->SeekPut(RecNum_);

  return *this;
}

/****************************************************************************/
ByteStream& SlottedFile::SeekGet(long RecNum_, int SeekDir_)
{
  if (_Status == SLOTTEDFILE_READY &&
      GetByteStreamBrowser() && IsValidSeek(RecNum_, SeekDir_, ios::in))
    return GetByteStreamBrowser()->SeekGet(RecNum_, SeekDir_);

  return *this;
}

/****************************************************************************/
ByteStream& SlottedFile::SeekPut(long RecNum_, int SeekDir_)
{
  if (_Status == SLOTTEDFILE_READY &&
      GetByteStreamBrowser() && IsValidSeek(RecNum_, SeekDir_, ios::out))
    return GetByteStreamBrowser()->SeekPut(RecNum_, SeekDir_);

  return *this;
}

/****************************************************************************/
ByteStreamIterator* SlottedFile::NewByteStreamIterator()
{
  return (new ByteStreamIterator(this, CurrentIOMode()));
}

/****************************************************************************/
/****************************************************************************/
SlottedFile* SlottedFile::Make(SlottedFileInfo* Info_)
{
  return (new SlottedFile(Info_));
}

/****************************************************************************/
SlottedFile* SlottedFile::Make(const File& Finfo_, SlottedFileInfo* Info_)
{
  return (new SlottedFile(Finfo_, Info_));
}

/****************************************************************************/
SlottedFile* SlottedFile::Make(const Object& Obj_)
{
  SlottedFileAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& SlottedFile::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  SlottedFileAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromSlottedFile(TrgComp_)));
}

/****************************************************************************/
Object* SlottedFile::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new SlottedFile(*this));

  SlottedFileAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromSlottedFile(TrgComp_)));
}

/****************************************************************************/
SlottedFile& SlottedFile::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static SlottedFile _NULLOBJECT(SlottedFileInfo::Make(NullObject::GiveNullObject()));
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& SlottedFile::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return SlottedFile::GiveNullObject();
}

/****************************************************************************/
Object& SlottedFile::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return SlottedFile::GiveNullObject();
}

/****************************************************************************/
int SlottedFile::ClassID() const
{
  return TypeInfo::SLOTTEDFILE_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* SlottedFile::GiveObjectAcceptor() const
{
  return (new SlottedFileAcceptor(this));
}

/****************************************************************************/
SlottedFile& SlottedFile::SetSlottedFileProperties(SlottedFileInfo* Info_)
{
  delete _Info;
  _Info = Info_;

  return *this;
}

/****************************************************************************/
SlottedFile& SlottedFile::operator = (const SlottedFile& Sinfo_)
{
  if (this != &Sinfo_)
  {
    delete _Info;
    _Info = (Sinfo_._Info != NULL) ? SlottedFileInfo::Make(*Sinfo_._Info):NULL;
    _StoreToHeader = Sinfo_._StoreToHeader;

    File::operator = (Sinfo_);
  }

  return *this;
}

/****************************************************************************/
long SlottedFile::HeaderSection() const
{
  return (_Info ? long(StorableFormWidth() + _Info->FormWidth()):0);
}

/****************************************************************************/
long SlottedFile::RecordSection() const
{
  return (_Info ? long(HeaderSection() + _Info->HeaderSize()):0);
}

/****************************************************************************/
long SlottedFile::EndOfFileOffset() const
{
  return (_Info ? long(StorableFormWidth() + _Info->DataSpan()):0);
}

/****************************************************************************/
Boolean SlottedFile::ReadFileInfo()
{
  if (!_Info->NullRecord())
    return FALSE;

  istream& Is_ = (istream&)(*GetReader());
  Is_.seekg(0);

  Boolean RdChk_;
  RdChk_ = ReadObjectData(*this, Is_, FALSE);

  if (RdChk_ && _Info && _Info->ReadFileInfo(Is_).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  return ReturnReadValid(Is_, &RdChk_);
}

/****************************************************************************/
Boolean SlottedFile::WriteFileInfo() const
{
  if (!_Info->NullRecord())
    return FALSE;

  ostream& Os_ = (ostream&)(*GetReader());
  Os_.seekp(0);

  Boolean WrChk_;
  WrChk_ = WriteObjectData(*this, Os_);

  if (WrChk_ && _Info && _Info->WriteFileInfo(Os_).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  return ReturnWriteValid(Os_, &WrChk_);
}

/****************************************************************************/
Boolean SlottedFile::Open(const char* Path_, int Modes_)
{
  SlottedFileInfo CurrInfo_(*_Info);
  Boolean ReadDone_ = FALSE;
  Boolean WriteDone_ = FALSE;
  Boolean Valid_ = FALSE;

  if (File::Open(Path_, Modes_))
  {
    ClearStatus(SLOTTEDFILE_NOT_OPENED);

    if ((_Mode & ios::in) && FileSize() != 0)
      ReadDone_ = ReadFileInfo();

    if ((_Mode & ios::out) || (_Mode & ios::app))
    {
      if (ReadDone_)
	WriteDone_ = ((*_Info != CurrInfo_) ? WriteFileInfo():TRUE);
      else
	WriteDone_ = WriteFileInfo();

      Valid_ = WriteDone_;
      if (WriteDone_)
	ClearStatus(SLOTTEDFILE_INFO_NOTREAD);
    }
    else
    {
      Valid_ = ReadDone_;
      if (ReadDone_)
	ClearStatus(SLOTTEDFILE_INFO_NOTREAD);
    }
  }

  Valid_ = Valid_ && InitializeHeader();
  if (Valid_)
    ClearStatus(SLOTTEDFILE_HEADER_NOTREAD);

  return Valid_;
}

/****************************************************************************/
Boolean SlottedFile::Close()
{
  if (((_Mode & ios::out) || (_Mode & ios::app)) && GetBuffer()->is_open())
  {
    Boolean WriteOK_ = WriteFileInfo();
    return (File::Close() && WriteOK_);
  }

  return File::Close();
}

/****************************************************************************/
Boolean SlottedFile::TestForSlottedFile()
{
  Boolean Result_ = FALSE;

  if (File::Open())
  {
    istream& Is_ = (istream&)(*GetReader());
    Is_.seekg(0);

    ReadObjectData(*this, Is_, FALSE);
    Result_ = ReturnReadValid(Is_);
    File::Close();
  }

  return Result_;
}

/****************************************************************************/
Boolean SlottedFile::IsSlottedFile(const char* Path_) const
{
  SlottedFile SlotFile_(*this);

  if (Path_)
    SlotFile_.SetFileProperties(Path_, ios::in);
  else
    SlotFile_.SetFileMode(ios::in);

  return SlotFile_.TestForSlottedFile();
}

/****************************************************************************/
Boolean SlottedFile::IsValidSeek(long RecNum_, int SeekDir_, int IOop_)
{
  return
  (
    (SeekDir_ == ios::beg) 		        ? IsValidSlot(RecNum_, 1):
    (SeekDir_ == ios::cur && IOop_ == ios::in)  ? IsValidSlot(TellGet() + RecNum_, 1):
    (SeekDir_ == ios::cur && IOop_ == ios::out) ? IsValidSlot(TellPut() + RecNum_, 1):
						  IsValidSlot(SlotsAllocated() + RecNum_, 1)
  );
}

/****************************************************************************/
Boolean SlottedFile::IsValidSlot(long SlotNum_, short IncludeEof_) const
{
  long NumSlots_ = SlotsAllocated() + IncludeEof_;
  return (NumSlots_ > 0 && SlotNum_ >= 0 && SlotNum_ < NumSlots_);
}

/****************************************************************************/
Boolean SlottedFile::IsValidSlot(long SlotNum_) const
{
  return IsValidSlot(SlotNum_, 0);
}

/****************************************************************************/
long SlottedFile::AddRecord(const Object& Record_)
{
  if (!_Info || _Status != SLOTTEDFILE_READY)
    return -1;

  Record_.WriteObjectWith(*this);

#if SLOTTEDFILE_RUNTIMECHECK
  SeekPut(0, ios::end);
  long Eof_ = GetOutStream().tellp();
  if (Eof_ == EndOfFileOffset())
#endif

    if (AppendRecord().Good())
    {
      if (_StoreToHeader)
	AppendIDToHeader(Record_.ClassID());

      _Info->AllocateSlot();
      return (SlotsAllocated() - 1);
    }

  return -1;
}

/****************************************************************************/
long SlottedFile::AllocateSlot()
{
  return AddRecord(*_Info->NullRecord());
}

/****************************************************************************/
Boolean SlottedFile::DeleteRecord(long SlotNum_)
{
  if (_Info)
    return ModifyRecord(SlotNum_, *_Info->NullRecord());

  return FALSE;
}

/****************************************************************************/
long SlottedFile::FirstRecord(Object& Record_)
{
  return NextRecord(-1, Record_);
}

/****************************************************************************/
Boolean SlottedFile::ModifyRecord(long SlotNum_, const Object& Record_)
{
  if (!_Info || SlotNum_ < 0 || _Status != SLOTTEDFILE_READY)
    return FALSE;

  Record_.WriteObjectWith(*this);

#if SLOTTEDFILE_RUNTIMECHECK
  SeekPut(0, ios::end);
  long Eof_ = GetOutStream().tellp();
  long OffSet_ = RecordSection() + SlotNum_ * _Info->SlotSize();
  if (OffSet_ < Eof_)
#endif

  if (IsValidSlot(SlotNum_))
  {
    SeekPut(SlotNum_);
    if (WriteRecord().Good())
    {
      if (_StoreToHeader)
	ModifyIDInHeader(SlotNum_, Record_.ClassID());

      return TRUE;
    }
  }
  else if (SlotNum_ >= 0 && SlotsAllocated() <= SlotNum_)
  {
    while (SlotsAllocated() <= SlotNum_)
      if (AllocateSlot() == -1)
	break;

    if (SlotsAllocated() > SlotNum_)
      ModifyRecord(SlotNum_, Record_);
  }

  return FALSE;
}

/****************************************************************************/
long SlottedFile::NextRecord(long CurSlot_, Object& Record_)
{
  ++CurSlot_;
  if (!_Info || CurSlot_ < 0 || _Status != SLOTTEDFILE_READY)
    return -1;

  Record_.ReadObjectWith(*this);
  long SlotNum_;

# if SLOTTEDFILE_RUNTIMECHECK
  SeekGet(0, ios::end);
  long Eof_ = GetInStream().tellg();
  long OffSet_ = RecordSection() + CurSlot_ * _Info->SlotSize();
  if (OffSet_ < Eof_)
#endif

    for (SlotNum_ = CurSlot_; IsValidSlot(SlotNum_); ++SlotNum_)
    {
      SeekGet(SlotNum_);
      if (!ReadRecord().Good())
	break;

      if (!Record_.IsNullObject())
	return SlotNum_;
    }

  return -1;
}

/****************************************************************************/
long SlottedFile::NextRecordID(long CurSlot_, ObjectData& ObjData_)
{
  ++CurSlot_;
  if (!_Info || CurSlot_ < 0 || _Status != SLOTTEDFILE_READY)
    return -1;

  Object* Record_ = NullRecord().Clone();
  Record_->ReadObjectWith(*this);

  long Result_ = -1;
  long SlotNum_;

# if SLOTTEDFILE_RUNTIMECHECK
  SeekGet(0, ios::end);
  long Eof_ = GetInStream().tellg();
  long OffSet_ = RecordSection() + CurSlot_ * _Info->SlotSize();
  if (OffSet_ < Eof_)
#endif

    for (SlotNum_ = CurSlot_; IsValidSlot(SlotNum_); ++SlotNum_)
    {
      SeekGet(SlotNum_);
      if (!StoreObjectData(ObjData_, *this))
	break;

      if (!ObjData_._NullStatus)
      {
	Result_ = SlotNum_;
	break;
      }
    }

  delete Record_;
  return Result_;
}

/****************************************************************************/
long SlottedFile::NthRecord(long NthRecNum_, Object& Record_)
{
  long Slot_ = FirstRecord(Record_);
  long Index_;

  for (Index_ = 0; Index_ < NthRecNum_ && Slot_ != -1; ++Index_)
    Slot_ = NextRecord(Slot_, Record_);

  return Slot_;
}

/****************************************************************************/
long SlottedFile::NthRecordSlot(long NthRecNum_)
{
  Object* ObjPtr_ = NullRecord().Clone();
  NthRecNum_ = NthRecord(NthRecNum_, *ObjPtr_);
  delete ObjPtr_;
  return NthRecNum_;
}

/****************************************************************************/
Boolean SlottedFile::RetrieveRecord(long SlotNum_, Object& Record_)
{
  if (!_Info || SlotNum_ < 0 || _Status != SLOTTEDFILE_READY)
    return FALSE;

  Record_.ReadObjectWith(*this);

#if SLOTTEDFILE_RUNTIMECHECK
  SeekGet(0, ios::end);
  long Eof_ = GetInStream().tellg();
  long OffSet_ = RecordSection() + SlotNum_ * _Info->SlotSize();
  if (OffSet_ < Eof_)
#endif

    if (IsValidSlot(SlotNum_))
    {
      SeekGet(SlotNum_);
      if (ReadRecord().Good())
	return TRUE;
    }

  return FALSE;
}

/****************************************************************************/
const Object& SlottedFile::NullRecord() const
{
  if (!_Info || !_Info->NullRecord())
    THROW (FallibleBase::DerefNullPointerErr());

  return *_Info->NullRecord();
}

/****************************************************************************/
int SlottedFile::IdentifyRecord(long NthRecNum_)
{
  ObjectData ObjData_;
  long Index_;

  long Slot_ = NextRecordID(-1, ObjData_);
  for (Index_ = 0; Index_ < NthRecNum_ && Slot_ != -1; ++Index_)
    Slot_ = NextRecordID(Slot_, ObjData_);

  return ((Slot_ == -1) ? TypeInfo::NOTYPE:ObjData_._IDvalue);
}

/****************************************************************************/
Boolean SlottedFile::StoreIDToHeader(Boolean Flag_)
{
  Boolean Old_ = _StoreToHeader;
  _StoreToHeader = Flag_;

  if (Old_ != Flag_ && Flag_)
  {
    long HdIndex_ = 0;
    long* HeaderData_ = New<long>(SlotsAllocated() + 1);
    HeaderData_[HdIndex_++] = SlotsAllocated();

    long Index_, Max_;
    Object* ObjPtr_ = NullRecord().Clone();

    for (Index_ = 0, Max_ = SlotsAllocated(); Index_ < Max_; ++Index_)
      if (RetrieveRecord(Index_, *ObjPtr_))
	HeaderData_[HdIndex_++] = ObjPtr_->ClassID();
      else
	break;

    Boolean Done_ =
	(Index_ == Max_) &&
	WriteHeader((char*)HeaderData_, sizeof(long) * HdIndex_);

    delete ObjPtr_;
    ::Delete(HeaderData_);

    return Done_;
  }

  return TRUE;
}

/****************************************************************************/
Boolean SlottedFile::ReadIDFromHeader(long* HeaderData_) const
{
  return ReadHeader((char*)HeaderData_, (SlotsAllocated() + 1) * sizeof(long));
}

/****************************************************************************/
void SlottedFile::AppendIDToHeader(long IDvalue_)
{
  long Hsize_ = SlotsAllocated() + 1;
  long* HeaderData_ = New<long>(Hsize_ + 1);
  ReadHeader((char*)HeaderData_, sizeof(long) * Hsize_);

  HeaderData_[0] = Hsize_;
  HeaderData_[Hsize_++] = IDvalue_;
  WriteHeader((char*)HeaderData_, sizeof(long) * Hsize_);
  ::Delete(HeaderData_);
}

/****************************************************************************/
void SlottedFile::ModifyIDInHeader(long SlotNum_, long IDvalue_)
{
  long Hsize_ = SlotsAllocated() + 1;
  long* HeaderData_ = New<long>(Hsize_);
  ReadHeader((char*)HeaderData_, sizeof(long) * Hsize_);

  HeaderData_[SlotNum_ + 1] = IDvalue_;
  WriteHeader((char*)HeaderData_, sizeof(long) * Hsize_);
  ::Delete(HeaderData_);
}

/****************************************************************************/
long SlottedFile::SlotSize() const
{
  if (_Info)
    return _Info->SlotSize();

  return 0;
}

/****************************************************************************/
long SlottedFile::SlotsAllocated() const
{
  if (_Info)
    return _Info->AllocatedSlots();

  return 0;
}

/****************************************************************************/
long SlottedFile::HeaderSize() const
{
  if (_Info)
    return _Info->HeaderSize();

  return 0;
}

/****************************************************************************/
Boolean SlottedFile::ReadHeader(char* Header_, long Size_) const
{
  istream& Is_ = (istream&)(*GetReader());
  long Hsize_ = (0 < Size_ && Size_ < _Info->HeaderSize()) ?
			Size_:_Info->HeaderSize();

  Is_.seekg(HeaderSection());

  Boolean Good_ = FALSE;
  if (Hsize_)
    Good_ = Is_.read(Header_, Hsize_).good();

  Is_.seekg(RecordSection());
  return Good_;
}

/****************************************************************************/
Boolean SlottedFile::WriteHeader(const char* Header_, long Size_)
{
  ostream& Os_ = (ostream&)(*GetReader());
  long Hsize_ = (0 < Size_ && Size_ < _Info->HeaderSize()) ?
			Size_:_Info->HeaderSize();

  Os_.seekp(HeaderSection());

  Boolean Good_ = FALSE;
  if (Hsize_)
    Good_ = Os_.write(Header_, Hsize_).good();

  Os_.seekp(RecordSection());
  return Good_;
}

/****************************************************************************/
Boolean SlottedFile::InitializeHeader()
{
  char* Buffer_ = New<char>(HeaderSize());

  if (Buffer_)
  {
    Boolean Result_ = WriteHeader(Buffer_);
    ::Delete(Buffer_);
    return Result_;
  }

  return FALSE;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* SlottedFile::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void SlottedFile::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* SlottedFile::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void SlottedFile::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if SLOTTEDFILE_DEBUG
#include "shortint.h"
int main()
{
  char Header_[128];
  StrFill(Header_, '\0', 128);
  strcpy(Header_, "This Is The Header Of The File");

  ShortInt NullRec_;
  ShortInt UseRec_;
  ShortInt Record1_(1000);
  ShortInt Record2_(210);
  ShortInt Record3_(3250);
  ShortInt Record4_(440);
  ShortInt Record5_(5700);

  SlottedFile* Sfile_ =
  SlottedFile::Make(SlottedFileInfo::Make(NullRec_, 0, 128));
  Sfile_->SetFileProperties("sample.dat", ios::out|ios::in|ios::binary);

  Sfile_->Open();

  Sfile_->AddRecord(Record1_);
  Sfile_->AddRecord(Record2_);
  Sfile_->AddRecord(Record3_);
  Sfile_->StoreIDToHeader();
  Sfile_->AddRecord(Record4_);
  Sfile_->AddRecord(Record5_);

  long Index_ = 0;
  while ((Index_ = Sfile_->NextRecord(Index_, UseRec_)) != -1)
    cout <<UseRec_ <<endl;

  Sfile_->FirstRecord(UseRec_);
  cout <<UseRec_ <<endl;

  UseRec_ = 21600;
  long Index2_ = Sfile_->NthRecordSlot(2);
  Sfile_->ModifyRecord(Index2_, UseRec_);

  Index_ = Sfile_->NthRecordSlot(1);
  Sfile_->DeleteRecord(Index_);

  if (Sfile_->RetrieveRecord(Index_, UseRec_))
    cout <<UseRec_ <<endl;

  if (Sfile_->RetrieveRecord(Index2_, UseRec_))
    cout <<UseRec_ <<endl;

  long Sz_ = Sfile_->SlotsAllocated() + 1;
  long* IDvalues_ = new long[Sz_];
  Sfile_->ReadIDFromHeader(IDvalues_);
  cout <<"Rec Ids:";

  for (Index_ = 0; Index_ < Sz_; ++Index_)
    cout <<" " <<IDvalues_[Index_];
  delete[] IDvalues_;

  Sfile_->WriteHeader((char*)Header_);
  Sfile_->ReadHeader((char*)Header_);
  cout <<endl <<Header_ <<endl;

  cout <<TypeInfo::GiveClassName(Sfile_->IdentifyRecord(Index2_)) <<endl;

  Sfile_->Close();
  cout <<Sfile_->IsSlottedFile("sample.dat") <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





