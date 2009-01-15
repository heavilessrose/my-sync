// Listing 2: ForLoop.cpp. Multiple for loop class
//
#ifndef FORLOOP_CPP
#define FORLOOP_CPP
#ifndef FORLOOP_H
  #include "forloop.h"
#endif

/****************************************************************************/
template <class T>
ForLoop<T>::ForLoop():
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
ForLoop<T>::ForLoop(T* StartVal_, T* Limit_, T* Step_,
		    T* StartValOffset_, T* LimitOffset_, T* StepOffset_,
		    Boolean* Cond_):
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
ForLoop<T>::ForLoop(const T& StartVal_, const T& Limit_, const T& Step_,
		    T* StartValOffset_, T* LimitOffset_, T* StepOffset_):
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
ForLoop<T>& ForLoop<T>::SetStartValuePtr(T* Ptr_, T* Offset_)
{
  _StartVal = Ptr_;
  _StartValOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
ForLoop<T>& ForLoop<T>::SetStartValue(const T& Val_, T* Offset_)
{
  _Object._StartVal = Val_;
  _StartVal = &_Object._StartVal;
  _StartValOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
ForLoop<T>& ForLoop<T>::AddLoopCondition(Boolean* Cond_)
{
  if (Cond_)
    _Condition = Cond_;

  return *this;
}

/****************************************************************************/
template <class T>
ForLoop<T>& ForLoop<T>::SetIndexLimitPtr(T* Ptr_, T* Offset_)
{
  _Limit = Ptr_;
  _LimitOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
ForLoop<T>& ForLoop<T>::SetIndexLimit(const T& Val_, T* Offset_)
{
  _Object._Limit = Val_;
  _Limit = &_Object._Limit;
  _LimitOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
ForLoop<T>& ForLoop<T>::SetStepValuePtr(T* Ptr_, T* Offset_)
{
  _Step = Ptr_;
  _StepOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
ForLoop<T>& ForLoop<T>::SetStepValue(const T& Val_, T* Offset_)
{
  _Object._Step = Val_;
  _Step = &_Object._Step;
  _StepOffset = Offset_;
  return *this;
}

/****************************************************************************/
template <class T>
void ForLoop<T>::Initialize()
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
Boolean ForLoop<T>::ConditionTrue()
{
  return ((_Limit ? (_Index < *_Limit + (_LimitOffset ? *_LimitOffset:0)):TRUE) && BoolCondition());
}

/****************************************************************************/
template <class T>
void ForLoop<T>::Increment()
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
ForLoop<T>& ForLoop<T>::DeepestLoop()
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
ForLoop<T>& ForLoop<T>::RemoveDeepestLoop()
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
ForLoop<T>& ForLoop<T>::RemoveNextLoop()
{
  _Next = NULL;
  return *this;
}

/****************************************************************************/
template <class T>
ForLoop<T>& ForLoop<T>::AddLoop(ForLoop<T>* Next_)
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
ForLoop<T>& ForLoop<T>::AddLoopBody(void(*FuncPtr_)(ForLoop<T>*, void*), void* ExecBlock_, int Pos_)
{
  if (Pos_ == PRE_LOOP)
    _PreLoopFncPtr.Init(FuncPtr_, ExecBlock_);
  else
    _PostLoopFncPtr.Init(FuncPtr_, ExecBlock_);

  return *this;
}

/****************************************************************************/
template <class T>
Boolean ForLoop<T>::Run()
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
template <class T>
void ForLoop<T>::ForLoopFncHelper(size_t LoopIndex_, size_t LastLoop_)
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
ForLoop<T>& ForLoop<T>::ForLoopFnc(T* StartVals_, T* EndVals_, size_t Loops_)
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
#if DEBUG_FORLOOP
#include <assert.h>
#if defined(__TURBOC__)
  #include <conio.h>
#endif
template <class T>
class Viewer;

template <class T>
void Work(ForLoop<T>* LoopPtr_, void* ExecBlock_)
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
    void Accept(ForLoop<T>* LoopPtr_)
	{ cout <<*LoopPtr_->GetIndex() <<" ";
	  ++_Count; }
};

int main()
{
  int cnt;
#if defined(__TURBOC__)  
  clrscr();
#endif

  // select sort loop
  // int lim = 20;
  // for (int x = 0; x < lim - 1; ++x)
  //   for (int y = x + 1; y < lim; ++y)
  //	 // Body of Loop should execute (19 * 10) / 2 = 190 times
  int Lim_ = 20;
  int lp2startoff = 1;
  int lp1indexoff = -1;
  Viewer<int> View_;
  Viewer<int> View2_;

  ForLoop<int> Loop1_(0, Lim_ - 1, 1);
  Loop1_
    .SetIndexLimitPtr(&Lim_, &lp1indexoff)
    .AddLoopBody(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBody(Work, &View2_, ForLoopEnums::POST_LOOP);

  ForLoop<int> Loop2_(0, Lim_, 1);
  Loop2_
    .SetStartValuePtr(Loop1_.GetIndex(), &lp2startoff)
    .SetIndexLimitPtr(&Lim_)
    .AddLoopBody(Work, &View_, ForLoopEnums::PRE_LOOP)
    .AddLoopBody(Work, &View2_, ForLoopEnums::POST_LOOP);

  Loop1_.AddLoop(&Loop2_);

  for (cnt = 0; Loop1_.Run(); ++cnt);
  Loop1_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  Loop2_
    .ClearLoopBody(ForLoopEnums::PRE_LOOP)
    .ClearLoopBody(ForLoopEnums::POST_LOOP);
  cout <<cnt <<endl;      
  assert(cnt == 190);

  // 4x4 matrix multiplication loop (incomplete)
  // int lim = 4
  // for (int i = 0; i < lim; ++i)
  //   for (int j = 0; j < lim; ++j)
  //     for (int k = 0; k < lim; ++k)
  //       // Body of Loop should execute 64 times
  ForLoop<int> Loop3_;
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

  // 8! (The hard & stupid way)
  // int m = 0;
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         for (int g = 0; g < 4; ++g)
  //           for (int h = 0; h < 3; ++h)
  //             for (int i = 0; i < 2; ++i)
  //               ++m; // final value should be 40320
  ForLoop<long> LoopArr_[7];
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

  // Test standalone loop function
  // 8! / 4! = P(8,4)
  // int m = 0
  // for (int c = 0; c < 8; ++c)
  //   for (int d = 0; d < 7; ++d)
  //     for (int e = 0; e < 6; ++e)
  //       for (int f = 0; f < 5; ++f)
  //         ++m; // final value should be 1680
  Viewer<int> View3_;
  int Startv_[4] = { 0, 0, 0, 0 };
  int Endv_[4] = { 8, 7, 6, 5 };

  Loop1_
    .AddLoopBody(Work, &View3_, ForLoopEnums::PRE_LOOP);
  Loop1_.ForLoopFnc(Startv_, Endv_, 4);
  cout <<endl <<View3_.Cnt() <<endl;

  return 0;
}
#endif
/****************************************************************************/
#endif





