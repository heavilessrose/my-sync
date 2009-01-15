//=========================================================
//  Numerics Library
//=========================================================
//  FPUtil.h
//
//  Various floating-point utilities
//
//  Copyright 1995-96 Scott Robert Ladd
//=========================================================

#ifndef FPUTIL_H
#define FPUTIL_H

#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif
#ifndef INCL_FLOAT_H
  #include <float.h>
  #define INCL_FLOAT_H
#endif

//-----------------------
// Round to nearest value
//-----------------------

double ToNearest(double x);
long double ToNearest(long double x);

//--------------------------------------------
// set number of significant digits in a value
//--------------------------------------------

double SigDig(double x, size_t n);
long double SigDig(long double x, size_t n);

//------------------------------------------------------
// lowest common denominator and greatest common multiple
//------------------------------------------------------

unsigned long LCM(unsigned long x, unsigned long y);
unsigned long GCD(unsigned long x, unsigned long y);

//-----------------------------------------
// Generic absolute value template function
//-----------------------------------------

template <class T>
inline T AbsVal(T value)
{
  return (value < 0 ? (-value) : value);
}

inline unsigned long AbsVal(unsigned long value)
{
  return value;
}

inline unsigned int AbsVal(unsigned int value)
{
  return value;
}

inline unsigned short AbsVal(unsigned short value)
{
  return value;
}

inline unsigned char AbsVal(unsigned char value)
{
  return value;
}

//-------------------------------
// min and max template functions
//-------------------------------

template <class T>
inline T MinOf(T x1, T x2)
{
  return (x1 < x2 ? x1 : x2);
}

template <class T>
inline T MaxOf(T x1, T x2)
{
  return (x1 > x2 ? x1 : x2);
}

//----------------------
// extra trig functions!
//----------------------
#if !defined(__DJGPP__) & !defined(__GNUC__)
//| defined(_MSC_VER)
inline double asinh(const double& x)
{
  return log(x + sqrt(x * x + 1.0));
}

inline double acosh(const double& x)
{
  return log(x + sqrt(x * x - 1.0));
}

inline double atanh(const double& x)
{
  return log((1.0 + x) / (1.0 - x)) / 2.0;
}

inline long double asinhl(const long double & x)
{
  return logl(x + sqrtl(x * x + 1.0L));
}

inline long double acoshl(const long double & x)
{
  return logl(x + sqrtl(x * x - 1.0L));
}

inline long double atanhl(const long double & x)
{
  return logl((1.0L + x) / (1.0L - x)) / 2.0L;
}
#endif

#endif




