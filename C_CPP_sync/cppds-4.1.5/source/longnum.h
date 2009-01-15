#ifndef LONGNUM_H
#define LONGNUM_H
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif

#ifndef BCDDIGIT_H
  #include "bcddigit.h"
#endif
#ifndef STRUTIL_H
  #include "strutil.h"
#endif
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif
#ifndef LONGDOUBLE_H
  #include "ldouble.h"
#endif
#ifndef DOUBLE_H
  #include "double.h"
#endif
#ifndef FLOAT_H
  #include "float.h"
#endif
#ifndef ULONGINT_H
  #include "ulongint.h"
#endif
#ifndef CHAR_H
  #include "char.h"
#endif
#ifndef MIXEDNUMWRAPPER_H
  #include "mixnumwrap.h"
#endif
#ifndef PATHTOKEN_H
  #include "pathtoken.h"
#endif

#define LONGNUM_DEBUG                   0
#define LONGNUM_DEBUG_SHOWOUTPUT        0
#define LONGNUM_DEBUG_ASSERT            0
#define LONGNUM_DEBUG_ASSERT2a          0
#define LONGNUM_DEBUG_ASSERT2b          0
#define LONGNUM_DEBUG_ASSERT3           0
#define LONGNUM_DEBUG_ASSERT4           0
#define LONGNUM_DEBUG_ASSERT5           0
#define LONGNUM_DEBUG_ASSERT5a          0
#define LONGNUM_DEBUG_ASSERT5b          0
#define LONGNUM_DEBUG_ASSERT5c          0
#define LONGNUM_DEBUG_ASSERT5d          0
#define LONGNUM_DEBUG_ASSERT5e          0
#define LONGNUM_DEBUG_ASSERT5f          0
#define LONGNUM_DEBUG_ASSERT6           0

#ifdef LONGNUM_DEBUG_ASSERT
  #ifndef INCL_ASSERT_H
    #include <assert.h>
    #define INCL_ASSERT_H
  #endif
#endif

// generated with 40-digit precision
#define READ_PRESETS                    1
#define PRESET_ROOT2                    "1.4142135623730950488016887242096980785697"
#define PRESET_ROOT3                    "1.7320508075688772935274463415058723669429"
#define PRESET_E                        "2.7182818284590452353602874713526624977552"
#define PRESET_PI                       "3.1415926535897932384626433832795028841972"

#define LONGNUMBER_ANSIMATH_OPERATIONS(Type)                            \
friend Type& _Sine(Type& Val_)                                          \
{ Val_ = Val_.sin();                                                    \
  return Val_; }                                                        \
friend Type& _Cosine(Type& Val_)                                        \
{ Val_ = Val_.cos();                                                    \
  return Val_; }                                                        \
friend Type& _Tangent(Type& Val_)                                       \
{ Val_ = Val_.tan();                                                    \
  return Val_; }                                                        \
friend Type& _ArcSine(Type& Val_)                                       \
{ Val_ = Val_.arcsin();                                                 \
  return Val_; }                                                        \
friend Type& _ArcCosine(Type& Val_)                                     \
{ Val_ = Val_.arccos();                                                 \
  return Val_; }                                                        \
friend Type& _ArcTangent(Type& Val_)                                    \
{ Val_ = Val_.arctan();                                                 \
  return Val_; }                                                        \
friend Type& _ArcTangent2(Type& Val1_, const Type& Val2_)               \
{ Val1_ = Val1_.arctan2(Val2_);                                         \
  return Val1_; }                                                       \
friend Type& _Exponent(Type& Base_, const Type& Exp_)                   \
{ Base_ = Base_.pow(Exp_);                                              \
  return Base_; }                                                       \
friend Type& _HyperSine(Type& Val_)                                     \
{ Val_ = Val_.sinh();                                                   \
  return Val_; }                                                        \
