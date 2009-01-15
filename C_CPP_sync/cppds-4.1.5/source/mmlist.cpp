#ifndef MMLIST_CPP
#define MMLIST_CPP
#ifndef MMLIST_H
  #include "mmlist.h"
#endif

/****************************************************************************/
/******************* Custom List Node For Memory Manager ********************/
/****************************************************************************/
template <class T>
MMNode<T>::MMNode(T* Obj_, MMNode<T>* Next_):
_Object(Obj_),
_Next(Next_)
{}

/****************************************************************************/
template <class T>
MMNode<T>::~MMNode()
{
  delete _Next;
  _Next = NULL;

  delete _Object;
  _Object = NULL;
}

/****************************************************************************/
template <class T>
void MMNode<T>::Insert(MMNode<T>* Node_)
{
  Node_->_Next = _Next;
  _Next = Node_;
}

/****************************************************************************/
/****************** Custom Linked List For Memory Manager *******************/
/****************************************************************************/
template <class T>
MMList<T>::MMList():
_List(NULL)
{}

/****************************************************************************/
template <class T>
MMList<T>::MMList(MMNode<T>* NodePtr_):
_List(NodePtr_)
{}

/****************************************************************************/
template <class T>
MMList<T>::~MMList()
{
  delete _List;
  _List = NULL;
}

