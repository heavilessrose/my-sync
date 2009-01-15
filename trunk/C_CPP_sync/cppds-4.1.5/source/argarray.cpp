#ifndef ARGARRAY_CPP
#define ARGARRAY_CPP
#ifndef ARGARRAY_H
  #include "argarray.h"
#endif

/****************************************************************************/
template <class T>
T& ArgSubArray<T>::DUMMY()
{
  ARXMARKER("Start: ArgSubArray<T>::DUMMY()", ARRAY_ERRSTREAM)

  static T _DUMMY;
  _DUMMY = T();

  ARXMARKER("End: ArgSubArray<T>::DUMMY()", ARRAY_ERRSTREAM)
  return _DUMMY;
}

/****************************************************************************/
template <class T>
ArgSubArray<T>::ArgSubArray(const T* Parr_, size_t NumArgs_):
TypedSubArray<T>(NumArgs_, NumArgs_)
{
  ARXMARKER("Start: ArgSubArray<T>::ArgSubArray(const T*, size_t)", ARRAY_ERRSTREAM)

  if (Parr_ == NULL)
    Xinv();

  for (size_t Index_ = 0; Index_ < NumArgs_; Index_++)
    Replace(Index_, &Parr_[Index_]);

  ARXMARKER("End: ArgSubArray<T>::ArgSubArray(const T*, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgSubArray<T>::ArgSubArray(const T& Obj_, size_t NumArgs_):
TypedSubArray<T>(NumArgs_, NumArgs_)
{
  ARXMARKER("Start: ArgSubArray<T>::ArgSubArray(const T&, size_t)", ARRAY_ERRSTREAM)

  for (size_t Index_ = 0; Index_ < NumArgs_; Index_++)
    Replace(Index_, &Obj_);

  ARXMARKER("End: ArgSubArray<T>::ArgSubArray(const T&, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgSubArray<T>::ArgSubArray(size_t NumArgs_, size_t ThisIndex_, int):
TypedSubArray<T>(NumArgs_, ThisIndex_)
{
  ARXMARKER("Start/End: ArgSubArray<T>::ArgSubArray(size_t, size_t, int)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgSubArray<T>::ArgSubArray(const TypedSubArray<T>& Obj_):
TypedSubArray<T>(Obj_)
{
  ARXMARKER("Start: ArgSubArray<T>::ArgSubArray(const TypedSubArray<T>&)", ARRAY_ERRSTREAM)

  InitSubArray(Obj_, GetIndex());

  ARXMARKER("End: ArgSubArray<T>::ArgSubArray(const TypedSubArray<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
template <class T>
ArgSubArray<T>::ArgSubArray(const ArgSubArray<T>& Obj_):
TypedSubArray<T>(*((TypedSubArray<T>*)&Obj_))
{
  ARXMARKER("Start: ArgSubArray<T>::ArgSubArray(const ArgSubArray<T>&)", ARRAY_ERRSTREAM)

  InitSubArray(*((TypedSubArray<T>*)&Obj_), GetIndex());

  ARXMARKER("End: ArgSubArray<T>::ArgSubArray(const ArgSubArray<T>&)", ARRAY_ERRSTREAM)
}
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SubArray* ArgSubArray<T>::NewCopy() const
{
  ARXMARKER("Start/End: ArgSubArray<T>::NewCopy() const", ARRAY_ERRSTREAM)
  return (new ArgSubArray<T>(*((const TypedSubArray<T>*)this)));
}

/****************************************************************************/
template <class T>
SubArray* ArgSubArray<T>::NewExtension(size_t Size_, size_t Index_)
{
  ARXMARKER("Start: ArgSubArray<T>::NewExtension(size_t, size_t)", ARRAY_ERRSTREAM)

  SubArray* Ext_ = new ArgSubArray<T>(Size_, Index_, 0);

  if (Ext_)
  {
    Ext_->InitPointers(GetBase(), this);
    Ext_->InitCounters();
  }

  ARXMARKER("End: ArgSubArray<T>::NewExtension(size_t, size_t)", ARRAY_ERRSTREAM)
  return Ext_;
}

/****************************************************************************/
template <class T>
T* ArgSubArray<T>::Replace(size_t Index_, const T* Ptr_, Boolean Valid_)
{
  ARXMARKER("Start: ArgSubArray<T>::Replace(size_t, const T*, Boolean)", ARRAY_ERRSTREAM)

  PtrVector::DATA_OBJECT Dobj_(Allocate(Ptr_), TypedObjectDeleter<T>::Make(), SetOwnership(Ptr_, TRUE));
  Assign(Index_, Dobj_, Valid_);

  ARXMARKER("End: ArgSubArray<T>::Replace(size_t, const T*, Boolean)", ARRAY_ERRSTREAM)
  return ((T*)Dobj_.Pointee());
}

/****************************************************************************/
template <class T>
T* ArgSubArray<T>::Allocate(const T* Ptr_)
{
  ARXMARKER("Start: ArgSubArray<T>::Allocate(const T*)", ARRAY_ERRSTREAM)

  if (Ptr_)
  {
    ARXMARKER("End(1): ArgSubArray<T>::Allocate(const T*)", ARRAY_ERRSTREAM)
    return ::Clone(*Ptr_);
  }

  ARXMARKER("End(2): ArgSubArray<T>::Allocate(const T*)", ARRAY_ERRSTREAM)
  return ((T*)NULL);
}

/****************************************************************************/
template <class T>
Boolean ArgSubArray<T>::SetOwnership(const T* Ptr_, Boolean IsOwner_)
{
  ARXMARKER("Start: ArgSubArray<T>::SetOwnership(const T*, Boolean)", ARRAY_ERRSTREAM)
  return (Ptr_ != NULL);
}

/****************************************************************************/
template <class T>
PtrVector::DATA_OBJECT ArgSubArray<T>::MakeDataObject(PtrVector::DATA_OBJECT Trg_, PtrVector::DATA_OBJECT Src_)
{
  ARXMARKER("Start: ArgSubArray<T>::MakeDataObject(PtrVector::DATA_OBJECT, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)

  T* Pointee_ = (T*)Src_.Pointee();

  Boolean OwnerShip_ = SetOwnership(Pointee_, Src_.HasOwnership());
  T* NewPtr_ = Allocate(Pointee_);
  PtrVector::DATA_OBJECT TempPtr_(NewPtr_, TypedObjectDeleter<T>::Make(), OwnerShip_);
  Trg_ = TempPtr_;

  ARXMARKER("End: ArgSubArray<T>::MakeDataObject(PtrVector::DATA_OBJECT, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)
  return Trg_;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& ArgSubArray<T>::Arg(size_t Index_, const T& Obj_, Boolean Valid_)
{
  ARXMARKER("Start: ArgSubArray<T>::Arg(size_t, const T&, Boolean)", ARRAY_ERRSTREAM)

  T* Ptr_ = Replace(Index_, &Obj_, Valid_);

  ARXMARKER("End: ArgSubArray<T>::Arg(size_t, const T&, Boolean)", ARRAY_ERRSTREAM)
  return ((Ptr_ == NULL) ? DUMMY():*Ptr_);
}

/****************************************************************************/
template <class T>
const T& ArgSubArray<T>::Arg(size_t Index_) const
{
  ARXMARKER("Start: ArgSubArray<T>::Arg(size_t) const", ARRAY_ERRSTREAM)

  T* Ptr_ = Retrieve(Index_);

  ARXMARKER("End: ArgSubArray<T>::Arg(size_t) const", ARRAY_ERRSTREAM)
  return ((Ptr_ == NULL) ? DUMMY():*Ptr_);
}

/****************************************************************************/
template <class T>
T& ArgSubArray<T>::Arg(size_t Index_)
{
  ARXMARKER("Start: ArgSubArray<T>::Arg(size_t)", ARRAY_ERRSTREAM)

  T* Ptr_ = Retrieve(Index_);

  ARXMARKER("Start: ArgSubArray<T>::Arg(size_t)", ARRAY_ERRSTREAM)
  return ((Ptr_ == NULL) ? DUMMY():*Ptr_);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ArgSubArray<T>::operator new (size_t Bytes_)
{
  ARXMARKER("Start/End: ArgSubArray<T>::operator new (size_t)", ARRAY_ERRSTREAM)
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ArgSubArray<T>::operator delete (void* Space_)
{
  ARXMARKER("Start/End: ArgSubArray<T>::operator delete (void*)", ARRAY_ERRSTREAM)
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ArgSubArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ArgSubArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
int(*ArgArray<T>::_CompFunc)(const T&, const T&) = NULL;

/****************************************************************************/
template <class T>
ArgArray<T>::ArgArray():
TypedArray<T>((TypedSubArray<T>*)ArgSubArray<T>::Make(DEFAULT_SIZE, DEFAULT_SIZE, 0),
	      DEFAULT_SIZE, DEFAULT_SIZE)
{
  ARXMARKER("Start: ArgArray<T>::ArgArray()", ARRAY_ERRSTREAM)

  ShrinkTo(0);

  ARXMARKER("End: ArgArray<T>::ArgArray()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgArray<T>::ArgArray(size_t NumArgs_):
TypedArray<T>((TypedSubArray<T>*)ArgSubArray<T>::Make(NumArgs_, NumArgs_, 0),
	      NumArgs_, NumArgs_)
{
  ARXMARKER("Start: ArgArray<T>::ArgArray(size_t)", ARRAY_ERRSTREAM)

  ShrinkTo(0);

  ARXMARKER("End: ArgArray<T>::ArgArray(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgArray<T>::ArgArray(const TypedArray<T>& Obj_):
TypedArray<T>(Obj_, (TypedSubArray<T>*)ArgSubArray<T>::Make(GiveSubArray(Obj_)))
{
  ARXMARKER("Start: ArgArray<T>::ArgArray(const TypedArray<T>&)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: ArgArray<T>::ArgArray(const TypedArray<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgArray<T>::ArgArray(size_t NumArgs_, const T& Obj_):
TypedArray<T>((TypedSubArray<T>*)ArgSubArray<T>::Make(Obj_, NumArgs_),
	      NumArgs_, NumArgs_)
{
  ARXMARKER("Start: ArgArray<T>::ArgArray(size_t, const T&)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: ArgArray<T>::ArgArray(size_t, const T&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgArray<T>::ArgArray(const T* Parr_, size_t Len_):
TypedArray<T>((TypedSubArray<T>*)ArgSubArray<T>::Make(Parr_, Len_),
	      Len_, Len_)
{
  ARXMARKER("Start: ArgArray<T>::ArgArray(const T*, size_t)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: ArgArray<T>::ArgArray(const T*, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
template <class T>
ArgArray<T>::ArgArray(const ArgArray<T>& Obj_):
TypedArray<T>((const TypedArray<T>&)Obj_, (TypedSubArray<T>*)ArgSubArray<T>::Make(GiveSubArray(*((const TypedArray<T>*)&Obj_))))
{
  ARXMARKER("Start: ArgArray<T>::ArgArray(const ArgArray<T>&)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: ArgArray<T>::ArgArray(const ArgArray<T>&)", ARRAY_ERRSTREAM)
}
#endif
/****************************** Swap Functions ******************************/
/****************************************************************************/
// procedure for swapping pointer elements in an array
// PASSED:
//   list : the array of pointer elements
//   index1, index2 : array indexex of elements to be swapped
// CHANGED:
//   the elements in positions index1 and index2 in the array is swapped
//
template <class T>
void ArgArray<T>::Swap(size_t index1, size_t index2)
{
  if (index1 != index2 && (&((*this)[index1]) != &((*this)[index2])))
  {  
    T Temp_ = (*this)[index1]();
    (*this)[index1] = (*this)[index2]();
    (*this)[index2] = Temp_;
  }
}

/*********************** Shell Sort Helper Functions ************************/
/*****************************************************************************
Procedure called by shell sort procedure to swap elements of the array into
sorted order
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
  spread : the gap between elements of the array
*/
template <class T>
void ArgArray<T>::ShellSwap(size_t lo, size_t hi, size_t Spread_, int Order_)
{
  Boolean Done_;
  size_t i;

  do{
    for (i=lo, Done_=TRUE; i<=hi-Spread_; i++)
      if (((*_CompFunc)((*this)[i](), (*this)[i+Spread_]()) * Order_) > 0)
      {
	Swap(i,i+Spread_);
	Done_ = FALSE;
      }
  }while (!Done_);
}

/*************************** Shell Sort Procedure ***************************/
/*****************************************************************************
procedure for shell sort
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  uses shellswap to swap elements of the array into sorted order
CHANGED:
  the array is sorted into order
*/
template <class T>
void ArgArray<T>::ShellSort(size_t lo, size_t hi, int Order_)
{
  size_t Spread_;	// gap between array elements

  for (Spread_=(hi-lo+1)/2;Spread_;)
  {
    ShellSwap(lo, hi, Spread_, Order_);
    Spread_ /= 2;
  }
}

/****************************************************************************/
template <class T>
void ArgArray<T>::SetComparisonFunction(int(*CmpFnc_)(const T&, const T&))
{
  _CompFunc = CmpFnc_;
}

/****************************************************************************/
template <class T>
int ArgArray<T>::Compare(const ArgArray<T>& Arr_) const
{
  if (_CompFunc && (RunLength() == Arr_.RunLength()))
  {
    size_t x, Max_;
    int Result_ = 0;
    
    for (x = 0, Max_ = RunLength(); x < Max_ && !Result_; x++)
      Result_ = (*_CompFunc)((*this)[x], Arr_[x]);

    return Result_;
  }

  Xran();
  return 0;
}

/****************************************************************************/
template <class T>
SimpleArray<int> ArgArray<T>::SeriesArrayCompare(const ArgArray<T>& Arr_) const
{
  if (_CompFunc && (RunLength() == Arr_.RunLength()))
  {
    SimpleArray<int> Result_(RunLength());
    size_t x, Max_;
    
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
    for (x = 0, Max_ = RunLength(); x < Max_; x++)
    {
      if ((*this)[x].IsVoid() || Arr_[x].IsVoid())
        (Result_.Base())[x] = INT_MAX;      
      else
        (Result_.Base())[x] = (*_CompFunc)((*this)[x](), Arr_[x]());
    }
#else
    for (x = 0, Max_ = RunLength(); x < Max_; x++)
    {
      if ((*this)[x].IsVoid() || Arr_[x].IsVoid())
        Result_[x] = INT_MAX;      
      else
        Result_[x] = (*_CompFunc)((*this)[x](), Arr_[x]());
    }
#endif

    return Result_;
  }

  SimpleArray<int> Temp_;
  Temp_.SetError(TRUE);
  return Temp_;
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::IsEqualToArgArray(const ArgArray<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (Compare(*Ptr_) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::IsLesserToArgArray(const ArgArray<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (Compare(*Ptr_) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::IsGreaterToArgArray(const ArgArray<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (Compare(*Ptr_) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ArgArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToArgArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ArgArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToArgArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  ArgArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToArgArray(TrgComp_);
}

/****************************************************************************/
template <class T>
void ArgArray<T>::Sort(int Order_)
{
  if (_CompFunc)
    ShellSort(0, RunLength() - 1, Order_);
}

/****************************************************************************/
template <class T>
int ArgArray<T>::ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*Fnc_)((*this)[Index_](), Obj_);

  return 0;
}

/****************************************************************************/
template <class T>
int ArgArray<T>::ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_)
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*Fnc_)((*this)[Index_](), Obj_);

  return 0;
}

/****************************************************************************/
template <class T>
int ArgArray<T>::ApplyUnary(int(*Fnc_)(T*))
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*Fnc_)((&((*this)[Index_]))());

  return 0;
}

/****************************************************************************/
template <class T>
int ArgArray<T>::ApplyUnary(T(*Fnc_)(T))
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*this)[Index_] = (*Fnc_)((*this)[Index_]());

  return 0;
}

/****************************************************************************/
template <class T>
ArgArray<T>* ArgArray<T>::Make()
{
  ARXMARKER("Start/End: ArgArray<T>::Make()", ARRAY_ERRSTREAM)
  return (new ArgArray<T>);
}

/****************************************************************************/
template <class T>
ArgArray<T>* ArgArray<T>::Make(size_t NumArgs_)
{
  ARXMARKER("Start/End: ArgArray<T>::Make(size_t)", ARRAY_ERRSTREAM)
  return (new ArgArray<T>(NumArgs_));
}

/****************************************************************************/
template <class T>
ArgArray<T>* ArgArray<T>::Make(const Object& Obj_)
{
  ARXMARKER("Start: ArgArray<T>::Make(const Object&)", ARRAY_ERRSTREAM)

  ArgArrayAcceptor<T> TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  ARXMARKER("End: ArgArray<T>::Make(const Object&)", ARRAY_ERRSTREAM)
  return TrgComp_._VarPtr;
}

/****************************************************************************/
template <class T>
ArgArray<T>* ArgArray<T>::Make(size_t Rep_, const T& Obj_)
{
  ARXMARKER("Start/End: ArgArray<T>::Make(size_t, const T&)", ARRAY_ERRSTREAM)
  return (new ArgArray<T>(Rep_, Obj_));
}

/****************************************************************************/
template <class T>
ArgArray<T>* ArgArray<T>::Make(const T* Parr_, size_t Len_)
{
  ARXMARKER("Start/End: ArgArray<T>::Make(const T*, size_t)", ARRAY_ERRSTREAM)
  return (new ArgArray<T>(Parr_, Len_));
}

/****************************************************************************/
template <class T>
const ArgArray<T>* ArgArray<T>::MakeConst(const Object& Obj_)
{
  ARXMARKER("Start: ArgArray<T>::MakeConst(const Object&)", ARRAY_ERRSTREAM)

  ArgArrayAcceptor<T> TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  ARXMARKER("End: ArgArray<T>::MakeConst(const Object&)", ARRAY_ERRSTREAM)
  return TrgComp_._ObjPtr;
}

/****************************************************************************/
template <class T>
const ArgArray<T>* ArgArray<T>::MakeConst(size_t Rep_, const T& Obj_)
{
  ARXMARKER("Start/End: ArgArray<T>::MakeConst(size_t, const T&)", ARRAY_ERRSTREAM)
  return Make(Rep_, Obj_);
}

/****************************************************************************/
template <class T>
const ArgArray<T>* ArgArray<T>::MakeConst(const T* Parr_, size_t Len_)
{
  ARXMARKER("Start/End: ArgArray<T>::MakeConst(const T*, size_t)", ARRAY_ERRSTREAM)
  return Make(Parr_, Len_);
}

/****************************************************************************/
template <class T>
Object& ArgArray<T>::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  ArgArrayAcceptor<T> TrgComp_(this);
  return *((Object*) (ObjComp_->AssignFromArgArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& ArgArray<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  ArgArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromArgArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* ArgArray<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new ArgArray<T>(*((const TypedArray<T>*)this)));

  ArgArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromArgArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
const Object* ArgArray<T>::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (!ObjComp_)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
Object* ArgArray<T>::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (!ObjComp_)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static ArgArray<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& ArgArray<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ArgArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& ArgArray<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ArgArray<T>::GiveNullObject();
}

/****************************************************************************/
// Returns the total size of all the data within the container structure
// excluding formatting and class ID information.
//
template <class T>
long ArgArray<T>::DataSize() const
{
  long DataSize_ = 0;

  for (size_t Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
    if (!(*this)[Index_].IsVoid())
      DataSize_ += SizeOfData((*this)[Index_]());

  return DataSize_;
}

/****************************************************************************/
template <class T>
long ArgArray<T>::StorableFormWidth() const
{
  // First data item is the ID code for the data structure
  long ObjSize_ = sizeof(long);

  // Second data item is the ID of the object
  ObjSize_ += sizeof(long);

  // Third data item is the null object status byte
  ObjSize_ += sizeof(Byte_t);

  // Fourth data item is the actual array size in number of elements
  ObjSize_ += sizeof(long);

  // Each array element is encoded as data comprising a byte to
  // indicate data validity and the actual size of the object
  for (size_t Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
  {
    ObjSize_ += sizeof(Byte_t);
    if (!(*this)[Index_].IsVoid())
      ObjSize_ += sizeof(long) + FormWidthOfObject((*this)[Index_]());
  }

  return ObjSize_;
}

/****************************************************************************/
template <class T>
int ArgArray<T>::ClassID() const
{
  return TypeInfo::ARGARRAY_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* ArgArray<T>::GiveObjectAcceptor() const
{
  ARXMARKER("Start/End: ArgArray<T>::GiveObjectAcceptor() const", ARRAY_ERRSTREAM)
  return (new ArgArrayAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
void ArgArray<T>::IgnoreElement(ByteStream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::IgnoreElement(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

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
    Ignore(Is_, *ObjPtr_, &Done_);

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: ArgArray<T>::IgnoreElement(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void ArgArray<T>::ReadElement(ByteStream& Is_, size_t Index_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::ReadElement(ByteStream&, size_t, Boolean*)", ARRAY_ERRSTREAM)

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
    Read(Is_, *ObjPtr_, &Done_);
    if (IstreamCheck(Is_, Ok_) && Done_)
      (*this)[Index_] = *ObjPtr_;
  }

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: ArgArray<T>::ReadElement(ByteStream&, size_t, Boolean*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void ArgArray<T>::WriteElement(ByteStream& Os_, size_t Index_, Boolean* Ok_) const
{
  ARXMARKER("Start: ArgArray<T>::WriteElement(ByteStream&, size_t, Boolean*) const", ARRAY_ERRSTREAM)

  const T* ObjPtr_ = &((*this)[Index_]);
  long Code_ = WhatIs(*ObjPtr_);
  Boolean WrOk_ = OstreamCheck(Os_.owrite((char*)(&Code_), sizeof(long)), Ok_);

  if (WrOk_)
  {
    Write(Os_, *ObjPtr_, Ok_);
    OstreamCheck(Os_, Ok_);
  }

  ARXMARKER("End: ArgArray<T>::WriteElement(ByteStream&, size_t, Boolean*) const", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void ArgArray<T>::IgnoreElement(istream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::IgnoreElement(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

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
    Ignore(Is_, *ObjPtr_, &Done_);

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: ArgArray<T>::IgnoreElement(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void ArgArray<T>::ReadElement(istream& Is_, size_t Index_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::ReadElement(ByteStream&, size_t, Boolean*)", ARRAY_ERRSTREAM)

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
    Read(Is_, *ObjPtr_, &Done_);
    if (IstreamCheck(Is_, Ok_) && Done_)
      (*this)[Index_] = *ObjPtr_;
  }

  if (!Match_)
    ::Delete(ObjPtr_);

  SetIOstate(Done_, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: ArgArray<T>::ReadElement(ByteStream&, size_t, Boolean*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void ArgArray<T>::WriteElement(ostream& Os_, size_t Index_, Boolean* Ok_) const
{
  ARXMARKER("Start: ArgArray<T>::WriteElement(ByteStream&, size_t, Boolean*) const", ARRAY_ERRSTREAM)

  const T* ObjPtr_ = &((*this)[Index_]);
  long Code_ = WhatIs(*ObjPtr_);
  Boolean WrOk_ = OstreamCheck(Os_.write((char*)(&Code_), sizeof(long)), Ok_);

  if (WrOk_)
  {
    Write(Os_, *ObjPtr_, Ok_);
    OstreamCheck(Os_, Ok_);
  }

  ARXMARKER("Start: ArgArray<T>::WriteElement(ByteStream&, size_t, Boolean*) const", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
istream& ArgArray<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::BinaryIgnore(istream&, Boolean*)", ARRAY_ERRSTREAM)

  Boolean RdChk_;
  long Size_, Index_;
  Byte_t Valid_;

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
	Is_.read((char*)(&Valid_), sizeof(Byte_t));

	if (IstreamCheck(Is_, Ok_) && Valid_)
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

  ARXMARKER("End: ArgArray<T>::BinaryIgnore(istream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
istream& ArgArray<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::BinaryRead(istream&, Boolean*)", ARRAY_ERRSTREAM)

  Boolean RdChk_;
  long Size_, Index_;
  Byte_t Valid_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Is_.read((char*)(&Size_), sizeof(long));

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	Is_.read((char*)(&Valid_), sizeof(Byte_t));

	if (IstreamCheck(Is_, Ok_) && Valid_)
	  ReadElement(Is_, Index_, Ok_);
	else
	{
	  (*this)[Index_].Erase();
	  IstreamCheck(Is_, Ok_);
	}

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }

    ShrinkTo(Size_);
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: ArgArray<T>::BinaryRead(istream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& ArgArray<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  ARXMARKER("Start: ArgArray<T>::BinaryWrite(ostream&, Boolean*)", ARRAY_ERRSTREAM)

  Boolean RdChk_;
  long RunLength_, Index_;
  Byte_t Valid_;

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
	Valid_ = !(*this)[Index_].IsVoid();
	Os_.write((char*)(&Valid_), sizeof(Byte_t));

	if (OstreamCheck(Os_, Ok_) && Valid_)
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

  ARXMARKER("End: ArgArray<T>::BinaryWrite(ostream&, Boolean*)", ARRAY_ERRSTREAM)
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& ArgArray<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::BinaryIgnore(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

  Boolean RdChk_;
  long Size_, Index_;
  Byte_t Valid_;

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
	Is_.iread((char*)(&Valid_), sizeof(Byte_t));

	if (IstreamCheck(Is_, Ok_) && Valid_)
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

  ARXMARKER("End: ArgArray<T>::BinaryIgnore(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& ArgArray<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: ArgArray<T>::BinaryRead(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

  Boolean RdChk_;
  long Size_, Index_;
  Byte_t Valid_;

  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Is_.iread((char*)(&Size_), sizeof(long));

    if (IstreamCheck(Is_, Ok_))
    {
      for (Index_ = 0; Index_ < Size_; Index_++)
      {
	Is_.iread((char*)(&Valid_), sizeof(Byte_t));

	if (IstreamCheck(Is_, Ok_) && Valid_)
	  ReadElement(Is_, Index_, Ok_);
	else
	{
	  (*this)[Index_].Erase();
	  IstreamCheck(Is_, Ok_);
	}

	if (IsIOError())
	  break;
      }

      if (!IsIOError())
	SetIOstate(TRUE, IO_CLEAR, TRUE);
    }

    ShrinkTo(Size_);
  }

  SetIOstate(FALSE, IO_NOTDONE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: ArgArray<T>::BinaryRead(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& ArgArray<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  ARXMARKER("Start: ArgArray<T>::BinaryWrite(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

  Boolean RdChk_;
  long RunLength_, Index_;
  Byte_t Valid_;

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
	Valid_ = !(*this)[Index_].IsVoid();
	Os_.owrite((char*)(&Valid_), sizeof(Byte_t));

	if (OstreamCheck(Os_, Ok_) && Valid_)
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

  ARXMARKER("End: ArgArray<T>::BinaryWrite(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
  return Os_;
}

/****************************************************************************/
template <class T>
ostream& ArgArray<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {    
    if ((*this)[i].IsVoid())
      Os_ <<"VOID";
    else
      WriteToOutput(Os_, (*this)[i]());
      
    Os_ <<" ";
  }

  return Os_;
}

/****************************************************************************/
/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
template <class T>
ArgArray<T>& ArgArray<T>::operator = (const ArgArray<T>& That_)
{
  return (ArgArray<T>::operator = (*((const TypedArray<T>*)&That_)));
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::operator += (const ArgArray<T>& That_)
{
  return (ArgArray<T>::operator += (*((const TypedArray<T>*)&That_)));
}
#endif
/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::operator = (const TypedArray<T>& That_)
{
  ARXMARKER("Start: ArgArray<T>::operator = (const TypedArray<T>&)", ARRAY_ERRSTREAM)

  if (this != &That_)
  {
    size_t Index_, Max_;

    for (Index_ = 0, Max_ = That_.RunLength(); Index_ < Max_; Index_++)
      (*this)[Index_].ConstCopy(That_, Index_);

    VoidArray::operator = ((const VoidArray&)That_);
    ShrinkTo(Max_);
  }

  ARXMARKER("End: ArgArray<T>::operator = (const TypedArray<T>&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::operator += (const TypedArray<T>& That_)
{
  ARXMARKER("Start: ArgArray<T>::operator += (const TypedArray<T>&)", ARRAY_ERRSTREAM)

  size_t Lindex_, Rindex_, Max_, DestMax_;
  Max_ = Lindex_ = RunLength();
  DestMax_ = That_.RunLength();

  size_t RemSz_ = MaxSize() - Max_;
  if (DestMax_ >= RemSz_)
    Xlen();

  Rindex_ = 0;
  for (Max_ += DestMax_; Lindex_ < Max_; Lindex_++, Rindex_++)
    (*this)[Lindex_].ConstCopy(That_, Rindex_);

  ARXMARKER("End: ArgArray<T>::operator += (const TypedArray<T>&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::operator += (const T& Obj_)
{
  ARXMARKER("Start: ArgArray<T>::operator += (const T&)", ARRAY_ERRSTREAM)

  if (RunLength() == MaxSize())
    Xlen();

  (*this)[RunLength()] = Obj_;

  ARXMARKER("End: ArgArray<T>::operator += (const T&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::Append(const T& Obj_, size_t Rep_)
{
  ARXMARKER("Start: ArgArray<T>::Append(const T&, size_t)", ARRAY_ERRSTREAM)

  size_t Index_, Max_;
  Max_ = Index_ = RunLength();

  size_t RemSz_ = MaxSize() - Max_;
  if (Rep_ >= RemSz_)
    Xlen();

  for (Max_ += Rep_; Index_ < Max_; Index_++)
    (*this)[Index_] = Obj_;

  ARXMARKER("End: ArgArray<T>::Append(const T&, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::Append(const T* Parr_, size_t Len_)
{
  ARXMARKER("Start: ArgArray<T>::Append(const T*, size_t)", ARRAY_ERRSTREAM)

  size_t Lindex_, Rindex_, Max_;
  Max_ = Lindex_ = RunLength();

  size_t RemSz_ = MaxSize() - Max_;
  if (Len_ >= RemSz_)
    Xlen();

  if (Parr_ == NULL && Len_ > 0)
    Xinv();

  Rindex_ = 0;
  for (Max_ += Len_; Lindex_ < Max_; Lindex_++, Rindex_++)
    (*this)[Lindex_] = Parr_[Rindex_];

  ARXMARKER("End: ArgArray<T>::Append(const T*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::AssignArg(const T& Obj_, size_t Rep_)
{
  ARXMARKER("Start: ArgArray<T>::AssignArg(const T&, size_t)", ARRAY_ERRSTREAM)

  size_t Index_;

  if (Rep_ >= MaxSize())
    Xlen();

  for (Index_ = 0; Index_ < Rep_; Index_++)
    (*this)[Index_] = Obj_;

  ShrinkTo(Rep_);

  ARXMARKER("End: ArgArray<T>::AssignArg(const T&, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::AssignArg(const T* Parr_, size_t Len_)
{
  ARXMARKER("Start: ArgArray<T>::AssignArg(const T*, size_t)", ARRAY_ERRSTREAM)

  size_t Index_;

  if (Len_ >= MaxSize())
    Xlen();

  if (Parr_ == NULL && Len_ > 0)
    Xinv();

  for (Index_ = 0; Index_ < Len_; Index_++)
    (*this)[Index_] = Parr_[Index_];

  ShrinkTo(Len_);

  ARXMARKER("End: ArgArray<T>::AssignArg(const T*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::Insert(size_t InsPos_, const ArgArray<T>& Arr_)
{
  ARXMARKER("Start: ArgArray<T>::Insert(size_t, const ArgArray<T>&)", ARRAY_ERRSTREAM)

  Boolean Identical_ = (this == &Arr_);
  TypedArray<T>* ArrPtr_ = NULL;
  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;

  ArrPtr_ = Identical_ ? (ArgArray<T>*)Arr_.Clone():((ArgArray<T>*)&Arr_);

  size_t Len_ = ArrPtr_->RunLength();
  size_t Index_ = RunLength() - 1;
  size_t Next_ = Index_ + Len_;
  size_t Pos_ = InsPos_;

  // Get remainder values
  size_t RemSz_ = MaxSize() - RunLength();
  size_t RemPos_ = MaxSize() - Pos_;

  // test for array limit overflow
  if (Len_ >= RemSz_ || Len_ >= RemPos_)
    Xlen();

  if (Pos_ <= Index_)
  {
    for (;Pos_ < Index_; Index_--, Next_--)
      (*this)[Next_].Copy(*ThisPtr_, Index_);
    (*this)[Next_].Copy(*ThisPtr_, Index_);
  }

  Index_ = 0;
  Next_ = Pos_;

  for (;Index_ < Len_; Index_++, Next_++)
    (*this)[Next_].Copy(*ArrPtr_, Index_);

  if (Identical_)
    delete ArrPtr_;

  ARXMARKER("End: ArgArray<T>::Insert(size_t, const ArgArray<T>&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::Insert(size_t InsPos_, const T& Obj_, size_t Rep_)
{
  ARXMARKER("Start: ArgArray<T>::Insert(size_t, const T&, size_t)", ARRAY_ERRSTREAM)

  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;
  size_t Index_ = RunLength() - 1;
  size_t Next_ = Index_ + Rep_;
  size_t Pos_ = InsPos_;

  // Get remainder values
  size_t RemSz_ = MaxSize() - RunLength();
  size_t RemPos_ = MaxSize() - Pos_;

  // test for array limit overflow
  if (Rep_ >= RemSz_ || Rep_ >= RemPos_)
    Xlen();

  if (Pos_ <= Index_)
  {
    for (;Pos_ < Index_; Index_--, Next_--)
      (*this)[Next_].Copy(*ThisPtr_, Index_);
    (*this)[Next_].Copy(*ThisPtr_, Index_);
  }

  Index_ = 0;
  Next_ = Pos_;

  for (;Index_ < Rep_; Index_++, Next_++)
    (*this)[Next_] = Obj_;

  ARXMARKER("End: ArgArray<T>::Insert(size_t, const T&, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::Insert(size_t InsPos_, const T* Parr_, size_t Len_)
{
  ARXMARKER("Start: ArgArray<T>::Insert(size_t, const T*, size_t)", ARRAY_ERRSTREAM)

  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;
  size_t Index_ = RunLength() - 1;
  size_t Next_ = Index_ + Len_;
  size_t Pos_ = InsPos_;

  // Get remainder values
  size_t RemSz_ = MaxSize() - RunLength();
  size_t RemPos_ = MaxSize() - Pos_;

  if (Len_ >= RemSz_ || Len_ >= RemPos_)
    Xlen();

  if (Parr_ == NULL && Len_ > 0)
    Xinv();

  if (Pos_ <= Index_)
  {
    for (;Pos_ < Index_; Index_--, Next_--)
      (*this)[Next_].Copy(*ThisPtr_, Index_);
    (*this)[Next_].Copy(*ThisPtr_, Index_);
  }

  Index_ = 0;
  Next_ = Pos_;

  for (;Index_ < Len_; Index_++, Next_++)
    (*this)[Next_] = Parr_[Index_];

  ARXMARKER("End: ArgArray<T>::Insert(size_t, const T*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::Remove(size_t Pos_, size_t Len_)
{
  ARXMARKER("Start: ArgArray<T>::Remove(size_t, size_t)", ARRAY_ERRSTREAM)

  if (Pos_ >= RunLength())
    Xran();

  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;
  size_t Remain_ = RunLength() - Pos_;
  size_t Xlen_ = (Len_ < Remain_) ? Len_:Remain_;

  if (Xlen_ < Len_ || Len_ == Remain_)
    Shrink(Xlen_);
  else
  {
    size_t Next_ = Pos_ + Xlen_;
    size_t Max_ = RunLength();

    for (;Next_ < Max_; Pos_++, Next_++)
      (*this)[Pos_].Copy(*ThisPtr_, Next_);

    ShrinkTo(Pos_);
  }

  ARXMARKER("End: ArgArray<T>::Remove(size_t, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray<T>& ArgArray<T>::Sub_Array(ArgArray<T>& Arr_, size_t Pos_, size_t Len_) const
{
  ARXMARKER("Start: ArgArray<T>::Sub_Array(ArgArray<T>&, size_t, size_t) const", ARRAY_ERRSTREAM)

  if (Pos_ >= RunLength())
    Xran();

  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;
  size_t Remain_ = RunLength() - Pos_;
  size_t Xlen_ = (Len_ < Remain_) ? Len_:Remain_;
  size_t Index_ = 0;

  for (size_t Max_ = Pos_ + Xlen_; Pos_ < Max_; Pos_++, Index_++)
    Arr_[Index_].ConstCopy(*ThisPtr_, Pos_);

  Arr_.ShrinkTo(Xlen_);

  ARXMARKER("End: ArgArray<T>::Sub_Array(ArgArray<T>&, size_t, size_t) const", ARRAY_ERRSTREAM)
  return Arr_;
}

/****************************************************************************/
template <class T>
ArgArray<T> ArgArray<T>::Sub_Array(size_t Pos_, size_t Len_) const
{
  ARXMARKER("Start: ArgArray<T>::Sub_Array(size_t, size_t) const", ARRAY_ERRSTREAM)

  ArgArray<T> NewArr_;

  ARXMARKER("End: ArgArray<T>::Sub_Array(size_t, size_t) const", ARRAY_ERRSTREAM)
  return Sub_Array(NewArr_, Pos_, Len_);
}

/****************************************************************************/
template <class T>
void ArgArray<T>::Swap(ArgArray<T>& Arr_)
{
  ARXMARKER("Start: ArgArray<T>::Swap(ArgArray<T>&)", ARRAY_ERRSTREAM)

  ArgArray<T> Temp_(*this);
  *this = Arr_;
  Arr_ = Temp_;

  ARXMARKER("End: ArgArray<T>::Swap(ArgArray<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
const T& ArgArray<T>::Get_At(size_t Pos_, Boolean& Valid_) const
{
  ARXMARKER("Start: ArgArray<T>::Get_At(size_t, Boolean&) const", ARRAY_ERRSTREAM)

  Valid_ = !(Limit() && (Pos_ >= Limit() || Pos_ >= RunLength()));

  if (!Valid_)
  {
    ARXMARKER("End(1): ArgArray<T>::Get_At(size_t, Boolean&) const", ARRAY_ERRSTREAM)
    return ArgSubArray<T>::DUMMY();
  }

  const ArgArrayRef<T> Ref_(_Head->Index(0, Pos_));

  ARXMARKER("End(2): ArgArray<T>::Get_At(size_t, Boolean&) const", ARRAY_ERRSTREAM)
  return ((const T&)Ref_);
}

/****************************************************************************/
template <class T>
T& ArgArray<T>::Get_At(size_t Pos_, Boolean& Valid_)
{
  ARXMARKER("Start: ArgArray<T>::Get_At(size_t, Boolean&)", ARRAY_ERRSTREAM)

  Valid_ = !(Limit() && (Pos_ >= Limit() || Pos_ >= RunLength()));

  if (!Valid_)
  {
    ARXMARKER("End(1): ArgArray<T>::Get_At(size_t, Boolean&)", ARRAY_ERRSTREAM)
    return ArgSubArray<T>::DUMMY();
  }

  ArgArrayRef<T> Ref_(_Head->Index(0, Pos_));

  ARXMARKER("End(2): ArgArray<T>::Get_At(size_t, Boolean&)", ARRAY_ERRSTREAM)
  return ((T&)Ref_);
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::Put_At(size_t Pos_, const T& Obj_)
{
  ARXMARKER("Start: ArgArray<T>::Put_At(size_t, const T&)", ARRAY_ERRSTREAM)

  if (Limit() && (Pos_ >= Limit() || Pos_ >= Total()))
  {
    ARXMARKER("End(1): ArgArray<T>::Put_At(size_t, const T&)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  ArgArrayRef<T> Ref_(_Head->Index(0, Pos_));
  Ref_ = Obj_;

  ARXMARKER("End(2): ArgArray<T>::Put_At(size_t, const T&)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
void ArgArray<T>::Shrink(size_t Size_)
{
  ARXMARKER("Start: ArgArray<T>::Shrink(size_t)", ARRAY_ERRSTREAM)

  size_t Index_, Count_;
  size_t Decr_ = 0;
  size_t Mid_ = RunLength() / 2;

  if (Size_ <= Mid_)
  {
    for (Index_ = RunLength() - 1, Count_ = 0; Count_ < Size_; ++Count_, --Index_)
      if ((*this)[Index_].IsValid())
	++Decr_;

    if (Decr_)
      DecrementSize(Decr_);
  }

  TypedArray<T>::Shrink(Size_);

  if (Size_ > Mid_)
    FindSize();

  ARXMARKER("End: ArgArray<T>::Shrink(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void ArgArray<T>::ShrinkTo(size_t Size_)
{
  ARXMARKER("Start: ArgArray<T>::ShrinkTo(size_t)", ARRAY_ERRSTREAM)

  size_t Index_, Count_;
  size_t Decr_ = 0;
  size_t Mid_ = RunLength() / 2;
  size_t ShrSize_ = RunLength() - Size_;

  if (ShrSize_ <= Mid_)
  {
    for (Index_ = RunLength() - 1, Count_ = 0; Count_ < ShrSize_; ++Count_, --Index_)
      if ((*this)[Index_].IsValid())
	++Decr_;

    if (Decr_)
      DecrementSize(Decr_);
  }

  TypedArray<T>::ShrinkTo(Size_);

  if (ShrSize_ > Mid_)
    FindSize();

  ARXMARKER("End: ArgArray<T>::ShrinkTo(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::Grow(size_t Size_, const T& Obj_)
{
  ARXMARKER("Start: ArgArray<T>::Grow(size_t, const T&)", ARRAY_ERRSTREAM)

  size_t Index_ = RunLength(),
	 Max_ = RunLength() + Size_;
  size_t RemSz_ = MaxSize() - Index_;

  if (Size_ >= RemSz_)
    Xlen();

  if (Max_ >= MaxSize() || Limit() && Max_ > Limit())
  {
    ARXMARKER("End(1): ArgArray<T>::Grow(size_t, const T&)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  for (;Index_ < Max_; Index_++)
    (*this)[Index_] = Obj_;

  ARXMARKER("End(2): ArgArray<T>::Grow(size_t, const T&)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean ArgArray<T>::GrowTo(size_t Size_, const T& Obj_)
{
  ARXMARKER("Start: ArgArray<T>::GrowTo(size_t, const T&)", ARRAY_ERRSTREAM)

  if (Size_ >= MaxSize() || Limit() && Size_ > Limit())
  {
    ARXMARKER("End(1): ArgArray<T>::GrowTo(size_t, const T&)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  for (size_t Index_ = RunLength(); Index_ < Size_; Index_++)
    (*this)[Index_] = Obj_;

  ARXMARKER("End(2): ArgArray<T>::GrowTo(size_t, const T&)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
ArgArrayRef<T> ArgArray<T>::operator [] (size_t Index_)
{
  ARXMARKER("Start/End: ArgArray<T>::operator [] (size_t)", ARRAY_ERRSTREAM)
  return ArgArrayRef<T>(_Head->Index(0, Index_));
}

/****************************************************************************/
template <class T>
const ArgArrayRef<T> ArgArray<T>::operator [] (size_t Index_) const
{
  ARXMARKER("Start/End: ArgArray<T>::operator [] (size_t) const", ARRAY_ERRSTREAM)
  return ((const ArgArrayRef<T>)(_Head->Index(0, Index_)));
}

/****************************************************************************/
template <class T>
T* ArgArray<T>::ToPtrArray(T* Parr_, size_t Len_)
{
  ARXMARKER("Start: ArgArray<T>::ToPtrArray(T*, size_t)", ARRAY_ERRSTREAM)

  if (Parr_)
  {
    if (Len_ == 0)
      Len_ = RunLength();

    for (size_t Index_ = 0; Index_ < Len_; Index_++)
      Parr_[Index_] = (*this)[Index_]();
  }

  ARXMARKER("End: ArgArray<T>::ToPtrArray(T*, size_t)", ARRAY_ERRSTREAM)
  return (Len_ ? Parr_:NULL);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ArgArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ArgArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ArgArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ArgArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
ArgArrayAcceptor<T>::ArgArrayAcceptor(const ArgArray<T>* Ptr_):
_ObjPtr(Ptr_)
{
  ARXMARKER("Start/End: ArgArrayAcceptor<T>::ArgArrayAcceptor(const ArgArray<T>*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
Boolean ArgArrayAcceptor<T>::IsTypedArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean ArgArrayAcceptor<T>::IsArgArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void ArgArrayAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const ArgArray<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* ArgArrayAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
void* ArgArrayAcceptor<T>::CreateFromArgArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: ArgArrayAcceptor<T>::CreateArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  _VarPtr = new ArgArray<T>(*((const ArgArray<T>*)Obj_.GiveDataPtr()));

  ARXMARKER("End: ArgArrayAcceptor<T>::CreateArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* ArgArrayAcceptor<T>::AssignFromArgArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: ArgArrayAcceptor<T>::AssignArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  const ArgArray<T>* TrgPtr_ = (const ArgArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;

  ARXMARKER("End: ArgArrayAcceptor<T>::AssignArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* ArgArrayAcceptor<T>::CreateFromPtrArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: ArgArrayAcceptor<T>::CreateArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  _VarPtr = new ArgArray<T>(*((const TypedArray<T>*)Obj_.GiveDataPtr()));

  ARXMARKER("End: ArgArrayAcceptor<T>::CreateArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* ArgArrayAcceptor<T>::AssignFromPtrArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: ArgArrayAcceptor<T>::AssignPtrArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  const TypedArray<T>* TrgPtr_ = (const TypedArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;

  ARXMARKER("End: ArgArrayAcceptor<T>::AssignPtrArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean ArgArrayAcceptor<T>::IsEqualToArgArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToArgArray((const ArgArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean ArgArrayAcceptor<T>::IsLesserToArgArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToArgArray((const ArgArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean ArgArrayAcceptor<T>::IsGreaterToArgArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToArgArray((const ArgArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ArgArrayAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ArgArrayAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
ArgArrayRef<T>::ArgArrayRef(const RefInfo& Info_):
TypedArrayRef<T>(Info_)
{
  ARXMARKER("Start/End: ArgArrayRef<T>::ArgArrayRef(const RefInfo&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgSubArray<T>* ArgArrayRef<T>::CastTo_ArgSubArray() const
{
  ARXMARKER("Start: ArgArrayRef<T>::CastTo_ArgSubArray() const", ARRAY_ERRSTREAM)

  TypedSubArray<T>* Ptr_ = ((TypedSubArray<T>*)_BossPtr);

  ARXMARKER("End: ArgArrayRef<T>::CastTo_ArgSubArray() const", ARRAY_ERRSTREAM)
  return ((ArgSubArray<T>*)Ptr_);
}

/****************************************************************************/
template <class T>
ArgArrayRef<T>& ArgArrayRef<T>::operator = (const T& Obj_)
{
  ARXMARKER("Start: ArgArrayRef<T>::operator = (const T&)", ARRAY_ERRSTREAM)

  if (!IsValid())
    IncrementSize(1);

  CastTo_ArgSubArray()->Arg(_ArrayIndex, Obj_);

  ARXMARKER("End: ArgArrayRef<T>::operator = (const T&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArrayRef<T>::operator const T& () const
{
  ARXMARKER("Start/End: ArgArrayRef<T>::operator const T& () const", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
template <class T>
ArgArrayRef<T>::operator T& ()
{
  ARXMARKER("Start/End: ArgArrayRef<T>::operator T& ()", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
template <class T>
ArgArrayRef<T>& ArgArrayRef<T>::Erase()
{
  ARXMARKER("Start: ArgArrayRef<T>::Erase()", ARRAY_ERRSTREAM)

  if (IsValid())
    DecrementSize(1);

  _Modified = TRUE;
  CastTo_ArgSubArray()->Replace(_ArrayIndex, NULL);

  ARXMARKER("End: ArgArrayRef<T>::Erase()", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
Boolean ArgArrayRef<T>::IsVoid() const
{
  ARXMARKER("Start/End: ArgArrayRef<T>::IsVoid()", ARRAY_ERRSTREAM)
  return !IsValid();
}

/****************************************************************************/
template <class T>
ArgArray_PtrProxy<T> ArgArrayRef<T>::operator & ()
{
  ARXMARKER("Start/End: ArgArrayRef<T>::operator & ()", ARRAY_ERRSTREAM)
  return (ArgArray_PtrProxy<T>(*this));
}

/****************************************************************************/
template <class T>
const ArgArray_PtrProxy<T> ArgArrayRef<T>::operator & () const
{
  ARXMARKER("Start/End: ArgArrayRef<T>::operator & () const", ARRAY_ERRSTREAM)
  return ((const ArgArray_PtrProxy<T>)(*this));
}

/****************************************************************************/
template <class T>
T& ArgArrayRef<T>::operator () ()
{
  ARXMARKER("Start/End: ArgArrayRef<T>::operator () ()", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
template <class T>
const T& ArgArrayRef<T>::operator () () const
{
  ARXMARKER("Start/End: ArgArrayRef<T>::operator () () const", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
ArgArray_PtrProxy<T>::ArgArray_PtrProxy(const ArgArrayRef<T>& Info_):
TypedArrayRef<T>((const RefInfo&)Info_)
{
  ARXMARKER("Start/End: ArgArray_PtrProxy<T>::ArgArray_PtrProxy(const ArgArrayRef<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
ArgSubArray<T>* ArgArray_PtrProxy<T>::CastTo_ArgSubArray() const
{
  ARXMARKER("Start: ArgArray_PtrProxy<T>::CastTo_ArgSubArray() const", ARRAY_ERRSTREAM)

  TypedSubArray<T>* Ptr_ = ((TypedSubArray<T>*)_BossPtr);

  ARXMARKER("End: ArgArray_PtrProxy<T>::CastTo_ArgSubArray() const", ARRAY_ERRSTREAM)
  return ((ArgSubArray<T>*)Ptr_);
}

/****************************************************************************/
template <class T>
ArgArray_PtrProxy<T>& ArgArray_PtrProxy<T>::operator = (T* Ptr_)
{
  ARXMARKER("Start: ArgArray_PtrProxy<T>::operator = (T*)", ARRAY_ERRSTREAM)

  _Modified = Ptr_ == NULL;

  if (_Modified)
  {
    if (IsValid())
      DecrementSize(1);
  }
  else
    if (!IsValid())
      IncrementSize(1);

  CastTo_ArgSubArray()->Replace(_ArrayIndex, Ptr_);

  ARXMARKER("End: ArgArray_PtrProxy<T>::operator = (T*)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
ArgArray_PtrProxy<T>::operator T* ()
{
  ARXMARKER("Start/End: ArgArray_PtrProxy<T>::operator T* ()", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Ptr(_ArrayIndex);
}

/****************************************************************************/
template <class T>
ArgArray_PtrProxy<T>::operator const T* () const
{
  ARXMARKER("Start/End: ArgArray_PtrProxy<T>::operator const T* () const", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Ptr(_ArrayIndex);
}

/****************************************************************************/
template <class T>
T& ArgArray_PtrProxy<T>::operator * ()
{
  ARXMARKER("Start: ArgArray_PtrProxy<T>::operator * ()", ARRAY_ERRSTREAM)

  T* Ptr_ = CastTo_ArgSubArray()->Ptr(_ArrayIndex);

  if (Ptr_ == NULL)
    Xnullp();

  ARXMARKER("End: ArgArray_PtrProxy<T>::operator * ()", ARRAY_ERRSTREAM)
  return *Ptr_;
}

/****************************************************************************/
template <class T>
const T& ArgArray_PtrProxy<T>::operator * () const
{
  ARXMARKER("Start: ArgArray_PtrProxy<T>::operator * () const", ARRAY_ERRSTREAM)

  T* Ptr_ = CastTo_ArgSubArray()->Ptr(_ArrayIndex);

  if (Ptr_ == NULL)
    Xnullp();

  ARXMARKER("End: ArgArray_PtrProxy<T>::operator * () const", ARRAY_ERRSTREAM)
  return *Ptr_;
}

/****************************************************************************/
template <class T>
T* ArgArray_PtrProxy<T>::operator () ()
{
  ARXMARKER("Start/End: ArgArray_PtrProxy<T>::operator () ()", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Ptr(_ArrayIndex);
}

/****************************************************************************/
template <class T>
const T* ArgArray_PtrProxy<T>::operator () () const
{
  ARXMARKER("Start/End: ArgArray_PtrProxy<T>::operator () () const", ARRAY_ERRSTREAM)
  return CastTo_ArgSubArray()->Ptr(_ArrayIndex);
}

/****************************************************************************/
template <class T>
Boolean ArgArray_PtrProxy<T>::IsVoid() const
{
  ARXMARKER("Start/End: ArgArray_PtrProxy<T>::IsVoid() const", ARRAY_ERRSTREAM)
  return !IsValid();
}

/****************************************************************************/
/****************************************************************************/
#if ARRAY_IMPLEMENT_STLCONTAINER
template <class T>
ArgArrayContainer<T>::ArgArrayContainer(ArgArray<T>* Ap_):
_ContainerPtr(Ap_)
{}

/****************************************************************************/
template <class T>
ArgArrayContainer<T>::ArgArrayContainer(ArgArrayContainer<T>& Ap_):
_ContainerPtr(Ap_._ContainerPtr)
{}

/****************************************************************************/
template <class T>
const ArgArrayIterator<T> ArgArrayContainer<T>::begin() const
{
  return (ArgArrayIterator<T>(_ContainerPtr));
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T> ArgArrayContainer<T>::begin()
{
  return (ArgArrayIterator<T>(_ContainerPtr));
}

/****************************************************************************/
template <class T>
const ArgArrayIterator<T> ArgArrayContainer<T>::end() const
{
  ArgArrayIterator<T> Iter_(_ContainerPtr);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T> ArgArrayContainer<T>::end()
{
  ArgArrayIterator<T> Iter_(_ContainerPtr);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> ArgArrayContainer<T>::rbegin() const
{
  ArgArrayIterator<T> Iter_(_ContainerPtr);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> ArgArrayContainer<T>::rbegin()
{
  ArgArrayIterator<T> Iter_(_ContainerPtr);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> ArgArrayContainer<T>::rend() const
{
  ArgArrayIterator<T> Iter_(_ContainerPtr);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> ArgArrayContainer<T>::rend()
{
  ArgArrayIterator<T> Iter_(_ContainerPtr);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
T& ArgArrayContainer<T>::front()
{
  return (*begin());
}

/****************************************************************************/
template <class T>
const T& ArgArrayContainer<T>::front() const
{
  return (*begin());
}

/****************************************************************************/
template <class T>
T& ArgArrayContainer<T>::back()
{
  return *(--end());
}

/****************************************************************************/
template <class T>
const T& ArgArrayContainer<T>::back() const
{
  return *(--end());
}

/****************************************************************************/
template <class T>
void ArgArrayContainer<T>::push_back(const T& x)
{
  insert(end(), x);
}

/****************************************************************************/
template <class T>
void ArgArrayContainer<T>::pop_back()
{
  ArgArrayIterator<T> Iter_ = end();
  --Iter_;
  erase(Iter_);
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T> ArgArrayContainer<T>::insert(ArgArrayIterator<T> pos, const T& x)
{
  --pos;

  if (pos.More())
    _ContainerPtr->Insert(pos.Position(), x);

  return pos;
}

/****************************************************************************/
template <class T>
void ArgArrayContainer<T>::insert(ArgArrayIterator<T> pos, size_t n, const T& x)
{
  --pos;

  if (pos.More())
  {
    size_t i;
    for (i = 0; i < n; ++i)
      _ContainerPtr->Insert(pos.Position(), x);
  }
}

/****************************************************************************/
template <class T>
void ArgArrayContainer<T>::insert(ArgArrayIterator<T> pos, const ArgArrayIterator<T> first, const ArgArrayIterator<T> last)
{
  --pos;

  if (pos.More())
    while (first != last && first.More())
    {
      Boolean Valid_;
      T Obj_ = _ContainerPtr->Get_At(first.Position(), Valid_);

      if (Valid_)
	_ContainerPtr->Insert(pos.Position(), Obj_);

      ++first;
    }
}

/****************************************************************************/
template <class T>
void ArgArrayContainer<T>::erase(ArgArrayIterator<T> pos)
{
  if (pos.More())
    (*_ContainerPtr)[pos.Position()].Erase();
}

/****************************************************************************/
template <class T>
void ArgArrayContainer<T>::erase(ArgArrayIterator<T> first, ArgArrayIterator<T> last)
{
  while (first != last && first.More())
  {
    (*_ContainerPtr)[first.Position()].Erase();
    ++first;
  }
}
#endif
/****************************************************************************/
/****************************************************************************/
#if ARRAY_IMPLEMENT_STLITERATOR
template <class T>
ArgArrayIterator<T>::ArgArrayIterator(const ArgArray<T>* Ap_, Boolean DestroyFromIter_):
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
ArgArrayIterator<T>::ArgArrayIterator(const ArgArrayIterator<T>& Ap_):
RandomAccessIterator<T>(),
_ArrayObjPtr(Ap_._ArrayObjPtr),
_Index(Ap_._Index)
{
  if (_ArrayObjPtr)
    _ArrayVarPtr->RegisterIterator();
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T>::~ArgArrayIterator()
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
ArgArrayIterator<T>* ArgArrayIterator<T>::Make(ArgArray<T>* Ap_)
{
  return (new ArgArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T>* ArgArrayIterator<T>::Make(ArgArrayIterator<T>& Ap_)
{
  return (new ArgArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const ArgArrayIterator<T>* ArgArrayIterator<T>::MakeConst(const ArgArray<T>* Ap_)
{
  return (new ArgArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const ArgArrayIterator<T>* ArgArrayIterator<T>::MakeConst(const ArgArrayIterator<T>& Ap_)
{
  return (new ArgArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
int ArgArrayIterator<T>::ClassID() const
{
  return TypeInfo::ARGARRAYITERATOR_TYPE;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>* ArgArrayIterator<T>::Clone()
{
  return (new ArgArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>* ArgArrayIterator<T>::Clone() const
{
  return (new ArgArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
Boolean ArgArrayIterator<T>::IsValid(SizeType Index_) const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (Index_ < _ArrayVarPtr->RunLength())
    return !((*_ArrayVarPtr)[Index_].IsVoid());

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean ArgArrayIterator<T>::IsValid() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return !((*_ArrayVarPtr)[_Index].IsVoid());

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean ArgArrayIterator<T>::More() const
{
  if (_ArrayVarPtr->RunLength() == 0)
  {
    ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;
    LocalThis_->_Index = SizeType(-1);
  }

  return !AtEnd();
}

/****************************************************************************/
template <class T>
void ArgArrayIterator<T>::Reset() const
{
  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;
  LocalThis_->_Index = 0;
}

/****************************************************************************/
template <class T>
void ArgArrayIterator<T>::SetToLast() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;
  LocalThis_->_Index = _ArrayObjPtr->RunLength() - 1;
}

/****************************************************************************/
template <class T>
void ArgArrayIterator<T>::SetToEnd() const
{
  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;
  LocalThis_->_Index = SizeType(-1);
}

/****************************************************************************/
template <class T>
const T* ArgArrayIterator<T>::ConstPointee() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return (&((*_ArrayVarPtr)[_Index]))();
  return (&((*_ArrayVarPtr)[0]))();
}

/****************************************************************************/
template <class T>
const T* ArgArrayIterator<T>::Pointee() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return (&((*_ArrayVarPtr)[_Index]))();
  return (&((*_ArrayVarPtr)[0]))();
}

/****************************************************************************/
template <class T>
T* ArgArrayIterator<T>::Pointee()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return (&((*_ArrayVarPtr)[_Index]))();
  return (&((*_ArrayVarPtr)[0]))();
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& ArgArrayIterator<T>::GotoHead() const
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& ArgArrayIterator<T>::GotoTail() const
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ArgArrayIterator<T>::GotoHead()
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ArgArrayIterator<T>::GotoTail()
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
Boolean ArgArrayIterator<T>::AtHead() const
{
  return (_Index == 0);
}

/****************************************************************************/
template <class T>
Boolean ArgArrayIterator<T>::AtTail() const
{
  return (_Index == _ArrayObjPtr->RunLength() - 1);
}

/****************************************************************************/
template <class T>
T& ArgArrayIterator<T>::operator * ()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return ((*_ArrayVarPtr)[_Index])();
  return ((*_ArrayVarPtr)[0])();
}

/****************************************************************************/
template <class T>
const T& ArgArrayIterator<T>::operator * () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return (*_ArrayObjPtr)[_Index]();
  return (*_ArrayObjPtr)[0]();
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ArgArrayIterator<T>::operator [] (SizeType Index_)
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
const RandomAccessIterator<T>& ArgArrayIterator<T>::operator [] (SizeType Index_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;
  LocalThis_->_Index = Index_;

  if (!AtEnd())
    ((*_ArrayObjPtr)[_Index]);
  else
    ((*_ArrayObjPtr)[0]);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ArgArrayIterator<T>::operator ++ ()
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
RandomAccessIterator<T> ArgArrayIterator<T>::operator ++ (int)
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
const RandomAccessIterator<T>& ArgArrayIterator<T>::operator ++ () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> ArgArrayIterator<T>::operator ++ (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ArgArrayIterator<T>::operator -- ()
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
RandomAccessIterator<T> ArgArrayIterator<T>::operator -- (int)
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
const RandomAccessIterator<T>& ArgArrayIterator<T>::operator -- () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> ArgArrayIterator<T>::operator -- (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> ArgArrayIterator<T>::operator + (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> ArgArrayIterator<T>::operator + (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> ArgArrayIterator<T>::operator - (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> ArgArrayIterator<T>::operator - (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ArgArrayIterator<T>::operator += (SizeType Diff_)
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
const RandomAccessIterator<T>& ArgArrayIterator<T>::operator += (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;

  if (_Index + Diff_ < _ArrayObjPtr->RunLength())
    LocalThis_->_Index += Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& ArgArrayIterator<T>::operator -= (SizeType Diff_)
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
const RandomAccessIterator<T>& ArgArrayIterator<T>::operator -= (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  ArgArrayIterator<T>* const LocalThis_ = (ArgArrayIterator<T>* const)this;

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
ArgArrayIterator<T>& ArgArrayIterator<T>::SetData(const T& Obj_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index] = Obj_;

  return *this;
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T>& ArgArrayIterator<T>::Erase()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index].Erase();

  return *this;
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T>& ArgArrayIterator<T>::operator = (const ArgArrayIterator<T>& Rhs_)
{
  if (this != &Rhs_)
  {
    _Index = Rhs_._Index;
    _ArrayVarPtr = Rhs_._ArrayVarPtr;
  }

  return *this;
}

/****************************************************************************/
template <class T>
ArgArrayIterator<T>& ArgArrayIterator<T>::operator = (T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    &((*_ArrayVarPtr)[_Index]) = Ptr_;

  return *this;
}
#endif
/****************************************************************************/
#endif





