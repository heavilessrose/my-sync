#ifndef DYNARRAY_H
#define DYNARRAY_H
#ifndef DYNARRAYBASE_CPP
  #include "dynarraybase.cpp"
#endif
#ifndef SIMPLEARRAY_CPP
  #include "simplearray.cpp"
#endif

/****************************************************************************/
template <class T>
class DynArrayIterator;

template <class T>
class DynArray;

template <class T>
class DynArrayAcceptor : public ObjectAcceptor
{
  friend class DynArray<T>;

  protected:
    union
    {
      const DynArray<T>* _ObjPtr;
      DynArray<T>* _VarPtr;
    };

    DynArrayAcceptor(const DynArray<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // DynArray class specific acceptor methods
    virtual Boolean IsDynArray() const;
    virtual void* CreateFromDynArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDynArray(const ObjectAcceptor& Obj_);

    // DynArray comparison methods
    virtual Boolean IsEqualToDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToDynArray(const ObjectAcceptor&) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class DynArray : public ObjectImp, public DynArrayBase<T>
{
  friend class DynArrayAcceptor<T>;

  friend Object& RootObject(DynArray<T>& Arr_)
	{ return *((Object*)(&Arr_)); }
  friend const Object& RootConstObject(const DynArray<T>& Arr_)
	{ return *((const Object*)(&Arr_)); }

  friend SimpleArray<int> SeriesArrayCompare(const DynArray<T> &ia1, const DynArray<T> &ia2);
  friend DynArray<T> Apply(const DynArray<T>& ia, T (*func)(T))
        { DynArray<T> Temp_(ia); Temp_.Apply(func); return Temp_; }

  friend DynArray<T> operator + (const DynArray<T>& Lhs_, const DynArray<T>& Rhs_)
    { return (DynArray<T>(Lhs_) += Rhs_); }
  friend DynArray<T> operator + (const DynArray<T>& Lhs_, const T& Rhs_)
    { return (DynArray<T>(Lhs_) += Rhs_); }
  friend DynArray<T> operator + (const T& Lhs_, const DynArray<T>& Rhs_)
    { return (DynArray<T>(Lhs_) += Rhs_); }

  public:
    typedef int(*CompFuncType)(const T&, const T&);

  protected:
    // Object factory type definition and data member.
    // Used for polymorphic creation of typed objects given a type code
    typedef T*(*ObjMaker)(int);
    ObjMaker _Factory;
    static CompFuncType _CompFunc;  // Comparison function for list items    

    void IgnoreElement(istream& Is_, Boolean* Ok_);
    void ReadElement(istream& Is_, size_t Index_, Boolean* Ok_);
    void WriteElement(ostream& Os_, size_t Index_, Boolean* Ok_) const;

    void IgnoreElement(ByteStream& Is_, Boolean* Ok_);
    void ReadElement(ByteStream& Is_, size_t Index_, Boolean* Ok_);
    void WriteElement(ByteStream& Os_, size_t Index_, Boolean* Ok_) const;

    virtual Boolean IsEqualToDynArray(const DynArray<T>* Ptr_) const;
    virtual Boolean IsLesserToDynArray(const DynArray<T>* Ptr_) const;
    virtual Boolean IsGreaterToDynArray(const DynArray<T>* Ptr_) const;

    void _NVMGrow(size_t n, const T* s=0, Boolean Trim_=0);
    virtual void _Grow(size_t n, const T* s=0, Boolean Trim_=0);    

  public:
    DynArray();
    DynArray(size_t n, capacity c);
    DynArray(const DynArray<T>& x);
    DynArray(const T& x, size_t n=1);
    DynArray(const T* s, size_t n);

    static DynArray<T>* Make()
	{ return (new DynArray<T>); }
    static DynArray<T>* Make(size_t n, capacity c)
	{ return (new DynArray<T>(n, c)); }
    static DynArray<T>* Make(const Object& Obj_)
	{ DynArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._VarPtr;
	}
    static DynArray<T>* Make(const T& x, size_t n=1)
	{ return (new DynArray<T>(x, n)); }
    static DynArray<T>* Make(const T* s, size_t n)
	{ return (new DynArray<T>(s, n)); }

    static const DynArray<T>* MakeConst(const Object& Obj_)
	{ DynArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._ObjPtr;
	}
    static const DynArray<T>* MakeConst(const T& x, size_t n=1)
	{ return (new DynArray<T>(x, n)); }
    static const DynArray<T>* MakeConst(const T* s, size_t n)
	{ return (new DynArray<T>(s, n)); }

    // Set comparison function method
    static void SetComparisonFunction(int(*CmpFnc_)(const T&, const T&));

    // comparison methods
    SimpleArray<int> SeriesArrayCompare(const DynArray<T> &ia) const;
    virtual int Compare(const DynArray<T>& Arr_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);

    static DynArray<T>& GiveNullObject();
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth () const;
    virtual long DataSize() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::DYNARRAY_TYPE; }
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

    // Setting object maker function
    void SetObjectMaker(ObjMaker Maker_)
	{ _Factory = Maker_; }

    // Make by type code method which invokes
    // the pointer to function data member
    T* MakeByTypeCode(int Code_);

    // sorting method
    DynArray<T>& Sort(SortingHelper<T, T>* SortHelpPtr_);

    // function application method
    DynArray<T>& ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const;
    DynArray<T>& ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_);
    DynArray<T>& ApplyUnary(T(*func)(T));    
    DynArray<T>& ApplyUnary(int(*Fnc_)(T*));

