#ifndef SET_CPP
#define SET_CPP
#ifndef SET_H
  #include "set.h"
#endif

/****************************************************************************/
template <class T>
int(*Set<T>::_EqualFunc)(const T&, const T&) = NULL;

/****************************************************************************/
template <class T>
Set<T>::Set()
{}

/****************************************************************************/
template <class T>
Set<T>::Set(const Set<T>& Data_):
_SetData(Data_._SetData)
{}

/****************************************************************************/
template <class T>
Set<T>::Set(T* Ptr_)
{
  if (Ptr_)
    _State.AppendHead(new T(*Ptr_));
}

/****************************************************************************/
template <class T>
Set<T>* Set<T>::Make()
{
  return (new Set<T>());
}

/****************************************************************************/
template <class T>
Set<T>* Set<T>::Make(const Set<T>& Data_)
{
  return (new Set<T>(Data_));
}

/****************************************************************************/
template <class T>
Set<T>* Set<T>::Make(T* Ptr_)
{
  return (new Set<T>(Ptr_));
}

/****************************************************************************/
template <class T>
void Set<T>::SetComparisonFunction(int(*EqFnc_)(const T&, const T&))
{
  _EqualFunc = EqFnc_;
}

/****************************************************************************/
template <class T>
Set<T>& Set<T>::operator = (const Set<T>& Data_)
{
  if (this != &Data_)  
    _SetData = Data_._SetData;

  return *this;
}

/****************************************************************************/
template <class T>
Set<T>& Set<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Set<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& Set<T>::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& Set<T>::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
int Set<T>::ClassID() const
{
  return TypeInfo::SET_TYPE;
}

/****************************************************************************/
template <class T>
Object* Set<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Set<T>(*this));

  return NULL;
}

/****************************************************************************/
template <class T>
Set<T>* Set<T>::CloneSet() const
{
  return (new Set<T>(*this));
}

/****************************************************************************/
template <class T>
Set<T> Set<T>::operator | (const Set<T>& Data_) const
{
  Set<T> NewSet_;
  SimpleListIterator<T> ThatIter_(&NewSet_._SetData);

  if (_SetData.Size() || Data_._SetData.Size())
  {
    Boolean Matched_;
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    SimpleListIterator<T> Iter1_((SearchableList<T>*)&_SetData);
    ConstListIterator<T> Iter2_((SearchableList<T>*)&Data_._SetData);

    if (Max1_)
      NewSet_ |= new T(*((SearchableList<T>*)&_SetData)->Head()->_Object);
    else if (Max2_)
      NewSet_ |= new T(*((SearchableList<T>*)&Data_._SetData)->Head()->_Object);
    
    for (x = 0; x < Max1_; x++)
    {
      const T* ThisItem_ = Iter1_.IndexNode(x)->_Object;
      T* ThatItem_ = NULL;
      Matched_ = FALSE;
      
      for (y = 0; y < NewSet_._SetData.Size(); y++)
      {
        ThatItem_ = ThatIter_.IndexNode(y)->_Object;
      
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          Matched_ = TRUE;
          break;
        }
      }

      if (!Matched_)
        NewSet_ |= (new T(*ThisItem_));
    }

    for (x = 0; x < Max2_; x++)
    {
      const T* ThisItem_ = Iter2_.IndexNode(x)->_Object;
      T* ThatItem_ = NULL;      
      Matched_ = FALSE;
      
      for (y = 0; y < NewSet_._SetData.Size(); y++)
      {
        ThatItem_ = ThatIter_.IndexNode(y)->_Object;      
        
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          Matched_ = TRUE;
          break;
        }
      }

      if (!Matched_)
        NewSet_ |= (new T(*ThisItem_));      
    }
  }

  return NewSet_;
}

/****************************************************************************/
template <class T>
Set<T> Set<T>::operator & (const Set<T>& Data_) const
{
  Set<T> NewSet_;

  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    SimpleListIterator<T> Iter1_((SearchableList<T>*)&_SetData);
    ConstListIterator<T> Iter2_((SearchableList<T>*)&Data_._SetData);
    
    for (x = 0; x < Max1_; x++)
      for (y = 0; y < Max2_; y++)
      {
        const T* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const T* ThatItem_ = Iter2_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          NewSet_ |= (new T(*ThisItem_));
          break;
        }
      }
  }

  return NewSet_;
}

/****************************************************************************/
template <class T>
Set<T> Set<T>::operator - (const Set<T>& Data_) const
{
  Set<T> NewSet_(*this);
  NewSet_ -= Data_;
  return NewSet_;  
}

