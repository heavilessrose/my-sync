/*****************************************************************************
* FILE		: STREAMRE.CPP
* PURPOSE	: Stream reader ADT source file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: Oct 14 1996
*****************************************************************************/
#ifndef STREAMREADER_CPP
#define STREAMREADER_CPP
#ifndef STREAMREADER_H
  #include "streamreader.h"
#endif

/****************************************************************************/
StreamBrowser::StreamBrowser(ByteStream* Stream_):
_Stream(Stream_)
{}

/****************************************************************************/
istream& StreamBrowser::iread(char* Bytes_, Ulong Max_)
{
  return ((istream&)(*_Stream)).read(Bytes_, Max_);
}

/****************************************************************************/
istream& StreamBrowser::iseekg(long Pos_)
{
  return ((istream&)(*_Stream)).seekg(Pos_);
}

/****************************************************************************/
istream& StreamBrowser::iseekg(long Pos_, int Dir_)
{
  return ((istream&)(*_Stream)).seekg(Pos_, (IOS_SEEKDIR)Dir_);
}

/****************************************************************************/
long StreamBrowser::itellg()
{
  return ((istream&)(*_Stream)).tellg();
}

/****************************************************************************/
long StreamBrowser::igcount()
{
  return ((istream&)(*_Stream)).gcount();
}

/****************************************************************************/
ostream& StreamBrowser::owrite(const char* Bytes_, Ulong Max_)
{
  return ((ostream&)(*_Stream)).write(Bytes_, Max_);
}

/****************************************************************************/
ostream& StreamBrowser::oseekp(long Pos_)
{
  return ((ostream&)(*_Stream)).seekp(Pos_);
}

/****************************************************************************/
ostream& StreamBrowser::oseekp(long Pos_, int Dir_)
{
  return ((ostream&)(*_Stream)).seekp(Pos_, (IOS_SEEKDIR)Dir_);
}

/****************************************************************************/
long StreamBrowser::otellp()
{
  return ((ostream&)(*_Stream)).tellp();
}

/****************************************************************************/
ostream& StreamBrowser::oflush()
{
  return ((ostream&)(*_Stream)).flush();
}

/****************************************************************************/
/****************************************************************************/
//istream& StreamBrowser::InputStream()
StreamBrowser::operator istream& ()
{
  return ((istream&)(*_Stream));
}

/****************************************************************************/
//ostream& StreamBrowser::OutputStream()
StreamBrowser::operator ostream& ()
{
  return ((ostream&)(*_Stream));
}

/****************************************************************************/
int StreamBrowser::CurrentIOMode() const
{
  return _Browser.GetMode();
}

/****************************************************************************/
const Object* StreamBrowser::CurrentObject() const
{
  return _Browser.GetObject();
}

/****************************************************************************/
ByteStream& StreamBrowser::WriteWithThisObject(const Object* ObjPtr_)
{
  _Browser = RecordBrowser(ObjPtr_, ios::out);
  return *this;
}

/****************************************************************************/
ByteStream& StreamBrowser::ReadWithThisObject(Object* ObjPtr_)
{
  _Browser.SetObject(ObjPtr_, ios::in|ios::out);
  return *this;
}

/****************************************************************************/
ByteStream& StreamBrowser::SeekGet(long RecNum_)
{
  if (_Browser.GetMode() & ios::in)
    _Stream->iseekg(_Browser.StorableFormWidth() * RecNum_);
  return *_Stream;
}

/****************************************************************************/
ByteStream& StreamBrowser::SeekPut(long RecNum_)
{
  _Stream->oseekp(_Browser.StorableFormWidth() * RecNum_);
  return *_Stream;
}

/****************************************************************************/
ByteStream& StreamBrowser::SeekGet(long Incr_, int SeekDir_)
{
  if (_Browser.GetMode() & ios::in)
    _Stream->iseekg(_Browser.StorableFormWidth() * Incr_, SeekDir_);
  return *_Stream;
}

/****************************************************************************/
ByteStream& StreamBrowser::SeekPut(long Incr_, int SeekDir_)
{
  _Stream->oseekp(_Browser.StorableFormWidth() * Incr_, SeekDir_);
  return *_Stream;
}

