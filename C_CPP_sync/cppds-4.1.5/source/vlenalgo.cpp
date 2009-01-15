// Variable length file algorithm testbed
//
#ifndef VLENALGO_H
  #include "vlenalgo.h"
#endif

/****************************************************************************/
/****************************************************************************/
TestClass::VarLengthFilePosInfo::VarLengthFilePosInfo(TestClass* ParPtr_):
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
TestClass::VarLengthFilePosInfo::VarLengthFilePosInfo(TestClass* ParPtr_, TestClass::VarLengthFilePosInfo* InfoPtr_):
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
TestClass::VarLengthFilePosInfo::~VarLengthFilePosInfo()
{
  _ChunkArray = TestClass::DeletePosArray(_ChunkArray);
}

/****************************************************************************/
void TestClass::VarLengthFilePosInfo::Reset()
{
  _RecordNum =
  _RecordPos =
  _ChunkNum =
  _ChunkPos = 0;
  _RealBytePos =
  _LinearBytePos = _InfoPtr->GiveFileOffset();
  _ChunkArray = TestClass::DeletePosArray(_ChunkArray);
  _ChunkArray = _InfoPtr->GetRecFnc(0);
}

/****************************************************************************/
Boolean TestClass::VarLengthFilePosInfo::SeekRecNum(long RecNum_)
{
  if (RecNum_ >= _InfoPtr->FileCapacity())
    return FALSE;

  Boolean SeekOk_ = TRUE;
  long** PosArr_ = _InfoPtr->GetRecFnc(RecNum_);
  
  if (PosArr_ && *PosArr_)
  {
    _ChunkArray = TestClass::DeletePosArray(_ChunkArray);
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
Boolean TestClass::VarLengthFilePosInfo::SeekFilePos(long Pos_)
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
    _ChunkArray = TestClass::DeletePosArray(_ChunkArray);
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
Boolean TestClass::VarLengthFilePosInfo::SeekFilePos(long Pos_, ios::seek_dir Dir_)
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
Boolean TestClass::VarLengthFilePosInfo::UpdateFilePos()
{
  if (_RecordNum >= _InfoPtr->FileCapacity())
    return FALSE;

  Boolean SeekOk_ = TRUE;
  long** PosArr_ = _InfoPtr->GetRecFnc(_RecordNum);
  
  if (PosArr_ && *PosArr_)
  {
    _ChunkArray = TestClass::DeletePosArray(_ChunkArray);
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
long TestClass::VarLengthFilePosInfo::Read(istream& Is_, char* Buffer_, long Max_)
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
long TestClass::VarLengthFilePosInfo::Write(ostream& Os_, const char* Buffer_, long Max_)
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
/****************************************************************************/
TestClass::TestClass(long ChunkSize_, long InitialRun_,
                     long Allocated_, long HeaderSize_):
_ChunkSize(ChunkSize_),
_InitialRun(InitialRun_),
_NullBlock((char*)memset(new char[ChunkSize_], 0, ChunkSize_)),
_HeaderSize(Allocated_ ? ((Allocated_ + 1) * sizeof(long)):HeaderSize_),
_HighestRecord(0),
_RecordCount(0),
_FileSize(0),
_Index(2),
_InputPos(NULL),
_OutputPos(NULL),
_FileBase(FormWidth() + HeaderSize()),
_FileOffset(0)
{
  _FileConfig[0] = _ChunkSize;
  _FileConfig[1] = _InitialRun;
  _FileConfig[2] = 1;

  _InputPos = new VarLengthFilePosInfo(this);
  _OutputPos = new VarLengthFilePosInfo(this);
}

/****************************************************************************/
TestClass::TestClass(const TestClass& Obj_):
_ChunkSize(Obj_._ChunkSize),
_InitialRun(Obj_._InitialRun),
_NullBlock((char*)memset(new char[Obj_._ChunkSize], 0, Obj_._ChunkSize)),
_HeaderSize(Obj_._HeaderSize),
_HighestRecord(Obj_._HighestRecord),
_RecordCount(Obj_._RecordCount),
_FileSize(Obj_._FileSize),
_Index(Obj_._Index),
_InputPos(NULL),
_OutputPos(NULL),
_FileBase(FormWidth() + HeaderSize()),
_FileOffset(0)
{
  memmove(_FileConfig, Obj_._FileConfig, sizeof(long) * MAXCONFIG);

  _InputPos = new VarLengthFilePosInfo(this, Obj_._InputPos);
  _OutputPos = new VarLengthFilePosInfo(this, Obj_._OutputPos);
}

/****************************************************************************/
TestClass::~TestClass()
{
  delete[] _NullBlock;
  _NullBlock = NULL;
}

/****************************************************************************/
TestClass* TestClass::Make(const TestClass& Finfo_)
{
  return (new TestClass(Finfo_));
}

/****************************************************************************/
TestClass* TestClass::Make(long ChunkSize_, long InitialRun_,
                           long Allocated_, long HeaderSize_)
{
  return (new TestClass(ChunkSize_, InitialRun_, Allocated_, HeaderSize_));
}

/****************************************************************************/
istream& TestClass::ReadFileInfo(istream& Is_)
{
  size_t i;
  long Value_;
  long PrevPos_ = Is_.tellg();

  Is_.seekg(_FileOffset);
  Is_.read((Byte_t*)(&_ChunkSize), sizeof(long));
  Is_.read((Byte_t*)(&_InitialRun), sizeof(long));

  for (i = 0; i < MAXCONFIG; i++)
  {
    Is_.read((Byte_t*)(&Value_), sizeof(long));
    _FileConfig[i] = Value_;
  }

  Is_.read((Byte_t*)(&_HeaderSize), sizeof(long));
  Is_.read((Byte_t*)(&_HighestRecord), sizeof(long));
  Is_.read((Byte_t*)(&_RecordCount), sizeof(long));
  Is_.read((Byte_t*)(&_FileSize), sizeof(long));
  Is_.read((Byte_t*)(&_Index), sizeof(long));
  Is_.seekg(PrevPos_);
  
  _InputPos->Reset();
  _OutputPos->Reset();
  
  return Is_;
}

/****************************************************************************/
ostream& TestClass::WriteFileInfo(ostream& Os_) const
{
  size_t i;
  long Value_;
  long PrevPos_ = Os_.tellp();
  
  Os_.seekp(_FileOffset);
  Os_.write((Byte_t*)(&_ChunkSize), sizeof(long));
  Os_.write((Byte_t*)(&_InitialRun), sizeof(long));

  for (i = 0; i < MAXCONFIG; i++)
  {
    Value_ = _FileConfig[i];
    Os_.write((Byte_t*)(&Value_), sizeof(long));
  }

  Os_.write((Byte_t*)(&_HeaderSize), sizeof(long));
  Os_.write((Byte_t*)(&_HighestRecord), sizeof(long));
  Os_.write((Byte_t*)(&_RecordCount), sizeof(long));
  Os_.write((Byte_t*)(&_FileSize), sizeof(long));
  Os_.write((Byte_t*)(&_Index), sizeof(long));
  Os_.seekp(PrevPos_);
  
  return Os_;
}

/****************************************************************************/
TestClass* TestClass::SetHeaderSize(long Size_)
{
  _HeaderSize = Size_;
  _FileBase = FormWidth() + Size_;
  return this;
}

/****************************************************************************/
TestClass* TestClass::SetFileOffset(long Offset_)
{
  _FileOffset = Offset_;
  return this;
}

/****************************************************************************/
TestClass* TestClass::AllocateSlots(long NewSlots_)
{
  if (NewSlots_)
    PutRec(HighestRecord() + NewSlots_);
    
  return this;
}

/****************************************************************************/
Boolean TestClass::operator == (TestClass& Info_)
{
  size_t i;
  Boolean IsEqualFileConfig_ = TRUE;

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
    Info_._HeaderSize == _HeaderSize &&
    IsEqualFileConfig_ &&
    Info_._HighestRecord == _HighestRecord &&
    Info_._RecordCount == _RecordCount &&
    Info_._Index == _Index
  );
}

/****************************************************************************/
long** TestClass::DeletePosArray(long** PosArray_)
{
  long** StartPtr_ = PosArray_;
  
  if (PosArray_)
    for (;*PosArray_; PosArray_++)
      delete *PosArray_;
      
  delete[] StartPtr_;      
  return NULL;
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::SeekGet(istream& Is_, long Pos_)
{
  Boolean Done_;
  if (Done_ = _InputPos->SeekRecNum(Pos_))
    Is_.seekg(_InputPos->_RealBytePos);      

  return (Done_ ? _InputPos:NULL);
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::SeekGet(istream& Is_, long Pos_, ios::seek_dir Dir_)
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
TestClass::VarLengthFilePosInfo* TestClass::Seekg(istream& Is_, long Pos_)
{
  TestClass::VarLengthFilePosInfo* RetPtr_ = (_InputPos->SeekFilePos(Pos_) ? _InputPos:NULL);
  Is_.seekg(_InputPos->_RealBytePos);
  return RetPtr_;
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::Seekg(istream& Is_, long Pos_, ios::seek_dir Dir_)
{
  TestClass::VarLengthFilePosInfo* RetPtr_ = (_InputPos->SeekFilePos(Pos_, Dir_) ? _InputPos:NULL);
  Is_.seekg(_InputPos->_RealBytePos);
  return RetPtr_;
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::TellGet()
{
  return _InputPos;
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::SeekPut(ostream& Os_, long Pos_)
{
  Boolean Done_;
  if (Done_ = _OutputPos->SeekRecNum(Pos_))
    Os_.seekp(_OutputPos->_RealBytePos);

  return (Done_ ? _OutputPos:NULL);
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::SeekPut(ostream& Os_, long Pos_, ios::seek_dir Dir_)
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
TestClass::VarLengthFilePosInfo* TestClass::Seekp(ostream& Os_, long Pos_)
{
  TestClass::VarLengthFilePosInfo* RetPtr_ = (_OutputPos->SeekFilePos(Pos_) ? _OutputPos:NULL);
  Os_.seekp(_OutputPos->_RealBytePos);
  return RetPtr_;
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::Seekp(ostream& Os_, long Pos_, ios::seek_dir Dir_)
{
  TestClass::VarLengthFilePosInfo* RetPtr_ = (_OutputPos->SeekFilePos(Pos_, Dir_) ? _OutputPos:NULL);
  Os_.seekp(_OutputPos->_RealBytePos);  
  return RetPtr_;
}

/****************************************************************************/
TestClass::VarLengthFilePosInfo* TestClass::TellPut()
{
  return _OutputPos;
}

/****************************************************************************/
long TestClass::Read(istream& Is_, char* Buffer_, long Max_)
{
  Boolean HigherRecord_ = _InputPos->_RecordNum > HighestRecord();
  Boolean GreaterSize_ = Max_ > RecordSize();

  if (HigherRecord_ || GreaterSize_)
    return 0;

  return _InputPos->Read(Is_, Buffer_, Max_);
}

/****************************************************************************/
long TestClass::Write(ostream& Os_, const char* Buffer_, long Max_)
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
long TestClass::ReadRec(istream& Is_, char* Buffer_, long Max_)
{
  long BytesRead_ = Read(Is_, Buffer_, Max_);
  SeekGet(Is_, _InputPos->_RecordNum + 1);
  return BytesRead_;
}

/****************************************************************************/    
long TestClass::WriteRec(ostream& Os_, const char* Buffer_, long Max_)
{
  long BytesWritten_ = Write(Os_, Buffer_, Max_);
  SeekPut(Os_, _OutputPos->_RecordNum + 1);
  return BytesWritten_;
}

/****************************************************************************/
long TestClass::UpdateFileSize(ostream& Os_)
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
      char* Leftover_ = (char*)memset(new char[Remain_], 0, Remain_);
      Os_.write(Leftover_, Remain_);
      delete[] Leftover_;
    }

    Os_.seekp(OldPos_);
    return Diff_;
  }

  return 0;
}

/****************************************************************************/
long TestClass::FileSize(istream& Is_)
{
  long OldPos_ = Is_.tellg();
  Is_.seekg(0, ios::end);
  long EofPos_ = Is_.tellg();
  EofPos_ -= GiveFileOffset();
  Is_.seekg(OldPos_);
  return EofPos_;
}

/****************************************************************************/
long TestClass::FileSize(ostream& Os_)
{
  long OldPos_ = Os_.tellp();
  Os_.seekp(0, ios::end);
  long EofPos_ = Os_.tellp();
  EofPos_ -= GiveFileOffset();
  Os_.seekp(OldPos_);
  return EofPos_;
}

/****************************************************************************/
long TestClass::Pow2(size_t Exp_) const
{
  long Val_ = 1;
  size_t Index_;
  
  for (Index_ = 0; Index_ < Exp_; ++Index_)
    Val_ *= 2;

  return Val_;
}

/****************************************************************************/
long TestClass::FileCapacity() const
{
  size_t Index_;
  long Capacity_ = 1;
  
  for (Index_ = 1; Index_ <= _Index; ++Index_)
    Capacity_ *= _FileConfig[Index_];

  return Capacity_;
}

/****************************************************************************/
long TestClass::SegmentsPerPage(size_t Exp_) const
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
long TestClass::RecordNumber(long RecNum_, size_t Exp_) const
{
  return
  (
    (Exp_ == Exponent()) ?
      RecNum_:
      (RecordNumber(RecNum_, Exp_ + 1) % (SegmentsPerPage(Exp_ + 1) / NestingValue(Exp_ + 1)))
  );
}

/****************************************************************************/
long TestClass::PageSeek(long RecNum_, size_t Exp_) const
{
  return (PageNumber(RecNum_, Exp_) * SegmentsPerPage(Exp_));
}

/****************************************************************************/
long TestClass::ClusterSeek(size_t Exp_, size_t Index_) const
{
  Index_ %= Pow2(Exp_);
  Index_ /= Pow2(Exp_ - 1);
  long cs_ = 0;
  
  if (Index_)
    cs_ = SegmentsPerPage(Exp_) / 2;

  return cs_;
}

/****************************************************************************/
long TestClass::RecordSeek(long RecNum_) const
{
  return (RecordNumber(RecNum_, 0) - (PageNumber(RecNum_, 0) * SegmentsPerPage(0)));
}

/****************************************************************************/
long TestClass::FindStartPosHelper(long RecNum_, size_t Index_, size_t Exp_)
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
long** TestClass::GetRecFnc(long RecNum_)
{
  long Index_;
  long Limit_ = _FileSize ? (HighestRecord() + 1):0;
  
  if (RecNum_ > Limit_ || RecNum_ >= FileCapacity())
    return NULL;

  size_t Exp_ = Exponent();
  Limit_ = Pow2(Exp_);
  long** StartPos_ = new long*[Limit_ + 1];
  
  for (Index_ = 0; Index_ < Limit_; ++Index_)
  {
    StartPos_[Index_] = new long;
    *StartPos_[Index_] = FindStartPos(RecNum_, Index_);
  }

  StartPos_[Index_] = NULL;
  return AddFileOffset(StartPos_);
}

/****************************************************************************/
long** TestClass::GetRec(long RecNum_)
{
  if (RecNum_ > HighestRecord())
    return NULL;

  return GetRecFnc(RecNum_);
}

/****************************************************************************/
void TestClass::AddPage()
{
  ++_FileConfig[_Index];
}

/****************************************************************************/
void TestClass::IncreaseRecSize()
{
  _FileConfig[++_Index] = 1;
}

/****************************************************************************/
long TestClass::PutRec(long RecNum_, long Size_)
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
long** TestClass::AddFileOffset(long** PosArray_)
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
long** TestClass::MinusFileOffset(long** PosArray_)
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
/*
#if OVERLOAD_NEW
void* operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
/*
void operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
/*
#if	HAS_ARRAY_NEW
void* operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
/*
void operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
void ShowPositions(long** Arr_)
{
  if (Arr_)
    while (*Arr_)
    {
      cout <<**Arr_ <<" ";
      ++Arr_;
    }

  cout <<endl;
}

/****************************************************************************/
#if VLENALGO_DEBUG
class SampleStr
{
  private:  
    long _Max;
    char* _Str;

  public:
    SampleStr(size_t Len_=0):
      _Max(Len_),
      _Str(Len_ ? (char*)memset(new char[Len_ + 1], 'x', Len_):NULL)
      { if (_Str)
          _Str[Len_] = 0; }
    SampleStr(char* Str_):
      _Max(Str_ ? strlen(Str_):0),
      _Str(strcpy(new char[strlen(Str_) + 1], Str_)) {}
    SampleStr(const SampleStr& Str_):
      _Max(Str_._Max),
      _Str(strcpy(new char[Str_._Max + 1], Str_._Str)) {}
    ~SampleStr()
    { delete[] _Str; }
    void SetString(char* Str_)
    { delete[] _Str;
      _Str = strcpy(new char[strlen(Str_) + 1], Str_);
      if (_Str) _Max = strlen(Str_); }
    const char* GetString() const
    { return _Str; }
    long StrLen() const
    { return _Max; }
    SampleStr& operator = (const SampleStr& Str_)
    { if (this != &Str_)
      { delete[] _Str;
        _Max = Str_._Max;
        _Str = strcpy(new char[_Max + 1], Str_._Str); }
      return *this; }
    long SizeOf() const
    { return (sizeof(long) + strlen(_Str) + 1); }
    void CrossOut()
    { size_t Len_ = strlen(_Str);
      _Str = (char*)memset(_Str, 'x', Len_); }
};

void main()
{
  long Diff_, OldSize_;
  long** PosArr_;
  TestClass* test_ = TestClass::Make();

#if VLENALGO_TEST1
  OldSize_ = test_->FileSize();
  Diff_ = test_->PutRec(0, 64);
  PosArr_ = test_->GetRec(0);
  ShowPositions(PosArr_);
  cout <<"Diff = " <<Diff_ <<", OldFileSize = " <<OldSize_ <<endl;
  PosArr_ = TestClass::DeletePosArray(PosArr_);  

  OldSize_ = test_->FileSize();  
  Diff_ = test_->PutRec(4, 64);
  PosArr_ = test_->GetRec(4);
  ShowPositions(PosArr_);
  cout <<"Diff = " <<Diff_ <<", OldFileSize = " <<OldSize_ <<endl;
  PosArr_ = TestClass::DeletePosArray(PosArr_);

  OldSize_ = test_->FileSize();
  Diff_ = test_->PutRec(5, 120);
  PosArr_ = test_->GetRec(5);
  ShowPositions(PosArr_);
  cout <<"Diff = " <<Diff_ <<", OldFileSize = " <<OldSize_ <<endl;  
  PosArr_ = TestClass::DeletePosArray(PosArr_);

  OldSize_ = test_->FileSize();
  Diff_ = test_->PutRec(10, 100);
  PosArr_ = test_->GetRec(10);
  ShowPositions(PosArr_);
  cout <<"Diff = " <<Diff_ <<", OldFileSize = " <<OldSize_ <<endl;  
  PosArr_ = TestClass::DeletePosArray(PosArr_);

  OldSize_ = test_->FileSize();
  Diff_ = test_->PutRec(13, 250);
  PosArr_ = test_->GetRec(13);
  ShowPositions(PosArr_);
  cout <<"Diff = " <<Diff_ <<", OldFileSize = " <<OldSize_ <<endl;  
  PosArr_ = TestClass::DeletePosArray(PosArr_);

  OldSize_ = test_->FileSize();
  Diff_ = test_->PutRec(27, 250);
  PosArr_ = test_->GetRec(27);
  ShowPositions(PosArr_);
  cout <<"Diff = " <<Diff_ <<", OldFileSize = " <<OldSize_ <<endl;  
  PosArr_ = TestClass::DeletePosArray(PosArr_);
#endif

#if VLENALGO_TEST2
  RANDOMIZE();

  SampleStr Str1_("String #1, Whoxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str2_("String #2, Canxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str3_("String #3, Sayxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str4_("String #4, Onlyxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str5_("String #5, Timex1bbb11xxx12bbb22xxx23bbb33xxx34bbb44xxx45bbb55xxx56bbb66xxx67bbb77xxx78bbb88xxx89bbb99xxx911b1111x1122b2222x2233b3333x33");
  
  SampleStr Str6_("String #1, Sixxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str7_("String #2, Sevenxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str8_("String #3, Eightxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str9_("String #4, Ninexbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");
  SampleStr Str10_("String #5, Tenxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx");

  fstream File_("sample.dat", ios::in | ios::out | ios::binary);
  test_->WriteFileInfo(File_);
  File_.close();
  File_.open("sample.dat", ios::in | ios::out | ios::binary);
  test_->ReadFileInfo(File_);

  long Sz_ = Str1_.StrLen();
  test_->PutRec(0, Str1_.SizeOf() - Str1_.StrLen());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Str1_.GetString(), Str1_.StrLen() + 1);
  Sz_ = Str2_.StrLen();
  test_->PutRec(1, Str2_.SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));  
  test_->WriteRec(File_, Str2_.GetString(), Str2_.StrLen() + 1);
  Sz_ = Str3_.StrLen();
  test_->PutRec(2, Str3_.SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));  
  test_->WriteRec(File_, Str3_.GetString(), Str3_.StrLen() + 1);
  Sz_ = Str4_.StrLen();
  test_->PutRec(3, Str4_.SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));  
  test_->WriteRec(File_, Str4_.GetString(), Str4_.StrLen() + 1);
  Sz_ = Str5_.StrLen();
  test_->PutRec(4, Str5_.SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));  
  test_->WriteRec(File_, Str5_.GetString(), Str5_.StrLen() + 1);
  Str1_.CrossOut();
  Str2_.CrossOut();
  Str3_.CrossOut();
  Str4_.CrossOut();
  Str5_.CrossOut();
  
  char Buffer_[256];
  test_->SeekGet(File_, 3);

  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Str4_.StrLen() + 1);
  Str4_.SetString(Buffer_);
  cout <<"str4: " <<Sz_ <<"  " <<Str4_.GetString() <<endl;
  assert(!strcmp(Str4_.GetString(), "String #4, Onlyxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx"));

  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Str5_.StrLen() + 1);
  Str5_.SetString(Buffer_);
  cout <<"str5: " <<Sz_ <<"  " <<Str5_.GetString() <<endl;
  assert(!strcmp(Str5_.GetString(), "String #5, Timex1bbb11xxx12bbb22xxx23bbb33xxx34bbb44xxx45bbb55xxx56bbb66xxx67bbb77xxx78bbb88xxx89bbb99xxx911b1111x1122b2222x2233b3333x33"));
  
  test_->SeekGet(File_, 0);

  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Str1_.StrLen() + 1);
  Str1_.SetString(Buffer_);
  cout <<"str1: " <<Sz_ <<"  " <<Str1_.GetString() <<endl;
  assert(!strcmp(Str1_.GetString(), "String #1, Whoxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx"));

  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Str2_.StrLen() + 1);
  Str2_.SetString(Buffer_);
  cout <<"str2: " <<Sz_ <<"  " <<Str2_.GetString() <<endl;
  assert(!strcmp(Str2_.GetString(), "String #2, Canxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx"));

  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Str3_.StrLen() + 1);
  Str3_.SetString(Buffer_);
  cout <<"str3: " <<Sz_ <<"  " <<Str3_.GetString() <<endl;
  assert(!strcmp(Str3_.GetString(), "String #3, Sayxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxxbbbbbxxxxx"));

  test_->Seekg(File_, -150, ios::end);
  test_->ReadRec(File_, Buffer_, 30);
  Buffer_[30] = 0;
  cout <<Buffer_ <<endl;
  assert(!strcmp(Buffer_, "xxx911b1111x1122b2222x2233b333"));

  test_->Seekg(File_, test_->GiveFileOffset() + 11);
  test_->ReadRec(File_, Buffer_, 34);
  Buffer_[34] = 0;
  cout <<Buffer_ <<endl;
  assert(!strcmp(Buffer_, "#1, Whoxxbbbbbxxxxxbbbbbxxxxxbbbbb"));

  cout <<"chunk size: " <<test_->ChunkSize() <<endl;
  cout <<"initial run: " <<test_->InitialRun() <<endl;
  cout <<"header size: " <<test_->HeaderSize() <<endl;
  cout <<"highest record: " <<test_->HighestRecord() <<endl;
  cout <<"file size: " <<test_->FileSize() <<endl;

  test_->WriteFileInfo(File_);
  File_.close();
  File_.open("sample.dat", ios::in | ios::out | ios::binary);    
  test_->ReadFileInfo(File_);

  cout <<"chunk size: " <<test_->ChunkSize() <<endl;
  cout <<"initial run: " <<test_->InitialRun() <<endl;
  cout <<"header size: " <<test_->HeaderSize() <<endl;
  cout <<"highest record: " <<test_->HighestRecord() <<endl;
  cout <<"file size: " <<test_->FileSize() <<endl;

  SampleStr** Vect_ = new SampleStr*[10];
  SampleStr** Sel_ = new SampleStr*[10];
  Sel_[0] = &Str1_;
  Sel_[1] = &Str2_;
  Sel_[2] = &Str3_;
  Sel_[3] = &Str4_;
  Sel_[4] = &Str5_;  
  Sel_[5] = &Str6_;
  Sel_[6] = &Str7_;
  Sel_[7] = &Str8_;
  Sel_[8] = &Str9_;
  Sel_[9] = &Str10_;
  
  int i, x;
  for (i = 0; i < 10; i++)
    Vect_[i] = NULL;

  for (i = 0; i < 10; i++)
  {
    while (Vect_[x = RANDOM(10)]);
    Vect_[x] = Sel_[i];
  }
    
  test_->SeekPut(File_, 0);  
  Sz_ = Vect_[0]->StrLen();
  test_->PutRec(0, Vect_[0]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[0]->GetString(), Vect_[0]->StrLen() + 1);
  Sz_ = Vect_[1]->StrLen();
  test_->PutRec(0, Vect_[1]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[1]->GetString(), Vect_[1]->StrLen() + 1);  
  Sz_ = Vect_[2]->StrLen();
  test_->PutRec(0, Vect_[2]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[2]->GetString(), Vect_[2]->StrLen() + 1);
  Sz_ = Vect_[3]->StrLen();
  test_->PutRec(0, Vect_[3]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[3]->GetString(), Vect_[3]->StrLen() + 1);
  Sz_ = Vect_[4]->StrLen();
  test_->PutRec(0, Vect_[4]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[4]->GetString(), Vect_[4]->StrLen() + 1);
  Sz_ = Vect_[5]->StrLen();
  test_->PutRec(0, Vect_[5]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[5]->GetString(), Vect_[5]->StrLen() + 1);
  Sz_ = Vect_[6]->StrLen();
  test_->PutRec(0, Vect_[6]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[6]->GetString(), Vect_[6]->StrLen() + 1);
  Sz_ = Vect_[7]->StrLen();
  test_->PutRec(0, Vect_[7]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[7]->GetString(), Vect_[7]->StrLen() + 1);
  Sz_ = Vect_[8]->StrLen();
  test_->PutRec(0, Vect_[8]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[8]->GetString(), Vect_[8]->StrLen() + 1);
  Sz_ = Vect_[9]->StrLen();
  test_->PutRec(0, Vect_[9]->SizeOf());
  test_->Write(File_, (char*)(&Sz_), sizeof(long));
  test_->WriteRec(File_, Vect_[9]->GetString(), Vect_[9]->StrLen() + 1);

  test_->SeekGet(File_, 0);
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[0]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[0]->GetString()));
  assert(Sz_ == Vect_[0]->StrLen());  
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[1]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[1]->GetString()));
  assert(Sz_ == Vect_[1]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[2]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[2]->GetString()));
  assert(Sz_ == Vect_[2]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[3]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[3]->GetString()));
  assert(Sz_ == Vect_[3]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[4]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[4]->GetString()));
  assert(Sz_ == Vect_[4]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[5]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[5]->GetString()));
  assert(Sz_ == Vect_[5]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[6]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[6]->GetString()));
  assert(Sz_ == Vect_[6]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[7]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[7]->GetString()));
  assert(Sz_ == Vect_[7]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[8]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[8]->GetString()));
  assert(Sz_ == Vect_[8]->StrLen());
  test_->Read(File_, (char*)(&Sz_), sizeof(long));
  test_->ReadRec(File_, Buffer_, Vect_[9]->StrLen() + 1);
  assert(!strcmp(Buffer_, Vect_[9]->GetString()));
  assert(Sz_ == Vect_[9]->StrLen());

  delete[] Vect_;
  delete[] Sel_;  
#endif

  delete test_;
}
#endif
