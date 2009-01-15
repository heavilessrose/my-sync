#ifndef INCL_STDLIB_H
  #include <stdio.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif

// From: Numerical Recipes in C, page 366
// Modified into template function
//
// Using a combination of Newton-Raphson and bisection, find the root of a function bracketed
// between x1 and x2. The root, returned as the function value rtsafe, will be refined until
// its accuracy is known within +=xacc. funcd is a user-supplied routine that returns both the
// function value and the first derivative of the function
template <class Type>
Type rtsafe(void (*funcd)(const Type&, Type*, Type*), const Type& x1, const Type& x2, const Type& xacc)
{
  void nrerror(char error_text[]);
  int j;
  Type df, dx, dxold, f, fh, fl;
  Type temp, xh, xl, rts;

  (*funcd)(x1, &fl, &df);
  (*funcd)(x2, &fh, &df);

  if ((fl > 0.0 && fh > 0.0) || (fl < 0.0 && fh < 0.0))
    nrerror("Root must be bracketed in rtsafe");

  if (fl == 0.0)
    return x1;

  if (fh == 0.0)
    return x2;

  if (fl < 0.0)         // Orient the search so that f(xl) < 0
  {
    xl = x1;
    xh = x2;
  }
  else
  {
    xh = x1;
    xl = x2;    
  }

  rts = 0.5 * (x1 + x2);                // Initialize the guess for root,
  dxold = fabs(x2 - x1);                // the "stepsize before last,"
  dx = dxold;                           // and the last step
  (*funcd)(rts, &f, &df);

  for (j = 1; j <= MAXIT; j++)          // Loop over allowed iterations
  {
    if ((((rts - xh) * df - f) * ((rts - xl) * df - f) > 0.0) ||        // Bisect if Newton out of range.
        (fabs(2.0 * f) > fabs(dxold * df)))                             // or not decreasing fast enough
    {
      dxold = dx;
      dx = 0.5 * (xh - xl);
      rts = xl + dx;
      if (xl == rts)
        return rts;        
    }
    else                                                                // Change in root is negligible
    {                                                                   // Newton step acceptable. Take it.
      dxold = dx;
      dx = f / df;
      temp = rts;
      rts -= dx;
      if (temp == rts)
        return rts;
    }

    if (fabs(dx) < xacc)        // convergence criterion
      return rts;

    // The one new function evaluation per iteration
    (*funcd)(rts, &f, &df);

    if (f < 0.0)
      xl = rts;
    else
      xh = rts;
  }

  nrerror("Maximum number of iterations exceeded in rtsafe");
  return Type(0.0);      // never get here
}
