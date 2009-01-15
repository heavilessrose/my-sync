// Listing 1: ForLoop.h. Definition of the ForLoop class
//
#ifndef FORLOOP_H
#define FORLOOP_H
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

#define DEBUG_FORLOOP	0

/****************************************************************************/
struct ForLoopEnums
{
  public:
    enum { PRE_LOOP, POST_LOOP };
};

template <class T>
class ForLoop : public ForLoopEnums
{
  public:
    typedef void(*WorkFunc)(ForLoop<T>*, void*);

  protected:
    struct ObjectRep
    {
      T _StartVal;
      T _Limit;
      T _Step;

      ObjectRep() {}
      ObjectRep(const T& StartVal_, const T& Limit_, const T& Step_):
	_StartVal(StartVal_),
	_Limit(Limit_),
	_Step(Step_) {}
    };

    struct ForLoopFnPtr
    {
      void* _ExecBlock;
      void(*_FuncPtr)(ForLoop<T>*, void*);
      Boolean _Done;

      ForLoopFnPtr():
	_Done(FALSE),
	_FuncPtr(NULL) {}

      void Reset()
	{ _Done = FALSE; }
      void Init(void(*FuncPtr_)(ForLoop<T>*, void*), void* Data_)
	{ _FuncPtr = FuncPtr_;
	  _ExecBlock = Data_; }
      void Run(ForLoop<T>* LoopPtr_)
	{ if (!_Done && _FuncPtr)
	    (*_FuncPtr)(LoopPtr_, _ExecBlock);
	  _Done = TRUE; }
    };

    ObjectRep _Object;

    T* _StartVal;
    T* _Limit;
    T* _Step;
    T* _StartValOffset;
    T* _LimitOffset;
    T* _StepOffset;
    Boolean* _Condition;

    ForLoopFnPtr _PreLoopFncPtr;
    ForLoop<T>* _Next;
    ForLoopFnPtr _PostLoopFncPtr;

    Boolean _Running;
    Boolean _InLoopBody;
    size_t _LoopNum;
    T _Index;
    T* _IndexPtr;

    void SetLoopNumber(size_t LoopNum_)
	{ _LoopNum = LoopNum_; }
    Boolean BoolCondition() const
	{ return (_Condition ? *_Condition:TRUE); }

    // standalone loop function helper
    void ForLoopFncHelper(size_t LoopIndex_, size_t LastLoop_);

  public:
    ForLoop();
    ForLoop(T* StartVal_, T* Limit_, T* Step_,
	    T* StartValOffset_=NULL, T* LimitOffset_=NULL, T* StepOffset_=NULL,
	    Boolean* Cond_=NULL);
    ForLoop(const T& StartVal_, const T& Limit_, const T& Step_,
	    T* StartValOffset_=NULL, T* LimitOffset_=NULL, T* StepOffset_=NULL);

    // set starting index values
    ForLoop<T>& SetStartValuePtr(T* Ptr_, T* Offset_=NULL);
    ForLoop<T>& SetStartValue(const T& Val_, T* Offset_=NULL);

    // Set terminating conditions
    ForLoop<T>& AddLoopCondition(Boolean* Cond_);

    // Set index limits
    ForLoop<T>& SetIndexLimitPtr(T* Ptr_, T* Offset_=NULL);
    ForLoop<T>& SetIndexLimit(const T& Val_, T* Offset_=NULL);

    // Set loop iteration step value
    ForLoop<T>& SetStepValuePtr(T* Ptr_, T* Offset_=NULL);
    ForLoop<T>& SetStepValue(const T& Val_, T* Offset_=NULL);

    // Loop data accessors
    T* GetIndex()
	{ return _IndexPtr; }
    T* GetStartValue()
	{ return _StartVal; }
    T* GetIndexLimit()
	{ return _Limit; }
    T* GetStepValue()
	{ return _Step; }
    T* GetStartValueOffset()
	{ return _StartValOffset; }
    T* GetIndexLimitOffset()
	{ return _LimitOffset; }
    T* GetStepValueOffset()
	{ return _StepOffset; }
    Boolean* GetLoopCondition()
	{ return _Condition; }

    Boolean Initialized() const
	{ return (_StartVal != NULL && _Limit != NULL && _Step != NULL); }
    Boolean IsRunning() const
	{ return _Running; }
    Boolean IsInBody() const
	{ return _InLoopBody; }

    // looping constucts
    void Initialize();
    Boolean ConditionTrue();
    void Increment();

    // Add next for loop
    ForLoop<T>& AddLoop(ForLoop<T>* Next_);

    // Loop Removal methods
    ForLoop<T>& RemoveNextLoop();
    ForLoop<T>& RemoveDeepestLoop();

    // Loop Accessor methods
    ForLoop<T>& DeepestLoop();
    ForLoop<T>& NextLoop()
	{ return (_Next ? *_Next:*this); }

    // Add/Clear loop body function ptrs
    ForLoop<T>& AddLoopBody(void(*FuncPtr_)(ForLoop<T>*, void*), void* ExecBlock_, int Pos_);
    ForLoop<T>& ClearLoopBody(int Pos_)
	{ return AddLoopBody(NULL, NULL, Pos_); }

    // Loop Number
    size_t GetLoopNumber() const
	{ return _LoopNum; }

    // Execute loops
    Boolean Run();

    // Standalone looping function
    ForLoop<T>& ForLoopFnc(T* StartVals_, T* EndVals_, size_t Loops_);
};

/****************************************************************************/
#endif