    DynArray<T>& operator = (const DynArray<T>& r);
    DynArray<T>& Append(const T& x, size_t n=1)
	{ DynArrayBase<T>::Append(x, n);
	  return *this; }
    DynArray<T>& Append(const T* s, size_t n=1, size_t d=1)
	{ DynArrayBase<T>::Append(s, n, d);
	  return *this; }
    DynArray<T>& operator += (const DynArray<T>& r)
	{ DynArrayBase<T>::operator += (r);
	  return *this; }
    DynArray<T>& operator += (const T& r)
	{ DynArrayBase<T>::operator += (r);
	  return *this; }

    DynArray<T>& AssignArg(const T& x, size_t n=1)
	{ DynArrayBase<T>::AssignArg(x, n);
	  return *this; }
    DynArray<T>& AssignArg(const T* s, size_t n=1, size_t d=1)
	{ DynArrayBase<T>::AssignArg(s, n, d);
	  return *this; }

    DynArray<T>& Insert(size_t p, const DynArray<T>& x)
	{ DynArrayBase<T>::Insert(p, x);
	  return *this; }
    DynArray<T>& Insert(size_t p, const T& x, size_t n=1)
	{ DynArrayBase<T>::Insert(p, x, n);
	  return *this; }
    DynArray<T>& Insert(size_t p, const T* s, size_t n=1, size_t d=1)
	{ DynArrayBase<T>::Insert(p, s, n, d);
	  return *this; }

    DynArray<T>& Remove(size_t p=0, size_t n=SIZET_MAX)
	{ DynArrayBase<T>::Remove(p, n);
	  return *this; }

    DynArray<T>& Sub_Array(DynArray<T>& x, size_t p, size_t n=SIZET_MAX)
	{ DynArrayBase<T>::Sub_Array(x, p, n);
	  return *this; }

    void Swap(DynArray<T>& x)
	{ DynArrayBase<T>::Swap(x); }

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
    STL_CONTAINER_DECL(T, DynArrayIterator<T>, ReverseRandomAccessIterator<T>)
#endif
};

