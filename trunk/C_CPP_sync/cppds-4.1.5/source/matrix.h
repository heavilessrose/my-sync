//=============================================================
//  Numerics Library
//=============================================================
//  Matrix.h
//
//  Templates for matrices
//
//=============================================================

#ifndef MATRIX_H
#define MATRIX_H

#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
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
#ifndef FPUTIL_H
  #include "fputil.h"
#endif

//------------------------------------
// Grid: A two-dimensional array class
//------------------------------------
template <class T>
class Grid : public ObjectSkeleton
{
  ROOTOBJECTFNCS_DEFN(Object, Grid<T>)

  friend Grid<T> Apply(const Grid<T>& m, T (*func)(const T& n))
  {
    Grid<T> result(m);
    for (size_t i = 0; i < m.N; ++i)
      result.Data[i] = func(result.Data[i]);
    return result;
  }

  protected:
    T * Data;
    size_t R;
    size_t C;
    size_t N;

    size_t Index(size_t row, size_t col) const;

    void Xalloc() const
        { THROW (FallibleBase::MatrixAllocationErr()); }
    void Xincompat() const
        { THROW (FallibleBase::IncompatibleMatrixErr()); }
    void Xindex() const
        { THROW (FallibleBase::InvalidIndexErr()); }
    void Xsingular() const
        { THROW (FallibleBase::SingularMatrixErr()); }
    void Xtoobig() const
        { THROW (FallibleBase::MatrixTooBigErr()); }
    void Xzerodim() const
        { THROW (FallibleBase::ZeroDimensionErr()); }
    void Xzerodiv() const
        { THROW (FallibleBase::ZeroDivisionErr()); }

  public:
    // constructors
    Grid();
    Grid(size_t rows, size_t cols);
    Grid(const Grid<T>& m);

    // destructors
    virtual ~Grid();

    // Virtual constructor and assignment methods
    static Grid<T>* Make();
    static Grid<T>* Make(size_t rows, size_t cols);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Grid<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::GRID_TYPE; }
    virtual int ClassID() const;

    // assignment operator
    Grid<T>& operator = (const Grid<T> & m);

    Grid<T>& operator = (const T * array);

    // interrogation
    size_t GetRows() const;
    size_t GetCols() const;

    bool IsVector() const;
    bool IsRowVector() const;
    bool IsColVector() const;
    bool IsSquare() const;

    // retrieve elements
    T Get(size_t row, size_t col) const;
    T & operator () (size_t row, size_t col);

    // internal function w/o exception check!
    T & Elem(size_t row, size_t col) const;

    // apply a function to each element
    void Apply(T (* func)(const T & n));

    // fill matrix with specific value
    void Fill(const T & x);

    // create a row matrix
    Grid<T> VectorRow(size_t row);

    // create a column vector
    Grid<T> VectorCol(size_t col);

    // transpose a matrix
    Grid<T> Transpose();

    // change size (destroying contents)
    void Resize(size_t rows, size_t cols);

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
template <class T>
inline size_t Grid<T>::Index(size_t row, size_t col) const
{
  return (row * C + col);
}

//=============================================================
template <class T>
Grid<T>::Grid()
{
  R = 1;
  C = 1;
  N = 1;

  Data = New<T>(N);

  if (Data == NULL)
    Xalloc();
}

//=============================================================
template <class T>
Grid<T>::Grid(size_t rows, size_t cols)
{
  R = rows;
  C = cols;
  N = rows * cols;

  if ((R == 0) || (C == 0))
    Xzerodim();

  if ((N < R) || (N < C))
    Xtoobig();

  Data = New<T>(N);

  if (Data == NULL)
    Xalloc();
}

//=============================================================
template <class T>
Grid<T>::Grid(const Grid<T> & m)
{
  R = m.R;
  C = m.C;
  N = m.N;

  Data = New<T>(N);

  if (Data == NULL)
    Xalloc();

  for (size_t i = 0; i < N; ++i)
    Data[i] = m.Data[i];
}

