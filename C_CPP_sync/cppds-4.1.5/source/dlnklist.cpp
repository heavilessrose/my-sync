#ifndef DOUBLELIST_CPP
#define DOUBLELIST_CPP
#ifndef DOUBLELIST_H
  #include "dlnklist.h"
#endif

/****************************************************************************/
template <class T>
int(*DoubleList<T>::_CompFunc)(const T&, const T&) = NULL;

/****************************************************************************/
/****************************************************************************/
template <class T>
DoubleListNode<T>::DoubleListNode():
_Next(NULL),
_Previous(NULL)
{}

/****************************************************************************/
template <class T>
DoubleListNode<T>::DoubleListNode(const T& Data_):
_Data(Data_),
_Next(NULL),
_Previous(NULL)
{}

/****************************************************************************/
template <class T>
DoubleListNode<T>::DoubleListNode(T* Data_, int StoreFlags_):
_Data(Data_, StoreFlags_),
_Next(NULL),
_Previous(NULL)
{}

/****************************************************************************/
template <class T>
DoubleListNode<T>::DoubleListNode(const DoubleListNode<T>& Node_):
_Data(Node_._Data),
_Next(NULL),
_Previous(NULL)
{
  if (Node_._Next)
  {
    _Next = Node_._Next->CloneNode();
    _Next->SetPrevious(this);
  }
}

/****************************************************************************/
template <class T>
DoubleListNode<T>::DoubleListNode(const DoubleListNode<T>& Node_, int, int):
_Data(Node_._Data),
_Next(NULL),
_Previous(NULL)
{}

