#ifndef STACK_CPP
#define STACK_CPP
#ifndef STACK_H
  #include "stack.h"
#endif
/****************************************************************************/
template <class T>
Stack<T>::Stack()
{}

/****************************************************************************/
template <class T>
Stack<T>::Stack(Stack<T>* List_, int StoreFlags_):
SingleList<T>(List_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Stack<T>::Stack(SingleList<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Stack<T>::Stack(SingleSublist<T>* Sublist_, int StoreFlags_):
SingleList<T>(Sublist_, StoreFlags_)
{}

/****************************************************************************/
template <class T>
Stack<T>::Stack(const Stack<T>& List_):
SingleList<T>(List_)
{}

/****************************************************************************/
template <class T>
Stack<T>::Stack(const SingleList<T>& Sublist_):
SingleList<T>(Sublist_)
{}

/****************************************************************************/
template <class T>
Stack<T>::Stack(const SingleSublist<T>& Sublist_):
SingleList<T>(Sublist_)
{}

/****************************************************************************/
template <class T>    
Stack<T>* Stack<T>::Make()
{
  return (new Stack<T>());
}

/****************************************************************************/
template <class T>
Stack<T>* Stack<T>::Make(Stack<T>* List_, int StoreFlags_)
{
  return (new Stack<T>(List_, StoreFlags_));
}

/****************************************************************************/
template <class T>
Stack<T>* Stack<T>::Make(SingleSublist<T>* Sublist_, int StoreFlags_)
{
  return (new Stack<T>(Sublist_, StoreFlags_));
}

/****************************************************************************/
template <class T>
Stack<T>* Stack<T>::Make(const Stack<T>& List_)
{
  return (new Stack<T>(List_));
}

/****************************************************************************/
template <class T>
Stack<T>* Stack<T>::Make(const SingleSublist<T>& Sublist_)
{
  return (new Stack<T>(Sublist_));
}

/****************************************************************************/
template <class T>
Object& Stack<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  StackAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromStack(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* Stack<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Stack<T>(*this));

  StackAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromStack(TrgComp_)));
}

/****************************************************************************/
template <class T>
Stack<T>* Stack<T>::CloneStack() const
{
  return (new Stack<T>(*this));
}

