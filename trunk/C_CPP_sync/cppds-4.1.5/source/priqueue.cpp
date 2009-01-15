#ifndef PRIORITYQUEUE_CPP
#define PRIORITYQUEUE_CPP
#ifndef PRIORITYQUEUE_H
  #include "priqueue.h"
#endif
/****************************************************************************/
template <class T>
PriorityQueue<T>::PriorityQueue():
_PriorityFunc(NULL)
{}

/****************************************************************************/
template <class T>
PriorityQueue<T>::PriorityQueue(PriorityQueue<T>* List_, int StoreFlags_):
SingleList<T>(List_, StoreFlags_),
_PriorityFunc(List_ ? List_->_PriorityFunc:NULL)
{}

/****************************************************************************/
template <class T>
PriorityQueue<T>::PriorityQueue(SingleList<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_),
_PriorityFunc(NULL)
{}

/****************************************************************************/
template <class T>
PriorityQueue<T>::PriorityQueue(SingleSublist<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_),
_PriorityFunc(NULL)
{}

/****************************************************************************/
template <class T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& List_):
SingleList<T>(List_),
_PriorityFunc(List_._PriorityFunc)
{}

/****************************************************************************/
template <class T>
PriorityQueue<T>::PriorityQueue(const SingleList<T>& Sublist_):
SingleList<T>(Sublist_),
_PriorityFunc(NULL)
{}

/****************************************************************************/
template <class T>
PriorityQueue<T>::PriorityQueue(const SingleSublist<T>& Sublist_):
SingleList<T>(Sublist_),
_PriorityFunc(NULL)
{}

/****************************************************************************/
template <class T>    
PriorityQueue<T>* PriorityQueue<T>::Make()
{
  return (new PriorityQueue<T>());
}

/****************************************************************************/
template <class T>
PriorityQueue<T>* PriorityQueue<T>::Make(PriorityQueue<T>* List_, int StoreFlags_)
{
  return (new PriorityQueue<T>(List_, StoreFlags_));
}

/****************************************************************************/
template <class T>
PriorityQueue<T>* PriorityQueue<T>::Make(SingleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new PriorityQueue<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
PriorityQueue<T>* PriorityQueue<T>::Make(const PriorityQueue<T>& List_)
{
  return (new PriorityQueue<T>(List_));
}

/****************************************************************************/
template <class T>
PriorityQueue<T>* PriorityQueue<T>::Make(const SingleSublist<T>& Sublist_)
{
  return (new PriorityQueue<T>(Sublist_));
}

/****************************************************************************/
template <class T>
Object& PriorityQueue<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  PriorityQueueAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromPriorityQueue(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* PriorityQueue<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new PriorityQueue<T>(*this));

  PriorityQueueAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromPriorityQueue(TrgComp_)));
}

/****************************************************************************/
template <class T>
PriorityQueue<T>* PriorityQueue<T>::ClonePriorityQueue() const
{
  return (new PriorityQueue<T>(*this));
}