friend Type& _HyperCosine(Type& Val_)                                   \
{ Val_ = Val_.cosh();                                                   \
  return Val_; }                                                        \
  friend Type& _HyperTangent(Type& Val_)                                \
{ Val_ = Val_.tanh();                                                   \
  return Val_; }                                                        \
friend Type& _NaturalExp(Type& Val_)                                    \
{ Val_ = Val_.exp();                                                    \
  return Val_; }                                                        \
friend Type& _NaturalLog(Type& Val_)                                    \
{ Val_ = Val_.log();                                                    \
  return Val_; }                                                        \
friend Type& _Log10(Type& Val_)                                         \
{ Val_ = Val_.log10();                                                  \
  return Val_; }                                                        \
friend Type& _Ceiling(Type& Val_)                                       \
{ Val_ = Val_.Ceiling();                                                \
  return Val_; }                                                        \
friend Type& _Floor(Type& Val_)                                         \
{ Val_ = Val_.Floor();                                                  \
  return Val_; }                                                        \
friend Type& _AbsoluteValue(Type& Val_)                                 \
{ Val_ = Val_.AbsoluteValue();                                          \
  return Val_; }                                                        \
friend Type& _SquareRoot(Type& Val_)                                    \
{ Val_ = Val_.sqrt();                                                   \
  return Val_; }                                                        \
friend Type& _FractionPart(Type& Val_)                                  \
{ LongNumber ip;                                                        \
  Val_ = Val_.modf(ip);                                                 \
  return Val_; }                                                        \
friend Type& _IntegerPart(Type& Val_)                                   \
{ LongNumber ip;                                                        \
  Val_.modf(ip);                                                        \
  Val_ = ip;                                                            \
  return Val_; }

/****************************************************************************/
class LongNumberAcceptor : public ObjectAcceptor
{
  friend class LongNumber;

  protected:
    union
    {
      const LongNumber* _ObjPtr;
      LongNumber* _VarPtr;
    };

