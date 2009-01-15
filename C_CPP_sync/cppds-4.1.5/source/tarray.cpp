/****************************************************************************/
// FILE : Tarray.cpp
// PURPOSE:
// TypedArray and TypedSubArray classes for typed data items to be stored in
// the array class.
/****************************************************************************/
#ifndef TYPEDARRAY_CPP
#define TYPEDARRAY_CPP
#ifndef TYPEDARRAY_H
  #include "tarray.h"
#endif

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Typed subarray class constructor with total and index arguments
//
// PRE:
//   size_t NumArgs_	: The total number of cells to initialize the array
//   size_t ThisIndex_	: The index value to initialize the array with.
//
// POST:
//   Calls the subarray constructor
//
template <class T>
TypedSubArray<T>::TypedSubArray(size_t NumArgs_, size_t ThisIndex_):
SubArray(NumArgs_, ThisIndex_)
{
  ARXMARKER("Start/End: TypedSubArray<T>::TypedSubArray(size_t, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Typed subarray copy constructor
//
// PRE:
//   TypedSubArray<T>& Obj_	: The typed subarray to copy
//
// POST:
//   Calls the subarray copy constructor
//
template <class T>
TypedSubArray<T>::TypedSubArray(const TypedSubArray<T>& Obj_):
SubArray((SubArray&)Obj_)
{
  ARXMARKER("Start/End: TypedSubArray<T>::TypedSubArray(const TypedSubArray<T>&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
void TypedSubArray<T>::InitSubArray(const TypedSubArray<T>& Obj_, size_t Max_)
{
  ARXMARKER("Start: TypedSubArray<T>::InitSubArray(const TypedSubArray<T>&, size_t)", ARRAY_ERRSTREAM)

  for (size_t Index_ = 0; Index_ < Max_; Index_++)
    Replace(Index_, Obj_.RetrieveDataObject(Index_), Obj_.IsDataDefined(Index_));

  ARXMARKER("End: TypedSubArray<T>::InitSubArray(const TypedSubArray<T>&, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Typed subarray assignment method for assigning pointer arguments to
//   the subarray
//
// PRE:
//   size_t Index_	: The index in the subarray to assign the argument
//   T* Ptr_		: The argument pointer to assign to the array
//   Boolean Valid_	: The validity of the data itme assigned
//			  this is set as the array element's validity state.
//
// POST:
//   Sets the indexed array cell to the pointer argument and sets its
//   corresponding validity state to the specified argument.
//
template <class T>
PtrVector::DATA_OBJECT TypedSubArray<T>::Assign(size_t Index_, PtrVector::DATA_OBJECT Ptr_, Boolean Valid_)
{
  ARXMARKER("Start/End: TypedSubArray<T>::Assign(size_t, PtrVector::DATA_OBJECT, Boolean)", ARRAY_ERRSTREAM)
  return SetDataObject(Index_, Ptr_, Valid_);
}

/****************************************************************************/
// PURPOSE:
//   Tyepd subarray element retrieval method
//
// PRE:
//   size_t Index_	: The index of the subarray element to be retrieved
//
// POST:
//   The subarray entry at the specified index is retrieved
//
template <class T>
T* TypedSubArray<T>::Retrieve(size_t Index_) const
{
  ARXMARKER("Start/End: TypedSubArray<T>::Retrieve(size_t) const", ARRAY_ERRSTREAM)
  return ((T*)SubArray::GetEntry(Index_));
}

/****************************************************************************/
template <class T>
PtrVector::DATA_OBJECT TypedSubArray<T>::RetrieveDataObject(size_t Index_) const
{
  ARXMARKER("Start/End: TypedSubArray<T>::RetrieveDataObject(size_t) const", ARRAY_ERRSTREAM)
  return GetDataObject(Index_);
}

/****************************************************************************/
// PURPOSE:
//   Method to replace the specified subarray element with another data
//   item passed as the pointer argument to the method. The existing data
//   item at the index location in the subarray is deleted and assigned the
//   passed data item. Any required memory allocation is performed by the
//   derived class Allocate method which has been declared virtual to allow
//   for different memory allocation strategies.
//
// PRE:
//   size_t Index_	: the specified index in which to place the data item
//   const T* Ptr_	: The pointer to the data item
//   Boolean Valid_	: The validity state of the data item
//
// POST:
//   Any existing data item at the specified index location in the subarray
//   is cleared and replaced with the passed argument data item. Memory
//   allocation for the new data item is handled polymorphically by the
//   derived subarray class
//
template <class T>
PtrVector::DATA_OBJECT TypedSubArray<T>::Replace(size_t Index_, PtrVector::DATA_OBJECT Ptr_, Boolean Valid_)
{
  ARXMARKER("Start: TypedSubArray<T>::Replace(size_t, PtrVector::DATA_OBJECT, Boolean) const", ARRAY_ERRSTREAM)
  PtrVector::DATA_OBJECT Dobj_;

  ARXMARKER("End: TypedSubArray<T>::Replace(size_t, PtrVector::DATA_OBJECT, Boolean) const", ARRAY_ERRSTREAM)
  return Assign(Index_, MakeDataObject(Dobj_, Ptr_), Valid_);
}

/****************************************************************************/
// PURPOSE:
//   Method to set the specified subarray entry with the given void pointer
//   argument. This method basically calls the replace method with a void
//   pointer argument cast to the appropriate type.
//
// PRE:
//   size_t Index_	: The subarray index to access.
//   void* Ptr_		: The void pointer argument to replace the existing
//			  data member with.
//   Boolean Valid_	: The validity state of the given pointer argument.
//
// POST:
//   Calls Replace method with pointer argument cast to appropriate type.
//
template <class T>
void* TypedSubArray<T>::SetEntry(size_t Index_, void* Ptr_, Boolean Valid_)
{
  ARXMARKER("Start: TypedSubArray<T>::SetEntry(size_t, void*, Boolean)", ARRAY_ERRSTREAM)
  return Replace(Index_, (T*)Ptr_, Valid_);
}

/****************************************************************************/
#if OVERLOAD_NEW
// PURPOSE:
//   Overloaded new operator for allocating memory using custom memory
//   manager class.
//
// PRE:
//   size_t Bytes_	: The size of the memory block to be allocated.
//
// POST:
//   A new memory block is allocated and used by the newly allocated
//   TypedSubArray object.
//
template <class T>
void* TypedSubArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
// PURPOSE:
//   Overloaded delete operator for releasing memory allocated with the
//   custom memory manager class.
//
// PRE:
//   void* Space_	: The memory block to be released to the memory mgr.
//
// POST:
//   The specified memory block is released to the custom memory manager.
//
template <class T>
void TypedSubArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
// PURPOSE:
//   Overloaded new operator for allocating memory using custom memory
//   manager class.
//
// PRE:
//   size_t Bytes_	: The size of the memory block to be allocated.
//
// POST:
//   A new memory block is allocated and used by the newly allocated
//   TypedSubArray object.
//
#if	HAS_ARRAY_NEW
template <class T>
void* TypedSubArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
// PURPOSE:
//   Overloaded delete operator for releasing memory allocated with the
//   custom memory manager class.
//
// PRE:
//   void* Space_	: The memory block to be released to the memory mgr.
//
// POST:
//   The specified memory block is released to the custom memory manager.
//
#if	HAS_ARRAY_NEW
template <class T>
void TypedSubArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
void TypedArray<T>::FindSize()
{
  ARXMARKER("Start: TypedArray<T>::FindSize()", ARRAY_ERRSTREAM)

  size_t Index_, RunLength_, Size_;

  for (Index_ = Size_ = 0, RunLength_ = RunLength(); Index_ < RunLength_; Index_++)
    if (TypedIndex(Index_).IsValid())
      ++Size_;

  SetSize(Size_);
  ARXMARKER("End: TypedArray<T>::FindSize()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Constructor for the TypedArray class
//   includes argument for the initial subarray, the highest valid element,
//   and the total number of accessable array cells.
//
// PRE:
//   SubArray* SubArrayPtr_	: The initial subarray to use
//   size_t Highest_		: The highest valid element index
//   size_t Total_		: The total number of accessable array cells
//
// POST:
//   Calls the VoidArray constructor to create the internal array object.
//   Sets the _Factory pointer to function to NULL. The _Factory pointer
//   to function is used for creating an object of T or derived from T given
//   a class ID value. This is used for polymorphic creation of type T
//   objects given a type code (possibly read from a file). This makes
//   file storage and persistence of polymorphic objects possible.
//
template <class T>
TypedArray<T>::TypedArray(TypedSubArray<T>* SubArrayPtr_, size_t Highest_, size_t Total_):
VoidArray(SubArrayPtr_, Highest_, Total_),
_Factory(NULL)
{
  ARXMARKER("Start/End: TypedArray<T>::TypedArray(TypedSubArray<T>*, size_t, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   TypedArray copy constructor.
//
// PRE:
//   SubArray* SubArrayPtr_	: The initial subarray object of the array
//				  to be copied.
//
// POST:
//   A copy of the desired array and all its contents is made starting
//   with its initial subarray.
//   Sets the _Factory pointer to function to NULL. The _Factory pointer
//   to function is used for creating an object of T or derived from T given
//   a class ID value. This is used for polymorphic creation of type T
//   objects given a type code (possibly read from a file). This makes
//   file storage and persistence of polymorphic objects possible.
//
template <class T>
TypedArray<T>::TypedArray(TypedSubArray<T>* SubArrayPtr_):
VoidArray((SubArray*)SubArrayPtr_),
_Factory(NULL)
{
  ARXMARKER("Start/End: TypedArray<T>::TypedArray(TypedSubArray<T>*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
template <class T>
TypedArray<T>::TypedArray(const TypedArray<T>& Obj_, TypedSubArray<T>* SubArrayPtr_):
VoidArray((const VoidArray&)Obj_, (SubArray*)SubArrayPtr_),
_Factory(NULL)
{
  ARXMARKER("Start/End: TypedArray<T>::TypedArray(const TypedArray<T>&, TypedSubArray<T>*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Method to dereference and invoke the _Factory pointer to function with
//   the specified type code to create a new object of T or derived from T.
//   if the pointer to function is null then a Null pointer exception is
//   thrown.
//
// PRE:
//   int Code_	: The type code to invoke the factory function with.
//
// POST:
//   If the factory function is not null then it should return a newly
//   allocated object of type T or derived from T with the same type code
//   as the one specified in the argument to the method.
//
template <class T>
T* TypedArray<T>::MakeByTypeCode(int Code_)
{
  ARXMARKER("Start: TypedArray<T>::MakeByTypeCode(int)", ARRAY_ERRSTREAM)

  // If pointer to function is null then throw an exception.
  if (_Factory == NULL)
    Xnullp();

  // Create object based on type code.
  ARXMARKER("End: TypedArray<T>::MakeByTypeCode(int)", ARRAY_ERRSTREAM)
  return (*_Factory)(Code_);
}

/****************************************************************************/
// PURPOSE:
//   Method to access the array through a RefInfo proxy object which
//   is almost the same as the RefInfo proxy object but with type specific
//   data and methods.
//
// PRE:
//   size_t Index_	: The index of the array element to be accessed.
//			  via a proxy object of the type TypedArrayRef.
//
// POST:
//   A proxy object of the RefInfo class is created and is set with
//   the indexed element found from the search of the array starting from
//   the head subarray.
//
template <class T>
RefInfo TypedArray<T>::IndexRef(size_t Index_) const
{
  ARXMARKER("Start/End: TypedArray<T>::IndexRef(size_t)", ARRAY_ERRSTREAM)

  // Search array starting from the head subarray calling its array
  // indexing method at 0 index. Set an object of the TypedArrayRef proxy
  // class to what was retrieved from the array searching operation.
  return _Head->Index(0, Index_);
}

/****************************************************************************/
// PURPOSE:
//   Method to access the array through a TypedArrayRef proxy object which
//   is almost the same as the RefInfo proxy object but with type specific
//   data and methods.
//
// PRE:
//   size_t Index_	: The index of the array element to be accessed.
//			  via a proxy object of the type TypedArrayRef.
//
// POST:
//   A proxy object of the TypedArrayRef class is created and is set with
//   the indexed element found from the search of the array starting from
//   the head subarray.
//
template <class T>
TypedArrayRef<T> TypedArray<T>::TypedIndex(size_t Index_) const
{
  ARXMARKER("Start/End: TypedArray<T>::TypedIndex(size_t)", ARRAY_ERRSTREAM)

  // Search array starting from the head subarray calling its array
  // indexing method at 0 index. Set an object of the TypedArrayRef proxy
  // class to what was retrieved from the array searching operation.
  return TypedArrayRef<T>(_Head->Index(0, Index_));
}

/****************************************************************************/
// PURPOSE:
//   Method to give the head subarray of the enclosing base owner array
//
template <class T>
TypedSubArray<T>& TypedArray<T>::GiveSubArray() const
{
  ARXMARKER("Start/End: TypedArray<T>::GiveSubArray() const", ARRAY_ERRSTREAM)
  return *((TypedSubArray<T>*)_Head);
}

/****************************************************************************/
#if OVERLOAD_NEW
// PURPOSE:
//   Overloaded new operator for allocating memory using custom memory
//   manager class.
//
// PRE:
//   size_t Bytes_	: The size of the memory block to be allocated.
//
// POST:
//   A new memory block is allocated and used by the newly allocated
//   TypedArray object.
//
template <class T>
void* TypedArray<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
// PURPOSE:
//   Overloaded delete operator for releasing memory allocated with the
//   custom memory manager class.
//
// PRE:
//   void* Space_	: The memory block to be released to the memory mgr.
//
// POST:
//   The specified memory block is released to the custom memory manager.
//
template <class T>
void TypedArray<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
// PURPOSE:
//   Overloaded new operator for allocating memory using custom memory
//   manager class.
//
// PRE:
//   size_t Bytes_	: The size of the memory block to be allocated.
//
// POST:
//   A new memory block is allocated and used by the newly allocated
//   TypedArray object.
//
#if	HAS_ARRAY_NEW
template <class T>
void* TypedArray<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
// PURPOSE:
//   Overloaded delete operator for releasing memory allocated with the
//   custom memory manager class.
//
// PRE:
//   void* Space_	: The memory block to be released to the memory mgr.
//
// POST:
//   The specified memory block is released to the custom memory manager.
//
#if	HAS_ARRAY_NEW
template <class T>
void TypedArray<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   TypedArrayRef class constructor given an RefInfo object argument.
//
// PRE:
//   RefInfo& Info_	: The proxy reference object to be used as data
//			  for this object.
//
// POST:
//   A new TypedArrayRef class objec is created with the same data as
//   specified in the RefInfo proxy object argument.
//
template <class T>
TypedArrayRef<T>::TypedArrayRef(const RefInfo& Info_):
RefInfo(Info_)
{
  ARXMARKER("Start/End: TypedArrayRef<T>::TypedArrayRef(const RefInfo&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Retrieval method for typed arrays which retrieves the address of the
//   typed object at the given index location.
//
// POST:
//   Retrieves the address of the typed object at the given index position
//   in the array.
//
template <class T>
T* TypedArrayRef<T>::Retrieve()
{
  ARXMARKER("Start/End: TypedArrayRef<T>::Retrieve()", ARRAY_ERRSTREAM)
  return CastTo_TypedSubArray()->Retrieve(_ArrayIndex);
}

/****************************************************************************/
// PURPOSE:
//   Method for copying the specified element of one array to another
//   The modified flag is set true so when the proxy object goes out of
//   existence the modified flag will indicate a change in the array which
//   will cause the proxy class destructor to call the EraseIfNull method
//   to erase any trailing null subarrays if any.
//
// PRE:
//   const TypedArrayRef<T>& Obj_	: the proxy class containing the
//				          array element to copy from the
//					  source array to the specified
//				          location in the target array
//
// POST:
//   Calls the replace function with the specified index value and data item
//   retrieve from the source proxy object. The modified flag is set TRUE to
//   indicate a change in the array which causes the EraseNullChain method to
//   be executed at proxy object destruction.
//
template <class T>
TypedArrayRef<T>& TypedArrayRef<T>::Copy(TypedArray<T>& That_, size_t Index_)
{
  ARXMARKER("Start: TypedArrayRef<T>::Copy(TypedArray<T>&, size_t)", ARRAY_ERRSTREAM)

  TypedArrayRef<T> Obj_(That_.IndexRef(Index_));
  _Modified = Obj_.Retrieve() == NULL;

  if (_Modified)
  {
    if (IsValid())
      DecrementSize(1);
  }
  else
    if (!IsValid())
      IncrementSize(1);

  CastTo_TypedSubArray()->Replace(_ArrayIndex, Obj_.RetrieveDataObject(), Obj_.IsDataDefined());

  ARXMARKER("End: TypedArrayRef<T>::Copy(TypedArray<T>&, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
// PURPOSE:
//   Method for copying the specified element of one array to another
//   The modified flag is set true so when the proxy object goes out of
//   existence the modified flag will indicate a change in the array which
//   will cause the proxy class destructor to call the EraseIfNull method
//   to erase any trailing null subarrays if any.
//
// PRE:
//   const TypedArrayRef<T>& Obj_	: the proxy class containing the
//				          array element to copy from the
//					  source array to the specified
//				          location in the target array
//
// POST:
//   Calls the replace function with the specified index value and data item
//   retrieve from the source proxy object. The modified flag is set TRUE to
//   indicate a change in the array which causes the EraseNullChain method to
//   be executed at proxy object destruction.
//
template <class T>
TypedArrayRef<T>& TypedArrayRef<T>::ConstCopy(const TypedArray<T>& That_, size_t Index_)
{
  ARXMARKER("Start: TypedArrayRef<T>::ConstCopy(TypedArray<T>&, size_t)", ARRAY_ERRSTREAM)

  TypedArrayRef<T> Obj_(That_.IndexRef(Index_));
  _Modified = Obj_.Retrieve() == NULL;

  if (_Modified)
  {
    if (IsValid())
      DecrementSize(1);
  }
  else
    if (!IsValid())
      IncrementSize(1);

  CastTo_TypedSubArray()->Replace(_ArrayIndex, Obj_.RetrieveDataObject(), Obj_.IsDataDefined());

  ARXMARKER("End: TypedArrayRef<T>::ConstCopy(TypedArray<T>&, size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
// PURPOSE:
//   Method for copying the specified element of one array to another
//   The modified flag is set true so when the proxy object goes out of
//   existence the modified flag will indicate a change in the array which
//   will cause the proxy class destructor to call the EraseIfNull method
//   to erase any trailing null subarrays if any.
//
// PRE:
//   const TypedArrayRef<T>& Obj_	: the proxy class containing the
//				          array element to copy from the
//					  source array to the specified
//				          location in the target array
//
// POST:
//   Calls the replace function with the specified index value and data item
//   retrieve from the source proxy object. The modified flag is set TRUE to
//   indicate a change in the array which causes the EraseNullChain method to
//   be executed at proxy object destruction.
//
template <class T>
TypedArrayRef<T>& TypedArrayRef<T>::AssignObject(PtrVector::DATA_OBJECT* That_, Boolean Valid_)
{
  ARXMARKER("Start: TypedArrayRef<T>::AssignObject(PtrVector::DATA_OBJECT*, Boolean)", ARRAY_ERRSTREAM)

  if (!That_)
  {
    ARXMARKER("End(1): TypedArrayRef<T>::AssignObject(PtrVector::DATA_OBJECT*, Boolean)", ARRAY_ERRSTREAM)
    return *this;
  }

  _Modified = That_->Pointee() == NULL;

  if (_Modified)
  {
    if (IsValid())
      DecrementSize(1);
  }
  else
    if (!IsValid())
      IncrementSize(1);

  CastTo_TypedSubArray()->Assign(_ArrayIndex, *That_, Valid_);

  ARXMARKER("End(2): TypedArrayRef<T>::AssignObject(PtrVector::DATA_OBJECT*, Boolean)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
#endif





