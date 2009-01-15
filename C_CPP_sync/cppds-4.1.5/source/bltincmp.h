#ifndef BUILTINSERIESCOMPARE_H
#define BUILTINSERIESCOMPARE_H
#ifndef BUILTINSERIESCMP_CPP
  #include "bltinseriescmp.cpp"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif

#define BLTINCMP_DEBUG	         0

#define BLTINCMP_INTSERIES       1
#define BLTINCMP_UINTSERIES      1
#define BLTINCMP_LONGSERIES      1
#define BLTINCMP_ULONGSERIES     1
#define BLTINCMP_SHORTSERIES     0
#define BLTINCMP_USHORTSERIES    0
#define BLTINCMP_CHARSERIES      1
#define BLTINCMP_UCHARSERIES     1

/****************************************************************************/
class intSeriesCompare : public BuiltInSeriesCompare<int>
{
  protected:
    intSeriesCompare(int Unit_, int* List_, size_t Sz_, Boolean DelParam_);
    intSeriesCompare& SetMinValue(int MinVal_);
    intSeriesCompare& SetMaxValue(int MaxVal_);
    intSeriesCompare& SetCalcDiff();

  public:
    static intSeriesCompare SumOfSeriesCmp(int Unit_, int* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ intSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(INT_MIN).SetMaxValue(INT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static intSeriesCompare DifferenceOfSeriesCmp(int Unit_, int* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ intSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue(INT_MIN).SetMaxValue(INT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static intSeriesCompare ProductOfSeriesCmp(int Unit_, int* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ intSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(INT_MIN).SetMaxValue(INT_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    intSeriesCompare(const intSeriesCompare& Obj_);
    intSeriesCompare& operator = (const intSeriesCompare& Obj_);
    virtual intSeriesCompare& FindQuotient();
};

class UintSeriesCompare : public BuiltInSeriesCompare<Uint>
{
  protected:
    UintSeriesCompare(Uint Unit_, Uint* List_, size_t Sz_, Boolean DelParam_);
    UintSeriesCompare& SetMinValue(Uint MinVal_);
    UintSeriesCompare& SetMaxValue(Uint MaxVal_);
    UintSeriesCompare& SetCalcDiff();

  public:
    static UintSeriesCompare SumOfSeriesCmp(Uint Unit_, Uint* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UintSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(0U).SetMaxValue(UINT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UintSeriesCompare DifferenceOfSeriesCmp(Uint Unit_, Uint* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UintSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue(0U).SetMaxValue(UINT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UintSeriesCompare ProductOfSeriesCmp(Uint Unit_, Uint* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UintSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(0U).SetMaxValue(UINT_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    UintSeriesCompare(const UintSeriesCompare& Obj_);
    UintSeriesCompare& operator = (const UintSeriesCompare& Obj_);
    virtual UintSeriesCompare& FindQuotient();
};

/****************************************************************************/
class charSeriesCompare : public BuiltInSeriesCompare<char>
{
  protected:
    charSeriesCompare(char Unit_, char* List_, size_t Sz_, Boolean DelParam_);
    charSeriesCompare& SetMinValue(char MinVal_);
    charSeriesCompare& SetMaxValue(char MaxVal_);
    charSeriesCompare& SetCalcDiff();

  public:
    static charSeriesCompare SumOfSeriesCmp(char Unit_, char* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ charSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(CHAR_MIN).SetMaxValue(CHAR_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static charSeriesCompare DifferenceOfSeriesCmp(char Unit_, char* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ charSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue(CHAR_MIN).SetMaxValue(CHAR_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static charSeriesCompare ProductOfSeriesCmp(char Unit_, char* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ charSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(CHAR_MIN).SetMaxValue(CHAR_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    charSeriesCompare(const charSeriesCompare& Obj_);
    charSeriesCompare& operator = (const charSeriesCompare& Obj_);
    virtual charSeriesCompare& FindQuotient();
};

class UcharSeriesCompare : public BuiltInSeriesCompare<Uchar>
{
  protected:
    UcharSeriesCompare(Uchar Unit_, Uchar* List_, size_t Sz_, Boolean DelParam_);
    UcharSeriesCompare& SetMinValue(Uchar MinVal_);
    UcharSeriesCompare& SetMaxValue(Uchar MaxVal_);
    UcharSeriesCompare& SetCalcDiff();

  public:
    static UcharSeriesCompare SumOfSeriesCmp(Uchar Unit_, Uchar* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UcharSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue('\0').SetMaxValue(UCHAR_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UcharSeriesCompare DifferenceOfSeriesCmp(Uchar Unit_, Uchar* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UcharSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue('\0').SetMaxValue(UCHAR_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UcharSeriesCompare ProductOfSeriesCmp(Uchar Unit_, Uchar* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UcharSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue('\0').SetMaxValue(UCHAR_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    UcharSeriesCompare(const UcharSeriesCompare& Obj_);
    UcharSeriesCompare& operator = (const UcharSeriesCompare& Obj_);
    virtual UcharSeriesCompare& FindQuotient();
};

/****************************************************************************/
class shortSeriesCompare : public BuiltInSeriesCompare<short>
{
  protected:
    shortSeriesCompare(short Unit_, short* List_, size_t Sz_, Boolean DelParam_);
    shortSeriesCompare& SetMinValue(short MinVal_);
    shortSeriesCompare& SetMaxValue(short MaxVal_);
    shortSeriesCompare& SetCalcDiff();

  public:
    static shortSeriesCompare SumOfSeriesCmp(short Unit_, short* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ shortSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(SHRT_MIN).SetMaxValue(SHRT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static shortSeriesCompare DifferenceOfSeriesCmp(short Unit_, short* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ shortSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue(SHRT_MIN).SetMaxValue(SHRT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static shortSeriesCompare ProductOfSeriesCmp(short Unit_, short* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ shortSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(SHRT_MIN).SetMaxValue(SHRT_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    shortSeriesCompare(const shortSeriesCompare& Obj_);
    shortSeriesCompare& operator = (const shortSeriesCompare& Obj_);
    virtual shortSeriesCompare& FindQuotient();
};

class UshortSeriesCompare : public BuiltInSeriesCompare<Ushort>
{
  protected:
    UshortSeriesCompare(Ushort Unit_, Ushort* List_, size_t Sz_, Boolean DelParam_);
    UshortSeriesCompare& SetMinValue(Ushort MinVal_);
    UshortSeriesCompare& SetMaxValue(Ushort MaxVal_);
    UshortSeriesCompare& SetCalcDiff();

  public:
    static UshortSeriesCompare SumOfSeriesCmp(Ushort Unit_, Ushort* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UshortSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(Ushort(0)).SetMaxValue(USHRT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UshortSeriesCompare DifferenceOfSeriesCmp(Ushort Unit_, Ushort* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UshortSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue(Ushort(0)).SetMaxValue(USHRT_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UshortSeriesCompare ProductOfSeriesCmp(Ushort Unit_, Ushort* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UshortSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(Ushort(0)).SetMaxValue(USHRT_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    UshortSeriesCompare(const UshortSeriesCompare& Obj_);
    UshortSeriesCompare& operator = (const UshortSeriesCompare& Obj_);
    virtual UshortSeriesCompare& FindQuotient();
};

/****************************************************************************/
class longSeriesCompare : public BuiltInSeriesCompare<long>
{
  protected:
    longSeriesCompare(long Unit_, long* List_, size_t Sz_, Boolean DelParam_);
    longSeriesCompare& SetMinValue(long MinVal_);
    longSeriesCompare& SetMaxValue(long MaxVal_);
    longSeriesCompare& SetCalcDiff();

  public:
    static longSeriesCompare SumOfSeriesCmp(long Unit_, long* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ longSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(LONG_MIN).SetMaxValue(LONG_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static longSeriesCompare DifferenceOfSeriesCmp(long Unit_, long* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ longSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue(LONG_MIN).SetMaxValue(LONG_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static longSeriesCompare ProductOfSeriesCmp(long Unit_, long* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ longSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(LONG_MIN).SetMaxValue(LONG_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    longSeriesCompare(const longSeriesCompare& Obj_);
    longSeriesCompare& operator = (const longSeriesCompare& Obj_);
    virtual longSeriesCompare& FindQuotient();
};

class UlongSeriesCompare : public BuiltInSeriesCompare<Ulong>
{
  protected:
    UlongSeriesCompare(Ulong Unit_, Ulong* List_, size_t Sz_, Boolean DelParam_);
    UlongSeriesCompare& SetMinValue(Ulong MinVal_);
    UlongSeriesCompare& SetMaxValue(Ulong MaxVal_);
    UlongSeriesCompare& SetCalcDiff();

  public:
    static UlongSeriesCompare SumOfSeriesCmp(Ulong Unit_, Ulong* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UlongSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(0UL).SetMaxValue(ULONG_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UlongSeriesCompare DifferenceOfSeriesCmp(Ulong Unit_, Ulong* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UlongSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetCalcDiff().SetMinValue(0UL).SetMaxValue(ULONG_MAX).GiveSumOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }
    static UlongSeriesCompare ProductOfSeriesCmp(Ulong Unit_, Ulong* List_, size_t Sz_, Boolean DelParam_=TRUE)
	{ UlongSeriesCompare CmpObj_(Unit_, List_, Sz_, DelParam_);
	  CmpObj_.SetMinValue(0UL).SetMaxValue(ULONG_MAX).GiveProductOfSeriesCmp(CmpObj_._DataList);
	  return CmpObj_; }

    UlongSeriesCompare(const UlongSeriesCompare& Obj_);
    UlongSeriesCompare& operator = (const UlongSeriesCompare& Obj_);
    virtual UlongSeriesCompare& FindQuotient();
};

/****************************************************************************/
#endif





