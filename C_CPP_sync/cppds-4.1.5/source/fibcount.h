#ifndef FIBCOUNT_H
#define FIBCOUNT_H
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

#define FIBCOUNT_DEBUG	0

/****************************************************************************/
template <class T>
class FibCounter_t
{
  protected:
    T _Previous;
    T _Count;

    T Decrement();
    T Increment();

  public:
    FibCounter_t(T Value_ = 0);

    void Reset()
	{ _Count = _Previous = 0; }

    T operator ++ ()
	{ return Increment(); }
    T operator -- ()
	{ return Decrement(); }
    T operator ++ (int);
    T operator -- (int);

    T Counter()
	{ return _Count; }

    T FindLesser(T Value_);
    T FindGreater(T Value_);
    T FindLesserOrEqual(T Value_);
    T FindGreaterOrEqual(T Value_);
    T FindFib(T Value_);
};

/****************************************************************************/
#endif





