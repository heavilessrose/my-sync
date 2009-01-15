#ifndef SORTEDLIST_CPP
#define SORTEDLIST_CPP
#ifndef SORTEDLIST_H
  #include "sortlist.h"
#endif
/****************************************************************************/
template <class T>
SortedList<T>::SortedList():
_SortOrder(SortOrder::INCREASING)
{}

/****************************************************************************/
template <class T>
SortedList<T>::SortedList(SortedList<T>* List_, int StoreFlags_):
SingleList<T>(List_, StoreFlags_),
_SortOrder(List_ ? List_->_SortOrder:SortOrder::INCREASING)
{}

/****************************************************************************/
template <class T>
SortedList<T>::SortedList(SingleList<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_),
_SortOrder(SortOrder::INCREASING)
{}

/****************************************************************************/
template <class T>
SortedList<T>::SortedList(SingleSublist<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_),
_SortOrder(SortOrder::INCREASING)
{}

/****************************************************************************/
template <class T>
SortedList<T>::SortedList(const SortedList<T>& List_):
SingleList<T>(List_),
_SortOrder(List_._SortOrder)
{}

/****************************************************************************/
template <class T>
SortedList<T>::SortedList(const SingleList<T>& Sublist_):
SingleList<T>(Sublist_),
_SortOrder(SortOrder::INCREASING)
{}

/****************************************************************************/
template <class T>
SortedList<T>::SortedList(const SingleSublist<T>& Sublist_):
SingleList<T>(Sublist_),
_SortOrder(SortOrder::INCREASING)
{}

/****************************************************************************/
template <class T>    
SortedList<T>* SortedList<T>::Make()
{
  return (new SortedList<T>());
}

/****************************************************************************/
template <class T>
SortedList<T>* SortedList<T>::Make(SortedList<T>* List_, int StoreFlags_)
{
  return (new SortedList<T>(List_, StoreFlags_));
}

/****************************************************************************/
template <class T>
SortedList<T>* SortedList<T>::Make(SingleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new SortedList<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
SortedList<T>* SortedList<T>::Make(const SortedList<T>& List_)
{
  return (new SortedList<T>(List_));
}

/****************************************************************************/
template <class T>
SortedList<T>* SortedList<T>::Make(const SingleSublist<T>& Sublist_)
{
  return (new SortedList<T>(Sublist_));
}

/****************************************************************************/
template <class T>
Object& SortedList<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  SortedListAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromSortedList(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* SortedList<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new SortedList<T>(*this));

  SortedListAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromSortedList(TrgComp_)));
}

/****************************************************************************/
template <class T>
SortedList<T>* SortedList<T>::CloneSortedList() const
{
  return (new SortedList<T>(*this));
}

