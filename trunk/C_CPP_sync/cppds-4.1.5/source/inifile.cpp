#ifndef INIFILE_CPP
#define INIFILE_CPP
#ifndef INIFILE_H
  #include "inifile.h"
#endif

/****************************************************************************/
/****************************************************************************/
IniEntry::IniEntry():
_Key(NULL),
_Value(NULL),
_Deleted(FALSE),
_Next(NULL)
{}

/****************************************************************************/
IniEntry::IniEntry(const char* Key_, const char* Val_):
_Key(new_char_string(Key_)),
_Value(new_char_string(Val_)),
_Deleted(FALSE),
_Next(NULL)
{}

/****************************************************************************/
IniEntry::IniEntry(const IniEntry& Obj_):
_Key(new_char_string(Obj_._Key)),
_Value(new_char_string(Obj_._Value)),
_Deleted(Obj_._Deleted),
_Next(Obj_._Next ? IniEntry::Make(*Obj_._Next):NULL)
{}

/****************************************************************************/    
IniEntry::~IniEntry()
{
  ::DeleteArray(_Key);
  ::DeleteArray(_Value);
  _Key = _Value = NULL;
  
  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
IniEntry* IniEntry::Make()
{
  return (new IniEntry());
}

/****************************************************************************/    
IniEntry* IniEntry::Make(const char* Key_, const char* Val_)
{
  return (new IniEntry(Key_, Val_));
}

/****************************************************************************/    
IniEntry* IniEntry::Make(const IniEntry& Obj_)
{
  return (new IniEntry(Obj_));
}

/****************************************************************************/
Boolean IniEntry::WriteTo(FileReader* FileRead_)
{
  Boolean Valid_;

  if (Valid_ = (FileRead_->Write(_Key)).Good())
    if (Valid_ = (FileRead_->Write("=")).Good())
      Valid_ = (FileRead_->WriteLine(_Value)).Good();

  if (Valid_ && _Next)
    return _Next->WriteTo(FileRead_);

  return Valid_;
}

/****************************************************************************/
IniEntry& IniEntry::operator = (const IniEntry& Obj_)
{
  if (this != &Obj_)
  {
    ReplaceEntry(Obj_);
    delete _Next;
    _Next = Obj_._Next ? IniEntry::Make(*Obj_._Next):NULL;    
  }

  return *this;
}

/****************************************************************************/
IniEntry& IniEntry::ReplaceEntry(const IniEntry& Obj_)
{
  if (this != &Obj_)
  {
    ::DeleteArray(_Key);
    ::DeleteArray(_Value);
    _Key = new_char_string(Obj_._Key);
    _Value = new_char_string(Obj_._Value);
  }

  return *this;
}

/****************************************************************************/
IniEntry& IniEntry::SetDeleted(Boolean Flag_)
{
  _Deleted = Flag_;
  return *this;
}

/****************************************************************************/
IniEntry& IniEntry::SetKey(const char* Key_)
{
  ::DeleteArray(_Key);
  _Key = new_char_string(Key_);
  return *this;
}

/****************************************************************************/
IniEntry& IniEntry::SetValue(const char* Val_)
{
  ::DeleteArray(_Value);
  _Value = new_char_string(Val_);
  return *this;
}

/****************************************************************************/    
IniEntry* IniEntry::SetNext(IniEntry* Next_)
{
  _Next = Next_;
  return this;
}

/****************************************************************************/
IniEntry* IniEntry::Append(IniEntry* Next_)
{
  if (_Next)
    _Next->Append(Next_);
  else
    _Next = Next_;

  return this;
}

/****************************************************************************/
const char* IniEntry::Key() const
{
  return _Key;
}

/****************************************************************************/    
const char* IniEntry::Value() const
{
  return _Value;
}

/****************************************************************************/
const IniEntry* IniEntry::Next(long Num_) const
{
  if (Num_ == 0)
    return this;

  IniEntry* Next_ = _Next;
  long x;
  
  for (x = 1; x < Num_; x++)
    Next_ = Next_->_Next;

  return Next_;
}

/****************************************************************************/
IniEntry* IniEntry::Next(long Num_)
{
  IniEntry* Next_ = this;
  long x;
  
  for (x = 0; x < Num_; x++)
    Next_ = Next_->_Next;

  return Next_;
}

/****************************************************************************/
const IniEntry* IniEntry::Last() const
{
  if (_Next)
    return _Next->Last();

  return this;
}

/****************************************************************************/
IniEntry* IniEntry::Last()
{
  if (_Next)
    return _Next->Last();

  return this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* IniEntry::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void IniEntry::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif

/****************************************************************************/
/****************************************************************************/
IniRecord::IniRecord():
_SectionName(NULL),
_NumEntries(0),
_Deleted(FALSE),
_Entry(NULL),
_Next(NULL)
{}

/****************************************************************************/
IniRecord::IniRecord(const char* Name_):
_SectionName(new_char_string(Name_)),
_NumEntries(0),
_Deleted(FALSE),
_Entry(NULL),
_Next(NULL)
{}

/****************************************************************************/
IniRecord::IniRecord(const IniRecord& Obj_):
_SectionName(new_char_string(Obj_._SectionName)),
_NumEntries(Obj_._NumEntries),
_Deleted(Obj_._Deleted),
_Entry(Obj_._Entry ? IniEntry::Make(*Obj_._Entry):NULL),
_Next(Obj_._Next ? IniRecord::Make(*Obj_._Next):NULL)
{}

/****************************************************************************/
IniRecord::~IniRecord()
{
  ::DeleteArray(_SectionName);
  _SectionName = NULL;
  
  delete _Entry;
  _Entry = NULL;

  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
IniRecord* IniRecord::Make()
{
  return (new IniRecord());
}

/****************************************************************************/
IniRecord* IniRecord::Make(const IniRecord& Obj_)
{
  return (new IniRecord(Obj_));
}

/****************************************************************************/
IniRecord* IniRecord::Make(const char* Name_)
{
  return (new IniRecord(Name_));
}

/****************************************************************************/
Boolean IniRecord::UpdateList()
{
  long Index_ = 0;
  Boolean Done_ = TRUE;
  IniEntry* RecPtr_ = _Entry;

  while (RecPtr_ && Done_ && Index_ < _NumEntries)
    if (RecPtr_->IsDeleted())
      RecPtr_ = ReallyDeleteEntry(Index_, Done_);
    else
    {
      RecPtr_ = RecPtr_->Next();
      ++Index_;      
    }

  return Done_;
}

/****************************************************************************/
Boolean IniRecord::WriteTo(FileReader* FileRead_)
{
  Boolean Valid_;

  UpdateList();
  if (Valid_ = (FileRead_->Write("[")).Good())
    if (Valid_ = (FileRead_->Write(_SectionName)).Good())
      if (Valid_ = (FileRead_->WriteLine("]")).Good())
        if (_Entry && _NumEntries)
          Valid_ = _Entry->WriteTo(FileRead_);

  if (Valid_ && _Next)    
    if (Valid_ = (FileRead_->WriteLine()).Good())
      return _Next->WriteTo(FileRead_);

  return Valid_;
}

/****************************************************************************/
IniRecord& IniRecord::operator = (const IniRecord& Obj_)
{
  if (this != &Obj_)
  {
    ReplaceRecord(Obj_);
    delete _Next;
    _Next = Obj_._Next ? IniRecord::Make(*Obj_._Next):NULL;
  }
  
  return *this;
}

/****************************************************************************/
IniRecord& IniRecord::ReplaceRecord(const IniRecord& Obj_)
{
  if (this != &Obj_)
  {
    ::DeleteArray(_SectionName);
    _SectionName = new_char_string(Obj_._SectionName);
    _NumEntries = Obj_._NumEntries;

    delete _Entry;
    _Entry = Obj_._Entry ? IniEntry::Make(*Obj_._Entry):NULL;
  }
  
  return *this;
}

/****************************************************************************/
void IniRecord::SetName(char* Name_)
{
  ::DeleteArray(_SectionName);
  _SectionName = new_char_string(Name_);
}

/****************************************************************************/
const char* IniRecord::GetName() const
{
  return _SectionName;
}

/****************************************************************************/
void IniRecord::AddEntry(IniEntry* Entry_)
{
  if (_Entry)
    _Entry->Append(Entry_);
  else
    _Entry = Entry_;

  ++_NumEntries;
}

/****************************************************************************/
IniEntry* IniRecord::ReallyDeleteEntry(long SlotNum_, Boolean& Done_)
{
  if (SlotNum_ < 0 || SlotNum_ >= _NumEntries || !_NumEntries)
  {
    Done_ = FALSE;
    return NULL;
  }

  IniEntry* PrevPtr_ = NULL;
  IniEntry* NextPtr_ = NULL;
  IniEntry* RecPtr_ = _Entry;

  if (RecPtr_ && _NumEntries)
  {
    if (SlotNum_)
    {
      long Index_ = SlotNum_ - 1;
      PrevPtr_ = Index_ ? RecPtr_->Next(Index_):RecPtr_;
      RecPtr_ = RecPtr_->Next(SlotNum_);
    }

    if (!PrevPtr_)
      _Entry = RecPtr_->Next();
    else
      PrevPtr_->SetNext(RecPtr_->Next());

    NextPtr_ = RecPtr_->Next();
    RecPtr_->SetNext(NULL);
    delete RecPtr_;
    --_NumEntries;

    Done_ = TRUE;
    return NextPtr_;
  }

  Done_ = FALSE;
  return NULL;
}

/****************************************************************************/
Boolean IniRecord::DeleteEntry(long SlotNum_)
{
  if (SlotNum_ < 0 || SlotNum_ >= _NumEntries || !_NumEntries)
    return FALSE;

  if (_Entry)
  {
    IniEntry* RecPtr_ = _Entry;
    if (SlotNum_)
      RecPtr_ = RecPtr_->Next(SlotNum_);

    RecPtr_->SetDeleted();
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
long IniRecord::FirstEntry(IniEntry*& Entry_)
{
  Entry_ = _Entry;

  if (_Entry)
    return 0;
  
  return -1;
}

/****************************************************************************/
Boolean IniRecord::ModifyEntry(long SlotNum_, const IniEntry& Entry_)
{
  if (SlotNum_ < 0 || SlotNum_ >= _NumEntries || !_NumEntries)
    return FALSE;

  IniEntry* RecPtr_ = _Entry;

  if (RecPtr_ && _NumEntries)
  {
    if (SlotNum_)
      RecPtr_ = RecPtr_->Next(SlotNum_);

    if (RecPtr_)
      RecPtr_->ReplaceEntry(Entry_);
    
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
long IniRecord::NextEntry(long CurSlot_, IniEntry*& Entry_)
{
  ++CurSlot_;
  if (CurSlot_ < 0)
    return -1;

  IniEntry* Source_ = GetEntry(CurSlot_);
  Entry_ = Source_;

  return (Source_ ? CurSlot_:-1);
}

/****************************************************************************/
const IniEntry* IniRecord::GetEntry(long Num_) const
{
  return _Entry->Next(Num_);
}

/****************************************************************************/
IniEntry* IniRecord::GetEntry(long Num_)
{
  return _Entry->Next(Num_);
}

/****************************************************************************/
long IniRecord::NumEntries() const
{
  return _NumEntries;
}

/****************************************************************************/
IniRecord& IniRecord::SetDeleted(Boolean Flag_)
{
  _Deleted = Flag_;
  return *this;
}

/****************************************************************************/
IniRecord* IniRecord::SetNext(IniRecord* Next_)
{
  _Next = Next_;
  return this;
}

/****************************************************************************/    
IniRecord* IniRecord::Append(IniRecord* Next_)
{
  if (_Next)
    _Next->Append(Next_);
  else
    _Next = Next_;

  return this;
}

/****************************************************************************/
const IniRecord* IniRecord::Next(long Num_) const
{
  if (Num_ == 0)
    return this;

  IniRecord* Next_ = _Next;
  long x;
  
  for (x = 1; x < Num_; x++)
    Next_ = Next_->_Next;

  return Next_;
}

/****************************************************************************/    
IniRecord* IniRecord::Next(long Num_)
{
  IniRecord* Next_ = this;
  long x;
  
  for (x = 0; x < Num_; x++)
    Next_ = Next_->_Next;

  return Next_;
}

/****************************************************************************/
const IniRecord* IniRecord::Last() const
{
  if (_Next)
    return _Next->Last();

  return this;
}

/****************************************************************************/
IniRecord* IniRecord::Last()
{
  if (_Next)
    return _Next->Last();

  return this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* IniRecord::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void IniRecord::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
IniFile::IniFile():
_ListTail(NULL),
_RecordList(NULL),
_RecordsAllocated(0),
_Status(INIFILE_NOTREADY | INIFILE_NOT_OPENED)
{}

/****************************************************************************/
IniFile::IniFile(const char* Fname_, int Mode_):
_ListTail(NULL),
_RecordList(NULL),
_RecordsAllocated(0),
_Status(INIFILE_NOTREADY | INIFILE_NOT_OPENED)
{
  SetFileProperties(Fname_, Mode_);
  Open();
}

/****************************************************************************/
IniFile::IniFile(const IniFile& Finfo_):
File(Finfo_),
_ListTail(NULL),
_RecordList(Finfo_._RecordList ? IniRecord::Make(*Finfo_._RecordList):NULL),
_RecordsAllocated(Finfo_._RecordsAllocated),
_Status(INIFILE_NOTREADY | INIFILE_NOT_OPENED)
{
  _ListTail = _RecordList->Last();
}

/****************************************************************************/
IniFile::~IniFile()
{
  delete _RecordList;
  _RecordList = NULL;
}

/****************************************************************************/
Boolean IniFile::TestForIniFile()
{
  char Buffer_[256];
  Boolean Status_ = FALSE;
  char* CharSet_ = MakeCharSet(isspace);
  
  iseekg(0);

  while (!GetReader()->EndOfFile())
  {
    GetReader()->ReadLine(Buffer_, 256);
    RemovePadding(Buffer_, CharSet_);
    
    if (strlen(Buffer_))
    {
      Status_ = (Buffer_[0] == '[' && Buffer_[strlen(Buffer_) - 1] == ']');      
      break;
    }
  }

  ::DeleteArray(CharSet_);
  return Status_;
}

/****************************************************************************/
Boolean IniFile::ReadIntoMemory()
{
  if (_Status != INIFILE_READY)
    return FALSE;

  char Buffer_[256];
  char* Rest_;  
  Boolean TitleRead_ = FALSE;
  Boolean HasTitle_ = FALSE;
  char* CharSet_ = MakeCharSet(isspace);
  
  iseekg(0);

  while (!GetReader()->EndOfFile())
  {
    GetReader()->ReadLine(Buffer_, 256);
    RemovePadding(Buffer_, CharSet_);
    size_t Len_ = strlen(Buffer_);    
    
    if (Len_)
    {
      HasTitle_ = (Buffer_[0] == '[' && Buffer_[Len_ - 1] == ']');
      
      if (HasTitle_ && !TitleRead_)
      {
        TitleRead_ = TRUE;
        ++_RecordsAllocated;
        
        Len_ -= 2;
        memmove(Buffer_, &Buffer_[1], Len_);
        Buffer_[Len_] = 0;

        if (_RecordList)
        {
          _ListTail->Append(IniRecord::Make(Buffer_));
          _ListTail = _ListTail->Last();
        }
        else
          _ListTail = _RecordList = IniRecord::Make(Buffer_);
      }
      else if (TitleRead_ && (Rest_ = strchr(Buffer_, '=')))
      {
        *Rest_ = 0;
        ++Rest_;

        _ListTail->AddEntry(IniEntry::Make(Buffer_, Rest_));
      }
    }
    else
      TitleRead_ = FALSE;
  }

  ::DeleteArray(CharSet_);
  return TRUE;
}

/****************************************************************************/
IniFile* IniFile::Make(char* FileName_, int Mode_)
{
  return (new IniFile(FileName_, Mode_));
}

/****************************************************************************/
Object* IniFile::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new IniFile(*this));

  return NULL;
}

/****************************************************************************/
IniFile& IniFile::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static IniFile _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& IniFile::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return IniFile::GiveNullObject();
}

/****************************************************************************/
Object& IniFile::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return IniFile::GiveNullObject();
}

/****************************************************************************/
int IniFile::ClassID() const
{
  return TypeInfo::INIFILE_TYPE;
}

/****************************************************************************/
Boolean IniFile::OpenForUpdate(int Modes_)
{
  int ForRead_ = Modes_ & ios::in;
  int ForWrite_ = Modes_ & ios::out;
  int ForAppend_ = Modes_ & ios::app;
  int ForTrunc_ = Modes_ & ios::trunc;

  if (ForTrunc_)
    return FALSE;

  return !(ForWrite_ && !ForRead_ && !ForAppend_);
}

/****************************************************************************/
IniFile& IniFile::operator = (const IniFile& Finfo_)
{
  if (this != &Finfo_)
  {
    _ListTail = NULL;
    delete _RecordList;
    _RecordList = Finfo_._RecordList ? IniRecord::Make(*Finfo_._RecordList):NULL;
    _RecordsAllocated = Finfo_._RecordsAllocated;
    _ListTail = _RecordList->Last();

    File::operator = (Finfo_);
  }

  return *this;
}

/****************************************************************************/
Boolean IniFile::Open(const char* Path_, int Modes_)
{
  Boolean Valid_ = File::Open(Path_, Modes_);

  if (Valid_)
  {
    ClearStatus(INIFILE_NOT_OPENED);

    if (OpenForUpdate(Modes_))
    {
      Valid_ = TestForIniFile();    
      if (Valid_)
        ClearStatus(INIFILE_NOTREADY);
    }
    else
      ClearStatus(INIFILE_NOTREADY);
  }

  return Valid_;
}

/****************************************************************************/
Boolean IniFile::Close()
{
  _Status = INIFILE_NOTREADY | INIFILE_NOT_OPENED;
  return File::Close();
}

/****************************************************************************/
Boolean IniFile::UpdateFile()
{
  UpdateList();

  if (((GetMode() & ios::out) || (GetMode() & ios::app)) &&
      _RecordList && _RecordsAllocated &&
      _Status == INIFILE_READY && !Bad())
  {
    Truncate();
    oseekp(0);
    GetReader()->ClearState();
    
    return _RecordList->WriteTo(GetReader());
  }

  return FALSE;
}

/****************************************************************************/
Boolean IniFile::UpdateList()
{
  long Index_ = 0;
  Boolean Done_ = TRUE;
  IniRecord* RecPtr_ = _RecordList;

  while (RecPtr_ && Done_ && Index_ < _RecordsAllocated)
    if (RecPtr_->IsDeleted())
      RecPtr_ = ReallyDeleteRecord(Index_, Done_);
    else
    {
      RecPtr_ = RecPtr_->Next();
      ++Index_;      
    }

  return Done_;
}

/****************************************************************************/
Boolean IniFile::IsIniFile(const char* Path_) const
{
  IniFile IniFile_(*this);

  if (Path_)
    IniFile_.SetFileProperties(Path_, ios::in);
  else
    IniFile_.SetFileMode(ios::in);

  return IniFile_.TestForIniFile();
}

/****************************************************************************/
long IniFile::RecordsAllocated() const
{
  return _RecordsAllocated;
}

/****************************************************************************/
long IniFile::AddRecord(const IniRecord& Record_)
{
  if (_Status != INIFILE_READY)
    return -1;

  ++_RecordsAllocated;

  if (_RecordList)
  {
    _ListTail->Append(IniRecord::Make(Record_));
    _ListTail = _ListTail->Last();
  }
  else
    _ListTail = _RecordList = IniRecord::Make(Record_);

  return (_RecordsAllocated - 1);
}

/****************************************************************************/
IniRecord* IniFile::ReallyDeleteRecord(long SlotNum_, Boolean& Done_)
{
  if (SlotNum_ < 0 || SlotNum_ >= _RecordsAllocated || !_RecordsAllocated)
  {
    Done_ = FALSE;
    return NULL;
  }

  IniRecord* PrevPtr_ = NULL;
  IniRecord* NextPtr_ = NULL;
  IniRecord* RecPtr_ = _RecordList;

  if (RecPtr_ && _RecordsAllocated)
  {
    if (SlotNum_)
    {
      long Index_ = SlotNum_ - 1;
      PrevPtr_ = Index_ ? RecPtr_->Next(Index_):RecPtr_;    
      RecPtr_ = RecPtr_->Next(SlotNum_);
    }

    if (!PrevPtr_)
      _RecordList = RecPtr_->Next();
    else
      PrevPtr_->SetNext(RecPtr_->Next());

    NextPtr_ = RecPtr_->Next();
    RecPtr_->SetNext(NULL);
    delete RecPtr_;
    --_RecordsAllocated;    

    if (!_RecordList || !_RecordsAllocated)
      _ListTail = _RecordList;

    Done_ = TRUE;
    return NextPtr_;    
  }

  Done_ = FALSE;
  return NULL;
}

/****************************************************************************/
Boolean IniFile::DeleteRecord(long SlotNum_)
{
  if (SlotNum_ < 0 || SlotNum_ >= _RecordsAllocated || !_RecordsAllocated)
    return FALSE;

  if (_RecordList)
  {
    IniRecord* RecPtr_ = _RecordList;  
    if (SlotNum_)
      RecPtr_ = RecPtr_->Next(SlotNum_);

    RecPtr_->SetDeleted(TRUE);
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
long IniFile::FirstRecord(IniRecord& Record_)
{
  if (_RecordList)
    Record_ = *_RecordList;
  else
    return -1;

  return 0;
}

/****************************************************************************/
Boolean IniFile::ModifyRecord(long SlotNum_, const IniRecord& Record_)
{
  if (SlotNum_ < 0 || SlotNum_ >= _RecordsAllocated ||
      !_RecordsAllocated || _Status != INIFILE_READY)
    return FALSE;

  IniRecord* RecPtr_ = _RecordList;

  if (RecPtr_ && _RecordsAllocated)
  {
    if (SlotNum_)
      RecPtr_ = RecPtr_->Next(SlotNum_);

    if (RecPtr_)
      RecPtr_->ReplaceRecord(Record_);
    
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
long IniFile::NextRecord(long CurSlot_, IniRecord& Record_)
{
  ++CurSlot_;
  if (CurSlot_ < 0 || _Status != INIFILE_READY)
    return -1;

  return (RetrieveRecord(CurSlot_, Record_) ? CurSlot_:-1);
}

/****************************************************************************/
Boolean IniFile::RetrieveRecord(long SlotNum_, IniRecord& Record_)
{
  if (SlotNum_ < 0 || SlotNum_ >= _RecordsAllocated ||
      !_RecordsAllocated || _Status != INIFILE_READY)
    return FALSE;

  IniRecord* RecPtr_ = _RecordList;

  if (RecPtr_)
  {
    if (SlotNum_)
      RecPtr_ = RecPtr_->Next(SlotNum_);

    Record_ = *RecPtr_;
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* IniFile::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void IniFile::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* IniFile::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void IniFile::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if INIFILE_DEBUG
int main()
{
  IniFile myini("sample.ini", ios::in | ios::out);
  
  if (myini.ReadIntoMemory())
  {
    IniRecord Record_, NewRecord_;

    myini.RetrieveRecord(2, Record_);
    Record_.DeleteEntry(3);
    Record_.DeleteEntry(4);
    Record_.DeleteEntry(5);

    IniEntry Entry_(Record_.GetEntry(12)->Key(), "Canada");
    Record_.ModifyEntry(12, Entry_);
    myini.ModifyRecord(2, Record_);
    myini.DeleteRecord(3);

    myini.RetrieveRecord(1, Record_);
    Entry_.SetKey("User"); Entry_.SetValue("Bugs Bunny");
    Record_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Password"); Entry_.SetValue("Whats Up Doc?");
    Record_.AddEntry(IniEntry::Make(Entry_));
    myini.ModifyRecord(1, Record_);

    NewRecord_.SetName("kill file");
    Entry_.SetKey("User"); Entry_.SetValue("Fred Flintstone");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Password"); Entry_.SetValue("Yaba Daba Doo!");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Drive Letter"); Entry_.SetValue("D:");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    Entry_.SetKey("Shares"); Entry_.SetValue("//pentium1/PENTIUM_C");
    NewRecord_.AddEntry(IniEntry::Make(Entry_));
    myini.AddRecord(NewRecord_);
    
    myini.UpdateFile();
  }

  myini.Close();
  return 0;
}
#endif
/****************************************************************************/
#endif




