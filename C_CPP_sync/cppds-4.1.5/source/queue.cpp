#ifndef QUEUE_CPP
#define QUEUE_CPP
#ifndef QUEUE_H
  #include "queue.h"
#endif
/****************************************************************************/
template <class T>
Queue<T>::Queue()
{}

/****************************************************************************/
template <class T>
Queue<T>::Queue(Queue<T>* List_, int StoreFlags_):
SingleList<T>(List_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Queue<T>::Queue(SingleList<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Queue<T>::Queue(SingleSublist<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Queue<T>::Queue(const Queue<T>& List_):
SingleList<T>(List_)
{}

/****************************************************************************/
template <class T>
Queue<T>::Queue(const SingleList<T>& Sublist_):
SingleList<T>(Sublist_)
{}

/****************************************************************************/
template <class T>
Queue<T>::Queue(const SingleSublist<T>& Sublist_):
SingleList<T>(Sublist_)
{}

/****************************************************************************/
template <class T>    
Queue<T>* Queue<T>::Make()
{
  return (new Queue<T>());
}

/****************************************************************************/
template <class T>
Queue<T>* Queue<T>::Make(Queue<T>* List_, int StoreFlags_)
{
  return (new Queue<T>(List_, StoreFlags_));
}

/****************************************************************************/
template <class T>
Queue<T>* Queue<T>::Make(SingleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new Queue<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
Queue<T>* Queue<T>::Make(const Queue<T>& List_)
{
  return (new Queue<T>(List_));
}

/****************************************************************************/
template <class T>
Queue<T>* Queue<T>::Make(const SingleSublist<T>& Sublist_)
{
  return (new Queue<T>(Sublist_));
}

/****************************************************************************/
template <class T>
Object& Queue<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  QueueAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromQueue(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* Queue<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Queue<T>(*this));

  QueueAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromQueue(TrgComp_)));
}

/****************************************************************************/
template <class T>
Queue<T>* Queue<T>::CloneQueue() const
{
  return (new Queue<T>(*this));
}

/****************************************************************************/
template <class T>
Queue<T>& Queue<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Queue<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>    
const Object& Queue<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Queue<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& Queue<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Queue<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int Queue<T>::ClassID() const
{
  return TypeInfo::QUEUE_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* Queue<T>::GiveObjectAcceptor() const
{
  return (new QueueAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Boolean Queue<T>::IsEqualToQueue(const Queue<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean Queue<T>::IsLesserToQueue(const Queue<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean Queue<T>::IsGreaterToQueue(const Queue<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
Queue<T>& Queue<T>::operator = (const Queue<T>& List_)
{
  if (this != &List_)
    SingleList<T>::operator = (List_);

  return *this;
}

/****************************************************************************/
template <class T>
Queue<T>& Queue<T>::AssignListNode(SingleListNode<T>* Node_, int StoreFlags_)
{
  SingleList<T>::AssignListNode(Node_, StoreFlags_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean Queue<T>::Enqueue(SingleSublist<T>* List_, Boolean DelArgList_)
{
  if (List_ && List_->NumNodes() + Size() <= Limit())
  {
    List_->ReleaseControl();
    AddSublist(List_, ListRelation::HEAD);
    if (List_->IsNewed() && DelArgList_)
      delete List_;

    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean Queue<T>::Enqueue(SingleListNode<T>* Node_)
{
  size_t Size_ = SingleListNode<T>::NumNodes(Node_);
  
  if (Node_ && (Size() + Size_ <= Limit()))
  {
    if (_Head)
    {
      _Head->Add(Node_, ListRelation::HEAD);

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
Boolean Queue<T>::Enqueue(const T& Obj_)
{
  return Enqueue(SingleListNode<T>::Make(Obj_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>* Queue<T>::PopNode()
{
  return DetachFromTail();
}

/****************************************************************************/
template <class T>
T* Queue<T>::PopItem()
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
T* Queue<T>::PeekItem()
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
void* Queue<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Queue<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* Queue<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Queue<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
QueueAcceptor<T>::QueueAcceptor(const Queue<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void QueueAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Queue<T>*)Ptr_;
}

/****************************************************************************/
template <class T>    
const void* QueueAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean QueueAcceptor<T>::IsQueue() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* QueueAcceptor<T>::CreateFromQueue(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Queue<T>(*((const Queue<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* QueueAcceptor<T>::CreateFromSingleList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Queue<T>(*((const SingleList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* QueueAcceptor<T>::CreateFromSingleSublist(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Queue<T>(*((const SingleSublist<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* QueueAcceptor<T>::AssignFromQueue(const ObjectAcceptor& Obj_)
{
  const Queue<T>* TrgPtr_ = (const Queue<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* QueueAcceptor<T>::AssignFromSingleList(const ObjectAcceptor& Obj_)
{
  const SingleList<T>* TrgPtr_ = (const SingleList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* QueueAcceptor<T>::AssignFromSingleSublist(const ObjectAcceptor& Obj_)
{
  const SingleSublist<T>* TrgPtr_ = (const SingleSublist<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean QueueAcceptor<T>::IsEqualToQueue(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToQueue((const Queue<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean QueueAcceptor<T>::IsLesserToQueue(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToQueue((const Queue<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean QueueAcceptor<T>::IsGreaterToQueue(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToQueue((const Queue<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* QueueAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void QueueAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if QUEUE_DEBUG
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
  // test basic workings
  Char c0, c3, ca('a'), cb('b');
  Queue<Char> s1, s2, s3;
  Boolean Valid_;
  size_t i;

  Char* pc4;
  Char* pc5;

  Queue<Char>::SetComparisonFunction(CompChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Enqueue(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Enqueue(SingleListNode<Char>::Make(c0));
    
  Queue<Char> s4(s3);
  Queue<Char> s5, s6;
  SingleListNode<Char> sNodea_(ca);
  SingleListNode<Char> sNodeb_(cb);

  s5.Enqueue(&sNodea_);
  s6.Enqueue(&sNodeb_);
  for (i = 0; i < 9; i++)  
    s6.Enqueue(SingleListNode<Char>::Make(cb));

  Queue<Char> s7;
  SingleListNode<Char> sNode2_(c0);
  
  s7.Enqueue(&sNode2_);
  for (i = 1; i < 3; i++)
    s7.Enqueue(SingleListNode<Char>::Make(c0));

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  assert(s1 == "XX");
  s1.Enqueue(SingleListNode<Char>::Make(ca));
  s1.Enqueue(SingleListNode<Char>::Make(ca));
  assert(s1 == "aaXX");
  s1.ShrinkTo(3); assert(s1 == "aaX");

  // test assigns
  SingleListNode<Char> sNode3_(cb);
  SingleListNode<Char> sNode4_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode3_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode4_, DataStorageAttributes::DEEP_COPY);
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;
  s1.Enqueue(Queue<Char>::Make(s5)); assert(s1 == "aa");
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  assert(s1 == "baa");
  s1.Enqueue(SingleListNode<Char>::Make(cb));
  assert(s1 == "bbaa");
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));  
  s1.Enqueue(SingleListNode<Char>::Make(cb));    
  assert(s1 == "bbbbbbbaa");
  s1.Enqueue(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE));
  assert(s1 == "abbbbbbbaa");
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  s1.Enqueue(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXabbbbbbbaa" && s1.Size() == 13);
  
  s4 = s1;

  // Test removes
  SingleListNode<Char>* Node_ = s1.PopNode();
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXXabbbbbbba" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1 == "XXXabbbbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXabbbbbb" && s1.Size() == 10);
  delete Node_;
  for (i = 0; i < 6; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == cb);
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
  Queue<Char>* ps1 = Queue<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  Queue<Char>* ps2 = Queue<Char>::Make();
  assert(!(*ps2 == *ps1));
  Assign(*ps2, *ps1);
  assert(*ps2 == *ps1);
  assert(!(*ps2 == s7));

  // Test binary IO methods
  assert(!(s6 == *ps2));
  ofstream fout("testing.bin");
  s6.BinaryWrite(fout);
  fout.close();

  ifstream fin("testing.bin");
  ps2->BinaryRead(fin);
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
  SingleListNode<Char> sNode5_(c3.Set(x));
  s1.Enqueue(&sNode5_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Enqueue(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode6_(c3.Set(x));
  s2.Enqueue(&sNode6_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Enqueue(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode7_(c3.Set(x));
  s3.Enqueue(&sNode7_);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Enqueue(SingleListNode<Char>::Make(c3.Set(x)));

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  assert(s1 == "EDCBA" && s1.Size() == 5);
  assert(s2 == "VWXYX" && s2.Size() == 5);
  assert(s3 == "ONMLY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);

#if QUEUE_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <Queue>" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




