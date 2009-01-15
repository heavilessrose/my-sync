#ifndef LONGINT_H
#define LONGINT_H
#ifndef UNSIGNED_H
  #include "unsigned.h"
#endif

#define LONGINT_DEBUG		0

/****************************************************************************/
class ULongIntAcceptor;

class LongIntAcceptor : public ObjectAcceptor
{
  friend class LongInt;

  protected:
    union
    {
      const LongInt* _ObjPtr;
      LongInt* _VarPtr;
    };

    LongIntAcceptor(const LongInt* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // LongInt class specific acceptor methods
    virtual Boolean IsLongInt() const;
    virtual Boolean IsTypeWrapper() const;    

    virtual void* CreateFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);    
    virtual void* CreateFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromInteger(const ObjectAcceptor& Obj_);
    virtual void* CreateFromUShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromUInteger(const ObjectAcceptor& Obj_);    
    virtual void* CreateFromFloat(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDouble(const ObjectAcceptor& Obj_);
    virtual void* CreateFromLongDouble(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromInteger(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUInteger(const ObjectAcceptor& Obj_);
    virtual void* AssignFromFloat(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDouble(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongDouble(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongDouble(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsLesserToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUInteger(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongDouble(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsGreaterToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsGreaterToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongDouble(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class LongInt : public IntegerAsBitsType<LongInt, long>
{
  friend class LongIntAcceptor;
  friend class ULongIntAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, LongInt)
  TYPEWRAPPER_INTEGER_OPERATIONS(LongInt)
  TYPEWRAPPER_ANSIMATH_OPERATIONS(LongInt)

  friend LongInt operator + (const LongInt& Val1_, const LongInt& Val2_);
  friend LongInt operator - (const LongInt& Val1_, const LongInt& Val2_);
  friend LongInt operator * (const LongInt& Val1_, const LongInt& Val2_);
  friend LongInt operator / (const LongInt& Val1_, const LongInt& Val2_);
  friend LongInt operator % (const LongInt& Val1_, const LongInt& Val2_);
  friend LongInt operator & (const LongInt& Val1_, const LongInt& Val2_);
  friend LongInt operator | (const LongInt& Val1_, const LongInt& Val2_);
  friend LongInt operator ^ (const LongInt& Val1_, const LongInt& Val2_);

  friend LongInt operator + (long Val1_, const LongInt& Val2_);
  friend LongInt operator - (long Val1_, const LongInt& Val2_);
  friend LongInt operator * (long Val1_, const LongInt& Val2_);
  friend LongInt operator / (long Val1_, const LongInt& Val2_);
  friend LongInt operator % (long Val1_, const LongInt& Val2_);
  friend LongInt operator & (long Val1_, const LongInt& Val2_);
  friend LongInt operator | (long Val1_, const LongInt& Val2_);
  friend LongInt operator ^ (long Val1_, const LongInt& Val2_);

  friend LongInt operator + (const LongInt& Val1_, long Val2_);
  friend LongInt operator - (const LongInt& Val1_, long Val2_);
  friend LongInt operator * (const LongInt& Val1_, long Val2_);
  friend LongInt operator / (const LongInt& Val1_, long Val2_);
  friend LongInt operator % (const LongInt& Val1_, long Val2_);
  friend LongInt operator & (const LongInt& Val1_, long Val2_);
  friend LongInt operator | (const LongInt& Val1_, long Val2_);
  friend LongInt operator ^ (const LongInt& Val1_, long Val2_);  

  protected:
    union
    {
      Byte_t _Valid;
      long _Value;
      Ulong _Uvalue;
    };

    virtual Boolean IsEqualToLongInt(const LongInt* Ptr_) const;
    virtual Boolean IsLesserToLongInt(const LongInt* Ptr_) const;
    virtual Boolean IsGreaterToLongInt(const LongInt* Ptr_) const;

    static Boolean IsUShortInt(const ShortInt& Obj_);
    
    LongInt(Ulong ULongVal_, Boolean SetToNull_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      LongInt(const UInteger& IntObj_);
    #endif
    void AssignUInteger(const UInteger& Obj_);

  public:
    LongInt(long LongVal_=0);
    LongInt(const ChrString& Str_, Boolean Signed_=TRUE, Boolean Validity_=FALSE);
    LongInt(const LongInt& IntObj_);
    LongInt(const ShortInt& IntObj_);
    LongInt(const Integer& IntObj_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      LongInt(const UShortInt& IntObj_);
    #endif
    virtual ~LongInt() {}

    virtual LongInt& DerivedClass();
    virtual const LongInt& DerivedClass() const;

    // Typed Wrapper methods
    virtual void SetData(const long& Obj_);
    virtual const long& GetData() const;
    operator long () const
	{ return _Value; }    

    // Virtual constructor and assignment methods
    static LongInt* Make(long IntVal_=0);
    static LongInt* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;        

    // Static null object accessor methods
    static LongInt& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static LongInt UpperBound()
	{ return LongInt(LONG_MAX); }
    static LongInt LowerBound()
	{ return LongInt(LONG_MIN); }
    static Boolean IsValue(const ChrString& Str_);
    static LongInt ValueOf(const ChrString& Str_);
    static ChrString ToString(const LongInt& Int_, int Radix_=10);
    static ChrString ToTruncString(const LongInt& Int_, int Radix_=10);
    virtual ChrString ToString(int Radix_=10) const;    
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual LongInt& FromString(const ChrString& Str_);

    virtual double FloatValue() const;
    long IntValue() const
	{ return _Value; }
    Ulong UnsignedValue() const
	{ return _Uvalue; }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::LONGINT_TYPE; }
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

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // At maximum/minimum value test functions
    virtual Boolean AtZero() const;
    virtual Boolean AtMaxValue() const;
    virtual Boolean AtMinValue() const;

    // Assignment method
    virtual LongInt& operator = (const long& IntVal_);
    virtual LongInt& operator = (const ChrString& Str_);
    virtual LongInt& operator = (const Integer& Obj_);
    virtual LongInt& operator = (const LongInt& Obj_);
    virtual LongInt& operator = (const ShortInt& Obj_);    
    virtual LongInt& operator = (const UShortInt& Obj_);

    // Unary operators
    virtual Boolean operator ! () const;
    LongInt operator - () const;
    LongInt operator + () const;
    LongInt operator ~ () const;
    LongInt operator << (size_t Shift_) const;
    LongInt operator >> (size_t Shift_) const;

    // Increment and decrement operators
    virtual const LongInt& operator ++ ();
    virtual const LongInt& operator ++ (int);
    virtual const LongInt& operator -- ();
    virtual const LongInt& operator -- (int);

    // Arithmetic assignment operators
    virtual LongInt& operator += (const NumOrdinalType<LongInt>& Obj_);
    virtual LongInt& operator -= (const NumOrdinalType<LongInt>& Obj_);
    virtual LongInt& operator *= (const NumOrdinalType<LongInt>& Obj_);
    virtual LongInt& operator /= (const NumOrdinalType<LongInt>& Obj_);
    virtual LongInt& operator %= (const NumOrdinalType<LongInt>& Obj_);

    virtual LongInt& operator += (const long& IntVal_);
    virtual LongInt& operator -= (const long& IntVal_);
    virtual LongInt& operator *= (const long& IntVal_);
    virtual LongInt& operator /= (const long& IntVal_);
    virtual LongInt& operator %= (const long& IntVal_);

    // Bitwise assignment operators
    virtual LongInt& operator |= (const NumOrdinalAsBitsType<LongInt>& Obj_);
    virtual LongInt& operator &= (const NumOrdinalAsBitsType<LongInt>& Obj_);
    virtual LongInt& operator ^= (const NumOrdinalAsBitsType<LongInt>& Obj_);
    virtual LongInt& operator |= (const long& IntVal_);
    virtual LongInt& operator &= (const long& IntVal_);
    virtual LongInt& operator ^= (const long& IntVal_);    
    LongInt& operator <<= (size_t Shift_);
    LongInt& operator >>= (size_t Shift_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
#endif





