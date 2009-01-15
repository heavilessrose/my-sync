#ifndef ASSOCIATION_CPP
#define ASSOCIATION_CPP
#ifndef ASSOCIATION_H
  #include "assoc.h"
#endif

//************************* Association procedures ******************************
//****************************************************************************
template <class KEY, class VALUE>
Association<KEY, VALUE>::Association(int Flags_):
_ValueStoreAttr(Flags_)
{}

//****************************************************************************
template <class KEY, class VALUE>
Association<KEY, VALUE>::Association(const Association<KEY, VALUE>& hb):
HashableClass<KEY, VALUE>(hb),
_Value(hb.GetValueStorePtr()),
_ValueStoreAttr(hb.GetValueStorageAttributes())
{}

//****************************************************************************
template <class KEY, class VALUE>
Association<KEY, VALUE>::~Association()
{}

//****************************************************************************
template <class KEY, class VALUE>
Object& Association<KEY, VALUE>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  AssociationAcceptor<KEY, VALUE> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromAssociation(TrgComp_)));
}

//****************************************************************************
template <class KEY, class VALUE>
Object* Association<KEY, VALUE>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Association<KEY, VALUE>(*this));

  AssociationAcceptor<KEY, VALUE> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromAssociation(TrgComp_)));
}

/****************************************************************************/
template <class KEY, class VALUE>
Association<KEY, VALUE>& Association<KEY, VALUE>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Association<KEY, VALUE> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class KEY, class VALUE>
const Object& Association<KEY, VALUE>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Association<KEY, VALUE>::GiveNullObject();
}

/****************************************************************************/
template <class KEY, class VALUE>
Object& Association<KEY, VALUE>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Association<KEY, VALUE>::GiveNullObject();
}

//****************************************************************************
template <class KEY, class VALUE>
int Association<KEY, VALUE>::ClassID() const
{
  return TypeInfo::ASSOCIATION_TYPE;
}

//****************************************************************************
template <class KEY, class VALUE>
const ObjectAcceptor* Association<KEY, VALUE>::GiveObjectAcceptor() const
{
  return (new AssociationAcceptor<KEY, VALUE>(this));
}

//****************************************************************************
template <class KEY, class VALUE>
Boolean Association<KEY, VALUE>::IsEqualValue(const Association<KEY, VALUE>* hb) const
{
  return
  (
    hb ?
    (
      (_Value.Pointee() && hb->_Value.Pointee()) ?
        (*_Value == *hb->_Value):
        (_Value.IsNull() && hb->_Value.IsNull())
    ):
    FALSE
  );
}

//****************************************************************************
template <class KEY, class VALUE>
Boolean Association<KEY, VALUE>::IsEqualToAssociation(const Association<KEY, VALUE>* hb) const
{
  return (HashableClass<KEY, VALUE>::IsEqualToHashable(hb) && IsEqualValue(hb));
}

//****************************************************************************
template <class KEY, class VALUE>
Boolean Association<KEY, VALUE>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  AssociationAcceptor<KEY, VALUE> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToAssociation(TrgComp_);
}

//****************************************************************************
template <class KEY, class VALUE>
const DataStorePtr<VALUE>& Association<KEY, VALUE>::GetValueStorePtr() const
{
  return _Value;
}

