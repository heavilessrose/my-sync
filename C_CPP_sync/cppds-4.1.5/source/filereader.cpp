// File reader ADT class
//
#ifndef FILEREADER_CPP
#define FILEREADER_CPP
#ifndef FILEREADER_H
  #include "filereader.h"
#endif

/****************************************************************************/
//			File Reader Class
/****************************************************************************/
FileReader::FileReader():
StreamReader(new ifstream, new ofstream),
_Fin((ifstream*)(_InStreamPtr == &cin ? NULL:_InStreamPtr)),
_Fout((ofstream*)(_OutStreamPtr == &cout ? NULL:_OutStreamPtr))
{}

/****************************************************************************/
FileReader::FileReader(filebuf* Buffer_):
StreamReader(Buffer_),
_Fin(NULL),
_Fout(NULL)
{}

/****************************************************************************/
FileReader::FileReader(ifstream* Infsp_, ofstream* Outfsp_):
StreamReader(Infsp_, Outfsp_),
_Fin(NULL),
_Fout(NULL)
{}

/****************************************************************************/
FileReader::FileReader(const StreamReader& Strmrd_):
StreamReader(Strmrd_),
_Fin(NULL),
_Fout(NULL)
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
FileReader::FileReader(const FileReader& Strmrd_):
StreamReader((StreamReader&)Strmrd_),
_Fin(NULL),
_Fout(NULL)
{}
#endif
/****************************************************************************/
FileReader::~FileReader()
{
  Close();

  delete _Fin;
  delete _Fout;
}

/****************************************************************************/
/****************************************************************************/
FileReader* FileReader::Make()
{
  return (new FileReader());
}

/****************************************************************************/
FileReader* FileReader::Make(filebuf* Buffer_)
{
  return (new FileReader(Buffer_));
}

/****************************************************************************/
FileReader* FileReader::Make(ifstream* Infsp_, ofstream* Outfsp_)
{
  return (new FileReader(Infsp_, Outfsp_));
}

/****************************************************************************/
FileReader* FileReader::Make(const Object& Obj_)
{
  FileReaderAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& FileReader::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  FileReaderAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromFileReader(TrgComp_)));
}

/****************************************************************************/
Object* FileReader::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new FileReader(*((StreamReader*)this)));

  FileReaderAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromFileReader(TrgComp_)));
}

/****************************************************************************/
FileReader& FileReader::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static FileReader _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& FileReader::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return FileReader::GiveNullObject();
}

/****************************************************************************/
Object& FileReader::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return FileReader::GiveNullObject();
}

