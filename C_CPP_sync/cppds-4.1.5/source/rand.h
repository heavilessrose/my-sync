//----------------------------------------------------------------------------
// SPECIFICATION FILE (rand.h)
// This module exports facilities for pseudorandom number
// generation.
// Machine dependency: long ints must be at least 32 bits (4 bytes).
//----------------------------------------------------------------------------
#ifndef RAND_H
#define RAND_H

#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_TIME_H
  #include <time.h>
  #define INCL_TIME_H
#endif
#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif

void SetSeed(long initSeed);
// PRE:		initSeed >= 1
// POST:        1 <= currentSeed < MAX
// NOTE:	This routine MUST be called prior to NextRand()

float NextRand();
// PRE: 	SetSeed previously invoked at least once
// POST:	FCTVAL == next pseudorandom number
//		&& 0.0 < FCTVAL < 1.0

int GiveRandom(int Num);
// PURPOSE:
//   Random integer function provided for convenience.
//
// PRE:		int Num  --  Upper range of random integer to generate
// POST:	returns a random integer in the range : 0 to Num - 1

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Inline functions for random integers and random generator seed setting
// provided for convenience
//
inline int RANDOM(int Num_)
{
  return GiveRandom(Num_);
}

inline int RAND()
{
  return abs(GiveRandom(INT_MIN));
}

inline void RANDOMIZE()
{
  SetSeed(time(NULL));
}

#endif
//----------------------------------------------------------------------------