//=============================================================
// destructors
template <class T>
Grid<T>::~Grid()
{
  ::DeleteArray(Data);
}

//=============================================================
template <class T>
Grid<T>* Grid<T>::Make()
{
  return (new Grid<T>());
}

//=============================================================
template <class T>
Grid<T>* Grid<T>::Make(size_t rows, size_t cols)
{
  return (new Grid<T>(rows, cols));
}

//=============================================================
template <class T>
Object* Grid<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Grid<T>(*this));

  return NULL;
}

//=============================================================
template <class T>
Grid<T>& Grid<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Grid<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//=============================================================
template <class T>
const Object& Grid<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Grid<T>::GiveNullObject();
}

//=============================================================
template <class T>
Object& Grid<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Grid<T>::GiveNullObject();
}

//=============================================================
template <class T>
int Grid<T>::ClassID() const
{
  return TypeInfo::GRID_TYPE;
}

//=============================================================
// assignment operator
template <class T>
Grid<T>& Grid<T>::operator = (const Grid<T> & m)
{
  if (this != &m)
  {
    R = m.R;
    C = m.C;
    N = m.N;

    ::DeleteArray(Data);

    Data = New<T>(N);

    if (Data == NULL)
      Xalloc();

    for (size_t i = 0; i < N; ++i)
      Data[i] = m.Data[i];
  }

  return *this;
}

//=============================================================
template <class T>
Grid<T>& Grid<T>::operator = (const T * array)
{
  // note: must assume that array has N members
  const T * aptr = array;
  T * mptr = Data;

  for (size_t i = 0; i < N; ++i)
  {
    (*mptr) = (*aptr);
    ++aptr;
    ++mptr;
  }

  return *this;
}

//=============================================================
// interrogation
template <class T>
inline size_t Grid<T>::GetRows() const
{
  return R;
}

//=============================================================
template <class T>
inline size_t Grid<T>::GetCols() const
{
  return C;
}

//=============================================================
template <class T>
inline bool Grid<T>::IsVector() const
{
  return ((R == 1) || (C == 1));
}

//=============================================================
template <class T>
inline bool Grid<T>::IsRowVector() const
{
  return (C == 1);
}

//=============================================================
template <class T>
inline bool Grid<T>::IsColVector() const
{
  return (R == 1);
}

//=============================================================
template <class T>
inline bool Grid<T>::IsSquare() const
{
  return (R == C);
}

//=============================================================
template <class T>
T Grid<T>::Get(size_t row, size_t col) const
{
  if ((row >= R) || (col >= C))
    Xindex();

  return Data[Index(row,col)];
}

//=============================================================
template <class T>
T & Grid<T>::operator () (size_t row, size_t col)
{
  if ((row >= R) || (col >= C))
    Xindex();
    
  return Data[Index(row,col)];
}

//=============================================================
template <class T>
inline T & Grid<T>::Elem(size_t row, size_t col) const
{
  return Data[Index(row,col)];
}

//=============================================================
// apply a function to each element
template <class T>
void Grid<T>::Apply(T (* func)(const T & n))
{
  for (size_t i = 0; i < N; ++i)
    Data[i] = func(Data[i]);
}

//=============================================================
// fill matrix with specific value
template <class T>
void Grid<T>::Fill(const T & x)
{
  for (size_t i = 0; i < N; ++i)
    Data[i] = x;
}

//=============================================================
// create a row matrix
template <class T>
Grid<T> Grid<T>::VectorRow(size_t row)
{
  if (row >= R)
    Xindex();

  Grid<T> vector(C,1);

  for (size_t i = 0; i < C; ++i)
    vector(i,0) = Data[Index(row,i)];

  return vector;
}

//=============================================================
// create a column vector
template <class T>
Grid<T> Grid<T>::VectorCol(size_t col)
{
  if (col >= C)
    Xindex();

  Grid<T> vector(1,R);

  for (size_t i = 0; i < R; ++i)
    vector(0,i) = Data[Index(i,col)];

  return vector;
}

