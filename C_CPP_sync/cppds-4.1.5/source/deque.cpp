#ifndef DEQUE_CPP
#define DEQUE_CPP
#ifndef DEQUE_H
  #include "deque.h"
#endif
/****************************************************************************/
template <class T>
Deque<T>::Deque()
{}

/****************************************************************************/
template <class T>
Deque<T>::Deque(Deque<T>* List_, int StoreFlags_):
SingleList<T>(List_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Deque<T>::Deque(SingleList<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Deque<T>::Deque(SingleSublist<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Deque<T>::Deque(const Deque<T>& List_):
SingleList<T>(List_)
{}

/****************************************************************************/
template <class T>
Deque<T>::Deque(const SingleList<T>& Sublist_):
SingleList<T>(Sublist_)
{}

/****************************************************************************/
template <class T>
Deque<T>::Deque(const SingleSublist<T>& Sublist_):
SingleList<T>(Sublist_)
{}

/****************************************************************************/
template <class T>    
Deque<T>* Deque<T>::Make()
{
  return (new Deque<T>());
}

/****************************************************************************/
template <class T>
Deque<T>* Deque<T>::Make(Deque<T>* List_, int StoreFlags_)
{
  return (new Deque<T>(List_, StoreFlags_));
}

/****************************************************************************/
template <class T>
Deque<T>* Deque<T>::Make(SingleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new Deque<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
Deque<T>* Deque<T>::Make(const Deque<T>& List_)
{
  return (new Deque<T>(List_));
}

/****************************************************************************/
template <class T>
Deque<T>* Deque<T>::Make(const SingleSublist<T>& Sublist_)
{
  return (new Deque<T>(Sublist_));
}

/****************************************************************************/
template <class T>
Object& Deque<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  DequeAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromDeque(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* Deque<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Deque<T>(*this));

  DequeAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromDeque(TrgComp_)));
}

/****************************************************************************/
template <class T>
Deque<T>* Deque<T>::CloneDeque() const
{
  return (new Deque<T>(*this));
}

/****************************************************************************/
template <class T>
Deque<T>& Deque<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Deque<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>    
const Object& Deque<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Deque<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& Deque<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Deque<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int Deque<T>::ClassID() const
{
  return TypeInfo::DEQUE_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* Deque<T>::GiveObjectAcceptor() const
{
  return (new DequeAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Boolean Deque<T>::IsEqualToDeque(const Deque<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean Deque<T>::IsLesserToDeque(const Deque<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean Deque<T>::IsGreaterToDeque(const Deque<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
Deque<T>& Deque<T>::operator = (const Deque<T>& List_)
{
  if (this != &List_)
    SingleList<T>::operator = (List_);

  return *this;
}

/****************************************************************************/
template <class T>
Deque<T>& Deque<T>::AssignListNode(SingleListNode<T>* Node_, int StoreFlags_)
{
  SingleList<T>::AssignListNode(Node_, StoreFlags_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean Deque<T>::Push(SingleSublist<T>* List_, int Relation_, Boolean DelArgList_)
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
Boolean Deque<T>::Push(SingleListNode<T>* Node_, int Relation_)
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
Boolean Deque<T>::Push(const T& Obj_, int Relation_)
{
  return Push(SingleListNode<T>::Make(Obj_), Relation_);
}

/****************************************************************************/
template <class T>
SingleListNode<T>* Deque<T>::PopNode(int Relation_)
{
  return ((Relation_ == ListRelation::HEAD) ? DetachFromHead():DetachFromTail());
}

/****************************************************************************/
template <class T>
T* Deque<T>::PopItem(int Relation_)
{
  T* Ptr_ = NULL;

  if (Relation_ == ListRelation::HEAD)
  {
    SingleListNode<T>* ListNode_ = DetachFromHead();

    if (ListNode_)
    {
      Ptr_ = ListNode_->GetItem();
      ListNode_->ReleaseControl();
      delete ListNode_;
    }
  }
  else
  {
    SingleListNode<T>* ListNode_ = DetachFromTail();

    if (ListNode_)
    {
      Ptr_ = ListNode_->GetItem();
      ListNode_->ReleaseControl();
      delete ListNode_;
    }
  }
  
  return Ptr_;  
}

/****************************************************************************/
template <class T>
T* Deque<T>::PeekItem(int Relation_)
{
  T* Ptr_ = NULL;

  if (Relation_ == ListRelation::HEAD)
  {
    SingleListNode<T>* ListNode_ = Head();

    if (ListNode_)
      Ptr_ = ListNode_->GetItem();
  }
  else
  {
    SingleListNode<T>* ListNode_ = Tail();

    if (ListNode_)
      Ptr_ = ListNode_->GetItem();
  }
  
  return Ptr_;
}

/****************************************************************************/
template <class T>
SingleList<T>& Deque<T>::Shrink(size_t Num_, int Relation_)
{
  if (Num_ && Num_ <= Size())
  {
    size_t i;
    SingleListNode<T>* Rest_;

    if (Relation_ == ListRelation::HEAD)
    {
      for (i = 0; i < Num_; i++)
        if (Rest_ = DetachFromHead())
          delete Rest_;    
    }
    else
    {
      for (i = 0; i < Num_; i++)
        if (Rest_ = DetachFromTail())
          delete Rest_;
    }
  }
  
  return *this;
}

/****************************************************************************/
template <class T>
SingleList<T>& Deque<T>::ShrinkTo(size_t Num_, int Relation_)
{
  if (Size() >= Num_)
  {
    size_t ShrinkNum_ = Size() - Num_;
    Shrink(ShrinkNum_, Relation_);
  }

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* Deque<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Deque<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* Deque<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Deque<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
DequeAcceptor<T>::DequeAcceptor(const Deque<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void DequeAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Deque<T>*)Ptr_;
}

/****************************************************************************/
template <class T>    
const void* DequeAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean DequeAcceptor<T>::IsDeque() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* DequeAcceptor<T>::CreateFromDeque(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Deque<T>(*((const Deque<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* DequeAcceptor<T>::CreateFromSingleList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Deque<T>(*((const SingleList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* DequeAcceptor<T>::CreateFromSingleSublist(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Deque<T>(*((const SingleSublist<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* DequeAcceptor<T>::AssignFromDeque(const ObjectAcceptor& Obj_)
{
  const Deque<T>* TrgPtr_ = (const Deque<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* DequeAcceptor<T>::AssignFromSingleList(const ObjectAcceptor& Obj_)
{
  const SingleList<T>* TrgPtr_ = (const SingleList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* DequeAcceptor<T>::AssignFromSingleSublist(const ObjectAcceptor& Obj_)
{
  const SingleSublist<T>* TrgPtr_ = (const SingleSublist<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean DequeAcceptor<T>::IsEqualToDeque(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToDeque((const Deque<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean DequeAcceptor<T>::IsLesserToDeque(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToDeque((const Deque<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean DequeAcceptor<T>::IsGreaterToDeque(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToDeque((const Deque<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DequeAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void DequeAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if DEQUE_DEBUG
class Char
{
  DEFAULT_ROOTOBJECT_DEFN(Char)

  friend ostream& operator << (ostream& os, const Char& obj)
  {
    os <<obj.val();
    return os;
  }

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
  Deque<Char> s1, s2, s3;
  Boolean Valid_;
  size_t i;

  Char* pc4;
  Char* pc5;

  Deque<Char>::SetComparisonFunction(CompChar);

  SingleListNode<Char> sNode1_(c0);
  s3.Push(&sNode1_, ListRelation::HEAD);
  for (i = 0; i < 3; i++)
    s3.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
    
  Deque<Char> s4(s3);
  Deque<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Push(&sNode2_, ListRelation::HEAD);
  s6.Push(&sNode3_, ListRelation::HEAD);
  for (i = 0; i < 9; i++)  
    s6.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);

  Deque<Char> s7;
  SingleListNode<Char> sNode4_(c0);
  
  s7.Push(&sNode4_, ListRelation::HEAD);
  for (i = 1; i < 3; i++)
    s7.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  assert(s1 == "XX");
  s1.Push(SingleListNode<Char>::Make(ca), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(ca), ListRelation::HEAD);
  assert(s1 == "aaXX");
  s1.ShrinkTo(3, ListRelation::HEAD); assert(s1 == "aXX");

  // test assigns
  SingleListNode<Char> sNode5_(cb);
  SingleListNode<Char> sNode6_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode5_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;  
  s1.Push(Deque<Char>::Make(s5), ListRelation::HEAD); assert(s1 == "aa");
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  assert(s1 == "baa");
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  assert(s1 == "bbaa");
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);  
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(cb), ListRelation::HEAD);  
  assert(s1 == "bbbbbbbaa");
  s1.Push(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE), ListRelation::HEAD);
  assert(s1 == "abbbbbbbaa");

  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  s1.Push(SingleListNode<Char>::Make(c0), ListRelation::HEAD);
  assert(s1 == "XXXabbbbbbbaa" && s1.Size() == 13);

  s4 = s1;

  // Test removes -- head end
  SingleListNode<Char>* Node_ = s1.PopNode(ListRelation::HEAD);
  assert(*Node_->GetItem() == c0);
  assert(s1 == "XXabbbbbbbaa" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == c0);
  assert(s1 == "Xabbbbbbbaa" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode(ListRelation::HEAD);
  assert(*Node_->GetItem() == c0);
  assert(s1 == "abbbbbbbaa" && s1.Size() == 10);
  delete Node_;
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == ca);
  assert(s1 == "bbbbbbbaa" && s1.Size() == 9);
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == cb);
  ::Delete(Item_);
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == cb);
  ::Delete(Item_);  
  assert(s1 == "bbbbbaa" && s1.Size() == 7);
  for (i = 0; i < 5; i++)
  {
    Item_ = s1.PopItem(ListRelation::HEAD);
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "aa" && s1.Size() == 2);  
  Node_ = s1.PopNode(ListRelation::HEAD);
  assert(*Node_->GetItem() == ca);
  assert(s1 == "a" && s1.Size() == 1);
  delete Node_;
  Item_ = s1.PopItem(ListRelation::HEAD);
  assert(*Item_ == ca);
  assert(s1.Size() == 0);
  ::Delete(Item_);
  s1 = s4;

  // Test removes -- tail end
  Node_ = s1.PopNode(ListRelation::TAIL);
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXXabbbbbbba" && s1.Size() == 12);
  delete Node_;
  Item_ = s1.PopItem(ListRelation::TAIL);
  assert(*Item_ == ca);
  assert(s1 == "XXXabbbbbbb" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode(ListRelation::TAIL);
  assert(*Node_->GetItem() == cb);
  assert(s1 == "XXXabbbbbb" && s1.Size() == 10);
  delete Node_;
  for (i = 0; i < 6; i++)
  {
    Item_ = s1.PopItem(ListRelation::TAIL);
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "XXXa" && s1.Size() == 4);
  Node_ = s1.PopNode(ListRelation::TAIL);
  assert(*Node_->GetItem() == ca);
  assert(s1 == "XXX" && s1.Size() == 3);
  delete Node_;
  for (i = 0; i < 3; i++)
  {
    Item_ = s1.PopItem(ListRelation::TAIL);
    assert(*Item_ == c0);
    ::Delete(Item_);
  }
  assert(s1.Size() == 0);  
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  pc5 = &c0;
  Deque<Char>* ps1 = Deque<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  Deque<Char>* ps2 = Deque<Char>::Make();
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
  SingleListNode<Char> sNode7_(c3.Set(x));  
  s1.Push(&sNode7_, ListRelation::TAIL);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Push(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::TAIL);

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode8_(c3.Set(x));  
  s2.Push(&sNode8_, ListRelation::TAIL);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Push(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::TAIL);

  x = 75;
  SingleListNode<Char> sNode9_(c3.Set(x));  
  s3.Push(&sNode9_, ListRelation::TAIL);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Push(SingleListNode<Char>::Make(c3.Set(x)), ListRelation::TAIL);

  s4 = s1;
  s5 = s2;
  s6 = s3;

  s2.GetNode(4)->SetItem(Char('X'));
  s3.GetNode(4)->SetItem(Char('Y'));

  assert(s1.IsEqual(s4));
  assert(s1.IsLesser(s3));
  assert(s2.IsGreater(s3));
  
  assert(s1 == "ABCDE" && s1.Size() == 5);
  assert(s2 == "ZYXWX" && s2.Size() == 5);
  assert(s3 == "KLMNY" && s3.Size() == 5);

  assert(s1 == s4);
  assert(s1 < s3);
  assert(s2 > s3);
  assert(s1 != s3);
  
#if DEQUE_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <Deque>" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




