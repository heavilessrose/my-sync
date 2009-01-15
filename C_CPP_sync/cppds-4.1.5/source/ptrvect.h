/****************************************************************************/
// FILE : PtrVect.h
// PURPOSE:
// Pointer vector and bitstring class for storage of data items and their
// validity state for the array class.
/****************************************************************************/
#ifndef PTRVECTOR_H
#define PTRVECTOR_H
#ifndef ARRAYDEF_H
  #include "arraydef.h"
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif
#ifndef SMARTPTR_CPP
  #include "smartptr.cpp"
#endif

#define PTRVECTOR_DEBUG		0

/****************************************************************************/
// Pointer vector class for the actual storage of the data member in the
// array class and their associated validity state.
class PtrVector
{
  public:
    typedef CountedVoidPtr<void> DATA_OBJECT;

#if !PTRVECTOR_DEBUG
  protected:
#endif
    // Pointer vector class constructor and destructor.
    PtrVector(size_t Size_);
    ~PtrVector();

    // Vector entry set and get methods
    CountedVoidPtr<void> SetEntry(size_t Index_, CountedVoidPtr<void> Ptr_, Boolean Valid_);
    CountedVoidPtr<void> GetEntry(size_t Index_) const;

    // Validity state mutator methods
    CountedVoidPtr<void> SetValid(size_t Index_, CountedVoidPtr<void> Ptr_)
	{ ARXMARKER("Start/End: PtrVector::SetValid(size_t, CountedVoidPtr<void>)", ARRAY_ERRSTREAM)
	  return SetState(VALID_STATE, Index_, Ptr_); }
    CountedVoidPtr<void> SetInvalid(size_t Index_)
	{ ARXMARKER("Start/End: PtrVector::SetInvalid(size_t)", ARRAY_ERRSTREAM)
	  return SetState(INVALID_STATE, Index_); }
    CountedVoidPtr<void> SetNull(size_t Index_)
	{ ARXMARKER("Start/End: PtrVector::SetNull(size_t)", ARRAY_ERRSTREAM)
	  return SetState(NULL_STATE, Index_); }

#if !PTRVECTOR_DEBUG
  private:
#endif
    // Possible validity states of array elements
    enum State_t { INVALID_STATE, VALID_STATE, NULL_STATE };

    size_t _Length;		// Vector length
    size_t _Active;		// Active cell count

    char* _Status;	// validity state bit vector
    CountedVoidPtr<void>* _ArgsArray;

    // Bit vector accessing/manipulation methods
    inline size_t ByteArraySize(size_t BitNum_) const
	{ return size_t(ceil(double(BitNum_) / 8)); }
    inline size_t ByteIndex(size_t BitNum_) const
	{ return size_t(floor(double(BitNum_) / 8)); }
    inline size_t BitIndex(size_t BitNum_) const
	{ return (BitNum_ - ByteIndex(BitNum_) * 8); }
    inline void SetBit(size_t BitNum_)
	{ _Status[ByteIndex(BitNum_)] |= (1 << BitIndex(BitNum_)); }
    inline void UnSetBit(size_t BitNum_)
	{ _Status[ByteIndex(BitNum_)] &= ~(1 << BitIndex(BitNum_)); }
    inline char GetBit(size_t BitNum_) const
	{ return (_Status[ByteIndex(BitNum_)] & (1 << BitIndex(BitNum_))); }

    // Validity state and data item state accessor methods
    inline Boolean IsValidStatus(size_t Index_) const
	{ return (GetBit(Index_) != 0); }
    inline Boolean IsNullEntry(size_t Index_) const
	{ return (_ArgsArray[Index_].Pointee() == NULL); }

    // Validity state mutator method and tester method
    CountedVoidPtr<void> SetState(int State_, size_t Index_, CountedVoidPtr<void> Ptr_=CountedVoidPtr<void>());
    Boolean InState(int State_, size_t Index_) const;

  public:
    // Validity state tester methods
    inline Boolean IsValid(size_t Index_) const
	{ ARXMARKER("Start/End: PtrVector::IsValid(size_t)", ARRAY_ERRSTREAM)
	  return InState(VALID_STATE, Index_); }
    inline Boolean IsInvalid(size_t Index_) const
	{ ARXMARKER("Start/End: PtrVector::IsInvalid(size_t)", ARRAY_ERRSTREAM)
	  return InState(INVALID_STATE, Index_); }
    inline Boolean IsNull(size_t Index_) const
	{ ARXMARKER("Start/End: PtrVector::IsNull(size_t)", ARRAY_ERRSTREAM)
	  return InState(NULL_STATE, Index_); }
    inline Boolean IsDataDefined(size_t Index_) const
	{ ARXMARKER("Start/End: PtrVector::IsDataDefined(size_t)", ARRAY_ERRSTREAM)
	  return IsValidStatus(Index_); }
    inline Boolean Ownership(size_t Index_) const
	{ ARXMARKER("Start/End: PtrVector::Ownership(size_t)", ARRAY_ERRSTREAM)
	  return _ArgsArray[Index_].HasOwnership(); }

    // inline methods for accessing active cell count and total vector length
    inline size_t ActiveCells() const
	{ return _Active; }
    inline size_t GetLength() const
	{ return _Length; }

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
#endif





