#ifndef SMARTHND_CPP
#define SMARTHND_CPP
#ifndef SMARTHND_H
  #include "smarthnd.h"
#endif

/****************************************************************************/
/****************************************************************************/
// Class ID method returns the class ID for the ObjectUpdate class
//
int ObjectUpdater::ClassID() const
{
  return TypeInfo::OBJECTUPDATER_TYPE;
}

/****************************************************************************/
// Object destruction method
//
Boolean ObjectUpdater::Destroy()
{
  delete this;
  return TRUE;
}

/****************************************************************************/
Boolean ObjectUpdater::IsEqual(const ObjectUpdater& Obj_) const
{
  return (this == &Obj_);
}

/****************************************************************************/
/****************************************************************************/
// Update bindings list constructor. The bindings list contains pointers to
// objects of type ObjectUpdater.
//
// PRE:
//   ObjectUpdater* _Head : The head of the bindings list
//   Boolean _HaltOnFalse : Halt execution of bindings on first occurence of
//                          a false return value by a binding
//
UpdaterBindingList::UpdaterBindingList():
_Head(NULL),
_HaltOnFalse(FALSE)
{}

/****************************************************************************/
// Constructor accepting the head of a updater binding list. Updater binding
// lists can also be pointer copied without any danger since they are
// reference counted objects. Shared pointers to binding lists will only
// be destroyed if the reference count determines an unique object.
//
UpdaterBindingList::UpdaterBindingList(ObjectUpdater* Ptr_):
_Head(Ptr_),
_HaltOnFalse(FALSE)
{}

/****************************************************************************/
UpdaterBindingList::UpdaterBindingList(const UpdaterBindingList& Obj_):
_Head(Obj_._Head),
_HaltOnFalse(FALSE),
refCount(Obj_.refCount)
{}

/****************************************************************************/
// Updater binding list destructor. Deleting the head of the list will
// recursively destroy all binding objects within the list.
//
UpdaterBindingList::~UpdaterBindingList()
{
  // If it is a unique pointer to an ObjectUpdater object.
  if (Unique())
    delete _Head;
    
  _Head = NULL;
}

/****************************************************************************/
UpdaterBindingList& UpdaterBindingList::operator = (ObjectUpdater* Ptr_)
{
  if (_Head != Ptr_)
  {
    if (Unique())
      delete _Head;

    _Head = Ptr_;
    refCount = ReferenceCount();
  }

  return *this;
}

/****************************************************************************/
UpdaterBindingList& UpdaterBindingList::operator = (const UpdaterBindingList& Obj_)
{
  if (_Head != Obj_._Head)
  {
    if (Unique())
      delete _Head;

    _Head = Obj_._Head;
    refCount = Obj_.GiveRefCount();
    _HaltOnFalse = Obj_._HaltOnFalse;    
  }

  return *this;
}

/****************************************************************************/
Boolean UpdaterBindingList::Unique() const
{
  return refCount.Unique();
}

/****************************************************************************/
void UpdaterBindingList::SetToNull()
{
  _Head = NULL;
  refCount.Decrement();
}

/****************************************************************************/    
const ReferenceCount& UpdaterBindingList::GiveRefCount() const
{
  return refCount;
}

/****************************************************************************/
// Method to add the next binding object into the list.
//
// PRE:
//   ObjectUpdater* Next_ : The next object updater object to be added
//   Boolean* Done_ : Indicates whether the add operation is completed.
//
UpdaterBindingList* UpdaterBindingList::AddNext(ObjectUpdater* Next_, Boolean* Done_)
{
  if (_Head)
    _Head->AddNext(Next_, Done_);
  else
    _Head = Next_;
    
  return this;
}

/****************************************************************************/
// Method to transfer the base object pointer of the each binding to the
// specified pointer.
//
UpdaterBindingList* UpdaterBindingList::TransferBasePtr(ObjectUpdater* Ptr_)
{
  if (_Head)
    _Head->TransferBasePtr(Ptr_);

  return this;
}

/****************************************************************************/
// Binding object removal helper function
//
Boolean UpdaterBindingList::HelpRemove(ObjectUpdater* Prev_,
                                          ObjectUpdater* Cur_,
                                          ObjectUpdater* Ptr_)
{
  if (!Cur_)
    return FALSE;

  if (Cur_ == Ptr_)
  {
    SetNext(Prev_, GetNext(Cur_));
    SetNext(Cur_, NULL);
    delete Cur_;
    return TRUE;
  }
  else
    HelpRemove(Cur_, GetNext(Cur_), Ptr_);

  return FALSE;
}

/****************************************************************************/
// Remove the binding object specified by Ptr_ from the bindings list
//
UpdaterBindingList* UpdaterBindingList::Remove(ObjectUpdater* Ptr_, Boolean* Done_)
{
  Boolean Dummy_;
  if (Done_ == NULL)
    Done_ = &Dummy_;

  if (!Ptr_ || !_Head)
  {
    *Done_ = FALSE;
    return this;
  }    

  ObjectUpdater* RetPtr_;
    
  if (_Head == Ptr_)
  {
    RetPtr_ = GetNext(_Head);
    SetNext(_Head, NULL);
    delete _Head;    
    _Head = RetPtr_;
    
    *Done_ = TRUE;
  }
  else
    *Done_ = HelpRemove(_Head, GetNext(_Head), Ptr_);

  return this;
}