//=============================================================
// transpose a matrix
template <class T>
Grid<T> Grid<T>::Transpose()
{
  Grid<T> result(C,R);

  const T * tptr = Data;
  T * rptr;

  for (size_t i = 0; i < R; ++i)
  {
    rptr = result.Data + i;

    for (size_t j = 0; j < C; ++j)
    {
      (*rptr) = (*tptr);
      ++tptr;
      rptr += R;
    }
  }

  return result;
}

//=============================================================
template <class T>
void Grid<T>::Resize(size_t rows, size_t cols)
{
  ::DeleteArray(Data);

  R = rows;
  C = cols;
  N = rows * cols;

  if ((R == 0) || (C == 0))
    Xzerodim();

  if ((N < R) || (N < C))
    Xtoobig();

  Data = New<T>(N);

  if (Data == NULL)
    Xalloc();
}

//=============================================================
#if OVERLOAD_NEW
template <class T>
void* Grid<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
template <class T>
void Grid<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//=============================================================
#if HAS_ARRAY_NEW
template <class T>
void* Grid<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
template <class T>
void Grid<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

//-----------------------------------------------------
//  Matrix: A two-dimensional mathematical matrix class
//-----------------------------------------------------
template <class T>
class Matrix : public Grid<T>
{
  protected:
    // internal recursive function for determinant
    T DetRecursive();

  public:
    // constructors
    Matrix();
    Matrix(size_t rows, size_t cols, const T & init = T(0));
    Matrix(const Matrix<T>& m);
    Matrix(const Grid<T>& m);

    // Virtual constructor and assignment methods
    static Matrix<T>* Make();
    static Matrix<T>* Make(size_t rows, size_t cols, const T& init = T(0));
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Matrix<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::MATRIX_TYPE; }
    virtual int ClassID() const;

    // assignment operator
    Matrix<T>& operator = (const Matrix<T> & m);
    Matrix<T>& operator = (const Grid<T> & m);
    Matrix<T>& operator = (const T * array);

    // interrogation
    bool IsZero() const;
    bool IsDiagonal() const;
    bool IsIdentity() const;
    bool IsTridiagonal() const;
    bool IsUpperTriangular() const;
    bool IsLowerTriangular() const;
    bool IsPermutation() const;
    bool IsSingular() const;

    // scalar addition
    Matrix<T> operator + (const Matrix<T> & m);
    void operator += (const Matrix<T> & m);
    Matrix<T> operator + (const T & x);
    void operator += (const T & x);

    // scalar subtraction
    Matrix<T> operator - (const Matrix<T> & m);
    void operator -= (const Matrix<T> & m);
    Matrix<T> operator - (const T & x);
    void operator -= (const T & x);

    // scalar multiplication
    Matrix<T> operator * (const Matrix<T> & m);
    void operator *= (const Matrix<T> & m);
    Matrix<T> operator * (const T & x);
    void operator *= (const T & x);

    // scalar division
    Matrix<T> operator / (const Matrix<T> & m);
    void operator /= (const Matrix<T> & m);
    Matrix<T> operator / (const T & x);
    void operator /= (const T & x);

    // matrix multiplication
    Matrix<T> operator % (const Matrix<T> & m);

    // comparison operators
    bool Equals(const Matrix<T> & m);
    Matrix<bool> operator == (const Matrix<T> & m);
    Matrix<bool> operator != (const Matrix<T> & m);
    Matrix<bool> operator < (const Matrix<T> & m);
    Matrix<bool> operator <= (const Matrix<T> & m);
    Matrix<bool> operator > (const Matrix<T> & m);
    Matrix<bool> operator >= (const Matrix<T> & m);

    // negate a matrix
    Matrix<T> operator - ();

    // change size (destroying contents)
    void Resize(size_t rows, size_t cols, const T & init = T(0));

    // inner and outer products
    T InnerProduct(const Matrix<T> & m);

    // calculation euclidean norm
    double Norm();

    // calculate determinant value
    T Determinant();

    // create a minor matrix
    Matrix<T> Minor(size_t rdel, size_t cdel);

    // solve system of linear equations
    Matrix<T> LinSolve();

    // LUP decomposition
    Matrix<size_t> LUPDecompose();

    // LUP decomposition (call w/ result of LUPDecomp)
    Matrix<T> LUPSolve(const Matrix<size_t> & perm, const Matrix<T> & b);

    // LUP inversion (call w/ result of LUPDecomp)
    Matrix<T> LUPInvert(const Matrix<size_t> & perm);

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
// constructors
template <class T>
inline Matrix<T>::Matrix():
Grid<T>()
{
  *Data = T(0);
}