/****************************************************************************/
#if DYNARRAY_IMPLEMENT_STLITERATOR
template <class T>
class DynArrayIterator : public RandomAccessIterator<T>
{
  friend RandomAccessIterator<T>& RootObject(DynArrayIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }
  friend const RandomAccessIterator<T>& RootConstObject(const DynArrayIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }

  private:
    union
    {
      const DynArray<T>* _ArrayObjPtr;
      DynArray<T>* _ArrayVarPtr;
    };

    SizeType _Index;

    virtual Boolean AtEnd() const
	{ return (_Index == SizeType(-1)); }
    virtual const void* Address() const
	{ return _ArrayObjPtr; }

    void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    DynArrayIterator(const DynArray<T>* Ap_, Boolean DestroyFromIter_=FALSE);
    DynArrayIterator(const DynArrayIterator<T>& Ap_);
    virtual ~DynArrayIterator();

    static DynArrayIterator<T>* Make(DynArray<T>* Ap_);
    static DynArrayIterator<T>* Make(DynArrayIterator<T>& Ap_);

    static const DynArrayIterator<T>* MakeConst(const DynArray<T>* Ap_);
    static const DynArrayIterator<T>* MakeConst(const DynArrayIterator<T>& Ap_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::DYNARRAYITERATOR_TYPE; }
    virtual int ClassID() const;            

    virtual RandomAccessIterator<T>* Clone();
    virtual const RandomAccessIterator<T>* Clone() const;

    virtual Boolean IsValid(SizeType Index_) const;
    virtual Boolean IsValid() const;
    virtual Boolean More() const;
    virtual void Reset() const;
    virtual void SetToLast() const;
    virtual void SetToEnd() const;
    virtual SizeType Position() const
	{ return _Index; }

    virtual const T* ConstPointee() const;
    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual const DynArrayIterator<T>& GotoHead() const;
    virtual const DynArrayIterator<T>& GotoTail() const;

    virtual DynArrayIterator<T>& GotoHead();
    virtual DynArrayIterator<T>& GotoTail();

    virtual Boolean AtHead() const;
    virtual Boolean AtTail() const;

    virtual T& operator * ();
    virtual const T& operator * () const;
    virtual RandomAccessIterator<T>& operator [] (SizeType Index_);
    virtual const RandomAccessIterator<T>& operator [] (SizeType Index_) const;

    virtual RandomAccessIterator<T>& operator ++ ();
    virtual RandomAccessIterator<T> operator ++ (int);

    virtual const RandomAccessIterator<T>& operator ++ () const;
    virtual const RandomAccessIterator<T> operator ++ (int) const;

    virtual RandomAccessIterator<T>& operator -- ();
    virtual RandomAccessIterator<T> operator -- (int);

    virtual const RandomAccessIterator<T>& operator -- () const;
    virtual const RandomAccessIterator<T> operator -- (int) const;

    virtual RandomAccessIterator<T> operator + (SizeType Dist_);
    virtual const RandomAccessIterator<T> operator + (SizeType Dist_) const;
    virtual RandomAccessIterator<T> operator - (SizeType Dist_);
    virtual const RandomAccessIterator<T> operator - (SizeType Dist_) const;

    virtual RandomAccessIterator<T>& operator += (SizeType Diff_);
    virtual const RandomAccessIterator<T>& operator += (SizeType Diff_) const;
    virtual RandomAccessIterator<T>& operator -= (SizeType Diff_);
    virtual const RandomAccessIterator<T>& operator -= (SizeType Diff_) const;

    virtual RandomAccessIterator<T>& GiveIterator()
	{ return *this; }
    virtual const RandomAccessIterator<T>& GiveIterator() const
	{ return *this; }

    virtual DynArrayIterator<T>& SetData(const T& Obj_);
    virtual DynArrayIterator<T>& Erase();

    DynArrayIterator<T>& operator = (T* Ptr_);
    DynArrayIterator<T>& operator = (const DynArrayIterator<T>& Rhs_);
};
#endif

/****************************************************************************/
#endif





