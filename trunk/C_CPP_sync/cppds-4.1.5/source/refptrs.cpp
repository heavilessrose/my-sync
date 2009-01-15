#ifndef REFPTRS_CPP
#define REFPTRS_CPP
#ifndef REFPTRS_H
  #include "refptrs.h"
#endif

/****************************************************************************/
/************************** Member Selection Class **************************/
/****************************************************************************/
template <class T>
CPtr<T>::CPtr(const CopyPtrClass<T>* Ptr_):
_BossPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
CPtr<T>& CPtr<T>::operator () (const CopyPtrClass<T>* Ptr_)
{
  _BossPtr = Ptr_;
  return *this;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
const CopyPtrClass<T>* CPtr<T>::operator () () const
{
  if (_BossPtr == NULL)
    Xnull();

  return _BossPtr;
}

/****************************************************************************/
template <class T>
T* CPtr<T>::operator -> () const
{
  T* Ptr_ = (*this)()->Pointee();

  if (Ptr_ == NULL)
    Xnull();

  return Ptr_;
}

/****************************************************************************/
template <class T>
T& CPtr<T>::operator * () const
{
  return **((*this)());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* CPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void CPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/**************************** Copy Pointer Impl. ****************************/
/****************************************************************************/
template <class T>
CopyPtrImp<T>::CopyPtrImp():
_Pointee(NULL)
{}

/****************************************************************************/
template <class T>
CopyPtrImp<T>::CopyPtrImp(T* ptr):
_Pointee(ptr)
{}

/****************************************************************************/
template <class T>
T* CopyPtrImp<T>::Pointee() const
{
  return _Pointee;
}

/****************************************************************************/
template <class T>
T& CopyPtrImp<T>::operator * () const
{
  if (_Pointee == NULL)
    Xnull();

  return *_Pointee;
}

/****************************************************************************/
template <class T>
Boolean CopyPtrImp<T>::IsEqual(const CopyPtrClass<T>& Obj_) const
{
  return (Pointee() == Obj_.Pointee());
}

/****************************************************************************/
template <class T>
const char* CopyPtrImp<T>::ClassName() const
{
  return NameOf(*this);
}

/****************************************************************************/
/**************************** Reference Pointers ****************************/
/****************************************************************************/
template <class T>
RefPtr<T>::RefPtr():
CopyPtrImp<T>(NULL)
{}

/****************************************************************************/
template <class T>
RefPtr<T>::RefPtr(T* ptr):
CopyPtrImp<T>(ptr)
{}

/****************************************************************************/
template <class T>
RefPtr<T>::RefPtr(const CopyPtrClass<T>& ptr):
CopyPtrImp<T>(ptr.Pointee())
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
RefPtr<T>::RefPtr(const RefPtr<T>& Ptr_):
CopyPtrImp<T>(Ptr_.Pointee())
{}
#endif
/****************************************************************************/
template <class T>
RefPtr<T>::~RefPtr()
{
  Erase();
}

/****************************************************************************/
template <class T>
int RefPtr<T>::ClassID() const
{
  return TypeInfo::REFPTR_TYPE;
}

/****************************************************************************/
template <class T>
T* RefPtr<T>::ClearPtr()
{
  T* OldPtr_ = _Pointee;
  *this = ((T*)NULL);
  return OldPtr_;
}

/****************************************************************************/
template <class T>
CopyPtrClass<T>& RefPtr<T>::operator = (const CopyPtrClass<T>& ptr)
{
  if (this != &ptr)
  {
    Erase();
    Alloc(ptr.Pointee());
  }

  return *this;
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
RefPtr<T>& RefPtr<T>::operator = (const RefPtr<T>& ptr)
{
  if (this != &ptr)
  {
    Erase();
    Alloc(ptr.Pointee());
  }

  return *this;
}
#endif
/****************************************************************************/
template <class T>
CopyPtrClass<T>& RefPtr<T>::operator = (T* ptr)
{
  *this = RefPtr<T>(ptr);
  return *this;
}

/****************************************************************************/
template <class T>
void RefPtr<T>::Erase()
{
  SetToNull();
}

/****************************************************************************/
template <class T>
void RefPtr<T>::Alloc(T* ptr)
{
  SetToAddress(ptr);
}

/****************************************************************************/
template <class T>
Boolean RefPtr<T>::IsShallowCopy() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean RefPtr<T>::IsDeepCopy() const
{
  return FALSE;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* RefPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void RefPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* RefPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void RefPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/***************************** Memory Pointers ******************************/
/****************************************************************************/
template <class T>
MemPtr<T>::MemPtr()
{}

/****************************************************************************/
template <class T>
MemPtr<T>::MemPtr(T* ptr):
CopyPtrImp<T>(ptr ? ::Clone(*ptr):NULL)
{}

/****************************************************************************/
template <class T>
MemPtr<T>::MemPtr(const CopyPtrClass<T>& ptr):
CopyPtrImp<T>(ptr.Pointee() ? ::Clone(*ptr.Pointee()):NULL)
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
MemPtr<T>::MemPtr(const MemPtr<T>& Ptr_):
CopyPtrImp<T>(Ptr_.Pointee() ? ::Clone(*Ptr_.Pointee()):NULL)
{}
#endif
/****************************************************************************/
template <class T>
MemPtr<T>::~MemPtr()
{
  Erase();
}

/****************************************************************************/
template <class T>
int MemPtr<T>::ClassID() const
{
  return TypeInfo::MEMPTR_TYPE;
}

/****************************************************************************/
template <class T>
T* MemPtr<T>::ClearPtr()
{
  T* OldPtr_ = _Pointee;
  *this = ((T*)NULL);
  return OldPtr_;
}

/****************************************************************************/
template <class T>
CopyPtrClass<T>& MemPtr<T>::operator = (const CopyPtrClass<T>& ptr)
{
  if (this != &ptr)
  {
    Erase();
    Alloc(ptr.Pointee());
  }

  return *this;
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
MemPtr<T>& MemPtr<T>::operator = (const MemPtr<T>& ptr)
{
  if (this != &ptr)
  {
    Erase();
    Alloc(ptr.Pointee());
  }

  return *this;
}
#endif
/****************************************************************************/
template <class T>
CopyPtrClass<T>& MemPtr<T>::operator = (T* ptr)
{
  *this = RefPtr<T>(ptr);
  return *this;
}

/****************************************************************************/
template <class T>
void MemPtr<T>::Alloc(T* ptr)
{
  if (SetToAddress(ptr))
    if (SetToAddress(::Clone(*_Pointee)) == NULL)
      Xmem();
}

/****************************************************************************/
template <class T>
void MemPtr<T>::Erase()
{
  DeletePointee();
  SetToNull();
}

/****************************************************************************/
template <class T>
Boolean MemPtr<T>::IsShallowCopy() const
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean MemPtr<T>::IsDeepCopy() const
{
  return TRUE;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* MemPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void MemPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* MemPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void MemPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/***************************** Static Pointers ******************************/
/****************************************************************************/
template <class T>
StaticPtr<T>::StaticPtr():
_Instances(0)
{}

/****************************************************************************/
template <class T>
StaticPtr<T>::StaticPtr(T* Ptr_):
MemPtr<T>(Ptr_),
_Instances(Ptr_ ? 1:0)
{}

/****************************************************************************/
template <class T>
StaticPtr<T>::StaticPtr(const CopyPtrClass<T>& Ptr_):
MemPtr<T>(Ptr_),
_Instances(Ptr_.Pointee() ? 1:0)
{}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
StaticPtr<T>::StaticPtr(const StaticPtr<T>& Ptr_):
MemPtr<T>(Ptr_),
_Instances(Ptr_.Pointee() ? 1:0)
{}
#endif
/****************************************************************************/
template <class T>
StaticPtr<T>::~StaticPtr()
{
  Erase();
}

/****************************************************************************/
template <class T>
int StaticPtr<T>::ClassID() const
{
  return TypeInfo::STATICPTR_TYPE;
}

/****************************************************************************/
template <class T>
void StaticPtr<T>::Create(T& Obj_)
{
  if (!_Instances)
    Alloc(&Obj_);

  ++_Instances;
}

/****************************************************************************/
template <class T>
void StaticPtr<T>::Destroy()
{
  --_Instances;

  if (_Instances == 0)
  {
    Erase();
    _Instances = 0;
  }
}

/****************************************************************************/
template <class T>
T* StaticPtr<T>::ClearPtr()
{
  T* OldPtr_ = MemPtr<T>::ClearPtr();
  _Instances = _Pointee ? 1:0;
  return OldPtr_;
}

/****************************************************************************/
template <class T>
CopyPtrClass<T>& StaticPtr<T>::operator = (const CopyPtrClass<T>& Ptr_)
{
  if (this != &Ptr_)
  {
    Erase();
    Alloc(Ptr_.Pointee());

    _Instances = _Pointee ? 1:0;
  }

  return *this;
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
StaticPtr<T>& StaticPtr<T>::operator = (const StaticPtr<T>& Ptr_)
{
  if (this != &Ptr_)
  {
    Erase();
    Alloc(Ptr_.Pointee());

    _Instances = _Pointee ? 1:0;
  }

  return *this;
}
#endif
/****************************************************************************/
template <class T>
CopyPtrClass<T>& StaticPtr<T>::operator = (T* Ptr_)
{
  *this = RefPtr<T>(Ptr_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean StaticPtr<T>::IsShallowCopy() const
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean StaticPtr<T>::IsDeepCopy() const
{
  return TRUE;
}

/****************************************************************************/
#if REFPTRS_DEBUG
#if defined(__TURBOC__)
  #include <conio.h>
#endif

class REFPTRSTestClass
{
  DEFAULT_ROOTOBJECT_DEFN(REFPTRSTestClass)

  public:
    int* member;
    static StaticPtr<int> _StatMem;

    REFPTRSTestClass(){}
    REFPTRSTestClass(int m):
	member(new int(m))
	{ _StatMem.Create(m); }
    REFPTRSTestClass(const REFPTRSTestClass& obj):
	member(new int(*obj.member))
	{ _StatMem.Create(*obj.member); }
    ~REFPTRSTestClass()
	{ delete member;
	  _StatMem.Destroy(); }

    REFPTRSTestClass& operator = (REFPTRSTestClass& obj)
	{ if (this != &obj){
	    delete member;
	    member = new int(*obj.member);
	  }
	  *_StatMem = *obj.member;
	  return *this; }

    void SetMember(int x)
	{ delete member;
	  member = new int(x); }

    int GetMember()
	{ return *member; }

    StaticPtr<int>& GetPointer()
	{ return _StatMem; }
};

StaticPtr<int> REFPTRSTestClass::_StatMem;

int main()
{
#if defined(__TURBOC__)
  clrscr();
#endif

#if REFPTRS_DEBUG1
  int a = 6, d = 9;

  // Test cases
  RefPtr<int> intp(&a);
  cout <<*intp.Pointee() <<endl;

  // Testing Master pointer semantics
  MemPtr<int> Mint(&d);
  cout <<*Mint.Pointee() <<endl;

  // testing Boolean operator == (RefPtr<T>&, RefPtr<T>&);
  cout <<(Mint == intp) <<endl;
  cout <<(Mint != intp) <<endl;
  cout <<(Mint - intp) <<endl;

  intp.ClearPtr();
  Mint.ClearPtr();

  int m = 11, n = 12, p = 13, k = 14;
  int *Val_;

  REFPTRSTestClass* Testptr_;
  REFPTRSTestClass Testobj_(n);
  RefPtr<REFPTRSTestClass> Testcp_;
  CPtr<REFPTRSTestClass> Cptr(&Testcp_);

  // testing RefPtr<int>::operator = (T*)
  intp = &m;
  cout <<*intp.Pointee() <<endl;

  Testcp_ = MakeAllocator(&Testobj_).AllocMem();
  cout <<Cptr->GetMember() <<endl;
  cout <<Testcp_.Pointee()->GetMember() <<endl;

  Testptr_ = Testcp_.ClearPtr();
  delete Testptr_;

  // testing RefPtr<T>::RefPtr(T*) conversion constructor
  intp = &p;
  cout <<*intp.Pointee() <<endl;

  // testing RefPtr<int>(T*)
  // testing RefPtr<T>& RefPtr<T>::operator = (RefPtr<T>&)
  Val_ = new int(k + 5);
  intp = RefPtr<int>(Val_);
  cout <<*intp.Pointee() <<endl;

  // testing MemPtr<T>::MemPtr(T*) conversion constructor
  Mint = &k;
  cout <<*Mint.Pointee() <<endl;

  // testing virtual RefPtr<T>& RefPtr<T>::operator = (RefPtr<T>&)
  Mint = intp;
  cout <<*Mint.Pointee() <<endl;
  delete Val_;
#endif

#if REFPTRS_DEBUG2
  int r = 13, nv = 66;
  int *sp, *tp;

  MemPtr<int> Memint(&r);

  cout <<"r   : " <<r <<endl
       <<endl;

  // testing PtrRef_t<T>::operator T* ()
  cout <<"Pointee()  : " <<Memint.Pointee() <<endl
       <<"*Pointee() : " <<Memint.Pointee() <<endl
       <<endl;

  sp = Memint.Pointee();
  cout <<"sp  : " <<sp <<endl
       <<"*sp : " <<*sp <<endl
       <<endl;

  Memint = &nv;
  tp = Memint.Pointee();
  cout <<"tp  : " <<tp <<endl
       <<"*tp : " <<*tp <<endl
       <<endl;
  assert(*tp == 66);

  cout <<"Pointee()  : " <<Memint.Pointee() <<endl
       <<"*Pointee() : " <<*Memint.Pointee() <<endl
       <<endl;
#endif

#if REFPTRS_DEBUG4
  // testing static pointer classes.
  // Changes to static pointers should be universal through all objects of
  // the same class. Changes of the static member in one object should be
  // effective in all other objects. Deletion of the static member should
  // only occur when the last object of the class containing the static
  // member is destroyed.

  REFPTRSTestClass StaticPtr1_(15);
  REFPTRSTestClass StaticPtr2_(21);
  REFPTRSTestClass StaticPtr3_(46);
  REFPTRSTestClass StaticPtr4_(StaticPtr3_);
  REFPTRSTestClass StaticPtr5_(98);

  cout <<(*StaticPtr5_.GetPointer()) <<endl;	// should be 15
  cout <<StaticPtr5_.GetMember() <<endl;	// should be 98

  cout <<StaticPtr1_.GetMember() <<endl;	// should be 15
  StaticPtr1_ = StaticPtr2_;                    // _StatMem = 21
  cout <<StaticPtr1_.GetMember() <<endl;	// should be 21

  cout <<StaticPtr4_.GetMember() <<endl;	// should be 46
  StaticPtr4_.SetMember(StaticPtr1_.GetMember());  // _StatMem is skipped
  cout <<StaticPtr4_.GetMember() <<endl;	// should be 21

  cout <<StaticPtr3_.GetMember() <<endl;	// should be 46
  StaticPtr3_ = StaticPtr4_;                    // _StatMem = 21
  cout <<StaticPtr3_.GetMember() <<endl;	// should be 21

  cout <<(*StaticPtr5_.GetPointer()) <<endl;	// should be 21
  cout <<StaticPtr5_.GetMember() <<endl;	// should be 98

  // check for proper deletion of static pointer member
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif





