// String input/output library
//
#ifndef STRIO_H
#define STRIO_H
#ifndef PORTABLE_H
  #include "portable.h"
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef INCL_STDARG_H
  #include <stdarg.h>
  #define INCL_STDARG_H
#endif
#ifndef INCL_STDIO_H
  #include <stdio.h>
  #define INCL_STDIO_H
#endif
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif

#if USE_STRSTREAM
  #if HAS_LONGFILENAME
    #ifndef INCL_SSTREAM_H
      #include <sstream>
      #define INCL_SSTREAM_H
    #endif
  #else
    #ifndef INCL_SSTREAM_H
      #include <sstream>
      #define INCL_SSTREAM_H
    #endif
  #endif
#else
  #ifndef INCL_SSTREAM_H
    #include <sstream>
    #define INCL_SSTREAM_H
  #endif
#endif

#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#if defined(__TURBOC__)
  #ifndef INCL_CONIO_H
    #include <conio.h>
    #define INCL_CONIO_H
  #endif
#endif
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

#define STRIO_DEBUG	0

#if STRIO_DEBUG
#  include "tconsole.h"
#endif

/****************************************************************************/
/* string formatting procedures */
char* MakeVstr(char *Fmt_, ...);
char* MakeVstrInBuf(char* Buf_, char* Fmt_, ...);

/* string input/output procedures */
Boolean ReadLine(istream& In_, char* Dest_, int Size_, char Delimiter_='\n');
size_t TruncLine(istream& In_, char* Dest_, int Size_, char Delimiter_='\n');
size_t Ignore(istream& In_, char Delimiter_);
istream& ReadChar(istream& In_, char& Dest_, char Delimiter_='\n');

/* output formatting procedures */
ostream& Center(ostream& Os_, char* Str_, int LineLen_=80);
void ShowTitle(ostream& Os_, char* Title_, int LineLen_=80);

/* error output functions */
void PrintMsg(ostream& Fp_, char* Msg_);
void ExitMsg(char* Msg_, int ExitCode_=EXIT_FAILURE);

/****************************************************************************/
#endif




