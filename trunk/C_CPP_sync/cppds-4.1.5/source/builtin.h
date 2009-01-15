#ifndef BUILTIN_H
#define BUILTIN_H
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

#define BUILTIN_DEBUG	0

/****************************************************************************/
// Memory Allocation Procedures
unsigned char* new_unsigned_char(unsigned char Val_);
unsigned* new_unsigned_int(unsigned Val_);
unsigned short* new_unsigned_short(unsigned short Val_);
unsigned long* new_unsigned_long(unsigned long Val_);

#if HAS_BOOL
bool* new_bool(bool Val_);
#endif
char* new_char(char Val_);
int* new_int(int Val_);
short* new_short(short Val_);
long* new_long(long Val_);

float* new_float(float Val_);
double* new_double(double Val_);
long double* new_long_double(long double Val_);

unsigned char* new_unsigned_chars(ptrdiff_t Max_, ...);
unsigned* new_unsigned_ints(ptrdiff_t Max_, ...);
unsigned short* new_unsigned_shorts(ptrdiff_t Max_, ...);
unsigned long* new_unsigned_longs(ptrdiff_t Max_, ...);

#if HAS_BOOL
bool* new_bools(ptrdiff_t Max_, ...);
#endif
char* new_chars(ptrdiff_t Max_, ...);
int* new_ints(ptrdiff_t Max_, ...);
short* new_shorts(ptrdiff_t Max, ...);
long* new_longs(ptrdiff_t Max_, ...);

float* new_floats(ptrdiff_t Max_, float* Args_=NULL, MemAllocMethod Method_=MEMMATRIX);
double* new_doubles(ptrdiff_t Max_, double* Args_=NULL, MemAllocMethod Method_=MEMMATRIX);
long double* new_long_doubles(ptrdiff_t Max_, long double* Args_=NULL, MemAllocMethod Method_=MEMMATRIX);

unsigned char* new_unsigned_char_string(const unsigned char* Arr_, MemAllocMethod Method_=MEMMATRIX);
unsigned char* new_unsigned_char_array(ptrdiff_t Max_, const unsigned char* Arr_, MemAllocMethod Method_=MEMMATRIX);
unsigned* new_unsigned_int_array(ptrdiff_t Max_, const unsigned* Arr_, MemAllocMethod Method_=MEMMATRIX);
unsigned short* new_unsigned_short_array(ptrdiff_t Max_, const unsigned short* Arr_, MemAllocMethod Method_=MEMMATRIX);
unsigned long* new_unsigned_long_array(ptrdiff_t Max_, const unsigned long* Arr_, MemAllocMethod Method_=MEMMATRIX);

char* new_char_string(const char* Arr_, MemAllocMethod Method_=MEMMATRIX);
char* new_char_array(ptrdiff_t Max_, const char* Arr_, MemAllocMethod Method_=MEMMATRIX);
int* new_int_array(ptrdiff_t Max_, const int* Arr_, MemAllocMethod Method_=MEMMATRIX);
short* new_short_array(ptrdiff_t Max_, const short* Arr_, MemAllocMethod Method_=MEMMATRIX);
long* new_long_array(ptrdiff_t Max_, const long* Arr_, MemAllocMethod Method_=MEMMATRIX);
#if HAS_BOOL
bool* new_bool_array(ptrdiff_t Max_, const bool* Arr_, MemAllocMethod Method_=MEMMATRIX);
#endif

struct BuiltIn
{
  static void* MakeObject(int TypeId_, void* Ptr_=NULL);
  static void* MakeArray(int TypeId_, ptrdiff_t Qty_, void* Ptr_=NULL);
};

