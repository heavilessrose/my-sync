#ifndef PTRARRAY_CPP
#define PTRARRAY_CPP
#ifndef PTRARRAY_H
  #include "ptrarray.h"
#endif

/****************************************************************************/
template <class T>
PtrSubArray<T>::PtrSubArray(T** Parr_, size_t NumArgs_):
TypedSubArray<T>(NumArgs_, NumArgs_)
{
  ARXMARKER("Start: PtrSubArray<T>::PtrSubArray(T**, size_t)", ARRAY_ERRSTREAM)

  if (Parr_ == NULL)
    Xinv();

  for (size_t Index_ = 0; Index_ < NumArgs_; Index_++)
    Replace(Index_, Parr_[Index_]);

  ARXMARKER("End: PtrSubArray<T>::PtrSubArray(T**, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrSubArray<T>::PtrSubArray(PtrVector::DATA_OBJECT* Ptr_, size_t NumArgs_):
TypedSubArray<T>(NumArgs_, NumArgs_)
{
  ARXMARKER("Start: PtrSubArray<T>::PtrSubArray(PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)

  if (Ptr_ == NULL)
    Xinv();

  for (size_t Index_ = 0; Index_ < NumArgs_; Index_++)
    Assign(Index_, *Ptr_);

  ARXMARKER("End: PtrSubArray<T>::PtrSubArray(PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrSubArray<T>::PtrSubArray(T* Ptr_, size_t NumArgs_):
TypedSubArray<T>(NumArgs_, NumArgs_)
{
  ARXMARKER("Start: PtrSubArray<T>::PtrSubArray(T*, size_t)", ARRAY_ERRSTREAM)

  for (size_t Index_ = 0; Index_ < NumArgs_; Index_++)
    Replace(Index_, Ptr_);

  ARXMARKER("End: PtrSubArray<T>::PtrSubArray(T*, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrSubArray<T>::PtrSubArray(size_t NumArgs_, size_t ThisIndex_, int):
TypedSubArray<T>(NumArgs_, ThisIndex_)
{
  ARXMARKER("Start/End: PtrSubArray<T>::PtrSubArray(size_t, size_t, int)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrSubArray<T>::PtrSubArray(const TypedSubArray<T>& Obj_):
TypedSubArray<T>(Obj_)
{
  ARXMARKER("Start: PtrSubArray<T>::PtrSubArray(const TypedSubArray<T>&)", ARRAY_ERRSTREAM)

  InitSubArray(Obj_, GetIndex());

  ARXMARKER("End: PtrSubArray<T>::PtrSubArray(const TypedSubArray<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
template <class T>
PtrSubArray<T>::PtrSubArray(const PtrSubArray<T>& Obj_):
TypedSubArray<T>(*((TypedSubArray<T>*)&Obj_))
{
  ARXMARKER("Start: PtrSubArray<T>::PtrSubArray(const PtrSubArray<T>&)", ARRAY_ERRSTREAM)

  InitSubArray(Obj_, GetIndex());

  ARXMARKER("End: PtrSubArray<T>::PtrSubArray(const PtrSubArray<T>&)", ARRAY_ERRSTREAM)
}
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
SubArray* PtrSubArray<T>::NewCopy() const
{
  ARXMARKER("Start/End: PtrSubArray<T>::NewCopy() const", ARRAY_ERRSTREAM)
  return (new PtrSubArray<T>(*((const TypedSubArray<T>*)this)));
}

/****************************************************************************/
template <class T>
SubArray* PtrSubArray<T>::NewExtension(size_t Size_, size_t Index_)
{
  ARXMARKER("Start: PtrSubArray<T>::NewExtension(size_t, size_t)", ARRAY_ERRSTREAM)

  SubArray* Ext_ = new PtrSubArray<T>(Size_, Index_, 0);

  if (Ext_)
  {
    Ext_->InitPointers(GetBase(), this);
    Ext_->InitCounters();
  }

  ARXMARKER("End: PtrSubArray<T>::NewExtension(size_t, size_t)", ARRAY_ERRSTREAM)
  return Ext_;
}

/****************************************************************************/
template <class T>
T* PtrSubArray<T>::Allocate(const T* Ptr_)
{
  ARXMARKER("Start: PtrSubArray<T>::Allocate(const T*)", ARRAY_ERRSTREAM)

  const T* Constp_ = Ptr_;
  if (Ptr_)
  {
    ARXMARKER("End(1): PtrSubArray<T>::Allocate(const T*)", ARRAY_ERRSTREAM)
    return ((T*)Constp_);
  }

  ARXMARKER("End(2): PtrSubArray<T>::Allocate(const T*)", ARRAY_ERRSTREAM)
  return ((T*)NULL);
}

/****************************************************************************/
template <class T>
Boolean PtrSubArray<T>::SetOwnership(const T* Ptr_, Boolean IsOwner_)
{
  ARXMARKER("Start/End: PtrSubArray<T>::SetOwnership(const T*, Boolean)", ARRAY_ERRSTREAM)
  return ((Ptr_ != NULL) ? IsOwner_:FALSE);
}

/****************************************************************************/
template <class T>
T* PtrSubArray<T>::Arg(size_t Index_, T* Ptr_, Boolean Valid_)
{
  ARXMARKER("Start/End: PtrSubArray<T>::Arg(size_t, T*, Boolean)", ARRAY_ERRSTREAM)
  return Replace(Index_, Ptr_, Valid_);
}

/****************************************************************************/
template <class T>
PtrVector::DATA_OBJECT PtrSubArray<T>::MakeDataObject(PtrVector::DATA_OBJECT Trg_, PtrVector::DATA_OBJECT Src_)
{
  ARXMARKER("Start: PtrSubArray<T>::MakeDataObject(PtrVector::DATA_OBJECT, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)

  T* Pointee_ = (T*)Src_.Pointee();

  Boolean OwnerShip_ = SetOwnership(Pointee_, Src_.HasOwnership());
  T* NewPtr_ = Allocate(Pointee_);

  if (NewPtr_ != NULL)
    Trg_ = Src_;
  else
  {
    PtrVector::DATA_OBJECT TempVal_(NewPtr_, TypedObjectDeleter<T>::Make(), OwnerShip_);
    Trg_ = TempVal_;
  }

  ARXMARKER("End: PtrSubArray<T>::MakeDataObject(PtrVector::DATA_OBJECT, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)
  return Trg_;
}

/****************************************************************************/
template <class T>
T* PtrSubArray<T>::Arg(size_t Index_)
{
  ARXMARKER("Start/End: PtrSubArray<T>::Arg(size_t)", ARRAY_ERRSTREAM)
  return Retrieve(Index_);
}

/****************************************************************************/
template <class T>
const T* PtrSubArray<T>::Arg(size_t Index_) const
{
  ARXMARKER("Start/End: PtrSubArray<T>::Arg(size_t) const", ARRAY_ERRSTREAM)
  return Retrieve(Index_);
}

/****************************************************************************/
template <class T>
T* PtrSubArray<T>::Ptr(size_t Index_)
{
  ARXMARKER("Start/End: PtrSubArray<T>::Ptr(size_t)", ARRAY_ERRSTREAM)
  return Retrieve(Index_);
}

/****************************************************************************/
template <class T>
const T* PtrSubArray<T>::Ptr(size_t Index_) const
{
  ARXMARKER("Start/End: PtrSubArray<T>::Ptr(size_t) const", ARRAY_ERRSTREAM)
  return Retrieve(Index_);
}

/****************************************************************************/
template <class T>
T* PtrSubArray<T>::Replace(size_t Index_, const T* Ptr_, Boolean Valid_)
{
  ARXMARKER("Start: PtrSubArray<T>::Replace(size_t, const T*, Boolean)", ARRAY_ERRSTREAM)

  PtrVector::DATA_OBJECT Dobj_(Allocate(Ptr_), TypedObjectDeleter<T>::Make(), FALSE);
  Assign(Index_, Dobj_, Valid_);

  ARXMARKER("End: PtrSubArray<T>::Replace(size_t, const T*, Boolean)", ARRAY_ERRSTREAM)
  return ((T*)Dobj_.Pointee());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PtrSubArray<T>::operator new (size_t Bytes_)
{
  ARXMARKER("Start/End: PtrSubArray<T>::operator new (size_t)", ARRAY_ERRSTREAM)
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PtrSubArray<T>::operator delete (void* Space_)
{
  ARXMARKER("Start/End: PtrSubArray<T>::operator delete (void*)", ARRAY_ERRSTREAM)
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* PtrSubArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void PtrSubArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
template <class T>
PtrArrayWrapper<T>::PtrArrayWrapper(const PtrArray<T>& Obj_):
PtrArray<T>(*((PtrArray<T>*)&Obj_))
{
  ARXMARKER("Start/End: PtrArrayWrapper<T>::PtrArrayWrapper(const PtrArray<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrArrayWrapper<T>::PtrArrayWrapper(const PtrArrayWrapper<T>& Obj_):
PtrArray<T>(*((PtrArray<T>*)&Obj_))
{
  ARXMARKER("Start/End: PtrArrayWrapper<T>::PtrArrayWrapper(const PtrArrayWrapper<T>&)", ARRAY_ERRSTREAM)
}
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
int(*PtrArray<T>::_CompFunc)(const T*, const T*) = NULL;

/****************************************************************************/
template <class T>
PtrArray<T>::PtrArray():
TypedArray<T>((TypedSubArray<T>*)PtrSubArray<T>::Make(DEFAULT_SIZE, DEFAULT_SIZE, 0),
	      DEFAULT_SIZE, DEFAULT_SIZE)
{
  ARXMARKER("Start: PtrArray<T>::PtrArray()", ARRAY_ERRSTREAM)

  ShrinkTo(0);

  ARXMARKER("End: PtrArray<T>::PtrArray()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrArray<T>::PtrArray(size_t NumArgs_):
TypedArray<T>((TypedSubArray<T>*)PtrSubArray<T>::Make(NumArgs_, NumArgs_, 0),
	      NumArgs_, NumArgs_)
{
  ARXMARKER("Start: PtrArray<T>::PtrArray(size_t)", ARRAY_ERRSTREAM)

  ShrinkTo(0);

  ARXMARKER("End: PtrArray<T>::PtrArray()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrArray<T>::PtrArray(TypedArray<T>& Obj_):
TypedArray<T>(Obj_, (TypedSubArray<T>*)PtrSubArray<T>::Make(GiveSubArray(Obj_)))
{
  ARXMARKER("Start: PtrArray<T>::PtrArray(TypedArray<T>&)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: PtrArray<T>::PtrArray(TypedArray<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrArray<T>::PtrArray(size_t NumArgs_, PtrVector::DATA_OBJECT* Ptr_):
TypedArray<T>((TypedSubArray<T>*)PtrSubArray<T>::Make(Ptr_, NumArgs_),
	      NumArgs_, NumArgs_)
{
  ARXMARKER("Start: PtrArray<T>::PtrArray(size_t, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)

  FindSize();
  delete Ptr_;

  ARXMARKER("End: PtrArray<T>::PtrArray(size_t, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrArray<T>::PtrArray(size_t NumArgs_, T* Ptr_):
TypedArray<T>((TypedSubArray<T>*)PtrSubArray<T>::Make(Ptr_, NumArgs_),
	      NumArgs_, NumArgs_)
{
  ARXMARKER("Start: PtrArray<T>::PtrArray(size_t, T*)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: PtrArray<T>::PtrArray(size_t, T*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrArray<T>::PtrArray(T** Parr_, size_t Len_):
TypedArray<T>((TypedSubArray<T>*)PtrSubArray<T>::Make(Parr_, Len_),
	      Len_, Len_)
{
  ARXMARKER("Start: PtrArray<T>::PtrArray(T**, size_t)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: PtrArray<T>::PtrArray(T**, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
template <class T>
PtrArray<T>::PtrArray(PtrArray<T>& Obj_):
TypedArray<T>((TypedArray<T>&)Obj_, (TypedSubArray<T>*)PtrSubArray<T>::Make(GiveSubArray(*((TypedArray<T>*)&Obj_))))
{
  ARXMARKER("Start: PtrArray<T>::PtrArray(PtrArray<T>&)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: PtrArray<T>::PtrArray(PtrArray<T>&)", ARRAY_ERRSTREAM)
}
/****************************************************************************/
template <class T>
PtrArray<T>::PtrArray(const PtrArrayWrapper<T>& Obj_):
TypedArray<T>((TypedSubArray<T>*)PtrSubArray<T>::Make(GiveSubArray(*((TypedArray<T>*)&Obj_))))
{
  ARXMARKER("Start: PtrArray<T>::PtrArray(PtrArrayWrapper<T>&)", ARRAY_ERRSTREAM)

  FindSize();

  ARXMARKER("End: PtrArray<T>::PtrArray(PtrArrayWrapper<T>&)", ARRAY_ERRSTREAM)
}
#endif
/****************************************************************************/
template <class T>
PtrArray<T>* PtrArray<T>::Make()
{
  ARXMARKER("Start/End: PtrArray<T>::Make()", ARRAY_ERRSTREAM)
  return (new PtrArray<T>);
}

/****************************************************************************/
template <class T>
PtrArray<T>* PtrArray<T>::Make(size_t NumArgs_)
{
  ARXMARKER("Start/End: PtrArray<T>::Make(size_t)", ARRAY_ERRSTREAM)
  return (new PtrArray<T>(NumArgs_));
}

/****************************************************************************/
template <class T>
PtrArray<T>* PtrArray<T>::Make(Object& Obj_)
{
  ARXMARKER("Start: PtrArray<T>::Make(Object&)", ARRAY_ERRSTREAM)

  PtrArrayAcceptor<T> TrgComp_(NULL);
  Obj_.ShallowClone(&TrgComp_);

  ARXMARKER("End: PtrArray<T>::Make(Object&)", ARRAY_ERRSTREAM)
  return TrgComp_._VarPtr;
}

/****************************************************************************/
template <class T>
PtrArray<T>* PtrArray<T>::Make(size_t Rep_, T* Ptr_)
{
  ARXMARKER("Start/End: PtrArray<T>::Make(size_t, T*)", ARRAY_ERRSTREAM)
  return (new PtrArray<T>(Rep_, Ptr_));
}

/****************************************************************************/
template <class T>
PtrArray<T>* PtrArray<T>::Make(size_t Rep_, PtrVector::DATA_OBJECT* Ptr_)
{
  ARXMARKER("Start/End: PtrArray<T>::Make(size_t, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
  return (new PtrArray<T>(Rep_, Ptr_));
}

/****************************************************************************/
template <class T>
PtrArray<T>* PtrArray<T>::Make(T** Parr_, size_t Len_)
{
  ARXMARKER("Start/End: PtrArray<T>::Make(T**, size_t)", ARRAY_ERRSTREAM)
  return (new PtrArray<T>(Parr_, Len_));
}

/****************************************************************************/
template <class T>
const PtrArray<T>* PtrArray<T>::MakeConst(size_t Rep_, T* Ptr_)
{
  ARXMARKER("Start/End: PtrArray<T>::MakeConst(size_t, T*)", ARRAY_ERRSTREAM)
  return Make(Rep_, Ptr_);
}

/****************************************************************************/
template <class T>
const PtrArray<T>* PtrArray<T>::MakeConst(size_t Rep_, PtrVector::DATA_OBJECT* Ptr_)
{
  ARXMARKER("Start/End: PtrArray<T>::MakeConst(size_t, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
  return Make(Rep_, Ptr_);
}

/****************************************************************************/
template <class T>
const PtrArray<T>* PtrArray<T>::MakeConst(T** Parr_, size_t Len_)
{
  ARXMARKER("Start/End: PtrArray<T>::Make(T**, size_t)", ARRAY_ERRSTREAM)
  return Make(Parr_, Len_);
}

/****************************************************************************/
template <class T>
const PtrArray<T>* PtrArray<T>::MakeConst(const Object& Obj_)
{
  ARXMARKER("Start: PtrArray<T>::MakeConst(const Object&)", ARRAY_ERRSTREAM)

  PtrArrayAcceptor<T> TrgComp_(NULL);
  Obj_.ShallowClone(&TrgComp_);

  ARXMARKER("End: PtrArray<T>::MakeConst(const Object&)", ARRAY_ERRSTREAM)
  return TrgComp_._ObjPtr;
}

/****************************************************************************/
template <class T>
PtrVector::DATA_OBJECT* PtrArray<T>::MakeDataStore(T* Ptr_, Boolean OwnerShip_)
{
  ARXMARKER("Start/End: PtrArray<T>::MakeDataStore(T*, Boolean)", ARRAY_ERRSTREAM)
  return (new PtrVector::DATA_OBJECT(Ptr_, TypedObjectDeleter<T>::Make(), OwnerShip_));
}

/****************************************************************************/
template <class T>
void PtrArray<T>::Swap(size_t index1, size_t index2)
{
  if (index1 != index2 && ((*this)[index1]() != (*this)[index2]()))
  {  
    T* Temp_ = (*this)[index1]();
    (*this)[index1] = (*this)[index2]();
    (*this)[index2] = Temp_;
  }
}

/****************************************************************************/
template <class T>
void PtrArray<T>::ShellSwap(size_t lo, size_t hi, size_t Spread_, int Order_)
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

/****************************************************************************/
template <class T>
void PtrArray<T>::ShellSort(size_t lo, size_t hi, int Order_)
{
  size_t Spread_;	// gap between array elements

  for (Spread_=(hi-lo+1)/2; Spread_;)
  {
    ShellSwap(lo, hi, Spread_, Order_);
    Spread_ /= 2;
  }
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::IsEqualToPtrArray(const PtrArray<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (Compare(*Ptr_) == 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::IsLesserToPtrArray(const PtrArray<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (Compare(*Ptr_) < 0);

  return 0;
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::IsGreaterToPtrArray(const PtrArray<T>* Ptr_) const
{
  if (_CompFunc && Ptr_)
    return (Compare(*Ptr_) > 0);

  return 0;
}

/****************************************************************************/
template <class T>
void PtrArray<T>::SetComparisonFunction(int(*CmpFnc_)(const T*, const T*))
{
  _CompFunc = CmpFnc_;
}

/****************************************************************************/
template <class T>
SimpleArray<int> PtrArray<T>::SeriesArrayCompare(const PtrArray<T>& Arr_) const
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
int PtrArray<T>::Compare(const PtrArray<T>& Arr_) const
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
Boolean PtrArray<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  PtrArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToPtrArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  PtrArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToPtrArray(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  PtrArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToPtrArray(TrgComp_);
}

/****************************************************************************/
template <class T>
int PtrArray<T>::ApplyBinary(int(*Fnc_)(const T*, const T*), const T* Ptr_) const
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*Fnc_)((*this)[Index_](), Ptr_);

  return 0;
}

/****************************************************************************/
template <class T>
int PtrArray<T>::ApplyBinary(int(*Fnc_)(T*, T*), T* Ptr_)
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*Fnc_)((*this)[Index_](), Ptr_);

  return 0;
}

/****************************************************************************/
template <class T>
int PtrArray<T>::ApplyUnary(int(*Fnc_)(T*))
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*Fnc_)((*this)[Index_]());

  return 0;
}

/****************************************************************************/
template <class T>
int PtrArray<T>::ApplyUnary(T*(*Fnc_)(T*))
{
  size_t Index_, Max_;

  if (Fnc_)
    for (Index_ = 0, Max_ = RunLength(); Index_ < Max_; Index_++)
      (*this)[Index_] = (*Fnc_)((*this)[Index_]());

  return 0;
}

/****************************************************************************/
template <class T>
void PtrArray<T>::Sort(int Order_)
{
  if (_CompFunc)
    ShellSort(0, RunLength() - 1, Order_);
}

/****************************************************************************/
template <class T>
Object& PtrArray<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  PtrArrayAcceptor<T> TrgComp_(this);
  return *((Object*) (ObjComp_->AssignFromPtrArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& PtrArray<T>::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  PtrArrayAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->ShallowAssign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromPtrArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* PtrArray<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (!ObjComp_)
    return NULL;

  PtrArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromPtrArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
const Object* PtrArray<T>::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new PtrArray<T>(*((TypedArray<T>*)this)));

  PtrArrayAcceptor<T> TrgComp_(this);
  return ((const Object*) (ObjComp_->CreateFromPtrArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* PtrArray<T>::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return (new PtrArray<T>(*((TypedArray<T>*)this)));

  PtrArrayAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromPtrArray(TrgComp_)));
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static PtrArray<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& PtrArray<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return PtrArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& PtrArray<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return PtrArray<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
long PtrArray<T>::StorableFormWidth() const
{
  return 0;
}

/****************************************************************************/
template <class T>
int PtrArray<T>::ClassID() const
{
  return TypeInfo::PTRARRAY_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* PtrArray<T>::GiveObjectAcceptor() const
{
  ARXMARKER("Start/End: PtrArray<T>::GiveObjectAcceptor() const", ARRAY_ERRSTREAM)
  return (new PtrArrayAcceptor<T>(this));
}

/****************************************************************************/
template <class T>
istream& PtrArray<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: PtrArray<T>::BinaryIgnore(istream&, Boolean*)", ARRAY_ERRSTREAM)

  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: PtrArray<T>::BinaryIgnore(istream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
istream& PtrArray<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: PtrArray<T>::BinaryRead(istream&, Boolean*)", ARRAY_ERRSTREAM)

  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: PtrArray<T>::BinaryRead(istream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
ostream& PtrArray<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  ARXMARKER("Start: PtrArray<T>::BinaryWrite(ostream&, Boolean*) const", ARRAY_ERRSTREAM)

  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);

  ARXMARKER("End: PtrArray<T>::BinaryWrite(ostream&, Boolean*) const", ARRAY_ERRSTREAM)
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& PtrArray<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: PtrArray<T>::BinaryIgnore(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: PtrArray<T>::BinaryIgnore(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& PtrArray<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ARXMARKER("Start: PtrArray<T>::BinaryRead(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);

  ARXMARKER("End: PtrArray<T>::BinaryRead(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
  return Is_;
}

/****************************************************************************/
template <class T>
ByteStream& PtrArray<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  ARXMARKER("Start: PtrArray<T>::BinaryWrite(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)

  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);

  ARXMARKER("End: PtrArray<T>::BinaryWrite(ByteStream&, Boolean*)", ARRAY_ERRSTREAM)
  return Os_;
}

/****************************************************************************/
template <class T>
ostream& PtrArray<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  size_t i, Max_;

  for (i = 0, Max_ = RunLength(); i < Max_; ++i)
  {    
    if ((*this)[i].IsVoid())
      Os_ <<"VOID";
    else
      WriteToOutput(Os_, *((*this)[i]));
      
    Os_ <<" ";
  }

  return Os_;
}

/****************************************************************************/
/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
template <class T>
PtrArray<T>& PtrArray<T>::operator = (PtrArray<T>& That_)
{
  return (PtrArray<T>::operator = (*((TypedArray<T>*)&That_)));
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::operator = (const PtrArrayWrapper<T>& That_)
{
  return (PtrArray<T>::operator = (*((TypedArray<T>*)&That_)));
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::operator += (PtrArray<T>& That_)
{
  return (PtrArray<T>::operator += (*((TypedArray<T>*)&That_)));
}
/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::operator += (const PtrArrayWrapper<T>& That_)
{
  return (PtrArray<T>::operator += (*((TypedArray<T>*)&That_)));
}
#endif
/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::operator = (TypedArray<T>& That_)
{
  ARXMARKER("Start: PtrArray<T>::operator = (TypedArray<T>&)", ARRAY_ERRSTREAM)

  if (this != &That_)
  {
    size_t Index_, Max_;

    for (Index_ = 0, Max_ = That_.RunLength(); Index_ < Max_; Index_++)
      (*this)[Index_].Copy(That_, Index_);

    VoidArray::operator = ((VoidArray&)That_);
    ShrinkTo(Max_);
  }

  ARXMARKER("End: PtrArray<T>::operator = (TypedArray<T>&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::operator += (TypedArray<T>& That_)
{
  ARXMARKER("Start: PtrArray<T>::operator += (TypedArray<T>&)", ARRAY_ERRSTREAM)

  size_t Lindex_, Rindex_, Max_, DestMax_;
  Max_ = Lindex_ = RunLength();
  DestMax_ = That_.RunLength();

  size_t RemSz_ = MaxSize() - Max_;
  if (DestMax_ >= RemSz_)
    Xlen();

  Rindex_ = 0;
  for (Max_ += DestMax_; Lindex_ < Max_; Lindex_++, Rindex_++)
    (*this)[Lindex_].Copy(That_, Rindex_);

  ARXMARKER("End: PtrArray<T>::operator += (TypedArray<T>&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::operator += (T* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::operator += (T*)", ARRAY_ERRSTREAM)

  if (RunLength() == MaxSize())
    Xlen();

  (*this)[RunLength()] = Ptr_;

  ARXMARKER("End: PtrArray<T>::operator += (T*)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::operator += (PtrVector::DATA_OBJECT* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::operator += (PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)

  if (RunLength() == MaxSize())
    Xlen();

  (*this)[RunLength()].AssignObject(Ptr_);

  delete Ptr_;

  ARXMARKER("End: PtrArray<T>::operator += (PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Append(T* Ptr_, size_t Rep_)
{
  ARXMARKER("Start: PtrArray<T>::Append(T*, size_t)", ARRAY_ERRSTREAM)

  size_t Index_, Max_;
  Max_ = Index_ = RunLength();

  size_t RemSz_ = MaxSize() - Max_;
  if (Rep_ >= RemSz_)
    Xlen();

  for (Max_ += Rep_; Index_ < Max_; Index_++)
    (*this)[Index_] = Ptr_;

  ARXMARKER("End: PtrArray<T>::Append(T*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Append(PtrVector::DATA_OBJECT* Ptr_, size_t Rep_)
{
  ARXMARKER("Start: PtrArray<T>::Append(PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)

  size_t Index_, Max_;
  Max_ = Index_ = RunLength();

  size_t RemSz_ = MaxSize() - Max_;
  if (Rep_ >= RemSz_)
    Xlen();

  for (Max_ += Rep_; Index_ < Max_; Index_++)
    (*this)[Index_].AssignObject(Ptr_);

  delete Ptr_;

  ARXMARKER("End: PtrArray<T>::Append(PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Append(T** Parr_, size_t Len_)
{
  ARXMARKER("Start: PtrArray<T>::Append(T**, size_t)", ARRAY_ERRSTREAM)

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

  ARXMARKER("End: PtrArray<T>::Append(T**, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::AssignArg(T* Ptr_, size_t Rep_)
{
  ARXMARKER("Start: PtrArray<T>::AssignArg(T*, size_t)", ARRAY_ERRSTREAM)

  size_t Index_;

  if (Rep_ >= MaxSize())
    Xlen();

  for (Index_ = 0; Index_ < Rep_; Index_++)
    (*this)[Index_] = Ptr_;

  ShrinkTo(Rep_);

  ARXMARKER("End: PtrArray<T>::AssignArg(T*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::AssignArg(PtrVector::DATA_OBJECT* Ptr_, size_t Rep_)
{
  ARXMARKER("Start: PtrArray<T>::AssignArg(PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)

  size_t Index_;

  if (Rep_ >= MaxSize())
    Xlen();

  for (Index_ = 0; Index_ < Rep_; Index_++)
    (*this)[Index_].AssignObject(Ptr_);

  ShrinkTo(Rep_);

  delete Ptr_;

  ARXMARKER("End: PtrArray<T>::AssignArg(PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::AssignArg(T** Parr_, size_t Len_)
{
  ARXMARKER("Start: PtrArray<T>::AssignArg(T**, size_t)", ARRAY_ERRSTREAM)

  size_t Index_;

  if (Len_ >= MaxSize())
    Xlen();

  if (Parr_ == NULL && Len_ > 0)
    Xinv();

  for (Index_ = 0; Index_ < Len_; Index_++)
    (*this)[Index_] = Parr_[Index_];

  ShrinkTo(Len_);

  ARXMARKER("End: PtrArray<T>::AssignArg(T**, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Insert(size_t InsPos_, PtrArray<T>& Arr_)
{
  ARXMARKER("Start: PtrArray<T>::Insert(size_t, PtrArray<T>&)", ARRAY_ERRSTREAM)

  Boolean Identical_ = (this == &Arr_);
  TypedArray<T>* ArrPtr_ = NULL;
  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;

  ArrPtr_ = Identical_ ? (PtrArray<T>*)Arr_.Clone():((PtrArray<T>*)&Arr_);

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

  ARXMARKER("End: PtrArray<T>::Insert(size_t, PtrArray<T>&)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Insert(size_t InsPos_, PtrVector::DATA_OBJECT* Ptr_, size_t Rep_)
{
  ARXMARKER("Start: PtrArray<T>::Insert(size_t, PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)

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
    (*this)[Next_].AssignObject(Ptr_);

  delete Ptr_;

  ARXMARKER("End: PtrArray<T>::Insert(size_t, PtrVector::DATA_OBJECT*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Insert(size_t InsPos_, T* Ptr_, size_t Rep_)
{
  ARXMARKER("Start: PtrArray<T>::Insert(size_t, T*, size_t)", ARRAY_ERRSTREAM)

  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;
  size_t Index_ = RunLength() - 1;
  size_t Next_ = Index_ + Rep_;
  size_t Pos_ = InsPos_;

  // Get remainder values
  size_t RemSz_ = MaxSize() - RunLength();
  size_t RemPos_ = MaxSize() - Pos_;

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
    (*this)[Next_] = Ptr_;

  ARXMARKER("End: PtrArray<T>::Insert(size_t, T*, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Insert(size_t InsPos_, T** Parr_, size_t Len_)
{
  ARXMARKER("Start: PtrArray<T>::Insert(size_t, T**, size_t)", ARRAY_ERRSTREAM)

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

  ARXMARKER("End: PtrArray<T>::Insert(size_t, T**, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Remove(size_t Pos_, size_t Len_)
{
  ARXMARKER("Start: PtrArray<T>::Remove(size_t, size_t)", ARRAY_ERRSTREAM)

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

  ARXMARKER("End: PtrArray<T>::Remove(size_t, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArray<T>& PtrArray<T>::Sub_Array(PtrArray<T>& Arr_, size_t Pos_, size_t Len_)
{
  ARXMARKER("Start: PtrArray<T>::Sub_Array(PtrArray<T>&, size_t, size_t)", ARRAY_ERRSTREAM)

  if (Pos_ >= RunLength())
    Xran();

  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;
  size_t Remain_ = RunLength() - Pos_;
  size_t Xlen_ = (Len_ < Remain_) ? Len_:Remain_;
  size_t Index_ = 0;

  for (size_t Max_ = Pos_ + Xlen_; Pos_ < Max_; Pos_++, Index_++)
    Arr_[Index_].Copy(*ThisPtr_, Pos_);

  Arr_.ShrinkTo(Xlen_);

  ARXMARKER("End: PtrArray<T>::Sub_Array(PtrArray<T>&, size_t, size_t)", ARRAY_ERRSTREAM)
  return Arr_;
}

/****************************************************************************/
template <class T>
const PtrArray<T> PtrArray<T>::Sub_Array(size_t Pos_, size_t Len_) const
{
  ARXMARKER("Start: PtrArray<T>::Sub_Array(size_t, size_t) const", ARRAY_ERRSTREAM)

  PtrArray<T> Arr_;

  if (Pos_ >= RunLength())
    Xran();

  TypedArray<T>* ThisPtr_ = (TypedArray<T>*)this;
  size_t Remain_ = RunLength() - Pos_;
  size_t Xlen_ = (Len_ < Remain_) ? Len_:Remain_;
  size_t Index_ = 0;

  for (size_t Max_ = Pos_ + Xlen_; Pos_ < Max_; Pos_++, Index_++)
    Arr_[Index_].ConstCopy(*ThisPtr_, Pos_);

  Arr_.ShrinkTo(Xlen_);

  ARXMARKER("End: PtrArray<T>::Sub_Array(size_t, size_t) const", ARRAY_ERRSTREAM)
  return Arr_;
}

/****************************************************************************/
template <class T>
void PtrArray<T>::Swap(PtrArray<T>& Arr_)
{
  ARXMARKER("Start: PtrArray<T>::Swap(PtrArray<T>&) const", ARRAY_ERRSTREAM)

  PtrArray<T> Temp_(*this);
  *this = Arr_;
  Arr_ = Temp_;

  ARXMARKER("End: PtrArray<T>::Swap(PtrArray<T>&) const", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
const T* PtrArray<T>::Get_At(size_t Pos_, Boolean& Valid_) const
{
  ARXMARKER("Start: PtrArray<T>::Get_At(size_t, Boolean&) const", ARRAY_ERRSTREAM)

  Valid_ = !(Limit() && (Pos_ >= Limit() || Pos_ >= RunLength()));

  if (!Valid_)
  {
    ARXMARKER("End(1): PtrArray<T>::Get_At(size_t, Boolean&) const", ARRAY_ERRSTREAM)
    return NULL;
  }

  const PtrArrayRef<T> Ref_(_Head->Index(0, Pos_));

  ARXMARKER("End(2): PtrArray<T>::Get_At(size_t, Boolean&) const", ARRAY_ERRSTREAM)
  return ((const T*)Ref_);
}

/****************************************************************************/
template <class T>
T* PtrArray<T>::Get_At(size_t Pos_, Boolean& Valid_)
{
  ARXMARKER("Start: PtrArray<T>::Get_At(size_t, Boolean&)", ARRAY_ERRSTREAM)

  Valid_ = !(Limit() && (Pos_ >= Limit() || Pos_ >= RunLength()));

  if (!Valid_)
  {
    ARXMARKER("End(1): PtrArray<T>::Get_At(size_t, Boolean&)", ARRAY_ERRSTREAM)
    return NULL;
  }

  PtrArrayRef<T> Ref_(_Head->Index(0, Pos_));

  ARXMARKER("End(2): PtrArray<T>::Get_At(size_t, Boolean&)", ARRAY_ERRSTREAM)
  return ((T*)Ref_);
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::Put_At(size_t Pos_, T* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::Put_At(size_t, T*) const", ARRAY_ERRSTREAM)

  if (Limit() && (Pos_ >= Limit() || Pos_ >= Total()))
  {
    ARXMARKER("End(1): PtrArray<T>::Put_At(size_t, T*) const", ARRAY_ERRSTREAM)
    return FALSE;
  }

  PtrArrayRef<T> Ref_(_Head->Index(0, Pos_));
  Ref_ = Ptr_;

  ARXMARKER("End(2): PtrArray<T>::Put_At(size_t, T*) const", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::Put_At(size_t Pos_, PtrVector::DATA_OBJECT* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::Put_At(size_t, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)

  if (Limit() && (Pos_ >= Limit() || Pos_ >= Total()))
  {
    ARXMARKER("End(1): PtrArray<T>::Put_At(size_t, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  PtrArrayRef<T> Ref_(_Head->Index(0, Pos_));
  Ref_.AssignObject(Ptr_);

  delete Ptr_;

  ARXMARKER("End(2): PtrArray<T>::Put_At(size_t, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
void PtrArray<T>::Shrink(size_t Size_)
{
  ARXMARKER("Start: PtrArray<T>::Shrink(size_t)", ARRAY_ERRSTREAM)

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

  ARXMARKER("End: PtrArray<T>::Shrink(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void PtrArray<T>::ShrinkTo(size_t Size_)
{
  ARXMARKER("Start: PtrArray<T>::ShrinkTo(size_t)", ARRAY_ERRSTREAM)

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

  ARXMARKER("End: PtrArray<T>::ShrinkTo(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::Grow(size_t Size_, T* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::Grow(size_t, T*)", ARRAY_ERRSTREAM)

  size_t Index_ = RunLength(),
	 Max_ = RunLength() + Size_;
  size_t RemSz_ = MaxSize() - Index_;

  if (Size_ >= RemSz_)
    Xlen();

  if (Max_ >= MaxSize() || Limit() && Max_ > Limit())
  {
    ARXMARKER("End(1): PtrArray<T>::Grow(size_t, T*)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  for (;Index_ < Max_; Index_++)
    (*this)[Index_] = Ptr_;

  ARXMARKER("End(2): PtrArray<T>::Grow(size_t, T*)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::Grow(size_t Size_, PtrVector::DATA_OBJECT* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::Grow(size_t, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)

  size_t Index_ = RunLength(),
	 Max_ = RunLength() + Size_;
  size_t RemSz_ = MaxSize() - Index_;

  if (Size_ >= RemSz_)
    Xlen();

  if (Max_ >= MaxSize() || Limit() && Max_ > Limit())
  {
    ARXMARKER("End(1): PtrArray<T>::Grow(size_t, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  for (;Index_ < Max_; Index_++)
    (*this)[Index_].AssignObject(Ptr_);

  delete Ptr_;

  ARXMARKER("End(2): PtrArray<T>::Grow(size_t, PtrVector::DATA_OBJECT)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::GrowTo(size_t Size_, T* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::GrowTo(size_t, T*)", ARRAY_ERRSTREAM)

  if (Size_ >= MaxSize() || Limit() && Size_ > Limit())
  {
    ARXMARKER("End(1): PtrArray<T>::GrowTo(size_t, T*)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  for (size_t Index_ = RunLength(); Index_ < Size_; Index_++)
    (*this)[Index_] = Ptr_;

  ARXMARKER("End(2): PtrArray<T>::GrowTo(size_t, T*)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean PtrArray<T>::GrowTo(size_t Size_, PtrVector::DATA_OBJECT* Ptr_)
{
  ARXMARKER("Start: PtrArray<T>::GrowTo(size_t, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)

  if (Size_ >= MaxSize() || Limit() && Size_ > Limit())
  {
    ARXMARKER("End(1): PtrArray<T>::GrowTo(size_t, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
    return FALSE;
  }

  for (size_t Index_ = RunLength(); Index_ < Size_; Index_++)
    (*this)[Index_].AssignObject(Ptr_);

  delete Ptr_;

  ARXMARKER("End(2): PtrArray<T>::GrowTo(size_t, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
  return TRUE;
}

/****************************************************************************/
template <class T>
PtrArrayRef<T> PtrArray<T>::operator [] (size_t Index_)
{
  ARXMARKER("Start/End: PtrArray<T>::operator [] (size_t)", ARRAY_ERRSTREAM)
  return PtrArrayRef<T>(_Head->Index(0, Index_));
}

/****************************************************************************/
template <class T>
const PtrArrayRef<T> PtrArray<T>::operator [] (size_t Index_) const
{
  ARXMARKER("Start/End: PtrArray<T>::operator [] (size_t) const", ARRAY_ERRSTREAM)
  return ((const PtrArrayRef<T>)(_Head->Index(0, Index_)));
}

/****************************************************************************/
template <class T>
T** PtrArray<T>::ToPtrArray(T** Parr_, size_t Len_)
{
  ARXMARKER("Start: PtrArray<T>::ToPtrArray(T**, size_t)", ARRAY_ERRSTREAM)

  if (Parr_)
  {
    if (Len_ == 0)
      Len_ = RunLength();

    for (size_t Index_ = 0; Index_ < Len_; Index_++)
      Parr_[Index_] = (*this)[Index_]();
  }

  ARXMARKER("End: PtrArray<T>::ToPtrArray(T**, size_t)", ARRAY_ERRSTREAM)
  return (Len_ ? Parr_:NULL);
}

/****************************************************************************/
#if defined(__TURBOC__) | defined(__BORLANDC__)
template <class T>
PtrArray<T> operator + (PtrArray<T>& Lhs_, PtrArray<T>& Rhs_)
{
  ARXMARKER("Start: PtrArray<T>::operator + (PtrArray<T>&, PtrArray<T>&)", ARRAY_ERRSTREAM)

  PtrArray<T> Lhsp_(Lhs_);
  Lhsp_ += Rhs_;

  ARXMARKER("End: PtrArray<T>::operator + (PtrArray<T>&, PtrArray<T>&)", ARRAY_ERRSTREAM)
  return Lhsp_;
}

/****************************************************************************/
template <class T>
PtrArray<T> operator + (PtrArray<T>& Lhs_, T* Rhs_)
{
  ARXMARKER("Start: PtrArray<T>::operator + (PtrArray<T>&, T*)", ARRAY_ERRSTREAM)

  PtrArray<T> Lhsp_(Lhs_);
  Lhsp_ += Rhs_;

  ARXMARKER("End: PtrArray<T>::operator + (PtrArray<T>&, T*)", ARRAY_ERRSTREAM)
  return Lhsp_;
}

/****************************************************************************/
template <class T>
PtrArray<T> operator + (PtrArray<T>& Lhs_, PtrVector::DATA_OBJECT* Rhs_)
{
  ARXMARKER("Start: PtrArray<T>::operator + (PtrArray<T>&, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)

  PtrArray<T> Lhsp_(Lhs_);
  Lhsp_ += Rhs_;

  ARXMARKER("End: PtrArray<T>::operator + (PtrArray<T>&, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
  return Lhsp_;
}

/****************************************************************************/
template <class T>
PtrArray<T> operator + (T* Lhs_, PtrArray<T>& Rhs_)
{
  ARXMARKER("Start: PtrArray<T>::operator + (T*, PtrArray<T>&)", ARRAY_ERRSTREAM)

  PtrArray<T> Lhsp_(1, Lhs_);
  Lhsp_ += Rhs_;

  ARXMARKER("End: PtrArray<T>::operator + (T*, PtrArray<T>&)", ARRAY_ERRSTREAM)
  return Lhsp_;
}

/****************************************************************************/
template <class T>
PtrArray<T> operator + (PtrVector::DATA_OBJECT* Lhs_, PtrArray<T>& Rhs_)
{
  ARXMARKER("Start: PtrArray<T>::operator + (PtrVector::DATA_OBJECT*, PtrArray<T>&)", ARRAY_ERRSTREAM)

  PtrArray<T> Lhsp_(1, Lhs_);
  Lhsp_ += Rhs_;

  ARXMARKER("End: PtrArray<T>::operator + (PtrVector::DATA_OBJECT*, PtrArray<T>&)", ARRAY_ERRSTREAM)
  return Lhsp_;
}
#endif
/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PtrArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PtrArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* PtrArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void PtrArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
PtrArrayAcceptor<T>::PtrArrayAcceptor(const PtrArray<T>* Ptr_):
_ObjPtr(Ptr_)
{
  ARXMARKER("Start/End: PtrArrayAcceptor<T>::operator + (PtrArray<T>*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
Boolean PtrArrayAcceptor<T>::IsTypedArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
Boolean PtrArrayAcceptor<T>::IsPtrArray() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void PtrArrayAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const PtrArray<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* PtrArrayAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
void* PtrArrayAcceptor<T>::CreateFromPtrArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: PtrArrayAcceptor<T>::CreateFromPtrArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  _VarPtr = new PtrArray<T>(*((PtrArray<T>*)Obj_.GiveDataPtr()));

  ARXMARKER("End: PtrArrayAcceptor<T>::CreateFromPtrArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* PtrArrayAcceptor<T>::AssignFromPtrArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: PtrArrayAcceptor<T>::AssignFromPtrArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  PtrArray<T>* TrgPtr_ = (PtrArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;

  ARXMARKER("End: PtrArrayAcceptor<T>::AssignFromPtrArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* PtrArrayAcceptor<T>::CreateFromArgArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: PtrArrayAcceptor<T>::CreateFromArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  _VarPtr = new PtrArray<T>(*((TypedArray<T>*)Obj_.GiveDataPtr()));

  ARXMARKER("End: PtrArrayAcceptor<T>::CreateFromArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* PtrArrayAcceptor<T>::AssignFromArgArray(const ObjectAcceptor& Obj_)
{
  ARXMARKER("Start: PtrArrayAcceptor<T>::AssignFromArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)

  TypedArray<T>* TrgPtr_ = (TypedArray<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;

  ARXMARKER("End: PtrArrayAcceptor<T>::AssignFromArgArray(const ObjectAcceptor&)", ARRAY_ERRSTREAM)
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean PtrArrayAcceptor<T>::IsEqualToPtrArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToPtrArray((const PtrArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean PtrArrayAcceptor<T>::IsLesserToPtrArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToPtrArray((const PtrArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>    
Boolean PtrArrayAcceptor<T>::IsGreaterToPtrArray(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToPtrArray((const PtrArray<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PtrArrayAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PtrArrayAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
PtrArrayRef<T>::PtrArrayRef(const RefInfo& Info_):
TypedArrayRef<T>(Info_)
{
  ARXMARKER("Start: PtrArrayRef<T>::PtrArrayRef(const RefInfo&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
PtrSubArray<T>* PtrArrayRef<T>::CastTo_PtrSubArray() const
{
  ARXMARKER("Start: PtrArrayRef<T>::CastTo_PtrSubArray() const", ARRAY_ERRSTREAM)

  TypedSubArray<T>* Ptr_ = ((TypedSubArray<T>*)_BossPtr);

  ARXMARKER("End: PtrArrayRef<T>::CastTo_PtrSubArray() const", ARRAY_ERRSTREAM)
  return ((PtrSubArray<T>*)Ptr_);
}

/****************************************************************************/
template <class T>
PtrArrayRef<T>& PtrArrayRef<T>::Assign(PtrVector::DATA_OBJECT* Ptr_)
{
  ARXMARKER("Start: PtrArrayRef<T>::Assign(PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)

  TypedArrayRef<T>::AssignObject(Ptr_, TRUE);

  ARXMARKER("End: PtrArrayRef<T>::Assign(PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArrayRef<T>& PtrArrayRef<T>::operator = (T* Ptr_)
{
  ARXMARKER("Start: PtrArrayRef<T>::operator = (T*)", ARRAY_ERRSTREAM)

  _Modified = Ptr_ == NULL;

  if (_Modified)
  {
    if (IsValid())
      DecrementSize(1);
  }
  else
    if (!IsValid())
      IncrementSize(1);

  CastTo_PtrSubArray()->Arg(_ArrayIndex, Ptr_);

  ARXMARKER("End: PtrArrayRef<T>::operator = (T*)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
template <class T>
PtrArrayRef<T>::operator T* ()
{
  ARXMARKER("Start/End: PtrArrayRef<T>::operator T* ()", ARRAY_ERRSTREAM)
  return CastTo_PtrSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
template <class T>
PtrArrayRef<T>::operator const T* () const
{
  ARXMARKER("Start/End: PtrArrayRef<T>::operator const T* () const", ARRAY_ERRSTREAM)
  return CastTo_PtrSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
template <class T>
T& PtrArrayRef<T>::operator * ()
{
  ARXMARKER("Start: PtrArrayRef<T>::operator * ()", ARRAY_ERRSTREAM)

  T* Ptr_ = CastTo_PtrSubArray()->Arg(_ArrayIndex);

  if (Ptr_ == NULL)
    Xnullp();

  ARXMARKER("End: PtrArrayRef<T>::operator * ()", ARRAY_ERRSTREAM)
  return *Ptr_;
}

/****************************************************************************/
template <class T>
const T& PtrArrayRef<T>::operator * () const
{
  ARXMARKER("Start: PtrArrayRef<T>::operator * () const", ARRAY_ERRSTREAM)

  T* Ptr_ = CastTo_PtrSubArray()->Arg(_ArrayIndex);

  if (Ptr_ == NULL)
    Xnullp();

  ARXMARKER("End: PtrArrayRef<T>::operator * () const", ARRAY_ERRSTREAM)
  return *Ptr_;
}

/****************************************************************************/
template <class T>
Boolean PtrArrayRef<T>::IsVoid() const
{
  ARXMARKER("Start/End: PtrArrayRef<T>::IsVoid() const", ARRAY_ERRSTREAM)
  return !IsDataDefined();
}

/****************************************************************************/
template <class T>
T* PtrArrayRef<T>::operator () ()
{
  ARXMARKER("Start/End: PtrArrayRef<T>::operator () ()", ARRAY_ERRSTREAM)
  return CastTo_PtrSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
template <class T>
const T* PtrArrayRef<T>::operator () () const
{
  ARXMARKER("Start/End: PtrArrayRef<T>::operator () () const", ARRAY_ERRSTREAM)
  return CastTo_PtrSubArray()->Arg(_ArrayIndex);
}

/****************************************************************************/
/****************************************************************************/
#if ARRAY_IMPLEMENT_STLCONTAINER
template <class T>
PtrArrayContainer<T>::PtrArrayContainer(PtrArray<T>* Ap_):
_ContainerPtr(Ap_)
{}

/****************************************************************************/
template <class T>
PtrArrayContainer<T>::PtrArrayContainer(PtrArrayContainer<T>& Ap_):
_ContainerPtr(Ap_._ContainerPtr)
{}

/****************************************************************************/
template <class T>
const PtrArrayIterator<T> PtrArrayContainer<T>::begin() const
{
  return (PtrArrayIterator<T>(_ContainerPtr));
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T> PtrArrayContainer<T>::begin()
{
  return (PtrArrayIterator<T>(_ContainerPtr));
}

/****************************************************************************/
template <class T>
const PtrArrayIterator<T> PtrArrayContainer<T>::end() const
{
  PtrArrayIterator<T> Iter_(_ContainerPtr);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T> PtrArrayContainer<T>::end()
{
  PtrArrayIterator<T> Iter_(_ContainerPtr);
  Iter_.SetToEnd();
  return Iter_;
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> PtrArrayContainer<T>::rbegin() const
{
  PtrArrayIterator<T> Iter_(_ContainerPtr);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> PtrArrayContainer<T>::rbegin()
{
  PtrArrayIterator<T> Iter_(_ContainerPtr);
  return ReverseRandomAccessIterator<T>(Iter_);
}

/****************************************************************************/
template <class T>
const ReverseRandomAccessIterator<T> PtrArrayContainer<T>::rend() const
{
  PtrArrayIterator<T> Iter_(_ContainerPtr);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
ReverseRandomAccessIterator<T> PtrArrayContainer<T>::rend()
{
  PtrArrayIterator<T> Iter_(_ContainerPtr);
  ReverseRandomAccessIterator<T> Riter_(Iter_);
  Riter_.SetToEnd();
  return Riter_;
}

/****************************************************************************/
template <class T>
T* PtrArrayContainer<T>::front()
{
  return &(*begin());
}

/****************************************************************************/
template <class T>
const T* PtrArrayContainer<T>::front() const
{
  return &(*begin());
}

/****************************************************************************/
template <class T>
T* PtrArrayContainer<T>::back()
{
  return &(*(--end()));
}

/****************************************************************************/
template <class T>
const T* PtrArrayContainer<T>::back() const
{
  return &(*(--end()));
}

/****************************************************************************/
template <class T>
void PtrArrayContainer<T>::push_back(T* x)
{
  insert(end(), x);
}

/****************************************************************************/
template <class T>
void PtrArrayContainer<T>::pop_back()
{
  PtrArrayIterator<T> Iter_ = end();
  --Iter_;
  erase(Iter_);
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T> PtrArrayContainer<T>::insert(PtrArrayIterator<T> pos, T* x)
{
  --pos;

  if (pos.More())
    _ContainerPtr->Insert(pos.Position(), x);

  return pos;
}

/****************************************************************************/
template <class T>
void PtrArrayContainer<T>::insert(PtrArrayIterator<T> pos, size_t n, T* x)
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
void PtrArrayContainer<T>::insert(PtrArrayIterator<T> pos, const PtrArrayIterator<T> first, const PtrArrayIterator<T> last)
{
  --pos;

  if (pos.More())
    while (first != last && first.More())
    {
      Boolean Valid_;
      T* Obj_ = _ContainerPtr->Get_At(first.Position(), Valid_);

      if (Valid_)
	_ContainerPtr->Insert(pos.Position(), Obj_);

      ++first;
    }
}

/****************************************************************************/
template <class T>
void PtrArrayContainer<T>::erase(PtrArrayIterator<T> pos)
{
  if (pos.More())
    (*_ContainerPtr)[pos.Position()] = NULL;
}

/****************************************************************************/
template <class T>
void PtrArrayContainer<T>::erase(PtrArrayIterator<T> first, PtrArrayIterator<T> last)
{
  while (first != last && first.More())
  {
    (*_ContainerPtr)[first.Position()] = NULL;
    ++first;
  }
}
#endif
/****************************************************************************/
/****************************************************************************/
#if ARRAY_IMPLEMENT_STLITERATOR
template <class T>
PtrArrayIterator<T>::PtrArrayIterator(const PtrArray<T>* Ap_, Boolean DestroyFromIter_):
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
PtrArrayIterator<T>::PtrArrayIterator(const PtrArrayIterator<T>& Ap_):
RandomAccessIterator<T>(),
_ArrayObjPtr(Ap_._ArrayObjPtr),
_Index(Ap_._Index)
{
  if (_ArrayObjPtr)
    _ArrayVarPtr->RegisterIterator();
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T>::~PtrArrayIterator()
{
  if (_ArrayObjPtr)
  {
    Boolean Destroy_ =
    _ArrayVarPtr->UnRegisterIterator();

    if (Destroy_)
      delete _ArrayObjPtr;
  }
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T>* PtrArrayIterator<T>::Make(PtrArrayIterator<T>& Ap_)
{
  return (new PtrArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T>* PtrArrayIterator<T>::Make(PtrArray<T>* Ap_)
{
  return (new PtrArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const PtrArrayIterator<T>* PtrArrayIterator<T>::MakeConst(const PtrArray<T>* Ap_)
{
  return (new PtrArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
const PtrArrayIterator<T>* PtrArrayIterator<T>::MakeConst(const PtrArrayIterator<T>& Ap_)
{
  return (new PtrArrayIterator<T>(Ap_));
}

/****************************************************************************/
template <class T>
int PtrArrayIterator<T>::ClassID() const
{
  return TypeInfo::PTRARRAYITERATOR_TYPE;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>* PtrArrayIterator<T>::Clone()
{
  return (new PtrArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>* PtrArrayIterator<T>::Clone() const
{
  return (new PtrArrayIterator<T>(*this));
}

/****************************************************************************/
template <class T>
Boolean PtrArrayIterator<T>::IsValid(SizeType Index_) const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (Index_ < _ArrayVarPtr->RunLength())
    return !((*_ArrayVarPtr)[Index_].IsVoid());

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrArrayIterator<T>::IsValid() const
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return !((*_ArrayVarPtr)[_Index].IsVoid());

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean PtrArrayIterator<T>::More() const
{
  if (_ArrayVarPtr->RunLength() == 0)
  {
    PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;
    LocalThis_->_Index = SizeType(-1);
  }

  return !AtEnd();
}

/****************************************************************************/
template <class T>
void PtrArrayIterator<T>::Reset() const
{
  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;
  LocalThis_->_Index = 0;
}

/****************************************************************************/
template <class T>
void PtrArrayIterator<T>::SetToLast() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;
  LocalThis_->_Index = _ArrayObjPtr->RunLength() - 1;
}

/****************************************************************************/
template <class T>
void PtrArrayIterator<T>::SetToEnd() const
{
  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;
  LocalThis_->_Index = SizeType(-1);
}

/****************************************************************************/
template <class T>
const T* PtrArrayIterator<T>::ConstPointee() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return ((*_ArrayObjPtr)[_Index])();
  return ((*_ArrayObjPtr)[0])();
}

/****************************************************************************/
template <class T>
const T* PtrArrayIterator<T>::Pointee() const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return ((*_ArrayObjPtr)[_Index])();
  return ((*_ArrayObjPtr)[0])();
}

/****************************************************************************/
template <class T>    
T* PtrArrayIterator<T>::Pointee()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return ((*_ArrayVarPtr)[_Index])();
  return ((*_ArrayVarPtr)[0])();
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T>& PtrArrayIterator<T>::GotoHead() const
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>    
const RandomAccessIterator<T>& PtrArrayIterator<T>::GotoTail() const
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrArrayIterator<T>::GotoHead()
{
  Reset();
  return *this;
}

/****************************************************************************/
template <class T>    
RandomAccessIterator<T>& PtrArrayIterator<T>::GotoTail()
{
  SetToLast();
  return *this;
}

/****************************************************************************/
template <class T>
Boolean PtrArrayIterator<T>::AtHead() const
{
  return (_Index == 0);
}

/****************************************************************************/
template <class T>
Boolean PtrArrayIterator<T>::AtTail() const
{
  return (_Index == _ArrayObjPtr->RunLength() - 1);
}

/****************************************************************************/
template <class T>
T& PtrArrayIterator<T>::operator * ()
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (More())
    return *((*_ArrayVarPtr)[_Index]);
  return *((*_ArrayVarPtr)[0]);
}

/****************************************************************************/
template <class T>
const T& PtrArrayIterator<T>::operator * () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  if (More())
    return *((*_ArrayObjPtr)[_Index]);
  return *((*_ArrayObjPtr)[0]);
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrArrayIterator<T>::operator [] (SizeType Index_)
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
const RandomAccessIterator<T>& PtrArrayIterator<T>::operator [] (SizeType Index_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;
  LocalThis_->_Index = Index_;

  if (!AtEnd())
    ((*_ArrayObjPtr)[_Index]);
  else
    ((*_ArrayObjPtr)[0]);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrArrayIterator<T>::operator ++ ()
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
RandomAccessIterator<T> PtrArrayIterator<T>::operator ++ (int)
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
const RandomAccessIterator<T>& PtrArrayIterator<T>::operator ++ () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrArrayIterator<T>::operator ++ (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;

  if (_Index < _ArrayObjPtr->RunLength() - 1)
    ++LocalThis_->_Index;
  else
    LocalThis_->_Index = SizeType(-1);

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrArrayIterator<T>::operator -- ()
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
RandomAccessIterator<T> PtrArrayIterator<T>::operator -- (int)
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
const RandomAccessIterator<T>& PtrArrayIterator<T>::operator -- () const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return *this;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrArrayIterator<T>::operator -- (int) const
{
  RandomAccessIterator<T> Ref_(*this);

  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;

  if (More())
    --LocalThis_->_Index;
  else
    LocalThis_->SetToLast();

  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> PtrArrayIterator<T>::operator + (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrArrayIterator<T>::operator + (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ += Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T> PtrArrayIterator<T>::operator - (SizeType Diff_)
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
const RandomAccessIterator<T> PtrArrayIterator<T>::operator - (SizeType Diff_) const
{
  RandomAccessIterator<T> Ref_(*this);
  Ref_ -= Diff_;
  return Ref_;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrArrayIterator<T>::operator += (SizeType Diff_)
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
const RandomAccessIterator<T>& PtrArrayIterator<T>::operator += (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;

  if (_Index + Diff_ < _ArrayObjPtr->RunLength())
    LocalThis_->_Index += Diff_;
  else
    LocalThis_->_Index = SizeType(-1);

  return *this;
}

/****************************************************************************/
template <class T>
RandomAccessIterator<T>& PtrArrayIterator<T>::operator -= (SizeType Diff_)
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
const RandomAccessIterator<T>& PtrArrayIterator<T>::operator -= (SizeType Diff_) const
{
  if (_ArrayObjPtr == NULL)
    Xnullp();

  PtrArrayIterator<T>* const LocalThis_ = (PtrArrayIterator<T>* const)this;

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
PtrArrayIterator<T>& PtrArrayIterator<T>::SetDataObject(PtrVector::DATA_OBJECT* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index].Assign(Ptr_);

  return *this;
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T>& PtrArrayIterator<T>::SetData(T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index] = Ptr_;

  return *this;
}

/****************************************************************************/
template <class T>
PtrArrayIterator<T>& PtrArrayIterator<T>::operator = (const PtrArrayIterator<T>& Rhs_)
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
PtrArrayIterator<T>& PtrArrayIterator<T>::operator = (T* Ptr_)
{
  if (_ArrayVarPtr == NULL)
    Xnullp();

  if (!AtEnd())
    (*_ArrayVarPtr)[_Index] = Ptr_;

  return *this;
}

#endif
/****************************************************************************/
#endif




