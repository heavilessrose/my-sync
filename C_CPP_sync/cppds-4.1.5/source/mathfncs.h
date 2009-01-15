#ifndef MATHFNCS_H
#define MATHFNCS_H
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif

//****************************************************************************
template <class T>
T AbsoluteValue(const T& Obj_, const T& ZeroValue_)
{ return ((Obj_ < ZeroValue_) ? T(-Obj_):T(Obj_)); }

template <class T>
T InvAbsoluteValue(const T& Obj_, const T& ZeroValue_)
{ return ((Obj_ > ZeroValue_) ? T(-Obj_):T(Obj_)); }

template <class T>
T Sqr(const T& x) {return (x*x);}

template <class T>
T Cube(const T& x) {return (x*x*x);}

template <class T>
int Even(const T& x) {return !(x%2);}

template <class T>
int Odd(const T& x) {return (x%2);}

//****************************************************************************
#endif




