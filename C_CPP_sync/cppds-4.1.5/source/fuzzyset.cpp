#ifndef FUZZYSET_CPP
#define FUZZYSET_CPP
#ifndef FUZZYSET_H
  #include "fuzzyset.h"
#endif

/****************************************************************************/
template <class T>
int(*FuzzySet<T>::_EqualFunc)(const T&, const T&) = NULL;

/****************************************************************************/
template <class KEY, class VALUE>
FuzzyNode<KEY,VALUE>& FuzzyNode<KEY,VALUE>::operator = (const FuzzyNode<KEY,VALUE>& Obj_)
{
  if (this != &Obj_)
  {
    _Key = Obj_._Key;
    _Value = Obj_._Value;
  }
  
  return *this;
}

/****************************************************************************/
template <class T>
FuzzySet<T>::FuzzySet()
{}

/****************************************************************************/
template <class T>
FuzzySet<T>::FuzzySet(const FuzzySet<T>& Data_):
_SetData(Data_._SetData)
{}

/****************************************************************************/
template <class T>
FuzzySet<T>::FuzzySet(FuzzyNode<double,T>* Ptr_)
{
  if (Ptr_)
    _State.AppendHead(new FuzzyNode<double,T>(*Ptr_));
}

/****************************************************************************/
template <class T>
FuzzySet<T>* FuzzySet<T>::Make()
{
  return (new FuzzySet<T>());
}

/****************************************************************************/
template <class T>
FuzzySet<T>* FuzzySet<T>::Make(const FuzzySet<T>& Data_)
{
  return (new FuzzySet<T>(Data_));
}

/****************************************************************************/
template <class T>
FuzzySet<T>* FuzzySet<T>::Make(FuzzyNode<double,T>* Ptr_)
{
  return (new FuzzySet<T>(Ptr_));
}

/****************************************************************************/
template <class T>
const FuzzyNode<double,T>* FuzzySet<T>::FindItem(const FuzzyNode<double,T>& Obj_) const
{
  FuzzyNode<double,T>* Ptr_ = (FuzzyNode<double,T>*)&Obj_;
  FuzzySet<T>* const LocalThis_ = (FuzzySet<T>* const)this;
  return FuzzySetTypeDefn<T>::NodeStore::Deref(LocalThis_->_SetData.Find(Ptr_));
}

/****************************************************************************/
template <class T>
FuzzyNode<double,T>* FuzzySet<T>::FindItem(const FuzzyNode<double,T>& Obj_)
{
  FuzzyNode<double,T>* Ptr_ = (FuzzyNode<double,T>*)&Obj_;
  return FuzzySetTypeDefn<T>::NodeStore::Deref(_SetData.Find(Ptr_));
}

/****************************************************************************/
template <class T>
Boolean FuzzySet<T>::IsEmpty() const
{
  return _SetData.Empty();
}

/****************************************************************************/
template <class T>
Boolean FuzzySet<T>::IsNewed() const
{
  return ObjectImp::IsNewed();
}

/****************************************************************************/
template <class T>
void FuzzySet<T>::Flush()
{
  _SetData.DeleteAll();
}

/****************************************************************************/
template <class T>
size_t FuzzySet<T>::Size() const
{
  return _SetData.Size();
}

/****************************************************************************/
template <class T>
void FuzzySet<T>::SetComparisonFunction(int(*EqFnc_)(const T&, const T&))
{
  _EqualFunc = EqFnc_;
}

/****************************************************************************/
template <class T>
TYPENAME FuzzySetTypeDefn<T>::NodeStore& FuzzySet<T>::GiveSetData()
{
  return _SetData;
}

/****************************************************************************/
template <class T>
const TYPENAME FuzzySetTypeDefn<T>::NodeStore& FuzzySet<T>::GiveSetData() const
{
  return _SetData;
}

/****************************************************************************/
template <class T>
FuzzySet<T>& FuzzySet<T>::operator = (const FuzzySet<T>& Data_)
{
  if (this != &Data_)
    _SetData = Data_._SetData;  

  return *this;
}

/****************************************************************************/
template <class T>
FuzzySet<T>& FuzzySet<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static FuzzySet<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& FuzzySet<T>::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& FuzzySet<T>::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>
int FuzzySet<T>::ClassID() const
{
  return TypeInfo::FUZZYSET_TYPE;
}