/****************************************************************************/
template <class T>
BOOLTYPE Delete(T* Ptr_)
{
#if OVERLOAD_NEW
  BOOLTYPE Result_ = Ptr_ != NULL;
  if (Ptr_)
    if (IsBuiltInType(Ptr_))
      RawDelete(Ptr_);
    else if (IsObjectType(Ptr_))
      delete Ptr_;
    else
    {
      BOOLTYPE Placed_;
      Result_ = MemMatrix::Matrix().HasThis(Ptr_, Placed_);

      if (Result_)
        if (IsExternalType(Ptr_))
        {
          if (Placed_)
  	       {
	         Ptr_->T::~T();
	         RawDelete(Ptr_);
          }
	       else
	         delete Ptr_;
        }
        else
          RawDelete(Ptr_);
    }

  return Result_;
#else
  delete Ptr_;
  return TRUE;
#endif
}

_FNC_INLINE BOOLTYPE Delete(void* Ptr_)
{
#if OVERLOAD_NEW
  return RawDelete(Ptr_);
#else
  delete Ptr_;
  return TRUE;
#endif
}

_FNC_INLINE BOOLTYPE Delete(void** Ptr_)
{
#if OVERLOAD_NEW
  return RawDelete(Ptr_);
#else
  delete Ptr_;
  return TRUE;
#endif
}

/****************************************************************************/
template <class T>
BOOLTYPE DeleteArray(T* Ptr_)
{
#if OVERLOAD_NEW
  BOOLTYPE Result_ = Ptr_ != NULL;
  if (Ptr_)
    if (IsBuiltInType(Ptr_))
      RawDeleteArray(Ptr_);
    else if (IsObjectType(Ptr_))
      delete[] Ptr_;
    else
    {
      BOOLTYPE Placed_;
      Result_ = MemMatrix::Matrix().HasThis(Ptr_, Placed_);

      if (Result_)
        if (IsExternalType(Ptr_))
        {      
	  if (Placed_)
	  {
	    Ptr_->T::~T();
	    RawDeleteArray(Ptr_);
	  }
	  else
  	    delete[] Ptr_;
        }
        else
          RawDeleteArray(Ptr_);
    }

  return Result_;
#else
  delete[] Ptr_;
  return TRUE;
#endif
}

_FNC_INLINE BOOLTYPE DeleteArray(void* Ptr_)
{
#if OVERLOAD_NEW
  return RawDeleteArray(Ptr_);
#else
  delete[] Ptr_;
  return TRUE;
#endif
}

_FNC_INLINE BOOLTYPE DeleteArray(void** Ptr_)
{
#if OVERLOAD_NEW
  return RawDeleteArray(Ptr_);
#else
  delete[] Ptr_;
  return TRUE;
#endif
}

/****************************************************************************/
template <class T>
class New
{
  private:
    T* _Ptr;

    static T* AllocBuiltIn(const T& Obj_, size_t Qty_)
    {
      return ((Qty_ > 1) ?
		(T*)BuiltIn::MakeArray(WhatIs(Obj_), Qty_):
		(T*)BuiltIn::MakeObject(WhatIs(Obj_)));
    }

    static T* Allocate(MemAllocMethod Method_, size_t Qty_)
    {
#     if OVERLOAD_NEW
	T* Ptr_ = NULL;
	T Temp_;

	if (Method_ == MEMMATRIX && Qty_)
	  if (IsBuiltInTypeImpl(Temp_))
	    return AllocBuiltIn(Temp_, Qty_);
	  else
          {
            int TypeCode_ = WhatIsPtr(Ptr_);
            
            if (TypeCode_ == TypeInfo::DOUBLE_VOID_POINTER)
              return ((T*)MemMatrix::Matrix().Allocate(sizeof(void*) * Qty_));
            else if (TypeCode_ == TypeInfo::TRIPLE_VOID_POINTER)
              return ((T*)MemMatrix::Matrix().Allocate(sizeof(void**) * Qty_));
            else
  	      return ((Qty_ > 1) ? (new T[Qty_]):(new T));
          }
#     endif

      return (Qty_ ? ((Qty_ > 1) ? (new T[Qty_]):(new T)):NULL);
    }

