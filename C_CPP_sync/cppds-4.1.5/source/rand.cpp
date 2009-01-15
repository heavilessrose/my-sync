//----------------------------------------------------------------------------
// IMPLEMENTATION FILE (rand.cpp)
// This module exports facilities for pseudorandom number
// generation.
// Machine dependency: long ints must be at least 32 bits (4 bytes).
//----------------------------------------------------------------------------
#ifndef RAND_CPP
#define RAND_CPP
#ifndef RAND_H
  #include "rand.h"
#endif

static long currentSeed;

const long MULTIPLIER 	= 16807;
const long MAX 		= 2147483647;
const long QUOT		= 127773;	// Quotient of MAX / MULTIPLIER
const long REM	        = 2836;		// Remainder of MAX / MULTIPLIER

//----------------------------------------------------------------------------
// PRE:		initSeed >= 1
// POST:        1 <= currentSeed < MAX
// NOTE:	This routine MUST be called prior to NextRand()
//----------------------------------------------------------------------------
void SetSeed(long initSeed)
{
  initSeed = initSeed % MAX;
  currentSeed = (initSeed > 0) ? initSeed : 1;
}

//----------------------------------------------------------------------------
// PRE:		1 <= currentSeed < MAX
// POST:	currentSeed ==
//			(currentSeed<entry> * MULTIPLIER) modulo MAX
// 	&& FCTVAL == currentSeed / MAX
// NOTE:	This is a prime modulus multiplicative linear
//		congruential generator that uses the
// 		global variable currentSeed
//----------------------------------------------------------------------------
float NextRand()
{
  long temp = MULTIPLIER * (currentSeed % QUOT) -
	      REM * (currentSeed / QUOT);

  currentSeed = (temp > 0) ? temp : temp + MAX;
  return (float(currentSeed) / float(MAX));
}

//----------------------------------------------------------------------------
// PURPOSE:
//   Random integer function provided for convenience.
//
// PRE:		int Num  --  Upper range of random integer to generate
// POST:	returns a random integer in the range : 0 to Num - 1
//----------------------------------------------------------------------------
int GiveRandom(int Num)
{
  return int(NextRand() * float(Num));
}

//----------------------------------------------------------------------------
#endif




