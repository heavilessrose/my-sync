/****************************************************************************/
// FILE : Array.h
// PURPOSE : Header file for multipurpose polymorphic array class
//	     The array handle memory allocation and shrinks and grows
//	     depending on total usage of the array. Also could store
//	     objects of derived classes and be treated polymorphically.
//           Uses lazy evaluation to save memory. Only allocates memory if
//	     array cell is actually in use, otherwise it is set NULL.
//	     Able to tell difference between invalid data (uninitialized)
//	     and NULL data. Has method to bound array to a set limit and
//	     many other useful methods.
/****************************************************************************/
#ifndef ARRAY_H
#define ARRAY_H
#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef PTRVECTOR_H
  #include "ptrvect.h"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef ARRAYCLASS_H
  #include "arrayclass.h"
#endif

/****************************************************************************/
// Subarray class
class SubArray : protected PtrVector
{
  private:
    size_t _Index;		// Highest index accessed by subarray
    ArrayClass* _ArrayBase; 	// Pointer to owning array object
    SubArray* _BackPtr;		// Back pointer to previous subarray
    SubArray* _Extension;	// Pointer to extension subarray

    // Disallow assignment of subarrays
    SubArray& operator = (const SubArray& Obj_)
	{ return *this; }

    // Link pointers set methods
    inline void SetBackPtr(SubArray* Back_)
	{ ARXMARKER("Start/End: SubArray::SetBackPtr(SubArray*)", ARRAY_ERRSTREAM)
	  _BackPtr = Back_; }
    inline void SetExtension(SubArray* Ext_)
	{ ARXMARKER("Start/End: SubArray::SetExtension(SubArray*)", ARRAY_ERRSTREAM)
	  _Extension = Ext_; }

    // Set tail of array method
    void SetTail(SubArray* Ptr_);
    inline void SetTail()
	{ ARXMARKER("Start/End: SubArray::SetTail(SubArray*)", ARRAY_ERRSTREAM)
	  SetTail(this); }

    // Base array maintenance methods
    void SetRunLength();
    void SetArray(size_t Distance_, short Direction_);

    // Array index validation and set methods
    void ValidateIndex();
    void SetIndex(size_t Index_);
    inline size_t NewIndex(size_t Index_) const
	{ ARXMARKER("Start/End: SubArray::NewIndex(size_t)", ARRAY_ERRSTREAM)
	  return (Index_ - GetLength()); }

    // Subarray shrinking, growing and extension methods
    size_t Deactivate(size_t Size_, size_t Deleted_);
    size_t HelpShrink(size_t Size_, Boolean DoDelete_);

    RefInfo Grow(size_t PrevSize_, size_t Index_);
    RefInfo Extension(size_t Index_);

    // Subarray copy or extension methods
    virtual SubArray* NewCopy() const 				= 0;
    virtual SubArray* NewExtension(size_t Size_, size_t Index_) = 0;

#if ARRAY_DELETE_FROM_TAIL
    // Object destruction method
    void Clear();
#endif

  protected:
    // Subarray constructors
    SubArray(size_t NumArgs_, size_t ThisIndex_);
    SubArray(const SubArray& Obj_);
    virtual SubArray* Clone() const;

    // Array element accessing, mutating methods
    virtual void* SetEntry(size_t Index_, void* Obj_, Boolean Valid_);
    virtual void* GetEntry(size_t Index_) const;

    PtrVector::DATA_OBJECT SetDataObject(size_t Index_, PtrVector::DATA_OBJECT Obj_, Boolean Valid_);
    PtrVector::DATA_OBJECT GetDataObject(size_t Index_) const;

    // Data member accesors
    inline ArrayClass* GetBase() const
	{ return _ArrayBase; }
    inline size_t GetIndex() const
	{ return _Index; }

    // Subarray exceptions
    inline void Xlen() const
	{ THROW (FallibleBase::ArrayLengthErr()); }
    inline void Xran() const
	{ THROW (FallibleBase::ArrayIndexRangeErr()); }
    inline void Xinv() const
	{ THROW (FallibleBase::ArrayInvalidArgumentErr()); }

  public:
    virtual ~SubArray();

    // Array cell validity status accessor methods
    PtrVector::IsValid;
    PtrVector::IsInvalid;
    PtrVector::IsNull;
    PtrVector::IsDataDefined;
    PtrVector::Ownership;

    // inline methods for accessing active cell count and total vector length
    PtrVector::ActiveCells;
    PtrVector::GetLength;

    // Subarray shrinking methods
    size_t Shrink(size_t Size_)
	{ ARXMARKER("Start/End: SubArray::Shrink(size_t)", ARRAY_ERRSTREAM)
	  return HelpShrink(Size_, _ArrayBase->ShouldEraseIfNull()); }
    size_t ShrinkTo(size_t Size_);

    // Subarray indexing method
    RefInfo Index(size_t PrevSize_, size_t Index_);

    // Null subarray chain deletion methods
    void Reset();
    SubArray* NullChain(SubArray* Ext_=NULL);

    // Subarray links and counters initiation methods
    void InitPointers(ArrayClass* Base_, SubArray* Back_=NULL);
    void InitCounters();

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
// Generic void type array
class VoidArray : public ArrayClass, public ObjectImp
{
  friend class SubArray;
  friend class RefInfo;
  
