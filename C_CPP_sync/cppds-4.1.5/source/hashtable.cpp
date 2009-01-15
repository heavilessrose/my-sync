/*****************************************************************************
*			Joseph Wong / Nov 29 1994			     *
* Hashtable source code file						     *
* Implements a Hashtable with container classes, Iterator classes,	     *
* and associate classes							     *
*****************************************************************************/
#ifndef HASHTABLE_CPP
#define HASHTABLE_CPP
#ifndef HASHTABLE_H
  #include "hashtable.h"
#endif

//************************* Hashtable procedures *****************************
//****************************************************************************
template <class KEY, class VALUE>
HashTable<KEY, VALUE>::HashTable(Ulong Size_, const VALUE& DefaultVal_, int Scheme_):
_DefaultValue(DefaultVal_),
_TblSize(Size_),
_ItemCount(0),
_FullTable(FALSE),
_Found(FALSE),
_OwnsPtr(FALSE),
_StorageScheme(Scheme_)
{
  _Table = AllocTable(Size_);

  for (Ulong i = 0; i < Size_; i++)
    _Table[i] = NULL;
}

//****************************************************************************
template <class KEY, class VALUE>
HashTable<KEY, VALUE>::HashTable(const HashTable<KEY, VALUE>& ht):
_DefaultValue(ht._DefaultValue),
_TblSize(ht._TblSize),
_ItemCount(0),
_FullTable(FALSE),
_Found(FALSE),
_OwnsPtr(ht._OwnsPtr),
_StorageScheme(ht._StorageScheme)
{
  _Table = AllocTable(ht._TblSize);
  const HashTableIterator<KEY, VALUE> Source_(ht);
  const HashableClass<KEY, VALUE>* SrcObj_;

  Ulong Index_;
  Boolean Done_ = TRUE;
  while (SrcObj_ = ++Source_ && Done_)
    Add((HashableClass<KEY, VALUE>*)(SrcObj_->Clone()), Index_, Done_);

  if (!Done_)
    Xfull();
}

//****************************************************************************
template <class KEY, class VALUE>
HashTable<KEY, VALUE>::~HashTable()
{
  EraseTable();
  ::DeleteArray(_Table);  
}

//****************************************************************************
template <class KEY, class VALUE>
void HashTable<KEY, VALUE>::EraseTable()
{
  while (_TblSize)
  {
    delete _Table[--_TblSize];
    _Table[_TblSize] = NULL;
  }

  _ItemCount = 0;
}

//****************************************************************************
template <class KEY, class VALUE>
HashTable<KEY, VALUE>& HashTable<KEY, VALUE>::SetOwnsValuePtr(Boolean Flag_)
{
  if (Flag_)
    _OwnsPtr |= OWNS_VALUE;
  else
    _OwnsPtr &= ~OWNS_VALUE;

  return *this;
}

//****************************************************************************
template <class KEY, class VALUE>
HashTable<KEY, VALUE>& HashTable<KEY, VALUE>::SetOwnsKeyPtr(Boolean Flag_)
{
  if (Flag_)
    _OwnsPtr |= OWNS_KEY;
  else
    _OwnsPtr &= ~OWNS_KEY;

  return *this;
}

