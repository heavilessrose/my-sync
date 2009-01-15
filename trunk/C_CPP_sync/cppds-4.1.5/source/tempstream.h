#ifndef TEMPSTREAM_H
#define TEMPSTREAM_H
#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERT_H
#endif
#ifndef LONESTRING_H
  #include "lonestr.h"
#endif
#ifndef CNTPTR_H
  #include "cntptr.h"
#endif

#if USE_STRSTREAM
  #if HAS_LONGFILENAME
    #ifndef INCL_STRSTREAM_H
      #include <strstream.h>
      #define INCL_STRSTREAM_H
    #endif
  #else
    #ifndef INCL_STRSTREAM_H
      #include <strstrea.h>
      #define INCL_STRSTREAM_H
    #endif
  #endif
#else
  #ifndef INCL_SSTREAM_H
    #include <sstream>
    #define INCL_SSTREAM_H
  #endif
#endif

#define TEMPSTREAM_DEBUG        0

/****************************************************************************/
class OutputBuffer;

class OutBufferStream : public std::ostringstream
{
  DEFAULT_ROOTOBJECT_DEFN(OutBufferStream)

  friend class OutputBuffer;

  private:
    static char* _Dummy;
  
    OutputBuffer* _Output;
    char*& _CstrPtr;
    ostream* _OutStream;
    Boolean _Destroyed;

    OutBufferStream(char*& StrStore_, ostream* Ostrm_);
    OutBufferStream(OutputBuffer* StrStore_, ostream* Ostrm_);

    void Append(const char* CurStr_);
  
  public:
    OutBufferStream(const OutBufferStream& Obj_);  
    virtual ~OutBufferStream();

    static CountedPtr<OutBufferStream> StreamPtr(char*& StrStore_, ostream* Ostrm_=NULL);
    static CountedPtr<OutBufferStream> StreamPtr(OutputBuffer* StrStore_, ostream* Ostrm_=NULL);
    static CountedPtr<OutBufferStream> StreamPtr(const OutBufferStream& Obj_);

    // returns current position ending of output character buffer.
    int CurrentPos();

    // Returns the current "C style" character string
    char* Cstring()
        { return _CstrPtr; }

    // Returns the current output character buffer
    OutputBuffer* Output()
        { return _Output; }

#if defined(__TURBOC__)
    virtual OutBufferStream* CloneIt() const
       { return (new OutBufferStream(*this)); }
#else
    virtual OutBufferStream* Clone() const
       { return (new OutBufferStream(*this)); }
#endif

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

class OutputBuffer
{
  DEFAULT_ROOTOBJECT_DEFN(OutputBuffer)

  friend class OutBufferStream;
  
  private:
    ostream* _OutStream;
    LoneString _Buffer;    
    Boolean _NoOutput;

    void Append(const LoneString& rhs, Subscript RunLen_);
    void SetNoOutput();

  public:
    OutputBuffer(ostream* Ostrm_=NULL);

    CountedPtr<OutBufferStream> StreamPtr();
    CountedPtr<OutBufferStream> StreamPtr(ostream* Ostrm_);

    // Returns the current output character buffer
    LoneString& Buffer()
        { return _Buffer; }

    // Has output from stream?
    Boolean HasOutput() const
        { return !_NoOutput; }

    // Method to set the stream pointer
    OutputBuffer& SetStream(ostream* Ostrm_)
	{ _OutStream = Ostrm_;
	  return *this; }

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




