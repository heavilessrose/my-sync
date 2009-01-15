// ptrdynarray standard header
#ifndef PTRDYNARRAY_H
#define PTRDYNARRAY_H
#ifndef DYNARRAYBASE_CPP
  #include "dynarraybase.cpp"
#endif
#ifndef SIMPLEARRAY_CPP
  #include "simplearray.cpp"
#endif

/****************************************************************************/
template <class T>
class PtrDynArrayIterator;

template <class T>
class PtrDynArray;

template <class T>
class PtrDynArrayAcceptor : public ObjectAcceptor
{
  friend class PtrDynArray<T>;

  protected:
    union
    {
      const PtrDynArray<T>* _ObjPtr;
      PtrDynArray<T>* _VarPtr;
    };

    PtrDynArrayAcceptor(const PtrDynArray<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // PtrDynArray class specific acceptor methods
    virtual Boolean IsDynArray() const;
    virtual Boolean IsPtrDynArray() const;
    virtual void* CreateFromPtrDynArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromPtrDynArray(const ObjectAcceptor& Obj_);

    // DynArray comparison methods
    virtual Boolean IsEqualToPtrDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToPtrDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToPtrDynArray(const ObjectAcceptor&) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class PtrDynArray : public ObjectImp, public DynArrayBase<void*>
{
  friend class PtrDynArrayAcceptor<T>;

  friend Object& RootObject(PtrDynArray<T>& Arr_)
	{ return *((Object*)(&Arr_)); }
  friend const Object& RootConstObject(const PtrDynArray<T>& Arr_)
	{ return *((const Object*)(&Arr_)); }

  friend SimpleArray<int> SeriesArrayCompare(const PtrDynArray<T> &ia1, const PtrDynArray<T> &ia2);
  friend PtrDynArray<T> Apply(const PtrDynArray<T>& ia, T*(*func)(T*))
        { PtrDynArray<T> Temp_(ia); Temp_.Apply(func); return Temp_; }
        
  public:
    typedef int(*CompFuncType)(const T*, const T*);

  protected:
    static CompFuncType _CompFunc;  // Comparison function for list items

    virtual Boolean IsEqualToPtrDynArray(const PtrDynArray<T>* Ptr_) const;
    virtual Boolean IsLesserToPtrDynArray(const PtrDynArray<T>* Ptr_) const;
    virtual Boolean IsGreaterToPtrDynArray(const PtrDynArray<T>* Ptr_) const;

    void _NVMGrow(size_t n, void* const* s=0, Boolean Trim_=0);
    virtual void _Grow(size_t n, void* const* s=0, Boolean Trim_=0);    

  public:
    PtrDynArray();
    PtrDynArray(size_t n, capacity c);
    PtrDynArray(const PtrDynArray<T>& x);
    PtrDynArray(T* x, size_t n=1);
    PtrDynArray(T** s, size_t n=1);

    static PtrDynArray<T>* Make()
	{ return (new PtrDynArray<T>); }
    static PtrDynArray<T>* Make(size_t n, capacity c)
	{ return (new PtrDynArray<T>(n, c)); }
    static PtrDynArray<T>* Make(const Object& Obj_)
	{ PtrDynArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.ShallowClone(&TrgComp_);
	  return TrgComp_._VarPtr;
	}
    static PtrDynArray<T>* Make(T* x, size_t n=1)
	{ return (new PtrDynArray<T>(x, n)); }
    static PtrDynArray<T>* Make(T** s, size_t n)
	{ return (new PtrDynArray<T>(s, n)); }

    static const PtrDynArray<T>* MakeConst(const Object& Obj_)
	{ PtrDynArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.ShallowClone(&TrgComp_);
	  return TrgComp_._ObjPtr;
	}
    static const PtrDynArray<T>* MakeConst(T* x, size_t n=1)
	{ return (new PtrDynArray<T>(x, n)); }
    static const PtrDynArray<T>* MakeConst(T** s, size_t n)
	{ return (new PtrDynArray<T>(s, n)); }

    // Set comparison function method
    static void SetComparisonFunction(int(*CmpFnc_)(const T*, const T*));

    // comparison methods
    SimpleArray<int> SeriesArrayCompare(const PtrDynArray<T> &ia) const;
    virtual int Compare(const PtrDynArray<T>& Arr_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static PtrDynArray<T>& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::PTRDYNARRAY_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // sorting method
    PtrDynArray<T>& Sort(SortingHelper<T*, T>* SortHelpPtr_);

    // function application method
    PtrDynArray<T>& ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const;
    PtrDynArray<T>& ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_);
    PtrDynArray<T>& ApplyUnary(int(*Fnc_)(T*));
    PtrDynArray<T>& ApplyUnary(T*(*func)(T*));

    PtrDynArray<T>& operator = (const PtrDynArray<T>& Rhs_);
    PtrDynArray<T>& operator += (const PtrDynArray<T>& Rhs_)
	{ (DynArrayBase<void*>::operator += ((const DynArrayBase<void*>&)Rhs_));
	  return *this; }
    PtrDynArray<T>& operator += (T* Rhs_)
	{ (DynArrayBase<void*>::operator += ((void*)Rhs_));
	  return *this;	}

    PtrDynArray<T>& Append(T* x, size_t n=1)
	{ (DynArrayBase<void*>::Append((void*)x, n));
	  return *this; }
    PtrDynArray<T>& Append(T** s, size_t n=1)
	{ (DynArrayBase<void*>::Append((void**)s, n));
	  return *this;	}

    PtrDynArray<T>& AssignArg(T* x, size_t n=1)
	{ (DynArrayBase<void*>::AssignArg((void*)x, n));
	  return *this; }
    PtrDynArray<T>& AssignArg(T** s, size_t n=1)
	{ (DynArrayBase<void*>::AssignArg((void**)s, n));
	  return *this;	}

    PtrDynArray<T>& Insert(size_t p, const PtrDynArray<T>& x)
	{ (DynArrayBase<void*>::Insert(p, (const DynArrayBase<void*>&)x));
	  return *this; }
    PtrDynArray<T>& Insert(size_t p, T* x, size_t n=1)
	{ (DynArrayBase<void*>::Insert(p, (void*)x, n));
	  return *this; }
    PtrDynArray<T>& Insert(size_t p, T** s, size_t n=1)
	{ (DynArrayBase<void*>::Insert(p, (void**)s, n));
	  return *this;	}

    PtrDynArray<T>& Remove(size_t p=0, size_t n=SIZET_MAX)
	{ (DynArrayBase<void*>::Remove(p, n));
	  return *this;	}

    PtrDynArray<T>& Sub_Array(PtrDynArray<T>& x, size_t p, size_t n=SIZET_MAX)
	{ (DynArrayBase<void*>::Sub_Array(x, p, n));
	  return *this;	}

    void Swap(PtrDynArray<T>& x)
	{ DynArrayBase<void*>::Swap(x); }

    T* Get_At(size_t p) const
	{ return ((T*)DynArrayBase<void*>::Get_At(p)); }
    void Put_At(size_t p, T* x)
	{ DynArrayBase<void*>::Put_At(p, (void*)x); }

    T*& operator [] (size_t p)
	{ return ((T*&)DynArrayBase<void*>::operator [] (p)); }
    T *const& operator [] (size_t p) const
	{ return ((T*&)DynArrayBase<void*>::operator [] (p)); }

    T** Base()
	{ return ((T**)DynArrayBase<void*>::Base()); }
    const T** Base() const
	{ return ((T**)DynArrayBase<void*>::Base()); }

    void Resize(size_t n)
	{ DynArrayBase<void*>::Resize(n); }
    void Resize(size_t n, T* x)
	{ DynArrayBase<void*>::Resize(n, (void*)x); }

    // container iterator methods
    virtual Boolean IsNewed() const;
    virtual void DestroyFromIterator();
    virtual Boolean RegisterIterator();
    virtual Boolean UnRegisterIterator();
    virtual size_t IteratorsActive();

#if OVERLOAD_NEW
    // new & delete operators
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

    // new & delete operators for arrays
#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

#if DYNARRAY_IMPLEMENT_STLCONTAINER
    STL_PTRCONTAINER_DECL(T, PtrDynArrayIterator<T>, ReverseRandomAccessIterator<T>)
#endif
};

template <class T>
PtrDynArray<T> operator + (const PtrDynArray<T>& Lhs_, const PtrDynArray<T>& Rhs_)
{
  return (PtrDynArray<T>(Lhs_) += Rhs_);
}

template <class T>
PtrDynArray<T> operator + (const PtrDynArray<T>& Lhs_, T* Rhs_)
{
  return (PtrDynArray<T>(Lhs_) += Rhs_);
}

template <class T>
PtrDynArray<T> operator + (T* Lhs_, const PtrDynArray<T>& Rhs_)
{
  return (PtrDynArray<T>(Lhs_) += Rhs_);
}

/****************************************************************************/
#if DYNARRAY_IMPLEMENT_STLITERATOR
template <class T>
class PtrDynArrayIterator : public RandomAccessIterator<T>
{
  friend Boolean operator == (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator != (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);

  friend Boolean operator < (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator > (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator <= (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator >= (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend ptrdiff_t operator - (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);

  protected:
    union
    {
      const PtrDynArray<T>* _ArrayObjPtr;
      PtrDynArray<T>* _ArrayVarPtr;
    };

    SizeType _Index;

    virtual Boolean AtEnd() const
	{ return (_Index == SizeType(-1)); }
    virtual const void* Address() const
	{ return _ArrayObjPtr; }

    inline void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    PtrDynArrayIterator(const PtrDynArray<T>* Ap_, Boolean DestroyFromIter_=FALSE);
    PtrDynArrayIterator(const PtrDynArrayIterator<T>& Ap_);
    virtual ~PtrDynArrayIterator();

    static PtrDynArrayIterator<T>* Make(PtrDynArray<T>* Ap_);
    static PtrDynArrayIterator<T>* Make(PtrDynArrayIterator<T>& Ap_);
    static const PtrDynArrayIterator<T>* MakeConst(const PtrDynArray<T>* Ap_);
    static const PtrDynArrayIterator<T>* MakeConst(const PtrDynArrayIterator<T>& Ap_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::PTRDYNARRAYITERATOR_TYPE; }
    virtual int ClassID() const;                

    virtual RandomAccessIterator<T>* Clone();
    virtual const RandomAccessIterator<T>* Clone() const;

    virtual Boolean IsValid() const;
    virtual Boolean IsValid(SizeType Index_) const;
    virtual Boolean More() const;
    virtual void Reset() const;
    virtual void SetToLast() const;
    virtual void SetToEnd() const;
    virtual SizeType Position() const
	{ return _Index; }

    virtual const T* ConstPointee() const;
    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual const PtrDynArrayIterator<T>& GotoHead() const;
    virtual const PtrDynArrayIterator<T>& GotoTail() const;

    virtual PtrDynArrayIterator<T>& GotoHead();
    virtual PtrDynArrayIterator<T>& GotoTail();

    virtual Boolean AtHead() const;
    virtual Boolean AtTail() const;

    virtual T& operator * ();
    virtual const T& operator * () const;
    virtual PtrDynArrayIterator<T>& operator [] (SizeType Index_);
    virtual const PtrDynArrayIterator<T>& operator [] (SizeType Index_) const;

    virtual RandomAccessIterator<T>& operator ++ ();
    virtual RandomAccessIterator<T> operator ++ (int);

    virtual const RandomAccessIterator<T>& operator ++ () const;
    virtual const RandomAccessIterator<T> operator ++ (int) const;

    virtual RandomAccessIterator<T>& operator -- ();
    virtual RandomAccessIterator<T> operator -- (int);

    virtual const RandomAccessIterator<T>& operator -- () const;
    virtual const RandomAccessIterator<T> operator -- (int) const;

    virtual RandomAccessIterator<T> operator + (SizeType Diff_);
    virtual const RandomAccessIterator<T> operator + (SizeType Diff_) const;
    virtual RandomAccessIterator<T> operator - (SizeType Diff_);
    virtual const RandomAccessIterator<T> operator - (SizeType Diff_) const;

    virtual RandomAccessIterator<T>& operator += (SizeType Diff_);
    virtual const RandomAccessIterator<T>& operator += (SizeType Diff_) const;
    virtual RandomAccessIterator<T>& operator -= (SizeType Diff_);
    virtual const RandomAccessIterator<T>& operator -= (SizeType Diff_) const;

    virtual RandomAccessIterator<T>& GiveIterator()
	{ return *this; }
    virtual const RandomAccessIterator<T>& GiveIterator() const
	{ return *this; }

    virtual PtrDynArrayIterator<T>& SetData(T* Ptr_);

    PtrDynArrayIterator<T>& operator = (T* Ptr_);
    PtrDynArrayIterator<T>& operator = (const PtrDynArrayIterator<T>& Rhs_);
};
#endif

/****************************************************************************/
#endif





