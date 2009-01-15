#ifndef RANDOMIZER_H
#define RANDOMIZER_H

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

// Lehmer Random Number Generator
template <class Type>
class Randomizer
{
  protected:
    long currentSeed;

    long MULTIPLIER;
    long MAX;
    long QUOT;                      // Quotient of MAX / MULTIPLIER
    long REM;                       // Remainder of MAX / MULTIPLIER
  
  public:
    Randomizer();
    Randomizer(long initSeed);

    void SetSeed(long initSeed);
    int GiveRandom(int Num);
    Type NextRand();

    inline int RANDOM(int Num_)
        { return GiveRandom(Num_); }
    inline int RAND()
        { return abs(GiveRandom(INT_MIN)); }
    inline void RANDOMIZE()
        { SetSeed(time(NULL)); }
};

//----------------------------------------------------------------------------
// PRE: uninitialized class data members MULTIPLIER, MAX, QUOT, REM,
//      currentSeed
// POST: MULTIPLER = 16807
//       MAX = 2147483647
//       QUOT = 127773
//       REM = 2836
//       currentSeed = 1
// NOTE: Randomizer class default constructor
//----------------------------------------------------------------------------
template <class Type>
Randomizer<Type>::Randomizer():
currentSeed(0),
MULTIPLIER(16807),
MAX(2147483647),
QUOT(127773),
REM(2836)
{
  SetSeed(0);
}

//----------------------------------------------------------------------------
// PRE: uninitialized class data members MULTIPLIER, MAX, QUOT, REM,
//      currentSeed
// POST: MULTIPLER = 16807
//       MAX = 2147483647
//       QUOT = 127773
//       REM = 2836
//       currentSeed = initSeed % MAX
// NOTE: Randomizer class constructor with initial random seed argument
//----------------------------------------------------------------------------
template <class Type>
Randomizer<Type>::Randomizer(long initSeed):
currentSeed(0),
MULTIPLIER(16807),
MAX(2147483647),
QUOT(127773),
REM(2836)
{
  SetSeed(initSeed);
}

//----------------------------------------------------------------------------
// PRE:		initSeed >= 1
// POST:    1 <= currentSeed < MAX
// NOTE:	This routine MUST be called prior to NextRand()
//----------------------------------------------------------------------------
template <class Type>
void Randomizer<Type>::SetSeed(long initSeed)
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
template <class Type>
Type Randomizer<Type>::NextRand()
{
  long temp = MULTIPLIER * (currentSeed % QUOT) -
	      REM * (currentSeed / QUOT);

  currentSeed = (temp > 0) ? temp : temp + MAX;
  return (Type(currentSeed) / Type(MAX));
}

//----------------------------------------------------------------------------
// PURPOSE:
//   Random integer function provided for convenience.
//
// PRE:		int Num  --  Upper range of random integer to generate
// POST:	returns a random integer in the range : 0 to Num - 1
//----------------------------------------------------------------------------
template <class Type>
int Randomizer<Type>::GiveRandom(int Num)
{
  return int(NextRand() * Type(Num));
}

//----------------------------------------------------------------------------
#endif
