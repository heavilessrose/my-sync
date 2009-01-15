#ifndef CONSTPTR_H
#define CONSTPTR_H
#ifndef BASEPTR_CPP
  #include "baseptr.cpp"
#endif
#ifndef TYPEDSMARTHND_CPP
  #include "typedhnd.cpp"
#endif

#define CONSTPTR_DEBUG			0
#define CONSTPTR_DEBUG_COPIEDPTR	0
#define CONSTPTR_DEBUG_COUNTEDPTR	0

#define SMARTPTR_TRACE        0
#if SMARTPTR_TRACE
#  define SPMARKER(x)   MARKER(x)
#else
#  define SPMARKER(x)
#endif

/****************************************************************************/
template <class T>
class ConstCopiedVoidPtr;

template <class T>
class CopiedPtrData
{
  friend class ConstCopiedVoidPtr<T>;

  private:
    CopiedPtrData(const T* just_newed, ObjectUpdater* Deleter_);
    CopiedPtrData(const CopiedPtrData<T>& Obj_);

  public:
    union
    {
      T* the_p;
      const T* const_p;
    };

    BaseCopiedPtr<ObjectUpdater> _Deleter;
    Boolean _Modified;

    void ErasePointee();
};

template <class T>
class ConstCopiedVoidPtr
{
  friend ConstCopiedVoidPtr<T>& RootObject(ConstCopiedVoidPtr<T>& Obj_)
	{ return &Obj_; }
  friend const ConstCopiedVoidPtr<T>& RootConstObject(const ConstCopiedVoidPtr<T>& Obj_)
	{ return &Obj_; }

  friend Boolean operator == (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs);
  friend Boolean operator != (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs);
  friend Boolean operator < (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs);
  friend Boolean operator <= (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs);
  friend Boolean operator > (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs);
  friend Boolean operator >= (const ConstCopiedVoidPtr<T>& lhs, const ConstCopiedVoidPtr<T>& rhs);        

  friend Boolean _IsEqualTo(const ConstCopiedVoidPtr<T>& Trg_, const ConstCopiedVoidPtr<T>& Src_)
	{ return Trg_.IsEqual(Src_); }
  friend Boolean _IsLesserTo(const ConstCopiedVoidPtr<T>& Trg_, const ConstCopiedVoidPtr<T>& Src_)
	{ return Trg_.IsLesser(Src_); }
  friend Boolean _IsGreaterTo(const ConstCopiedVoidPtr<T>& Trg_, const ConstCopiedVoidPtr<T>& Src_)
	{ return Trg_.IsGreater(Src_); }          

  friend ConstCopiedVoidPtr<T>* _Clone(const ConstCopiedVoidPtr<T>& Obj_)
	{ return ((ConstCopiedVoidPtr<T>*)Obj_.Clone()); }
  friend const ConstCopiedVoidPtr<T>* _ConstClone(const ConstCopiedVoidPtr<T>& Obj_)
	{ return &Obj_; }
  friend ConstCopiedVoidPtr<T>* _NonConstClone(ConstCopiedVoidPtr<T>& Obj_)
	{ return &Obj_; }  

  friend int _WhatIs(const ConstCopiedVoidPtr<T>& Obj_)
	{ return Obj_.ClassID(); }

  friend Boolean _IsMatchable(const ConstCopiedVoidPtr<T>& Obj_)
	{ return TRUE; }
  friend Boolean _IsSortable(const ConstCopiedVoidPtr<T>& Obj_)
	{ return TRUE; }

  protected:
    CopiedPtrData<T> _Data;

    ConstCopiedVoidPtr<T>& operator = (T* just_newed);
    ConstCopiedVoidPtr<T>& operator = (const ConstCopiedVoidPtr<T>& Obj_);

    virtual const BaseCopiedPtr<ObjectUpdater>& GiveDeleter() const;
    virtual T* VarPointee();

