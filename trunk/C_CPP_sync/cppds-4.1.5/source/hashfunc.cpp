#ifndef HASHFUNC_CPP
#define HASHFUNC_CPP
#ifndef HASHFUNC_H
  #include "hashfunc.h"
#endif
//****************************************************************************
// PURPOSE:
//   Custom hashing function, By Joseph Wong
//
// PRE:
//   const char* Key_	: The data to be hashed in character string format
//   size_t Len_	: The length of the character string
//   long Size_		: The size of the hash table in number of slots
//
// POST:
//   returns the hash value for the specified piece of data
//   If the hash value is greater or equal to the table size then the
//   hash value modulo the table size is returned
//
Ulong HashJKCW(const char* Key_, size_t Len_, Ulong Size_)
{
  // Hashing constants for this hash function
  static const Ushort HASH_SEED		= 48893;
  static const Ushort MULTIPLIER	= 16807;

  Ulong Index_ = HASH_SEED;
  Index_ *= Len_;

  for (size_t i = 0; i < Len_; i++)
  {
    Index_ *= MULTIPLIER;
    Index_ += Key_[i];
  }

  return ((Index_ >= Size_) ? (Index_ % Size_):Index_);
}

//****************************************************************************
// PURPOSE:
//   HashPJW:
//   An adaptation of Peter Weinberger's (EJW) generic hashing
//   algorithm based on Allen Holub's version. Accepts a pointer
//   to a datum to be hashed and returns an unsigned integer
//
// PRE:
//   const char* datum	: The data to be hashed in character string format
//   size_t Len_	: The length of the character string
//   long Size_		: The size of the hash table in number of slots
//
// POST:
//   returns the hash value for the specified piece of data
//   If the hash value is greater or equal to the table size then the
//   hash value modulo the table size is returned
//
Ulong HashPJW(const char* datum, size_t Len_, Ulong Size_)
{
  const unsigned BITS_IN_int	= ( sizeof(int) * CHAR_BIT );
  const unsigned THREE_QUARTERS	= ((int) ((BITS_IN_int * 3) / 4));
  const unsigned ONE_EIGHTH	= ((int) (BITS_IN_int / 8));
  const unsigned HIGH_BITS	= ( ~((unsigned int)(~0) >> ONE_EIGHTH ));

  Ulong hash_value = 0;

  Ulong i;
  size_t count;

  for (count = 0; count < Len_; ++datum, ++count)
  {
    hash_value = (hash_value << ONE_EIGHTH) + *datum;
    if ((i = hash_value & HIGH_BITS) != 0)
      hash_value = (hash_value ^ (i >> THREE_QUARTERS)) & ~HIGH_BITS;
  }

  return ((hash_value >= Size_) ? (hash_value % Size_):hash_value);
}

//****************************************************************************
// PURPOSE:
//   ElfHash:
//   The published hash algorithm used in the UNIX ELF format
//   for object files. Accepts a pointer to a string to be hashed
//   and returns a long
//
// PRE:
//   const char* name	: The data to be hashed in character string format
//   size_t Len_	: The length of the character string
//   long Size_		: The size of the hash table in number of slots
//
// POST:
//   returns the hash value for the specified piece of data
//   If the hash value is greater or equal to the table size then the
//   hash value modulo the table size is returned
//
Ulong ElfHash(const char* name, size_t Len_, Ulong Size_)
{
  long h = 0, g;
  size_t i;

  for (i = 0; i < Len_; ++i)
  {
    h = (h << 4) + *name++;
    if (g = h & 0xF0000000)
      h ^= g >> 24;
    h &= ~g;
  }

  return ((h >= Size_) ? (h % Size_):h);
}

//****************************************************************************
Ulong DoubleHash1(Ulong Key_, Ulong Size_)
{
  return (Size_ - 2 - (Key_ % (Size_ - 2)));
}

//****************************************************************************
Ulong DoubleHash2(Ulong Key_, Ulong Size_)
{
  return (8 - (Key_ % 8));
}

//****************************************************************************
#endif





