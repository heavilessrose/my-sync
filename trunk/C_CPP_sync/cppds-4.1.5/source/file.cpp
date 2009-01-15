#ifndef FILE_CPP
#define FILE_CPP
#ifndef FILE_H
  #include "file.h"
#endif

/****************************************************************************/
FileAcceptor::FileAcceptor(const File* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
void FileAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const File*)Ptr_;
}

/****************************************************************************/
const void* FileAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
// File class specific acceptor methods
Boolean FileAcceptor::IsFile() const
{
  return TRUE;
}

/****************************************************************************/
void* FileAcceptor::CreateFromFile(const ObjectAcceptor& Obj_)
{
  _VarPtr = new File(*((const File*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* FileAcceptor::AssignFromFile(const ObjectAcceptor& Obj_)
{
  const File* TrgPtr_ = (const File*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* FileAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void FileAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
File::File(int Mode_):
_Reader(FileReader::Make(&_FileBuffer)),
_FileDesc(EOF),
_Mode(Mode_),
_Opened(FALSE)
{}

/****************************************************************************/
File::File(int FileDesc_, int Mode_):
_Reader(FileReader::Make(&_FileBuffer)),
_FileDesc(FileDesc_),
_Mode(Mode_),
_Opened(FALSE)
{}

/****************************************************************************/
File::File(const char* Fname_, int Mode_):
_Reader(FileReader::Make(&_FileBuffer)),
_FileDesc(EOF),
_Fname(Fname_),
_Mode(Mode_),
_Opened(FALSE)
{}

/****************************************************************************/
File::File(const File& Finfo_):
ObjectImp((Object&)Finfo_),
_Reader(FileReader::Make(&_FileBuffer)),
_FileDesc(Finfo_._FileDesc),
_Fname(Finfo_._Fname),
_Mode(Finfo_._Mode),
_Opened(FALSE)
{}

/****************************************************************************/
File::~File()
{
  Close();
  SetReader(NULL);
}

/****************************************************************************/
ByteStream* File::ByteStream_Dispatch()
{
  // Check for null letter pointer before returning
  ByteStream* Letter_ = GetReader();
  
  if (!Letter_ || GetReader()->IsNullObject())
    THROW (FallibleBase::DerefNullLetterObjectErr());

  // Return letter of envelope
  return Letter_;
}

/****************************************************************************/
const ByteStream* File::ByteStream_Dispatch() const
{
  // Check for null letter pointer before returning
  ByteStream* Letter_ = GetReader();
  
  if (!Letter_ || GetReader()->IsNullObject())
    THROW (FallibleBase::DerefNullLetterObjectErr());

  // Return letter of envelope
  return Letter_;
}

/****************************************************************************/
/****************************************************************************/
File* File::Make(int Mode_)
{
  return (new File(Mode_));
}

/****************************************************************************/
File* File::Make(int FileDesc_, int Mode_)
{
  return (new File(FileDesc_, Mode_));
}

/****************************************************************************/
File* File::Make(const char* Fname_, int Mode_)
{
  return (new File(Fname_, Mode_));
}

/****************************************************************************/
File* File::Make(const Object& Obj_)
{
  FileAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& File::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  FileAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromFile(TrgComp_)));
}

/****************************************************************************/
Object* File::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new File(*this));

  FileAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromFile(TrgComp_)));
}

/****************************************************************************/
File& File::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static File _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& File::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return File::GiveNullObject();
}

/****************************************************************************/
Object& File::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return File::GiveNullObject();
}