/****************************************************************************/
long StreamBrowser::TellGet()
{
  if (_Browser.GetMode() & ios::in)
    return (_Stream->itellg() / _Browser.StorableFormWidth());
  return 0;
}

/****************************************************************************/
long StreamBrowser::TellPut()
{
  return (_Stream->otellp() / _Browser.StorableFormWidth());
}

/****************************************************************************/
ByteStream& StreamBrowser::ReadRecord()
{
  if (_Browser.GetMode() & ios::in)
    return _Browser.Read(*_Stream);
  return *_Stream;
}

/****************************************************************************/
ByteStream& StreamBrowser::PeekRecord()
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
ByteStream& StreamBrowser::WriteRecord()
{
  return _Browser.Write(*_Stream);
}

/****************************************************************************/
ByteStream& StreamBrowser::AppendRecord()
{
  SeekPut(0, ios::end);
  return WriteRecord();
}

/****************************************************************************/
Boolean StreamBrowser::Verified() const
{
  return _Browser.Verified();
}

/****************************************************************************/
long StreamBrowser::SizePut()
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
long StreamBrowser::SizeGet()
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
long StreamBrowser::RecordFormWidth() const
{
  return _Browser.StorableFormWidth();
}

/****************************************************************************/
ByteStream* StreamBrowser::MakeByteStreamBrowser()
{
  return (new StreamBrowser(_Stream));
}

/****************************************************************************/
void StreamBrowser::SetByteStreamBrowser(ByteStream* Browser_)
{
  _Stream->SetByteStreamBrowser(Browser_);
}

/****************************************************************************/
ByteStream* StreamBrowser::GetByteStreamBrowser() const
{
  return _Stream->GetByteStreamBrowser();
}

/****************************************************************************/
ByteStreamIterator* StreamBrowser::NewByteStreamIterator()
{
  return (new ByteStreamIterator(_Stream, CurrentIOMode()));
}

/****************************************************************************/
//			Stream Reader Class
/****************************************************************************/
StreamReader::StreamReader():
ObjectImp(),
_InStreamPtr(&cin),
_OutStreamPtr(&cout),
_Count(0),
_NoDelim(FALSE),
_Browser(NULL)
{
  SetStreamBuffer(_InStreamPtr);
  MakeByteStreamBrowser();
}

/****************************************************************************/
StreamReader::StreamReader(streambuf* Buffer_):
ObjectImp(),
_InStreamPtr((istream*)this),
_OutStreamPtr((ostream*)this),
_Count(0),
_NoDelim(FALSE),
_Browser(NULL)
{
  if (Buffer_)
    ios::init(Buffer_);

  MakeByteStreamBrowser();
}

/****************************************************************************/
StreamReader::StreamReader(istream* Insp_, ostream* Outsp_):
ObjectImp(),
_InStreamPtr(Insp_ ? Insp_:&cin),
_OutStreamPtr(Outsp_ ? Outsp_:&cout),
_Count(0),
_NoDelim(FALSE),
_Browser(NULL)
{
  SetStreamBuffer(_InStreamPtr);
  MakeByteStreamBrowser();
}

/****************************************************************************/
StreamReader::StreamReader(const StreamReader& Strmrd_):
ObjectImp((ObjectImp&)Strmrd_),
_InStreamPtr(Strmrd_._InStreamPtr),
_OutStreamPtr(Strmrd_._OutStreamPtr),
_Count(Strmrd_._Count),
_NoDelim(FALSE),
_Browser(NULL)
{
  SetStreamBuffer(_InStreamPtr);
  MakeByteStreamBrowser();
}

/****************************************************************************/
StreamReader::~StreamReader()
{
  delete _Browser;

  // Set to default browser object to prevent risk of
  // double deletion of allocated data.
  ios::init(&_DefaultBuf);
}

/****************************************************************************/
ByteStream* StreamReader::GetByteStreamBrowser() const
{
  return _Browser;
}

/****************************************************************************/
void StreamReader::SetByteStreamBrowser(ByteStream* Browser_)
{
  delete _Browser;

  if (Browser_)
    _Browser = Browser_;
  else
    _Browser = NULL;
}

