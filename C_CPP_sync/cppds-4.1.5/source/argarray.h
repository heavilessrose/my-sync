#ifndef ARGARRAY_H
#define ARGARRAY_H
#ifndef TYPEDARRAY_CPP
  #include "tarray.cpp"
#endif
#ifndef SIMPLEARRAY_CPP
  #include "simplearray.cpp"
#endif

/****************************************************************************/
template <class T>
class ArgArrayIterator;

template <class T>
class ArgArray;

template <class T>
class ArgArrayAcceptor : public ObjectAcceptor
{
  friend class ArgArray<T>;

  protected:
    union
    {
      const ArgArray<T>* _ObjPtr;
      ArgArray<T>* _VarPtr;
    };

    ArgArrayAcceptor(const ArgArray<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // ArgArray class specific acceptor methods
    virtual Boolean IsArgArray() const;
    virtual Boolean IsTypedArray() const;
    virtual void* CreateFromArgArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromPtrArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromArgArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromPtrArray(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToArgArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToArgArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToArgArray(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    // new & delete operators
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class ArgArrayRef;

template <class T>
class ArgArray_PtrProxy;

template <class T>
class ArgSubArray : public TypedSubArray<T>
{
  friend class ArgArray<T>;
  friend class ArgArrayRef<T>;
  friend class ArgArray_PtrProxy<T>;

  private:
    static T& DUMMY();

    virtual T* Allocate(const T* Ptr_);
    virtual Boolean SetOwnership(const T* Ptr_, Boolean IsOwner_);
    virtual PtrVector::DATA_OBJECT MakeDataObject(PtrVector::DATA_OBJECT Trg_, PtrVector::DATA_OBJECT Src_);

    T& Arg(size_t Index_, const T& Obj_, Boolean Valid_=TRUE);
    T& Arg(size_t Index_);
    const T& Arg(size_t Index_) const;

    inline T* Ptr(size_t Index_)
	{ ARXMARKER("Start/End: PtrArrayRef<T>::Ptr(size_t)", ARRAY_ERRSTREAM)
	  return Retrieve(Index_); }
    inline const T* Ptr(size_t Index_) const
	{ ARXMARKER("Start/End: PtrArrayRef<T>::Ptr(size_t) const", ARRAY_ERRSTREAM)
	  return Retrieve(Index_); }

    static ArgSubArray<T>* Make(const TypedSubArray<T>& Obj_)
	{ return (new ArgSubArray<T>(Obj_)); }
    static ArgSubArray<T>* Make(size_t NumArgs_, size_t ThisIndex_, int)
	{ return (new ArgSubArray<T>(NumArgs_, ThisIndex_, 0)); }
    static ArgSubArray<T>* Make(const T& Obj_, size_t NumArgs_)
	{ return (new ArgSubArray<T>(Obj_, NumArgs_)); }
    static ArgSubArray<T>* Make(const T* Parr_, size_t NumArgs_)
	{ return (new ArgSubArray<T>(Parr_, NumArgs_)); }
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    static ArgSubArray<T>* Make(const ArgSubArray<T>& Obj_)
	{ return (new ArgSubArray<T>(*((TypedSubArray<T>*)&Obj_))); }
#endif

    virtual SubArray* NewCopy() const;
    virtual SubArray* NewExtension(size_t Size_, size_t Index_);

    ArgSubArray(const T& Obj_, size_t NumArgs_);
    ArgSubArray(const T* Parr_, size_t NumArgs_);
    ArgSubArray(size_t NumArgs_, size_t ThisIndex_, int);
    ArgSubArray(const TypedSubArray<T>& Obj_);
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    ArgSubArray(const ArgSubArray<T>& Obj_);
#endif

  protected:
    // Replacement, assignment and retrieval methods for typed array elements
    virtual T* Replace(size_t Index_, const T* Ptr_, Boolean Valid_=TRUE);

  public:

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
};

/****************************************************************************/
template <class T>
class ArgArray : public TypedArray<T>
{
  friend class ArgSubArray<T>;
  friend class ArgArrayAcceptor<T>;

#if defined(__GNUC__)
  friend VoidArray& RootObject(ArgArray<T>& Arr_)
   { return *((VoidArray*)(&Arr_)); }
  friend const VoidArray& RootConstObject(const ArgArray<T>& Arr_)
   { return *((const VoidArray*)(&Arr_)); }
#else
  friend VoidArray& RootObject(ArgArray<T>& Arr_)
	{ return *((VoidArray*)(&Arr_)); }
  friend const VoidArray& RootConstObject(const ArgArray<T>& Arr_)
	{ return *((const VoidArray*)(&Arr_)); }
#endif

  friend ArgArray<T> operator + (const ArgArray<T>& Lhs_, const ArgArray<T>& Rhs_)
  { ARXMARKER("Start/End: ArgArray<T>::operator + (const ArgArray<T>&, const ArgArray<T>&)", ARRAY_ERRSTREAM)
    return (ArgArray<T>(Lhs_) += Rhs_); }
  friend ArgArray<T> operator + (const ArgArray<T>& Lhs_, const T& Rhs_)
  { ARXMARKER("Start/End: ArgArray<T>::operator + (const ArgArray<T>&, const T&)", ARRAY_ERRSTREAM)
    return (ArgArray<T>(Lhs_) += Rhs_); }  
  friend ArgArray<T> operator + (const T& Lhs_, const ArgArray<T>& Rhs_)
  { ARXMARKER("Start/End: ArgArray<T>::operator + (const T&, const ArgArray<T>&)", ARRAY_ERRSTREAM)
    return (ArgArray<T>(1, Lhs_) += Rhs_); }

  friend SimpleArray<int> SeriesArrayCompare(const ArgArray<T>& Sublist1_,
                                             const ArgArray<T>& Sublist2_)
  { if (_CompFunc && (Sublist1_.RunLength() == Sublist2_.RunLength()))
    { SimpleArray<int> Result_(Sublist1_.RunLength());
      size_t x, Max_;    
      for (x = 0, Max_ = Sublist1_.RunLength(); x < Max_; x++)
        if (Sublist1_[x].IsVoid() || Sublist2_[x].IsVoid())
          Result_[x] = INT_MAX;
        else
          Result_[x] = (*_CompFunc)(Sublist1_[x](), Sublist2_[x]());
      return Result_; }
    SimpleArray<int> Temp_;
    Temp_.SetError(TRUE);
    return Temp_; }

  public:
    typedef int(*CompFuncType)(const T&, const T&);

  protected:
    static CompFuncType _CompFunc;  // Comparison function for list items
  
    void IgnoreElement(istream& Is_, Boolean* Ok_);
    void ReadElement(istream& Is_, size_t Index_, Boolean* Ok_);
    void WriteElement(ostream& Os_, size_t Index_, Boolean* Ok_) const;

    void IgnoreElement(ByteStream& Is_, Boolean* Ok_);
    void ReadElement(ByteStream& Is_, size_t Index_, Boolean* Ok_);
    void WriteElement(ByteStream& Os_, size_t Index_, Boolean* Ok_) const;

    // comparison methods
    virtual Boolean IsEqualToArgArray(const ArgArray<T>* Ptr_) const;
    virtual Boolean IsLesserToArgArray(const ArgArray<T>* Ptr_) const;
    virtual Boolean IsGreaterToArgArray(const ArgArray<T>* Ptr_) const;

    void Swap(size_t index1, size_t index2);
    void ShellSwap(size_t lo, size_t hi, size_t Spread_, int Order_);
    void ShellSort(size_t lo, size_t hi, int Order_);

  public:
    ArgArray();
    ArgArray(size_t NumArgs_);
    ArgArray(const TypedArray<T>& Obj_);
    ArgArray(size_t NumArgs_, const T& Obj_);
    ArgArray(const T* Parr_, size_t Len_);
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    ArgArray(const ArgArray<T>& Obj_);
#endif

    static ArgArray<T>* Make();
    static ArgArray<T>* Make(size_t NumArgs_);
    static ArgArray<T>* Make(const Object& Obj_);
    static ArgArray<T>* Make(size_t Rep_, const T& Obj_);
    static ArgArray<T>* Make(const T* Parr_, size_t Len_);

    static const ArgArray<T>* MakeConst(const Object& Obj_);
    static const ArgArray<T>* MakeConst(size_t Rep_, const T& Obj_);
    static const ArgArray<T>* MakeConst(const T* Parr_, size_t Len_);

    // Set comparison function method
    static void SetComparisonFunction(int(*CmpFnc_)(const T&, const T&));

    // comparison methods
    SimpleArray<int> SeriesArrayCompare(const ArgArray<T>& Arr_) const;
    virtual int Compare(const ArgArray<T>& Arr_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // function application method
    virtual int ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const;
    virtual int ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_);
    virtual int ApplyUnary(int(*Fnc_)(T*));
    virtual int ApplyUnary(T(*Fnc_)(T));    

    // sort method
    virtual void Sort(int Order_=SortOrder::INCREASING);

    // assignment and virtual cloning methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;

    static ArgArray<T>& GiveNullObject();
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth () const;
    virtual long DataSize() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::ARGARRAY_TYPE; }
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
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL)
	{ return Is_; }
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    virtual ArgArray<T>& operator = (const ArgArray<T>& That_);
#endif
    virtual ArgArray<T>& operator = (const TypedArray<T>& That_);

#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    virtual ArgArray<T>& operator += (const ArgArray<T>& That_);
#endif
    virtual ArgArray<T>& operator += (const TypedArray<T>& That_);
    virtual ArgArray<T>& operator += (const T& Obj_);

    virtual ArgArray<T>& Append(const T& Obj_, size_t Rep_=1);
    virtual ArgArray<T>& Append(const T* Parr_, size_t Len_=1);

    virtual ArgArray<T>& AssignArg(const T& Obj_, size_t Rep_=1);
    virtual ArgArray<T>& AssignArg(const T* Parr_, size_t Len_=1);

    virtual ArgArray<T>& Insert(size_t InsPos_, const ArgArray<T>& Arr_);
    virtual ArgArray<T>& Insert(size_t InsPos_, const T& Obj_, size_t Rep_=1);
    virtual ArgArray<T>& Insert(size_t InsPos_, const T* Parr_, size_t Len_=1);

    virtual ArgArray<T>& Remove(size_t Pos_=0, size_t Len_=SIZET_MAX);

    virtual ArgArray<T>& Sub_Array(ArgArray<T>& Arr_, size_t Pos_, size_t Len_=SIZET_MAX) const;
    virtual ArgArray<T> Sub_Array(size_t Pos_, size_t Len_=SIZET_MAX) const;

    virtual void Swap(ArgArray<T>& Arr_);

    virtual const T& Get_At(size_t Pos_, Boolean& Valid_) const;
    virtual T& Get_At(size_t Pos_, Boolean& Valid_);
    virtual Boolean Put_At(size_t Pos_, const T& Obj_);

    virtual Boolean Grow(size_t Size_, const T& Obj_);
    virtual Boolean GrowTo(size_t Size_, const T& Obj_);

    virtual void Shrink(size_t Size_);
    virtual void ShrinkTo(size_t Size_);

    virtual ArgArrayRef<T> operator [] (size_t Index_);
    virtual const ArgArrayRef<T> operator [] (size_t Index_) const;

    virtual T* ToPtrArray(T* Parr_, size_t Len_=0);

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
};

/****************************************************************************/
#if ARRAY_IMPLEMENT_STLCONTAINER
template <class T>
class ArgArrayContainer
{
  protected:
    ArgArray<T>* _ContainerPtr;

  public:
    ArgArrayContainer(ArgArray<T>* Ap_);
    ArgArrayContainer(ArgArrayContainer<T>& Ap_);

    STL_CONTAINER_DECL(T, ArgArrayIterator<T>, ReverseRandomAccessIterator<T>)
};
#endif

#if ARRAY_IMPLEMENT_STLITERATOR
template <class T>
class ArgArrayIterator : public RandomAccessIterator<T>
{
  friend RandomAccessIterator<T>& RootObject(ArgArrayIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }
  friend const RandomAccessIterator<T>& RootConstObject(const ArgArrayIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }

  protected:
    union
    {
      const ArgArray<T>* _ArrayObjPtr;
      ArgArray<T>* _ArrayVarPtr;
    };

    SizeType _Index;

    virtual Boolean AtEnd() const
	{ return (_Index == SizeType(-1)); }
    virtual const void* Address() const
	{ return _ArrayObjPtr; }

    inline void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    ArgArrayIterator(const ArgArray<T>* Ap_, Boolean DestroyFromIter_=FALSE);
    ArgArrayIterator(const ArgArrayIterator<T>& Ap_);
    virtual ~ArgArrayIterator();

    static ArgArrayIterator<T>* Make(ArgArray<T>* Ap_);
    static ArgArrayIterator<T>* Make(ArgArrayIterator<T>& Ap_);

    static const ArgArrayIterator<T>* MakeConst(const ArgArray<T>* Ap_);
    static const ArgArrayIterator<T>* MakeConst(const ArgArrayIterator<T>& Ap_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::ARGARRAYITERATOR_TYPE; }
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

    virtual const RandomAccessIterator<T>& GotoHead() const;
    virtual const RandomAccessIterator<T>& GotoTail() const;
    
    virtual RandomAccessIterator<T>& GotoHead();
    virtual RandomAccessIterator<T>& GotoTail();

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

    virtual ArgArrayIterator<T>& SetData(const T& Obj_);
    virtual ArgArrayIterator<T>& Erase();

    ArgArrayIterator<T>& operator = (const ArgArrayIterator<T>& Rhs_);
    ArgArrayIterator<T>& operator = (T* Ptr_);
};
#endif

/****************************************************************************/
template <class T>
class ArgArrayRef : public TypedArrayRef<T>
{
  friend class ArgArray<T>;

  private:
    ArgSubArray<T>* CastTo_ArgSubArray() const;

  protected:
    ArgArrayRef(const RefInfo& Info_);

    inline void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    ArgArrayRef<T>& operator = (const T& Obj_);

    operator T& ();
    operator const T& () const;

    ArgArrayRef<T>& Erase();
    Boolean IsVoid() const;

    ArgArray_PtrProxy<T> operator & ();
    const ArgArray_PtrProxy<T> operator & () const;

    T& operator () ();
    const T& operator () () const;
};

/****************************************************************************/
template <class T>
class ArgArray_PtrProxy : public TypedArrayRef<T>
{
  friend class ArgArrayRef<T>;

  private:
    ArgSubArray<T>* CastTo_ArgSubArray() const;

    inline void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

    ArgArray_PtrProxy(const ArgArrayRef<T>& Info_);

  public:
    ArgArray_PtrProxy<T>& operator = (T* Ptr_);
    Boolean IsVoid() const;

    operator T* ();
    T& operator * ();
    operator const T* () const;
    const T& operator * () const;

    T* operator () ();
    const T* operator () () const;
};

/****************************************************************************/
#endif





