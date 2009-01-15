#ifndef UNSIGNED_H
#define UNSIGNED_H
#ifndef INTEGER_H
  #include "integer.h"
#endif

#define UNSIGNED_DEBUG		0

/****************************************************************************/
class UIntegerAcceptor : public IntegerAcceptor
{
  friend class UInteger;

  private:
    UIntegerAcceptor(const UInteger* Ptr_);

  public:
    // Integer class specific acceptor methods
    virtual Boolean IsInteger() const;
    virtual Boolean IsUInteger() const;

    virtual void* CreateFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* CreateFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromInteger(const ObjectAcceptor& Obj_);
    virtual void* CreateFromULongInt(const ObjectAcceptor& Obj_);
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
    virtual void* AssignFromULongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUInteger(const ObjectAcceptor& Obj_);    
    virtual void* AssignFromFloat(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDouble(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongDouble(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class UInteger : public Integer
{
  friend class IntegerAcceptor;
  friend class UIntegerAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, UInteger)

  friend UInteger operator + (const UInteger& Val1_, const UInteger& Val2_);
  friend UInteger operator - (const UInteger& Val1_, const UInteger& Val2_);
  friend UInteger operator * (const UInteger& Val1_, const UInteger& Val2_);
  friend UInteger operator / (const UInteger& Val1_, const UInteger& Val2_);
  friend UInteger operator % (const UInteger& Val1_, const UInteger& Val2_);
  friend UInteger operator & (const UInteger& Val1_, const UInteger& Val2_);
  friend UInteger operator | (const UInteger& Val1_, const UInteger& Val2_);
  friend UInteger operator ^ (const UInteger& Val1_, const UInteger& Val2_);        

  friend UInteger operator + (Uint Val1_, const UInteger& Val2_);
  friend UInteger operator - (Uint Val1_, const UInteger& Val2_);
  friend UInteger operator * (Uint Val1_, const UInteger& Val2_);
  friend UInteger operator / (Uint Val1_, const UInteger& Val2_);
  friend UInteger operator % (Uint Val1_, const UInteger& Val2_);
  friend UInteger operator & (Uint Val1_, const UInteger& Val2_);
  friend UInteger operator | (Uint Val1_, const UInteger& Val2_);
  friend UInteger operator ^ (Uint Val1_, const UInteger& Val2_);

  friend UInteger operator + (const UInteger& Val1_, Uint Val2_);
  friend UInteger operator - (const UInteger& Val1_, Uint Val2_);
  friend UInteger operator * (const UInteger& Val1_, Uint Val2_);
  friend UInteger operator / (const UInteger& Val1_, Uint Val2_);
  friend UInteger operator % (const UInteger& Val1_, Uint Val2_);
  friend UInteger operator & (const UInteger& Val1_, Uint Val2_);
  friend UInteger operator | (const UInteger& Val1_, Uint Val2_);
  friend UInteger operator ^ (const UInteger& Val1_, Uint Val2_);  

  protected:
    virtual Boolean IsLesserToInteger(const Integer* Ptr_) const;
    virtual Boolean IsGreaterToInteger(const Integer* Ptr_) const;

  public:
    UInteger(Uint IntVal_=0);
    UInteger(const ChrString& Str_);
    UInteger(const Integer& IntObj_);
    UInteger(const ShortInt& IntObj_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      UInteger(const UInteger& IntObj_);
      UInteger(const UShortInt& UIntObj_);
    #endif

    // Virtual constructor and assignment methods
    static UInteger* Make(Uint IntVal_=0);
    static UInteger* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);    
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static UInteger& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static UInteger UpperBound()
	{ return UInteger(UINT_MAX); }
    static UInteger LowerBound()
	{ return UInteger(0); }
    static Boolean IsValue(const ChrString& Str_);
    static UInteger ValueOf(const ChrString& Str_);
    static ChrString ToString(const Integer& Int_, int Radix_=10);
    static ChrString ToTruncString(const Integer& Int_, int Radix_=10);
    virtual ChrString ToString(int Radix_=10) const;    
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual Integer& FromString(const ChrString& Str_);

    operator Uint () const
	{ return _Uvalue; }
    virtual double FloatValue() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::UINTEGER_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // At maximum/minimum value test functions
    virtual Boolean AtMaxValue() const;
    virtual Boolean AtMinValue() const;

    // Assignment method
    virtual Integer& operator = (const int& IntVal_);
    virtual Integer& operator = (const ChrString& Str_);
    virtual Integer& operator = (const Integer& Obj_);
    virtual Integer& operator = (const UInteger& Obj_);
    virtual Integer& operator = (const ShortInt& Obj_);
    virtual Integer& operator = (const UShortInt& Obj_);

    // Unary operators
    UInteger operator + () const;
    UInteger operator ~ () const;

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





