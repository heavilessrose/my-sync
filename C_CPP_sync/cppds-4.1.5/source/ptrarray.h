#ifndef PTRARRAY_H
#define PTRARRAY_H
#ifndef TYPEDARRAY_CPP
  #include "tarray.cpp"
#endif
#ifndef SIMPLEARRAY_CPP
  #include "simplearray.cpp"
#endif

// 1) Use Bit vector to indicate state of used, but unoccupied cells in the
//    array instead of the DUMMY static T variable. Other methods that
//    test for NULL and the DUMMY static variable needs to be changed.
// 2) Implement the ObjectID method for both PtrSubArray and ArgSubArray
//    classes for implementing object identity of SubArray objects.
//    This is not really needed since the SubArray classes are conceptually
//    a part of the Array classes, but only implemented separately because of
//    necessity.
// 3) Rewrite NullChain method of PtrSubArray class to pass in successive
//    tail SubArray pointers instead of destroying back links.
// 4) Implement generic factory function methods for polymorphic objects that
//    support polymorphic construction. Make(T& Obj_) in the ArgSubArray
//    class and normal construction for primitive types. template functions
//    could be used to differentiate between polymorpic and non-polymorphic
//    types.
// 5) Move the DUMMY static variable definition and methods to the
//    ArgSubArray class. Implement the ValidData method to test for valid
//    object references in both the ArgSubArray and ArgArray Classes.
//    This follows normal C++ object array semantics, so no conflicts.
//    Return this for NULL cells in the ArgArray class.
// 6) Address-Of feature: overload the address of & operator for the
//    ArgArrayRef class to return a proxy class that supports pointer
//    operations. This feature allows for swapping pointers of the underlying
//    array without needing to explicitly declare a pointer array. Uses the
//    Retrieve() and Assign(T*, size_t) straight assignment and retrieval
//    methods for supporting the operations.
// 7) A PtrVector class which uses generic void* array of pointers to store
//    data. Provides methods for encapsulating the state of data and
//    lazy evaluation by placing NULL for invalid data and unsetting the
//    validity bit for that index. Also returns number of cells active as
//    in valid NULL data.
// 8) virtual Memory allocation deallocation methods for different subarray
//    classes to handle different memory allocation strategies
//    polymorphically.
//
/****************************************************************************/
template <class T>
class PtrArrayIterator;

template <class T>
class PtrArray;

template <class T>
class PtrArrayAcceptor : public ObjectAcceptor
{
  friend class PtrArray<T>;

  protected:
    union
    {
      const PtrArray<T>* _ObjPtr;
      PtrArray<T>* _VarPtr;
    };

