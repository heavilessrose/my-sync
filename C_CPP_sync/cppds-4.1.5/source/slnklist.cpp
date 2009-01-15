#ifndef SINGLELIST_CPP
#define SINGLELIST_CPP
#ifndef SINGLELIST_H
  #include "slnklist.h"
#endif

/****************************************************************************/
template <class T>
int(*SingleList<T>::_CompFunc)(const T&, const T&) = NULL;

/****************************************************************************/
/****************************************************************************/
template <class T>
SingleListNode<T>::SingleListNode():
_Next(NULL)
{}

/****************************************************************************/
template <class T>
SingleListNode<T>::SingleListNode(const T& Data_):
_Data(Data_),
_Next(NULL)
{}

/****************************************************************************/
template <class T>
SingleListNode<T>::SingleListNode(T* Data_, int StoreFlags_):
_Data(Data_, StoreFlags_),
_Next(NULL)
{}

/****************************************************************************/
template <class T>
SingleListNode<T>::SingleListNode(const SingleListNode<T>& Node_):
_Data(Node_._Data),
_Next(NULL)
{
  if (Node_._Next)
    _Next = Node_._Next->CloneNode();
}

/****************************************************************************/
template <class T>
SingleListNode<T>::SingleListNode(const SingleListNode<T>& Node_, int, int):
_Data(Node_._Data),
_Next(NULL)
{}

/****************************************************************************/
template <class T>
SingleListNode<T>::~SingleListNode()
{
  if (_Next && _Next == this)
    _Next = NULL;
  else
  {
    delete _Next;
    _Next = NULL;
  }
}

