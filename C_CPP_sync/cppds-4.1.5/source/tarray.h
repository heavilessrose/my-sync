/****************************************************************************/
// FILE : Tarray.h
// PURPOSE:
// TypedArray and TypedSubArray classes for typed data items to be stored in
// the array class.
/****************************************************************************/
#ifndef TYPEDARRAY_H
#define TYPEDARRAY_H
#ifndef ARRAY_H
  #include "array.h"
#endif
#if ARRAY_IMPLEMENT_STLITERATOR
  #ifndef ITERATOR_CPP
    #include "iterator.cpp"
  #endif
#endif

/****************************************************************************/
// TypedArrayRef proxy class forward declaration
template <class T>
class TypedArrayRef;

// Typed sub array class for storage of typed elements
template <class T>
class TypedSubArray : public SubArray
{
  friend class TypedArrayRef<T>;

  private:
    // Virtual methods for handling memory allocation of array elements
    virtual T* Allocate(const T* Ptr_)					= 0;
    virtual Boolean SetOwnership(const T* Ptr_, Boolean IsOwner_)	= 0;
    virtual PtrVector::DATA_OBJECT MakeDataObject(PtrVector::DATA_OBJECT Trg_, PtrVector::DATA_OBJECT Src_) = 0;

  protected:
    // Class constructors for creation and copying
    TypedSubArray(size_t NumArgs_, size_t ThisIndex_);
    TypedSubArray(const TypedSubArray<T>& Obj_);

    // Subarray initialization method
    void InitSubArray(const TypedSubArray<T>& Obj_, size_t Max_);

    // Virtual array entry setting method with void data item
    virtual void* SetEntry(size_t Index_, void* Ptr_, Boolean Valid_=TRUE);

    // Replacement, assignment and retrieval methods for typed array elements
    virtual T* Replace(size_t Index_, const T* Ptr_, Boolean Valid_=TRUE) = 0;
    PtrVector::DATA_OBJECT RetrieveDataObject(size_t Index_) const;
    T* Retrieve(size_t Index_) const;

    PtrVector::DATA_OBJECT Assign(size_t Index_, PtrVector::DATA_OBJECT Ptr_, Boolean Valid_=TRUE);
    PtrVector::DATA_OBJECT Replace(size_t Index_, PtrVector::DATA_OBJECT Ptr_, Boolean Valid_=TRUE);

    // retrieval and validation methods with typed subarray argument.
    PtrVector::DATA_OBJECT RetrieveDataObject(const TypedSubArray<T>& This_, size_t Index_) const
	{ ARXMARKER("Start/End: TypedSubArray<T>::RetrieveDataObject(const TypedSubArray<T>&, size_t) const", ARRAY_ERRSTREAM)
	  return This_.RetrieveDataObject(Index_); }

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
// Typed array for storage of typed data items within typed subarrays
template <class T>
class TypedArray : public VoidArray
{
  friend class TypedArrayRef<T>;

  protected:
    // Object factory type definition and data member.
    // Used for polymorphic creation of typed objects given a type code
    typedef T*(*ObjMaker)(int);
    ObjMaker _Factory;

    // Null pointer exception
    void Xnullp()
	{ THROW (FallibleBase::DerefNullPointerErr()); }

    // Typed array class constructors
    TypedArray(TypedSubArray<T>* SubArrayPtr_, size_t Highest_, size_t Total_);
    TypedArray(TypedSubArray<T>* SubArrayPtr_);
    TypedArray(const TypedArray<T>& Obj_, TypedSubArray<T>* SubArrayPtr_);

    // Find number of non-null elements in vector
    virtual void FindSize();

    // Typed array indexing method and head subarray accessor method
    TypedArrayRef<T> TypedIndex(size_t Index_) const;
    RefInfo IndexRef(size_t Index_) const;
    TypedSubArray<T>& GiveSubArray() const;

    TypedSubArray<T>& GiveSubArray(const TypedArray<T>& This_) const
	{ ARXMARKER("Start/End: TypedArray<T>::GiveSubArray(const TypedArray<T>&) const", ARRAY_ERRSTREAM)
	  return This_.GiveSubArray(); }

  public:
    // Setting object maker function
    void SetObjectMaker(ObjMaker Maker_)
	{ ARXMARKER("Start/End: TypedArray<T>::SetObjectMaker(ObjMaker)", ARRAY_ERRSTREAM)
	  _Factory = Maker_; }

    // Make by type code method which invokes
    // the pointer to function data member
    T* MakeByTypeCode(int Code_);

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
// Typed array reference proxy class
template <class T>
class TypedArrayRef : public RefInfo
{
  friend class TypedArray<T>;

  private:
    // inline method for casting to typed subarray
    inline TypedSubArray<T>* CastTo_TypedSubArray() const
	{ ARXMARKER("Start/End: TypedArrayRef<T>::CastTo_TypedSubArray() const", ARRAY_ERRSTREAM)
	  return ((TypedSubArray<T>*)_BossPtr); }

  protected:
    // Typed array proxy class constructor
    TypedArrayRef(const RefInfo& Info_);

    // Array element validity state accessors
    Boolean IsDataDefined() const
	{ ARXMARKER("Start/End: TypedArrayRef<T>::IsDataDefined() const", ARRAY_ERRSTREAM)
	  return _BossPtr->IsDataDefined(_ArrayIndex); }
    PtrVector::DATA_OBJECT RetrieveDataObject() const
	{ ARXMARKER("Start/End: TypedArrayRef<T>::RetrieveDataObject() const", ARRAY_ERRSTREAM)
	  return CastTo_TypedSubArray()->RetrieveDataObject(_ArrayIndex); }
    Boolean IsValid() const
	{ ARXMARKER("Start/End: TypedArrayRef<T>::IsValid() const", ARRAY_ERRSTREAM)
	  return _BossPtr->IsValid(_ArrayIndex); }

    // Array element retrieval and copying methods
    T* Retrieve();

    TypedArrayRef<T>& Copy(TypedArray<T>& This_, size_t Index_);
    TypedArrayRef<T>& ConstCopy(const TypedArray<T>& This_, size_t Index_);
    TypedArrayRef<T>& AssignObject(PtrVector::DATA_OBJECT* This_, Boolean Valid_=TRUE);
};

/****************************************************************************/
#endif