/****************************************************************************/
template <class T>
SortedList<T>& SortedList<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static SortedList<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>    
const Object& SortedList<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return SortedList<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& SortedList<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return SortedList<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int SortedList<T>::ClassID() const
{
  return TypeInfo::SORTEDLIST_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* SortedList<T>::GiveObjectAcceptor() const
{
  return (new SortedListAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Boolean SortedList<T>::IsEqualToSortedList(const SortedList<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean SortedList<T>::IsLesserToSortedList(const SortedList<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean SortedList<T>::IsGreaterToSortedList(const SortedList<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
SortedList<T>& SortedList<T>::operator = (const SortedList<T>& List_)
{
  if (this != &List_)
  {
    SingleList<T>::operator = (List_);
    _SortOrder = List_._SortOrder;
  }

  return *this;
}

/****************************************************************************/
template <class T>
SortedList<T>& SortedList<T>::AssignListNode(SingleListNode<T>* Node_, int StoreFlags_)
{
  SingleList<T>::AssignListNode(Node_, StoreFlags_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean SortedList<T>::Insert(SingleSublist<T>* List_, Boolean DelArgList_)
{
  if (List_ && List_->NumNodes() + Size() <= Limit())
  {
    List_->ReleaseControl();
    SingleListNode<T>* ListPtr_ = List_->Head();
    SingleListNode<T>* NextPtr_;

    while (ListPtr_)
    {
      NextPtr_ = ListPtr_->Next();
      ListPtr_->DetachNode();
      Insert(ListPtr_);
      ListPtr_ = NextPtr_;
    }

    if (List_->IsNewed() && DelArgList_)
      delete List_;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SortedList<T>::Insert(SingleListNode<T>* Node_)
{
  size_t Size_ = SingleListNode<T>::NumNodes(Node_);
  
  if (Node_ && Node_->GetItem() && (Size() + Size_ <= Limit()) && _CompFunc)
  {
    if (_Head)
    {
      SingleListNode<T>* PrevPtr_ = NULL;
      SingleListNode<T>* ListPtr_ = _Head;
      T* ItemPtr_ = Node_->GetItem();
      
      while (ListPtr_)      
        if (ListPtr_->GetItem() == (T*)NULL ||
           ((*_CompFunc)(*ItemPtr_, *ListPtr_->GetItem()) * _SortOrder > 0))
        {
          PrevPtr_ = ListPtr_;
          ListPtr_ = ListPtr_->Next();
        }
        else
          break;

      if (ListPtr_)
      {
        if (PrevPtr_)
          AddListNode(Node_, PrevPtr_);
        else
          AddListNode(Node_, ListRelation::HEAD);
      }
      else
        AddListNode(Node_, ListRelation::TAIL);
    }
    else
      AssignListNode(Node_, Node_->GetStorageAttributes());

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean SortedList<T>::Insert(const T& Obj_)
{
  return Insert(SingleListNode<T>::Make(Obj_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>* SortedList<T>::PopNode()
{
  return DetachFromHead();
}

/****************************************************************************/
template <class T>
T* SortedList<T>::PopItem()
{
  SingleListNode<T>* ListNode_ = DetachFromHead();
  T* Ptr_ = NULL;

  if (ListNode_)
  {  
    Ptr_ = ListNode_->GetItem();
    ListNode_->ReleaseControl();
    delete ListNode_;
  }
  
  return Ptr_;
}

/****************************************************************************/
template <class T>
T* SortedList<T>::PeekItem()
{
  SingleListNode<T>* ListNode_ = Head();
  T* Ptr_ = NULL;

  if (ListNode_)
    Ptr_ = ListNode_->GetItem();
  
  return Ptr_;
}

/****************************************************************************/
template <class T>
SingleList<T>& SortedList<T>::Shrink(size_t Num_)
{
  if (Num_ && Num_ <= Size())
  {
    size_t i;
    SingleListNode<T>* Rest_;

    for (i = 0; i < Num_; i++)
      if (Rest_ = DetachFromHead())
        delete Rest_;
  }
  
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SortedList<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SortedList<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* SortedList<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void SortedList<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
SortedListAcceptor<T>::SortedListAcceptor(const SortedList<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void SortedListAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const SortedList<T>*)Ptr_;
}

/****************************************************************************/
template <class T>    
const void* SortedListAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean SortedListAcceptor<T>::IsSortedList() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* SortedListAcceptor<T>::CreateFromSortedList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new SortedList<T>(*((const SortedList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* SortedListAcceptor<T>::CreateFromSingleList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new SortedList<T>(*((const SingleList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* SortedListAcceptor<T>::CreateFromSingleSublist(const ObjectAcceptor& Obj_)
{
  _VarPtr = new SortedList<T>(*((const SingleSublist<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* SortedListAcceptor<T>::AssignFromSortedList(const ObjectAcceptor& Obj_)
{
  const SortedList<T>* TrgPtr_ = (const SortedList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* SortedListAcceptor<T>::AssignFromSingleList(const ObjectAcceptor& Obj_)
{
  const SingleList<T>* TrgPtr_ = (const SingleList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* SortedListAcceptor<T>::AssignFromSingleSublist(const ObjectAcceptor& Obj_)
{
  const SingleSublist<T>* TrgPtr_ = (const SingleSublist<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean SortedListAcceptor<T>::IsEqualToSortedList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToSortedList((const SortedList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean SortedListAcceptor<T>::IsLesserToSortedList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToSortedList((const SortedList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean SortedListAcceptor<T>::IsGreaterToSortedList(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToSortedList((const SortedList<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* SortedListAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void SortedListAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if SORTEDLIST_DEBUG
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
  if (!s)
    return 0;

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

SingleList<Char> operator + (const SingleList<Char>&, const Char&);
SingleList<Char> operator + (const Char&, const SingleList<Char>&);
SingleList<Char> operator + (const Char&, const SingleList<Char>&);

int main()
{
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  SortedList<Char> s1, s2, s3;
  Boolean Valid_;
  size_t i;

  Char* pc4;
  Char* pc5;

  SortedList<Char>::SetComparisonFunction(CompChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Insert(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Insert(SingleListNode<Char>::Make(c0));
    
  SortedList<Char> s4(s3);
  SortedList<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Insert(&sNode2_);
  s6.Insert(&sNode3_);
  for (i = 0; i < 9; i++)  
    s6.Insert(SingleListNode<Char>::Make(cb));

  SortedList<Char> s7;
  SingleListNode<Char> sNode4_(c0);
  
  s7.Insert(&sNode4_);
  for (i = 1; i < 3; i++)
    s7.Insert(SingleListNode<Char>::Make(c0));

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XX");
  s1.Insert(SingleListNode<Char>::Make(ca));
  s1.Insert(SingleListNode<Char>::Make(ca));
  assert(s1 == "XXaa");
  s1.ShrinkTo(3); assert(s1 == "Xaa");

  // test assigns
  SingleListNode<Char> sNode5_(cb);
  SingleListNode<Char> sNode6_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode5_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));
  s1.Insert(SingleListNode<Char>::Make(cb));  
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;
  s1.Insert(SortedList<Char>::Make(s5)); assert(s1 == "aa");
  s1.Insert(SingleListNode<Char>::Make(cb));
  assert(s1 == "aab");
  s1.Insert(SingleListNode<Char>::Make(cb));
  assert(s1 == "aabb");
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));  
  s1.Insert(SingleListNode<Char>::Make(cb));    
  assert(s1 == "aabbbbbbb");
  s1.Insert(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE));
  assert(s1 == "aaabbbbbbb");
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  s1.Insert(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXaaabbbbbbb" && s1.Size() == 13);
  
  s4 = s1;

  // Test removes
  SingleListNode<Char>* Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "XXaaabbbbbbb" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == c0);
  assert(s1 == "Xaaabbbbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "aaabbbbbbb" && s1.Size() == 10);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1 == "aabbbbbbb" && s1.Size() == 9);
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  ::Delete(Item_);
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  ::Delete(Item_);  
  assert(s1 == "bbbbbbb" && s1.Size() == 7);
  for (i = 0; i < 5; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "bb" && s1.Size() == 2);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == cb);
  assert(s1 == "b" && s1.Size() == 1);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == cb);
  assert(s1.Size() == 0);
  ::Delete(Item_);
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  pc5 = &c0;
  SortedList<Char>* ps1 = SortedList<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  SortedList<Char>* ps2 = SortedList<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test binary IO methods
  assert(!(s6 == *ps2));
  ofstream fout("testing.bin");
  Write(fout, s6);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, *ps2);
  fin.close();
  assert(s6 == *ps2);

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
  SingleListNode<Char> sNode7_(c3.Set(x));
  s1.Insert(&sNode7_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode8_(c3.Set(x));
  s2.Insert(&sNode8_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s3.Insert(&sNode9_);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  assert(s1 == "ABCDE" && s1.Size() == 5);
  assert(s2 == "VWXYX" && s2.Size() == 5);
  assert(s3 == "KLMNY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);

#if SORTEDLIST_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <SortedList>" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




