#ifndef TYPEDSMARTHND_CPP
#define TYPEDSMARTHND_CPP
#ifndef TYPEDSMARTHND_H
  #include "typedhnd.h"
#endif

/****************************************************************************/
template <class T>
TypedObjectDeleter<T>::TypedObjectDeleter():
_Next(NULL)
{}

/****************************************************************************/
// Copy constructor for object deleter. All bindings are cloned
//
template <class T>
TypedObjectDeleter<T>::TypedObjectDeleter(const TypedObjectDeleter<T>& Obj_):
_Next(Obj_._Next ? Obj_._Next->Clone():NULL)
{}

/****************************************************************************/
// Destructor for object deleter. Binding list is recursively deleted.
//
template <class T>
TypedObjectDeleter<T>::~TypedObjectDeleter()
{
  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
template <class T>
TypedObjectDeleter<T>* TypedObjectDeleter<T>::Clone() const
{
  return (new TypedObjectDeleter<T>(*this));
}

/****************************************************************************/
// Method to add the next binding object into the binding list
//
// PRE:
//   ObjectUpdater* Next_ : The binding object to be added
//   Boolean* Done_       : Indicates whether the add operation succeeded
//
template <class T>
ObjectUpdater* TypedObjectDeleter<T>::AddNext(ObjectUpdater* Next_, Boolean* Done_)
{
  Boolean Dummy_;
  if (Done_ == NULL)
    Done_ = &Dummy_;

  if (_Next)
    _Next->AddNext(Next_, Done_);
  else if (Next_ && Next_->IsDeleter())
  {
    _Next = Next_;
    *Done_ = TRUE;
  }
  else
    *Done_ = FALSE;

  return this;
}

/****************************************************************************/
template <class T>
ObjectUpdater* TypedObjectDeleter<T>::TransferBasePtr(ObjectUpdater* Ptr_)
{
  return this;
}

/****************************************************************************/
// Method to do the actual update on the client data when the sponsor is
// notified. In this case the client data is deleted.
//
// PRE:
//   VOIDPTR Obj_         : void pointer to the client data.
//   Boolean HaltOnFalse_ : indicates whether to halt updates on false return
//
template <class T>
Boolean TypedObjectDeleter<T>::DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_)
{
  Boolean RetCode_ = TypedErase((T*)Obj_);  

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);

  return RetCode_;
}

/****************************************************************************/
template <class T>
Boolean TypedObjectDeleter<T>::DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const
{
  Boolean RetCode_ = TypedErase((T*)Obj_);

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);
      
  return RetCode_;
}

/****************************************************************************/
template <class T>
void TypedObjectDeleter<T>::SetNextPtr(ObjectUpdater* Ptr_)
{
  if (!Ptr_ || Ptr_->IsDeleter())
    _Next = Ptr_;
}

/****************************************************************************/
template <class T>
ObjectUpdater* TypedObjectDeleter<T>::GetNextPtr()
{
  return _Next;
}

/****************************************************************************/
template <class T>
Boolean TypedObjectDeleter<T>::TypedErase(T* Obj_) const
{
  return ::Delete(Obj_);	// Smart delete function
}

