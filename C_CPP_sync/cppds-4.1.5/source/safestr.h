/*****************************************************************************
* FILE		: SAFESTR.H
* PURPOSE	: Safe string wrapper functions header file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: March 4 2003
*****************************************************************************/
#ifndef SAFESTR_H
#define SAFESTR_H
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif

#ifndef PORTABLE_H
  #include "portable.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

/*****************************************************************************/
// Standard byte string functions (safe versions)
inline void* SafeMemSet(void* Str_, int Char_, size_t Max_)
        { return (Str_ ? ::memset(Str_, Char_, Max_):Str_); }
inline void* SafeMemCpy(void* Str1_, const void* Str2_, size_t Max_)
	{ return ((Str1_ && Str2_) ? ::memcpy(Str1_, Str2_, Max_):Str1_); }
inline void* SafeMemMove(void* Str1_, const void* Str2_, size_t Max_)
	{ return ((Str1_ && Str2_) ? ::memmove(Str1_, Str2_, Max_):Str1_); }
inline int SafeMemCmp(const void* Str1_, const void* Str2_, size_t Max_)
        { return ((!Str1_ || !Str2_) ? (Str1_ ? 1:(Str2_ ? -1:0)):memcmp(Str1_, Str2_, Max_)); }

/*****************************************************************************/
// Standard string functions (safe versions)
inline size_t SafeStrLen(const char* Str_)
        { return (Str_ ? ::strlen(Str_):0); }
inline char* SafeStrCpy(char* Str1_, const char* Str2_)
	{ return (Str1_ ? ::strcpy(Str1_, (Str2_ ? Str2_:"")):Str1_); }
inline char* SafeStrnCpy(char* Str1_, const char* Str2_, size_t Max_)
	{ return (Str1_ ? ::strncpy(Str1_, (Str2_ ? Str2_:""), Max_):Str1_); }
inline char* SafeStrCat(char* Str1_, const char* Str2_)
        { return (Str1_ ? ::strcat(Str1_, (Str2_ ? Str2_:"")):Str1_); }
inline char* SafeStrnCat(char* Str1_, const char* Str2_, size_t Max_)
        { return (Str1_ ? ::strncat(Str1_, (Str2_ ? Str2_:""), Max_):Str1_); }
inline int SafeStrCmp(const char* Str1_, const char* Str2_)
        { return ((!Str1_ || !Str2_) ? (Str1_ ? 1:(Str2_ ? -1:0)):strcmp(Str1_, Str2_)); }
inline int SafeStrnCmp(const char* Str1_, const char* Str2_, size_t Max_)
        { return ((!Str1_ || !Str2_) ? (Str1_ ? 1:(Str2_ ? -1:0)):strncmp(Str1_, Str2_, Max_)); }
inline char* SafeStrChr(const char* Str1_, int c)
        { return (Str1_ ? ::strchr(Str1_, c):NULL); }
inline char* SafeStrrChr(const char* Str1_, int c)
        { return (Str1_ ? ::strrchr(Str1_, c):NULL); }
inline size_t SafeStrSpn(const char* Str1_, const char* Accept_)
        { return ((Str1_ && Accept_) ? ::strspn(Str1_, Accept_):0); }
inline size_t SafeStrcSpn(const char* Str1_, const char* Reject_)
        { return ((Str1_ && Reject_) ? ::strcspn(Str1_, Reject_):0); }
inline char* SafeStrpBrk(const char* Str1_, const char* Accept_)
        { return ((Str1_ && Accept_) ? ::strpbrk(Str1_, Accept_):NULL); }
inline char* SafeStrStr(const char* Haystack_, const char* Needle_)
        { return ((Haystack_ && Needle_) ? ::strstr(Haystack_, Needle_):NULL); }
inline char* SafeStrTok(char* Str_, const char* Delim_)
        { return (Delim_ ? ::strtok(Str_, Delim_):NULL); }

// only for unix systems
#if defined(__linux__)
inline char* SafeStrTok_R(char* Str_, const char* Delim_, char** ptrptr_)
        { return ((Delim_ && ptrptr_) ? ::strtok_r(Str_, Delim_, ptrptr_):NULL); }
#endif

/*****************************************************************************/
/*****************************************************************************/
// Standard byte string functions (memory pool fitting versions)
inline void* SafeFitMemSet(void* Str_, int Char_, size_t Max_)
        { return ((Str_ && MemPool::MemFit(Str_, Max_)) ? ::memset(Str_, Char_, Max_):Str_); }
inline void* SafeFitMemCpy(void* Str1_, const void* Str2_, size_t Max_)
	{ return ((Str1_ && Str2_ && MemPool::MemFit(Str1_, Max_)) ? ::memcpy(Str1_, Str2_, Max_):Str1_); }
inline void* SafeFitMemMove(void* Str1_, const void* Str2_, size_t Max_)
	{ return ((Str1_ && Str2_ && MemPool::MemFit(Str1_, Max_)) ? ::memmove(Str1_, Str2_, Max_):Str1_); }

/*****************************************************************************/
// Standard string functions (memory pool fitting versions)
inline char* SafeFitStrCpy(char* Str1_, const char* Str2_)
	{ return ((Str1_ && MemPool::MemFit(Str1_, SafeStrLen(Str2_))) ? ::strcpy(Str1_, (Str2_ ? Str2_:"")):Str1_); }
inline char* SafeFitStrnCpy(char* Str1_, const char* Str2_, size_t Max_)
	{ return ((Str1_ && MemPool::MemFit(Str1_, Max_)) ? ::strncpy(Str1_, (Str2_ ? Str2_:""), Max_):Str1_); }
inline char* SafeFitStrCat(char* Str1_, const char* Str2_)
        { return ((Str1_ && MemPool::MemFit(Str1_, strlen(Str1_) + SafeStrLen(Str2_))) ? ::strcat(Str1_, (Str2_ ? Str2_:"")):Str1_); }
inline char* SafeFitStrnCat(char* Str1_, const char* Str2_, size_t Max_)
        { return ((Str1_ && MemPool::MemFit(Str1_, strlen(Str1_) + Max_)) ? ::strncat(Str1_, (Str2_ ? Str2_:""), Max_):Str1_); }

/*****************************************************************************/
#endif