/****************************************************************************/
/****************************************************************************/
int File::ClassID() const
{
  return TypeInfo::FILE_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* File::GiveObjectAcceptor() const
{
  return (new FileAcceptor(this));
}

/****************************************************************************/
istream& File::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& File::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& File::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& File::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& File::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& File::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
/****************************************************************************/
File& File::SetFileMode(int Mode_)
{
  _Mode = Mode_;
  return *this;
}

/****************************************************************************/
File& File::SetFileDescriptor(int FileDesc_)
{
  _FileDesc = FileDesc_;
  return *this;
}

/****************************************************************************/
# if HAS_NONSTD_FILEBUF
File& File::SetFileBuffer(char* Buffer_, int Size_)
{
  GetBuffer()->setbuf(Buffer_, Size_);
  return *this;
}
# endif
/****************************************************************************/
File& File::SetFileProperties(const char* Fname_, int Mode_)
{
  _Fname = Fname_;
  _Mode = Mode_;

  return *this;
}

/****************************************************************************/
File& File::operator = (const File& Finfo_)
{
  if (this != &Finfo_)
  {
    SetReader(Finfo_._Reader);

    _FileDesc = Finfo_._FileDesc;
    _Fname = Finfo_._Fname;
    _Mode = Finfo_._Mode;

    ObjectImp::operator = ((Object&)Finfo_);
  }

  return  *this;
}

/****************************************************************************/
FileReader* File::operator -> () const
{
  Assert(_Reader != NULL, "Dereferencing NULL File Reader pointer");
  return _Reader;
}

/****************************************************************************/
filebuf* File::GetBuffer() const
{
  Assert(_Reader != NULL, "Dereferencing NULL File Reader pointer");
  return _Reader->GetBuffer(_Mode);
}

/****************************************************************************/
/****************************************************************************/
// file open procedures
Boolean File::Open(const char* Path_, int Modes_)
{
  Boolean Done_ = FALSE;
  Close();

  if (_Reader)
  {
    if (Path_)
      _Fname = Path_;

    if (Modes_)
      _Mode = Modes_;
  
    if (ValidFileName())
      Done_ = _Opened = _Reader->Open(_Fname.c_str(), _Mode);
# if HAS_NONSTD_FILEBUF
    else if (_FileDesc != EOF)
    {
      GetBuffer()->attach(_FileDesc);
      Done_ = _Opened = (GetBuffer()->fd() != EOF);
    }
# endif
  }

  return Done_;
}

/****************************************************************************/
// file close procedures
Boolean File::Close()
{
  Boolean Done_ = FALSE;

  if (_Reader)
  {
    Done_ = _Reader->Close();
    _Opened = !Done_;
  }

  return Done_;
}

/****************************************************************************/
Boolean File::Truncate()
{
  int Opened_ = Opened();
  int OldModes_ = _Mode;

  if ((Opened_ && Close()) || !Opened_)
    if (Open(_Fname.c_str(), ios::out))
      if (Close())
        return (Opened_ ? Open(_Fname.c_str(), OldModes_):TRUE);

  return FALSE;
}

/****************************************************************************/
long File::FileSize()
{
  if (_Reader)
    return _Reader->FileSize(_Mode);

  return 0;
}

/****************************************************************************/
Boolean File::ChangeFileSize(long Size_)
{
  if (_Reader)
    return _Reader->ChangeFileSize(_Mode, Size_);

  return 0;
}

/****************************************************************************/
// Data member mutators
File& File::SetReader(FileReader* Reader_)
{
  if (_Reader && _Reader->IsNewed())
    delete _Reader;

  _Reader = Reader_;
  return *this;
}

/****************************************************************************/
const char* File::GetPath(char* Result_, char PathSep_) const
{
  if (!ValidFileName())
    return NULL;

  Fallible<Subscript> Index_;
  static char Buffer_[256];

  if (Result_ == NULL)
    Result_ = Buffer_;

  Index_ = _Fname.findlast(PathSep_);
  if (Index_.valid())
    return strncpy(Result_, _Fname.c_str(), Subscript(Index_) + 1);

  return NULL;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* File::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void File::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* File::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void File::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if FILE_DEBUG
void main()
{
  char Buffer_[128];
  StreamReader Sreader_;  
  File InFile_("text.in", ios::in|ios::out);

  InFile_.Open();  

  while (!InFile_->ReadLine(Buffer_, 128).EndOfFile())
    Sreader_.WriteLine(Buffer_);

  InFile_.Close();
}
#endif
/****************************************************************************/
#endif





