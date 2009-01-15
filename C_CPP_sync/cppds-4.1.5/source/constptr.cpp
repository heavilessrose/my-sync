#ifndef CONSTPTR_CPP
#define CONSTPTR_CPP
#ifndef CONSTPTR_H
  #include "constptr.h"
#endif

/****************************************************************************/
template <class T>
CopiedPtrData<T>::CopiedPtrData(const T* just_newed, ObjectUpdater* Deleter_):
const_p(just_newed),
_Deleter(Deleter_),
_Modified(FALSE)
{}

/****************************************************************************/
template <class T>
CopiedPtrData<T>::CopiedPtrData(const CopiedPtrData<T>& Obj_):
const_p(Obj_.const_p ? ::Clone(*Obj_.const_p):NULL),
_Deleter(Obj_._Deleter),
_Modified(Obj_._Modified)
{}

/****************************************************************************/
template <class T>
void CopiedPtrData<T>::ErasePointee()
{
  if (!_Deleter.IsNull())
    _Deleter->DoUpdate(the_p, FALSE);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean operator == (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs)
{
  return lhs.Pointee() == rhs.Pointee();
}

/****************************************************************************/
template <class T>
Boolean operator != (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs)
{
  return lhs.Pointee() != rhs.Pointee();
}

/****************************************************************************/
template <class T>
Boolean operator < (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs)
{
  return lhs.Pointee() < rhs.Pointee();
}

/****************************************************************************/
template <class T>
Boolean operator <= (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs)
{
  return lhs.Pointee() <= rhs.Pointee();
}

/****************************************************************************/
template <class T>
Boolean operator > (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs)
{
  return lhs.Pointee() > rhs.Pointee();
}

/****************************************************************************/
template <class T>
Boolean operator >= (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs)
{
  return lhs.Pointee() >= rhs.Pointee();
}

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>::ConstCopiedVoidPtr(ObjectUpdater* Deleter_):
_Data(NULL, Deleter_)
{}                              // Construct as null pointer

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>::ConstCopiedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_):
_Data(just_newed, Deleter_)
{}        // Construct from new pointer

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>::ConstCopiedVoidPtr(const T& obj, ObjectUpdater* Deleter_):
_Data(::Clone(obj), Deleter_)
{}

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>::ConstCopiedVoidPtr(const ConstCopiedVoidPtr<T>& Obj_):
_Data(Obj_._Data)
{}

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>::~ConstCopiedVoidPtr()
{
  _Data.ErasePointee();
  _Data.the_p = NULL;
  _Data._Modified = TRUE;
}

/****************************************************************************/
template <class T>
int ConstCopiedVoidPtr<T>::ClassID() const
{
  return TypeInfo::CONSTCOPIEDVOIDPTR_TYPE;
}

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>* ConstCopiedVoidPtr<T>::Clone() const
{
  return (new ConstCopiedVoidPtr<T>(*this));
}

/****************************************************************************/
template <class T>
Boolean ConstCopiedVoidPtr<T>::operator ! () const
{
  return !_Data.const_p;
}

/****************************************************************************/
template <class T>
Boolean ConstCopiedVoidPtr<T>::IsNull() const
{
  return _Data.const_p == 0;
}

