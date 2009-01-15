#ifndef TYPEWRAPPER_H
#define TYPEWRAPPER_H
#ifndef OBJECTENVELOPE
  #include "objenvelope.h"
#endif
#ifndef OBJIMP_H
  #include "objimp.h"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef BUILTINSERIESCOMPARE_H
  #include "bltincmp.h"
#endif

#define TYPEWRAPPER_ANSIMATH_OPERATIONS(Type)                           \
friend Type& _Sine(Type& Val_)                                          \
{ Val_ = ::sin(Val_.FloatValue());                                      \
  return Val_; }                                                        \
friend Type& _Cosine(Type& Val_)                                        \
{ Val_ = ::cos(Val_.FloatValue());                                      \
  return Val_; }                                                        \
friend Type& _Tangent(Type& Val_)                                       \
{ Val_ = ::tan(Val_.FloatValue());                                      \
  return Val_; }                                                        \
friend Type& _ArcSine(Type& Val_)                                       \
{ Val_ = ::asin(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _ArcCosine(Type& Val_)                                     \
{ Val_ = ::acos(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _ArcTangent(Type& Val_)                                    \
{ Val_ = ::atan(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _ArcTangent2(Type& Val1_, const Type& Val2_)               \
{ Val1_ = ::atan2(Val1_.FloatValue(), Val2_.FloatValue());              \
  return Val1_; }                                                       \
friend Type& _Exponent(Type& Base_, const Type& Exp_)                   \
{ Base_ = ::pow(Base_.FloatValue(), Exp_.FloatValue());                 \
  return Base_; }                                                       \
friend Type& _HyperSine(Type& Val_)                                     \
{ Val_ = ::sinh(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _HyperCosine(Type& Val_)                                   \
{ Val_ = ::cosh(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _HyperTangent(Type& Val_)                                  \
{ Val_ = ::tanh(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _NaturalExp(Type& Val_)                                    \
{ Val_ = ::exp(Val_.FloatValue());                                      \
  return Val_; }                                                        \
friend Type& _NaturalLog(Type& Val_)                                    \
{ Val_ = ::log(Val_.FloatValue());                                      \
  return Val_; }                                                        \
friend Type& _Log10(Type& Val_)                                         \
{ Val_ = ::log10(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _Ceiling(Type& Val_)                                       \
{ Val_ = ::ceil(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _Floor(Type& Val_)                                         \
{ Val_ = ::floor(Val_.FloatValue());                                    \
  return Val_; }                                                        \
friend Type& _SquareRoot(Type& Val_)                                    \
{ Val_ = ::sqrt(Val_.FloatValue());                                     \
  return Val_; }                                                        \
friend Type& _FractionPart(Type& Val_)                                  \
{ double ip;                                                            \
  Val_ = ::modf(Val_.FloatValue(), &ip);                                \
  return Val_; }                                                        \
friend Type& _IntegerPart(Type& Val_)                                   \
{ double ip;                                                            \
  ::modf(Val_.FloatValue(), &ip);                                       \
  Val_ = ip;                                                            \
  return Val_; }

#define TYPEWRAPPER_FLOATINGPOINT_OPERATIONS_NOABS(Type)                \
friend Boolean _AtZero(const Type& Obj_)                                \
{ return Obj_.AtZero(); }                                               \
friend Boolean _NotZero(const Type& Obj_)                               \
{ return !Obj_.AtZero(); }                                              \
friend Type& _Minus(Type& Trg_, const Type& Src_)			               \
{ return (Trg_ -= Src_); }						                              \
friend Type& _Add(Type& Trg_, const Type& Src_)			                  \
{ return (Trg_ += Src_); }                          			            \
friend Type& _Divide(Type& Trg_, const Type& Src_)			               \
{ return (Trg_ /= Src_); }						                              \
friend Type& _Multiply(Type& Trg_, const Type& Src_)		               \
{ return (Trg_ *= Src_); }                                              \
friend Type& _Invert(Type& Trg_)					                           \
{ return (Trg_ = -Trg_); }                                              \
friend Boolean _IsGreaterThan(const Type& Trg_, const Type& Src_)	      \
{ return (Trg_ > Src_); }                                      		   \
friend Boolean _IsLessThan(const Type& Trg_, const Type& Src_)	         \
{ return (Trg_ < Src_); }                                      		   \
friend Boolean _IsEqualTo(const Type& Trg_, const Type& Src_)	         \
{ return (Trg_ == Src_); }                                      	      \
friend ostream& _WriteToOutput(ostream& os, const Type& Obj_)	         \
{ os <<Obj_; return os; }                                               \
friend istream& _ReadFromInput(istream& is, Type& Obj_)		            \
{ is >>Obj_; return is; }

#define TYPEWRAPPER_FLOATINGPOINT_OPERATIONS(Type)			               \
TYPEWRAPPER_FLOATINGPOINT_OPERATIONS_NOABS(Type)                        \
friend Type& _AbsoluteValue(Type& Val_)                                 \
{ Val_ = ::fabs(Val_.FloatValue());                                     \
  return Val_; }

#define TYPEWRAPPER_INTEGER_OPERATIONS(Type)				                  \
TYPEWRAPPER_FLOATINGPOINT_OPERATIONS_NOABS(Type)                        \
friend Type& _AbsoluteValue(Type& Val_)                                 \
{ if (WhatIs(Val_) == TypeInfo::LONGINT_TYPE ||                         \
      WhatIs(Val_) == TypeInfo::ULONGINT_TYPE)                          \
    Val_ = ::labs((long)Val_.IntValue());                               \
  else                                                                  \
    Val_ = ::abs((int)Val_.IntValue());                                 \
  return Val_; }                                                        \
friend Type& _Modulo(Type& Trg_, const Type& Src_)			               \
{ return (Trg_ %= Src_); }						                              \
friend const Type& _Increment(Type& Trg_)				                     \
{ ++Trg_;                                                               \
  return Trg_; }                         				                     \
friend const Type& _Decrement(Type& Trg_)				                     \
{ --Trg_;                                                               \
  return Trg_; }                                            	         \
friend Boolean _LogicalAnd(const Type& Trg_, const Type& Src_)	         \
{ return (Trg_ && Src_); }                                      	      \
friend Boolean _LogicalOr(const Type& Trg_, const Type& Src_)	         \
{ return (Trg_ || Src_); }                                      	      \
friend Boolean _LogicalNot(const Type& Src_)			                     \
{ return (!Src_); }                                                     \
friend Type& _BitwiseAnd(Type& Trg_, const Type& Src_)		            \
{ return (Trg_ &= Src_); }						                              \
friend Type& _BitwiseOr(Type& Trg_, const Type& Src_)		               \
{ return (Trg_ |= Src_); }                          			            \
friend Type& _BitwiseXor(Type& Trg_, const Type& Src_)		            \
{ return (Trg_ ^= Src_); }						                              \
friend Type& _BitwiseInvert(Type& Trg_)				                     \
{ return (Trg_ = ~Trg_); }                                              \
friend Type& _BitwiseLeftShift(Type& Trg_, size_t Shift_)               \
{ return (Trg_ <<= Shift_); }                                           \
friend Type& _BitwiseRightShift(Type& Trg_, size_t Shift_)              \
{ return (Trg_ >>= Shift_); }

#define LONGNUMBER_OPERATIONS(Type)				                           \
TYPEWRAPPER_FLOATINGPOINT_OPERATIONS_NOABS(Type)                        \
friend Type& _Modulo(Type& Trg_, const Type& Src_)			               \
{ return (Trg_ %= Src_); }						                              \
friend const Type& _Increment(Type& Trg_)				                     \
{ ++Trg_;                                                               \
  return Trg_; }                          				                  \
friend const Type& _Decrement(Type& Trg_)				                     \
{ --Trg_;                                                               \
  return Trg_; }                                            	         \
friend Boolean _LogicalAnd(const Type& Trg_, const Type& Src_)	         \
{ return (Trg_ && Src_); }                                      	      \
friend Boolean _LogicalOr(const Type& Trg_, const Type& Src_)	         \
{ return (Trg_ || Src_); }                                      	      \
friend Boolean _LogicalNot(const Type& Src_)			                     \
{ return (!Src_); }

/****************************************************************************/
#define TYPEWRAPPER_ANSIMATH_STUBS(Type)                                \
friend Type& _Sine(Type& Val_)                                          \
{ return Val_; }                                                        \
friend Type& _Cosine(Type& Val_)                                        \
{ return Val_; }                                                        \
friend Type& _Tangent(Type& Val_)                                       \
{ return Val_; }                                                        \
friend Type& _ArcSine(Type& Val_)                                       \
{ return Val_; }                                                        \
friend Type& _ArcCosine(Type& Val_)                                     \
{ return Val_; }                                                        \
friend Type& _ArcTangent(Type& Val_)                                    \
{ return Val_; }                                                        \
friend Type& _ArcTangent2(Type& Val1_, const Type& Val2_)               \
{ return Val1_; }                                                       \
friend Type& _Exponent(Type& Base_, const Type& Exp_)                   \
{ return Base_; }                                                       \
friend Type& _HyperSine(Type& Val_)                                     \
{ return Val_; }                                                        \
friend Type& _HyperCosine(Type& Val_)                                   \
{ return Val_; }                                                        \
friend Type& _HyperTangent(Type& Val_)                                  \
{ return Val_; }                                                        \
friend Type& _NaturalExp(Type& Val_)                                    \
{ return Val_; }                                                        \
friend Type& _NaturalLog(Type& Val_)                                    \
{ return Val_; }                                                        \
friend Type& _Log10(Type& Val_)                                         \
{ return Val_; }                                                        \
friend Type& _Ceiling(Type& Val_)                                       \
{ return Val_; }                                                        \
friend Type& _Floor(Type& Val_)                                         \
{ return Val_; }                                                        \
friend Type& _AbsoluteValue(Type& Val_)                                 \
{ return Val_; }                                                        \
friend Type& _SquareRoot(Type& Val_)                                    \
{ return Val_; }                                                        \
friend Type& _FractionPart(Type& Val_)                                  \
{ return Val_; }                                                        \
friend Type& _IntegerPart(Type& Val_)                                   \
{ return Val_; }

#define TYPEWRAPPER_FLOATINGPOINT_STUBS(Type)			                  \
friend Boolean _AtZero(const Type& Obj_)                                \
{ return Obj_.AtZero(); }                                               \
friend Boolean _NotZero(const Type& Obj_)                               \
{ return !Obj_.AtZero(); }                                              \
friend Type& _Minus(Type& Trg_, const Type& Src_)			               \
{ return Trg_; }						                                       \
friend Type& _Add(Type& Trg_, const Type& Src_)			                  \
{ return Trg_; }                          			                     \
friend Type& _Divide(Type& Trg_, const Type& Src_)			               \
{ return Trg_; }						                                       \
friend Type& _Multiply(Type& Trg_, const Type& Src_)		               \
{ return Trg_; }                                                        \
friend Type& _Invert(Type& Trg_)					                           \
{ return Trg_; }                                                        \

#define TYPEWRAPPER_INTEGER_STUBS(Type)				                     \
TYPEWRAPPER_FLOATINGPOINT_STUBS(Type)                                   \
friend Type& _Modulo(Type& Trg_, const Type& Src_)			               \
{ return Trg_; }						                                       \
friend const Type& _Increment(Type& Trg_)				                     \
{ return Trg_; }                          				                  \
friend const Type& _Decrement(Type& Trg_)				                     \
{ return Trg_; }                                            	         \
friend Boolean _LogicalAnd(const Type& Trg_, const Type& Src_)	         \
{ return FALSE; }                                      	               \
friend Boolean _LogicalOr(const Type& Trg_, const Type& Src_)	         \
{ return FALSE; }                                      	               \
friend Boolean _LogicalNot(const Type& Src_)			                     \
{ return FALSE; }                                                       \
friend Type& _BitwiseAnd(Type& Trg_, const Type& Src_)		            \
{ return Trg_; }						                                       \
friend Type& _BitwiseOr(Type& Trg_, const Type& Src_)		               \
{ return Trg_; }                          			                     \
friend Type& _BitwiseXor(Type& Trg_, const Type& Src_)		            \
{ return Trg_; }						                                       \
friend Type& _BitwiseInvert(Type& Trg_)				                     \
{ return Trg_; }                                                        \
friend Type& _BitwiseLeftShift(Type& Trg_, size_t Shift_)               \
{ return Trg_; }                                                        \
friend Type& _BitwiseRightShift(Type& Trg_, size_t Shift_)              \
{ return Trg_; }

/****************************************************************************/
class TypeWrapper
{
  friend class TypeWrapperImp;
  friend class TypeWrapperEnvelope;

  protected:
    enum
    {
      BOUNDS_CHECKING_BIT	= 0x0040,
      TERMINATE_ON_ERROR_BIT	= 0x0080,
      ALL_OPTIONS               = 0x00C0
    };

    virtual void SetUnderflow() = 0;
    virtual void SetOverflow() = 0;
    virtual void SetDivideByZero() = 0;
    virtual void SetDataConversionError() = 0;
    virtual void SetNoComplexRoots() = 0;
    virtual void SetFindRootFunctionError() = 0;    
    virtual void SetRootNotFoundError() = 0;
    
    virtual void SetExtErrorStatus(unsigned BitFlag_)            = 0;
    virtual void ClearExtErrorStatus(int ErrBits_=ALL_EXTERRORS) = 0;
    virtual Boolean IsExtStatusSet(int BitFlag_) const           = 0;
    virtual int ExtErrorStatus() const                           = 0;
    virtual unsigned GetExtErrorStatusFlag() const               = 0;
    virtual void SetExtErrorStatusFlag(unsigned Value_)          = 0;
    
    // return Object class representation
    virtual Object& _EnclosedObject() = 0;
    virtual const Object& _EnclosedObject() const = 0;

  public:
    enum
    {
      UNDERFLOW_ERROR 		= 0x0001,
      OVERFLOW_ERROR 		= 0x0002,
      DIVIDE_BY_ZERO_ERROR	= 0x0004,
      DATA_CONVERSION_ERROR     = 0x0008,
      ALL_STDERRORS		= 0x000F,

      NOCOMPLEXROOTS_ERROR      = 0x0010,
      FINDROOTFUNCTION_ERROR    = 0x0020,
      ROOTNOTFOUND_ERROR        = 0x0040,
      ALL_EXTERRORS		= 0x0070,

      ALL_ERRORS		= 0x007F
    };

    virtual Boolean IsSummableType() const                      = 0;
    virtual Boolean IsMultipliableType() const                  = 0;
    virtual Boolean IsOrdinalType() const                       = 0;
    virtual Boolean IsInvertableType() const                    = 0;
    virtual Boolean IsLogicalType() const                       = 0;
    virtual Boolean IsBitwiseableType() const                   = 0;
    virtual Boolean IsStreamableType() const                    = 0;
    virtual Boolean IsSortableType() const                      = 0;
    virtual Boolean IsMatchableType() const                     = 0;

    virtual Boolean AtZero() const                              = 0;
    virtual ChrString ToString(int = 10) const                  = 0;
    virtual ChrString ToTruncString(int = 10) const             = 0;
    virtual TypeWrapper& FromString(const ChrString& Str_)      = 0;

    virtual TypeWrapper& TerminateOnError(Boolean Flag_=TRUE)   = 0;
    virtual TypeWrapper& CheckBounds(Boolean Flag_=TRUE)        = 0;
    virtual void ClearError(int ErrBits_=ALL_ERRORS)            = 0;
    virtual void ThrowError()                                   = 0;
    virtual int ErrorStatus() const                             = 0;
    virtual int Options() const                                 = 0;

    virtual Boolean IsStatusSet(int BitFlag_) const             = 0;
    virtual Boolean IsOptionSet(int BitFlag_) const             = 0;

    inline Boolean BoundsChecking() const
        { return IsOptionSet(BOUNDS_CHECKING_BIT); }
    inline Boolean TerminateOnError() const
        { return IsOptionSet(TERMINATE_ON_ERROR_BIT); }
    
    inline Boolean Underflow() const
	{ return IsStatusSet(UNDERFLOW_ERROR); }    
    inline Boolean Overflow() const
	{ return IsStatusSet(OVERFLOW_ERROR); }
    inline Boolean DivideByZero() const
	{ return IsStatusSet(DIVIDE_BY_ZERO_ERROR); }    
    inline Boolean DataConversionError() const
        { return IsStatusSet(DATA_CONVERSION_ERROR); }
    inline Boolean NoComplexRootError() const
        { return IsStatusSet(NOCOMPLEXROOTS_ERROR); }
    inline Boolean FindRootFunctionError() const
        { return IsStatusSet(FINDROOTFUNCTION_ERROR); }
    inline Boolean RootNotFoundError() const
        { return IsStatusSet(ROOTNOTFOUND_ERROR); }
    inline Boolean NoError() const
        { return (ErrorStatus() == 0); }        
};

/****************************************************************************/
class TypeWrapperImp : public TypeWrapper, public ObjectImp
{
  ROOTOBJECTFNCS_STUB(TypeWrapperImp, TypeWrapperImp)
  OBJECTPROPERTIESFNCS_DEFN(TypeWrapperImp)
  GENERIC_MINIMAL_FNC_DEFN(TypeWrapperImp)  
  TYPEWRAPPER_ANSIMATH_STUBS(TypeWrapperImp)
  TYPEWRAPPER_INTEGER_STUBS(TypeWrapperImp)

  protected:
    Byte_t _ErrorStatus;

    virtual void SetUnderflow();
    virtual void SetOverflow();
    virtual void SetDivideByZero();
    virtual void SetDataConversionError();
    virtual void SetNoComplexRoots();
    virtual void SetFindRootFunctionError();
    virtual void SetRootNotFoundError();    

    virtual void SetExtErrorStatus(unsigned BitFlag_);
    virtual void ClearExtErrorStatus(int ErrBits_=ALL_EXTERRORS);
    virtual Boolean IsExtStatusSet(int BitFlag_) const;
    virtual int ExtErrorStatus() const;
    virtual unsigned GetExtErrorStatusFlag() const;
    virtual void SetExtErrorStatusFlag(unsigned Value_);
    
    // return Object class representation
    virtual Object& _EnclosedObject();
    virtual const Object& _EnclosedObject() const;

    TypeWrapperImp(int Options_);
    TypeWrapperImp(const TypeWrapper& Obj_);

  public:
    virtual Boolean AtZero() const                              = 0;  
    virtual ChrString ToString(int = 10) const                  = 0;
    virtual ChrString ToTruncString(int = 10) const             = 0;
    virtual TypeWrapper& FromString(const ChrString& Str_)      = 0;

    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;
  
    virtual TypeWrapper& TerminateOnError(Boolean Flag_=TRUE);
    virtual TypeWrapper& CheckBounds(Boolean Flag_=TRUE);
    virtual void ClearError(int ErrBits_=TypeWrapper::ALL_ERRORS);
    virtual void ThrowError();
    virtual int ErrorStatus() const;
    virtual int Options() const;

    virtual Boolean IsStatusSet(int BitFlag_) const;
    virtual Boolean IsOptionSet(int BitFlag_) const;

    TypeWrapperImp& operator = (const TypeWrapperImp& Obj_);
};

/****************************************************************************/
class OrdinalType : public TypeWrapperImp
{
  protected:
    OrdinalType(int Options_);
    OrdinalType(const TypeWrapper& Obj_);
  
  public:  
    // TypeWrapper class virtual methods
    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;

    // Increment and decrement operators
    virtual const OrdinalType& operator ++ ()    = 0;
    virtual const OrdinalType& operator ++ (int) = 0;
    virtual const OrdinalType& operator -- ()	 = 0;
    virtual const OrdinalType& operator -- (int) = 0;
};

/****************************************************************************/
template <class DERIVED>
class MixedNumType : public OrdinalType
{
  protected:
    unsigned _ExtErrorStatus;

    virtual unsigned GetExtErrorStatusFlag() const;
    virtual void SetExtErrorStatusFlag(unsigned Value_);
    
    MixedNumType(int Options_);
    MixedNumType(const TypeWrapper& Obj_);
  
  public:
    // TypeWrapper class virtual methods
    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;
  
    virtual DERIVED& DerivedClass() = 0;
    virtual const DERIVED& DerivedClass() const = 0;
  
    // Unary operators
    virtual Boolean operator ! () const = 0;        
  
    // Assignment methods
    virtual DERIVED& operator = (const DERIVED& Obj_) = 0;

    // Arithmetic assignment operators
    virtual MixedNumType<DERIVED>& operator += (const MixedNumType<DERIVED>& Obj_) = 0;
    virtual MixedNumType<DERIVED>& operator -= (const MixedNumType<DERIVED>& Obj_) = 0;
    virtual MixedNumType<DERIVED>& operator *= (const MixedNumType<DERIVED>& Obj_) = 0;
    virtual MixedNumType<DERIVED>& operator /= (const MixedNumType<DERIVED>& Obj_) = 0;
    virtual MixedNumType<DERIVED>& operator %= (const MixedNumType<DERIVED>& Obj_) = 0;

    // Rounding methods
    virtual MixedNumType& Ceiling()   = 0;
    virtual MixedNumType& Floor()     = 0;
    virtual MixedNumType& Round()     = 0;
};

/****************************************************************************/
template <class DERIVED>
class NumOrdinalType : public OrdinalType
{
  protected:
    NumOrdinalType(int Options_):
        OrdinalType(Options_) {}
    NumOrdinalType(const TypeWrapper& Obj_):
        OrdinalType(Obj_) {}
  
  public:
    // TypeWrapper class virtual methods
    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;
  
    virtual DERIVED& DerivedClass() = 0;
    virtual const DERIVED& DerivedClass() const = 0;
  
    // Unary operators
    virtual Boolean operator ! () const = 0;        
  
    // Assignment methods
    virtual DERIVED& operator = (const DERIVED& Obj_) = 0;

    // Arithmetic assignment operators
    virtual NumOrdinalType<DERIVED>& operator += (const NumOrdinalType<DERIVED>& Obj_) = 0;
    virtual NumOrdinalType<DERIVED>& operator -= (const NumOrdinalType<DERIVED>& Obj_) = 0;
    virtual NumOrdinalType<DERIVED>& operator *= (const NumOrdinalType<DERIVED>& Obj_) = 0;
    virtual NumOrdinalType<DERIVED>& operator /= (const NumOrdinalType<DERIVED>& Obj_) = 0;
    virtual NumOrdinalType<DERIVED>& operator %= (const NumOrdinalType<DERIVED>& Obj_) = 0;

    // At zero/maximum/minimum value test functions
    virtual Boolean AtMaxValue() const = 0;
    virtual Boolean AtMinValue() const = 0;
};

/****************************************************************************/
template <class DERIVED>
class NumOrdinalAsBitsType : public NumOrdinalType<DERIVED>
{
  protected:
    NumOrdinalAsBitsType(int Options_):
        NumOrdinalType<DERIVED>(Options_) {}
    NumOrdinalAsBitsType(const TypeWrapper& Obj_):
        NumOrdinalType<DERIVED>(Obj_) {}
  
  public:
    // TypeWrapper class virtual methods
    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;
  
    // Bitwise assignment operators
    virtual NumOrdinalAsBitsType<DERIVED>& operator |= (const NumOrdinalAsBitsType<DERIVED>& Obj_) = 0;
    virtual NumOrdinalAsBitsType<DERIVED>& operator &= (const NumOrdinalAsBitsType<DERIVED>& Obj_) = 0;
    virtual NumOrdinalAsBitsType<DERIVED>& operator ^= (const NumOrdinalAsBitsType<DERIVED>& Obj_) = 0;
    virtual NumOrdinalAsBitsType<DERIVED>& operator <<= (size_t Shift_) = 0;
    virtual NumOrdinalAsBitsType<DERIVED>& operator >>= (size_t Shift_) = 0;
};

/****************************************************************************/
class RealType : public TypeWrapperImp
{
  protected:
    RealType(int Options_);
    RealType(const TypeWrapper& Obj_);
  
  public:
    // TypeWrapper class virtual methods
    virtual Boolean IsOrdinalType() const;

    // Rounding methods
    virtual RealType& Ceiling()   = 0;
    virtual RealType& Floor()     = 0;
    virtual RealType& Round()     = 0;
};

/****************************************************************************/
template <class DERIVED>
class NumRealType : public RealType
{
  protected:
    NumRealType(int Options_):
        RealType(Options_) {}
    NumRealType(const TypeWrapper& Obj_):
        RealType(Obj_) {}
  
  public:
    // TypeWrapper class virtual methods
    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;
  
    virtual DERIVED& DerivedClass() = 0;
    virtual const DERIVED& DerivedClass() const = 0;
  
    // Unary operators
    virtual Boolean operator ! () const = 0;
  
    // Assignment methods
    virtual DERIVED& operator = (const DERIVED& Obj_) = 0;    

    // Arithmetic assignment operators
    virtual NumRealType<DERIVED>& operator += (const NumRealType<DERIVED>& Obj_) = 0;
    virtual NumRealType<DERIVED>& operator -= (const NumRealType<DERIVED>& Obj_) = 0;
    virtual NumRealType<DERIVED>& operator *= (const NumRealType<DERIVED>& Obj_) = 0;
    virtual NumRealType<DERIVED>& operator /= (const NumRealType<DERIVED>& Obj_) = 0;
};

/****************************************************************************/
template <class DERIVED>
class StdNumRealType : public NumRealType<DERIVED>
{
  protected:
    StdNumRealType(int Options_):
        NumRealType<DERIVED>(Options_) {}
    StdNumRealType(const TypeWrapper& Obj_):
        NumRealType<DERIVED>(Obj_) {}
  
  public:
    // Special value methods
    virtual Boolean IsNan() const = 0;
    virtual Boolean IsInfinite() const = 0;
};

/****************************************************************************/
template <class NESTEDTYPE>
class TypedWrapper
{
  public:
    virtual void SetData(const NESTEDTYPE& Obj_) = 0;
    virtual const NESTEDTYPE& GetData() const = 0;
    virtual operator NESTEDTYPE () const = 0;
};

/****************************************************************************/
template <class DERIVED, class NESTEDTYPE>
class MixedNumberType : public MixedNumType<DERIVED>, public TypedWrapper<NESTEDTYPE>
{
  protected:
    virtual unsigned GetExtErrorStatusFlag() const;
    virtual void SetExtErrorStatusFlag(unsigned Value_);

    MixedNumberType(int Options_):
        MixedNumType<DERIVED>(Options_) {}
    MixedNumberType(const TypeWrapper& Obj_):
        MixedNumType<DERIVED>(Obj_) {}
  
  public:  
    virtual DERIVED& operator = (const NESTEDTYPE& IntVal_) = 0;
};

/****************************************************************************/
template <class DERIVED, class NESTEDTYPE>
class IntegerType : public NumOrdinalType<DERIVED>, public TypedWrapper<NESTEDTYPE>
{
  protected:
    IntegerType(int Options_):
        NumOrdinalType<DERIVED>(Options_) {}
    IntegerType(const TypeWrapper& Obj_):
        NumOrdinalType<DERIVED>(Obj_) {}
  
  public:  
    virtual DERIVED& operator = (const NESTEDTYPE& IntVal_) = 0;
};

/****************************************************************************/
template <class DERIVED, class NESTEDTYPE>
class IntegerAsBitsType : public NumOrdinalAsBitsType<DERIVED>, public TypedWrapper<NESTEDTYPE>
{
  protected:
    IntegerAsBitsType(int Options_):
        NumOrdinalAsBitsType<DERIVED>(Options_) {}
    IntegerAsBitsType(const TypeWrapper& Obj_):
        NumOrdinalAsBitsType<DERIVED>(Obj_) {}
  
  public:  
    virtual DERIVED& operator = (const NESTEDTYPE& IntVal_) = 0;  
};

/****************************************************************************/
template <class DERIVED, class NESTEDTYPE>
class FloatingPointType : public NumRealType<DERIVED>, public TypedWrapper<NESTEDTYPE>
{
  protected:
    FloatingPointType(int Options_):
        NumRealType<DERIVED>(Options_) {}
    FloatingPointType(const TypeWrapper& Obj_):
        NumRealType<DERIVED>(Obj_) {}  
  
  public:
    virtual DERIVED& operator = (const NESTEDTYPE& IntVal_) = 0;  
};

/****************************************************************************/
template <class DERIVED, class NESTEDTYPE>
class StdFloatingPointType : public StdNumRealType<DERIVED>, public TypedWrapper<NESTEDTYPE>
{
  protected:
    StdFloatingPointType(int Options_):
        StdNumRealType<DERIVED>(Options_) {}
    StdFloatingPointType(const TypeWrapper& Obj_):
        StdNumRealType<DERIVED>(Obj_) {}
  
  public:  
    virtual DERIVED& operator = (const NESTEDTYPE& IntVal_) = 0;
};

/****************************************************************************/
struct UnsignedIntValue
{
  Ulong _Value;
  UnsignedIntValue(Ulong Val_):
      _Value(Val_) {}
};

template <class DERIVED, class NESTEDTYPE>
class StdFloatingPointWrapper : public StdFloatingPointType<DERIVED, NESTEDTYPE>
{
  protected:
    StdFloatingPointWrapper(int Options_):
        StdFloatingPointType<DERIVED, NESTEDTYPE>(Options_) {}
    StdFloatingPointWrapper(const TypeWrapper& Obj_):
        StdFloatingPointType<DERIVED, NESTEDTYPE>(Obj_) {}

  public:
    // Arithmetic assignment operators
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator += (long Val_) = 0;
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator -= (long Val_) = 0;
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator *= (long Val_) = 0;
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator /= (long Val_) = 0;

    // Arithmetic assignment operators
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator += (UnsignedIntValue Val_) = 0;
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator -= (UnsignedIntValue Val_) = 0;
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator *= (UnsignedIntValue Val_) = 0;
    virtual StdFloatingPointWrapper<DERIVED, NESTEDTYPE>& operator /= (UnsignedIntValue Val_) = 0;    
};

/****************************************************************************/
/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsSummableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsMultipliableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsOrdinalType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsInvertableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsLogicalType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsBitwiseableType() const
{
  return FALSE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean MixedNumType<DERIVED>::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsSummableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsMultipliableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsOrdinalType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsInvertableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsLogicalType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsBitwiseableType() const
{
  return FALSE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalType<DERIVED>::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsSummableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsMultipliableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsOrdinalType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsInvertableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsLogicalType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsBitwiseableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumOrdinalAsBitsType<DERIVED>::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsSummableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsMultipliableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsOrdinalType() const
{
  return FALSE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsInvertableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsLogicalType() const
{
  return FALSE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsBitwiseableType() const
{
  return FALSE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
template <class DERIVED>
Boolean NumRealType<DERIVED>::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
#endif





