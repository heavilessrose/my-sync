// Stream Reader class ADT
//
#ifndef STREAMREADER_H
#define STREAMREADER_H
#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif
#ifndef INCL_FSTREAM_H
  #include <fstream.h>
  #define INCL_FSTREAM_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef OBJECTACCEPTOR_H
  #include "objaccpt.h"
#endif
#ifndef STRIO_H
  #include "strio.h"
#endif
#ifndef STREAM_H
  #include "stream.h"
#endif
#ifndef RECBROWSER_H
  #include "recbrowser.h"
#endif

#define STREAMREADER_DEBUG 	0

/****************************************************************************/
class StreamReaderAcceptor : public ObjectAcceptor
{
  friend class StreamReader;

  protected:
    union
    {
      const StreamReader* _ObjPtr;
      StreamReader* _VarPtr;
    };

    StreamReaderAcceptor(const StreamReader* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // StreamReader class specific acceptor methods
    virtual Boolean IsStreamReader() const;
    virtual Boolean Isostream() const;
    virtual Boolean Isistream() const;
    virtual Boolean Isios() const;

    virtual void* CreateFromStreamReader(const ObjectAcceptor& Obj_);
    virtual void* AssignFromStreamReader(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class StreamBrowser : public ByteStream
{
  protected:
    RecordBrowser _Browser;
    ByteStream* _Stream;

  public:
	#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
      typedef std::_Ios_Seekdir IOS_SEEKDIR;
      typedef std::_Ios_Iostate IOS_IOSTATE;
      typedef std::_Ios_Fmtflags IOS_FMTFLAGS;  
	#else
	  typedef ios::seek_dir IOS_SEEKDIR;
      typedef int IOS_IOSTATE;
      typedef int IOS_FMTFLAGS;  
	#endif

  public:
    StreamBrowser(ByteStream* Stream_);

    // overriding binary input stream methods
    virtual istream& iread(char* Bytes_, Ulong Max_);
    virtual istream& iseekg(long Pos_);
    virtual istream& iseekg(long Pos_, int Dir_);
    virtual long itellg();
    virtual long igcount();

    // overriding binary output stream methods
    virtual ostream& owrite(const char* Bytes_, Ulong Max_);
    virtual ostream& oseekp(long Pos_);
    virtual ostream& oseekp(long Pos_, int Dir_);
    virtual long otellp();
    virtual ostream& oflush();

    // Standard iostreams conversion operators
    virtual operator istream& ();
    virtual operator ostream& ();

    // stream methods modified to work using custom
    // defined records as basic units instead of bytes
    virtual int CurrentIOMode() const;
    virtual const Object* CurrentObject() const;
    virtual ByteStream& WriteWithThisObject(const Object* ObjPtr_);
    virtual ByteStream& ReadWithThisObject(Object* ObjPtr_);
    virtual ByteStream& SeekGet(long RecNum_);
    virtual ByteStream& SeekPut(long RecNum_);
    virtual ByteStream& SeekGet(long Incr_, int SeekDir_);
    virtual ByteStream& SeekPut(long Incr_, int SeekDir_);
    virtual long TellGet();
    virtual long TellPut();
    virtual long SizeGet();
    virtual long SizePut();
    virtual ByteStream& ReadRecord();
    virtual ByteStream& PeekRecord();
    virtual ByteStream& WriteRecord();
    virtual ByteStream& AppendRecord();
    virtual Boolean Verified() const;
    virtual long RecordFormWidth() const;

    // Stream browser constructor
    virtual ByteStream* MakeByteStreamBrowser();
    virtual void SetByteStreamBrowser(ByteStream* Browser_);
    virtual ByteStream* GetByteStreamBrowser() const;

    // object iterator allocation and return method
    virtual ByteStreamIterator* NewByteStreamIterator();
};

/****************************************************************************/
// NOTE: Do not try to implement dynamic allocation of stream buffers
// ie. streambuf or filebuf since this has been attempted many times and
// the result is nothing but grief. Deallocation of the respective stream
// buffers will cause the program to behave strangely or crash. Because of
// this problem the stream buffer passed to the reader classes will not be
// allocated, deep copied or deallocated in any manner.
//
class StreamReader : public ObjectImp, public ByteStream, protected ifstream, protected ofstream
{
  friend class StreamReaderAcceptor;

  friend Object& RootObject(StreamReader& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const StreamReader& Obj_)
	{ return *((const Object*)(&Obj_)); }

  public:
    enum IOResult {IO_FAIL, IO_DONE, IO_NODELIM};
	#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
      typedef std::_Ios_Seekdir IOS_SEEKDIR;
      typedef std::_Ios_Iostate IOS_IOSTATE;
      typedef std::_Ios_Fmtflags IOS_FMTFLAGS;
	#else
	  typedef ios::seek_dir IOS_SEEKDIR;
      typedef int IOS_IOSTATE;
      typedef int IOS_FMTFLAGS;
	#endif

  protected:
    // Dummy stream buffer used to fix double destruction bug. Needs to be
    // filebuf since streambuf ctor is protected in standard C++ library.
    filebuf _DefaultBuf;
    
    istream* _InStreamPtr;
    ostream* _OutStreamPtr;

    ByteStream* _Browser;

    long _Count;
    Boolean _NoDelim;

    void SetStreamBuffer(ios* StreamPtr_);
    void FlushStream(ostream* OsPtr_);

    istream& AccessInStream(Boolean PrepareToRead_, int Noskip_ = 0);
    ostream& AccessOutStream(Boolean PrepareToWrite_);

    inline void SetNoDelim(Boolean Flag_)
	{ _NoDelim = Flag_; }

  public:
    // Constructor / Destructor
    StreamReader();
    StreamReader(streambuf* Buffer_);
    StreamReader(istream* Insp_, ostream* Outsp_);
    StreamReader(const StreamReader& Strmrd_);
    virtual ~StreamReader();

    // Virtual constructor and assignment methods
    static StreamReader* Make();
    static StreamReader* Make(streambuf* Buffer_);
    static StreamReader* Make(istream* Insp_, ostream* Outsp_);
    static StreamReader* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static StreamReader& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::STREAMREADER_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Assignment operator
    virtual StreamReader& operator = (const StreamReader& Strmrd_);

    // Input stream fix
    Boolean Ipfx(istream& Is_, int Noskip_ = 0);
    Boolean Ipfx(int Noskip_)
	{ return Ipfx(*this, Noskip_); }

    // Output stream fix
    Boolean Opfx(ostream& Os_);
    Boolean Opfx()
	{ return Opfx(*this); }

    // Output stream wrap up
    void Osfx(ostream& Os_);
    void Osfx()
	{ Osfx(*this); }

    // Standard iostreams accessor methods
    virtual istream& GetInStream(Boolean PrepareToRead_=TRUE);
    virtual ostream& GetOutStream(Boolean PrepareToWrite_=TRUE);

    // Standard iostreams conversion operators
    virtual operator istream& ()
	{ return GetInStream(); }
    virtual operator ostream& ()
	{ return GetOutStream(); }

    // Stream member mutators
    virtual istream& operator () (istream* Insp_);
    virtual ostream& operator () (ostream* Outsp_);

    // class data accessors
    StreamReader::IOResult GetIOResult() const;
    inline void SetCount(long Count_)
	{ _Count = Count_; }
    inline long GetCount() const
	{ return _Count; }
    inline streambuf* GiveBuffer() const
	{ return ios::rdbuf(); }
    inline Boolean SharedBuffer()
	{ return (_InStreamPtr->rdbuf() == _OutStreamPtr->rdbuf() &&
		  _InStreamPtr->rdbuf() == ios::rdbuf()); }

    // input procedures
    virtual StreamReader& operator >> (Object& Obj_);
    virtual StreamReader& operator >> (char* InputStr_);

    virtual StreamReader& Read(char* InputStr_);
    virtual StreamReader& ReadObject(Object& Obj_);
    virtual StreamReader& IgnoreObject(Object& Obj_);
    virtual StreamReader& ReadBytes(char* Buffer_, long Size_);
    virtual StreamReader& ReadLine(char* InputStr_, short InputLen_, const char* Sep_="\n");
    virtual StreamReader& TruncLine(char* InputStr_, short InputLen_, const char* Sep_="\n");
    virtual StreamReader& Ignore(const char* Sep_="\n");
    virtual StreamReader& ReadBuffer(streambuf* Sb_, const char* Sep_="\n");

    // output procedures
    virtual StreamReader& operator << (const Object& Obj_);
    virtual StreamReader& operator << (const char* OutputStr_);

    virtual StreamReader& Write(const char* OutputStr_);
    virtual StreamReader& WriteObject(const Object& Obj_);
    virtual StreamReader& WriteBytes(const char* Buffer_, long Size_);
    virtual StreamReader& WriteLine(const char* OutputStr_="", const char* Sep_="\n");
    virtual StreamReader& WriteLines(const char* StrArray_[], const char* Sep_="\n");

    // Stream procedures
    // overriding binary input stream methods
    virtual istream& iread(char* Bytes_, Ulong Max_);
    virtual istream& iseekg(long Pos_);
    virtual istream& iseekg(long Pos_, int Dir_);
    virtual long itellg();
    virtual long igcount();

    // overriding binary output stream methods
    virtual ostream& owrite(const char* Bytes_, Ulong Max_);
    virtual ostream& oseekp(long Pos_);
    virtual ostream& oseekp(long Pos_, int Dir_);
    virtual long otellp();
    virtual ostream& oflush();

    // stream methods modified to work using custom
    // defined records as basic units instead of bytes
    virtual int CurrentIOMode() const;
    virtual const Object* CurrentObject() const;
    virtual ByteStream& WriteWithThisObject(const Object* ObjPtr_);
    virtual ByteStream& ReadWithThisObject(Object* ObjPtr_);
    virtual ByteStream& SeekGet(long RecNum_);
    virtual ByteStream& SeekPut(long RecNum_);
    virtual ByteStream& SeekGet(long RecNum_, int SeekDir_);
    virtual ByteStream& SeekPut(long RecNum_, int SeekDir_);
    virtual long TellGet();
    virtual long TellPut();
    virtual long SizeGet();
    virtual long SizePut();
    virtual ByteStream& ReadRecord();
    virtual ByteStream& PeekRecord();
    virtual ByteStream& WriteRecord();
    virtual ByteStream& AppendRecord();
    virtual Boolean Verified() const;
    virtual long RecordFormWidth() const;

    // Stream browser constructor
    virtual ByteStream* MakeByteStreamBrowser();
    virtual void SetByteStreamBrowser(ByteStream* Browser_);
    virtual ByteStream* GetByteStreamBrowser() const;

    // object iterator allocation and return method
    virtual ByteStreamIterator* NewByteStreamIterator();

    // Returns if stream is OK and record I/O operation is successful
    inline Boolean RecordVerified() const
	{ return (Verified() && _Browser && _Browser->Verified()); }

    // IO Format methods
    void ResetFormat();
    void SetFormat(IosFormat& IosForm_);

    // stream reset procedure
    void ResetStreams();

    // ios methods
    ios& GetIos()
	{ return *this; }
    ostream* TiedStream()
	{ return ios::tie(); }
    ostream* TieStreams();
    void UntieStreams();

    void ClearState(int State_=0);

    // I/O state accessor methods
    virtual Boolean EndOfFile();
    virtual Boolean Good();
    virtual Boolean Bad();
    virtual Boolean Fail();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
#endif





