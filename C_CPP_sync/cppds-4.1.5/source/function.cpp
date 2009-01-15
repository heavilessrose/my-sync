#ifndef FUNCTION_CPP
#define FUNCTION_CPP
#ifndef FUNCTION_H
  #include "function.h"
#endif
/****************************************************************************/
/****************************************************************************/
template <class R, class T>
UnaryFunctionType<R, T>::UnaryFunctionType(R(*fn)(T)):
_fncp(fn)
{}

/****************************************************************************/
template <class R, class T>
R UnaryFunctionType<R, T>::operator () (T arg)
{
  return (*_fncp)(arg);
}

/****************************************************************************/
/****************************************************************************/
template <class R, class F, class S>
BinaryFunctionType<R, F, S>::BinaryFunctionType(R(*fn)(F, S)):
_fncp(fn)
{}

/****************************************************************************/
template <class R, class F, class S>
R BinaryFunctionType<R, F, S>::operator () (F arg1, S arg2)
{
  return (*_fncp)(arg1, arg2);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
UnaryPredicateType<T>::UnaryPredicateType(Boolean(*fn)(T)):
UnaryFunctionType<Boolean, T>(fn)
{}

/****************************************************************************/
template <class T>
Boolean UnaryPredicateType<T>::operator () (T arg)
{
  return UnaryFunctionType<Boolean, T>::operator () (arg);
}

/****************************************************************************/
/****************************************************************************/
template <class F, class S>
BinaryPredicateType<F, S>::BinaryPredicateType(Boolean(*fn)(F, S)):
BinaryFunctionType<Boolean, F, S>(fn)
{}

/****************************************************************************/
template <class F, class S>
Boolean BinaryPredicateType<F, S>::operator () (F arg1, S arg2)
{
  return BinaryFunctionType<Boolean, F, S>::operator () (arg1, arg2);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
Negate<T>::Negate(UnaryPredicateType<T>& upred):
UnaryPredicateType<T>(upred)
{}

/****************************************************************************/
template <class T>
UnaryPredicateType<T>& Negate<T>::operator () (UnaryPredicateType<T>& upred)
{
  UnaryPredicateType<T>::operator = (upred);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean Negate<T>::operator () (T arg)
{
  return !(UnaryPredicateType<T>::operator () (arg));
}

/****************************************************************************/
/****************************************************************************/
template <class F, class S>
Negate2<F, S>::Negate2(BinaryPredicateType<F, S>& bpred):
BinaryPredicateType<F, S>(bpred)
{}

/****************************************************************************/
template <class F, class S>
BinaryPredicateType<F, S>& Negate2<F, S>::operator () (BinaryPredicateType<F, S>& bpred)
{
  BinaryPredicateType<F, S>::operator = (bpred);
  return *this;
}

/****************************************************************************/
template <class F, class S>
Boolean Negate2<F, S>::operator () (F arg1, S arg2)
{
  return !(BinaryPredicateType<F, S>::operator () (arg1, arg2));
}

/****************************************************************************/
/****************************************************************************/
template <class R, class F, class S>
Bind1st<R, F, S>::Bind1st(BinaryFunctionType<R, F, S>& bfnc, F arg1):
_arg1(arg1),
BinaryFunctionType<R, F, S>(bfnc),
UnaryFunctionType<R, S>(NULL)
{}

/****************************************************************************/
template <class R, class F, class S>
UnaryFunctionType<R, S>& Bind1st<R, F, S>::operator () (BinaryFunctionType<R, F, S>& bfnc, F arg1)
{
  _arg1 = arg1;
  BinaryFunctionType<R, F, S>::operator = (bfnc);

  return *this;
}

/****************************************************************************/
template <class R, class F, class S>
R Bind1st<R, F, S>::operator () (S arg2)
{
  return BinaryFunctionType<R, F, S>::operator () (_arg1, arg2);
}

/****************************************************************************/
/****************************************************************************/
template <class R, class F, class S>
Bind2nd<R, F, S>::Bind2nd(BinaryFunctionType<R, F, S>& bfnc, S arg2):
_arg2(arg2),
BinaryFunctionType<R, F, S>(bfnc),
UnaryFunctionType<R, F>(NULL)
{}

/****************************************************************************/
template <class R, class F, class S>
UnaryFunctionType<R, F>& Bind2nd<R, F, S>::operator () (BinaryFunctionType<R, F, S>& bfnc, S arg2)
{
  _arg2 = arg2;
  BinaryFunctionType<R, F, S>::operator = (bfnc);

  return *this;
}

/****************************************************************************/
template <class R, class F, class S>
R Bind2nd<R, F, S>::operator () (F arg1)
{
  return BinaryFunctionType<R, F, S>::operator () (arg1, _arg2);
}

/****************************************************************************/
/****************************************************************************/
#if FUNCTION_DEBUG
#include <iostream.h>
//#include <conio.h>
/****************************************************************************/
short square(short num)
{
  return num * num;
}

/****************************************************************************/
short add(short num, short num2)
{
  return num + num2;
}

/****************************************************************************/
Boolean even(short num)
{
  return (num % 2 == 0);
}

/****************************************************************************/
Boolean lesser(short num, short num2)
{
  return (num < num2);
}

/****************************************************************************/
void main()
{
  BinaryPredicateType<short, short>* nbpp;
  UnaryPredicateType<short>* nupp;
  UnaryFunctionType<short, short> uf(square);
  BinaryFunctionType<short, short, short> bf = add;
  UnaryPredicateType<short> up = even;
  BinaryPredicateType<short, short> bp = lesser;
  Negate<short> neg(up);
  Negate2<short, short> neg2(bp);
  Bind1st<short, short, short> bind1(bf, 26);
  Bind2nd<Boolean, short, short> bind2(bp, 35);
  Generator<short> gen(10);
  Incrementor<short> incr(25);
  Decrementor<short> decr(45);

  int val1, val2;
//  clrscr();

  nupp = &neg(up);
  nbpp = &neg2(bp);

  val1 = uf(5);
  val2 = bf(uf(6), 3);

  cout <<val1 <<"\t" <<val2 <<endl;

  val1++;
  cout <<up(val1) <<"\t" <<up(val2) <<endl;
  cout <<(*nupp)(val1) <<"\t" <<(*nupp)(val2) <<endl;
  val2++;
  val1++;
  cout <<up(val1) <<"\t" <<up(val2) <<endl;
  cout <<bp(val1, val2) <<"\t" <<bp(val2, val1) <<endl;
  cout <<(*nbpp)(val1, val2) <<"\t" <<(*nbpp)(val2, val1) <<endl;
  cout <<val1 <<"\t" <<val2 <<endl;

  cout <<bind1(val1) <<"\t" <<bind1(val2) <<endl;
  cout <<bind2(val1) <<"\t" <<bind2(val2) <<endl;

  cout <<gen() <<"\t";
  cout <<gen() <<endl;
  cout <<incr() <<"\t";
  cout <<incr() <<endl;
  cout <<decr() <<"\t";
  cout <<decr() <<endl;
}                       
#endif
/****************************************************************************/
#endif