/****************************************************************************/
Boolean UpdaterBindingList::HelpIncludes(ObjectUpdater* SrchPtr_, ObjectUpdater* Ptr_)
{
  if (SrchPtr_ != Ptr_ && SrchPtr_)
    return HelpIncludes(GetNext(SrchPtr_), Ptr_);

  return (SrchPtr_ != NULL);
}

/****************************************************************************/
// Method returns whether or not the specified binding object is a member of
// the current bindings list
//
Boolean UpdaterBindingList::Includes(ObjectUpdater* Ptr_)
{
  return ((Ptr_ && _Head) ? HelpIncludes(_Head, Ptr_):FALSE);
}

/****************************************************************************/
UpdaterBindingList* UpdaterBindingList::HaltOnFirstFalse(Boolean Flag_)
{
  _HaltOnFalse = Flag_;
  return this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* UpdaterBindingList::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void UpdaterBindingList::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* UpdaterBindingList::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void UpdaterBindingList::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
DeleterBindingList::DeleterBindingList():
_Head(NULL),
_HaltOnFalse(FALSE)
{}

/****************************************************************************/
DeleterBindingList::DeleterBindingList(ObjectUpdater* Ptr_):
_Head(Ptr_),
_HaltOnFalse(FALSE)
{}

/****************************************************************************/
DeleterBindingList::DeleterBindingList(const DeleterBindingList& Obj_):
_Head(Obj_._Head),
_HaltOnFalse(FALSE),
refCount(Obj_.refCount)
{}

/****************************************************************************/
DeleterBindingList::~DeleterBindingList()
{
  if (Unique())
    delete _Head;
    
  _Head = NULL;
}

/****************************************************************************/
DeleterBindingList& DeleterBindingList::operator = (ObjectUpdater* Ptr_)
{
  if (_Head != Ptr_)
  {
    if (Unique())
      delete _Head;

    _Head = Ptr_;
    refCount = ReferenceCount();
  }

  return *this;
}

/****************************************************************************/
DeleterBindingList& DeleterBindingList::operator = (const DeleterBindingList& Obj_)
{
  if (_Head != Obj_._Head)
  {
    if (Unique())
      delete _Head;

    _Head = Obj_._Head;
    refCount = Obj_.GiveRefCount();
    _HaltOnFalse = Obj_._HaltOnFalse;    
  }

  return *this;
}

/****************************************************************************/
Boolean DeleterBindingList::Unique() const
{
  return refCount.Unique();
}

/****************************************************************************/
void DeleterBindingList::SetToNull()
{
  _Head = NULL;
  refCount.Decrement();
}

/****************************************************************************/    
const ReferenceCount& DeleterBindingList::GiveRefCount() const
{
  return refCount;
}

/****************************************************************************/
DeleterBindingList* DeleterBindingList::AddNext(ObjectUpdater* Next_, Boolean* Done_)
{
  if (_Head)
    _Head->AddNext(Next_, Done_);
  else    
    _Head = Next_;
    
  return this;
}

/****************************************************************************/        
DeleterBindingList* DeleterBindingList::TransferBasePtr(ObjectUpdater* Ptr_)
{
  if (_Head)
    _Head->TransferBasePtr(Ptr_);

  return this;
}

/****************************************************************************/
Boolean DeleterBindingList::HelpRemove(ObjectUpdater* Prev_,
                                          ObjectUpdater* Cur_,
                                          ObjectUpdater* Ptr_)
{
  if (!Cur_)
    return FALSE;

  if (Cur_ == Ptr_)
  {
    SetNext(Prev_, GetNext(Cur_));
    SetNext(Cur_, NULL);
    delete Cur_;
    return TRUE;
  }
  else
    HelpRemove(Cur_, GetNext(Cur_), Ptr_);

  return FALSE;
}

/****************************************************************************/
DeleterBindingList* DeleterBindingList::Remove(ObjectUpdater* Ptr_, Boolean* Done_)
{
  Boolean Dummy_;
  if (Done_ == NULL)
    Done_ = &Dummy_;

  if (!Ptr_ || !_Head)
  {
    *Done_ = FALSE;
    return this;
  }    

  ObjectUpdater* RetPtr_;
    
  if (_Head == Ptr_)
  {
    RetPtr_ = GetNext(_Head);
    SetNext(_Head, NULL);
    delete _Head;    
    _Head = RetPtr_;
    
    *Done_ = TRUE;        
  }
  else
    *Done_ = HelpRemove(_Head, GetNext(_Head), Ptr_);

  return this;
}

/****************************************************************************/
Boolean DeleterBindingList::HelpIncludes(ObjectUpdater* SrchPtr_, ObjectUpdater* Ptr_)
{
  if (SrchPtr_ != Ptr_ && SrchPtr_)
    return HelpIncludes(GetNext(SrchPtr_), Ptr_);

  return (SrchPtr_ != NULL);
}

/****************************************************************************/
Boolean DeleterBindingList::Includes(ObjectUpdater* Ptr_)
{
  return ((Ptr_ && _Head) ? HelpIncludes(_Head, Ptr_):FALSE);
}

/****************************************************************************/
DeleterBindingList* DeleterBindingList::HaltOnFirstFalse(Boolean Flag_)
{
  _HaltOnFalse = Flag_;
  return this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* DeleterBindingList::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void DeleterBindingList::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* DeleterBindingList::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void DeleterBindingList::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif




