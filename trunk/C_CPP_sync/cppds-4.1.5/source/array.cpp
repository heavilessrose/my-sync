/****************************************************************************/
// FILE : Array.cpp
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
#ifndef ARRAY_CPP
#define ARRAY_CPP
#ifndef ARRAY_H
  #include "array.h"
#endif

/****************************************************************************/
// PURPOSE:
//   Subarray class constructor with total cells and array index argument
//
// PRE:
//   size_t NumArgs	: The totla number of cells in the subarray
//   size_t ThisIndex	: The initial subarray index
//
// POST:
//   Calls PtrVector constructor to create the null pointer vector and
//   validity state bit array
//   If number of arguments exceeds or equals the maximum value of unsigned
//   short type then length exception is thrown otherwise the subarray index
//   is validated and all elements in pointer vector is set to invalid state.
//
SubArray::SubArray(size_t NumArgs_, size_t ThisIndex_):
PtrVector(NumArgs_),
_ArrayBase(NULL),
_Index(ThisIndex_),
_BackPtr(NULL),
_Extension(NULL)
{
  ARXMARKER("Start: SubArray::SubArray(size_t, size_t)", ARRAY_ERRSTREAM)

  // Throw exception if specified total array cells is greater than or equal
  // to maximum of counter type.
  if (NumArgs_ >= ArrayClass::MaxSize())
    Xlen();

  // Validate specified index
  ValidateIndex();

  // Initialize all array elements to invalid state.
  for (size_t Index_ = 0, Max_ = GetIndex(); Index_ < Max_; Index_++)
    SetInvalid(Index_);

  ARXMARKER("End: SubArray::SubArray(size_t, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Subarray copy constructor
//
// PRE:
//   const SubArray& Obj_	: The argument subarray to be copied
//
// POST:
//   Calls PtrVector constructor to copy argument pointer vector and
//   initialize all data members to same value as argument subarray object.
//   Validates subarray index
//
SubArray::SubArray(const SubArray& Obj_):
PtrVector(Obj_.GetLength()),
_ArrayBase(NULL),
_Index(Obj_.GetIndex()),
_BackPtr(NULL),
_Extension(NULL)
{
  ARXMARKER("Start: SubArray::SubArray(const SubArray&)", ARRAY_ERRSTREAM)

  if (Obj_._Extension)
    _Extension = Obj_._Extension->Clone();

  ValidateIndex();

  ARXMARKER("End: SubArray::SubArray(const SubArray&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
#if ARRAY_DELETE_FROM_TAIL
void SubArray::Clear()
{
  ARXMARKER("Start: SubArray::Clear()", ARRAY_ERRSTREAM)

  // Set back pointers to null
  // Recursively call destruction method
  if (_Extension)
  {
    _Extension->SetBackPtr(NULL);
    _Extension->Clear();
  }

  // Set TSubArray index to 0 update main array total
  SetIndex(0);
  _ArrayBase->DecrementTotal(GetLength());

  // if back link exists then set previous TSubArray as new array tail
  // otherwise if total array cells is 0 then set array tail to null
  if (_BackPtr)
  {
    _BackPtr->SetExtension(NULL);
    _BackPtr->SetTail();
  }
  else
    if (!_ArrayBase->Total())
      _ArrayBase->SetTail(NULL);

  _ArrayBase = NULL;
  delete _Extension;
  _Extension = NULL;

  ARXMARKER("End: SubArray::Clear()", ARRAY_ERRSTREAM)
}
#endif
/****************************************************************************/
// PURPOSE:
//   SubArray destructor
//
// POST:
//   Destroys SubArray elements and all other SubArray extensions starting
//   from this SubArray.
//
SubArray::~SubArray()
{
  ARXMARKER("Start: SubArray::~SubArray()", ARRAY_ERRSTREAM)

#if ARRAY_DELETE_FROM_TAIL
  if (_ArrayBase)
    Clear();
#else
  // Set back pointers to null
  if (_Extension)
    _Extension->SetBackPtr(NULL);

  // Delete extension TSubArray
  delete _Extension;
  _Extension = NULL;

  // Set TSubArray index to 0 update main array total
  SetIndex(0);
  _ArrayBase->DecrementTotal(GetLength());

  // if back link exists then set previous TSubArray as new array tail
  // otherwise if total array cells is 0 then set array tail to null
  if (_BackPtr)
  {
    _BackPtr->SetExtension(NULL);
    _BackPtr->SetTail();
  }
  else
    if (!_ArrayBase->Total())
      _ArrayBase->SetTail(NULL);
#endif

  ARXMARKER("End: SubArray::~SubArray()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Subarray cloning method
//
// POST:
//   Calls NewCopy method to generate new copy of itself.
//
SubArray* SubArray::Clone() const
{
  ARXMARKER("Start/End: SubArray::Clone()", ARRAY_ERRSTREAM)
  return NewCopy();
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Set tail subarray of main base array
//
// PRE:
//   SubArray* Ptr_	: The subarray to set as tail.
//
// POST:
//   If this subarray is current tail then the specified subarray pointer is
//   set as the new tail.
//
void SubArray::SetTail(SubArray* Ptr_)
{
  ARXMARKER("Start: SubArray::SetTail(SubArray*)", ARRAY_ERRSTREAM)

  if (!_Extension)
  {
    size_t Index_, NullEntry_;
    Index_ = GetIndex();

    if (Index_)
      --Index_;

    for (;(NullEntry_ = IsNull(Index_)) && Index_; Index_--);

    SetIndex(NullEntry_ ? 0:(Index_ + 1));
    _ArrayBase->SetTail(Ptr_);
  }

  ARXMARKER("End: SubArray::SetTail(SubArray*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Set the run-length of the array as highest indexed valid element
//
// POST:
//   The run-length of the array is set as highest indexed valid element
//
void SubArray::SetRunLength()
{
  ARXMARKER("Start: SubArray::SetRunLength()", ARRAY_ERRSTREAM)

  // if is tail subarray
  if (!_Extension)
  {
    size_t Total_ = _ArrayBase->Total();
    _ArrayBase->SetRunLength(Total_ - (GetLength() - _Index));
  }

  ARXMARKER("End: SubArray::SetRunLength()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Set pointer vector elements and validity state.
//
// PRE:
//   size_t Distance_	: The number of cells to set
//   short Direction_	: The direction of shrinking/growing
//
// POST:
//   In the case of growing subarray (direction > 0)
//   the array elements is set null invalid, representing invalid data
//   In the case of shrinking subarray (direction < 0)
//   the array elements is set null valid, representing null pointer value.
//
void SubArray::SetArray(size_t Distance_, short Direction_)
{
  ARXMARKER("Start: SubArray::SetArray(size_t, short)", ARRAY_ERRSTREAM)

  Boolean Valid_;
  size_t Index_, Max_;

  if (Direction_ > 0)
  {
    Index_ = GetIndex() - Distance_;
    Max_ = GetIndex();
    Valid_ = FALSE;
  }
  else if (Direction_ < 0)
  {
    Max_ = GetIndex() + Distance_;
    Index_ = GetIndex();
    Valid_ = TRUE;
  }
  else
    Index_ = Max_ = 0;

  // Set pointer vector to appropriate null value (valid/invalid) depending
  // on shrinking or growing of subarray.
  for (;Index_ < Max_; Index_++)
    SetEntry(Index_, NULL, Valid_);

  ARXMARKER("End: SubArray::SetArray(size_t, short)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Validate array index
//
// POST:
//   If invalid subarray index value (greater than total cells in subarray)
//   then set subarray index to total subarray cells.
//
void SubArray::ValidateIndex()
{
  ARXMARKER("Start: SubArray::ValidateIndex()", ARRAY_ERRSTREAM)

  if (_Index > GetLength())
    _Index = GetLength();

  ARXMARKER("End: SubArray::ValidateIndex()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE
//   Method to set subarray index
//
// PRE:
//   size_t Index_	: The new specified value of the subarray index
//
// POST:
//   The new value is set as the subarray index and the subarray is set
//   depending on whether shrinking or growing of the subarray has occurred.
//
void SubArray::SetIndex(size_t Index_)
{
  ARXMARKER("Start: SubArray::SetIndex(size_t)", ARRAY_ERRSTREAM)

  if (_Index != Index_)
  {
    size_t OldIndex_ = _Index;
    _Index = Index_;
    ValidateIndex();				// validate new index

    if (_Index < OldIndex_)
      SetArray(OldIndex_ - _Index, -1);		// subarray has shrunk
    else if (_Index > OldIndex_)
      SetArray(_Index - OldIndex_, 1);		// subarray has grown
    SetRunLength();					// Set size of array
  }

  ARXMARKER("End: SubArray::SetIndex(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Subarray elements deactivation method.
//
// PRE:
//   size_t Size_ 	: The total block size to delete in number of cells
//   size_t Deleted_	: Number of cells already deleted
//
// POST:
//   If number of cells remaining to delete is greater than the total
//   specified number of cells in subarray then entire subarray is
//   deactivated otherwise a portion of it is deactivated.
//   Returns the number of cells deleted from this subarray.
//
size_t SubArray::Deactivate(size_t Size_, size_t Deleted_)
{
  ARXMARKER("Start: SubArray::Deactivate(size_t, size_t)", ARRAY_ERRSTREAM)

  size_t RealSize_ = GetIndex();
  size_t Remaining_ = Size_ - Deleted_;
  size_t ToDelete_ = (Remaining_ >= RealSize_) ? RealSize_:Remaining_;

  SetIndex(GetIndex() - ToDelete_);
  Deleted_ += ToDelete_;

  ARXMARKER("End: SubArray::Deactivate(size_t, size_t)", ARRAY_ERRSTREAM)
  return Deleted_;
}

/****************************************************************************/
// PURPOSE:
//   Helper method for shrinking the array
//
// PRE:
//   size_t Size_	: The size of the subarray to shrink
//   Boolean DoDelete_	: Flag to indicate actual deletion of subarray
//			  extensions.
//
// POST:
//   Shrinks current subarray cells and any extension subarrays up until
//   specified number of cells is deleted. The DoDelete_ flag indicate
//   whether or not to actually delete deactivated extension subarrays.
//
size_t SubArray::HelpShrink(size_t Size_, Boolean DoDelete_)
{
  ARXMARKER("Start: SubArray::HelpShrink(size_t, Boolean)", ARRAY_ERRSTREAM)

  size_t Deleted_ = 0;
  Boolean ExtDone_ = _Extension == NULL;

  while (Deleted_ < Size_)
    if (!ExtDone_)
    {
      // Recursively call shrink method on each subarray extension
      Deleted_ = _Extension->Shrink(Size_);

      // If all cells of extension subarray are deactivated
      if (!_Extension->GetIndex())
      {
	if (DoDelete_)		// If actual deletion specified
	{
	  delete _Extension;
	  _Extension = NULL;
	}

	ExtDone_ = TRUE;	// Extension is processed
      }
    }
    else
    {
      // Deactivate current subarray cells given number of deleted cells
      // from subarray extensions and specified number to delete.
      Deleted_ = Deactivate(Size_, Deleted_);
      break;
    }

  ARXMARKER("End: SubArray::HelpShrink(size_t, Boolean)", ARRAY_ERRSTREAM)
  return Deleted_;	// return number of deleted subarray elements
}

/****************************************************************************/
// PURPOSE:
//   ShrinkTo method to shrink down to specified number of cells
//
// PRE:
//   size_t Size_	: number of cells to shrink down to.
//
// POST:
//   The current subarray including its extensions is shrunk down to the
//   number of cells specified
//
size_t SubArray::ShrinkTo(size_t Size_)
{
  ARXMARKER("Start: SubArray::ShrinkTo(size_t)", ARRAY_ERRSTREAM)

  // Find number of nodes to delete by subtracting new specified size from
  // the current size then call helpshrink method
  if (Size_ < _ArrayBase->RunLength())
  {
    Size_ = _ArrayBase->RunLength() - Size_;
    if (Size_ <= _ArrayBase->RunLength())
    {
      ARXMARKER("End(1): SubArray::ShrinkTo(size_t)", ARRAY_ERRSTREAM)
      return HelpShrink(Size_, _ArrayBase->ShouldEraseIfNull());
    }
  }

  ARXMARKER("End(2): SubArray::ShrinkTo(size_t)", ARRAY_ERRSTREAM)
  return 0;
}

/****************************************************************************/
// PURPOSE:
//   Method used by head subarray to remove all subarray extensions and
//   empty the head subarray of all elements and reset its index to 0.
//
void SubArray::Reset()
{
  ARXMARKER("Start: SubArray::Reset()", ARRAY_ERRSTREAM)

  if (_Extension)
  {
    // set back link of extension subarray to null
    _Extension->SetBackPtr(NULL);

    delete _Extension;
    _Extension = NULL;
  }

  // Empty subarray and reset array index to 0
  SetIndex(0);

  ARXMARKER("End: SubArray::Reset()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Method to make an extension subarray and index an element within it.
//   This is done if the specified index is out of range for the current
//   fixed size subarray.
//
// PRE:
//   size_t PrevSize_	: Previous size of subarrray
//   size_t Index_	: specified index of element to access
//
// POST:
//   A new extension subarray is created and stored as data within the
//   current subarray. An element within the extension array is accessed
//   with the specified index.
//
RefInfo SubArray::Grow(size_t PrevSize_, size_t Index_)
{
  ARXMARKER("Start: SubArray::Grow(size_t, size_t)", ARRAY_ERRSTREAM)

  // Find new subarray size and adjusted index
  size_t NewSize_ = PrevSize_ + GetLength();
  size_t NewIndex_ = NewIndex(Index_);

  // Create new extension subarray with new size and index
  _Extension = NewExtension(NewSize_, NewIndex_ + 1);

  // If extension cannot be created throw range exception
  if (_Extension == NULL)
    Xran();

  // Access array element in extension subarray
  ARXMARKER("End: SubArray::Grow(size_t, size_t)", ARRAY_ERRSTREAM)
  return Extension(NewIndex_);
}

/****************************************************************************/
// PURPOSE:
//   Method to access extension subarray with specified index.
//
// PRE:
//   size_t Index_	: The index to use for accessing extension subarray
//
// POST:
//   The index of the current subarray is set to the maximum size of the
//   subarray. The extension subarray is accessed using the specified
//   index argument. The size of the current array is passed as the previous
//   size in the extension subarray indexing method call.
//
RefInfo SubArray::Extension(size_t Index_)
{
  ARXMARKER("Start: SubArray::Extension(size_t)", ARRAY_ERRSTREAM)

  SetIndex(GetLength());			// Max out current array index
  SubArray* ArrayExt_ = (SubArray*)_Extension;

  ARXMARKER("End: SubArray::Extension(size_t)", ARRAY_ERRSTREAM)
  return ArrayExt_->Index(GetLength(), Index_); // Index to subarray element
}

/****************************************************************************/
// PURPOSE:
//   Method to index subarray element with given index.
//   This method is the important core part of how the array class works
//
// PRE:
//   size_t PrevSize_	: Size of previous subarray. Default to 0 if at head.
//   size_t Index_	: The index of the element to access.
//
// POST:
//   If the specified index is within the range of the current subarray then
//   a proxy class containing references to the current subarray and given
//   index is created and returned by value.
//   If the specified index is out of range from the current subarray then
//   an extension subarray is created which size is the sum of the size of
//   the previous subarray and the current subarray. The given index adjusted
//   to the new extension subarray is referenced by the proxy class object
//   and returned to the calling program by value. If the specified index
//   is greater than the internal index, the internal index is set as one
//   higher than the given index.
//
RefInfo SubArray::Index(size_t PrevSize_, size_t Index_)
{
  ARXMARKER("Start: SubArray::Index(size_t, size_t)", ARRAY_ERRSTREAM)

  // If index is greater than specified length of subarray then create
  // extension subarray if it doesn't exist already and access the
  // appropriate element within the extension subarray.
  if (Index_ >= GetLength())
    if (_Extension)
    {
      ARXMARKER("End(1): SubArray::Index(size_t, size_t)", ARRAY_ERRSTREAM)
      return Extension(NewIndex(Index_));
    }
    else
    {
      ARXMARKER("End(2): SubArray::Index(size_t, size_t)", ARRAY_ERRSTREAM)
      return Grow(PrevSize_, Index_);
    }

  // If the specified index is greater than the internal subarray index
  // update internal index to one greater than the given index argument
  if (Index_ >= GetIndex())
    SetIndex(Index_ + 1);

  // Return proxy class containing reference to current subarray and adjusted
  // element index to the calling program.
  ARXMARKER("End(3): SubArray::Index(size_t, size_t)", ARRAY_ERRSTREAM)
  return RefInfo(GetBase(), this, Index_);
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Method to set the array entry indexed at the given location to the
//   given element value and validity state.
//
// PRE:
//   size_t Index_	: The index of the array entry to be changed.
//   Boolean Valid_ 	: The validity state of the corresponding array
//			  element
//
// POST:
//   The array entry is set with the given array element value and
//   set to the given validity state.
//
PtrVector::DATA_OBJECT SubArray::SetDataObject(size_t Index_, PtrVector::DATA_OBJECT Ptr_, Boolean Valid_)
{
  ARXMARKER("Start/End: SubArray::SetDataObject(size_t, PtrVector::DATA_OBJECT, Boolean)", ARRAY_ERRSTREAM)
  return PtrVector::SetEntry(Index_, Ptr_, Valid_);
}

/****************************************************************************/
// PURPOSE:
//   Method to set the array entry indexed at the given location to the
//   given element value and validity state.
//
// PRE:
//   Ushort Index_	: The index of the array entry to be changed.
//   Boolean Valid_ 	: The validity state of the corresponding array
//			  element
//
// POST:
//   The array entry is set with the given array element value and
//   set to the given validity state.
//
void* SubArray::SetEntry(size_t Index_, void* Ptr_, Boolean Valid_)
{
  ARXMARKER("Start: SubArray::SetEntry(size_t, void*, Boolean)", ARRAY_ERRSTREAM)

  PtrVector::DATA_OBJECT Dobj_(Ptr_, FALSE);
  PtrVector::SetEntry(Index_, Dobj_, Valid_);

  ARXMARKER("End: SubArray::SetEntry(size_t, void*, Boolean)", ARRAY_ERRSTREAM)
  return Dobj_.Pointee();
}

/****************************************************************************/
// PURPOSE:
//   Method to get the array entry indexed at the given location.
//
// PRE:
//   size_t Index_	: The index of the array entry to be accessed
//
// POST:
//   The array entry is indexed at the specified location is accessed.
//
void* SubArray::GetEntry(size_t Index_) const
{
  ARXMARKER("Start/End: SubArray::GetEntry(size_t) const", ARRAY_ERRSTREAM)
  return PtrVector::GetEntry(Index_).Pointee();
}

/****************************************************************************/
PtrVector::DATA_OBJECT SubArray::GetDataObject(size_t Index_) const
{
  ARXMARKER("Start/End: SubArray::GetDataObject(size_t) const", ARRAY_ERRSTREAM)
  return PtrVector::GetEntry(Index_);
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Method to determine the beginning of a trailing null chain of empty
//   subarrays to be removed from the main chain of valid subarrays.
//
// PRE:
//   SubArray* Ext_	: The extension subarray of the current subarray.
//
// POST:
//   The starting point of the trailing null chain of empty subarrays is
//   found and returned.
//
SubArray* SubArray::NullChain(SubArray* Ext_)
{
  ARXMARKER("Start: SubArray::NullChain(SubArray*)", ARRAY_ERRSTREAM)

  SubArray* NullNode_ = NULL;

  if (!_Extension)
    if (!ActiveCells())		// if subarray is empty (no active cells)
    {
      if (_BackPtr)
      {
	// set back array extension link to null to invoke recursion of
	// NullChain method on previous subarrays.
	_BackPtr->SetExtension(NULL);
	NullNode_ = _BackPtr->NullChain(this);
	_Extension = Ext_;
	_BackPtr = NULL;
      }
      else
      {
	// This return is for the head node of the null list
	// This means the entire list is null
	_Extension = Ext_;
	SetTail();
	ARXMARKER("End(1): SubArray::NullChain(SubArray*)", ARRAY_ERRSTREAM)
	return this;
      }
    }
    else
    {
      // This return is for the remaining tail node of the list for
      // non-null lists.
      SetTail();
      ARXMARKER("End(2): SubArray::NullChain(SubArray*)", ARRAY_ERRSTREAM)
      return Ext_;
    }

  ARXMARKER("End(3): SubArray::NullChain(SubArray*)", ARRAY_ERRSTREAM)
  return NullNode_;
}

/****************************************************************************/
// PURPOSE:
//   Method to initialize the total cells counter of the main owner array
//   with the prespecified length of each of its subarrays.
//
// POST:
//   The "total" counter of the main owner array is initialized to the
//   total number of cells contained in its subarrays
//
void SubArray::InitCounters()
{
  ARXMARKER("Start: SubArray::InitCounters()", ARRAY_ERRSTREAM)

  _ArrayBase->IncrementTotal(GetLength());

  if (_Extension)
    _Extension->InitCounters();
  else
  {
    SetRunLength();
    _ArrayBase->SetTail(this);
  }

  ARXMARKER("End: SubArray::InitCounters()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Method to initialize the base array pointer and back pointer of each
//   subarray in the subarray chain.
//
// PRE:
//   ArrayClass* Base_	: The pointer to the base array which contains
//			  the subarrays.
//
// POST:
//   Each subarray in the chain has its base array pointer and back link
//   pointer initialized to the specified value. The back link pointers
//   will specify each succeeding array within the chain as the back link.
//
void SubArray::InitPointers(ArrayClass* Base_, SubArray* Back_)
{
  ARXMARKER("Start: SubArray::InitPointers(ArrayClass*, SubArray*)", ARRAY_ERRSTREAM)

  _ArrayBase = Base_;	// initialize base array pointer
  _BackPtr = Back_;	// initialize back link pointer

  // Do recursive call with each subarray extension in the chain.
  if (_Extension)
    _Extension->InitPointers(Base_, this);

  ARXMARKER("End: SubArray::InitPointers(ArrayClass*, SubArray*)", ARRAY_ERRSTREAM)
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
//   SubArray object.
//
void* SubArray::operator new (size_t Bytes_)
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
void SubArray::operator delete (void* Space_)
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
//   SubArray object.
//
#if	HAS_ARRAY_NEW
void* SubArray::operator new[] (size_t Bytes_)
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
void SubArray::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/******************************** Array Class *******************************/
/****************************************************************************/
// PURPOSE:
//   Generic void array class constructor with initial subarray argument.
//   The valid size and total allocated cells is also specified.
//
// PRE:
//   SubArray* SubArrayPtr_	: The pointer to the initial subarray
//   size_t High_		: The highest indexable valid array cell.
//   size_t Total_		: The total allocated array cells.
//
// POST:
//   A new void array is constructed with the preceeding arguments as
//   data members.
//
VoidArray::VoidArray(SubArray* SubArrayPtr_, size_t High_, size_t Total_):
ObjectImp(),
_Head(SubArrayPtr_),
_Tail(SubArrayPtr_),
_Size(0),
_Highest(High_),
_Total(Total_),
_ArrayBound(0),
_IteratorsActive(0),
_EraseIfNull(TRUE),
_DeleteItem(FALSE),
_DestroyFromIterator(FALSE)
{
  ARXMARKER("Start: VoidArray::VoidArray(SubArray*, size_t, size_t)", ARRAY_ERRSTREAM)

  _Head->InitPointers(this);

  ARXMARKER("End: VoidArray::VoidArray(SubArray*, size_t, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Generic void array class copy constructor
//
// PRE:
//   SubArray* SubArrayPtr_	: The pointer to the initial subarray of
//				  the main array being copied.
//
// POST:
//   A new void array is constructed with its subarray size and contents
//   the same as the array to be copied.
//
VoidArray::VoidArray(SubArray* SubArrayPtr_):
ObjectImp(),
_Head(SubArrayPtr_),
_Tail(SubArrayPtr_),
_Size(0),
_Highest(0),
_Total(0),
_ArrayBound(0),
_IteratorsActive(0),
_EraseIfNull(TRUE),
_DeleteItem(FALSE),
_DestroyFromIterator(FALSE)
{
  ARXMARKER("Start: VoidArray::VoidArray(SubArray*)", ARRAY_ERRSTREAM)

  _Head->InitPointers(this);
  _Head->InitCounters();

  ARXMARKER("End: VoidArray::VoidArray(SubArray*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
VoidArray::VoidArray(const VoidArray& Obj_, SubArray* SubArrayPtr_):
ObjectImp((Object&)Obj_),
_Head(SubArrayPtr_),
_Tail(SubArrayPtr_),
_Size(0),
_Highest(0),
_Total(0),
_ArrayBound(0),
_IteratorsActive(0),
_EraseIfNull(TRUE),
_DeleteItem(FALSE),
_DestroyFromIterator(FALSE)
{
  ARXMARKER("Start: VoidArray::VoidArray(const VoidArray&, SubArray*)", ARRAY_ERRSTREAM)

  _Head->InitPointers(this);
  _Head->InitCounters();

  ARXMARKER("End: VoidArray::VoidArray(const VoidArray&, SubArray*)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Void array destructor
//
// POST:
//   The entire array including all subarrays is destroyed starting with
//   the head subarray.
//
VoidArray::~VoidArray()
{
  ARXMARKER("Start: VoidArray::~VoidArray()", ARRAY_ERRSTREAM)

  if (_Tail != NULL)	// if the array is determined non-null
    delete _Head;
  _Head = NULL;

  ARXMARKER("End: VoidArray::~VoidArray()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
Boolean VoidArray::IsSummableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean VoidArray::IsMultipliableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean VoidArray::IsOrdinalType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean VoidArray::IsInvertableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean VoidArray::IsLogicalType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean VoidArray::IsBitwiseableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean VoidArray::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean VoidArray::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean VoidArray::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
VoidArray& VoidArray::operator = (const VoidArray& Obj_)
{
  ObjectImp::operator = ((ObjectImp&)Obj_);
  return *this;
}

/****************************************************************************/
Boolean VoidArray::IsNewed() const
{
  ARXMARKER("Start: VoidArray::IsNewed() const", ARRAY_ERRSTREAM)

  if (!NewedVerified())
  {
    SetNewedVerify();
    if (ObjectImp::IsNewed((Container*)this) || ObjectImp::IsNewed((Object*)this))
      SetNewedFlag();
  }

  ARXMARKER("End: VoidArray::IsNewed() const", ARRAY_ERRSTREAM)
  return NewedFlag();
}

/****************************************************************************/
// PURPOSE:
//   Method to return the total number of array cells allocated.
//
size_t VoidArray::Total() const
{
  ARXMARKER("Start/End: VoidArray::Total() const", ARRAY_ERRSTREAM)
  return _Total;
}

/****************************************************************************/
void VoidArray::DestroyFromIterator()
{
  ARXMARKER("Start: VoidArray::DestroyFromIterator()", ARRAY_ERRSTREAM)

# if OVERLOAD_NEW
    _DestroyFromIterator = IsNewed();
# else
    _DestroyFromIterator = TRUE;
# endif

  ARXMARKER("End: VoidArray::DestroyFromIterator()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
Boolean VoidArray::RegisterIterator()
{
  ARXMARKER("Start: VoidArray::RegisterIterator()", ARRAY_ERRSTREAM)

  if (_IteratorsActive < MaxSize())
  {
    if (!_IteratorsActive)
    {
      _DeleteItem = ShouldEraseIfNull();
      KeepIfNull();
    }

    ++_IteratorsActive;
    ARXMARKER("End(1): VoidArray::RegisterIterator()", ARRAY_ERRSTREAM)
    return TRUE;
  }

  ARXMARKER("End(2): VoidArray::RegisterIterator()", ARRAY_ERRSTREAM)
  return FALSE;
}

/****************************************************************************/
Boolean VoidArray::UnRegisterIterator()
{
  ARXMARKER("Start: VoidArray::UnRegisterIterator()", ARRAY_ERRSTREAM)

  if (_IteratorsActive)
  {
    --_IteratorsActive;
    if (!_IteratorsActive)
    {
      if (_DeleteItem)
	EraseIfNull();
      else
	KeepIfNull();

      if (_DestroyFromIterator)
      {
	ARXMARKER("End(1): VoidArray::UnRegisterIterator()", ARRAY_ERRSTREAM)
	return TRUE;
      }
    }
  }

  ARXMARKER("End(2): VoidArray::UnRegisterIterator()", ARRAY_ERRSTREAM)
  return FALSE;
}

/****************************************************************************/
size_t VoidArray::IteratorsActive()
{
  ARXMARKER("Start/End: VoidArray::IteratorsActive()", ARRAY_ERRSTREAM)
  return _IteratorsActive;
}

/****************************************************************************/
void VoidArray::SetSize(size_t Size_)
{
  ARXMARKER("Start/End: VoidArray::SetSize(size_t)", ARRAY_ERRSTREAM)
  _Size = Size_;
}

/****************************************************************************/
// PURPOSE:
//   Method to increment the "size" counter with the given increment value.
//
void VoidArray::IncrementSize(size_t Incr_)
{
  ARXMARKER("Start/End: VoidArray::IncrementSize(size_t)", ARRAY_ERRSTREAM)
  _Size += Incr_;
}

/****************************************************************************/
// PURPOSE:
//   Method to decrement the "size" counter with the given increment value.
//
void VoidArray::DecrementSize(size_t Decr_)
{
  ARXMARKER("Start/End: VoidArray::DecrementSize(size_t)", ARRAY_ERRSTREAM)
  _Size -= Decr_;
}

/****************************************************************************/
// PURPOSE:
//   Method to increment the "total" counter with the given increment value.
//
void VoidArray::IncrementTotal(size_t Incr_)
{
  ARXMARKER("Start/End: VoidArray::IncrementTotal(size_t)", ARRAY_ERRSTREAM)
  _Total += Incr_;
}

/****************************************************************************/
// PURPOSE:
//   Method to decrement the "total" counter with the given decrement value.
//
void VoidArray::DecrementTotal(size_t Decr_)
{
  ARXMARKER("Start/End: VoidArray::DecrementTotal(size_t)", ARRAY_ERRSTREAM)
  _Total -= Decr_;
}

/****************************************************************************/
// PURPOSE:
//   Method to set the tail subarray data member to the given value.
//
void VoidArray::SetTail(SubArray* Ptr_)
{
  ARXMARKER("Start/End: VoidArray::SetTail(SubArray*)", ARRAY_ERRSTREAM)
  _Tail = Ptr_;
}

/****************************************************************************/
// PURPOSE:
//   Method to set the highest valid element index to the specified value.
//   If the resulting index value is greater than the total number of cells
//   allocated it will be assigned that value instead.
//
void VoidArray::SetRunLength(size_t Size_)
{
  ARXMARKER("Start: VoidArray::SetRunLength(size_t)", ARRAY_ERRSTREAM)

  _Highest = Size_;
  if (_Highest > _Total)
    _Highest = _Total;

  ARXMARKER("End: VoidArray::SetRunLength(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Method to shrink the array a given number of cells.
//
// PRE:
//   size_t Size_	: The given number of cells to remove from the array.
//
// POST:
//   The array is shrunk the given number of cells and if the option is
//   active, the trailing null chain of subarrays if any are removed.
//
void VoidArray::Shrink(size_t Size_)
{
  ARXMARKER("Start: VoidArray::Shrink(size_t)", ARRAY_ERRSTREAM)

  _Head->Shrink(Size_);
  EraseNullChain();	// remove any resulting null trailing subarrays

  ARXMARKER("End: VoidArray::Shrink(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Method to shrink the array down to the given number of cells.
//
// PRE:
//   size_t Size_	: The number of cells to shrink down to.
//
// POST:
//   The array is shrunk down to the specified number of cells and if the
//   option is active, the trailing null chain of subarrays if any are
//   removed.
//
void VoidArray::ShrinkTo(size_t Size_)
{
  ARXMARKER("Start: VoidArray::ShrinkTo(size_t)", ARRAY_ERRSTREAM)

  _Head->ShrinkTo(Size_);
  EraseNullChain();	// remove any resulting null trailing subarrays

  ARXMARKER("End: VoidArray::ShrinkTo(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Method to return the number occupied array element index
//
size_t VoidArray::Size() const
{
  return _Size;
}

/****************************************************************************/
// PURPOSE:
//   Method to return the highest valid array element index
//
size_t VoidArray::RunLength() const
{
  return _Highest;
}

/****************************************************************************/
// PURPOSE:
//   Method to set active the option to erase trailing null subarrays.
//
// POST:
//   The flag indicating the option is set TRUE.
//   A call to the method for finding null trailing subarrays is called
//   immediately afterwards.
//
ArrayClass& VoidArray::EraseIfNull()
{
  ARXMARKER("Start: VoidArray::EraseIfNull()", ARRAY_ERRSTREAM)

  _EraseIfNull = TRUE;
  EraseNullChain();
  FindSize();

  ARXMARKER("End: VoidArray::EraseIfNull()", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
// PURPOSE:
//   Method to set inactive the option to erase trailing null subarrays.
//
// POST:
//   The flag indicating the option is set FALSE.
//
ArrayClass& VoidArray::KeepIfNull()
{
  ARXMARKER("Start: VoidArray::KeepIfNull()", ARRAY_ERRSTREAM)

  _EraseIfNull = FALSE;

  ARXMARKER("End: VoidArray::KeepIfNull()", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
// PURPOSE:
//   Method to return the value of the "erase null chain" option
//
Boolean VoidArray::ShouldEraseIfNull() const
{
  return _EraseIfNull;
}

/****************************************************************************/
// PURPOSE:
//   Method to erase all trailing null subarrays from the chain.
//
// POST:
//   If the "erase null chain" option is active and the tail subarray exists,
//   a method is called for determining the starting point of the trailing
//   null chain of subarrays. If the null chain starts with the head
//   subarray all extensions from the head subarray is removed and the head
//   subarray is emptied of all contents and reset to 0 index. If the null
//   chain does not start with the head subarray then all subarrays including
//   the initial starting subarray of the null chain is removed.
//
void VoidArray::EraseNullChain()
{
  ARXMARKER("Start: VoidArray::EraseNullChain()", ARRAY_ERRSTREAM)

  if (_Tail && _EraseIfNull)
  {
    SubArray* NullNode_ = _Tail->NullChain();

    if (_Head == NullNode_)
      _Head->Reset();		// remove extensions and reset head subarray
    else
      delete NullNode_;		// remove all subarrays including initial
  }

  ARXMARKER("End: VoidArray::EraseNullChain()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Method to set the array bound flag to non-zero (TRUE) and also set the
//   bounding limit.
//
// PRE:
//   size_t Limit_	: The bounding limit of the array.
//			  If zero then the bounding limit is set as the
//			  total number of cells.
//			  otherwise it is set as the given value.
//
// POST:
//   The array bound flag is set to the given limit or the total number of
//   cells if no or zero limit is specified. The bounding limit also acts
//   as a boolean flag indicating whether array is bound
//   (0 = unbounded, non-zero = bounded)
//
ArrayClass& VoidArray::Bound(size_t Limit_)
{
  ARXMARKER("Start: VoidArray::Bound(size_t)", ARRAY_ERRSTREAM)

  _ArrayBound = (Limit_ == 0) ? _Total:Limit_;

  ARXMARKER("End: VoidArray::Bound(size_t)", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
// PURPOSE:
//   Method to set the array bound flag false (set it to 0).
//
ArrayClass& VoidArray::Unbound()
{
  ARXMARKER("Start: VoidArray::Unbound()", ARRAY_ERRSTREAM)

  _ArrayBound = 0;

  ARXMARKER("End: VoidArray::Unbound()", ARRAY_ERRSTREAM)
  return *this;
}

/****************************************************************************/
// PURPOSE:
//   Method to return the array bound flag to the calling program.
//
size_t VoidArray::Limit() const
{
  return _ArrayBound;
}

/****************************************************************************/
Boolean VoidArray::IsFull() const
{
  return (Limit() && Size() >= Limit());
}

/****************************************************************************/
Boolean VoidArray::IsEmpty() const
{
  return (Size() == 0);
}

/****************************************************************************/
// PURPOSE:
//   Method to perform a delete operation on all elements in the array and
//   shrink the array to zero size.
//
// POST:
//   The delete array item flag is set TRUE to indicate deletion of array
//   elements upon their removal from the array. The array is then shrunk
//   to zero size with each element deleted after its removal from the array.
//
void VoidArray::Flush()
{
  ARXMARKER("Start: VoidArray::Flush()", ARRAY_ERRSTREAM)
  ShrinkTo(0);
  ARXMARKER("End: VoidArray::Flush()", ARRAY_ERRSTREAM)
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
//   VoidArray object.
//
void* VoidArray::operator new (size_t Bytes_)
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
void VoidArray::operator delete (void* Space_)
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
//   VoidArray object.
//
#if	HAS_ARRAY_NEW
void* VoidArray::operator new[] (size_t Bytes_)
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
void VoidArray::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Proxy array info class constructor
//
// PRE:
//   ArrayClass* Super_	: The main base array
//   SubArray* Boss_	: The subarray within the base array to be referenced
//   size_t Index_	: The array index within the subarray to be used for
//			  accessing the element.
//
// POST:
//   A proxy class containing references to the main base array, subarray and
//   element index within the subarray is created.
//
RefInfo::RefInfo(ArrayClass* Super_, SubArray* Boss_, size_t Index_):
_SuperPtr(Super_),
_BossPtr(Boss_),
_ArrayIndex(Index_),
_Modified(FALSE)
{
  ARXMARKER("Start/End: RefInfo::RefInfo(ArrayClass*, SubArray*, size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Proxy class copy constructor.
//
// PRE:
//   RefInfo& Info_	: The given proxy class object to be copied.
//
// POST:
//   A new proxy class object is created with the same references included
//   within the specified proxy object argument.
//
RefInfo::RefInfo(const RefInfo& Info_):
_SuperPtr((ArrayClass*)Info_._SuperPtr),
_BossPtr((SubArray*)Info_._BossPtr),
_ArrayIndex(Info_._ArrayIndex),
_Modified(FALSE)
{
  ARXMARKER("Start/End: RefInfo::RefInfo(const RefInfo&)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Proxy class destructor.
//
// POST:
//   If the modified flag is set, indicating a change in the array
//   made through methods included within the proxy class or its derived
//   classes then the EraseNullChain method is invoked through the array
//   pointer data member to remove trailing null subarrays if any or if
//   necessary.
//
RefInfo::~RefInfo()
{
  ARXMARKER("Start: RefInfo::~RefInfo()", ARRAY_ERRSTREAM)

  if (_Modified && _SuperPtr != NULL)
    _SuperPtr->EraseNullChain();

  ARXMARKER("End: RefInfo::~RefInfo()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
#endif