//****************************************************************************
template <class KEY, class VALUE>
HashTable<KEY, VALUE>& HashTable<KEY, VALUE>::operator = (const HashTable<KEY, VALUE>& Htable_)
{
  if (this != &Htable_)
  {
    EraseTable();
    ::DeleteArray(_Table);
  
    _Table = AllocTable(Htable_._TblSize);
    _OwnsPtr = Htable_._OwnsPtr;

    const HashTableIterator<KEY, VALUE> Source_(Htable_);
    const HashableClass<KEY, VALUE>* SrcObj_;

    Ulong Index_;
    Boolean Done_ = TRUE;
    while (SrcObj_ = ++Source_ && Done_)
      Add((HashableClass<KEY, VALUE>*)(SrcObj_->Clone()), Index_, Done_);

    if (!Done_)
      Xfull();
  }

  return *this;
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>** HashTable<KEY, VALUE>::AllocTable(Ulong Size_)
{
  Ulong i;
  HashableClass<KEY, VALUE>** TablePtr_;
  Size_ = NextPrime(Size_);
  _TblSize = Size_;

#if OVERLOAD_NEW
  TablePtr_ = ((HashableClass<KEY, VALUE>**)MemMatrix::Matrix().Allocate(sizeof(HashableClass<KEY, VALUE>*) * Size_));
#else
  TablePtr_ = (new HashableClass<KEY, VALUE>*[Size_]);
#endif

  for (i = 0; i < Size_; i++)
    TablePtr_[i] = NULL;

  return TablePtr_;
}

//****************************************************************************
template <class KEY, class VALUE>
Ulong HashTable<KEY, VALUE>::GetTableSize() const
{
  return _TblSize;
}

//****************************************************************************
template <class KEY, class VALUE>
Ulong HashTable<KEY, VALUE>::GetItemCount() const
{
  return _ItemCount;
}

//****************************************************************************
template <class KEY, class VALUE>
Ulong HashTable<KEY, VALUE>::NextPrime(Ulong Size_)
{
  static PrimesTable PrimesTbl_;
  return PrimesTbl_.Ceiling(Size_);
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayReference<KEY, VALUE> HashTable<KEY, VALUE>::operator [] (const KEY& Destination_)
{
  HashableClass<KEY, VALUE>* DataStore_ = new Association<KEY, VALUE>;
  KEY* NewData_ = ::Clone(Destination_);
  DataStore_->SetKey(NewData_);
  ArrayReference<KEY, VALUE> HtRef_(*this, DataStore_);
  ::Delete(NewData_);
  delete DataStore_;
  return HtRef_;
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayReference<KEY, VALUE> HashTable<KEY, VALUE>::operator [] (const HashableClass<KEY, VALUE>& Destination_)
{
  ArrayReference<KEY, VALUE> HtRef_(*this, ((HashableClass<KEY, VALUE>*)&Destination_));
  return HtRef_;
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayPtrReference<KEY, VALUE> HashTable<KEY, VALUE>::operator () (KEY* Destination_)
{
  HashableClass<KEY, VALUE>* DataStore_ = new Association<KEY, VALUE>;

  int StoreAttrib_ =  DataStorageAttributes::ACTIVE;
  if (OwnsKeyPtr())
    StoreAttrib_ |= DataStorageAttributes::DESTROY;  

  DataStore_->SetKeyStorageAttributes(StoreAttrib_);

  StoreAttrib_ =  DataStorageAttributes::ACTIVE;
  if (OwnsValuePtr())
    StoreAttrib_ |= DataStorageAttributes::DESTROY;
  
  DataStore_->SetValueStorageAttributes(StoreAttrib_);
  
  DataStore_->SetKey(Destination_);
  ArrayPtrReference<KEY, VALUE> HtRef_(*this, DataStore_);
  delete DataStore_;
  return HtRef_;
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayPtrReference<KEY, VALUE> HashTable<KEY, VALUE>::operator () (HashableClass<KEY, VALUE>* Destination_)
{
  int StoreAttrib_ =  DataStorageAttributes::ACTIVE;
  if (OwnsKeyPtr())
    StoreAttrib_ |= DataStorageAttributes::DESTROY;

  Destination_->SetKeyStorageAttributes(StoreAttrib_);

  StoreAttrib_ =  DataStorageAttributes::ACTIVE;
  if (OwnsValuePtr())
    StoreAttrib_ |= DataStorageAttributes::DESTROY;

  Destination_->SetValueStorageAttributes(StoreAttrib_);
  
  ArrayPtrReference<KEY, VALUE> HtRef_(*this, Destination_);
  return HtRef_;
}

/****************************************************************************/
template <class KEY, class VALUE>
VALUE& HashTable<KEY, VALUE>::GiveDefaultValue()
{
  return _DefaultValue;
}

/****************************************************************************/
template <class KEY, class VALUE>
const VALUE& HashTable<KEY, VALUE>::GiveDefaultValue() const
{
  return _DefaultValue;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTable<KEY, VALUE>::StoreItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_)
{
  if (Source_ && !Source_->IsEqualToHashable(Target_))
    if (Source_->IsEmptySlot(Target_))
    {
      if (Target_)
        delete Target_;

      Target_ = (HashableClass<KEY, VALUE>*)Source_->Clone();
      return Target_;
    }
    else if (!Target_->IsVisited())
    {
      Target_->SetVisited(TRUE);
      Source_->ProbeAhead(_TblSize);
      Source_ = StoreItem(_Table[Source_->GetIndex()], Source_);
      Target_->SetVisited(FALSE);
      return Source_;
    }
    else
      SetFullTable(TRUE);
    
  return NULL;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTable<KEY, VALUE>::FindItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_)
{
  if (Source_)
    if (Source_->IsEqualToHashable(Target_))
      return Target_;    
    else if (!Target_ || !Target_->IsVisited())
    {
      if (!Target_)
        Target_ = new HashableClass<KEY, VALUE>;

      Target_->SetVisited(TRUE);    
      Source_->ProbeAhead(_TblSize);
      Source_ = FindItem(_Table[Source_->GetIndex()], Source_);
      Target_->SetVisited(FALSE);
      return Source_;      
    }
  
  return NULL;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTable<KEY, VALUE>::ChainStoreItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_)
{
  if (Source_ && !Source_->IsEqualToHashable(Target_))
    if (Source_->IsEmptySlot(Target_))
    {
      if (Target_)
        delete Target_;

      Target_ = (HashableClass<KEY, VALUE>*)Source_->Clone();
      return Target_;        
    }
    else
    {
      HashableClass<KEY, VALUE>* Ptr_ =
      ChainStoreItem(Target_->GetNext(), Source_);
      
      if (Ptr_)
        Target_->SetNext(Ptr_);
        
      return Target_;
    }

  return NULL;
}

/****************************************************************************/
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTable<KEY, VALUE>::ChainFindItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_)
{
  if (Source_)
    if (Source_->IsEqualToHashable(Target_))
      return Target_;
    else if (Target_ && Target_->GetNext())
      return ChainFindItem(Target_->GetNext(), Source_);
    else
      return NULL;
    
  return NULL;
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTable<KEY, VALUE>::LookUp(HashableClass<KEY, VALUE>* HbPtr_, Ulong& Index_, Boolean& Done_)
{
  Done_ = FALSE;
  if (!HbPtr_ || !HbPtr_->GetKey())
    return NULL;  

  HbPtr_->MakeIndex(_TblSize);
  if (!HbPtr_->IsReady())
    return NULL;

  if (_StorageScheme == DOUBLEHASH)
    HbPtr_ = FindItem(_Table[HbPtr_->GetIndex()], HbPtr_);
  else
    HbPtr_ = ChainFindItem(_Table[HbPtr_->GetIndex()], HbPtr_);

  Done_ = HbPtr_ != NULL;
  
  if (Done_)
    Index_ = HbPtr_->GetIndex();

  return HbPtr_;
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTable<KEY, VALUE>::Add(HashableClass<KEY, VALUE>* HbPtr_, Ulong& Index_, Boolean& Done_)
{
  Done_ = FALSE;
  if (!HbPtr_ || !HbPtr_->GetKey())
    return NULL;

  HbPtr_->MakeIndex(_TblSize);
  if (!HbPtr_->IsReady())
    return NULL;

  if (_StorageScheme == DOUBLEHASH)    
    HbPtr_ = _Table[HbPtr_->GetIndex()] = StoreItem(_Table[HbPtr_->GetIndex()], HbPtr_);
  else
    HbPtr_ = _Table[HbPtr_->GetIndex()] = ChainStoreItem(_Table[HbPtr_->GetIndex()], HbPtr_);
  
  Done_ = HbPtr_ != NULL;
  
  if (Done_)
  {
    ++_ItemCount;  
    Index_ = HbPtr_->GetIndex();
  }
  
  return HbPtr_;
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTable<KEY, VALUE>::Remove(HashableClass<KEY, VALUE>* HbPtr_, Ulong& Index_, Boolean& Done_)
{
  HbPtr_ = LookUp(HbPtr_, Index_, Done_);

  if (HbPtr_ && Done_)
  {
    --_ItemCount;
    HbPtr_->SetNil();    
  }

  return HbPtr_;
}

//********************** array_reference procedures **************************
//****************************************************************************
template <class KEY, class VALUE>
ArrayReference<KEY, VALUE>::ArrayReference(HashTable<KEY, VALUE>& MyBoss_, HashableClass<KEY, VALUE>* MyData_):
_HtPtr(&MyBoss_),
_HbPtr(MyData_ ? (HashableClass<KEY, VALUE>*)MyData_->Clone():NULL)
{}

/****************************************************************************/
template <class KEY, class VALUE>
ArrayReference<KEY, VALUE>::~ArrayReference()
{
  if (refCount.Unique())
  {
    delete _HbPtr;
    _HbPtr = NULL;
  }
}

//****************************************************************************
template <class KEY, class VALUE>
VALUE& ArrayReference<KEY, VALUE>::Store()
{
  HashableClass<KEY, VALUE>* Hptr_;
  Ulong TblIndex_;
  Boolean Done_;
  VALUE* RetVal_ = NULL;

  Hptr_ = _HtPtr->Add(_HbPtr, TblIndex_, Done_);
  Done_ = (Done_ && Hptr_);
  RetVal_ = (Done_ ? Hptr_->GetData():(VALUE*)NULL);
  _HtPtr->SetFound(Done_);
  
  return (RetVal_ ? *RetVal_:_HtPtr->GiveDefaultValue());
}

//****************************************************************************
template <class KEY, class VALUE>
Boolean ArrayReference<KEY, VALUE>::Erase()
{
  HashableClass<KEY, VALUE>* Hptr_;
  Ulong TblIndex_;
  Boolean Done_;

  Hptr_ = _HtPtr->Remove(_HbPtr, TblIndex_, Done_);
  Done_ = (Done_ && Hptr_);
  _HtPtr->SetFound(Done_);

  return Done_;
}

//****************************************************************************
template <class KEY, class VALUE>
VALUE& ArrayReference<KEY, VALUE>::operator = (const VALUE& Val_)
{
  VALUE NewVal_(Val_);
  _HbPtr->SetData(&NewVal_);
  return Store();
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayReference<KEY, VALUE>::operator const VALUE& () const
{
  return ArrayReference<KEY, VALUE>::operator VALUE& ();
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayReference<KEY, VALUE>::operator VALUE& ()
{
  HashableClass<KEY, VALUE>* Hptr_;
  Ulong TblIndex_;
  Boolean Done_;
  VALUE* RetVal_ = NULL;

  Hptr_ = _HtPtr->LookUp(_HbPtr, TblIndex_, Done_);
  Done_ = (Done_ && Hptr_);
  RetVal_ = (Done_ ? Hptr_->GetData():(VALUE*)NULL);
  _HtPtr->SetFound(Done_);

  return (RetVal_ ? *RetVal_:_HtPtr->GiveDefaultValue());
}

//********************** ArrayPtrReference procedures **************************
//****************************************************************************
template <class KEY, class VALUE>
ArrayPtrReference<KEY, VALUE>::ArrayPtrReference(HashTable<KEY, VALUE>& MyBoss_, HashableClass<KEY, VALUE>* MyData_):
_HtPtr(&MyBoss_),
_HbPtr(MyData_ ? (HashableClass<KEY, VALUE>*)MyData_->Clone():NULL)
{}

/****************************************************************************/
template <class KEY, class VALUE>
ArrayPtrReference<KEY, VALUE>::~ArrayPtrReference()
{
  if (refCount.Unique())
  {
    delete _HbPtr;
    _HbPtr = NULL;
  }
}

//****************************************************************************
template <class KEY, class VALUE>
VALUE* ArrayPtrReference<KEY, VALUE>::Store()
{
  HashableClass<KEY, VALUE>* Hptr_;
  Ulong TblIndex_;
  Boolean Done_;
  VALUE* RetVal_ = NULL;

  Hptr_ = _HtPtr->Add(_HbPtr, TblIndex_, Done_);
  Done_ = (Done_ && Hptr_);
  RetVal_ = (Done_ ? Hptr_->GetData():(VALUE*)NULL);
  _HtPtr->SetFound(Done_);  
  return RetVal_;
}

//****************************************************************************
template <class KEY, class VALUE>
Boolean ArrayPtrReference<KEY, VALUE>::Erase()
{
  HashableClass<KEY, VALUE>* Hptr_;
  Ulong TblIndex_;
  Boolean Done_;

  Hptr_ = _HtPtr->Remove(_HbPtr, TblIndex_, Done_);
  Done_ = (Done_ && Hptr_);
  _HtPtr->SetFound(Done_);

  return Done_;
}

//****************************************************************************
template <class KEY, class VALUE>
VALUE* ArrayPtrReference<KEY, VALUE>::operator = (VALUE* Val_)
{
  _HbPtr->SetData(Val_);
  return Store();
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayPtrReference<KEY, VALUE>::operator const VALUE* () const
{
  return ArrayPtrReference<KEY, VALUE>::operator VALUE* ();
}

//****************************************************************************
template <class KEY, class VALUE>
ArrayPtrReference<KEY, VALUE>::operator VALUE* ()
{
  HashableClass<KEY, VALUE>* Hptr_;
  Ulong TblIndex_;
  Boolean Done_;
  VALUE* RetVal_ = NULL;

  Hptr_ = _HtPtr->LookUp(_HbPtr, TblIndex_, Done_);
  Done_ = (Done_ && Hptr_);
  RetVal_ = (Done_ ? Hptr_->GetData():(VALUE*)NULL);
  _HtPtr->SetFound(Done_);
  return RetVal_;
}

//***************** HashTable Const Iterator procedures **********************
//****************************************************************************
template <class KEY, class VALUE>
HashTableIterator<KEY, VALUE>::HashTableIterator(const HashTable<KEY, VALUE>& Target_):
_ArraySize(Target_.GetTableSize()),
_IterIndex(Target_.GetTableSize()),
_PostIncValid(TRUE),
_ConstArrPtr(&Target_)
{}

//****************************************************************************
template <class KEY, class VALUE>
void HashTableIterator<KEY, VALUE>::Attach(const HashTable<KEY, VALUE>& Target_) const
{
  _ConstArrPtr = &Target_;
  _IterIndex = _ArraySize = Target_.GetTableSize();
  _PostIncValid = TRUE;
}

//****************************************************************************
template <class KEY, class VALUE>
const HashableClass<KEY, VALUE>* HashTableIterator<KEY, VALUE>::operator ++ () const
{
  const HashableClass<KEY, VALUE>* BasePtr_;

  if (_IterIndex && (BasePtr_ = _ArrPtr->_Table[--_IterIndex]))
    return ((const HashableClass<KEY, VALUE>*)BasePtr_);

  _PostIncValid = FALSE;
  return NULL;
}

//****************************************************************************
template <class KEY, class VALUE>
const HashableClass<KEY, VALUE>* HashTableIterator<KEY, VALUE>::operator ++ (int) const
{
  if (_IterIndex == _ArraySize)
    ++(*this);

  const HashableClass<KEY, VALUE>* RetVal_ =
    _PostIncValid ? _ArrPtr->_Table[_IterIndex]:NULL;

  if (RetVal_)
    ++(*this);

  return RetVal_;
}

//****************************************************************************
template <class KEY, class VALUE>
void HashTableIterator<KEY, VALUE>::Attach(HashTable<KEY, VALUE>& Target_)
{
  _ArrPtr = &Target_;
  _IterIndex = _ArraySize = Target_.GetTableSize();
  _PostIncValid = TRUE;
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTableIterator<KEY, VALUE>::operator ++ ()
{
  HashableClass<KEY, VALUE>* BasePtr_;

  if (_IterIndex && (BasePtr_ = _ArrPtr->_Table[--_IterIndex]))
    return ((HashableClass<KEY, VALUE>*)BasePtr_);

  _PostIncValid = FALSE;
  return NULL;
}

//****************************************************************************
template <class KEY, class VALUE>
HashableClass<KEY, VALUE>* HashTableIterator<KEY, VALUE>::operator ++ (int)
{
  if (_IterIndex == _ArraySize)
    ++(*this);

  HashableClass<KEY, VALUE>* RetVal_ = _PostIncValid ? _ArrPtr->_Table[_IterIndex]:NULL;

  if (RetVal_)
    ++(*this);

  return RetVal_;
}

//****************************************************************************
//****************************************************************************
#if DEBUG_HASHTABLE
int main()
{
  HashTable<ChrString, ChrString> HashTab_(1000, ChrString(""), HashTableScheme::DOUBLEHASH);

  ChrString Str1Key_ = "Fred-Flintstone";
  ChrString Str1Val_ = "488-0093";
  ChrString Str2Key_ = "Winnie-Pooh";
  ChrString Str2Val_ = "255-0162";

  HashTab_["Fred-Flintstone"] = ChrString("488-0093");
  cout <<"key = " <<"Fred-Flintstone" <<endl;
  cout <<"data = " <<HashTab_["Fred-Flintstone"] <<endl;
  cout <<"found = " <<HashTab_.IsFound() <<endl;  

  HashTab_["Winnie-Pooh"] = ChrString("255-0162");
  cout <<"key = " <<"Winnie-Pooh" <<endl;
  cout <<"data = " <<HashTab_["Winnie-Pooh"] <<endl;
  cout <<"found = " <<HashTab_.IsFound() <<endl;

  HashTab_["Fred-Flintstone"].Erase();
  cout <<"Fred-Flintstone Removed" <<endl;
  cout <<"data = " <<HashTab_["Fred-Flintstone"] <<endl;
  cout <<"found = " <<HashTab_.IsFound() <<endl;

  return 0;
}
#endif
//****************************************************************************
#endif





