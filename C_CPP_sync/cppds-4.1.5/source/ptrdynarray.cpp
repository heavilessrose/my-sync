#ifndef PTRDYNARRAY_CPP
#define PTRDYNARRAY_CPP
#ifndef PTRDYNARRAY_H
  #include "ptrdynarray.h"
#endif

/****************************************************************************/
template <class T>
int(*PtrDynArray<T>::_CompFunc)(const T*, const T*) = NULL;

/****************************************************************************/
template <class T>
SimpleArray<int> SeriesArrayCompare(const PtrDynArray<T> &ia1, const PtrDynArray<T> &ia2)
{
  if (_CompFunc && (ia1.RunLength() == ia2.RunLength()))
  {
    SimpleArray<int> Result_(ia1.RunLength());
    size_t x, Max_;
    
    for (x = 0, Max_ = ia1.RunLength(); x < Max_; x++)
      Result_[x] = (*_CompFunc)(ia1[x], ia2[x]);

    return Result_;
  }

  SimpleArray<int> Temp_;
  Temp_.SetError(TRUE);
  return Temp_;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
PtrDynArray<T>::PtrDynArray():
DynArrayBase<void*>()
{
  _Tidy();
}

/****************************************************************************/
template <class T>
PtrDynArray<T>::PtrDynArray(size_t n, capacity c):
DynArrayBase<void*>(n, c)
{
  _Tidy(), _Res = n;

  if (c == default_size)
    PtrDynArray<T>::_NVMGrow(n);
}

/****************************************************************************/
template <class T>
PtrDynArray<T>::PtrDynArray(const PtrDynArray<T>& x):
ObjectImp((ObjectImp&)x),
DynArrayBase<void*>(x)
{
  _Tidy();
  PtrDynArray<T>::_NVMGrow(x.RunLength());

  size_t i;
  for (i = 0; i < _Len; ++i)
    _Ptr[i] = x[i];
}

/****************************************************************************/
template <class T>
PtrDynArray<T>::PtrDynArray(T* x, size_t n):
DynArrayBase<void*>((void*)x, n)
{
  _Tidy();
  PtrDynArray<T>::_NVMGrow(n, (void**)&x);
}

/****************************************************************************/
template <class T>
PtrDynArray<T>::PtrDynArray(T** s, size_t n):
DynArrayBase<void*>((void**)s, n)
{
  if (s == 0)
    _Xinv();

  _Tidy(), AssignArg(s, n);
}

/****************************************************************************/
template <class T>
PtrDynArray<T>& PtrDynArray<T>::operator = (const PtrDynArray<T>& Rhs_)
{
  (DynArrayBase<void*>::operator = ((const DynArrayBase<void*>&)Rhs_));
  ObjectImp::operator = ((Object&)Rhs_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::IsNewed() const
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
void PtrDynArray<T>::DestroyFromIterator()
{
# if OVERLOAD_NEW
    _DestroyFromIterator = IsNewed();
# else
    _DestroyFromIterator = TRUE;
# endif
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::RegisterIterator()
{
  if (_IteratorsActive < MaxSize())
  {
    ++_IteratorsActive;
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::UnRegisterIterator()
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
size_t PtrDynArray<T>::IteratorsActive()
{
  return _IteratorsActive;
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::_NVMGrow(size_t n, void* const* s, Boolean Trim_)
{
  size_t Os_ = _Ptr == 0 ? 0:_Res;

  if (n == 0)
  {
    if (Trim_)
      _Tidy(1);
  }
  else if (n == Os_ || n == Os_ && !Trim_)
	;
  else if (n == SIZET_MAX)
    _Xlen();
  else
  {
    size_t i, m = _Ptr == 0 && n < _Res ? _Res:n;

    Boolean Active_ = MemMatrix::Matrix().IsNewHandlerActive();
    MemMatrix::Matrix().SetNewHandlerInactive();

#if OVERLOAD_NEW
    void** Np_ = (void**)MemMatrix::Matrix().Allocate(sizeof(void*) * m);
#else
    void** Np_ = new void*[m];
#endif

    if (Np_ == 0)
      MemMatrix::Matrix().NoMemory();

    if (Active_)
      MemMatrix::Matrix().SetNewHandlerActive();

    _Res = m, m = n < _Len ? n:_Len;

    for (i = 0; i < m; ++i)
      Np_[i] = _Ptr[i];

    if (s != 0)
      for (; i < _Res; ++i)
	Np_[i] = *s;

    _Tidy(1), _Ptr = Np_;
  }

  _Len = n;
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::_Grow(size_t n, void* const* s, Boolean Trim_)
{
  PtrDynArray<T>::_NVMGrow(n, s, Trim_);
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::SetComparisonFunction(int(*CmpFnc_)(const T*, const T*))
{
  _CompFunc = CmpFnc_;
}

/****************************************************************************/
template <class T>
SimpleArray<int> PtrDynArray<T>::SeriesArrayCompare(const PtrDynArray<T>& ia) const
{
  if (_CompFunc && (RunLength() == ia.RunLength()))
  {
    SimpleArray<int> Result_(RunLength());
    size_t x, Max_;
    
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
    for (x = 0, Max_ = RunLength(); x < Max_; x++)
      (Result_.Base())[x] = (*_CompFunc)((*this)[x], ia[x]);
#else
    for (x = 0, Max_ = RunLength(); x < Max_; x++)
      Result_[x] = (*_CompFunc)((*this)[x], ia[x]);
#endif

    return Result_;
  }

  SimpleArray<int> Temp_;
  Temp_.SetError(TRUE);
  return Temp_;
}

/****************************************************************************/
template <class T>
int PtrDynArray<T>::Compare(const PtrDynArray<T>& Arr_) const
{
  if (_CompFunc && (RunLength() == Arr_.RunLength()))
  {
    size_t x, Max_;
    int Result_ = 0;
    
    for (x = 0, Max_ = RunLength(); x < Max_ && !Result_; x++)
      Result_ = (*_CompFunc)((*this)[x], Arr_[x]);

    return Result_;
  }

  _Xran();
  return 0;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::IsEqualToPtrDynArray(const PtrDynArray<T>* Ptr_) const
{
  if (_CompFunc && (RunLength() == Ptr_->RunLength()))
  {
    size_t x, Max_;
    int Result_ = 0;
    
    for (x = 0, Max_ = RunLength(); x < Max_ && !Result_; x++)
      Result_ = (*_CompFunc)((*this)[x], (*Ptr_)[x]);

    return (Result_ == 0);
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::IsLesserToPtrDynArray(const PtrDynArray<T>* Ptr_) const
{
  if (_CompFunc && (RunLength() == Ptr_->RunLength()))
  {
    size_t x, Max_;
    int Result_ = 0;
    
    for (x = 0, Max_ = RunLength(); x < Max_ && !Result_; x++)
      Result_ = (*_CompFunc)((*this)[x], (*Ptr_)[x]);

    return (Result_ < 0);
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::IsGreaterToPtrDynArray(const PtrDynArray<T>* Ptr_) const
{
  if (_CompFunc && (RunLength() == Ptr_->RunLength()))
  {
    size_t x, Max_;
    int Result_ = 0;
    
    for (x = 0, Max_ = RunLength(); x < Max_ && !Result_; x++)
      Result_ = (*_CompFunc)((*this)[x], (*Ptr_)[x]);

    return (Result_ > 0);
  }

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  PtrDynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToPtrDynArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  PtrDynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToPtrDynArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean PtrDynArray<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  PtrDynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToPtrDynArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Object& PtrDynArray<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  PtrDynArrayAcceptor<T> TrgComp_(this);
  return *((Object*) (ObjComp_->AssignFromPtrDynArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& PtrDynArray<T>::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  PtrDynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->ShallowAssign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromPtrDynArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* PtrDynArray<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return NULL;

  PtrDynArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromPtrDynArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
const Object* PtrDynArray<T>::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new PtrDynArray<T>(*this));

  PtrDynArrayAcceptor<T> TrgComp_(this);
  return ((const Object*) (ObjComp_->CreateFromPtrDynArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* PtrDynArray<T>::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return (new PtrDynArray<T>(*this));

  PtrDynArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromPtrDynArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
PtrDynArray<T>& PtrDynArray<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static PtrDynArray<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& PtrDynArray<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return PtrDynArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& PtrDynArray<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return PtrDynArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
long PtrDynArray<T>::StorableFormWidth() const
{
  return 0;
}

/****************************************************************************/
template <class T>
int PtrDynArray<T>::ClassID() const
{
  return TypeInfo::PTRDYNARRAY_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* PtrDynArray<T>::GiveObjectAcceptor() const
{
  return (new PtrDynArrayAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
istream& PtrDynArray<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
istream& PtrDynArray<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& PtrDynArray<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& PtrDynArray<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& PtrDynArray<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& PtrDynArray<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
istream& PtrDynArray<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& PtrDynArray<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;
  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    const T* Ptr_ = ((*this)[i]);
    
    if (Ptr_)
      WriteToOutput(Os_, *Ptr_);
      
    Os_ <<" ";
  }

  return Os_;
}

/****************************************************************************/
template <class T>
PtrDynArray<T>& PtrDynArray<T>::ApplyUnary(T* (*func)(T*))
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      _Ptr[i] = (*func)((T*)_Ptr[i]);
  }

  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArray<T>& PtrDynArray<T>::ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      if (_Ptr[i])
        (*func)(*((T*)_Ptr[i]), Obj_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArray<T>& PtrDynArray<T>::ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_)
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      if (_Ptr[i])    
        (*func)(*((T*)_Ptr[i]), Obj_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArray<T>& PtrDynArray<T>::ApplyUnary(int(*Fnc_)(T*))
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      (*func)((T*)_Ptr[i]);
  }

  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArray<T>& PtrDynArray<T>::Sort(SortingHelper<T*, T>* SortHelpPtr_)
{
  ItemSorter<T*, T> Sorter_;
    
  Sorter_.SetSortHelper(SortHelpPtr_);
  Sorter_.QuickSort(Base(), 0, RunLength() - 1);

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PtrDynArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* PtrDynArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void PtrDynArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if DYNARRAY_IMPLEMENT_STLCONTAINER
template <class T>
const PtrDynArrayIterator<T> PtrDynArray<T>::begin() const
{
  return (PtrDynArrayIterator<T>(this));
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T> PtrDynArray<T>::begin()
{
  return (PtrDynArrayIterator<T>(this));
}

/****************************************************************************/
template <class T>
const PtrDynArrayIterator<T> PtrDynArray<T>::end() const
{
  PtrDynArrayIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T> PtrDynArray<T>::end()
{
  PtrDynArrayIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> PtrDynArray<T>::rbegin() const
{
  PtrDynArrayIterator<T> Iter_(this);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> PtrDynArray<T>::rbegin()
{
  PtrDynArrayIterator<T> Iter_(this);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> PtrDynArray<T>::rend() const
{
  PtrDynArrayIterator<T> Iter_(this);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> PtrDynArray<T>::rend()
{
  PtrDynArrayIterator<T> Iter_(this);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
T* PtrDynArray<T>::front()
{
  return &(*begin());
}

/****************************************************************************/
template <class T>
const T* PtrDynArray<T>::front() const
{
  return &(*begin());
}

/****************************************************************************/
template <class T>
T* PtrDynArray<T>::back()
{
  return &(*(--end()));
}

/****************************************************************************/
template <class T>
const T* PtrDynArray<T>::back() const
{
  return &(*(--end()));
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::push_back(T* x)
{
  insert(end(), x);
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::pop_back()
{
  PtrDynArrayIterator<T> Iter_ = end();
  --Iter_;
  erase(Iter_);
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T> PtrDynArray<T>::insert(PtrDynArrayIterator<T> pos, T* x)
{
  --pos;

  if (pos.More())
    Insert(pos.Position(), x);

  return pos;
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::insert(PtrDynArrayIterator<T> pos, size_t n, T* x)
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
void PtrDynArray<T>::insert(PtrDynArrayIterator<T> pos, const PtrDynArrayIterator<T> first, const PtrDynArrayIterator<T> last)
{
  --pos;

  if (pos.More())
    while (first != last && first.More())
    {
      T* Obj_ = Get_At(first.Position());
      Insert(pos.Position(), Obj_);
      ++first;
    }
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::erase(PtrDynArrayIterator<T> pos)
{
  if (pos.More())
    (*this)[pos.Position()] = NULL;
}

/****************************************************************************/
template <class T>
void PtrDynArray<T>::erase(PtrDynArrayIterator<T> first, PtrDynArrayIterator<T> last)
{
  while (first != last && first.More())
  {
    (*this)[first.Position()] = NULL;
    ++first;
  }
}
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
PtrDynArrayAcceptor<T>::PtrDynArrayAcceptor(const PtrDynArray<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayAcceptor<T>::IsDynArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayAcceptor<T>::IsPtrDynArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void PtrDynArrayAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const PtrDynArray<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* PtrDynArrayAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
void* PtrDynArrayAcceptor<T>::CreateFromPtrDynArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new PtrDynArray<T>(*((const PtrDynArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* PtrDynArrayAcceptor<T>::AssignFromPtrDynArray(const ObjectAcceptor& Obj_)
{
  const PtrDynArray<T>* TrgPtr_ = (const PtrDynArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayAcceptor<T>::IsEqualToPtrDynArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToPtrDynArray((const PtrDynArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayAcceptor<T>::IsLesserToPtrDynArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToPtrDynArray((const PtrDynArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayAcceptor<T>::IsGreaterToPtrDynArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToPtrDynArray((const PtrDynArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PtrDynArrayAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PtrDynArrayAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if DYNARRAY_IMPLEMENT_STLITERATOR
template <class T>
PtrDynArrayIterator<T>::PtrDynArrayIterator(const PtrDynArray<T>* Ap_, Boolean DestroyFromIter_):
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
PtrDynArrayIterator<T>::PtrDynArrayIterator(const PtrDynArrayIterator<T>& Ap_):
RandomAccessIterator<T>(),
_ArrayObjPtr(Ap_._ArrayObjPtr),
_Index(Ap_._Index)
{
  if (_ArrayObjPtr)
    _ArrayVarPtr->RegisterIterator();
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>::~PtrDynArrayIterator()
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
PtrDynArrayIterator<T>* PtrDynArrayIterator<T>::Make(PtrDynArray<T>* Ap_)
{
  return (new PtrDynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>* PtrDynArrayIterator<T>::Make(PtrDynArrayIterator<T>& Ap_)
{
  return (new PtrDynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const PtrDynArrayIterator<T>* PtrDynArrayIterator<T>::MakeConst(const PtrDynArray<T>* Ap_)
{
  return (new PtrDynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const PtrDynArrayIterator<T>* PtrDynArrayIterator<T>::MakeConst(const PtrDynArrayIterator<T>& Ap_)
{
  return (new PtrDynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
int PtrDynArrayIterator<T>::ClassID() const
{
  return TypeInfo::PTRDYNARRAYITERATOR_TYPE;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>* PtrDynArrayIterator<T>::Clone()
{
  return (new PtrDynArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>* PtrDynArrayIterator<T>::Clone() const
{
  return (new PtrDynArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayIterator<T>::IsValid(SizeType Index_) const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (Index_ < _ArrayVarPtr->RunLength())
    return TRUE;

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayIterator<T>::IsValid() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return TRUE;

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayIterator<T>::More() const
{
  if (_ArrayVarPtr->RunLength() == 0)
  {
    PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;
    LocalThis_->_Index = SizeType(-1);
  }

  return !AtEnd();
}

/****************************************************************************/
template <class T>
void PtrDynArrayIterator<T>::Reset() const
{
  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;
  LocalThis_->_Index = 0;
}

/****************************************************************************/
template <class T>
void PtrDynArrayIterator<T>::SetToLast() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;
  LocalThis_->_Index = _ArrayObjPtr->RunLength() - 1;
}

/****************************************************************************/
template <class T>
void PtrDynArrayIterator<T>::SetToEnd() const
{
  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;
  LocalThis_->_Index = SizeType(-1);
}

/****************************************************************************/
template <class T>
const T* PtrDynArrayIterator<T>::ConstPointee() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return ((*_ArrayObjPtr)[_Index]);
  return ((*_ArrayObjPtr)[0]);
}

/****************************************************************************/
template <class T>
const T* PtrDynArrayIterator<T>::Pointee() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return ((*_ArrayVarPtr)[_Index]);
  return ((*_ArrayVarPtr)[0]);
}

/****************************************************************************/
template <class T>
T* PtrDynArrayIterator<T>::Pointee()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return ((*_ArrayVarPtr)[_Index]);
  return ((*_ArrayVarPtr)[0]);
}

/****************************************************************************/
template <class T>
const PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::GotoHead() const
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>
const PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::GotoTail() const
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::GotoHead()
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::GotoTail()
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayIterator<T>::AtHead() const
{
  return (_Index == 0);
}

/****************************************************************************/
template <class T>
Boolean PtrDynArrayIterator<T>::AtTail() const
{
  return (_Index == _ArrayObjPtr->RunLength() - 1);
}

/****************************************************************************/
template <class T>
T& PtrDynArrayIterator<T>::operator * ()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return *((*_ArrayVarPtr)[_Index]);
  return *((*_ArrayVarPtr)[0]);
}

/****************************************************************************/
template <class T>
const T& PtrDynArrayIterator<T>::operator * () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return *((*_ArrayObjPtr)[_Index]);
  return *((*_ArrayObjPtr)[0]);
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::operator [] (SizeType Index_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  _Index = Index_;
  if (!AtEnd())
    ((*_ArrayVarPtr)[_Index]);
  else
    ((*_ArrayVarPtr)[0]);

  return *this;
}

/****************************************************************************/
template <class T>
const PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::operator [] (SizeType Index_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;
  LocalThis_->_Index = Index_;

  if (!AtEnd())
    ((*_ArrayObjPtr)[_Index]);
  else
    ((*_ArrayObjPtr)[0]);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator ++ ()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (_Index < _ArrayVarPtr->RunLength() - 1)
    ++_Index;
  else
    _Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> PtrDynArrayIterator<T>::operator ++ (int)
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (_Index < _ArrayVarPtr->RunLength() - 1)
    ++_Index;
  else
    _Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator ++ () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrDynArrayIterator<T>::operator ++ (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator -- ()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    --_Index;
  else
    SetToLast();

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> PtrDynArrayIterator<T>::operator -- (int)
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    --_Index;
  else
    SetToLast();

  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator -- () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrDynArrayIterator<T>::operator -- (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> PtrDynArrayIterator<T>::operator + (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrDynArrayIterator<T>::operator + (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> PtrDynArrayIterator<T>::operator - (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrDynArrayIterator<T>::operator - (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator += (SizeType Diff_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (_Index + Diff_ < _ArrayVarPtr->RunLength())
    _Index += Diff_;
  else
    _Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator += (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;

  if (_Index + Diff_ < _ArrayObjPtr->RunLength())
    LocalThis_->_Index += Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator -= (SizeType Diff_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (_Index >= Diff_)
    _Index -= Diff_;
  else if (!More() && Diff_ <= _ArrayObjPtr->RunLength())
    _Index = _ArrayObjPtr->RunLength() - Diff_;
  else
    _Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& PtrDynArrayIterator<T>::operator -= (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrDynArrayIterator<T>* const LocalThis_ = (PtrDynArrayIterator<T>* const)this;

  if (_Index >= Diff_)
    LocalThis_->_Index -= Diff_;
  else if (!More() && Diff_ <= _ArrayObjPtr->RunLength())
    LocalThis_->_Index = _ArrayObjPtr->RunLength() - Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::SetData(T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index] = Ptr_;

  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::operator = (T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index] = Ptr_;

  return *this;
}

/****************************************************************************/
template <class T>
PtrDynArrayIterator<T>& PtrDynArrayIterator<T>::operator = (const PtrDynArrayIterator<T>& Rhs_)
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
#endif