/****************************************************************************/
ByteStream* StreamReader::MakeByteStreamBrowser()
{
  delete _Browser;

  if (ClassID() == StaticID())
    _Browser = new StreamBrowser((ByteStream*)this);
  else
    _Browser = NULL;

  return _Browser;
}

/****************************************************************************/
/****************************************************************************/
StreamReader* StreamReader::Make()
{
  return (new StreamReader);
}

/****************************************************************************/
StreamReader* StreamReader::Make(streambuf* Buffer_)
{
  return (new StreamReader(Buffer_));
}

/****************************************************************************/
StreamReader* StreamReader::Make(istream* Insp_, ostream* Outsp_)
{
  return (new StreamReader(Insp_, Outsp_));
}

/****************************************************************************/
StreamReader* StreamReader::Make(const Object& Obj_)
{
  StreamReaderAcceptor TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
Object& StreamReader::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  StreamReaderAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromStreamReader(TrgComp_)));
}

/****************************************************************************/
Object* StreamReader::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new StreamReader(*this));

  StreamReaderAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromStreamReader(TrgComp_)));
}

/****************************************************************************/
StreamReader& StreamReader::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static StreamReader _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& StreamReader::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return StreamReader::GiveNullObject();
}

/****************************************************************************/
Object& StreamReader::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return StreamReader::GiveNullObject();
}

