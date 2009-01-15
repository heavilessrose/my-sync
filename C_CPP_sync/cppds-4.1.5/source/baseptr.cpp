#ifndef BASEPTR_CPP
#define BASEPTR_CPP
#ifndef BASEPTR_H
  #include "baseptr.h"
#endif

/****************************************************************************/
template <class T>
Boolean operator == (const BaseCopiedPtr<T>& lhs, const BaseCopiedPtr<T>& rhs)
{
  return lhs.Pointee() == rhs.Pointee();
}

/****************************************************************************/
template <class T>
Boolean operator != (const BaseCopiedPtr<T>& lhs, const BaseCopiedPtr<T>& rhs)
{
  return lhs.Pointee() != rhs.Pointee();
}

/****************************************************************************/
template <class T>
BaseCopiedPtr<T>::BaseCopiedPtr():
the_p(0)
{}                              // Construct as null pointer

/****************************************************************************/
template <class T>
BaseCopiedPtr<T>::BaseCopiedPtr(T* just_newed):
the_p(just_newed)
{}        // Construct from new pointer

/****************************************************************************/
template <class T>
BaseCopiedPtr<T>::BaseCopiedPtr(const T& obj):
the_p((T*)obj.Clone())
{}

/****************************************************************************/
template <class T>
BaseCopiedPtr<T>::BaseCopiedPtr(const BaseCopiedPtr<T>& aCP):
the_p(aCP.IsNull() ? 0 : (T*)aCP.Pointee()->Clone())
{}

/****************************************************************************/
template <class T>
BaseCopiedPtr<T>::~BaseCopiedPtr()
{
  delete the_p;
  the_p = NULL;
}

/****************************************************************************/
template <class T>
BaseCopiedPtr<T>& BaseCopiedPtr<T>::operator = (const BaseCopiedPtr<T>& Obj_)
{
  if (the_p != Obj_.Pointee())
  {
    delete the_p;
    the_p = Obj_.IsNull() ? 0 : (T*)Obj_.Pointee()->Clone();
  }

  return *this;
}

/****************************************************************************/
template <class T>
BaseCopiedPtr<T>& BaseCopiedPtr<T>::operator = (T* rhs)
{
  if (the_p != rhs)
  {
    delete the_p;
    the_p = rhs ? (T*)rhs->Clone() : 0;
  }

  return *this;
}

/****************************************************************************/
template <class T>
T* BaseCopiedPtr<T>::operator -> ()
{
  if (the_p)
    return the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return the_p;
}

/****************************************************************************/
template <class T>
T& BaseCopiedPtr<T>::operator * ()
{
  if (the_p)
    return *the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *the_p;
}

/****************************************************************************/
template <class T>
const T* BaseCopiedPtr<T>::operator -> () const
{
  if (the_p)
    return the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return the_p;
}

/****************************************************************************/
template <class T>
const T& BaseCopiedPtr<T>::operator * () const
{
  if (the_p)
    return *the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *the_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* BaseCopiedPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void BaseCopiedPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* BaseCopiedPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void BaseCopiedPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean operator == (const BaseCountedPtr<T>& lhs, const BaseCountedPtr<T>& rhs)
{
  return lhs.Pointee() == rhs.Pointee();
}

/****************************************************************************/
template <class T>
Boolean operator != (const BaseCountedPtr<T>& lhs, const BaseCountedPtr<T>& rhs)
{
  return lhs.Pointee() != rhs.Pointee();
}

/****************************************************************************/
/****************************************************************************/
template <class T>
BaseCountedPtr<T>::BaseCountedPtr():
the_p(0)
{}                             // Construct as null pointer

/****************************************************************************/
template <class T>
BaseCountedPtr<T>::BaseCountedPtr(T* just_newed):
the_p(just_newed)
{}       // Construct pointing at heap object

/****************************************************************************/
template<class T>
BaseCountedPtr<T>::~BaseCountedPtr()
{
  if (Unique())
  {
    delete the_p;
    the_p = NULL;
  }
}

/****************************************************************************/
template <class T>
BaseCountedPtr<T>& BaseCountedPtr<T>::operator = (BaseCountedPtr<T>& Obj_)
{
  if (the_p != Obj_.Pointee())
  {
    if (Unique())
      delete the_p;

    the_p = Obj_.the_p;
    refCount = Obj_.refCount;
  }

  return *this;
}

/****************************************************************************/
template <class T>
BaseCountedPtr<T>& BaseCountedPtr<T>::operator = (T* rhs)
{
  if (the_p != rhs)
  {
    if (Unique())
      delete the_p;

    the_p = rhs;
    refCount = ReferenceCount();
  }

  return *this;
}

/****************************************************************************/
template <class T>
T* BaseCountedPtr<T>::operator-> ()
{
  if (the_p)
    return the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return the_p; // Should never reach this line
}

/****************************************************************************/
template <class T>
T& BaseCountedPtr<T>::operator * ()
{
  if (the_p)
    return *the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *the_p; // Should never reach this line
}

/****************************************************************************/
template <class T>
const T* BaseCountedPtr<T>::operator-> () const
{
  if (the_p)
    return the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return the_p; // Should never reach this line
}

/****************************************************************************/
template <class T>
const T& BaseCountedPtr<T>::operator * () const
{
  if (the_p)
    return *the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *the_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
void BaseCountedPtr<T>::SetToNull()
{
  the_p = NULL;
  refCount.Decrement();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* BaseCountedPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void BaseCountedPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* BaseCountedPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void BaseCountedPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if BASEPTR_DEBUG
class TestClass
{
  private:
    int x;

  public:
    TestClass():
      x(0) {}
    TestClass(int v):
      x(v) {}

    void Set(int v)
      { x = v; }
    int Get()
      { return x; }

    TestClass* Clone() const
      { return (new TestClass(*this)); }
};

int main()
{
  TestClass* tp1 = new TestClass(10);
  BaseCountedPtr<TestClass> Ptr1(tp1);
  BaseCountedPtr<TestClass>* Ptr2 = new BaseCountedPtr<TestClass>(Ptr1);

  assert(Ptr1->Get() == 10);
  assert((*Ptr2)->Get() == 10);
  Ptr1->Set((*Ptr2)->Get() * 2);
  assert((*Ptr2)->Get() == 20);
  assert(Ptr1->Get() == 20);
  Ptr1 = *Ptr2;
  assert(Ptr1->Get() == 20);
  delete Ptr2;
  assert(!Ptr1.IsNull() && Ptr1->Get() == 20);

  TestClass* tp3 = new TestClass(50);
  BaseCopiedPtr<TestClass> Ptr3(tp3);
  BaseCopiedPtr<TestClass>* Ptr4 = new BaseCopiedPtr<TestClass>(Ptr3);

  assert(Ptr3->Get() == 50);
  assert((*Ptr4)->Get() == 50);
  Ptr3->Set((*Ptr4)->Get() * 3);
  assert((*Ptr4)->Get() == 50);
  assert(Ptr3->Get() == 150);
  delete Ptr4;
  assert(!Ptr3.IsNull() && Ptr3->Get() == 150);

  return 0;
}
#endif
/****************************************************************************/
#endif





