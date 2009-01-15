// Listing 1: AdvForLoop.h. Definition of the AdvForLoop class
//
#ifndef ADVFORLOOP_H
#define ADVFORLOOP_H
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef FORLOOP_H
  #include "forloop.h"
#endif

#define DEBUG_ADVFORLOOP	0

/****************************************************************************/
template <class T>
class AdvForLoop;

template <class T>
class AdvForLoopAcceptor : public ObjectAcceptor
{
  friend class AdvForLoop<T>;

  protected:
    union
    {
      const AdvForLoop<T>* _ObjPtr;
      AdvForLoop<T>* _VarPtr;
    };

    AdvForLoopAcceptor(const AdvForLoop<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // AdvForLoop class specific acceptor methods
    virtual Boolean IsAdvForLoop() const;
    virtual void* CreateFromAdvForLoop(const ObjectAcceptor& Obj_);
    virtual void* AssignFromAdvForLoop(const ObjectAcceptor& Obj_);

    virtual void* CreateFromNullObject(const ObjectAcceptor&);
    virtual void* AssignFromNullObject(const ObjectAcceptor&);
    virtual Boolean IsEqualToNullObject(const ObjectAcceptor&) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class AdvForLoop : public ObjectImp, public ForLoopEnums
{
  friend Object& RootObject(AdvForLoop<T>& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const AdvForLoop<T>& Obj_)
	{ return *((const Object*)(&Obj_)); }

  public:
    typedef void(*VoidWorkFunc)(AdvForLoop<T>*, void*);
    typedef void(*ObjectWorkFunc)(AdvForLoop<T>*, Object*);

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
      Object* _ObjectPtr;

      void(*_FuncPtr)(AdvForLoop<T>*, void*);
      void(*_ObjectFuncPtr)(AdvForLoop<T>*, Object*);
      Boolean _Done;

      ForLoopFnPtr():
	_ExecBlock(NULL),
	_ObjectPtr(NULL),
	_FuncPtr(NULL),
	_ObjectFuncPtr(NULL),
	_Done(FALSE) {}

      void Reset()
	{ _Done = FALSE; }

      void InitWithObjectPtr(void(*ObjectFuncPtr_)(AdvForLoop<T>*, Object*), Object* Data_)
	{ _FuncPtr = NULL;
	  _ObjectFuncPtr = ObjectFuncPtr_;
	  _ObjectPtr = Data_;
	  _ExecBlock = NULL; }

      void InitWithVoidPtr(void(*FuncPtr_)(AdvForLoop<T>*, void*), void* Data_)
	{ _FuncPtr = FuncPtr_;
	  _ObjectFuncPtr = NULL;
	  _ObjectPtr = NULL;
	  _ExecBlock = Data_; }

      void Run(AdvForLoop<T>* LoopPtr_)
	{ if (!_Done)
	    if (_FuncPtr)
	      (*_FuncPtr)(LoopPtr_, _ExecBlock);
	    else if (_ObjectFuncPtr)
	      (*_ObjectFuncPtr)(LoopPtr_, _ObjectPtr);
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
    AdvForLoop<T>* _Next;
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
    AdvForLoop();
    AdvForLoop(T* StartVal_, T* Limit_, T* Step_,
	    T* StartValOffset_=NULL, T* LimitOffset_=NULL, T* StepOffset_=NULL,
	    Boolean* Cond_=NULL);
    AdvForLoop(const T& StartVal_, const T& Limit_, const T& Step_,
	    T* StartValOffset_=NULL, T* LimitOffset_=NULL, T* StepOffset_=NULL);

    static AdvForLoop<T>* Make();
    static AdvForLoop<T>* Make(const Object& Obj_);
    static AdvForLoop<T>* Make(T* StartVal_, T* Limit_, T* Step_,
	    T* StartValOffset_=NULL, T* LimitOffset_=NULL, T* StepOffset_=NULL,
	    Boolean* Cond_=NULL);
    static AdvForLoop<T>* Make(const T& StartVal_, const T& Limit_, const T& Step_,
	    T* StartValOffset_=NULL, T* LimitOffset_=NULL, T* StepOffset_=NULL);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::ADVFORLOOP_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Static null object accessor methods
    static AdvForLoop<T>& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // set starting index values
    AdvForLoop<T>& SetStartValuePtr(T* Ptr_, T* Offset_=NULL);
    AdvForLoop<T>& SetStartValue(const T& Val_, T* Offset_=NULL);

    // Set terminating conditions
    AdvForLoop<T>& AddLoopCondition(Boolean* Cond_);

    // Set index limits
    AdvForLoop<T>& SetIndexLimitPtr(T* Ptr_, T* Offset_=NULL);
    AdvForLoop<T>& SetIndexLimit(const T& Val_, T* Offset_=NULL);

    // Set loop iteration step value
    AdvForLoop<T>& SetStepValuePtr(T* Ptr_, T* Offset_=NULL);
    AdvForLoop<T>& SetStepValue(const T& Val_, T* Offset_=NULL);

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
    AdvForLoop<T>& AddLoop(AdvForLoop<T>* Next_);

    // Loop Removal methods
    AdvForLoop<T>& RemoveNextLoop();
    AdvForLoop<T>& RemoveDeepestLoop();

    // Loop Accessor methods
    AdvForLoop<T>& DeepestLoop();
    AdvForLoop<T>& NextLoop()
	{ return (_Next ? *_Next:*this); }

    // Add/Clear loop body function ptrs
    AdvForLoop<T>& AddLoopBodyAsVoidPtr(void(*FuncPtr_)(AdvForLoop<T>*, void*), void* ExecBlock_, int Pos_);
    AdvForLoop<T>& AddLoopBodyAsObject(void(*FuncPtr_)(AdvForLoop<T>*, Object*), Object* ExecBlock_, int Pos_);
    AdvForLoop<T>& AddLoopBodyAsObject(Object* ExecBlock_, int Pos_);
    AdvForLoop<T>& ClearLoopBody(int Pos_);

    // Loop Number
    size_t GetLoopNumber() const
        { return _LoopNum; }

    // Execute loops
    virtual Boolean Run();
    virtual Boolean Run(const AdvForLoop<T>& Loop_);

    // Standalone looping function
    AdvForLoop<T>& ForLoopFnc(T* StartVals_, T* EndVals_, size_t Loops_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

// Default callback function for executing body of the for loop as an object.
// Uses the virtual assignment method of the object class to manipulate the
// enclosing loop object polymorphically
//
template <class T>
void _ForLoopBodyAsObject(AdvForLoop<T>* LoopPtr_, Object* ExecBlock_)
{
  ExecBlock_->Assign(*((Object*)LoopPtr_));
}

/****************************************************************************/
#endif