/****************************************************************************/
template <class T>
MMNode<T>* MMList<T>::Find(const T* Obj_, PtrCompare<T>& Compare_) const
{
  MMNode<T>* Node_ = _List;

  if (Obj_)
    while (Node_ && !Compare_(Obj_, Node_->_Object))
      Node_ = Node_->_Next;

  return ((Obj_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
MMNode<T>* MMList<T>::Find(const T* Obj_) const
{
  PtrEqual<T> Comp_(MatchUsingBuiltInOperators<T>::Trait());
  return Find(Obj_, Comp_);
}

/****************************************************************************/
template <class T>
MMNode<T>* MMList<T>::FindPrevious(const MMNode<T>* Ptr_) const
{
  MMNode<T>* Node_ = _List;

  if (Ptr_ == Node_)
    return NULL;

  if (Ptr_)
    while (Node_ && Ptr_ != Node_->_Next)
      Node_ = Node_->_Next;

  return ((Ptr_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
void MMList<T>::Remove(MMNode<T>* Ptr_)
{
  if (Ptr_ == _List)
    delete PopNode();
  else
  {
    MMNode<T>* Node_ = FindPrevious(Ptr_);

    if (Node_)
    {
      Node_->_Next = Ptr_->_Next;
      Ptr_->_Next = NULL;
      delete Ptr_;
    }
  }
}

/****************************************************************************/
template <class T>
void MMList<T>::InsertAfter(MMNode<T>* Ptr_, T* NewPtr_)
{
  if (Ptr_)
    Ptr_->Insert(new MMNode<T>(NewPtr_));
}

/****************************************************************************/
template <class T>
void MMList<T>::InsertBefore(MMNode<T>* Ptr_, T* NewPtr_)
{
  if (Ptr_ == _List)
    AppendHead(NewPtr_);
  else
  {
    MMNode<T>* Node_ = FindPrevious(Ptr_);
    InsertAfter(Node_, NewPtr_);
  }
}

/****************************************************************************/
template <class T>
MMList<T>& MMList<T>::AppendHead(T* Obj_)
{
  _List = new MMNode<T>(Obj_, _List);
  return *this;
}

/****************************************************************************/
template <class T>
MMList<T>& MMList<T>::AppendTail(T* Obj_)
{
  MMNode<T>* Node_ = _List;

  if (Obj_)
    if (_List)
    {
      while (Node_->_Next)
	Node_ = Node_->_Next;
      Node_->_Next = new MMNode<T>(Obj_, Node_->_Next);
    }
    else
      _List = new MMNode<T>(Obj_, _List);

  return *this;
}

/****************************************************************************/
template <class T>
T* MMList<T>::PopNode()
{
  MMNode<T>* Node_ = _List;

  if (Node_ == NULL)
    return NULL;

  T* Object_ = Node_->_Object;
  Node_->_Object = NULL; 	// So node won't try to delete it
  _List = Node_->_Next;
  Node_->_Next = NULL; 		// So it won't delete the list

  delete Node_;
  return Object_;
}

/****************************************************************************/
template <class T>
void MMList<T>::DeleteAll()
{
  delete _List;
  _List = NULL;
}

/****************************************************************************/
template <class T>
BOOLTYPE MMList<T>::Empty() const
{
  return (_List == NULL);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
MMListBrowser<T>::MMListBrowser(MMNode<T>* Ptr_):
MMList<T>(Ptr_)
{}

/****************************************************************************/
template <class T>
MMListBrowser<T>::MMListBrowser(const MMList<T>& Alist_):
MMList<T>(Alist_.Head())
{}

/****************************************************************************/
template <class T>
MMListBrowser<T>::~MMListBrowser()
{
  _List = NULL;
}

/****************************************************************************/
template <class T>
MMListBrowser<T>& MMListBrowser<T>::operator = (const MMList<T>& Alist_)
{
  if (this != &Alist_)
    _List = Alist_.Head();

  return *this;
}

/****************************************************************************/
template <class T>
MMListBrowser<T>& MMListBrowser<T>::operator = (MMNode<T>* Ptr_)
{
  _List = Ptr_;
  return *this;
}

/****************************************************************************/
template <class T>
MMNode<T>* MMListBrowser<T>::NextMatch(const T* Obj_, PtrCompare<T>& Compare_)
{
  if (_List)
  {
    _List = _List->_Next;
    _List = Find(Obj_, Compare_);
  }

  return _List;
}

/****************************************************************************/
template <class T>
MMNode<T>* MMListBrowser<T>::NextMatch(const T* Obj_)
{
  PtrEqual<T> Comp_(MatchUsingBuiltInOperators<T>::Trait());
  return NextMatch(Obj_, Comp_);
}

/****************************************************************************/
template <class T>
MMNode<T>* MMListBrowser<T>::IndexNode(size_t Index_)
{
  for (size_t Count_ = 0; _List && Count_ < Index_; Count_++)
    ++(*this);

  return _List;
}

/****************************************************************************/
template <class T>
MMNode<T>* MMListBrowser<T>::operator ++ ()
{
  if (_List)
    _List = _List->_Next;

  return _List;
}

/****************************************************************************/
template <class T>
MMNode<T>* MMListBrowser<T>::operator ++ (int)
{
  MMNode<T>* RetPtr_ = _List;

  if (_List)
    _List = _List->_Next;

  return RetPtr_;
}

/****************************************************************************/
/****************************************************************************/
#if MMLIST_DEBUG
#include <conio.h>
void main()
{
  clrscr();
  MMList<short>* Alist_ = new MMList<short>;

  short
  *Num13_ = new short(13),
  *Num26_ = new short(26),
  *Num37_ = new short(37),
  *Num81_ = new short(81),
  *Num78_ = new short(78);

  MMNode<short>* Nodep_;
  PtrEqual<short> Comp_;

  Alist_->AppendHead(Num13_);
  Alist_->AppendHead(Num26_);
  Alist_->AppendHead(Num37_);

  for (Nodep_ = Alist_->Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_->InsertAfter(Alist_->Find(Num26_, Comp_), Num81_);
  Alist_->InsertBefore(Alist_->Find(Num26_, Comp_), Num78_);

  for (Nodep_ = Alist_->Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_->Remove(Alist_->Find(Num13_));
  Alist_->Remove(Alist_->Find(Num26_));

  for (Nodep_ = Alist_->Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  Alist_->AppendHead(Num81_);
  Nodep_ = Alist_->Find(Num81_);
  if (Nodep_)
    cout <<(*Nodep_->_Object) <<" ";

  MMListBrowser<short> Browse_ = *Alist_;

  Nodep_ = Browse_.NextMatch(Nodep_->_Object);
  if (Nodep_)
    cout <<(*Nodep_->_Object) <<" ";

  cout <<endl;
  MMListBrowser<short> Alist2_ = *Alist_;

  for (Nodep_ = Alist2_.Head(); Nodep_; Nodep_ = Nodep_->_Next)
    cout <<(*Nodep_->_Object) <<" ";
  cout <<endl;

  delete Alist_;
}
#endif
/****************************************************************************/
#endif




