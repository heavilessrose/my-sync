#ifndef TEMPSTREAM_CPP
#define TEMPSTREAM_CPP
#ifndef TEMPSTREAM_H
  #include "tempstream.h"
#endif

char* OutBufferStream::_Dummy = NULL;

/****************************************************************************/
/****************************************************************************/
OutBufferStream::OutBufferStream(char*& StrBuffer_, ostream* Ostrm_):
_Output(NULL),
_CstrPtr(StrBuffer_),
_OutStream(Ostrm_),
_Destroyed(FALSE)
{}

/****************************************************************************/
OutBufferStream::OutBufferStream(OutputBuffer* StrBuffer_, ostream* Ostrm_):
_Output(StrBuffer_),
_CstrPtr(_Dummy),
_OutStream(Ostrm_),
_Destroyed(FALSE)
{}

/****************************************************************************/
OutBufferStream::OutBufferStream(const OutBufferStream& Obj_):
_Output(Obj_._Output),
_CstrPtr(Obj_._CstrPtr),
_OutStream(Obj_._OutStream),
_Destroyed(FALSE)
{}

/****************************************************************************/
void OutBufferStream::Append(const char* CurStr_)
{
  char* OldStr_ = _CstrPtr;  
  _CstrPtr = strcat(
               strcpy(
                 new char[(OldStr_ ? strlen(OldStr_):0) + (CurStr_ ? strlen(CurStr_):0) + 1],
                 (OldStr_ ? OldStr_:"")),
               (CurStr_ ? CurStr_:""));

  delete[] OldStr_;
}

/****************************************************************************/
// PURPOSE
//   Destructor of output stream wrapper class
//
OutBufferStream::~OutBufferStream()
{
  if (!_Destroyed)
  {
    // Official count of characters from ostrstream
    std::string string_;
    string_ = str();
    const char* CurrentStr_ = string_.c_str();
    size_t Pcount_ = SafeStrLen(CurrentStr_);
  
    // zero terminate current string freeze and output to main output stream
    *this <<ends;    
    size_t Len_ = Pcount_;
    Boolean NoOut_ = !Pcount_ || !CurrentStr_ || !Len_;

    // if current string exists and has been outputted to then...
    // append the current output string to the existing string stored in
    // the data member character buffer. If the output stream pointer exists
    // then send the string to the output stream.
    if (Pcount_ && CurrentStr_)
    {
      if (_OutStream)
        *_OutStream <<CurrentStr_;
    
      if (Len_)
        if (_Output)
          _Output->Append(CurrentStr_, Len_);
        else if (_CstrPtr)
          Append(CurrentStr_);
    }

    // If no or invalid output then set flag in buffer notifying of this
    if (NoOut_ && _Output)
      _Output->SetNoOutput();

    // remove the current output string or unfreeze
    // freeze(0);
    _Destroyed = TRUE;
  }
}

/****************************************************************************/
int OutBufferStream::CurrentPos()
{
  if (_Output)
    return _Output->Buffer().strlen();
  else if (_CstrPtr)
    return strlen(_CstrPtr);

  return 0;
}

/****************************************************************************/
CountedPtr<OutBufferStream> OutBufferStream::StreamPtr(char*& StrStore_, ostream* Ostrm_)
{
  return CountedPtr<OutBufferStream>(new OutBufferStream(StrStore_, Ostrm_));
}

/****************************************************************************/
CountedPtr<OutBufferStream> OutBufferStream::StreamPtr(OutputBuffer* StrStore_, ostream* Ostrm_)
{
  return CountedPtr<OutBufferStream>(new OutBufferStream(StrStore_, Ostrm_));
}

/****************************************************************************/
CountedPtr<OutBufferStream> OutBufferStream::StreamPtr(const OutBufferStream& Obj_)
{
  return CountedPtr<OutBufferStream>(new OutBufferStream(Obj_));
}

/****************************************************************************/
#if OVERLOAD_NEW
void* OutBufferStream::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void OutBufferStream::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
    
/****************************************************************************/
#if HAS_ARRAY_NEW
void* OutBufferStream::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/    
void OutBufferStream::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
OutputBuffer::OutputBuffer(ostream* Ostrm_):
_OutStream(Ostrm_),
_NoOutput(TRUE)
{}

/****************************************************************************/
void OutputBuffer::Append(const LoneString& rhs, Subscript RunLen_)
{
  _Buffer.Append(rhs, RunLen_);
  _NoOutput = FALSE;
}

/****************************************************************************/
void OutputBuffer::SetNoOutput()
{
  _NoOutput = TRUE;
}

/****************************************************************************/
CountedPtr<OutBufferStream> OutputBuffer::StreamPtr()
{
  return OutBufferStream::StreamPtr(this, _OutStream);
}

/****************************************************************************/
CountedPtr<OutBufferStream> OutputBuffer::StreamPtr(ostream* Ostrm_)
{
  if (!Ostrm_)
    return StreamPtr();

  return OutBufferStream::StreamPtr(this, Ostrm_);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* OutputBuffer::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void OutputBuffer::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
    
/****************************************************************************/
#if HAS_ARRAY_NEW
void* OutputBuffer::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/    
void OutputBuffer::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if TEMPSTREAM_DEBUG
int main()
{
  OutputBuffer OutBuf_;
  
  *OutBuf_.StreamPtr() <<"hello world!";
  cout <<OutBuf_.Buffer() <<endl;
  *OutBuf_.StreamPtr() <<" Is anybody out there!";
  cout <<OutBuf_.Buffer() <<endl;

  OutBuf_.Buffer() = "";
  *OutBuf_.StreamPtr() <<"ah doo doo doo!\nah da da da!";
  cout <<OutBuf_.Buffer() <<endl;

  return 0;
}
#endif
/****************************************************************************/
#endif




