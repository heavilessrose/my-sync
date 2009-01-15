#ifndef BASEPTR_H
#define BASEPTR_H
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif
#ifndef REFERENCECOUNT_H
  #include "refcount.h"
#endif
#ifndef LONEFALL_H
  #include "lonefall.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

#define BASEPTR_DEBUG	0

/****************************************************************************/
template <class T>
class BaseCopiedPtr
{
  friend Boolean operator == (const BaseCopiedPtr<T>& lhs, const BaseCopiedPtr<T>& rhs);
  friend Boolean operator != (const BaseCopiedPtr<T>& lhs, const BaseCopiedPtr<T>& rhs);

  protected:
    T* the_p;

  public:
    BaseCopiedPtr();
    BaseCopiedPtr(T* just_newed);
    BaseCopiedPtr(const T& obj);                         // Copy object
    BaseCopiedPtr(const BaseCopiedPtr<T>& aCP);  // Copy heap object
    virtual ~BaseCopiedPtr();

    BaseCopiedPtr<T>& operator = (const BaseCopiedPtr<T>& Obj_);
    BaseCopiedPtr<T>& operator = (T* rhs);

    T& operator * ();
    T* operator -> ();
    const T& operator * () const;
    const T* operator -> () const;

    Boolean operator ! () const
	{ return !the_p; }
    Boolean IsNull() const
	{ return the_p == 0; }

    T* Pointee()
	{ return the_p; }
    const T* Pointee() const
	{ return the_p; }

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
template <class T>
class BaseCountedPtr
{
  friend Boolean operator==(const BaseCountedPtr<T>& lhs, const BaseCountedPtr<T>& rhs);
  friend Boolean operator!=(const BaseCountedPtr<T>& lhs, const BaseCountedPtr<T>& rhs);

  protected:
    ReferenceCount refCount; // Number of pointers to heap object
    T* the_p;

    void SetToNull();

  public:
    BaseCountedPtr();
    BaseCountedPtr(T* just_newed);
    virtual ~BaseCountedPtr();

    BaseCountedPtr<T>& operator = (BaseCountedPtr<T>& Obj_);
    BaseCountedPtr<T>& operator = (T* rhs);

    Boolean Unique() const
	{ return refCount.Unique(); }			          // Is count one ?

    T& operator * ();
    T* operator-> ();
    const T& operator * () const;
    const T* operator -> () const;

    Boolean operator ! () const
	{ return !the_p; }
    Boolean IsNull() const
	{ return the_p == 0; }

    T* Pointee()
	{ return the_p; }
    const T* Pointee() const
	{ return the_p; }

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
#endif




