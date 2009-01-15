#ifndef SHORTINT_H
#define SHORTINT_H
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif

#define SHORTINT_DEBUG		0
#define SHORTINT_DEBUG1		0
#define SHORTINT_DEBUG2		0
#define SHORTINT_DEBUG3		0

/****************************************************************************/
class ShortIntAcceptor : public ObjectAcceptor
{
  friend class ShortInt;

  protected:
    union
    {
      const ShortInt* _ObjPtr;
      ShortInt* _VarPtr;
    };

    ShortIntAcceptor(const ShortInt* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // ShortInt class specific acceptor methods
    virtual Boolean IsShortInt() const;
    virtual Boolean IsTypeWrapper() const;

    virtual void* CreateFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* CreateFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromInteger(const ObjectAcceptor& Obj_);
    virtual void* CreateFromUInteger(const ObjectAcceptor& Obj_);
    virtual void* CreateFromULongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromFloat(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDouble(const ObjectAcceptor& Obj_);
    virtual void* CreateFromLongDouble(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromInteger(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUInteger(const ObjectAcceptor& Obj_);
    virtual void* AssignFromULongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromFloat(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDouble(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongDouble(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongDouble(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsLesserToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToULongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongDouble(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsGreaterToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsGreaterToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToULongInt(const ObjectAcceptor& Obj_) const;
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
class ShortInt : public IntegerAsBitsType<ShortInt, short>
{
  friend class ShortIntAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, ShortInt)
  TYPEWRAPPER_INTEGER_OPERATIONS(ShortInt)
  TYPEWRAPPER_ANSIMATH_OPERATIONS(ShortInt)

  friend ShortInt operator + (const ShortInt& Val1_, const ShortInt& Val2_);
  friend ShortInt operator - (const ShortInt& Val1_, const ShortInt& Val2_);
  friend ShortInt operator * (const ShortInt& Val1_, const ShortInt& Val2_);
  friend ShortInt operator / (const ShortInt& Val1_, const ShortInt& Val2_);
  friend ShortInt operator % (const ShortInt& Val1_, const ShortInt& Val2_);
  friend ShortInt operator & (const ShortInt& Val1_, const ShortInt& Val2_);
  friend ShortInt operator | (const ShortInt& Val1_, const ShortInt& Val2_);
  friend ShortInt operator ^ (const ShortInt& Val1_, const ShortInt& Val2_);

  friend ShortInt operator + (short Val1_, const ShortInt& Val2_);
  friend ShortInt operator - (short Val1_, const ShortInt& Val2_);
  friend ShortInt operator * (short Val1_, const ShortInt& Val2_);
  friend ShortInt operator / (short Val1_, const ShortInt& Val2_);
  friend ShortInt operator % (short Val1_, const ShortInt& Val2_);
  friend ShortInt operator & (short Val1_, const ShortInt& Val2_);
  friend ShortInt operator | (short Val1_, const ShortInt& Val2_);
  friend ShortInt operator ^ (short Val1_, const ShortInt& Val2_);

  friend ShortInt operator + (const ShortInt& Val1_, short Val2_);
  friend ShortInt operator - (const ShortInt& Val1_, short Val2_);
  friend ShortInt operator * (const ShortInt& Val1_, short Val2_);
  friend ShortInt operator / (const ShortInt& Val1_, short Val2_);
  friend ShortInt operator % (const ShortInt& Val1_, short Val2_);
  friend ShortInt operator & (const ShortInt& Val1_, short Val2_);
  friend ShortInt operator | (const ShortInt& Val1_, short Val2_);
  friend ShortInt operator ^ (const ShortInt& Val1_, short Val2_);  

  protected:
    union
    {
      Byte_t _Valid;
      short _Value;
      Ushort _Uvalue;
    };

    virtual Boolean IsEqualToShortInt(const ShortInt* Ptr_) const;
    virtual Boolean IsLesserToShortInt(const ShortInt* Ptr_) const;
    virtual Boolean IsGreaterToShortInt(const ShortInt* Ptr_) const;

    ShortInt(Ushort UIntVal_, Boolean SetToNull_);

  public:
    ShortInt(short IntVal_=0);
    ShortInt(const ChrString& Str_, Boolean Signed_=TRUE, Boolean Validity_=FALSE);
    ShortInt(const ShortInt& IntObj_);
    virtual ~ShortInt() {}

    virtual ShortInt& DerivedClass();
    virtual const ShortInt& DerivedClass() const;
    
    // Typed Wrapper methods
    virtual const short& GetData() const;
    virtual void SetData(const short& Obj_);
    operator short () const
	{ return _Value; }

    // Virtual constructor and assignment methods
    static ShortInt* Make(short IntVal_=0);
    static ShortInt* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    // Static null object accessor methods
    static ShortInt& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static ShortInt UpperBound()
	{ return ShortInt(SHRT_MAX); }
    static ShortInt LowerBound()
	{ return ShortInt(SHRT_MIN); }
    static Boolean IsValue(const ChrString& Str_);
    static ShortInt ValueOf(const ChrString& Str_);
    static ChrString ToString(const ShortInt& Int_, int Radix_=10);
    static ChrString ToTruncString(const ShortInt& Int_, int Radix_=10);
    virtual ChrString ToString(int Radix_=10) const;    
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual ShortInt& FromString(const ChrString& Str_);

    virtual double FloatValue() const;
    short IntValue() const
	{ return _Value; }
    Ushort UnsignedValue() const
	{ return _Uvalue; }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SHORTINT_TYPE; }
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
    virtual ShortInt& operator = (const short& IntVal_);
    virtual ShortInt& operator = (const ChrString& Str_);
    virtual ShortInt& operator = (const ShortInt& Obj_);

    // Unary operators
    virtual Boolean operator ! () const;
    ShortInt operator - () const;
    ShortInt operator + () const;
    ShortInt operator ~ () const;
    ShortInt operator << (size_t Shift_) const;
    ShortInt operator >> (size_t Shift_) const;

    // Increment and decrement operators
    virtual const ShortInt& operator ++ ();
    virtual const ShortInt& operator ++ (int);
    virtual const ShortInt& operator -- ();
    virtual const ShortInt& operator -- (int);

    // Arithmetic assignment operators
    virtual ShortInt& operator += (const NumOrdinalType<ShortInt>& Obj_);
    virtual ShortInt& operator -= (const NumOrdinalType<ShortInt>& Obj_);
    virtual ShortInt& operator *= (const NumOrdinalType<ShortInt>& Obj_);
    virtual ShortInt& operator /= (const NumOrdinalType<ShortInt>& Obj_);
    virtual ShortInt& operator %= (const NumOrdinalType<ShortInt>& Obj_);

    virtual ShortInt& operator += (const short& IntVal_);
    virtual ShortInt& operator -= (const short& IntVal_);
    virtual ShortInt& operator *= (const short& IntVal_);
    virtual ShortInt& operator /= (const short& IntVal_);
    virtual ShortInt& operator %= (const short& IntVal_);

    // Bitwise assignment operators
    virtual ShortInt& operator |= (const NumOrdinalAsBitsType<ShortInt>& Obj_);
    virtual ShortInt& operator &= (const NumOrdinalAsBitsType<ShortInt>& Obj_);
    virtual ShortInt& operator ^= (const NumOrdinalAsBitsType<ShortInt>& Obj_);
    virtual ShortInt& operator |= (const short& IntVal_);
    virtual ShortInt& operator &= (const short& IntVal_);
    virtual ShortInt& operator ^= (const short& IntVal_);
    virtual ShortInt& operator <<= (size_t Shift_);
    virtual ShortInt& operator >>= (size_t Shift_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

#if SHORTINT_DEBUG1
#include "thread.h"
#include "mutex.h"
struct ShortIntData
{
	enum { SHORT_VALUE, STRING_OBJECT, SHORTINT_OBJECT };

	int _Type;
	Object* _ObjData;
	short _ValData;
};

class ShortIntAction
{
  private:
	static int _Init;	
	CRITICAL_SECTION* GiveMutex()
	{ 	static CRITICAL_SECTION _Mutex;
		if (!_Init)
		{
			InitializeCriticalSection(&_Mutex);
			_Init = 1;
		}
		return &_Mutex; }

  public:
	void Execute(ShortIntData*);
};

class ShortIntRunnable : public THREAD
{
  protected:
	ShortIntAction _Action;
    virtual void* Run(void*);	
};

#endif

/****************************************************************************/
#endif





