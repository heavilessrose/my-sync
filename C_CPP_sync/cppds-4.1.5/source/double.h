#ifndef DOUBLE_H
#define DOUBLE_H
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif
#ifndef RATIONAL_CPP
  #include "rational.cpp"
#endif

#define DOUBLE_DEBUG		0

/****************************************************************************/
class DoubleAcceptor : public ObjectAcceptor
{
  friend class Double;

  protected:
    union
    {
      const Double* _ObjPtr;
      Double* _VarPtr;
    };

    DoubleAcceptor(const Double* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Double class specific acceptor methods
    virtual Boolean IsDouble() const;
    virtual Boolean IsTypeWrapper() const;
    
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
    virtual void* CreateFromRational(const ObjectAcceptor& Obj_);
    
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
    virtual void* AssignFromRational(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToRational(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsLesserToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUInteger(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToRational(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsGreaterToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsGreaterToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToRational(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class Double : public StdFloatingPointWrapper<Double, double>
{
  friend class DoubleAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, Double)
  TYPEWRAPPER_FLOATINGPOINT_OPERATIONS(Double)
  TYPEWRAPPER_ANSIMATH_OPERATIONS(Double)

  friend Double operator + (const Double& Val1_, const Double& Val2_);
  friend Double operator - (const Double& Val1_, const Double& Val2_);
  friend Double operator * (const Double& Val1_, const Double& Val2_);
  friend Double operator / (const Double& Val1_, const Double& Val2_);
  
  friend Double operator + (long Val1_, const Double& Val2_);
  friend Double operator - (long Val1_, const Double& Val2_);
  friend Double operator * (long Val1_, const Double& Val2_);
  friend Double operator / (long Val1_, const Double& Val2_);
  
  friend Double operator + (const Double& Val1_, long Val2_);
  friend Double operator - (const Double& Val1_, long Val2_);
  friend Double operator * (const Double& Val1_, long Val2_);
  friend Double operator / (const Double& Val1_, long Val2_);

  friend Double operator + (UnsignedIntValue Val1_, const Double& Val2_);
  friend Double operator - (UnsignedIntValue Val1_, const Double& Val2_);
  friend Double operator * (UnsignedIntValue Val1_, const Double& Val2_);
  friend Double operator / (UnsignedIntValue Val1_, const Double& Val2_);
  
  friend Double operator + (const Double& Val1_, UnsignedIntValue Val2_);
  friend Double operator - (const Double& Val1_, UnsignedIntValue Val2_);
  friend Double operator * (const Double& Val1_, UnsignedIntValue Val2_);
  friend Double operator / (const Double& Val1_, UnsignedIntValue Val2_);

  protected:
    union
    {
      Byte_t _Valid;
      double _Value;
    };

    virtual Boolean IsEqualToDouble(const Double* Ptr_) const;
    virtual Boolean IsLesserToDouble(const Double* Ptr_) const;
    virtual Boolean IsGreaterToDouble(const Double* Ptr_) const;

  public:
    enum { TYPE_DOUBLE_PRECISION = 15 };
  
    static const double vPI;
    static const double vE;
    static const double vNAN;
    static const double vINFINITY;

    Double(double FltVal_=0);
    Double(const ChrString& Str_);
    Double(const Double& FltObj_);
    virtual ~Double() {}

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    virtual Double& DerivedClass();
    virtual const Double& DerivedClass() const;

    // Virtual constructor and assignment methods
    static Double* Make(double IntVal_=0);
    static Double* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Double& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static Boolean IsValue(const ChrString& Str_);
    static Double ValueOf(const ChrString& Str_);
    static ChrString ToString(const Double& Dbl_, int Prec_=10);
    virtual ChrString ToString(int Prec_=10) const;
    static ChrString ToTruncString(const Double& Dbl_, int Radix_=10);
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual Double& FromString(const ChrString& Str_);

    // Typed wrapper methods
    virtual const double& GetData() const;
    virtual void SetData(const double& Obj_);
    operator double () const
	{ return _Value; }

    static Boolean IsNan(double FltVal_);
    static Boolean IsInfinite(double FltVal_);

    static Double Ceiling(double FltVal_);
    static Double Floor(double FltVal_);
    static Double Round(double FltVal_);

    double FloatValue() const;
    virtual long IntValue() const
	{ return long(_Value); }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::DOUBLE_TYPE; }
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

    // Special value methods
    virtual Boolean IsNan() const;
    virtual Boolean IsInfinite() const;

    // Rounding methods
    virtual Double& Ceiling();
    virtual Double& Floor();
    virtual Double& Round();

    // Unary operators
    virtual Boolean AtZero() const;
    virtual Boolean operator ! () const;
    Double operator - () const;
    Double operator + () const;

    // Assignment method
    virtual Double& operator = (const double& IntVal_);
    virtual Double& operator = (const ChrString& Str_);
    virtual Double& operator = (const Double& Obj_);

    // Arithmetic assignment operators
    virtual Double& operator += (const NumRealType<Double>& Obj_);
    virtual Double& operator -= (const NumRealType<Double>& Obj_);
    virtual Double& operator *= (const NumRealType<Double>& Obj_);
    virtual Double& operator /= (const NumRealType<Double>& Obj_);

    // Arithmetic assignment operators
    virtual Double& operator += (long Val_);
    virtual Double& operator -= (long Val_);
    virtual Double& operator *= (long Val_);
    virtual Double& operator /= (long Val_);

    // Arithmetic assignment operators
    virtual Double& operator += (UnsignedIntValue Val_);
    virtual Double& operator -= (UnsignedIntValue Val_);
    virtual Double& operator *= (UnsignedIntValue Val_);
    virtual Double& operator /= (UnsignedIntValue Val_);
    
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





