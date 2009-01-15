#ifndef SMARTPTR_H
#define SMARTPTR_H
#ifndef CONSTPTR_CPP
  #include "constptr.cpp"
#endif

#define SMARTPTR_DEBUG		0

/****************************************************************************/
template <class T>
class CopiedVoidPtr : public ConstCopiedVoidPtr<T>
{
  friend ConstCopiedVoidPtr<T>& RootObject(CopiedVoidPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)&Obj_); }
  friend const ConstCopiedVoidPtr<T>& RootConstObject(const CopiedVoidPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)&Obj_); }

  public:
    CopiedVoidPtr(ObjectUpdater* Deleter_=NULL);
    CopiedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_);
    CopiedVoidPtr(const T& obj, ObjectUpdater* Deleter_);
    CopiedVoidPtr(const ConstCopiedVoidPtr<T>& Obj_);  // Copy heap object

    virtual CopiedVoidPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::COPIEDVOIDPTR_TYPE; }
    virtual int ClassID() const;                

    CopiedVoidPtr<T>& operator = (T* rhs);
    CopiedVoidPtr<T>& operator = (const CopiedVoidPtr<T>& Obj_);

    virtual const T* Pointee() const;    
    virtual T* Pointee();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class T>
class CopiedBuiltInPtr : public ConstCopiedBuiltInPtr<T>
{
  friend ConstCopiedVoidPtr<T>& RootObject(CopiedBuiltInPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)&Obj_); }
  friend const ConstCopiedVoidPtr<T>& RootConstObject(const CopiedBuiltInPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)&Obj_); }

  public:
    CopiedBuiltInPtr();
    CopiedBuiltInPtr(T* just_newed);
    CopiedBuiltInPtr(const T& obj);                         	// Copy object
    CopiedBuiltInPtr(const ConstCopiedBuiltInPtr<T>& Obj_);	// Copy heap object

    virtual CopiedBuiltInPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::COPIEDBUILTINPTR_TYPE; }
    virtual int ClassID() const;                    

    CopiedBuiltInPtr<T>& operator = (T* rhs);                          		// Delete then assign to new pointer.
    CopiedBuiltInPtr<T>& operator = (const CopiedBuiltInPtr<T>& rhs);  	// Copy heap object

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual T* ReleaseControl();     // Given up lifetime control; no delete.

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template<class T>
class CopiedObjPtr : public ConstCopiedObjPtr<T>
{
  friend ConstCopiedVoidPtr<T>& RootObject(CopiedObjPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)&Obj_); }
  friend const ConstCopiedVoidPtr<T>& RootConstObject(const CopiedObjPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)&Obj_); }

  public:
    CopiedObjPtr();
    CopiedObjPtr(T* just_newed);
    CopiedObjPtr(const T& obj);                             // Copy object
    CopiedObjPtr(const ConstCopiedObjPtr<T>& p);

    virtual CopiedObjPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::COPIEDOBJPTR_TYPE; }
    virtual int ClassID() const;                    

    CopiedObjPtr<T>& operator = (T* rhs);
    CopiedObjPtr<T>& operator = (const CopiedObjPtr<T>& rhs);

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual T* Pointee();
    virtual const T* Pointee() const;

    virtual T* operator-> ();
    virtual const T* operator-> () const;

    virtual T* ReleaseControl();     // Given up lifetime control; no delete.

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
/****************************************************************************/
template <class T>
class VoidPtrClass
{  
  public:
    virtual const ConstCountedVoidPtr<T>& GiveBase() const = 0;
    virtual ConstCountedVoidPtr<T>& GiveBase() = 0;  
  
    virtual const T* ConstPointee() const = 0;
    virtual const T* Pointee() const = 0;
    virtual T* Pointee() = 0;
};

