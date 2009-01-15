#ifndef ULONGINT_H
#define ULONGINT_H
#ifndef LONGINT_H
  #include "longint.h"
#endif

#define ULONGINT_DEBUG		0

/****************************************************************************/
class ULongIntAcceptor : public LongIntAcceptor
{
  friend class ULongInt;

  private:
    ULongIntAcceptor(const ULongInt* Ptr_);

  public:
    // LongInt class specific acceptor methods
    virtual Boolean IsLongInt() const;
    virtual Boolean IsULongInt() const;    

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
class ULongInt : public LongInt
{
  friend class LongIntAcceptor;
  friend class ULongIntAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, ULongInt)        

  friend ULongInt operator + (const ULongInt& Val1_, const ULongInt& Val2_);
  friend ULongInt operator - (const ULongInt& Val1_, const ULongInt& Val2_);
  friend ULongInt operator * (const ULongInt& Val1_, const ULongInt& Val2_);
  friend ULongInt operator / (const ULongInt& Val1_, const ULongInt& Val2_);
  friend ULongInt operator % (const ULongInt& Val1_, const ULongInt& Val2_);
  friend ULongInt operator & (const ULongInt& Val1_, const ULongInt& Val2_);
  friend ULongInt operator | (const ULongInt& Val1_, const ULongInt& Val2_);
  friend ULongInt operator ^ (const ULongInt& Val1_, const ULongInt& Val2_);

  friend ULongInt operator + (Ulong Val1_, const ULongInt& Val2_);
  friend ULongInt operator - (Ulong Val1_, const ULongInt& Val2_);
  friend ULongInt operator * (Ulong Val1_, const ULongInt& Val2_);
  friend ULongInt operator / (Ulong Val1_, const ULongInt& Val2_);
  friend ULongInt operator % (Ulong Val1_, const ULongInt& Val2_);
  friend ULongInt operator & (Ulong Val1_, const ULongInt& Val2_);
  friend ULongInt operator | (Ulong Val1_, const ULongInt& Val2_);
  friend ULongInt operator ^ (Ulong Val1_, const ULongInt& Val2_);

  friend ULongInt operator + (const ULongInt& Val1_, Ulong Val2_);
  friend ULongInt operator - (const ULongInt& Val1_, Ulong Val2_);
  friend ULongInt operator * (const ULongInt& Val1_, Ulong Val2_);
  friend ULongInt operator / (const ULongInt& Val1_, Ulong Val2_);
  friend ULongInt operator % (const ULongInt& Val1_, Ulong Val2_);
  friend ULongInt operator & (const ULongInt& Val1_, Ulong Val2_);
  friend ULongInt operator | (const ULongInt& Val1_, Ulong Val2_);
  friend ULongInt operator ^ (const ULongInt& Val1_, Ulong Val2_);  

  protected:
    virtual Boolean IsLesserToLongInt(const LongInt* Ptr_) const;
    virtual Boolean IsGreaterToLongInt(const LongInt* Ptr_) const;

  public:
    ULongInt(Ulong IntVal_=0);
    ULongInt(const ChrString& Str_);
    ULongInt(const ShortInt& IntObj_);
    ULongInt(const LongInt& IntObj_);
    ULongInt(const Integer& IntObj_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      ULongInt(const UShortInt& IntObj_);
      ULongInt(const ULongInt& IntObj_);
      ULongInt(const UInteger& IntObj_);
    #endif

    // Virtual constructor and assignment methods
    static ULongInt* Make(Ulong IntVal_=0);
    static ULongInt* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);    
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static ULongInt& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static ULongInt UpperBound()
	{ return ULongInt(ULONG_MAX); }
    static ULongInt LowerBound()
	{ return ULongInt(0); }
    static Boolean IsValue(const ChrString& Str_);
    static ULongInt ValueOf(const ChrString& Str_);
    static ChrString ToString(const LongInt& Int_, int Radix_=10);
    virtual ChrString ToString(int Radix_=10) const;
    static ChrString ToTruncString(const LongInt& Int_, int Radix_=10);
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual LongInt& FromString(const ChrString& Str_);

    operator Ulong () const
	{ return _Uvalue; }
    virtual double FloatValue() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::ULONGINT_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Text input and output
    istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // At maximum/minimum value test functions
    virtual Boolean AtMaxValue() const;
    virtual Boolean AtMinValue() const;    

    // Assignment method
    virtual LongInt& operator = (const long& IntVal_);
    virtual LongInt& operator = (const ChrString& Str_);
    virtual LongInt& operator = (const ShortInt& Obj_);
    virtual LongInt& operator = (const UShortInt& Obj_);
    virtual LongInt& operator = (const Integer& Obj_);
    virtual LongInt& operator = (const UInteger& Obj_);
    virtual LongInt& operator = (const LongInt& Obj_);
    virtual LongInt& operator = (const ULongInt& Obj_);

    // Unary operators
    ULongInt operator + () const;
    ULongInt operator ~ () const;

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