/****************************************************************************/
template <class T>
T* ConstCopiedVoidPtr<T>::VarPointee()
{
  _Data._Modified = TRUE;
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* ConstCopiedVoidPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
const T* ConstCopiedVoidPtr<T>::ConstPointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
Boolean ConstCopiedVoidPtr<T>::IsEqual(const ConstCopiedVoidPtr& Obj_) const
{
  return (ConstPointee() == Obj_.ConstPointee());
}

/****************************************************************************/
template <class T>
Boolean ConstCopiedVoidPtr<T>::IsGreater(const ConstCopiedVoidPtr& Obj_) const
{
  return (ConstPointee() > Obj_.ConstPointee());
}

/****************************************************************************/
template <class T>
Boolean ConstCopiedVoidPtr<T>::IsLesser(const ConstCopiedVoidPtr& Obj_) const
{
  return (ConstPointee() < Obj_.ConstPointee());
}

/****************************************************************************/
template <class T>
const BaseCopiedPtr<ObjectUpdater>& ConstCopiedVoidPtr<T>::GiveDeleter() const
{
  return _Data._Deleter;
}

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>& ConstCopiedVoidPtr<T>::operator = (T* just_newed)
{
  if (_Data.the_p != just_newed)
  {
    _Data.ErasePointee();
    _Data.the_p = just_newed;
    _Data._Modified = TRUE;
  }

  return *this;
}

/****************************************************************************/
template <class T>
ConstCopiedVoidPtr<T>& ConstCopiedVoidPtr<T>::operator = (const ConstCopiedVoidPtr<T>& Obj_)
{
  if (_Data.const_p != Obj_.Pointee())
  {
    _Data.ErasePointee();
    _Data.the_p = Obj_.IsNull() ? 0 : ::Clone(*Obj_.Pointee());
    _Data._Deleter = Obj_.GiveDeleter();
    _Data._Modified = TRUE;
  }

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstCopiedVoidPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstCopiedVoidPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ConstCopiedVoidPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ConstCopiedVoidPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
ConstCopiedBuiltInPtr<T>::ConstCopiedBuiltInPtr():
ConstCopiedVoidPtr<T>(BuiltInDeleter::Make())
{}                              // Construct as null pointer

/****************************************************************************/
template <class T>
ConstCopiedBuiltInPtr<T>::ConstCopiedBuiltInPtr(const T* just_newed):
ConstCopiedVoidPtr<T>(just_newed, BuiltInDeleter::Make())
{}        // Construct from new pointer

/****************************************************************************/
template <class T>
ConstCopiedBuiltInPtr<T>::ConstCopiedBuiltInPtr(const T& obj):
ConstCopiedVoidPtr<T>(obj, BuiltInDeleter::Make())
{}

/****************************************************************************/
template <class T>
ConstCopiedBuiltInPtr<T>::ConstCopiedBuiltInPtr(const ConstCopiedVoidPtr<T>& Obj_):
ConstCopiedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ConstCopiedBuiltInPtr<T>::ConstCopiedBuiltInPtr(const ConstCopiedBuiltInPtr<T>& aCP):
ConstCopiedVoidPtr<T>(aCP)
{}

/****************************************************************************/
template <class T>
ConstCopiedBuiltInPtr<T>* ConstCopiedBuiltInPtr<T>::Clone() const
{
  return (new ConstCopiedBuiltInPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstCopiedBuiltInPtr<T>::ClassID() const
{
  return TypeInfo::CONSTCOPIEDBUILTINPTR_TYPE;
}

/****************************************************************************/
template <class T>
const T& ConstCopiedBuiltInPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstCopiedBuiltInPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstCopiedBuiltInPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ConstCopiedBuiltInPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ConstCopiedBuiltInPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
ConstCopiedObjPtr<T>::ConstCopiedObjPtr():
ConstCopiedVoidPtr<T>(TypedObjectDeleter<T>::Make())
{}

/****************************************************************************/
template <class T>
ConstCopiedObjPtr<T>::ConstCopiedObjPtr(const T* just_newed):
ConstCopiedVoidPtr<T>(just_newed, TypedObjectDeleter<T>::Make())
{}

/****************************************************************************/
template <class T>
ConstCopiedObjPtr<T>::ConstCopiedObjPtr(const T& Obj_):
ConstCopiedVoidPtr<T>(Obj_, TypedObjectDeleter<T>::Make())
{}

/****************************************************************************/
template <class T>
ConstCopiedObjPtr<T>::ConstCopiedObjPtr(const ConstCopiedObjPtr<T>& Obj_):
ConstCopiedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ConstCopiedObjPtr<T>::ConstCopiedObjPtr(const ConstCopiedVoidPtr<T>& Obj_):
ConstCopiedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ConstCopiedObjPtr<T>* ConstCopiedObjPtr<T>::Clone() const
{
  return (new ConstCopiedObjPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstCopiedObjPtr<T>::ClassID() const
{
  return TypeInfo::CONSTCOPIEDOBJPTR_TYPE;
}

/****************************************************************************/
template <class T>
const T& ConstCopiedObjPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p;
}

/****************************************************************************/
template <class T>
const T* ConstCopiedObjPtr<T>::operator-> () const
{
  if (_Data.const_p)
    return _Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.const_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstCopiedObjPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstCopiedObjPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ConstCopiedObjPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ConstCopiedObjPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
CountedPtrData<T>::CountedPtrData(const T* just_newed, ObjectUpdater* Deleter_, Boolean OwnsPtr_):
const_p(just_newed),
_Deleter(Deleter_),
_Ownership(OwnsPtr_),
_Modified(FALSE)
{}

/****************************************************************************/
template <class T>
void CountedPtrData<T>::ErasePointee()
{
  if (!_Deleter.IsNull())
    _Deleter->DoUpdate(the_p, FALSE);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean operator == (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs)
{
  return (lhs.Pointee() == rhs.Pointee());
}

/****************************************************************************/
template <class T>
Boolean operator != (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs)
{
  return (lhs.Pointee() != rhs.Pointee());
}

/****************************************************************************/
template <class T>
Boolean operator < (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs)
{
  return (lhs.Pointee() < rhs.Pointee());
}

/****************************************************************************/
template <class T>
Boolean operator <= (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs)
{
  return (lhs.Pointee() <= rhs.Pointee());
}

/****************************************************************************/
template <class T>
Boolean operator > (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs)
{
  return (lhs.Pointee() > rhs.Pointee());
}

/****************************************************************************/
template <class T>
Boolean operator >= (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs)
{
  return (lhs.Pointee() >= rhs.Pointee());
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>::ConstCountedVoidPtr(ObjectUpdater* Deleter_):
_Data(NULL, Deleter_, FALSE)
{}                             // Construct as null pointer

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>::ConstCountedVoidPtr(const ConstCountedVoidPtr<T>& Obj_):
_Data(Obj_._Data)
{}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>::ConstCountedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_, Boolean OwnsPtr_):
_Data(just_newed, Deleter_, OwnsPtr_)
{}

/****************************************************************************/
template<class T>
ConstCountedVoidPtr<T>::~ConstCountedVoidPtr()
{
  if (Unique() && HasOwnership())
  {
    _Data.ErasePointee();
    _Data.the_p = NULL;
    _Data._Modified = TRUE;
  }
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>* ConstCountedVoidPtr<T>::Clone() const
{
  return (new ConstCountedVoidPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstCountedVoidPtr<T>::ClassID() const
{
  return TypeInfo::CONSTCOUNTEDVOIDPTR_TYPE;
}

/****************************************************************************/
template <class T>
const BaseCountedPtr<ObjectUpdater>& ConstCountedVoidPtr<T>::GiveDeleter() const
{
  return _Data._Deleter;
}

/****************************************************************************/
template <class T>
BaseCountedPtr<ObjectUpdater>& ConstCountedVoidPtr<T>::GiveDeleter()
{
  return _Data._Deleter;
}

/****************************************************************************/
template <class T>
const ReferenceCount& ConstCountedVoidPtr<T>::GiveRefCount() const
{
  return _Data.refCount;
}

/****************************************************************************/
template <class T>
T* ConstCountedVoidPtr<T>::VarPointee()
{
  _Data._Modified = TRUE;
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
Boolean ConstCountedVoidPtr<T>::HasOwnership() const
{
  return _Data._Ownership;
}

/****************************************************************************/
template <class T>
Boolean ConstCountedVoidPtr<T>::Unique() const
{
  return _Data.refCount.Unique();
}

/****************************************************************************/
template <class T>
Boolean ConstCountedVoidPtr<T>::operator ! () const
{
  return !_Data.const_p;
}

/****************************************************************************/
template <class T>
Boolean ConstCountedVoidPtr<T>::IsNull() const
{
  return _Data.const_p == 0;
}

/****************************************************************************/
template <class T>
const T* ConstCountedVoidPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
const T* ConstCountedVoidPtr<T>::ConstPointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
Boolean ConstCountedVoidPtr<T>::IsEqual(const ConstCountedVoidPtr<T>& Obj_) const
{
  return (ConstPointee() == Obj_.ConstPointee());
}

/****************************************************************************/
template <class T>
Boolean ConstCountedVoidPtr<T>::IsGreater(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_) const
{
  if (!Ptr_)
    return Obj_.IsLesser(*this, this);
  
  return (ConstPointee() > Obj_.ConstPointee());
}

/****************************************************************************/
template <class T>
Boolean ConstCountedVoidPtr<T>::IsLesser(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_) const
{
  if (!Ptr_)
    return Obj_.IsGreater(*this, this);
    
  return (ConstPointee() < Obj_.ConstPointee());
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>& ConstCountedVoidPtr<T>::operator = (ConstCountedVoidPtr<T>& Obj_)
{
  if (_Data.const_p != Obj_.Pointee())
  {
    if (Unique() && HasOwnership())
      _Data.ErasePointee();

    _Data.the_p = Obj_.VarPointee();
    _Data.refCount = Obj_.GiveRefCount();
    _Data._Ownership = Obj_.HasOwnership();
    _Data._Deleter = Obj_.GiveDeleter();
    _Data._Modified = TRUE;
  }

  return *this;
}

/****************************************************************************/
template<class T>
ConstCountedVoidPtr<T>& ConstCountedVoidPtr<T>::operator = (T* just_newed)
{
  if (_Data.the_p != just_newed)
  {
    if (Unique() && HasOwnership())
      _Data.ErasePointee();

    _Data.the_p = just_newed;
    _Data.refCount = ReferenceCount();
    _Data._Modified = TRUE;
  }

  return *this;
}

/****************************************************************************/
template<class T>
void ConstCountedVoidPtr<T>::SetToNull()
{
  _Data.the_p = NULL;
  _Data.refCount.Decrement();
  _Data._Modified = TRUE;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstCountedVoidPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstCountedVoidPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ConstCountedVoidPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ConstCountedVoidPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template<class T>
ConstCountedObjPtr<T>::ConstCountedObjPtr():
ConstCountedVoidPtr<T>(TypedObjectDeleter<T>::Make())
{}

/****************************************************************************/
template<class T>
ConstCountedObjPtr<T>::ConstCountedObjPtr(const ConstCountedVoidPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template<class T>
ConstCountedObjPtr<T>::ConstCountedObjPtr(const ConstCountedObjPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template<class T>
ConstCountedObjPtr<T>::ConstCountedObjPtr(const T* just_newed, Boolean OwnsPtr_):
ConstCountedVoidPtr<T>(just_newed, TypedObjectDeleter<T>::Make(), OwnsPtr_)
{}

/****************************************************************************/
template <class T>
ConstCountedObjPtr<T>* ConstCountedObjPtr<T>::Clone() const
{
  return (new ConstCountedObjPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstCountedObjPtr<T>::ClassID() const
{
  return TypeInfo::CONSTCOUNTEDOBJPTR_TYPE;
}

/****************************************************************************/
template <class T>
const T& ConstCountedObjPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p; // Should never reach this line
}

/****************************************************************************/
template <class T>
const T* ConstCountedObjPtr<T>::operator-> () const
{
  if (_Data.const_p)
    return _Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.const_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstCountedObjPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstCountedObjPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ConstCountedObjPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ConstCountedObjPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
ConstCountedBuiltInPtr<T>::ConstCountedBuiltInPtr():
ConstCountedVoidPtr<T>(BuiltInDeleter::Make())
{}                  	// Construct as null pointer

/****************************************************************************/
template <class T>
ConstCountedBuiltInPtr<T>::ConstCountedBuiltInPtr(const ConstCountedVoidPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ConstCountedBuiltInPtr<T>::ConstCountedBuiltInPtr(const ConstCountedBuiltInPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ConstCountedBuiltInPtr<T>::ConstCountedBuiltInPtr(const T* just_newed, Boolean OwnsPtr_):
ConstCountedVoidPtr<T>(just_newed, BuiltInDeleter::Make(), OwnsPtr_)
{}       		// Construct pointing at heap object

/****************************************************************************/
template <class T>
ConstCountedBuiltInPtr<T>* ConstCountedBuiltInPtr<T>::Clone() const
{
  return (new ConstCountedBuiltInPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstCountedBuiltInPtr<T>::ClassID() const
{
  return TypeInfo::CONSTCOUNTEDBUILTINPTR_TYPE;
}

/****************************************************************************/
template <class T>
const T& ConstCountedBuiltInPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p; // Should never reach this line
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstCountedBuiltInPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstCountedBuiltInPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ConstCountedBuiltInPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ConstCountedBuiltInPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template<class T>
ConstUpdatePtr<T>::ConstUpdatePtr(ObjectUpdater* Deleter_,
                                  ObjectUpdater* Updater_,
                                  ObjectUpdater* Validator_):
ConstCountedVoidPtr<T>(Deleter_),
_Updater(UpdaterBindingList::Make(Updater_)->HaltOnFirstFalse(FALSE)),
_Validator(UpdaterBindingList::Make(Validator_)->HaltOnFirstFalse(TRUE))
{}

/****************************************************************************/
template<class T>
ConstUpdatePtr<T>::ConstUpdatePtr(const ConstCountedVoidPtr<T>& Obj_,
                                  ObjectUpdater* Updater_,
                                  ObjectUpdater* Validator_):
ConstCountedVoidPtr<T>(Obj_),
_Updater(UpdaterBindingList::Make(Updater_)->HaltOnFirstFalse(FALSE)),
_Validator(UpdaterBindingList::Make(Validator_)->HaltOnFirstFalse(TRUE))
{}

/****************************************************************************/
template<class T>
ConstUpdatePtr<T>::ConstUpdatePtr(const ConstUpdatePtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_),
_Updater(Obj_._Updater),
_Validator(Obj_._Validator)
{}

/****************************************************************************/
template<class T>
ConstUpdatePtr<T>::~ConstUpdatePtr()
{
  Update();
}

/****************************************************************************/
template <class T>
ConstUpdatePtr<T>* ConstUpdatePtr<T>::Clone() const
{
  return (new ConstUpdatePtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstUpdatePtr<T>::ClassID() const
{
  return TypeInfo::CONSTUPDATEPTR_TYPE;
}

/****************************************************************************/
template <class T>
ConstUpdatePtr<T>& ConstUpdatePtr<T>::SetUpdater(ObjectUpdater* Updater_)
{
  _Updater = UpdaterBindingList::Make(Updater_)->HaltOnFirstFalse(FALSE);
  return *this;
}

/****************************************************************************/
template <class T>
ConstUpdatePtr<T>& ConstUpdatePtr<T>::SetValidator(ObjectUpdater* Validator_)
{
  _Validator = UpdaterBindingList::Make(Validator_)->HaltOnFirstFalse(TRUE);
  return *this;
}

/****************************************************************************/
template <class T>
UpdaterBindings* ConstUpdatePtr<T>::GetUpdater() const
{
  return _Updater.Pointee();
}

/****************************************************************************/
template <class T>
UpdaterBindings* ConstUpdatePtr<T>::GetValidator() const
{
  return _Validator.Pointee();
}

/****************************************************************************/
template <class T>
Boolean ConstUpdatePtr<T>::Update() const
{
  Boolean Result_ = FALSE;

  if (!IsNull(_Updater) && _Data._Modified && Validate())
  {
    Result_ = _Updater->Update(_Data.the_p);

    if (Result_)
    {
      ConstUpdatePtr<T> *const LocalThis_ = (ConstUpdatePtr<T> *const)this;
      LocalThis_->_Data._Modified = FALSE;
    }
  }

  return Result_;
}

/****************************************************************************/
template <class T>
Boolean ConstUpdatePtr<T>::Validate() const
{
  if (!IsNull(_Validator))
    return _Validator->Update(_Data.the_p);

  return TRUE;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstUpdatePtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template<class T>
void ConstUpdatePtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template<class T>
void* ConstUpdatePtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template<class T>
void ConstUpdatePtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean operator <= (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs)
{
  return ((lhs - rhs) <= 0);
}

/****************************************************************************/
template <class T>
Boolean operator >= (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs)
{
  return ((lhs - rhs) >= 0);
}

/****************************************************************************/
template <class T>
Boolean operator < (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs)
{
  return ((lhs - rhs) < 0);
}

/****************************************************************************/
template <class T>
Boolean operator > (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs)
{
  return ((lhs - rhs) > 0);
}

/****************************************************************************/
template <class T>
const ConstArrayPtr<T> operator + (ptrdiff_t Index_, const ConstArrayPtr<T>& Rhs_)
{
  return (ConstArrayPtr<T>(Rhs_) += Index_);
}

/****************************************************************************/
template <class T>
const ConstArrayPtr<T> operator - (ptrdiff_t Index_, const ConstArrayPtr<T>& Rhs_)
{
  return (ConstArrayPtr<T>(Rhs_) -= Index_);
}

/****************************************************************************/
template <class T>
ptrdiff_t operator - (const T* Ptr_, const ConstArrayPtr<T>& Rhs_)
{
  return (Ptr_ - Rhs_.base_p);
}

/****************************************************************************/
template<class T>
ConstArrayPtr<T>::ConstArrayPtr(ObjectUpdater* Deleter_):
ConstCountedVoidPtr<T>(Deleter_),
_Size(1),
base_p(NULL)
{}

/****************************************************************************/
template<class T>
ConstArrayPtr<T>::ConstArrayPtr(const ConstArrayPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_),
_Size(Obj_._Size),
base_p(Obj_.Pointee())
{}

/****************************************************************************/
template<class T>
ConstArrayPtr<T>::ConstArrayPtr(const ConstCountedVoidPtr<T>& Obj_, size_t Size_, const T* Base_):
ConstCountedVoidPtr<T>(Obj_),
_Size(Size_),
base_p(Base_ ? Base_:Obj_.Pointee())
{}

/****************************************************************************/
template <class T>
ConstArrayPtr<T>* ConstArrayPtr<T>::Clone() const
{
  return (new ConstArrayPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstArrayPtr<T>::ClassID() const
{
  return TypeInfo::CONSTARRAYPTR_TYPE;
}

/****************************************************************************/
template<class T>
ConstArrayPtr<T>& ConstArrayPtr<T>::operator += (ptrdiff_t Index_)
{
  if (_Data.const_p)
    if (Pointee() + Index_ < base_p || Pointee() + Index_ >= base_p + _Size)
      THROW (LoneFallibleBase::ArrayIndexRangeErr());
    else
    {
      _Data.the_p += Index_;
      return *this;
    }

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *this; // Should never reach this line
}

/****************************************************************************/
template<class T>
ConstArrayPtr<T>& ConstArrayPtr<T>::operator -= (ptrdiff_t Index_)
{
  return (*this += (-Index_));
}

/****************************************************************************/
template<class T>
const T& ConstArrayPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
const T& ConstArrayPtr<T>::operator [] (ptrdiff_t Index_) const
{
  if (_Data.const_p)
    if (Pointee() + Index_ < base_p || Pointee() + Index_ >= base_p + _Size)
      THROW (LoneFallibleBase::ArrayIndexRangeErr());
    else
      return *(Pointee() + Index_);

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
const ConstArrayPtr<T> ConstArrayPtr<T>::operator + (ptrdiff_t Index_) const
{
  return (ConstArrayPtr<T>(*this) += Index_);
}

/****************************************************************************/
template<class T>
const ConstArrayPtr<T> ConstArrayPtr<T>::operator - (ptrdiff_t Index_) const
{
  return (ConstArrayPtr<T>(*this) -= Index_);
}

/****************************************************************************/
template<class T>
ptrdiff_t ConstArrayPtr<T>::operator - (const ConstArrayPtr<T>& Ptr_) const
{
  if (base_p != Ptr_.base_p)
    THROW (LoneFallibleBase::NotSameBasePointerErr());

  return (base_p - Ptr_.base_p);
}

/****************************************************************************/
template<class T>
ptrdiff_t ConstArrayPtr<T>::operator - (const T* Ptr_) const
{
  return (base_p - Ptr_);
}

/****************************************************************************/
template <class T>
Boolean ConstArrayPtr<T>::IsGreater(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_) const
{
  if (!Ptr_)
    return Obj_.IsLesser(*this, this);
    
  return ((*this - ((ConstArrayPtr<T>&)Obj_)) > 0);
}

/****************************************************************************/
template <class T>
Boolean ConstArrayPtr<T>::IsLesser(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_) const
{
  if (!Ptr_)
    return Obj_.IsGreater(*this, this);

  return ((*this - ((ConstArrayPtr<T>&)Obj_)) < 0);
}

/****************************************************************************/
#if OVERLOAD_NEW
template<class T>
void* ConstArrayPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template<class T>
void ConstArrayPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template<class T>
void* ConstArrayPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template<class T>
void ConstArrayPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template<class T>
ConstStaticObjPtr<T>::ConstStaticObjPtr():
ConstCountedVoidPtr<T>(TypedObjectDeleter<T>::Make())
{}

/****************************************************************************/
template<class T>
ConstStaticObjPtr<T>::ConstStaticObjPtr(const T* just_newed):
ConstCountedVoidPtr<T>(just_newed, TypedObjectDeleter<T>::Make(), TRUE)
{}

/****************************************************************************/
template<class T>
ConstStaticObjPtr<T>::ConstStaticObjPtr(const ConstCountedVoidPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ConstStaticObjPtr<T>* ConstStaticObjPtr<T>::Clone() const
{
  return (new ConstStaticObjPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ConstStaticObjPtr<T>::ClassID() const
{
  return TypeInfo::CONSTSTATICOBJPTR_TYPE;
}

/****************************************************************************/
template<class T>
const T& ConstStaticObjPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
const T* ConstStaticObjPtr<T>::operator-> () const
{
  if (_Data.const_p)
    return _Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.const_p;
}

/****************************************************************************/
template<class T>
ConstStaticObjPtr<T>& ConstStaticObjPtr<T>::Increment()
{
  _Data.refCount.Increment();
  return *this;
}

/****************************************************************************/
template<class T>
ConstStaticObjPtr<T>& ConstStaticObjPtr<T>::Decrement()
{
  _Data.refCount.Decrement();
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstStaticObjPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstStaticObjPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ConstStaticObjPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ConstStaticObjPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* ConstProxyPtr<CLIENT, SPONSOR, LETTER>::Dispatch() const
{
  ::NotImplemented("ConstProxyPtr", "Object* Dispatch()");
  exit(-1);
  return this;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp():
_BossPtr(NULL, FALSE),
_Bindings(),
_Type(BindingType::NOBINDINGS),
_NotificationCode(0),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_BossPtr(Obj_.ClientPtr()),
_Bindings(Obj_.Bindings(), Obj_.BindingType()),
_Type(Obj_.BindingType()),
_NotificationCode(Obj_.NotificationCode()),
_Sponsor(Obj_.Sponsor())
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>& Obj_):
_BossPtr(Obj_._BossPtr),
_Bindings(Obj_._Bindings, Obj_._Type),
_Type(Obj_._Type),
_NotificationCode(Obj_._NotificationCode),
_Sponsor(Obj_._Sponsor)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_ConstType Method_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::CONSTBINDING0P),
_NotificationCode(0),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_ConstType Method_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::CONSTBINDING1P),
_NotificationCode(IdCode_),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::CONSTBINDING2P),
_NotificationCode(IdCode_),
_Sponsor(Sponsor_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_ConstType Method_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::CONSTBINDING0R),
_NotificationCode(0),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_ConstType Method_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::CONSTBINDING1R),
_NotificationCode(IdCode_),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::CONSTBINDING2R),
_NotificationCode(IdCode_),
_Sponsor(Sponsor_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>* ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::Clone() const
{
  return (new ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
int ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::ClassID() const
{
  return TypeInfo::CONSTPROXYPTRIMP_TYPE;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
Boolean ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::IsPtrLetter() const
{
  return
  (
    _Type == BindingType::BINDING0P ||
    _Type == BindingType::BINDING1P ||
    _Type == BindingType::BINDING2P ||
  
    _Type == BindingType::CONSTBINDING0P ||
    _Type == BindingType::CONSTBINDING1P ||
    _Type == BindingType::CONSTBINDING2P
  );
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
const LETTER* ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::GetLetterPointer() const
{
  long Code_ = _NotificationCode;
  if (!Code_ && _Sponsor)
    Code_ = CountOf(*_Sponsor);
  
  if (!_BossPtr.IsNull() && _Type != BindingType::NOBINDINGS)
    return
    (
      (_Type == BindingType::CONSTBINDING0P) ?
             (_BossPtr.Pointee()->*_Bindings._Bind0.PtrTo_ConstPtr)():
      (_Type == BindingType::CONSTBINDING1P) ?
             (_BossPtr.Pointee()->*_Bindings._Bind1.PtrTo_ConstPtr)(Code_):
      (_Type == BindingType::CONSTBINDING2P && _Sponsor) ?
             (_BossPtr.Pointee()->*_Bindings._Bind2.PtrTo_ConstPtr)(*_Sponsor, Code_):
             ((LETTER*)NULL)
    );

  return ((LETTER*)NULL);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
const LETTER& ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::GetLetterObject() const
{
  static LETTER* Dummy_;
  
  long Code_ = _NotificationCode;
  if (!Code_ && _Sponsor)
    Code_ = CountOf(*_Sponsor);

  if (!_BossPtr.IsNull() && _Type != BindingType::NOBINDINGS &&
      (_Type != BindingType::CONSTBINDING2 || _Sponsor))
    return
    (
      (_Type == BindingType::CONSTBINDING0R) ?
             (_BossPtr.Pointee()->*_Bindings._Bind0.RefTo_ConstPtr)():
      (_Type == BindingType::CONSTBINDING1R) ?
             (_BossPtr.Pointee()->*_Bindings._Bind1.RefTo_ConstPtr)(Code_):
      (_Type == BindingType::CONSTBINDING2R && _Sponsor) ?
             (_BossPtr.Pointee()->*_Bindings._Bind2.RefTo_ConstPtr)(*_Sponsor, Code_):
             *Dummy_
    );

  THROW (LoneFallibleBase::DerefNullPointerErr());  
  return *Dummy_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ConstProxyPointerPtr():
_Letter(TRUE)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ConstProxyPointerPtr(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ConstProxyPointerPtr(const ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ConstProxyPointerPtr(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_ConstType Method_):
_Letter(Base_, Method_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ConstProxyPointerPtr(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_ConstType Method_, long IdCode_):
_Letter(Base_, Method_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ConstProxyPointerPtr(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_):
_Letter(Base_, Method_, Sponsor_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>* ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::Clone() const
{
  return (new ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
int ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ClassID() const
{
  return TypeInfo::CONSTPROXYPOINTERPTR_TYPE;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator const LETTER* () const
{
  const LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
  return EncPtr_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
const LETTER& ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator * () const
{
  const LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
    
  if (EncPtr_ == NULL)
    THROW (LoneFallibleBase::DerefNullPointerErr());

  return *EncPtr_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ConstProxyObjectPtr():
_Letter(FALSE)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ConstProxyObjectPtr(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ConstProxyObjectPtr(const ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ConstProxyObjectPtr(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_ConstType Method_):
_Letter(Base_, Method_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ConstProxyObjectPtr(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_ConstType Method_, long IdCode_):
_Letter(Base_, Method_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ConstProxyObjectPtr(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_):
_Letter(Base_, Method_, Sponsor_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>* ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::Clone() const
{
  return (new ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
int ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ClassID() const
{
  return TypeInfo::CONSTPROXYOBJECTPTR_TYPE;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator const LETTER& () const
{
  const LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
    
  if (EncPtr_ == NULL)
    THROW (LoneFallibleBase::DerefNullPointerErr());

  return *EncPtr_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER> ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator & () const
{
  return (ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class CLIENT>
ConstTempObject<CLIENT>::ConstTempObject()
{}

/****************************************************************************/
template <class CLIENT>
ConstTempObject<CLIENT>::ConstTempObject(const CLIENT* Base_):
_BossPtr(Base_)
{}

/****************************************************************************/
template <class CLIENT>
ConstTempObject<CLIENT>::ConstTempObject(const ConstCountedBuiltInPtr<CLIENT>& Obj_):
_BossPtr(Obj_)
{}

/****************************************************************************/
template <class CLIENT>
ConstTempObject<CLIENT>::ConstTempObject(const ConstTempObjectClass<CLIENT>& Obj_):
_BossPtr(Obj_.ClientPtr())
{}

/****************************************************************************/
template <class CLIENT>
int ConstTempObject<CLIENT>::ClassID() const
{
  return TypeInfo::CONSTTEMPOBJECT_TYPE;
}

/****************************************************************************/
template <class CLIENT>
ConstTempObject<CLIENT>* ConstTempObject<CLIENT>::Clone() const
{
  return (new ConstTempObject<CLIENT>(*this));
}

/****************************************************************************/
template <class CLIENT>
ConstTempObject<CLIENT>::operator const CLIENT& () const
{
  static CLIENT* Dummy_;

  if (!_BossPtr.IsNull())
    return *_BossPtr;

  THROW (LoneFallibleBase::DerefNullPointerErr());  
  return *Dummy_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT>
void* ConstTempObject<CLIENT>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT>
void ConstTempObject<CLIENT>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT>
void* ConstTempObject<CLIENT>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT>
void ConstTempObject<CLIENT>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if CONSTPTR_DEBUG
#include <stdlib.h>

ConstStaticObjPtr<EnclosedClass> EnclosedClass::_StaObj(EnclosedClass::Make());

inline EnclosedClass* Clone(const EnclosedClass& Obj_)
	{ return Obj_.Clone(); }

inline int* Clone(int Val_)
	{ return (new int(Val_)); }

void main()
{
#if CONSTPTR_DEBUG_COPIEDPTR
  ConstCopiedObjPtr<EnclosedClass> cpobjp;
  ConstCopiedBuiltInPtr<int>* cpbip = new ConstCopiedBuiltInPtr<int>(new int(5));
  ConstCopiedBuiltInPtr<int> cpbip2(new int(7));

  cout <<**cpbip <<" ";
  cout <<*cpbip2 <<endl;
  assert(**cpbip == 5);
  assert(*cpbip2 == 7);

  delete cpbip;
#endif

#if CONSTPTR_DEBUG_COUNTEDPTR
  ConstCountedBuiltInPtr<int>* cnbip = new ConstCountedBuiltInPtr<int>(new int(15));
  cout <<**cnbip <<" ";
  ConstCountedBuiltInPtr<int> cnbip2(*cnbip);
  cout <<*cnbip2 <<endl;
  assert(**cnbip == 15);
  assert(*cnbip2 == 15);

  cout <<cnbip2.Unique() <<" ";
  assert(cnbip2.Unique() == 0);
  delete cnbip;
  cout <<cnbip2.Unique() <<endl;
  assert(cnbip2.Unique() == 1);

/// Testing Counted Object Pointers ///
  EnclosedClass* w1 = new EnclosedClass;
  EnclosedClass* w2 = new EnclosedClass;
  EnclosedClass* w3 = new EnclosedClass;

  ConstCountedObjPtr<EnclosedClass>* cnobjp = new ConstCountedObjPtr<EnclosedClass>(w1);

  delete w2;
  delete w3;

  cout <<(void*)(cnobjp->Pointee()) <<endl;
  delete cnobjp;
#endif
}
#endif
/****************************************************************************/
#endif





