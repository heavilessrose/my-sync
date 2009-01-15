/*****************************************************************************
* FILE		: STRUTIL.H
* PURPOSE	: String utilities class header file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: March 1 1997
*****************************************************************************/
#ifndef STRUTIL_H
#define STRUTIL_H
#ifndef INCL_STDIO_H
  #include <stdio.h>
  #define INCL_STDIO_H
#endif
#if defined(__linux__)
  #ifndef INCL_UNISTD_H
    #include <unistd.h>
    #define INCL_UNISTD_H
  #endif
#endif
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif
#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef COMPARE_CPP
  #include "compare.cpp"
#endif
#ifndef CHUTIL_CPP
  #include "chutil.cpp"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif
#ifndef SAFESTR_H
  #include "safestr.h"
#endif
#ifndef STRIO_H
  #include "strio.h"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif

#define STRUTIL_DEBUG		        0
#define STRUTIL_DEBUG_CONVERT           0
#define STRUTIL_DEBUG_HEXINPUT          0
#define STRUTIL_DEBUG_VALIDATE          0
#define STRUTIL_DEBUG_SUBST	        0
#define STRUTIL_DEBUG_MISCFUNC	        0
#define STRUTIL_DEBUG_REMOVAL	        0
#define STRUTIL_DEBUG_STRCMP	        0
#define STRUTIL_DEBUG_READSTRING	0
#define STRUTIL_DEBUG_BASECONV          0

static const char* ERRMSG_INVALIDRADIX 	= "ERROR : Invalid radix";
static const char* ERRMSG_CONVERT      	= "ERROR : Cannot convert base";

/****************************************************************************/
// local static procedures
static char* ExtractAnyNumber(const char* number, int signedval, int floatval, int scinote, int anynum, int& ptrdiff_, int alloc_=1, int radix=0);
static int IsNumType(const char* value, const char* minstr, const char* maxstr, int radix);
static int FindRadix(const char* Number_, int Radix_);

// Radix determination procedure
int FindRadix(const char* Number_);

// Numeric type value checking procedures
int IsNumber(const char* number, int signedval, int floatval, int scinote, int radix);
int IsSignedNum(const char* value, int radix=0);
int IsUnsignedNum(const char* value, int radix=0);
int IsFloat(const char* value, int radix=0);
int IsInt(const char* value, int radix=0);
int IsUint(const char* value, int radix=0);
int IsShort(const char* value, int radix=0);
int IsUshort(const char* value, int radix=0);
int IsLong(const char* value, int radix=0);
int IsUlong(const char* value, int radix=0);

// Number extraction from string
char* ExtractNumber(const char* number, int signedval, int floatval, int scinote, int& ptrdiff_, int alloc_, int radix);
char* ExtractAnyNum(const char* value, int signedval, int& ptrdiff_, int alloc_=1, int radix=0);
char* ExtractSignedNum(const char* value, int& ptrdiff_, int alloc_=1, int radix=0);
char* ExtractUnsignedNum(const char* value, int& ptrdiff_, int alloc_=1, int radix=0);
char* ExtractFloat(const char* value, int& ptrdiff_, int alloc_=1, int radix=0);

// Radix verification procedures
int IsIntBin(const char* value);
int IsIntOct(const char* value);
int IsIntDec(const char* value);
int IsIntHex(const char* value);

/* number to string conversion functions */
static char* LowIntToStrHelper(int value, char* result, int radix);
static char* LowIntToStr(int value, char* result, int radix);
static char* LowLongToStrHelper(long value, char* result, int radix);
static char* LowLongToStr(long value, char* result, int radix);
static char* LowShortToStrHelper(short value, char* result, int radix);
static char* LowShortToStr(short value, char* result, int radix);
static char* NumToStrHelper(unsigned long value, char* result, int radix);
static char* NumToStr(unsigned long value, int sign, char* result, int radix);
static char* BinToHexHelper(char* str_);

const char* ToBinStr(char ch);
char* FloatToStr(double value, char* result, int width, int prec, char fmt='g');
char* LongFloatToStr(Ldouble value, char* result, int width, int prec, char fmt='g');
char* UlongToStr(unsigned long value, char* result, int radix=10);
char* LongToStr(long value, char* result, int radix=10);
char* UshortToStr(unsigned short value, char* result, int radix=10);
char* ShortToStr(short value, char* result, int radix=10);
char* UintToStr(unsigned int value, char* result, int radix=10);
char* IntToStr(int value, char* result, int radix=10);
char* CharToStr(unsigned char ch, char* result);
char* CharToStr(unsigned char ch);
char* BinToHex(char* str_);
int BytesInCmp2Bin(const char* str_);
char* BinToCmp2Bin(char* str_, int NumBytes_);
char* Cmp2BinToBin(char* str_);

char* AddPosSign(char* result);
char* AddRadixPrefix(char* result, int radix);

#ifndef strtold
#define strtold(np, ep)		StrToLd(np, ep)
#endif
long double StrToLd(const char* nptr, char** endptr);

/* character set functions */
char* MakeCharSet(int(*iscsetfn)(int), const char* src, size_t RunLen_);
char* MakeCharSet(const char* src, size_t RunLen_=0);
inline char* MakeCharSet(int(*iscsetfn)(int))
        { return MakeCharSet(iscsetfn, NULL, 0); }

Boolean InCharSet(char ch, const char* chset);
void CopyCharSet(char* dest, const char* src);
size_t CharSetLen(const char* chset);

////////////////////////////////////////////////////////////////////////////
// Equivalent methods for byte strings (not null terminated char arrays)
////////////////////////////////////////////////////////////////////////////
char* RemoveLeadingBytes(char* str, size_t& RunLen_, const char* chset);
char* RemoveTrailingBytes(char* str, size_t& RunLen_, const char* chset);
char* RemovePaddingBytes(char* str, size_t& RunLen_, const char* chset);
char* RemoveByte(char* str, size_t& RunLen_, const char* chset);

char* SubstNull(char* s, size_t RunLen_, char pad, char nullchar='\0');
char* SubstByte(char* s, size_t RunLen_, const char* chset, char pad);
char* SubstnByte(char* s, size_t RunLen_, const char* chset, char pad, size_t n, size_t index=0);
char* StrByteFill(char* s, size_t RunLen_, char pad, size_t len=0);
size_t CountByte(const char* s, size_t RunLen_, const char* chset);

char* ByteStrReverse(char* str, size_t RunLen_);

// raw bytes allocation procedure
#if OVERLOAD_NEW
inline char* AllocateBytes(size_t Bytes_, MemAllocMethod Method_=MEMMATRIX)
	{ return (char*)RawAllocateWith(Method_, Bytes_); }
#else
inline char* AllocateBytes(size_t Bytes_, MemAllocMethod Method_=STD_NEW)
	{ return (char*)RawAllocateWith(Method_, Bytes_); }
#endif
        
// byte string allocation & copy procedures
char* NewByteString(const char* src, size_t RunLen_, MemAllocMethod Method_=MEMMATRIX);
char* ResizeByteString(char* src, size_t& RunLen_, size_t NewSize_, MemAllocMethod Method_=MEMMATRIX);
char* ReplaceByteString(char* trg, const char* src, size_t RunLen_, MemAllocMethod Method_=MEMMATRIX);

/* String word token procedure */
size_t FindNextByteStr(const char* Str_, size_t RunLen_, const char* Delims_, size_t& Index_, size_t& NextPos_);

// character find functions
size_t ByteStrHasChar(const char* Str_, size_t RunLen_, const char* chset);

////////////////////////////////////////////////////////////////////////////
// string manipulation procedures
////////////////////////////////////////////////////////////////////////////
char* RemoveLeading(char* str, const char* chset);
char* RemoveTrailing(char* str, const char* chset);
char* RemovePadding(char* str, const char* chset);
char* RemoveChar(char* str, const char* chset);

inline char* SubstChar(char* s, const char* chset, char pad)
	{ return SubstByte(s, SafeStrLen(s), chset, pad); }
inline char* SubstnChar(char* s, const char* chset, char pad, size_t n, size_t index=0)
	{ return SubstnByte(s, SafeStrLen(s), chset, pad, n, index); }
inline char* StrFill(char* s, char pad, size_t len=0)
	{ return StrByteFill(s, SafeStrLen(s), pad, len); }
inline size_t CountChar(const char* s, const char* chset)
	{ return CountByte(s, SafeStrLen(s), chset); }

inline char* StrReverse(char* str)
	{ return ByteStrReverse(str, SafeStrLen(str)); }

// string allocation & copy procedures
char* NewString(const char* src, MemAllocMethod Method_=MEMMATRIX);
char* ResizeString(char* src, size_t NewSize_, MemAllocMethod Method_=MEMMATRIX);
char* ReplaceString(char* trg, const char* src, MemAllocMethod Method_=MEMMATRIX);
char* EraseString(char* str);

/* String word token procedure */
inline size_t FindNextWord(const char* Str_, const char* Delims_, size_t& Index_, size_t& NextPos_)
	{ return FindNextByteStr(Str_, SafeStrLen(Str_), Delims_, Index_, NextPos_); }

// character find functions
inline size_t StrHasChar(const char* Str_, const char* chset)
        { return ByteStrHasChar(Str_, SafeStrLen(Str_), chset); }

// function delimiters functions
char* RetrieveFunctionArguments(const char* SubList_, int* DelimArray_);
int FindFunctionDelimiters(const char* list, size_t len, int** DelimArrayPtr_, size_t ArraySize_,
                           const char* LeftBrackets_, const char* RightBrackets_, char SepChar_=',',
                           char QuoteChar_='\"', MemAllocMethod Method_=MEMMATRIX);

// string uppercase/lowercase conversion functions
char* UpCase(char* Str_);
char* DownCase(char* Str_);

// if pre-defined macro of case insensitive string comparison function is
// absent then define it and set it to call the user defined version
#ifndef strcmpi
#define strcmpi(s1, s2)		StriComp(s1, s2)
#endif
int StriComp(const char* s1, const char* s2);

// if pre-defined macro of case insensitive specific length string comparison
// function is absent then define it and set it to call the user defined
// version
#ifndef strncmpi
#define strncmpi(s1, s2, m)     StrniComp(s1, s2, m)
#endif
int StrniComp(const char* s1, const char* s2, size_t max);

// Memory block case-insensitive comparison method
int MemiComp(const char* s1, const char* s2, size_t max);

// arbitrary length string extraction function
char* ReadStringUntil(istream& In_, char* Dest_, int& Size_, char Delimiter_='\n', int Max_=30000);
int ReadStringPart(istream& In_, char* Dest_, int Size_, char Delimiter_, int RdStatus_=-1);

/****************************************************************************/
#endif