//=============================================================
template <class T>
Matrix<T>::Matrix(size_t rows, size_t cols, const T & init):
Grid<T>(rows,cols)
{
  for (size_t i = 0; i < N; ++i)
    Data[i] = init;
}

//=============================================================
template <class T>
inline Matrix<T>::Matrix(const Matrix<T> & m):
Grid<T>(m)
{
  // place holder
}

//=============================================================
template <class T>
inline Matrix<T>::Matrix(const Grid<T> & m):
Grid<T>(m)
{
  // place holder
}

//=============================================================
template <class T>
Matrix<T>* Matrix<T>::Make()
{
  return (new Matrix<T>());
}

//=============================================================
template <class T>
Matrix<T>* Matrix<T>::Make(size_t rows, size_t cols, const T& init)
{
  return (new Matrix<T>(rows, cols, init));
}

//=============================================================
template <class T>    
Object* Matrix<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Matrix<T>(*this));

  return NULL;
}

//=============================================================
template <class T>
Matrix<T>& Matrix<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Matrix<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//=============================================================
template <class T>
const Object& Matrix<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Matrix<T>::GiveNullObject();
}

//=============================================================
template <class T>    
Object& Matrix<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Matrix<T>::GiveNullObject();
}

//=============================================================
template <class T>    
int Matrix<T>::ClassID() const
{
  return TypeInfo::MATRIX_TYPE;
}

//=============================================================
// assignment operator
template <class T>
inline Matrix<T>& Matrix<T>::operator = (const Matrix<T> & m)
{
  if (this != &m)
    Grid<T>::operator = (m);
  return *this;
}

//=============================================================
template <class T>
inline Matrix<T>& Matrix<T>::operator = (const Grid<T> & m)
{
  if (this != &m)
    Grid<T>::operator = (m);
  return *this;
}

//=============================================================
template <class T>
inline Matrix<T>& Matrix<T>::operator = (const T * array)
{
  Grid<T>::operator = (array);
  return *this;
}

//=============================================================
// interrogation
template <class T>
bool Matrix<T>::IsZero() const
{
  const T * ptr = Data;
  
  for (size_t i = 0; i < N; ++i)
  {
    if ((*ptr) != (T)0)
      return false;

    ++ptr;
  }

  return true;
}

//=============================================================
template <class T>
bool Matrix<T>::IsDiagonal() const
{
  if (C != R)
    return false;

  const T * ptr = Data;

  for (size_t ir = 0; ir < R; ++ir)
  {
    for (size_t ic = 0; ic < C; ++ic)
    {
      if (ir == ic)
      {
        if ((*ptr) == (T)0)
          return false;
      }
      else
      {
        if ((*ptr) != (T)0)
          return false;
      }

      ++ptr;
    }
  }

  return true;
}

//=============================================================
template <class T>
bool Matrix<T>::IsIdentity() const
{
  if (C != R)
    return false;

  const T * ptr = Data;

  for (size_t ir = 0; ir < R; ++ir)
  {
    for (size_t ic = 0; ic < C; ++ic)
    {
      if (ir == ic)
      {
        if ((*ptr) != (T)1)
          return false;
      }
      else
      {
        if ((*ptr) != (T)0)
          return false;
      }
      
      ++ptr;
    }
  }

  return true;
}

