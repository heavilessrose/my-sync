/****************************************************************************/
// FILE : BitVect.h
// PURPOSE:
// Simple bit vector class for storage of bits
/****************************************************************************/
#ifndef BITVECTOR_H
#define BITVECTOR_H
#ifndef BUILTIN_H
  #include "builtin.h"
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif

#define BITVECTOR_DEBUG		0

/****************************************************************************/
// Bit vector reference class forward declaration
class BitVectorRef;

// Bit vector class for the actual storage of the data member in the
// array class and their associated validity state.
class BitVector
{
  friend int Compare(const BitVector& Val1_, const BitVector& Val2_);
  friend Boolean operator < (const BitVector& Val1_, const BitVector& Val2_);
  friend Boolean operator > (const BitVector& Val1_, const BitVector& Val2_);
  friend Boolean operator <= (const BitVector& Val1_, const BitVector& Val2_);
  friend Boolean operator >= (const BitVector& Val1_, const BitVector& Val2_);
  friend Boolean operator == (const BitVector& Val1_, const BitVector& Val2_);
  friend Boolean operator != (const BitVector& Val1_, const BitVector& Val2_);
  
  friend Boolean operator == (Ulong Val1_, const BitVector& Val2_);
  friend Boolean operator != (Ulong Val1_, const BitVector& Val2_);
  friend Boolean operator == (const BitVector& Val1_, Ulong Val2_);
  friend Boolean operator != (const BitVector& Val1_, Ulong Val2_);
  
  friend BitVector operator & (const BitVector& Val1_, const BitVector& Val2_);
  friend BitVector operator | (const BitVector& Val1_, const BitVector& Val2_);
  friend BitVector operator ^ (const BitVector& Val1_, const BitVector& Val2_);
  friend BitVector operator & (Ulong Val1_, const BitVector& Val2_);
  friend BitVector operator | (Ulong Val1_, const BitVector& Val2_);
  friend BitVector operator ^ (Ulong Val1_, const BitVector& Val2_);  
  friend BitVector operator & (const BitVector& Val1_, Ulong Val2_);
  friend BitVector operator | (const BitVector& Val1_, Ulong Val2_);
  friend BitVector operator ^ (const BitVector& Val1_, Ulong Val2_);

  friend BitVector operator + (const BitVector& Val1_, const BitVector& Val2_);
  friend BitVector operator + (Ulong Val1_, const BitVector& Val2_);
  friend BitVector operator + (const BitVector& Val1_, Ulong Val2_);

  friend istream& operator >> (istream& is, BitVector& obj);
  friend ostream& operator << (ostream& os, const BitVector& obj);

  // Hashing helper functions
  friend const char* _HashableData(const BitVector& Obj_)
        { return Obj_.GiveHashableData(); }
  friend long _HashableDataLength(const BitVector& Obj_)
        { return Obj_.GiveHashableDataLength(); }  

  public:
    typedef Ulong SizeType;

  protected:
    SizeType _Length;	// Vector length
    Byte_t* _Status;	// validity state bit vector

    // Bit vector accessing/manipulation methods
    static SizeType ByteArraySize(SizeType BitNum_)
	{ return SizeType(ceil(double(BitNum_) / 8)); }
    static SizeType ByteIndex(SizeType BitNum_)
	{ return SizeType(floor(double(BitNum_) / 8)); }
    static SizeType BitIndex(SizeType BitNum_)
	{ return (BitNum_ - ByteIndex(BitNum_) * 8); }

    static char* StrRev(char* Str_);
    static Byte_t* MemMove(Byte_t* Str1_, const Byte_t* Str2_, size_t Max_);
    static Byte_t* MemSet(Byte_t* Str_, int Char_, size_t Max_);
    static char* AppendStr(char* Input_, char* Buffer_);    

  public:
    // Pointer vector class constructor and destructor.
    BitVector(SizeType Size_=0, char* Str_=NULL, Ulong Value_=0);
    BitVector(const BitVector& Obj_);
    virtual ~BitVector();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::BITVECTOR_TYPE; }
    virtual int ClassID() const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;

    // Virtual constructor and assignment methods
    static BitVector* Make(SizeType Size_);
    virtual BitVector* Clone() const;

    // I/O methods
    istream& BinaryRead(istream& Is_);
    ostream& BinaryWrite(ostream& Os_) const;

    istream& TextRead(istream& Is_);
    ostream& TextWrite(ostream& Os_) const;

    // Input/Output conversion methods
    char* ToString() const;
    BitVector& FromString(char* Str_);
    char* ToHexString(Boolean WithPrefix_) const;
    BitVector& FromHexString(char* Str_, Boolean WithPrefix_);
    Ulong ToIntValue(Boolean& Convertable_) const;
    BitVector& FromIntValue(Ulong Val_);

    // Assignment and appending
    BitVector& operator = (const BitVector& Obj_);
    BitVector& operator += (const BitVector& Obj_);
    BitVector& Grow(SizeType Ext_);
    BitVector& Resize(SizeType Len_);

    // Bit vector length methods
    SizeType Trim();
    inline SizeType GetLength() const
	{ return _Length; }

    // Bit manipulation methods
    void ToggleBit(SizeType BitNum_);
    BitVector& SetBit(SizeType BitNum_);
    BitVector& UnSetBit(SizeType BitNum_);
    Byte_t GetBit(SizeType BitNum_) const;

    BitVectorRef operator [] (SizeType BitNum_);
    BitVectorRef operator [] (SizeType BitNum_) const;

    BitVector& SetAllBits();
    BitVector& UnSetAllBits();
    BitVector& ToggleAllBits();

    // Bitwise tests
    BitVector::SizeType NextBit(SizeType Pos_, Boolean& Found_) const;
    SizeType PreviousBit(SizeType Pos_, Boolean& Found_) const;
    Boolean Any() const;
    Boolean None() const;
    BitVector::SizeType GetMsb(Boolean& Found_) const;
    BitVector::SizeType GetLsb(Boolean& Found_) const;
    BitVector::SizeType NumBitsSet() const;

    // Bit operations
    Boolean operator ! () const;    
    BitVector operator ~ () const;
    BitVector operator << (SizeType Shift_) const;
    BitVector operator >> (SizeType Shift_) const;
    
    BitVector& operator |= (const BitVector& Obj_);
    BitVector& operator &= (const BitVector& Obj_);
    BitVector& operator ^= (const BitVector& Obj_);
    BitVector& operator <<= (SizeType Shift_);
    BitVector& operator >>= (SizeType Shift_);

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
class BitVectorRef
{
  friend class BitVector;

  protected:
    Boolean _Const;
    BitVector::SizeType _BitNum;
    
    union
    {
      BitVector* _Bvp;
      const BitVector* _ConstBvp;
    };

    BitVectorRef(const BitVector* Bv_, BitVector::SizeType BitNum_);
    BitVectorRef(BitVector* Bv_, BitVector::SizeType BitNum_, int);

  public:
    BitVectorRef& operator = (Byte_t Bit_);
    operator Byte_t () const;
};

/****************************************************************************/
#endif