/****************************************************************************/
template <class T>
PriorityQueue<T>& PriorityQueue<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static PriorityQueue<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>    
const Object& PriorityQueue<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return PriorityQueue<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& PriorityQueue<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return PriorityQueue<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int PriorityQueue<T>::ClassID() const
{
  return TypeInfo::PRIORITYQUEUE_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* PriorityQueue<T>::GiveObjectAcceptor() const
{
  return (new PriorityQueueAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Boolean PriorityQueue<T>::IsEqualToPriorityQueue(const PriorityQueue<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean PriorityQueue<T>::IsLesserToPriorityQueue(const PriorityQueue<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean PriorityQueue<T>::IsGreaterToPriorityQueue(const PriorityQueue<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
PriorityQueue<T>& PriorityQueue<T>::operator = (const PriorityQueue<T>& List_)
{
  if (this != &List_)
  {
    SingleList<T>::operator = (List_);
    _PriorityFunc = List_._PriorityFunc;
  }

  return *this;
}

/****************************************************************************/
template <class T>
PriorityQueue<T>& PriorityQueue<T>::AssignListNode(SingleListNode<T>* Node_, int StoreFlags_)
{
  SingleList<T>::AssignListNode(Node_, StoreFlags_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean PriorityQueue<T>::Insert(SingleSublist<T>* List_, Boolean DelArgList_)
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
Boolean PriorityQueue<T>::Insert(SingleListNode<T>* Node_)
{
  size_t Size_ = SingleListNode<T>::NumNodes(Node_);
  
  if (Node_ && Node_->GetItem() && (Size() + Size_ <= Limit()) && _PriorityFunc)
  {
    if (_Head)
    {
      SingleListNode<T>* PrevPtr_ = NULL;
      SingleListNode<T>* ListPtr_ = _Head;
      T* ItemPtr_ = Node_->GetItem();
      
      while (ListPtr_)      
        if (ListPtr_->GetItem() == (T*)NULL ||
           ((*_PriorityFunc)(*ItemPtr_) - (*_PriorityFunc)(*ListPtr_->GetItem()) > 0))
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
Boolean PriorityQueue<T>::Insert(const T& Obj_)
{
  return Insert(SingleListNode<T>::Make(Obj_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>* PriorityQueue<T>::PopNode()
{
  return DetachFromTail();
}

/****************************************************************************/
template <class T>
T* PriorityQueue<T>::PopItem()
{
  SingleListNode<T>* ListNode_ = DetachFromTail();
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
T* PriorityQueue<T>::PeekItem()
{
  SingleListNode<T>* ListNode_ = Tail();
  T* Ptr_ = NULL;

  if (ListNode_)
    Ptr_ = ListNode_->GetItem();
  
  return Ptr_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PriorityQueue<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PriorityQueue<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* PriorityQueue<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PriorityQueue<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
PriorityQueueAcceptor<T>::PriorityQueueAcceptor(const PriorityQueue<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void PriorityQueueAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const PriorityQueue<T>*)Ptr_;
}

/****************************************************************************/
template <class T>    
const void* PriorityQueueAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean PriorityQueueAcceptor<T>::IsPriorityQueue() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* PriorityQueueAcceptor<T>::CreateFromPriorityQueue(const ObjectAcceptor& Obj_)
{
  _VarPtr = new PriorityQueue<T>(*((const PriorityQueue<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* PriorityQueueAcceptor<T>::CreateFromSingleList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new PriorityQueue<T>(*((const SingleList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* PriorityQueueAcceptor<T>::CreateFromSingleSublist(const ObjectAcceptor& Obj_)
{
  _VarPtr = new PriorityQueue<T>(*((const SingleSublist<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* PriorityQueueAcceptor<T>::AssignFromPriorityQueue(const ObjectAcceptor& Obj_)
{
  const PriorityQueue<T>* TrgPtr_ = (const PriorityQueue<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* PriorityQueueAcceptor<T>::AssignFromSingleList(const ObjectAcceptor& Obj_)
{
  const SingleList<T>* TrgPtr_ = (const SingleList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* PriorityQueueAcceptor<T>::AssignFromSingleSublist(const ObjectAcceptor& Obj_)
{
  const SingleSublist<T>* TrgPtr_ = (const SingleSublist<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean PriorityQueueAcceptor<T>::IsEqualToPriorityQueue(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToPriorityQueue((const PriorityQueue<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean PriorityQueueAcceptor<T>::IsLesserToPriorityQueue(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToPriorityQueue((const PriorityQueue<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean PriorityQueueAcceptor<T>::IsGreaterToPriorityQueue(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToPriorityQueue((const PriorityQueue<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PriorityQueueAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void PriorityQueueAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if PRIORITYQUEUE_DEBUG
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

int PriorityChar(const Char& c1)
{
  return c1.val();
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
  PriorityQueue<Char> s1, s2, s3;
  Boolean Valid_;
  size_t i;

  PriorityQueue<Char>::SetComparisonFunction(CompChar);

  Char* pc4;
  Char* pc5;

  s1.SetPriorityFunction(PriorityChar);
  s2.SetPriorityFunction(PriorityChar);
  s3.SetPriorityFunction(PriorityChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Insert(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Insert(SingleListNode<Char>::Make(c0));
    
  PriorityQueue<Char> s4(s3);
  PriorityQueue<Char> s5, s6;

  s4.SetPriorityFunction(PriorityChar);
  s5.SetPriorityFunction(PriorityChar);
  s6.SetPriorityFunction(PriorityChar);
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Insert(&sNode2_);
  s6.Insert(&sNode3_);
  for (i = 0; i < 9; i++)  
    s6.Insert(SingleListNode<Char>::Make(cb));

  PriorityQueue<Char> s7;
  s7.SetPriorityFunction(PriorityChar);
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
  s1.ShrinkTo(3); assert(s1 == "XXa");
  SingleListNode<Char> sNode5_(cb);
  SingleListNode<Char> sNode6_(cb);

  // test assigns
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
  s1.Insert(PriorityQueue<Char>::Make(s5)); assert(s1 == "aa");
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
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXaaabbbbbb" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == cb);
  assert(s1 == "XXXaaabbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXaaabbbb" && s1.Size() == 10);
  delete Node_;
  for (i = 0; i < 6; i++)
  {  
    Item_ = s1.PopItem();
    if (i < 4)
      assert(*Item_ == cb);
    else
      assert(*Item_ == ca);
    ::Delete(Item_);
  }
  assert(s1 == "XXXa" && s1.Size() == 4);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXX" && s1.Size() == 3);
  delete Node_;
  for (i = 0; i < 3; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == c0);
    ::Delete(Item_);
  }
  assert(s1.Size() == 0);  
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  pc5 = &c0;
  PriorityQueue<Char>* ps1 = PriorityQueue<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  PriorityQueue<Char>* ps2 = PriorityQueue<Char>::Make();
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
  SingleListNode<Char> sNode8_(c3.Set(x));
  s1.Insert(&sNode8_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s2.Insert(&sNode9_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Insert(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode10_(c3.Set(x));
  s3.Insert(&sNode10_);
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

#if PRIORITYQUEUE_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <PriorityQueue>" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