/****************************************************************************/
template <class T>
Object* FuzzySet<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new FuzzySet<T>(*this));

  return NULL;
}

/****************************************************************************/
template <class T>
FuzzySet<T>* FuzzySet<T>::CloneSet() const
{
  return (new FuzzySet<T>(*this));
}

/****************************************************************************/
template <class T>
FuzzySet<T> FuzzySet<T>::operator | (const FuzzySet<T>& Data_) const
{
  FuzzySet<T> NewSet_;
  TYPENAME FuzzySetTypeDefn<T>::NodeIter ThatIter_(&NewSet_._SetData);

  if (_SetData.Size() || Data_._SetData.Size())
  {  
    Boolean Matched_;
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter1_((TYPENAME FuzzySetTypeDefn<T>::NodeStore*)&_SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_((TYPENAME FuzzySetTypeDefn<T>::NodeStore*)&Data_._SetData);

    if (Max1_)
      NewSet_ |= new FuzzyNode<double,T>(*((TYPENAME FuzzySetTypeDefn<T>::NodeStore*)&_SetData)->Head()->_Object);
    else if (Max2_)
      NewSet_ |= new FuzzyNode<double,T>(*((TYPENAME FuzzySetTypeDefn<T>::NodeStore*)&Data_._SetData)->Head()->_Object);
    
    for (x = 0; x < Max1_; x++)
    {
      const FuzzyNode<double,T>* ThisItem_ = Iter1_.IndexNode(x)->_Object;
      FuzzyNode<double,T>* ThatItem_ = NULL;
      Matched_ = FALSE;
      
      for (y = 0; y < NewSet_._SetData.Size(); y++)
      {
        ThatItem_ = ThatIter_.IndexNode(y)->_Object;
      
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
        {
          Matched_ = TRUE;
          break;
        }
      }

      if (!Matched_)
        NewSet_ |= (new FuzzyNode<double,T>(*ThisItem_));
      else if (ThisItem_ && ThatItem_ && ThisItem_->_Key > ThatItem_->_Key)
        ThatItem_->_Key = ThisItem_->_Key;
    }

    for (x = 0; x < Max2_; x++)
    {
      const FuzzyNode<double,T>* ThisItem_ = Iter2_.IndexNode(x)->_Object;
      FuzzyNode<double,T>* ThatItem_ = NULL;
      Matched_ = FALSE;
      
      for (y = 0; y < NewSet_._SetData.Size(); y++)
      {
        ThatItem_ = ThatIter_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
        {
          Matched_ = TRUE;
          break;
        }
      }

      if (!Matched_)
        NewSet_ |= (new FuzzyNode<double,T>(*ThisItem_));
      else if (ThisItem_ && ThatItem_ && ThisItem_->_Key > ThatItem_->_Key)
        ThatItem_->_Key = ThisItem_->_Key;      
    }
  }

  return NewSet_;
}

/****************************************************************************/
template <class T>
FuzzySet<T> FuzzySet<T>::operator & (const FuzzySet<T>& Data_) const
{
  FuzzySet<T> NewSet_;

  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter1_((TYPENAME FuzzySetTypeDefn<T>::NodeStore*)&_SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_((TYPENAME FuzzySetTypeDefn<T>::NodeStore*)&Data_._SetData);
    
    for (x = 0; x < Max1_; x++)
      for (y = 0; y < Max2_; y++)
      {
        const FuzzyNode<double,T>* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const FuzzyNode<double,T>* ThatItem_ = Iter2_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
        {
          if (ThisItem_->_Key < ThatItem_->_Key)
            NewSet_ |= (new FuzzyNode<double,T>(*ThisItem_));
          else
            NewSet_ |= (new FuzzyNode<double,T>(*ThatItem_));

          break;
        }
      }
  }

  return NewSet_;
}

/****************************************************************************/
template <class T>
FuzzySet<T> FuzzySet<T>::operator - (const FuzzySet<T>& Data_) const
{
  FuzzySet<T> NewSet_(*this);
  NewSet_ -= Data_;
  return NewSet_;  
}

/****************************************************************************/
template <class T>
FuzzySet<T>& FuzzySet<T>::operator |= (const FuzzySet<T>& Data_)
{
  if (_SetData.Size() || Data_._SetData.Size())
  {
    Boolean Matched_;  
    size_t x, y;
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter1_(&_SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_(&Data_._SetData);

    for (x = 0; x < Max2_; x++)
    {
      const FuzzyNode<double,T>* ThisItem_ = Iter2_.IndexNode(x)->_Object;
      FuzzyNode<double,T>* ThatItem_ = NULL;
      Matched_ = FALSE;
      
      for (y = 0; y < _SetData.Size(); y++)
      {
        ThatItem_ = Iter1_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
        {
          Matched_ = TRUE;
          break;
        }
      }

      if (!Matched_)      
        *this |= (new FuzzyNode<double,T>(*ThisItem_));
      else if (ThisItem_ && ThatItem_ && ThisItem_->_Key > ThatItem_->_Key)
        ThatItem_->_Key = ThisItem_->_Key;
    }
  }

  return *this;
}

/****************************************************************************/
template <class T>
FuzzySet<T>& FuzzySet<T>::operator &= (const FuzzySet<T>& Data_)
{
  *this = (*this & Data_);
  return *this;
}

/****************************************************************************/
template <class T>
FuzzySet<T>& FuzzySet<T>::operator -= (const FuzzySet<T>& Data_)
{
  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter1_(&_SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_(&Data_._SetData);
    
    for (x = 0; x < _SetData.Size(); x++)
      for (y = 0; y < Max2_; y++)
      {
        FuzzyNode<double,T>* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const FuzzyNode<double,T>* ThatItem_ = Iter2_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
          if (ThisItem_->_Key > ThatItem_->_Key)
            ThisItem_->_Key -= ThatItem_->_Key;
          else
          {
            TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter_(&_SetData);
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
Boolean FuzzySet<T>::operator > (const FuzzySet<T>& Data_) const
{
  Boolean Result_ = FALSE;
  FuzzySet<T> NewSet_(*this);

  if (NewSet_._SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = NewSet_._SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter1_(&NewSet_._SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_(&Data_._SetData);
    size_t* XMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max1_);
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;    
    
    for (x = 0; x < Max1_; x++)
    {
      XMatches_[x] = 0;
      for (y = 0; y < Max2_; y++)
      {
        const FuzzyNode<double,T>* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const FuzzyNode<double,T>* ThatItem_ = Iter2_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
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
Boolean FuzzySet<T>::operator >= (const FuzzySet<T>& Data_) const
{
  Boolean Result_ = FALSE;
  FuzzySet<T> NewSet_(*this);

  if (NewSet_._SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = NewSet_._SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter1_(&NewSet_._SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_(&Data_._SetData);
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;
    
    for (x = 0; x < Max1_; x++)
      for (y = 0; y < Max2_; y++)
      {
        const FuzzyNode<double,T>* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const FuzzyNode<double,T>* ThatItem_ = Iter2_.IndexNode(y)->_Object;
        
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
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
Boolean FuzzySet<T>::operator < (const FuzzySet<T>& Data_) const
{
  return (Data_ > *this);
}

/****************************************************************************/
template <class T>    
Boolean FuzzySet<T>::operator <= (const FuzzySet<T>& Data_) const
{
  return (Data_ >= *this);
}

/****************************************************************************/
template <class T>
Boolean FuzzySet<T>::operator == (const FuzzySet<T>& Data_) const
{
  Boolean Result_ = FALSE;

  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter1_(&_SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_(&Data_._SetData);
    size_t* XMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max1_);    
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;
    
    for (x = 0; x < Max1_; x++)
    {
      XMatches_[x] = 0;
      for (y = 0; y < Max2_; y++)
      {
        const FuzzyNode<double,T>* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const FuzzyNode<double,T>* ThatItem_ = Iter2_.IndexNode(y)->_Object;
      
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
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
Boolean FuzzySet<T>::Disjoint(const FuzzySet<T>& Data_) const
{
  Boolean Result_ = FALSE;

  if (_SetData.Size() && Data_._SetData.Size())
  {
    size_t x, y;
    size_t Max1_ = _SetData.Size();
    size_t Max2_ = Data_._SetData.Size();
    TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter1_(&_SetData);
    TYPENAME FuzzySetTypeDefn<T>::ConstIter Iter2_(&Data_._SetData);
    size_t* XMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max1_);    
    size_t* YMatches_ = (size_t*)MemMatrix::Matrix().Allocate(sizeof(size_t) * Max2_);

    for (y = 0; y < Max2_; y++)
      YMatches_[y] = 0;
    
    for (x = 0; x < Max1_; x++)
    {
      XMatches_[x] = 0;
      for (y = 0; y < Max2_; y++)
      {
        const FuzzyNode<double,T>* ThisItem_ = Iter1_.IndexNode(x)->_Object;
        const FuzzyNode<double,T>* ThatItem_ = Iter2_.IndexNode(y)->_Object;
      
        if (ThisItem_ && ThatItem_ &&
            (*_EqualFunc)(ThisItem_->_Value, ThatItem_->_Value) == 0)
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
Boolean FuzzySet<T>::operator != (const FuzzySet<T>& Data_) const
{
  return !(*this == Data_);
}

/****************************************************************************/
template <class T>
FuzzySet<T> FuzzySet<T>::operator | (FuzzyNode<double,T>* Ptr_) const
{
  FuzzySet<T> NewSet_(*this);
  NewSet_ |= Ptr_;
  return NewSet_;
}

/****************************************************************************/
template <class T>
FuzzySet<T> FuzzySet<T>::operator - (FuzzyNode<double,T>* Ptr_) const
{
  FuzzySet<T> NewSet_(*this);
  NewSet_ -= Ptr_;
  return NewSet_;
}

/****************************************************************************/
template <class T>
FuzzySet<T>& FuzzySet<T>::operator |= (FuzzyNode<double,T>* Ptr_)
{
  if (Ptr_ && (Ptr_->_Key < 0.0F || Ptr_->_Key > 1.0F))
    XFuzzyRange();
    
  if (Ptr_)
    _SetData.AppendHead(new FuzzyNode<double,T>(*Ptr_));

  return *this;
}

/****************************************************************************/
template <class T>
FuzzySet<T>& FuzzySet<T>::operator -= (FuzzyNode<double,T>* Ptr_)
{
  if (Ptr_ && (Ptr_->_Key < 0.0F || Ptr_->_Key > 1.0F))
    XFuzzyRange();

  if (Ptr_)
  {
    TYPENAME FuzzySetTypeDefn<T>::NodeListNode* NodePtr_ = _SetData.Find(Ptr_);
    if (NodePtr_)
      _SetData.Remove(NodePtr_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
Boolean FuzzySet<T>::operator > (const FuzzyNode<double,T>* Ptr_) const
{
  if (Ptr_)
  {
    const TYPENAME FuzzySetTypeDefn<T>::NodeListNode* NodePtr_;
    
    if (NodePtr_ = _SetData.Find(Ptr_))
      if (_SetData.NextMatch(NodePtr_))
        return TRUE;      
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean FuzzySet<T>::operator >= (const FuzzyNode<double,T>* Ptr_) const
{
  if (Ptr_)
    if (_SetData.Find(Ptr_))
      return TRUE;

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean FuzzySet<T>::operator == (const FuzzyNode<double,T>* Ptr_) const
{
  if (Ptr_)
  {
    Boolean Result_ = FALSE;
    const TYPENAME FuzzySetTypeDefn<T>::NodeListNode* NodePtr_;
    
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
FuzzySet<T> FuzzySet<T>::Complement() const
{
  TYPENAME FuzzySetTypeDefn<T>::NodeIter Iter_((TYPENAME FuzzySetTypeDefn<T>::NodeStore*)&_SetData);
  FuzzySet<T> NewSet_;
  size_t x, Max_;
    
  for (x = 0, Max_ = _SetData.Size(); x < Max_; x++)
  {  
    const FuzzyNode<double,T>* ThisItem_ = Iter_.IndexNode(x)->_Object;
    FuzzyNode<double,T> TargetNode_(1.0F - ThisItem_->_Key, ThisItem_->_Value);
    NewSet_ |= &TargetNode_;
  }

  return NewSet_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* FuzzySet<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void FuzzySet<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* FuzzySet<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void FuzzySet<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if FUZZYSET_DEBUG

int operator == (FuzzySetTypeDefn<char>::NodeStore& x, const char* s)
{
  FuzzySetTypeDefn<char>::NodeIter Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
    if (Iter_.IndexNode(i)->_Object->_Value != s[i])
      return 0;

  return (s[x.Size()] == '\0');
}

void ShowFuzzySet(FuzzySetTypeDefn<char>::NodeStore& x)
{
  FuzzySetTypeDefn<char>::NodeIter Iter_(&x);

  for (int i = 0; i < x.Size(); ++i)
  {
    if (Iter_.IndexNode(i) && Iter_.IndexNode(i)->_Object)
    {
      cout <<"[" <<i <<"] = "
           <<"(" <<Iter_.IndexNode(i)->_Object->_Value
           <<"," <<Iter_.IndexNode(i)->_Object->_Key
           <<"), ";
    }
    cout <<"\r\n";
  }

  cout <<endl;
}

int CompChar(const char& c1, const char& c2)
{
  return (c1 - c2);
}

void TestFuzzySets(ostream& os)
{
  os <<"Fuzzy set testing\r\n"
     <<"-----------------\r\n";

  const char* iname     = "abcd";
  const float amemb[4]  = { 0.9F, 0.4F, 0.5F, 0.0F };
  const float bmemb[4]  = { 0.7F, 0.6F, 0.3F, 0.8F };

  const size_t n1       = 6;
  const char* name1     = "ABCDEF";
  const float memb1[6]  = { 0.1F, 0.6F, 0.3F, 0.4F, 0.9F, 0.1F };
  const size_t n2       = 4;
  const char* name2     = "AEGH";
  const float memb2[4]  = { 0.7F, 0.5F, 0.6F, 0.4F };

  FuzzySet<char> A;
  FuzzySet<char> B;
  FuzzySet<char> C;
  FuzzySet<char> D;
  FuzzyNode<double,char> Node_;

  size_t n;
  for (n = 0; n < 4; ++n)
  {
    Node_ = FuzzyNode<double,char>(amemb[n], iname[n]);
    A |= &Node_;
    Node_ = FuzzyNode<double,char>(bmemb[n], iname[n]);
    B |= &Node_;
  }

  os <<"\r\nset A =\r\n";
  ShowFuzzySet(A.GiveSetData());
  os <<"\r\nset B =\r\n";
  ShowFuzzySet(B.GiveSetData());

  C = A | B;
  D = C.Complement();

  os <<"\r\nset c = union(A,B)\r\n";
  ShowFuzzySet(C.GiveSetData());

  os <<"\r\nset d = complement(C)\r\n";
  ShowFuzzySet(D.GiveSetData());

  FuzzySet<char> fs1;
  FuzzySet<char> fs2;
  FuzzySet<char> fs3;
  FuzzySet<char> fs4;
  FuzzySet<char> fs5;

  for (n = 0; n < n1; ++n)
  {
    Node_ = FuzzyNode<double,char>(memb1[n], name1[n]);
    fs1 |= &Node_;
  }

  for (n = 0; n < n2; ++n)
  {
    Node_ = FuzzyNode<double,char>(memb2[n], name2[n]);
    fs2 |= &Node_;
  }  

  fs3 = fs1 | fs2;
  fs4 = fs1 & fs2;
  fs5 = fs3.Complement();

  os <<"\r\nset fs1 =\r\n";
  ShowFuzzySet(fs1.GiveSetData());

  os <<"\r\nset fs2 =\r\n";
  ShowFuzzySet(fs2.GiveSetData());

  os <<"\r\nset fs3 = union(fs1,fs2)\r\n";
  ShowFuzzySet(fs3.GiveSetData());

  os <<"\r\nset fs4 = intersection(fs1,fs2)\r\n";
  ShowFuzzySet(fs4.GiveSetData());

  os <<"\r\nset fs5 = complement(fs3)\r\n";
  ShowFuzzySet(fs5.GiveSetData());
}

int main()
{
  FuzzySet<char>::SetComparisonFunction(CompChar);
  TestFuzzySets(cout);

  char c0('0');
  char c1('1');
  char c2('2');
  char c3('3');
  char c4('4');
  char c5('5');
  char c6('6');
  char c7('7');
  char c8('8');
  char c9('9');
  char ca('A');
  char cb('B');
  char cc('C');
  char cd('D');
  char ce('E');
  char cf('F');

  FuzzySet<char> s1;
  FuzzySet<char> s2;
  FuzzySet<char> s3;
  FuzzySet<char> s4;
  FuzzySet<char> s5;
  FuzzySet<char> s6;
  FuzzySet<char> s7;
  FuzzySet<char> s8;
  FuzzyNode<double,char> Node_;
  FuzzyNode<double,char>* NodePtr_;

  Node_ = FuzzyNode<double,char>(0.1, c0);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, c1);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.3, c2);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.4, c3);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.5, c4);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.6, c5);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.7, c6);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.8, c7);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.9, c8);
  s1 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.5, c9);
  s1 |= &Node_;

  Node_ = FuzzyNode<double,char>(0.1, c0);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, c1);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.3, c2);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.4, c3);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.5, c4);
  s2 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.6, c5);
  s2 |= &Node_;

  s3 = s2;

  Node_ = FuzzyNode<double,char>(0.7, c6);
  s4 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, c7);
  s4 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.9, c8);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.4, c9);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.5, ca);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.6, cb);
  s4 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.7, cc);
  s4 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.8, cd);
  s4 |= &Node_;

  s5 = s4;
  Node_ = FuzzyNode<double,char>(0.1, ca);
  s5 |= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, cb);
  s5 |= &Node_;

  s6 = s1 & s4;
  s7 = s1 | s4;
  s8 = s1 - s4;

  assert(s1 > s2);
  assert(s1 >= s2);
  assert(s3 >= s2);
  assert(s3 == s2);
  assert(s1 != s2);

  ShowFuzzySet(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowFuzzySet(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD0123456789");
  ShowFuzzySet(s8.GiveSetData());
  assert(s8.GiveSetData() == "97543210");

  size_t Index_;
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, c0));
  assert(NodePtr_ != NULL && NodePtr_->_Value == '0');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, c4));
  assert(NodePtr_ != NULL && NodePtr_->_Value == '4');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, c9));
  assert(NodePtr_ != NULL && NodePtr_->_Value == '9');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, cd));
  assert(NodePtr_ != NULL && NodePtr_->_Value == 'D');
  NodePtr_ = s7.FindItem(FuzzyNode<double,char>(0.1, char('x')));
  assert(NodePtr_ == NULL);

  s8 = s7 = s6 = s1;
  s6 &= s4;
  s7 |= s4;
  s8 -= s4;

  ShowFuzzySet(s6.GiveSetData());
  assert(s6.GiveSetData() == "6789");
  ShowFuzzySet(s7.GiveSetData());
  assert(s7.GiveSetData() == "ABCD9876543210");
  ShowFuzzySet(s8.GiveSetData());
  assert(s8.GiveSetData() == "97543210");

  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 > &Node_);
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 > &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(!(s5 == &Node_));
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(!(s5 == &Node_));

  assert(!(s5 > s4));
  assert(s5 >= s4);
  assert(s5 == s4);

  Node_ = FuzzyNode<double,char>(0.1, ca);
  s5 -= &Node_;
  Node_ = FuzzyNode<double,char>(0.2, cb);
  s5 -= &Node_;
  Node_ = FuzzyNode<double,char>(0.3, c4);
  s5 |= &Node_;  
  Node_ = FuzzyNode<double,char>(0.4, c5);
  s5 |= &Node_;  

  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(!(s5 > &Node_));
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(!(s5 > &Node_));
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 >= &Node_);
  Node_ = FuzzyNode<double,char>(0.1, ca);
  assert(s5 == &Node_);
  Node_ = FuzzyNode<double,char>(0.1, cb);
  assert(s5 == &Node_);

  assert(s5 > s4);
  assert(s5 >= s4);
  assert(!(s5 == s4));

#if FUZZYSET_NOTIFY_SUCCESS
  cout <<"SUCCESS testing <FuzzySet>" <<endl;
#endif
  return 0;
}
#endif
/****************************************************************************/
#endif




