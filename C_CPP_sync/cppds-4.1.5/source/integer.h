#ifndef INTEGER_H
#define INTEGER_H
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif
#ifndef USHORTINT_H
  #include "ushortint.h"
#endif

#define INTEGER_DEBUG		0
#define INTEGER_DEBUG1		0
#define INTEGER_DEBUG2		0
#define INTEGER_DEBUG3		0

#define INTEGER_TRACE	0
#if INTEGER_TRACE
#  define INTXMARKER(x, strm)	XMARKER(x, strm)
#else
#  define INTXMARKER(x, strm)
#endif

/****************************************************************************/
class UIntegerAcceptor;

class IntegerAcceptor : public ObjectAcceptor
{
  friend class Integer;  

  protected:
    union
    {
      const Integer* _ObjPtr;
      Integer* _VarPtr;
    };

    IntegerAcceptor(const Integer* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Integer class specific acceptor methods
    virtual Boolean IsInteger() const;
    virtual Boolean IsTypeWrapper() const;

    virtual void* CreateFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* CreateFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromInteger(const ObjectAcceptor& Obj_);
    virtual void* CreateFromUShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromULongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromFloat(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDouble(const ObjectAcceptor& Obj_);
    virtual void* CreateFromLongDouble(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromInteger(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromULongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromFloat(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDouble(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongDouble(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongDouble(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsLesserToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToULongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongDouble(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsGreaterToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsGreaterToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUShortInt(const ObjectAcceptor& Obj_) const;
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
class Integer : public IntegerAsBitsType<Integer, int>
{
  friend class IntegerAcceptor;
  friend class UIntegerAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, Integer)
  TYPEWRAPPER_INTEGER_OPERATIONS(Integer)
  TYPEWRAPPER_ANSIMATH_OPERATIONS(Integer)

  friend Integer operator + (const Integer& Val1_, const Integer& Val2_);
  friend Integer operator - (const Integer& Val1_, const Integer& Val2_);
  friend Integer operator * (const Integer& Val1_, const Integer& Val2_);
  friend Integer operator / (const Integer& Val1_, const Integer& Val2_);
  friend Integer operator % (const Integer& Val1_, const Integer& Val2_);
  friend Integer operator & (const Integer& Val1_, const Integer& Val2_);
  friend Integer operator | (const Integer& Val1_, const Integer& Val2_);
  friend Integer operator ^ (const Integer& Val1_, const Integer& Val2_);        

  friend Integer operator + (int Val1_, const Integer& Val2_);
  friend Integer operator - (int Val1_, const Integer& Val2_);
  friend Integer operator * (int Val1_, const Integer& Val2_);
  friend Integer operator / (int Val1_, const Integer& Val2_);
  friend Integer operator % (int Val1_, const Integer& Val2_);
  friend Integer operator & (int Val1_, const Integer& Val2_);
  friend Integer operator | (int Val1_, const Integer& Val2_);
  friend Integer operator ^ (int Val1_, const Integer& Val2_);

  friend Integer operator + (const Integer& Val1_, int Val2_);
  friend Integer operator - (const Integer& Val1_, int Val2_);
  friend Integer operator * (const Integer& Val1_, int Val2_);
  friend Integer operator / (const Integer& Val1_, int Val2_);
  friend Integer operator % (const Integer& Val1_, int Val2_);
  friend Integer operator & (const Integer& Val1_, int Val2_);
  friend Integer operator | (const Integer& Val1_, int Val2_);
  friend Integer operator ^ (const Integer& Val1_, int Val2_);  

  protected:
    union
    {
      Byte_t _Valid;
      int _Value;
      Uint _Uvalue;
    };

    virtual Boolean IsEqualToInteger(const Integer* Ptr_) const;
    virtual Boolean IsLesserToInteger(const Integer* Ptr_) const;
    virtual Boolean IsGreaterToInteger(const Integer* Ptr_) const;

    Integer(Uint UIntVal_, Boolean SetNull_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      Integer(const UShortInt& IntObj_);
    #endif
    void AssignUShortInt(const UShortInt& Obj_);

  public:
    Integer(int IntVal_=0);
    Integer(const ChrString& Str_, Boolean Signed_=TRUE, Boolean Validity_=FALSE);
    Integer(const Integer& IntObj_);
    Integer(const ShortInt& IntObj_);
    virtual ~Integer() {}

    virtual Integer& DerivedClass();
    virtual const Integer& DerivedClass() const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    // Typed Wrapper methods
    virtual void SetData(const int& Obj_);
    virtual const int& GetData() const;
    virtual operator int () const
	{ return _Value; }    

    // Virtual constructor and assignment methods
    static Integer* Make(int IntVal_=0);
    static Integer* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Integer& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static Integer UpperBound()
	{ return Integer(INT_MAX); }
    static Integer LowerBound()
	{ return Integer(INT_MIN); }
    static Boolean IsValue(const ChrString& Str_);
    static Integer ValueOf(const ChrString& Str_);
    static ChrString ToString(const Integer& Int_, int Radix_=10);
    static ChrString ToTruncString(const Integer& Int_, int Radix_=10);
    virtual ChrString ToString(int Radix_=10) const;    
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual Integer& FromString(const ChrString& Str_);

    virtual double FloatValue() const;
    int IntValue() const
	{ return _Value; }
    Uint UnsignedValue() const
	{ return _Uvalue; }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::INTEGER_TYPE; }
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
    virtual Integer& operator = (const int& IntVal_);
    virtual Integer& operator = (const ChrString& Str_);
    virtual Integer& operator = (const Integer& Obj_);
    virtual Integer& operator = (const ShortInt& Obj_);

    // Unary operators
    virtual Boolean operator ! () const;
    Integer operator - () const;
    Integer operator + () const;
    Integer operator ~ () const;
    Integer operator << (size_t Shift_) const;
    Integer operator >> (size_t Shift_) const;

    // Increment and decrement operators
    virtual const Integer& operator ++ ();
    virtual const Integer& operator ++ (int);
    virtual const Integer& operator -- ();
    virtual const Integer& operator -- (int);

    // Arithmetic assignment operators
    virtual Integer& operator += (const NumOrdinalType<Integer>& Obj_);
    virtual Integer& operator -= (const NumOrdinalType<Integer>& Obj_);
    virtual Integer& operator *= (const NumOrdinalType<Integer>& Obj_);
    virtual Integer& operator /= (const NumOrdinalType<Integer>& Obj_);
    virtual Integer& operator %= (const NumOrdinalType<Integer>& Obj_);    

    virtual Integer& operator += (const int& IntVal_);
    virtual Integer& operator -= (const int& IntVal_);
    virtual Integer& operator *= (const int& IntVal_);
    virtual Integer& operator /= (const int& IntVal_);
    virtual Integer& operator %= (const int& IntVal_);

    // Bitwise assignment operators
    virtual Integer& operator |= (const NumOrdinalAsBitsType<Integer>& Obj_);
    virtual Integer& operator &= (const NumOrdinalAsBitsType<Integer>& Obj_);
    virtual Integer& operator ^= (const NumOrdinalAsBitsType<Integer>& Obj_);
    virtual Integer& operator |= (const int& IntVal_);
    virtual Integer& operator &= (const int& IntVal_);
    virtual Integer& operator ^= (const int& IntVal_);
    virtual Integer& operator <<= (size_t Shift_);
    virtual Integer& operator >>= (size_t Shift_);

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
struct IntegerData
{
	enum { INT_VALUE, STRING_OBJECT, INTEGER_OBJECT };

	int _Type;
	Object* _ObjData;
	int _ValData;
};

class IntegerAction
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
	void Execute(IntegerData*);
};

class IntegerRunnable : public THREAD
{
  protected:
	IntegerAction _Action;
    virtual void* Run(void*);	
};

#endif

/****************************************************************************/
#endif





