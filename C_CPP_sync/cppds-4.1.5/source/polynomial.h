//=============================================================
//  Numerics Library
//=============================================================
//  Polynomial.h
//
//  Template for polynomials
//
//=============================================================

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef INCL_FLOAT_H
  #include <float.h>
  #define INCL_FLOAT_H
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif
#ifndef INCL_LIMITS_H
  #include "limits.h"
  #define INCL_LIMITS_H
#endif

#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef OBJECTACCEPTOR_H
  #include "objaccpt.h"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif
#ifndef COMPLEX_CPP
  #include "complex.cpp"
#endif

//---------------------------
// Polynomial class
//---------------------------
template <class D>
class Polynomial : public ObjectSkeleton
{
  ROOTOBJECTFNCS_DEFN(Object, Polynomial<D>)

  public:
    static void Xoverflow()
        { THROW (FallibleBase::PolynomialOverflowErr()); }

  protected:
    D * Coeff;
    size_t N;

    void Xalloc() const
        { THROW (FallibleBase::PolynomialAllocationErr()); }
    void Xrange() const
        { THROW (FallibleBase::PolynomialRangeErr()); }        
    void Xtoosmall() const
        { THROW (FallibleBase::PolynomialTooSmallErr()); }
    void Xincompat() const
        { THROW (FallibleBase::IncompatiblePolynomialErr()); }

    Polynomial();

  public:
    // constructor
    Polynomial(size_t terms, const D* array=NULL);
    
    // copy constructor
    Polynomial(const Polynomial<D>& poly);

    // destructor
    ~Polynomial();

    // Virtual constructor and assignment methods
    static Polynomial<D>* Make(size_t terms, const D* array=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Polynomial<D>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::POLYNOMIAL_TYPE; }
    virtual int ClassID() const;

    // assignment operators
    Polynomial<D>& operator = (const Polynomial<D>& poly);
    Polynomial<D>& operator = (const D* array);

    // conversion to array
    operator D * () const;

    // increase degree
    Polynomial<D> Stretch(size_t newN) const;

    // interrogation
    size_t Degree() const;

    // get coefficients
    D Get(size_t term) const;

    D & operator [] (size_t term);

    // evaluate a polynomial for given value
    D operator () (D x) const;

    // operations
    Polynomial<D> operator - () const;
    Polynomial<D> operator + () const;

    Polynomial<D> operator + (const Polynomial<D> & poly) const;

    void operator += (const Polynomial<D> & poly);

    Polynomial<D> operator - (const Polynomial<D> & poly) const;

    void operator -= (const Polynomial<D> & poly);

    Polynomial<D> operator * (const Polynomial<D> & poly) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

//=============================================================
template <class D>
Polynomial<D>::Polynomial()
{
  N = 2;
  Coeff = new D [N];

  if (Coeff == NULL)
    Xalloc();

  for (size_t i = 0; i < N; ++i)
    Coeff[i] = (D)0;
}

//=============================================================
// constructor
template <class D>
Polynomial<D>::Polynomial(size_t terms, const D * array)
{
  if (terms < 2)
    Xtoosmall();

  N = terms;

  Coeff = new D [N];

  if (Coeff == NULL)
    Xalloc();

  if (array == NULL)
  {
    for (size_t i = 0; i < N; ++i)
      Coeff[i] = (D)0;
  }
  else
  {
    for (size_t i = 0; i < N; ++i)
      Coeff[i] = array[i];
  }
}

//=============================================================
// copy constructor
template <class D>
Polynomial<D>::Polynomial(const Polynomial<D>& poly)
{
  N     = poly.N;
  Coeff = new D [N];

  if (Coeff == NULL)
    Xalloc();

  for (size_t i = 0; i < N; ++i)
    Coeff[i] = poly.Coeff[i];
}

//=============================================================
// destructor
template <class D>
Polynomial<D>::~Polynomial()
{
  delete[] Coeff;
}

//=============================================================
template <class D>
Polynomial<D>* Polynomial<D>::Make(size_t terms, const D* array)
{
  return (new Polynomial<D>(terms, array));
}

//=============================================================
template <class D>
Object* Polynomial<D>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Polynomial<D>(*this));

  return NULL;
}

//=============================================================
template <class D>
Polynomial<D>& Polynomial<D>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Polynomial<D> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//=============================================================
template <class D>
const Object& Polynomial<D>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Polynomial<D>::GiveNullObject();
}

//=============================================================
template <class D>    
Object& Polynomial<D>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Polynomial<D>::GiveNullObject();
}

//=============================================================
template <class D>
int Polynomial<D>::ClassID() const
{
  return TypeInfo::POLYNOMIAL_TYPE;
}

