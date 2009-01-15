/*****************************************************************************
* FILE		: ASSERTS.CPP
* PURPOSE	: Assertion functions library source file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: March 1 1997
*****************************************************************************/
#ifndef ASSERTS_CPP
#define ASSERTS_CPP
/******************************* Header Files ******************************/
#ifndef ASSERTS_H
  #include "asserts.h"
#endif

/************************* Message Output Functions *************************/
/****************************************************************************/
// PURPOSE:
//   Procedure to output into variable argument string given the
//   format specifier string
//
// PRE:
//   char* Fmt_		: The format specifier string
//
// POST:
//   The arguments for the string is printed onto the static message buffer
//   and returned to the program.
//
const char* MakeVariableStr(const char* Fmt_, ...)
{
  va_list Args_;
  static char Msg_[200];

  va_start(Args_, Fmt_);
  vsprintf(Msg_, Fmt_, Args_);
  va_end(Args_);

  return Msg_;
}

/****************************************************************************/
/****************************************************************************/
Assertion::Assertion():
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
_ErrStream(cerr)
#else
_ErrStream(std::cerr)
#endif
{}

/****************************************************************************/
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
Assertion::Assertion(ostream& ErrStream_):
#else
Assertion::Assertion(std::ostream& ErrStream_):
#endif
_ErrStream(ErrStream_)
{}

/****************************************************************************/
// Procedure to print a message to the standard error device and exit the
// program. This procedure is used by the ASSERT macro
//
// PRE:
//   const char* msg : the message string
//   const char* def : the funtion definition
//   const char* fname : the name of the file
//   unsigned line : the line number in the file
//
// POST:
//   Show message, function definition, filename and the line number in the
//   given file on the output stream specified by the _ErrStream data member
//
void Assertion::ShowWarning(const char* msg, const char* def, const char* fname, unsigned line)
{
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
  ostrstream oss;
  char* string;

  oss <<endl <<((msg==NULL)?def:msg) <<" : " <<fname <<", line " <<line <<ends;
  string = oss.str();
  flush(cout);
  _ErrStream <<string;
  delete string;

#else

  std::ostringstream oss;
  std::string string_;

  oss <<endl <<((msg==NULL)?def:msg) <<" : " <<fname <<", line " <<line <<ends;
  string_ = oss.str();
  flush(cout);
  _ErrStream <<string_;
#endif
}

/****************************************************************************/
// Procedure to print a message to the standard error device and exit the
// program. This procedure is used by the ASSERT macro
//
// PRE:
//   const char* msg : the message string
//   const char* def : the funtion definition
//   const char* fname : the name of the file
//   unsigned line : the line number in the file
//
// POST:
//   Calls ShowWarning to show the given parameters and aborts the program
//   by calling the abort() standard C function.
//
void Assertion::ShowError(const char* msg, const char* def, const char* fname, unsigned line)
{
  ShowWarning(msg, def, fname, line);
  abort();
}

/****************************************************************************/
// Procedure to show a not implemented message composed of the class name and
// the method name then exit the program
//
// PRE:
//   const char* ClassName_ : the name of the class
//   const char* MethodName_ : the name of the method
//
// POST:
//   Shows not implemented message given the specified class and method name
//   then exits
//
void Assertion::NotImplemented(const char* ClassName_, const char* MethodName_)
{
  XSHOWFATAL(MakeVariableStr("Class '%s' does not implement method : '%s'\n", ClassName_, MethodName_), _ErrStream);
}

/****************************************************************************/
// Program to show an error message if the specified condition variable is
// false then exit the program via standard C function abort()
//
// PRE:
//   int safeCondition : Condition needs to be true to avoid error message and
//                       program termination.
//   const char* errMsg : The error message to show if the condition is false.
//
// POST:
//   if safe condition is false the shows message and terminates program
//
void Assertion::Assert(int safeCondition, const char* errMsg)
{
  if (!safeCondition)
  {
    _ErrStream << "***Error: " << errMsg << endl;
    abort();
  }
}

/****************************************************************************/
// Program to show an warning message if the specified condition variable is
// false
//
// PRE:
//   int safeCondition : Condition needs to be true to avoid error message
//   const char* errMsg : The error message to show if the condition is false.
//
// POST:
//   if safe condition is false the shows message.
//
void Assertion::Warn(int safeCondition, const char* errMsg)
{
  if (!safeCondition)
    _ErrStream << "***Warning: " << errMsg << endl;
}

/****************************************************************************/
/****************************************************************************/
#if ASSERTS_DEBUG
#ifndef INCL_CONIO_H
  #include <conio.h>
  #define INCL_CONIO_H
#endif
void main()
{
  int* i;
  int p=0;

  clrscr();

  cout <<endl <<"Testing ASSERT macro" <<endl;
  ASSERT(p,"ASSERT Tested: OK");
}
#endif
/****************************************************************************/
#endif