/****************************************************************************/
template <class T>
class CountedVoidPtr : public ConstCountedVoidPtr<T>, public VoidPtrClass<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(CountedVoidPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const CountedVoidPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  protected:
    CountedVoidPtr(const ConstCountedVoidPtr<T>& Obj_);

  public:
    CountedVoidPtr(ObjectUpdater* Deleter_=NULL);
    CountedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_, Boolean OwnsPtr_=TRUE);
    CountedVoidPtr(const CountedVoidPtr<T>& Obj_);
    CountedVoidPtr(const VoidPtrClass<T>& Obj_);  // Copy heap object

    virtual CountedVoidPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::COUNTEDVOIDPTR_TYPE; }
    virtual int ClassID() const;                    

    static CountedVoidPtr<T> CastAwayConst(const ConstCountedVoidPtr<T>& Obj_);
    CountedVoidPtr<T>& operator = (VoidPtrClass<T>& Obj_);
    CountedVoidPtr<T>& operator = (T* rhs);

    virtual const ConstCountedVoidPtr<T>& GiveBase() const;
    virtual ConstCountedVoidPtr<T>& GiveBase();

    virtual const T* ConstPointee() const;
    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual T* ReleaseControl();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class T>
class CountedBuiltInPtr : public ConstCountedBuiltInPtr<T>, public VoidPtrClass<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(CountedBuiltInPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const CountedBuiltInPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  protected:
    CountedBuiltInPtr(const ConstCountedBuiltInPtr<T>& Obj_);

  public:
    CountedBuiltInPtr();
    CountedBuiltInPtr(T* just_newed, Boolean OwnsPtr_=TRUE);
    CountedBuiltInPtr(const CountedBuiltInPtr<T>& Obj_);

    virtual CountedBuiltInPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::COUNTEDBUILTINPTR_TYPE; }
    virtual int ClassID() const;                    

    static CountedBuiltInPtr<T> CastAwayConst(const ConstCountedBuiltInPtr<T>& Obj_);
    CountedBuiltInPtr<T>& operator = (CountedBuiltInPtr<T>& rhs);
    CountedBuiltInPtr<T>& operator = (T* just_newed);

    virtual const ConstCountedVoidPtr<T>& GiveBase() const;
    virtual ConstCountedVoidPtr<T>& GiveBase();

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual T* Pointee();
    virtual const T* Pointee() const;
    virtual const T* ConstPointee() const;

    virtual T* ReleaseControl();                             // Given up lifetime control; no delete.

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class T>
class CountedObjPtr : public ConstCountedObjPtr<T>, public VoidPtrClass<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(CountedObjPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const CountedObjPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  protected:
    CountedObjPtr(const ConstCountedObjPtr<T>& Obj_);

  public:
    CountedObjPtr();
    CountedObjPtr(T* just_newed, Boolean OwnsPtr_=TRUE);
    CountedObjPtr(const CountedObjPtr<T>& Obj_);

    virtual CountedObjPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::COUNTEDOBJPTR_TYPE; }
    virtual int ClassID() const;                        

    static CountedObjPtr<T> CastAwayConst(const ConstCountedObjPtr<T>& Obj_);
    CountedObjPtr<T>& operator = (CountedObjPtr<T>& rhs);
    CountedObjPtr<T>& operator = (T* just_newed);

    virtual const ConstCountedVoidPtr<T>& GiveBase() const;
    virtual ConstCountedVoidPtr<T>& GiveBase();

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual T* Pointee();
    virtual const T* Pointee() const;
    virtual const T* ConstPointee() const;

    virtual T* operator-> ();
    virtual const T* operator-> () const;

    virtual T* ReleaseControl();                             // Given up lifetime control; no delete.

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class T>
class UpdatePtr : public ConstUpdatePtr<T>, public VoidPtrClass<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(UpdatePtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const UpdatePtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  protected:
    UpdatePtr(const ConstUpdatePtr<T>& Obj_);

  public:
    UpdatePtr(ObjectUpdater* Deleter_=NULL,
              ObjectUpdater* Updater_=NULL,
              ObjectUpdater* Validator_=NULL);
    UpdatePtr(const VoidPtrClass<T>& Obj_,
              ObjectUpdater* Updater_,
              ObjectUpdater* Validator_=NULL);
    UpdatePtr(const UpdatePtr<T>& Obj_);    
    virtual ~UpdatePtr();

    virtual UpdatePtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::UPDATEPTR_TYPE; }
    virtual int ClassID() const;                        

    static UpdatePtr<T> CastAwayConst(const ConstUpdatePtr<T>& Obj_);
    UpdatePtr<T>& operator = (UpdatePtr<T>& rhs);
    UpdatePtr<T>& operator = (T* just_newed);
    UpdatePtr<T>& CopyBindings(UpdatePtr<T>& rhs);

    virtual const ConstCountedVoidPtr<T>& GiveBase() const;
    virtual ConstCountedVoidPtr<T>& GiveBase();

    UpdatePtr<T>& SetUpdatablePtr(VoidPtrClass<T>& Obj_,
                                  ObjectUpdater* Updater_,
                                  ObjectUpdater* Validator_);
    UpdatePtr<T>& SetUpdater(ObjectUpdater* Updater_);
    UpdatePtr<T>& SetValidator(ObjectUpdater* Validator_);

    virtual const T* ConstPointee() const;
    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual T* ReleaseControl();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class T>
