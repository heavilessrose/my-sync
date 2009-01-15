#ifndef ITERBASE_CPP
#define ITERBASE_CPP
#ifndef ITERBASE_H
  #include "iterbase.h"
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
RandomAccessIterator<T>* RandomAccessIterator<T>::Dispatch()
{
  if (_VarPtr == NULL)
    Xnull();

  return _VarPtr;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>* RandomAccessIterator<T>::Dispatch() const
{
  if (_ObjPtr == NULL)
    Xnull();

  return _ObjPtr;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& RandomAccessIterator<T>::operator = (const RandomAccessIterator<T>& Rhs_)
{
  if (this != &Rhs_)
    _ObjPtr = Rhs_.GiveIterator().Clone();

  return *this;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator ++ ()
{
  Dispatch()->operator -- ();
  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator ++ (int)
{
  RandomAccessIterator<T> RevRef_(Clone());
  Dispatch()->operator -- ();
  return RevRef_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator ++ () const
{
  Dispatch()->operator -- ();
  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator ++ (int) const
{
  RandomAccessIterator<T> RevRef_(Clone());
  Dispatch()->operator -- ();
  return RevRef_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator -- ()
{
  Dispatch()->operator ++ ();
  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator -- (int)
{
  RandomAccessIterator<T> RevRef_(Clone());
  Dispatch()->operator ++ ();
  return RevRef_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator -- () const
{
  Dispatch()->operator ++ ();
  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator -- (int) const
{
  RandomAccessIterator<T> RevRef_(Clone());
  Dispatch()->operator ++ ();
  return RevRef_;
}

/****************************************************************************/
template <class T>
RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator + (size_t Diff_)
{
  RandomAccessIterator<T> Ref_(Clone());
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator + (size_t Diff_) const
{
  RandomAccessIterator<T> Ref_(Clone());
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator - (size_t Diff_)
{
  RandomAccessIterator<T> Ref_(Clone());
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIteratorProxy<T> ReverseRandomAccessIterator<T>::operator - (size_t Diff_) const
{
  RandomAccessIterator<T> Ref_(Clone());
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator += (size_t Diff_)
{
  Dispatch()->operator -= (Diff_);
  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator += (size_t Diff_) const
{
  Dispatch()->operator -= (Diff_);
  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator -= (size_t Diff_)
{
  Dispatch()->operator += (Diff_);
  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& ReverseRandomAccessIterator<T>::operator -= (size_t Diff_) const
{
  Dispatch()->operator += (Diff_);
  return *this;
}

/****************************************************************************/
#endif