    LongNumberAcceptor(const LongNumber* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // LongNumber class specific acceptor methods
    virtual Boolean IsLongNumber() const;
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

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
// LongNumber class forward declaration
class LongNumber;

class LongNode
{
  protected:  
    Ulong _BcdStr;

    LongNode* _Prev;
    LongNode* _Next;

    static Ulong FindBcdStrFromChrStr(const char* NumStr_, Ushort Radix_);

  public:
    LongNode();
    LongNode(Ulong NumVal_);
    LongNode(const char* NumStr_, Ushort Radix_);
    LongNode(LongNode* CurLink_, LongNode* PrevLink_);
    ~LongNode();

    LongNode* AddNodeWithStr(const char* NumStr_, Ushort Radix_);
    LongNode* AddNodeWithValue(Ulong NumVal_);
    LongNode* AppendNodeWithStr(const char* NumStr_, Ushort Radix_);
    LongNode* AppendNodeWithValue(Ulong NumVal_);

    Ulong TotalDigits() const;
          
    BcdDigit GetDigit(Ulong Exp_) const;
    void SetDigit(const BcdDigit& Digit_, Ulong Exp_);
    void SetCarry(const BcdDigit& Digit_, Ulong Exp_);

    inline LongNode* GetNext()
        { return _Next; }
    inline LongNode* GetPrevious()
        { return _Prev; }

    // ----- Saving and restoration in binary form ----
    long DataSize () const;
    Boolean BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    Boolean BinaryWrite(const LongNumber* Refer_, ostream& Os_, Boolean* Ok_=NULL) const;
    Boolean BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    Boolean BinaryWrite(const LongNumber* Refer_, ByteStream& Os_, Boolean* Ok_=NULL) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif        
};

/****************************************************************************/
class DecimalPoint
{
  protected:
    Ulong _RightShift;
    Ulong _DecExponent;
    Ulong _IncExponent;

    void SciNoteDecShift(const char* NumStr_);
    char* RemoveSciNoteSymbol(const char* ptr_, ptrdiff_t& Epos_);

  public:
    DecimalPoint();
    DecimalPoint(const DecimalPoint& Obj_);
    DecimalPoint(const char* NumStr_);

    DecimalPoint& operator = (const DecimalPoint& Obj_);
    DecimalPoint& operator = (const char* NumStr_);

    Ulong GetRightShift() const
        { return _RightShift; }
    Ulong GetIncExponent() const
        { return _IncExponent; }
    Ulong GetDecExponent() const
        { return _DecExponent; }

    DecimalPoint& SetRightShift(Ulong Shift_);
    DecimalPoint& SetDecExponent(Ulong Shift_);
    DecimalPoint& SetIncExponent(Ulong Shift_);
    DecimalPoint& IncreaseExponent(Ulong Shift_, Boolean Back_=FALSE);
    DecimalPoint& DecreaseExponent(Ulong Shift_, Boolean Back_=FALSE);
    DecimalPoint& ShiftRight(Ulong Shift_, Boolean Back_=FALSE);

    Boolean operator == (const DecimalPoint& Obj_) const;
    Boolean operator != (const DecimalPoint& Obj_) const;
    Boolean operator < (const DecimalPoint& Obj_) const;
    Boolean operator > (const DecimalPoint& Obj_) const;
    Boolean operator <= (const DecimalPoint& Obj_) const;
    Boolean operator >= (const DecimalPoint& Obj_) const;

    // ----- Saving and restoration in binary form ----
    long DataSize () const;    
    Boolean BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    Boolean BinaryWrite(const LongNumber* Refer_, ostream& Os_, Boolean* Ok_=NULL) const;
    Boolean BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    Boolean BinaryWrite(const LongNumber* Refer_, ByteStream& Os_, Boolean* Ok_=NULL) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif            
};

struct NumberSign
{
  enum
  {
    MINUS = -1,
    PLUS = 1
  };
};

class LongNumber : public MixedNumberType<LongNumber, ChrString>
{
  friend class LongNumberAcceptor;
  friend class LongNode;
  friend class DecimalPoint;

  ROOTOBJECTFNCS_DEFN(TypeWrapperImp, LongNumber)
  LONGNUMBER_OPERATIONS(LongNumber)
  LONGNUMBER_ANSIMATH_OPERATIONS(LongNumber)

  // Comparison functions
  friend Boolean operator == (const LongNumber& Num1_, const LongNumber& Num2_)
      { return (Num1_.IsEqual(Num2_)); }  
  friend Boolean operator != (const LongNumber& Num1_, const LongNumber& Num2_)
      { return (!Num1_.IsEqual(Num2_)); }  
  friend Boolean operator < (const LongNumber& Num1_, const LongNumber& Num2_)
      { return (Num1_.IsLesser(Num2_)); }  
  friend Boolean operator > (const LongNumber& Num1_, const LongNumber& Num2_)
      { return (Num1_.IsGreater(Num2_)); }  
  friend Boolean operator <= (const LongNumber& Num1_, const LongNumber& Num2_)
      { return (Num1_.IsLesser(Num2_) || Num1_.IsEqual(Num2_)); }  
  friend Boolean operator >= (const LongNumber& Num1_, const LongNumber& Num2_)
      { return (Num1_.IsGreater(Num2_) || Num1_.IsEqual(Num2_)); }

  friend Boolean operator == (const LongNumber& Num1_, long Num2_)
      { return (Num1_.IsEqual(LongInt(Num2_))); }  
  friend Boolean operator != (const LongNumber& Num1_, long Num2_)
      { return (!Num1_.IsEqual(LongInt(Num2_))); }  
  friend Boolean operator < (const LongNumber& Num1_, long Num2_)
      { return (Num1_.IsLesser(LongInt(Num2_))); }  
  friend Boolean operator > (const LongNumber& Num1_, long Num2_)
      { return (Num1_.IsGreater(LongInt(Num2_))); }  
  friend Boolean operator <= (const LongNumber& Num1_, long Num2_)
      { return (Num1_.IsLesser(LongInt(Num2_)) || Num1_.IsEqual(LongInt(Num2_))); }  
  friend Boolean operator >= (const LongNumber& Num1_, long Num2_)
      { return (Num1_.IsGreater(LongInt(Num2_)) || Num1_.IsEqual(LongInt(Num2_))); }  
      
  friend Boolean operator == (long Num1_, const LongNumber& Num2_)
      { LongInt Lnum1_(Num1_);
        return (LongNumber(Lnum1_).IsEqual(Num2_)); }
  friend Boolean operator != (long Num1_, const LongNumber& Num2_)
      { LongInt Lnum1_(Num1_);
        return (!LongNumber(Lnum1_).IsEqual(Num2_)); }
  friend Boolean operator < (long Num1_, const LongNumber& Num2_)
      { LongInt Lnum1_(Num1_);
        return (LongNumber(Lnum1_).IsLesser(Num2_)); }
  friend Boolean operator > (long Num1_, const LongNumber& Num2_)
      { LongInt Lnum1_(Num1_);
        return (LongNumber(Lnum1_).IsGreater(Num2_)); }
  friend Boolean operator <= (long Num1_, const LongNumber& Num2_)
      { LongInt Lnum1_(Num1_);
        return (LongNumber(Lnum1_).IsLesser(Num2_) || LongNumber(Lnum1_).IsEqual(Num2_)); }
  friend Boolean operator >= (long Num1_, const LongNumber& Num2_)
      { LongInt Lnum1_(Num1_);
        return (LongNumber(Lnum1_).IsGreater(Num2_) || LongNumber(Lnum1_).IsEqual(Num2_)); }
      
  // Arithmetic functions
  friend LongNumber operator + (const LongNumber& Obj1_, const LongNumber& Obj2_)
      { return (LongNumber(Obj1_) += Obj2_); }
  friend LongNumber operator - (const LongNumber& Obj1_, const LongNumber& Obj2_)
      { return (LongNumber(Obj1_) -= Obj2_); }
  friend LongNumber operator * (const LongNumber& Obj1_, const LongNumber& Obj2_)
      { return (LongNumber(Obj1_) *= Obj2_); }
  friend LongNumber operator / (const LongNumber& Obj1_, const LongNumber& Obj2_)
      { return (LongNumber(Obj1_) /= Obj2_); }
  friend LongNumber operator % (const LongNumber& Obj1_, const LongNumber& Obj2_)
      { return (LongNumber(Obj1_) %= Obj2_); }
  friend LongNumber DivInt(const LongNumber& Obj1_, const LongNumber& Obj2_)
      { return (LongNumber(Obj1_).DivInt(Obj2_)); }

  friend LongNumber operator + (const LongNumber& Obj1_, long Obj2_)
      { return (LongNumber(Obj1_) += Obj2_); }
  friend LongNumber operator - (const LongNumber& Obj1_, long Obj2_)
      { return (LongNumber(Obj1_) -= Obj2_); }
  friend LongNumber operator * (const LongNumber& Obj1_, long Obj2_)
      { return (LongNumber(Obj1_) *= Obj2_); }
  friend LongNumber operator / (const LongNumber& Obj1_, long Obj2_)
      { return (LongNumber(Obj1_) /= Obj2_); }
  friend LongNumber operator % (const LongNumber& Obj1_, long Obj2_)
      { return (LongNumber(Obj1_) %= Obj2_); }
  friend LongNumber DivInt(const LongNumber& Obj1_, long Obj2_)
      { return (LongNumber(Obj1_).DivInt(Obj2_)); }

  friend LongNumber operator + (long Obj1_, const LongNumber& Obj2_)
      { LongInt Lnum1_(Obj1_);
        return (LongNumber(Lnum1_) += Obj2_); }
  friend LongNumber operator - (long Obj1_, const LongNumber& Obj2_)
      { LongInt Lnum1_(Obj1_);
        return (LongNumber(Lnum1_) -= Obj2_); }
  friend LongNumber operator * (long Obj1_, const LongNumber& Obj2_)
      { LongInt Lnum1_(Obj1_);
        return (LongNumber(Lnum1_) *= Obj2_); }
  friend LongNumber operator / (long Obj1_, const LongNumber& Obj2_)
      { LongInt Lnum1_(Obj1_);
        return (LongNumber(Lnum1_) /= Obj2_); }
  friend LongNumber operator % (long Obj1_, const LongNumber& Obj2_)
      { LongInt Lnum1_(Obj1_);
        return (LongNumber(Lnum1_) %= Obj2_); }
  friend LongNumber DivInt(long Obj1_, const LongNumber& Obj2_)
      { LongInt Lnum1_(Obj1_);
        return (LongNumber(Lnum1_).DivInt(Obj2_)); }

  protected:
    enum { DIV_PRECISION = 40 };

    int _Sign;
    Ulong _NumNodes;
    long _DivPrecision;
    Ushort _Base;
    Ushort _OriginalBase;
    Byte_t _SciNote;    
    
    LongNode* _Head;
    LongNode* _Tail;
    
    DecimalPoint* _DecimalPt;
    ChrString* _StringRep;
    bool _NanVal;
    bool _InfVal;
    
    static long _Instances;
    static int _OmitConsts;
    static int _UseSeries;
    static long _DefaultPrecision;
    static const char* _DataFile;
    fstream _File;

    void Clear();
    LongNode* FindTail();
    void FeedNumberStr(const char* NumStr_);
    void FeedNumberValue(Ulong Value_);
    void StoreConstants();    
    void SetConstants();
    void ReadPresets();    

    static char* Substring(const char* Src_, char* Buf_, size_t Start_, size_t End_);
    static char* StripPaddingZeros(char* str, Ushort Radix_);
    static char* StripLeadingZeros(char* str);
    static char* StripTrailingZeros(char* NumStr_);      
        
    LongNumber* ConvertBaseToDec(const char* NumStr_, Ushort Radix_) const;
    char* ConvertDecToBase(Ushort Radix_) const;    

    LongNumber& AddNodeWithStr(const char* NumStr_, Ushort Radix_, Boolean RightOfPoint_);
    LongNumber& AddNodeWithValue(Ulong NumVal_, Boolean RightOfPoint_);

    LongNumber& AppendNodeWithStr(const char* NumStr_, Ushort Radix_);
    LongNumber& AppendNodeWithValue(Ulong NumVal_);

    // shift decimal position
    LongNumber& IncExponent(long Shift_, Boolean Back_=FALSE);
    LongNumber& DecExponent(long Shift_, Boolean Back_=FALSE);    

    BcdDigit RawGetDigit(Ulong Exp_) const;
    LongNumber& RawClearDigit(Ulong Exp_);
    LongNumber& RawSetDigit(const BcdDigit& Digit_, Ulong Exp_);
    LongNumber& RawSetCarry(const BcdDigit& Digit_, Ulong Exp_);
    char* RawToString(int Radix_=10) const;
    long OffsetExponent(long Exp_) const;
    static char* RemoveNonDigits(char* NumStr_, Ushort Radix_);
    char* PrependBaseModifier(char* NumStr_, Ushort Radix_) const;
    char* PrependSign(char* NumStr_) const;
    char* PrependSciNote(char* NumStr_, Ushort Radix_) const;

    // Integer roots methods
    static void FindIntRoot(const LongNumber& Value_, LongNumber* Fval_, LongNumber* Dval_,
                            const LongNumber& Intercept_, long Root_);
    static LongNumber RtSafe(const LongNumber& x1, const LongNumber& x2, const LongNumber& xacc,
                             const LongNumber& Intercept_, long Root_);

    // Division method
    LongNumber& DivProc(const LongNumber& Obj_, LongNumber* Remainder_, Boolean IntDiv_);
    long FindDivGuess(LongNumber* Src_, LongNumber* Trg_);

    static char* NewString(const char* NumStr_);
    static char* FormatForBcdNode(char* NumStr_);    
    static Ulong CharToBcd(char DigitChar_, Ushort Exp_);
    static char BcdToChar(Ushort DigitVal_);
    static short SubtractFromTensCmp(LongNumber& NewResult_, LongNumber& Result_,
                                     long Exp_, long Min_, short Unit_);

    // series approximation methods
    // (*) convergence of power series eats up too much time
    //    
    static LongNumber Pi_Series();
    static LongNumber Sin_Series(const LongNumber& InVal_);
    static LongNumber Cos_Series(const LongNumber& InVal_);
    static LongNumber E_Series(const LongNumber& InVal_);
    static LongNumber Ln_Series(const LongNumber& InVal_);         //   (*)
    static LongNumber Asin_Series(const LongNumber& InVal_);       //   (*)
    static LongNumber Atan_Series(const LongNumber& InVal_);       //   (*)

    virtual Boolean IsEqualToLongNumber(const LongNumber* Ptr_) const;
    virtual Boolean IsLesserToLongNumber(const LongNumber* Ptr_) const;
    virtual Boolean IsGreaterToLongNumber(const LongNumber* Ptr_) const;

    // Set Nan and Infinite flags
    void SetNan(bool Flag_);
    void SetInfinite(bool Flag_);

    // Constructor for setting static special value objects
    LongNumber(const Double& FltObj_, int, int, int);    

    static const LongNumber* p_E;
    static const LongNumber* p_PI;
    static const LongNumber* p_ROOT2;
    static const LongNumber* p_ROOT3;
    static const LongNumber* p_NAN;
    static const LongNumber* p_INFINITY;

  public:
    enum
    {
      COS_SERIES    = 1,
      SIN_SERIES    = 2,
      ATAN_SERIES   = 4,
      ASIN_SERIES   = 8,
      LN_SERIES     = 16,
      E_SERIES      = 32,
      PI_SERIES     = 64,
      SQRT_SERIES   = 128
    };
  
    // Constants accessor methods
    static const LongNumber& vE();
    static const LongNumber& vPI();
    static const LongNumber& vROOT2();
    static const LongNumber& vROOT3();
    static const LongNumber& vNAN();
    static const LongNumber& vINFINITY();
    
  public:
    LongNumber();
    LongNumber(long Value_);
    LongNumber(Ulong Value_, int Sign_);
    LongNumber(const ChrString& NumStr_);
    LongNumber(const LongNumber& Obj_);
    LongNumber(const LongDouble& FltObj_);
    LongNumber(const Double& FltObj_);
    LongNumber(const Float& FltObj_);    
    LongNumber(const ShortInt& IntObj_);
    LongNumber(const LongInt& IntObj_);
    LongNumber(const Integer& IntObj_);    
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      LongNumber(const UShortInt& IntObj_);
      LongNumber(const ULongInt& IntObj_);
      LongNumber(const UInteger& IntObj_);
    #endif
    
    virtual ~LongNumber();

    // Class Identification methods
    static int StaticID()
	     { return TypeInfo::LONGNUMBER_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Virtual constructor and assignment methods
    static LongNumber* Make()
        { return (new LongNumber); }
    static LongNumber* Make(const ChrString& NumStr_)
        { return (new LongNumber(NumStr_)); }
    static LongNumber* Make(Ulong Value_, int Sign_)
        { return (new LongNumber(Value_, Sign_)); }
    static LongNumber* Make(const LongDouble& FltObj_)
        { return (new LongNumber(FltObj_)); }
    static LongNumber* Make(const Double& FltObj_)
        { return (new LongNumber(FltObj_)); }
    static LongNumber* Make(const Float& FltObj_)
        { return (new LongNumber(FltObj_)); }        
    static LongNumber* Make(const LongNumber& Obj_)
        { return (new LongNumber(Obj_)); }        
    static LongNumber* Make(const ShortInt& IntObj_)
        { return (new LongNumber(IntObj_)); }
    static LongNumber* Make(const LongInt& IntObj_)
        { return (new LongNumber(IntObj_)); }
    static LongNumber* Make(const Integer& IntObj_)
        { return (new LongNumber(IntObj_)); }
    static LongNumber* Make(const UShortInt& IntObj_)
        { return (new LongNumber(IntObj_)); }
    static LongNumber* Make(const ULongInt& IntObj_)
        { return (new LongNumber(IntObj_)); }
    static LongNumber* Make(const UInteger& IntObj_)
        { return (new LongNumber(IntObj_)); }        
    
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;        

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

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;

    // Static null object accessor methods
    static LongNumber& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();    

    virtual LongNumber& DerivedClass();
    virtual const LongNumber& DerivedClass() const;

    static Boolean IsInSciNote(const ChrString& Str_);
    static Boolean IsValue(const ChrString& Str_);
    static LongNumber ValueOf(const ChrString& Str_);

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    LongNumber& SetValue(Ulong Value_, int Sign_);
    virtual LongNumber& operator = (long Value_);
    virtual LongNumber& operator = (const LongNumber& Obj_);
    virtual LongNumber& operator = (const ChrString& Str_);
    virtual LongNumber& operator = (const LongDouble& FltObj_);
    virtual LongNumber& operator = (const Double& FltObj_);
    virtual LongNumber& operator = (const Float& FltObj_);
    virtual LongNumber& operator = (const Integer& Obj_);
    virtual LongNumber& operator = (const LongInt& Obj_);
    virtual LongNumber& operator = (const ShortInt& Obj_);
    virtual LongNumber& operator = (const UInteger& Obj_);
    virtual LongNumber& operator = (const ULongInt& Obj_);    
    virtual LongNumber& operator = (const UShortInt& Obj_);

    // Unary operators
    Boolean AtZero() const;
    Boolean operator ! () const;
    LongNumber operator - () const;
    LongNumber operator + () const;
    LongNumber Complement() const;
    LongNumber UnsignedValue() const;
    int GetSign() const
        { return _Sign; }
    void FlipSign()
        { _Sign *= -1; }
    void SetSign(int Sign_)
        { _Sign = (Sign_ > 0) ? (NumberSign::PLUS):(NumberSign::MINUS); }

    virtual LongNumber& operator += (const MixedNumType<LongNumber>& Obj_);
    virtual LongNumber& operator -= (const MixedNumType<LongNumber>& Obj_);
    virtual LongNumber& operator *= (const MixedNumType<LongNumber>& Obj_);
    virtual LongNumber& operator /= (const MixedNumType<LongNumber>& Obj_);
    virtual LongNumber& operator %= (const MixedNumType<LongNumber>& Obj_);
    virtual LongNumber& DivInt(const MixedNumType<LongNumber>& Obj_);

    virtual LongNumber& operator += (long Val_);
    virtual LongNumber& operator -= (long Val_);
    virtual LongNumber& operator *= (long Val_);
    virtual LongNumber& operator /= (long Val_);
    virtual LongNumber& operator %= (long Val_);
    virtual LongNumber& DivInt(long Val_);

    // Increment and decrement operators
    virtual const LongNumber& operator ++ ();
    virtual const LongNumber& operator ++ (int);
    virtual const LongNumber& operator -- ();
    virtual const LongNumber& operator -- (int);

    // Rounding methods
    virtual LongNumber& Round(long Exp_);
    virtual LongNumber& Ceiling();
    virtual LongNumber& Floor();
    virtual LongNumber& Round();
    virtual LongNumber& AbsoluteValue();
    
#if HAS_BOOL
    operator bool () const
	{ return !AtZero(); }
#else
    operator Boolean () const
	{ return !AtZero(); }
#endif

    Boolean IsInteger() const;
    long LowestExp() const;
    long HighestExp() const;
    Ulong TotalDigits() const;
    Ulong AvailableDigits() const
        { return (_NumNodes * BcdDigit::DigitsPerLong()); }

    Ushort ConvertedFromBase() const;
    static long SetDefaultPrecision(long Prec_);
    long GetPrecision() const;
    BcdDigit GetDigit(long Exp_) const;
    LongNumber& ClearDigit(long Exp_);
    LongNumber& SetDigit(const BcdDigit& Digit_, long Exp_);
    LongNumber& SetCarry(const BcdDigit& Digit_, long Exp_);
    LongNumber& SetPrecision(long Prec_);
    LongNumber& SetSciNotation(Boolean Flag_=TRUE);
    LongNumber& SetFixedPoint(Boolean Flag_=TRUE);

    static ChrString ToString(const LongNumber& Int_, int Radix_=10);
    static ChrString ToTruncString(const LongNumber& Int_, int Radix_=10);

    virtual ChrString ToString(int Radix_=10) const;
    virtual ChrString ToTruncString(int Radix_=10) const;
    virtual LongNumber& FromString(const ChrString& Str_);

    virtual void SetData(const ChrString& Obj_);
    virtual const ChrString& GetData() const;
    virtual operator ChrString () const;

    static LongNumber ExpPow(const LongNumber& Pow_);
    static LongNumber IntPow(const LongNumber& Value_, long Exp_);
    static LongNumber IntRoot(const LongNumber& Value_, long Root_, long Prec_=0);    
    static LongNumber Factorial(Ulong Max_, int Prec_=0);
    
    // ANSI C math functions
    // Not implemented -- (*) convergence of power series eats up too much time
    //
    LongNumber sin() const;                       // (*) uses standard C math functions
    LongNumber cos() const;                       // (*) uses standard C math functions
    LongNumber tan() const;                       // (*) uses standard C math functions
    LongNumber arcsin() const;                    // (*) uses standard C math functions
    LongNumber arccos() const;                    // (*) uses standard C math functions
    LongNumber arctan() const;                    // (*) uses standard C math functions
    LongNumber arctan2(const LongNumber& y);      // (*) uses standard C math functions
    LongNumber sinh() const;                      // (*) uses standard C math functions
    LongNumber cosh() const;                      // (*) uses standard C math functions
    LongNumber tanh() const;                      // (*) uses standard C math functions
    LongNumber exp() const;                       // (*) uses standard C math functions
    LongNumber log() const;                       // (*) uses standard C math functions
    LongNumber log10() const;                     // (*) uses standard C math functions
    LongNumber pow(const LongNumber& y) const;    // (*) uses standard C math functions
    LongNumber ldexp(long Exp_);
    LongNumber frexp(long* Exp_);
    LongNumber fabs() const;
    LongNumber sqrt() const;                      // (*) uses standard C math functions
    LongNumber modf(LongNumber& ip) const;
    LongNumber fmod(const LongNumber& y) const;

    // Infinity and NonNumber Special Value Methods
    static Boolean IsNan(const LongNumber& Val_);
    static Boolean IsInfinite(const LongNumber& Val_);
    virtual Boolean IsNan() const;
    virtual Boolean IsInfinite() const;    

    // Omit setting up constants
    static void OmitConstants(Boolean Flag_=TRUE);

    // Use the custom power series (slow) for the function as specified in the bit string
    static void UseSeriesApprox(int BitStr_);

    // All custom power series are switched off,
    // functions marked by (*) uses standard C math functions instead (default setting)
    static void SetSpeedCritical();    
    

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




