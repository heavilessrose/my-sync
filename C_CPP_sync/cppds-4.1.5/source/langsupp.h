// C++ language support
//
#ifndef LANGSUPP_H
#define LANGSUPP_H

#define HAVE_NEW_IOSTREAMS

#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
/* Some useful constants.  */
#define M_E			    2.7182818284590452354   /* e */
#define M_LOG2E			1.4426950408889634074	/* log_2 e */
#define M_LOG10E		0.43429448190325182765	/* log_10 e */
#define M_LN2			0.69314718055994530942	/* log_e 2 */
#define M_LN10			2.30258509299404568402	/* log_e 10 */
#define M_PI			3.14159265358979323846	/* pi */
#define M_PI_2			1.57079632679489661923	/* pi/2 */
#define M_PI_4			0.78539816339744830962	/* pi/4 */
#define M_1_PI			0.31830988618379067154	/* 1/pi */
#define M_2_PI			0.63661977236758134308	/* 2/pi */
#define M_2_SQRTPI		1.12837916709551257390	/* 2/sqrt(pi) */
#define M_SQRT2			1.41421356237309504880	/* sqrt(2) */
#define M_SQRT1_2		0.70710678118654752440	/* 1/sqrt(2) */

/* The above constants are not adequate for computation using `long double's.
   Therefore we provide as an extension constants with similar names as a
   GNU extension.  Provide enough digits for the 128-bit IEEE quad.  */
#define M_El			2.7182818284590452353602874713526625  /* e */
#define M_LOG2El	    1.4426950408889634073599246810018922  /* log_2 e */
#define M_LOG10El		0.4342944819032518276511289189166051  /* log_10 e */
#define M_LN2l			0.6931471805599453094172321214581766  /* log_e 2 */
#define M_LN10l	        2.3025850929940456840179914546843642  /* log_e 10 */
#define M_PIl			3.1415926535897932384626433832795029  /* pi */
#define M_PI_2l	        1.5707963267948966192313216916397514  /* pi/2 */
#define M_PI_4l	        0.7853981633974483096156608458198757  /* pi/4 */
#define M_1_PIl	        0.3183098861837906715377675267450287  /* 1/pi */
#define M_2_PIl	        0.6366197723675813430755350534900574  /* 2/pi */
#define M_2_SQRTPIl		1.1283791670955125738961589031215452  /* 2/sqrt(pi) */
#define M_SQRT2l	    1.4142135623730950488016887242096981  /* sqrt(2) */
#define M_SQRT1_2l		0.7071067811865475244008443621048490  /* 1/sqrt(2) */
#endif

// Use strstream instead of sstream
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__GNUC__)
#  ifdef HAVE_NEW_IOSTREAMS
#    define USE_STRSTREAM           0
#  else
#    define USE_STRSTREAM           1
#  endif
#else
#  define USE_STRSTREAM             1
#endif

// Explicity declaration of typename required
#define TYPENAME_EXPLICIT           1
#if TYPENAME_EXPLICIT
  #define TYPENAME typename
#else
  #define TYPENAME
#endif

// General Definitions
// Use overloaded new/delete operators and custom memory manager
#define OVERLOAD_NEW		1

// array new[] and delete[] operator supported/not
#define HAS_ARRAY_NEW		0

// Long file name feature supported/not
#if defined(__DJGPP__) | defined(__linux__) | defined(__unix__) | defined(__sgi)
#  define HAS_LONGFILENAME        1
#else
#  define HAS_LONGFILENAME        0
#endif

// Name spaces feature supported/not
#define HAS_NAMESPACES          0

// Throwing exceptions supported/not
#if defined(__DJGPP__) | defined(_MSC_VER) | defined(__GNUC__)
#  define HAS_EXCEPTIONS          1
#else
#  define HAS_EXCEPTIONS          0
#endif

// has bool built-in type
#if defined(_MSC_VER) | defined(__GNUC__)
#  define HAS_BOOL                1
#else
#  define HAS_BOOL                0
#endif

// Use STL test configuration file
#define STL_TESTCONFIG          0

// Has non-standard filebuf methods
#define HAS_NONSTD_FILEBUF      0

// Has mutable keyword
#define HAS_MUTABLE             0

// Has standard C math functions for long double type
#if defined(__linux__) | defined(__unix__) & (!defined(__DJGPP__))
#  define HAS_LONGDOUBLE_STDFNCS          1
#else
#  define HAS_LONGDOUBLE_STDFNCS	  0
#endif

#endif




