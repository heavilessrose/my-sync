//  NumArray.h
//
//  A variant of the Array class for numbers.
//
#ifndef NUMARRAY_H
#define NUMARRAY_H

#ifndef INDEX_H
  #include "index.h"
#endif
#ifndef DYNARRAY_CPP
  #include "dynarray.cpp"
#endif

/****************************************************************************/
template <class T>
class NumericArray;

template <class T>
class NumericArrayAcceptor : public ObjectAcceptor
{
  friend class NumericArray<T>;

  protected:
    union
    {
      const NumericArray<T>* _ObjPtr;
      NumericArray<T>* _VarPtr;
    };

    NumericArrayAcceptor(const NumericArray<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Integer class specific acceptor methods
    virtual Boolean IsNumericArray() const;
    virtual Boolean IsDynamicArray() const;
    
    virtual void* CreateFromNumericArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDynamicArray(const ObjectAcceptor& Obj_);

    virtual void* AssignFromNumericArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDynamicArray(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class NumericArray : public DynArray<T>
{
  friend Object& RootObject(NumericArray<T>& Arr_)
	{ return *((Object*)(&Arr_)); }
  friend const Object& RootConstObject(const NumericArray<T>& Arr_)
	{ return *((const Object*)(&Arr_)); }

  friend NumericArray<T> Abs(const NumericArray<T> & ia)
      { NumericArray<T> Temp_(ia); Temp_.Abs(); return Temp_; }

  protected:
    MUTABLE int _ErrorStat;

    void XInCompat() const
	{ THROW (FallibleBase::IncompatableArraysErr()); }

  public:
    // constructors
    NumericArray();
    NumericArray(size_t n, capacity c);
    NumericArray(const NumericArray<T>& x);
    NumericArray(const DynArray<T>& x);
    NumericArray(const T& x, size_t n=1);
    NumericArray(const T* s, size_t n);

    static NumericArray<T>* Make()
	{ return (new NumericArray<T>); }
    static NumericArray<T>* Make(size_t n, capacity c)
	{ return (new NumericArray<T>(n, c)); }
    static NumericArray<T>* Make(const Object& Obj_)
	{ NumericArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._VarPtr;
	}
    static NumericArray<T>* Make(const T& x, size_t n=1)
	{ return (new NumericArray<T>(x, n)); }
    static NumericArray<T>* Make(const T* s, size_t n)
	{ return (new NumericArray<T>(s, n)); }

    static const NumericArray<T>* MakeConst(const Object& Obj_)
	{ NumericArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._ObjPtr;
	}
    static const NumericArray<T>* MakeConst(const T& x, size_t n=1)
	{ return (new NumericArray<T>(x, n)); }
    static const NumericArray<T>* MakeConst(const T* s, size_t n)
	{ return (new NumericArray<T>(s, n)); }

    // comparison methods
    NumericArray<Boolean> operator == (const NumericArray<T>& Obj2_) const;
    NumericArray<Boolean> operator != (const NumericArray<T>& Obj2_) const;
    NumericArray<Boolean> operator > (const NumericArray<T>& Obj2_) const;
    NumericArray<Boolean> operator < (const NumericArray<T>& Obj2_) const;
    NumericArray<Boolean> operator >= (const NumericArray<T>& Obj2_) const;
    NumericArray<Boolean> operator <= (const NumericArray<T>& Obj2_) const;

    // virtual assigment and object construction methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);

    // null object methods
    static NumericArray<T>& GiveNullObject();
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::NUMERICARRAY_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // assignment operator
    NumericArray<T>& operator = (const NumericArray<T>& a);
    NumericArray<T>& operator = (const DynArray<T>& a);

    // unary operators
    NumericArray<T> operator + ();
    NumericArray<T> operator - ();

    // basic math operators
    NumericArray<T> operator + (const NumericArray<T>& ia);
    NumericArray<T> operator + (T n);
    NumericArray<T> operator - (const NumericArray<T>& ia);
    NumericArray<T> operator - (T n);
    NumericArray<T> operator * (const NumericArray<T>& ia);
    NumericArray<T> operator * (T n);
    NumericArray<T> operator / (const NumericArray<T>& ia);
    NumericArray<T> operator / (T n);

    // shorthand operators
    NumericArray<T>& operator += (const NumericArray<T>& ia);
    NumericArray<T>& operator += (T n);
    NumericArray<T>& operator -= (const NumericArray<T>& ia);
    NumericArray<T>& operator -= (T n);
    NumericArray<T>& operator *= (const NumericArray<T>& ia);
    NumericArray<T>& operator *= (T n);
    NumericArray<T>& operator /= (const NumericArray<T>& ia);
    NumericArray<T>& operator /= (T n);

    // fill functions
    void FillArithmetic(T first, T incr);
    void FillGeometric(T first, T mult);

    // absolute value function
    NumericArray & Abs();

#if OVERLOAD_NEW
    // new & delete operators
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

    // new & delete operators for arrays
#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif            
};

/****************************************************************************/
template <class T>
NumericArray<Boolean> NumericArray<T>::operator == (const NumericArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  NumericArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left == *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
NumericArray<Boolean> NumericArray<T>::operator != (const NumericArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  NumericArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left != *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
NumericArray<Boolean> NumericArray<T>::operator < (const NumericArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  NumericArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left < *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
NumericArray<Boolean> NumericArray<T>::operator <= (const NumericArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  NumericArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left <= *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
NumericArray<Boolean> NumericArray<T>::operator > (const NumericArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  NumericArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left > *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
template <class T>
NumericArray<Boolean> NumericArray<T>::operator >= (const NumericArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  NumericArray<Boolean> result(RunLength(), default_size);
  DynArrayIterator<Boolean> dest(&result);
  dest.GotoHead();
  
  const T* left  = Base();
  const T* right = Obj2_.Base();
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    *dest = (*left >= *right);
    ++dest;
    ++left;
    ++right;
  }

  return result;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
NumericArray<T>::NumericArray():
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
NumericArray<T>::NumericArray(size_t n, capacity c):
DynArray<T>(n, c),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
NumericArray<T>::NumericArray(const NumericArray<T>& x):
DynArray<T>(x),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
NumericArray<T>::NumericArray(const DynArray<T>& x):
DynArray<T>(x),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
NumericArray<T>::NumericArray(const T& x, size_t n):
DynArray<T>(x, n),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
NumericArray<T>::NumericArray(const T* s, size_t n):
DynArray<T>(s, n),
_ErrorStat(0)
{}

/****************************************************************************/
template <class T>
Object& NumericArray<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  NumericArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromNumericArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& NumericArray<T>::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  NumericArrayAcceptor<T> TrgComp_(this);
  return *((Object*) ObjComp_->AssignFromNumericArray(TrgComp_));
}

/****************************************************************************/
template <class T>
Object* NumericArray<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new NumericArray<T>(*this));

  NumericArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromNumericArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
const Object* NumericArray<T>::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
Object* NumericArray<T>::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
NumericArray<T>& NumericArray<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static NumericArray<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& NumericArray<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return NumericArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& NumericArray<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return NumericArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int NumericArray<T>::ClassID() const
{
  return TypeInfo::NUMERICARRAY_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* NumericArray<T>::GiveObjectAcceptor() const
{
  return (new NumericArrayAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
istream& NumericArray<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& NumericArray<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;
  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
    Os_ <<((*this)[i]) <<" ";

  return Os_;
}

/****************************************************************************/
template <class T>
inline NumericArray<T> & NumericArray<T>::operator = (const NumericArray<T> & a)
{
  DynArray<T>::operator = (a);
  return *this;
}

/****************************************************************************/
template <class T>
inline NumericArray<T> & NumericArray<T>::operator = (const DynArray<T> & a)
{
  DynArray<T>::operator = (a);
  return *this;
}

/****************************************************************************/
template <class T>
inline NumericArray<T> NumericArray<T>::operator + ()
{
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator - ()
{
  NumericArray<T> result(*this);

  T * res  = result.Base();
  T * orig = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *res = -(*orig);
    ++res;
    ++orig;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator + (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();
	
  NumericArray<T> result(*this);

  T* ptr1 = result.Base();
  const T* ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 += *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator + (T n)
{
  NumericArray<T> result(*this);
  
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr += n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator - (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  NumericArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 -= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator - (T n)
{
  NumericArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr -= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator * (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  NumericArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 *= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator * (T n)
{
  NumericArray<T> result(*this);
	
  T* ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr *= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator / (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  NumericArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 /= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> NumericArray<T>::operator / (T n)
{
  NumericArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr /= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator += (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 += *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator += (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr += n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator -= (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 -= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator -= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr -= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator *= (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 *= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator *= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr *= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator /= (const NumericArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 /= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> & NumericArray<T>::operator /= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr /= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
void NumericArray<T>::FillArithmetic(T first, T incr)
{
  T value = first;
  
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr = value;
    value += incr;
    ++ptr;
  }
}

/****************************************************************************/
template <class T>
void NumericArray<T>::FillGeometric(T first, T mult)
{
  T value = first;
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr = value;
    value *= mult;
    ++ptr;
  }
}

/****************************************************************************/
// absolute value functions
template <class T>
NumericArray<T> & NumericArray<T>::Abs()
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr = ((*ptr < 0) ? -(*ptr) : (*ptr));
    ++ptr;
  }

  return *this;
}

/****************************************************************************/
template <class T>
NumericArray<T> Abs(const NumericArray<T> & ia)
{
  NumericArray<T> result(ia);

  T * ptr = result.Base();
  size_t Max_ = result.RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr = ((*ptr < 0) ? -(*ptr) : (*ptr));
    ++ptr;
  }

  return result;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* NumericArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void NumericArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* NumericArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void NumericArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
NumericArrayAcceptor<T>::NumericArrayAcceptor(const NumericArray<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void NumericArrayAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const NumericArray<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* NumericArrayAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean NumericArrayAcceptor<T>::IsNumericArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>    
Boolean NumericArrayAcceptor<T>::IsDynamicArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>    
void* NumericArrayAcceptor<T>::CreateFromNumericArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new NumericArray<T>(*((const NumericArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* NumericArrayAcceptor<T>::CreateFromDynamicArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new NumericArray<T>(*((const DynArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* NumericArrayAcceptor<T>::AssignFromNumericArray(const ObjectAcceptor& Obj_)
{
  const NumericArray<T>* TrgPtr_ = (const NumericArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* NumericArrayAcceptor<T>::AssignFromDynamicArray(const ObjectAcceptor& Obj_)
{
  const DynArray<T>* TrgPtr_ = (const DynArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* NumericArrayAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void NumericArrayAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
#endif