//=============================================================
template <class T>
bool Matrix<T>::IsTridiagonal() const
{
  if ((C != R) || (C < 3))
    return false;

  const T * ptr = Data;

  for (size_t ir = 0; ir < R; ++ir)
  {
    for (size_t ic = 0; ic < C; ++ic)
    {
      if (ir != ic)
      {
        if (ir > ic)
        {
          if ((ir - ic) > 1)
          {
            if ((*ptr) != (T)0)
              return false;
          }
        }
        else
        {
          if ((ic - ir) > 1)
          {
            if ((*ptr) != (T)0)
            return false;
          }
        }
      }

      ++ptr;
    }
  }

  return true;
}

//=============================================================
template <class T>
bool Matrix<T>::IsUpperTriangular() const
{
  if ((C != R) || (C < 2))
    return false;

  size_t steps = 1;
  const T * ptr = Data + C;
  
  for (size_t ir = 1; ir < R; ++ir)
  {
    for (size_t s = 0; s < steps; ++s)
    {
      if (ptr[s] != (T)0)
        return false;
    }
    
    ++steps;
    ptr += C;
  }

  return true;
}

//=============================================================
template <class T>
bool Matrix<T>::IsLowerTriangular() const
{
  if ((C != R) || (C < 2))
    return false;

  size_t steps = C - 1;
  const T * ptr = Data;

  for (size_t ir = 1; ir < R; ++ir)
  {
    for (size_t s = steps; s > 0; --s)
    {
      if (ptr[s] != (T)0)
        return false;
    }
    
    --steps;
    ptr += C + 1;
  }

  return true;
}

//=============================================================
template <class T>
bool Matrix<T>::IsPermutation() const
{
  if (C != R)
    return false;

  char * ctags = new_char_array(C, NULL);

  if (ctags == NULL)
    Xalloc();

  char * rtags = new_char_array(R, NULL);

  if (rtags == NULL)
    Xalloc();

  memset(ctags,0,C);
  memset(rtags,0,R);

  bool result = true;
  const T * ptr = Data;

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      if ((*ptr))
      {
        if (((*ptr) > 1) ||
            (rtags[ri] == 1) ||
            (ctags[ci] == 1))
        {
          result = false;
          goto finished;
        }

        rtags[ri] = 1;
        ctags[ci] = 1;
      }

      ++ptr;
    }
  }

  // a goto label!
  finished:

  ::DeleteArray(ctags);
  ::DeleteArray(rtags);
  return result;
}

//=============================================================
template <class T>
bool Matrix<T>::IsSingular() const
{
  if (C != R)
    return false;

  Matrix<T> csum(1,C), rsum(R,1);
  const T * ptr = Data;

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      csum(0,ci) += *ptr;
      rsum(ri,0) += *ptr;
      ++ptr;
    }
  }

  for (size_t i = 0; i < R; ++i)
  {
    if ((csum(0,i) == 0) || (rsum(i,0) == 0))
      return true;
  }

  return false;
}

