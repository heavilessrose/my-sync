#ifndef USHORTINT_H
#define USHORTINT_H
#ifndef SHORTINT_H
  #include "shortint.h"
#endif

#define USHORTINT_DEBUG		0

/****************************************************************************/
class UShortIntAcceptor : public ShortIntAcceptor
{
  friend class UShortInt;

  private:
    UShortIntAcceptor(const UShortInt* Ptr_);

  public:
    // ShortInt class specific acceptor methods
    virtual Boolean IsShortInt() const;
    virtual Boolean IsUShortInt() const;

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
class UShortInt : public ShortInt
{
  friend class ShortIntAcceptor;
  friend class UShortIntAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, UShortInt)

  friend UShortInt operator + (const UShortInt& Val1_, const UShortInt& Val2_);
  friend UShortInt operator - (const UShortInt& Val1_, const UShortInt& Val2_);
  friend UShortInt operator * (const UShortInt& Val1_, const UShortInt& Val2_);
  friend UShortInt operator / (const UShortInt& Val1_, const UShortInt& Val2_);
  friend UShortInt operator % (const UShortInt& Val1_, const UShortInt& Val2_);
  friend UShortInt operator & (const UShortInt& Val1_, const UShortInt& Val2_);
  friend UShortInt operator | (const UShortInt& Val1_, const UShortInt& Val2_);
  friend UShortInt operator ^ (const UShortInt& Val1_, const UShortInt& Val2_);

  friend UShortInt operator + (Ushort Val1_, const UShortInt& Val2_);
  friend UShortInt operator - (Ushort Val1_, const UShortInt& Val2_);
  friend UShortInt operator * (Ushort Val1_, const UShortInt& Val2_);
  friend UShortInt operator / (Ushort Val1_, const UShortInt& Val2_);
  friend UShortInt operator % (Ushort Val1_, const UShortInt& Val2_);
  friend UShortInt operator & (Ushort Val1_, const UShortInt& Val2_);
  friend UShortInt operator | (Ushort Val1_, const UShortInt& Val2_);
  friend UShortInt operator ^ (Ushort Val1_, const UShortInt& Val2_);

  friend UShortInt operator + (const UShortInt& Val1_, Ushort Val2_);
  friend UShortInt operator - (const UShortInt& Val1_, Ushort Val2_);
  friend UShortInt operator * (const UShortInt& Val1_, Ushort Val2_);
  friend UShortInt operator / (const UShortInt& Val1_, Ushort Val2_);
  friend UShortInt operator % (const UShortInt& Val1_, Ushort Val2_);
  friend UShortInt operator & (const UShortInt& Val1_, Ushort Val2_);
  friend UShortInt operator | (const UShortInt& Val1_, Ushort Val2_);
  friend UShortInt operator ^ (const UShortInt& Val1_, Ushort Val2_);  

  protected:
    virtual Boolean IsLesserToShortInt(const ShortInt* Ptr_) const;
    virtual Boolean IsGreaterToShortInt(const ShortInt* Ptr_) const;

  public:
    UShortInt(Ushort IntVal_=0);
    UShortInt(const ChrString& Str_);
    UShortInt(const ShortInt& IntObj_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      UShortInt(const UShortInt& IntObj_);
    #endif

    // Virtual constructor and assignment methods
    static UShortInt* Make(Ushort IntVal_=0);
    static UShortInt* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);    
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static UShortInt& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static UShortInt UpperBound()
	{ return UShortInt(USHRT_MAX); }
    static UShortInt LowerBound()
	{ return UShortInt(0); }
    static Boolean IsValue(const ChrString& Str_);
    static UShortInt ValueOf(const ChrString& Str_);
    static ChrString ToString(const ShortInt& Int_, int Radix_=10);
    static ChrString ToTruncString(const ShortInt& Int_, int Radix_=10);    
    virtual ChrString ToString(int Radix_=10) const;    
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual ShortInt& FromString(const ChrString& Str_);

    operator Ushort () const
	{ return _Uvalue; }
    virtual double FloatValue() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::USHORTINT_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // At maximum/minimum value test functions
    virtual Boolean AtMaxValue() const;
    virtual Boolean AtMinValue() const;

    // Assignment method
    virtual ShortInt& operator = (const short& IntVal_);
    virtual ShortInt& operator = (const ChrString& Str_);
    virtual ShortInt& operator = (const ShortInt& Obj_);
    virtual ShortInt& operator = (const UShortInt& Obj_);

    // Unary operators
    UShortInt operator + () const;
    UShortInt operator ~ () const;

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





