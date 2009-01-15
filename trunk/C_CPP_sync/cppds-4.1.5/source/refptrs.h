//////////////////////////////////////////////////////////////////////////////
// FILE          : REFPTRS.H
// PURPOSE       : Smart pointer classes: Basic smart pointer class
//                 for handling pointers with reference semantics,
//                 Master pointer class for handling pointers with copy
//                 semantics.
//
// WRITTEN BY    : Joseph Wong
//
// MODIFIED BY:         LAST MODIFIED:  COMMENTS:
// ------------         --------------  ---------
// Joseph Wong          May 22 1997     Original Writing
// Joseph Wong		Dec 9 1997      Made following methods virtual:
//						void allocmem();
//                                              void erasemem();
//
//					to provide a way for derived classes
//                                      to customize memory allocation.
// Joseph Wong          May 10 2000     Officially Deprecated
//
//////////////////////////////////////////////////////////////////////////////
#ifndef REFPTRS_H
#define REFPTRS_H
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef ALLOCATE_CPP
  #include "allocate.cpp"
#endif

#define REFPTRS_DEBUG	0
#define REFPTRS_DEBUG1	0
#define REFPTRS_DEBUG2	0
#define REFPTRS_DEBUG4	0

/****************************************************************************/
template <class T>
class CopyPtrClass
{
  friend int WhatIs(const CopyPtrClass<T>& Obj_)
	{ return Obj_.ClassID(); }
  friend CopyPtrClass<T>& RootObject(CopyPtrClass<T>& Obj_)
	{ return &Obj_; }
  friend const CopyPtrClass<T>& RootConstObject(const CopyPtrClass<T>& Obj_)
	{ return &Obj_; }
        
  friend Boolean _IsEqualTo(const CopyPtrClass<T>& Trg_, const CopyPtrClass<T>& Src_)
	{ return Trg_.IsEqual(Src_); }        

  protected:
    void Xnull() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }
    void Xmem() const
	{ THROW (FallibleBase::OutOfMemoryErr()); }

    virtual void Erase()						= 0;
    virtual void Alloc(T* ptr)						= 0;

  public:
    virtual int ClassID() const						= 0;

    virtual CopyPtrClass<T>& operator = (const CopyPtrClass<T>& ptr)	= 0;
    virtual CopyPtrClass<T>& operator = (T* ptr)			= 0;
    virtual T* ClearPtr()						= 0;
    virtual T* Pointee() const						= 0;
    virtual T& operator * () const					= 0;

    // Comparison methods
    virtual Boolean IsEqual(const CopyPtrClass<T>& Obj_) const = 0;

    Boolean IsNULL() const
	{ return (Pointee() == NULL); }
    Boolean operator ! () const
	{ return !Pointee(); }

    virtual Boolean IsShallowCopy() const				= 0;
    virtual Boolean IsDeepCopy() const					= 0;    
};

/****************************************************************************/
template <class T>
class CopyPtrImp : public CopyPtrClass<T>
{
  protected:
    T* _Pointee;

    CopyPtrImp();
    CopyPtrImp(T* ptr);

    void DeletePointee()
	{ ::Delete(_Pointee); }
    void SetToNull()
	{ _Pointee = NULL; }
    T* SetToAddress(T* ptr)
	{ _Pointee = ptr;
	  return _Pointee; }

  public:
    virtual T* Pointee() const;
    virtual T& operator * () const;

    virtual Boolean IsEqual(const CopyPtrClass<T>& Obj_) const;

    const char* ClassName() const;
};

/****************************************************************************/
// Smart pointer that traps an attempt to dereference nil
//
template <class T>
class RefPtr : public CopyPtrImp<T>
{
  friend int WhatIs(const RefPtr<T>& Obj_)
	{ return Obj_.ClassID(); }
  friend CopyPtrClass<T>& RootObject(RefPtr<T>& Obj_)
	{ return *((CopyPtrClass<T>*)(&Obj_)); }
  friend const CopyPtrClass<T>& RootConstObject(const RefPtr<T>& Obj_)
	{ return *((const CopyPtrClass<T>*)(&Obj_)); }

  protected:
    virtual void Erase();
    virtual void Alloc(T* ptr);

  public:
    RefPtr();
    RefPtr(T* ptr);
    RefPtr(const CopyPtrClass<T>& ptr); 	// Copies the Pointee
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      RefPtr(const RefPtr<T>& Ptr_);
    #endif
    virtual ~RefPtr();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::REFPTR_TYPE; }
    virtual int ClassID() const;

    virtual T* ClearPtr();
    virtual CopyPtrClass<T>& operator = (const CopyPtrClass<T>& ptr);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      virtual RefPtr<T>& operator = (const RefPtr<T>& ptr);
    #endif
    virtual CopyPtrClass<T>& operator = (T* ptr);

    virtual Boolean IsShallowCopy() const;
    virtual Boolean IsDeepCopy() const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
// Prototypical master/memory pointer template
template <class T>
class MemPtr : public CopyPtrImp<T>
{
  friend int WhatIs(const MemPtr<T>& Obj_)
	{ return Obj_.ClassID(); }
  friend CopyPtrClass<T>& RootObject(MemPtr<T>& Obj_)
	{ return *((CopyPtrClass<T>*)(&Obj_)); }
  friend const CopyPtrClass<T>& RootConstObject(const MemPtr<T>& Obj_)
	{ return *((const CopyPtrClass<T>*)(&Obj_)); }

  protected:
    virtual void Alloc(T* ptr);
    virtual void Erase();

