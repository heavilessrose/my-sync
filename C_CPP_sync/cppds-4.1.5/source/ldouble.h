#ifndef LONGDOUBLE_H
#define LONGDOUBLE_H
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif
#ifndef RATIONAL_CPP
  #include "rational.cpp"
#endif

#define LONGDOUBLE_DEBUG		0

#if HAS_LONGDOUBLE_STDFNCS
#define LONGDOUBLE_ANSIMATH_OPERATIONS(Type)					               \
friend Type& _AbsoluteValue(Type& Val_)                                 \
{ Val_ = ::fabsl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _Sine(Type& Val_)                                          \
{ Val_ = ::sinl(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _Cosine(Type& Val_)                                        \
{ Val_ = ::cosl(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _Tangent(Type& Val_)                                       \
{ Val_ = ::tanl(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _ArcSine(Type& Val_)                                       \
{ Val_ = ::asinl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _ArcCosine(Type& Val_)                                     \
{ Val_ = ::acosl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _ArcTangent(Type& Val_)                                    \
{ Val_ = ::atanl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _ArcTangent2(Type& Val1_, const Type& Val2_)               \
{ Val1_ = ::atan2l(Val1_.FloatValue(), Val2_.FloatValue());             \
  return Val1_; }                                                       \
friend Type& _Exponent(Type& Base_, const Type& Exp_)                   \
{ Base_ = ::powl(Base_.FloatValue(), Exp_.FloatValue());                \
  return Base_; }                                                       \
friend Type& _HyperSine(Type& Val_)                                     \
{ Val_ = ::sinhl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _HyperCosine(Type& Val_)                                   \
{ Val_ = ::coshl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _HyperTangent(Type& Val_)                                  \
{ Val_ = ::tanhl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _NaturalExp(Type& Val_)                                    \
{ Val_ = ::expl(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _NaturalLog(Type& Val_)                                    \
{ Val_ = ::logl(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _Log10(Type& Val_)                                         \
{ Val_ = ::log10l(Val_.FloatValue());                                   \
  return Val_; }                                                        \
friend Type& _Ceiling(Type& Val_)                                       \
{ Val_ = ::ceill(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _Floor(Type& Val_)                                         \
{ Val_ = ::floorl(Val_.FloatValue());                                   \
  return Val_; }                                                        \
friend Type& _SquareRoot(Type& Val_)                                    \
{ Val_ = ::sqrtl(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _FractionPart(Type& Val_)                                  \
{ Ldouble ip;                                                           \
  Val_ = ::modfl(Val_.FloatValue(), &ip);                               \
  return Val_; }                                                        \
friend Type& _IntegerPart(Type& Val_)                                   \
{ Ldouble ip;                                                           \
  ::modfl(Val_.FloatValue(), &ip);                                      \
  Val_ = ip;                                                            \
  return Val_; }
#else
#define LONGDOUBLE_ANSIMATH_OPERATIONS(Type)                            \
TYPEWRAPPER_ANSIMATH_OPERATIONS(Type)                                   \
friend Type& _AbsoluteValue(Type& Val_)                                 \
{ Val_ = ::fabs(Val_.FloatValue());                                     \
  return Val_; }
#endif

/****************************************************************************/
class LongDoubleAcceptor : public ObjectAcceptor
{
  friend class LongDouble;

  protected:
    union
    {
      const LongDouble* _ObjPtr;
      LongDouble* _VarPtr;
    };

    LongDoubleAcceptor(const LongDouble* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // LongDouble class specific acceptor methods
    virtual Boolean IsLongDouble() const;
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
class LongDouble : public StdFloatingPointWrapper<LongDouble, Ldouble>
{
  friend class LongDoubleAcceptor;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, LongDouble)
  TYPEWRAPPER_FLOATINGPOINT_OPERATIONS_NOABS(LongDouble)
  LONGDOUBLE_ANSIMATH_OPERATIONS(LongDouble)

  friend LongDouble operator + (const LongDouble& Val1_, const LongDouble& Val2_);
  friend LongDouble operator - (const LongDouble& Val1_, const LongDouble& Val2_);
  friend LongDouble operator * (const LongDouble& Val1_, const LongDouble& Val2_);
  friend LongDouble operator / (const LongDouble& Val1_, const LongDouble& Val2_);
  
  friend LongDouble operator + (long Val1_, const LongDouble& Val2_);
  friend LongDouble operator - (long Val1_, const LongDouble& Val2_);
  friend LongDouble operator * (long Val1_, const LongDouble& Val2_);
  friend LongDouble operator / (long Val1_, const LongDouble& Val2_);
  
  friend LongDouble operator + (const LongDouble& Val1_, long Val2_);
  friend LongDouble operator - (const LongDouble& Val1_, long Val2_);
  friend LongDouble operator * (const LongDouble& Val1_, long Val2_);
  friend LongDouble operator / (const LongDouble& Val1_, long Val2_);

  friend LongDouble operator + (UnsignedIntValue Val1_, const LongDouble& Val2_);
  friend LongDouble operator - (UnsignedIntValue Val1_, const LongDouble& Val2_);
  friend LongDouble operator * (UnsignedIntValue Val1_, const LongDouble& Val2_);
  friend LongDouble operator / (UnsignedIntValue Val1_, const LongDouble& Val2_);
  
  friend LongDouble operator + (const LongDouble& Val1_, UnsignedIntValue Val2_);
  friend LongDouble operator - (const LongDouble& Val1_, UnsignedIntValue Val2_);
  friend LongDouble operator * (const LongDouble& Val1_, UnsignedIntValue Val2_);
  friend LongDouble operator / (const LongDouble& Val1_, UnsignedIntValue Val2_);

  protected:
    union
    {
      Byte_t _Valid;
      Ldouble _Value;
    };

    virtual Boolean IsEqualToLongDouble(const LongDouble* Ptr_) const;
    virtual Boolean IsLesserToLongDouble(const LongDouble* Ptr_) const;
    virtual Boolean IsGreaterToLongDouble(const LongDouble* Ptr_) const;

  public:
    enum { TYPE_LONGDOUBLE_PRECISION = 19 };
  
    static const double vPI;
    static const double vE;
    static const double vNAN;
    static const double vINFINITY;

    LongDouble(Ldouble FltVal_=0);
    LongDouble(const ChrString& Str_);
    LongDouble(const LongDouble& FltObj_);
    virtual ~LongDouble() {}

    virtual LongDouble& DerivedClass();
    virtual const LongDouble& DerivedClass() const;

    // Virtual constructor and assignment methods
    static LongDouble* Make(Ldouble IntVal_=0);
    static LongDouble* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    // Static null object accessor methods
    static LongDouble& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static Boolean IsValue(const ChrString& Str_);
    static LongDouble ValueOf(const ChrString& Str_);
    static ChrString ToString(const LongDouble& Dbl_, int Prec_=10);
    virtual ChrString ToString(int Prec_=10) const;
    static ChrString ToTruncString(const LongDouble& Dbl_, int Radix_=10);
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual LongDouble& FromString(const ChrString& Str_);

    virtual const Ldouble& GetData() const;
    virtual void SetData(const Ldouble& Obj_);
    operator Ldouble () const
	{ return _Value; }    

    static Boolean IsNan(Ldouble FltVal_);
    static Boolean IsInfinite(Ldouble FltVal_);

    static LongDouble Ceiling(Ldouble FltVal_);
    static LongDouble Floor(Ldouble FltVal_);
    static LongDouble Round(Ldouble FltVal_);

    Ldouble FloatValue() const;
    virtual long IntValue() const
	{ return long(_Value); }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::LONGDOUBLE_TYPE; }
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

    virtual Boolean IsNan() const;
    virtual Boolean IsInfinite() const;

    virtual LongDouble& Ceiling();
    virtual LongDouble& Floor();
    virtual LongDouble& Round();

    // Unary operators
    virtual Boolean AtZero() const;
    virtual Boolean operator ! () const;
    LongDouble operator - () const;
    LongDouble operator + () const;    

    // Assignment method
    virtual LongDouble& operator = (const Ldouble& Value_);
    virtual LongDouble& operator = (const ChrString& Str_);
    virtual LongDouble& operator = (const LongDouble& Obj_);

    // Arithmetic assignment operators
    virtual LongDouble& operator += (const NumRealType<LongDouble>& Obj_);
    virtual LongDouble& operator -= (const NumRealType<LongDouble>& Obj_);
    virtual LongDouble& operator *= (const NumRealType<LongDouble>& Obj_);
    virtual LongDouble& operator /= (const NumRealType<LongDouble>& Obj_);

    // Arithmetic assignment operators
    virtual LongDouble& operator += (long Val_);
    virtual LongDouble& operator -= (long Val_);
    virtual LongDouble& operator *= (long Val_);
    virtual LongDouble& operator /= (long Val_);

    // Arithmetic assignment operators
    virtual LongDouble& operator += (UnsignedIntValue Val_);
    virtual LongDouble& operator -= (UnsignedIntValue Val_);
    virtual LongDouble& operator *= (UnsignedIntValue Val_);
    virtual LongDouble& operator /= (UnsignedIntValue Val_);

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





