#ifndef FIBCOUNT_CPP
#define FIBCOUNT_CPP
#ifndef FIBCOUNT_H
  #include "fibcount.h"
#endif

/****************************************************************************/
template <class T>
FibCounter_t<T>::FibCounter_t(T Value_):
_Count(0),
_Previous(0)
{
  if (Value_)
    FindFib(Value_);
}

/****************************************************************************/
template <class T>
T FibCounter_t<T>::Increment()
{
  if (!_Count)
    _Count = 1;
  else
  {
    T Temp_ = _Count;
    _Count += _Previous;
    _Previous = Temp_;
  }

  return _Count;
}

/****************************************************************************/
template <class T>
T FibCounter_t<T>::Decrement()
{
  if (!_Previous)
    _Count = 0;
  else
  {
    T Temp_ = _Previous;
    _Previous = _Count - _Previous;
    _Count = Temp_;
  }

  return _Count;
}

/****************************************************************************/
// Find closest Fibonacci number that is less than or equal to the specified
// value
//
template <class T>
T FibCounter_t<T>::FindLesser(T Value_)
{
  FindGreaterOrEqual(Value_);
  Decrement();

  return _Count;
}

/****************************************************************************/
// Find closest Fibonacci number that is less than or equal to the specified
// value
//
template <class T>
T FibCounter_t<T>::FindGreater(T Value_)
{
  FindLesserOrEqual(Value_);
  Increment();

  return _Count;
}

/****************************************************************************/
// Find closest Fibonacci number that is greater than the specified value
//
template <class T>
T FibCounter_t<T>::FindLesserOrEqual(T Value_)
{

  FindGreaterOrEqual(Value_);
  if (Counter() > Value_)
    Decrement();

  return _Count;
}

/****************************************************************************/
// Find closest Fibonacci number that is greater than the specified value
//
template <class T>
T FibCounter_t<T>::FindGreaterOrEqual(T Value_)
{
  for (Reset(); Value_ > Counter(); Increment());
  return _Count;
}

/****************************************************************************/
template <class T>
T FibCounter_t<T>::FindFib(T Value_)
{
  Reset();
  for (T Index_ = 0; Index_ < Value_; Index_++, Increment());

  return _Count;
}

/****************************************************************************/
template <class T>
T FibCounter_t<T>::operator ++ (int)
{
  T Count_ = _Count;
  Increment();
  return Count_;
}

/****************************************************************************/
template <class T>
T FibCounter_t<T>::operator -- (int)
{
  T Count_ = _Count;
  Decrement();
  return Count_;
}

/****************************************************************************/
/****************************************************************************/
#if FIBCOUNT_DEBUG
#include <iostream.h>
void main()
{
  FibCounter_t Count_;
  short Index_;

  for (Index_ = 0; Index_ < 20; Index_++, Count_++)
    cout <<Count_.Counter() <<" ";
  cout <<endl;

  for (;Index_ >= 0; Index_--, Count_--)
    cout <<Count_.Counter() <<" ";
  cout <<endl;

  cout <<Count_.FindFib(5) <<" "
       <<Count_.FindFib(10) <<" "
       <<Count_.FindFib(0) <<" "
       <<Count_.FindFib(15) <<endl;

  cout <<Count_.FindGreater(55) <<" "
       <<Count_.FindLesser(55) <<" "
       <<Count_.FindGreaterOrEqual(55) <<" "
       <<Count_.FindLesserOrEqual(55) <<endl;

  cout <<Count_.FindGreater(40) <<" "
       <<Count_.FindLesser(40) <<" "
       <<Count_.FindGreaterOrEqual(40) <<" "
       <<Count_.FindLesserOrEqual(40) <<endl;

  FibCounter_t Count2_(20);
  cout <<Count2_.Counter() <<endl;
}
#endif
/****************************************************************************/
#endif





