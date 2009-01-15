#ifndef DATASTORE_CPP
#define DATASTORE_CPP
#ifndef DATASTORE_H
  #include "datastore.h"
#endif

/****************************************************************************/
template <class T>
DataStorePtr<T>::DataStorePtr():
DataStorageAttributes(0),
_PtrNum(0)
{}

/****************************************************************************/
template <class T>
DataStorePtr<T>::DataStorePtr(const T& Obj_):
DataStorageAttributes(DataStorageAttributes::DEEP_COPY),
_PtrNum(0)
{
  SetupNewedPointer(::Clone(Obj_));
}

/****************************************************************************/
template <class T>
DataStorePtr<T>::DataStorePtr(T* Ptr_, int Flags_):
DataStorageAttributes(Flags_),
_PtrNum(0)
{
  SetupPointer(Ptr_);
}

/****************************************************************************/
template <class T>
DataStorePtr<T>::DataStorePtr(const DataStorePtr<T>& Obj_):
DataStorageAttributes(Obj_),
_PtrNum(0)
{
  SetupPointer(Obj_);
}

/****************************************************************************/
template <class T>
DataStorePtr<T>::DataStorePtr(const ArrayStorePtr<T>& Obj_):
DataStorageAttributes(Obj_.GetStorageAttributes()),
_PtrNum(0)
{
  SetupPointer(((ArrayStorePtr<T>*)&Obj_)->Pointee());
}

/****************************************************************************/
template <class T>
DataStorePtr<T>::~DataStorePtr()
{
  Destroy();
}

/****************************************************************************/
template <class T>
DataStorePtr<T>* DataStorePtr<T>::Make()
{
  return (new DataStorePtr<T>());
}