  ROOTOBJECTFNCS_STUB(VoidArray, VoidArray)
  WHATISFNC_DEFN(VoidArray)
  IOFNCS_DEFN(VoidArray)
  CLONEFNCS_DEFN(VoidArray)
  SWAPFNC_DEFN(VoidArray)
  SIZEOFDATAFNC_DEFN(VoidArray)
  FORMWIDTHOFOBJECTFNC_DEFN(VoidArray)
  ASSIGNFNC_DEFN(VoidArray)
  NONCONSTASSIGNFNC_DEFN(VoidArray)
  COMPARISONFNCS_DEFN(VoidArray)
  REDIRECTFNCS_DEFN(VoidArray)
  COUNTOFFNC_DEFN(VoidArray)
  ISNULLOBJECTFNC_DEFN(VoidArray)
  SHOWINFOFNC_DEFN(VoidArray)
  HASHINGFNCS_DEFN(VoidArray)
  OBJECTPROPERTIESFNCS_DEFN(VoidArray)
  CONTAINER_ZEROVALUEFNCS_DEFN(VoidArray)

  protected:
    enum { DEFAULT_SIZE = 1 };	// Default array size

    SubArray* _Head;		// Head subarray
    SubArray* _Tail;		// Tail subarray
    size_t _Size;		// Number of occupied array cells
    size_t _Highest;		// Highest indexed valid array cell
    size_t _Total;		// Total allocated array cells
    size_t _ArrayBound;		// Array bound flag and bounding limit
    size_t _IteratorsActive;	// Number of iterators active

    Boolean _EraseIfNull; 		// Erase null trailing subarrays flag
    Boolean _DeleteItem;		// Saved erase flag
    Boolean _DestroyFromIterator;	// Destroy container from iterator

    // Subarray maintenance methods
    virtual void EraseNullChain();
    virtual void SetTail(SubArray* Ptr_);
    virtual void SetRunLength(size_t Size_);

    // Total occupied (non-null) cells : set, increment, decrement.
    virtual void SetSize(size_t Size_);
    virtual void IncrementSize(size_t Incr_);
    virtual void DecrementSize(size_t Decr_);

    // Total cells allocated : increment, decrement.
    virtual void IncrementTotal(size_t Incr_);
    virtual void DecrementTotal(size_t Decr_);

    // Array exceptions
    inline void Xlen() const
	{ THROW (FallibleBase::ArrayLengthErr()); }
    inline void Xran() const
	{ THROW (FallibleBase::ArrayIndexRangeErr()); }
    inline void Xinv() const
	{ THROW (FallibleBase::ArrayInvalidArgumentErr()); }

  public:
    // Void array constructor
    VoidArray(SubArray* SubArrayPtr_, size_t Highest_, size_t Total_);
    VoidArray(SubArray* SubArrayPtr_);
    VoidArray(const VoidArray& Obj_, SubArray* SubArrayPtr_);
    virtual ~VoidArray();

    // object property methods
    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;    

    // Assignment method
    VoidArray& operator = (const VoidArray& Obj_);

    // Methods to check if object(s) is dynamically allocated by searching
    // through the custom memory manager
    virtual Boolean IsNewed() const;

    // Array shrinking methods
    virtual void Shrink(size_t Size_);
    virtual void ShrinkTo(size_t Size_);

    // Size and total data accessors
    virtual size_t Size() const;
    virtual size_t RunLength() const;
    virtual size_t Total() const;

    // Array iterator method
    virtual void DestroyFromIterator();
    virtual Boolean RegisterIterator();
    virtual Boolean UnRegisterIterator();
    virtual size_t IteratorsActive();

    // Self deleting/shrinking option methods
    virtual ArrayClass& EraseIfNull();
    virtual ArrayClass& KeepIfNull();
    virtual Boolean ShouldEraseIfNull() const;

    // Array bound methods
    virtual ArrayClass& Bound(size_t Limit_=0);
    virtual ArrayClass& Unbound();
    virtual size_t Limit() const;

    // container full / empty test methods
    virtual Boolean IsFull() const;
    virtual Boolean IsEmpty() const;

    // Array element data deletion methods
    virtual void Flush();

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
// Proxy info class for array
class RefInfo
{
  friend class SubArray;
  friend class VoidArray;

  protected:
    ArrayClass* _SuperPtr;	// Pointer to owning main array.
    SubArray* _BossPtr;		// Pointer to subarray owned by main array.
    size_t _ArrayIndex;		// Index of array element
    Boolean _Modified;		// Array modified flag

    // Total occupied (non-void) cells : increment, decrement.
    void IncrementSize(size_t Incr_)
	{ ARXMARKER("Start/End: RefInfo::IncrementSize(size_t)", ARRAY_ERRSTREAM)
	  _SuperPtr->IncrementSize(Incr_); }
    void DecrementSize(size_t Decr_)
	{ ARXMARKER("Start/End: RefInfo::DecrementSize(size_t)", ARRAY_ERRSTREAM)
	  _SuperPtr->DecrementSize(Decr_); }

    // Proxy info class constructors
    RefInfo(ArrayClass* Super_, SubArray* Boss_, size_t Index_);

  public:
    RefInfo(const RefInfo& Info_);
    virtual ~RefInfo();
};

/****************************************************************************/
#endif





