#ifndef SERIESCOMPARE_H
#define SERIESCOMPARE_H
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif

#define SERCMP_DEBUG	   0
#define SERCMP_DEBUG1	0
#define SERCMP_DEBUG2	0

#define SERCMP_BOUNDS_CHECK 	0

struct NumberingScheme
{
  enum
  {
    SIGNED_1SCOMP,
    SIGNED_2SCOMP,
    UNSIGNED_INTS,
    EQUAL_LEN_RANGES,
    POS_RANGE_GREATER,
    NEG_RANGE_GREATER
  };

  static Boolean IsNegRangeLarger(int Scheme_)
	{ return (Scheme_ == SIGNED_2SCOMP || Scheme_ == NEG_RANGE_GREATER); }
  static Boolean IsPosRangeLarger(int Scheme_)
	{ return (Scheme_ == UNSIGNED_INTS || Scheme_ == POS_RANGE_GREATER); }
  static Boolean IsEqualRanges(int Scheme_)
	{ return (Scheme_ == SIGNED_1SCOMP || Scheme_ == EQUAL_LEN_RANGES); }
};

template <class T>
class SeriesCmpData
{
  protected:
    enum
    {
      ZERO_BIT		= 0x01,
      FLIPPED_BIT	= 0x02,
      NEGSCALE_BIT	= 0x04,
      DIVIDEBYZERO_BIT  = 0x08,
      MINVALSET_BIT	= 0x10,
      MAXVALSET_BIT	= 0x20,
      EQUALRANGE_BIT	= 0x40,
      CALCDIFF_BIT	= 0x80
    };

    T _Unit;
    T _ZeroValue;
    long _Quantity;
    T _Fraction;

    short _Result;
    T _ValOffset;
    T _MinVal;
    T _MaxVal;

    Byte_t _StatusBits;

    inline Boolean Flipped() const
	{ return (_StatusBits & FLIPPED_BIT); }
    inline Boolean NegScale() const
	{ return (_StatusBits & NEGSCALE_BIT); }
    inline Boolean MinValSet() const
	{ return (_StatusBits & MINVALSET_BIT); }
    inline Boolean MaxValSet() const
	{ return (_StatusBits & MAXVALSET_BIT); }
    inline Boolean EqualRanges() const
	{ return (_StatusBits & EQUALRANGE_BIT); }
    inline Boolean CalcDiffOfSeries() const
	{ return (_StatusBits & CALCDIFF_BIT); }
    inline void SetStatusBit(int BitFlag_)
	{ _StatusBits |= BitFlag_; }
    inline void UnsetStatusBit(int BitFlag_)
	{ _StatusBits &= ~BitFlag_; }

    SeriesCmpData(const T& Unit_, const T& ZeroValue_,
		  long Qty_, const T& Fraction_,
		  short Result_, Boolean Zero_, int Scheme_,
		  const T& ValOffset_, Boolean Flipped_);

  public:
    inline Boolean Zero() const
	{ return (_StatusBits & ZERO_BIT); }
    inline Boolean DivideByZero() const
	{ return (_StatusBits & DIVIDEBYZERO_BIT); }

    Boolean Negative() const
	{ return ((_Unit < _ZeroValue && !Flipped()) ||
		  (_Unit > _ZeroValue && Flipped())); }

    short Result() const
	{ return _Result; }
    const T& Unit() const
	{ return _Unit; }
    const T& ZeroValue() const
	{ return _ZeroValue; }
    long Quotient() const
	{ return _Quantity; }
    T Remainder() const
	{ return _Fraction; }
};

template <class T>
class SeriesCompare : public SeriesCmpData<T>
{
  protected:
    static T LengthValue(const T& Val_, const T& Zero_, Boolean NegRangeLarger_);

    const T& Magnitude() const;
    T LengthValue(const T& Val_) const;

    void Decrement(const T& Obj_);
    void Increment(const T& Obj_);

    inline Boolean IsLesserLen(const T& Mag_, const T& Obj_)
	{ return (NegScale() ? (Mag_ > Obj_):(Mag_ < Obj_)); }
    inline Boolean IsGreaterLen(const T& Mag_, const T& Obj_)
	{ return (NegScale() ? (Mag_ < Obj_):(Mag_ > Obj_)); }
    inline Boolean IsLenOne(const T& Value_)
	{ return (NegScale() ? (Value_ / Value_ == -Value_):
			       (Value_ / Value_ == Value_)); }