//=============================================================
// assignment operator
template <class D>
Polynomial<D>& Polynomial<D>::operator = (const Polynomial<D> & poly)
{
  if (this != &poly)
  {
    if (N != poly.N)
    {
      delete [] Coeff;

      N = poly.N;
      Coeff = new D [N];

      if (Coeff == NULL)
        Xalloc();
    }

    for (size_t i = 0; i < N; ++i)
      Coeff[i] = poly.Coeff[i];
  }

  return *this;
}

//=============================================================
template <class D>
Polynomial<D>& Polynomial<D>::operator = (const D * array)
{
  if (array == NULL)
    return *this;

  for (size_t i = 0; i < N; ++i)
    Coeff[i] = array[i];

  return *this;
}

//=============================================================
// conversion to array
template <class D>
Polynomial<D>::operator D * () const
{
  D * result = new D [N];

  for (size_t i = 0; i < N; ++i)
    result[i] = Coeff[i];

  return result;
}

//=============================================================
// increase degree
template <class D>
Polynomial<D> Polynomial<D>::Stretch(size_t newN) const
{
  if (newN <= N)
    return (*this);

  Polynomial<D> result(newN);

  for (size_t i = 0; i < N; ++i)
    result.Coeff[i] = Coeff[i];

  return result;
}

//=============================================================
// interrogation
template <class D>
inline size_t Polynomial<D>::Degree() const
{
  return N;
}

//=============================================================
// get coefficients
template <class D>
D Polynomial<D>::Get(size_t term) const
{
  if (term >= N)
    Xrange();

  return Coeff[term];
}

//=============================================================
template <class D>
D & Polynomial<D>::operator [] (size_t term)
{
  if (term >= N)
    Xrange();

  return Coeff[term];
}

//=============================================================
// evaluate a polynomial for given value
template <class D>
D Polynomial<D>::operator () (D x) const
{
  D y = Coeff[N - 1];
  size_t i = N - 2;
  
  while(1)
  {
    y = x * y + Coeff[i];

    if (i == 0)
      break;

    --i;
  }

  return y;
}

//=============================================================
// operations
template <class D>
Polynomial<D> Polynomial<D>::operator - () const
{
  Polynomial<D> result(N);

  for (size_t i = 0; i < N; ++ i)
    result.Coeff[i] = -Coeff[i];

  return result;
}

//=============================================================
template <class D>
inline Polynomial<D> Polynomial<D>::operator + () const
{
  return *this;
}

//=============================================================
template <class D>
Polynomial<D> Polynomial<D>::operator + (const Polynomial<D> & poly) const
{
  if (N > poly.N)
  {
    Polynomial<D> res1(*this);

    for (size_t i = 0; i < poly.N; ++i)
      res1.Coeff[i] += poly.Coeff[i];

    return res1;
  }
  else
  {
    Polynomial<D> res2(poly);

    for (size_t i = 0; i < N; ++i)
      res2.Coeff[i] += Coeff[i];

    return res2;
  }
}

//=============================================================
template <class D>
inline void Polynomial<D>::operator += (const Polynomial<D> & poly)
{
  *this = (*this) + poly;
}

//=============================================================
template <class D>
Polynomial<D> Polynomial<D>::operator - (const Polynomial<D> & poly) const
{
  if (N > poly.N)
  {
    Polynomial<D> res1(*this);

    for (size_t i = 0; i < poly.N; ++i)
      res1.Coeff[i] -= poly.Coeff[i];

    return res1;
  }
  else
  {
    Polynomial<D> res2(-poly);

    for (size_t i = 0; i < N; ++i)
      res2.Coeff[i] += Coeff[i];

    return res2;
  }
}

//=============================================================
template <class D>
inline void Polynomial<D>::operator -= (const Polynomial<D> & poly)
{
  *this = (*this) - poly;
}

//=============================================================
template <class D>
Polynomial<D> Polynomial<D>::operator * (const Polynomial<D> & poly) const
{
  if (N != poly.N)
    Xincompat();

  Polynomial<D> result(2 * N - 1);
  
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < N; ++j)
      result[i + j] += Coeff[i] * poly.Coeff[j];
  }

  return result;
}

//=============================================================
#if OVERLOAD_NEW
template <class D>
void* Polynomial<D>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
template <class D>
void Polynomial<D>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//=============================================================
#if HAS_ARRAY_NEW
template <class D>
void* Polynomial<D>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
template <class D>
void Polynomial<D>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

//=============================================================
//-------------
// declarations
//-------------
typedef Polynomial<float>       FPoly;
typedef Polynomial<double>      DPoly;
typedef Polynomial<long double> LPoly;

typedef Polynomial<FComplex>    CFPoly;
typedef Polynomial<DComplex>    CDPoly;
typedef Polynomial<LComplex>    CLPoly;

DPoly FFTMultiply(const DPoly & p1, const DPoly & p2);

//=============================================================
#endif