/****************************************************************************/
template <class T>
size_t SingleListNode<T>::NumNodes(const SingleListNode<T>* Node_)
{
  if (!Node_)
    return 0;

  size_t i;
  const SingleListNode<T>* ListPtr_ = Node_;
  const SingleListNode<T>* FirstPtr_ = ListPtr_;
  for (i = 1; ListPtr_->Next() && ListPtr_->Next() != FirstPtr_;
       i++, ListPtr_ = ListPtr_->Next());

  return i;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Make()
{
  return (new SingleListNode<T>());
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Make(const T& Data_)
{
  return (new SingleListNode<T>(Data_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Make(T* Destination_, int StoreFlags_)
{
  return (new SingleListNode<T>(Destination_, StoreFlags_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Make(const SingleListNode<T>& Node_)
{
  return (new SingleListNode<T>(Node_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Make(const SingleListNode<T>& Node_, int, int)
{
  return (new SingleListNode<T>(Node_, 0, 0));
}

/****************************************************************************/
template <class T>
SingleListNode<T>& SingleListNode<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static SingleListNode<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& SingleListNode<T>::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& SingleListNode<T>::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
int SingleListNode<T>::ClassID() const
{
  return TypeInfo::SINGLELISTNODE_TYPE;
}

/****************************************************************************/
template <class T>
Object* SingleListNode<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (!ObjComp_)
    return (new SingleListNode<T>(*this));

  return NULL;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::CloneNode() const
{
  return (new SingleListNode<T>(*this));
}

/****************************************************************************/
template <class T>
int SingleListNode<T>::CompareItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const
{
  if (!_Data.Pointee())
    Xnull();

  return ((*CmpFnc_)(*_Data.Pointee(), Obj_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>& SingleListNode<T>::operator = (const SingleListNode<T>& Node_)
{
  if (this != &Node_)
  {
    _Data = Node_._Data;
    _Next = NULL;
    
    if (Node_._Next)
      _Next = Node_._Next->CloneNode();
    else
      _Next = NULL;
  }

  return *this;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::ReplaceData(SingleListNode<T>* Node_)
{
  if (this != Node_);
    _Data = Node_->_Data;

  return this;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Add(SingleListNode<T>* Node_, int Relation_)
{
  if (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL)
  {
    SingleListNode<T>* Next_ = Next();
    SetNext(Node_);
    SingleListNode<T>* Last_ = Node_->Last();
    if (Last_)
      Last_->SetNext(Next_);
  }
  else if (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD)
  {
    SingleListNode<T>* Last_ = Node_->Last();
    if (Last_)
      Last_->SetNext(this);
  }

  return this;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Add(SingleSublist<T>* Sublist_, int Relation_)
{
  Sublist_->ReleaseControl();

  if (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL)
  {
    SingleListNode<T>* Next_ = Next();
    SetNext(Sublist_->Head());
    SingleListNode<T>* Tail_ = Sublist_->Tail();
    if (Tail_)
      Tail_->SetNext(Next_);
  }
  else if (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD)
  {
    SingleListNode<T>* Tail_ = Sublist_->Tail();
    if (Tail_)
      Tail_->SetNext(this);
  }

  return this;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Erase()
{
  _Data.Destroy();
  return this;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::SetItem(T* Ptr_, int StoreFlags_)
{
  _Data.AssignPtr(Ptr_, StoreFlags_);
  return this;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::SetItem(const T& Obj_)
{
  _Data.AssignObject(Obj_);
  return this;
}

/****************************************************************************/
template <class T>
const T* SingleListNode<T>::GetItem() const
{
  return _Data.ConstPointee();
}

/****************************************************************************/
template <class T>
T* SingleListNode<T>::GetItem()
{
  return _Data.Pointee();
}

/****************************************************************************/
template <class T>
const T* SingleListNode<T>::FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const
{
  return ((CompareItem(Obj_, CmpFnc_) == 0) ? _Data.Pointee():NULL);
}

/****************************************************************************/
template <class T>
T* SingleListNode<T>::FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&))
{
  return ((CompareItem(Obj_, CmpFnc_) == 0) ? _Data.Pointee():NULL);
}

/****************************************************************************/
template <class T>
int SingleListNode<T>::ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const
{
  if (!_Data.Pointee())
    Xnull();

  return ((*Fnc_)(*_Data.Pointee(), Obj_));
}

/****************************************************************************/
template <class T>
int SingleListNode<T>::ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_)
{
  if (!_Data.Pointee())
    Xnull();

  return ((*Fnc_)(*_Data.Pointee(), Obj_));
}

/****************************************************************************/
template <class T>
int SingleListNode<T>::ApplyUnary(int(*Fnc_)(T*))
{
  return ((*Fnc_)(_Data.Pointee()));
}

/****************************************************************************/
template <class T>
int SingleListNode<T>::ApplyUnary(T(*Fnc_)(T))
{
  if (_Data.Pointee())
    SetItem((*Fnc_)(*_Data.Pointee()));

  return 1;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::DetachNode()
{
  SetNext(NULL);
  return this;
}

/****************************************************************************/
template <class T>
int SingleListNode<T>::GetStorageAttributes() const
{
  return _Data.GetStorageAttributes();
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::SetNext(SingleListNode<T>* Next_)
{
  _Next = Next_;
  return this;
}

/****************************************************************************/
template <class T>
const SingleListNode<T>* SingleListNode<T>::Next(size_t Nodes_) const
{
  if (Nodes_)
    return (_Next ? _Next->Next(Nodes_ - 1):NULL);

  return this;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Next(size_t Nodes_)
{
  if (Nodes_)
    return (_Next ? _Next->Next(Nodes_ - 1):NULL);
    
  return this;
}

/****************************************************************************/
template <class T>
const SingleListNode<T>* SingleListNode<T>::Last() const
{
  const SingleListNode<T>* Current_ = this;
  const SingleListNode<T>* Ptr_ = this;

  while (Ptr_->_Next && Ptr_->_Next != Current_)
    Ptr_ = Ptr_->_Next;

  return Ptr_;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListNode<T>::Last()
{
  SingleListNode<T>* Current_ = this;
  SingleListNode<T>* Ptr_ = this;

  while (Ptr_->_Next && Ptr_->_Next != Current_)
    Ptr_ = Ptr_->_Next;

  return Ptr_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SingleListNode<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SingleListNode<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SingleListNode<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SingleListNode<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SingleSublist<T>::SingleSublist():
_Head(NULL),
_Tail(NULL),
_NumNodes(0)
{}

/****************************************************************************/
template <class T>    
SingleSublist<T>::SingleSublist(SingleSublist<T>* Sublist_, int StoreFlags_):
_Head(Sublist_->Head()),
_Tail(NULL),
_NumNodes(Sublist_->_NumNodes),
_Data(Sublist_->Head(), StoreFlags_)
{
  Sublist_->ReleaseControl();

  _Head = _Data.Pointee();
  SingleListNode<T>* ListPtr_ = _Head;

  size_t i;
  for (i = 0; i < _NumNodes && ListPtr_->Next(); i++)
    ListPtr_ = ListPtr_->Next();

  _Tail = ListPtr_;    
}

/****************************************************************************/
template <class T>
SingleSublist<T>::SingleSublist(const SingleSublist<T>& Sublist_):
_Head(NULL),
_Tail(NULL),
_NumNodes(Sublist_._NumNodes),
_Data(Sublist_._Data)
{
  _Head = _Data.Pointee();
  SingleListNode<T>* ListPtr_ = _Head;

  size_t i;
  for (i = 0; i < _NumNodes && ListPtr_->Next(); i++)
    ListPtr_ = ListPtr_->Next();

  _Tail = ListPtr_;
}

/****************************************************************************/
template <class T>
SingleSublist<T>* SingleSublist<T>::Make()
{
  return (new SingleSublist<T>());
}

/****************************************************************************/
template <class T>
SingleSublist<T>* SingleSublist<T>::Make(SingleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new SingleSublist<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
SingleSublist<T>* SingleSublist<T>::Make(const SingleSublist<T>& Sublist_)
{
  return (new SingleSublist<T>(Sublist_));
}

/****************************************************************************/
/*
procedure for quick sort partitioning
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  uses the comparison function assigned to the comp function pointer
CHANGED:
  the array of pointers is sorted into order
RETURNED:
  returns the pivot position of the array
*/
template <class T>
size_t SingleSublist<T>::Partition(SingleListNode<T>** List_, size_t lo, size_t hi,
                                   int Order_, int(*CmpFnc_)(const T&, const T&))
{
  short dir = -1;
  SingleListNode<T>* pivot = List_[lo];  // pivot element

  while (lo < hi)
  {
    switch (dir)
    {
      case -1 : while (((*CmpFnc_)(*List_[hi]->GetItem(), *pivot->GetItem()) * Order_) >= 0 && lo < hi)
		  hi += dir;
		List_[lo] = List_[hi];
		break;

      case 1 : while (((*CmpFnc_)(*List_[lo]->GetItem(), *pivot->GetItem()) * Order_) <= 0 && lo < hi)
		 lo += dir;
	       List_[hi] = List_[lo];
	       break;
    }

    dir *= -1; /* change direction of pointer movement */
  }

  List_[lo] = pivot;
  return lo;
}

/****************************************************************************/
/*
procedure for quick sort
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  uses partition to sort each subarray of the array
  uses quicksort to sort recursively
CHANGED:
  the array is sorted into order
*/
template <class T>
void SingleSublist<T>::_QuickSort(SingleListNode<T>** List_, size_t lo, size_t hi,
                                  int Order_, int(*CmpFnc_)(const T&, const T&))
{
  size_t newx;

  if (lo < hi && hi != SIZET_MAX)
  {
    newx = Partition(List_, lo, hi, Order_, CmpFnc_);  /* partition array */
    _QuickSort(List_, lo, newx-1, Order_, CmpFnc_);    /* sort left subarray */
    _QuickSort(List_, newx+1, hi, Order_, CmpFnc_);    /* sort right subarray */
  }
}

/****************************************************************************/
/*
quicksort wrapper procedure for checking pointers
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  _quicksort to call the quicksort sorting procedure
*/
template <class T>
void SingleSublist<T>::QuickSort(SingleListNode<T>** List_, size_t lo, size_t hi,
                                 int Order_, int(*CmpFnc_)(const T&, const T&))
{
  if (!List_)
    Xnull();
  
  _QuickSort(List_, lo, hi, Order_, CmpFnc_);   // execute quick sort procedure
}

/****************************************************************************/
template <class T>
Boolean SingleSublist<T>::Destroy()
{
  _Data.Destroy();
  _NumNodes = 0;
  _Head = _Tail = NULL;
  return TRUE;
}

/****************************************************************************/
template <class T>
SingleSublist<T>* SingleSublist<T>::CloneSublist() const
{
  return (new SingleSublist<T>(*this));
}

/****************************************************************************/
template <class T>
int SingleSublist<T>::CompareSublist(const SingleSublist<T>& Sublist_,
                                     int(*CmpFnc_)(const T&, const T&)) const
{
  if (!_Head || !Sublist_._Head)
    return (_Head ? 1:
	    Sublist_._Head ? -1:0);

  const SingleListNode<T>* lp1_ = Head();
  const SingleListNode<T>* lp2_ = Sublist_.Head();

  int diff;  
  size_t Max1_ = NumNodes();
  size_t Max2_ = Sublist_.NumNodes();
  
  for (diff = 0;
       !diff && lp1_ && lp2_ && Max1_ && Max2_;
       lp1_ = lp1_->Next(), lp2_ = lp2_->Next(), Max1_--, Max2_--)
  {
    if (!lp1_ || !lp2_)
      diff = (lp1_ ? 1:
  	      lp2_ ? -1:0);
    else
    {
      if (!lp2_->GetItem())
        Xnull();

      diff = lp1_->CompareItem(*lp2_->GetItem(), CmpFnc_);
    }
  }
  
  
  return
  (
    diff ? diff:
    (
      (!Max1_ && !Max2_) || (!lp1_ && !lp2_) ? 0:
      (Max1_ - Max2_)
    )
  );
}

/****************************************************************************/
template <class T>
SingleSublist<T>& SingleSublist<T>::operator = (const SingleSublist<T>& Sublist_)
{
  if (this != &Sublist_)
  {
    _Data.Destroy();

    _Head = _Tail = NULL;
    _NumNodes = Sublist_._NumNodes;
    _Data = Sublist_._Data;

    _Head = _Data.Pointee();
    SingleListNode<T>* ListPtr_ = _Head;

    size_t i;
    for (i = 0; i < _NumNodes && ListPtr_->Next() && ListPtr_->Next() != _Head; i++)
      ListPtr_ = ListPtr_->Next();

    _Tail = ListPtr_;
  }

  return *this;
}

/****************************************************************************/
template <class T>
SingleSublist<T>& SingleSublist<T>::AssignListNode(SingleListNode<T>* Node_, int StoreFlags_)
{
  if (Head() != Node_)
  {
    _Data.Destroy();
  
    _Head = _Tail = NULL;
    _NumNodes = 1;
    _Data.AssignPtr(Node_, StoreFlags_);

    _Head = _Data.Pointee();
    _Tail = _Head;
  }

  return *this;
}

/****************************************************************************/
template <class T>
SingleSublist<T>& SingleSublist<T>::AddSublist(SingleSublist<T>* Sublist_, int Relation_)
{
  if (Sublist_)
  {
    Sublist_->ReleaseControl();
    size_t Size_ = Sublist_->NumNodes();
    
    if ((Relation_ == ListRelation::NEXT ||
         Relation_ == ListRelation::TAIL) && _Tail)
    {
      _Tail->Add(Sublist_, ListRelation::NEXT);
      while (_Tail->Next() && _Tail->Next() != _Head)
        _Tail = _Tail->Next();
      _NumNodes += Size_;
    }
    else if ((Relation_ == ListRelation::PREVIOUS ||
              Relation_ == ListRelation::HEAD) && _Head)
    {
      SingleListNode<T>* NewHead_ = Sublist_->Head();    
      _Head->Add(Sublist_, ListRelation::PREVIOUS);

      if (NewHead_ != _Head)
      {
        _Data.ReleaseControl();
        _Data.AssignPtr(NewHead_, GetStorageAttributes());
        _Head = _Data.Pointee();
        _Tail = _Head->Last();
      }

      _NumNodes += Size_;
    }
    else
      SingleSublist<T>::operator = (*Sublist_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
const SingleListNode<T>* SingleSublist<T>::FindSublistItem(const T& Obj_, size_t& Index_, int(*CmpFnc_)(const T&, const T&)) const
{
  const SingleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    if (ListPtr_->FindItem(Obj_, CmpFnc_))
      return ListPtr_;

  return NULL;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleSublist<T>::FindSublistItem(const T& Obj_, size_t& Index_, int(*CmpFnc_)(const T&, const T&))
{
  SingleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    if (ListPtr_->FindItem(Obj_, CmpFnc_))
      return ListPtr_;

  return NULL;
}

/****************************************************************************/
template <class T>
int SingleSublist<T>::ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const
{
  size_t Index_;
  SingleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyBinary(Fnc_, Obj_);

  return 0;
}

/****************************************************************************/
template <class T>
int SingleSublist<T>::ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_)
{
  size_t Index_;
  SingleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyBinary(Fnc_, Obj_);

  return 0;
}

/****************************************************************************/
template <class T>
int SingleSublist<T>::ApplyUnary(int(*Fnc_)(T*))
{
  size_t Index_;
  SingleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyUnary(Fnc_);

  return 0;
}

/****************************************************************************/
template <class T>
int SingleSublist<T>::ApplyUnary(T(*Fnc_)(T))
{
  size_t Index_;
  SingleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyUnary(Fnc_);

  return 0;
}

/****************************************************************************/
template <class T>
int SingleSublist<T>::GetStorageAttributes() const
{
  return _Data.GetStorageAttributes();
}

/****************************************************************************/
template <class T>
void SingleSublist<T>::SortSublist(int Order_, int(*CmpFnc_)(const T&, const T&))
{
  SingleListNode<T>* ListPtr_ = _Head;
  SingleListNode<T>** Array_ = new SingleListNode<T>*[_NumNodes];
  size_t i;

  for (i = 0; i < _NumNodes && ListPtr_; i++)
  {
    Array_[i] = ListPtr_;
    ListPtr_ = ListPtr_->Next();
  }

  QuickSort(Array_, 0, _NumNodes - 1, Order_, CmpFnc_);
  Relink(Array_);

  _Data.ReleaseControl();
  _Data.AssignPtr(Array_[0], _Data.GetStorageAttributes());
  _Head = _Data.Pointee();
  _Tail = _Head->Last();
}

/****************************************************************************/
template <class T>
void SingleSublist<T>::Relink(SingleListNode<T>** Array_)
{
  size_t i;

  for (i = 0; i < _NumNodes; i++)
    if (i + 1 < _NumNodes && Array_[i] && Array_[i + 1])
      Array_[i]->SetNext(Array_[i + 1]);

  Array_[i - 1]->SetNext(NULL);
}

/****************************************************************************/
template <class T>    
const SingleListNode<T>* SingleSublist<T>::GetNode(size_t Index_) const
{
  SingleListNode<T>* ListPtr_ = _Head;
  size_t i;

  if (0 <= Index_ && Index_ < _NumNodes)
  {
    for (i = 0; i < Index_ && ListPtr_; i++, ListPtr_ = ListPtr_->Next());
    return ListPtr_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>    
SingleListNode<T>* SingleSublist<T>::GetNode(size_t Index_)
{
  SingleListNode<T>* ListPtr_ = _Head;
  size_t i;

  if (0 <= Index_ && Index_ < _NumNodes)
  {
    for (i = 0; i < Index_ && ListPtr_; i++, ListPtr_ = ListPtr_->Next());
    return ListPtr_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>
size_t SingleSublist<T>::NumNodes() const
{
  return _NumNodes;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SingleSublist<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void SingleSublist<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SingleSublist<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SingleSublist<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleArray<int> SeriesListCompare(const SingleSublist<T>& Sublist1_,
                                   const SingleSublist<T>& Sublist2_)
{
  if (_CompFunc && (Sublist1_.NumNodes() == Sublist2_.NumNodes()))
  {
    SimpleArray<int> Result_(Sublist1_.NumNodes());
    size_t x, Max_;
    
    for (x = 0, Max_ = Sublist1_.NumNodes(); x < Max_; x++)
    {
      const T* Ptr1_ = Sublist1_.GetNode(x)->GetItem();
      const T* Ptr2_ = Sublist2_.GetNode(x)->GetItem();

      if (Ptr1_ && Ptr2_)
        Result_[x] = (*_CompFunc)(*Ptr1_, *Ptr2_);
      else
        Result_[x] = INT_MAX;
    }

    return Result_;
  }

  SimpleArray<int> Temp_;
  Temp_.SetError(TRUE);
  return Temp_;
}

/****************************************************************************/
template <class T>
SingleList<T>::SingleList():
_Limit(SIZET_MAX),
_IteratorsActive(0),
_DestroyFromIterator(FALSE)
{}

/****************************************************************************/
template <class T>
SingleList<T>::SingleList(SingleList<T>* List_, int StoreFlags_):
SingleSublist<T>(List_, StoreFlags_),
_Limit(List_->_Limit),
_IteratorsActive(List_->_IteratorsActive),
_DestroyFromIterator(List_->_DestroyFromIterator)
{}

/****************************************************************************/
template <class T>
SingleList<T>::SingleList(SingleSublist<T>* Sublist_, int StoreFlags_):
SingleSublist<T>(Sublist_, StoreFlags_),
_Limit(SIZET_MAX),
_IteratorsActive(0),
_DestroyFromIterator(FALSE)
{}

/****************************************************************************/
template <class T>
SingleList<T>::SingleList(const SingleList<T>& List_):
SingleSublist<T>(List_),
_Limit(List_._Limit),
_IteratorsActive(List_._IteratorsActive),
_DestroyFromIterator(List_._DestroyFromIterator)
{}

/****************************************************************************/
template <class T>
SingleList<T>::SingleList(const SingleSublist<T>& Sublist_):
SingleSublist<T>(Sublist_),
_Limit(SIZET_MAX),
_IteratorsActive(0),
_DestroyFromIterator(FALSE)
{}

/****************************************************************************/
template <class T>
SingleList<T>* SingleList<T>::Make()
{
  return (new SingleList<T>());
}

/****************************************************************************/
template <class T>
SingleList<T>* SingleList<T>::Make(SingleList<T>* List_, int StoreFlags_)
{
  return (new SingleList<T>(List_, StoreFlags_));
}

/****************************************************************************/
template <class T>
SingleList<T>* SingleList<T>::Make(SingleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new SingleList<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
SingleList<T>* SingleList<T>::Make(const SingleList<T>& List_)
{
  return (new SingleList<T>(List_));
}

/****************************************************************************/
template <class T>
SingleList<T>* SingleList<T>::Make(const SingleSublist<T>& Sublist_)
{
  return (new SingleList<T>(Sublist_));
}

/****************************************************************************/
template <class T>
void SingleList<T>::Flush()
{
  Destroy();
}

/****************************************************************************/
template <class T>
SimpleArray<int> SingleList<T>::SeriesListCompare(const SingleSublist<T>& Sublist_) const
{
  if (_CompFunc && (Size() == Sublist_.NumNodes()))
  {
    SimpleArray<int> Result_(Size());
    size_t x, Max_;
    
    for (x = 0, Max_ = Size(); x < Max_; x++)
    {
      const T* Ptr1_ = GetNode(x)->GetItem();
      const T* Ptr2_ = Sublist_.GetNode(x)->GetItem();

#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
      if (Ptr1_ && Ptr2_)
        (Result_.Base())[x] = (*_CompFunc)(*Ptr1_, *Ptr2_);
      else
        (Result_.Base())[x] = INT_MAX;
#else
      if (Ptr1_ && Ptr2_)
        Result_[x] = (*_CompFunc)(*Ptr1_, *Ptr2_);
      else
        Result_[x] = INT_MAX;
#endif
    }

    return Result_;
  }

  SimpleArray<int> Temp_;
  Temp_.SetError(TRUE);
  return Temp_;
}

/****************************************************************************/
template <class T>
int SingleList<T>::Compare(const SingleSublist<T>& Sublist_) const
{
  if (_CompFunc)
    return CompareSublist(Sublist_, _CompFunc);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsEqualToSingleList(const SingleSublist<T>* Sublist_) const
{
  if (_CompFunc && Sublist_)
    return (CompareSublist(*Sublist_, _CompFunc) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsLesserToSingleList(const SingleSublist<T>* Sublist_) const
{
  if (_CompFunc && Sublist_)
    return (CompareSublist(*Sublist_, _CompFunc) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsGreaterToSingleList(const SingleSublist<T>* Sublist_) const
{
  if (_CompFunc && Sublist_)
    return (CompareSublist(*Sublist_, _CompFunc) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  SingleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToSingleList(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  SingleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToSingleList(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  SingleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToSingleList(TrgComp_);
}

/****************************************************************************/
template <class T>
void SingleList<T>::IgnoreElement(ByteStream& Is_, Boolean* Ok_)
{
  T Object_;  
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.iread((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
    Ignore(Is_, ((T&)(*ObjPtr_)), &Done_);

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void SingleList<T>::ReadElement(ByteStream& Is_, SingleListNode<T>* ListPtr_, Boolean* Ok_)
{
  T Object_;
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.iread((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
  {
    Read(Is_, ((T&)(*ObjPtr_)), &Done_);
    if (IstreamCheck(Is_, Ok_) && Done_)
      ListPtr_->SetItem(*ObjPtr_);
  }

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void SingleList<T>::WriteElement(ByteStream& Os_, const SingleListNode<T>* ListPtr_, Boolean* Ok_) const
{
  const T* ObjPtr_ = ListPtr_->GetItem();
  long Code_ = WhatIs(*ObjPtr_);
  Boolean WrOk_ = OstreamCheck(Os_.owrite((char*)(&Code_), sizeof(long)), Ok_);

  if (WrOk_)
  {
    Write(Os_, *ObjPtr_, Ok_);
    OstreamCheck(Os_, Ok_);
  }
}

/****************************************************************************/
template <class T>
void SingleList<T>::IgnoreElement(istream& Is_, Boolean* Ok_)
{
  T Object_;
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.read((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
    Ignore(Is_, ((T&)(*ObjPtr_)), &Done_);

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void SingleList<T>::ReadElement(istream& Is_, SingleListNode<T>* ListPtr_, Boolean* Ok_)
{
  T Object_;
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.read((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
  {
    Read(Is_, ((T&)(*ObjPtr_)), &Done_);
    
    if (IstreamCheck(Is_, Ok_) && Done_)
      ListPtr_->SetItem(*ObjPtr_);
  }

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void SingleList<T>::WriteElement(ostream& Os_, const SingleListNode<T>* ListPtr_, Boolean* Ok_) const
{
  const T* ObjPtr_ = ListPtr_->GetItem();
  long Code_ = WhatIs(*ObjPtr_);
  Boolean WrOk_ = OstreamCheck(Os_.write((char*)(&Code_), sizeof(long)), Ok_);

  if (WrOk_)
  {  
    Write(Os_, *ObjPtr_, Ok_);
    OstreamCheck(Os_, Ok_);
  }
}

/****************************************************************************/
template <class T>
istream& SingleList<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    Is_.read((char*)(&Size_), sizeof(long));

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	     IgnoreElement(Is_, Ok_);

	     if (IsIOError())
	       break;
      }

      if (!IsIOError())
	     SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
istream& SingleList<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  SingleListNode<T>* ListPtr_;
  Boolean RdChk_;
  long Size_;
  long Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Flush();
    Is_.read(((char*)(&Size_)), sizeof(long));

    if (Size_)
    {
      SingleListNode<T> sNode_;
      AssignListNode(&sNode_, DataStorageAttributes::DEEP_COPY);
      for (Index_ = 1; Index_ < Size_; Index_++)
        AddListNode(SingleListNode<T>::Make(), ListRelation::NEXT);
    }

    if (IstreamCheck(Is_, Ok_))
    {
      ListPtr_ = Head();

      for (Index_ = 0; Index_ < Size_ && ListPtr_; Index_++, ListPtr_ = ListPtr_->Next())
      {
	     ReadElement(Is_, ListPtr_, Ok_);

	     if (IsIOError())
	       break;
      }

      if (!IsIOError())
	     SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& SingleList<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  const SingleListNode<T>* ListPtr_;
  Boolean RdChk_;
  long RunLength_;
  long Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    RunLength_ = (long)Size();
    Os_.write(((char*)(&RunLength_)), sizeof(long));

    if (OstreamCheck(Os_, Ok_))
    {
      ListPtr_ = Head();

      for (Index_ = 0; Index_ < RunLength_ && ListPtr_; Index_++, ListPtr_ = ListPtr_->Next())
      {
	     WriteElement(Os_, ListPtr_, Ok_);

	     if (IsIOError())
	       break;
      }

      if (!IsIOError())
	     SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& SingleList<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    Is_.iread((char*)(&Size_), sizeof(long));

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	     IgnoreElement(Is_, Ok_);

	     if (IsIOError())
	       break;
      }

      if (!IsIOError())
	     SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& SingleList<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  SingleListNode<T>* ListPtr_;
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Flush();
    Is_.iread((char*)(&Size_), sizeof(long));

    if (Size_)
    {
      SingleListNode<T> sNode_;
      AssignListNode(&sNode_, DataStorageAttributes::DEEP_COPY);
      for (Index_ = 1; Index_ < Size_; Index_++)
        AddListNode(SingleListNode<T>::Make(), ListRelation::NEXT);
    }

    if (IstreamCheck(Is_, Ok_))
    {
      ListPtr_ = Head();

      for (Index_ = 0; Index_ < Size_ && ListPtr_; Index_++, ListPtr_ = ListPtr_->Next())
      {
	     ReadElement(Is_, ListPtr_, Ok_);

	     if (IsIOError())
	       break;
      }

      if (!IsIOError())
	     SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& SingleList<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  const SingleListNode<T>* ListPtr_;
  Boolean RdChk_;
  long RunLength_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    RunLength_ = (long)Size();
    Os_.owrite((char*)(&RunLength_), sizeof(long));

    if (OstreamCheck(Os_, Ok_))
    {
      ListPtr_ = Head();

      for (Index_ = 0; Index_ < RunLength_ && ListPtr_; Index_++, ListPtr_ = ListPtr_->Next())
      {
	     WriteElement(Os_, ListPtr_, Ok_);

	     if (IsIOError())
	       break;
      }

      if (!IsIOError())
	     SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
ostream& SingleList<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;

  for (i = 0, Max_ = Size(); i < Max_; ++i)
  {
    const T* Ptr_ = GetNode(i)->GetItem();
    if (Ptr_)
      WriteToOutput(Os_, *Ptr_);
    Os_ <<" ";
  }

  return Os_;
}

/****************************************************************************/
// Returns the total size of all the data within the container structure
// excluding formatting and class ID information.
//
template <class T>
long SingleList<T>::DataSize() const
{
  const T* ItemPtr_ = NULL;
  const SingleListNode<T>* ListPtr_ = Head();
  long DataSize_ = 0;

  for (size_t Index_ = 0, Max_ = Size();
       Index_ < Max_ && ListPtr_; Index_++, ListPtr_ = ListPtr_->Next())
    if (ItemPtr_ = ListPtr_->GetItem())
      DataSize_ += SizeOfData(*ItemPtr_);

  return DataSize_;
}

/****************************************************************************/
template <class T>
long SingleList<T>::StorableFormWidth() const
{
  const T* ItemPtr_ = NULL;
  const SingleListNode<T>* ListPtr_ = Head();

  // First data item is the ID code for the data structure
  long ObjSize_ = sizeof(long);

  // Second data item is the ID of the object
  ObjSize_ += sizeof(long);

  // Third data item is the null object status byte
  ObjSize_ += sizeof(Byte_t);

  // Fourth data item is the actual array size in number of elements
  ObjSize_ += sizeof(long);

  // Each list element is encoded as data comprising
  // the actual size of the object
  for (size_t Index_ = 0, Max_ = Size();
       Index_ < Max_ && ListPtr_; Index_++, ListPtr_ = ListPtr_->Next())
    if (ItemPtr_ = ListPtr_->GetItem())
      ObjSize_ += SizeOfData(*ItemPtr_);

  return ObjSize_;
}

/****************************************************************************/
// PURPOSE:
//   Method to dereference and invoke the _Factory pointer to function with
//   the specified type code to create a new object of T or derived from T.
//   if the pointer to function is null then a Null pointer exception is
//   thrown.
//
// PRE:
//   int Code_	: The type code to invoke the factory function with.
//
// POST:
//   If the factory function is not null then it should return a newly
//   allocated object of type T or derived from T with the same type code
//   as the one specified in the argument to the method.
//
template <class T>
T* SingleList<T>::MakeByTypeCode(int Code_)
{
  // If pointer to function is null then throw an exception.
  if (_Factory == NULL)
    Xnull();

  // Create object based on type code.
  return (*_Factory)(Code_);
}

/****************************************************************************/
template <class T>
const SingleListNode<T>* SingleList<T>::FindItem(const T& Obj_, size_t& Index_) const
{
  if (_CompFunc)
    return FindSublistItem(Obj_, Index_, _CompFunc);

  return NULL;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleList<T>::FindItem(const T& Obj_, size_t& Index_)
{
  if (_CompFunc)
    return FindSublistItem(Obj_, Index_, _CompFunc);

  return NULL;
}

/****************************************************************************/
template <class T>
void SingleList<T>::Sort(int Order_)
{
  if (_CompFunc)
    SortSublist(Order_, _CompFunc);
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static SingleList<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& SingleList<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return SingleList<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& SingleList<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return SingleList<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int SingleList<T>::ClassID() const
{
  return TypeInfo::SINGLELIST_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* SingleList<T>::GiveObjectAcceptor() const
{
  return (new SingleListAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Object& SingleList<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  SingleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromSingleList(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* SingleList<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new SingleList<T>(*this));

  SingleListAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromSingleList(TrgComp_)));
}

/****************************************************************************/
template <class T>
SingleList<T>* SingleList<T>::CloneList() const
{
  return (new SingleList<T>(*this));
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::operator = (const SingleList<T>& List_)
{
  if (this != &List_)
  {
    SingleSublist<T>::operator = (List_);

    _IteratorsActive = List_._IteratorsActive;
    _DestroyFromIterator = List_._DestroyFromIterator;
    _Limit = List_._Limit;
    _CompFunc = List_._CompFunc;
  }

  return *this;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::operator += (const SingleList<T>& List_)
{
  SingleSublist<T>* Sublist_ = new SingleSublist<T>(List_);
  AddList(Sublist_, ListRelation::TAIL);
  return *this;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::operator += (const SingleListNode<T>& Node_)
{
  SingleListNode<T>* ListNode_ = new SingleListNode<T>(Node_);
  AddListNode(ListNode_, ListRelation::NEXT);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::AddList(SingleSublist<T>* List_, int Relation_, Boolean DelArgList_)
{
  if (List_ && List_->NumNodes() + Size() <= Limit())
  {
    List_->ReleaseControl();
    AddSublist(List_, Relation_);
    if (List_->IsNewed() && DelArgList_)
      delete List_;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::AssignListNode(SingleListNode<T>* Node_, int StoreFlags_)
{
  SingleSublist<T>::AssignListNode(Node_, StoreFlags_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::AddListNode(SingleListNode<T>* Node_, int Relation_)
{
  size_t Size_ = SingleListNode<T>::NumNodes(Node_);
  
  if (Node_ && (Size() + Size_ <= Limit()))
  {
    if ((Relation_ == ListRelation::TAIL ||
         Relation_ == ListRelation::NEXT) && _Tail)
    {
      _Tail->Add(Node_, Relation_);
      while (_Tail->Next() && _Tail->Next() != _Head)
        _Tail = _Tail->Next();
      _NumNodes += Size_;      
    }
    else if ((Relation_ == ListRelation::HEAD ||
              Relation_ == ListRelation::PREVIOUS) && _Head)
    {
      _Head->Add(Node_, Relation_);

      _Data.ReleaseControl();
      _Data.AssignPtr(Node_, GetStorageAttributes());
      _Head = _Data.Pointee();
      _Tail = _Head->Last();

      _NumNodes += Size_;
    }
    else
      AssignListNode(Node_, Node_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::AddListNode(SingleListNode<T>* Node_, SingleListNode<T>* IndexNode_)
{
  size_t Size_ = SingleListNode<T>::NumNodes(Node_);
  
  if (Node_ && (Size() + Size_ <= Limit()) && IndexNode_ != NULL)
  {
    Boolean AtTail_ = IndexNode_ == Tail();
    IndexNode_->Add(Node_, ListRelation::NEXT);

    if (AtTail_)
      _Tail = Node_->Last();

    IncreaseSize(Size_);
    return TRUE;  
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::AddListNodeAtIndex(SingleListNode<T>* Node_, size_t Index_)
{
  size_t Size_ = SingleListNode<T>::NumNodes(Node_);
  
  if (Node_ && (Size() + Size_ <= Limit()) && Index_ < Size())
  {
    GetNode(Index_)->Add(Node_, ListRelation::NEXT);

    if (Index_ == Size() - 1)
      _Tail = Node_->Last();

    IncreaseSize(Size_);
    return TRUE;  
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleList<T>::DetachFromHead()
{
  if (IsEmpty())
    return NULL;

  SingleListNode<T>* NewHead_ = _Head->Next();
  size_t Size_ = Size();
  Boolean Valid_ = (_Head != NULL) && ((Size_ > 1) ? (NewHead_ != NULL):Size_);

  if (Valid_ && _Head)
  {
    SingleListNode<T>* NewListNode_ = _Head->DetachNode();
    _Data.ReleaseControl();

    if (NewHead_)
    {
      _Data.AssignPtr(NewHead_, GetStorageAttributes());
      _Head = _Data.Pointee();
      _Tail = _Head->Last();
    }
    else
      _Head = _Tail = NULL;
      
    _NumNodes--;
    return NewListNode_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleList<T>::DetachFromTail()
{
  if (IsEmpty())
    return NULL;

  size_t i;
  SingleListNode<T>* NewTail_ = NULL;

  if (_Head->Next())
  {
    NewTail_ = _Head;
    for (i = 0; i < _NumNodes && NewTail_->Next(2) && NewTail_->Next() != _Head; i++)
      NewTail_ = NewTail_->Next();
  }

  size_t Size_ = Size();
  Boolean Valid_ = (_Tail != NULL) && ((Size_ > 1) ? (NewTail_ != NULL):Size_);

  if (Valid_ && _Head)
  {
    if (NewTail_)
      NewTail_->DetachNode();
      
    SingleListNode<T>* NewListNode_ = _Tail;
    
    if (NewTail_)
      _Tail = NewTail_;
    else
    {
      _Data.ReleaseControl();
      _Head = _Tail = NULL;    
    }
    
    _NumNodes--;
    return NewListNode_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleList<T>::DetachFromMiddle(SingleListNode<T>* IndexNode_)
{
  if (IsEmpty())
    return NULL;

  if (IndexNode_ != NULL)
  {
    Boolean AtHead_ = IndexNode_ == Head();
    Boolean AtTail_ = IndexNode_ == Tail();
  
    if (AtHead_)
      return DetachFromHead();

    if (AtTail_)
      return DetachFromTail();

    SingleListNode<T>* Prev_;
    for (Prev_ = Head(); Prev_ && Prev_->Next() != IndexNode_; Prev_ = Prev_->Next());
    SingleListNode<T>* This_ = IndexNode_;
    SingleListNode<T>* Next_ = This_->Next();

    if (Prev_ && This_ && Next_)
    {
      Prev_->SetNext(Next_);
      SingleListNode<T>* RetNode_ = This_->DetachNode();
      _NumNodes--;
      return RetNode_;      
    }    
  }

  return NULL;
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleList<T>::DetachFromMiddle(size_t Index_)
{
  if (IsEmpty())
    return NULL;

  if (Index_ <= Size() - 1)
  {
    if (Index_ == 0)
      return DetachFromHead();

    if (Index_ >= Size() - 1)
      return DetachFromTail();

    SingleListNode<T>* Prev_ = GetNode(Index_ - 1);
    SingleListNode<T>* This_ = Prev_->Next();
    SingleListNode<T>* Next_ = This_->Next();

    if (Prev_ && This_ && Next_)
    {
      Prev_->SetNext(Next_);
      SingleListNode<T>* RetNode_ = This_->DetachNode();
      _NumNodes--;
      return RetNode_;      
    }    
  }

  return NULL;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsEmpty() const
{
  return (Size() == 0);
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsFull() const
{
  return (Size() == Limit());
}

/****************************************************************************/
template <class T>
size_t SingleList<T>::Limit() const
{
  return _Limit;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::SetLimit(size_t Limit_, Boolean Shrink_)
{
  if (Size() > Limit_ && Shrink_)
    ShrinkTo(Limit_);

  _Limit = Limit_;
  return *this;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::IncreaseLimit(size_t Ext_, Boolean Shrink_)
{
  SetLimit(_Limit + Ext_, Shrink_);
  return *this;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::DecreaseLimit(size_t Ext_, Boolean Shrink_)
{
  SetLimit(_Limit - Ext_, Shrink_);
  return *this;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::Shrink(size_t Num_)
{
  if (Num_ && Num_ <= Size())
  {
    size_t i;
    SingleListNode<T>* Rest_;

    for (i = 0; i < Num_; i++)
      if (Rest_ = DetachFromTail())
        delete Rest_;
  }
  
  return *this;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::ShrinkTo(size_t Num_)
{
  if (Size() >= Num_)
  {
    size_t ShrinkNum_ = Size() - Num_;
    Shrink(ShrinkNum_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
size_t SingleList<T>::Size() const
{
  return _NumNodes;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::IsNewed() const
{
  if (!NewedVerified())
  {
    SetNewedVerify();
    if (ObjectImp::IsNewed((Container*)this) || ObjectImp::IsNewed((Object*)this))
      SetNewedFlag();
  }

  return NewedFlag();
}

/****************************************************************************/
template <class T>
void SingleList<T>::DestroyFromIterator()
{
# if OVERLOAD_NEW
    _DestroyFromIterator = IsNewed();
# else
    _DestroyFromIterator = TRUE;
# endif
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::RegisterIterator()
{
  if (_IteratorsActive < Size())
  {
    ++_IteratorsActive;
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SingleList<T>::UnRegisterIterator()
{
  if (_IteratorsActive)
  {
    --_IteratorsActive;
    if (!_IteratorsActive && _DestroyFromIterator)
      return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
size_t SingleList<T>::IteratorsActive()
{
  return _IteratorsActive;
}

/****************************************************************************/
template <class T>
void SingleList<T>::SetComparisonFunction(int(*CmpFnc_)(const T&, const T&))
{
  _CompFunc = CmpFnc_;
}

/****************************************************************************/
template <class T>
SingleList<T>& SingleList<T>::ReverseList()
{
  SingleListNode<T>* TempPtr_;
  SingleListNode<T>* ListPtr_ = _Head;
  SingleListNode<T>** Array_ =
  (SingleListNode<T>**)MemMatrix::Matrix().Allocate(sizeof(SingleListNode<T>*) * (_NumNodes + 1));
  size_t i;

  for (i = 0; i < _NumNodes && ListPtr_; i++, ListPtr_ = ListPtr_->Next())
    Array_[i] = ListPtr_;
  Array_[i] = NULL;

  for (i = _NumNodes - 1; i; i--)
    Array_[i]->SetNext(Array_[i - 1]);
  Array_[0]->SetNext(NULL);

  TempPtr_ = _Head;
  _Head = _Tail;
  _Tail = TempPtr_;

  if (_Head != TempPtr_)
  {
    _Data.ReleaseControl();
    _Data.AssignPtr(_Head, GetStorageAttributes());
    _Head = _Data.Pointee();
    _Tail = _Head->Last();
  }

  ::DeleteArray(Array_);
  return *this;
}

/****************************************************************************/
template <class T>
T** SingleList<T>::ListToPtrArray()
{
  SingleListNode<T>* ListPtr_ = _Head;
  T** Array_ = New<T*>(_NumNodes + 1);
  size_t i;

  for (i = 0; i < _NumNodes && ListPtr_; i++, ListPtr_ = ListPtr_->Next())
    Array_[i] = ListPtr_->GetItem();

  Array_[i] = NULL;
  return Array_;
}

/****************************************************************************/
template <class T>
T* SingleList<T>::ListToArray()
{
  SingleListNode<T>* ListPtr_ = _Head;
  T* Array_ = New<T>(_NumNodes);
  size_t i;

  for (i = 0; i < _NumNodes && ListPtr_; i++, ListPtr_ = ListPtr_->Next())
    Array_[i] = *ListPtr_->GetItem();

  return Array_;
}

/****************************************************************************/
template <class T>    
SingleList<T>& SingleList<T>::PtrArrayToList(T** Array_, size_t Size_)
{
  size_t i;

  if (Size_)
  {
    Flush();
    AssignListNode(SingleListNode<T>::Make(Array_[0], DataStorageAttributes::DEEP_COPY), DataStorageAttributes::DEEP_COPY);

    for (i = 1; i < Size_; i++)
      AddListNode(SingleListNode<T>::Make(Array_[i], DataStorageAttributes::DEEP_COPY), ListRelation::NEXT);
  }

  return *this;
}

/****************************************************************************/
template <class T>    
SingleList<T>& SingleList<T>::ArrayToList(T* Array_, size_t Size_)
{
  size_t i;

  if (Size_)
  {
    Flush();
    AssignListNode(SingleListNode<T>::Make(Array_[0]), DataStorageAttributes::DEEP_COPY);

    for (i = 1; i < Size_; i++)
      AddListNode(SingleListNode<T>::Make(Array_[i]), ListRelation::NEXT);
  }

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SingleList<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SingleList<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* SingleList<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SingleList<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SingleListAcceptor<T>::SingleListAcceptor(const SingleList<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void SingleListAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const SingleList<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* SingleListAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean SingleListAcceptor<T>::IsSingleList() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* SingleListAcceptor<T>::CreateFromSingleList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new SingleList<T>(*((const SingleList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* SingleListAcceptor<T>::CreateFromSingleSublist(const ObjectAcceptor& Obj_)
{
  _VarPtr = new SingleList<T>(*((const SingleSublist<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* SingleListAcceptor<T>::AssignFromSingleList(const ObjectAcceptor& Obj_)
{
  const SingleList<T>* TrgPtr_ = (const SingleList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* SingleListAcceptor<T>::AssignFromSingleSublist(const ObjectAcceptor& Obj_)
{
  const SingleSublist<T>* TrgPtr_ = (const SingleSublist<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean SingleListAcceptor<T>::IsEqualToSingleList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToSingleList((const SingleList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean SingleListAcceptor<T>::IsLesserToSingleList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToSingleList((const SingleList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean SingleListAcceptor<T>::IsGreaterToSingleList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToSingleList((const SingleList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SingleListAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SingleListAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if SINGLELIST_IMPLEMENT_STLCONTAINER
template <class T>
const SingleListIterator<T> SingleList<T>::begin() const
{
  return (SingleListIterator<T>(this));
}

/****************************************************************************/
template <class T>
SingleListIterator<T> SingleList<T>::begin()
{
  return (SingleListIterator<T>(this));
}

/****************************************************************************/
template <class T>
const SingleListIterator<T> SingleList<T>::end() const
{
  SingleListIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
SingleListIterator<T> SingleList<T>::end()
{
  SingleListIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
const ReverseIterator<T> SingleList<T>::rbegin() const
{
  SingleListIterator<T> Iter_(this);
  return ReverseIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
ReverseIterator<T> SingleList<T>::rbegin()
{
  SingleListIterator<T> Iter_(this);
  return ReverseIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
const ReverseIterator<T> SingleList<T>::rend() const
{
  SingleListIterator<T> Iter_(this);
  ReverseIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
ReverseIterator<T> SingleList<T>::rend()
{
  SingleListIterator<T> Iter_(this);
  ReverseIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
T& SingleList<T>::front()
{
  return (*begin());
}

/****************************************************************************/
template <class T>
const T& SingleList<T>::front() const
{
  return (*begin());
}

/****************************************************************************/
template <class T>
T& SingleList<T>::back()
{
  return *(--end());
}

/****************************************************************************/
template <class T>
const T& SingleList<T>::back() const
{
  return *(--end());
}

/****************************************************************************/
template <class T>
void SingleList<T>::push_back(const T& x)
{
  insert(end(), x);
}

/****************************************************************************/
template <class T>
void SingleList<T>::pop_back()
{
  SingleListIterator<T> Iter_ = end();
  --Iter_;
  erase(Iter_);
}

/****************************************************************************/
template <class T>
SingleListIterator<T> SingleList<T>::insert(SingleListIterator<T> pos, const T& x)
{
  --pos;

  if (pos.More())
    Insert(pos.Position(), x);

  return pos;
}

/****************************************************************************/
template <class T>
void SingleList<T>::insert(SingleListIterator<T> pos, size_t n, const T& x)
{
  --pos;

  if (pos.More())
  {
    size_t i;
    for (i = 0; i < n; ++i)
      Insert(pos.Position(), x);
  }
}

/****************************************************************************/
template <class T>
void SingleList<T>::insert(SingleListIterator<T> pos, const SingleListIterator<T> first, const SingleListIterator<T> last)
{
  --pos;

  if (pos.More())
    while (first != last && first.More())
    {
      T Obj_ = Get_At(first.Position());
      Insert(pos.Position(), Obj_);
      ++first;
    }
}

/****************************************************************************/
template <class T>
void SingleList<T>::erase(SingleListIterator<T> pos)
{
  if (pos.More())
  {
    SingleListNode<T>* ListPtr_ = GetNode(pos.Position());
    if (ListPtr_)
      ListPtr_->SetItem(T());
  }
}

/****************************************************************************/
template <class T>
void SingleList<T>::erase(SingleListIterator<T> first, SingleListIterator<T> last)
{
  while (first != last && first.More())
  {
    SingleListNode<T>* ListPtr_ = GetNode(first.Position());
    if (ListPtr_)
    {
      ListPtr_->SetItem(T());
      ++first;
    }
  }
}
#endif
/****************************************************************************/
/****************************************************************************/
#if SINGLELIST_IMPLEMENT_STLITERATOR
template <class T>
SingleListIterator<T>::SingleListIterator(const SingleList<T>* Ap_, Boolean DestroyFromIter_):
ForwardIterator<T>(),
_ArrayObjPtr(Ap_),
_Index(0)
{
  if (Ap_)
  {
    _ArrayVarPtr->RegisterIterator();
    if (DestroyFromIter_)
      _ArrayVarPtr->DestroyFromIterator();
  }
}

/****************************************************************************/
template <class T>
SingleListIterator<T>::SingleListIterator(const SingleListIterator<T>& Ap_):
ForwardIterator<T>(),
_ArrayObjPtr(Ap_._ArrayObjPtr),
_Index(Ap_._Index)
{
  if (_ArrayObjPtr)
    _ArrayVarPtr->RegisterIterator();
}

/****************************************************************************/
template <class T>
SingleListIterator<T>::~SingleListIterator()
{
  if (_ArrayObjPtr)
  {
    Boolean Destroy_ =
    _ArrayVarPtr->UnRegisterIterator();

    if (Destroy_)
      delete _ArrayVarPtr;
  }
}

/****************************************************************************/
template <class T>
SingleListIterator<T>* SingleListIterator<T>::Make(SingleList<T>* Ap_)
{
  return (new SingleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
SingleListIterator<T>* SingleListIterator<T>::Make(SingleListIterator<T>& Ap_)
{
  return (new SingleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const SingleListIterator<T>* SingleListIterator<T>::MakeConst(const SingleList<T>* Ap_)
{
  return (new SingleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const SingleListIterator<T>* SingleListIterator<T>::MakeConst(const SingleListIterator<T>& Ap_)
{
  return (new SingleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
int SingleListIterator<T>::ClassID() const
{
  return TypeInfo::SINGLELISTITERATOR_TYPE;
}

/****************************************************************************/
template <class T>
ForwardIterator<T>* SingleListIterator<T>::Clone()
{
  return (new SingleListIterator<T>(*this));
}

/****************************************************************************/
template <class T>
const ForwardIterator<T>* SingleListIterator<T>::Clone() const
{
  return (new SingleListIterator<T>(*this));
}

/****************************************************************************/
template <class T>
Boolean SingleListIterator<T>::IsValid(SizeType Index_) const
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (Index_ < _ArrayVarPtr->Size())
    return !(_ArrayVarPtr->GetNode(Index_)->GetItem() == ((T*)NULL));

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SingleListIterator<T>::IsValid() const
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (More())
    return !(_ArrayVarPtr->GetNode(_Index)->GetItem() == ((T*)NULL));

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SingleListIterator<T>::More() const
{
  if (_ArrayVarPtr->Size() == 0)
  {
    SingleListIterator<T>* const LocalThis_ = (SingleListIterator<T>* const)this;
    LocalThis_->_Index = SizeType(-1);
  }

  return !AtEnd();
}

/****************************************************************************/
template <class T>
void SingleListIterator<T>::Reset() const
{
  SingleListIterator<T>* const LocalThis_ = (SingleListIterator<T>* const)this;
  LocalThis_->_Index = 0;
}

/****************************************************************************/
template <class T>
void SingleListIterator<T>::SetToLast() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  SingleListIterator<T>* const LocalThis_ = (SingleListIterator<T>* const)this;
  LocalThis_->_Index = _ArrayObjPtr->Size() - 1;
}

/****************************************************************************/
template <class T>
void SingleListIterator<T>::SetToEnd() const
{
  SingleListIterator<T>* const LocalThis_ = (SingleListIterator<T>* const)this;
  LocalThis_->_Index = SizeType(-1);
}

/****************************************************************************/
template <class T>
const T* SingleListIterator<T>::ConstPointee() const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  if (More())
    return _ArrayObjPtr->GetNode(_Index)->GetItem();
  return _ArrayObjPtr->GetNode(0)->GetItem();
}

/****************************************************************************/
template <class T>
const T* SingleListIterator<T>::Pointee() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index)->GetItem();
  return _ArrayVarPtr->GetNode(0)->GetItem();
}

/****************************************************************************/
template <class T>
T* SingleListIterator<T>::Pointee()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index)->GetItem();
  return _ArrayVarPtr->GetNode(0)->GetItem();
}

/****************************************************************************/
template <class T>
const SingleListNode<T>* SingleListIterator<T>::ConstNode() const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  if (More())
    return _ArrayObjPtr->GetNode(_Index);
  return _ArrayObjPtr->GetNode(0);
}

/****************************************************************************/
template <class T>
const SingleListNode<T>* SingleListIterator<T>::Node() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index);
  return _ArrayVarPtr->GetNode(0);
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SingleListIterator<T>::Node()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index);
  return _ArrayVarPtr->GetNode(0);
}

/****************************************************************************/
template <class T>
const SingleListIterator<T>& SingleListIterator<T>::GotoHead() const
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>
const SingleListIterator<T>& SingleListIterator<T>::GotoTail() const
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
SingleListIterator<T>& SingleListIterator<T>::GotoHead()
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>    
SingleListIterator<T>& SingleListIterator<T>::GotoTail()
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
Boolean SingleListIterator<T>::AtHead() const
{
  return (_Index == 0);
}

/****************************************************************************/
template <class T>    
Boolean SingleListIterator<T>::AtTail() const
{
  return (_Index == _ArrayObjPtr->Size() - 1);
}

/****************************************************************************/
template <class T>
T& SingleListIterator<T>::operator * ()
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (More())
    return *(_ArrayVarPtr->GetNode(_Index)->GetItem());
  return *(_ArrayVarPtr->GetNode(0)->GetItem());
}

/****************************************************************************/
template <class T>
const T& SingleListIterator<T>::operator * () const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  if (More())
    return *(_ArrayVarPtr->GetNode(_Index)->GetItem());
  return *(_ArrayVarPtr->GetNode(0)->GetItem());
}

/****************************************************************************/
template <class T>
ForwardIterator<T>& SingleListIterator<T>::operator ++ ()
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (_Index < _ArrayVarPtr->Size() - 1)
    ++_Index;
  else
    _Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
ForwardIterator<T> SingleListIterator<T>::operator ++ (int)
{
  ForwardIterator<T> Ref_(*this);

  if (_ArrayVarPtr == NULL)
    Xnull();

  if (_Index < _ArrayVarPtr->Size() - 1)
    ++_Index;
  else
    _Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
const ForwardIterator<T>& SingleListIterator<T>::operator ++ () const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  SingleListIterator<T>* const LocalThis_ = (SingleListIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->Size() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const ForwardIterator<T> SingleListIterator<T>::operator ++ (int) const
{
  ForwardIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnull();

  SingleListIterator<T>* const LocalThis_ = (SingleListIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->Size() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
ForwardIterator<T> SingleListIterator<T>::operator + (SizeType Diff_)
{
  ForwardIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const ForwardIterator<T> SingleListIterator<T>::operator + (SizeType Diff_) const
{
  ForwardIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
ForwardIterator<T>& SingleListIterator<T>::operator += (SizeType Diff_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (_Index + Diff_ < _ArrayVarPtr->Size())
    _Index += Diff_;
  else
    _Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const ForwardIterator<T>& SingleListIterator<T>::operator += (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  SingleListIterator<T>* const LocalThis_ = (SingleListIterator<T>* const)this;

  if (_Index + Diff_ < _ArrayObjPtr->Size())
    LocalThis_->_Index += Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
SingleListIterator<T>& SingleListIterator<T>::SetData(const T& Obj_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd())
    _ArrayVarPtr->GetNode(_Index)->SetItem(Obj_);

  return *this;
}

/****************************************************************************/
template <class T>
SingleListIterator<T>& SingleListIterator<T>::SetData(T* Ptr_, int StoreFlags_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd())
    _ArrayVarPtr->GetNode(_Index)->SetItem(Ptr_, StoreFlags_);

  return *this;
}

/****************************************************************************/
template <class T>
SingleListIterator<T>& SingleListIterator<T>::Erase()
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd())
    _ArrayVarPtr->GetNode(_Index)->Erase();

  return *this;
}

/****************************************************************************/
template <class T>
SingleListIterator<T>& SingleListIterator<T>::operator = (T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd() && Ptr_)
    _ArrayVarPtr->GetNode(_Index)->SetItem(*Ptr_);

  return *this;
}

/****************************************************************************/
template <class T>
SingleListIterator<T>& SingleListIterator<T>::operator = (const SingleListIterator<T>& Rhs_)
{
  if (this != &Rhs_)
  {
    _Index = Rhs_._Index;
    _ArrayVarPtr = Rhs_._ArrayVarPtr;
  }

  return *this;
}
#endif
/****************************************************************************/
/****************************************************************************/
#if SINGLELIST_DEBUG
int Square(int x)
{ return (x * x); }

class Char
{
  DEFAULT_ROOTOBJECT_DEFN(Char)

  private:
    char ch;

  public:
    Char(char c = 'X'):
	ch(c) {};
    Char& Set(char c)
        { ch = c; return *this; }
    char val() const
	{ return (ch); }
};

int operator == (const Char& c1, const Char& c2)
	{ return (c1.val() == c2.val()); }

int operator == (const SingleList<Char>& x, const char* s)
{
  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

void ShowTheList(const SingleSublist<Char>& x)
{
  for (int i = 0; i < x.NumNodes(); ++i)
  {
    if (x.GetNode(i) && x.GetNode(i)->GetItem())
      cout <<x.GetNode(i)->GetItem()->val();
    cout <<",";
  }

  cout <<endl;
}

int operator == (const SingleList<Char>& x, const Char* s)
{
  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i].val())
      return 0;

  return 1;
}

int CompChar(const Char& c1, const Char& c2)
{
  return (c1.val() - c2.val());
}

SingleList<Char> operator + (const SingleList<Char>&, const Char&);
SingleList<Char> operator + (const Char&, const SingleList<Char>&);
SingleList<Char> operator + (const Char&, const SingleList<Char>&);

int main()
{
#if SINGLELIST_GENERAL_DEBUG
  int i; Boolean Ok_;
  Ushort Index_, Max_;
  SingleList<short> Argt_;
  SingleList<short> Args_;
  SingleList<short> Argv_;

  short x = 65;
  SingleListNode<short> sNode1_(x);
  Argt_.AssignListNode(&sNode1_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    Argt_.AddListNode(SingleListNode<short>::Make(x), ListRelation::NEXT);

  SingleListNode<short> sNode2_(x);
  Args_.AssignListNode(&sNode2_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    Args_.AddListNode(SingleListNode<short>::Make(x), ListRelation::NEXT);

  SingleListNode<short> sNode3_(x);
  Argv_.AssignListNode(&sNode3_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    Argv_.AddListNode(SingleListNode<short>::Make(x), ListRelation::NEXT);

  short Num41_ = 41;
  short Num7_ = 7;
  short Num3_ = 3;
  short Num99_ = 99;
  short Num67_ = 67;

  short* TempPtr_;
  short TempVal_;

  SingleListIterator<short> ArgsIter_(&Args_);

  ArgsIter_.SetData(Num41_); ArgsIter_++;
  ArgsIter_.SetData(Num7_); ArgsIter_++;
  ArgsIter_.SetData(Num3_); ArgsIter_++;
  ArgsIter_.SetData(Num99_); ArgsIter_++;
  ArgsIter_.SetData(Num67_);

// test1
  Argv_.GetNode(0)->SetItem(41);
  Argv_.GetNode(1)->SetItem(7);
  Argv_.GetNode(2)->SetItem(3);
  Argv_.GetNode(3)->SetItem(99);
  Argv_.GetNode(4)->SetItem(67);

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"SingleList = ";
#endif

  for (ArgsIter_.Reset(), i = 0; ArgsIter_.More(); ArgsIter_++, ++i)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    cout <<*(ArgsIter_.Pointee()) <<" ";
#endif
    Ok_ = (i == 0) ? (*ArgsIter_ == 41):
	  (i == 1) ? (*ArgsIter_ == 7):
	  (i == 2) ? (*ArgsIter_ == 3):
	  (i == 3) ? (*ArgsIter_ == 99):
	  (i == 4) ? (*ArgsIter_ == 67):0;
    assert(Ok_);
  }

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: " <<"Size == " <<Args_.Size();
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  assert(Args_.Size() == 5);
  
  SingleListIterator<short> ArgvIter_(&Argv_);
  for (i = 0; ArgvIter_.More(); ArgvIter_++, ++i)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    cout <<*(ArgvIter_.Node()->GetItem()) <<" ";
#endif
    Ok_ = (i == 0) ? (*ArgvIter_ == 41):
	  (i == 1) ? (*ArgvIter_ == 7):
	  (i == 2) ? (*ArgvIter_ == 3):
	  (i == 3) ? (*ArgvIter_ == 99):
	  (i == 4) ? (*ArgvIter_ == 67):0;
    assert(Ok_);
  }

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: " <<"Size == " <<Argv_.Size();
  cout <<endl;
#endif
  assert(Argv_.Size() == 5);

// test1
#if SINGLELIST_CLONE_TEST
  SingleList<short>* Newv_ = (SingleList<short>*)Argv_.Clone();
  *Newv_ = Argv_;

  (*Newv_).GetNode(0)->SetItem(128);
  (*Newv_).GetNode(4)->SetItem(512);

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"NewArray = ";
#endif
  SingleListIterator<short> NewvIter_(Newv_);
  for (i = 0; NewvIter_.More(); ++NewvIter_, ++i)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    if (NewvIter_.IsValid())
      cout <<*NewvIter_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*NewvIter_ == 128):
	  (i == 1) ? (*NewvIter_ == 7):
	  (i == 2) ? (*NewvIter_ == 3):
	  (i == 3) ? (*NewvIter_ == 99):
	  (i == 4) ? (*NewvIter_ == 512):
	  (i == 5) ? (NewvIter_.IsValid() == 0):0;
    assert(Ok_);
  }

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Newv_->Size() <<endl;
#endif
  assert(Newv_->Size() == 5);
  
#endif	// Array Clone test
// test1

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  SingleListIterator<short> ArgvIter2_(&Argv_);
  for (i = 0; ArgvIter2_.More(); ++ArgvIter2_, ++i)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    if (ArgvIter2_.IsValid())
      cout <<*ArgvIter2_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*ArgvIter2_ == 41):
	  (i == 1) ? (*ArgvIter2_ == 7):
	  (i == 2) ? (*ArgvIter2_ == 3):
	  (i == 3) ? (*ArgvIter2_ == 99):
	  (i == 4) ? (*ArgvIter2_ == 67):
	  (i == 5) ? (ArgvIter2_.IsValid() == 0):0;
    assert(Ok_);
  }

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size() <<endl;
#endif

 // test1
#if SINGLELIST_CLONE_TEST
  delete Newv_;
#endif
 // test1

  for (i = 5, x++; i < 11; i++, x++)
    Argv_.AddListNode(SingleListNode<short>::Make(x), ListRelation::NEXT);

  ArgvIter2_.Reset();
  ArgvIter2_.SetData(64); ArgvIter2_++;
  ArgvIter2_.SetData(128); ArgvIter2_++;
  ArgvIter2_.SetData(256); ArgvIter2_++;
  ArgvIter2_.Erase(); ArgvIter2_++;
  ArgvIter2_.SetData(1024); ArgvIter2_++;
  ArgvIter2_.Erase(); ArgvIter2_++;
  ArgvIter2_.SetData(4096); ArgvIter2_++;
  ArgvIter2_.Erase(); ArgvIter2_++;
  ArgvIter2_.SetData(16384); ArgvIter2_++;
  ArgvIter2_.SetData(2000); ArgvIter2_++;
  ArgvIter2_.SetData(1000);

  Argt_ = Argv_;

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  SingleListIterator<short> ArgvIter3_(&Argv_);
  for (Index_ = 0; ArgvIter3_.More(); ++ArgvIter3_, ++Index_)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    if (ArgvIter3_.IsValid())
      cout <<*ArgvIter3_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (Index_ == 0) ? (*ArgvIter3_ == 64):
	  (Index_ == 1) ? (*ArgvIter3_ == 128):
	  (Index_ == 2) ? (*ArgvIter3_ == 256):
	  (Index_ == 3) ? (ArgvIter3_.IsValid() == 0):
	  (Index_ == 4) ? (*ArgvIter3_ == 1024):
	  (Index_ == 5) ? (ArgvIter3_.IsValid() == 0):
	  (Index_ == 6) ? (*ArgvIter3_ == 4096):
	  (Index_ == 7) ? (ArgvIter3_.IsValid() == 0):
	  (Index_ == 8) ? (*ArgvIter3_ == 16384):
	  (Index_ == 9) ? (*ArgvIter3_ == 2000):
	  (Index_ == 10) ? (*ArgvIter3_ == 1000):0;
    assert(Ok_);
  }

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size();
#endif
  assert(Argv_.Size() == 11);

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl <<"Shrinking Down To 6";
  Argv_.ShrinkTo(6);
  cout <<endl <<"Shrinking Done";
  cout <<"\t: " <<"Size == " <<Argv_.Size();
  cout <<endl;
#endif
  Argv_.ShrinkTo(6);
  assert(Argv_.Size() == 6);

  Argv_.Flush();
#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<"SingleList Flushed";

  cout <<endl;
  cout <<"SingleList = ";
#endif
  for (ArgvIter3_.Reset(); ArgvIter3_.More(); ++ArgvIter3_)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    if (ArgvIter3_.IsValid())
      cout <<*ArgvIter3_ <<" ";
    else
      cout <<"VOID ";
#endif
    assert(ArgvIter3_.IsValid() == 0);
  }

#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"TSingleList = ";
#endif
  SingleListIterator<short> Tpiter_(&Argt_);
  for (i = 0; Tpiter_.More(); ++Tpiter_, ++i)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    if (Tpiter_.IsValid())
      cout <<*Tpiter_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*Tpiter_ == 64):
	  (i == 1) ? (*Tpiter_ == 128):
	  (i == 2) ? (*Tpiter_ == 256):
	  (i == 3) ? (Tpiter_.IsValid() == 0):
	  (i == 4) ? (*Tpiter_ == 1024):
	  (i == 5) ? (Tpiter_.IsValid() == 0):
	  (i == 6) ? (*Tpiter_ == 4096):
	  (i == 7) ? (Tpiter_.IsValid() == 0):
	  (i == 8) ? (*Tpiter_ == 16384):
	  (i == 9) ? (*Tpiter_ == 2000):
	  (i == 10) ? (*Tpiter_ == 1000):0;
    assert(Ok_);
  }

  SingleListNode<short> sNode4_(2141);
  Argv_.AssignListNode(&sNode4_, DataStorageAttributes::DEEP_COPY);
  Argv_.AddListNode(SingleListNode<short>::Make(227), ListRelation::NEXT);
  Argv_.AddListNode(SingleListNode<short>::Make(233), ListRelation::NEXT);
  Argv_.AddListNode(SingleListNode<short>::Make(2499), ListRelation::NEXT);
  Argv_.AddListNode(SingleListNode<short>::Make(2567), ListRelation::NEXT);

  // Testing virtual assignment method
  Argt_.Assign(Argv_);
#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"TSingleList = ";
#endif
  for (i = 0, Tpiter_.Reset(); Tpiter_.More(); ++Tpiter_, ++i)
  {
#if SINGLELIST_SHOW_DEBUG_OUTPUT
    if (Tpiter_.IsValid())
      cout <<*Tpiter_ <<" ";
    else
      cout <<"VOID ";
#endif
    Ok_ = (i == 0) ? (*Tpiter_ == 2141):
	  (i == 1) ? (*Tpiter_ == 227):
	  (i == 2) ? (*Tpiter_ == 233):
	  (i == 3) ? (*Tpiter_ == 2499):
	  (i == 4) ? (*Tpiter_ == 2567):(Tpiter_.IsValid() == 0);
    assert(Ok_);
  }

  // Testing null object methods
  Object* ObjPtr1_ = &Args_.NULLOBJECT();
  Object* ObjPtr2_ = &Argv_.NULLOBJECT();
  assert(ObjPtr1_->IsNullObject());
  assert(ObjPtr2_->IsNullObject());
#if SINGLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"NULL: " <<ObjPtr1_->IsNullObject() <<"\t" <<ObjPtr2_->IsNullObject();
#endif

#if SINGLELIST_NOTIFY_SUCCESS
  cout <<endl <<"SUCCESS General array classes testing" <<endl;
#endif
///
#endif

#if SINGLELIST_STANDARD_DEBUG
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  SingleList<Char> s1, s2, s3;
  Boolean Valid_;
  size_t i;

  Char* pc4;
  Char* pc5;

  SingleList<Char>::SetComparisonFunction(CompChar);
  SingleListNode<Char> sNode1_(c0);

  s3.AssignListNode(&sNode1_, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 3; i++)
    s3.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
    
  SingleList<Char> s4(s3);
  SingleList<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.AssignListNode(&sNode2_, DataStorageAttributes::DEEP_COPY);
  s6.AssignListNode(&sNode3_, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 9; i++)  
    s6.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);

  SingleList<Char> s7;
  SingleListNode<Char> sNode4_(c0);
  
  s7.AssignListNode(&sNode4_, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 3; i++)
    s7.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);  
  assert(s1 == "XX");
  s1.AddListNode(SingleListNode<Char>::Make(ca), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(ca), ListRelation::TAIL);
  assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "XXa");

  // test assigns
  SingleListNode<Char> sNode6_(cb);
  SingleListNode<Char> sNode7_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode7_, DataStorageAttributes::DEEP_COPY);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "bbbbb");
  s1.AssignListNode(
    SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE), DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "bXXX");

  // test appends
  s1 = s5;
  s1 += s5; assert(s1 == "aa");
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);
  assert(s1 == "aab");
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "aabb");
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(SingleListNode<Char>::Make(cb), ListRelation::TAIL);    
  assert(s1 == "aabbbbbbb");
  s1.AddListNode(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), ListRelation::TAIL);
  assert(s1 == "aabbbbbbba");  
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(SingleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "aabbbbbbbaXXX" && s1.Size() == 13);  
  SingleList<Char> s8(s1);
  s1 = s4 + s5; assert(s1 == "XXXXa" && s1.Size() == 5);

  // test inserts, removes, etc.
  s1 = s4;
  s1.AddListNodeAtIndex(SingleListNode<Char>::Make(*s5.GetNode(0)), 0);
  ShowTheList(s1);
  assert(s1 == "XaXXX" && s1.Size() == 5);
  s1.AddListNodeAtIndex(SingleListNode<Char>::Make(cb), 1);
  assert(s1 == "XabXXX" && s1.Size() == 6);
  s1.AddListNodeAtIndex(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), 5);
  assert(s1 == "XabXXXa" && s1.Size() == 7);

  delete s1.DetachFromMiddle(3);
  assert(s1 == "XabXXa" && s1.Size() == 6);

  pc4 = s1.ListToArray();
  SingleList<Char>* cs7p = (SingleList<Char>*)s7.Clone();
  pc5 = cs7p->ListToArray();
  assert(pc4[1] == ca && pc5[2] == c0);
  delete cs7p;
  ::Delete(pc4);
  ::Delete(pc5);

  // Test class hierarchy extensions
  // Test virtual object constructor
  pc5 = &c0;
  SingleList<Char>* ps1 = SingleList<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  SingleList<Char>* ps2 = SingleList<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test binary IO methods
  assert(!(s6 == *ps2));
  ofstream fout("testing.bin");
  Write(fout, *ps2);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, s6);
  fin.close();
  assert(s6 == *ps2);

  // testing series comparison methods
  s8.Sort();
  assert(s8 == "XXXaaabbbbbbb");

  SingleList<int> s9;
  SingleListNode<int> sNode0_(0);
  s9.AssignListNode(&sNode0_, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 10; i++)
    s9.AddListNode(SingleListNode<int>::Make(i * 2), ListRelation::NEXT);

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(*s9.GetNode(i)->GetItem() == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  SingleList<Char> s10 = s8;
  assert(s10 == s8);
  assert(s10.Compare(s8) == 0);
  s10.GetNode(8)->SetItem(Char('c'));
  s10.GetNode(10)->SetItem(Char('c'));
  SimpleArray<int> Result_ = s8.SeriesListCompare(s10);
  
  for (i = 0; i < Result_.GetCount(); i++)
    if (i == 8 || i == 10)
      assert(Result_[i] < 0);
    else
      assert(Result_[i] == 0);

  assert(s10 > s8);
  assert(s8 < s10);
  assert(s10.Compare(s8) > 0);
  assert(s8.Compare(s10) < 0);

  // Test class/object identification methods
  ShowInfo(cout, *ps2);
  cout <<endl;
  ShowTheList(*ps2);
  cout <<endl;

  delete ps1;
  delete ps2;

  /// customize testing
  s1.Flush();
  s2.Flush();
  s3.Flush();

  int x = 65;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s1.AssignListNode(&sNode9_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s1.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode10_(c3.Set(x));
  s2.AssignListNode(&sNode10_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x--; i < 5; i++, x--)
    s2.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 75;
  SingleListNode<Char> sNode11_(c3.Set(x));
  s3.AssignListNode(&sNode11_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s3.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2 += s3;
  assert(s2.Size() == 10);
  s1 += s2;

  assert(s1 == "ABCDEZYXWVKLMNO" && s1.Size() == 15);
  s1.ReverseList();
  assert(s1 == "ONMLKVWXYZEDCBA" && s1.Size() == 15);

  size_t Index_;
  SingleListNode<Char>* FoundNode_ = s1.FindItem(Char('V'), Index_);
  assert(FoundNode_ != NULL && Index_ == 5 && FoundNode_->GetItem()->val() == 'V');

  s1.Flush();
  s1 = s4;

  Char** ptrarr_ = new Char*[10];
  for (i = 0; i < 10; i++)
    ptrarr_[i] = new Char('0' + i);

  s1.Flush();
  s1.PtrArrayToList(ptrarr_, 10);
  assert(s1 == "0123456789");
  s1.GetNode(4)->SetItem(Char('F'));
  s1.GetNode(5)->SetItem(Char('E'));
  Char** ptrarr2_ = s1.ListToPtrArray();
  assert(ptrarr2_[0]->val() == '0');
  assert(ptrarr2_[1]->val() == '1');
  assert(ptrarr2_[2]->val() == '2');
  assert(ptrarr2_[3]->val() == '3');
  assert(ptrarr2_[4]->val() == 'F');
  assert(ptrarr2_[5]->val() == 'E');
  assert(ptrarr2_[6]->val() == '6');
  assert(ptrarr2_[7]->val() == '7');
  assert(ptrarr2_[8]->val() == '8');
  assert(ptrarr2_[9]->val() == '9');
  ::Delete(ptrarr2_);

  for (i = 0; i < 10; i++)
    delete ptrarr_[i];
  delete[] ptrarr_;

  pc4 = new Char[10];
  for (i = 0; i < 10; i++)
    pc4[i] = Char('0' + i);

  s1.Flush();
  s1.ArrayToList(pc4, 10);
  assert(s1 == "0123456789");
  s1.GetNode(4)->SetItem(Char('F'));
  s1.GetNode(5)->SetItem(Char('E'));
  pc5 = s1.ListToArray();
  assert(pc5[0].val() == '0');
  assert(pc5[1].val() == '1');
  assert(pc5[2].val() == '2');
  assert(pc5[3].val() == '3');
  assert(pc5[4].val() == 'F');
  assert(pc5[5].val() == 'E');
  assert(pc5[6].val() == '6');
  assert(pc5[7].val() == '7');
  assert(pc5[8].val() == '8');
  assert(pc5[9].val() == '9');
  ::Delete(pc5);

  delete[] pc4;

  s1 = s4;
  s2 = s4;
  s3 = s4;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s2));
  assert(s3.IsGreater(s2));

  assert(s1 == s4);
  assert(s1 < s2);
  assert(s3 > s2);
  assert(s1 != s3);

#if SINGLELIST_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <SingleList>" <<endl;
#endif
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