/****************************************************************************/
template <class T>
Set<T>& Set<T>::operator |= (const Set<T>& Data_)
{
  if (_SetData.Size() || Data_._SetData.Size())
  {
    Boolean Matched_;  
    size_t x, y;
    size_t Max2_ = Data_._SetData.Size();
    SimpleListIterator<T> Iter1_(&_SetData);
    ConstListIterator<T> Iter2_(&Data_._SetData);

    for (x = 0; x < Max2_; x++)
    {
      const T* ThisItem_ = Iter2_.IndexNode(x)->_Object;    
      Matched_ = FALSE;
      
      for (y = 0; y < _SetData.Size(); y++)
      {
        const T* ThatItem_ = Iter1_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          Matched_ = TRUE;
          break;
        }
      }

      if (!Matched_)
        *this |= (new T(*ThisItem_));
    }
  }

  return *this;
}

/****************************************************************************/
template <class T>
Set<T>& Set<T>::operator &= (const Set<T>& Data_)
{
  *this = (*this & Data_);
  return *this;
}

/****************************************************************************/
template <class T>
Set<T>& Set<T>::operator -= (const Set<T>& Data_)
{
  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max2_ = Data_._SetData.Size();
    SimpleListIterator<T> Iter1_(&_SetData);
    ConstListIterator<T> Iter2_(&Data_._SetData);
    
    for (x = 0; x < _SetData.Size(); x++)
      for (y = 0; y < Max2_; y++)
      {
        const T* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const T* ThatItem_ = Iter2_.IndexNode(y)->_Object;

        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          SimpleListIterator<T> Iter_(&_SetData);
          Iter_.IndexNode(x);
          Iter_.Remove();
          x--;
          break;        
        }
      }
  }

  return *this;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator > (const Set<T>& Data_) const
{
  Boolean Result_ = FALSE;
  Set<T> NewSet_(*this);

  if (NewSet_._SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = NewSet_._SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    SimpleListIterator<T> Iter1_(&NewSet_._SetData);
    ConstListIterator<T> Iter2_(&Data_._SetData);
    size_t* XMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max1_);
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;    
    
    for (x = 0; x < Max1_; x++)
    {
      XMatches_[x] = 0;
      for (y = 0; y < Max2_; y++)
      {
        const T* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const T* ThatItem_ = Iter2_.IndexNode(y)->_Object;      
        
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          XMatches_[x] = YMatches_[y] = 1;
          break;
        }
      }
    }

    Result_ = TRUE;
    for (y = 0; y < Max2_; y++)
      if (!YMatches_[y])
      {
        Result_ = FALSE;
        break;
      }

    if (Result_)
    {
      Result_ = FALSE;
      for (x = 0; x < Max1_; x++)
        if (!XMatches_[x])
        {
          Result_ = TRUE;
          break;
        }
    }

    ::Delete(XMatches_);
    ::Delete(YMatches_);
  }

  return Result_;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator >= (const Set<T>& Data_) const
{
  Boolean Result_ = FALSE;
  Set<T> NewSet_(*this);

  if (NewSet_._SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = NewSet_._SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    SimpleListIterator<T> Iter1_(&NewSet_._SetData);
    ConstListIterator<T> Iter2_(&Data_._SetData);
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;    
    
    for (x = 0; x < Max1_; x++)
      for (y = 0; y < Max2_; y++)
      {
        const T* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const T* ThatItem_ = Iter2_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          YMatches_[y] = 1;
          break;
        }
      }

    Result_ = TRUE;
    for (y = 0; y < Max2_; y++)
      if (!YMatches_[y])
      {
        Result_ = FALSE;
        break;
      }

    ::Delete(YMatches_);
  }

  return Result_;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator < (const Set<T>& Data_) const
{
  return (Data_ > *this);
}