/****************************************************************************/
template <class T>
Stack<T>& Stack<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Stack<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>    
const Object& Stack<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Stack<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& Stack<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Stack<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
int Stack<T>::ClassID() const
{
  return TypeInfo::STACK_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* Stack<T>::GiveObjectAcceptor() const
{
  return (new StackAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
Boolean Stack<T>::IsEqualToStack(const Stack<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean Stack<T>::IsLesserToStack(const Stack<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean Stack<T>::IsGreaterToStack(const Stack<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (CompareSublist(*Ptr_, _CompFunc) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
Stack<T>& Stack<T>::operator = (const Stack<T>& List_)
{
  if (this != &List_)
    SingleList<T>::operator = (List_);

  return *this;
}

/****************************************************************************/
template <class T>
Stack<T>& Stack<T>::AssignListNode(SingleListNode<T>* Node_, int StoreFlags_)
{
  SingleList<T>::AssignListNode(Node_, StoreFlags_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean Stack<T>::Push(SingleSublist<T>* List_, Boolean DelArgList_)
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
Boolean Stack<T>::Push(SingleListNode<T>* Node_)
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
Boolean Stack<T>::Push(const T& Obj_)
{
  return Push(SingleListNode<T>::Make(Obj_));
}

/****************************************************************************/
template <class T>
SingleListNode<T>* Stack<T>::PopNode()
{
  return DetachFromHead();
}

/****************************************************************************/
template <class T>
T* Stack<T>::PopItem()
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
T* Stack<T>::PeekItem()
{
  SingleListNode<T>* ListNode_ = Head();
  T* Ptr_ = NULL;

  if (ListNode_)
    Ptr_ = ListNode_->GetItem();
  
  return Ptr_;
}

/****************************************************************************/
template <class T>
SingleList<T>& Stack<T>::Shrink(size_t Num_)
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
void* Stack<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Stack<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* Stack<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Stack<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
StackAcceptor<T>::StackAcceptor(const Stack<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void StackAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Stack<T>*)Ptr_;
}

/****************************************************************************/
template <class T>    
const void* StackAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean StackAcceptor<T>::IsStack() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* StackAcceptor<T>::CreateFromStack(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Stack<T>(*((const Stack<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* StackAcceptor<T>::CreateFromSingleList(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Stack<T>(*((const SingleList<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* StackAcceptor<T>::CreateFromSingleSublist(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Stack<T>(*((const SingleSublist<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* StackAcceptor<T>::AssignFromStack(const ObjectAcceptor& Obj_)
{
  const Stack<T>* TrgPtr_ = (const Stack<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* StackAcceptor<T>::AssignFromSingleList(const ObjectAcceptor& Obj_)
{
  const SingleList<T>* TrgPtr_ = (const SingleList<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>    
void* StackAcceptor<T>::AssignFromSingleSublist(const ObjectAcceptor& Obj_)
{
  const SingleSublist<T>* TrgPtr_ = (const SingleSublist<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean StackAcceptor<T>::IsEqualToStack(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToStack((const Stack<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean StackAcceptor<T>::IsLesserToStack(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToStack((const Stack<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean StackAcceptor<T>::IsGreaterToStack(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToStack((const Stack<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* StackAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>    
void StackAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if STACK_DEBUG
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
  Stack<Char> s1, s2, s3;
  Boolean Valid_;
  size_t i;

  Char* pc4;
  Char* pc5;

  Stack<Char>::SetComparisonFunction(CompChar);
  SingleListNode<Char> sNode1_(c0);

  s3.Push(&sNode1_);
  for (i = 0; i < 3; i++)
    s3.Push(SingleListNode<Char>::Make(c0));
    
  Stack<Char> s4(s3);
  Stack<Char> s5, s6;
  SingleListNode<Char> sNode2_(ca);
  SingleListNode<Char> sNode3_(cb);

  s5.Push(&sNode2_);
  s6.Push(&sNode3_);
  for (i = 0; i < 9; i++)  
    s6.Push(SingleListNode<Char>::Make(cb));

  Stack<Char> s7;
  SingleListNode<Char>sNode4_(c0);
  
  s7.Push(&sNode4_);
  for (i = 1; i < 3; i++)
    s7.Push(SingleListNode<Char>::Make(c0));

  assert(s1.Size() == 0);
  assert(s2.Size() == 0);
  assert(s3.Size() == 4 && s3 == "XXXX");
  assert(s4.Size() == 4 && s4 == "XXXX");
  assert(s5.Size() == 1 && *s5.GetNode(0)->GetItem() == ca);
  assert(s6.Size() == 10 && s6 == "bbbbbbbbbb");
  assert(s7.Size() == 3 && s7 == "XXX");
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  assert(s1 == "XX");
  s1.Push(SingleListNode<Char>::Make(ca));
  s1.Push(SingleListNode<Char>::Make(ca));
  assert(s1 == "aaXX");
  s1.ShrinkTo(3); assert(s1 == "aXX");

  // test assigns
  SingleListNode<Char> sNode6_(cb);
  SingleListNode<Char> sNode7_(cb);
  
  s1.Flush();
  s1 = s5, assert(s1 == "a");
  s1 = s1, assert(s1 == "a");
  s1.AssignListNode(&sNode6_, DataStorageAttributes::DEEP_COPY);
  assert(s1 == "b");
  s1.AssignListNode(&sNode7_, DataStorageAttributes::DEEP_COPY);
  s1.Push(SingleListNode<Char>::Make(cb));
  s1.Push(SingleListNode<Char>::Make(cb));
  s1.Push(SingleListNode<Char>::Make(cb));
  s1.Push(SingleListNode<Char>::Make(cb));  
  assert(s1 == "bbbbb");
  s1.AssignListNode(SingleListNode<Char>::Make(&cb, DataStorageAttributes::ACTIVE),
                    DataStorageAttributes::DESTROY);
  assert(s1 == "b");
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXb");

  // test appends
  s1 = s5;
  s1.Push(Stack<Char>::Make(s5)); assert(s1 == "aa");
  s1.Push(SingleListNode<Char>::Make(cb));
  assert(s1 == "baa");
  s1.Push(SingleListNode<Char>::Make(cb));
  assert(s1 == "bbaa");
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));  
  s1.Push(SingleListNode<Char>::Make(cb));    
  assert(s1 == "bbbbbbbaa");
  s1.Push(SingleListNode<Char>::Make(&ca, DataStorageAttributes::ACTIVE));
  assert(s1 == "abbbbbbbaa");
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  s1.Push(SingleListNode<Char>::Make(c0));
  assert(s1 == "XXXabbbbbbbaa" && s1.Size() == 13);
  
  s4 = s1;

  // Test removes
  SingleListNode<Char>* Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "XXabbbbbbbaa" && s1.Size() == 12);
  delete Node_;
  Char* Item_ = s1.PopItem();
  assert(*Item_ == c0);
  assert(s1 == "Xabbbbbbbaa" && s1.Size() == 11);
  ::Delete(Item_);
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == c0);
  assert(s1 == "abbbbbbbaa" && s1.Size() == 10);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1 == "bbbbbbbaa" && s1.Size() == 9);
  Item_ = s1.PopItem();
  assert(*Item_ == cb);
  ::Delete(Item_);
  Item_ = s1.PopItem();
  assert(*Item_ == cb);
  ::Delete(Item_);  
  assert(s1 == "bbbbbaa" && s1.Size() == 7);
  for (i = 0; i < 5; i++)
  {
    Item_ = s1.PopItem();
    assert(*Item_ == cb);
    ::Delete(Item_);
  }
  assert(s1 == "aa" && s1.Size() == 2);  
  Node_ = s1.PopNode();
  assert(*Node_->GetItem() == ca);
  assert(s1 == "a" && s1.Size() == 1);
  delete Node_;
  Item_ = s1.PopItem();
  assert(*Item_ == ca);
  assert(s1.Size() == 0);
  ::Delete(Item_);
  s1 = s4;

  // Test class hierarchy extensions
  // Test virtual object constructor
  pc5 = &c0;
  Stack<Char>* ps1 = Stack<Char>::Make(s1);
  assert(!(*ps1 == pc5));
  pc5 = ps1->ListToArray();
  assert(*ps1 == pc5);
  ::Delete(pc5);

  // Test virtual assignment
  Stack<Char>* ps2 = Stack<Char>::Make();
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
  s1.Push(&sNode8_);
  for (i = 1, x++; i < 5; i++, x++)
    s1.Push(SingleListNode<Char>::Make(c3.Set(x)));

  x = 65 + 26 - 1;
  SingleListNode<Char> sNode9_(c3.Set(x));
  s2.Push(&sNode9_);
  for (i = 1, x--; i < 5; i++, x--)
    s2.Push(SingleListNode<Char>::Make(c3.Set(x)));

  x = 75;
  SingleListNode<Char> sNode10_(c3.Set(x));
  s3.Push(&sNode10_);
  for (i = 1, x++; i < 5; i++, x++)
    s3.Push(SingleListNode<Char>::Make(c3.Set(x)));

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

#if STACK_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <Stack>" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




