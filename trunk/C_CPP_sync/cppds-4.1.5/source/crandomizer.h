#ifndef CRANDOMIZER_H
#define CRANDOMIZER_H

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
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif

// Central Random Number Generator
template <class Type>
class CentralRandomizer
{
  protected:
    long currentSeed;
    
    long MULTIPLIER;
    long INCREMENT;
    long MODULO;
    Type DIVISOR;

  public:
    CentralRandomizer();
    CentralRandomizer(long initSeed);

    void SetSeed(long initSeed);
    int GiveRandom(int Num);
    Type NextRand();

    inline int RANDOM(int Num_)
        { return GiveRandom(Num_); }
    inline int RAND()
        { return abs(GiveRandom(65535)); }
    inline void RANDOMIZE()
        { SetSeed(time(NULL)); }  
};

//----------------------------------------------------------------------------
// PRE: uninitialized class data members MULTIPLIER, MAX, QUOT, REM,
//      currentSeed
// POST: MULTIPLER = 9301
//       INCREMENT = 49297
//       MODULO = 233280
//       DIVISOR = 233280.0
//       currentSeed = 1
// NOTE: CentralRandomizer class default constructor
//----------------------------------------------------------------------------
template <class Type>
CentralRandomizer<Type>::CentralRandomizer():
currentSeed(0),
MULTIPLIER(9301),
INCREMENT(49297),
MODULO(233280),
DIVISOR(233280.0)
{
  SetSeed(0);
}

//----------------------------------------------------------------------------
// PRE: uninitialized class data members MULTIPLIER, MAX, QUOT, REM,
//      currentSeed
// POST: MULTIPLER = 9301
//       INCREMENT = 49297
//       MODULO = 233280
//       DIVISOR = 233280.0
//       currentSeed = initSeed
// NOTE: CentralRandomizer class constructor with initial random seed argument
//----------------------------------------------------------------------------
template <class Type>
CentralRandomizer<Type>::CentralRandomizer(long initSeed):
currentSeed(0),
MULTIPLIER(9301),
INCREMENT(49297),
MODULO(233280),
DIVISOR(233280.0)
{
  SetSeed(initSeed);
}

//----------------------------------------------------------------------------
// PRE:		initSeed >= 1
// POST:    currentSeed = initSeed
// NOTE:	This routine MUST be called prior to NextRand()
//----------------------------------------------------------------------------
template <class Type>
void CentralRandomizer<Type>::SetSeed(long initSeed)
{
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
Type CentralRandomizer<Type>::NextRand()
{
  long temp = (currentSeed * MULTIPLIER + INCREMENT) % MODULO;
  return (temp / DIVISOR);  
}

//----------------------------------------------------------------------------
// PURPOSE:
//   Random integer function provided for convenience.
//
// PRE:		int Num  --  Upper range of random integer to generate
// POST:	returns a random integer in the range : 0 to Num - 1
//----------------------------------------------------------------------------
template <class Type>
int CentralRandomizer<Type>::GiveRandom(int Num)
{
  double temp = ::ceil(NextRand() * Num);
  return int(temp);
}

//----------------------------------------------------------------------------
#endif
