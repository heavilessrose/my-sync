#ifndef DYNARRAY_CPP
#define DYNARRAY_CPP
#ifndef DYNARRAY_H
  #include "dynarray.h"
#endif

/****************************************************************************/
template <class T>
int(*DynArray<T>::_CompFunc)(const T&, const T&) = NULL;

/****************************************************************************/
template <class T>
SimpleArray<int> SeriesArrayCompare(const DynArray<T> &ia1, const DynArray<T> &ia2)
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
DynArray<T>::DynArray():
DynArrayBase<T>()
{
  _Tidy();
}

/****************************************************************************/
template <class T>
DynArray<T>::DynArray(size_t n, capacity c):
DynArrayBase<T>(n, c)
{
  _Tidy(), _Res = n;

  if (c == default_size)
    DynArray<T>::_NVMGrow(n);
}

/****************************************************************************/
template <class T>
DynArray<T>::DynArray(const DynArray<T>& x):
ObjectImp((ObjectImp&)x),
DynArrayBase<T>(x)
{
  _Tidy();
  DynArray<T>::_NVMGrow(x.RunLength());

  size_t i;
  for (i = 0; i < _Len; ++i)
    _Ptr[i] = x[i];
}

/****************************************************************************/
template <class T>
DynArray<T>::DynArray(const T& x, size_t n):
DynArrayBase<T>(x, n)
{
  _Tidy();
  DynArray<T>::_NVMGrow(n, &x);
}

/****************************************************************************/
template <class T>
DynArray<T>::DynArray(const T* s, size_t n):
DynArrayBase<T>(s, n)
{
  if (s == 0)
    _Xinv();

  _Tidy(), AssignArg(s, n);
}

/****************************************************************************/
template <class T>
DynArray<T>& DynArray<T>::operator = (const DynArray<T>& r)
{
  DynArrayBase<T>::operator = (r);
  ObjectImp::operator = ((Object&)r);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean DynArray<T>::IsNewed() const
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
void DynArray<T>::DestroyFromIterator()
{
# if OVERLOAD_NEW
    _DestroyFromIterator = IsNewed();
# else
    _DestroyFromIterator = TRUE;
# endif
}

/****************************************************************************/
template <class T>
Boolean DynArray<T>::RegisterIterator()
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
Boolean DynArray<T>::UnRegisterIterator()
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
size_t DynArray<T>::IteratorsActive()
{
  return _IteratorsActive;
}

/****************************************************************************/
template <class T>
void DynArray<T>::_NVMGrow(size_t n, const T* s, Boolean Trim_)
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
    T* Np_ = New<T>(m);
#else
    T* Np_ = new T[m];
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
void DynArray<T>::_Grow(size_t n, const T* s, Boolean Trim_)
{
  DynArray<T>::_NVMGrow(n, s, Trim_);
}

/****************************************************************************/
template <class T>
void DynArray<T>::SetComparisonFunction(int(*CmpFnc_)(const T&, const T&))
{
  _CompFunc = CmpFnc_;
}

/****************************************************************************/
template <class T>
SimpleArray<int> DynArray<T>::SeriesArrayCompare(const DynArray<T>& ia) const
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
int DynArray<T>::Compare(const DynArray<T>& Arr_) const
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
Boolean DynArray<T>::IsEqualToDynArray(const DynArray<T>* Ptr_) const
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
Boolean DynArray<T>::IsLesserToDynArray(const DynArray<T>* Ptr_) const
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
Boolean DynArray<T>::IsGreaterToDynArray(const DynArray<T>* Ptr_) const
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
Boolean DynArray<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToDynArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean DynArray<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToDynArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean DynArray<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToDynArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Object& DynArray<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  DynArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromDynArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& DynArray<T>::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  DynArrayAcceptor<T> TrgComp_(this);
  return *((Object*) ObjComp_->AssignFromDynArray(TrgComp_));
}

/****************************************************************************/
template <class T>
Object* DynArray<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new DynArray<T>(*this));

  DynArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromDynArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
const Object* DynArray<T>::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
Object* DynArray<T>::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
DynArray<T>& DynArray<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static DynArray<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& DynArray<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return DynArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& DynArray<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return DynArray<T>::GiveNullObject();
}

