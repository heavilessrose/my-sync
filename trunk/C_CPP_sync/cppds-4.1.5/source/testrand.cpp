//=============================================================
//  Numerics Library
//=============================================================
//  TestRand.cpp
//
//  Tests for Random # Generators
//
//  Copyright 1992-1996 by Scott Robert Ladd.
//=============================================================

#include <iomanip.h>

#include "randdev.h"  // uniform deviates
#include "randtable.h" // table of number created by RandGen

void TestRandom(ostream & buffer)
{
    buffer << "Uniform Deviate Test\r\n\r\n";

    const int TEST_SIZE  = 31231;
    const int ARRAY_SIZE =  1000;
    float chiSq = 0.0F;
    int i, j;
    RandDev udev;
    unsigned int numbers[ARRAY_SIZE];

    // perform a chi-square test
    // fill array with zeros
    for (i = 0; i < ARRAY_SIZE; ++i)
         numbers[i] = 0;

    // increment
    for (i = 0; i < TEST_SIZE; ++i)
         numbers[udev(ARRAY_SIZE)]++;

    // calculate chi-square value
    for (i = 0; i < ARRAY_SIZE; ++i)
         chiSq += (float)numbers[i] * (float)numbers[i];

    chiSq = (ARRAY_SIZE * chiSq / TEST_SIZE) - TEST_SIZE;

    // display chi-square value
    buffer << "Chi-Square test value: " << chiSq << "\r\n\r\n";

    // output a set of values for each generator
    for (i = 0; i < 20; ++i)
        {
        for (j = 0; j < 5; ++j)
            {
            buffer << '\t' << setw(10) << udev();
            }

        buffer << "\r\n";
        }

    buffer << "\r\nRandom Number Table Test\r\n\r\n";

    RandTable<100> rtable;

    for (size_t n1 = 0; n1 < 15; ++n1)
        {
        for (size_t n2 = 0; n2 < 10; ++n2)
            {
            buffer << "\t" << setw(3) << rtable();
            }

        buffer << "\r\n";
        }
}

int main()
{
  TestRandom(cout);
  return 0;
}
