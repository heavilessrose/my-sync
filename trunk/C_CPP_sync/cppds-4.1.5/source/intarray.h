//  IntArray.h
//
//  A variant of the NumericArray class for integers.
//
#ifndef INTARRAY_H
#define INTARRAY_H

#ifndef NUMARRAY_H
  #include "numarray.h"
#endif

/****************************************************************************/
template <class T>
class IntegerArray;

template <class T>
class IntegerArrayAcceptor : public ObjectAcceptor
{
  friend class IntegerArray<T>;

  protected:
    union
    {
      const IntegerArray<T>* _ObjPtr;
      IntegerArray<T>* _VarPtr;
    };

    IntegerArrayAcceptor(const IntegerArray<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Integer class specific acceptor methods
    virtual Boolean IsIntegerArray() const;
    virtual Boolean IsNumericArray() const;
    virtual Boolean IsDynamicArray() const;
    
    virtual void* CreateFromIntegerArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromNumericArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDynamicArray(const ObjectAcceptor& Obj_);

    virtual void* AssignFromIntegerArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromNumericArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDynamicArray(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class IntegerArray : public NumericArray<T>
{
  friend Object& RootObject(IntegerArray<T>& Arr_)
	{ return *((Object*)(&Arr_)); }
  friend const Object& RootConstObject(const IntegerArray<T>& Arr_)
	{ return *((const Object*)(&Arr_)); }

  public:
    // constructors
    IntegerArray();
    IntegerArray(size_t n, capacity c);
    IntegerArray(const IntegerArray<T>& x);
    IntegerArray(const NumericArray<T>& x);
    IntegerArray(const DynArray<T>& x);
    IntegerArray(const T& x, size_t n=1);
    IntegerArray(const T* s, size_t n);

    static IntegerArray<T>* Make()
	{ return (new IntegerArray<T>); }
    static IntegerArray<T>* Make(size_t n, capacity c)
	{ return (new IntegerArray<T>(n, c)); }
    static IntegerArray<T>* Make(const Object& Obj_)
	{ IntegerArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._VarPtr;
	}
    static IntegerArray<T>* Make(const T& x, size_t n=1)
	{ return (new IntegerArray<T>(x, n)); }
    static IntegerArray<T>* Make(const T* s, size_t n)
	{ return (new IntegerArray<T>(s, n)); }

    static const IntegerArray<T>* MakeConst(const Object& Obj_)
	{ IntegerArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._ObjPtr;
	}
    static const IntegerArray<T>* MakeConst(const T& x, size_t n=1)
	{ return (new IntegerArray<T>(x, n)); }
    static const IntegerArray<T>* MakeConst(const T* s, size_t n)
	{ return (new IntegerArray<T>(s, n)); }

    IntegerArray<Boolean> operator == (const IntegerArray<T>& Obj2_) const;
    IntegerArray<Boolean> operator != (const IntegerArray<T>& Obj2_) const;
    IntegerArray<Boolean> operator > (const IntegerArray<T>& Obj2_) const;
    IntegerArray<Boolean> operator < (const IntegerArray<T>& Obj2_) const;
    IntegerArray<Boolean> operator >= (const IntegerArray<T>& Obj2_) const;
    IntegerArray<Boolean> operator <= (const IntegerArray<T>& Obj2_) const;

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);

    static IntegerArray<T>& GiveNullObject();
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::INTEGERARRAY_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // assignment operator
    IntegerArray<T> & operator = (const IntegerArray<T>& a);
    IntegerArray<T> & operator = (const NumericArray<T>& a);
    IntegerArray<T> & operator = (const DynArray<T>& a);

    // unary operators
    IntegerArray<T> operator ~ ();
    IntegerArray<T> operator ! ();
    
    // modulus, shift, and bitwise operators
    IntegerArray<T> operator %  (const IntegerArray<T> & ia);
    IntegerArray<T> operator %  (T n);
    IntegerArray<T> operator << (const IntegerArray<T> & ia);
    IntegerArray<T> operator << (T n);
    IntegerArray<T> operator >> (const IntegerArray<T> & ia);
    IntegerArray<T> operator >> (T n);
    IntegerArray<T> operator &  (const IntegerArray<T> & ia);
    IntegerArray<T> operator &  (T n);
    IntegerArray<T> operator |  (const IntegerArray<T> & ia);
    IntegerArray<T> operator |  (T n);
    IntegerArray<T> operator ^  (const IntegerArray<T> & ia);
    IntegerArray<T> operator ^  (T n);
    
    // shorthand operators
    IntegerArray<T> & operator %=  (const IntegerArray<T> & ia);
    IntegerArray<T> & operator %=  (T n);
    IntegerArray<T> & operator <<= (const IntegerArray<T> & ia);
    IntegerArray<T> & operator <<= (T n);
    IntegerArray<T> & operator >>= (const IntegerArray<T> & ia);
    IntegerArray<T> & operator >>= (T n);
    IntegerArray<T> & operator &=  (const IntegerArray<T> & ia);
    IntegerArray<T> & operator &=  (T n);
    IntegerArray<T> & operator |=  (const IntegerArray<T> & ia);
    IntegerArray<T> & operator |=  (T n);
    IntegerArray<T> & operator ^=  (const IntegerArray<T> & ia);
    IntegerArray<T> & operator ^=  (T n);

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
IntegerArray<Boolean> IntegerArray<T>::operator == (const IntegerArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  IntegerArray<Boolean> result(RunLength(), default_size);
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
IntegerArray<Boolean> IntegerArray<T>::operator != (const IntegerArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  IntegerArray<Boolean> result(RunLength(), default_size);
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
IntegerArray<Boolean> IntegerArray<T>::operator < (const IntegerArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  IntegerArray<Boolean> result(RunLength(), default_size);
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
IntegerArray<Boolean> IntegerArray<T>::operator <= (const IntegerArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  IntegerArray<Boolean> result(RunLength(), default_size);
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
IntegerArray<Boolean> IntegerArray<T>::operator > (const IntegerArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  IntegerArray<Boolean> result(RunLength(), default_size);
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
IntegerArray<Boolean> IntegerArray<T>::operator >= (const IntegerArray<T>& Obj2_) const
{
  if (RunLength() != Obj2_.RunLength())
    XInCompat();

  IntegerArray<Boolean> result(RunLength(), default_size);
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
IntegerArray<T>::IntegerArray()
{}

/****************************************************************************/
template <class T>
IntegerArray<T>::IntegerArray(size_t n, capacity c):
NumericArray<T>(n, c)
{}

/****************************************************************************/
template <class T>
IntegerArray<T>::IntegerArray(const IntegerArray<T>& x):
NumericArray<T>(x)
{}

/****************************************************************************/
template <class T>
IntegerArray<T>::IntegerArray(const NumericArray<T>& x):
NumericArray<T>(x)
{}

/****************************************************************************/
template <class T>
IntegerArray<T>::IntegerArray(const DynArray<T>& x):
NumericArray<T>(x)
{}

/****************************************************************************/
template <class T>
IntegerArray<T>::IntegerArray(const T& x, size_t n):
NumericArray<T>(x, n)
{}

/****************************************************************************/
template <class T>
IntegerArray<T>::IntegerArray(const T* s, size_t n):
NumericArray<T>(s, n)
{}

/****************************************************************************/
template <class T>
Object& IntegerArray<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  IntegerArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromIntegerArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& IntegerArray<T>::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  IntegerArrayAcceptor<T> TrgComp_(this);
  return *((Object*) ObjComp_->AssignFromIntegerArray(TrgComp_));
}

/****************************************************************************/
template <class T>
Object* IntegerArray<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new IntegerArray<T>(*this));

  IntegerArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromIntegerArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
const Object* IntegerArray<T>::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
Object* IntegerArray<T>::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
IntegerArray<T>& IntegerArray<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static IntegerArray<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& IntegerArray<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return IntegerArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& IntegerArray<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return IntegerArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int IntegerArray<T>::ClassID() const
{
  return TypeInfo::INTEGERARRAY_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* IntegerArray<T>::GiveObjectAcceptor() const
{
  return (new IntegerArrayAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
istream& IntegerArray<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& IntegerArray<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;
  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
    Os_ <<((*this)[i]) <<" ";

  return Os_;
}

/****************************************************************************/
template <class T>
inline IntegerArray<T> & IntegerArray<T>::operator = (const IntegerArray<T> & a)
{
  NumericArray<T>::operator = (a);
  return *this;
}

/****************************************************************************/
template <class T>
inline IntegerArray<T> & IntegerArray<T>::operator = (const NumericArray<T> & a)
{
  NumericArray<T>::operator = (a);
  return *this;
}

/****************************************************************************/
template <class T>
inline IntegerArray<T> & IntegerArray<T>::operator = (const DynArray<T> & a)
{
  NumericArray<T>::operator = (a);
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator ~ ()
{
  IntegerArray<T> result(*this);

  T * res  = result.Base();
  T * orig = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *res = ~(*orig);
    ++res;
    ++orig;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator ! ()
{
  IntegerArray<T> result(*this);

  T * res  = result.Base();
  T * orig = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *res = !(*orig);
    ++res;
    ++orig;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator % (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  IntegerArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 %= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator % (T n)
{
  IntegerArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr %= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator << (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  IntegerArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 <<= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator << (T n)
{
  IntegerArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr <<= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator >> (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  IntegerArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 >>= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator >> (T n)
{
  IntegerArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr >>= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator & (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  IntegerArray<T> result(*this);

  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 &= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator & (T n)
{
  IntegerArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr &= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator | (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  IntegerArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 |= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator | (T n)
{
  IntegerArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr |= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator ^ (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  IntegerArray<T> result(*this);
	
  T * ptr1 = result.Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 ^= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> IntegerArray<T>::operator ^ (T n)
{
  IntegerArray<T> result(*this);
	
  T * ptr = result.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr ^= n;
    ++ptr;
  }
	
  return result;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator %= (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 %= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator %= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr %= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator <<= (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 <<= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator <<= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr <<= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator >>= (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 >>= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator >>= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr >>= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator &= (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 &= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator &= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr &= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator |= (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 |= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator |= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr |= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator ^= (const IntegerArray<T> & ia)
{
  if (RunLength() != ia.RunLength())
    XInCompat();

  T * ptr1 = Base();
  const T * ptr2 = ia.Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr1 ^= *ptr2;
    ++ptr1;
    ++ptr2;
  }
	
  return *this;
}

/****************************************************************************/
template <class T>
IntegerArray<T> & IntegerArray<T>::operator ^= (T n)
{
  T * ptr = Base();
  size_t Max_ = RunLength();
  size_t i;

  for (i = 0; i < Max_; ++i)
  {
    *ptr ^= n;
    ++ptr;
  }
	
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* IntegerArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void IntegerArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* IntegerArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void IntegerArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
IntegerArrayAcceptor<T>::IntegerArrayAcceptor(const IntegerArray<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void IntegerArrayAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const IntegerArray<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* IntegerArrayAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean IntegerArrayAcceptor<T>::IsIntegerArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>    
Boolean IntegerArrayAcceptor<T>::IsNumericArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>    
Boolean IntegerArrayAcceptor<T>::IsDynamicArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>    
void* IntegerArrayAcceptor<T>::CreateFromIntegerArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new IntegerArray<T>(*((const IntegerArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* IntegerArrayAcceptor<T>::CreateFromNumericArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new IntegerArray<T>(*((const NumericArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* IntegerArrayAcceptor<T>::CreateFromDynamicArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new IntegerArray<T>(*((const DynArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* IntegerArrayAcceptor<T>::AssignFromIntegerArray(const ObjectAcceptor& Obj_)
{
  const IntegerArray<T>* TrgPtr_ = (const IntegerArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* IntegerArrayAcceptor<T>::AssignFromNumericArray(const ObjectAcceptor& Obj_)
{
  const NumericArray<T>* TrgPtr_ = (const NumericArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* IntegerArrayAcceptor<T>::AssignFromDynamicArray(const ObjectAcceptor& Obj_)
{
  const DynArray<T>* TrgPtr_ = (const DynArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* IntegerArrayAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void IntegerArrayAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
#endif




