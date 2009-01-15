#ifndef HASHABLE_CPP
#define HASHABLE_CPP
#ifndef HASAHBLE_H
  #include "hashable.h"
#endif

//************************* Hashable procedures ******************************
//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>::HashableClass(int Flags_):
_HashIndex(0),
_ProbeIncr(0),
_HashFn(ElfHash),
_DblHashFn(DoubleHash2),
_Next(NULL),
_Nil(TRUE),
_Visited(FALSE),
_KeyStoreAttr(Flags_)
{}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>::HashableClass(const HashableClass<KEY, VALUE>& hb):
ObjectSkeleton((Object&)hb),
_Data(hb.GetKeyStorePtr()),
_HashIndex(hb._HashIndex),
_ProbeIncr(hb._ProbeIncr),
_HashFn(hb._HashFn),
_DblHashFn(hb._DblHashFn),
_Next(NULL),
_Nil(hb.IsNil()),
_Visited(FALSE),
_KeyStoreAttr(hb.GetKeyStorageAttributes())
{
  if (hb._Next)
    _Next = (HashableClass<KEY, VALUE>*)hb._Next->Clone();
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>::~HashableClass()
{
  if (_Next)
  {
    delete _Next;
    _Next = NULL;
  }
}

//****************************************************************************
template <class KEY, class VALUE>
Object& HashableClass<KEY, VALUE>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  HashableAcceptor<KEY, VALUE> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromHashable(TrgComp_)));
}

//****************************************************************************
template <class KEY, class VALUE>
Object* HashableClass<KEY, VALUE>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new HashableClass<KEY, VALUE>(*this));

  HashableAcceptor<KEY, VALUE> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromHashable(TrgComp_)));
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static HashableClass<KEY, VALUE> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class KEY, class VALUE>
const Object& HashableClass<KEY, VALUE>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return HashableClass<KEY, VALUE>::GiveNullObject();
}

/****************************************************************************/
template <class KEY, class VALUE>
Object& HashableClass<KEY, VALUE>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return HashableClass<KEY, VALUE>::GiveNullObject();
}

//****************************************************************************
template <class KEY, class VALUE>
long HashableClass<KEY, VALUE>::DataSize () const
{
  return 0;
}

//****************************************************************************
template <class KEY, class VALUE>
int HashableClass<KEY, VALUE>::ClassID() const
{
  return TypeInfo::HASHABLE_TYPE;
}

//****************************************************************************
template <class KEY, class VALUE>
const ObjectAcceptor* HashableClass<KEY, VALUE>::GiveObjectAcceptor() const
{
  return (new HashableAcceptor<KEY, VALUE>(this));
}

//****************************************************************************
template <class KEY, class VALUE>
const char* HashableClass<KEY, VALUE>::GiveHashableData() const
{
  return (_Data.Pointee() ?
            ::HashableData(*_Data.Pointee()):
            ((const char*)_Data.Pointee()));
}

//****************************************************************************
template <class KEY, class VALUE>
long HashableClass<KEY, VALUE>::GiveHashableDataLength() const
{
  return (_Data.Pointee() ? ::HashableDataLength(*_Data.Pointee()):0);
}

//****************************************************************************
template <class KEY, class VALUE>
Boolean HashableClass<KEY, VALUE>::IsEqualToHashable(const HashableClass<KEY, VALUE>* hb) const
{
  return
  (
    hb && IsNil() == hb->IsNil() &&
    !IsNil() && _Data.Pointee() &&
    GetIndex() == hb->GetIndex() &&
    StringKey() == hb->StringKey()
  );
}

//****************************************************************************
template <class KEY, class VALUE>
Boolean HashableClass<KEY, VALUE>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  HashableAcceptor<KEY, VALUE> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToHashable(TrgComp_);
}

/****************************************************************************/
template <class KEY, class VALUE>
const DataStorePtr<KEY>& HashableClass<KEY, VALUE>::GetKeyStorePtr() const
{
  return _Data;
}

/****************************************************************************/
template <class KEY, class VALUE>
ByteString HashableClass<KEY, VALUE>::StringKey() const
{
  if (_Data.Pointee())
    return ByteString(HashableData(*_Data.Pointee()), HashableDataLength(*_Data.Pointee()));

  return ByteString();
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::operator = (const HashableClass<KEY, VALUE>& hb)
{
  if (&hb != this && hb.IsHashable())
  {
    SetKey((KEY*)hb.GetKey());
    SetNil(hb.IsNil());
    SetIndex(hb._HashIndex);
    SetProbeIncr(hb._ProbeIncr);
    SetPrimaryHashFunction(hb._HashFn);
    SetDoubleHashFunction(hb._DblHashFn);
    SetNext(NULL);
    SetVisited(FALSE);

    _KeyStoreAttr = hb.GetKeyStorageAttributes();
    _Data = hb.GetKeyStorePtr();
    
    ObjectSkeleton::operator = ((Object&)hb);
  }
  
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetData(VALUE* ptr)
{
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
const VALUE* HashableClass<KEY, VALUE>::GetData() const
{
  return NULL;
}

/****************************************************************************/
template <class KEY, class VALUE>
VALUE* HashableClass<KEY, VALUE>::GetData()
{
  return NULL;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetKey(KEY* ptr)
{
  _Data.AssignPtr(ptr, GetKeyStorageAttributes().GetStorageAttributes());
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
const KEY* HashableClass<KEY, VALUE>::GetKey() const
{
  return _Data.Pointee();
}

/****************************************************************************/
template <class KEY, class VALUE>
KEY* HashableClass<KEY, VALUE>::GetKey()
{
  return _Data.Pointee();
}

/****************************************************************************/
template <class KEY, class VALUE>
void HashableClass<KEY, VALUE>::DestroyKey()
{
  _Data.Destroy();
}

/****************************************************************************/
template <class KEY, class VALUE>    
void HashableClass<KEY, VALUE>::DestroyData()
{}

/****************************************************************************/
template <class KEY, class VALUE>
Ulong HashableClass<KEY, VALUE>::GetIndex() const
{
  if (_Data.IsNull())
    Xnull();

  return _HashIndex;
}

/****************************************************************************/
template <class KEY, class VALUE>
Ulong HashableClass<KEY, VALUE>::GetProbeIncr() const
{
  if (_Data.IsNull())
    Xnull();

  return _ProbeIncr;
}

/****************************************************************************/
template <class KEY, class VALUE>
Ulong HashableClass<KEY, VALUE>::MakeIndex(Ulong Tabsize_)
{
  if (IsHashable() && !_Data.IsNull() && _HashFn && Tabsize_)
  {
    SetIndex(HashValue(_HashFn, _Data.Pointee(), Tabsize_));
    SetNil(FALSE);
  }
  
  return GetIndex();
}

/****************************************************************************/
template <class KEY, class VALUE>
Ulong HashableClass<KEY, VALUE>::MakeProbeIncr(Ulong Tabsize_)
{  
  if (IsHashable() && !_Data.IsNull() && _DblHashFn && Tabsize_)
    SetProbeIncr(DblHashValue(_DblHashFn, _HashIndex, Tabsize_));
    
  return GetProbeIncr();
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::ProbeAhead(Ulong Tabsize_)
{
  if (!_ProbeIncr)
    MakeProbeIncr(Tabsize_);
  
  _HashIndex += _ProbeIncr;
  _HashIndex %= Tabsize_;
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean HashableClass<KEY, VALUE>::CollidesWith(const HashableClass<KEY, VALUE>* hb) const
{
  return
  (
    hb && !_Data.IsNull() &&
    StringKey() != hb->StringKey() &&
    GetIndex() == hb->GetIndex() &&
    IsNil() == hb->IsNil() && !IsNil()
  );
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetNil(Boolean Flag_)
{
  _Nil = Flag_;
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetKeyStorageAttributes(int Flags_)
{
  _KeyStoreAttr.SetStorageAttributes(Flags_);
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
const DataStorageAttributes& HashableClass<KEY, VALUE>::GetKeyStorageAttributes() const
{
  return _KeyStoreAttr;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetVisited(Boolean Flag_)
{
  _Visited = Flag_;
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetPrimaryHashFunction(HashFunction Fn_)
{
  _HashFn = Fn_;
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetDoubleHashFunction(DblHashFunction Fn_)
{
  _DblHashFn = Fn_;
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>& HashableClass<KEY, VALUE>::SetNext(HashableClass<KEY, VALUE>* hb)
{
  _Next = hb;
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean HashableClass<KEY, VALUE>::IsHashable() const
{
  return TRUE;
}

//****************************************************************************
#if OVERLOAD_NEW
template <class KEY, class VALUE>
void* HashableClass<KEY, VALUE>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//****************************************************************************
template <class KEY, class VALUE>
void HashableClass<KEY, VALUE>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//****************************************************************************
#if	HAS_ARRAY_NEW
template <class KEY, class VALUE>
void* HashableClass<KEY, VALUE>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
//****************************************************************************
#if	HAS_ARRAY_NEW
template <class KEY, class VALUE>
void HashableClass<KEY, VALUE>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
Hashable<T>::Hashable(int Flags_):
HashableClass<T, T>(Flags_)
{}

/****************************************************************************/
template <class T>
Hashable<T>::Hashable(const Hashable<T>& hb):
HashableClass<T, T>(hb)
{}

/****************************************************************************/
template <class T>
Hashable<T>::~Hashable()
{}

/****************************************************************************/
template <class T>
HashableClass<T, T>& Hashable<T>::SetData(T* ptr)
{
  _Data.AssignPtr(ptr, GetKeyStorageAttributes().GetStorageAttributes());
  return *this;
}

/****************************************************************************/
template <class T>
const T* Hashable<T>::GetData() const
{
  return _Data.Pointee();
}

/****************************************************************************/
template <class T>
T* Hashable<T>::GetData()
{
  return _Data.Pointee();
}

/****************************************************************************/
template <class T>
void Hashable<T>::DestroyData()
{
  _Data.Destroy();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* Hashable<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void Hashable<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* Hashable<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void Hashable<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif    

/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
HashableAcceptor<KEY, VALUE>::HashableAcceptor(const HashableClass<KEY, VALUE>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean HashableAcceptor<KEY, VALUE>::IsHashable() const
{
  return TRUE;
}

/****************************************************************************/
template <class KEY, class VALUE>
void HashableAcceptor<KEY, VALUE>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const HashableClass<KEY, VALUE>*)Ptr_;
}

/****************************************************************************/
template <class KEY, class VALUE>
const void* HashableAcceptor<KEY, VALUE>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* HashableAcceptor<KEY, VALUE>::CreateFromHashable(const ObjectAcceptor& Obj_)
{
  if (!(_ObjPtr->GetKey() &&
	Obj_.IsEqvHashable(WhatIs(*_ObjPtr->GetKey()), sizeof(KEY))))
    THROW (FallibleBase::IncompatibleTypeErr());

  _VarPtr = new HashableClass<KEY, VALUE>(*((const HashableClass<KEY, VALUE>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* HashableAcceptor<KEY, VALUE>::AssignFromHashable(const ObjectAcceptor& Obj_)
{
  if (!(_ObjPtr->GetKey() &&
	Obj_.IsEqvHashable(WhatIs(*_ObjPtr->GetKey()), sizeof(KEY))))
    THROW (FallibleBase::IncompatibleTypeErr());

  const HashableClass<KEY, VALUE>* TrgPtr_ = (const HashableClass<KEY, VALUE>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean HashableAcceptor<KEY, VALUE>::IsEqvHashable(int ID_, size_t Size_) const
{
  return (ID_ == WhatIs(*_ObjPtr->GetKey()) &&
	  Size_ == sizeof(KEY));
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean HashableAcceptor<KEY, VALUE>::IsEqualToHashable(const ObjectAcceptor& Obj_) const
{
  if (!(_ObjPtr->GetKey() &&
	Obj_.IsEqvHashable(WhatIs(*_ObjPtr->GetKey()), sizeof(KEY))))
    THROW (FallibleBase::IncompatibleTypeErr());

  return _ObjPtr->IsEqualToHashable((const HashableClass<KEY, VALUE>*)Obj_.GiveDataPtr());
}

//****************************************************************************
#if OVERLOAD_NEW
template <class KEY, class VALUE>
void* HashableAcceptor<KEY, VALUE>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//****************************************************************************
template <class KEY, class VALUE>
void HashableAcceptor<KEY, VALUE>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
//****************************************************************************
//****************************************************************************
#if HASHABLE_DEBUG

int main()
{
  const Ulong tabsize = 2001;

  ChrString Str1Key_("Fred-Flintstone");
  ChrString Str1Val_("488-0093");
  ChrString Str2Key_("Winnie-Pooh");
  ChrString Str2Val_("255-0162");
  Hashable<ChrString> Node_;

  Node_.SetData(&Str2Key_);
  if (Node_.GetData())
    cout <<*Node_.GetData() <<endl;

  Node_.MakeIndex(tabsize);
  cout <<Node_.GetIndex() <<endl;

  int val = 115;
  Hashable<int> hash1;

  hash1.SetData(&val);
  if (hash1.GetData())
    cout <<*hash1.GetData() <<endl;    

  hash1.MakeIndex(tabsize);
  cout <<hash1.GetIndex() <<endl;

  Hashable<ChrString>* Node1_ = new Hashable<ChrString>;
  Node1_->SetData(&Str1Key_);
  Hashable<ChrString>* Node2_ = new Hashable<ChrString>(*Node1_);
  Hashable<ChrString>* Node3_ = new Hashable<ChrString>;
  *Node3_ = *Node2_;

  delete Node1_;
  delete Node2_;
  delete Node3_;

  return 0;
}
#endif
//****************************************************************************
#endif





