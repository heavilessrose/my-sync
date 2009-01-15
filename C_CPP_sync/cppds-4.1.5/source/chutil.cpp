/*****************************************************************************
* FILE		: CHUTIL.CPP
* PURPOSE	: character utilities source file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: Oct 14 1996
*****************************************************************************/
#ifndef CHUTIL_CPP
#define CHUTIL_CPP
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif

/****************************************************************************/
// character / data validation procedures
inline int IsDecc(char x)
	{ return (((unsigned char)x <= 127) && ('0' <= x && x <= '9')); }

inline int IsHexc(char x)
	{ return (IsDecc(x) ||
              (((unsigned char)x <= 127) &&
		         ('A' <= (char)toupper(x) && (char)toupper(x) <= 'F'))); }

inline int IsOctc(char x)
	{ return (((unsigned char)x <= 127) && ('0' <= x && x <= '7')); }

inline int IsBinc(char x)
	{ return (((unsigned char)x <= 127) && (x == '0' || x == '1')); }

inline int IsDecv(int x)
	{ return (0 <= x && x <= 9); }

inline int IsHexv(int x)
	{ return (0 <= x && x <= 0xF); }

inline int IsOctv(int x)
	{ return (0 <= x && x <= 07); }

inline int IsBinv(int x)
	{ return (x == 0 || x == 1); }

inline int IsBasec(char x, int r)
{
  return (((unsigned char)x <= 127) &&
     ((r == 1) ? (x == '1'):
	   (2 <= r && r <= 10) ? ('0' <= x && x <= '1' + (r - 2)):
	   (11 <= r && r <= 36) ?
              ('0' <= x && x <= '9') ||
              ('A' <= toupper(x) && toupper(x) <= 'A' + (r - 11)):0));
}

inline int IsBasev(int x, int r)
{
  return ((r == 1) ? (x == 1):
	  (2 <= r && r <= 10) ? (0 <= x && x <= 1 + (r - 2)):
	  (11 <= r && r <= 36) ?
             (0 <= x && x <= 9) ||
             (10 <= x && x <= 10 + (r - 11)):0);
}        

inline int IsValidDigit(char Digit_, unsigned Radix_)
{
  return (((unsigned char)Digit_ <= 127) &&
          ((Radix_ == 8)  ? (IsDecc(Digit_) && Digit_ <= '7'):
           (Radix_ == 10) ? IsDecc(Digit_):
           (Radix_ == 16) ? IsHexc(Digit_):IsBasec(Digit_, Radix_)));
}        

inline int IsHexPrefix(const char* x)
	{ return (x && strlen(x) && x[0] == '0' && toupper(x[1]) == 'X'); }

inline int IsOctPrefix(const char* x)
	{ return (x && strlen(x) && *x == '0'); }

inline int IsSign(char x)
	{ return (x=='+' || x=='-'); }

inline int IsMathOp(char x)
	{ return (x=='+' || x=='-' || x=='*' || x=='/' || x=='%'); }

inline int IsRelOp(const char* x)
	{ return (
	    x && strlen(x) &&
	    (!strcmp(x, "<") || !strcmp(x, ">") ||
	     !strcmp(x, "<=") || !strcmp(x, ">=") ||
	     !strcmp(x, "==") || !strcmp(x, "!="))); }

inline int IsLogicalOp(const char* x)
	{ return (
	    x && strlen(x) &&
	    (!strcmp(x, "&&") || !strcmp(x, "||") || !strcmp(x, "!"))); }

inline int IsBitwiseOp(const char* x)
	{ return (
	    x && strlen(x) &&
	    (!strcmp(x, "|") || !strcmp(x, "&") ||
	     !strcmp(x, "^") || !strcmp(x, "~") ||
	     !strcmp(x, "<<") || !strcmp(x, ">>"))); }

inline int AssignmentOp(const char* x)
	{ return (x && strlen(x) && x[strlen(x)-1] == '='); }

// l = blink, b = background, f = foreground
inline char VideoAttr(char l, char b, char f)
	{ return (((l<<7)|(b<<4))|f); }

/****************************************************************************/
#endif




