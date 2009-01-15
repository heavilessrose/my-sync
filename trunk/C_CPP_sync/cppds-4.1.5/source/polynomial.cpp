//=============================================================
//  Numerics Library
//=============================================================
//  Polynomial.cpp
//
//  Template for polynomials
//
//  Copyright 1995-1996 by Scott Robert Ladd.
//=============================================================

#ifndef POLYNOMIAL_H
  #include "polynomial.h"
#endif

// constant required by FFT routines
static ComplexNumber<double> PI2I = 6.283185307179586 * ComplexNumber<double>(0.0, 1.0);

//=============================================================
// returns largest power of two that holds n
static size_t log2(size_t n)
{
  // returns 1 if n == 0!
  size_t x = 1, c = 0;

  for (;;)
  {
    if (x >= n)
      break;

    ++c;
    x <<= 1;

    if (x == 0)
      break;
  }

  return c;
}

//=============================================================
// reverses a sequence of bits
static size_t FlipBits(size_t k, size_t bits)
{
  size_t lm = 1 << (bits - 1);
  size_t rm = 1;
  size_t  r = 0;

  while (lm)
  {
    if (k & rm)
      r |= (lm);

    lm >>= 1;
    rm <<= 1;
  }

  return r;
}

//=============================================================
// stretches the length of a polynomial to a power of two
template <class T>
static size_t FFTStretch(Polynomial<T> & p)
{
  size_t n = 1;
  size_t d = p.Degree();

  while (1)
  {
    if (d <= n)
      break;
      
    n <<= 1;

    if (n == 0)
      Polynomial<T>::Xoverflow();
  }

  n <<= 1;

  p = p.Stretch(n);

  return n;
}

//=============================================================
// performs a reverse-bit copy of a DPoly into a new CDPoly
static CDPoly BitRevCopy(const DPoly & p)
{
  size_t n = p.Degree();
  size_t b = log2(n);
  CDPoly a(n);

  for (size_t k = 0; k < n; ++k)
    a[FlipBits(k,b)] = DComplex(p.Get(k));

  return a;
}

//=============================================================
// performs a reverse-bit copy of a CDPoly into a new CDPoly
static CDPoly BitRevCopy(const CDPoly & p)
{
  size_t n = p.Degree();
  size_t b = log2(n);
  CDPoly a(n);

  for (size_t k = 0; k < n; ++k)
    a[FlipBits(k,b)] = p.Get(k);

  return a;
}

//=============================================================
// Fast Fourier transform of DPoly to CDPoly
static CDPoly PolyFFT(const DPoly & p)
{
  size_t n  = p.Degree();
  size_t nl = log2(n);
  size_t j, k, m, m2, s;
  DComplex wm, w, t, u;
  CDPoly a = BitRevCopy(p);

  m  = 2;
  m2 = 1;

  for (s = 0; s < nl; ++s)
  {
    wm = exp(PI2I / double(m));
    w  = DComplex(1.0);

    for (j = 0; j <= (m2 - 1); ++j)
    {
      for (k = j; k <= n - 1; k += m)
      {
        t = w * a[k + m2];
        u = a[k];
        a[k] = u + t;
        a[k + m2] = u - t;
      }

      w *= wm;
    }

    m  <<= 1;
    m2 <<= 1;
  }

  return a;
}

//=============================================================
// inverse FFT of CDPoly to CDPoly
static CDPoly PolyInvFFT(const CDPoly & p)
{
  size_t n  = p.Degree();
  size_t nl = log2(n);
  size_t j, k, m, m2, s;
  DComplex wm, w, t, u;
  CDPoly a = BitRevCopy(p);

  m  = 2;
  m2 = 1;

  for (s = 0; s < nl; ++s)
  {
    wm = exp(-PI2I / double(m));
    w  = DComplex(1.0);

    for (j = 0; j <= (m2 - 1); ++j)
    {
      for (k = j; k <= n - 1; k += m)
      {
        t = w * a[k + m2];
        u = a[k];
        a[k] = u + t;
        a[k + m2] = u - t;
      }

      w *= wm;
    }

    m  <<= 1;
    m2 <<= 1;
  }

  for (j = 0; j < n; ++j)
    a[j] /= double(n);

  return a;
}

//=============================================================
// multiplication routine
DPoly FFTMultiply(const DPoly & p1, const DPoly & p2)
{
  size_t n2, k;

  // duplicate p1 and p2 to preserve originals
  DPoly a1(p1);
  DPoly a2(p2);

  // expand polynomials to next-largest power of two
  if (a1.Degree() > a2.Degree())
  {
    n2 = FFTStretch(a1);
    a2 = a2.Stretch(n2);
  }
  else
  {
    n2 = FFTStretch(a2);
    a1 = a1.Stretch(n2);
  }

  // FFT polynomials
  CDPoly dft1 = PolyFFT(a1);
  CDPoly dft2 = PolyFFT(a2);

  // multiply coefficients
  for (k = 0; k < n2; ++k)
    dft1[k] *= dft2[k];

  // inverse DFT to obtain result
  dft2 = PolyInvFFT(dft1);

  // convert back to DPoly
  DPoly result(n2 - 1);

  for (k = 0; k < n2 - 1; ++k)
    result[k] = dft2[k].RealVal();

  // return result
  return result;
}






