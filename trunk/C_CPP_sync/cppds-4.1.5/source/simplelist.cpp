#ifndef SIMPLELIST_CPP
#define SIMPLELIST_CPP
#ifndef SIMPLELIST_H
  #include "simplelist.h"
#endif

/****************************************************************************/
template <class T>
ConstListIterator<T>::ConstListIterator(const SimpleList<T>* List_):
_ConstList(List_),
_ConstHere(List_ ? List_->Head():NULL),
_Index(0)
{}

/****************************************************************************/
template <class T>
Boolean ConstListIterator<T>::More() const
{
  return (_ConstHere->_Next != NULL);
}

/****************************************************************************/
template <class T>
Boolean ConstListIterator<T>::Less() const
{
  return (_Index > 0);
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* ConstListIterator<T>::Next()
{
  if (!More())
    return NULL;

  _ConstHere = _ConstHere->_Next;
  _Index++;
  return _ConstHere;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* ConstListIterator<T>::Prev()
{
  if (!Less())
    return NULL;

  return IndexNode(_Index - 1);
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* ConstListIterator<T>::operator ++ (int)
{
  const SimpleNode<T>* RetPtr_ = _ConstHere;
  Next();

  return RetPtr_;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* ConstListIterator<T>::operator -- (int)
{
  const SimpleNode<T>* RetPtr_ = _ConstHere;
  Prev();

  return RetPtr_;
}

/****************************************************************************/
template <class T>
ConstListIterator<T>& ConstListIterator<T>::Head()
{
  _ConstHere = _ConstList->Head();
  _Index = 0;
  return *this;
}

/****************************************************************************/
template <class T>
ConstListIterator<T>& ConstListIterator<T>::Tail()
{
  _ConstHere = _ConstList->Tail();
  _Index = _ConstList->Size();
  
  if (_Index)
    --_Index;
  
  return *this;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* ConstListIterator<T>::IndexNode(size_t Index_)
{
  if (Index_ < Count())
  {
    Head();
    for (size_t Count_ = 0; _ConstList && Count_ < Index_; Count_++)
      ++(*this);
  }
  else
    return NULL;

  return _ConstHere;
}

/****************************************************************************/
template <class T>
size_t ConstListIterator<T>::Count() const
{
  return _ConstList->Size();
}

/****************************************************************************/
template <class T>
size_t ConstListIterator<T>::Position() const
{
  return _Index;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ConstListIterator<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ConstListIterator<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* ConstListIterator<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void ConstListIterator<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleListIterator<T>::SimpleListIterator(SimpleList<T>* List_):
_List(List_),
_Here(List_ ? List_->Head():NULL),
_Index(0)
{}

/****************************************************************************/
template <class T>
Boolean SimpleListIterator<T>::More() const
{
  return (_Here->_Next != NULL);
}

/****************************************************************************/
template <class T>
Boolean SimpleListIterator<T>::Less() const
{
  return (_Index > 0);
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleListIterator<T>::Next()
{
  if (!More())
    return NULL;

  _Here = _Here->_Next;
  _Index++;  
  return _Here;
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleListIterator<T>::Prev()
{
  if (!Less())
    return NULL;

  return IndexNode(_Index - 1);
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleListIterator<T>::operator ++ (int)
{
  SimpleNode<T>* RetPtr_ = _Here;
  Next();

  return RetPtr_;
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleListIterator<T>::operator -- (int)
{
  SimpleNode<T>* RetPtr_ = _Here;
  Prev();

  return RetPtr_;
}

/****************************************************************************/
template <class T>
SimpleListIterator<T>& SimpleListIterator<T>::Head()
{
  _Here = _List->Head();
  _Index = 0;  
  return *this;
}

/****************************************************************************/
template <class T>
SimpleListIterator<T>& SimpleListIterator<T>::Tail()
{
  _Here = _List->Tail();
  _Index = _List->Size();
  
  if (_Index)
    --_Index;
  
  return *this;
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleListIterator<T>::Update(size_t Index_, T* Data_)
{
  if (Index_ < Count())
  {
    Head();
    for (size_t Count_ = 0; _List && Count_ < Index_; Count_++)
      ++(*this);      

    ::Delete(_Here->_Object);
    _Here->_Object = Data_;
  }
  else
    return NULL;
  
  return _Here;
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleListIterator<T>::IndexNode(size_t Index_)
{
  if (Index_ < Count())
  {
    Head();  
    for (size_t Count_ = 0; _List && Count_ < Index_; Count_++)
      ++(*this);
  }
  else
    return NULL;

  return _Here;
}

/****************************************************************************/
template <class T>
void SimpleListIterator<T>::Remove()
{
  _List->Remove(_Here);
  Head();
}

/****************************************************************************/
template <class T>
size_t SimpleListIterator<T>::Position() const
{
  return _Index;
}

/****************************************************************************/
template <class T>
size_t SimpleListIterator<T>::Count() const
{
  return _List->Size();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SimpleListIterator<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleListIterator<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SimpleListIterator<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void SimpleListIterator<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleNode<T>::SimpleNode(T* Obj_, SimpleNode<T>* Next_):
_Object(Obj_),
_Next(Next_)
{}

/****************************************************************************/
template <class T>
SimpleNode<T>::~SimpleNode()
{
  delete _Next;
  _Next = NULL;

  ::Delete(_Object);
  _Object = NULL;
}

/****************************************************************************/
template <class T>
void SimpleNode<T>::Insert(SimpleNode<T>* Node_)
{
  Node_->_Next = _Next;
  _Next = Node_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SimpleNode<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleNode<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SimpleNode<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void SimpleNode<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleList<T>::SimpleList(ObjectCloner<T>* ClonerPtr_):
_Cloner(ClonerPtr_ ? ClonerPtr_:RealCloner<T>::Make()),
_Count(0),
_List(NULL)
{}

/****************************************************************************/
template <class T>
SimpleList<T>::SimpleList(const SimpleNode<T>* NodePtr_, ObjectCloner<T>* ClonerPtr_):
_Cloner(ClonerPtr_ ? ClonerPtr_:RealCloner<T>::Make()),
_Count(0),
_ConstList(NodePtr_)
{}

/****************************************************************************/
template <class T>
SimpleList<T>::SimpleList(const SimpleList<T>& Alist_):
_Cloner(Alist_._Cloner->Clone()),
_Count(0),
_List(NULL)
{
  for (SimpleNode<T>* Node_ = Alist_._List; Node_ != NULL; Node_ = Node_->_Next)
    AppendTail(Node_->_Object ? _Cloner->Duplicate(*Node_->_Object):NULL);
}

/****************************************************************************/
template <class T>
SimpleList<T>::~SimpleList()
{
  delete _List;
  _List = NULL;

  delete _Cloner;
}

/****************************************************************************/
template <class T>
SimpleList<T>& SimpleList<T>::SetCloner(ObjectCloner<T>* Cloner_)
{
  delete _Cloner;
  _Cloner = Cloner_;
  return *this;
}

/****************************************************************************/
template <class T>
SimpleList<T>& SimpleList<T>::operator = (const SimpleList<T>& Alist_)
{
  if (this != &Alist_)
  {  
    delete _List;
    _List = NULL;
    _Count = 0;

    for (SimpleNode<T>* Node_ = Alist_._List; Node_ != NULL; Node_ = Node_->_Next)
      AppendTail(Node_->_Object ? _Cloner->Duplicate(*Node_->_Object):NULL);
  }

  return *this;
}

/****************************************************************************/
template <class T>
SimpleList<T>& SimpleList<T>::operator += (const SimpleList<T>& Alist_)
{
  if (this != &Alist_)
    for (SimpleNode<T>* Node_ = Alist_._List; Node_ != NULL; Node_ = Node_->_Next)
      AppendTail(Node_->_Object ? _Cloner->Duplicate(*Node_->_Object):NULL);

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SimpleList<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleList<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SimpleList<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void SimpleList<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleList<T>::Tail()
{
  if (_List)
  {
    SimpleNode<T>* Node_ = _List;
    while (Node_->_Next)
      Node_ = Node_->_Next;

    return Node_;
  }

  return _List;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SimpleList<T>::Tail() const
{
  if (_List)
  {
    SimpleNode<T>* Node_ = _List;
    while (Node_->_Next)
      Node_ = Node_->_Next;

    return Node_;
  }

  return _List;
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SimpleList<T>::FindPrevious(SimpleNode<T>* Ptr_)
{
  SimpleNode<T>* Node_ = _List;

  if (Ptr_ == Node_)
    return NULL;

  if (Ptr_)
    while (Node_ && Ptr_ != Node_->_Next)
      Node_ = Node_->_Next;

  return ((Ptr_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
void SimpleList<T>::Remove(SimpleNode<T>* Ptr_)
{
  if (Ptr_ == _List)
    ::Delete(PopNode());
  else
  {
    SimpleNode<T>* Node_ = FindPrevious(Ptr_);

    if (Node_)
    {
      Node_->_Next = Ptr_->_Next;
      Ptr_->_Next = NULL;

      delete Ptr_;
      --_Count;
    }
  }
}

/****************************************************************************/
template <class T>
void SimpleList<T>::InsertAfter(SimpleNode<T>* Ptr_, T* NewPtr_)
{
  if (Ptr_)
  {
    SimpleNode<T>* NewNode_ = new SimpleNode<T>(NewPtr_);

    if (NewNode_)
    {
      Ptr_->Insert(NewNode_);
      ++_Count;
    }
  }
}

/****************************************************************************/
template <class T>
void SimpleList<T>::InsertBefore(SimpleNode<T>* Ptr_, T* NewPtr_)
{
  if (Ptr_ == _List)
    AppendHead(NewPtr_);
  else
  {
    SimpleNode<T>* Node_ = FindPrevious(Ptr_);
    InsertAfter(Node_, NewPtr_);
  }
}

/****************************************************************************/
template <class T>
SimpleList<T>& SimpleList<T>::AppendHead(T* Obj_)
{
  _List = new SimpleNode<T>(Obj_, _List);

  if (_List)
    ++_Count;

  return *this;
}

/****************************************************************************/
template <class T>
SimpleList<T>& SimpleList<T>::AppendTail(T* Obj_)
{
  SimpleNode<T>* Node_ = _List;

  if (Obj_)
    if (_List)
    {
      while (Node_->_Next)
	Node_ = Node_->_Next;

      Node_->_Next = new SimpleNode<T>(Obj_, Node_->_Next);
      if (Node_->_Next)
	++_Count;
    }
    else
    {
      _List = new SimpleNode<T>(Obj_, _List);
      if (_List)
	++_Count;
    }

  return *this;
}

/****************************************************************************/
template <class T>
T* SimpleList<T>::PopNode()
{
  SimpleNode<T>* Node_ = _List;

  if (Node_ == NULL)
    return NULL;

  T* Object_ = Node_->_Object;
  Node_->_Object = NULL; 	// So node won't try to delete it
  _List = Node_->_Next;
  Node_->_Next = NULL; 		// So it won't delete the list

  delete Node_;
  --_Count;

  return Object_;
}

/****************************************************************************/
template <class T>
void SimpleList<T>::DeleteAll()
{
  delete _List;
  _List = NULL;
  _Count = 0;
}

/****************************************************************************/
template <class T>
Boolean SimpleList<T>::Empty(void) const
{
  return (_List == NULL);
}

/****************************************************************************/
template <class T>
size_t SimpleList<T>::Size(void) const
{
  return _Count;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
SearchableList<T>::SearchableList()
{}

/****************************************************************************/
template <class T>
SearchableList<T>::SearchableList(const SimpleNode<T>* NodePtr_):
SimpleList<T>(NodePtr_)
{}

/****************************************************************************/
template <class T>
SearchableList<T>::SearchableList(const SearchableList<T>& List_):
SimpleList<T>(List_)
{}

/****************************************************************************/
template <class T>
SearchableList<T>& SearchableList<T>::operator = (const SimpleList<T>& SimpList_)
{
  SimpleList<T>::operator = (SimpList_);
  return *this;
}

/****************************************************************************/
template <class T>
SearchableList<T>& SearchableList<T>::operator += (const SimpleList<T>& SimpList_)
{
  SimpleList<T>::operator += (SimpList_);
  return *this;
}

/****************************************************************************/
#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
SearchableList<T>& SearchableList<T>::operator = (const SearchableList<T>& SimpList_)
{
  SimpleList<T>::operator = (SimpList_);
  return *this;
}

/****************************************************************************/
template <class T>
SearchableList<T>& SearchableList<T>::operator += (const SearchableList<T>& SimpList_)
{
  SimpleList<T>::operator += (SimpList_);
  return *this;
}
#endif
/****************************************************************************/
template <class T>
SimpleNode<T>* SearchableList<T>::Find(T* Obj_, PtrCompare<T>& Compare_)
{
  SimpleNode<T>* Node_ = _List;

  if (Obj_)
    while (Node_ && !Compare_(Obj_, Node_->_Object))
      Node_ = Node_->_Next;

  return ((Obj_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SearchableList<T>::Find(T* Obj_)
{
  PtrEqual<T> Comp_(SortUsingBuiltInOperators<T>::Trait());
  return Find(Obj_, Comp_);
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SearchableList<T>::NextMatch(SimpleNode<T>* Ptr_, PtrCompare<T>& Compare_)
{
  SimpleNode<T>* Node_ = Ptr_->_Next;

  if (Ptr_)
    while (Node_ && !Compare_(Ptr_->_Object, Node_->_Object))
      Node_ = Node_->_Next;

  return ((Ptr_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
SimpleNode<T>* SearchableList<T>::NextMatch(SimpleNode<T>* Ptr_)
{
  PtrEqual<T> Comp_(SortUsingBuiltInOperators<T>::Trait());
  return NextMatch(Ptr_, Comp_);
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SearchableList<T>::Find(const T* Obj_, PtrCompare<T>& Compare_) const
{
  const SimpleNode<T>* Node_ = _List;

  if (Obj_)
    while (Node_ && !Compare_(Obj_, Node_->_Object))
      Node_ = Node_->_Next;

  return ((Obj_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SearchableList<T>::Find(const T* Obj_) const
{
  PtrEqual<T> Comp_(SortUsingBuiltInOperators<T>::Trait());
  return Find(Obj_, Comp_);
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SearchableList<T>::NextMatch(const SimpleNode<T>* Ptr_, PtrCompare<T>& Compare_) const
{
  const SimpleNode<T>* Node_ = Ptr_->_Next;

  if (Ptr_)
    while (Node_ && !Compare_(Ptr_->_Object, Node_->_Object))
      Node_ = Node_->_Next;

  return ((Ptr_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SearchableList<T>::NextMatch(const SimpleNode<T>* Ptr_) const
{
  PtrEqual<T> Comp_(SortUsingBuiltInOperators<T>::Trait());
  return NextMatch(Ptr_, Comp_);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SearchableList<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SearchableList<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SearchableList<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void SearchableList<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SimpleListBrowser<T>::SimpleListBrowser(const SearchableList<T>& Alist_):
SearchableList<T>(Alist_.Head())
{}

/****************************************************************************/
template <class T>
SimpleListBrowser<T>::SimpleListBrowser(const SimpleNode<T>* Ptr_):
SearchableList<T>(Ptr_)
{}

/****************************************************************************/
template <class T>
SimpleListBrowser<T>::~SimpleListBrowser()
{
  _ConstList = NULL;
}

/****************************************************************************/
template <class T>
Boolean SimpleListBrowser<T>::More() const
{
  return (_ConstList != NULL && _ConstList->_Next != NULL);
}

/****************************************************************************/
template <class T>
SimpleListBrowser<T>& SimpleListBrowser<T>::operator = (const SearchableList<T>& Alist_)
{
  if (this != &Alist_)
    _ConstList = Alist_.Head();

  return *this;
}

/****************************************************************************/
template <class T>
SimpleListBrowser<T>& SimpleListBrowser<T>::operator = (const SimpleNode<T>* Ptr_)
{
  _ConstList = Ptr_;
  return *this;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SimpleListBrowser<T>::NextMatch(const T* Obj_, PtrCompare<T>& Compare_) const
{
  if (_ConstList)
  {
    _ConstList = _ConstList->_Next;
    _ConstList = Find(Obj_, Compare_);
  }

  return _ConstList;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SimpleListBrowser<T>::NextMatch(const T* Obj_) const
{
  PtrEqual<T> Comp_;
  return NextMatch(Obj_, Comp_);
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SimpleListBrowser<T>::IndexNode(size_t Index_) const
{
  for (size_t Count_ = 0; _ConstList && Count_ < Index_; Count_++)
    ++(*this);

  return _ConstList;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SimpleListBrowser<T>::operator ++ () const
{
  if (_ConstList)
    _ConstList = _ConstList->_Next;

  return _ConstList;
}

/****************************************************************************/
template <class T>
const SimpleNode<T>* SimpleListBrowser<T>::operator ++ (int) const
{
  const SimpleNode<T>* RetPtr_ = _ConstList;

  if (_ConstList)
    _ConstList = _ConstList->_Next;

  return RetPtr_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SimpleListBrowser<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SimpleListBrowser<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* SimpleListBrowser<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void SimpleListBrowser<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#if SIMPLELIST_DEBUG
#if defined(__TURBOC__)
  #ifndef __CONIO_H
    #include <conio.h>
  #endif
#endif

short* Clone(short Obj_)
   { return (new short(Obj_)); }

int main()
{
#if defined(__TURBOC__)
  clrscr();
#endif
  SearchableList<short> Alist_;
  SearchableList<short> Alist2_;
  
  short* Num13_ = new short(13);
  short* Num26_ = new short(26);
  short* Num37_ = new short(37);
  short* Num81_ = new short(81);
  short* Num78_ = new short(78);
  short* Num81b_ = new short(81);

  SimpleNode<short>* Nodep_;
  PtrEqual<short> Comp_(SortUsingBuiltInOperators<short>::Trait());

  Alist_.AppendHead(Num13_);
  Alist_.AppendHead(Num26_);
  Alist_.AppendHead(Num37_);

  for (Nodep_ = Alist_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_.InsertAfter(Alist_.Find(Num26_, Comp_), Num81_);
  Alist_.InsertBefore(Alist_.Find(Num26_, Comp_), Num78_);

  for (Nodep_ = Alist_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_.Remove(Alist_.Find(Num13_));
  Alist_.Remove(Alist_.Find(Num26_));

  for (Nodep_ = Alist_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_.AppendHead(Num81b_);
  Nodep_ = Alist_.Find(Num81b_);
  if (Nodep_)
    cout <<(*Nodep_->_Object) <<" ";
  Nodep_ = Alist_.NextMatch(Nodep_);
  if (Nodep_)
    cout <<(*Nodep_->_Object) <<" ";

  cout <<endl;
  Alist2_ = Alist_;

  for (Nodep_ = Alist2_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;
  
  return 0;
}
#endif
/****************************************************************************/
#endif