  public:
#   if OVERLOAD_NEW
      New(MemAllocMethod Method_=MEMMATRIX):
	_Ptr(New<T>::Allocate(Method_, 1)) {}
      New(ptrdiff_t Len_, MemAllocMethod Method_=MEMMATRIX):
	_Ptr(New<T>::Allocate(Method_, Len_)) {}
#   else
      New(MemAllocMethod Method_=STD_NEW):
	_Ptr(New<T>::Allocate(Method_, 1)) {}
      New(ptrdiff_t Len_, MemAllocMethod Method_=STD_NEW):
	_Ptr(New<T>::Allocate(Method_, Len_)) {}
#   endif

    operator T* ()
	{ return _Ptr; }
};

/****************************************************************************/
// Risky to use -- might not be entirely portable, might crash and burn.
template <class T>
T* ReallocateWith(MemAllocMethod Method_, T* Address_, size_t Instances_)
{
  if (Instances_)
  {
    T* Temp_ = New<T>(Instances_, Method_);

    if (Temp_)
    {
      size_t TypeSz_ = sizeof(Temp_) / Instances_;
      size_t OldInsts_ = sizeof(Address_) / TypeSz_;
      size_t Index_;

      if (OldInsts_ < Instances_)
	for (Index_ = 0; Index_ < OldInsts_; ++Index_)
	  Temp_[Index_] = Address_[Index_];
      else
	for (Index_ = 0; Index_ < Instances_; ++Index_)
	  Temp_[Index_] = Address_[Index_];
    }

    ::Delete(Address_);
    Address_ = Temp_;
  }

  return Address_;
}

// Template specializations for built in types
/****************************************************************************/
#define BUILTIN_GENERIC_FNC_DEFN(Type)          \
SIZEOFDATAFNC_INLINE_DEFN(Type)                 \
FORMWIDTHOFOBJECTFNC_INLINE_DEFN(Type)          \
COUNTOFFNC_INLINE_DEFN(Type)                    \
ISNULLOBJECTFNC_INLINE_DEFN(Type)               \
SHOWINFOFNC_INLINE_DEFN(Type)                   \
HASHINGFNCS_INLINE_DEFN(Type)

#if HAS_BOOL
BUILTIN_GENERIC_FNC_DEFN(bool)
#endif
BUILTIN_GENERIC_FNC_DEFN(char)
BUILTIN_GENERIC_FNC_DEFN(unsigned char)
BUILTIN_GENERIC_FNC_DEFN(int)
BUILTIN_GENERIC_FNC_DEFN(unsigned int)

BUILTIN_GENERIC_FNC_DEFN(short)
BUILTIN_GENERIC_FNC_DEFN(unsigned short)
BUILTIN_GENERIC_FNC_DEFN(long)
BUILTIN_GENERIC_FNC_DEFN(unsigned long)

BUILTIN_GENERIC_FNC_DEFN(float)
BUILTIN_GENERIC_FNC_DEFN(double)
BUILTIN_GENERIC_FNC_DEFN(long double)

/****************************************************************************/
#define ANSIMATH_OPERATIONS(Type)                                       \
_FNC_INLINE Type& _Sine(Type& Val_)                                     \
{ Val_ = ::sin(Val_);                                                   \
  return Val_; }                                                        \
_FNC_INLINE Type& _Cosine(Type& Val_)                                   \
{ Val_ = ::cos(Val_);                                                   \
  return Val_; }                                                        \
