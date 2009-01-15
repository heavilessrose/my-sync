#ifndef FUNCTION_H
#define FUNCTION_H
#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif

#define FUNCTION_DEBUG	0
/****************************************************************************/
template <class R, class T>
class UnaryFunctionType
{
  protected:
    typedef R(*Unaryfn_t)(T);
    Unaryfn_t _fncp;

  public:
    UnaryFunctionType(R(*fn)(T));
    virtual R operator () (T arg);
};

/****************************************************************************/
template <class R, class F, class S>
class BinaryFunctionType
{
  protected:
    typedef R(*Binaryfn_t)(F, S);
    Binaryfn_t _fncp;

  public:
    BinaryFunctionType(R(*fn)(F, S));
    virtual R operator () (F arg1, S arg2);
};

/****************************************************************************/
template <class T>
class UnaryPredicateType : public UnaryFunctionType<Boolean, T>
{
  public:
    UnaryPredicateType(Boolean(*fn)(T));
    virtual Boolean operator () (T arg);
};

/****************************************************************************/
template <class F, class S>
class BinaryPredicateType : public BinaryFunctionType<Boolean, F, S>
{
  public:
    BinaryPredicateType(Boolean(*fn)(F, S));
    virtual Boolean operator () (F arg1, S arg2);
};

/****************************************************************************/
template <class T>
class Negate : public UnaryPredicateType<T>
{
  public:
    Negate(UnaryPredicateType<T>& upred);

    UnaryPredicateType<T>& operator () (UnaryPredicateType<T>& upred);
    virtual Boolean operator () (T arg);
};

/****************************************************************************/
template <class F, class S>
class Negate2 : public BinaryPredicateType<F, S>
{
  public:
    Negate2(BinaryPredicateType<F, S>& bpred);

    BinaryPredicateType<F, S>& operator () (BinaryPredicateType<F, S>& bpred);
    virtual Boolean operator () (F arg1, S arg2);
};

/****************************************************************************/
template <class R, class F, class S>
class Bind1st : public BinaryFunctionType<R, F, S>, public UnaryFunctionType<R, S>
{
  protected:
    F _arg1;

  public:
    Bind1st(BinaryFunctionType<R, F, S>& bfnc, F arg1);

    UnaryFunctionType<R, S>& operator () (BinaryFunctionType<R, F, S>& bfnc, F arg1);
    virtual R operator () (S arg2);
};

/****************************************************************************/
template <class R, class F, class S>
class Bind2nd : public BinaryFunctionType<R, F, S>, public UnaryFunctionType<R, F>
{
  protected:
    S _arg2;

  public:
    Bind2nd(BinaryFunctionType<R, F, S>& bfnc, S arg2);

    UnaryFunctionType<R, F>& operator () (BinaryFunctionType<R, F, S>& bfnc, S arg2);
    virtual R operator () (F arg1);
};

/****************************************************************************/
template <class T>
class Generator
{
  protected:
    T _current;

  public:
    Generator(T start):
	_current(start){}

    virtual T operator () ()
	{ return _current; }
};

/****************************************************************************/
template <class T>
class Incrementor : public Generator<T>
{
  public:
    Incrementor(T start):
	Generator<T>(start){}

    virtual T operator () ()
	{ return _current++; }
};

/****************************************************************************/
template <class T>
class Decrementor : public Generator<T>
{
  public:
    Decrementor(T start):
	Generator<T>(start){}

    virtual T operator () ()
	{ return _current--; }
};

/****************************************************************************/
#endif





