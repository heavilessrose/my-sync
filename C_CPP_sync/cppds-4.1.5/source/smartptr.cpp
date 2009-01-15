#ifndef SMARTPTR_CPP
#define SMARTPTR_CPP
#ifndef SMARTPTR_H
  #include "smartptr.h"
#endif

/****************************************************************************/
template <class T>
CopiedVoidPtr<T>::CopiedVoidPtr(ObjectUpdater* Deleter_):
ConstCopiedVoidPtr<T>(Deleter_)
{}

/****************************************************************************/
template <class T>
CopiedVoidPtr<T>::CopiedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_):
ConstCopiedVoidPtr<T>(just_newed, Deleter_)
{}

/****************************************************************************/
template <class T>
CopiedVoidPtr<T>::CopiedVoidPtr(const T& obj, ObjectUpdater* Deleter_):
ConstCopiedVoidPtr<T>(obj, Deleter_)
{}

/****************************************************************************/
template <class T>
CopiedVoidPtr<T>::CopiedVoidPtr(const ConstCopiedVoidPtr<T>& Obj_):
ConstCopiedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
CopiedVoidPtr<T>* CopiedVoidPtr<T>::Clone() const
{
  return (new CopiedVoidPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int CopiedVoidPtr<T>::ClassID() const
{
  return TypeInfo::COPIEDVOIDPTR_TYPE;
}

/****************************************************************************/
template <class T>
CopiedVoidPtr<T>& CopiedVoidPtr<T>::operator = (const CopiedVoidPtr<T>& Obj_)
{
  ConstCopiedVoidPtr<T>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
template <class T>
CopiedVoidPtr<T>& CopiedVoidPtr<T>::operator = (T* rhs)
{
  ConstCopiedVoidPtr<T>::operator = (rhs);
  return *this;
}

/****************************************************************************/
template <class T>
const T* CopiedVoidPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
T* CopiedVoidPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CopiedVoidPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CopiedVoidPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* CopiedVoidPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void CopiedVoidPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
CopiedBuiltInPtr<T>::CopiedBuiltInPtr():
ConstCopiedBuiltInPtr<T>((T*)NULL)
{}	// Construct as null pointer

/****************************************************************************/
template <class T>
CopiedBuiltInPtr<T>::CopiedBuiltInPtr(T* just_newed):
ConstCopiedBuiltInPtr<T>(just_newed)
{}      // Construct from new pointer

/****************************************************************************/
template <class T>
CopiedBuiltInPtr<T>::CopiedBuiltInPtr(const T& obj):
ConstCopiedBuiltInPtr<T>(obj)
{}

/****************************************************************************/
template <class T>
CopiedBuiltInPtr<T>::CopiedBuiltInPtr(const ConstCopiedBuiltInPtr<T>& Obj_):
ConstCopiedBuiltInPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
CopiedBuiltInPtr<T>* CopiedBuiltInPtr<T>::Clone() const
{
  return (new CopiedBuiltInPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int CopiedBuiltInPtr<T>::ClassID() const
{
  return TypeInfo::COPIEDBUILTINPTR_TYPE;
}

/****************************************************************************/
template <class T>
T* CopiedBuiltInPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* CopiedBuiltInPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
CopiedBuiltInPtr<T>& CopiedBuiltInPtr<T>::operator = (T* rhs)
{
  ConstCopiedVoidPtr<T>::operator = (rhs);
  return *this;
}

/****************************************************************************/
template <class T>
CopiedBuiltInPtr<T>& CopiedBuiltInPtr<T>::operator = (const CopiedBuiltInPtr<T>& rhs)
{
  ConstCopiedVoidPtr<T>::operator = (rhs);
  return *this;
}

/****************************************************************************/
template <class T>
T& CopiedBuiltInPtr<T>::operator * ()
{
  if (_Data.the_p)
    return *_Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.the_p;
}

/****************************************************************************/
template <class T>
const T& CopiedBuiltInPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p;
}

/****************************************************************************/
template <class T>
T* CopiedBuiltInPtr<T>::ReleaseControl()
{
    T* save_p = _Data.the_p;
    _Data.the_p = 0;
    return save_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CopiedBuiltInPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CopiedBuiltInPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* CopiedBuiltInPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void CopiedBuiltInPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
CopiedObjPtr<T>::CopiedObjPtr()
{}

/****************************************************************************/
template <class T>
CopiedObjPtr<T>::CopiedObjPtr(T* just_newed):
ConstCopiedObjPtr<T>(just_newed)
{}

/****************************************************************************/
template <class T>
CopiedObjPtr<T>::CopiedObjPtr(const T& obj):
ConstCopiedObjPtr<T>(obj)
{}

/****************************************************************************/
template <class T>
CopiedObjPtr<T>::CopiedObjPtr(const ConstCopiedObjPtr<T>& p):
ConstCopiedObjPtr<T>(p)
{}

/****************************************************************************/
template <class T>
CopiedObjPtr<T>* CopiedObjPtr<T>::Clone() const
{
  return (new CopiedObjPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int CopiedObjPtr<T>::ClassID() const
{
  return TypeInfo::COPIEDOBJPTR_TYPE;
}

/****************************************************************************/
template <class T>
CopiedObjPtr<T>& CopiedObjPtr<T>::operator = (T* rhs)
{
  ConstCopiedVoidPtr<T>::operator = (rhs);
  return *this;
}

/****************************************************************************/
template <class T>
CopiedObjPtr<T>& CopiedObjPtr<T>::operator = (const CopiedObjPtr<T>& rhs)
{
  ConstCopiedVoidPtr<T>::operator = (rhs);
  return *this;
}

/****************************************************************************/
template <class T>
T* CopiedObjPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* CopiedObjPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
T& CopiedObjPtr<T>::operator * ()
{
  if (_Data.the_p)
    return *_Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.the_p;
}

/****************************************************************************/
template <class T>
const T& CopiedObjPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p;
}

/****************************************************************************/
template <class T>
T* CopiedObjPtr<T>::operator-> ()
{
  if (_Data.the_p)
    return _Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* CopiedObjPtr<T>::operator-> () const
{
  if (_Data.const_p)
    return _Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
T* CopiedObjPtr<T>::ReleaseControl()
{
    T* save_p = _Data.the_p;
    _Data.the_p = 0;
    return save_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CopiedObjPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CopiedObjPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* CopiedObjPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void CopiedObjPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
CountedVoidPtr<T>::CountedVoidPtr(ObjectUpdater* Deleter_):
ConstCountedVoidPtr<T>(Deleter_)
{}

/****************************************************************************/
template <class T>
CountedVoidPtr<T>::CountedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_, Boolean OwnsPtr_):
ConstCountedVoidPtr<T>(just_newed, Deleter_, OwnsPtr_)
{}

/****************************************************************************/
template <class T>
CountedVoidPtr<T>::CountedVoidPtr(const CountedVoidPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
CountedVoidPtr<T>::CountedVoidPtr(const VoidPtrClass<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_.GiveBase())
{}

/****************************************************************************/
template <class T>
CountedVoidPtr<T>::CountedVoidPtr(const ConstCountedVoidPtr<T>& Obj_):
ConstCountedVoidPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
CountedVoidPtr<T>* CountedVoidPtr<T>::Clone() const
{
  return (new CountedVoidPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int CountedVoidPtr<T>::ClassID() const
{
  return TypeInfo::COUNTEDVOIDPTR_TYPE;
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>& CountedVoidPtr<T>::GiveBase()
{
  return *this;
}

/****************************************************************************/
template <class T>
const ConstCountedVoidPtr<T>& CountedVoidPtr<T>::GiveBase() const
{
  return *this;
}

/****************************************************************************/
template <class T>
CountedVoidPtr<T> CountedVoidPtr<T>::CastAwayConst(const ConstCountedVoidPtr<T>& Obj_)
{
  return (CountedVoidPtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
CountedVoidPtr<T>& CountedVoidPtr<T>::operator = (VoidPtrClass<T>& Obj_)
{
  ConstCountedVoidPtr<T>::operator = (Obj_.GiveBase());
  return *this;
}

/****************************************************************************/
template <class T>
CountedVoidPtr<T>& CountedVoidPtr<T>::operator = (T* rhs)
{
  ConstCountedVoidPtr<T>::operator = (rhs);
  return *this;
}

/****************************************************************************/
template <class T>
const T* CountedVoidPtr<T>::ConstPointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
const T* CountedVoidPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
T* CountedVoidPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
template<class T>
T* CountedVoidPtr<T>::ReleaseControl()
{
    T* save_p = _Data.the_p;
    SetToNull();
    return save_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CountedVoidPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CountedVoidPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* CountedVoidPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void CountedVoidPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Bytes_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
// Construct as null pointer
template <class T>
CountedBuiltInPtr<T>::CountedBuiltInPtr():
ConstCountedBuiltInPtr<T>()
{}

/****************************************************************************/
// Construct pointing at heap object
template <class T>
CountedBuiltInPtr<T>::CountedBuiltInPtr(T* just_newed, Boolean OwnsPtr_):
ConstCountedBuiltInPtr<T>(just_newed, OwnsPtr_)
{}

/****************************************************************************/
template <class T>
CountedBuiltInPtr<T>::CountedBuiltInPtr(const CountedBuiltInPtr<T>& Obj_):
ConstCountedBuiltInPtr<T>(Obj_)
{}

/****************************************************************************/
// Construct as null pointer
template <class T>
CountedBuiltInPtr<T>::CountedBuiltInPtr(const ConstCountedBuiltInPtr<T>& Obj_):
ConstCountedBuiltInPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
CountedBuiltInPtr<T>* CountedBuiltInPtr<T>::Clone() const
{
  return (new CountedBuiltInPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int CountedBuiltInPtr<T>::ClassID() const
{
  return TypeInfo::COUNTEDBUILTINPTR_TYPE;
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>& CountedBuiltInPtr<T>::GiveBase()
{
  return *this;
}

/****************************************************************************/
template <class T>
const ConstCountedVoidPtr<T>& CountedBuiltInPtr<T>::GiveBase() const
{
  return *this;
}

/****************************************************************************/
template <class T>
CountedBuiltInPtr<T> CountedBuiltInPtr<T>::CastAwayConst(const ConstCountedBuiltInPtr<T>& Obj_)
{
  return (CountedBuiltInPtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
CountedBuiltInPtr<T>& CountedBuiltInPtr<T>::operator = (CountedBuiltInPtr<T>& Obj_)
{
  ConstCountedVoidPtr<T>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
template <class T>
CountedBuiltInPtr<T>& CountedBuiltInPtr<T>::operator = (T* just_newed)
{
  ConstCountedVoidPtr<T>::operator = (just_newed);
  return *this;
}

/****************************************************************************/
template <class T>
T& CountedBuiltInPtr<T>::operator * ()
{
  if (_Data.the_p)
    return *_Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.the_p;  // Should never reach this line
}

/****************************************************************************/
template <class T>
const T& CountedBuiltInPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p;  // Should never reach this line
}

/****************************************************************************/
template <class T>
const T* CountedBuiltInPtr<T>::ConstPointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
const T* CountedBuiltInPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
T* CountedBuiltInPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
template<class T>
T* CountedBuiltInPtr<T>::ReleaseControl()
{
    T* save_p = _Data.the_p;
    SetToNull();
    return save_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CountedBuiltInPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CountedBuiltInPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* CountedBuiltInPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void CountedBuiltInPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template<class T>
CountedObjPtr<T>::CountedObjPtr()
{}

/****************************************************************************/
template<class T>
CountedObjPtr<T>::CountedObjPtr(T* just_newed, Boolean OwnsPtr_):
ConstCountedObjPtr<T>(just_newed, OwnsPtr_)
{}

/****************************************************************************/
template<class T>
CountedObjPtr<T>::CountedObjPtr(const CountedObjPtr<T>& Obj_):
ConstCountedObjPtr<T>(Obj_)
{}

/****************************************************************************/
template<class T>
CountedObjPtr<T>::CountedObjPtr(const ConstCountedObjPtr<T>& Obj_):
ConstCountedObjPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
CountedObjPtr<T>* CountedObjPtr<T>::Clone() const
{
  return (new CountedObjPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int CountedObjPtr<T>::ClassID() const
{
  return TypeInfo::COUNTEDOBJPTR_TYPE;
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>& CountedObjPtr<T>::GiveBase()
{
  return *this;
}

/****************************************************************************/
template <class T>
const ConstCountedVoidPtr<T>& CountedObjPtr<T>::GiveBase() const
{
  return *this;
}

/****************************************************************************/
template<class T>
CountedObjPtr<T> CountedObjPtr<T>::CastAwayConst(const ConstCountedObjPtr<T>& Obj_)
{
  return (CountedObjPtr<T>(Obj_));
}

/****************************************************************************/
template<class T>
CountedObjPtr<T>& CountedObjPtr<T>::operator = (T* just_newed)
{
  ConstCountedVoidPtr<T>::operator = (just_newed);
  return *this;
}

/****************************************************************************/
template<class T>
CountedObjPtr<T>& CountedObjPtr<T>::operator = (CountedObjPtr<T>& rhs)
{
  ConstCountedVoidPtr<T>::operator = (rhs);
  return *this;
}

/****************************************************************************/
template <class T>
const T* CountedObjPtr<T>::ConstPointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
const T* CountedObjPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
T* CountedObjPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
T& CountedObjPtr<T>::operator * ()
{
  if (_Data.the_p)
    return *_Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.the_p;
}

/****************************************************************************/
template <class T>
const T& CountedObjPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p;
}

/****************************************************************************/
template <class T>
T* CountedObjPtr<T>::operator-> ()
{
  if (_Data.the_p)
    return _Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* CountedObjPtr<T>::operator-> () const
{
  if (_Data.const_p)
    return _Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.const_p;
}

/****************************************************************************/
template<class T>
T* CountedObjPtr<T>::ReleaseControl()
{
    T* save_p = _Data.the_p;
    SetToNull();
    return save_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CountedObjPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CountedObjPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* CountedObjPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void CountedObjPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template<class T>
UpdatePtr<T>::UpdatePtr(ObjectUpdater* Deleter_,
                        ObjectUpdater* Updater_,
                        ObjectUpdater* Validator_):
ConstUpdatePtr<T>(Deleter_, Updater_, Validator_)
{}

/****************************************************************************/
template<class T>
UpdatePtr<T>::UpdatePtr(const VoidPtrClass<T>& Obj_,
                        ObjectUpdater* Updater_,
                        ObjectUpdater* Validator_):
ConstUpdatePtr<T>(Obj_.GiveBase(), Updater_, Validator_)
{}

/****************************************************************************/
template<class T>
UpdatePtr<T>::UpdatePtr(const ConstUpdatePtr<T>& Obj_):
ConstUpdatePtr<T>(Obj_)
{}

/****************************************************************************/
template<class T>
UpdatePtr<T>::UpdatePtr(const UpdatePtr<T>& Obj_):
ConstUpdatePtr<T>(Obj_)
{}

/****************************************************************************/
template<class T>
UpdatePtr<T>::~UpdatePtr()
{
  Update();
}

/****************************************************************************/
template <class T>
UpdatePtr<T>* UpdatePtr<T>::Clone() const
{
  return (new UpdatePtr<T>(*this));
}

/****************************************************************************/
template <class T>
int UpdatePtr<T>::ClassID() const
{
  return TypeInfo::UPDATEPTR_TYPE;
}

/****************************************************************************/
template <class T>
const ConstCountedVoidPtr<T>& UpdatePtr<T>::GiveBase() const
{
  return *this;
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>& UpdatePtr<T>::GiveBase()
{
  return *this;
}

/****************************************************************************/
template <class T>
UpdatePtr<T> UpdatePtr<T>::CastAwayConst(const ConstUpdatePtr<T>& Obj_)
{
  return (UpdatePtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
UpdatePtr<T>& UpdatePtr<T>::CopyBindings(UpdatePtr<T>& Obj_)
{
  if (this != &Obj_)
  {
    if (!IsNull(_Updater) && !IsNull(_Updater))
      SetUpdater(_Updater->TransferBasePtr(Obj_._Updater->Clone()));
    else    
      _Updater = Obj_._Updater;

    if (!IsNull(Obj_._Validator) && !IsNull(_Validator))
      SetValidator(_Validator->TransferBasePtr(Obj_._Validator->Clone()));
    else    
      _Validator = Obj_._Validator;      
  }

  return *this;
}

/****************************************************************************/
template <class T>
UpdatePtr<T>& UpdatePtr<T>::operator = (UpdatePtr<T>& Obj_)
{
  if (this != &Obj_)
  {
    ConstCountedVoidPtr<T>::operator = (Obj_);
    Update();
  }

  return *this;
}

/****************************************************************************/
template <class T>
UpdatePtr<T>& UpdatePtr<T>::operator = (T* just_newed)
{
  if (_Data.const_p != just_newed)
  {
    ConstCountedVoidPtr<T>::operator = (just_newed);
    Update();
  }

  return *this;
}

/****************************************************************************/
template <class T>
UpdatePtr<T>& UpdatePtr<T>::SetUpdatablePtr(VoidPtrClass<T>& Obj_,
                                            ObjectUpdater* Updater_,
                                            ObjectUpdater* Validator_)
{
  SPMARKER("start: UpdatePtr<T>::SetUpdatablePtr(VoidPtrClass<T>&, ObjectUpdater*)")
  
  if (&GiveBase() != &Obj_.GiveBase())
  {
    ConstCountedVoidPtr<T>::operator = (Obj_.GiveBase());
    Update();
    
    _Updater = UpdaterBindingList::Make(Updater_)->HaltOnFirstFalse(FALSE);
    _Validator = UpdaterBindingList::Make(Validator_)->HaltOnFirstFalse(TRUE);
  }

  SPMARKER("end: UpdatePtr<T>::SetUpdatablePtr(VoidPtrClass<T>&, ObjectUpdater*)")
  return *this;
}

/****************************************************************************/
template <class T>
UpdatePtr<T>& UpdatePtr<T>::SetUpdater(ObjectUpdater* Updater_)
{
  _Updater = UpdaterBindingList::Make(Updater_)->HaltOnFirstFalse(FALSE);
  return *this;
}

/****************************************************************************/
template <class T>
UpdatePtr<T>& UpdatePtr<T>::SetValidator(ObjectUpdater* Validator_)
{
  _Validator = UpdaterBindingList::Make(Validator_)->HaltOnFirstFalse(TRUE);
  return *this;
}

/****************************************************************************/
template <class T>
T* UpdatePtr<T>::ReleaseControl()
{
  T* save_p = _Data.the_p;
  SetToNull();
  return save_p;
}

/****************************************************************************/
template <class T>
T* UpdatePtr<T>::Pointee()
{
  _Data._Modified = TRUE;
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* UpdatePtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
const T* UpdatePtr<T>::ConstPointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* UpdatePtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template<class T>
void UpdatePtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template<class T>
void* UpdatePtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template<class T>
void UpdatePtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
ArrayPtr<T>::ArrayPtr(const ConstArrayPtr<T>& Obj_):
ConstArrayPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ArrayPtr<T>::ArrayPtr(ObjectUpdater* Deleter_):
ConstArrayPtr<T>(Deleter_)
{}

/****************************************************************************/
template <class T>
ArrayPtr<T>::ArrayPtr(const ArrayPtr<T>& Obj_):
ConstArrayPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
ArrayPtr<T>::ArrayPtr(const VoidPtrClass<T>& Obj_, size_t Size_, T* Base_):
ConstArrayPtr<T>(Obj_.GiveBase(), Size_, Base_)
{}

/****************************************************************************/
template <class T>
ArrayPtr<T>* ArrayPtr<T>::Clone() const
{
  return (new ArrayPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int ArrayPtr<T>::ClassID() const
{
  return TypeInfo::ARRAYPTR_TYPE;
}

/****************************************************************************/
template <class T>
ArrayPtr<T> ArrayPtr<T>::CastAwayConst(const ConstArrayPtr<T>& Obj_)
{
  return (ArrayPtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
ArrayPtr<T>& ArrayPtr<T>::operator = (ArrayPtr<T>& rhs)
{
  if (this != &rhs)
  {
    ConstCountedVoidPtr<T>::operator = (rhs);
    _Size = rhs._Size;
    base_p = rhs.base_p;
  }

  return *this;
}

/****************************************************************************/
template <class T>
ArrayPtr<T>& ArrayPtr<T>::operator = (T* just_newed)
{
  if (_Data.const_p != just_newed)
    ConstCountedVoidPtr<T>::operator = (just_newed);

  return *this;
}

/****************************************************************************/
template <class T>
const ConstCountedVoidPtr<T>& ArrayPtr<T>::GiveBase() const
{
  return *this;
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>& ArrayPtr<T>::GiveBase()
{
  return *this;
}

/****************************************************************************/
template <class T>
const T* ArrayPtr<T>::ConstPointee() const
{
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* ArrayPtr<T>::Pointee() const
{
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
T* ArrayPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
T* ArrayPtr<T>::ReleaseControl()
{
  T* save_p = _Data.the_p;
  SetToNull();
  return save_p;
}

/****************************************************************************/
template <class T>
ArrayPtr<T> ArrayPtr<T>::operator + (ptrdiff_t Index_)
{
  ArrayPtr<T> Temp_(*this);
  Temp_ += Index_;
  return Temp_;
}

/****************************************************************************/
template <class T>
ArrayPtr<T> ArrayPtr<T>::operator - (ptrdiff_t Index_)
{
  ArrayPtr<T> Temp_(*this);
  Temp_ -= Index_;
  return Temp_;
}

/****************************************************************************/
template <class T>
T& ArrayPtr<T>::operator * ()
{
  if (_Data.the_p)
    return *_Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.the_p; // Should never reach this line
}

/****************************************************************************/
template <class T>
T& ArrayPtr<T>::operator [] (ptrdiff_t Index_)
{
  if (_Data.the_p)
    if (Pointee() + Index_ < base_p || Pointee() + Index_ >= base_p + _Size)
      THROW (LoneFallibleBase::ArrayIndexRangeErr());
    else
      return *(Pointee() + Index_);

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.the_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
const T& ArrayPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
const T& ArrayPtr<T>::operator [] (ptrdiff_t Index_) const
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
template <class T>
ArrayPtr<T>& ArrayPtr<T>::operator ++ ()
{
  (*this += 1);
  return *this;
}

/****************************************************************************/
template <class T>
ArrayPtr<T> ArrayPtr<T>::operator ++ (int)
{
  ArrayPtr<T> Temp_(*this);
  (*this += 1);
  return Temp_;
}

/****************************************************************************/
template <class T>
ArrayPtr<T>& ArrayPtr<T>::operator -- ()
{
  (*this -= 1);
  return *this;
}

/****************************************************************************/
template <class T>
ArrayPtr<T> ArrayPtr<T>::operator -- (int)
{
  ArrayPtr<T> Temp_(*this);
  (*this -= 1);
  return Temp_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ArrayPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ArrayPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ArrayPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ArrayPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
StaticObjPtr<T>::StaticObjPtr():
ConstStaticObjPtr<T>()
{}

/****************************************************************************/
template<class T>
StaticObjPtr<T>::StaticObjPtr(T* just_newed):
ConstStaticObjPtr<T>(just_newed)
{}

/****************************************************************************/
template<class T>
StaticObjPtr<T>::StaticObjPtr(const ConstStaticObjPtr<T>& Obj_):
ConstStaticObjPtr<T>(Obj_)
{}

/****************************************************************************/
template <class T>
StaticObjPtr<T>* StaticObjPtr<T>::Clone() const
{
  return (new StaticObjPtr<T>(*this));
}

/****************************************************************************/
template <class T>
int StaticObjPtr<T>::ClassID() const
{
  return TypeInfo::STATICOBJPTR_TYPE;
}

/****************************************************************************/
template <class T>
const ConstCountedVoidPtr<T>& StaticObjPtr<T>::GiveBase() const
{
  return *this;
}

/****************************************************************************/
template <class T>
ConstCountedVoidPtr<T>& StaticObjPtr<T>::GiveBase()
{
  return *this;
}

/****************************************************************************/
template <class T>
StaticObjPtr<T> StaticObjPtr<T>::CastAwayConst(const ConstStaticObjPtr<T>& Obj_)
{
  return (StaticObjPtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
StaticObjPtr<T>& StaticObjPtr<T>::operator = (T* just_newed)
{
  ConstCountedVoidPtr<T>::operator = (just_newed);
  return *this;
}

/****************************************************************************/
template <class T>
StaticObjPtr<T>& StaticObjPtr<T>::operator = (StaticObjPtr<T>& Obj_)
{
  ConstCountedVoidPtr<T>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
template<class T>
const T& StaticObjPtr<T>::operator * () const
{
  if (_Data.const_p)
    return *_Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.const_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
T& StaticObjPtr<T>::operator * ()
{
  if (_Data.the_p)
    return *_Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_Data.the_p; // Should never reach this line
}

/****************************************************************************/
template<class T>
const T* StaticObjPtr<T>::operator-> () const
{
  if (_Data.const_p)
    return _Data.const_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.const_p;
}

/****************************************************************************/
template<class T>
T* StaticObjPtr<T>::operator-> ()
{
  if (_Data.the_p)
    return _Data.the_p;

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return _Data.the_p;
}

/****************************************************************************/
template <class T>
const T* StaticObjPtr<T>::ConstPointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
const T* StaticObjPtr<T>::Pointee() const
{
  return _Data.const_p;
}

/****************************************************************************/
template <class T>
T* StaticObjPtr<T>::Pointee()
{
  return _Data.the_p;
}

/****************************************************************************/
template<class T>
StaticObjPtr<T>& StaticObjPtr<T>::Increment()
{
  _Data.refCount.Increment();
  return *this;
}

/****************************************************************************/
template<class T>
StaticObjPtr<T>& StaticObjPtr<T>::Decrement()
{
  _Data.refCount.Decrement();
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* StaticObjPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void StaticObjPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* StaticObjPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void StaticObjPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtr<CLIENT, SPONSOR, LETTER>* ProxyPtr<CLIENT, SPONSOR, LETTER>::Dispatch()
{
  ::NotImplemented("ConstProxyPtr", "Object* Dispatch()");
  exit(-1);
  return this;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* ProxyPtr<CLIENT, SPONSOR, LETTER>::Dispatch() const
{
  ::NotImplemented("ConstProxyPtr", "Object* Dispatch()");
  exit(-1);
  return this;
}

/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp():
_BossPtr(NULL, FALSE),
_Bindings(),
_Type(BindingType::NOBINDINGS),
_NotificationCode(0),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(const ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_BossPtr(((ProxyPtr<CLIENT, SPONSOR, LETTER>*)&Obj_)->ClientPtr()),
_Bindings(Obj_.Bindings(), Obj_.BindingType()),
_Type(Obj_.BindingType()),
_NotificationCode(Obj_.NotificationCode()),
_Sponsor(Obj_.Sponsor())
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(const ProxyPtrImp<CLIENT, SPONSOR, LETTER>& Obj_):
_BossPtr(Obj_._BossPtr),
_Bindings(Obj_._Bindings, Obj_._Type),
_Type(Obj_._Type),
_NotificationCode(Obj_._NotificationCode),
_Sponsor(Obj_._Sponsor)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Method_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::BINDING0P),
_NotificationCode(0),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Method_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::BINDING1P),
_NotificationCode(IdCode_),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::BINDING2P),
_NotificationCode(IdCode_),
_Sponsor(Sponsor_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Method_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::BINDING0R),
_NotificationCode(0),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Method_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::BINDING1R),
_NotificationCode(IdCode_),
_Sponsor(NULL)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_):
_BossPtr(Base_, FALSE),
_Bindings(Method_),
_Type(BindingType::BINDING2R),
_NotificationCode(IdCode_),
_Sponsor(Sponsor_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtrImp<CLIENT, SPONSOR, LETTER>* ProxyPtrImp<CLIENT, SPONSOR, LETTER>::Clone() const
{
  return (new ProxyPtrImp<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
int ProxyPtrImp<CLIENT, SPONSOR, LETTER>::ClassID() const
{
  return TypeInfo::PROXYPTRIMP_TYPE;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
Boolean ProxyPtrImp<CLIENT, SPONSOR, LETTER>::IsPtrLetter() const
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
const LETTER* ProxyPtrImp<CLIENT, SPONSOR, LETTER>::GetLetterPointer() const
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
             ((ProxyPtrImp<CLIENT, SPONSOR, LETTER> *const)this)->GetLetterPointer()
    );

  return ((LETTER*)NULL);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
const LETTER& ProxyPtrImp<CLIENT, SPONSOR, LETTER>::GetLetterObject() const
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
             ((ProxyPtrImp<CLIENT, SPONSOR, LETTER> *const)this)->GetLetterObject()
    );

  THROW (LoneFallibleBase::DerefNullPointerErr());  
  return *Dummy_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
LETTER* ProxyPtrImp<CLIENT, SPONSOR, LETTER>::GetLetterPointer()
{
  long Code_ = _NotificationCode;
  if (!Code_ && _Sponsor)
    Code_ = CountOf(*_Sponsor);
  
  if (!_BossPtr.IsNull() && _Type != BindingType::NOBINDINGS)
    return
    (
      (_Type == BindingType::BINDING0P) ?
             (_BossPtr.Pointee()->*_Bindings._Bind0.PtrTo_Ptr)():
      (_Type == BindingType::BINDING1P) ?
             (_BossPtr.Pointee()->*_Bindings._Bind1.PtrTo_Ptr)(Code_):
      (_Type == BindingType::BINDING2P && _Sponsor) ?
             (_BossPtr.Pointee()->*_Bindings._Bind2.PtrTo_Ptr)(*_Sponsor, Code_):
             ((LETTER*)NULL)
    );

  return ((LETTER*)NULL);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
LETTER& ProxyPtrImp<CLIENT, SPONSOR, LETTER>::GetLetterObject()
{
  static LETTER* Dummy_;
  
  long Code_ = _NotificationCode;
  if (!Code_ && _Sponsor)
    Code_ = CountOf(*_Sponsor);

  if (!_BossPtr.IsNull() && _Type != BindingType::NOBINDINGS &&
      (_Type != BindingType::BINDING2 || _Sponsor))
    return
    (
      (_Type == BindingType::BINDING0R) ?
             (_BossPtr.Pointee()->*_Bindings._Bind0.RefTo_Ptr)():
      (_Type == BindingType::BINDING1R) ?
             (_BossPtr.Pointee()->*_Bindings._Bind1.RefTo_Ptr)(Code_):
      (_Type == BindingType::BINDING2R && _Sponsor) ?
             (_BossPtr.Pointee()->*_Bindings._Bind2.RefTo_Ptr)(*_Sponsor, Code_):
             *Dummy_
    );

  THROW (LoneFallibleBase::DerefNullPointerErr());  
  return *Dummy_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPtr<CLIENT, SPONSOR, LETTER>& ProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator = (ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
{
  _BossPtr = Obj_.ClientPtr();
  _Bindings = Obj_.Bindings();
  _Type = Obj_.BindingType();
  _NotificationCode = Obj_.NotificationCode();
  _Sponsor = Obj_.Sponsor();
  return *this;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>    
ProxyPtr<CLIENT, SPONSOR, LETTER>& ProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator = (CountedObjPtr<CLIENT>& Obj_)
{
  _BossPtr = Obj_;
  return *this;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>    
ProxyPtr<CLIENT, SPONSOR, LETTER>& ProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator = (CLIENT* Ptr_)
{
  _BossPtr = Ptr_;
  return *this;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::SetBinding(TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Method_)
{
  _Bindings = Method_;
  _Type = BindingType::BINDING0P;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>    
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::SetBinding(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Method_, long IdCode_)
{
  _Bindings = Method_;
  _Type = BindingType::BINDING1P;
  _NotificationCode = IdCode_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>    
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::SetBinding(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_)
{
  _Bindings = Method_;
  _Type = BindingType::BINDING2P;
  _NotificationCode = IdCode_;
  _Sponsor = Sponsor_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::SetBinding(TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Method_)
{
  _Bindings = Method_;
  _Type = BindingType::BINDING0R;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>    
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::SetBinding(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Method_, long IdCode_)
{
  _Bindings = Method_;
  _Type = BindingType::BINDING1R;
  _NotificationCode = IdCode_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>    
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::SetBinding(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_)
{
  _Bindings = Method_;
  _Type = BindingType::BINDING2R;
  _NotificationCode = IdCode_;
  _Sponsor = Sponsor_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyPtrImp<CLIENT, SPONSOR, LETTER>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ProxyPointerPtr():
_Letter(TRUE)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ProxyPointerPtr(const ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ProxyPointerPtr(const ProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ProxyPointerPtr(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Method_):
_Letter(Base_, Method_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ProxyPointerPtr(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Method_, long IdCode_):
_Letter(Base_, Method_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ProxyPointerPtr(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_):
_Letter(Base_, Method_, Sponsor_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>* ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::Clone() const
{
  return (new ProxyPointerPtr<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
int ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::ClassID() const
{
  return TypeInfo::PROXYPOINTERPTR_TYPE;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator const LETTER* () const
{
  const LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
  return EncPtr_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
const LETTER& ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator * () const
{
  const LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
    
  if (EncPtr_ == NULL)
    THROW (LoneFallibleBase::DerefNullPointerErr());

  return *EncPtr_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator LETTER* ()
{
  LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
  return EncPtr_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
LETTER& ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator * ()
{
  LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
    
  if (EncPtr_ == NULL)
    THROW (LoneFallibleBase::DerefNullPointerErr());

  return *EncPtr_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyPointerPtr<CLIENT, SPONSOR, LETTER>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ProxyObjectPtr():
_Letter(FALSE)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ProxyObjectPtr(const ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ProxyObjectPtr(const ProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_):
_Letter(Obj_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ProxyObjectPtr(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Method_):
_Letter(Base_, Method_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ProxyObjectPtr(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Method_, long IdCode_):
_Letter(Base_, Method_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ProxyObjectPtr(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_):
_Letter(Base_, Method_, Sponsor_, IdCode_)
{}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>* ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::Clone() const
{
  return (new ProxyObjectPtr<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
int ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::ClassID() const
{
  return TypeInfo::PROXYOBJECTPTR_TYPE;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator const LETTER& () const
{
  const LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
    
  if (EncPtr_ == NULL)
    THROW (LoneFallibleBase::DerefNullPointerErr());

  return *EncPtr_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator LETTER& ()
{
  LETTER* EncPtr_ = IsPtrLetter() ? GetLetterPointer():&GetLetterObject();
    
  if (EncPtr_ == NULL)
    THROW (LoneFallibleBase::DerefNullPointerErr());

  return *EncPtr_;
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
ProxyPointerPtr<CLIENT, SPONSOR, LETTER> ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator & () const
{
  return (ProxyPointerPtr<CLIENT, SPONSOR, LETTER>(*this));
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void* ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class SPONSOR, class LETTER>
void ProxyObjectPtr<CLIENT, SPONSOR, LETTER>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class CLIENT>
TempObject<CLIENT>::TempObject()
{}

/****************************************************************************/
template <class CLIENT>
TempObject<CLIENT>::TempObject(CLIENT* Base_):
_BossPtr(Base_)
{}

/****************************************************************************/
template <class CLIENT>
TempObject<CLIENT>::TempObject(const CountedBuiltInPtr<CLIENT>& Obj_):
_BossPtr(Obj_)
{}

/****************************************************************************/
template <class CLIENT>
TempObject<CLIENT>::TempObject(const TempObjectClass<CLIENT>& Obj_):
_BossPtr(Obj_.ClientPtr())
{}

/****************************************************************************/
template <class CLIENT>
int TempObject<CLIENT>::ClassID() const
{
  return TypeInfo::TEMPOBJECT_TYPE;
}

/****************************************************************************/
template <class CLIENT>
TempObject<CLIENT>* TempObject<CLIENT>::Clone() const
{
  return (new TempObject<CLIENT>(*this));
}

/****************************************************************************/
template <class CLIENT>
TempObject<CLIENT>::operator const CLIENT& () const
{
  static CLIENT* Dummy_;

  if (!_BossPtr.IsNull())
    return *_BossPtr;

  THROW (LoneFallibleBase::DerefNullPointerErr());  
  return *Dummy_;
}

/****************************************************************************/
template <class CLIENT>
TempObject<CLIENT>::operator CLIENT& ()
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
void* TempObject<CLIENT>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT>
void TempObject<CLIENT>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT>
void* TempObject<CLIENT>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT>
void TempObject<CLIENT>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if SMARTPTR_DEBUG
#include <stdlib.h>
StaticObjPtr<EnclosedClass> EnclosedClass::_StaObj;

Boolean MasterClass::Update(const EnclosedClass& w, long IdCode_)
	{ EnclosedClass& nw = (EnclosedClass&)w;
          dupv = nw.showX();
	  if (&what != &nw)
	    what = nw;            
	  return TRUE; }

const Boolean& MasterClass::ConstUpdateReturnRef(const EnclosedClass& w, long IdCode_) const
	{ static Boolean Dummy_ = TRUE;
          MasterClass *const LocalThis_ = (MasterClass *const)this;
          EnclosedClass& nw = (EnclosedClass&)w;
          LocalThis_->dupv = nw.showX();
	  if (&what != &nw)
	    LocalThis_->what = nw;
	  return Dummy_; }
const Boolean* MasterClass::ConstUpdateReturnPtr(const EnclosedClass& w, long IdCode_) const
	{ static Boolean Dummy_ = TRUE;
          ConstUpdateReturnRef(w, IdCode_);
	  return &Dummy_; }

Boolean& MasterClass::UpdateReturnRef(const EnclosedClass& w, long IdCode_)
	{ static Boolean Dummy_ = TRUE;
          MasterClass *const LocalThis_ = (MasterClass *const)this;
          EnclosedClass& nw = (EnclosedClass&)w;
          LocalThis_->dupv = nw.showX();
	  if (&what != &nw)
	    LocalThis_->what = nw;
	  return Dummy_; }
Boolean* MasterClass::UpdateReturnPtr(const EnclosedClass& w, long IdCode_)
	{ static Boolean Dummy_ = TRUE;
          UpdateReturnRef(w, IdCode_);
	  return &Dummy_; }                    

inline EnclosedClass* Clone(const EnclosedClass& Obj_)
	{ return Obj_.Clone(); }
inline int* Clone(int Val_)
	{ return (new int(Val_)); }

int main()
{
  int val;
  EnclosedClass cpobjp_data(1);
  cpobjp_data.SetOwner("cpobjp");
  CopiedObjPtr<EnclosedClass> cpobjp(cpobjp_data);
  CopiedBuiltInPtr<int>* cpbip = new CopiedBuiltInPtr<int>(new int(5));
  CopiedBuiltInPtr<int> cpbip2(new int(7));

  val = **cpbip;
  assert(val == 5);
  val = *cpbip2;
  assert(val == 7);
  *cpbip = cpbip2;
  val = **cpbip;
  assert(val == 7);
  *cpbip = 9;
  val = **cpbip;
  assert(val == 9);
  delete cpbip;

  int* dap = cpbip2.ReleaseControl();
  assert(*dap == 7);

  CountedVoidPtr<void> vptr;
  EnclosedClass* cnobjp_data = new EnclosedClass(2);
  cnobjp_data->SetOwner("cnobjp");
  CountedObjPtr<EnclosedClass>* cnobjp = new CountedObjPtr<EnclosedClass>(cnobjp_data);
  delete cnobjp;

  CountedBuiltInPtr<int>* cnbip = new CountedBuiltInPtr<int>(new int(15));
  val = **cnbip;
  assert(val == 15);
  CountedBuiltInPtr<int>* cnbip2 = cnbip->Clone();
  val = **cnbip2;
  assert(val == 15);

  int i11 = 11;
  *cnbip2 = &i11;
  val = **cnbip2;
  assert(val == 11);

  *cnbip2 = *cnbip;
  val = **cnbip2;
  assert(val == 15);
  delete cnbip;

  assert(cnbip2->Unique());

  EnclosedClass clon2_data(3);
  clon2_data.SetOwner("clon2");
  CopiedObjPtr<EnclosedClass> clon2(clon2_data);
  CountedBuiltInPtr<int> cnbip3(new int[10]);
  ArrayPtr<int> cap1(cnbip3, 10);

  int i;
  for (i = 0; i < 10; ++i)
    cap1[i] = (i + 1);

  for (i = 0; i < 9; ++i)
  {
    val = *cap1++;
    assert(val == i + 1);
  }
  assert(*cap1 == 10);

  EnclosedClass* w1 = new EnclosedClass();
  EnclosedClass* w2 = new EnclosedClass();
  EnclosedClass* w3 = new EnclosedClass();

  delete w1;
  delete w2;
  delete w3;
  delete cnbip2;

/// Testing object updater classes
  MasterClass mc2;
  MasterClass mc;
  assert(mc.ShowX() == 10);
  assert(mc.ShowDupv() == 10);
  mc.ReturnAndUpdate().Pointee()->SetX(mc2.ReturnAndUpdate().Pointee()->showX() * 100 + 99);
  assert(mc.ShowX() == 1099);
  assert(mc.ShowDupv() == 1099);

  UpdatePtr<EnclosedClass> _Uptr2(mc.ReturnAndUpdate());
  mc.MakeAnother();
  assert(mc.ShowX() == 1099);
  assert(mc.ShowDupv() == 1099);
  assert(mc._Next->ShowX() == 10);
  assert(mc._Next->ShowDupv() == 10);
  mc.ReturnAndUpdate().Pointee()->SetX(mc.ReturnAndUpdate().Pointee()->showX() * 100 + 22);
  assert(mc.ShowX() == 109922);
  assert(mc.ShowDupv() == 109922);
  assert(mc._Next->ShowX() == 109922);
  assert(mc._Next->ShowDupv() == 109922);

/// Testing Const Proxy classes
  EnclosedClass Wsponsor_;
  Wsponsor_.SetX(780021);
  ConstProxyObjectPtr<MasterClass, EnclosedClass, Boolean> ConstProxObj_(&mc2, &MasterClass::ConstUpdateReturnRef, &Wsponsor_);
  if (ConstProxObj_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);

  ConstProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ConstProxPtr1_ = &ConstProxObj_;
  if (*ConstProxPtr1_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);    

  Wsponsor_.SetX(Wsponsor_.showX() + 1100);
  ConstProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ConstProxPtr2_(&mc2, &MasterClass::ConstUpdateReturnPtr, &Wsponsor_);
  if (*ConstProxPtr2_)
  {
    assert(mc2.ShowX() == 781121);
    assert(mc2.ShowDupv() == 781121);
  }
  else
    assert(0);

/// Testing Non-Const Proxy classes
  Wsponsor_.SetX(780021);
  ProxyObjectPtr<MasterClass, EnclosedClass, Boolean> ProxObj_(&mc2, &MasterClass::UpdateReturnRef, &Wsponsor_);
  if (ProxObj_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);

  ProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ProxPtr1_ = &ProxObj_;
  if (*ProxPtr1_)
  {
    assert(mc2.ShowX() == 780021);
    assert(mc2.ShowDupv() == 780021);
  }
  else
    assert(0);    

  Wsponsor_.SetX(Wsponsor_.showX() + 1100);
  ProxyPointerPtr<MasterClass, EnclosedClass, Boolean> ProxPtr2_(&mc2, &MasterClass::UpdateReturnPtr, &Wsponsor_);
  if (*ProxPtr2_)
  {
    assert(mc2.ShowX() == 781121);
    assert(mc2.ShowDupv() == 781121);
  }
  else
    assert(0);

/// Testing Built-in updater classes
  int svar = 5;         // sponsor pointer
  int cvar = 10;        // client pointer
  CountedBuiltInPtr<int> cntvar(&svar);
  UpdatePtr<int> cntudp(cntvar, BuiltInUpdater<int, int>::Make(&cvar));
  *cntudp.Pointee() += 10;
  cntudp.Update();
  assert(*cntudp.Pointee() == 15);
  assert(cvar == 15);

/// Testing void pointer classes, Built-in deleter class
  CountedVoidPtr<void> cvp1_(new short(5), BuiltInDeleter::Make());
  CountedVoidPtr<void> cvp2_(new short(10), BuiltInDeleter::Make());
  cvp1_ = cvp2_;

/// Testing temporary object class
  EnclosedClass* Ptr_ = new EnclosedClass;
  TempObject<EnclosedClass> Temp = Ptr_;
  assert(Temp.Pointee() == Ptr_);

// Testing containership of complex classes
  CopiedObjPtr<SimpleStr> cpobjpstr(SimpleStr("Hello"));

  cout <<"SUCCESS Testing SmartPtr classes" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif





