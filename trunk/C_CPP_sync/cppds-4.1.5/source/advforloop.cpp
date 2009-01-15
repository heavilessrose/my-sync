// Listing 2: AdvForLoop.cpp. Multiple for loop class
//
#ifndef ADVFORLOOP_CPP
#define ADVFORLOOP_CPP
#ifndef ADVFORLOOP_H
  #include "advforloop.h"
#endif

/****************************************************************************/
template <class T>
AdvForLoop<T>::AdvForLoop():
ObjectImp(),
_StartVal(NULL),
_Limit(NULL),
_Step(NULL),
_StartValOffset(NULL),
_LimitOffset(NULL),
_StepOffset(NULL),
_Condition(NULL),
_Next(NULL),
_Running(FALSE),
_InLoopBody(FALSE),
_LoopNum(0),
_IndexPtr(&_Index)
{}

/****************************************************************************/
template <class T>
AdvForLoop<T>::AdvForLoop(T* StartVal_, T* Limit_, T* Step_,
		    T* StartValOffset_, T* LimitOffset_, T* StepOffset_,
		    Boolean* Cond_):
ObjectImp(),
_StartVal(StartVal_),
_Limit(Limit_),
_Step(Step_),
_StartValOffset(StartValOffset_),
_LimitOffset(LimitOffset_),
_StepOffset(StepOffset_),
_Condition(Cond_),
_Next(NULL),
_Running(FALSE),
_InLoopBody(FALSE),
_LoopNum(0),
_IndexPtr(&_Index)
{}

/****************************************************************************/
template <class T>
AdvForLoop<T>::AdvForLoop(const T& StartVal_, const T& Limit_, const T& Step_,
		    T* StartValOffset_, T* LimitOffset_, T* StepOffset_):
ObjectImp(),
_Object(StartVal_, Limit_, Step_),
_StartVal(&_Object._StartVal),
_Limit(&_Object._Limit),
_Step(&_Object._Step),
_StartValOffset(StartValOffset_),
_LimitOffset(LimitOffset_),
_StepOffset(StepOffset_),
_Condition(NULL),
_Next(NULL),
_Running(FALSE),
_InLoopBody(FALSE),
_LoopNum(0),
_IndexPtr(&_Index)
{}

/****************************************************************************/
template <class T>
AdvForLoop<T>* AdvForLoop<T>::Make()
{
  return (new AdvForLoop<T>);
}

