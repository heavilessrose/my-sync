// Compiler Definitions
//
//   _MSC_VER        Microsoft C 6.0 and later
//   _QC             Microsoft Quick C 2.51 and later
//   __TURBOC__      Borland Turbo C, Turbo C++, and Borland C++
//   __BORLANDC__    Borland C++
//   __ZTC__         Zortech C++ and Symantec C++
//   __SC__          Symantec C++
//   __WATCOMC__     WATCOM C
//   __POWERC        Mix Power C
//   __MWERKS__      Metro Werks Code Warrior
//   __GNUC__        GNU C/C++
//   __DJGPP__       DJGPP DOS port of GNUC C
//
////////////////////////////////////////////////////////
// Sample Macro Declarations for each compiler brand
//
#if defined(_MSC_VER)
#elif defined(_QC)
#elif defined(__WATCOM__)
#elif defined(__ZTC__)
#elif defined(__TURBOC__)
#elif defined(__GNUC__)
#elif defined(__DJGPP__)
#endif

// Sample definitions for operating systems
//
#if defined(__linux__)
#elif defined(__unix__)
#elif defined(__sgi)
#endif






