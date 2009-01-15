//  RandTable.h
//
//  Defines a precalculated, automatically-refreshed set of
//  random numbers.
//
#ifndef RANDTABLE_H
#define RANDTABLE_H

#ifndef RANDDEV_H
  #include "randdev.h"
#endif

#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif

//=============================================================
template <size_t Size>
class RandTable : private RandDev
{
  private:
    size_t Index;
    float  Values[Size];

    // initialize an array
    void Initialize();
    
  public:
    // constructors
    RandTable(unsigned long seed = TimeSeed());
    RandTable(const RandTable<Size> & table);

    // assignment
    void operator = (const RandTable<Size> & table);

    // get next value
    float operator () ();
};

//=============================================================
// constructors
template <size_t Size>
inline RandTable<Size>::RandTable(unsigned long seed):
RandDev(seed)
{
  Initialize();
}

//=============================================================
template <size_t Size>
inline RandTable<Size>::RandTable(const RandTable<Size> & table):
RandDev(table)
{
  Index = table.Index;
  memcpy(Values,table.Values,sizeof(float) * Size);
}

//=============================================================
// assignment
template <size_t Size>
inline void RandTable<Size>::operator = (const RandTable<Size> & table)
{
  Index = table.Index;
  memcpy(Values,table.Values,sizeof(float) * Size);
}

//=============================================================
// get number
template <size_t Size>
float RandTable<Size>::operator () ()
{
  float result = Values[Index];

  ++Index;

  if (Index == Size)
    Initialize();

  return result;
}

//=============================================================
// initialize an array
template <size_t Size>
void RandTable<Size>::Initialize()
{
  for (size_t n = 0; n < Size; ++n)
    Values[n] = RandDev::operator ()();

  Index = 0;
}

//=============================================================
#endif