  public:
    ConstCopiedVoidPtr(ObjectUpdater* Deleter_=NULL);
    ConstCopiedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_);
    ConstCopiedVoidPtr(const T& obj, ObjectUpdater* Deleter_);
    ConstCopiedVoidPtr(const ConstCopiedVoidPtr<T>& Obj_);  // Copy heap object
    virtual ~ConstCopiedVoidPtr();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::CONSTCOPIEDVOIDPTR_TYPE; }
    virtual int ClassID() const;

    virtual ConstCopiedVoidPtr<T>* Clone() const;

    virtual Boolean operator ! () const;
    virtual Boolean IsNull() const;
    virtual const T* Pointee() const;
    virtual const T* ConstPointee() const;
    virtual Boolean IsEqual(const ConstCopiedVoidPtr& Obj_) const;
    virtual Boolean IsGreater(const ConstCopiedVoidPtr<T>& Obj_) const;
    virtual Boolean IsLesser(const ConstCopiedVoidPtr<T>& Obj_) const;

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
class ConstCopiedBuiltInPtr : public ConstCopiedVoidPtr<T>
{
  friend ConstCopiedVoidPtr<T>& RootObject(ConstCopiedBuiltInPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCopiedVoidPtr<T>& RootConstObject(const ConstCopiedBuiltInPtr<T>& Obj_)
	{ return *((const ConstCopiedVoidPtr<T>*)(&Obj_)); }

  private:
    ConstCopiedBuiltInPtr<T>& operator = (T*)
        { return *this; }
    ConstCopiedBuiltInPtr<T>& operator = (const ConstCopiedVoidPtr<T>&)
        { return *this; }
  
  protected:
    ConstCopiedBuiltInPtr(const ConstCopiedVoidPtr<T>& Obj_);    

  public:
    ConstCopiedBuiltInPtr();
    ConstCopiedBuiltInPtr(const T* just_newed);
    ConstCopiedBuiltInPtr(const T& obj);                         // Copy object
    ConstCopiedBuiltInPtr(const ConstCopiedBuiltInPtr<T>& aCP);  // Copy heap object

    virtual ConstCopiedBuiltInPtr<T>* Clone() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::CONSTCOPIEDBUILTINPTR_TYPE; }
    virtual int ClassID() const;        

