#ifndef VARLENGTHFILE_CPP
#define VARLENGTHFILE_CPP
#ifndef VARLENGTHFILE_H
  #include "varlengthfile.h"
#endif

/****************************************************************************/
VarLengthFileAcceptor::VarLengthFileAcceptor(const VarLengthFile* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
void VarLengthFileAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const VarLengthFile*)Ptr_;
}

/****************************************************************************/
const void* VarLengthFileAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
Boolean VarLengthFileAcceptor::IsFile() const
{
  return TRUE;
}

/****************************************************************************/
Boolean VarLengthFileAcceptor::IsVarLengthFile() const
{
  return TRUE;
}

/****************************************************************************/
void* VarLengthFileAcceptor::CreateFromVarLengthFile(const ObjectAcceptor& Obj_)
{
  _VarPtr = new VarLengthFile(*((const VarLengthFile*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* VarLengthFileAcceptor::AssignFromVarLengthFile(const ObjectAcceptor& Obj_)
{
  const VarLengthFile* TrgPtr_ = (const VarLengthFile*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* VarLengthFileAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void VarLengthFileAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
VarLengthFileBrowser::VarLengthFileBrowser(ByteStream* Stream_):
StreamBrowser(Stream_)
{}

/****************************************************************************/
ByteStream& VarLengthFileBrowser::ReadRecord()
{
  if (_Browser.GetMode() & ios::in)
    return _Browser.Read(*_Stream);
    
  return *_Stream;
}

/****************************************************************************/
ByteStream& VarLengthFileBrowser::PeekRecord()
{
  if (_Browser.GetMode() & ios::in)
  {
    _Browser.Read(*_Stream);
    if (_Stream->Verified())
      _Stream->SeekGet(-1, ios::cur);
  }

  return *_Stream;
}

/****************************************************************************/
ByteStream& VarLengthFileBrowser::WriteRecord()
{
  return _Browser.Write(*_Stream);
}

/****************************************************************************/
Boolean VarLengthFileBrowser::Verified() const
{
  return _Browser.Verified();
}

/****************************************************************************/
ByteStream* VarLengthFileBrowser::MakeByteStreamBrowser()
{
  return (new VarLengthFileBrowser(_Stream));
}

/****************************************************************************/
void VarLengthFileBrowser::SetByteStreamBrowser(ByteStream* Browser_)
{
  _Stream->SetByteStreamBrowser(Browser_);
}

/****************************************************************************/
ByteStream* VarLengthFileBrowser::GetByteStreamBrowser() const
{
  return _Stream->GetByteStreamBrowser();
}

/****************************************************************************/
ByteStreamIterator* VarLengthFileBrowser::NewByteStreamIterator()
{
  return (new ByteStreamIterator(_Stream, CurrentIOMode()));
}

/****************************************************************************/
/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo::VarLengthFilePosInfo(VarLengthFileInfo* ParPtr_):
_InfoPtr(ParPtr_),
_ChunkArray(ParPtr_->GetRecFnc(0)),
_RecordNum(0),
_RecordPos(0),
_ChunkNum(0),
_ChunkPos(0),
_RealBytePos(ParPtr_->GiveFileOffset()),
_LinearBytePos(ParPtr_->GiveFileOffset())
{}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo::VarLengthFilePosInfo(VarLengthFileInfo* ParPtr_, VarLengthFileInfo::VarLengthFilePosInfo* InfoPtr_):
_InfoPtr(ParPtr_),
_ChunkArray(ParPtr_->GetRecFnc(InfoPtr_->_RecordNum)),
_RecordNum(InfoPtr_->_RecordNum),
_RecordPos(InfoPtr_->_RecordPos),
_ChunkNum(InfoPtr_->_ChunkNum),
_ChunkPos(InfoPtr_->_ChunkPos),
_RealBytePos(InfoPtr_->_RealBytePos),
_LinearBytePos(InfoPtr_->_LinearBytePos)
{}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo::~VarLengthFilePosInfo()
{
  _ChunkArray = VarLengthFileInfo::DeletePosArray(_ChunkArray);
}

/****************************************************************************/
void VarLengthFileInfo::VarLengthFilePosInfo::Reset()
{
  _RecordNum =
  _RecordPos =
  _ChunkNum =
  _ChunkPos = 0;
  _RealBytePos =
  _LinearBytePos = _InfoPtr->GiveFileOffset();
  _ChunkArray = VarLengthFileInfo::DeletePosArray(_ChunkArray);
  _ChunkArray = _InfoPtr->GetRecFnc(0);
}

/****************************************************************************/
Boolean VarLengthFileInfo::VarLengthFilePosInfo::SeekRecNum(long RecNum_)
{
  if (RecNum_ >= _InfoPtr->FileCapacity())
    return FALSE;

  Boolean SeekOk_ = TRUE;
  long** PosArr_ = _InfoPtr->GetRecFnc(RecNum_);
  
  if (PosArr_ && *PosArr_)
  {
    _ChunkArray = VarLengthFileInfo::DeletePosArray(_ChunkArray);
    _ChunkArray = PosArr_;
  
    _RecordNum = RecNum_;
    _RecordPos =
    _ChunkNum =
    _ChunkPos = 0;
    _RealBytePos = **PosArr_;
    _LinearBytePos = (_RecordNum * _InfoPtr->RecordSize()) + _InfoPtr->GiveFileOffset();
  }
  else
    SeekOk_ = FALSE;

  return SeekOk_;
}

/****************************************************************************/
Boolean VarLengthFileInfo::VarLengthFilePosInfo::SeekFilePos(long Pos_)
{
  if (!IsValidFilePos(Pos_))
    return FALSE;

  _LinearBytePos = Pos_;
  Pos_ -= _InfoPtr->GiveFileOffset();
  _RecordNum = Pos_ / _InfoPtr->RecordSize();
  _RecordPos = Pos_ % _InfoPtr->RecordSize();
  _ChunkNum = _RecordPos / _InfoPtr->ChunkSize();
  _ChunkPos = _RecordPos % _InfoPtr->ChunkSize();

  long** PosArr_ = _InfoPtr->GetRecFnc(_RecordNum);
  
  if (PosArr_ && *PosArr_)
  {
    _ChunkArray = VarLengthFileInfo::DeletePosArray(_ChunkArray);
    _ChunkArray = PosArr_;
  
    long Index_;
    for (Index_ = 0; Index_ < _ChunkNum && *PosArr_; Index_++, PosArr_++);

    if (*PosArr_)
      _RealBytePos = **PosArr_ + _ChunkPos;
  }

  if (PosArr_ && *PosArr_)
    return TRUE;

  return FALSE;
}

/****************************************************************************/
Boolean VarLengthFileInfo::VarLengthFilePosInfo::SeekFilePos(long Pos_, int Dir_)
{
  if (Dir_ == ios::beg)
  {
    if (IsValidFilePos(Pos_))
      return SeekFilePos(Pos_);
  }
  else if (Dir_ == ios::cur)
  {
    Pos_ += _LinearBytePos;
    if (IsValidFilePos(Pos_))
      return SeekFilePos(Pos_);
  }
  else if (Dir_ == ios::end)
  {
    Pos_ += SeekLimitSize();
    if (IsValidFilePos(Pos_))
      return SeekFilePos(Pos_);
  }

  return FALSE;
}

/****************************************************************************/
Boolean VarLengthFileInfo::VarLengthFilePosInfo::UpdateFilePos()
{
  if (_RecordNum >= _InfoPtr->FileCapacity())
    return FALSE;

  Boolean SeekOk_ = TRUE;
  long** PosArr_ = _InfoPtr->GetRecFnc(_RecordNum);
  
  if (PosArr_ && *PosArr_)
  {
    _ChunkArray = VarLengthFileInfo::DeletePosArray(_ChunkArray);
    _ChunkArray = PosArr_;

    long Index_;
    for (Index_ = 0; Index_ < _ChunkNum && *PosArr_; Index_++, PosArr_++);
    
    if (*PosArr_)
    {
      _RealBytePos = **PosArr_ + _ChunkPos;
      _LinearBytePos = (_RecordNum * _InfoPtr->RecordSize()) + _RecordPos +
                        _InfoPtr->GiveFileOffset();
      SeekOk_ = IsValidFilePos(_LinearBytePos);
    }
  }

  if (PosArr_ && *PosArr_ && SeekOk_)
    return TRUE;

  return FALSE;
}

/****************************************************************************/
long VarLengthFileInfo::VarLengthFilePosInfo::Read(istream& Is_, char* Buffer_, long Max_)
{
  if (_RecordNum > _InfoPtr->HighestRecord())
    return 0;

  long Cpr_ = _InfoPtr->ChunksPerRecord();
  long Total_ = Max_;
  Boolean ReadOk_ = TRUE;
    
  while (_ChunkNum < Cpr_ && Max_)
  {
    Is_.seekg(_RealBytePos);
    long Diff_ = _InfoPtr->ChunkSize();
      
    if (_ChunkPos)
      Diff_ -= _ChunkPos;

    long ActualRead_ = (Max_ < Diff_) ? Max_:Diff_;
    Is_.read(Buffer_, ActualRead_);
    Max_ -= ActualRead_;
    Buffer_ += ActualRead_;

    _RecordPos += ActualRead_;
    _LinearBytePos += ActualRead_;

    if (ActualRead_ < Diff_)
    {
      _ChunkPos += ActualRead_;
      _RealBytePos += ActualRead_;
    }
    else
    {
      _ChunkPos = 0;    
      _ChunkNum++;

      if (_ChunkNum == Cpr_)
        SeekRecNum(_RecordNum + 1);
      else if (_ChunkArray && _ChunkArray[_ChunkNum])
        _RealBytePos = *(_ChunkArray[_ChunkNum]);
      else
      {
        ReadOk_ = FALSE;
        break;
      }      
    }          
  }

  if (!ReadOk_)
    return 0;

  return (!Max_ ? Total_:(Total_ - Max_));
}

/****************************************************************************/
long VarLengthFileInfo::VarLengthFilePosInfo::Write(ostream& Os_, const char* Buffer_, long Max_)
{
  if (_RecordNum > _InfoPtr->HighestRecord())
    return 0;

  long Cpr_ = _InfoPtr->ChunksPerRecord();
  long Total_ = Max_;
  Boolean WriteOk_ = TRUE;
    
  while (_ChunkNum < Cpr_ && Max_)
  {
    Os_.seekp(_RealBytePos);
    long Diff_ = _InfoPtr->ChunkSize();
      
    if (_ChunkPos)
      Diff_ -= _ChunkPos;

    long ActualWritten_ = (Max_ < Diff_) ? Max_:Diff_;
    Os_.write(Buffer_, ActualWritten_);
    Max_ -= ActualWritten_;
    Buffer_ += ActualWritten_;

    _RecordPos += ActualWritten_;
    _LinearBytePos += ActualWritten_;
    
    if (ActualWritten_ < Diff_)
    {
      _ChunkPos += ActualWritten_;
      _RealBytePos += ActualWritten_;
    }
    else
    {
      _ChunkPos = 0;    
      _ChunkNum++;

      if (_ChunkNum == Cpr_)
        SeekRecNum(_RecordNum + 1);
      else if (_ChunkArray && _ChunkArray[_ChunkNum])
        _RealBytePos = *(_ChunkArray[_ChunkNum]);
      else
      {
        WriteOk_ = FALSE;
        break;
      }      
    }      
  }

  if (!WriteOk_)
    return 0;

  return (!Max_ ? Total_:(Total_ - Max_));
}

/****************************************************************************/
#if OVERLOAD_NEW
void* VarLengthFileInfo::VarLengthFilePosInfo::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void VarLengthFileInfo::VarLengthFilePosInfo::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* VarLengthFileInfo::VarLengthFilePosInfo::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void VarLengthFileInfo::VarLengthFilePosInfo::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
VarLengthFileInfo::VarLengthFileInfo(const Object& Record_,
                                     long ChunkSize_, long InitialRun_,
                                     long Allocated_, long HeaderSize_):
_ChunkSize(ChunkSize_),
_InitialRun(InitialRun_),
_NullBlock((char*)SafeMemSet(new_char_array(ChunkSize_, NULL), 0, ChunkSize_)),
_ObjectRecordSize(Record_.NULLOBJECT().StorableFormWidth()),
_HeaderSize(Allocated_ ? ((Allocated_ + 1) * sizeof(long)):HeaderSize_),
_HighestRecord(0),
_RecordCount(0),
_FileSize(0),
_Index(2),
_InputPos(NULL),
_OutputPos(NULL),
_FileBase(FormWidth() + HeaderSize()),
_FileOffset(0),
_ObjectRecord(Record_.NULLOBJECT().Clone())
{
  _FileConfig[0] = _ChunkSize;
  _FileConfig[1] = _InitialRun;
  _FileConfig[2] = 1;

  _InputPos = new VarLengthFilePosInfo(this);
  _OutputPos = new VarLengthFilePosInfo(this);
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFileInfo(const VarLengthFileInfo& Obj_):
_ChunkSize(Obj_._ChunkSize),
_InitialRun(Obj_._InitialRun),
_NullBlock((char*)SafeMemSet(new_char_array(Obj_._ChunkSize, NULL), 0, Obj_._ChunkSize)),
_ObjectRecordSize(Obj_._ObjectRecordSize),
_HeaderSize(Obj_._HeaderSize),
_HighestRecord(Obj_._HighestRecord),
_RecordCount(Obj_._RecordCount),
_FileSize(Obj_._FileSize),
_Index(Obj_._Index),
_InputPos(NULL),
_OutputPos(NULL),
_FileBase(FormWidth() + HeaderSize()),
_FileOffset(0),
_ObjectRecord(Obj_._ObjectRecord ? Obj_._ObjectRecord->Clone():NULL)
{
  memmove(_FileConfig, Obj_._FileConfig, sizeof(long) * MAXCONFIG);

  _InputPos = new VarLengthFilePosInfo(this, Obj_._InputPos);
  _OutputPos = new VarLengthFilePosInfo(this, Obj_._OutputPos);
}

/****************************************************************************/
VarLengthFileInfo::~VarLengthFileInfo()
{
  ::DeleteArray(_NullBlock);
  _NullBlock = NULL;

  delete _ObjectRecord;
  _ObjectRecord = NULL;

  delete _OutputPos;
  delete _InputPos;
  _OutputPos = _InputPos = NULL;
}

/****************************************************************************/
VarLengthFileInfo* VarLengthFileInfo::Make(const VarLengthFileInfo& Finfo_)
{
  return (new VarLengthFileInfo(Finfo_));
}

/****************************************************************************/
VarLengthFileInfo* VarLengthFileInfo::Make(const Object& Record_,
                                           long ChunkSize_, long InitialRun_,
                                           long Allocated_, long HeaderSize_)
{
  return (new VarLengthFileInfo(Record_, ChunkSize_, InitialRun_, Allocated_, HeaderSize_));
}

/****************************************************************************/
istream& VarLengthFileInfo::ReadFileInfo(istream& Is_)
{
  size_t i;
  long Value_;
  long PrevPos_ = Is_.tellg();

  if (_ObjectRecord)
  {
    Boolean Ok_;

    Is_.seekg(_FileOffset);
    Is_.read((char*)(&_ObjectRecordSize), sizeof(long));
    _ObjectRecord->BinaryRead(Is_, &Ok_);

    if (Ok_)
    {
      Is_.read((char*)(&_ChunkSize), sizeof(long));
      Is_.read((char*)(&_InitialRun), sizeof(long));

      for (i = 0; i < MAXCONFIG; i++)
      {
        Is_.read((char*)(&Value_), sizeof(long));
        _FileConfig[i] = Value_;
      }

      Is_.read((char*)(&_HeaderSize), sizeof(long));
      Is_.read((char*)(&_HighestRecord), sizeof(long));
      Is_.read((char*)(&_RecordCount), sizeof(long));
      Is_.read((char*)(&_FileSize), sizeof(long));
      Is_.read((char*)(&_Index), sizeof(long));
    }

    Is_.seekg(PrevPos_);    
  }  
  
  _InputPos->Reset();
  _OutputPos->Reset();
  
  return Is_;
}

/****************************************************************************/
ostream& VarLengthFileInfo::WriteFileInfo(ostream& Os_) const
{
  size_t i;
  long Value_;
  long PrevPos_ = Os_.tellp();
  
  if (_ObjectRecord)
  {
    Boolean Ok_;

    Os_.seekp(_FileOffset);
    Os_.write((char*)(&_ObjectRecordSize), sizeof(long));
    _ObjectRecord->BinaryWrite(Os_, &Ok_);

    if (Ok_)
    {
      Os_.write((char*)(&_ChunkSize), sizeof(long));
      Os_.write((char*)(&_InitialRun), sizeof(long));

      for (i = 0; i < MAXCONFIG; i++)
      {
        Value_ = _FileConfig[i];
        Os_.write((char*)(&Value_), sizeof(long));
      }

      Os_.write((char*)(&_HeaderSize), sizeof(long));
      Os_.write((char*)(&_HighestRecord), sizeof(long));
      Os_.write((char*)(&_RecordCount), sizeof(long));      
      Os_.write((char*)(&_FileSize), sizeof(long));
      Os_.write((char*)(&_Index), sizeof(long));
    }

    Os_.seekp(PrevPos_);    
  }

  return Os_;
}

/****************************************************************************/
VarLengthFileInfo& VarLengthFileInfo::operator = (const VarLengthFileInfo& Obj_)
{
  if (this != &Obj_)
  {
    _ChunkSize = Obj_._ChunkSize;
    _InitialRun = Obj_._InitialRun;
    _ObjectRecordSize = Obj_._ObjectRecordSize;
    _HeaderSize = Obj_._HeaderSize;
    _HighestRecord = Obj_._HighestRecord;
    _RecordCount = Obj_._RecordCount;
    _FileSize = Obj_._FileSize;
    _Index = Obj_._Index;
    _FileBase = FormWidth() + HeaderSize();
    _FileOffset = 0;
    delete _ObjectRecord;
    _ObjectRecord = Obj_._ObjectRecord ? Obj_._ObjectRecord->Clone():NULL;
    memmove(_FileConfig, Obj_._FileConfig, sizeof(long) * MAXCONFIG);
    delete _OutputPos;
    delete _InputPos;
    _InputPos = new VarLengthFilePosInfo(this, Obj_._InputPos);
    _OutputPos = new VarLengthFilePosInfo(this, Obj_._OutputPos);
  }

  return *this;    
}

/****************************************************************************/
VarLengthFileInfo* VarLengthFileInfo::SetHeaderSize(long Size_)
{
  _HeaderSize = Size_;
  _FileBase = FormWidth() + Size_;
  return this;
}

/****************************************************************************/
VarLengthFileInfo* VarLengthFileInfo::SetFileOffset(long Offset_)
{
  _FileOffset = Offset_;
  return this;
}

/****************************************************************************/
VarLengthFileInfo* VarLengthFileInfo::AllocateSlots(long NewSlots_)
{
  if (NewSlots_)
    PutRec(HighestRecord() + NewSlots_);
    
  return this;
}

/****************************************************************************/
Boolean VarLengthFileInfo::operator == (VarLengthFileInfo& Info_)
{
  size_t i;
  Boolean IsEqualFileConfig_ = TRUE;
  Boolean NullObjEqual_ = TRUE;

  if (Info_._ObjectRecord && _ObjectRecord)
    NullObjEqual_ =
    Info_._ObjectRecord->IsNullObject() && _ObjectRecord->IsNullObject();

  if (_Index == Info_._Index)
    for (i = 0; i <= _Index; i++)
      if (_FileConfig[i] != Info_._FileConfig[i])
      {
        IsEqualFileConfig_ = FALSE;
        break;
      }

  return
  (
    Info_._ChunkSize == _ChunkSize &&
    Info_._InitialRun == _InitialRun &&
    Info_._ObjectRecordSize == _ObjectRecordSize &&
    Info_._HeaderSize == _HeaderSize &&
    IsEqualFileConfig_ &&
    Info_._HighestRecord == _HighestRecord &&
    Info_._RecordCount == _RecordCount &&    
    Info_._Index == _Index &&
    NullObjEqual_
  );
}

/****************************************************************************/
long** VarLengthFileInfo::DeletePosArray(long** PosArray_)
{
  long** StartPtr_ = PosArray_;
  
  if (PosArray_)
    for (;*PosArray_; PosArray_++)
      ::Delete(*PosArray_);

  RawDeleteArray(StartPtr_);  
  return NULL;
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::SeekGet(istream& Is_, long Pos_)
{
  Boolean Done_;
  if (Done_ = _InputPos->SeekRecNum(Pos_))
    Is_.seekg(_InputPos->_RealBytePos);      

  return (Done_ ? _InputPos:NULL);
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::SeekGet(istream& Is_, long Pos_, int Dir_)
{
  long Limit_ = _FileSize ? (HighestRecord() + 1):0;
  long RecNum_;

  if (Dir_ == ios::beg)
    return SeekGet(Is_, Pos_);

  if (Pos_ < 0)
  {
    if (Dir_ == ios::end)
      SeekGet(Is_, Limit_);

    long AbsCurPos_ = _InputPos->_RecordNum;

    if (Pos_ + AbsCurPos_ < 0L)
      Pos_ = -AbsCurPos_;
  }
  else if (Pos_ > 0 && Dir_ == ios::end)
    Pos_ = 0;

  RecNum_ = (Dir_ == ios::end) ? (Limit_ + Pos_):(_InputPos->_RecordNum + Pos_);
  return SeekGet(Is_, RecNum_);
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::Seekg(istream& Is_, long Pos_)
{
  VarLengthFileInfo::VarLengthFilePosInfo* RetPtr_ = (_InputPos->SeekFilePos(Pos_) ? _InputPos:NULL);
  Is_.seekg(_InputPos->_RealBytePos);
  return RetPtr_;
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::Seekg(istream& Is_, long Pos_, int Dir_)
{
  VarLengthFileInfo::VarLengthFilePosInfo* RetPtr_ = (_InputPos->SeekFilePos(Pos_, Dir_) ? _InputPos:NULL);
  Is_.seekg(_InputPos->_RealBytePos);
  return RetPtr_;
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::TellGet()
{
  return _InputPos;
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::SeekPut(ostream& Os_, long Pos_)
{
  Boolean Done_;
  if (Done_ = _OutputPos->SeekRecNum(Pos_))
    Os_.seekp(_OutputPos->_RealBytePos);

  return (Done_ ? _OutputPos:NULL);
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::SeekPut(ostream& Os_, long Pos_, int Dir_)
{
  long Limit_ = _FileSize ? (HighestRecord() + 1):0;
  long RecNum_;

  if (Dir_ == ios::beg)
    return SeekPut(Os_, Pos_);

  if (Pos_ < 0)
  {
    if (Dir_ == ios::end)
      SeekPut(Os_, Limit_);

    long AbsCurPos_ = _OutputPos->_RecordNum;

    if (Pos_ + AbsCurPos_ < 0L)
      Pos_ = -AbsCurPos_;
  }
  else if (Pos_ > 0 && Dir_ == ios::end)
    Pos_ = 0;

  RecNum_ = (Dir_ == ios::end) ? (Limit_ + Pos_):(_OutputPos->_RecordNum + Pos_);
  return SeekPut(Os_, RecNum_);
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::Seekp(ostream& Os_, long Pos_)
{
  VarLengthFileInfo::VarLengthFilePosInfo* RetPtr_ = (_OutputPos->SeekFilePos(Pos_) ? _OutputPos:NULL);
  Os_.seekp(_OutputPos->_RealBytePos);
  return RetPtr_;
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::Seekp(ostream& Os_, long Pos_, int Dir_)
{
  VarLengthFileInfo::VarLengthFilePosInfo* RetPtr_ = (_OutputPos->SeekFilePos(Pos_, Dir_) ? _OutputPos:NULL);
  Os_.seekp(_OutputPos->_RealBytePos);  
  return RetPtr_;
}

/****************************************************************************/
VarLengthFileInfo::VarLengthFilePosInfo* VarLengthFileInfo::TellPut()
{
  return _OutputPos;
}

/****************************************************************************/
long VarLengthFileInfo::Read(istream& Is_, char* Buffer_, long Max_)
{
  Boolean HigherRecord_ = _InputPos->_RecordNum > HighestRecord();
  Boolean GreaterSize_ = Max_ > RecordSize();

  if (HigherRecord_ || GreaterSize_)
    return 0;

  return _InputPos->Read(Is_, Buffer_, Max_);
}

/****************************************************************************/
long VarLengthFileInfo::Write(ostream& Os_, const char* Buffer_, long Max_)
{
  long RecNum_ = _OutputPos->_RecordNum;
  Boolean HigherRecord_ = RecNum_ > HighestRecord();
  Boolean GreaterSize_ = Max_ > RecordSize();

  if (HigherRecord_ || GreaterSize_)
  {
    if (GreaterSize_)
      RecNum_ = HighestRecord();
      
    PutRec(RecNum_, Max_);
    UpdateFileSize(Os_);

    if (HigherRecord_)
      SeekPut(Os_, RecNum_);    
  }
  
  return _OutputPos->Write(Os_, Buffer_, Max_);
}

/****************************************************************************/
long VarLengthFileInfo::ReadRec(istream& Is_, char* Buffer_, long Max_)
{
  long BytesRead_ = Read(Is_, Buffer_, Max_);
  SeekGet(Is_, _InputPos->_RecordNum + 1);
  return BytesRead_;
}

/****************************************************************************/    
long VarLengthFileInfo::WriteRec(ostream& Os_, const char* Buffer_, long Max_)
{
  long BytesWritten_ = Write(Os_, Buffer_, Max_);
  SeekPut(Os_, _OutputPos->_RecordNum + 1);
  return BytesWritten_;
}

/****************************************************************************/
long VarLengthFileInfo::UpdateFileSize(ostream& Os_)
{
  long Sz_ = FileSize(Os_);
  long Diff_ = FileSize() - Sz_;

  if (Diff_ > 0)
  {  
    long NumChunks_ = Diff_ / ChunkSize();
    long Remain_ = Diff_ % ChunkSize();

    long OldPos_ = Os_.tellp();
    Os_.seekp(0, ios::end);

    long Index_;    
    for (Index_ = 0; Index_ < NumChunks_; Index_++)
      Os_.write(_NullBlock, ChunkSize());

    if (Remain_)
    {
      char* Leftover_ = (char*)memset(new_char_array(Remain_, NULL), 0, Remain_);
      Os_.write(Leftover_, Remain_);
      ::DeleteArray(Leftover_);
    }

    Os_.seekp(OldPos_);
    return Diff_;
  }

  return 0;
}

/****************************************************************************/
long VarLengthFileInfo::FileSize(istream& Is_)
{
  long OldPos_ = Is_.tellg();
  Is_.seekg(0, ios::end);
  long EofPos_ = Is_.tellg();
  EofPos_ -= GiveFileOffset();
  Is_.seekg(OldPos_);
  return EofPos_;
}

/****************************************************************************/
long VarLengthFileInfo::FileSize(ostream& Os_)
{
  long OldPos_ = Os_.tellp();
  Os_.seekp(0, ios::end);
  long EofPos_ = Os_.tellp();
  EofPos_ -= GiveFileOffset();
  Os_.seekp(OldPos_);
  return EofPos_;
}

/****************************************************************************/
long VarLengthFileInfo::Pow2(size_t Exp_) const
{
  long Val_ = 1;
  size_t Index_;
  
  for (Index_ = 0; Index_ < Exp_; ++Index_)
    Val_ *= 2;

  return Val_;
}

/****************************************************************************/
long VarLengthFileInfo::FileCapacity() const
{
  size_t Index_;
  long Capacity_ = 1;
  
  for (Index_ = 1; Index_ <= _Index; ++Index_)
    Capacity_ *= _FileConfig[Index_];

  return Capacity_;
}

/****************************************************************************/
long VarLengthFileInfo::SegmentsPerPage(size_t Exp_) const
{
  size_t Index_, Cnt_;
  
  long Spp_ = 1;
  Index_ = 1;
  
  for (Cnt_ = 0; Cnt_ <= Exp_; ++Cnt_, ++Index_)
    Spp_ *= _FileConfig[Index_];

  Spp_ *= NestingValue(Exp_);
  return Spp_;
}

/****************************************************************************/
long VarLengthFileInfo::RecordNumber(long RecNum_, size_t Exp_) const
{
  return
  (
    (Exp_ == Exponent()) ?
      RecNum_:
      (RecordNumber(RecNum_, Exp_ + 1) % (SegmentsPerPage(Exp_ + 1) / NestingValue(Exp_ + 1)))
  );
}

/****************************************************************************/
long VarLengthFileInfo::PageSeek(long RecNum_, size_t Exp_) const
{
  return (PageNumber(RecNum_, Exp_) * SegmentsPerPage(Exp_));
}

/****************************************************************************/
long VarLengthFileInfo::ClusterSeek(size_t Exp_, size_t Index_) const
{
  Index_ %= Pow2(Exp_);
  Index_ /= Pow2(Exp_ - 1);
  long cs_ = 0;
  
  if (Index_)
    cs_ = SegmentsPerPage(Exp_) / 2;

  return cs_;
}

/****************************************************************************/
long VarLengthFileInfo::RecordSeek(long RecNum_) const
{
  return (RecordNumber(RecNum_, 0) - (PageNumber(RecNum_, 0) * SegmentsPerPage(0)));
}

/****************************************************************************/
long VarLengthFileInfo::FindStartPosHelper(long RecNum_, size_t Index_, size_t Exp_)
{
  if (Exp_ != 0)
    return
    (
      PageSeek(RecNum_, Exp_) + ClusterSeek(Exp_, Index_) +
      FindStartPosHelper(RecNum_, Index_, Exp_ - 1)
    );

  return (PageSeek(RecNum_, Exp_) + RecordSeek(RecNum_));
}

/****************************************************************************/
long** VarLengthFileInfo::GetRecFnc(long RecNum_)
{
  long Index_;
  long Limit_ = _FileSize ? (HighestRecord() + 1):0;
  
  if (RecNum_ > Limit_ || RecNum_ >= FileCapacity())
    return NULL;

  size_t Exp_ = Exponent();
  Limit_ = Pow2(Exp_);
  long** StartPos_ = ((long**)RawAllocateWith(MEMMATRIX, sizeof(long*) * (Limit_ + 1)));
  
  for (Index_ = 0; Index_ < Limit_; ++Index_)
  {
    StartPos_[Index_] = new_long(0);
    *StartPos_[Index_] = FindStartPos(RecNum_, Index_);
  }

  StartPos_[Index_] = NULL;
  return AddFileOffset(StartPos_);
}

/****************************************************************************/
long** VarLengthFileInfo::GetRec(long RecNum_)
{
  if (RecNum_ > HighestRecord())
    return NULL;

  return GetRecFnc(RecNum_);
}

/****************************************************************************/
void VarLengthFileInfo::AddPage()
{
  ++_FileConfig[_Index];
}

/****************************************************************************/
void VarLengthFileInfo::IncreaseRecSize()
{
  _FileConfig[++_Index] = 1;
}

/****************************************************************************/
long VarLengthFileInfo::PutRec(long RecNum_, long Size_)
{
  Boolean UpdateSize_ = FALSE;
  long RecsTotal_ = RecNum_;
  long OldSize_ = _FileSize ? (FileCapacity() * RecordSize()):0;
  _FileSize = OldSize_;

  if (RecNum_ >= _HighestRecord)
  {
    _HighestRecord = RecNum_;
    
    if (RecNum_ >= _RecordCount)
    {
      ++RecsTotal_;
      _RecordCount = RecsTotal_;
    }
  }

  if (RecsTotal_ >= FileCapacity())
  {
    UpdateSize_ = TRUE;
    while (RecsTotal_ >= FileCapacity())
      AddPage();
  }

  if (Size_ > RecordSize())
  {
    UpdateSize_ = TRUE;  
    while (Size_ > RecordSize())
      IncreaseRecSize();
  }

  if (UpdateSize_)
  {
    _InputPos->UpdateFilePos();
    _OutputPos->UpdateFilePos();
  }

  long NewSize_ = FileCapacity() * RecordSize();
  long Diff_ = NewSize_ - OldSize_;
  _FileSize += Diff_;

  return Diff_;
}

/****************************************************************************/
long** VarLengthFileInfo::AddFileOffset(long** PosArray_)
{
  long** StartPos_ = PosArray_;

  if (PosArray_)
    while (*PosArray_)
    {
      **PosArray_ += GiveFileOffset();
      ++PosArray_;
    }

  return StartPos_;
}

/****************************************************************************/
long** VarLengthFileInfo::MinusFileOffset(long** PosArray_)
{
  long** StartPos_ = PosArray_;

  if (PosArray_)
    while (*PosArray_)
    {
      **PosArray_ -= GiveFileOffset();
      ++PosArray_;
    }

  return StartPos_;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* VarLengthFileInfo::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void VarLengthFileInfo::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* VarLengthFileInfo::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void VarLengthFileInfo::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
VarLengthFile::VarLengthFile(VarLengthFileInfo* Info_):
_Info(Info_),
_Status(VARLENGTHFILE_NOTREADY),
_StoreToHeader(FALSE)
{
  MakeByteStreamBrowser();
  _Info->SetFileOffset(StorableFormWidth());
}

/****************************************************************************/
VarLengthFile::VarLengthFile(const File& Finfo_, VarLengthFileInfo* Info_):
File(Finfo_),
_Info(Info_),
_Status(VARLENGTHFILE_NOTREADY),
_StoreToHeader(FALSE)
{
  MakeByteStreamBrowser();
  _Info->SetFileOffset(RecordFormWidth());
}

/****************************************************************************/
VarLengthFile::VarLengthFile(const VarLengthFile& Finfo_):
File(Finfo_),
_Info(Finfo_._Info ? VarLengthFileInfo::Make(*Finfo_._Info):NULL),
_Status(VARLENGTHFILE_NOTREADY),
_StoreToHeader(Finfo_._StoreToHeader)
{
  MakeByteStreamBrowser();
}

/****************************************************************************/
VarLengthFile::~VarLengthFile()
{
  delete _Info;
  _Info = NULL;
}

/****************************************************************************/
/****************************************************************************/
ByteStream* VarLengthFile::MakeByteStreamBrowser()
{
  if (ClassID() == StaticID())
    GetReader()->SetByteStreamBrowser(new VarLengthFileBrowser((ByteStream*)this));
  else
    GetReader()->SetByteStreamBrowser(NULL);

  return GetReader()->GetByteStreamBrowser();
}

/****************************************************************************/
void VarLengthFile::SetByteStreamBrowser(ByteStream* Browser_)
{
  if (Browser_)
    GetReader()->SetByteStreamBrowser(Browser_);
  else
    GetReader()->SetByteStreamBrowser(NULL);
}

/****************************************************************************/
ByteStream* VarLengthFile::GetByteStreamBrowser() const
{
  return GetReader()->GetByteStreamBrowser();
}

/****************************************************************************/
istream& VarLengthFile::iseekg(long Pos_)
{
  VLFMARKER("start: VarLengthFile::iseekg(long)")

  if (Pos_ < 0)
    Pos_ = 0;

  VLFTRACE(RecordSection())
  VLFTRACE(Pos_)

  istream& Is_ = (istream&)(*GetReader());
  _Info->Seekg(Is_, RecordSection() + Pos_);
  
  IstreamCheck(Is_);

  VLFMARKER("end: VarLengthFile::iseekg(long)")
  return Is_;
}

/****************************************************************************/
istream& VarLengthFile::iseekg(long Pos_, int Dir_)
{
  VLFMARKER("start: VarLengthFile::iseekg(long)")

  if (Pos_ < 0)
    Pos_ = 0;

  VLFTRACE(RecordSection())
  VLFTRACE(Pos_)

  istream& Is_ = (istream&)(*GetReader());
  
  if (Dir_ == ios::beg)
    _Info->Seekg(Is_, RecordSection() + Pos_, Dir_);
  else
    _Info->Seekg(Is_, Pos_, Dir_);
  
  IstreamCheck(Is_);

  VLFMARKER("end: VarLengthFile::iseekg(long)")
  return Is_;
}

/****************************************************************************/
istream& VarLengthFile::iread(char* Bytes_, Ulong Max_)
{
  istream& Is_ = (istream&)(*GetReader());
  _Info->Read(Is_, Bytes_, Max_);
  IstreamCheck(Is_);
  return Is_;
}

/****************************************************************************/
long VarLengthFile::itellg()
{
  long Pos_ = GetByteStreamBrowser()->itellg() - RecordSection();
  IstreamCheck(GetInStream(FALSE));
  return Pos_;
}

/****************************************************************************/
ostream& VarLengthFile::oseekp(long Pos_)
{
  VLFMARKER("start: VarLengthFile::oseekp(long)")

  VLFTRACE(RecordSection())
  VLFTRACE(Pos_)

  if (Pos_ < 0)
    Pos_ = 0;

  ostream& Os_ = (ostream&)(*GetReader());
  _Info->Seekp(Os_, RecordSection() + Pos_);
  
  OstreamCheck(Os_);

  VLFMARKER("end: VarLengthFile::oseekp(long)")
  return Os_;
}

/****************************************************************************/
ostream& VarLengthFile::oseekp(long Pos_, int Dir_)
{
  VLFMARKER("start: VarLengthFile::oseekp(long)")

  if (Pos_ < 0)
    Pos_ = 0;

  VLFTRACE(RecordSection())
  VLFTRACE(Pos_)

  ostream& Os_ = (ostream&)(*GetReader());
  
  if (Dir_ == ios::beg)
    _Info->Seekp(Os_, RecordSection() + Pos_, Dir_);
  else
    _Info->Seekp(Os_, Pos_, Dir_);
  
  OstreamCheck(Os_);

  VLFMARKER("end: VarLengthFile::oseekp(long)")
  return Os_;
}

/****************************************************************************/
ostream& VarLengthFile::owrite(const char* Bytes_, Ulong Max_)
{
  ostream& Os_ = (ostream&)(*GetReader());
  _Info->Write(Os_, Bytes_, Max_);
  OstreamCheck(Os_);  
  return Os_;  
}

/****************************************************************************/
long VarLengthFile::otellp()
{
  long Pos_ = GetByteStreamBrowser()->otellp() - RecordSection();
  OstreamCheck(GetOutStream(FALSE));
  return Pos_;
}

/****************************************************************************/
ByteStream& VarLengthFile::SeekGet(long RecNum_)
{
  if (_Status == VARLENGTHFILE_READY && _Info && IsValidSlot(RecNum_))
  {
    istream& Is_ = (istream&)(*GetReader());  
    _Info->SeekGet(Is_, RecNum_);
  }

  return *this;
}

/****************************************************************************/
ByteStream& VarLengthFile::SeekPut(long RecNum_)
{
  if (_Status == VARLENGTHFILE_READY && _Info && IsValidSlot(RecNum_))
  {
    ostream& Os_ = (ostream&)(*GetReader());
    _Info->SeekPut(Os_, RecNum_);
  }

  return *this;
}

/****************************************************************************/
ByteStream& VarLengthFile::SeekGet(long RecNum_, int SeekDir_)
{
  if (_Status == VARLENGTHFILE_READY && _Info &&
      IsValidSeek(RecNum_, SeekDir_, ios::in))
  {
    istream& Is_ = (istream&)(*GetReader());    
    _Info->SeekGet(Is_, RecNum_, SeekDir_);
  }

  return *this;
}

/****************************************************************************/
ByteStream& VarLengthFile::SeekPut(long RecNum_, int SeekDir_)
{
  if (_Status == VARLENGTHFILE_READY && _Info &&
      IsValidSeek(RecNum_, SeekDir_, ios::out))
  {
    ostream& Os_ = (ostream&)(*GetReader());  
    _Info->SeekPut(Os_, RecNum_, SeekDir_);
  }

  return *this;
}

/****************************************************************************/
long VarLengthFile::TellGet()
{
  return (_Info->TellGet()->_RecordNum);
}

/****************************************************************************/
long VarLengthFile::TellPut()
{
  return (_Info->TellPut()->_RecordNum);
}

/****************************************************************************/
long VarLengthFile::SizeGet()
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

/****************************************************************************/
long VarLengthFile::SizePut()
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
ByteStream& VarLengthFile::AppendRecord()
{
  SeekPut(0, ios::end);
  _Info->PutRec(StorableFormWidth());

  return WriteRecord();
}

/****************************************************************************/
ByteStreamIterator* VarLengthFile::NewByteStreamIterator()
{
  return (new ByteStreamIterator(this, CurrentIOMode()));
}

/****************************************************************************/
/****************************************************************************/
VarLengthFile* VarLengthFile::Make(VarLengthFileInfo* Info_)
{
  return (new VarLengthFile(Info_));
}

/****************************************************************************/
VarLengthFile* VarLengthFile::Make(const File& Finfo_, VarLengthFileInfo* Info_)
{
  return (new VarLengthFile(Finfo_, Info_));
}

/****************************************************************************/
VarLengthFile* VarLengthFile::Make(const Object& Obj_)
{
  VarLengthFileAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& VarLengthFile::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  VarLengthFileAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromVarLengthFile(TrgComp_)));
}

/****************************************************************************/
Object* VarLengthFile::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new VarLengthFile(*this));

  VarLengthFileAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromVarLengthFile(TrgComp_)));
}

/****************************************************************************/
VarLengthFile& VarLengthFile::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static VarLengthFile _NULLOBJECT(VarLengthFileInfo::Make(NullObject::GiveNullObject()));
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& VarLengthFile::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return VarLengthFile::GiveNullObject();
}

/****************************************************************************/
Object& VarLengthFile::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return VarLengthFile::GiveNullObject();
}

/****************************************************************************/
int VarLengthFile::ClassID() const
{
  return TypeInfo::VARLENGTHFILE_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* VarLengthFile::GiveObjectAcceptor() const
{
  return (new VarLengthFileAcceptor(this));
}

/****************************************************************************/
VarLengthFile& VarLengthFile::SetVarLengthFileProperties(VarLengthFileInfo* Info_)
{
  delete _Info;
  _Info = Info_;

  return *this;
}

/****************************************************************************/
VarLengthFile& VarLengthFile::operator = (const VarLengthFile& Sinfo_)
{
  if (this != &Sinfo_)
  {
    delete _Info;
    _Info = (Sinfo_._Info != NULL) ? VarLengthFileInfo::Make(*Sinfo_._Info):NULL;
    _StoreToHeader = Sinfo_._StoreToHeader;

    File::operator = (Sinfo_);
  }

  return *this;
}

/****************************************************************************/
long VarLengthFile::HeaderSection() const
{
  return (_Info ? long(StorableFormWidth() + _Info->FormWidth()):0);
}

/****************************************************************************/
long VarLengthFile::RecordSection() const
{
  return (_Info ? _Info->GiveFileOffset():0);
}

/****************************************************************************/
long VarLengthFile::EndOfFileOffset() const
{
  return (_Info ? long(StorableFormWidth() + _Info->DataSpan()):0);
}

/****************************************************************************/
Boolean VarLengthFile::ReadFileInfo()
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
Boolean VarLengthFile::WriteFileInfo() const
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
Boolean VarLengthFile::Open(const char* Path_, int Modes_)
{
  VarLengthFileInfo CurrInfo_(*_Info);
  Boolean ReadDone_ = FALSE;
  Boolean WriteDone_ = FALSE;
  Boolean Valid_ = FALSE;

  if (File::Open(Path_, Modes_))
  {
    ClearStatus(VARLENGTHFILE_NOT_OPENED);

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
	ClearStatus(VARLENGTHFILE_INFO_NOTREAD);
    }
    else
    {
      Valid_ = ReadDone_;
      if (ReadDone_)
	ClearStatus(VARLENGTHFILE_INFO_NOTREAD);
    }
  }

  Valid_ = Valid_ && InitializeHeader();
  if (Valid_)
    ClearStatus(VARLENGTHFILE_HEADER_NOTREAD);

  return Valid_;
}

/****************************************************************************/
Boolean VarLengthFile::Close()
{
  if (((_Mode & ios::out) || (_Mode & ios::app)) && GetBuffer()->is_open())
  {
    Boolean WriteOK_ = WriteFileInfo();
    return (File::Close() && WriteOK_);
  }

  return File::Close();
}

/****************************************************************************/
Boolean VarLengthFile::TestForVarLengthFile()
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
Boolean VarLengthFile::IsVarLengthFile(const char* Path_) const
{
  VarLengthFile SlotFile_(*this);

  if (Path_)
    SlotFile_.SetFileProperties(Path_, ios::in);
  else
    SlotFile_.SetFileMode(ios::in);

  return SlotFile_.TestForVarLengthFile();
}

/****************************************************************************/
Boolean VarLengthFile::IsValidSeek(long RecNum_, int SeekDir_, int IOop_)
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
Boolean VarLengthFile::IsValidSlot(long SlotNum_, short IncludeEof_) const
{
  long NumSlots_ = SlotsAllocated() + IncludeEof_;
  return (NumSlots_ > 0 && SlotNum_ >= 0 && SlotNum_ < NumSlots_);
}

/****************************************************************************/
Boolean VarLengthFile::IsValidSlot(long SlotNum_) const
{
  return IsValidSlot(SlotNum_, 0);
}

/****************************************************************************/
long VarLengthFile::AddRecord(const Object& Record_)
{
  if (!_Info || _Status != VARLENGTHFILE_READY)
    return -1;

  Record_.WriteObjectWith(*this);

  if (AppendRecord().Good())
  {
    if (_StoreToHeader)
      AppendIDToHeader(Record_.ClassID());

    return (SlotsAllocated() - 1);
  }

  return -1;
}

/****************************************************************************/
long VarLengthFile::AllocateSlot(const Object* Record_)
{
  if (Record_)
    return AddRecord(Record_->NULLOBJECT());
  return AddRecord(*_Info->NullRecord());
}

/****************************************************************************/
Boolean VarLengthFile::DeleteRecord(long SlotNum_)
{
  if (_Info)
    return ModifyRecord(SlotNum_, *_Info->NullRecord());

  return FALSE;
}

/****************************************************************************/
long VarLengthFile::FirstRecord(Object& Record_)
{
  return NextRecord(-1, Record_);
}

/****************************************************************************/
Boolean VarLengthFile::ModifyRecord(long SlotNum_, const Object& Record_)
{
  if (!_Info || SlotNum_ < 0 || _Status != VARLENGTHFILE_READY)
    return FALSE;

  Record_.WriteObjectWith(*this);

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
      if (AllocateSlot(&Record_) == -1)
        break;

    if (SlotsAllocated() > SlotNum_)
      ModifyRecord(SlotNum_, Record_);
  }

  return FALSE;
}

/****************************************************************************/
long VarLengthFile::NextRecord(long CurSlot_, Object& Record_)
{
  ++CurSlot_;
  if (!_Info || CurSlot_ < 0 || _Status != VARLENGTHFILE_READY)
    return -1;

  Record_.ReadObjectWith(*this);
  long SlotNum_;

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
long VarLengthFile::NextRecordID(long CurSlot_, ObjectData& ObjData_)
{
  ++CurSlot_;
  if (!_Info || CurSlot_ < 0 || _Status != VARLENGTHFILE_READY)
    return -1;

  Object* Record_ = NullRecord().Clone();
  Record_->ReadObjectWith(*this);

  long Result_ = -1;
  long SlotNum_;

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
long VarLengthFile::NthRecord(long NthRecNum_, Object& Record_)
{
  long Slot_ = FirstRecord(Record_);
  long Index_;

  for (Index_ = 0; Index_ < NthRecNum_ && Slot_ != -1; ++Index_)
    Slot_ = NextRecord(Slot_, Record_);

  return Slot_;
}

/****************************************************************************/
long VarLengthFile::NthRecordSlot(long NthRecNum_)
{
  Object* ObjPtr_ = NullRecord().Clone();
  NthRecNum_ = NthRecord(NthRecNum_, *ObjPtr_);
  delete ObjPtr_;
  return NthRecNum_;
}

/****************************************************************************/
Boolean VarLengthFile::RetrieveRecord(long SlotNum_, Object& Record_)
{
  if (!_Info || SlotNum_ < 0 || _Status != VARLENGTHFILE_READY)
    return FALSE;

  Record_.ReadObjectWith(*this);

  if (IsValidSlot(SlotNum_))
  {
    SeekGet(SlotNum_);
    if (ReadRecord().Good())
      return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
const Object& VarLengthFile::NullRecord() const
{
  if (!_Info || !_Info->NullRecord())
    THROW (FallibleBase::DerefNullPointerErr());

  return *_Info->NullRecord();
}

/****************************************************************************/
int VarLengthFile::IdentifyRecord(long NthRecNum_)
{
  ObjectData ObjData_;
  long Index_;

  long Slot_ = NextRecordID(-1, ObjData_);
  for (Index_ = 0; Index_ < NthRecNum_ && Slot_ != -1; ++Index_)
    Slot_ = NextRecordID(Slot_, ObjData_);

  return ((Slot_ == -1) ? TypeInfo::NOTYPE:ObjData_._IDvalue);
}

/****************************************************************************/
Boolean VarLengthFile::StoreIDToHeader(Boolean Flag_)
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
	WriteHeader((Byte_t*)HeaderData_, sizeof(long) * HdIndex_);

    delete ObjPtr_;
    ::Delete(HeaderData_);

    return Done_;
  }

  return TRUE;
}

/****************************************************************************/
Boolean VarLengthFile::ReadIDFromHeader(long* HeaderData_) const
{
  return ReadHeader((Byte_t*)HeaderData_, (SlotsAllocated() + 1) * sizeof(long));
}

/****************************************************************************/
void VarLengthFile::AppendIDToHeader(long IDvalue_)
{
  long Hsize_ = SlotsAllocated();
  long* HeaderData_ = New<long>(Hsize_ + 1);
  ReadHeader((Byte_t*)HeaderData_, sizeof(long) * Hsize_);

  HeaderData_[0] = SlotsAllocated();
  HeaderData_[Hsize_++] = IDvalue_;
  WriteHeader((Byte_t*)HeaderData_, sizeof(long) * Hsize_);
  ::Delete(HeaderData_);
}

/****************************************************************************/
void VarLengthFile::ModifyIDInHeader(long SlotNum_, long IDvalue_)
{
  long Hsize_ = SlotsAllocated() + 1;
  long* HeaderData_ = New<long>(Hsize_);
  ReadHeader((Byte_t*)HeaderData_, sizeof(long) * Hsize_);

  HeaderData_[SlotNum_ + 1] = IDvalue_;
  WriteHeader((Byte_t*)HeaderData_, sizeof(long) * Hsize_);
  ::Delete(HeaderData_);
}

/****************************************************************************/
long VarLengthFile::SlotSize() const
{
  if (_Info)
    return _Info->RecordSize();

  return 0;
}

/****************************************************************************/
long VarLengthFile::SlotsAllocated() const
{
  if (_Info)
    return (_Info->HighestRecord() + 1);

  return 0;
}

/****************************************************************************/
long VarLengthFile::HeaderSize() const
{
  if (_Info)
    return _Info->HeaderSize();

  return 0;
}

/****************************************************************************/
Boolean VarLengthFile::ReadHeader(Byte_t* Header_, long Size_) const
{
  istream& Is_ = (istream&)(*GetReader());
  long Hsize_ = (0 < Size_ && Size_ < _Info->HeaderSize()) ?
			Size_:_Info->HeaderSize();

  Is_.seekg(HeaderSection());

  Boolean Good_ = FALSE;
  if (Hsize_)
    Good_ = Is_.read((char*)Header_, Hsize_).good();

  Is_.seekg(RecordSection());
  return Good_;
}

/****************************************************************************/
Boolean VarLengthFile::WriteHeader(const Byte_t* Header_, long Size_)
{
  ostream& Os_ = (ostream&)(*GetReader());
  long Hsize_ = (0 < Size_ && Size_ < _Info->HeaderSize()) ?
			Size_:_Info->HeaderSize();

  Os_.seekp(HeaderSection());

  Boolean Good_ = FALSE;
  if (Hsize_)
    Good_ = Os_.write((char*)Header_, Hsize_).good();

  Os_.seekp(RecordSection());
  return Good_;
}

/****************************************************************************/
Boolean VarLengthFile::InitializeHeader()
{
  Byte_t* Buffer_ = New<Byte_t>(HeaderSize());

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
void* VarLengthFile::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void VarLengthFile::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* VarLengthFile::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void VarLengthFile::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if VARLENGTHFILE_DEBUG
#include "shortint.h"
int main()
{
  char Header_[128];
  StrFill(Header_, '\0', 128);
  strcpy(Header_, "This Is The Header Of The File");

  ChrString Record1_("String #1, Whoxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record2_("String #2, Canxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record3_("String #3, Sayxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record4_("String #4, Onlyxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  ChrString Record5_("String #5, Timex1bbb11xxx12bbb22xxx23bbb33xxx34bbb44xxx45bbb55xxx56bbb66xxx67bbb77xxx78bbb88xxx89bbb99xxx911b1111x1122b2222x2233b3333x33");
  
  ChrString NullRec_;
  ChrString UseRec_;

  VarLengthFile* Sfile_ =
  VarLengthFile::Make(VarLengthFileInfo::Make(NullRec_));
  Sfile_->SetFileProperties("vsample.dat", ios::out|ios::in|ios::binary);

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

  UseRec_ = ShortInt::ToString(ShortInt(21600));
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

  Sfile_->WriteHeader((Byte_t*)Header_);
  Sfile_->ReadHeader((Byte_t*)Header_);
  cout <<endl <<Header_ <<endl;

  cout <<TypeInfo::GiveClassName(Sfile_->IdentifyRecord(Index2_)) <<endl;

  Sfile_->Close();
  cout <<Sfile_->IsVarLengthFile("vsample.dat") <<endl;

  delete Sfile_;
  return 0;
}
#endif
/****************************************************************************/
#endif





