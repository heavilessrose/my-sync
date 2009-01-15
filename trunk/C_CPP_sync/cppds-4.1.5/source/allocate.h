#ifndef ALLOCATE_H
#define ALLOCATE_H
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif

/****************************************************************************/
// Allocator and Deallocator classes for handling dynamic copying and erasing
// of objects. Allocation and deallocation is split into two separate classes
// to avoid the problem associated with objects held in abstract class
// pointers. Instances of abstract classes themselves cannot be created but,
// they can hold pointers to objects from classes derived from them. If these
// objects were created dynamically they can be destroyed through their base
// class pointers. This can be done through the deallocator object or a
// pointer to a deallocator object. The allocator class is derived from
// the deallocator class so that a base deallocator class pointer can hold
// an object of the allocator class which could have been created dynamically
// and invoke its virtual methods polymorphically through the pointer.
//
// These classes could allow for allocation or deallocation of dynamically
// created objects held in abstract pointer types which disallows construction
// of objects of its class. The evaluation of memory allocation and
// deallocation commands could be postponed until runtime where the
// polymorphic mechanism can finally resolve the actual method call through
// the base class pointer.
//
template <class T>
class MemManipulatorClass
{
  public:
    virtual MemManipulatorClass<T>& operator () (T* Ptr_)	= 0;
    virtual T* AllocMem() 	      				= 0;
    virtual T* EraseMem()					= 0;
};

template <class T>
class MemManipulatorImp : public MemManipulatorClass<T>
{
  protected:
    T* _Member;

    T* DeletePtr(T* Ptr_);
    T* SetToAddress(T* ptr)
	{ _Member = ptr;
	  return _Member; }
    T* GiveMember() const
	{ return _Member; }

  public:
    MemManipulatorImp(T* Ptr_);
    ~MemManipulatorImp();

    virtual MemManipulatorClass<T>& operator () (T* Ptr_);
};

template <class T>
class Deallocator : public MemManipulatorImp<T>
{
  friend MemManipulatorClass<T>& RootObject(Deallocator<T>& Obj_)
	{ return *((MemManipulatorClass<T>*)(&Obj_)); }
  friend const MemManipulatorClass<T>& RootConstObject(const Deallocator<T>& Obj_)
	{ return *((const MemManipulatorClass<T>*)(&Obj_)); }

  public:
    Deallocator(T* Ptr_);

    virtual T* AllocMem(); 		// stub does nothing, returns _Member
    virtual T* EraseMem();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

template <class T>
class Allocator : public MemManipulatorImp<T>
{
  friend MemManipulatorClass<T>& RootObject(Allocator<T>& Obj_)
	{ return *((MemManipulatorClass<T>*)(&Obj_)); }
  friend const MemManipulatorClass<T>& RootConstObject(const Allocator<T>& Obj_)
	{ return *((const MemManipulatorClass<T>*)(&Obj_)); }

  protected:
    T* AllocMem(T* Ptr_);
    void Xmem() const
	{ THROW (FallibleBase::OutOfMemoryErr()); }

  public:
    Allocator(T* Ptr_);

    virtual T* AllocMem();
    virtual T* EraseMem();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

// Automatic instance creation functions which creates an automatic instance
// of either the allocator or deallocator classes given the address to use
// for memory operations
//
template <class T>
Deallocator<T> MakeDeallocator(T* Ptr_)
{
  Deallocator<T> Deallocator_(Ptr_);
  return Deallocator_;
}

template <class T>
Allocator<T> MakeAllocator(T* Ptr_)
{
  Allocator<T> Allocator_(Ptr_);
  return Allocator_;
}

/****************************************************************************/
#endif




