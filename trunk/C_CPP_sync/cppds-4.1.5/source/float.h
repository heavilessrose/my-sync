#ifndef FLOAT_H
#define FLOAT_H
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif
#ifndef RATIONAL_CPP
  #include "rational.cpp"
#endif

#define FLOAT_DEBUG		0

/****************************************************************************/
class FloatAcceptor : public ObjectAcceptor
{
  friend class Float;

  protected:
    union
    {
      const Float* _ObjPtr;
      Float* _VarPtr;
    };

    FloatAcceptor(const Float* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Float class specific acceptor methods
    virtual Boolean IsFloat() const;
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
class Float : public StdFloatingPointWrapper<Float, float>
{
  friend class FloatAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, Float)
  TYPEWRAPPER_FLOATINGPOINT_OPERATIONS(Float)
  TYPEWRAPPER_ANSIMATH_OPERATIONS(Float)

  friend Float operator + (const Float& Val1_, const Float& Val2_);
  friend Float operator - (const Float& Val1_, const Float& Val2_);
  friend Float operator * (const Float& Val1_, const Float& Val2_);
  friend Float operator / (const Float& Val1_, const Float& Val2_);
  
  friend Float operator + (long Val1_, const Float& Val2_);
  friend Float operator - (long Val1_, const Float& Val2_);
  friend Float operator * (long Val1_, const Float& Val2_);
  friend Float operator / (long Val1_, const Float& Val2_);
  
  friend Float operator + (const Float& Val1_, long Val2_);
  friend Float operator - (const Float& Val1_, long Val2_);
  friend Float operator * (const Float& Val1_, long Val2_);
  friend Float operator / (const Float& Val1_, long Val2_);

  friend Float operator + (UnsignedIntValue Val1_, const Float& Val2_);
  friend Float operator - (UnsignedIntValue Val1_, const Float& Val2_);
  friend Float operator * (UnsignedIntValue Val1_, const Float& Val2_);
  friend Float operator / (UnsignedIntValue Val1_, const Float& Val2_);
  
  friend Float operator + (const Float& Val1_, UnsignedIntValue Val2_);
  friend Float operator - (const Float& Val1_, UnsignedIntValue Val2_);
  friend Float operator * (const Float& Val1_, UnsignedIntValue Val2_);
  friend Float operator / (const Float& Val1_, UnsignedIntValue Val2_);
  
  protected:
    union
    {
      Byte_t _Valid;
      float _Value;
    };

    virtual Boolean IsEqualToFloat(const Float* Ptr_) const;
    virtual Boolean IsLesserToFloat(const Float* Ptr_) const;
    virtual Boolean IsGreaterToFloat(const Float* Ptr_) const;

  public:
    enum { TYPE_FLOAT_PRECISION = 7 };
  
    static const double vPI;
    static const double vE;
    static const double vNAN;
    static const double vINFINITY;

    Float(float FltVal_=0);
    Float(const ChrString& Str_);
    Float(const Float& FltObj_);
    virtual ~Float() {}

    virtual Float& DerivedClass();
    virtual const Float& DerivedClass() const;

    // Virtual constructor and assignment methods
    static Float* Make(float IntVal_=0);
    static Float* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    // Static null object accessor methods
    static Float& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static Boolean IsValue(const ChrString& Str_);
    static Float ValueOf(const ChrString& Str_);
    static ChrString ToString(const Float& Flt_, int Prec_=7);
    virtual ChrString ToString(int Prec_=7) const;
    static ChrString ToTruncString(const Float& Flt_, int Radix_=10);
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual Float& FromString(const ChrString& Str_);

    virtual const float& GetData() const;
    virtual void SetData(const float& Obj_);
    operator float () const
	{ return _Value; }    

    static Boolean IsNan(float FltVal_);
    static Boolean IsInfinite(float FltVal_);

    static Float Ceiling(float FltVal_);
    static Float Floor(float FltVal_);
    static Float Round(float FltVal_);

    float FloatValue() const;
    virtual long IntValue() const
	{ return long(_Value); }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::FLOAT_TYPE; }
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

    virtual Float& Ceiling();
    virtual Float& Floor();
    virtual Float& Round();

    virtual Boolean IsNan() const;
    virtual Boolean IsInfinite() const;    

    // Unary operators
    virtual Boolean AtZero() const;
    virtual Boolean operator ! () const;
    Float operator - () const;
    Float operator + () const;

    // Assignment method
    virtual Float& operator = (const float& IntVal_);
    virtual Float& operator = (const ChrString& Str_);
    virtual Float& operator = (const Float& Obj_);

    // Arithmetic assignment operators
    virtual Float& operator += (const NumRealType<Float>& Obj_);
    virtual Float& operator -= (const NumRealType<Float>& Obj_);
    virtual Float& operator *= (const NumRealType<Float>& Obj_);
    virtual Float& operator /= (const NumRealType<Float>& Obj_);

    // Arithmetic assignment operators
    virtual Float& operator += (long Val_);
    virtual Float& operator -= (long Val_);
    virtual Float& operator *= (long Val_);
    virtual Float& operator /= (long Val_);

    // Arithmetic assignment operators
    virtual Float& operator += (UnsignedIntValue Val_);
    virtual Float& operator -= (UnsignedIntValue Val_);
    virtual Float& operator *= (UnsignedIntValue Val_);
    virtual Float& operator /= (UnsignedIntValue Val_);

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