    virtual const T& operator * () const;

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
class ConstCopiedObjPtr : public ConstCopiedVoidPtr<T>
{
  friend ConstCopiedVoidPtr<T>& RootObject(ConstCopiedObjPtr<T>& Obj_)
	{ return *((ConstCopiedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCopiedVoidPtr<T>& RootConstObject(const ConstCopiedObjPtr<T>& Obj_)
	{ return *((const ConstCopiedVoidPtr<T>*)(&Obj_)); }

  private:
    ConstCopiedObjPtr<T>& operator = (T*)
        { return *this; }
    ConstCopiedObjPtr<T>& operator = (const ConstCopiedVoidPtr<T>&)
        { return *this; }

  protected:
    ConstCopiedObjPtr(const ConstCopiedVoidPtr<T>& Obj_);

  public:
    ConstCopiedObjPtr();
    ConstCopiedObjPtr(const T* just_newed);
    ConstCopiedObjPtr(const T& Obj_);
    ConstCopiedObjPtr(const ConstCopiedObjPtr<T>& Obj_);

    virtual ConstCopiedObjPtr<T>* Clone() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::CONSTCOPIEDOBJPTR_TYPE; }
    virtual int ClassID() const;    

    virtual const T& operator * () const;
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
template <class T>
class ConstCountedVoidPtr;

template <class T>
class CountedPtrData
{
  friend class ConstCountedVoidPtr<T>;

  public:
    union
    {
      T* the_p;
      const T* const_p;
    };

    BaseCountedPtr<ObjectUpdater> _Deleter;

    ReferenceCount refCount; // Number of pointers to heap object
    Boolean _Ownership;	     // Has ownership of pointer (permission to delete)
    Boolean _Modified;	     // Data ptr is modified

    CountedPtrData(const T* just_newed, ObjectUpdater* Deleter_, Boolean OwnsPtr_);
    void ErasePointee();
};

template <class T>
class ConstCountedVoidPtr
{
  friend ConstCountedVoidPtr<T>& RootObject(ConstCountedVoidPtr<T>& Obj_)
	{ return &Obj_; }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const ConstCountedVoidPtr<T>& Obj_)
	{ return &Obj_; }

  friend Boolean operator == (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs);
  friend Boolean operator != (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs);
  friend Boolean operator < (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs);
  friend Boolean operator <= (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs);
  friend Boolean operator > (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs);
  friend Boolean operator >= (const ConstCountedVoidPtr<T>& lhs, const ConstCountedVoidPtr<T>& rhs);

  friend Boolean _IsEqualTo(const ConstCountedVoidPtr<T>& Trg_, const ConstCountedVoidPtr<T>& Src_)
	{ return Trg_.IsEqual(Src_); }
  friend Boolean _IsLesserTo(const ConstCountedVoidPtr<T>& Trg_, const ConstCountedVoidPtr<T>& Src_)
	{ return Trg_.IsLesser(Src_); }
  friend Boolean _IsGreaterTo(const ConstCountedVoidPtr<T>& Trg_, const ConstCountedVoidPtr<T>& Src_)
	{ return Trg_.IsGreater(Src_); }

  friend ConstCountedVoidPtr<T>* _Clone(const ConstCountedVoidPtr<T>& Obj_)
	{ return ((ConstCountedVoidPtr<T>*)Obj_.Clone()); }
  friend const ConstCountedVoidPtr<T>* _ConstClone(const ConstCountedVoidPtr<T>& Obj_)
	{ return ((const ConstCountedVoidPtr<T>*)Obj_.Clone()); }
  friend ConstCountedVoidPtr<T>* _NonConstClone(ConstCountedVoidPtr<T>& Obj_)
	{ return ((ConstCountedVoidPtr<T>*)Obj_.Clone()); }
        
  friend int _WhatIs(const ConstCountedVoidPtr<T>& Obj_)
	{ return Obj_.ClassID(); }

  friend Boolean _IsMatchable(const ConstCountedVoidPtr<T>& Obj_)
	{ return TRUE; }
  friend Boolean _IsSortable(const ConstCountedVoidPtr<T>& Obj_)
	{ return TRUE; }        

  protected:
    CountedPtrData<T> _Data;

    void SetToNull();

    ConstCountedVoidPtr<T>& operator = (T* just_newed);
    ConstCountedVoidPtr<T>& operator = (ConstCountedVoidPtr<T>& Obj_);

    virtual const BaseCountedPtr<ObjectUpdater>& GiveDeleter() const;
    virtual BaseCountedPtr<ObjectUpdater>& GiveDeleter();

    virtual const ReferenceCount& GiveRefCount() const;
    virtual T* VarPointee();

  public:
    ConstCountedVoidPtr(ObjectUpdater* Deleter_=NULL);
    ConstCountedVoidPtr(const T* just_newed, ObjectUpdater* Deleter_, Boolean OwnsPtr_=TRUE);
    ConstCountedVoidPtr(const ConstCountedVoidPtr<T>& Obj_);
    virtual ~ConstCountedVoidPtr();

    virtual ConstCountedVoidPtr<T>* Clone() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::CONSTCOUNTEDVOIDPTR_TYPE; }
    virtual int ClassID() const;

    virtual Boolean HasOwnership() const;
    virtual Boolean Unique() const;

    virtual Boolean operator ! () const;
    virtual Boolean IsNull() const;
    virtual const T* Pointee() const;
    virtual const T* ConstPointee() const;
    virtual Boolean IsEqual(const ConstCountedVoidPtr<T>& Obj_) const;
    virtual Boolean IsGreater(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_=NULL) const;
    virtual Boolean IsLesser(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_=NULL) const;

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
class ConstCountedObjPtr : public ConstCountedVoidPtr<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(ConstCountedObjPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const ConstCountedObjPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  private:
    ConstCountedObjPtr<T>& operator = (T*)
        { return *this; }
    ConstCountedObjPtr<T>& operator = (ConstCountedVoidPtr<T>&)
        { return *this; }

  protected:
    ConstCountedObjPtr(const ConstCountedVoidPtr<T>& Obj_);

  public:
    ConstCountedObjPtr();
    ConstCountedObjPtr(const ConstCountedObjPtr<T>& Obj_);
    ConstCountedObjPtr(const T* just_newed, Boolean OwnsPtr_=TRUE);

    virtual ConstCountedObjPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::CONSTCOUNTEDOBJPTR_TYPE; }
    virtual int ClassID() const;        

    virtual const T& operator * () const;
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
template <class T>
class ConstCountedBuiltInPtr : public ConstCountedVoidPtr<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(ConstCountedBuiltInPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const ConstCountedBuiltInPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  private:
    ConstCountedBuiltInPtr<T>& operator = (T*)
	{ return *this; }
    ConstCountedBuiltInPtr<T>& operator = (ConstCountedVoidPtr<T>&)
	{ return *this; }

  protected:
    ConstCountedBuiltInPtr(const ConstCountedVoidPtr<T>& Obj_);

  public:
    ConstCountedBuiltInPtr();
    ConstCountedBuiltInPtr(const ConstCountedBuiltInPtr<T>& Obj_);
    ConstCountedBuiltInPtr(const T* just_newed, Boolean OwnsPtr_=TRUE);

    virtual ConstCountedBuiltInPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::CONSTCOUNTEDBUILTINPTR_TYPE; }
    virtual int ClassID() const;        

    virtual const T& operator * () const;

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
class ConstUpdatePtr : public ConstCountedVoidPtr<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(ConstUpdatePtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const ConstUpdatePtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  private:
    ConstUpdatePtr<T>& operator = (T*)
	{ return *this; }
    ConstUpdatePtr<T>& operator = (ConstCountedVoidPtr<T>&)
	{ return *this; }

  protected:
    BaseCountedPtr<UpdaterBindings> _Updater;
    BaseCountedPtr<UpdaterBindings> _Validator;

    static Boolean IsNull(const BaseCountedPtr<UpdaterBindings>& Bindings_)
        { return (Bindings_.IsNull() || Bindings_->IsNull()); }

  public:
    ConstUpdatePtr(ObjectUpdater* Deleter_=NULL,
                   ObjectUpdater* Updater_=NULL,
                   ObjectUpdater* Validator_=NULL);
    ConstUpdatePtr(const ConstCountedVoidPtr<T>& Obj_,
                   ObjectUpdater* Updater_,
                   ObjectUpdater* Validator_=NULL);
    ConstUpdatePtr(const ConstUpdatePtr<T>& Obj_);                          
    virtual ~ConstUpdatePtr();

    virtual ConstUpdatePtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::CONSTUPDATEPTR_TYPE; }
    virtual int ClassID() const;            

    ConstUpdatePtr<T>& SetUpdater(ObjectUpdater* Updater_);
    ConstUpdatePtr<T>& SetValidator(ObjectUpdater* Validator_);
    UpdaterBindings* GetUpdater() const;
    UpdaterBindings* GetValidator() const;

    virtual Boolean Update() const;
    virtual Boolean Validate() const;

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
class ConstArrayPtr : public ConstCountedVoidPtr<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(ConstArrayPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const ConstArrayPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  friend Boolean operator <= (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs);
  friend Boolean operator >= (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs);
  friend Boolean operator < (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs);
  friend Boolean operator > (const ConstArrayPtr<T>& lhs, const ConstArrayPtr<T>& rhs);

  friend const ConstArrayPtr<T> operator + (ptrdiff_t Index_, const ConstArrayPtr<T>& Rhs_);
  friend const ConstArrayPtr<T> operator - (ptrdiff_t Index_, const ConstArrayPtr<T>& Rhs_);
  friend ptrdiff_t operator - (const T* Ptr_, const ConstArrayPtr<T>& Rhs_);

  private:
    ConstArrayPtr<T>& operator = (T*)
	{ return *this; }
    ConstArrayPtr<T>& operator = (ConstCountedVoidPtr<T>&)
	{ return *this; }

  protected:
    size_t _Size;
    const T* base_p;

    ConstArrayPtr<T>& operator += (ptrdiff_t Index_);
    ConstArrayPtr<T>& operator -= (ptrdiff_t Index_);

  public:
    ConstArrayPtr(ObjectUpdater* Deleter_=NULL);
    ConstArrayPtr(const ConstArrayPtr<T>& Obj_);
    ConstArrayPtr(const ConstCountedVoidPtr<T>& Obj_, size_t Size_=1, const T* Base_=NULL);
    virtual ~ConstArrayPtr() {}

    virtual ConstArrayPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::CONSTARRAYPTR_TYPE; }
    virtual int ClassID() const;

    size_t GetSize() const
	{ return _Size; }
    const T* GetBase() const
	{ return base_p; }

    virtual const T& operator * () const;
    virtual const T& operator [] (ptrdiff_t Index_) const;

    const ConstArrayPtr<T> operator + (ptrdiff_t Index_) const;
    const ConstArrayPtr<T> operator - (ptrdiff_t Index_) const;

    ptrdiff_t operator - (const ConstArrayPtr<T>& Ptr_) const;
    ptrdiff_t operator - (const T* Ptr_) const;

    virtual Boolean IsGreater(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_=NULL) const;
    virtual Boolean IsLesser(const ConstCountedVoidPtr<T>& Obj_, const ConstCountedVoidPtr<T>* Ptr_=NULL) const;

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
class ConstStaticObjPtr : public ConstCountedVoidPtr<T>
{
  friend ConstCountedVoidPtr<T>& RootObject(ConstStaticObjPtr<T>& Obj_)
	{ return *((ConstCountedVoidPtr<T>*)(&Obj_)); }
  friend const ConstCountedVoidPtr<T>& RootConstObject(const ConstStaticObjPtr<T>& Obj_)
	{ return *((const ConstCountedVoidPtr<T>*)(&Obj_)); }

  private:
    ConstStaticObjPtr<T>& operator = (T*)
        { return *this; }
    ConstStaticObjPtr<T>& operator = (ConstCountedVoidPtr<T>&)
        { return *this; }  

  protected:
    ConstStaticObjPtr(const ConstCountedVoidPtr<T>& Obj_);

  public:
    ConstStaticObjPtr();			// Construct as null pointer
    ConstStaticObjPtr(const T* just_newed);     // Construct pointing at heap object

    virtual ConstStaticObjPtr<T>* Clone() const;

    static int StaticID()
	{ return TypeInfo::CONSTSTATICOBJPTR_TYPE; }
    virtual int ClassID() const;                

    ConstStaticObjPtr<T>& Increment();
    ConstStaticObjPtr<T>& Decrement();

    virtual const T& operator * () const;
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
template <class CLIENT, class SPONSOR, class LETTER>
class ConstProxyPtrImp;

template <class CLIENT, class SPONSOR, class LETTER>
class ProxyPtrImp;

template <class CLIENT, class SPONSOR, class LETTER>
class ConstProxyPtr
{
  friend class ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>;
  friend class ProxyPtrImp<CLIENT, SPONSOR, LETTER>;

  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootObject(ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return &Obj_; }
  friend const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootConstObject(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return &Obj_; }

  friend Boolean operator == (const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& lhs, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& rhs)
        { return lhs.IsEqual(rhs); }
  friend Boolean operator != (const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& lhs, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& rhs)
        { return !lhs.IsEqual(rhs); }
  friend Boolean operator < (const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& lhs, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& rhs)
        { return lhs.IsLesser(rhs); }
  friend Boolean operator <= (const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& lhs, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& rhs)
        { return (lhs.IsLesser(rhs) || lhs.IsEqual(rhs)); }
  friend Boolean operator > (const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& lhs, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& rhs)
        { return lhs.IsGreater(rhs); }
  friend Boolean operator >= (const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& lhs, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& rhs)
        { return (lhs.IsGreater(rhs) || lhs.IsEqual(rhs)); }        
        
  friend Boolean _IsEqualTo(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Trg_, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Src_)
	{ return Trg_.IsEqual(Src_); }
  friend Boolean _IsLesserTo(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Trg_, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Src_)
	{ return Trg_.IsLesser(Src_); }
  friend Boolean _IsGreaterTo(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Trg_, const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Src_)
	{ return Trg_.IsGreater(Src_); }
        
  friend int _WhatIs(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return Obj_.ClassID(); }

  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>* _Clone(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return ((ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)Obj_.Clone()); }
  friend const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* _ConstClone(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return ((const ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)Obj_.Clone()); }
  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>* _NonConstClone(ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return ((ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)Obj_.Clone()); }

  friend Boolean _IsMatchable(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return TRUE; }
  friend Boolean _IsSortable(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return TRUE; }

  protected:
    virtual const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch() const;
  
    virtual const BindingsUnion<CLIENT, SPONSOR, LETTER>& Bindings() const
        { return Dispatch()->Bindings(); }

    virtual int BindingType() const
        { return Dispatch()->BindingType(); }
    virtual Boolean IsPtrLetter() const
        { return Dispatch()->IsPtrLetter(); }
    virtual long NotificationCode() const
        { return Dispatch()->NotificationCode(); }
    virtual const SPONSOR* Sponsor() const
        { return Dispatch()->Sponsor(); }

    virtual const LETTER* GetLetterPointer() const
        { return Dispatch()->GetLetterPointer(); }
    virtual const LETTER& GetLetterObject() const
        { return Dispatch()->GetLetterObject(); }

  public:  
    virtual Boolean Unique() const
        { return Dispatch()->Unique(); }

    virtual int ClassID() const = 0;
    virtual ConstProxyPtr<CLIENT, SPONSOR, LETTER>* Clone() const = 0;

    virtual Boolean operator ! () const
        { return Dispatch()->operator ! (); }
    virtual Boolean IsNull() const
        { return Dispatch()->IsNull(); }

    virtual const CLIENT* Pointee() const
        { return Dispatch()->Pointee(); }
    virtual const CLIENT* ConstPointee() const
        { return Dispatch()->ConstPointee(); }
    virtual const CLIENT* operator-> () const
        { return Dispatch()->operator -> (); }
    virtual Boolean IsEqual(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_) const
        { return Dispatch()->IsEqual(Obj_); }
    virtual Boolean IsLesser(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_) const
        { return Dispatch()->IsLesser(Obj_); }
    virtual Boolean IsGreater(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_) const
        { return Dispatch()->IsGreater(Obj_); }

    virtual const ConstCountedObjPtr<CLIENT>& ClientPtr() const
        { return Dispatch()->ClientPtr(); }        
};

/****************************************************************************/
template <class CLIENT, class SPONSOR, class LETTER>
class ConstProxyPtrImp : public ConstProxyPtr<CLIENT, SPONSOR, LETTER>
{
  protected:
    ConstCountedObjPtr<CLIENT> _BossPtr;
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
    virtual const LETTER* GetLetterPointer() const;
    virtual const LETTER& GetLetterObject() const;

  public:
    ConstProxyPtrImp();
    ConstProxyPtrImp(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    ConstProxyPtrImp(const ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>& Obj_);
  
    ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_ConstType Method_);
    ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_ConstType Method_, long IdCode_=0);
    ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_=0);
    
    ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_ConstType Method_);
    ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_ConstType Method_, long IdCode_=0);
    ConstProxyPtrImp(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    static int StaticID()
	{ return TypeInfo::CONSTPROXYPTRIMP_TYPE; }
    virtual int ClassID() const;
    virtual ConstProxyPtrImp<CLIENT, SPONSOR, LETTER>* Clone() const;
  
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

    virtual const CLIENT* Pointee() const
	{ return _BossPtr.Pointee(); }
    virtual const CLIENT* ConstPointee() const
        { return _BossPtr.ConstPointee(); }
    virtual const CLIENT* operator-> () const
        { return _BossPtr.operator -> (); }

    virtual const ConstCountedObjPtr<CLIENT>& ClientPtr() const
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
class ConstProxyPointerPtr : public ConstProxyPtr<CLIENT, SPONSOR, LETTER>
{
  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootObject(ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }
  friend const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootConstObject(const ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((const ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }

  protected:
    ConstProxyPtrImp<CLIENT, SPONSOR, LETTER> _Letter;
    virtual const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch() const
        { return &_Letter; }

  public:
    ConstProxyPointerPtr();
    ConstProxyPointerPtr(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    ConstProxyPointerPtr(const ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    
    ConstProxyPointerPtr(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::PtrTo_ConstType Method_);
    ConstProxyPointerPtr(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::PtrTo_ConstType Method_, long IdCode_=0);
    ConstProxyPointerPtr(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::PtrTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    static int StaticID()
	{ return TypeInfo::CONSTPROXYPOINTERPTR_TYPE; }
    virtual int ClassID() const;
    virtual ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER>* Clone() const;

    // Enclosed pointer retrieval (casting to) method,
    // calls CLIENT::*Bindings();
    virtual operator const LETTER* () const;

    // Enclosed pointer dereference method,
    // calls *(CLIENT::*Bindings());
    virtual const LETTER& operator * () const;

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
class ConstProxyObjectPtr : public ConstProxyPtr<CLIENT, SPONSOR, LETTER>
{
  friend ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootObject(ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }
  friend const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& RootConstObject(const ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_)
	{ return *((const ConstProxyPtr<CLIENT, SPONSOR, LETTER>*)(&Obj_)); }

  protected:
    ConstProxyPtrImp<CLIENT, SPONSOR, LETTER> _Letter;
    virtual const ConstProxyPtr<CLIENT, SPONSOR, LETTER>* Dispatch() const
        { return &_Letter; }
    
  public:
    ConstProxyObjectPtr();
    ConstProxyObjectPtr(const ConstProxyPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    ConstProxyObjectPtr(const ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>& Obj_);
    
    ConstProxyObjectPtr(const CLIENT* Base_, TYPENAME ProxyBinding0<CLIENT, LETTER>::RefTo_ConstType Method_);
    ConstProxyObjectPtr(const CLIENT* Base_, TYPENAME ProxyBinding1<CLIENT, long, LETTER>::RefTo_ConstType Method_, long IdCode_=0);
    ConstProxyObjectPtr(const CLIENT* Base_, TYPENAME ProxyBinding2<CLIENT, SPONSOR, LETTER>::RefTo_ConstType Method_, const SPONSOR* Sponsor_, long IdCode_=0);

    static int StaticID()
	{ return TypeInfo::CONSTPROXYOBJECTPTR_TYPE; }
    virtual int ClassID() const;
    virtual ConstProxyObjectPtr<CLIENT, SPONSOR, LETTER>* Clone() const;

    // Enclosed object retrieval (casting to) method,
    // calls T::RetrieveObjectAtIndex(size_t);
    virtual operator const LETTER& () const;

    // Enclosed object "address of" method
    // calls ProxyPointerPtr(T*, size_t) with internal data members
    ConstProxyPointerPtr<CLIENT, SPONSOR, LETTER> operator & () const;

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
class ConstTempObjectClass
{
  public:
    virtual int ClassID() const = 0;  
    virtual Boolean Unique() const = 0;

    virtual Boolean operator ! () const = 0;
    virtual Boolean IsNull() const = 0;
    virtual Boolean IsEqual(const ConstTempObjectClass<CLIENT>& Obj_) const = 0;
    virtual Boolean IsLesser(const ConstTempObjectClass<CLIENT>& Obj_) const = 0;
    virtual Boolean IsGreater(const ConstTempObjectClass<CLIENT>& Obj_) const = 0;

    virtual const CLIENT* Pointee() const = 0;
    virtual const CLIENT* ConstPointee() const = 0;

    virtual operator const CLIENT& () const = 0;
    virtual const ConstCountedBuiltInPtr<CLIENT>& ClientPtr() const = 0;
};

template <class CLIENT>
class ConstTempObject : public ConstTempObjectClass<CLIENT>
{
  protected:
    ConstCountedBuiltInPtr<CLIENT> _BossPtr;

  public:
    ConstTempObject();
    ConstTempObject(const CLIENT* Base_);
    ConstTempObject(const ConstCountedBuiltInPtr<CLIENT>& Obj_);
    ConstTempObject(const ConstTempObjectClass<CLIENT>& Obj_);

    static int StaticID()
	{ return TypeInfo::CONSTTEMPOBJECT_TYPE; }
    virtual int ClassID() const;
    virtual ConstTempObject<CLIENT>* Clone() const;
  
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

    virtual const CLIENT* Pointee() const
	{ return _BossPtr.Pointee(); }
    virtual const CLIENT* ConstPointee() const
        { return _BossPtr.ConstPointee(); }

    virtual operator const CLIENT& () const;

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
#if CONSTPTR_DEBUG
class EnclosedClass
{
  DEFAULT_EXTERNALCLASS_DEFN(EnclosedClass)
  
  private:
    int x;
    static ConstStaticObjPtr<EnclosedClass> _StaObj;

    EnclosedClass(int n):
    x(n) {}

    static EnclosedClass* Make()
	{ return (new EnclosedClass(999)); }

  public:
    EnclosedClass()
	{ if (!_StaObj.IsNull())
	    _StaObj.Increment(); }
    ~EnclosedClass()
    { }

    int showX() const
	{ return x; }
    EnclosedClass* Clone() const
	{ return new EnclosedClass(*this); }

    MEMORYOPS_DEFN(EnclosedClass)
};
#endif
/****************************************************************************/
#endif





