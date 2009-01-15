//=========================================================
//  Numerics Library
//=========================================================
//  FPUtil.cpp
//
//  Various floating-point utilities
//
//  Copyright 1995-96 Scott Robert Ladd
//=========================================================

#ifndef FPUTIL_H
  #include "fputil.h"
#endif

//-----------------------
// Round to nearest value
//-----------------------

double ToNearest(double x)
{
    double i, f, dummy;

    f = fabs(modf(x,&i));

    if (f == 0.0)
        return i;

    if (f == 0.5)
    {
        if (modf(i / 2.0, &dummy) != 0.0)
        {
            if (x < 0.0)
                i -= 1.0;
            else
                i += 1.0;
        }
    }
    else
    {
        if (f > 0.5)
        {
            if (x < 0.0)
                i -= 1.0;
            else
                i += 1.0;
        }
    }

    return i;
}

long double ToNearest(long double x)
{
    double i, f, dummy;

    f = fabs(modf(x,&i));

    if (f == 0.0L)
        return i;

    if (f == 0.5L)
    {
        if (modf(i / 2.0L, &dummy) != 0.0L)
        {
            if (x < 0.0L)
                i -= 1.0L;
            else
                i += 1.0L;
        }
    }
    else
    {
        if (f > 0.5L)
        {
            if (x < 0.0L)
                i -= 1.0L;
            else
                i += 1.0L;
        }
    }

    return i;
}

//--------------------------------------------
// set number of significant digits in a value
//--------------------------------------------

double SigDig(double x, size_t n)
{
    double s, result;

    if (fabs(x) < 1.0E-300)
        result = 0.0;
    else
    {
        if ((n == 0U) || (n > DBL_DIG))
            result = x;
        else
        {
            #ifdef __BORLANDC__
                s = pow10((int)n - 1 - (int)floor(log10(fabs(x))));
            #else
                s = pow(10.0,double((int)n - 1 - (int)floor(log10(fabs(x)))));
            #endif
            result = ToNearest(x * s) / s;
        }
    }

    return result;
}

long double SigDig(long double x, size_t n)
{
    long double shift, result;

    if ((n == 0u) || (n > LDBL_DIG))
        result = x;
    else
    {
        --n;
        
#if defined(__DJGPP__) | defined(_MSC_VER)
        shift = pow(10.0L,(double)n - floor(log10(fabs(x))));
#else
        shift = powl(10.0L,(double)n - floorl(log10l(fabsl(x))));
#endif

        result = ToNearest(x * shift) / shift;
    }

    return result;
}

//------------------------------------------------------
// lowest common denominator and greatest common multiple
//------------------------------------------------------

unsigned long LCM(unsigned long x, unsigned long y)
{
    unsigned long s, l;

    if (x == y)
        return x;

    if (x < y)
    {
        s = x;
        l = y;
    }
    else
    {
        l = x;
        s = y;
    }

    return ((s * l) / GCD(s,l));
}

unsigned long GCD(unsigned long x, unsigned long y)
{
    unsigned long temp;

    while (y != 0ul)
    {
        temp = x % y;
        x = y;
        y = temp;
    }

    return x;
}