class ArrayPtr : public ConstArrayPtr<T>, public VoidPtrClass<T>
{
  friend ArrayPtr<T>& RootObject(ArrayPtr<T>& Obj_)
	{ return Obj_; }
  friend const ArrayPtr<T>& RootConstObject(const ArrayPtr<T>& Obj_)
	{ return Obj_; }

  friend Boolean _IsGreaterThan(const ArrayPtr<T>& Trg_, const ArrayPtr<T>& Src_)
	{ return Trg_.IsGreater(Src_); }
  friend Boolean _IsLessThan(const ArrayPtr<T>& Trg_, const ArrayPtr<T>& Src_)
	{ return Trg_.IsLesser(Src_); }
  friend Boolean _IsEqualTo(const ArrayPtr<T>& Trg_, const ArrayPtr<T>& Src_)
	{ return Trg_.IsEqual(Src_); }

  friend const ArrayPtr<T>& _Increment(ArrayPtr<T>& Trg_)
        { return (++Trg_); }
  friend const ArrayPtr<T>& _Decrement(ArrayPtr<T>& Trg_)
        { return (++Trg_); }

  friend int _WhatIs(const ArrayPtr<T>& Obj_)
	{ return Obj_.ClassID(); }

  friend ArrayPtr<T>* _Clone(const ArrayPtr<T>& Obj_)
	{ return ((ArrayPtr<T>*)Obj_.Clone()); }
  friend const ArrayPtr<T>* _ConstClone(const ArrayPtr<T>& Obj_)
	{ return ((const ArrayPtr<T>*)Obj_.ShallowClone()); }
  friend ArrayPtr<T>* _NonConstClone(ArrayPtr<T>& Obj_)
	{ return ((ArrayPtr<T>*)Obj_.ShallowClone()); }

  // Object properties functions
  friend Boolean _IsOrdinal(const ArrayPtr<T>& Obj_)
	{ return TRUE; }
  friend Boolean _IsSortable(const ArrayPtr<T>& Obj_)
	{ return TRUE; }
  friend Boolean _IsMatchable(const ArrayPtr<T>& Obj_)
	{ return TRUE; }

  protected:
    ArrayPtr(const ConstArrayPtr<T>& Obj_);

  public:
    ArrayPtr(ObjectUpdater* Deleter_=NULL);
    ArrayPtr(const ArrayPtr<T>& Obj_);
    ArrayPtr(const VoidPtrClass<T>& Obj_, size_t Size_=1, T* Base_=NULL);

    virtual ArrayPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::ARRAYPTR_TYPE; }
    virtual int ClassID() const;                        

    static ArrayPtr<T> CastAwayConst(const ConstArrayPtr<T>& Obj_);
    ArrayPtr<T>& operator = (ArrayPtr<T>& rhs);
    ArrayPtr<T>& operator = (T* just_newed);

    virtual const ConstCountedVoidPtr<T>& GiveBase() const;
    virtual ConstCountedVoidPtr<T>& GiveBase();

    virtual const T* ConstPointee() const;
    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual T* ReleaseControl();

    ArrayPtr<T> operator + (ptrdiff_t Index_);
    ArrayPtr<T> operator - (ptrdiff_t Index_);

    virtual T& operator * ();
    virtual const T& operator * () const;
    virtual T& operator [] (ptrdiff_t Index_);
    virtual const T& operator [] (ptrdiff_t Index_) const;

    ArrayPtr<T>& operator ++ ();
    ArrayPtr<T> operator ++ (int);

    ArrayPtr<T>& operator -- ();
    ArrayPtr<T> operator -- (int);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class T>