/****************************************************************************/
/****************************************************************************/
int FileReader::ClassID() const
{
  return TypeInfo::FILEREADER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* FileReader::GiveObjectAcceptor() const
{
  return (new FileReaderAcceptor(this));
}

/****************************************************************************/
/****************************************************************************/
StreamReader& FileReader::operator = (const StreamReader& Sr_)
{
  StreamReader* BaseSr_ = (StreamReader*)(this);

  if (BaseSr_ != &Sr_)
  {
    Close();
    StreamReader::operator = (Sr_);
  }

  return *this;
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
FileReader& FileReader::operator = (const FileReader& Fr_)
{
  StreamReader* Sr_ = (StreamReader*)(&Fr_);

  if (this != &Fr_)
  {
    Close();
    StreamReader::operator = (*Sr_);
  }

  return *this;
}
#endif
/****************************************************************************/
/****************************************************************************/
filebuf* FileReader::GetInBuffer()
{
  // Accessing buffer only so no need to prepare stream
  AccessInStream(FALSE);
  return ((filebuf*)ios::rdbuf());
}

/****************************************************************************/
filebuf* FileReader::GetOutBuffer()
{
  // Accessing buffer only so no need to prepare stream
  AccessOutStream(FALSE);
  return ((filebuf*)ios::rdbuf());
}

/****************************************************************************/
filebuf* FileReader::GetBuffer(int Mode_)
{
  return
  (
    (Mode_ & ios::out) || (Mode_ & ios::app) ?
	GetOutBuffer():
	GetInBuffer()
  );
}

/****************************************************************************/
istream& FileReader::operator () (istream* Insp_)
{
  GetInBuffer()->close();
  return StreamReader::operator () (Insp_);
}

/****************************************************************************/
ostream& FileReader::operator () (ostream* Outsp_)
{
  GetOutBuffer()->close();
  return StreamReader::operator () (Outsp_);
}

/****************************************************************************/
/****************************************************************************/
Boolean FileReader::OpenIn(const char* Fname_, int Mode_)
{
  GetInBuffer()->open(Fname_, (IOS_OPENMODE)Mode_);
  return good();
}

/****************************************************************************/
Boolean FileReader::OpenOut(const char* Fname_, int Mode_)
{
  GetOutBuffer()->open(Fname_, (IOS_OPENMODE)Mode_);
  return good();
}

/****************************************************************************/
Boolean FileReader::Open(const char* Fname_, int Mode_)
{
  if (SharedBuffer())
  {
    if ((Mode_ & ios::out) || (Mode_ & ios::app))
      return OpenOut(Fname_, Mode_);
    else if (Mode_ & ios::in)
      return OpenIn(Fname_, Mode_);
  }
  else
  {
    Boolean OpenOK_ = TRUE;

    if ((Mode_ & ios::out) || (Mode_ & ios::app))
      OpenOK_ = OpenOut(Fname_, Mode_);

    if (Mode_ & ios::in)
      OpenOK_ = OpenOK_ && OpenIn(Fname_, Mode_);

    return OpenOK_;
  }

  return FALSE;
}

/****************************************************************************/
long FileReader::FileSize(int Mode_)
{
  long StartPos_, CurPos_, Diff_;

  if ((Mode_ & ios::out) || (Mode_ & ios::app))
  {
    CurPos_ = tellp();
    seekp(0);
    StartPos_ = tellp();
    seekp(0, ios::end);
    Diff_ = tellp() - StartPos_;
    seekp(CurPos_);
  }
  else if (Mode_ & ios::in)
  {
    CurPos_ = tellg();
    seekg(0);
    StartPos_ = tellg();
    seekg(0, ios::end);
    Diff_ = tellg() - StartPos_;
    seekg(CurPos_);
  }

  return Diff_;
}

/****************************************************************************/
Boolean FileReader::ChangeFileSize(int Mode_, long Size_)
{
  char Blank_ = 0;
  char EofMarker_ = EOF;

  if (!(Mode_ & ios::out) && !(Mode_ & ios::app))
    return FALSE;

  long CurPos_ = tellp();
  seekp(0, ios::end);
  write(&Blank_, 1);
  clear();

  seekp(Size_ + 1);
  write(&EofMarker_, 1);
  seekp(CurPos_);
  clear();

  return TRUE;
}

/****************************************************************************/
Boolean FileReader::Close()
{
  Boolean InCloseOK_, OutCloseOK_;

  if (SharedBuffer())
    return (((filebuf*)ios::rdbuf())->close() != 0);

  InCloseOK_ = GetInBuffer()->close() != 0;
  OutCloseOK_ = GetOutBuffer()->close() != 0;

  return (InCloseOK_ && OutCloseOK_);
}

/****************************************************************************/
/****************************************************************************/
#if OVERLOAD_NEW
void* FileReader::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void FileReader::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* FileReader::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void FileReader::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
FileReaderAcceptor::FileReaderAcceptor(const FileReader* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
void FileReaderAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const FileReader*)Ptr_;
}

/****************************************************************************/
const void* FileReaderAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
Boolean FileReaderAcceptor::Isostream() const
{
  return TRUE;
}

/****************************************************************************/
Boolean FileReaderAcceptor::Isistream() const
{
  return TRUE;
}

/****************************************************************************/
Boolean FileReaderAcceptor::Isofstream() const
{
  return TRUE;
}

/****************************************************************************/
Boolean FileReaderAcceptor::Isifstream() const
{
  return TRUE;
}

/****************************************************************************/
Boolean FileReaderAcceptor::Isios() const
{
  return TRUE;
}

/****************************************************************************/
Boolean FileReaderAcceptor::IsStreamReader() const
{
  return TRUE;
}

/****************************************************************************/
Boolean FileReaderAcceptor::IsFileReader() const
{
  return TRUE;
}

/****************************************************************************/
void* FileReaderAcceptor::CreateFromFileReader(const ObjectAcceptor& Obj_)
{
  _VarPtr = new FileReader(*((const StreamReader*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* FileReaderAcceptor::AssignFromFileReader(const ObjectAcceptor& Obj_)
{
  const FileReader* TrgPtr_ = (const FileReader*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* FileReaderAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void FileReaderAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if FILEREADER_DEBUG
void main()
{
  static const char* TESTIN 	= "text.in";
  static const char* TESTOUT	= "text.out";

  char Buffer_[128];
  FileReader Fr_;

  if (Fr_.Open(TESTIN, ios::in) && Fr_.Open(TESTOUT, ios::out))
    while (!Fr_.ReadLine(Buffer_, 128).EndOfFile())
      Fr_.WriteLine(Buffer_);
}
#endif
/****************************************************************************/
#endif