  public:
    MemPtr();
    MemPtr(T* ptr);
    MemPtr(const CopyPtrClass<T>& ptr);		// Copies the Pointee
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      MemPtr(const MemPtr<T>& Ptr_);
    #endif
    virtual ~MemPtr();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::MEMPTR_TYPE; }
    virtual int ClassID() const;

    virtual T* ClearPtr();
    virtual CopyPtrClass<T>& operator = (const CopyPtrClass<T>& ptr);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      virtual MemPtr<T>& operator = (const MemPtr<T>& ptr);
    #endif
    virtual CopyPtrClass<T>& operator = (T* ptr);

    virtual Boolean IsShallowCopy() const;
    virtual Boolean IsDeepCopy() const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
// Member selection class for smart pointers
// The member selection operator -> () has to be separately defined in
// another class because the enclosing member pointee of the smart pointer
// object could be a primitive simple type rather than an aggregate class
// type in which case dereferencing through member selection would produce
// an error since there would be no class members for primitive types.
// The member selection class would require some way of storing the smart
// pointer so that it could be accessed for subsequent operations. One idea
// was to derive the member selection class from the base pointer RefPtr<T>
// class this design was to allow for a way of accessing the original pointer
// object while enclosed within a class used for the pointee member selection
// However by inheriting from the base pointer RefPtr<T> this restricts the
// type that is used for the smart pointer and would result in the original
// pointer not always being accurately represented. A better solution would
// be to include a pointer to the base pointer class as a data member access
// to this data member would be provided by the no-argument function call
// operator. This nicely separates the implementation of the function call
// operator into 2 versions. One in which the operator takes an argument as
// the smart pointer to be stored in the data member while returning the
// wrapper class object and another in which the operator returns the
// currently stored smart pointer. This implementation should be intuitive to
// the user. C++ smart pointer idioms via the member selection operator -> ()
// could not be used since it was taken up for pointee dereferencing and
// member selection. The function call operator () is a nice alternative.
// This solves the requirement of having the smart pointer stored for later
// access within the same member selection class. This is a major convenience
// to the class user since all relevant operations on the pointer can be
// performed from one single object. This also solve the problem of smart
// pointer class representation since a base class pointer can store pointers
// from derived classes and still support derived class methods via
// polymorphism and virtual methods.
//
template <class T>
class CPtr
{
  protected:
    const CopyPtrClass<T>* _BossPtr;

    void Xnull() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    CPtr(const CopyPtrClass<T>* Ptr_);
    CPtr<T>& operator () (const CopyPtrClass<T>* Ptr_);

    const CopyPtrClass<T>* operator () () const;
    T* operator -> () const;
    T& operator * () const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
// Prototypical static pointer template
// Only one instance of the pointer object can be allocated at any one time.
// The pointer can be set to an object using the argument constructors or the
// Create method. Once the pointer is allocated repeated calls to Create will
// increment an instance count instead of allocating a new object. Repeated
// calls to Destroy will decrement the instance count until it reaches zero
// in which case it will do the actual deletion of the pointer. Once the
// pointer is removed by deletion and the instance count is equal to zero,
// another pointer object is allowed to be allocated.
// The pointer can be replaced with another one of the same type by
// passing it the the Replace method. Replacement is uses straight
// pointer-to-pointer copy (shallow copying) without allocating memory and
// the instance count is reset to 1. The previous pointer is not deleted and
// returned.
//
template <class T>
class StaticPtr : public MemPtr<T>
{
  friend int WhatIs(const StaticPtr<T>& Obj_)
	{ return Obj_.ClassID(); }
  friend CopyPtrClass<T>& RootObject(StaticPtr<T>& Obj_)
	{ return *((CopyPtrClass<T>*)(&Obj_)); }
  friend const CopyPtrClass<T>& RootConstObject(const StaticPtr<T>& Obj_)
	{ return *((const CopyPtrClass<T>*)(&Obj_)); }

  protected:
    size_t _Instances;

  public:
    StaticPtr();
    StaticPtr(T* Ptr_);
    StaticPtr(const CopyPtrClass<T>& Ptr_);		// Copies the Pointee
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      StaticPtr(const StaticPtr<T>& Ptr_);
    #endif
    ~StaticPtr();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::STATICPTR_TYPE; }
    virtual int ClassID() const;

    void Create(T& Obj_);
    void Destroy();

    virtual CopyPtrClass<T>& operator = (const CopyPtrClass<T>& Ptr_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      virtual StaticPtr<T>& operator = (const StaticPtr<T>& Ptr_);
    #endif
    virtual CopyPtrClass<T>& operator = (T* Ptr_);
    virtual T* ClearPtr();

    virtual Boolean IsShallowCopy() const;
    virtual Boolean IsDeepCopy() const;
};

/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean operator == (const CopyPtrClass<T>& r1, const CopyPtrClass<T>& r2)
{
  return r1.IsEqual(r2);
}

/****************************************************************************/
template <class T>
Boolean operator != (const CopyPtrClass<T>& r1, const CopyPtrClass<T>& r2)
{
  return !r1.IsEqual(r2);
}

/****************************************************************************/
template <class T>
ptrdiff_t operator - (const CopyPtrClass<T>& r1, const CopyPtrClass<T>& r2)
{
  T* Ptr1_ = (T*)r1.Pointee();
  T* Ptr2_ = (T*)r2.Pointee();

  return (Ptr1_ - Ptr2_);
}

/****************************************************************************/
#endif





