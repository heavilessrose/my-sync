#ifndef DYNARRAYBASE_CPP
#define DYNARRAYBASE_CPP
#ifndef DYNARRAYBASE_H
  #include "dynarraybase.h"
#endif

/****************************************************************************/
template <class T>
DynArrayBase<T>::~DynArrayBase()
{
  _Tidy(1);
}

/****************************************************************************/
template <class T>
DynArrayBase<T>& DynArrayBase<T>::operator = (const DynArrayBase<T>& r)
{
  if (this != &r)
    AssignArg(r.Base(), r.RunLength());

  return *this;
}

/****************************************************************************/
template <class T>
void DynArrayBase<T>::_Tidy(Boolean Constructed_)
{
  if (Constructed_ && _Ptr != 0)
    ::Delete(_Ptr);

  _Len = 0, _Ptr = 0; _Res = 0;
}

/****************************************************************************/
template <class T>
DynArrayBase<T>& DynArrayBase<T>::Append(const T* s, size_t n, size_t d)
{
  if (SIZET_MAX - _Len <= n)
    _Xlen();

  size_t i = _Len;
  for (_Grow(n += i); i < n; ++i, s += d)
    _Ptr[i] = *s;

  return (*this);
}

/****************************************************************************/
template <class T>
DynArrayBase<T>&
DynArrayBase<T>::AssignArg(const T* s, size_t n, size_t d)
{
  _Grow(n, 0, 1);
  for (size_t i = 0; i < n; ++i, s += d)
    _Ptr[i] = *s;

  return (*this);
}

/****************************************************************************/
template <class T>
DynArrayBase<T>&
DynArrayBase<T>::Insert(size_t p, const T* s, size_t n, size_t d)
{
  if (_Len < p)
    _Xran();

  if (SIZET_MAX - _Len <= n)
    _Xlen();

  if (0 < n)
  {
    size_t i = _Len - p;

    for (_Grow(n + _Len); 0 < i; )
      --i, _Ptr[p + n + i] = _Ptr[p + i];

    for (i = 0; i < n; ++i, s += d)
      _Ptr[p + i] = *s;
  }

  return (*this);
}

/****************************************************************************/
template <class T>
DynArrayBase<T>& DynArrayBase<T>::Remove(size_t p, size_t n)
{
  if (_Len < p)
    _Xran();

  if (_Len - p < n)
    n = _Len - p;

  if (0 < n)
  {
    size_t m = _Len - p - n;
    for (size_t i = 0; i < m; ++i)
      _Ptr[p + i] = _Ptr[p + i + n];
    _Grow(_Len - n);
  }

  return (*this);
}

/****************************************************************************/
template <class T>
void DynArrayBase<T>::Resize(size_t n)
{
  _Grow(n, 0, 1);
}

/****************************************************************************/
template <class T>
void DynArrayBase<T>::Resize(size_t n, const T& x)
{
  _Grow(n, &x, 1);
}

/****************************************************************************/
template <class T>
size_t DynArrayBase<T>::Size() const
{
  return (_Len);
}

/****************************************************************************/
template <class T>
void DynArrayBase<T>::Flush()
{
  Resize(0);
}

/****************************************************************************/
template <class T>
size_t DynArrayBase<T>::Limit() const
{
  return RunLength();
}

/****************************************************************************/
template <class T>
Boolean DynArrayBase<T>::IsFull() const
{
  return (_Len == _Res);
}

/****************************************************************************/
template <class T>
Boolean DynArrayBase<T>::IsEmpty() const
{
  return !_Len;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DynArrayBase<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DynArrayBase<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* DynArrayBase<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void DynArrayBase<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif





