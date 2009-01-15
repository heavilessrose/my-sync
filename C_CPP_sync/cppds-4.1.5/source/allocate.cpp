#ifndef ALLOCATE_CPP
#define ALLOCATE_CPP
#ifndef ALLOCATE_H
  #include "allocate.h"
#endif

/****************************************************************************/
/************************* Memory Manipulator Impl. *************************/
/****************************************************************************/
template <class T>
MemManipulatorImp<T>::MemManipulatorImp(T* Ptr_):
_Member(Ptr_)
{}

/****************************************************************************/
// Sets _Member to NULL. Probably unneccessary, but no harm done and it will
// insure that the compiler calls this method to set the member to NULL at
// destruction time instead of attempting to deallocate memory from _Member.
// The destructor is non-virtual so other derived classes of Deallocator
// won't be able to mess with _Member in their destructor since _Member
// basically acts as a intermediate storage variable and is not intended to
// be used directly.
//
template <class T>
MemManipulatorImp<T>::~MemManipulatorImp()
{
  _Member = NULL;
}

/****************************************************************************/
template <class T>
MemManipulatorClass<T>& MemManipulatorImp<T>::operator () (T* Ptr_)
{
  _Member = Ptr_;
  return *this;
}

/****************************************************************************/
template <class T>
T* MemManipulatorImp<T>::DeletePtr(T* Ptr_)
{
  ::Delete(Ptr_);
  Ptr_ = NULL;
  return Ptr_;
}

/****************************************************************************/
/************************* Memory Deallocator Class *************************/
/****************************************************************************/
template <class T>
Deallocator<T>::Deallocator(T* Ptr_):
MemManipulatorImp<T>(Ptr_)
{}

/****************************************************************************/
template <class T>
T* Deallocator<T>::AllocMem()
{
  return GiveMember();
}

/****************************************************************************/
template <class T>
T* Deallocator<T>::EraseMem()
{
  SetToAddress(DeletePtr(GiveMember()));
  return GiveMember();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* Deallocator<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Deallocator<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* Deallocator<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void Deallocator<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/************************** Memory Allocator Class **************************/
/****************************************************************************/
template <class T>
Allocator<T>::Allocator(T* Ptr_):
MemManipulatorImp<T>(Ptr_)
{}

/****************************************************************************/
template <class T>
T* Allocator<T>::AllocMem(T* Ptr_)
{
  if (Ptr_)
  {
    Ptr_ = ::Clone(*Ptr_);
    if (Ptr_ == NULL)
      Xmem();
  }

  return Ptr_;
}

/****************************************************************************/
template <class T>
T* Allocator<T>::AllocMem()
{
  SetToAddress(AllocMem(GiveMember()));
  return GiveMember();
}

/****************************************************************************/
template <class T>
T* Allocator<T>::EraseMem()
{
  SetToAddress(DeletePtr(GiveMember()));
  return GiveMember();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* Allocator<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Allocator<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* Allocator<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void Allocator<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
#endif