_FNC_INLINE Type& _Tangent(Type& Val_)                                  \
{ Val_ = ::tan(Val_);                                                   \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcSine(Type& Val_)                                  \
{ Val_ = ::asin(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcCosine(Type& Val_)                                \
{ Val_ = ::acos(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcTangent(Type& Val_)                               \
{ Val_ = ::atan(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcTangent2(Type& Val1_, const Type& Val2_)          \
{ Val1_ = ::atan2(Val1_, Val2_);                                        \
  return Val1_; }                                                       \
_FNC_INLINE Type& _Exponent(Type& Base_, const Type& Exp_)              \
{ Base_ = ::pow(Base_, Exp_);                                           \
  return Base_; }                                                       \
_FNC_INLINE Type& _HyperSine(Type& Val_)                                \
{ Val_ = ::sinh(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _HyperCosine(Type& Val_)                              \
{ Val_ = ::cosh(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _HyperTangent(Type& Val_)                             \
{ Val_ = ::tanh(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _NaturalExp(Type& Val_)                               \
{ Val_ = ::exp(Val_);                                                   \
  return Val_; }                                                        \
_FNC_INLINE Type& _NaturalLog(Type& Val_)                               \
{ Val_ = ::log(Val_);                                                   \
  return Val_; }                                                        \
_FNC_INLINE Type& _Log10(Type& Val_)                                    \
{ Val_ = ::log10(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _Ceiling(Type& Val_)                                  \
{ Val_ = ::ceil(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _Floor(Type& Val_)                                    \
{ Val_ = ::floor(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _AbsoluteValue(Type& Val_)                            \
{ if (IsFloatTypeImpl(Val_))                                            \
    Val_ = ::fabs((double)Val_);                                        \
  else if (WhatIs(Val_) == TypeInfo::BUILTIN_LONG ||                    \
           WhatIs(Val_) == TypeInfo::BUILTIN_UNSIGNED_LONG)             \
    Val_ = ::labs((long)Val_);                                          \
  else                                                                  \
    Val_ = ::abs((int)Val_);                                            \
  return Val_; }                                                        \
_FNC_INLINE Type& _SquareRoot(Type& Val_)                               \
{ Val_ = ::sqrt(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _FractionPart(Type& Val_)                             \
{ double ip;                                                            \
  Val_ = ::modf(Val_, &ip);                                             \
  return Val_; }                                                        \
_FNC_INLINE Type& _IntegerPart(Type& Val_)                              \
{ double ip;                                                            \
  ::modf(Val_, &ip);                                                    \
  Val_ = ip;                                                            \
  return Val_; }

#if HAS_LONGDOUBLE_STDFNCS
#define STDLONGDOUBLE_ANSIMATH_OPERATIONS(Type)                         \
_FNC_INLINE Type& _Sine(Type& Val_)                                     \
{ Val_ = ::sinl(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _Cosine(Type& Val_)                                   \
{ Val_ = ::cosl(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _Tangent(Type& Val_)                                  \
{ Val_ = ::tanl(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcSine(Type& Val_)                                  \
{ Val_ = ::asinl(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcCosine(Type& Val_)                                \
{ Val_ = ::acosl(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcTangent(Type& Val_)                               \
{ Val_ = ::atanl(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _ArcTangent2(Type& Val1_, const Type& Val2_)          \
{ Val1_ = ::atan2l(Val1_, Val2_);                                       \
  return Val1_; }                                                       \
_FNC_INLINE Type& _Exponent(Type& Base_, const Type& Exp_)              \
{ Base_ = ::powl(Base_, Exp_);                                          \
  return Base_; }                                                       \
_FNC_INLINE Type& _HyperSine(Type& Val_)                                \
{ Val_ = ::sinhl(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _HyperCosine(Type& Val_)                              \
{ Val_ = ::coshl(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _HyperTangent(Type& Val_)                             \
{ Val_ = ::tanhl(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _NaturalExp(Type& Val_)                               \
{ Val_ = ::expl(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _NaturalLog(Type& Val_)                               \
{ Val_ = ::logl(Val_);                                                  \
  return Val_; }                                                        \
_FNC_INLINE Type& _Log10(Type& Val_)                                    \
{ Val_ = ::log10l(Val_);                                                \
  return Val_; }                                                        \
_FNC_INLINE Type& _Ceiling(Type& Val_)                                  \
{ Val_ = ::ceill(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _Floor(Type& Val_)                                    \
{ Val_ = ::floorl(Val_);                                                \
  return Val_; }                                                        \
_FNC_INLINE Type& _AbsoluteValue(Type& Val_)                            \
{ if (IsFloatTypeImpl(Val_))                                            \
    Val_ = ::fabsl((Ldouble)Val_);                                      \
  else if (WhatIs(Val_) == TypeInfo::BUILTIN_LONG ||                    \
           WhatIs(Val_) == TypeInfo::BUILTIN_UNSIGNED_LONG)             \
    Val_ = ::labs((long)Val_);                                          \
  else                                                                  \
    Val_ = ::abs((int)Val_);                                            \
  return Val_; }                                                        \
_FNC_INLINE Type& _SquareRoot(Type& Val_)                               \
{ Val_ = ::sqrtl(Val_);                                                 \
  return Val_; }                                                        \
_FNC_INLINE Type& _FractionPart(Type& Val_)                             \
{ Ldouble ip;                                                            \
  Val_ = ::modfl(Val_, &ip);                                            \
  return Val_; }                                                        \
_FNC_INLINE Type& _IntegerPart(Type& Val_)                              \
{ Ldouble ip;                                                            \
  ::modfl(Val_, &ip);                                                   \
  Val_ = ip;                                                            \
  return Val_; }
#else
#define STDLONGDOUBLE_ANSIMATH_OPERATIONS(Type)                         \
ANSIMATH_OPERATIONS(Type)
#endif

#define FLOATINGPOINT_OPERATIONS(Type)					                     \
_FNC_INLINE Boolean _IsSummable(const Type& Obj_)			               \
{ return TRUE; }                                        		            \
_FNC_INLINE Boolean _IsMultipliable(const Type& Obj_)			            \
{ return TRUE; }                                        		            \
_FNC_INLINE Boolean _IsInvertable(const Type& Obj_)      		         \
{ return TRUE; }                                        		            \
_FNC_INLINE Boolean _IsStreamable(const Type& Obj_)      		         \
{ return TRUE; }                                        		            \
_FNC_INLINE Boolean _IsSortable(const Type& Obj_)      			         \
{ return TRUE; }                                        		            \
_FNC_INLINE Boolean _IsMatchable(const Type& Obj_)                      \
{ return TRUE; }                                                        \
_FNC_INLINE Boolean _AtZero(const Type& Trg_)				               \
{ return (Trg_ == ((Type)0)); }                                         \
_FNC_INLINE Boolean _NotZero(const Type& Trg_)				               \
{ return (Trg_ != ((Type)0)); }                                         \
_FNC_INLINE Type& _Minus(Type& Trg_, const Type& Src_)			         \
{ return (Trg_ -= Src_); }						                              \
_FNC_INLINE Type& _Add(Type& Trg_, const Type& Src_)			            \
{ return (Trg_ += Src_); }                          			            \
_FNC_INLINE Type& _Divide(Type& Trg_, const Type& Src_)			         \
{ return (Trg_ /= Src_); }						                              \
_FNC_INLINE Type& _Multiply(Type& Trg_, const Type& Src_)		         \
{ return (Trg_ *= Src_); }                                              \
_FNC_INLINE Type& _Invert(Type& Trg_)					                     \
{ return (Trg_ = -Trg_); }                                              \
_FNC_INLINE Boolean _IsGreaterThan(const Type& Trg_, const Type& Src_)	\
{ return (Trg_ > Src_); }                                      		   \
_FNC_INLINE Boolean _IsLessThan(const Type& Trg_, const Type& Src_)	   \
{ return (Trg_ < Src_); }                                      		   \
_FNC_INLINE Boolean _IsEqualTo(const Type& Trg_, const Type& Src_)	   \
{ return (Trg_ == Src_); }                                      	      \
_FNC_INLINE ostream& _WriteToOutput(ostream& os, const Type& Obj_)	   \
{ os <<Obj_; return os; }                                               \
_FNC_INLINE istream& _ReadFromInput(istream& is, Type& Obj_)		      \
{ is >>Obj_; return is; }

#define INTEGER_OPERATIONS(Type)				  	                           \
FLOATINGPOINT_OPERATIONS(Type)                                          \
_FNC_INLINE Boolean _IsOrdinal(const Type& Obj_)			               \
{ return TRUE; }                                                        \
_FNC_INLINE Boolean _IsLogical(const Type& Obj_)                        \
{ return TRUE; }                                                        \
_FNC_INLINE Boolean _IsBitwiseable(const Type& Obj_)                    \
{ return TRUE; }                                                        \
_FNC_INLINE Type& _Modulo(Type& Trg_, const Type& Src_)			         \
{ return (Trg_ %= Src_); }						                              \
_FNC_INLINE const Type& _Increment(Type& Trg_)				               \
{ ++Trg_;                                                               \
  return Trg_; }                          				                  \
_FNC_INLINE const Type& _Decrement(Type& Trg_)				               \
{ --Trg_;                                                               \
  return Trg_; }                                            	         \
_FNC_INLINE Boolean _LogicalAnd(const Type& Trg_, const Type& Src_)	   \
{ return (Trg_ && Src_); }                                      	      \
_FNC_INLINE Boolean _LogicalOr(const Type& Trg_, const Type& Src_)	   \
{ return (Trg_ || Src_); }                                      	      \
_FNC_INLINE Boolean _LogicalNot(const Type& Src_)			               \
{ return (!Src_); }                                      		         \
_FNC_INLINE Type& _BitwiseAnd(Type& Trg_, const Type& Src_)		         \
{ return (Trg_ &= Src_); }						                              \
_FNC_INLINE Type& _BitwiseOr(Type& Trg_, const Type& Src_)		         \
{ return (Trg_ |= Src_); }                          			            \
_FNC_INLINE Type& _BitwiseXor(Type& Trg_, const Type& Src_)		         \
{ return (Trg_ ^= Src_); }						                              \
_FNC_INLINE Type& _BitwiseInvert(Type& Trg_)				                  \
{ return (Trg_ = ~Trg_); }                                              \
_FNC_INLINE Type& _BitwiseLeftShift(Type& Trg_, size_t Shift_)          \
{ return (Trg_ <<= Shift_); }                                           \
_FNC_INLINE Type& _BitwiseRightShift(Type& Trg_, size_t Shift_)         \
{ return (Trg_ >>= Shift_); }

INTEGER_OPERATIONS(char)
INTEGER_OPERATIONS(unsigned char)
INTEGER_OPERATIONS(int)
INTEGER_OPERATIONS(unsigned int)

INTEGER_OPERATIONS(short)
INTEGER_OPERATIONS(unsigned short)
INTEGER_OPERATIONS(long)
INTEGER_OPERATIONS(unsigned long)

FLOATINGPOINT_OPERATIONS(float)
FLOATINGPOINT_OPERATIONS(double)
FLOATINGPOINT_OPERATIONS(long double)

ANSIMATH_OPERATIONS(float)
ANSIMATH_OPERATIONS(double)
STDLONGDOUBLE_ANSIMATH_OPERATIONS(long double)

#if HAS_BOOL
_FNC_INLINE Boolean _IsLogical(const bool& Obj_)
{ return TRUE; }
_FNC_INLINE Boolean _LogicalAnd(const bool& Trg_, const bool& Src_)
{ return (Trg_ && Src_); }
_FNC_INLINE Boolean _LogicalOr(const bool& Trg_, const bool& Src_)
{ return (Trg_ || Src_); }
_FNC_INLINE Boolean _LogicalNot(const bool& Src_)
{ return (!Src_); }
#endif

/****************************************************************************/
#define BUILTIN_SWAP_FNC_DEFN(Type)		                 \
_FNC_INLINE void _Swap(Type& Obj1_, Type& Obj2_)        \
{ Type Temp_ = Obj1_;                                   \
  Obj1_ = Obj2_;                                        \
  Obj2_ = Temp_; }

#if HAS_BOOL
BUILTIN_SWAP_FNC_DEFN(bool)
#endif
BUILTIN_SWAP_FNC_DEFN(char)
BUILTIN_SWAP_FNC_DEFN(unsigned char)
BUILTIN_SWAP_FNC_DEFN(int)
BUILTIN_SWAP_FNC_DEFN(unsigned int)

BUILTIN_SWAP_FNC_DEFN(short)
BUILTIN_SWAP_FNC_DEFN(unsigned short)
BUILTIN_SWAP_FNC_DEFN(long)
BUILTIN_SWAP_FNC_DEFN(unsigned long)

BUILTIN_SWAP_FNC_DEFN(float)
BUILTIN_SWAP_FNC_DEFN(double)
BUILTIN_SWAP_FNC_DEFN(long double)

/****************************************************************************/
#define BUILTIN_CLONE_FNC_DEFN(Type, Id)			                \
_FNC_INLINE Type* _Clone(const Type& Obj_)			             \
{ Type Temp(Obj_);                                              \
  return ((Type*)BuiltIn::MakeObject(Id, &Temp)); }

#if HAS_BOOL
BUILTIN_CLONE_FNC_DEFN(bool, TypeInfo::BUILTIN_BOOL)
#endif
BUILTIN_CLONE_FNC_DEFN(char, TypeInfo::BUILTIN_CHAR)
BUILTIN_CLONE_FNC_DEFN(unsigned char, TypeInfo::BUILTIN_UNSIGNED_CHAR)
BUILTIN_CLONE_FNC_DEFN(int, TypeInfo::BUILTIN_INT)
BUILTIN_CLONE_FNC_DEFN(unsigned int, TypeInfo::BUILTIN_UNSIGNED_INT)

BUILTIN_CLONE_FNC_DEFN(short, TypeInfo::BUILTIN_SHORT)
BUILTIN_CLONE_FNC_DEFN(unsigned short, TypeInfo::BUILTIN_UNSIGNED_SHORT)
BUILTIN_CLONE_FNC_DEFN(long, TypeInfo::BUILTIN_LONG)
BUILTIN_CLONE_FNC_DEFN(unsigned long, TypeInfo::BUILTIN_UNSIGNED_LONG)

BUILTIN_CLONE_FNC_DEFN(float, TypeInfo::BUILTIN_FLOAT)
BUILTIN_CLONE_FNC_DEFN(double, TypeInfo::BUILTIN_DOUBLE)
BUILTIN_CLONE_FNC_DEFN(long double, TypeInfo::BUILTIN_LONG_DOUBLE)

/****************************************************************************/
// Needed only for Borland and Turbo C++ compilers
#if defined(__TURBOC__) | defined(__BORLANDC__)

#define BUILTIN_ASSIGN_FNC_DEFN(Type)		        	             \
_FNC_INLINE Type& _Assign(Type& Obj1_, const Type& Obj2_)       \
{ Obj1_ = Obj2_;                                                \
  return Obj1_; }

#if HAS_BOOL
BUILTIN_ASSIGN_FNC_DEFN(bool)
#endif
BUILTIN_ASSIGN_FNC_DEFN(char)
BUILTIN_ASSIGN_FNC_DEFN(unsigned char)
BUILTIN_ASSIGN_FNC_DEFN(int)
BUILTIN_ASSIGN_FNC_DEFN(unsigned int)

BUILTIN_ASSIGN_FNC_DEFN(short)
BUILTIN_ASSIGN_FNC_DEFN(unsigned short)
BUILTIN_ASSIGN_FNC_DEFN(long)
BUILTIN_ASSIGN_FNC_DEFN(unsigned long)

BUILTIN_ASSIGN_FNC_DEFN(float)
BUILTIN_ASSIGN_FNC_DEFN(double)
BUILTIN_ASSIGN_FNC_DEFN(long double)

#endif
/****************************************************************************/
/****************************************************************************/
_FNC_INLINE void Swap(void*& x, void*& y)
{
  void* temp = x;
  x = y;
  y = temp;
}

/****************************************************************************/
_FNC_INLINE long SizeOfData(void* Obj_)
{
  return sizeof(Obj_);
}

/****************************************************************************/
_FNC_INLINE long FormWidthOfObject(void* Obj_)
{
  return
  (
    long(sizeof(long) +
	 sizeof(Byte_t)) +
    SizeOfData(Obj_)
  );
}

/****************************************************************************/
_FNC_INLINE void* Clone(void* Ptr_)
{
#if OVERLOAD_NEW
  return memmove(MemMatrix::Matrix().Allocate(sizeof(Ptr_)), Ptr_, sizeof(Ptr_));
#else
  return memmove(new Byte_t[sizeof(Ptr_)], Ptr_, sizeof(Ptr_));
#endif
}

/****************************************************************************/
_FNC_INLINE const void* ConstClone(const void* Ptr_)
{
  return Ptr_;
}

/****************************************************************************/
_FNC_INLINE void* NonConstClone(void* Ptr_)
{
  return Ptr_;
}

/****************************************************************************/
_FNC_INLINE void*& Assign(void*& trg, void*& src)
{
  trg = src;
  return trg;
}

/****************************************************************************/
_FNC_INLINE void*& NonConstAssign(void*& Trg_, void*& Src_)
{
  Trg_ = Src_;
  return Trg_;
}

/****************************************************************************/
_FNC_INLINE Boolean IsStreamable(const void* Obj_)
{ return TRUE; }

/****************************************************************************/
_FNC_INLINE Boolean IsSortable(const void* Obj_)
{ return TRUE; }

/****************************************************************************/
_FNC_INLINE Boolean IsMatchable(const void* Obj_)
{ return TRUE; }

/****************************************************************************/
_FNC_INLINE Boolean IsGreaterThan(const void* Trg_, const void* Src_)
{ return (Trg_ > Src_); }

/****************************************************************************/
_FNC_INLINE Boolean IsLessThan(const void* Trg_, const void* Src_)
{ return (Trg_ < Src_); }

/****************************************************************************/
_FNC_INLINE Boolean IsEqualTo(const void* Trg_, const void* Src_)
{ return (Trg_ == Src_); }

/****************************************************************************/
_FNC_INLINE Boolean AtZero(const void* Trg_)
{ return (Trg_ == ((void*)0)); }

/****************************************************************************/
_FNC_INLINE Boolean NotZero(const void* Trg_)
{ return (Trg_ != ((void*)0)); }

/****************************************************************************/
#if defined(__GNUC__)
_FNC_INLINE Boolean IsStreamable(void* Obj_)
{ return TRUE; }

/****************************************************************************/
_FNC_INLINE Boolean IsSortable(void* Obj_)
{ return TRUE; }

/****************************************************************************/
_FNC_INLINE Boolean IsMatchable(void* Obj_)
{ return TRUE; }

/****************************************************************************/
_FNC_INLINE Boolean IsGreaterThan(void* Trg_, void* Src_)
{ return (Trg_ > Src_); }

/****************************************************************************/
_FNC_INLINE Boolean IsLessThan(void* Trg_, void* Src_)
{ return (Trg_ < Src_); }

/****************************************************************************/
_FNC_INLINE Boolean IsEqualTo(void* Trg_, void* Src_)
{ return (Trg_ == Src_); }
#endif
/****************************************************************************/
_FNC_INLINE ostream& WriteToOutput(ostream& os, void* Obj_)
{
  os <<Obj_;
  return os;
}

/****************************************************************************/
#endif




