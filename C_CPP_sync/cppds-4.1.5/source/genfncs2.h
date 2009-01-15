#ifndef GENFNCS2_H
#define GENFNCS2_H
#ifndef BUILTIN_H
  #include "builtin.h"
#endif
#ifndef STREAM_H
  #include "stream.h"
#endif

/****************************************************************************/
template <class T>
void _Swap(T& x, T& y)
{
  T* temp = ::Clone(x);
  x = y;
  y = *temp;
  ::Delete(temp);
}

/****************************************************************************/
template <class T>
void Swap(T& x, T& y)
{
  _Swap(RootObject(x), RootObject(y));
}

/****************************************************************************/
#if defined(__TURBOC__) | defined(__BORLANDC__)
template <class T>
ostream& Write(ostream& os, const T& obj, Boolean* Ok_=NULL)
{
  os.write((const char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = os.good();

  return os;
}

/****************************************************************************/
template <class T>
istream& Ignore(istream& is, T& obj, Boolean* Ok_=NULL)
{
  T* ObjPtr_ = ::Clone(obj);

  if (ObjPtr_)
  {
    istream& StrmRef_ = Read(is, *ObjPtr_, Ok_);
    ::Delete(ObjPtr_);
    return StrmRef_;
  }

  return is;
}

/****************************************************************************/
template <class T>
istream& Read(istream& is, T& obj, Boolean* Ok_=NULL)
{
  is.read((char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = is.good();

  return is;
}

/****************************************************************************/
template <class T>
ByteStream& Write(ByteStream& os, const T& obj, Boolean* Ok_=NULL)
{
  os.owrite((const char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = os.GetOutStream().good();

  return os;
}

/****************************************************************************/
template <class T>
ByteStream& Ignore(ByteStream& is, T& obj, Boolean* Ok_=NULL)
{
  T* ObjPtr_ = ::Clone(obj);

  if (ObjPtr_)
  {
    ByteStream& StrmRef_ = Read(is, *ObjPtr_, Ok_);
    ::Delete(ObjPtr_);
    return StrmRef_;
  }

  return is;
}

/****************************************************************************/
template <class T>
ByteStream& Read(ByteStream& is, T& obj, Boolean* Ok_=NULL)
{
  is.iread((char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = is.GetInStream().good();

  return is;
}

/****************************************************************************/
/****************************************************************************/
#else
template <class T>
ostream& _Write(ostream& os, const T& obj, Boolean* Ok_)
{
  os.write((const char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = os.good();

  return os;
}

/****************************************************************************/
template <class T>
istream& _Ignore(istream& is, T& obj, Boolean* Ok_)
{
  T* ObjPtr_ = ::Clone(obj);

  if (ObjPtr_)
  {
    istream& StrmRef_ = Read(is, *ObjPtr_, Ok_);
    ::Delete(ObjPtr_);
    return StrmRef_;
  }

  return is;
}

/****************************************************************************/
template <class T>
istream& _Read(istream& is, T& obj, Boolean* Ok_)
{
  is.read((char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = is.good();

  return is;
}

/****************************************************************************/
template <class T>
ByteStream& _Write(ByteStream& os, const T& obj, Boolean* Ok_)
{
  os.owrite((const char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = os.GetOutStream().good();

  return os;
}

/****************************************************************************/
template <class T>
ByteStream& _Ignore(ByteStream& is, T& obj, Boolean* Ok_)
{
  T* ObjPtr_ = ::Clone(obj);

  if (ObjPtr_)
  {
    ByteStream& StrmRef_ = Read(is, *ObjPtr_, Ok_);
    ::Delete(ObjPtr_);
    return StrmRef_;
  }

  return is;
}

/****************************************************************************/
template <class T>
ByteStream& _Read(ByteStream& is, T& obj, Boolean* Ok_)
{
  is.iread((char*)(&obj), SizeOfData(obj));

  if (Ok_)
    *Ok_ = is.GetInStream().good();

  return is;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
ostream& Write(ostream& os, const T& obj, Boolean* Ok_)
{
  return _Write(os, RootConstObject(obj), Ok_);
}

/****************************************************************************/
template <class T>
istream& Ignore(istream& is, T& obj, Boolean* Ok_)
{
  return _Ignore(is, RootObject(obj), Ok_);
}

/****************************************************************************/
template <class T>
istream& Read(istream& is, T& obj, Boolean* Ok_)
{
  return _Read(is, RootObject(obj), Ok_);
}

/****************************************************************************/
template <class T>
ByteStream& Write(ByteStream& os, const T& obj, Boolean* Ok_)
{
  return _Write(os, RootConstObject(obj), Ok_);
}

/****************************************************************************/
template <class T>
ByteStream& Ignore(ByteStream& is, T& obj, Boolean* Ok_)
{
  return _Ignore(is, RootObject(obj), Ok_);
}

/****************************************************************************/
template <class T>
ByteStream& Read(ByteStream& is, T& obj, Boolean* Ok_)
{
  return _Read(is, RootObject(obj), Ok_);
}

/****************************************************************************/
template <class T>
ostream& Write(ostream& os, const T& obj)
{
  return _Write(os, RootConstObject(obj), NULL);
}

/****************************************************************************/
template <class T>
istream& Ignore(istream& is, T& obj)
{
  return _Ignore(is, RootObject(obj), NULL);
}

/****************************************************************************/
template <class T>
istream& Read(istream& is, T& obj)
{
  return _Read(is, RootObject(obj), NULL);
}

/****************************************************************************/
template <class T>
ByteStream& Write(ByteStream& os, const T& obj)
{
  return _Write(os, RootConstObject(obj), NULL);
}

/****************************************************************************/
template <class T>
ByteStream& Ignore(ByteStream& is, T& obj)
{
  return _Ignore(is, RootObject(obj), NULL);
}

/****************************************************************************/
template <class T>
ByteStream& Read(ByteStream& is, T& obj)
{
  return _Read(is, RootObject(obj), NULL);
}
#endif

/****************************************************************************/
#endif