//=============================================================
// addition
template <class T>
Matrix<T> Matrix<T>::operator + (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<T> result(m);

  for (size_t i = 0; i < N; ++i)
    result.Data[i] += Data[i];

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator += (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  for (size_t i = 0; i < N; ++i)
    Data[i] += m.Data[i];
}

//=============================================================
template <class T>
Matrix<T> Matrix<T>::operator + (const T & x)
{
  Matrix<T> result(*this);

  for (size_t i = 0; i < N; ++i)
    result.Data[i] += x;

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator += (const T & x)
{
  for (size_t i = 0; i < N; ++i)
    Data[i] += x;
}

//=============================================================
// subtraction
template <class T>
Matrix<T> Matrix<T>::operator - (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<T> result(*this);

  for (size_t i = 0; i < N; ++i)
    result.Data[i] -= m.Data[i];

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator -= (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  for (size_t i = 0; i < N; ++i)
    Data[i] -= m.Data[i];
}

//=============================================================
template <class T>
Matrix<T> Matrix<T>::operator - (const T & x)
{
  Matrix<T> result(*this);

  for (size_t i = 0; i < N; ++i)
    result.Data[i] -= x;

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator -= (const T & x)
{
  for (size_t i = 0; i < N; ++i)
    Data[i] -= x;
}

//=============================================================
// scalar product
template <class T>
Matrix<T> Matrix<T>::operator * (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<T> result(m);

  for (size_t i = 0; i < N; ++i)
    result.Data[i] *= Data[i];

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator *= (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  for (size_t i = 0; i < N; ++i)
    Data[i] *= m.Data[i];
}

//=============================================================
template <class T>
Matrix<T> Matrix<T>::operator * (const T & x)
{
  Matrix<T> result(*this);

  for (size_t i = 0; i < N; ++i)
    result.Data[i] *= x;

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator *= (const T & x)
{
  for (size_t i = 0; i < N; ++i)
    Data[i] *= x;
}

//=============================================================
// division
template <class T>
Matrix<T> Matrix<T>::operator / (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<T> result(*this);

  for (size_t i = 0; i < N; ++i)
  {
    if (m.Data[i] == (T)0)
      Xzerodiv();
      
    result.Data[i] /= m.Data[i];
  }

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator /= (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  for (size_t i = 0; i < N; ++i)
  {
    if (m.Data[i] == (T)0)
      Xzerodiv();

    Data[i] /= m.Data[i];
  }
}

//=============================================================
template <class T>
Matrix<T> Matrix<T>::operator / (const T & x)
{
  if (x == (T)0)
    Xzerodiv();

  Matrix<T> result(*this);

  for (size_t i = 0; i < N; ++i)
    result.Data[i] /= x;

  return result;
}

//=============================================================
template <class T>
void Matrix<T>::operator /= (const T & x)
{
  if (x == (T)0)
    Xzerodiv();

  for (size_t i = 0; i < N; ++i)
    Data[i] /= x;
}

//=============================================================
// matrix multiplication
template <class T>
Matrix<T> Matrix<T>::operator % (const Matrix<T> & m)
{
  if (C != m.R)
    Xincompat();

  Matrix<T> result(R,m.C);

  T * rptr = result.Data;
  const T * tptr, * mptr;

  for (size_t i = 0; i < R; ++i)
  {
    for (size_t j = 0; j < m.C; ++j)
    {
      tptr =   Data + (i * R);
      mptr = m.Data +  j;

      for (size_t k = 0; k < C; ++k)
      {
        (*rptr) += (*tptr) * (*mptr);
        ++tptr;
        mptr += m.C;
      }

      ++rptr;
    }
  }

  return result;
}

//=============================================================
// comparison operators
template <class T>
bool Matrix<T>::Equals(const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      if (Data[Index(ri,ci)] != m.Data[Index(ri,ci)])
        return false;
    }
  }

  return true;
}

//=============================================================
template <class T>
Matrix<bool> Matrix<T>::operator == (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<bool> result(R,C);

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      result(ri,ci) = (Data[Index(ri,ci)] == m.Data[Index(ri,ci)]);
    }
  }

  return result;
}

//=============================================================
template <class T>
Matrix<bool> Matrix<T>::operator != (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<bool> result(R,C);

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      result(ri,ci) = (Data[Index(ri,ci)] != m.Data[Index(ri,ci)]);
    }
  }

  return result;
}

//=============================================================
template <class T>
Matrix<bool> Matrix<T>::operator < (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<bool> result(R,C);

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      result(ri,ci) = (Data[Index(ri,ci)] < m.Data[Index(ri,ci)]);
    }
  }

  return result;
}

//=============================================================
template <class T>
Matrix<bool> Matrix<T>::operator <= (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<bool> result(R,C);

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      result(ri,ci) = (Data[Index(ri,ci)] <= m.Data[Index(ri,ci)]);
    }
  }

  return result;
}

//=============================================================
template <class T>
Matrix<bool> Matrix<T>::operator > (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<bool> result(R,C);

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      result(ri,ci) = (Data[Index(ri,ci)] > m.Data[Index(ri,ci)]);
    }
  }

  return result;
}

