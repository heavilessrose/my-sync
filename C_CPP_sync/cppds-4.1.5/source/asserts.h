/*****************************************************************************
* FILE		: ASSERTS.H
* PURPOSE	: Assertion functions library header file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: March 1 1997
*****************************************************************************/
#ifndef ASSERTS_H
#define ASSERTS_H
/**************************** Standard Libaries *****************************/
#ifndef PORTABLE_H
  #include "portable.h"
#endif

#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERTS_H
#endif
#ifndef INCL_FSTREAM_H
  #include <fstream.h>
  #define INCL_FSTREAM_H
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif

#if USE_STRSTREAM
  #if HAS_LONGFILENAME
    #ifndef INCL_STRSTREAM_H
      #include <strstream.h>
      #define INCL_STRSTREAM_H
    #endif
  #else
    #ifndef INCL_STRSTREAM_H
      #include <strstrea.h>
      #define INCL_STRSTREAM_H
    #endif
  #endif
#else
  #ifndef INCL_SSTREAM_H
    #include <sstream>
    #define INCL_SSTREAM_H
  #endif
#endif

#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_STDARG_H
  #include <stdarg.h>
  #define INCL_STDARG_H
#endif
#ifndef INCL_STDIO_H
  #include <stdio.h>
  #define INCL_STDIO_H
#endif

/******************************** Constants *********************************/
// library driver flag
#define ASSERTS_DEBUG     0

/******************************** Prototypes ********************************/
// To use Assertion class:
//   Assertion() constructor will make an Assertion object with error stream
//   defaulted as cerr.
//   Assertion(ostream& ErrStream_) constructor will make an Assertion object
//   with error stream specified to ErrStream_.
//   ShowError will show the error and terminate program
//   ShowWarning will show the warning and continue program
//   NotImplemented will show the error and terminate program
//
class Assertion
{
  protected:
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
    ostream& _ErrStream;
#else
	std::ostream& _ErrStream;
#endif

  public:
    Assertion();
	#if (!defined(__DJGPP__) & !defined(__linux__))
      Assertion(ostream& ErrStream_);
	#else
      Assertion(std::ostream& ErrStream_);
	#endif

    // Error Display function called by macros
    void ShowError(const char* msg, const char* def, const char* fname, unsigned line);
    void ShowWarning(const char* msg, const char* def, const char* fname, unsigned line);

    // standalone assert & warn functions
    void NotImplemented(const char* ClassName_, const char* MethodName_);
    void Assert(int safeCondition, const char* errMsg);
    void Warn(int safeCondition, const char* errMsg);
};

static const char* MakeVariableStr(const char* Fmt_, ...);

// Error Display function called by macros
inline void ShowError(const char* msg, const char* def, const char* fname, unsigned line)
    { Assertion().ShowError(msg, def, fname, line); }
inline void ShowWarning(const char* msg, const char* def, const char* fname, unsigned line)
    { Assertion().ShowWarning(msg, def, fname, line); }

// standalone assert & warn functions
inline void NotImplemented(const char* ClassName_, const char* MethodName_)
    { Assertion().NotImplemented(ClassName_, MethodName_); }
inline void Assert(int safeCondition, const char* errMsg)
    { Assertion().Assert(safeCondition, errMsg); }
inline void Warn(int safeCondition, const char* errMsg)
    { Assertion().Warn(safeCondition, errMsg); }

/********************************** Macros **********************************/
// TRACE(x) shows the string given by x and the value held by x
// XTRACE(x, strm) does the same thing as TRACE, but with the option to
// specify the output stream
// MARKER(x) outputs an arbitrary string given by x
// XMARKER(x, strm) does the same thing as MARKER, but with the option to
// specify the output stream
//
#define TRACECODE 1
#if TRACECODE
  #define TRACE(x)                  \
    cerr <<(#x " = ") <<x <<endl;
  #define XTRACE(x, strm)           \
    strm <<(#x " = ") <<x <<endl;
  #define MARKER(x)                 \
    cerr <<x <<endl;
  #define XMARKER(x, strm)          \
    strm <<x <<endl;
#else
  #define TRACE(x)
  #define XTRACE(x, strm)
  #define MARKER(x)
  #define XMARKER(x, strm)
#endif

// ASSERT(f, msg) will show the message msg via the ShowError function if the
// condition given by f is false.
// XASSERT(f, msg, strm) does the same thing as ASSERT, but with the option of
// specifying the output stream.
//
#define ASSERTCODE 1
#if ASSERTCODE
  #define ASSERT(f,msg)                 \
    if (f)                              \
      NULL;                             \
    else                                \
      ShowError(msg, "Assertion Failed", __FILE__, __LINE__);

  #define XASSERT(f,msg,strm)           \
    if (f)                              \
      NULL;                             \
    else                                \
      Assertion(strm).ShowError(msg, "Assertion Failed", __FILE__, __LINE__);
#else
  #define ASSERT(f,msg)
  #define XASSERT(f,msg)
#endif

// Usage: SHOWFATAL(error_message)
// error_message will be shown and program will terminate
//
#define SHOWFATAL(msg)                    \
	ShowError(msg, "FATAL ERROR", __FILE__, __LINE__);

// Usage: SHOWERROR(error_message)
// error_message will be shown and program will continue
//
#define SHOWERROR(msg)                    \
	ShowWarning(msg, "ERROR", __FILE__, __LINE__);

// Usage: SHOWWARNING(warning_message)
// warning_message will be shown and program will continue
//
#define SHOWWARNING(msg)               \
	ShowWarning(msg, "WARNING", __FILE__, __LINE__);

// Usage: XSHOWFATAL(error_message, output_stream)
// error_message will be shown on the given output stream and
// program will terminate
//
#define XSHOWFATAL(msg, strm)          \
	Assertion(strm).ShowError(msg, "FATAL ERROR", __FILE__, __LINE__);

// Usage: SHOWERROR(error_message, output_stream)
// error_message will be shown on the given output stream and
// program will continue
//
#define XSHOWERROR(msg, strm)          \
	Assertion(strm).ShowWarning(msg, "ERROR", __FILE__, __LINE__);

// Usage: SHOWWARNING(warning_message, output_stream)
// warning_message will be shown on the given output stream and
// program will continue
//
#define XSHOWWARNING(msg, strm)        \
	Assertion(strm).ShowWarning(msg, "WARNING", __FILE__, __LINE__);

/****************************************************************************/
#endif