/****************************************************************************/
template <class T>
DoubleListNode<T>::~DoubleListNode()
{
  if (_Previous)
    _Previous->SetNext(NULL);
  
  _Previous = NULL;
  
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
size_t DoubleListNode<T>::NumNodes(const DoubleListNode<T>* Node_)
{
  if (!Node_)
    return 0;

  size_t i;
  const DoubleListNode<T>* ListPtr_ = Node_->First();
  const DoubleListNode<T>* FirstPtr_ = ListPtr_;
  for (i = 1; ListPtr_->Next() && ListPtr_->Next() != FirstPtr_;
       i++, ListPtr_ = ListPtr_->Next());

  return i;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Make()
{
  return (new DoubleListNode<T>());
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Make(const T& Data_)
{
  return (new DoubleListNode<T>(Data_));
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Make(T* Destination_, int StoreFlags_)
{
  return (new DoubleListNode<T>(Destination_, StoreFlags_));
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Make(const DoubleListNode<T>& Node_)
{
  return (new DoubleListNode<T>(Node_));
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Make(const DoubleListNode<T>& Node_, int, int)
{
  return (new DoubleListNode<T>(Node_, 0, 0));
}

/****************************************************************************/
template <class T>
DoubleListNode<T>& DoubleListNode<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static DoubleListNode<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& DoubleListNode<T>::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& DoubleListNode<T>::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
int DoubleListNode<T>::ClassID() const
{
  return TypeInfo::DOUBLELISTNODE_TYPE;
}

/****************************************************************************/
template <class T>
Object* DoubleListNode<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (!ObjComp_)
    return (new DoubleListNode<T>(*this));

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::CloneNode() const
{
  return (new DoubleListNode<T>(*this));
}

/****************************************************************************/
template <class T>
int DoubleListNode<T>::CompareItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const
{
  if (!_Data.Pointee())
    Xnull();

  return ((*CmpFnc_)(*_Data.Pointee(), Obj_));
}

/****************************************************************************/
template <class T>
DoubleListNode<T>& DoubleListNode<T>::operator = (const DoubleListNode<T>& Node_)
{
  if (this != &Node_)
  {
    _Data = Node_._Data;
    _Next = NULL;
    _Previous = NULL;
    
    if (Node_._Next)
    {
      _Next = Node_._Next->CloneNode();
      _Next->SetPrevious(this);
    }
    else
      _Next = NULL;
  }

  return *this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::ReplaceData(DoubleListNode<T>* Node_)
{
  if (this != Node_)
    _Data = Node_->_Data;

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::ReplaceNode(DoubleListNode<T>* Node_)
{
  Node_->SetNext(Next());
  Node_->SetPrevious(Previous());

  if (Previous())
    Previous()->SetNext(Node_);

  if (Next())
    Next()->SetPrevious(Node_);

  SetNext(NULL);
  SetPrevious(NULL);

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Replace(DoubleListNode<T>* Sublist_)
{
  DoubleListNode<T>* Prev_ = Previous();
  DoubleListNode<T>* Next_ = Next();
  DoubleListNode<T>* Head_ = Sublist_->First();
  DoubleListNode<T>* Tail_ = Sublist_->Last();
  Boolean Found_ = FALSE;
  size_t Size_ = NumNodes(Sublist_);
  size_t i;

  for (i = 0; i < Size_ && Head_; i++)
    if (Found_ = (Head_ == this))
      break;
    else
      Head_ = Head_->Next();

  if (Found_)
    return this;

  Head_ = Sublist_->First();

  if (Sublist_ && Head_)
  {
    if (Next())
    {
      ReplaceData(Head_);
      Head_->ReplaceNode(this);
      delete Head_;
    }
    else
    {
      ReplaceData(Tail_);
      Tail_->ReplaceNode(this);
      delete Tail_;
    }

    DoubleListNode<T>* First_ = First();
    DoubleListNode<T>* Last_ = Last();

    First_->SetPrevious(Prev_);
    if (Prev_)
      Prev_->SetNext(First_);

    Last_->SetNext(Next_);
    if (Next_)
      Next_->SetPrevious(Last_);
  }

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Replace(DoubleSublist<T>* Sublist_)
{
  DoubleListNode<T>* Prev_ = Previous();
  DoubleListNode<T>* Next_ = Next();
  DoubleListNode<T>* Head_ = Sublist_->Head();
  Boolean Found_ = FALSE;
  size_t Size_ = Sublist_->NumNodes();
  size_t i;

  for (i = 0; i < Size_ && Head_; i++)
    if (Found_ = (Head_ == this))
      break;
    else
      Head_ = Head_->Next();

  if (Found_)
    return this;

  if (Sublist_ && Sublist_->Head())
  {
    Sublist_->ReleaseControl();

    if (Next())
    {
      ReplaceData(Sublist_->Head());
      Sublist_->SetHead(this);
    }
    else
    {
      ReplaceData(Sublist_->Tail());
      Sublist_->SetTail(this);
    }

    Sublist_->Head()->SetPrevious(Prev_);
    if (Prev_)
      Prev_->SetNext(Sublist_->Head());

    Sublist_->Tail()->SetNext(Next_);
    if (Next_)
      Next_->SetPrevious(Sublist_->Tail());
  }

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Add(DoubleListNode<T>* Node_, int Relation_)
{
  if (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL)
  {
    DoubleListNode<T>* Next_ = Next();
    SetNext(Node_->First());
    DoubleListNode<T>* Last_ = Node_->Last();
    Last_->SetNext(Next_);
    Node_->First()->SetPrevious(this);

    if (Next_)
      Next_->SetPrevious(Last_);
  }
  else if (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD)
  {
    DoubleListNode<T>* Prev_ = Previous();  
    SetPrevious(Node_->Last());
    DoubleListNode<T>* First_ = Node_->First();
    First_->SetPrevious(Prev_);
    Node_->Last()->SetNext(this);

    if (Prev_)
      Prev_->SetNext(First_);
  }

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Add(DoubleSublist<T>* Sublist_, int Relation_)
{
  Sublist_->ReleaseControl();

  if (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL)
  {
    DoubleListNode<T>* Next_ = Next();
    SetNext(Sublist_->Head());
    DoubleListNode<T>* Tail_ = Sublist_->Tail();
    Tail_->SetNext(Next_);
    Sublist_->Head()->SetPrevious(this);

    if (Next_)
      Next_->SetPrevious(Tail_);
  }
  else if (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD)
  {
    DoubleListNode<T>* Prev_ = Previous();  
    SetPrevious(Sublist_->Tail());
    DoubleListNode<T>* Head_ = Sublist_->Head();
    Head_->SetPrevious(Prev_);
    Sublist_->Tail()->SetNext(this);

    if (Prev_)
      Prev_->SetNext(Head_);
  }

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Erase()
{
  _Data.Destroy();
  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::SetItem(T* Ptr_, int StoreFlags_)
{
  _Data.AssignPtr(Ptr_, StoreFlags_);
  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::SetItem(const T& Obj_)
{
  _Data.AssignObject(Obj_);
  return this;
}

/****************************************************************************/
template <class T>
const T* DoubleListNode<T>::GetItem() const
{
  return _Data.ConstPointee();
}

/****************************************************************************/
template <class T>
T* DoubleListNode<T>::GetItem()
{
  return _Data.Pointee();
}

/****************************************************************************/
template <class T>
const T* DoubleListNode<T>::FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const
{
  return ((CompareItem(Obj_, CmpFnc_) == 0) ? _Data.Pointee():NULL);
}

/****************************************************************************/
template <class T>
T* DoubleListNode<T>::FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&))
{
  return ((CompareItem(Obj_, CmpFnc_) == 0) ? _Data.Pointee():NULL);
}

/****************************************************************************/
template <class T>
int DoubleListNode<T>::ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const
{
  if (!_Data.Pointee())
    Xnull();

  return ((*Fnc_)(*_Data.Pointee(), Obj_));
}

/****************************************************************************/
template <class T>
int DoubleListNode<T>::ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_)
{
  if (!_Data.Pointee())
    Xnull();

  return ((*Fnc_)(*_Data.Pointee(), Obj_));
}

/****************************************************************************/
template <class T>
int DoubleListNode<T>::ApplyUnary(int(*Fnc_)(T*))
{
  return ((*Fnc_)(_Data.Pointee()));
}

/****************************************************************************/
template <class T>
int DoubleListNode<T>::ApplyUnary(T(*Fnc_)(T))
{
  if (_Data.Pointee())
    SetItem((*Fnc_)(*_Data.Pointee()));

  return 1;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Duplicate(size_t NumNodes_)
{
  size_t i;
  for (i = 0; i < NumNodes_; i++)
    Add(CloneNode(), ListRelation::NEXT);

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::DetachNode()
{
  if (_Next)
    _Next->SetPrevious(_Previous);

  if (_Previous)
    _Previous->SetNext(_Next);

  SetNext(NULL);
  SetPrevious(NULL);

  return this;
}

/****************************************************************************/
template <class T>
DoubleSublist<T>* DoubleListNode<T>::DetachSublist(int Relation_, size_t NumNodes_, Boolean ToEnd_)
{
  if (ToEnd_)
    NumNodes_ = 0;

  if (ToEnd_ || NumNodes_)
  {
    size_t i;
    DoubleListNode<T>* Head_;
    DoubleListNode<T>* Tail_;
    DoubleListNode<T>* Prev_ = Previous();
    DoubleListNode<T>* Next_ = Next();
  
    if (Relation_ == ListRelation::NEXT)
    {
      if (_Previous)
        _Previous->SetNext(NULL);
        
      SetPrevious(NULL);    
      Tail_ = Head_ = this;

      for (i = 1; (i < NumNodes_) || ToEnd_; i++)
        if (Tail_->Next())
          Tail_ = Tail_->Next();
        else
          break;

      Next_ = Tail_->Next();
      if (Prev_)
        Prev_->SetNext(Next_);
      
      if (Next_)
        Next_->SetPrevious(Prev_);

      Tail_->SetNext(NULL);
      return (new DoubleSublist<T>(Head_, i, DataStorageAttributes::DESTROY));
    }
    else if (Relation_ == ListRelation::PREVIOUS)
    {
      if (_Next)
        _Next->SetPrevious(NULL);
        
      SetNext(NULL);
      Tail_ = Head_ = this;

      for (i = 1; (i < NumNodes_) || ToEnd_; i++)
        if (Head_->Previous())
          Head_ = Head_->Previous();
        else
          break;

      Prev_ = Head_->Previous();
      if (Next_)
        Next_->SetPrevious(Prev_);
      
      if (Prev_)
        Prev_->SetNext(Next_);

      Head_->SetPrevious(NULL);
      return (new DoubleSublist<T>(Head_, i, DataStorageAttributes::DESTROY));
    }
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleSublist<T>* DoubleListNode<T>::NewSublist(int Relation_, size_t NumNodes_, Boolean ToEnd_)
{
  if (ToEnd_)
    NumNodes_ = 0;

  if (ToEnd_ || NumNodes_)
  {
    size_t i;
    DoubleListNode<T>* Head_;
    DoubleListNode<T>* Tail_;
    DoubleListNode<T>* Prev_ = Previous();
    DoubleListNode<T>* Next_ = Next();
  
    if (Relation_ == ListRelation::NEXT)
    {
      if (_Previous)
        _Previous->SetNext(NULL);
        
      SetPrevious(NULL);    
      Tail_ = Head_ = this;

      for (i = 1; (i < NumNodes_) || ToEnd_; i++)
        if (Tail_->Next())
          Tail_ = Tail_->Next();
        else
          break;

      Next_ = Tail_->Next();
      if (Next_)
        Next_->SetPrevious(NULL);        
      Tail_->SetNext(NULL);
      
      DoubleSublist<T>* NewList_ = (new DoubleSublist<T>(Head_, i, DataStorageAttributes::DEEP_COPY));
      
      SetPrevious(Prev_);      
      if (Prev_)
        Prev_->SetNext(this);
        
      Tail_->SetNext(Next_);
      if (Tail_->Next())
        Tail_->Next()->SetPrevious(Tail_);
      
      return NewList_;
    }
    else if (Relation_ == ListRelation::PREVIOUS)
    {
      if (_Next)
        _Next->SetPrevious(NULL);
        
      SetNext(NULL);
      Tail_ = Head_ = this;

      for (i = 1; (i < NumNodes_) || ToEnd_; i++)
        if (Head_->Previous())
          Head_ = Head_->Previous();
        else
          break;

      Prev_ = Head_->Previous();
      if (Prev_)
        Prev_->SetNext(NULL);
      Head_->SetPrevious(NULL);

      DoubleSublist<T>* NewList_ = (new DoubleSublist<T>(Head_, i, DataStorageAttributes::DEEP_COPY));

      SetNext(Next_);
      if (Next_)
        Next_->SetPrevious(this);

      Head_->SetPrevious(Prev_);
      if (Head_->Previous())
        Head_->Previous()->SetNext(Head_);

      return NewList_;
    }
  }

  return NULL;
}

/****************************************************************************/
template <class T>
int DoubleListNode<T>::GetStorageAttributes() const
{
  return _Data.GetStorageAttributes();
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::SetNext(DoubleListNode<T>* Next_)
{
  _Next = Next_;
  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::SetPrevious(DoubleListNode<T>* Prev_)
{
  _Previous = Prev_;
  return this;
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleListNode<T>::Next(size_t Nodes_) const
{
  if (Nodes_)
    return (_Next ? _Next->Next(Nodes_ - 1):NULL);

  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Next(size_t Nodes_)
{
  if (Nodes_)
    return (_Next ? _Next->Next(Nodes_ - 1):NULL);
    
  return this;
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleListNode<T>::Last() const
{
  const DoubleListNode<T>* Current_ = this;
  const DoubleListNode<T>* Ptr_ = this;

  while (Ptr_->_Next && Ptr_->_Next != Current_)
    Ptr_ = Ptr_->_Next;

  return Ptr_;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Last()
{
  DoubleListNode<T>* Current_ = this;
  DoubleListNode<T>* Ptr_ = this;

  while (Ptr_->_Next && Ptr_->_Next != Current_)
    Ptr_ = Ptr_->_Next;

  return Ptr_;
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleListNode<T>::Previous(size_t Nodes_) const
{
  if (Nodes_)
    return (_Previous ? _Previous->Previous(Nodes_ - 1):NULL);
    
  return this;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::Previous(size_t Nodes_)
{
  if (Nodes_)
    return (_Previous ? _Previous->Previous(Nodes_ - 1):NULL);
    
  return this;
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleListNode<T>::First() const
{
  const DoubleListNode<T>* Current_ = this;
  const DoubleListNode<T>* Ptr_ = this;

  while (Ptr_->_Previous && Ptr_->_Previous != Current_)
    Ptr_ = Ptr_->_Previous;

  return Ptr_;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListNode<T>::First()
{
  DoubleListNode<T>* Current_ = this;
  DoubleListNode<T>* Ptr_ = this;

  while (Ptr_->_Previous && Ptr_->_Previous != Current_)
    Ptr_ = Ptr_->_Previous;

  return Ptr_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DoubleListNode<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DoubleListNode<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* DoubleListNode<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DoubleListNode<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
DoubleSublist<T>::DoubleSublist():
_Head(NULL),
_Tail(NULL),
_NumNodes(0)
{}

/****************************************************************************/
template <class T>
DoubleSublist<T>::DoubleSublist(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_):
_Head(Node_),
_Tail(NULL),
_NumNodes(NumNodes_),
_Data(Node_, StoreFlags_)
{
  _Head = _Data.Pointee();
  DoubleListNode<T>* ListPtr_ = _Head;

  size_t i;
  for (i = 0; i < _NumNodes && ListPtr_->Next(); i++)
    ListPtr_ = ListPtr_->Next();

  _Tail = ListPtr_;
}

/****************************************************************************/
template <class T>    
DoubleSublist<T>::DoubleSublist(DoubleSublist<T>* Sublist_, int StoreFlags_):
_Head(Sublist_->Head()),
_Tail(NULL),
_NumNodes(Sublist_->_NumNodes),
_Data(Sublist_->Head(), StoreFlags_)
{
  Sublist_->ReleaseControl();

  _Head = _Data.Pointee();
  DoubleListNode<T>* ListPtr_ = _Head;

  size_t i;
  for (i = 0; i < _NumNodes && ListPtr_->Next(); i++)
    ListPtr_ = ListPtr_->Next();

  _Tail = ListPtr_;    
}

/****************************************************************************/
template <class T>
DoubleSublist<T>::DoubleSublist(const DoubleSublist<T>& Sublist_):
_Head(NULL),
_Tail(NULL),
_NumNodes(Sublist_._NumNodes),
_Data(Sublist_._Data)
{
  _Head = _Data.Pointee();
  DoubleListNode<T>* ListPtr_ = _Head;

  size_t i;
  for (i = 0; i < _NumNodes && ListPtr_->Next(); i++)
    ListPtr_ = ListPtr_->Next();

  _Tail = ListPtr_;
}

/****************************************************************************/
template <class T>
DoubleSublist<T>* DoubleSublist<T>::Make()
{
  return (new DoubleSublist<T>());
}

/****************************************************************************/
template <class T>
DoubleSublist<T>* DoubleSublist<T>::Make(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_)
{
  return (new DoubleSublist<T>(Node_, NumNodes_, StoreFlags_));
}

/****************************************************************************/
template <class T>
DoubleSublist<T>* DoubleSublist<T>::Make(DoubleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new DoubleSublist<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
DoubleSublist<T>* DoubleSublist<T>::Make(const DoubleSublist<T>& Sublist_)
{
  return (new DoubleSublist<T>(Sublist_));
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
size_t DoubleSublist<T>::Partition(DoubleListNode<T>** List_, size_t lo, size_t hi,
                                   int Order_, int(*CmpFnc_)(const T&, const T&))
{
  short dir = -1;
  DoubleListNode<T>* pivot = List_[lo];  // pivot element

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
void DoubleSublist<T>::_QuickSort(DoubleListNode<T>** List_, size_t lo, size_t hi,
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
void DoubleSublist<T>::QuickSort(DoubleListNode<T>** List_, size_t lo, size_t hi,
                                 int Order_, int(*CmpFnc_)(const T&, const T&))
{
  if (!List_)
    Xnull();
  
  _QuickSort(List_, lo, hi, Order_, CmpFnc_);   // execute quick sort procedure
}

/****************************************************************************/
template <class T>
Boolean DoubleSublist<T>::Destroy()
{
  _Data.Destroy();
  _NumNodes = 0;
  _Head = _Tail = NULL;
  return TRUE;
}

/****************************************************************************/
template <class T>
DoubleSublist<T>* DoubleSublist<T>::CloneSublist() const
{
  return (new DoubleSublist<T>(*this));
}

/****************************************************************************/
template <class T>
int DoubleSublist<T>::CompareSublist(const DoubleSublist<T>& Sublist_,
                                     int(*CmpFnc_)(const T&, const T&)) const
{
  if (!_Head || !Sublist_._Head)
    return (_Head ? 1:
	    Sublist_._Head ? -1:0);

  const DoubleListNode<T>* lp1_ = Head();
  const DoubleListNode<T>* lp2_ = Sublist_.Head();

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
DoubleSublist<T>& DoubleSublist<T>::operator = (const DoubleSublist<T>& Sublist_)
{
  if (this != &Sublist_)
  {
    _Data.Destroy();

    _Head = _Tail = NULL;
    _NumNodes = Sublist_._NumNodes;
    _Data = Sublist_._Data;

    _Head = _Data.Pointee();
    DoubleListNode<T>* ListPtr_ = _Head;

    size_t i;
    for (i = 0; i < _NumNodes && ListPtr_->Next() && ListPtr_->Next() != _Head; i++)
      ListPtr_ = ListPtr_->Next();

    _Tail = ListPtr_;
  }

  return *this;
}

/****************************************************************************/
template <class T>
DoubleSublist<T>& DoubleSublist<T>::AssignSublist(DoubleSublist<T>* Sublist_, int StoreFlags_)
{
  if (this != Sublist_)
  {
    Sublist_->ReleaseControl();
    _Data.Destroy();
  
    _Head = _Tail = NULL;
    _NumNodes = Sublist_->_NumNodes;
    _Data.AssignPtr(Sublist_->Head(), StoreFlags_);
    
    _Head = _Data.Pointee();
    DoubleListNode<T>* ListPtr_ = _Head;

    size_t i;
    for (i = 0; i < _NumNodes && ListPtr_->Next() && ListPtr_->Next() != _Head; i++)
      ListPtr_ = ListPtr_->Next();

    _Tail = ListPtr_;
  }
  
  return *this;
}

/****************************************************************************/
template <class T>
DoubleSublist<T>& DoubleSublist<T>::AssignListNode(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_)
{
  if (Head() != Node_)
  {
    _Data.Destroy();
  
    _Head = _Tail = NULL;
    _NumNodes = NumNodes_;
    _Data.AssignPtr(Node_, StoreFlags_);

    _Head = _Data.Pointee();
    DoubleListNode<T>* ListPtr_ = _Head;

    size_t i;
    for (i = 0; i < _NumNodes && ListPtr_->Next() && ListPtr_->Next() != _Head; i++)
      ListPtr_ = ListPtr_->Next();

    _Tail = ListPtr_;
  }

  return *this;
}

/****************************************************************************/
template <class T>
DoubleSublist<T>& DoubleSublist<T>::AddSublist(DoubleSublist<T>* Sublist_, int Relation_)
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
      _Head->Add(Sublist_, ListRelation::PREVIOUS);
      DoubleListNode<T>* NewHead_ = _Head;
      while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
        NewHead_ = NewHead_->Previous();

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
      DoubleSublist<T>::operator = (*Sublist_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleSublist<T>::FindSublistItem(
  const T& Obj_, size_t& Index_,
  int(*CmpFnc_)(const T&, const T&),
  size_t StartIndex_, int Toward_) const
{
  const DoubleListNode<T>* ListPtr_;

  if (0 <= StartIndex_ && StartIndex_ < _NumNodes &&
      (ListPtr_ = GetNode(StartIndex_)))
  {
    if (Toward_ == ListRelation::TAIL)
    {
      for (Index_ = StartIndex_; Index_ < _NumNodes && ListPtr_;
           Index_++, ListPtr_ = ListPtr_->Next())
        if (ListPtr_->FindItem(Obj_, CmpFnc_))
          return ListPtr_;
    }
    else if (Toward_ == ListRelation::HEAD)
    {
      for (Index_ = StartIndex_; ListPtr_;
           Index_--, ListPtr_ = ListPtr_->Previous())
        if (ListPtr_->FindItem(Obj_, CmpFnc_))
          return ListPtr_;
        else if (!Index_)
          break;
    }
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleSublist<T>::FindSublistItem(
  const T& Obj_, size_t& Index_,
  int(*CmpFnc_)(const T&, const T&),
  size_t StartIndex_, int Toward_)
{
  DoubleListNode<T>* ListPtr_;

  if (0 <= StartIndex_ && StartIndex_ < _NumNodes &&
      (ListPtr_ = GetNode(StartIndex_)))
  {
    if (Toward_ == ListRelation::TAIL)
    {
      for (Index_ = StartIndex_; Index_ < _NumNodes && ListPtr_;
           Index_++, ListPtr_ = ListPtr_->Next())
        if (ListPtr_->FindItem(Obj_, CmpFnc_))
          return ListPtr_;
    }
    else if (Toward_ == ListRelation::HEAD)
    {
      for (Index_ = StartIndex_; ListPtr_;
           Index_--, ListPtr_ = ListPtr_->Previous())
        if (ListPtr_->FindItem(Obj_, CmpFnc_))
          return ListPtr_;
        else if (!Index_)
          break;
    }
  }

  return NULL;
}

/****************************************************************************/
template <class T>
int DoubleSublist<T>::ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const
{
  size_t Index_;
  DoubleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyBinary(Fnc_, Obj_);

  return 0;
}

/****************************************************************************/
template <class T>
int DoubleSublist<T>::ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_)
{
  size_t Index_;
  DoubleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyBinary(Fnc_, Obj_);

  return 0;
}

/****************************************************************************/
template <class T>
int DoubleSublist<T>::ApplyUnary(T(*Fnc_)(T))
{
  size_t Index_;
  DoubleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyUnary(Fnc_);

  return 0;
}

/****************************************************************************/
template <class T>
int DoubleSublist<T>::ApplyUnary(int(*Fnc_)(T*))
{
  size_t Index_;
  DoubleListNode<T>* ListPtr_ = _Head;

  for (Index_ = 0; Index_ < _NumNodes && ListPtr_;
       Index_++, ListPtr_ = ListPtr_->Next())
    ListPtr_->ApplyUnary(Fnc_);

  return 0;
}

/****************************************************************************/
template <class T>
int DoubleSublist<T>::GetStorageAttributes() const
{
  return _Data.GetStorageAttributes();
}

/****************************************************************************/
template <class T>
void DoubleSublist<T>::SetHead(DoubleListNode<T>* Node_)
{
  if (_Head)
  {
    Node_->SetNext(_Head->Next());
    Node_->SetPrevious(NULL);
    _Head->DetachNode();
    DoubleListNode<T>* OldNode_ = _Head;
    _Data.ReleaseControl();
    _Head = Node_;
    DoubleListNode<T>* Previous_ = _Head->Next()->Previous();
    _Head->Next()->SetPrevious(_Head);
    if (Previous_)
      Previous_->SetNext(_Head);
    delete OldNode_;
  }
}

/****************************************************************************/
template <class T>
void DoubleSublist<T>::SetTail(DoubleListNode<T>* Node_)
{
  if (_Tail)
  {
    Node_->SetPrevious(_Tail->Previous());
    Node_->SetNext(NULL);
    _Tail->DetachNode();
    DoubleListNode<T>* OldNode_ = _Tail;
    _Tail = Node_;
    DoubleListNode<T>* Next_ = _Tail->Previous()->Next();
    _Tail->Previous()->SetNext(_Tail);
    if (Next_)
      Next_->SetPrevious(_Tail);
    delete OldNode_;
  }
}

/****************************************************************************/
template <class T>
void DoubleSublist<T>::SortSublist(int Order_, int(*CmpFnc_)(const T&, const T&))
{
  DoubleListNode<T>* ListPtr_ = _Head;
  DoubleListNode<T>** Array_ = new DoubleListNode<T>*[_NumNodes];
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
void DoubleSublist<T>::Relink(DoubleListNode<T>** Array_)
{
  size_t i;

  if (Array_[0] && _NumNodes)
    Array_[0]->SetPrevious(NULL);
  
  for (i = 0; i < _NumNodes; i++)
    if (i + 1 < _NumNodes && Array_[i] && Array_[i + 1])
    {
      Array_[i]->SetNext(Array_[i + 1]);
      Array_[i + 1]->SetPrevious(Array_[i]);
    }

  Array_[i - 1]->SetNext(NULL);
}

/****************************************************************************/
template <class T>    
const DoubleListNode<T>* DoubleSublist<T>::GetNode(size_t Index_) const
{
  DoubleListNode<T>* ListPtr_ = _Head;
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
DoubleListNode<T>* DoubleSublist<T>::GetNode(size_t Index_)
{
  DoubleListNode<T>* ListPtr_ = _Head;
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
size_t DoubleSublist<T>::NumNodes() const
{
  return _NumNodes;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DoubleSublist<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void DoubleSublist<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* DoubleSublist<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DoubleSublist<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleArray<int> SeriesListCompare(const DoubleSublist<T> &Sublist1_,
                               const DoubleSublist<T> &Sublist2_)
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
DoubleList<T>::DoubleList():
_Limit(SIZET_MAX),
_IteratorsActive(0),
_DestroyFromIterator(FALSE)
{}

/****************************************************************************/
template <class T>
DoubleList<T>::DoubleList(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_):
DoubleSublist<T>(Node_, NumNodes_, StoreFlags_),
_Limit(SIZET_MAX),
_IteratorsActive(0),
_DestroyFromIterator(FALSE)
{}

/****************************************************************************/
template <class T>
DoubleList<T>::DoubleList(DoubleList<T>* List_, int StoreFlags_):
DoubleSublist<T>(List_, StoreFlags_),
_Limit(List_->_Limit),
_IteratorsActive(List_->_IteratorsActive),
_DestroyFromIterator(List_->_DestroyFromIterator)
{}

/****************************************************************************/
template <class T>
DoubleList<T>::DoubleList(DoubleSublist<T>* Sublist_, int StoreFlags_):
DoubleSublist<T>(Sublist_, StoreFlags_),
_Limit(SIZET_MAX),
_IteratorsActive(0),
_DestroyFromIterator(FALSE)
{}

/****************************************************************************/
template <class T>
DoubleList<T>::DoubleList(const DoubleList<T>& List_):
DoubleSublist<T>(List_),
_Limit(List_._Limit),
_IteratorsActive(List_._IteratorsActive),
_DestroyFromIterator(List_._DestroyFromIterator)
{}

/****************************************************************************/
template <class T>
DoubleList<T>::DoubleList(const DoubleSublist<T>& Sublist_):
DoubleSublist<T>(Sublist_),
_Limit(SIZET_MAX),
_IteratorsActive(0),
_DestroyFromIterator(FALSE)
{}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::Make()
{
  return (new DoubleList<T>());
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::Make(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_)
{
  return (new DoubleList<T>(Node_, NumNodes_, StoreFlags_));
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::Make(DoubleList<T>* List_, int StoreFlags_)
{
  return (new DoubleList<T>(List_, StoreFlags_));
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::Make(DoubleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new DoubleList<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::Make(const DoubleList<T>& List_)
{
  return (new DoubleList<T>(List_));
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::Make(const DoubleSublist<T>& Sublist_)
{
  return (new DoubleList<T>(Sublist_));
}

/****************************************************************************/
template <class T>
void DoubleList<T>::Flush()
{
  Destroy();
}

/****************************************************************************/
template <class T>
SimpleArray<int> DoubleList<T>::SeriesListCompare(const DoubleSublist<T> &Sublist_) const
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
int DoubleList<T>::Compare(const DoubleSublist<T>& Sublist_) const
{
  if (_CompFunc)
    return CompareSublist(Sublist_, _CompFunc);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsEqualToDoubleList(const DoubleSublist<T>* Sublist_) const
{
  if (_CompFunc && Sublist_)
    return (CompareSublist(*Sublist_, _CompFunc) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsLesserToDoubleList(const DoubleSublist<T>* Sublist_) const
{
  if (_CompFunc && Sublist_)
    return (CompareSublist(*Sublist_, _CompFunc) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsGreaterToDoubleList(const DoubleSublist<T>* Sublist_) const
{
  if (_CompFunc && Sublist_)
    return (CompareSublist(*Sublist_, _CompFunc) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DoubleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToDoubleList(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DoubleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToDoubleList(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DoubleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToDoubleList(TrgComp_);
}

/****************************************************************************/
template <class T>
void DoubleList<T>::IgnoreElement(ByteStream& Is_, Boolean* Ok_)
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
void DoubleList<T>::ReadElement(ByteStream& Is_, DoubleListNode<T>* ListPtr_, Boolean* Ok_)
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
void DoubleList<T>::WriteElement(ByteStream& Os_, const DoubleListNode<T>* ListPtr_, Boolean* Ok_) const
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
void DoubleList<T>::IgnoreElement(istream& Is_, Boolean* Ok_)
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
void DoubleList<T>::ReadElement(istream& Is_, DoubleListNode<T>* ListPtr_, Boolean* Ok_)
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
void DoubleList<T>::WriteElement(ostream& Os_, const DoubleListNode<T>* ListPtr_, Boolean* Ok_) const
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
istream& DoubleList<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& DoubleList<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  DoubleListNode<T>* ListPtr_;
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Flush();
    Is_.read((char*)(&Size_), sizeof(long));

    if (Size_)
    {
      DoubleListNode<T> dNode1_;
      AssignListNode(&dNode1_, 1, DataStorageAttributes::DEEP_COPY);
      for (Index_ = 1; Index_ < Size_; Index_++)
        AddListNode(DoubleListNode<T>::Make(), ListRelation::NEXT);
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
ostream& DoubleList<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  const DoubleListNode<T>* ListPtr_;
  Boolean RdChk_;
  long RunLength_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    RunLength_ = (long)Size();
    Os_.write((char*)(&RunLength_), sizeof(long));

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
ByteStream& DoubleList<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& DoubleList<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  DoubleListNode<T>* ListPtr_;
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
      DoubleListNode<T> dNode1_;
      AssignListNode(&dNode1_, 1, DataStorageAttributes::DEEP_COPY);
      for (Index_ = 1; Index_ < Size_; Index_++)
        AddListNode(DoubleListNode<T>::Make(), ListRelation::NEXT);
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
ByteStream& DoubleList<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  const DoubleListNode<T>* ListPtr_;
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
ostream& DoubleList<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
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
long DoubleList<T>::DataSize() const
{
  const T* ItemPtr_ = NULL;
  const DoubleListNode<T>* ListPtr_ = Head();
  long DataSize_ = 0;

  for (size_t Index_ = 0, Max_ = Size();
       Index_ < Max_ && ListPtr_; Index_++, ListPtr_ = ListPtr_->Next())
    if (ItemPtr_ = ListPtr_->GetItem())
      DataSize_ += SizeOfData(*ItemPtr_);

  return DataSize_;
}

/****************************************************************************/
template <class T>
long DoubleList<T>::StorableFormWidth() const
{
  const T* ItemPtr_ = NULL;
  const DoubleListNode<T>* ListPtr_ = Head();

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
T* DoubleList<T>::MakeByTypeCode(int Code_)
{
  // If pointer to function is null then throw an exception.
  if (_Factory == NULL)
    Xnull();

  // Create object based on type code.
  return (*_Factory)(Code_);
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleList<T>::FindItem(const T& Obj_, size_t& Index_, size_t StartIndex_, int Toward_) const
{
  if (_CompFunc)
    return FindSublistItem(Obj_, Index_, _CompFunc, StartIndex_, Toward_);

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleList<T>::FindItem(const T& Obj_, size_t& Index_, size_t StartIndex_, int Toward_)
{
  if (_CompFunc)
    return FindSublistItem(Obj_, Index_, _CompFunc, StartIndex_, Toward_);

  return NULL;
}

/****************************************************************************/
template <class T>
void DoubleList<T>::Sort(int Order_)
{
  if (_CompFunc)
    SortSublist(Order_, _CompFunc);
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static DoubleList<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& DoubleList<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return DoubleList<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& DoubleList<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return DoubleList<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int DoubleList<T>::ClassID() const
{
  return TypeInfo::DOUBLELIST_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* DoubleList<T>::GiveObjectAcceptor() const
{
  return (new DoubleListAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Object& DoubleList<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  DoubleListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromDoubleList(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* DoubleList<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new DoubleList<T>(*this));

  DoubleListAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromDoubleList(TrgComp_)));
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::CloneList() const
{
  return (new DoubleList<T>(*this));
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::operator += (const DoubleSublist<T>& List_)
{
  DoubleSublist<T>* Sublist_ = new DoubleSublist<T>(List_);
  AddList(Sublist_, ListRelation::TAIL);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::AddList(DoubleSublist<T>* List_, int Relation_, Boolean DelArgList_)
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
Boolean DoubleList<T>::AddList(DoubleSublist<T>* List_, DoubleListNode<T>* IndexNode_, int Relation_, Boolean DelArgList_)
{
  if (List_ && List_->NumNodes() + Size() <= Limit() && IndexNode_ != NULL)
  {
    Boolean AtTail_ = IndexNode_ == Tail();
    Boolean AtHead_ = IndexNode_ == Head();
    List_->ReleaseControl();
    IndexNode_->Add(List_, Relation_);

    if (AtHead_ &&
       (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD))
    {
      DoubleListNode<T>* NewHead_ = List_->Head();
      if (NewHead_ != _Head)
      {
        _Data.ReleaseControl();
        _Data.AssignPtr(NewHead_, GetStorageAttributes());
        _Head = _Data.Pointee();
        _Tail = _Head->Last();
      }
    }

    if (AtTail_ &&
       (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL))
      _Tail = List_->Tail();

    IncreaseSize(List_->NumNodes());
    if (List_->IsNewed() && DelArgList_)
      delete List_;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::AddListAtIndex(DoubleSublist<T>* List_, size_t Index_, int Relation_, Boolean DelArgList_)
{
  if (List_ && List_->NumNodes() + Size() <= Limit() && Index_ < Size())
  {
    List_->ReleaseControl();
    GetNode(Index_)->Add(List_, Relation_);

    if (Index_ == 0 &&
       (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD))
    {
      DoubleListNode<T>* NewHead_ = List_->Head();
      if (NewHead_ != _Head)
      {
        _Data.ReleaseControl();
        _Data.AssignPtr(NewHead_, GetStorageAttributes());
        _Head = _Data.Pointee();
        _Tail = _Head->Last();
      }
    }

    if (Index_ == Size() - 1 &&
       (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL))
      _Tail = List_->Tail();

    IncreaseSize(List_->NumNodes());
    if (List_->IsNewed() && DelArgList_)
      delete List_;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::AddListNode(DoubleListNode<T>* Node_, int Relation_)
{
  size_t Size_ = DoubleListNode<T>::NumNodes(Node_);
  
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
      DoubleListNode<T>* NewHead_ = _Head;
      while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
        NewHead_ = NewHead_->Previous();

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
      AssignListNode(Node_, Size_, Node_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::AddListNode(DoubleListNode<T>* Node_, DoubleListNode<T>* IndexNode_, int Relation_)
{
  size_t Size_ = DoubleListNode<T>::NumNodes(Node_);
  
  if (Node_ && (Size() + Size_ <= Limit()) && IndexNode_ != NULL)
  {
    Boolean AtTail_ = IndexNode_ == Tail();
    Boolean AtHead_ = IndexNode_ == Head();
    IndexNode_->Add(Node_, Relation_);

    if (AtHead_ &&
       (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD))
    {
      _Data.ReleaseControl();
      _Data.AssignPtr(Node_, GetStorageAttributes());
      _Head = _Data.Pointee();
      _Tail = _Head->Last();
    }

    if (AtTail_ &&
       (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL))
      _Tail = Node_->Last();

    IncreaseSize(Size_);
    return TRUE;  
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::AddListNodeAtIndex(DoubleListNode<T>* Node_, size_t Index_, int Relation_)
{
  size_t Size_ = DoubleListNode<T>::NumNodes(Node_);
  
  if (Node_ && (Size() + Size_ <= Limit()) && Index_ < Size())
  {
    GetNode(Index_)->Add(Node_, Relation_);

    if (Index_ == 0 &&
       (Relation_ == ListRelation::PREVIOUS || Relation_ == ListRelation::HEAD))
    {
      _Data.ReleaseControl();
      _Data.AssignPtr(Node_, GetStorageAttributes());
      _Head = _Data.Pointee();
      _Tail = _Head->Last();
    }

    if (Index_ == Size() - 1 &&
       (Relation_ == ListRelation::NEXT || Relation_ == ListRelation::TAIL))
      _Tail = Node_->Last();

    IncreaseSize(Size_);
    return TRUE;  
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::operator = (const DoubleList<T>& List_)
{
  if (this != &List_)
  {
    DoubleSublist<T>::operator = (List_);

    _IteratorsActive = List_._IteratorsActive;
    _DestroyFromIterator = List_._DestroyFromIterator;
    _Limit = List_._Limit;
    _CompFunc = List_._CompFunc;
  }

  return *this;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::AssignList(DoubleList<T>* List_, int StoreFlags_)
{
  if (this != List_)
  {
    DoubleSublist<T>::AssignSublist(List_, StoreFlags_);

    _IteratorsActive = List_->_IteratorsActive;
    _DestroyFromIterator = List_->_DestroyFromIterator;
    _Limit = List_->_Limit;
    _CompFunc = List_->_CompFunc;
  }

  return *this;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::AssignListNode(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_)
{
  DoubleSublist<T>::AssignListNode(Node_, NumNodes_, StoreFlags_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::ReplaceList(DoubleSublist<T>* Sublist_, int Relation_, Boolean DelArgList_)
{
  size_t Size_ = Sublist_->NumNodes();

  if (Sublist_ && (Size() - 1 + Size_ <= Limit()))
  {
    Sublist_->ReleaseControl();

    if ((Relation_ == ListRelation::TAIL ||
         Relation_ == ListRelation::NEXT) && _Tail)
    {
      _Tail->Replace(Sublist_);
      while (_Tail->Next() && _Tail->Next() != _Head)
        _Tail = _Tail->Next();
      _NumNodes += (Size_ - 1);
    }
    else if ((Relation_ == ListRelation::HEAD ||
              Relation_ == ListRelation::PREVIOUS) && _Head)
    {
      _Head->Replace(Sublist_);

      DoubleListNode<T>* NewHead_ = _Head;
      while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
        NewHead_ = NewHead_->Previous();

      if (NewHead_ != _Head)
      {
        _Data.ReleaseControl();
        _Data.AssignPtr(NewHead_, GetStorageAttributes());
        _Head = _Data.Pointee();
        _Tail = _Head->Last();
      }

      if (Sublist_->IsNewed() && DelArgList_)
        delete Sublist_;

      _NumNodes += (Size_ - 1);
    }
    else
      AssignSublist(Sublist_, Sublist_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::ReplaceList(DoubleSublist<T>* Sublist_, DoubleListNode<T>* IndexNode_, Boolean DelArgList_)
{
  size_t Size_ = Sublist_->NumNodes();

  if (Sublist_ && (Size() - 1 + Size_ <= Limit()) && Index_ != NULL)
  {
    Boolean AtHead_ = IndexNode_ == Head();
    Boolean AtTail_ = IndexNode_ == Tail();
    Sublist_->ReleaseControl();

    if (AtTail_)
      ReplaceList(Sublist_, ListRelation::TAIL);
    else if (AtHead_)
      ReplaceList(Sublist_, ListRelation::HEAD);
    else if (_Head && Size())
    {
      DoubleListNode<T>* NodePtr_ = IndexNode_;
      
      if (NodePtr_)
      {
        NodePtr_->Replace(Sublist_);

        DoubleListNode<T>* NewHead_ = _Head;
        while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
          NewHead_ = NewHead_->Previous();

        if (NewHead_ != _Head)
        {
          _Data.ReleaseControl();
          _Data.AssignPtr(NewHead_, GetStorageAttributes());
          _Head = _Data.Pointee();
          _Tail = _Head->Last();
        }

        if (Sublist_->IsNewed() && DelArgList_)
          delete Sublist_;

        _NumNodes += (Size_ - 1);
      }
    }
    else
      AssignSublist(Sublist_, Sublist_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::ReplaceListAtIndex(DoubleSublist<T>* Sublist_, size_t Index_, Boolean DelArgList_)
{
  size_t Size_ = Sublist_->NumNodes();

  if (Sublist_ && (Size() - 1 + Size_ <= Limit()) && Index_ < Size())
  {
    Sublist_->ReleaseControl();

    if (Index_ == Size() - 1)
      ReplaceList(Sublist_, ListRelation::TAIL);
    else if (Index_ == 0)
      ReplaceList(Sublist_, ListRelation::HEAD);
    else if (_Head && Size())
    {
      DoubleListNode<T>* NodePtr_ = GetNode(Index_);
      
      if (NodePtr_)
      {
        NodePtr_->Replace(Sublist_);

        DoubleListNode<T>* NewHead_ = _Head;
        while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
          NewHead_ = NewHead_->Previous();

        if (NewHead_ != _Head)
        {
          _Data.ReleaseControl();
          _Data.AssignPtr(NewHead_, GetStorageAttributes());
          _Head = _Data.Pointee();
          _Tail = _Head->Last();
        }

        if (Sublist_->IsNewed() && DelArgList_)
          delete Sublist_;

        _NumNodes += (Size_ - 1);
      }
    }
    else
      AssignSublist(Sublist_, Sublist_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::ReplaceListNode(DoubleListNode<T>* Node_, int Relation_)
{
  size_t Size_ = DoubleListNode<T>::NumNodes(Node_);

  if (Node_ && (Size() - 1 + Size_ <= Limit()))
  {
    if ((Relation_ == ListRelation::TAIL ||
         Relation_ == ListRelation::NEXT) && _Tail)
    {
      _Tail->Replace(Node_);
      while (_Tail->Next() && _Tail->Next() != _Head)
        _Tail = _Tail->Next();
      _NumNodes += (Size_ - 1);
    }
    else if ((Relation_ == ListRelation::HEAD ||
              Relation_ == ListRelation::PREVIOUS) && _Head)
    {
      _Head->Replace(Node_);

      DoubleListNode<T>* NewHead_ = _Head;
      while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
        NewHead_ = NewHead_->Previous();

      if (NewHead_ != _Head)
      {
        _Data.ReleaseControl();
        _Data.AssignPtr(NewHead_, GetStorageAttributes());
        _Head = _Data.Pointee();
        _Tail = _Head->Last();
      }

      _NumNodes += (Size_ - 1);
    }
    else
      AssignListNode(Node_, Size_, Node_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::ReplaceListNode(DoubleListNode<T>* Node_, DoubleListNode<T>* IndexNode_)
{
  size_t Size_ = DoubleListNode<T>::NumNodes(Node_);

  if (Node_ && (Size() - 1 + Size_ <= Limit()) && Index_ != NULL)
  {
    Boolean AtHead_ = IndexNode_ == Head();
    Boolean AtTail_ = IndexNode_ == Tail();

    if (AtTail_)
      ReplaceListNode(Node_, ListRelation::TAIL);
    else if (AtHead_)
      ReplaceListNode(Node_, ListRelation::HEAD);
    else if (_Head && Size())
    {
      DoubleListNode<T>* NodePtr_ = IndexNode_;
      
      if (NodePtr_)
      {
        NodePtr_->Replace(Node_);

        DoubleListNode<T>* NewHead_ = _Head;
        while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
          NewHead_ = NewHead_->Previous();

        if (NewHead_ != _Head)
        {
          _Data.ReleaseControl();
          _Data.AssignPtr(NewHead_, GetStorageAttributes());
          _Head = _Data.Pointee();
          _Tail = _Head->Last();
        }

        _NumNodes += (Size_ - 1);
      }
    }
    else
      AssignListNode(Node_, Size_, Node_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::ReplaceListNodeAtIndex(DoubleListNode<T>* Node_, size_t Index_)
{
  size_t Size_ = DoubleListNode<T>::NumNodes(Node_);

  if (Node_ && (Size() - 1 + Size_ <= Limit()) && Index_ < Size())
  {
    if (Index_ == Size() - 1)
      ReplaceListNode(Node_, ListRelation::TAIL);
    else if (Index_ == 0)
      ReplaceListNode(Node_, ListRelation::HEAD);
    else if (_Head && Size())
    {
      DoubleListNode<T>* NodePtr_ = GetNode(Index_);
      
      if (NodePtr_)
      {
        NodePtr_->Replace(Node_);

        DoubleListNode<T>* NewHead_ = _Head;
        while (NewHead_->Previous() && NewHead_->Previous() != _Tail)
          NewHead_ = NewHead_->Previous();

        if (NewHead_ != _Head)
        {
          _Data.ReleaseControl();
          _Data.AssignPtr(NewHead_, GetStorageAttributes());
          _Head = _Data.Pointee();
          _Tail = _Head->Last();
        }

        _NumNodes += (Size_ - 1);
      }
    }
    else
      AssignListNode(Node_, Size_, Node_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::DetachFromHead(size_t NumNodes_)
{
  if (!NumNodes_)
    return NULL;

  DoubleListNode<T>* NewHead_ = _Head->Next(NumNodes_);
  size_t Size_ = Size();
  Boolean Valid_ = (_Head != NULL) && ((Size_ > NumNodes_) ? (NewHead_ != NULL && NumNodes_ <= Size_):Size_);

  if (Valid_)
  {
    DoubleList<T>* NewList_ =
    new DoubleList<T>(_Head->DetachSublist(ListRelation::NEXT, NumNodes_), _Data.GetStorageAttributes());
    _Data.ReleaseControl();

    if (NewHead_)
    {
      _Data.AssignPtr(NewHead_, GetStorageAttributes());
      _Head = _Data.Pointee();
      _Tail = _Head->Last();
    }
    else
      _Head = _Tail = NULL;
    
    _NumNodes -= NumNodes_;
    return NewList_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::DetachFromTail(size_t NumNodes_)
{
  if (!NumNodes_)
    return NULL;

  DoubleListNode<T>* NewTail_ = _Tail->Previous(NumNodes_);
  size_t Size_ = Size();
  Boolean Valid_ = (_Tail != NULL) && ((Size_ > NumNodes_) ? (NewTail_ != NULL && NumNodes_ <= Size_):Size_);

  if (Valid_)
  {
    DoubleList<T>* NewList_ =
    new DoubleList<T>(_Tail->DetachSublist(ListRelation::PREVIOUS, NumNodes_), _Data.GetStorageAttributes());
    DoubleListNode<T>* NewHead_ = NULL;

    if (NewTail_)
    {
      _Tail = NewTail_;
      NewHead_ = _Tail->First();

      if (NewHead_ != _Head)
      {
        _Data.ReleaseControl();
        _Data.AssignPtr(NewHead_, GetStorageAttributes());
        _Head = _Data.Pointee();
        _Tail = _Head->Last();
      }      
    }
    else
    {
      _Data.ReleaseControl();
      _Head = _Tail = NULL;    
    }

    _NumNodes -= NumNodes_;    
    return NewList_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::DetachFromMiddle(size_t Index_, size_t NumNodes_)
{
  if (!NumNodes_)
    return NULL;

  if ((NumNodes_ + Index_) <= Size())
  {
    if (Index_ == 0)
      return DetachFromHead(NumNodes_);

    if ((Index_ + NumNodes_) >= Size())
      return DetachFromTail(Size() - Index_);

    DoubleList<T>* RetList_ =
    new DoubleList<T>(GetNode(Index_)->DetachSublist(ListRelation::NEXT, NumNodes_), _Data.GetStorageAttributes());
    _NumNodes -= NumNodes_;
    
    return RetList_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::DetachFromMiddle(DoubleListNode<T>* IndexNode_)
{
  if (IndexNode_ != NULL)
  {
    Boolean AtHead_ = IndexNode_ == Head();
    Boolean AtTail_ = IndexNode_ == Tail();

    if (AtHead_)
      return DetachFromHead(1);

    if (AtTail_)
      return DetachFromTail(1);

    DoubleList<T>* RetList_ =
    new DoubleList<T>(IndexNode_->DetachSublist(ListRelation::NEXT, 1), _Data.GetStorageAttributes());
    --_NumNodes;

    return RetList_;
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::DetachFromMiddleToEnd(size_t Index_)
{
  if (Index_ <= Size())
  {
    if (Index_ == 0)
      return this;

    if (Index_ >= Size())
      return NULL;

    return DetachFromTail(Size() - Index_);
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleList<T>* DoubleList<T>::DetachFromMiddleToStart(size_t Index_)
{
  if (Index_ <= Size())
  {
    if (Index_ >= Size() - 1)
      return this;

    return DetachFromHead(Index_ + 1);
  }

  return NULL;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::Duplicate(size_t Index_, size_t NumNodes_)
{
  DoubleListNode<T>* Node_ = GetNode(Index_);

  if (Node_)
  {
    Boolean UpdateTail_ = Index_ == Size() - 1;
    Node_->Duplicate(NumNodes_);
    if (UpdateTail_)
      _Tail = Node_->Last();

    IncreaseSize(NumNodes_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsCircular() const
{
  return
  (
    (_Head && _Tail) ?
      (_Head->Previous() == _Tail && _Tail->Next() == _Head):
      FALSE
  );
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsLinear() const
{
  return !IsCircular();
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsEmpty() const
{
  return (Size() == 0);
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsFull() const
{
  return (Size() == Limit());
}

/****************************************************************************/
template <class T>
size_t DoubleList<T>::Limit() const
{
  return _Limit;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::SetLimit(size_t Limit_, Boolean Shrink_)
{
  if (Size() > Limit_ && Shrink_)
    ShrinkTo(Limit_);

  _Limit = Limit_;
  return *this;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::IncreaseLimit(size_t Ext_, Boolean Shrink_)
{
  SetLimit(_Limit + Ext_, Shrink_);
  return *this;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::DecreaseLimit(size_t Ext_, Boolean Shrink_)
{
  SetLimit(_Limit - Ext_, Shrink_);
  return *this;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::Shrink(size_t Num_, int Relation_)
{
  if (Num_ && Num_ <= Size())
  {
    DoubleList<T>* Rest_ = DetachFromTail(Num_);
    if (Rest_)
      delete Rest_;
  }
  
  return *this;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::ShrinkTo(size_t Num_, int Relation_)
{
  if (Size() >= Num_)
  {
    size_t ShrinkNum_ = Size() - Num_;
    Shrink(ShrinkNum_, Relation_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
size_t DoubleList<T>::Size() const
{
  return _NumNodes;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::MakeCircular()
{
  if (_Tail && _Head)
  {
    _Tail->SetNext(_Head);
    _Head->SetPrevious(_Tail);
  }
  
  return *this;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::MakeLinear()
{
  if (_Tail && _Head)
  {
    _Tail->SetNext(NULL);
    _Head->SetPrevious(NULL);
  }

  return *this;
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::IsNewed() const
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
void DoubleList<T>::DestroyFromIterator()
{
# if OVERLOAD_NEW
    _DestroyFromIterator = IsNewed();
# else
    _DestroyFromIterator = TRUE;
# endif
}

/****************************************************************************/
template <class T>
Boolean DoubleList<T>::RegisterIterator()
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
Boolean DoubleList<T>::UnRegisterIterator()
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
size_t DoubleList<T>::IteratorsActive()
{
  return _IteratorsActive;
}

/****************************************************************************/
template <class T>
void DoubleList<T>::SetComparisonFunction(int(*CmpFnc_)(const T&, const T&))
{
  _CompFunc = CmpFnc_;
}

/****************************************************************************/
template <class T>
DoubleList<T>& DoubleList<T>::ReverseList()
{
  size_t i;
  DoubleListNode<T>* NodePtr_ = _Head;
  DoubleListNode<T>* TempPtr_;
  
  for (i = 0; i < _NumNodes && NodePtr_; i++)
  {
    TempPtr_ = NodePtr_->Next();
    NodePtr_->SetNext(NodePtr_->Previous());
    NodePtr_->SetPrevious(TempPtr_);
    NodePtr_ = NodePtr_->Previous();
  }

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

  return *this;
}

/****************************************************************************/
template <class T>
T** DoubleList<T>::ListToPtrArray()
{
  DoubleListNode<T>* ListPtr_ = _Head;
  T** Array_ = New<T*>(_NumNodes + 1);
  size_t i;

  for (i = 0; i < _NumNodes && ListPtr_; i++, ListPtr_ = ListPtr_->Next())
    Array_[i] = ListPtr_->GetItem();

  Array_[i] = NULL;
  return Array_;
}

/****************************************************************************/
template <class T>
T* DoubleList<T>::ListToArray()
{
  DoubleListNode<T>* ListPtr_ = _Head;
  T* Array_ = New<T>(_NumNodes);
  size_t i;

  for (i = 0; i < _NumNodes && ListPtr_; i++, ListPtr_ = ListPtr_->Next())
    Array_[i] = *ListPtr_->GetItem();

  return Array_;
}

/****************************************************************************/
template <class T>    
DoubleList<T>& DoubleList<T>::PtrArrayToList(T** Array_, size_t Size_)
{
  size_t i;

  if (Size_)
  {
    Flush();
    DoubleListNode<T>* FirstPtr_ = DoubleListNode<T>::Make(Array_[0], DataStorageAttributes::DEEP_COPY);
    DoubleListNode<T>* ListPtr_ = FirstPtr_;

    for (i = 1; i < Size_; i++)
    {
      ListPtr_->Add(DoubleListNode<T>::Make(Array_[i], DataStorageAttributes::DEEP_COPY), ListRelation::NEXT);
      ListPtr_ = ListPtr_->Next();
    }

    AssignListNode(FirstPtr_, Size_, DataStorageAttributes::DEEP_COPY);
    delete ListPtr_;
  }

  return *this;
}

/****************************************************************************/
template <class T>    
DoubleList<T>& DoubleList<T>::ArrayToList(T* Array_, size_t Size_)
{
  size_t i;

  if (Size_)
  {
    Flush();
    DoubleListNode<T>* FirstPtr_ = DoubleListNode<T>::Make(Array_[0]);
    DoubleListNode<T>* ListPtr_ = FirstPtr_;

    for (i = 1; i < Size_; i++)
    {
      ListPtr_->Add(DoubleListNode<T>::Make(Array_[i]), ListRelation::NEXT);
      ListPtr_ = ListPtr_->Next();
    }

    AssignListNode(FirstPtr_, Size_, DataStorageAttributes::DEEP_COPY);
    delete ListPtr_;
  }

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DoubleList<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DoubleList<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* DoubleList<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DoubleList<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
DoubleListAcceptor<T>::DoubleListAcceptor(const DoubleList<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void DoubleListAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const DoubleList<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* DoubleListAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean DoubleListAcceptor<T>::IsDoubleList() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::CreateFromDoubleList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new DoubleList<T>(*((const DoubleList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::CreateFromDoubleSublist(const ObjectAcceptor& Obj_)
{
  _VarPtr = new DoubleList<T>(*((const DoubleSublist<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::CreateFromSingleList(const ObjectAcceptor& Obj_)
{
  SingleList<T>* ListPtr_ = ((SingleList<T>*)Obj_.GiveDataPtr());
  _VarPtr = new DoubleList<T>();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::CreateFromStack(const ObjectAcceptor& Obj_)
{
  Stack<T>* ListPtr_ = ((Stack<T>*)Obj_.GiveDataPtr());
  _VarPtr = new DoubleList<T>();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::CreateFromQueue(const ObjectAcceptor& Obj_)
{
  Queue<T>* ListPtr_ = ((Queue<T>*)Obj_.GiveDataPtr());
  _VarPtr = new DoubleList<T>();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::CreateFromDeque(const ObjectAcceptor& Obj_)
{
  Deque<T>* ListPtr_ = ((Deque<T>*)Obj_.GiveDataPtr());
  _VarPtr = new DoubleList<T>();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::CreateFromSortedList(const ObjectAcceptor& Obj_)
{
  SortedList<T>* ListPtr_ = ((SortedList<T>*)Obj_.GiveDataPtr());
  _VarPtr = new DoubleList<T>();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::AssignFromDoubleList(const ObjectAcceptor& Obj_)
{
  const DoubleList<T>* TrgPtr_ = (const DoubleList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::AssignFromDoubleSublist(const ObjectAcceptor& Obj_)
{
  const DoubleSublist<T>* TrgPtr_ = (const DoubleSublist<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::AssignFromSingleList(const ObjectAcceptor& Obj_)
{
  SingleList<T>* ListPtr_ = ((SingleList<T>*)Obj_.GiveDataPtr());
  _VarPtr->Flush();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::AssignFromStack(const ObjectAcceptor& Obj_)
{
  Stack<T>* ListPtr_ = ((Stack<T>*)Obj_.GiveDataPtr());
  _VarPtr->Flush();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::AssignFromQueue(const ObjectAcceptor& Obj_)
{
  Queue<T>* ListPtr_ = ((Queue<T>*)Obj_.GiveDataPtr());
  _VarPtr->Flush();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::AssignFromDeque(const ObjectAcceptor& Obj_)
{
  Deque<T>* ListPtr_ = ((Deque<T>*)Obj_.GiveDataPtr());
  _VarPtr->Flush();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DoubleListAcceptor<T>::AssignFromSortedList(const ObjectAcceptor& Obj_)
{
  SortedList<T>* ListPtr_ = ((SortedList<T>*)Obj_.GiveDataPtr());
  _VarPtr->Flush();

  if (ListPtr_ && ListPtr_->Size())
  {
    size_t i;
    size_t Max_ = ListPtr_->Size();
    SingleListNode<T>* NodePtr_ = ListPtr_->GetNode(0);

    _VarPtr->SetLimit(ListPtr_->Limit());
    DoubleListNode<T> dNode1_(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes());
    _VarPtr->AssignListNode(&dNode1_, 1, ListPtr_->GetStorageAttributes());
    for (i = 1; i < Max_; i++)
    {
      NodePtr_ = ListPtr_->GetNode(i);
      _VarPtr->AddListNode(DoubleListNode<T>::Make(NodePtr_->GetItem(), NodePtr_->GetStorageAttributes()), ListRelation::TAIL);
    }
  }

  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean DoubleListAcceptor<T>::IsEqualToDoubleList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToDoubleList((const DoubleList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean DoubleListAcceptor<T>::IsLesserToDoubleList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToDoubleList((const DoubleList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean DoubleListAcceptor<T>::IsGreaterToDoubleList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToDoubleList((const DoubleList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DoubleListAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DoubleListAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if DOUBLELIST_IMPLEMENT_STLCONTAINER
template <class T>
const DoubleListIterator<T> DoubleList<T>::begin() const
{
  return (DoubleListIterator<T>(this));
}

/****************************************************************************/
template <class T>
DoubleListIterator<T> DoubleList<T>::begin()
{
  return (DoubleListIterator<T>(this));
}

/****************************************************************************/
template <class T>
const DoubleListIterator<T> DoubleList<T>::end() const
{
  DoubleListIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
DoubleListIterator<T> DoubleList<T>::end()
{
  DoubleListIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> DoubleList<T>::rbegin() const
{
  DoubleListIterator<T> Iter_(this);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> DoubleList<T>::rbegin()
{
  DoubleListIterator<T> Iter_(this);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> DoubleList<T>::rend() const
{
  DoubleListIterator<T> Iter_(this);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> DoubleList<T>::rend()
{
  DoubleListIterator<T> Iter_(this);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
T& DoubleList<T>::front()
{
  return (*begin());
}

/****************************************************************************/
template <class T>
const T& DoubleList<T>::front() const
{
  return (*begin());
}

/****************************************************************************/
template <class T>
T& DoubleList<T>::back()
{
  return *(--end());
}

/****************************************************************************/
template <class T>
const T& DoubleList<T>::back() const
{
  return *(--end());
}

/****************************************************************************/
template <class T>
void DoubleList<T>::push_back(const T& x)
{
  insert(end(), x);
}

/****************************************************************************/
template <class T>
void DoubleList<T>::pop_back()
{
  DoubleListIterator<T> Iter_ = end();
  --Iter_;
  erase(Iter_);
}

/****************************************************************************/
template <class T>
DoubleListIterator<T> DoubleList<T>::insert(DoubleListIterator<T> pos, const T& x)
{
  --pos;

  if (pos.More())
    Insert(pos.Position(), x);

  return pos;
}

/****************************************************************************/
template <class T>
void DoubleList<T>::insert(DoubleListIterator<T> pos, size_t n, const T& x)
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
void DoubleList<T>::insert(DoubleListIterator<T> pos, const DoubleListIterator<T> first, const DoubleListIterator<T> last)
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
void DoubleList<T>::erase(DoubleListIterator<T> pos)
{
  if (pos.More())
  {
    DoubleListNode<T>* ListPtr_ = GetNode(pos.Position());
    if (ListPtr_)
      ListPtr_->SetItem(T());
  }
}

/****************************************************************************/
template <class T>
void DoubleList<T>::erase(DoubleListIterator<T> first, DoubleListIterator<T> last)
{
  while (first != last && first.More())
  {
    DoubleListNode<T>* ListPtr_ = GetNode(first.Position());
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
#if DOUBLELIST_IMPLEMENT_STLITERATOR
template <class T>
DoubleListIterator<T>::DoubleListIterator(const DoubleList<T>* Ap_, Boolean DestroyFromIter_):
RandomAccessIterator<T>(),
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
DoubleListIterator<T>::DoubleListIterator(const DoubleListIterator<T>& Ap_):
RandomAccessIterator<T>(),
_ArrayObjPtr(Ap_._ArrayObjPtr),
_Index(Ap_._Index)
{
  if (_ArrayObjPtr)
    _ArrayVarPtr->RegisterIterator();
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>::~DoubleListIterator()
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
DoubleListIterator<T>* DoubleListIterator<T>::Make(DoubleList<T>* Ap_)
{
  return (new DoubleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>* DoubleListIterator<T>::Make(DoubleListIterator<T>& Ap_)
{
  return (new DoubleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const DoubleListIterator<T>* DoubleListIterator<T>::MakeConst(const DoubleList<T>* Ap_)
{
  return (new DoubleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const DoubleListIterator<T>* DoubleListIterator<T>::MakeConst(const DoubleListIterator<T>& Ap_)
{
  return (new DoubleListIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
int DoubleListIterator<T>::ClassID() const
{
  return TypeInfo::DOUBLELISTITERATOR_TYPE;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>* DoubleListIterator<T>::Clone()
{
  return (new DoubleListIterator<T>(*this));
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>* DoubleListIterator<T>::Clone() const
{
  return (new DoubleListIterator<T>(*this));
}

/****************************************************************************/
template <class T>
Boolean DoubleListIterator<T>::IsValid(SizeType Index_) const
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (Index_ < _ArrayVarPtr->Size())
    return !(_ArrayVarPtr->GetNode(Index_)->GetItem() == ((T*)NULL));

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleListIterator<T>::IsValid() const
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (More())
    return !(_ArrayVarPtr->GetNode(_Index)->GetItem() == ((T*)NULL));

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DoubleListIterator<T>::More() const
{
  if (_ArrayVarPtr->Size() == 0)
  {
    DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;
    LocalThis_->_Index = SizeType(-1);
  }

  return !AtEnd();
}

/****************************************************************************/
template <class T>
void DoubleListIterator<T>::Reset() const
{
  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;
  LocalThis_->_Index = 0;
}

/****************************************************************************/
template <class T>
void DoubleListIterator<T>::SetToLast() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;
  LocalThis_->_Index = _ArrayObjPtr->Size() - 1;
}

/****************************************************************************/
template <class T>
void DoubleListIterator<T>::SetToEnd() const
{
  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;
  LocalThis_->_Index = SizeType(-1);
}

/****************************************************************************/
template <class T>
const T* DoubleListIterator<T>::ConstPointee() const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  if (More())
    return _ArrayObjPtr->GetNode(_Index)->GetItem();
  return _ArrayObjPtr->GetNode(0)->GetItem();
}

/****************************************************************************/
template <class T>
const T* DoubleListIterator<T>::Pointee() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index)->GetItem();
  return _ArrayVarPtr->GetNode(0)->GetItem();
}

/****************************************************************************/
template <class T>
T* DoubleListIterator<T>::Pointee()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index)->GetItem();
  return _ArrayVarPtr->GetNode(0)->GetItem();
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleListIterator<T>::ConstNode() const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  if (More())
    return _ArrayObjPtr->GetNode(_Index);
  return _ArrayObjPtr->GetNode(0);
}

/****************************************************************************/
template <class T>
const DoubleListNode<T>* DoubleListIterator<T>::Node() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index);
  return _ArrayVarPtr->GetNode(0);
}

/****************************************************************************/
template <class T>
DoubleListNode<T>* DoubleListIterator<T>::Node()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return _ArrayVarPtr->GetNode(_Index);
  return _ArrayVarPtr->GetNode(0);
}

/****************************************************************************/
template <class T>
const DoubleListIterator<T>& DoubleListIterator<T>::GotoHead() const
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>
const DoubleListIterator<T>& DoubleListIterator<T>::GotoTail() const
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>& DoubleListIterator<T>::GotoHead()
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>    
DoubleListIterator<T>& DoubleListIterator<T>::GotoTail()
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
Boolean DoubleListIterator<T>::AtHead() const
{
  return (_Index == 0);
}

/****************************************************************************/
template <class T>    
Boolean DoubleListIterator<T>::AtTail() const
{
  return (_Index == _ArrayObjPtr->Size() - 1);
}

/****************************************************************************/
template <class T>
T& DoubleListIterator<T>::operator * ()
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (More())
    return *(_ArrayVarPtr->GetNode(_Index)->GetItem());
  return *(_ArrayVarPtr->GetNode(0)->GetItem());
}

/****************************************************************************/
template <class T>
const T& DoubleListIterator<T>::operator * () const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  if (More())
    return *(_ArrayVarPtr->GetNode(_Index)->GetItem());
  return *(_ArrayVarPtr->GetNode(0)->GetItem());
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DoubleListIterator<T>::operator [] (SizeType Index_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  _Index = Index_;
  if (!AtEnd())
    _ArrayVarPtr->GetNode(_Index);
  else
    _ArrayVarPtr->GetNode(0);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& DoubleListIterator<T>::operator [] (SizeType Index_) const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;
  LocalThis_->_Index = Index_;

  if (!AtEnd())
    _ArrayObjPtr->GetNode(_Index);
  else
    _ArrayObjPtr->GetNode(0);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DoubleListIterator<T>::operator ++ ()
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
RandomAccessIterator<T> DoubleListIterator<T>::operator ++ (int)
{
  RandomAccessIterator<T> Ref_(*this);

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
const RandomAccessIterator<T>& DoubleListIterator<T>::operator ++ () const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->Size() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DoubleListIterator<T>::operator ++ (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnull();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->Size() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DoubleListIterator<T>::operator -- ()
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (More())
    --_Index;
  else
    SetToLast();

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> DoubleListIterator<T>::operator -- (int)
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayVarPtr == NULL)
    Xnull();

  if (More())
    --_Index;
  else
    SetToLast();

  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& DoubleListIterator<T>::operator -- () const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DoubleListIterator<T>::operator -- (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnull();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> DoubleListIterator<T>::operator + (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DoubleListIterator<T>::operator + (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> DoubleListIterator<T>::operator - (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DoubleListIterator<T>::operator - (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DoubleListIterator<T>::operator += (SizeType Diff_)
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
const RandomAccessIterator<T>& DoubleListIterator<T>::operator += (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;

  if (_Index + Diff_ < _ArrayObjPtr->Size())
    LocalThis_->_Index += Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DoubleListIterator<T>::operator -= (SizeType Diff_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (_Index >= Diff_)
    _Index -= Diff_;
  else if (!More() && Diff_ <= _ArrayObjPtr->Size())
    _Index = _ArrayObjPtr->Size() - Diff_;
  else
    _Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& DoubleListIterator<T>::operator -= (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnull();

  DoubleListIterator<T>* const LocalThis_ = (DoubleListIterator<T>* const)this;

  if (_Index >= Diff_)
    LocalThis_->_Index -= Diff_;
  else if (!More() && Diff_ <= _ArrayObjPtr->Size())
    LocalThis_->_Index = _ArrayObjPtr->Size() - Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>& DoubleListIterator<T>::SetData(const T& Obj_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd())
    _ArrayVarPtr->GetNode(_Index)->SetItem(Obj_);

  return *this;
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>& DoubleListIterator<T>::SetData(T* Ptr_, int StoreFlags_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd())
    _ArrayVarPtr->GetNode(_Index)->SetItem(Ptr_, StoreFlags_);

  return *this;
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>& DoubleListIterator<T>::Erase()
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd())
    _ArrayVarPtr->GetNode(_Index)->Erase();

  return *this;
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>& DoubleListIterator<T>::operator = (T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnull();

  if (!AtEnd() && Ptr_)
    _ArrayVarPtr->GetNode(_Index)->SetItem(*Ptr_);

  return *this;
}

/****************************************************************************/
template <class T>
DoubleListIterator<T>& DoubleListIterator<T>::operator = (const DoubleListIterator<T>& Rhs_)
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
#if DOUBLELIST_DEBUG
int Square(int x)
{ return (x * x); }

class Char
{
  DEFAULT_ROOTOBJECT_DEFN(Char)

  friend ostream& _WriteToOutput(ostream& os, const Char& Obj_)
	{ os <<Obj_.val(); return os; }

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

int operator == (const DoubleList<Char>& x, const char* s)
{
  if (!s)
    return 0;

  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

void ShowTheList(const DoubleSublist<Char>& x)
{
  for (int i = 0; i < x.NumNodes(); ++i)
  {
    if (x.GetNode(i) && x.GetNode(i)->GetItem())
      cout <<x.GetNode(i)->GetItem()->val();
    cout <<",";
  }

  cout <<endl;
}

int operator == (const DoubleList<Char>& x, const Char* s)
{
  if (!s)
    return 0;

  for (int i = 0; i < x.Size(); ++i)
    if (x.GetNode(i)->GetItem()->val() != s[i].val())
      return 0;

  return 1;
}

int CompChar(const Char& c1, const Char& c2)
{
  return (c1.val() - c2.val());
}

DoubleList<Char> operator + (const DoubleList<Char>&, const Char&);
DoubleList<Char> operator + (const Char&, const DoubleList<Char>&);
DoubleList<Char> operator + (const Char&, const DoubleList<Char>&);

int main()
{
#if DOUBLELIST_GENERAL_DEBUG
  int i; Boolean Ok_;
  Ushort Index_, Max_;
  DoubleList<short> Argt_;
  DoubleList<short> Args_;
  DoubleList<short> Argv_;

  short x = 65;
  DoubleListNode<short> sNode1_(x);
  Argt_.AssignListNode(&sNode1_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    Argt_.AddListNode(DoubleListNode<short>::Make(x), ListRelation::NEXT);

  DoubleListNode<short> sNode2_(x);
  Args_.AssignListNode(&sNode2_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    Args_.AddListNode(DoubleListNode<short>::Make(x), ListRelation::NEXT);

  DoubleListNode<short> sNode3_(x);
  Argv_.AssignListNode(&sNode3_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    Argv_.AddListNode(DoubleListNode<short>::Make(x), ListRelation::NEXT);

  short Num41_ = 41;
  short Num7_ = 7;
  short Num3_ = 3;
  short Num99_ = 99;
  short Num67_ = 67;

  short* TempPtr_;
  short TempVal_;

  DoubleListIterator<short> ArgsIter_(&Args_);

  ArgsIter_[0]; ArgsIter_.SetData(Num41_);
  ArgsIter_[1]; ArgsIter_.SetData(Num7_);
  ArgsIter_[2]; ArgsIter_.SetData(Num3_);
  ArgsIter_[3]; ArgsIter_.SetData(Num99_);
  ArgsIter_[4]; ArgsIter_.SetData(Num67_);

// test1
  Argv_.GetNode(0)->SetItem(41);
  Argv_.GetNode(1)->SetItem(7);
  Argv_.GetNode(2)->SetItem(3);
  Argv_.GetNode(3)->SetItem(99);
  Argv_.GetNode(4)->SetItem(67);

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"DoubleList = ";
#endif

  for (ArgsIter_.Reset(), i = 0; ArgsIter_.More(); ArgsIter_++, ++i)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
    cout <<*(ArgsIter_.Pointee()) <<" ";
#endif
    Ok_ = (i == 0) ? (*ArgsIter_ == 41):
	  (i == 1) ? (*ArgsIter_ == 7):
	  (i == 2) ? (*ArgsIter_ == 3):
	  (i == 3) ? (*ArgsIter_ == 99):
	  (i == 4) ? (*ArgsIter_ == 67):0;
    assert(Ok_);
  }

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: " <<"Size == " <<Args_.Size();
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  assert(Args_.Size() == 5);
  
  DoubleListIterator<short> ArgvIter_(&Argv_);
  for (i = 0; ArgvIter_.More(); ArgvIter_++, ++i)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
    cout <<*(ArgvIter_.Node()->GetItem()) <<" ";
#endif
    Ok_ = (i == 0) ? (*ArgvIter_ == 41):
	  (i == 1) ? (*ArgvIter_ == 7):
	  (i == 2) ? (*ArgvIter_ == 3):
	  (i == 3) ? (*ArgvIter_ == 99):
	  (i == 4) ? (*ArgvIter_ == 67):0;
    assert(Ok_);
  }

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: " <<"Size == " <<Argv_.Size();
  cout <<endl;
#endif
  assert(Argv_.Size() == 5);

// test1
#if DOUBLELIST_CLONE_TEST
  DoubleList<short>* Newv_ = (DoubleList<short>*)Argv_.Clone();
  *Newv_ = Argv_;

  (*Newv_).GetNode(0)->SetItem(128);
  (*Newv_).GetNode(4)->SetItem(512);

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"NewArray = ";
#endif
  DoubleListIterator<short> NewvIter_(Newv_);
  for (i = 0; NewvIter_.More(); ++NewvIter_, ++i)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
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

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Newv_->Size() <<endl;
#endif
  assert(Newv_->Size() == 5);
  
#endif	// Array Clone test
// test1

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  DoubleListIterator<short> ArgvIter2_(&Argv_);
  for (i = 0; ArgvIter2_.More(); ++ArgvIter2_, ++i)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
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

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size() <<endl;
#endif

 // test1
#if DOUBLELIST_CLONE_TEST
  delete Newv_;
#endif
 // test1

  for (i = 5, x++; i < 11; i++, x++)
    Argv_.AddListNode(DoubleListNode<short>::Make(x), ListRelation::NEXT);

  ArgvIter2_.Reset();
  ArgvIter2_[0], ArgvIter2_.SetData(64);
  ArgvIter2_[1], ArgvIter2_.SetData(128);
  ArgvIter2_[2], ArgvIter2_.SetData(256);
  ArgvIter2_[3], ArgvIter2_.Erase();
  ArgvIter2_[4], ArgvIter2_.SetData(1024);
  ArgvIter2_[5], ArgvIter2_.Erase();
  ArgvIter2_[6], ArgvIter2_.SetData(4096);
  ArgvIter2_[7], ArgvIter2_.Erase();
  ArgvIter2_[8], ArgvIter2_.SetData(16384);
  ArgvIter2_[9], ArgvIter2_.SetData(2000);
  ArgvIter2_[10], ArgvIter2_.SetData(1000);

  Argt_ = Argv_;

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"ArgArray = ";
#endif

  DoubleListIterator<short> ArgvIter3_(&Argv_);
  for (Index_ = 0; ArgvIter3_.More(); ++ArgvIter3_, ++Index_)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
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

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<"\t: ";
  cout <<"Size == " <<Argv_.Size();
#endif
  assert(Argv_.Size() == 11);

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"Reverse DoubleList = ";
  ReverseRandomAccessIterator<short> RevIter_(ArgvIter3_);
  for (RevIter_.Reset(); RevIter_.More();)
    if (RevIter_.IsValid())
      cout <<*RevIter_++ <<" ";
    else
    {
      cout <<"VOID ";
      ++RevIter_;
    }

  cout <<endl;
#endif

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl <<"Shrinking Down To 6";
  Argv_.ShrinkTo(6);
  cout <<endl <<"Shrinking Done";
  cout <<"\t: " <<"Size == " <<Argv_.Size();
  cout <<endl;
#endif
  Argv_.ShrinkTo(6);
  assert(Argv_.Size() == 6);

  Argv_.Flush();
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<"DoubleList Flushed";

  cout <<endl;
  cout <<"DoubleList = ";
#endif
  for (ArgvIter3_.Reset(); ArgvIter3_.More(); ++ArgvIter3_)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
    if (ArgvIter3_.IsValid())
      cout <<*ArgvIter3_ <<" ";
    else
      cout <<"VOID ";
#endif
    assert(ArgvIter3_.IsValid() == 0);
  }

#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"TDoubleList = ";
#endif
  DoubleListIterator<short> Tpiter_(&Argt_);
  for (i = 0; Tpiter_.More(); ++Tpiter_, ++i)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
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

  DoubleListNode<short> sNode4_(2141);
  delete Argt_.DetachFromMiddleToEnd(5);
  Argv_.AssignListNode(&sNode4_, 1, DataStorageAttributes::DEEP_COPY);
  Argv_.AddListNode(DoubleListNode<short>::Make(227), ListRelation::NEXT);
  Argv_.AddListNode(DoubleListNode<short>::Make(233), ListRelation::NEXT);
  Argv_.AddListNode(DoubleListNode<short>::Make(2499), ListRelation::NEXT);
  Argv_.AddListNode(DoubleListNode<short>::Make(2567), ListRelation::NEXT);

  // Testing virtual assignment method
  Argt_.Assign(Argv_);
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"TDoubleList = ";
#endif
  for (i = 0, Tpiter_.Reset(); Tpiter_.More(); ++Tpiter_, ++i)
  {
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
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
#if DOUBLELIST_SHOW_DEBUG_OUTPUT
  cout <<endl;
  cout <<"NULL: " <<ObjPtr1_->IsNullObject() <<"\t" <<ObjPtr2_->IsNullObject();
#endif

#if DOUBLELIST_NOTIFY_SUCCESS
  cout <<endl <<"SUCCESS General array classes testing" <<endl;
#endif
///
#endif

#if DOUBLELIST_STANDARD_DEBUG
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  DoubleList<Char> s1, s2, s3;
  Boolean Valid_;
  size_t i;

  Char* pc4;
  Char* pc5;

  DoubleList<Char>::SetComparisonFunction(CompChar);
  DoubleListNode<Char> sNode1_(c0);

  s3.AssignListNode(&sNode1_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 3; i++)
    s3.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
    
  DoubleList<Char> s4(s3);
  DoubleList<Char> s5, s6;
  DoubleListNode<Char> sNode2_(ca);
  DoubleListNode<Char> sNode3_(cb);

  s5.AssignListNode(&sNode2_, 1, DataStorageAttributes::DEEP_COPY);
  s6.AssignListNode(&sNode3_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 0; i < 9; i++)  
    s6.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);

  DoubleList<Char> s7;
  DoubleListNode<Char> sNode4_(c0);
  
  s7.AssignListNode(&sNode4_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 3; i++)
    s7.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);  
  assert(s1 == "XX");
  s1.AddListNode(DoubleListNode<Char>::Make(ca), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(ca), ListRelation::TAIL);
  assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "XXa");

  // test assigns
  DoubleListNode<Char> sNode5_(cb);
  DoubleListNode<Char> sNode6_(cb);
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode5_, 1, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode6_, 1, DataStorageAttributes::DEEP_COPY);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "bbbbb");
  s1.AssignListNode(
    DoubleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
    1, DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "bXXX");

  // test appends
  s1 = s5;
  s1 += s5; assert(s1 == "aa");
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);
  assert(s1 == "aab");
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  assert(s1 == "aabb");
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);  
  s1.AddListNode(DoubleListNode<Char>::Make(cb), ListRelation::TAIL);    
  assert(s1 == "aabbbbbbb");
  s1.AddListNode(DoubleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), ListRelation::TAIL);
  assert(s1 == "aabbbbbbba");  
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  s1.AddListNode(DoubleListNode<Char>::Make(c0), ListRelation::TAIL);
  assert(s1 == "aabbbbbbbaXXX");
  s1 = s4 + s5; assert(s1 == "XXXXa");

  // test inserts, removes, etc.
  s1 = s4;
  s1.AddListAtIndex(DoubleList<Char>::Make(s5), 0, ListRelation::NEXT);
  ShowTheList(s1);
  assert(s1 == "XaXXX");
  s1.AddListNodeAtIndex(DoubleListNode<Char>::Make(cb), 1, ListRelation::NEXT);
  assert(s1 == "XabXXX");
  for (i = 0; i < 3; i++)
    s1.AddListNodeAtIndex(DoubleListNode<Char>::Make(ca), 0, ListRelation::PREVIOUS);
  assert(s1 == "aaaXabXXX");
  s1.AddListNodeAtIndex(
    DoubleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE),
    8, ListRelation::NEXT);
  assert(s1 == "aaaXabXXXa");

  for (i = 0; i < 3; i++)
    s1.AddListNodeAtIndex(DoubleListNode<Char>::Make(c3), 1, ListRelation::PREVIOUS);
  assert(s1 == "aXXXaaXabXXXa");
  DoubleList<Char> s8(s1);
  
  delete s1.DetachFromMiddleToEnd(7);
  assert(s1 == "aXXXaaX");
  
  delete s1.DetachFromMiddle(2, 1);
  assert(s1 == "aXXaaX");
  s1.Flush();
  assert(s1 == "");

  s1.AssignSublist(
    s6.GetNode(2)->NewSublist(ListRelation::NEXT, 0, TRUE),
    DataStorageAttributes::DESTROY);
  assert(s6 == "bbbbbbbbbb" && s1 == "bbbbbbbb");
  s1.AssignSublist(
    s6.GetNode(3)->NewSublist(ListRelation::NEXT, 2),
    DataStorageAttributes::DESTROY);  
  assert(s1 == "bb");
  assert(s3 == "XXXX");
  
  DoubleList<Char> stemp_ = s1;
  s1 = s3;
  s3 = stemp_;
  assert(s1 == "XXXX" && s3 == "bb");
  
  // test access functions
  s1 = s7; assert(*s1.GetNode(0)->GetItem() == c0);
  s1.GetNode(0)->SetItem(ca); assert(*s1.GetNode(0)->GetItem() == ca);
  s1.GetNode(1)->SetItem(cb); assert(*s1.GetNode(1)->GetItem() == cb);

  pc4 = s1.ListToArray();
  DoubleList<Char>* cs7p = (DoubleList<Char>*)s7.Clone();
  pc5 = cs7p->ListToArray();
  assert(pc4[1] == cb && pc5[2] == c0);
  delete cs7p;
  ::Delete(pc4);
  ::Delete(pc5);

  // Test class hierarchy extensions
  // Test virtual object constructor
  pc5 = &c0;
  DoubleList<Char>* ps1 = DoubleList<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  DoubleList<Char>* ps2 = DoubleList<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test binary IO methods
  assert(!(s6 == *ps2));
  ofstream fout("testing.bin");
  ps2->BinaryWrite(fout);
  fout.close();

  ifstream fin("testing.bin");
  s6.BinaryRead(fin);
  fin.close();
  assert(s6 == *ps2);

  // testing series comparison methods
  s8.Sort();
  assert(s8 == "XXXXXXXaaaaab");

  DoubleList<int> s9;
  DoubleListNode<int> sNode7_(0);
  
  s9.AssignListNode(&sNode7_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1; i < 10; i++)
    s9.AddListNode(DoubleListNode<int>::Make(i * 2), ListRelation::NEXT);

  s9.ApplyUnary(Square);
  for (i = 0; i < 10; i++)
    assert(*s9.GetNode(i)->GetItem() == (i * 2) * (i * 2));

  cout <<s9 <<endl;
  DoubleList<Char> s10 = s8;
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

  SingleList<Char> ss1, ss2, ss3;

  int x = 'a';
  SingleListNode<Char> sNode8_(c3.Set(x));
  ss1.AssignListNode(&sNode8_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    ss1.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 'a' + 26 - 1;
  SingleListNode<Char> sNode9_(c3.Set(x));
  ss2.AssignListNode(&sNode9_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x--; i < 5; i++, x--)
    ss2.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 'k';
  SingleListNode<Char> sNode10_(c3.Set(x));
  ss3.AssignListNode(&sNode10_, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    ss3.AddListNode(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  s1.Assign(ss2);
  s2.Assign(ss3);
  s3.Assign(ss1);

  assert(s1 == "zyxwv" && s1.Size() == 5);
  assert(s2 == "klmno" && s2.Size() == 5);
  assert(s3 == "abcde" && s3.Size() == 5);

  x = 65;
  DoubleListNode<Char> sNode11_(c3.Set(x));
  s1.AssignListNode(&sNode11_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s1.AddListNode(DoubleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 65 + 26 - 1;
  DoubleListNode<Char> sNode12_(c3.Set(x));
  s2.AssignListNode(&sNode12_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x--; i < 5; i++, x--)
    s2.AddListNode(DoubleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  x = 75;
  DoubleListNode<Char> sNode13_(c3.Set(x));
  s3.AssignListNode(&sNode13_, 1, DataStorageAttributes::DEEP_COPY);
  for (i = 1, x++; i < 5; i++, x++)
    s3.AddListNode(DoubleListNode<Char>::Make(c3.Set(x)), ListRelation::NEXT);

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s1.AddList(DoubleSublist<Char>::Make(s3), ListRelation::HEAD);
  s1 += s2;

  assert(s1 == "KLMNOABCDEZYXWV");
  s1.Sort();
  assert(s1 == "ABCDEKLMNOVWXYZ");

  s1.ReplaceListAtIndex(DoubleSublist<Char>::Make(s3), 14);
  ShowTheList(s1);
  s1.ReplaceList(DoubleSublist<Char>::Make(s2), ListRelation::HEAD);
  ShowTheList(s1);

  assert(s1 == "ZYXWVBCDEKLMNOVWXYKLMNO");

  s1.Sort(SortOrder::DECREASING);
  assert(s1 == "ZYYXXWWVVOONNMMLLKKEDCB");

  s1.ReverseList();
  assert(s1 == "BCDEKKLLMMNNOOVVWWXXYYZ");

  s1.ReplaceListAtIndex(DoubleSublist<Char>::Make(s2), 10);
  assert(s1 == "BCDEKKLLMMZYXWVNOOVVWWXXYYZ");

  s1.Flush();
  s1 = s4;

  s1.AddListNode(DoubleSublist<Char>::Make(s3)->GetNode(2), ListRelation::HEAD);
  s1 += s2;

  assert(s1 == "KLMNOABCDEZYXWV");
  s1.Sort();
  assert(s1 == "ABCDEKLMNOVWXYZ");

  s1.ReplaceListNodeAtIndex(DoubleSublist<Char>::Make(s3)->GetNode(2), 14);
  s1.ReplaceListNode(DoubleSublist<Char>::Make(s2)->GetNode(2), ListRelation::HEAD);

  assert(s1 == "ZYXWVBCDEKLMNOVWXYKLMNO");

  s1.Sort(SortOrder::DECREASING);
  assert(s1 == "ZYYXXWWVVOONNMMLLKKEDCB");

  s1.ReverseList();
  assert(s1 == "BCDEKKLLMMNNOOVVWWXXYYZ");

  s1.ReplaceListNodeAtIndex(DoubleSublist<Char>::Make(s2)->GetNode(4), 10);
  assert(s1 == "BCDEKKLLMMZYXWVNOOVVWWXXYYZ");

  size_t Index_;
  DoubleListNode<Char>* FoundNode_ = s1.FindItem(Char('V'), Index_);
  assert(FoundNode_ != NULL && Index_ == 14 && FoundNode_->GetItem()->val() == 'V');
  FoundNode_ = s1.FindItem(Char('V'), Index_, s1.Size() - 1, ListRelation::HEAD);
  assert(FoundNode_ != NULL && Index_ == 19 && FoundNode_->GetItem()->val() == 'V');

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

  s1.Duplicate(0, 3);
  s1.Duplicate(7, 3);
  s1.Duplicate(5, 3);

  assert(s1 == "AAAABCCCCDEEEE");

#if DOUBLELIST_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <DoubleList>" <<endl;
#endif
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