    void SumOfSeriesCmpHelper(T** List_);
    void SumOfSeriesCmpResult();

    Boolean PrepareForMult(T** List_, size_t Sz_, short& Sign_);
    void SortList(T** List_, size_t Size_);
    size_t SizeOfList(T** List_);

    Boolean IsFlippable() const;
    void ShiftBackToZero();
    void UnflipNegRange();
    void UnflipPosRange();

    void ReverseScalePos();
    void ReverseScaleNeg();
    void FindQuotientNeg();
    void FindQuotientPos();

    SeriesCompare<T>& SetMinValue(const T& MinVal_);
    SeriesCompare<T>& SetMaxValue(const T& MaxVal_);
    SeriesCompare<T>& SetCalcDiff();

    short GiveSumOfSeriesCmp(T** List_);
    short GiveProductOfSeriesCmp(T** List_);

    SeriesCompare(const T& Unit_, const T& ZeroValue_, Boolean Zero_, int Scheme_);

  public:
    virtual ~SeriesCompare() {}

    static SeriesCompare<T> SumOfSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, int NumScheme_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumScheme_);
	  CmpObj_.GiveSumOfSeriesCmp(List_);
	  return CmpObj_; }
    static SeriesCompare<T> SumOfSignedSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, const T& MinVal_, const T& MaxVal_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumberingScheme::SIGNED_2SCOMP);
	  CmpObj_.SetMinValue(MinVal_).SetMaxValue(MaxVal_).GiveSumOfSeriesCmp(List_);
	  return CmpObj_; }
    static SeriesCompare<T> SumOfUnsignedSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, const T& MaxVal_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumberingScheme::UNSIGNED_INTS);
	  CmpObj_.SetMinValue(ZeroValue_).SetMaxValue(MaxVal_).GiveSumOfSeriesCmp(List_);
	  return CmpObj_; }

    static SeriesCompare<T> DifferenceOfSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, int NumScheme_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumScheme_);
	  CmpObj_.SetCalcDiff().GiveSumOfSeriesCmp(List_);
	  return CmpObj_; }
    static SeriesCompare<T> DifferenceOfSignedSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, const T& MinVal_, const T& MaxVal_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumberingScheme::SIGNED_2SCOMP);
	  CmpObj_.SetCalcDiff().SetMinValue(MinVal_).SetMaxValue(MaxVal_).GiveSumOfSeriesCmp(List_);
	  return CmpObj_; }
    static SeriesCompare<T> DifferenceOfUnsignedSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, const T& MaxVal_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumberingScheme::UNSIGNED_INTS);
	  CmpObj_.SetCalcDiff().SetMinValue(ZeroValue_).SetMaxValue(MaxVal_).GiveSumOfSeriesCmp(List_);
	  return CmpObj_; }

    static SeriesCompare<T> ProductOfSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, int NumScheme_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumScheme_);
	  CmpObj_.GiveProductOfSeriesCmp(List_);
	  return CmpObj_; }
    static SeriesCompare<T> ProductOfSignedSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, const T& MinVal_, const T& MaxVal_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumberingScheme::SIGNED_2SCOMP);
	  CmpObj_.SetMinValue(MinVal_).SetMaxValue(MaxVal_).GiveProductOfSeriesCmp(List_);
	  return CmpObj_; }
    static SeriesCompare<T> ProductOfUnsignedSeriesCmp(const T& Unit_, T** List_, const T& ZeroValue_, const T& MaxVal_, Boolean Zero_=FALSE)
	{ SeriesCompare<T> CmpObj_(Unit_, ZeroValue_, Zero_, NumberingScheme::UNSIGNED_INTS);
	  CmpObj_.SetMinValue(ZeroValue_).SetMaxValue(MaxVal_).GiveProductOfSeriesCmp(List_);
	  return CmpObj_; }

    SeriesCompare<T>& operator = (const SeriesCompare<T>& Obj_);
    virtual SeriesCompare<T>& FindQuotient();
};

#endif






