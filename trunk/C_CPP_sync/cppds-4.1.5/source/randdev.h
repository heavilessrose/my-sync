//=============================================================
//  Numerics Library
//=============================================================
//  RandDev.h
//
//  "Random number" generator that produces statistically-
//  excellent uniform deviates, modified from code published
//  in Chapter 7 of NUMERICAL RECIPES IN C, 2nd ed, after
//  a suggestion by P. L'Ecuyer.
//
//  Copyright 1992-1996 by Scott Robert Ladd.
//=============================================================

#ifndef RANDDEV_H
#define RANDDEV_H

#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef INCL_TIME_H
  #include <time.h>
  #define INCL_TIME_H
#endif
#ifndef INCL_FLOAT_H
  #include <float.h>
  #define INCL_FLOAT_H
#endif

//=============================================================
class RandDev
{
  private:
    long Seed;

  protected:
    // sets default seed argument from system time
    static long TimeSeed()
    {
      return (long)time(NULL);
    }

  public:
    // constructor
    RandDev(long initSeed = TimeSeed());

    // set seed value
    void SetSeed(long newSeed = TimeSeed());

    // get a uniform deviate between 0.0 and 1.0
    float operator () ();

    // get a uniform deviate between 0 and max
    size_t operator () (size_t max);

    int operator () (int max);
};

//=============================================================
inline RandDev::RandDev(long initSeed)
{
  if (initSeed < 0)
    Seed = initSeed;
  else
    Seed = -initSeed;
}

//=============================================================
inline void RandDev::SetSeed(long initSeed)
{
  if (initSeed < 0)
    Seed = initSeed;
  else
    Seed = -initSeed;
}

//=============================================================
inline size_t RandDev::operator () (size_t max)
{
  return size_t(float(max) * (*this)());
}

//=============================================================
inline int RandDev::operator () (int max)
{
  return int(float(max) * (*this)());
}

//=============================================================
#endif