/****************************************************************************/
template <class T>
AdvForLoop<T>* AdvForLoop<T>::Make(const Object& Obj_)
{
  AdvForLoopAcceptor<T> TrgComp_(NULL);
  Obj_.Clone(&TrgComp_);

  return TrgComp_._VarPtr;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>* AdvForLoop<T>::Make(T* StartVal_, T* Limit_, T* Step_,
				   T* StartValOffset_, T* LimitOffset_, T* StepOffset_,
				   Boolean* Cond_)
{
  return (new AdvForLoop<T>(StartVal_, Limit_, Step_, StartValOffset_, LimitOffset_, StepOffset_, Cond_));
}

/****************************************************************************/
template <class T>
AdvForLoop<T>* AdvForLoop<T>::Make(const T& StartVal_, const T& Limit_, const T& Step_,
				   T* StartValOffset_, T* LimitOffset_, T* StepOffset_)
{
  return (new AdvForLoop<T>(StartVal_, Limit_, Step_, StartValOffset_, LimitOffset_, StepOffset_));
}

/****************************************************************************/
template <class T>
int AdvForLoop<T>::ClassID() const
{
  return TypeInfo::ADVFORLOOP_TYPE;
}

/****************************************************************************/
template <class T>
const ObjectAcceptor* AdvForLoop<T>::GiveObjectAcceptor() const
{
  return NULL;
}

/****************************************************************************/
template <class T>
Object* AdvForLoop<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new AdvForLoop<T>(*this));

  AdvForLoopAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromAdvForLoop(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object& AdvForLoop<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  AdvForLoopAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromAdvForLoop(TrgComp_)));
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static AdvForLoop<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& AdvForLoop<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return AdvForLoop<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
Object& AdvForLoop<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return AdvForLoop<T>::GiveNullObject();
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::SetStartValuePtr(T* Ptr_, T* Offset_)
{
  _StartVal = Ptr_;
  _StartValOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::SetStartValue(const T& Val_, T* Offset_)
{
  _Object._StartVal = Val_;
  _StartVal = &_Object._StartVal;
  _StartValOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::AddLoopCondition(Boolean* Cond_)
{
  if (Cond_)
    _Condition = Cond_;

  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::SetIndexLimitPtr(T* Ptr_, T* Offset_)
{
  _Limit = Ptr_;
  _LimitOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::SetIndexLimit(const T& Val_, T* Offset_)
{
  _Object._Limit = Val_;
  _Limit = &_Object._Limit;
  _LimitOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::SetStepValuePtr(T* Ptr_, T* Offset_)
{
  _Step = Ptr_;
  _StepOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::SetStepValue(const T& Val_, T* Offset_)
{
  _Object._Step = Val_;
  _Step = &_Object._Step;
  _StepOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
void AdvForLoop<T>::Initialize()
{
  if (_StartVal)
  {
    _IndexPtr = &_Index;
    _Index = *_StartVal;
    if (_StartValOffset)
      _Index += *_StartValOffset;
  }

  _InLoopBody = FALSE;
  _PreLoopFncPtr.Reset();
  _PostLoopFncPtr.Reset();
}

/****************************************************************************/
template <class T>
Boolean AdvForLoop<T>::ConditionTrue()
{
  return ((_Limit ? (_Index < *_Limit + (_LimitOffset ? *_LimitOffset:0)):TRUE) && BoolCondition());
}

/****************************************************************************/
template <class T>
void AdvForLoop<T>::Increment()
{
  if (_StartVal && _Step)
  {
    _Index += *_Step;
    if (_StepOffset)
      _Index += *_StepOffset;
  }
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::DeepestLoop()
{
  if (_Next)
  {
    if (_Next->_Next)
      return _Next->DeepestLoop();
    return *_Next;
  }

  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::RemoveDeepestLoop()
{
  if (_Next)
    if (_Next->_Next)
      _Next->RemoveDeepestLoop();
    else
      _Next = NULL;

  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::RemoveNextLoop()
{
  _Next = NULL;
  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::AddLoop(AdvForLoop<T>* Next_)
{
  if (_Next)
    _Next->AddLoop(Next_);
  else
  {
    _Next = Next_;
    if (_Next)
      _Next->SetLoopNumber(_LoopNum + 1);
  }

  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::AddLoopBodyAsVoidPtr(void(*FuncPtr_)(AdvForLoop<T>*, void*), void* ExecBlock_, int Pos_)
{
  if (Pos_ == PRE_LOOP)
    _PreLoopFncPtr.InitWithVoidPtr(FuncPtr_, ExecBlock_);
  else
    _PostLoopFncPtr.InitWithVoidPtr(FuncPtr_, ExecBlock_);

  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::AddLoopBodyAsObject(void(*FuncPtr_)(AdvForLoop<T>*, Object*), Object* ExecBlock_, int Pos_)
{
  if (Pos_ == PRE_LOOP)
    _PreLoopFncPtr.InitWithObjectPtr(FuncPtr_, ExecBlock_);
  else
    _PostLoopFncPtr.InitWithObjectPtr(FuncPtr_, ExecBlock_);

  return *this;
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::AddLoopBodyAsObject(Object* ExecBlock_, int Pos_)
{
  return AddLoopBodyAsObject(_ForLoopBodyAsObject, ExecBlock_, Pos_);
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::ClearLoopBody(int Pos_)
{
  AddLoopBodyAsObject(NULL, Pos_);
  AddLoopBodyAsVoidPtr(NULL, NULL, Pos_);
  return *this;
}

/****************************************************************************/
template <class T>
Boolean AdvForLoop<T>::Run()
{
  if (!_Running)
    Initialize();

  while (_Running = ConditionTrue() || _InLoopBody)
  {
    if (!_InLoopBody || (_Next && _Next->IsInBody()))
    {
      _InLoopBody = TRUE;
      _PreLoopFncPtr.Run(this);
      Boolean IterDone_ = _Next ? _Next->Run():TRUE;

      if (IterDone_)
	return TRUE;
    }

    _PostLoopFncPtr.Run(this);
    Increment();
    _PreLoopFncPtr.Reset();
    _PostLoopFncPtr.Reset();
    _InLoopBody = FALSE;
  }

  _InLoopBody = FALSE;
  return FALSE;
}

/****************************************************************************/
// This method may be overrided in derived classes to take advantage of the
// information passed in as a parameter to the method. The loop object that
// is passed in is from an enclosing AdvForLoop object which has this object
// as part of its body.
//
template <class T>
Boolean AdvForLoop<T>::Run(const AdvForLoop<T>& Loop_)
{
  return Run();
}

/****************************************************************************/
template <class T>
void AdvForLoop<T>::ForLoopFncHelper(size_t LoopIndex_, size_t LastLoop_)
{
  T Index_ = _StartVal[LoopIndex_];
  if (LoopIndex_ == LastLoop_)
    _IndexPtr = &Index_;

  for (;Index_ < _Limit[LoopIndex_]; ++Index_)
    if (LoopIndex_ == LastLoop_)
    {
      _PreLoopFncPtr.Run(this);
      _PreLoopFncPtr.Reset();
    }
    else
      ForLoopFncHelper(LoopIndex_ + 1, LastLoop_);
}

/****************************************************************************/
template <class T>
AdvForLoop<T>& AdvForLoop<T>::ForLoopFnc(T* StartVals_, T* EndVals_, size_t Loops_)
{
  if (Loops_ > 0)
  {
    T* OldStart_ = GetStartValue();
    T* OldLimit_ = GetIndexLimit();
    T* OldIndex_ = GetIndex();

    SetStartValuePtr(StartVals_);
    SetIndexLimitPtr(EndVals_);
    ForLoopFncHelper(0, Loops_ - 1);

    _IndexPtr = OldIndex_;
    SetIndexLimitPtr(OldLimit_);
    SetStartValuePtr(OldStart_);
  }

  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* AdvForLoop<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void AdvForLoop<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* AdvForLoop<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void AdvForLoop<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
AdvForLoopAcceptor<T>::AdvForLoopAcceptor(const AdvForLoop<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
void AdvForLoopAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const AdvForLoop<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* AdvForLoopAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
Boolean AdvForLoopAcceptor<T>::IsAdvForLoop() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void* AdvForLoopAcceptor<T>::CreateFromAdvForLoop(const ObjectAcceptor& Obj_)
{
  _VarPtr = (new AdvForLoop<T>(*((const AdvForLoop<T>*)Obj_.GiveDataPtr())));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* AdvForLoopAcceptor<T>::AssignFromAdvForLoop(const ObjectAcceptor& Obj_)
{
  _VarPtr->Run(*((const AdvForLoop<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* AdvForLoopAcceptor<T>::CreateFromNullObject(const ObjectAcceptor&)
{
  _VarPtr = (AdvForLoop<T>*)(_VarPtr->NULLOBJECT().Clone());
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* AdvForLoopAcceptor<T>::AssignFromNullObject(const ObjectAcceptor&)
{
  *((ObjectImp*)_VarPtr) = (ObjectImp&)(_VarPtr->NULLOBJECT());
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean AdvForLoopAcceptor<T>::IsEqualToNullObject(const ObjectAcceptor&) const
{
  return _ObjPtr->IsNullObject();
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* AdvForLoopAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void AdvForLoopAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif

/****************************************************************************/
#if DEBUG_ADVFORLOOP
#include <assert.h>
#if defined(__TURBOC__)
  #include <conio.h>
#endif
//#include "shortint.h"
template <class T>
class Viewer;

template <class T>
void Work(AdvForLoop<T>* LoopPtr_, void* ExecBlock_)
{
  ((Viewer<T>*)ExecBlock_)->Accept(LoopPtr_);
}

template <class T>
class Viewer
{
  private:
    long _Count;

  public:
    Viewer(): _Count(0) {}
    long Cnt() const
	{ return _Count; }
    void Accept(AdvForLoop<T>* LoopPtr_)
	{ cout <<*LoopPtr_->GetIndex() <<" ";
	  ++_Count; }
};

int main()
{
  int cnt;
#if defined(__TURBOC__)
  clrscr();
#endif

  // /* select sort loop */
  // int lim = 20;
  // for (int x = 0; x < lim - 1; ++x)
  //   for (int y = x + 1; y < lim; ++y)
  //	 /* Body of Loop should execute (19 * 10) / 2 = 190 times */
  int Lim_ = 20;
  int lp2startoff = 1;
  int lp1indexoff = -1;
  Viewer<int> View_;
  Viewer<int> View2_;

  AdvForLoop<int> Loop1_(0, Lim_ - 1, 1);
  Loop1_
    .SetIndexLimitPtr(&Lim_, &lp1indexoff)
    .AddLoopBodyAsVoidPtr(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBodyAsVoidPtr(Work, &View2_, ForLoopEnums::POST_LOOP);

  AdvForLoop<int> Loop2_(0, Lim_, 1);
  Loop2_
    .SetStartValuePtr(Loop1_.GetIndex(), &lp2startoff)
    .SetIndexLimitPtr(&Lim_)
    .AddLoopBodyAsVoidPtr(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBodyAsVoidPtr(Work, &View2_, ForLoopEnums::POST_LOOP);

  Loop1_.AddLoop(&Loop2_);

  for (cnt = 0; Loop1_.Run(); ++cnt);
  Loop1_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  Loop2_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  assert(cnt == 190);
  cout <<endl <<endl;

  // /* 4x4 matrix multiplication loop (incomplete) */
  // int lim = 4
  // for (int i = 0; i < lim; ++i)
  //   for (int j = 0; j < lim; ++j)
  //     for (int k = 0; k < lim; ++k)
  //       /* Body of Loop should execute 64 times */
  AdvForLoop<int> Loop3_;
  Lim_ = 4;
  Loop1_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_);
  Loop2_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_);
  Loop3_
    .SetStartValue(0)
    .SetIndexLimitPtr(&Lim_)
    .SetStepValue(1);
  Loop2_.AddLoop(&Loop3_);
  for (cnt = 0; Loop1_.Run(); ++cnt);
  cout <<cnt <<endl;
  assert(cnt == 64);

  // /* 8! (The hard & stupid way) */
  // int m = 0;
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         for (int g = 0; g < 4; ++g)
  //           for (int h = 0; h < 3; ++h)
  //             for (int i = 0; i < 2; ++i)
  //               ++m; /* final value should be 40320 */
  AdvForLoop<long> LoopArr_[7];
  int i; long Step_ = 1;
  for (i = 0, cnt = 8; cnt > 1; --cnt, ++i)
  {
    LoopArr_[i]
      .SetStartValue(0)
      .SetIndexLimit(cnt)
      .SetStepValue(Step_);

    if (i)
      LoopArr_[i - 1].AddLoop(&LoopArr_[i]);
  }
  long longcnt;
  for (longcnt = 0; LoopArr_[0].Run(); ++longcnt);
  cout <<longcnt <<endl;
  assert(longcnt == 40320);

  // /* Test standalone loop function   */
  // /* 8! / 4! = P(8,4) 		*/
  // int m = 0
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         ++m; /* final value should be 1680 */
  Viewer<int> View3_;
  int Startv_[4] = { 0, 0, 0, 0 };
  int Endv_[4] = { 8, 7, 6, 5 };

  Loop1_
    .AddLoopBodyAsVoidPtr(Work, &View3_, ForLoopEnums::PRE_LOOP);
  Loop1_.ForLoopFnc(Startv_, Endv_, 4);
  cout <<endl <<View3_.Cnt() <<endl;

  return 0;
}
#endif
/****************************************************************************/
#endif