//****************************************************************************
template <class KEY, class VALUE>
Association<KEY, VALUE>& Association<KEY, VALUE>::operator = (const Association<KEY, VALUE>& hb)
{
  if (&hb != this)
  {
    HashableClass<KEY, VALUE>::operator = (hb);
    SetValue((VALUE*)hb.GetValue());
  }
  
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
Association<KEY, VALUE>& Association<KEY, VALUE>::SetData(VALUE* ptr)
{
  _Value.AssignPtr(ptr, GetValueStorageAttributes().GetStorageAttributes());
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
const VALUE* Association<KEY, VALUE>::GetData() const
{
  return _Value.Pointee();
}

/****************************************************************************/
template <class KEY, class VALUE>
VALUE* Association<KEY, VALUE>::GetData()
{
  return _Value.Pointee();
}

/****************************************************************************/
template <class KEY, class VALUE>        
Association<KEY, VALUE>& Association<KEY, VALUE>::SetValue(VALUE* ptr)
{
  SetData(ptr);
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>    
const VALUE* Association<KEY, VALUE>::GetValue() const
{
  return _Value.Pointee();
}

/****************************************************************************/
template <class KEY, class VALUE>    
VALUE* Association<KEY, VALUE>::GetValue()
{
  return _Value.Pointee();
}

/****************************************************************************/
template <class KEY, class VALUE>
Association<KEY, VALUE>& Association<KEY, VALUE>::SetPair(KEY* Key_, VALUE* Value_)
{
  SetKey(Key_);
  SetValue(Value_);
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
void Association<KEY, VALUE>::DestroyData()
{
  _Value.Destroy();
}

/****************************************************************************/
template <class KEY, class VALUE>    
void Association<KEY, VALUE>::DestroyValue()
{
  DestroyData();
}

/****************************************************************************/
template <class KEY, class VALUE>    
void Association<KEY, VALUE>::DestroyPair()
{
  DestroyKey();
  DestroyValue();  
}

/****************************************************************************/
template <class KEY, class VALUE>
Association<KEY, VALUE>& Association<KEY, VALUE>::SetValueStorageAttributes(int Flags_)
{
  _ValueStoreAttr.SetStorageAttributes(Flags_);
  return *this;
}

/****************************************************************************/
template <class KEY, class VALUE>
const DataStorageAttributes& Association<KEY, VALUE>::GetValueStorageAttributes() const
{
  return _ValueStoreAttr;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean Association<KEY, VALUE>::IsAssociation() const
{
  return TRUE;
}

//****************************************************************************
#if OVERLOAD_NEW
template <class KEY, class VALUE>
void* Association<KEY, VALUE>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//****************************************************************************
template <class KEY, class VALUE>
void Association<KEY, VALUE>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//****************************************************************************
#if	HAS_ARRAY_NEW
template <class KEY, class VALUE>
void* Association<KEY, VALUE>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
//****************************************************************************
#if	HAS_ARRAY_NEW
template <class KEY, class VALUE>
void Association<KEY, VALUE>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
AssociationAcceptor<KEY, VALUE>::AssociationAcceptor(const Association<KEY, VALUE>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean AssociationAcceptor<KEY, VALUE>::IsAssociation() const
{
  return TRUE;
}

/****************************************************************************/
template <class KEY, class VALUE>
void AssociationAcceptor<KEY, VALUE>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Association<KEY, VALUE>*)Ptr_;
}

/****************************************************************************/
template <class KEY, class VALUE>
const void* AssociationAcceptor<KEY, VALUE>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* AssociationAcceptor<KEY, VALUE>::CreateFromAssociation(const ObjectAcceptor& Obj_)
{
  if (!(_ObjPtr->GetKey() &&
        _ObjPtr->GetValue() &&
        Obj_.IsEqvAssociation(WhatIs(*_ObjPtr->GetKey()), sizeof(KEY),
                              WhatIs(*_ObjPtr->GetValue()), sizeof(VALUE))))
    THROW (FallibleBase::IncompatibleTypeErr());

  _VarPtr = new Association<KEY, VALUE>(*((const Association<KEY, VALUE>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* AssociationAcceptor<KEY, VALUE>::AssignFromAssociation(const ObjectAcceptor& Obj_)
{
  if (!(_ObjPtr->GetKey() &&
        _ObjPtr->GetValue() &&
        Obj_.IsEqvAssociation(WhatIs(*_ObjPtr->GetKey()), sizeof(KEY),
                              WhatIs(*_ObjPtr->GetValue()), sizeof(VALUE))))
    THROW (FallibleBase::IncompatibleTypeErr());

  const Association<KEY, VALUE>* TrgPtr_ = (const Association<KEY, VALUE>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean AssociationAcceptor<KEY, VALUE>::IsEqvAssociation(int KeyID_, size_t KeySize_,
                                                          int ValID_, size_t ValSize_) const
{
  return
  (
    KeyID_ == WhatIs(*_ObjPtr->GetKey()) &&
    KeySize_ == sizeof(KEY) &&
    ValID_ == WhatIs(*_ObjPtr->GetValue()) &&
    ValSize_ == sizeof(VALUE)
  );
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean AssociationAcceptor<KEY, VALUE>::IsEqualToAssociation(const ObjectAcceptor& Obj_) const
{
  if (!(_ObjPtr->GetKey() &&
        _ObjPtr->GetValue() &&
        Obj_.IsEqvAssociation(WhatIs(*_ObjPtr->GetKey()), sizeof(KEY),
                              WhatIs(*_ObjPtr->GetValue()), sizeof(VALUE))))
    THROW (FallibleBase::IncompatibleTypeErr());

  return _ObjPtr->IsEqualToAssociation((const Association<KEY, VALUE>*)Obj_.GiveDataPtr());
}

//****************************************************************************
#if OVERLOAD_NEW
template <class KEY, class VALUE>
void* AssociationAcceptor<KEY, VALUE>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//****************************************************************************
template <class KEY, class VALUE>
void AssociationAcceptor<KEY, VALUE>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
//****************************************************************************
//****************************************************************************
#if ASSOCIATION_DEBUG
int main()
{
  const Ulong tabsize = 2001;

  ChrString Key_("Hello World!");
  ChrString Data_("Guard");
  
  Association<ChrString, ChrString> Node_;

  Node_.SetData(&Data_);
  Node_.SetKey(&Key_);
  
  if (Node_.GetData() && Node_.GetKey())
    cout <<"Key = " <<*Node_.GetKey()
         <<", Value = " <<*Node_.GetData() <<endl;

  Node_.MakeIndex(tabsize);
  cout <<"Index = " <<Node_.GetIndex() <<endl;
  Node_.ProbeAhead(tabsize);
  cout <<"Index = " <<Node_.GetIndex()
       <<", Incr = " <<Node_.GetProbeIncr() <<endl;

  Association<ChrString, ChrString> Node1_;

  ChrString* Key3_ = ::Clone(Key_);
  ChrString Key4_ = *Key3_;
  Node1_.SetData(&Data_);
  Node1_.SetKey(Key3_);
  ::Delete(Key3_);

  Association<ChrString, ChrString>* NodePtr_ = (Association<ChrString, ChrString>*)Node1_.Clone();
  NodePtr_->MakeIndex(tabsize);
  cout <<"Index = " <<NodePtr_->GetIndex() <<endl;
  delete NodePtr_;

  int val = 115;
  ChrString Key2_ = ("This Is A Test Of The Association Class");
  Association<ChrString, int> hash1;

  hash1.SetData(&val);
  hash1.SetKey(&Key2_);
  
  if (hash1.GetData() && hash1.GetKey())
    cout <<"Key = " <<*hash1.GetKey()
         <<", Value = " <<*hash1.GetData() <<endl;

  hash1.MakeIndex(tabsize);
  cout <<"Index = " <<hash1.GetIndex() <<endl;
  hash1.ProbeAhead(tabsize);
  cout <<"Index = " <<hash1.GetIndex()
       <<", Incr = " <<hash1.GetProbeIncr() <<endl;

  return 0;
}
#endif
//****************************************************************************
#endif





