#ifndef STREAM_CPP
#define STREAM_CPP
#ifndef STREAM_H
  #include "stream.h"
#endif

/****************************************************************************/
ByteStream* ByteStream::ByteStream_Dispatch()
{
  NotImplemented("ByteStream", "ByteStream* ByteStream_Dispatch()");
  exit(-1); return this;
}

/****************************************************************************/
const ByteStream* ByteStream::ByteStream_Dispatch() const
{
  NotImplemented("ByteStream", "ByteStream* ByteStream_Dispatch()");
  exit(-1); return this;
}

/****************************************************************************/
ByteStreamIterator::ByteStreamIterator(ByteStream* ObjPtr_, int Dir_):
_Stream(ObjPtr_),
_Shared(ObjPtr_ != NULL && ((istream&)(*ObjPtr_)).rdbuf() == ((ostream&)(*ObjPtr_)).rdbuf()),
_Dir(Dir_),
_Count(0),
_Limit((ObjPtr_ != NULL) ? ((Dir_ & ios::in) ? ObjPtr_->SizeGet():ObjPtr_->SizePut()):0)
{
  assert(_Stream == NULL);

  if ((Dir_ & ios::in) &&
      (Dir_ & ios::out) || (Dir_ & ios::app))
  {
    long PutLim_ = ObjPtr_->SizePut();

    if (PutLim_ < _Limit)
      _Limit = PutLim_;
  }
}

/****************************************************************************/
ByteStreamIterator::ByteStreamIterator(const ByteStreamIterator& Obj_):
_Stream(Obj_._Stream),
_Shared(Obj_._Stream != NULL &&
	((istream&)(*Obj_._Stream)).rdbuf() == ((ostream&)(*Obj_._Stream)).rdbuf()),
_Dir(Obj_._Dir),
_Count(0),
_Limit(Obj_._Limit)
{}

/****************************************************************************/
void ByteStreamIterator::Reset()
{
  _Count = 0;
  ((istream&)(*_Stream)).clear();
}

/****************************************************************************/
Boolean ByteStreamIterator::More() const
{
  return (_Count < (_Limit - 1));
}

/****************************************************************************/
ByteStream& ByteStreamIterator::Next()
{
  if (More())
  {
    int InMode_ = _Dir & ios::in;

    if (InMode_)
      _Stream->SeekGet(++_Count);

    if (!_Shared || !InMode_)
      if ((_Dir & ios::out) || (_Dir & ios::app))
	_Stream->SeekPut(++_Count);

    return *_Stream;
  }

  return Current();
}

/****************************************************************************/
ByteStream& ByteStreamIterator::Current()
{
  int InMode_ = _Dir & ios::in;

  if (InMode_)
    _Stream->SeekGet(_Count);

  if (!_Shared || !InMode_)
    if ((_Dir & ios::out) || (_Dir & ios::app))
      _Stream->SeekPut(_Count);

  return *_Stream;
}

/****************************************************************************/
#endif