/****************************************************************************/
template <class T>
Boolean TypedObjectDeleter<T>::IsUpdater()
{
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean TypedObjectDeleter<T>::IsDeleter()
{
  return TRUE;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* TypedObjectDeleter<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void TypedObjectDeleter<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* TypedObjectDeleter<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void TypedObjectDeleter<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>::TypedObjectUpdater():
_BasePtr(NULL),
_Bindings(),
_Type(BindingType::NOBINDINGS),
_NotificationCode(0),
_Next(NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>::TypedObjectUpdater(const TypedObjectUpdater<CLIENT, T>& Obj_):
_BasePtr(NULL),
_Bindings(Obj_._Bindings, Obj_._Type),
_Type(Obj_._Type),
_NotificationCode(Obj_._NotificationCode),
_Next(Obj_._Next ? Obj_._Next->Clone():NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>::TypedObjectUpdater(TypedObjectUpdater<CLIENT, T>* Ptr_):
_BasePtr(Ptr_->_BasePtr),
_Bindings(),
_Type(BindingType::NOBINDINGS),
_NotificationCode(0),
_Next(Ptr_->_Next ? Ptr_->_Next->Clone():NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>::TypedObjectUpdater(CLIENT* Base_, TYPENAME ClassBinding0<CLIENT>::Type Method_):
_BasePtr(Base_),
_Bindings(Method_),
_Type(BindingType::BINDING0),
_NotificationCode(0),
_Next(NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>::TypedObjectUpdater(CLIENT* Base_, TYPENAME ClassBinding1<CLIENT, long>::Type Method_, long IdCode_):
_BasePtr(Base_),
_Bindings(Method_),
_Type(BindingType::BINDING1),
_NotificationCode(IdCode_),
_Next(NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>::TypedObjectUpdater(CLIENT* Base_, TYPENAME ClassBinding2<CLIENT, T>::Type Method_, long IdCode_):
_BasePtr(Base_),
_Bindings(Method_),
_Type(BindingType::BINDING2),
_NotificationCode(IdCode_),
_Next(NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>::~TypedObjectUpdater()
{
  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
// Method to transfer the base client pointer to the specified ObjectUpdater
// object.
//
// PRE:
//   ObjectUpdater* Ptr_ : The pointer to the ObjectUpdater object
//
template <class CLIENT, class T>
ObjectUpdater* TypedObjectUpdater<CLIENT, T>::TransferBasePtr(ObjectUpdater* Ptr_)
{
  // Get the head of the object updater binding list
  ObjectUpdater* Head_ = Ptr_;

  // Cast to a typed object updater pointer and set the base pointer to this
  // object's base pointer
  if (Ptr_ != this)
    while (Ptr_ && Ptr_->IsUpdater() && !Ptr_->IsDeleter())
    {
      TypedObjectUpdater<CLIENT, T>* Cptr_ = (TypedObjectUpdater<CLIENT, T>*)Ptr_;
      Cptr_->SetBasePtr(_BasePtr);
      Ptr_ = Cptr_->_Next;
    }

  return Head_;
}

/****************************************************************************/
template <class CLIENT, class T>
void TypedObjectUpdater<CLIENT, T>::SetBinding(TYPENAME ClassBinding0<CLIENT>::Type Method_)
{
  _Bindings._Bind0.Ptr = Method_;
  _Type = BindingType::BINDING0;
}

/****************************************************************************/
template <class CLIENT, class T>
void TypedObjectUpdater<CLIENT, T>::SetBinding(TYPENAME ClassBinding1<CLIENT, long>::Type Method_, long IdCode_)
{
  _Bindings._Bind1.Ptr = Method_;
  _NotificationCode = IdCode_;  
  _Type = BindingType::BINDING1;
}

/****************************************************************************/
template <class CLIENT, class T>
void TypedObjectUpdater<CLIENT, T>::SetBinding(TYPENAME ClassBinding2<CLIENT, T>::Type Method_, long IdCode_)
{
  _Bindings._Bind2.Ptr = Method_;
  _NotificationCode = IdCode_;
  _Type = BindingType::BINDING2;
}

/****************************************************************************/
// Method to set the client base pointer to the specified pointer.
//
template <class CLIENT, class T>
void TypedObjectUpdater<CLIENT, T>::SetBasePtr(CLIENT* Base_)
{
  _BasePtr = Base_;
}

/****************************************************************************/
template <class CLIENT, class T>
CLIENT* TypedObjectUpdater<CLIENT, T>::GetBasePtr()
{
  return _BasePtr;
}

/****************************************************************************/
template <class CLIENT, class T>
const CLIENT* TypedObjectUpdater<CLIENT, T>::GetBasePtr() const
{
  return _BasePtr;
}

/****************************************************************************/
template <class CLIENT, class T>
TypedObjectUpdater<CLIENT, T>* TypedObjectUpdater<CLIENT, T>::Clone() const
{
  return (new TypedObjectUpdater<CLIENT, T>(*this));
}

/****************************************************************************/
// Method to add the next binding object into the binding list
//
// PRE:
//   ObjectUpdater* Next_ : The binding object to be added
//   Boolean* Done_       : Indicates whether the add operation succeeded
//
template <class CLIENT, class T>
ObjectUpdater* TypedObjectUpdater<CLIENT, T>::AddNext(ObjectUpdater* Next_, Boolean* Done_)
{
  Boolean Dummy_;
  if (Done_ == NULL)
    Done_ = &Dummy_;

  if (_Next)
    _Next->AddNext(Next_, Done_);
  else if (Next_ && Next_->IsUpdater() && !Next_->IsDeleter())
  {
    _Next = Next_;
    *Done_ = TRUE;
  }
  else
    *Done_ = FALSE;

  return this;
}

/****************************************************************************/
// Method to do the actual update on the client data when the sponsor is
// notified. In this case the base client pointer stored within this object
// is used to execute bindings of the client object.
//
// PRE:
//   VOIDPTR Obj_         : void pointer to the client data.
//   Boolean HaltOnFalse_ : indicates whether to halt updates on false return
//
template <class CLIENT, class T>
Boolean TypedObjectUpdater<CLIENT, T>::DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_)
{
  Boolean RetCode_ = TypedUpdate((T*)Obj_);

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);

  return RetCode_;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean TypedObjectUpdater<CLIENT, T>::DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const
{
  Boolean RetCode_ = TypedUpdate((T*)Obj_);

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);

  return RetCode_;
}

/****************************************************************************/
template <class CLIENT, class T>
void TypedObjectUpdater<CLIENT, T>::SetNextPtr(ObjectUpdater* Ptr_)
{
  if (!Ptr_ || (Ptr_->IsUpdater() && !Ptr_->IsDeleter()))
    _Next = Ptr_;
}

/****************************************************************************/
template <class CLIENT, class T>
ObjectUpdater* TypedObjectUpdater<CLIENT, T>::GetNextPtr()
{
  return _Next;
}

/****************************************************************************/
// Method to do the actual execution of bindings. The base pointer of this
// object is used to execute bindings stored in a bindings union object.
//
// PRE:
//   const T* Obj_ : Object to be passed to the binding if necessary
//
template <class CLIENT, class T>
Boolean TypedObjectUpdater<CLIENT, T>::TypedUpdate(const T* Obj_) const
{
  long Code_ = _NotificationCode;
  if (!Code_ && Obj_)
    Code_ = CountOf(*Obj_);

  // Determine what type of binding is assigned then execute it through
  // the client's pointer: _BasePtr
  if (_BasePtr && _Type != BindingType::NOBINDINGS)  
    return
    (
      (_Type == BindingType::BINDING0) ?
             (_BasePtr->*_Bindings._Bind0.Ptr)():
      (_Type == BindingType::BINDING1) ?
             (_BasePtr->*_Bindings._Bind1.Ptr)(Code_):
      (_Type == BindingType::BINDING2 && Obj_) ?
             (_BasePtr->*_Bindings._Bind2.Ptr)(*Obj_, Code_):
             FALSE
    );

  return FALSE;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean TypedObjectUpdater<CLIENT, T>::IsUpdater()
{
  return TRUE;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean TypedObjectUpdater<CLIENT, T>::IsDeleter()
{
  return FALSE;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class T>
void* TypedObjectUpdater<CLIENT, T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class T>
void TypedObjectUpdater<CLIENT, T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class T>
void* TypedObjectUpdater<CLIENT, T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class T>
void TypedObjectUpdater<CLIENT, T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class T>
BuiltInUpdater<CLIENT, T>::BuiltInUpdater():
_BasePtr(NULL),
_Next(NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
BuiltInUpdater<CLIENT, T>::BuiltInUpdater(const BuiltInUpdater<CLIENT, T>& Obj_):
_BasePtr(NULL),
_Next(Obj_._Next ? Obj_._Next->Clone():NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
BuiltInUpdater<CLIENT, T>::BuiltInUpdater(BuiltInUpdater<CLIENT, T>* Ptr_):
_BasePtr(Ptr_->_BasePtr),
_Next(Ptr_->_Next ? Ptr_->_Next->Clone():NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
BuiltInUpdater<CLIENT, T>::BuiltInUpdater(CLIENT* Base_):
_BasePtr(Base_),
_Next(NULL)
{}

/****************************************************************************/
template <class CLIENT, class T>
BuiltInUpdater<CLIENT, T>::~BuiltInUpdater()
{
  delete _Next;
  _Next = NULL;
}

/****************************************************************************/
template <class CLIENT, class T>
BuiltInUpdater<CLIENT, T>* BuiltInUpdater<CLIENT, T>::Clone() const
{
  return (new BuiltInUpdater<CLIENT, T>(*this));
}

/****************************************************************************/
template <class CLIENT, class T>
ObjectUpdater* BuiltInUpdater<CLIENT, T>::AddNext(ObjectUpdater* Next_, Boolean* Done_)
{
  Boolean Dummy_;
  if (Done_ == NULL)
    Done_ = &Dummy_;

  if (_Next)
    _Next->AddNext(Next_, Done_);
  else if (Next_ && Next_->IsUpdater() && !Next_->IsDeleter())
  {
    _Next = Next_;
    *Done_ = TRUE;
  }
  else
    *Done_ = FALSE;

  return this;
}

/****************************************************************************/
template <class CLIENT, class T>
ObjectUpdater* BuiltInUpdater<CLIENT, T>::TransferBasePtr(ObjectUpdater* Ptr_)
{
  ObjectUpdater* Head_ = Ptr_;

  if (Ptr_ != this)
    while (Ptr_ && Ptr_->IsUpdater() && !Ptr_->IsDeleter())
    {
      BuiltInUpdater<CLIENT, T>* Cptr_ = (BuiltInUpdater<CLIENT, T>*)Ptr_;
      Cptr_->SetBasePtr(_BasePtr);
      Ptr_ = Cptr_->_Next;
    }

  return Head_;
}

/****************************************************************************/
template <class CLIENT, class T>
void BuiltInUpdater<CLIENT, T>::SetBasePtr(CLIENT* Base_)
{
  _BasePtr = Base_;
}

/****************************************************************************/
template <class CLIENT, class T>
CLIENT* BuiltInUpdater<CLIENT, T>::GetBasePtr()
{
  return _BasePtr;
}

/****************************************************************************/
template <class CLIENT, class T>
const CLIENT* BuiltInUpdater<CLIENT, T>::GetBasePtr() const
{
  return _BasePtr;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean BuiltInUpdater<CLIENT, T>::DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_)
{
  Boolean RetCode_ = TypedUpdate((T*)Obj_);

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);

  return RetCode_;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean BuiltInUpdater<CLIENT, T>::DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const
{
  Boolean RetCode_ = TypedUpdate((T*)Obj_);

  if (_Next)
    if (HaltOnFalse_ || RetCode_)
      RetCode_ = RetCode_ && _Next->DoUpdate(Obj_, HaltOnFalse_);
    else
      _Next->DoUpdate(Obj_, HaltOnFalse_);

  return RetCode_;
}

/****************************************************************************/
template <class CLIENT, class T>
void BuiltInUpdater<CLIENT, T>::SetNextPtr(ObjectUpdater* Ptr_)
{
  if (!Ptr_ || (Ptr_->IsUpdater() && !Ptr_->IsDeleter()))
    _Next = Ptr_;
}

/****************************************************************************/
template <class CLIENT, class T>
ObjectUpdater* BuiltInUpdater<CLIENT, T>::GetNextPtr()
{
  return _Next;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean BuiltInUpdater<CLIENT, T>::TypedUpdate(const T* Obj_) const
{
  if (_BasePtr && Obj_)
  {
    *_BasePtr = *Obj_;
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean BuiltInUpdater<CLIENT, T>::IsUpdater()
{
  return TRUE;
}

/****************************************************************************/
template <class CLIENT, class T>
Boolean BuiltInUpdater<CLIENT, T>::IsDeleter()
{
  return FALSE;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class CLIENT, class T>
void* BuiltInUpdater<CLIENT, T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class CLIENT, class T>
void BuiltInUpdater<CLIENT, T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class T>
void* BuiltInUpdater<CLIENT, T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class CLIENT, class T>
void BuiltInUpdater<CLIENT, T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif





