#ifndef STREAM_H
#define STREAM_H
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif

/****************************************************************************/
class Object;
class ByteStreamIterator;

class ByteStream
{
  protected:
    // This method does the actual dispatching work
    // This isn't called directly, but is overwritten in the
    // envelope class. Since the dispatched methods were originally
    // designated as pure abstract, calling this dispatch method through
    // them is flagged as a fatal error because it means they were never
    // reimplemented in the derived classes as a "concrete" implementation.
    virtual ByteStream* ByteStream_Dispatch();
    virtual const ByteStream* ByteStream_Dispatch() const;

  public:
    // overriding binary input stream methods
    virtual istream& iread(char* Bytes_, Ulong Max_)
	{ return ByteStream_Dispatch()->iread(Bytes_, Max_); }
    virtual istream& iseekg(long Pos_)
	{ return ByteStream_Dispatch()->iseekg(Pos_); }
    virtual istream& iseekg(long Pos_, int Dir_)
	{ return ByteStream_Dispatch()->iseekg(Pos_, Dir_); }
    virtual long itellg()
	{ return ByteStream_Dispatch()->itellg(); }
    virtual long igcount()
	{ return ByteStream_Dispatch()->igcount(); }

    // overriding binary output stream methods
    virtual ostream& owrite(const char* Bytes_, Ulong Max_)
	{ return ByteStream_Dispatch()->owrite(Bytes_, Max_); }
    virtual ostream& oseekp(long Pos_)
	{ return ByteStream_Dispatch()->oseekp(Pos_); }
    virtual ostream& oseekp(long Pos_, int Dir_)
	{ return ByteStream_Dispatch()->oseekp(Pos_, Dir_); }
    virtual long otellp()
	{ return ByteStream_Dispatch()->otellp(); }
    virtual ostream& oflush()
	{ return ByteStream_Dispatch()->oflush(); }

    // Standard iostreams conversion operators
    virtual operator istream& ()
	{ return ByteStream_Dispatch()->operator istream& (); }
    virtual operator ostream& ()
	{ return ByteStream_Dispatch()->operator ostream& (); }

    // Standard iostream accessor methods
    virtual istream& GetInStream(Boolean PrepareToRead_=TRUE)
	{ return ByteStream_Dispatch()->GetInStream(PrepareToRead_); }
    virtual ostream& GetOutStream(Boolean PrepareToWrite_=TRUE)
	{ return ByteStream_Dispatch()->GetOutStream(PrepareToWrite_); }

    // stream methods modified to work using custom
    // defined records as basic units instead of bytes
    virtual int CurrentIOMode() const
	{ return ByteStream_Dispatch()->CurrentIOMode(); }
    virtual const Object* CurrentObject() const
	{ return ByteStream_Dispatch()->CurrentObject(); }
    virtual ByteStream& WriteWithThisObject(const Object* ObjPtr_)
	{ return ByteStream_Dispatch()->WriteWithThisObject(ObjPtr_); }
    virtual ByteStream& ReadWithThisObject(Object* ObjPtr_)
	{ return ByteStream_Dispatch()->ReadWithThisObject(ObjPtr_); }
    virtual ByteStream& SeekGet(long RecNum_)
	{ return ByteStream_Dispatch()->SeekGet(RecNum_); }
    virtual ByteStream& SeekPut(long RecNum_)
	{ return ByteStream_Dispatch()->SeekPut(RecNum_); }
    virtual ByteStream& SeekGet(long RecNum_, int Dir_)
	{ return ByteStream_Dispatch()->SeekGet(RecNum_, Dir_); }
    virtual ByteStream& SeekPut(long RecNum_, int Dir_)
	{ return ByteStream_Dispatch()->SeekPut(RecNum_, Dir_); }
    virtual long TellGet()
	{ return ByteStream_Dispatch()->TellGet(); }
    virtual long TellPut()
	{ return ByteStream_Dispatch()->TellPut(); }
    virtual long SizeGet()
	{ return ByteStream_Dispatch()->SizeGet(); }
    virtual long SizePut()
	{ return ByteStream_Dispatch()->SizePut(); }
    virtual ByteStream& ReadRecord()
	{ return ByteStream_Dispatch()->ReadRecord(); }
    virtual ByteStream& PeekRecord()
	{ return ByteStream_Dispatch()->PeekRecord(); }
    virtual ByteStream& WriteRecord()
	{ return ByteStream_Dispatch()->WriteRecord(); }
    virtual ByteStream& AppendRecord()
	{ return ByteStream_Dispatch()->AppendRecord(); }
    virtual Boolean Verified() const
	{ return ByteStream_Dispatch()->Verified(); }
    virtual long RecordFormWidth() const
        { return ByteStream_Dispatch()->RecordFormWidth(); }

    // I/O stream status bits accessor methods
    virtual Boolean EndOfFile()
	{ return ByteStream_Dispatch()->EndOfFile(); }
    virtual Boolean Good()
	{ return ByteStream_Dispatch()->Good(); }
    virtual Boolean Bad()
	{ return ByteStream_Dispatch()->Bad(); }
    virtual Boolean Fail()
	{ return ByteStream_Dispatch()->Fail(); }

    // Stream browser constructor
    virtual ByteStream* MakeByteStreamBrowser()
	{ return ByteStream_Dispatch()->MakeByteStreamBrowser(); }
    virtual void SetByteStreamBrowser(ByteStream* Browser_)
	{ ByteStream_Dispatch()->SetByteStreamBrowser(Browser_); }
    virtual ByteStream* GetByteStreamBrowser() const
	{ return ByteStream_Dispatch()->GetByteStreamBrowser(); }

    // object iterator allocation and return method
    virtual ByteStreamIterator* NewByteStreamIterator()
	{ return ByteStream_Dispatch()->NewByteStreamIterator(); }
};

/****************************************************************************/
class ByteStreamIterator
{
  private:
    ByteStream* _Stream;
    int _Dir;

    long _Count;
    long _Limit;
    Byte_t _Shared;

  public:
    ByteStreamIterator(ByteStream* ObjPtr_, int Dir_);
    ByteStreamIterator(const ByteStreamIterator& Obj_);

    virtual void Reset();
    virtual Boolean More() const;
    virtual ByteStream& Next();
    virtual ByteStream& Current();
};

/****************************************************************************/
#endif