//=============================================================
template <class T>
Matrix<bool> Matrix<T>::operator >= (const Matrix<T> & m)
{
  if ((R != m.R) || (C != m.C))
    Xincompat();

  Matrix<bool> result(R,C);

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
    {
      result(ri,ci) = (Data[Index(ri,ci)] >= m.Data[Index(ri,ci)]);
    }
  }

  return result;
}

//=============================================================
// negate a matrix
template <class T>
Matrix<T> Matrix<T>::operator - ()
{
  Matrix<T> result(R,C);

  for (size_t ri = 0; ri < R; ++ri)
  {
    for (size_t ci = 0; ci < C; ++ci)
      result(ri,ci) = -(Get(ri,ci));
  }

  return result;
}

//=============================================================
// change size (destroying contents)
template <class T>
void Matrix<T>::Resize(size_t rows, size_t cols, const T & init)
{
  this->Grid<T>::Resize(rows,cols);

  for (size_t i = 0; i < N; ++i)
    Data[i] = init;
}

//=============================================================
// inner products
template <class T>
T Matrix<T>::InnerProduct(const Matrix<T> & m)
{
  T result = (T)0;

  if (((R == 1) && (m.C == 1) && (C == m.R)) ||
      ((C == 1) && (m.R == 1) && (R == m.C)))
  {
    const T * ptr1 = Data;
    const T * ptr2 = m.Data;

    size_t end = (C == 1 ? R : C);

    for (size_t n = 0; n < end; ++n)
    {
      result += (*ptr1) * (*ptr2);
      ++ptr1;
      ++ptr2;
    }
  }
  else
    Xincompat();

  return result;
}

//=============================================================
// euclidean norm
template <class T>
inline double Matrix<T>::Norm()
{
  return sqrt(double(InnerProduct(Transpose())));
}

//=============================================================
// calculate determinant value
template <class T>
T Matrix<T>::Determinant()
{
  if (R != C)
    Xincompat();

  if (C == 1)
    return (*Data);

  if (IsSingular())
    return (T)0;

  return DetRecursive();
}

//=============================================================
template <class T>
T Matrix<T>::DetRecursive()
{
  if (C == 2)
  {
    return ((*Data) * (*(Data + 3))) - ((*(Data + 1)) * (*(Data + 2)));
  }

  T result      = (T)0;
  const T * ptr = Data;

  for (size_t x = 0; x < C; ++x)
  {
    if (x & 1) // if on an even column
      result -= (*ptr) * (Minor(0,x)).DetRecursive();
    else
      result += (*ptr) * (Minor(0,x)).DetRecursive();

    ++ptr;
  }

  return result;
}

//=============================================================
// create a minor matrix
template <class T>
Matrix<T> Matrix<T>::Minor(size_t rdel, size_t cdel)
{
  if ((R != C) || (R < 2))
    Xincompat();

  Matrix<T> result(R-1,C-1);

  const T * psrc = Data;
  T * pdest = result.Data;

  for (size_t rsrc = 0; rsrc < R; ++rsrc)
  {
    if (rsrc != rdel)
    {
      for (size_t csrc = 0; csrc < C; ++csrc)
      {
        if (csrc != cdel)
        {
          *pdest = *psrc;
          ++pdest;
        }
        
        ++psrc;
      }
    }
    else
      psrc += C;
  }

  return result;
}

//=============================================================
// solve a system of linear equations via Gaussian elimination
template <class T>
Matrix<T> Matrix<T>::LinSolve()
{
  if (((C - R) != 1) || (IsSingular()))
    Xincompat();

  size_t i, j, k, max;
  T temp;

  // forward elimination
  for (i = 0; i < R; ++i)
  {
    max = i;

    for (j = i + 1; j < R; ++j)
    {
      if (AbsVal(Elem(j,i)) > AbsVal(Elem(max,i)))
        max = j;
    }

    for (k = i; k < C; ++k)
    {
      temp = Elem(i,k);
      Elem(i,k) = Elem(max,k);
      Elem(max,k) = temp;
    }

    for (j = i + 1; j < R; ++j)
    {
      for (k = R; k >= i; --k)
      {
        Elem(j,k) -= Elem(i,k) * Elem(j,i) / Elem(i,i);

        if (k == 0)
          break;
      }
    }
  }

  // backward substitution
  Matrix<T> X(R,1); // results

  for (j = R - 1; ; --j)
  {
    temp = (T)0;

    for (k = j + 1; k < R; ++k)
      temp += Elem(j,k) * X.Elem(k,0);

    X.Elem(j,0) = (Elem(j,R) - temp) / Elem(j,j);

    if (j == 0)
      break;
  }

  return X;
}