/****************************************************************************/
// Returns the total size of all the data within the container structure
// excluding formatting and class ID information.
//
template <class T>
long DynArray<T>::DataSize() const
{
  long DataSize_ = 0;

  for (size_t Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
    DataSize_ += SizeOfData((*this)[Index_]);

  return DataSize_;
}

/****************************************************************************/
template <class T>
long DynArray<T>::StorableFormWidth() const
{
  // First data item is the ID code for the data structure
  long ObjSize_ = sizeof(long);

  // Second data item is the ID of the object
  ObjSize_ += sizeof(long);

  // Third data item is the null object status byte
  ObjSize_ += sizeof(Byte_t);

  // Fourth data item is the actual array size in number of elements
  ObjSize_ += sizeof(long);

  // Each array element is encoded as data comprising
  // the actual size of the object
  for (size_t Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
    ObjSize_ += sizeof(long) + FormWidthOfObject((*this)[Index_]);

  return ObjSize_;
}

/****************************************************************************/
template <class T>
int DynArray<T>::ClassID() const
{
  return TypeInfo::DYNARRAY_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* DynArray<T>::GiveObjectAcceptor() const
{
  return (new DynArrayAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
void DynArray<T>::IgnoreElement(ByteStream& Is_, Boolean* Ok_)
{
  T Object_;  
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.iread((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
    Ignore(Is_, ((T&)(*ObjPtr_)), &Done_);

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void DynArray<T>::ReadElement(ByteStream& Is_, size_t Index_, Boolean* Ok_)
{
  T Object_;
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.iread((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
  {
    Read(Is_, ((T&)(*ObjPtr_)), &Done_);
    if (IstreamCheck(Is_, Ok_) && Done_)
      (*this)[Index_] = *ObjPtr_;
  }

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void DynArray<T>::WriteElement(ByteStream& Os_, size_t Index_, Boolean* Ok_) const
{
  const T* ObjPtr_ = &((*this)[Index_]);
  long Code_ = WhatIs(*ObjPtr_);
  Boolean WrOk_ = OstreamCheck(Os_.owrite((char*)(&Code_), sizeof(long)), Ok_);

  if (WrOk_)
  {
    Write(Os_, *ObjPtr_, Ok_);
    OstreamCheck(Os_, Ok_);
  }
}

/****************************************************************************/
template <class T>
void DynArray<T>::IgnoreElement(istream& Is_, Boolean* Ok_)
{
  T Object_;
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.read((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
    Ignore(Is_, ((T&)(*ObjPtr_)), &Done_);

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void DynArray<T>::ReadElement(istream& Is_, size_t Index_, Boolean* Ok_)
{
  T Object_;
  T* ObjPtr_ = NULL;

  long RdCode_;
  long ObjCode_ = WhatIs(Object_);
  Boolean Match_ = TRUE;
  Boolean Done_ = FALSE;

  if (Is_.read((char*)(&RdCode_), sizeof(long)).good())
    if (ObjCode_ == RdCode_)
      ObjPtr_ = &Object_;
    else
    {
      ObjPtr_ = MakeByTypeCode(RdCode_);
      Match_ = FALSE;
    }

  if (ObjPtr_)
  {
    Read(Is_, ((T&)(*ObjPtr_)), &Done_);
    if (IstreamCheck(Is_, Ok_) && Done_)
      (*this)[Index_] = *ObjPtr_;
  }

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
}

/****************************************************************************/
template <class T>
void DynArray<T>::WriteElement(ostream& Os_, size_t Index_, Boolean* Ok_) const
{
  const T* ObjPtr_ = &((*this)[Index_]);
  long Code_ = WhatIs(*ObjPtr_);
  Boolean WrOk_ = OstreamCheck(Os_.write((char*)(&Code_), sizeof(long)), Ok_);

  if (WrOk_)
  {
    Write(Os_, *ObjPtr_, Ok_);
    OstreamCheck(Os_, Ok_);
  }
}

/****************************************************************************/
template <class T>
istream& DynArray<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    Is_.read((char*)(&Size_), sizeof(long));

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	IgnoreElement(Is_, Ok_);

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
istream& DynArray<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Is_.read((char*)(&Size_), sizeof(long));
    _Grow((size_t)Size_, 0, 1);

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	ReadElement(Is_, Index_, Ok_);

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }

    Resize(Size_);
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& DynArray<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  long RunLength_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    RunLength_ = (long)RunLength();
    Os_.write((char*)(&RunLength_), sizeof(long));

    if (OstreamCheck(Os_, Ok_))
    {
      for (Index_ = 0; Index_ < RunLength_; Index_++)
      {
	WriteElement(Os_, Index_, Ok_);

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& DynArray<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);

  if (*Ok_)
  {
    Is_.iread((char*)(&Size_), sizeof(long));

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	IgnoreElement(Is_, Ok_);

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& DynArray<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Size_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Is_.iread((char*)(&Size_), sizeof(long));
    _Grow((size_t)Size_, 0, 1);

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	ReadElement(Is_, Index_, Ok_);

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }

    Resize(Size_);
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& DynArray<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean RdChk_;
  long RunLength_, Index_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    RunLength_ = (long)RunLength();
    Os_.owrite((char*)(&RunLength_), sizeof(long));

    if (OstreamCheck(Os_, Ok_))
    {
      for (Index_ = 0; Index_ < RunLength_; Index_++)
      {
	WriteElement(Os_, Index_, Ok_);

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
istream& DynArray<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& DynArray<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;
  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {
    WriteToOutput(Os_, ((*this)[i]));
    Os_ <<" ";
  }

  return Os_;
}

/****************************************************************************/
// PURPOSE:
//   Method to dereference and invoke the _Factory pointer to function with
//   the specified type code to create a new object of T or derived from T.
//   if the pointer to function is null then a Null pointer exception is
//   thrown.
//
// PRE:
//   int Code_	: The type code to invoke the factory function with.
//
// POST:
//   If the factory function is not null then it should return a newly
//   allocated object of type T or derived from T with the same type code
//   as the one specified in the argument to the method.
//
template <class T>
T* DynArray<T>::MakeByTypeCode(int Code_)
{
  // If pointer to function is null then throw an exception.
  if (_Factory == NULL)
    Xnullp();

  // Create object based on type code.
  return (*_Factory)(Code_);
}

/****************************************************************************/
template <class T>
DynArray<T>& DynArray<T>::Sort(SortingHelper<T, T>* SortHelpPtr_)
{
  ItemSorter<T, T> Sorter_;
    
  Sorter_.SetSortHelper(SortHelpPtr_);
  Sorter_.QuickSort(Base(), 0, RunLength() - 1);

  return *this;
}

/****************************************************************************/
template <class T>
DynArray<T>& DynArray<T>::ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      (*func)(_Ptr[i], Obj_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
DynArray<T>& DynArray<T>::ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_)
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      (*func)(_Ptr[i], Obj_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
DynArray<T>& DynArray<T>::ApplyUnary(T(*func)(T))
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      _Ptr[i] = (*func)(_Ptr[i]);
  }

  return *this;
}

/****************************************************************************/
template <class T>
DynArray<T>& DynArray<T>::ApplyUnary(int(*Fnc_)(T*))
{
  if (func && RunLength())
  {
    size_t i, Max_;
    for (i = 0, Max_ = RunLength(); i < Max_; ++i)
      (*func)(&_Ptr[i]);
  }

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DynArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DynArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* DynArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void DynArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if DYNARRAY_IMPLEMENT_STLCONTAINER
template <class T>
const DynArrayIterator<T> DynArray<T>::begin() const
{
  return (DynArrayIterator<T>(this));
}

/****************************************************************************/
template <class T>
DynArrayIterator<T> DynArray<T>::begin()
{
  return (DynArrayIterator<T>(this));
}

/****************************************************************************/
template <class T>
const DynArrayIterator<T> DynArray<T>::end() const
{
  DynArrayIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
DynArrayIterator<T> DynArray<T>::end()
{
  DynArrayIterator<T> Iter_(this);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> DynArray<T>::rbegin() const
{
  DynArrayIterator<T> Iter_(this);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> DynArray<T>::rbegin()
{
  DynArrayIterator<T> Iter_(this);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> DynArray<T>::rend() const
{
  DynArrayIterator<T> Iter_(this);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> DynArray<T>::rend()
{
  DynArrayIterator<T> Iter_(this);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
T& DynArray<T>::front()
{
  return (*begin());
}

/****************************************************************************/
template <class T>
const T& DynArray<T>::front() const
{
  return (*begin());
}

/****************************************************************************/
template <class T>
T& DynArray<T>::back()
{
  return *(--end());
}

/****************************************************************************/
template <class T>
const T& DynArray<T>::back() const
{
  return *(--end());
}

/****************************************************************************/
template <class T>
void DynArray<T>::push_back(const T& x)
{
  insert(end(), x);
}

/****************************************************************************/
template <class T>
void DynArray<T>::pop_back()
{
  DynArrayIterator<T> Iter_ = end();
  --Iter_;
  erase(Iter_);
}

/****************************************************************************/
template <class T>
DynArrayIterator<T> DynArray<T>::insert(DynArrayIterator<T> pos, const T& x)
{
  --pos;

  if (pos.More())
    Insert(pos.Position(), x);

  return pos;
}

/****************************************************************************/
template <class T>
void DynArray<T>::insert(DynArrayIterator<T> pos, size_t n, const T& x)
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
void DynArray<T>::insert(DynArrayIterator<T> pos, const DynArrayIterator<T> first, const DynArrayIterator<T> last)
{
  --pos;

  if (pos.More())
    while (first != last && first.More())
    {
      T Obj_ = Get_At(first.Position());
      Insert(pos.Position(), Obj_);
      ++first;
    }
}

/****************************************************************************/
template <class T>
void DynArray<T>::erase(DynArrayIterator<T> pos)
{
  if (pos.More())
    (*this)[pos.Position()] = T();
}

/****************************************************************************/
template <class T>
void DynArray<T>::erase(DynArrayIterator<T> first, DynArrayIterator<T> last)
{
  while (first != last && first.More())
  {
    (*this)[first.Position()] = T();
    ++first;
  }
}
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
DynArrayAcceptor<T>::DynArrayAcceptor(const DynArray<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
Boolean DynArrayAcceptor<T>::IsDynArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void DynArrayAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const DynArray<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* DynArrayAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
void* DynArrayAcceptor<T>::CreateFromDynArray(const ObjectAcceptor& Obj_)
{
  _VarPtr = new DynArray<T>(*((const DynArray<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* DynArrayAcceptor<T>::AssignFromDynArray(const ObjectAcceptor& Obj_)
{
  const DynArray<T>* TrgPtr_ = (const DynArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean DynArrayAcceptor<T>::IsEqualToDynArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToDynArray((const DynArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean DynArrayAcceptor<T>::IsLesserToDynArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToDynArray((const DynArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean DynArrayAcceptor<T>::IsGreaterToDynArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToDynArray((const DynArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* DynArrayAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void DynArrayAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if DYNARRAY_IMPLEMENT_STLITERATOR
template <class T>
DynArrayIterator<T>::DynArrayIterator(const DynArray<T>* Ap_, Boolean DestroyFromIter_):
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
DynArrayIterator<T>::DynArrayIterator(const DynArrayIterator<T>& Ap_):
RandomAccessIterator<T>(),
_ArrayObjPtr(Ap_._ArrayObjPtr),
_Index(Ap_._Index)
{
  if (_ArrayObjPtr)
    _ArrayVarPtr->RegisterIterator();
}

/****************************************************************************/
template <class T>
DynArrayIterator<T>::~DynArrayIterator()
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
DynArrayIterator<T>* DynArrayIterator<T>::Make(DynArray<T>* Ap_)
{
  return (new DynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
DynArrayIterator<T>* DynArrayIterator<T>::Make(DynArrayIterator<T>& Ap_)
{
  return (new DynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const DynArrayIterator<T>* DynArrayIterator<T>::MakeConst(const DynArray<T>* Ap_)
{
  return (new DynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const DynArrayIterator<T>* DynArrayIterator<T>::MakeConst(const DynArrayIterator<T>& Ap_)
{
  return (new DynArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
int DynArrayIterator<T>::ClassID() const
{
  return TypeInfo::DYNARRAYITERATOR_TYPE;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>* DynArrayIterator<T>::Clone()
{
  return (new DynArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>* DynArrayIterator<T>::Clone() const
{
  return (new DynArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
Boolean DynArrayIterator<T>::IsValid(SizeType Index_) const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (Index_ < _ArrayVarPtr->RunLength())
    return TRUE;

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DynArrayIterator<T>::IsValid() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return TRUE;

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean DynArrayIterator<T>::More() const
{
  if (_ArrayVarPtr->RunLength() == 0)
  {
    DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;
    LocalThis_->_Index = SizeType(-1);
  }

  return !AtEnd();
}

/****************************************************************************/
template <class T>
void DynArrayIterator<T>::Reset() const
{
  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;
  LocalThis_->_Index = 0;
}

/****************************************************************************/
template <class T>
void DynArrayIterator<T>::SetToLast() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;
  LocalThis_->_Index = _ArrayObjPtr->RunLength() - 1;
}

/****************************************************************************/
template <class T>
void DynArrayIterator<T>::SetToEnd() const
{
  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;
  LocalThis_->_Index = SizeType(-1);
}

/****************************************************************************/
template <class T>
const T* DynArrayIterator<T>::ConstPointee() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return &(*_ArrayObjPtr)[_Index];
  return &(*_ArrayObjPtr)[0];
}

/****************************************************************************/
template <class T>
const T* DynArrayIterator<T>::Pointee() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return &(*_ArrayVarPtr)[_Index];
  return &(*_ArrayVarPtr)[0];
}

/****************************************************************************/
template <class T>
T* DynArrayIterator<T>::Pointee()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return &(*_ArrayVarPtr)[_Index];
  return &(*_ArrayVarPtr)[0];
}

/****************************************************************************/
template <class T>
const DynArrayIterator<T>& DynArrayIterator<T>::GotoHead() const
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>
const DynArrayIterator<T>& DynArrayIterator<T>::GotoTail() const
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
DynArrayIterator<T>& DynArrayIterator<T>::GotoHead()
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>    
DynArrayIterator<T>& DynArrayIterator<T>::GotoTail()
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
Boolean DynArrayIterator<T>::AtHead() const
{
  return (_Index == 0);
}

/****************************************************************************/
template <class T>    
Boolean DynArrayIterator<T>::AtTail() const
{
  return (_Index == _ArrayObjPtr->RunLength() - 1);
}

/****************************************************************************/
template <class T>
T& DynArrayIterator<T>::operator * ()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return (*_ArrayVarPtr)[_Index];
  return (*_ArrayVarPtr)[0];
}

/****************************************************************************/
template <class T>
const T& DynArrayIterator<T>::operator * () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return (*_ArrayObjPtr)[_Index];
  return (*_ArrayObjPtr)[0];
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DynArrayIterator<T>::operator [] (SizeType Index_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  _Index = Index_;
  if (!AtEnd())
    (*_ArrayVarPtr)[_Index];
  else
    (*_ArrayVarPtr)[0];

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& DynArrayIterator<T>::operator [] (SizeType Index_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;
  LocalThis_->_Index = Index_;

  if (!AtEnd())
    (*_ArrayObjPtr)[_Index];
  else
    (*_ArrayObjPtr)[0];

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DynArrayIterator<T>::operator ++ ()
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
RandomAccessIterator<T> DynArrayIterator<T>::operator ++ (int)
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
const RandomAccessIterator<T>& DynArrayIterator<T>::operator ++ () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DynArrayIterator<T>::operator ++ (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DynArrayIterator<T>::operator -- ()
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
RandomAccessIterator<T> DynArrayIterator<T>::operator -- (int)
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
const RandomAccessIterator<T>& DynArrayIterator<T>::operator -- () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DynArrayIterator<T>::operator -- (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> DynArrayIterator<T>::operator + (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DynArrayIterator<T>::operator + (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> DynArrayIterator<T>::operator - (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> DynArrayIterator<T>::operator - (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DynArrayIterator<T>::operator += (SizeType Diff_)
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
const RandomAccessIterator<T>& DynArrayIterator<T>::operator += (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;

  if (_Index + Diff_ < _ArrayObjPtr->RunLength())
    LocalThis_->_Index += Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& DynArrayIterator<T>::operator -= (SizeType Diff_)
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
const RandomAccessIterator<T>& DynArrayIterator<T>::operator -= (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  DynArrayIterator<T>* const LocalThis_ = (DynArrayIterator<T>* const)this;

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
DynArrayIterator<T>& DynArrayIterator<T>::SetData(const T& Obj_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index] = Obj_;

  return *this;
}

/****************************************************************************/
template <class T>
DynArrayIterator<T>& DynArrayIterator<T>::Erase()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index] = T();

  return *this;
}

/****************************************************************************/
template <class T>
DynArrayIterator<T>& DynArrayIterator<T>::operator = (T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd() && Ptr_)
    (*_ArrayVarPtr)[_Index] = *Ptr_;

  return *this;
}

/****************************************************************************/
template <class T>
DynArrayIterator<T>& DynArrayIterator<T>::operator = (const DynArrayIterator<T>& Rhs_)
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