    PtrArrayAcceptor(const PtrArray<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // PtrArray class specific acceptor methods
    virtual Boolean IsPtrArray() const;
    virtual Boolean IsTypedArray() const;
    virtual void* CreateFromPtrArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromArgArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromPtrArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromArgArray(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToPtrArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToPtrArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToPtrArray(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    // new & delete operators
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class PtrArrayRef;

template <class T>
class PtrSubArray : public TypedSubArray<T>
{
  friend class PtrArrayRef<T>;
  friend class PtrArray<T>;

  private:
    virtual T* Allocate(const T* Ptr_);
    virtual Boolean SetOwnership(const T* Ptr_, Boolean IsOwner_);
    virtual PtrVector::DATA_OBJECT MakeDataObject(PtrVector::DATA_OBJECT Trg_, PtrVector::DATA_OBJECT Src_);

    T* Arg(size_t Index_, T* Ptr_, Boolean Valid_=TRUE);
    T* Arg(size_t Index_);
    const T* Arg(size_t Index_) const;

    T* Ptr(size_t Index_);
    const T* Ptr(size_t Index_) const;

    static PtrSubArray<T>* Make(const TypedSubArray<T>& Obj_)
	{ return (new PtrSubArray<T>(Obj_)); }
    static PtrSubArray<T>* Make(size_t NumArgs_, size_t ThisIndex_, int)
	{ return (new PtrSubArray<T>(NumArgs_, ThisIndex_, 0)); }
    static PtrSubArray<T>* Make(T* Ptr_, size_t NumArgs_)
	{ return (new PtrSubArray<T>(Ptr_, NumArgs_)); }
    static PtrSubArray<T>* Make(PtrVector::DATA_OBJECT* Ptr_, size_t NumArgs_)
	{ return (new PtrSubArray<T>(Ptr_, NumArgs_)); }
    static PtrSubArray<T>* Make(T** Parr_, size_t NumArgs_)
	{ return (new PtrSubArray<T>(Parr_, NumArgs_)); }
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    static PtrSubArray<T>* Make(const PtrSubArray<T>& Obj_)
	{ return (new PtrSubArray<T>(*((TypedSubArray<T>*)&Obj_))); }
#endif

    virtual SubArray* NewCopy() const;
    virtual SubArray* NewExtension(size_t Size_, size_t Index_);

    PtrSubArray(T* Ptr_, size_t NumArgs_);
    PtrSubArray(PtrVector::DATA_OBJECT* Ptr_, size_t NumArgs_);
    PtrSubArray(T** Parr_, size_t NumArgs_);
    PtrSubArray(size_t NumArgs_, size_t ThisIndex_, int);
    PtrSubArray(const TypedSubArray<T>& Obj_);
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    PtrSubArray(const PtrSubArray<T>& Obj_);
#endif
    
  protected:
    // Replacement, assignment and retrieval methods for typed array elements
    T* Replace(size_t Index_, const T* Ptr_, Boolean Valid_=TRUE);

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
class PtrArrayWrapper;

template <class T>
class PtrArray : public TypedArray<T>
{
  friend class PtrSubArray<T>;
  friend class PtrArrayAcceptor<T>;

#if defined(__GNUC__)
  friend VoidArray& RootObject(PtrArray<T>& Arr_)
   { return *((VoidArray*)(&Arr_)); }
  friend const VoidArray& RootConstObject(const PtrArray<T>& Arr_)
   { return *((const VoidArray*)(&Arr_)); }
#else
  friend VoidArray& RootObject(PtrArray<T>& Arr_)
	{ return *((VoidArray*)(&Arr_)); }
  friend const VoidArray& RootConstObject(const PtrArray<T>& Arr_)
	{ return *((const VoidArray*)(&Arr_)); }
#endif

#if !defined(__TURBOC__) & !defined(__BORLANDC__)
  friend PtrArrayWrapper<T> operator + (PtrArray<T>& Lhs_, PtrArray<T>& Rhs_)
  { ARXMARKER("Start: PtrArray<T>::operator + (PtrArray<T>&, PtrArray<T>&)", ARRAY_ERRSTREAM)
    PtrArray<T> Lhsp_(Lhs_);
    Lhsp_ += Rhs_;
    ARXMARKER("End: PtrArray<T>::operator + (PtrArray<T>&, PtrArray<T>&)", ARRAY_ERRSTREAM)
    return PtrArrayWrapper<T>(Lhsp_); }
  friend PtrArrayWrapper<T> operator + (PtrArray<T>& Lhs_, PtrVector::DATA_OBJECT* Rhs_)
  { ARXMARKER("Start: PtrArray<T>::operator + (PtrArray<T>&, T*)", ARRAY_ERRSTREAM)
    PtrArray<T> Lhsp_(Lhs_);
    Lhsp_ += Rhs_;
    ARXMARKER("End: PtrArray<T>::operator + (PtrArray<T>&, T*)", ARRAY_ERRSTREAM)
    return PtrArrayWrapper<T>(Lhsp_); }
  friend PtrArrayWrapper<T> operator + (PtrArray<T>& Lhs_, T* Rhs_)
  { ARXMARKER("Start: PtrArray<T>::operator + (PtrArray<T>&, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
    PtrArray<T> Lhsp_(Lhs_);
    Lhsp_ += Rhs_;
    ARXMARKER("End: PtrArray<T>::operator + (PtrArray<T>&, PtrVector::DATA_OBJECT*)", ARRAY_ERRSTREAM)
    return PtrArrayWrapper<T>(Lhsp_); }
  friend PtrArrayWrapper<T> operator + (PtrVector::DATA_OBJECT* Lhs_, PtrArray<T>& Rhs_)
  { ARXMARKER("Start: PtrArray<T>::operator + (T*, PtrArray<T>&)", ARRAY_ERRSTREAM)
    PtrArray<T> Lhsp_(1, Lhs_);
    Lhsp_ += Rhs_;
    ARXMARKER("End: PtrArray<T>::operator + (T*, PtrArray<T>&)", ARRAY_ERRSTREAM)
    return PtrArrayWrapper<T>(Lhsp_); }
  friend PtrArrayWrapper<T> operator + (T* Lhs_, PtrArray<T>& Rhs_)
  { ARXMARKER("Start: PtrArray<T>::operator + (PtrVector::DATA_OBJECT*, PtrArray<T>&)", ARRAY_ERRSTREAM)
    PtrArray<T> Lhsp_(1, Lhs_);
    Lhsp_ += Rhs_;
    ARXMARKER("End: PtrArray<T>::operator + (PtrVector::DATA_OBJECT*, PtrArray<T>&)", ARRAY_ERRSTREAM)
    return PtrArrayWrapper<T>(Lhsp_); }
#else
  friend PtrArray<T> operator + (PtrArray<T>& Lhs_, PtrArray<T>& Rhs_);
  friend PtrArray<T> operator + (PtrArray<T>& Lhs_, PtrVector::DATA_OBJECT* Rhs_);
  friend PtrArray<T> operator + (PtrArray<T>& Lhs_, T* Rhs_);
  friend PtrArray<T> operator + (PtrVector::DATA_OBJECT* Lhs_, PtrArray<T>& Rhs_);
  friend PtrArray<T> operator + (T* Lhs_, PtrArray<T>& Rhs_);
#endif

  friend SimpleArray<int> SeriesArrayCompare(const PtrArray<T>& Sublist1_,
                                             const PtrArray<T>& Sublist2_)
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
    typedef int(*CompFuncType)(const T*, const T*);

  protected:
    static CompFuncType _CompFunc;  // Comparison function for list items

    // comparison methods
    virtual Boolean IsEqualToPtrArray(const PtrArray<T>* Ptr_) const;
    virtual Boolean IsLesserToPtrArray(const PtrArray<T>* Ptr_) const;
    virtual Boolean IsGreaterToPtrArray(const PtrArray<T>* Ptr_) const;

    void Swap(size_t index1, size_t index2);
    void ShellSwap(size_t lo, size_t hi, size_t Spread_, int Order_);
    void ShellSort(size_t lo, size_t hi, int Order_);

  public:
    PtrArray();
    PtrArray(size_t NumArgs_);
    PtrArray(TypedArray<T>& Obj_);
    PtrArray(size_t NumArgs_, PtrVector::DATA_OBJECT* Ptr_);
    PtrArray(size_t NumArgs_, T* Ptr_);
    PtrArray(T** Parr_, size_t Len_);
#if !defined(__BORLANDC__) & !defined(__TURBOC__)        
    PtrArray(PtrArray<T>& Obj_);
    PtrArray(const PtrArrayWrapper<T>& Obj_);
#endif        

    static PtrArray<T>* Make();
    static PtrArray<T>* Make(size_t NumArgs_);
    static PtrArray<T>* Make(size_t Rep_, PtrVector::DATA_OBJECT* Ptr_);
    static PtrArray<T>* Make(size_t Rep_, T* Ptr_);
    static PtrArray<T>* Make(T** Parr_, size_t Len_);
    static PtrArray<T>* Make(Object& Obj_);

    static const PtrArray<T>* MakeConst(size_t Rep_, PtrVector::DATA_OBJECT* Ptr_);
    static const PtrArray<T>* MakeConst(size_t Rep_, T* Ptr_);
    static const PtrArray<T>* MakeConst(T** Parr_, size_t Len_);
    static const PtrArray<T>* MakeConst(const Object& Obj_);

    // Set comparison function method
    static void SetComparisonFunction(int(*CmpFnc_)(const T*, const T*));

    // comparison methods
    SimpleArray<int> SeriesArrayCompare(const PtrArray<T>& Arr_) const;
    virtual int Compare(const PtrArray<T>& Arr_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // function application method
    virtual int ApplyBinary(int(*Fnc_)(const T*, const T*), const T* Ptr_) const;
    virtual int ApplyBinary(int(*Fnc_)(T*, T*), T* Ptr_);
    virtual int ApplyUnary(int(*Fnc_)(T*));
    virtual int ApplyUnary(T*(*Fnc_)(T*));

    // sort method
    virtual void Sort(int Order_=SortOrder::INCREASING);

    static PtrVector::DATA_OBJECT* MakeDataStore(T* Ptr_, Boolean OwnerShip_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;

    static PtrArray<T>& GiveNullObject();
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::PTRARRAY_TYPE; }
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
    virtual PtrArray<T>& operator = (PtrArray<T>& That_);
    virtual PtrArray<T>& operator = (const PtrArrayWrapper<T>& That_);
#endif
    virtual PtrArray<T>& operator = (TypedArray<T>& That_);

#if !defined(__BORLANDC__) & !defined(__TURBOC__)
    virtual PtrArray<T>& operator += (PtrArray<T>& That_);
    virtual PtrArray<T>& operator += (const PtrArrayWrapper<T>& That_);
#endif
    virtual PtrArray<T>& operator += (TypedArray<T>& That_);
    virtual PtrArray<T>& operator += (PtrVector::DATA_OBJECT* Ptr_);
    virtual PtrArray<T>& operator += (T* Ptr_);

    virtual PtrArray<T>& Append(PtrVector::DATA_OBJECT* Ptr_, size_t Rep_=1);
    virtual PtrArray<T>& Append(T* Ptr_, size_t Rep_=1);
    virtual PtrArray<T>& Append(T** Parr_, size_t Len_=1);

    virtual PtrArray<T>& AssignArg(PtrVector::DATA_OBJECT* Ptr_, size_t Rep_=1);
    virtual PtrArray<T>& AssignArg(T* Ptr_, size_t Rep_=1);
    virtual PtrArray<T>& AssignArg(T** Parr_, size_t Len_=1);

    virtual PtrArray<T>& Insert(size_t InsPos_, PtrArray<T>& Arr_);
    virtual PtrArray<T>& Insert(size_t InsPos_, PtrVector::DATA_OBJECT* Ptr_, size_t Rep_=1);
    virtual PtrArray<T>& Insert(size_t InsPos_, T* Ptr_, size_t Rep_=1);
    virtual PtrArray<T>& Insert(size_t InsPos_, T** Parr_, size_t Len_=1);

    virtual PtrArray<T>& Remove(size_t Pos_=0, size_t Len_=SIZET_MAX);

    virtual PtrArray<T>& Sub_Array(PtrArray<T>& Arr_, size_t Pos_, size_t Len_=SIZET_MAX);
    virtual const PtrArray<T> Sub_Array(size_t Pos_, size_t Len_=SIZET_MAX) const;

    virtual void Swap(PtrArray<T>& Arr_);

    virtual const T* Get_At(size_t Pos_, Boolean& Valid_) const;
    virtual T* Get_At(size_t Pos_, Boolean& Valid_);
    virtual Boolean Put_At(size_t Pos_, PtrVector::DATA_OBJECT* Ptr_);
    virtual Boolean Put_At(size_t Pos_, T* Ptr_);

    virtual Boolean Grow(size_t Size_, T* Ptr_);
    virtual Boolean Grow(size_t Size_, PtrVector::DATA_OBJECT* Ptr_);
    virtual Boolean GrowTo(size_t Size_, T* Ptr_);
    virtual Boolean GrowTo(size_t Size_, PtrVector::DATA_OBJECT* Ptr_);

    virtual void Shrink(size_t Size_);
    virtual void ShrinkTo(size_t Size_);

    virtual PtrArrayRef<T> operator [] (size_t Index_);
    virtual const PtrArrayRef<T> operator [] (size_t Index_) const;

    virtual T** ToPtrArray(T** Parr_, size_t Len_=0);

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

#if !defined(__TURBOC__) & !defined(__BORLANDC__)
template <class T>
class PtrArrayWrapper : public PtrArray<T>
{
  public:
    PtrArrayWrapper(const PtrArray<T>& Obj_);
    PtrArrayWrapper(const PtrArrayWrapper<T>& Obj_);
};
#endif

/****************************************************************************/
#if ARRAY_IMPLEMENT_STLCONTAINER
template <class T>
class PtrArrayContainer
{
  protected:
    PtrArray<T>* _ContainerPtr;

  public:
    PtrArrayContainer(PtrArray<T>* Ap_);
    PtrArrayContainer(PtrArrayContainer<T>& Ap_);

    STL_PTRCONTAINER_DECL(T, PtrArrayIterator<T>, ReverseRandomAccessIterator<T>)
};
#endif

#if ARRAY_IMPLEMENT_STLITERATOR
template <class T>
class PtrArrayIterator : public RandomAccessIterator<T>
{
  friend RandomAccessIterator<T>& RootObject(PtrArrayIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }
  friend const RandomAccessIterator<T>& RootConstObject(const PtrArrayIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }

  protected:
    union
    {
      const PtrArray<T>* _ArrayObjPtr;
      PtrArray<T>* _ArrayVarPtr;
    };

    SizeType _Index;

    virtual Boolean AtEnd() const
	{ return (_Index == SizeType(-1)); }
    virtual const void* Address() const
	{ return _ArrayObjPtr; }

    inline void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    PtrArrayIterator(const PtrArray<T>* Ap_, Boolean DestroyFromIter_=FALSE);
    PtrArrayIterator(const PtrArrayIterator<T>& Ap_);
    virtual ~PtrArrayIterator();

    static PtrArrayIterator<T>* Make(PtrArray<T>* Ap_);
    static PtrArrayIterator<T>* Make(PtrArrayIterator<T>& Ap_);
    static const PtrArrayIterator<T>* MakeConst(const PtrArray<T>* Ap_);
    static const PtrArrayIterator<T>* MakeConst(const PtrArrayIterator<T>& Ap_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::PTRARRAYITERATOR_TYPE; }
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

    virtual PtrArrayIterator<T>& SetData(T* Ptr_);
    virtual PtrArrayIterator<T>& SetDataObject(PtrVector::DATA_OBJECT* Ptr_);

    PtrArrayIterator<T>& operator = (const PtrArrayIterator<T>& Rhs_);
    PtrArrayIterator<T>& operator = (T* Ptr_);
};
#endif

/****************************************************************************/
template <class T>
class PtrArrayRef : public TypedArrayRef<T>
{
  friend class PtrArray<T>;

  private:
    PtrSubArray<T>* CastTo_PtrSubArray() const;

    inline void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  protected:
    PtrArrayRef(const RefInfo& Info_);

  public:
    PtrArrayRef<T>& operator = (T* Ptr_);
    PtrArrayRef<T>& Assign(PtrVector::DATA_OBJECT* Ptr_);

    operator T* ();
    operator const T* () const;

    T& operator * ();
    const T& operator * () const;
    
    Boolean IsVoid() const;

    T* operator () ();
    const T* operator () () const;
};

/****************************************************************************/
#endif