/****************************************************************************/
template <class T>
DataStorePtr<T>* DataStorePtr<T>::Make(const T& Obj_)
{
  return (new DataStorePtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
DataStorePtr<T>* DataStorePtr<T>::Make(T* Ptr_, int Flags_)
{
  return (new DataStorePtr<T>(Ptr_, Flags_));
}

/****************************************************************************/
template <class T>
DataStorePtr<T>* DataStorePtr<T>::Make(const DataStorePtr<T>& Obj_)
{
  return (new DataStorePtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
DataStorePtr<T>* DataStorePtr<T>::Make(const ArrayStorePtr<T>& Obj_)
{
  return (new DataStorePtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
void DataStorePtr<T>::SetTypeFlags(int& Act_, int& Deep_, int& Dest_)
{
  Act_ = IsActive();
  Deep_ = ShouldDeepCopy();
  Dest_ = ShouldDestroy();
}

/****************************************************************************/
template <class T>
void DataStorePtr<T>::SetupNewedPointer(T* Ptr_)
{
  int Act_ = IsActive();

  if (Act_ || Ptr_)
  {
    if (IsBuiltInType(Ptr_))
    {
      _CpPrimPtr = new CopiedBuiltInPtr<T>(Ptr_);
      _PtrNum = 2;
    }
    else
    {
      _CpObjPtr = new CopiedObjPtr<T>(Ptr_);
      _PtrNum = 3;
    }
  }
  else
    _RawPtr = NULL;
}

/****************************************************************************/
template <class T>
void DataStorePtr<T>::SetupPointer(const DataStorePtr<T>& Obj_)
{
  _PtrNum = Obj_._PtrNum;

  switch (_PtrNum)
  {
    case 1:
      _ConstRawPtr = Obj_.Pointee();
      SetActive(TRUE);
      break;

    case 2:
      _CpPrimPtr = new CopiedBuiltInPtr<T>(*Obj_._CpPrimPtr);
      break;    
    
    case 3:
      _CpObjPtr = new CopiedObjPtr<T>(*Obj_._CpObjPtr);
      break;

    case 4:
      _CntPrimPtr = new CountedBuiltInPtr<T>(*Obj_._CntPrimPtr);
      break;

    case 5:
      _CntObjPtr = new CountedObjPtr<T>(*Obj_._CntObjPtr);
      break;    
    
    default:
      _RawPtr = NULL;    
  }
}

/****************************************************************************/
template <class T>
void DataStorePtr<T>::SetupPointer(T* Ptr_)
{
  int Act_, Deep_, Dest_;
  SetTypeFlags(Act_, Deep_, Dest_);

  if (Act_ || Ptr_)
  {
    if (Deep_)
    {
      if (IsBuiltInType(Ptr_))
      {
        _CpPrimPtr = new CopiedBuiltInPtr<T>(::Clone(*Ptr_));
        _PtrNum = 2;
      }
      else
      {
        _CpObjPtr = new CopiedObjPtr<T>(::Clone(*Ptr_));
        _PtrNum = 3;
      }
    }
    else if (Dest_)
    {
      if (IsBuiltInType(Ptr_))
      {
        _CntPrimPtr = new CountedBuiltInPtr<T>(Ptr_);
        _PtrNum = 4;
      }
      else
      {
        _CntObjPtr = new CountedObjPtr<T>(Ptr_);
        _PtrNum = 5;
      }    
    }
    else
    {
      _RawPtr = Ptr_;
      _PtrNum = 1;
      SetActive(TRUE);
    }
  }
  else
    _RawPtr = NULL;
}

/****************************************************************************/
template <class T>
void DataStorePtr<T>::Destroy()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        delete _CpPrimPtr;
        _CpPrimPtr = NULL;
        break;
      
      case 3:
        delete _CpObjPtr;
        _CpObjPtr = NULL;
        break;

      case 4:
        delete _CntPrimPtr;
        _CntPrimPtr = NULL;
        break;
      
      case 5:
        delete _CntObjPtr;
        _CntObjPtr = NULL;
        break;        
      
      default:
        _RawPtr = NULL;
    }

  _PtrNum = 0;
}

/****************************************************************************/
template <class T>    
DataStorePtr<T>* DataStorePtr<T>::Clone()
{
  return (new DataStorePtr<T>(*this));
}

/****************************************************************************/
template <class T>        
int DataStorePtr<T>::ClassID() const
{
  return TypeInfo::DATASTOREPTR_TYPE;
}

/****************************************************************************/
template <class T>
DataStorePtr<T>& DataStorePtr<T>::operator = (const DataStorePtr<T>& rhs)
{
  if (this != &rhs)
  {
    Destroy();
    SetStorageAttributes(rhs);
    SetupPointer(rhs);
  }

  return *this;
}

/****************************************************************************/
template <class T>
DataStorePtr<T>& DataStorePtr<T>::operator = (const ArrayStorePtr<T>& rhs)
{
  if (Pointee() != rhs.Pointee())
  {
    Destroy();
    SetStorageAttributes(rhs.GetStorageAttributes());
    SetupPointer(((ArrayStorePtr<T>*)&rhs)->Pointee());
  }

  return *this;
}

/****************************************************************************/
template <class T>
DataStorePtr<T>& DataStorePtr<T>::AssignObject(const T& Obj_)
{
  Destroy();
  SetStorageAttributes(DataStorageAttributes::DEEP_COPY);
  SetupNewedPointer(::Clone(Obj_));

  return *this;
}

/****************************************************************************/
template <class T>
DataStorePtr<T>& DataStorePtr<T>::AssignPtr(T* Ptr_, int Flags_)
{
  if (Pointee() != Ptr_)
  {
    Destroy();
    SetStorageAttributes(Flags_);
    SetupPointer(Ptr_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
Boolean DataStorePtr<T>::operator ! () const
{
  return (Pointee() == ((T*)NULL));
}

/****************************************************************************/
template <class T>
Boolean DataStorePtr<T>::IsNull() const
{
  return (Pointee() == ((T*)NULL));
}

/****************************************************************************/
template <class T>
T& DataStorePtr<T>::operator * ()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return (**_CpPrimPtr);

      case 3:
        return (**_CpObjPtr);

      case 4:
        return (**_CntPrimPtr);

      case 5:
        return (**_CntObjPtr);
        
      default:
        return *_RawPtr;
    }

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_RawPtr;
}

/****************************************************************************/
template <class T>    
const T& DataStorePtr<T>::operator * () const
{
  if (_PtrNum)
    switch (_PtrNum)
    {
      case 2:
        return (**_CpPrimPtr);

      case 3:
        return (**_CpObjPtr);

      case 4:
        return (**_CntPrimPtr);

      case 5:
        return (**_CntObjPtr);
        
      default:
        return *_RawPtr;
    }

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_RawPtr;
}

/****************************************************************************/
template <class T>
T* DataStorePtr<T>::Pointee()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->Pointee();
            
      case 3:
        return _CpObjPtr->Pointee();

      case 4:
        return _CntPrimPtr->Pointee();
            
      case 5:
        return _CntObjPtr->Pointee();
        
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>    
const T* DataStorePtr<T>::Pointee() const
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->Pointee();
            
      case 3:
        return _CpObjPtr->Pointee();

      case 4:
        return _CntPrimPtr->Pointee();
            
      case 5:
        return _CntObjPtr->Pointee();        
      
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>    
const T* DataStorePtr<T>::ConstPointee() const
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->ConstPointee();
            
      case 3:
        return _CpObjPtr->ConstPointee();

      case 4:
        return _CntPrimPtr->ConstPointee();
            
      case 5:
        return _CntObjPtr->ConstPointee();
        
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>
T* DataStorePtr<T>::ReleaseControl()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->ReleaseControl();
            
      case 3:
        return _CpObjPtr->ReleaseControl();

      case 4:
        return _CntPrimPtr->ReleaseControl();
            
      case 5:
        return _CntObjPtr->ReleaseControl();
        
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>
Boolean DataStorePtr<T>::IsActive() const
{
  return DataStorageAttributes::IsActive();
}

/****************************************************************************/
template <class T>
Boolean DataStorePtr<T>::ShouldDeepCopy() const
{
  return DataStorageAttributes::ShouldDeepCopy();
}

/****************************************************************************/
template <class T>    
Boolean DataStorePtr<T>::ShouldDestroy() const
{
  return DataStorageAttributes::ShouldDestroy();
}

/****************************************************************************/
template <class T>    
int DataStorePtr<T>::GetStorageAttributes() const
{
  return DataStorageAttributes::GetStorageAttributes();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DataStorePtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void DataStorePtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* DataStorePtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void DataStorePtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>    
ArrayStorePtr<T>::ArrayStorePtr():
DataStorageAttributes(0),
_PtrNum(0),
_Length(0)
{}

/****************************************************************************/
template <class T>    
ArrayStorePtr<T>::ArrayStorePtr(const T* Ptr_, size_t Len_):
DataStorageAttributes(DataStorageAttributes::DEEP_COPY),
_PtrNum(0),
_Length(Len_)
{
  size_t x;
  T* NewObj_ = New<T>(Len_);
  
  for (x = 0; x < Len_; x++)
    NewObj_[x] = Ptr_[x];
  
  SetupNewedPointer(NewObj_);
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>::ArrayStorePtr(T* Ptr_, size_t Len_, int Flags_):
DataStorageAttributes(Flags_),
_PtrNum(0),
_Length(Len_)
{
  SetupPointer(Ptr_, Len_);
}

/****************************************************************************/
template <class T>        
ArrayStorePtr<T>::ArrayStorePtr(const ArrayStorePtr<T>& Obj_):
DataStorageAttributes(Obj_),
_PtrNum(0),
_Length(Obj_.Length())
{
  SetupPointer(Obj_);
}

/****************************************************************************/
template <class T>    
ArrayStorePtr<T>::ArrayStorePtr(const DataStorePtr<T>& Obj_):
DataStorageAttributes(Obj_.GetStorageAttributes()),
_PtrNum(0),
_Length(sizeof(Obj_.Pointee()))
{
  SetupPointer(((DataStorePtr<T>*)&Obj_)->Pointee(), sizeof(Obj_.Pointee()));
}

/****************************************************************************/
template <class T>    
ArrayStorePtr<T>::~ArrayStorePtr()
{
  Destroy();
}

/****************************************************************************/
template <class T>    
ArrayStorePtr<T>* ArrayStorePtr<T>::Make()
{
  return (new ArrayStorePtr<T>());
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>* ArrayStorePtr<T>::Make(const T* Ptr_, size_t Len_)
{
  return (new ArrayStorePtr<T>(Ptr_, Len_));
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>* ArrayStorePtr<T>::Make(T* Ptr_, size_t Len_, int Flags_)
{
  return (new ArrayStorePtr<T>(Ptr_, Len_, Flags_));
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>* ArrayStorePtr<T>::Make(const ArrayStorePtr<T>& Obj_)
{
  return (new ArrayStorePtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>* ArrayStorePtr<T>::Make(const DataStorePtr<T>& Obj_)
{
  return (new ArrayStorePtr<T>(Obj_));
}

/****************************************************************************/
template <class T>
void ArrayStorePtr<T>::SetTypeFlags(int& Act_, int& Deep_, int& Dest_)
{
  Act_ = IsActive();
  Deep_ = ShouldDeepCopy();
  Dest_ = ShouldDestroy();
}

/****************************************************************************/
template <class T>
void ArrayStorePtr<T>::SetupNewedPointer(T* Ptr_)
{
  int Act_ = IsActive();

  if (Act_ || Ptr_)
  {
    if (IsBuiltInType(Ptr_))
    {
      _CpPrimPtr = new CopiedBuiltInPtr<T>(Ptr_);
      _PtrNum = 2;
    }
    else
    {
      _CpObjPtr = new CopiedObjPtr<T>(Ptr_);
      _PtrNum = 3;
    }
  }
  else
    _RawPtr = NULL;
}

/****************************************************************************/
template <class T>
void ArrayStorePtr<T>::SetupPointer(const ArrayStorePtr<T>& Obj_)
{
  _PtrNum = Obj_._PtrNum;
  _Length = Obj_.Length();

  switch (_PtrNum)
  {
    case 1:
      _ConstRawPtr = Obj_.Pointee();
      SetActive(TRUE);
      break;

    case 2:
      _CpPrimPtr = new CopiedBuiltInPtr<T>(*Obj_._CpPrimPtr);
      break;    
    
    case 3:
      _CpObjPtr = new CopiedObjPtr<T>(*Obj_._CpObjPtr);
      break;

    case 4:
      _CntPrimPtr = new CountedBuiltInPtr<T>(*Obj_._CntPrimPtr);
      break;

    case 5:
      _CntObjPtr = new CountedObjPtr<T>(*Obj_._CntObjPtr);
      break;    
    
    default:
      _RawPtr = NULL;    
  }
}

/****************************************************************************/
template <class T>
void ArrayStorePtr<T>::SetupPointer(T* Ptr_, size_t Len_)
{
  int Act_, Deep_, Dest_;
  
  SetTypeFlags(Act_, Deep_, Dest_);
  _Length = Len_;

  if (Act_ || Ptr_)
  {
    if (Deep_)
    {
      size_t x;
      T* NewObj_ = New<T>(Len_);
  
      for (x = 0; x < Len_; x++)
        NewObj_[x] = Ptr_[x];
    
      if (IsBuiltInType(Ptr_))
      {
        _CpPrimPtr = new CopiedBuiltInPtr<T>(NewObj_);
        _PtrNum = 2;
      }
      else
      {
        _CpObjPtr = new CopiedObjPtr<T>(NewObj_);
        _PtrNum = 3;
      }
    }
    else if (Dest_)
    {
      if (IsBuiltInType(Ptr_))
      {
        _CntPrimPtr = new CountedBuiltInPtr<T>(Ptr_);
        _PtrNum = 4;
      }
      else
      {
        _CntObjPtr = new CountedObjPtr<T>(Ptr_);
        _PtrNum = 5;
      }    
    }
    else
    {
      _RawPtr = Ptr_;
      _PtrNum = 1;
      SetActive(TRUE);
    }
  }
  else
    _RawPtr = NULL;
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>* ArrayStorePtr<T>::Clone()
{
  return (new ArrayStorePtr<T>(*this));
}

/****************************************************************************/
template <class T>
void ArrayStorePtr<T>::Destroy()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        delete _CpPrimPtr;
        _CpPrimPtr = NULL;
        break;
      
      case 3:
        delete _CpObjPtr;
        _CpObjPtr = NULL;
        break;

      case 4:
        delete _CntPrimPtr;
        _CntPrimPtr = NULL;
        break;
      
      case 5:
        delete _CntObjPtr;
        _CntObjPtr = NULL;
        break;        
      
      default:
        _RawPtr = NULL;
    }

  _PtrNum = 0;
}

/****************************************************************************/
template <class T>        
int ArrayStorePtr<T>::ClassID() const
{
  return TypeInfo::ARRAYSTOREPTR_TYPE;
}

/****************************************************************************/
template <class T>
T& ArrayStorePtr<T>::operator [] (ptrdiff_t Index_)
{
  if (0 <= Index_ && Index_ < Length())
    return (Pointee())[Index_];

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return (Pointee())[0];
}

/****************************************************************************/
template <class T>
const T& ArrayStorePtr<T>::operator [] (ptrdiff_t Index_) const
{
  if (0 <= Index_ && Index_ < Length())
    return (Pointee())[Index_];

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return (Pointee())[0];
}

/****************************************************************************/
template <class T>
T* ArrayStorePtr<T>::operator () (ptrdiff_t Index_)
{
  if (0 <= Index_ && Index_ < Length())
    return (Pointee() + Index_);

  return NULL;
}

/****************************************************************************/
template <class T>
const T* ArrayStorePtr<T>::operator () (ptrdiff_t Index_) const
{
  if (0 <= Index_ && Index_ < Length())
    return (Pointee() + Index_);

  return NULL;
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>& ArrayStorePtr<T>::operator = (const ArrayStorePtr<T>& rhs)
{
  if (this != &rhs)
  {
    Destroy();
    SetStorageAttributes(rhs);
    SetupPointer(rhs);
  }

  return *this;
}

/****************************************************************************/
template <class T>
ArrayStorePtr<T>& ArrayStorePtr<T>::operator = (const DataStorePtr<T>& rhs)
{
  if (Pointee() != rhs.Pointee())
  {
    Destroy();
    SetStorageAttributes(rhs.GetStorageAttributes());
    SetupPointer(((DataStorePtr<T>*)&rhs)->Pointee(), sizeof(rhs.Pointee()));
  }

  return *this;
}

/****************************************************************************/
template <class T>    
ArrayStorePtr<T>& ArrayStorePtr<T>::AssignPtr(T* Ptr_, size_t Len_, int Flags_)
{
  if (Pointee() != Ptr_)
  {
    Destroy();
    SetStorageAttributes(Flags_);
    SetupPointer(Ptr_, Len_);
  }

  return *this;
}

/****************************************************************************/
template <class T>    
ArrayStorePtr<T>& ArrayStorePtr<T>::AssignObject(const T* Ptr_, size_t Len_)
{
  Destroy();
  SetStorageAttributes(DataStorageAttributes::DEEP_COPY);

  size_t x;
  T* NewObj_ = New<T>(Len_);
  
  for (x = 0; x < Len_; x++)
    NewObj_[x] = Ptr_[x];
  
  SetupNewedPointer(NewObj_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean ArrayStorePtr<T>::operator ! () const
{
  return (Pointee() == ((T*)NULL));
}

/****************************************************************************/
template <class T>
Boolean ArrayStorePtr<T>::IsNull() const
{
  return (Pointee() == ((T*)NULL));
}

/****************************************************************************/
template <class T>
T& ArrayStorePtr<T>::operator * ()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return (**_CpPrimPtr);

      case 3:
        return (**_CpObjPtr);

      case 4:
        return (**_CntPrimPtr);

      case 5:
        return (**_CntObjPtr);
        
      default:
        return *_RawPtr;
    }

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_RawPtr;
}

/****************************************************************************/
template <class T>    
const T& ArrayStorePtr<T>::operator * () const
{
  if (_PtrNum)
    switch (_PtrNum)
    {
      case 2:
        return (**_CpPrimPtr);

      case 3:
        return (**_CpObjPtr);

      case 4:
        return (**_CntPrimPtr);

      case 5:
        return (**_CntObjPtr);
        
      default:
        return *_RawPtr;
    }

  THROW (LoneFallibleBase::DerefNullPointerErr());
  return *_RawPtr;
}

/****************************************************************************/
template <class T>
T* ArrayStorePtr<T>::Pointee()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->Pointee();
            
      case 3:
        return _CpObjPtr->Pointee();

      case 4:
        return _CntPrimPtr->Pointee();
            
      case 5:
        return _CntObjPtr->Pointee();
        
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>    
const T* ArrayStorePtr<T>::Pointee() const
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->Pointee();
            
      case 3:
        return _CpObjPtr->Pointee();

      case 4:
        return _CntPrimPtr->Pointee();
            
      case 5:
        return _CntObjPtr->Pointee();        
      
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>    
const T* ArrayStorePtr<T>::ConstPointee() const
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->ConstPointee();
            
      case 3:
        return _CpObjPtr->ConstPointee();

      case 4:
        return _CntPrimPtr->ConstPointee();
            
      case 5:
        return _CntObjPtr->ConstPointee();
        
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>
T* ArrayStorePtr<T>::ReleaseControl()
{
  if (_PtrNum)
    switch (_PtrNum)
    {        
      case 2:
        return _CpPrimPtr->ReleaseControl();
            
      case 3:
        return _CpObjPtr->ReleaseControl();

      case 4:
        return _CntPrimPtr->ReleaseControl();
            
      case 5:
        return _CntObjPtr->ReleaseControl();
        
      default:
        return _RawPtr;
    }

  return NULL;
}

/****************************************************************************/
template <class T>
Boolean ArrayStorePtr<T>::IsActive() const
{
  return DataStorageAttributes::IsActive();
}

/****************************************************************************/
template <class T>
Boolean ArrayStorePtr<T>::ShouldDeepCopy() const
{
  return DataStorageAttributes::ShouldDeepCopy();
}

/****************************************************************************/
template <class T>    
Boolean ArrayStorePtr<T>::ShouldDestroy() const
{
  return DataStorageAttributes::ShouldDestroy();
}

/****************************************************************************/
template <class T>    
int ArrayStorePtr<T>::GetStorageAttributes() const
{
  return DataStorageAttributes::GetStorageAttributes();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ArrayStorePtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void ArrayStorePtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ArrayStorePtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void ArrayStorePtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if DATASTOREPTR_DEBUG

template <class T>
SimpleContainer<T>::SimpleContainer():
_name(SimpleContainer<T>::GetCount())
{}

template <class T>
SimpleContainer<T>::SimpleContainer(T* Data_, int Flags_):
_name(SimpleContainer<T>::GetCount()),
_dsptr(Data_, Flags_)
{
  Data_->SetOwner(_name);
  _dsptr.Pointee()->SetOwner(_name);
}

template <class T>
SimpleContainer<T>::SimpleContainer(const SimpleContainer<T>& cont_):
_name(SimpleContainer<T>::GetCount()),
_dsptr(cont_._dsptr)
{
  _dsptr.Pointee()->SetOwner(_name);
}

template <class T>
SimpleContainer<T>::~SimpleContainer()
{
  ::Delete(_name);
  _name = NULL;
}

template <class T>
char* SimpleContainer<T>::GetCount()
{
  static size_t InstCnt_ = 0;
  ++InstCnt_;  
  char* Buf_ = UintToStr(InstCnt_, NULL);
  size_t Len_ = strlen(Buf_);
  memmove(&Buf_[4], Buf_, Len_ + 1);
  memmove(Buf_, "cont", 4);
  return Buf_;
}

template <class T>
SimpleContainer<T>& SimpleContainer<T>::operator = (const SimpleContainer<T>& cont_)
{
  if (this != &cont_)
  {
    _dsptr = cont_._dsptr;
    _dsptr.Pointee()->SetOwner(_name);    
  }

  return *this;
}

template <class T>
void SimpleContainer<T>::SetData(T* Data_, int Flags_)
{
  _dsptr.AssignPtr(Data_, Flags_);
  Data_->SetOwner(_name);
  _dsptr.Pointee()->SetOwner(_name);  
}
    
template <class T>
DataStorePtr<T>& SimpleContainer<T>::GetData()
{
  return _dsptr;
}

template <class T>
const DataStorePtr<T>& SimpleContainer<T>::GetData() const
{
  return _dsptr;
}

/****************************************************************************/
int main()
{
  Sample* spdata = new Sample(4, 16);
  Sample* spdata2 = new Sample(999, 555);
  spdata2->SetOwner("spdata2");
  Sample* spdata3 = new Sample(4, 16);
  
  int* intdata = new int(10);
  int* intdata2 = new int(24);
  int intval = 56;
  Sample spobject(32, 512);
  spobject.SetOwner("spobject");

  // Testing const DataStorePtr objects as class members
  SimpleContainer<Sample> cont1_(spdata3, DataStorageAttributes::DESTROY);
  SimpleContainer<Sample> cont2_;
  const SimpleContainer<Sample> cont3_(cont1_);
  const SimpleContainer<Sample> cont4_(spdata3, DataStorageAttributes::DEEP_COPY);
  const SimpleContainer<Sample> cont5_(cont4_);
  SimpleContainer<Sample> cont6_(cont4_);
  cont2_ = cont1_;
  cont6_ = cont5_;      // note destructor message "owned by cont6"

  // Testing different storage schemes for each DataStorePtr object
  DataStorePtr<Sample> dsp1_(spdata2, DataStorageAttributes::DEEP_COPY);
  dsp1_.Pointee()->SetOwner("dsp1_");
  DataStorePtr<Sample> dsp2_(spdata, DataStorageAttributes::DESTROY);
  dsp2_.Pointee()->SetOwner("dsp2_");  
  DataStorePtr<int>* dsp3_ = new DataStorePtr<int>;
  DataStorePtr<Sample>* dsp4_ = new DataStorePtr<Sample>;
  DataStorePtr<int>* dsp11_ = new DataStorePtr<int>(intval);
  DataStorePtr<Sample>* dsp12_ = new DataStorePtr<Sample>;

  DataStorePtr<int> dsp21_(*dsp11_);
  DataStorePtr<Sample> dsp22_;

  dsp12_->AssignObject(spobject);
  dsp12_->Pointee()->SetOwner("dsp12_");
  dsp3_->AssignPtr(intdata2, 0);
  *dsp4_ = dsp2_;
  DataStorePtr<Sample>* dsp5_ = new DataStorePtr<Sample>(dsp1_);
  dsp5_->Pointee()->SetOwner("dsp5_");
  DataStorePtr<int>* dsp6_ = dsp3_->Clone();
  dsp22_ = *dsp12_;
  dsp22_.Pointee()->SetOwner("dsp22_");

  // testing if values and addresses are consistent
  // for different storage schemes
  assert(dsp1_.Pointee()->Getx() == 999 && dsp1_.Pointee()->Gety() == 555);
  assert(dsp2_.Pointee()->Getx() == 4 && dsp2_.Pointee()->Gety() == 16);
  assert(**dsp3_ == 24);
  assert(dsp4_->Pointee()->Getx() == 4 && dsp4_->Pointee()->Gety() == 16);
  assert(dsp5_->Pointee()->Getx() == 999 && dsp5_->Pointee()->Gety() == 555);
  assert(**dsp6_ == 24);
  assert(dsp2_.Pointee() == dsp4_->Pointee());
  assert(dsp5_->Pointee() != dsp1_.Pointee());
  assert(dsp6_->Pointee() == dsp3_->Pointee());
  
  assert(dsp11_->Pointee() != dsp21_.Pointee());
  assert(*dsp11_->Pointee() == *dsp21_.Pointee());
  assert(dsp12_->Pointee() != dsp22_.Pointee());
  assert(dsp12_->Pointee()->Getx() == 32 && dsp12_->Pointee()->Gety() == 512);
  assert(dsp22_.Pointee()->Getx() == 32 && dsp22_.Pointee()->Gety() == 512);

#if DATASTOREPTR_SHOW_DEBUG_OUTPUT
  cout <<"dsp1: " <<(*dsp1_) <<endl;
  cout <<"dsp2: " <<(*dsp2_) <<endl;
  cout <<"dsp3: " <<(**dsp3_) <<endl;
  cout <<"dsp4: " <<(**dsp4_) <<endl;
  cout <<"dsp5: " <<(**dsp5_) <<endl;
  cout <<"dsp6: " <<(**dsp6_) <<endl;
  cout <<"&dsp4 == &dsp2: " <<(dsp2_.Pointee() == dsp4_->Pointee()) <<endl;
  cout <<"&dsp5 == &dsp1: " <<(dsp5_->Pointee() == dsp1_.Pointee()) <<endl;
  cout <<"&dsp6 == &dsp3: " <<(dsp6_->Pointee() == dsp3_->Pointee()) <<endl;
  cout <<"dsp11: " <<(**dsp11_) <<endl;
  cout <<"dsp12: " <<(**dsp12_) <<endl;
  cout <<"dsp21: " <<(*dsp21_) <<endl;
  cout <<"dsp22: " <<(*dsp22_) <<endl;
#endif

  // testing if values and addresses are consistent after destruction
  // of copied or shared pointers
  dsp2_.Destroy();       // note delayed destructor message
  delete dsp3_;
  delete dsp5_;          // note destructor message "owned by dsp5"
  delete dsp11_;
  delete dsp12_;         // note destructor message "owned by dsp12"

  assert(dsp1_.Pointee()->Getx() == 999 && dsp1_.Pointee()->Gety() == 555);
  assert(dsp4_->Pointee()->Getx() == 4 && dsp4_->Pointee()->Gety() == 16);
  assert(**dsp6_ == 24);
  assert(*dsp21_ == 56);
  assert(dsp22_.Pointee()->Getx() == 32 && dsp22_.Pointee()->Gety() == 512);

#if DATASTOREPTR_SHOW_DEBUG_OUTPUT
  cout <<"dsp2_.Destroy();\n";
  cout <<"delete dsp3_;\n";
  cout <<"delete dsp5_;\n";
  cout <<"delete dsp11_;\n";
  cout <<"delete dsp12_;\n\n";
  
  cout <<"dsp1: " <<(*dsp1_) <<endl;
  cout <<"dsp4: " <<(**dsp4_) <<endl;
  cout <<"dsp6: " <<(**dsp6_) <<endl;
  cout <<"dsp21: " <<(*dsp21_) <<endl;
  cout <<"dsp22: " <<(*dsp22_) <<endl;
#endif

  delete dsp4_;          // note destructor message "owned by dsp2"
  delete spdata2;        // note destructor message "owned by spdata2"

#if DATASTOREPTR_SHOW_DEBUG_OUTPUT
  cout <<"delete dsp4_;\n";
  cout <<"delete spdata2;\n";
#endif

#if DATASTOREPTR_NOTIFY_SUCCESS
  cout <<"SUCCESS Testing DataStorePtr classes" <<endl;
#endif

  return 0;
// note destructor message for: containers: cont2, cont4, cont5, con6
//                              pointers: dsp1, dsp22
//                              object: spobject
}
#endif
/****************************************************************************/
#endif




