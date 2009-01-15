// FILE : HASHFUNC.H
// PURPOSE:
//   Miscellaneous hashing functions
//
#ifndef HASHFUNC_H
#define HASHFUNC_H
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

Ulong HashJKCW(const char* Key_, size_t Len_, Ulong Size_);
Ulong HashPJW(const char* Key_, size_t Len_, Ulong Size_);
Ulong ElfHash(const char* Key_, size_t Len_, Ulong Size_);

Ulong DoubleHash1(Ulong Key_, Ulong Size_);
Ulong DoubleHash2(Ulong Key_, Ulong Size_);

#endif