/****************************************************************************/
/****************************************************************************/
int StreamReader::ClassID() const
{
  return TypeInfo::STREAMREADER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* StreamReader::GiveObjectAcceptor() const
{
  return (new StreamReaderAcceptor(this));
}

/****************************************************************************/
istream& StreamReader::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& StreamReader::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& StreamReader::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& StreamReader::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& StreamReader::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& StreamReader::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
/****************************************************************************/
StreamReader& StreamReader::operator = (const StreamReader& Strmrd_)
{
  if (this != &Strmrd_)
  {
    _InStreamPtr = Strmrd_._InStreamPtr;
    _OutStreamPtr = Strmrd_._OutStreamPtr;
    _NoDelim = Strmrd_._NoDelim;
    _Count = Strmrd_._Count;

    ObjectImp::operator = ((Object&)Strmrd_);
  }

  return *this;
}

/****************************************************************************/
/****************************************************************************/
void StreamReader::FlushStream(ostream* OsPtr_)
{
  OsPtr_->flush();
  if (OsPtr_->tie() != 0 && OsPtr_ != OsPtr_->tie())
    FlushStream(OsPtr_->tie());
}

/****************************************************************************/
Boolean StreamReader::Ipfx(istream& Is_, int Noskip_)
{
  if (Is_.good())
  {
    if (Is_.tie() != 0)
      FlushStream(Is_.tie());

    if (!Noskip_ && Is_.flags() & ios::skipws)
    {
      int ch;
      while (isspace(ch = Is_.rdbuf()->sbumpc()))
	;
      if (ch != EOF)
	Is_.rdbuf()->sputbackc(ch);
    }

    if (Is_.good())
      return 1;
  }

  Is_.clear(ios::failbit);
  return 0;
}

/****************************************************************************/
Boolean StreamReader::Opfx(ostream& Os_)
{
  if (Os_.good() && Os_.tie() != 0)
    FlushStream(Os_.tie());

  return Os_.good();
}

/****************************************************************************/
void StreamReader::Osfx(ostream& Os_)
{
  if (Os_.flags() && ios::unitbuf)
    Os_.flush();
}

/****************************************************************************/
void StreamReader::SetStreamBuffer(ios* StreamPtr_)
{
  if (this != StreamPtr_)
  {
    ostream* Os_ 	= StreamPtr_->tie();
    int State_		= StreamPtr_->rdstate();
    long Flags_ 	= StreamPtr_->flags();
    int Wide_ 		= StreamPtr_->width();
    int Prec_ 		= StreamPtr_->precision();
    char Fill_ 		= StreamPtr_->fill();

    ios::init(StreamPtr_->rdbuf());
    if (Os_)
      ios::tie(Os_);

//  ios::setstate(State_);
    ios::clear(rdstate() | (IOS_IOSTATE)State_);
    ios::flags((IOS_FMTFLAGS)Flags_);
    ios::width(Wide_);
    ios::precision(Prec_);
    ios::fill(Fill_);
  }
}

/****************************************************************************/
/****************************************************************************/
istream& StreamReader::AccessInStream(Boolean PrepareToRead_, int Noskip_)
{
  if (_InStreamPtr)
  {
    if (_InStreamPtr->rdbuf() != ios::rdbuf())
      SetStreamBuffer(_InStreamPtr);

    if (PrepareToRead_ && !Ipfx(Noskip_))
      THROW (FallibleBase::IOFailureErr());
  }

  return *this;
}

/****************************************************************************/
ostream& StreamReader::AccessOutStream(Boolean PrepareToWrite_)
{
  if (_OutStreamPtr)
  {
    if (_OutStreamPtr->rdbuf() != ios::rdbuf())
      SetStreamBuffer(_OutStreamPtr);

    if (PrepareToWrite_ && !Opfx())
      THROW (FallibleBase::IOFailureErr());
  }

  return *this;
}

/****************************************************************************/
istream& StreamReader::GetInStream(Boolean PrepareToRead_)
{
  if (_InStreamPtr)
    return ((_InStreamPtr->eof() || eof()) ?
		AccessInStream(FALSE):
		AccessInStream(PrepareToRead_, 1));

  return *this;
}

/****************************************************************************/
ostream& StreamReader::GetOutStream(Boolean PrepareToWrite_)
{
  if (_OutStreamPtr)
    return ((_OutStreamPtr->eof() || eof()) ?
		AccessOutStream(FALSE):
		AccessOutStream(PrepareToWrite_));

  return *this;
}

/****************************************************************************/
/****************************************************************************/
istream& StreamReader::operator () (istream* Insp_)
{
  if (Insp_ == NULL)
    Insp_ = &cin;

  if (_InStreamPtr->rdbuf() != Insp_->rdbuf())
  {
    int State_ = (_OutStreamPtr && !SharedBuffer()) ? _OutStreamPtr->rdstate():0;
    ios::clear((IOS_IOSTATE)State_);
  }

  _InStreamPtr = Insp_;
  return GetInStream();
}

/****************************************************************************/
ostream& StreamReader::operator () (ostream* Outsp_)
{
  if (Outsp_ == NULL)
    Outsp_ = &cout;

  if (_OutStreamPtr->rdbuf() != Outsp_->rdbuf())
  {
    int State_ = (_InStreamPtr && !SharedBuffer()) ? _InStreamPtr->rdstate():0;
    ios::clear((IOS_IOSTATE)State_);
  }

  _OutStreamPtr = Outsp_;
  return GetOutStream();
}

/****************************************************************************/
StreamReader::IOResult StreamReader::GetIOResult() const
{
  StreamReader *const LocalThis_ = (StreamReader *const)this;
  return (!LocalThis_->Good() ? IO_FAIL:
	  _NoDelim ? IO_NODELIM:
		     IO_DONE);
}

/****************************************************************************/
/****************************************************************************/
StreamReader& StreamReader::operator >> (Object& Obj_)
{
  AccessInStream(TRUE) >>Obj_;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::operator >> (char* InputStr_)
{
  istream& Is_ = AccessInStream(TRUE);

  if (IstreamCheck(Is_) && InputStr_)
  {
    Is_ >>InputStr_;
    IstreamCheck(Is_);
  }

  _Count = Good() ? strlen(InputStr_):0;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::Read(char* InputStr_)
{
  istream& Is_ = AccessInStream(TRUE, 1);

  if (IstreamCheck(Is_) && InputStr_)
  {
    Is_ >>InputStr_;
    IstreamCheck(Is_);
  }

  _Count = Good() ? strlen(InputStr_):0;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::ReadObject(Object& Obj_)
{
  ::Read(AccessInStream(TRUE, 1), Obj_);
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::IgnoreObject(Object& Obj_)
{
  ::Ignore(AccessInStream(TRUE, 1), Obj_);
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::ReadBytes(char* Buffer_, long Size_)
{
  char* StartPos_ = Buffer_;
  istream& Is_ = AccessInStream(TRUE, 1);

  if (IstreamCheck(Is_) && Buffer_)
  {
    while (Size_ > INT_MAX)
    {
      Is_.read(Buffer_, INT_MAX);
      IstreamCheck(Is_);

      if (IsIOError())
	break;

      Size_ -= INT_MAX;
      Buffer_ += INT_MAX;
    }

    if (!IsIOError() || StartPos_ == Buffer_)
    {
      Is_.read(Buffer_, Size_);
      IstreamCheck(Is_);

      if (!IsIOError())
	Buffer_ += Size_;
    }
  }

  _Count = Buffer_ - StartPos_;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::ReadLine(char* InputStr_, short InputLen_, const char* Sep_)
{
  istream& Is_ = AccessInStream(TRUE, 1);

  if (IstreamCheck(Is_) && InputStr_ && InputLen_ && Sep_)
  {
    Boolean Rflag_ = ::ReadLine(Is_, InputStr_, InputLen_, *Sep_);
    IstreamCheck(Is_);

    if (Good())
      if (!Rflag_)
	SetNoDelim(TRUE);
      else
	SetNoDelim(FALSE);
  }

  _Count = Good() ? gcount():0;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::TruncLine(char* InputStr_, short InputLen_, const char* Sep_)
{
  istream& Is_ = AccessInStream(TRUE, 1);

  if (IstreamCheck(Is_) && InputStr_ && InputLen_ && Sep_)
  {
    ::TruncLine(Is_, InputStr_, InputLen_, *Sep_);
    IstreamCheck(Is_);
  }

  _Count = Good() ? gcount():0;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::Ignore(const char* Sep_)
{
  istream& Is_ = AccessInStream(TRUE, 1);

  if (IstreamCheck(Is_) && Sep_)
  {
    ::Ignore(Is_, *Sep_);
    IstreamCheck(Is_);
  }

  _Count = Good() ? gcount():0;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::ReadBuffer(streambuf* Sb_, const char* Sep_)
{
  istream& Is_ = AccessInStream(TRUE, 1);

  if (IstreamCheck(Is_) && Sb_ && Sep_)
  {
    Is_.get(*Sb_, *Sep_);
    IstreamCheck(Is_);
  }

  _Count = Good() ? gcount():0;
  return *this;
}

/****************************************************************************/
/****************************************************************************/
StreamReader& StreamReader::operator << (const Object& Obj_)
{
  AccessOutStream(TRUE) <<Obj_;
  Osfx();
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::operator << (const char* OutputStr_)
{
  Write(OutputStr_);
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::Write(const char* OutputStr_)
{
  ostream& Os_ = AccessOutStream(TRUE);

  if (OutputStr_ && OstreamCheck(Os_))
  {
    Os_ <<OutputStr_;
    OstreamCheck(Os_);
  }

  _Count = Good() ? strlen(OutputStr_):0;

  Osfx();
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::WriteObject(const Object& Obj_)
{
  ::Write(AccessOutStream(TRUE), Obj_);

  Osfx();
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::WriteBytes(const char* Buffer_, long Size_)
{
  const char* StartPos_ = Buffer_;
  ostream& Os_ = AccessOutStream(TRUE);

  if (OstreamCheck(Os_) && Buffer_)
  {
    while (Size_ > INT_MAX)
    {
      Os_.write(Buffer_, INT_MAX);
      OstreamCheck(Os_);

      if (IsIOError())
	break;

      Size_ -= INT_MAX;
      Buffer_ += INT_MAX;
    }

    if (!IsIOError() || StartPos_ == Buffer_)
    {
      Os_.write(Buffer_, Size_);
      OstreamCheck(Os_);

      if (!IsIOError())
	Buffer_ += Size_;
    }
  }

  _Count = Buffer_ - StartPos_;
  Osfx();
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::WriteLine(const char* OutputStr_, const char* Sep_)
{
  long Len_ = Write(OutputStr_).GetCount();

  if (!IsIOError())
    Len_ += Write(Sep_).GetCount();

  _Count = Len_;
  return *this;
}

/****************************************************************************/
StreamReader& StreamReader::WriteLines(const char* StrArray_[], const char* Sep_)
{
  if (Good() && StrArray_)
  {
    long Index_, Lines_;
    for (Index_ = Lines_ = 0; StrArray_[Index_]; Index_++)
      if (Write(StrArray_[Index_]).Good())
      {
	if (!IsIOError())
	  ++Lines_;

	if (!Write(Sep_).Good())
	  break;
      }
      else
	break;

    _Count = Lines_;
  }
  else
    _Count = 0;

  OstreamCheck(AccessOutStream(FALSE));
  SetIOstate(Good() && _Count, IO_NOTDONE);

  return *this;
}

/****************************************************************************/
/****************************************************************************/
istream& StreamReader::iread(char* Bytes_, Ulong Max_)
{
  istream& Strm_ = _Browser->iread(Bytes_, Max_);
  IstreamCheck(Strm_);
  return Strm_;
}

/****************************************************************************/
istream& StreamReader::iseekg(long Pos_)
{
  if (Pos_ < 0)
    Pos_ = 0;

  istream& Strm_ = _Browser->iseekg(Pos_);
  IstreamCheck(Strm_);
  return Strm_;
}

/****************************************************************************/
istream& StreamReader::iseekg(long Pos_, int Dir_)
{
  if (Dir_ == ios::beg)
    return iseekg(Pos_);

  if (Pos_ < 0)
  {
    if (Dir_ == ios::end)
    {
      iseekg(0, ios::end);
      if (IsIOError())
	return *this;
    }

    long AbsCurPos_ = itellg();
    if (Pos_ + AbsCurPos_ < 0L)
      Pos_ = -AbsCurPos_;
  }
  else if (Pos_ > 0 && Dir_ == ios::end)
    Pos_ = 0;

  istream& Strm_ = _Browser->iseekg(Pos_, Dir_);
  IstreamCheck(Strm_);

  return Strm_;
}

/****************************************************************************/
long StreamReader::itellg()
{
  long Pos_ = _Browser->itellg();
  IstreamCheck(GetInStream(FALSE));
  return Pos_;
}

/****************************************************************************/
long StreamReader::igcount()
{
  _Count = _Browser->igcount();
  IstreamCheck(GetInStream(FALSE));
  return GetCount();
}

/****************************************************************************/
ostream& StreamReader::owrite(const char* Bytes_, Ulong Max_)
{
  ostream& Strm_ = _Browser->owrite(Bytes_, Max_);
  OstreamCheck(Strm_);
  return Strm_;
}

/****************************************************************************/
ostream& StreamReader::oseekp(long Pos_)
{
  if (Pos_ < 0)
    Pos_ = 0;

  ostream& Strm_ = _Browser->oseekp(Pos_);
  OstreamCheck(Strm_);
  return Strm_;
}

/****************************************************************************/
ostream& StreamReader::oseekp(long Pos_, int Dir_)
{
  if (Dir_ == ios::beg)
    return oseekp(Pos_);

  if (Pos_ < 0)
  {
    if (Dir_ == ios::end)
    {
      oseekp(0, ios::end);
      if (IsIOError())
	return *this;
    }

    long AbsCurPos_ = otellp();
    if (Pos_ + AbsCurPos_ < 0L)
      Pos_ = -AbsCurPos_;
  }
  else if (Pos_ > 0 && Dir_ == ios::end)
    Pos_ = 0;

  ostream& Strm_ = _Browser->oseekp(Pos_, Dir_);
  OstreamCheck(Strm_);

  return Strm_;
}

/****************************************************************************/
long StreamReader::otellp()
{
  long Pos_ = _Browser->otellp();
  OstreamCheck(GetOutStream(FALSE));
  return Pos_;
}

/****************************************************************************/
ostream& StreamReader::oflush()
{
  ostream& Strm_ = _Browser->oflush();
  OstreamCheck(Strm_);
  return Strm_;
}

/****************************************************************************/
int StreamReader::CurrentIOMode() const
{
  return _Browser->CurrentIOMode();
}

/****************************************************************************/
const Object* StreamReader::CurrentObject() const
{
  return _Browser->CurrentObject();
}

/****************************************************************************/
ByteStream& StreamReader::WriteWithThisObject(const Object* ObjPtr_)
{
  _Browser->WriteWithThisObject(ObjPtr_);
  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::ReadWithThisObject(Object* ObjPtr_)
{
  _Browser->ReadWithThisObject(ObjPtr_);
  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::SeekGet(long RecNum_)
{
  if (_Browser)
    return _Browser->SeekGet(RecNum_);

  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::SeekPut(long RecNum_)
{
  if (_Browser)
    return _Browser->SeekPut(RecNum_);

  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::SeekGet(long RecNum_, int SeekDir_)
{
  if (_Browser)
    return _Browser->SeekGet(RecNum_, SeekDir_);

  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::SeekPut(long RecNum_, int SeekDir_)
{
  if (_Browser)
    return _Browser->SeekPut(RecNum_, SeekDir_);

  return *this;
}

/****************************************************************************/
long StreamReader::TellGet()
{
  if (_Browser)
    return _Browser->TellGet();

  return 0;
}

/****************************************************************************/
long StreamReader::TellPut()
{
  if (_Browser)
    return _Browser->TellPut();

  return 0;
}

/****************************************************************************/
ByteStream& StreamReader::ReadRecord()
{
  if (_Browser && (CurrentIOMode() & ios::in))
  {
    _Browser->ReadRecord();
    SetIOstate(RecordVerified(), IO_NOTDONE);
  }

  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::PeekRecord()
{
  if (_Browser && (CurrentIOMode() & ios::in))
  {
    _Browser->PeekRecord();
    SetIOstate(RecordVerified(), IO_NOTDONE);
  }

  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::AppendRecord()
{
  if (_Browser)
  {
    _Browser->AppendRecord();
    SetIOstate(RecordVerified(), IO_NOTDONE);
  }

  return *this;
}

/****************************************************************************/
ByteStream& StreamReader::WriteRecord()
{
  if (_Browser)
  {
    _Browser->WriteRecord();
    SetIOstate(RecordVerified(), IO_NOTDONE);
  }

  return *this;
}

/****************************************************************************/
Boolean StreamReader::Verified() const
{
  StreamReader *const LocalThis_ = (StreamReader *const)this;
  return LocalThis_->Good();
}

/****************************************************************************/
long StreamReader::SizeGet()
{
  if (_Browser && (CurrentIOMode() & ios::in))
  {
    long Size_ = _Browser->SizeGet();
    SetIOstate(Verified(), IO_NOTDONE);
    return Size_;
  }

  return 0;
}

/****************************************************************************/
long StreamReader::SizePut()
{
  if (_Browser)
  {
    long Size_ = _Browser->SizePut();
    SetIOstate(Verified(), IO_NOTDONE);
    return Size_;
  }

  return 0;
}

/****************************************************************************/
long StreamReader::RecordFormWidth() const
{
  return _Browser->RecordFormWidth();
}

/****************************************************************************/
ByteStreamIterator* StreamReader::NewByteStreamIterator()
{
  return (new ByteStreamIterator(this, CurrentIOMode()));
}

/****************************************************************************/
/****************************************************************************/
void StreamReader::ResetFormat()
{
  AccessInStream(FALSE) >>IosReset();
  AccessOutStream(FALSE) <<IosReset();
}

/****************************************************************************/
void StreamReader::SetFormat(IosFormat& IosForm_)
{
  AccessInStream(FALSE) >>IosSet(IosForm_);
  AccessOutStream(FALSE) <<IosSet(IosForm_);
}

/****************************************************************************/
ostream* StreamReader::TieStreams()
{
  if (_InStreamPtr)
  {
    _InStreamPtr->tie(_OutStreamPtr);
    AccessInStream(FALSE);
    return ios::tie();
  }

  return NULL;
}

/****************************************************************************/
void StreamReader::UntieStreams()
{
  if (_InStreamPtr)
    _InStreamPtr->tie(NULL);

  if (_OutStreamPtr)
    _OutStreamPtr->tie(NULL);

  ios::tie(NULL);
}

/****************************************************************************/
void StreamReader::ClearState(int State_)
{
  if (_InStreamPtr)
    _InStreamPtr->clear((IOS_IOSTATE)State_);

  if (_OutStreamPtr)
    _OutStreamPtr->clear((IOS_IOSTATE)State_);

  ios::clear((IOS_IOSTATE)State_);
  ClearIOstate();
}

/****************************************************************************/
void StreamReader::ResetStreams()
{
  (GetInStream().seekg(0) >>IosReset()).clear();
  (GetOutStream().flush().seekp(0) <<IosReset()).clear();
}

/****************************************************************************/
Boolean StreamReader::EndOfFile()
{
  return AccessInStream(FALSE).eof();
}

/****************************************************************************/
Boolean StreamReader::Good()
{
  return
  (
    !IsIOError() &&
    (!SharedBuffer() && TiedStream() != 0) ?
	(_InStreamPtr->good() && _OutStreamPtr->good()):
	good()
  );
}

/****************************************************************************/
Boolean StreamReader::Bad()
{
  return
  (
    IsIOError() ||
    (!SharedBuffer() && TiedStream() != 0) ?
	(_InStreamPtr->bad() || _OutStreamPtr->bad()):
	bad()
  );
}

/****************************************************************************/
Boolean StreamReader::Fail()
{
  return
  (
    IsIOError() ||
    (!SharedBuffer() && TiedStream() != 0) ?
	(_InStreamPtr->fail() || _OutStreamPtr->fail()):
	fail()
  );
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StreamReader::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StreamReader::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* StreamReader::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void StreamReader::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
StreamReaderAcceptor::StreamReaderAcceptor(const StreamReader* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
void StreamReaderAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const StreamReader*)Ptr_;
}

/****************************************************************************/
const void* StreamReaderAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
Boolean StreamReaderAcceptor::IsStreamReader() const
{
  return TRUE;
}

/****************************************************************************/
Boolean StreamReaderAcceptor::Isostream() const
{
  return TRUE;
}

/****************************************************************************/
Boolean StreamReaderAcceptor::Isistream() const
{
  return TRUE;
}

/****************************************************************************/
Boolean StreamReaderAcceptor::Isios() const
{
  return TRUE;
}

/****************************************************************************/
void* StreamReaderAcceptor::CreateFromStreamReader(const ObjectAcceptor& Obj_)
{
  _VarPtr = new StreamReader(*((const StreamReader*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* StreamReaderAcceptor::AssignFromStreamReader(const ObjectAcceptor& Obj_)
{
  const StreamReader* TrgPtr_ = (const StreamReader*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StreamReaderAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StreamReaderAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if STREAMREADER_DEBUG
int main()
{
#if defined(WIN_DOS)
  clrscr();
#endif
  char Buffer_[128];

  StreamReader Sr_;

  ios& iosRef = Sr_.GetIos();
  Sr_.TruncLine(Buffer_, 128);
  Sr_.WriteLine(Buffer_);
  Sr_.Write(Buffer_);

  Sr_.Ignore("\\").Read(Buffer_);
  Sr_.WriteLine(Buffer_);

  const char* Array_[] =
  {
    "Fubarred",
    "Anotherd",
    "Yippee Yahoo",
    "Hello World!",
    "Blah Blah Blah",
    NULL
  };

  Sr_.WriteLines(Array_);

  IosFormat Form_;

  Form_.SetFormat(ios::skipws | ios::dec);
  Form_.SetWidth(12);
  Form_.SetFill('*');
  Form_.SetPrecision(2);
  Sr_.SetFormat(Form_);

  double SomeFloat_ = 3.1415927;
  (ostream&)(Sr_) <<SomeFloat_ <<endl;

  (ostream&)(Sr_) <<IosReset();
  (ostream&)(Sr_) <<SomeFloat_ <<endl;

  StreamReader* SrPtr_ = new StreamReader;
  (ostream&)(*SrPtr_) <<"testing" <<endl;
  delete SrPtr_;

  return 0;
}
#endif
/****************************************************************************/
#endif