/****************************************************************************/
template <class T>    
Boolean Set<T>::operator <= (const Set<T>& Data_) const
{
  return (Data_ >= *this);
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator == (const Set<T>& Data_) const
{
  Boolean Result_ = FALSE;

  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    ConstListIterator<T> Iter1_(&_SetData);
    ConstListIterator<T> Iter2_(&Data_._SetData);
    size_t* XMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max1_);    
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;
    
    for (x = 0; x < Max1_; x++)
    {
      XMatches_[x] = 0;
      for (y = 0; y < Max2_; y++)
      {
        const T* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const T* ThatItem_ = Iter2_.IndexNode(y)->_Object;
            
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          XMatches_[x] = YMatches_[y] = 1;
          break;
        }
      }
    }

    Result_ = TRUE;
    for (y = 0; y < Max2_; y++)
      if (!YMatches_[y])
      {
        Result_ = FALSE;
        break;
      }

    if (Result_)
      for (x = 0; x < Max1_; x++)
        if (!XMatches_[x])
        {
          Result_ = FALSE;
          break;
        }
      
    ::Delete(XMatches_);
    ::Delete(YMatches_);    
  }

  return Result_;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::Disjoint(const Set<T>& Data_) const
{
  Boolean Result_ = FALSE;

  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    SimpleListIterator<T> Iter1_(&_SetData);
    ConstListIterator<T> Iter2_(&Data_._SetData);
    size_t* XMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max1_);    
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;
    
    for (x = 0; x < Max1_; x++)
    {
      XMatches_[x] = 0;
      for (y = 0; y < Max2_; y++)
      {
        const T* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const T* ThatItem_ = Iter2_.IndexNode(y)->_Object;      
      
        if (ThisItem_ && ThatItem_ && (*_EqualFunc)(*ThisItem_, *ThatItem_) == 0)
        {
          XMatches_[x] = YMatches_[y] = 1;
          break;
        }
      }
    }

    Result_ = TRUE;
    for (y = 0; y < Max2_; y++)
      if (YMatches_[y])
      {
        Result_ = FALSE;
        break;
      }

    if (Result_)
      for (x = 0; x < Max1_; x++)
        if (XMatches_[x])
        {
          Result_ = FALSE;
          break;
        }
      
    ::Delete(XMatches_);
    ::Delete(YMatches_);    
  }

  return Result_;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator != (const Set<T>& Data_) const
{
  return !(*this == Data_);
}

/****************************************************************************/
template <class T>
Set<T> Set<T>::operator | (T* Ptr_) const
{
  Set<T> NewSet_(*this);
  NewSet_ |= Ptr_;
  return NewSet_;
}

/****************************************************************************/
template <class T>
Set<T> Set<T>::operator - (T* Ptr_) const
{
  Set<T> NewSet_(*this);
  NewSet_ -= Ptr_;
  return NewSet_;
}

/****************************************************************************/
template <class T>
Set<T>& Set<T>::operator |= (T* Ptr_)
{
  if (Ptr_)
    _SetData.AppendHead(new T(*Ptr_));

  return *this;
}