//=============================================================
// LUP decomposition
template <class T>
Matrix<size_t> Matrix<T>::LUPDecompose()
{
  // make sure its square
  if ((R != C) || (R < 2))
    Xincompat();

  // LU decomposition
  size_t i, j, k, k2, t;
  T p, temp;
  Matrix<size_t> perm(R,1); // permutation matrix

  // initialize permutation
  for (i = 0; i < R; ++i)
    perm(i,0) = i;

  for (k = 0; k < (R - 1); ++k)
  {
    p = T(0);

    for (i = k; i < R; ++i)
    {
      temp = AbsVal(Elem(i,k));

      if (temp > p)
      {
        p  = temp;
        k2 = i;
      }
    }

    if (p == T(0))
      Xsingular();

    // exchange rows
    t = perm(k,0);
    perm(k,0)  = perm(k2,0);
    perm(k2,0) = t;

    for (i = 0; i < R; ++i)
    {
      temp = Elem(k,i);
      Elem(k,i) = Elem(k2,i);
      Elem(k2,i) = temp;
    }

    for (i = k + 1; i < R; ++i)
    {
      Elem(i,k) /= Elem(k,k);

      for (j = k + 1; j < R; ++j)
        Elem(i,j) -= Elem(i,k) * Elem(k,j);
    }
  }

  // return values
  return perm;
}

//=============================================================
// LUP decomposition (call w/ result of LUPDecomp)
template <class T>
Matrix<T> Matrix<T>::LUPSolve(const Matrix<size_t> & perm, const Matrix<T> & b)
{
  if ((R != b.R) || (R != perm.GetRows()))
    Xincompat();

  size_t i, j, j2;
  T sum, u;
  Matrix<T> y(R,1), x(R,1);

  for (i = 0; i < R; ++i)
  {
    sum = T(0);
    j2  = 0;

    for (j = 1; j <= i; ++j)
    {
      sum += Elem(i,j2) * y.Elem(j2,0);
      ++j2;
    }

    y.Elem(i,0) = b.Elem(perm.Elem(i,0),0) - sum;
  }

  i = R - 1;

  while (1)
  {
    sum = T(0);
    u   = Elem(i,i);

    for (j = i + 1; j < R; ++j)
      sum += Elem(i,j) * x.Elem(j,0);

    x.Elem(i,0) = (y.Elem(i,0) - sum) / u;

    if (i == 0)
      break;

    --i;
  }

  return x;
}

//=============================================================
// LUP inversion (call for result of LUPDecomp)
template <class T>
Matrix<T> Matrix<T>::LUPInvert(const Matrix<size_t> & perm)
{
  size_t i, j;
  Matrix<T> p(R,1);
  Matrix<T> result(R,R);

  for (j = 0; j < R; ++j)
  {
    for (i = 0; i < R; ++i)
      p.Elem(i,0) = T(0);

    p.Elem(j,0) = T(1);

    p = LUPSolve(perm,p);

    for (i = 0; i < R; ++i)
      result.Elem(i,j) = p.Elem(i,0);
  }

  return result;
}

//=============================================================
#if OVERLOAD_NEW
template <class T>
void* Matrix<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
template <class T>
void Matrix<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//=============================================================
#if HAS_ARRAY_NEW
template <class T>
void* Matrix<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
template <class T>
void Matrix<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

//=============================================================
#endif