class StaticObjPtr : public ConstStaticObjPtr<T>, public VoidPtrClass<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(StaticObjPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const StaticObjPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  protected:
    StaticObjPtr(const ConstStaticObjPtr<T>& Obj_);

  public:
    StaticObjPtr();			// Construct as null pointer
    StaticObjPtr(T* just_newed);        // Construct pointing at heap object

    virtual StaticObjPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::STATICOBJPTR_TYPE; }
    virtual int ClassID() const;                        

    static StaticObjPtr<T> CastAwayConst(const ConstStaticObjPtr<T>& Obj_);
    StaticObjPtr<T>& operator = (StaticObjPtr<T>& rhs);
    StaticObjPtr<T>& operator = (T* just_newed);

    virtual const ConstCountedVoidPtr<T>& GiveBase() const;
    virtual ConstCountedVoidPtr<T>& GiveBase();

    StaticObjPtr<T>& Increment();
    StaticObjPtr<T>& Decrement();

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual T* Pointee();
    virtual const T* Pointee() const;
    virtual const T* ConstPointee() const;

    virtual T* operator-> ();
    virtual const T* operator-> () const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
class ProxyPtr : public ConstProxyPtr<CLIENT, SPONSOR, LETTER>
{
  friend class ProxyPtrImp<CLIENT, SPONSOR, LETTER>;

  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootObject(ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }
  friend const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootConstObject(const ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((const ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }  

  protected:
    virtual const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch() const;
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch();
    
    virtual LETTER* GetLetterPointer()
        { return Dispatch()->GetLetterPointer(); }
    virtual LETTER& GetLetterObject()
        { return Dispatch()->GetLetterObject(); }
    virtual const LETTER* GetLetterPointer() const
        { return ConstProxyPtr<CLIENT, SPONSOR, LETTER>::GetLetterPointer(); }
    virtual const LETTER& GetLetterObject() const
        { return ConstProxyPtr<CLIENT, SPONSOR, LETTER>::GetLetterObject(); }

  public:
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>& operator = (ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
        { return Dispatch()->operator = (Obj_); }
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>& operator = (CountedObjPtr<CLIENT>& Obj_)
        { return Dispatch()->operator = (Obj_); }
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>& operator = (CLIENT* rhs)
        { return Dispatch()->operator = (rhs); }

    virtual void SetBinding(TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Method_)
        { Dispatch()->SetBinding(Method_); }
    virtual void SetBinding(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Method_, long IdCode_=0)
        { Dispatch()->SetBinding(Method_, IdCode_); }
    virtual void SetBinding(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0)
        { Dispatch()->SetBinding(Method_, Sponsor_, IdCode_); }

    virtual void SetBinding(TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Method_)
        { Dispatch()->SetBinding(Method_); }
    virtual void SetBinding(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Method_, long IdCode_=0)
        { Dispatch()->SetBinding(Method_, IdCode_); }
    virtual void SetBinding(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0)
        { Dispatch()->SetBinding(Method_, Sponsor_, IdCode_); }

    virtual CLIENT* ReleaseControl()
	{ return Dispatch()->ReleaseControl(); }
    virtual const CLIENT* Pointee() const
        { return Dispatch()->Pointee(); }
    virtual CLIENT* Pointee()
        { return Dispatch()->Pointee(); }
    virtual const CLIENT* operator-> () const
        { return Dispatch()->operator -> (); }
    virtual CLIENT* operator-> ()
        { return Dispatch()->operator -> (); }

    virtual const ConstCountedObjPtr<CLIENT>& ClientPtr() const
        { return Dispatch()->ClientPtr(); }
    virtual CountedObjPtr<CLIENT>& ClientPtr()
        { return Dispatch()->ClientPtr(); }        
};

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
class ProxyPtrImp : public ProxyPtr<CLIENT, SPONSOR, LETTER>
{
  protected:
    CountedObjPtr<CLIENT> _BossPtr;
    BindingsUnion<CLIENT, SPONSOR, LETTER> _Bindings;

    int _Type;
    long _NotificationCode;
    const SPONSOR* _Sponsor;

    virtual const BindingsUnion<CLIENT, SPONSOR, LETTER>& Bindings() const
        { return _Bindings; }
        
    virtual int BindingType() const
        { return _Type; }
    virtual Boolean IsPtrLetter() const;
    virtual long NotificationCode() const
       { return _NotificationCode; }
    virtual const SPONSOR* Sponsor() const
       { return _Sponsor; }

    // Enclosed pointer retrieval (casting to) method,
    // calls CLIENT::*Bindings();
    virtual LETTER* GetLetterPointer();
    virtual LETTER& GetLetterObject();
    virtual const LETTER* GetLetterPointer() const;
    virtual const LETTER& GetLetterObject() const;

  public:
    ProxyPtrImp();
    ProxyPtrImp(const ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    ProxyPtrImp(const ProxyPtrImp<CLIENT, SPONSOR, LETTER>& Obj_);
  
    ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Method_);
    ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Method_, long IdCode_=0);
    ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0);
    
    ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Method_);
    ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Method_, long IdCode_=0);
    ProxyPtrImp(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>& operator = (ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>& operator = (CountedObjPtr<CLIENT>& Obj_);
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>& operator = (CLIENT* rhs);
    
    virtual void SetBinding(TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Method_);
    virtual void SetBinding(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Method_, long IdCode_=0);
    virtual void SetBinding(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    virtual void SetBinding(TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Method_);
    virtual void SetBinding(TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Method_, long IdCode_=0);
    virtual void SetBinding(TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    static int StaticID()
	{ return TypeInfo::PROXYPTRIMP_TYPE; }
    virtual int ClassID() const;
    virtual ProxyPtrImp<CLIENT, SPONSOR, LETTER>* Clone() const;

    virtual Boolean Unique() const
	{ return _BossPtr.Unique(); }

    virtual Boolean operator ! () const
        { return !_BossPtr; }
    virtual Boolean IsNull() const
	{ return _BossPtr.IsNull(); }
    virtual Boolean IsEqual(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_) const
        { return _BossPtr.IsEqual(Obj_.ClientPtr()); }
    virtual Boolean IsLesser(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_) const
        { return _BossPtr.IsLesser(Obj_.ClientPtr()); }
    virtual Boolean IsGreater(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_) const
        { return _BossPtr.IsGreater(Obj_.ClientPtr()); }        

    virtual CLIENT* ReleaseControl()
	{ return _BossPtr.ReleaseControl(); }
    virtual const CLIENT* ConstPointee() const
        { return _BossPtr.ConstPointee(); }
    virtual const CLIENT* Pointee() const
        { return _BossPtr.Pointee(); }
    virtual CLIENT* Pointee()
        { return _BossPtr.Pointee(); }
    virtual const CLIENT* operator-> () const
        { return _BossPtr.operator -> (); }
    virtual CLIENT* operator-> ()
        { return _BossPtr.operator -> (); }

    virtual const ConstCountedObjPtr<CLIENT>& ClientPtr() const
        { return _BossPtr; }
    virtual CountedObjPtr<CLIENT>& ClientPtr()
        { return _BossPtr; }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
class ProxyPointerPtr : public ProxyPtr<CLIENT, SPONSOR, LETTER>
{
  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootObject(ProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }
  friend const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootConstObject(const ProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((const ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }

  protected:
    ProxyPtrImp<CLIENT, SPONSOR, LETTER> _Letter;
    
    virtual const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch() const
        { return &_Letter; }
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch()
        { return &_Letter; }        

  public:
    ProxyPointerPtr();
    ProxyPointerPtr(const ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    ProxyPointerPtr(const ProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    
    ProxyPointerPtr(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_Type Method_);
    ProxyPointerPtr(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_Type Method_, long IdCode_=0);
    ProxyPointerPtr(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    static int StaticID()
	{ return TypeInfo::PROXYPOINTERPTR_TYPE; }
    virtual int ClassID() const;                    
    virtual ProxyPointerPtr<CLIENT, SPONSOR, LETTER>* Clone() const;

    // Cast-away-const method
    static ProxyPointerPtr<CLIENT, SPONSOR, LETTER>
    CastAwayConst(const ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_);

    // Enclosed pointer retrieval (casting to) method,
    // calls CLIENT::*Bindings();
    virtual operator const LETTER* () const;
    virtual operator LETTER* ();

    // Enclosed pointer dereference method,
    // calls *(CLIENT::*Bindings());
    virtual const LETTER& operator * () const;
    virtual LETTER& operator * ();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif    
};

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
class ProxyObjectPtr : public ProxyPtr<CLIENT, SPONSOR, LETTER>
{
  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootObject(ProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }
  friend const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootConstObject(const ProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((const ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }

  protected:
    ProxyPtrImp<CLIENT, SPONSOR, LETTER> _Letter;
    
    virtual const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch() const
        { return &_Letter; }
    virtual ProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch()
        { return &_Letter; }                
    
  public:
    ProxyObjectPtr();
    ProxyObjectPtr(const ProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    ProxyObjectPtr(const ProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    
    ProxyObjectPtr(CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_Type Method_);
    ProxyObjectPtr(CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_Type Method_, long IdCode_=0);
    ProxyObjectPtr(CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_Type Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    static int StaticID()
	{ return TypeInfo::PROXYOBJECTPTR_TYPE; }
    virtual int ClassID() const;
    virtual ProxyObjectPtr<CLIENT, SPONSOR, LETTER>* Clone() const;

    // Cast-away-const method
    static ProxyObjectPtr<CLIENT, SPONSOR, LETTER>
    CastAwayConst(const ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_);

    // Enclosed object retrieval (casting to) method,
    // calls T::RetrieveObjectAtIndex(size_t);
    virtual operator const LETTER& () const;
    virtual operator LETTER& ();

    // Enclosed object "address of" method
    // calls ProxyPointerPtr(T*, size_t) with internal data members
    ProxyPointerPtr<CLIENT, SPONSOR, LETTER> operator & () const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
template <class CLIENT>
class TempObjectClass : public ConstTempObjectClass<CLIENT>
{
  public:
    virtual CLIENT* Pointee() = 0;
    virtual const CLIENT* Pointee() const = 0;

    virtual operator CLIENT& () = 0;
    virtual operator const CLIENT& () const = 0;

    virtual CountedBuiltInPtr<CLIENT>& ClientPtr() = 0;
    virtual const ConstCountedBuiltInPtr<CLIENT>& ClientPtr() const = 0;
};

template <class CLIENT>
class TempObject : public TempObjectClass<CLIENT>
{
  protected:
    CountedBuiltInPtr<CLIENT> _BossPtr;

  public:
    TempObject();
    TempObject(CLIENT* Base_);
    TempObject(const CountedBuiltInPtr<CLIENT>& Obj_);
    TempObject(const TempObjectClass<CLIENT>& Obj_);

    static int StaticID()
	{ return TypeInfo::TEMPOBJECT_TYPE; }
    virtual int ClassID() const;
    virtual TempObject<CLIENT>* Clone() const;
  
    virtual Boolean Unique() const
	{ return _BossPtr.Unique(); }

    virtual Boolean operator ! () const
        { return !_BossPtr; }
    virtual Boolean IsNull() const
	{ return _BossPtr.IsNull(); }
    virtual Boolean IsEqual(const ConstTempObjectClass<CLIENT>& Obj_) const
        { return _BossPtr.IsEqual(Obj_.ClientPtr()); }
    virtual Boolean IsLesser(const ConstTempObjectClass<CLIENT>& Obj_) const
        { return _BossPtr.IsLesser(Obj_.ClientPtr()); }
    virtual Boolean IsGreater(const ConstTempObjectClass<CLIENT>& Obj_) const
        { return _BossPtr.IsGreater(Obj_.ClientPtr()); }

    virtual CLIENT* Pointee()
        { return _BossPtr.Pointee(); }
    virtual const CLIENT* Pointee() const
	{ return _BossPtr.Pointee(); }
    virtual const CLIENT* ConstPointee() const
        { return _BossPtr.ConstPointee(); }

    virtual operator CLIENT& ();
    virtual operator const CLIENT& () const;

    virtual CountedBuiltInPtr<CLIENT>& ClientPtr()
        { return _BossPtr; }
    virtual const ConstCountedBuiltInPtr<CLIENT>& ClientPtr() const
        { return _BossPtr; }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif            
};

/****************************************************************************/
/****************************************************************************/
#if SMARTPTR_DEBUG

class SimpleStr
{
  DEFAULT_EXTERNALCLASS_DEFN(SimpleStr)

  private:
    char* _str;

  public:
    SimpleStr():
      _str(NULL) {}
    SimpleStr(const char* str_):
      _str(new_char_string(str_)) {}
    ~SimpleStr()
      { DeleteArray(_str); }

    SimpleStr& operator = (const char* str_)
      { DeleteArray(_str);
        _str = new_char_string(str_);
        return *this; }

    char* Str()
      { return _str; }
      
    MEMORYOPS_DEFN(SimpleStr)
};

class EnclosedClass
{
  DEFAULT_EXTERNALCLASS_DEFN(EnclosedClass)

  private:
    long x;
    char* _own;
    static StaticObjPtr<EnclosedClass> _StaObj;

    EnclosedClass* Make()
	{ return (new EnclosedClass(999)); }

  public:
    EnclosedClass():
    x(0), _own(new_char_string("NONE"))
	{ if (_StaObj.IsNull())
	    _StaObj = EnclosedClass::Make();
	  else
	    _StaObj.Increment(); }
    EnclosedClass(long n):
    x(n), _own(new_char_string("NONE")) {}
    EnclosedClass(const EnclosedClass& Obj_):
    x(Obj_.x), _own(new_char_string(Obj_._own)) {}
    ~EnclosedClass()
    { cout <<"EnclosedClass class destructor called, owned by: " <<_own <<endl;
      ::Delete(_own); }
            
    void SetOwner(const char* Owner_)
        { ::Delete(_own);
          _own = new_char_string(Owner_); }
    void SetX(long newx)
	{ x = newx; }
    long showX()
	{ return x; }
    EnclosedClass* Clone() const
	{ return new EnclosedClass(*this); }

    MEMORYOPS_DEFN(EnclosedClass)
};

class MasterClass
{
  DEFAULT_EXTERNALCLASS_DEFN(MasterClass)

  public:
    long dupv;
    EnclosedClass what;
    MasterClass* _Next;
    UpdatePtr<EnclosedClass> _Uptr;

    MasterClass() : _Next(NULL)
	{ what.SetX(10); dupv = 10; }
    ~MasterClass()
	{ delete _Next; _Next = NULL; }

    void MakeAnother()
	{ _Next = new MasterClass; }

    Boolean Update(const EnclosedClass& w, long IdCode_);
    const Boolean& ConstUpdateReturnRef(const EnclosedClass& w, long IdCode_) const;
    const Boolean* ConstUpdateReturnPtr(const EnclosedClass& w, long IdCode_) const;
    Boolean& UpdateReturnRef(const EnclosedClass& w, long IdCode_);
    Boolean* UpdateReturnPtr(const EnclosedClass& w, long IdCode_);    
    
    TypedObjectUpdater<MasterClass, EnclosedClass>* Updater()
	{ SPMARKER("start/end: MasterClass::Updater()")
          return TypedObjectUpdater<MasterClass, EnclosedClass>::Make(this, &MasterClass::Update, 0); }

    UpdatePtr<EnclosedClass> ReturnAndUpdate()
	{ SPMARKER("start: MasterClass::ReturnAndUpdate()")
        
          CountedObjPtr<EnclosedClass> ObjPtr_(&what);
	  UpdatePtr<EnclosedClass> Ptr_;
	  if (_Next)
            Ptr_.SetUpdatablePtr(ObjPtr_, Updater()->AddNext(_Next->Updater()), NULL);
	  else
	    Ptr_.SetUpdatablePtr(ObjPtr_, Updater(), NULL);
            
          SPMARKER("end: MasterClass::ReturnAndUpdate()")
	  return Ptr_; }

    long ShowX()
	{ return what.showX(); }
    long ShowDupv()
	{ return dupv; }

    MEMORYOPS_DEFN(MasterClass)
};

#endif
/****************************************************************************/
#endif