/****************************************************************************/
template <class T>
Set<T>& Set<T>::operator -= (T* Ptr_)
{
  if (Ptr_)
  {
    SimpleNode<T>* NodePtr_ = _SetData.Find(Ptr_);
    if (NodePtr_)
      _SetData.Remove(NodePtr_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator > (const T* Ptr_) const
{
  if (Ptr_)
  {
    const SimpleNode<T>* NodePtr_;
    
    if (NodePtr_ = _SetData.Find(Ptr_))
      if (_SetData.NextMatch(NodePtr_))
        return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator >= (const T* Ptr_) const
{
  if (Ptr_)
    if (_SetData.Find(Ptr_))
      return TRUE;    

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean Set<T>::operator == (const T* Ptr_) const
{
  if (Ptr_)
  {
    Boolean Result_ = FALSE;
    const SimpleNode<T>* NodePtr_;
    
    if (NodePtr_ = _SetData.Find(Ptr_))
      if (_SetData.NextMatch(NodePtr_))
        Result_ = FALSE;
      else
        Result_ = TRUE;

    return Result_;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
SearchableList<T>& Set<T>::GiveSetData()
{
  return _SetData;
}

/****************************************************************************/
template <class T>
const SearchableList<T>& Set<T>::GiveSetData() const
{
  return _SetData;
}

/****************************************************************************/
template <class T>
void Set<T>::Flush()
{
  _SetData.DeleteAll();
}

/****************************************************************************/
template <class T>
size_t Set<T>::Size() const
{
  return _SetData.Size();
}

/****************************************************************************/
template <class T>
const T* Set<T>::FindItem(const T& Obj_) const
{
  T* Ptr_ = (T*)&Obj_;
  Set<T>* const LocalThis_ = (Set<T>* const)this;
  return SimpleList<T>::Deref(LocalThis_->_SetData.Find(Ptr_));
}

/****************************************************************************/
template <class T>
T* Set<T>::FindItem(const T& Obj_)
{
  T* Ptr_ = (T*)&Obj_;
  return SimpleList<T>::Deref(_SetData.Find(Ptr_));
}

/****************************************************************************/
template <class T>
Boolean Set<T>::IsEmpty() const
{
  return _SetData.Empty();
}

/****************************************************************************/
template <class T>
Boolean Set<T>::IsNewed() const
{
  return ObjectImp::IsNewed();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* Set<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Set<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* Set<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Set<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if SET_DEBUG
class Char
{
  DEFAULT_ROOTOBJECT_DEFN(Char)

  friend Boolean operator == (const Char& obj1, const Char& obj2)
      { return (obj1.val() == obj2.val()); }
  friend Boolean operator != (const Char& obj1, const Char& obj2)
      { return (obj1.val() != obj2.val()); }
  friend Boolean operator > (const Char& obj1, const Char& obj2)
      { return (obj1.val() > obj2.val()); }
  friend Boolean operator < (const Char& obj1, const Char& obj2)
      { return (obj1.val() < obj2.val()); }
  friend Boolean operator >= (const Char& obj1, const Char& obj2)
      { return (obj1.val() >= obj2.val()); }
  friend Boolean operator <= (const Char& obj1, const Char& obj2)
      { return (obj1.val() <= obj2.val()); }

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

int operator == (SimpleList<Char>& x, const char* s)
{
  SimpleListIterator<Char> Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
    if (Iter_.IndexNode(i)->_Object->val() != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

int operator == (SimpleList<Char>& x, const Char* s)
{
  SimpleListIterator<Char> Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
    if (Iter_.IndexNode(i)->_Object->val() != s[i].val())
      return 0;

  return 1;
}

void ShowTheList(SimpleList<Char>& x)
{
  SimpleListIterator<Char> Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
  {
    if (Iter_.IndexNode(i) && Iter_.IndexNode(i)->_Object)
      cout <<"[" <<i <<"] = "
           <<Iter_.IndexNode(i)->_Object->val() <<", ";

    cout <<"\r\n";
  }

  cout <<endl;
}

int CompChar(const Char& c1, const Char& c2)
{
  return (c1.val() - c2.val());
}        

int main()
{
  Char c0('0');
  Char c1('1');
  Char c2('2');
  Char c3('3');
  Char c4('4');
  Char c5('5');
  Char c6('6');
  Char c7('7');
  Char c8('8');
  Char c9('9');
  Char ca('A');
  Char cb('B');
  Char cc('C');
  Char cd('D');
  Char ce('E');
  Char cf('F');

  Set<Char>::SetComparisonFunction(CompChar);

  Set<Char> s1;
  Set<Char> s2;
  Set<Char> s3;
  Set<Char> s4;
  Set<Char> s5;
  Set<Char> s6;
  Set<Char> s7;
  Set<Char> s8;  
  
  s1 |= &c0;
  s1 |= &c1;
  s1 |= &c2;
  s1 |= &c3;
  s1 |= &c4;
  s1 |= &c5;
  s1 |= &c6;
  s1 |= &c7;
  s1 |= &c8;
  s1 |= &c9;

  s2 |= &c0;
  s2 |= &c1;
  s2 |= &c2;
  s2 |= &c3;
  s2 |= &c4;
  s2 |= &c5;

  s3 = s2;

  s4 |= &c6;
  s4 |= &c7;
  s4 |= &c8;
  s4 |= &c9;
  s4 |= &ca;
  s4 |= &cb;
  s4 |= &cc;
  s4 |= &cd;

  s5 = s4;
  s5 |= &ca;
  s5 |= &cb;

  s6 = s1 & s4;
  s7 = s1 | s4;
  s8 = s1 - s4;

  assert(s1 > s2);
  assert(s1 >= s2);
  assert(s3 >= s2);
  assert(s3 == s2);
  assert(s1 != s2);

  ShowTheList(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowTheList(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD0123456789");
  ShowTheList(s8.GiveSetData());
  assert(s8.GiveSetData() == "543210");

  size_t Index_;
  Char* CharPtr_;
  CharPtr_ = s7.FindItem(c0);
  assert(CharPtr_ != NULL && CharPtr_->val() == '0');
  CharPtr_ = s7.FindItem(c4);
  assert(CharPtr_ != NULL && CharPtr_->val() == '4');
  CharPtr_ = s7.FindItem(c9);
  assert(CharPtr_ != NULL && CharPtr_->val() == '9');
  CharPtr_ = s7.FindItem(cd);
  assert(CharPtr_ != NULL && CharPtr_->val() == 'D');
  CharPtr_ = s7.FindItem(Char('x'));
  assert(CharPtr_ == NULL);

  s8 = s7 = s6 = s1;
  s6 &= s4;
  s7 |= s4;
  s8 -= s4;

  ShowTheList(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowTheList(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD9876543210");
  ShowTheList(s8.GiveSetData());
  assert(s8.GiveSetData() == "543210");

  assert(s5 > &ca);
  assert(s5 >= &ca);
  assert(s5 > &cb);
  assert(s5 >= &cb);
  assert(!(s5 == &ca));
  assert(!(s5 == &cb));

  assert(!(s5 > s4));
  assert(s5 >= s4);
  assert(s5 == s4);

  s5 -= &ca;
  s5 -= &cb;
  s5 |= &c4;
  s5 |= &c5;

  assert(!(s5 > &ca));
  assert(s5 >= &ca);
  assert(!(s5 > &cb));
  assert(s5 >= &cb);
  assert(s5 == &ca);
  assert(s5 == &cb);

  assert(s5 > s4);
  assert(s5 >= s4);
  assert(!(s5 == s4));

#if SET_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <Set>" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




