/****************************************************************************
* FILE		: TYPEDEFS.H
* PURPOSE	: Type definitions class header file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: Oct 14 1996
****************************************************************************/
#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif
#ifndef PORTABLE_H
  #include "portable.h"
#endif

/***************************** Type Definitions ****************************/
typedef unsigned char Byte_t;
typedef size_t Word_t;

typedef unsigned char Uchar;
typedef unsigned int Uint;
typedef unsigned short Ushort;
typedef unsigned long Ulong;
typedef long double Ldouble;

/******************************** Constants ********************************/
/* Program termination codes */
#ifndef EXIT_FAILURE
  #define EXIT_FAILURE 1
#endif
#ifndef EXIT_SUCCESS
  #define EXIT_SUCCESS 0
#endif

// Type Limits
#ifndef ULONG_MAX
#define ULONG_MAX ((unsigned long) ~(unsigned long) 0)
#endif

#ifndef LONG_MAX
#define	LONG_MAX (~(1 << (sizeof (long) * 8 - 1)))
#endif

#ifndef LONG_MIN
#define LONG_MIN (-LONG_MAX - 1)
#endif

#ifndef UINT_MAX
#define UINT_MAX ((unsigned) ~(unsigned) 0)
#endif

#ifndef INT_MAX
#define	INT_MAX (~(1 << (sizeof (int) * 8 - 1)))
#endif

#ifndef INT_MIN
#define INT_MIN (-INT_MAX - 1)
#endif

#ifndef USHRT_MAX
#define USHRT_MAX ((unsigned short) ~(unsigned short) 0)
#endif

#ifndef SHRT_MAX
#define	SHRT_MAX (~(1 << (sizeof (short) * 8 - 1)))
#endif

#ifndef SHRT_MIN
#define SHRT_MIN (-SHRT_MAX - 1)
#endif

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#ifndef UCHAR_MAX
#define UCHAR_MAX ((unsigned char) ~(unsigned char) 0)
#endif

#ifndef CHAR_MAX
#define	CHAR_MAX (~(1 << (sizeof (char) * 8 - 1)))
#endif

#ifndef CHAR_MIN
#define CHAR_MIN (-CHAR_MAX - 1)
#endif

#ifndef SCHAR_MAX
#define SCHAR_MAX CHAR_MAX
#endif

#ifndef SCHAR_MIN
#define SCHAR_MIN CHAR_MIN
#endif

#ifndef SIZET_MAX
#define SIZET_MAX ((size_t) ~(size_t) 0)
#endif

#ifndef PTRDIFFT_MAX
#define PTRDIFFT_MAX ((ptrdiff_t) ~(ptrdiff_t) 0)
#endif

#ifndef PTRDIFFT_MIN
#define PTRDIFFT_MIN (-PTRDIFFT_MAX - 1)
#endif

// Generic conditional macro
#define IF_COND(c,x,y) ((c) ? (x):(y))

// long double math
#if HAS_LONGDOUBLE_STDFNCS
#define SINL(x) ::sinl(x)
#define COSL(x) ::cosl(x)
#define TANL(x) ::tanl(x)
#define ASINL(x) ::asinl(x)
#define ACOSL(x) ::acosl(x)
#define ATANL(x) ::atanl(x)
#define ATAN2L(y,x) ::atan2l(y,x)
#define POWL(x,y) ::powl(x,y)
#define SINHL(x) ::sinhl(x)
#define COSHL(x) ::coshl(x)
#define TANHL(x) ::tanhl(x)
#define EXPL(x) ::expl(x)
#define LOGL(x) ::logl(x)
#define LOG10L(x) ::log10l(x)
#define CEILL(x) ::ceill(x)
#define FLOORL(x) ::floorl(x)
#define FABSL(x) ::fabsl(x)
#define SQRTL(x) ::sqrtl(x)
#define MODFL(x,ip) ::modfl(x,ip)
#define FMODL(x,y) ::fmodl(x,y)
#define FREXP(x,ep) ::frexpl(x,ep)
#define LDEXP(x,y) ::ldexpl(x,y)
#else
#define SINL(x) ::sin(x)
#define COSL(x) ::cos(x)
#define TANL(x) ::tan(x)
#define ASINL(x) ::asin(x)
#define ACOSL(x) ::acos(x)
#define ATANL(x) ::atan(x)
#define ATAN2L(y,x) ::atan2(y,x)
#define POWL(x,y) ::pow(x,y)
#define SINHL(x) ::sinh(x)
#define COSHL(x) ::cosh(x)
#define TANHL(x) ::tanh(x)
#define EXPL(x) ::exp(x)
#define LOGL(x) ::log(x)
#define LOG10L(x) ::log10(x)
#define CEILL(x) ::ceil(x)
#define FLOORL(x) ::floor(x)
#define FABSL(x) ::fabs(x)
#define SQRTL(x) ::sqrt(x)
#define MODFL(x,ip) ::modf(x,ip)
#define FMODL(x,y) ::fmod(x,y)
#define FREXP(x,ep) ::frexp(x,ep)
#define LDEXP(x,y) ::ldexp(x,y)
#endif

/****************************************************************************/
// inline functions
inline short LOWBYTE(short x)			{ return (x&0xFF); }
inline short HIGHBYTE(short x)			{ return ((x&(0xFF<<8))>>8); }
inline short SETHIGHBYTE(short x, short h)	{ return (x|(h<<8)); }

struct FloatTypesPrecision
{
  public:
    enum
    {
      FLOAT = 7,
      DOUBLE = 15,
      LONGDOUBLE = 19
    };
};

struct SortOrder
{
  public:
    enum
    {
      INCREASING = 1,
      DECREASING = -1
    };
};

struct RelationalEnum
{
  enum
  {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL
  };
};

struct LogicalEnum
{
  enum
  {
    AND,
    OR,
    NOT,
    XOR,
    LEFT_SHIFT,
    RIGHT_SHIFT
  };
};

typedef LogicalEnum BitwiseEnum;

/****************************************************************************/
#endif




