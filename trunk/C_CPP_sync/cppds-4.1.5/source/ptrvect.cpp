/****************************************************************************/
// FILE : PtrVect.cpp
// PURPOSE:
// Pointer vector and bitstring class for storage of data items and their
// validity state for the array class.
/****************************************************************************/
#ifndef PTRVECTOR_CPP
#define PTRVECTOR_CPP
#ifndef PTRVECTOR_H
  #include "ptrvect.h"
#endif

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Pointer vector constructor
//
// PRE:
//   size_t Size_	: Size of pointer vector in number of cells
//
// POST:
//   Creates new pointer vector object with the specified number of cells
//
PtrVector::PtrVector(size_t Size_):
_Length(Size_),
_Active(0),
_Status(new_char_array(ByteArraySize(Size_), NULL)),
_ArgsArray(new CountedVoidPtr<void>[Size_])
{
  ARXMARKER("Start: PtrVector::PtrVector(size_t)", ARRAY_ERRSTREAM)

  // Set all data to NULL state and no active cells
  memset(_Status, 0xFF, ByteArraySize(Size_));

  ARXMARKER("End: PtrVector::PtrVector(size_t)", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Pointer vector class destructor.
//
// POST:
//   Deletes the data item pointer array and the validity state bit array.
//
PtrVector::~PtrVector()
{
  ARXMARKER("Start: PtrVector::~PtrVector()", ARRAY_ERRSTREAM)

  delete[] _ArgsArray;
  _ArgsArray = NULL;

  ::Delete(_Status);
  _Status = NULL;

  ARXMARKER("End: PtrVector::~PtrVector()", ARRAY_ERRSTREAM)
}

/****************************************************************************/
// PURPOSE:
//   Method for setting a specific entry in the pointer vector with a given
//   data item and validity state.
//
// PRE:
//   size_t Index_	: The index position of the desired vector element
//   void* Ptr_		: The generic pointer to the data item to be assigned
//   Boolean Valid_	: The validity state of the assigned data item.
//
// POST:
//   Assigned the vector element at the specified index position with the
//   given data item pointer and validity state.
//
CountedVoidPtr<void> PtrVector::SetEntry(size_t Index_, CountedVoidPtr<void> Ptr_ , Boolean Valid_)
{
  ARXMARKER("Start: PtrVector::SetEntry(size_t, CountedVoidPtr<void>, Boolean)", ARRAY_ERRSTREAM)

  if (Valid_)
  {
    ARXMARKER("End(1): PtrVector::SetEntry(size_t, CountedVoidPtr<void>, Boolean)", ARRAY_ERRSTREAM)
    return SetValid(Index_, Ptr_);
  }

  ARXMARKER("End(2): PtrVector::SetEntry(size_t, CountedVoidPtr<void>, Boolean)", ARRAY_ERRSTREAM)
  return SetInvalid(Index_);
}

/****************************************************************************/
// PURPOSE:
//   Method to get the vector element at the specified index location.
//
// PRE:
//   size_t Index_	: The index of the vector element to be retrieved
//
// POST:
//   The vector element pointer at the specified index position is retrieved
//   and returned in generic void* pointer form.
//
CountedVoidPtr<void> PtrVector::GetEntry(size_t Index_) const
{
  ARXMARKER("Start/End: PtrVector::GetEntry(size_t)", ARRAY_ERRSTREAM)
  return _ArgsArray[Index_];
}

/****************************************************************************/
// PURPOSE:
//   Method to set the validity state of the element at the specified vector
//   position. The element is also set to a new pointer value.
//
// PRE:
//   State_t State_	: The state to set the indexed element to
//   size_t Index_	: The index of the desired vector element
//   void* Ptr_		: The new pointer value to set the vector element to
//
// POST:
//   The vector element at the specified index position is assigned the new
//   pointer value and set to the given validity state.
//
CountedVoidPtr<void> PtrVector::SetState(int State_, size_t Index_, CountedVoidPtr<void> Ptr_)
{
  ARXMARKER("Start: PtrVector::SetState(int, size_t, CountedVoidPtr<void>)", ARRAY_ERRSTREAM)

  switch (State_)
  {
    // If in invalid state the vector cell is still considered active
    // therefore is incremented into the running total. The validity bit
    // is unset and the array element is assigned to null as part of
    // lazy evaluation and data storage strategy.
    case INVALID_STATE:
      if (InState(NULL_STATE, Index_))
	++_Active;
      UnSetBit(Index_);
      _ArgsArray[Index_] = NULL;
      break;

    // If in valid state then if data item pointer is valid (not null) then
    // increment running total and set the validity bit on. Assign the passed
    // data item argument to the indexed position in the data array.
    case VALID_STATE:
      if (Ptr_.Pointee() != NULL)
      {
	if (InState(NULL_STATE, Index_))
	  ++_Active;
	SetBit(Index_);
	_ArgsArray[Index_] = Ptr_;
	break;
      }

    // If in null state then cell is not considered active. Decrement the
    // active cell running total and set the validity bit on since null data
    // value is also considered valid. Set the indexed position in the data
    // array to null.
    case NULL_STATE:
      if (!InState(NULL_STATE, Index_))
	--_Active;
      SetBit(Index_);
      _ArgsArray[Index_] = NULL;
      break;
  }

  // Return the vector entry at the given index position
  ARXMARKER("End: PtrVector::SetState(int, size_t, CountedVoidPtr<void>)", ARRAY_ERRSTREAM)
  return GetEntry(Index_);
}

/****************************************************************************/
// PURPOSE:
//   Method to test the "in state" status of the given vector entry with the
//   specified state value.
//
// PRE:
//   State_t State_	: The state to test whether vector entry is in
//   size_t Index_	: The index position of the vector entry to test.
//
// POST:
//   Returns TRUE if the element at the given index position is actually
//   in the state specified by the argument value, otherwise returns FALSE.
//
Boolean PtrVector::InState(int State_, size_t Index_) const
{
  ARXMARKER("Start: PtrVector::InState(int, size_t) const", ARRAY_ERRSTREAM)

  switch (State_)
  {
    case INVALID_STATE:
      ARXMARKER("End(1): PtrVector::InState(int, size_t) const", ARRAY_ERRSTREAM)
      return (!IsValidStatus(Index_) && IsNullEntry(Index_));
    case VALID_STATE:
      ARXMARKER("End(2): PtrVector::InState(int, size_t) const", ARRAY_ERRSTREAM)
      return (IsValidStatus(Index_) && !IsNullEntry(Index_));
    case NULL_STATE:
      ARXMARKER("End(3): PtrVector::InState(int, size_t) const", ARRAY_ERRSTREAM)
      return (IsValidStatus(Index_) && IsNullEntry(Index_));
  }

  ARXMARKER("End(4): PtrVector::InState(int, size_t) const", ARRAY_ERRSTREAM)
  return FALSE;
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
//   TPtrVector object.
//
void* PtrVector::operator new (size_t Bytes_)
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
void PtrVector::operator delete (void* Space_)
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
//   TPtrVector object.
//
#if	HAS_ARRAY_NEW
void* PtrVector::operator new[] (size_t Bytes_)
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
void PtrVector::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if PTRVECTOR_DEBUG
#include <iostream.h>
#if defined(__TURBOC__)
  #include <conio.h>
#endif

int main()
{
  int i;
  short Values_[20];
  PtrVector pv_(20);
  PtrVector pv2_(20);

#if defined(__TURBOC__)
  clrscr();
#endif

  for (i = 0; i < 20; i++)
    Values_[i] = i + 1;

  for (i = 0; i < 10; i++)
    if (i % 2)
      pv_.SetValid(i, PtrVector::DATA_OBJECT(&Values_[i], TypedObjectDeleter<short>::Make(), FALSE));
    else
      pv_.SetInvalid(i);

  cout <<"1st Init Done" <<endl;
  cout <<"Length = " <<pv_.GetLength() <<endl;
  cout <<"Active = " <<pv_.ActiveCells() <<endl;

  for (;i < 20; i++)
    if (i % 2)
      pv_.SetValid(i, PtrVector::DATA_OBJECT(&Values_[i], TypedObjectDeleter<short>::Make(), FALSE));
    else
      pv_.SetValid(i, PtrVector::DATA_OBJECT(NULL, TypedObjectDeleter<short>::Make(), FALSE));

  cout <<"2nd Init Done" <<endl;
  cout <<"Length = " <<pv_.GetLength() <<endl;
  cout <<"Active = " <<pv_.ActiveCells() <<endl;

  for (i = 0; i < 20; i++)
    if (pv_.IsValid(i))
      cout <<*((short*)pv_.GetEntry(i).Pointee()) <<" ";
    else if (pv_.IsNull(i))
      cout <<"N ";
    else if (pv_.IsInvalid(i))
      cout <<"I ";
    else
      cout <<"U ";

  cout <<endl <<"pv2_ = " <<endl;
  cout <<"Not Init" <<endl;
  cout <<"Length = " <<pv2_.GetLength() <<endl;
  cout <<"Active = " <<pv2_.ActiveCells() <<endl;

  for (i = 0; i < 20; i++)
    if (pv2_.IsValid(i))
      cout <<*((short*)pv2_.GetEntry(i).Pointee()) <<" ";
    else if (pv2_.IsNull(i))
      cout <<"N ";
    else if (pv2_.IsInvalid(i))
      cout <<"I ";
    else
      cout <<"U ";

  return 0;
}
#endif
/****************************************************************************/
#endif






