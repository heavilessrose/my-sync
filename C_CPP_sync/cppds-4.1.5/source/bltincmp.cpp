#ifndef BUILTINSERIESCOMPARE_CPP
#define BUILTINSERIESCOMPARE_CPP
#ifndef BUILTINSERIESCOMPARE_H
  #include "bltincmp.h"
#endif
/****************************************************************************/
intSeriesCompare::intSeriesCompare(int Unit_, int* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<int>(Unit_, List_, Sz_, NumberingScheme::SIGNED_2SCOMP, 0, SHRT_MIN / 2, new_int, DelParam_)
{}

/****************************************************************************/
intSeriesCompare::intSeriesCompare(const intSeriesCompare& Obj_):
BuiltInSeriesCompare<int>(Obj_)
{}

/****************************************************************************/
intSeriesCompare& intSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<int>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
intSeriesCompare& intSeriesCompare::SetMinValue(int MinVal_)
{
  BuiltInSeriesCompare<int>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
intSeriesCompare& intSeriesCompare::SetMaxValue(int MaxVal_)
{
  BuiltInSeriesCompare<int>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
intSeriesCompare& intSeriesCompare::operator = (const intSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<int>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
intSeriesCompare& intSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<int>::FindQuotient();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
UintSeriesCompare::UintSeriesCompare(Uint Unit_, Uint* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<Uint>(Unit_, List_, Sz_, NumberingScheme::UNSIGNED_INTS, 0U, SHRT_MAX / 2, new_unsigned_int, DelParam_)
{}

/****************************************************************************/
UintSeriesCompare::UintSeriesCompare(const UintSeriesCompare& Obj_):
BuiltInSeriesCompare<Uint>(Obj_)
{}

/****************************************************************************/
UintSeriesCompare& UintSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<Uint>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
UintSeriesCompare& UintSeriesCompare::SetMinValue(Uint MinVal_)
{
  BuiltInSeriesCompare<Uint>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
UintSeriesCompare& UintSeriesCompare::SetMaxValue(Uint MaxVal_)
{
  BuiltInSeriesCompare<Uint>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
UintSeriesCompare& UintSeriesCompare::operator = (const UintSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<Uint>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
UintSeriesCompare& UintSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<Uint>::FindQuotient();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
charSeriesCompare::charSeriesCompare(char Unit_, char* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<char>(Unit_, List_, Sz_, NumberingScheme::SIGNED_2SCOMP, 0, -16, new_char, DelParam_)
{}

/****************************************************************************/
charSeriesCompare::charSeriesCompare(const charSeriesCompare& Obj_):
BuiltInSeriesCompare<char>(Obj_)
{}

/****************************************************************************/
charSeriesCompare& charSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<char>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
charSeriesCompare& charSeriesCompare::SetMinValue(char MinVal_)
{
  BuiltInSeriesCompare<char>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
charSeriesCompare& charSeriesCompare::SetMaxValue(char MaxVal_)
{
  BuiltInSeriesCompare<char>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
charSeriesCompare& charSeriesCompare::operator = (const charSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<char>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
charSeriesCompare& charSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<char>::FindQuotient();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
UcharSeriesCompare::UcharSeriesCompare(Uchar Unit_, Uchar* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<Uchar>(Unit_, List_, Sz_, NumberingScheme::UNSIGNED_INTS, 0, 16, new_unsigned_char, DelParam_)
{}

/****************************************************************************/
UcharSeriesCompare::UcharSeriesCompare(const UcharSeriesCompare& Obj_):
BuiltInSeriesCompare<Uchar>(Obj_)
{}

/****************************************************************************/
UcharSeriesCompare& UcharSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<Uchar>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
UcharSeriesCompare& UcharSeriesCompare::SetMinValue(Uchar MinVal_)
{
  BuiltInSeriesCompare<Uchar>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
UcharSeriesCompare& UcharSeriesCompare::SetMaxValue(Uchar MaxVal_)
{
  BuiltInSeriesCompare<Uchar>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
UcharSeriesCompare& UcharSeriesCompare::operator = (const UcharSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<Uchar>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
UcharSeriesCompare& UcharSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<Uchar>::FindQuotient();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
shortSeriesCompare::shortSeriesCompare(short Unit_, short* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<short>(Unit_, List_, Sz_, NumberingScheme::SIGNED_2SCOMP, 0, -256, new_short, DelParam_)
{}

/****************************************************************************/
shortSeriesCompare::shortSeriesCompare(const shortSeriesCompare& Obj_):
BuiltInSeriesCompare<short>(Obj_)
{}

/****************************************************************************/
shortSeriesCompare& shortSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<short>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
shortSeriesCompare& shortSeriesCompare::SetMinValue(short MinVal_)
{
  BuiltInSeriesCompare<short>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
shortSeriesCompare& shortSeriesCompare::SetMaxValue(short MaxVal_)
{
  BuiltInSeriesCompare<short>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
shortSeriesCompare& shortSeriesCompare::operator = (const shortSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<short>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
shortSeriesCompare& shortSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<short>::FindQuotient();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
UshortSeriesCompare::UshortSeriesCompare(Ushort Unit_, Ushort* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<Ushort>(Unit_, List_, Sz_, NumberingScheme::UNSIGNED_INTS, 0, 256, new_unsigned_short, DelParam_)
{}

/****************************************************************************/
UshortSeriesCompare::UshortSeriesCompare(const UshortSeriesCompare& Obj_):
BuiltInSeriesCompare<Ushort>(Obj_)
{}

/****************************************************************************/
UshortSeriesCompare& UshortSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<Ushort>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
UshortSeriesCompare& UshortSeriesCompare::SetMinValue(Ushort MinVal_)
{
  BuiltInSeriesCompare<Ushort>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
UshortSeriesCompare& UshortSeriesCompare::SetMaxValue(Ushort MaxVal_)
{
  BuiltInSeriesCompare<Ushort>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
UshortSeriesCompare& UshortSeriesCompare::operator = (const UshortSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<Ushort>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
UshortSeriesCompare& UshortSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<Ushort>::FindQuotient();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
longSeriesCompare::longSeriesCompare(long Unit_, long* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<long>(Unit_, List_, Sz_, NumberingScheme::SIGNED_2SCOMP, 0, -65536L, new_long, DelParam_)
{}

/****************************************************************************/
longSeriesCompare::longSeriesCompare(const longSeriesCompare& Obj_):
BuiltInSeriesCompare<long>(Obj_)
{}

/****************************************************************************/
longSeriesCompare& longSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<long>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
longSeriesCompare& longSeriesCompare::SetMinValue(long MinVal_)
{
  BuiltInSeriesCompare<long>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
longSeriesCompare& longSeriesCompare::SetMaxValue(long MaxVal_)
{
  BuiltInSeriesCompare<long>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
longSeriesCompare& longSeriesCompare::operator = (const longSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<long>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
longSeriesCompare& longSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<long>::FindQuotient();
  return *this;
}

/****************************************************************************/
/****************************************************************************/
UlongSeriesCompare::UlongSeriesCompare(Ulong Unit_, Ulong* List_, size_t Sz_, Boolean DelParam_):
BuiltInSeriesCompare<Ulong>(Unit_, List_, Sz_, NumberingScheme::UNSIGNED_INTS, 0, 65536L, new_unsigned_long, DelParam_)
{}

/****************************************************************************/
UlongSeriesCompare::UlongSeriesCompare(const UlongSeriesCompare& Obj_):
BuiltInSeriesCompare<Ulong>(Obj_)
{}

/****************************************************************************/
UlongSeriesCompare& UlongSeriesCompare::SetCalcDiff()
{
  BuiltInSeriesCompare<Ulong>::SetCalcDiff();
  return *this;
}

/****************************************************************************/
UlongSeriesCompare& UlongSeriesCompare::SetMinValue(Ulong MinVal_)
{
  BuiltInSeriesCompare<Ulong>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
UlongSeriesCompare& UlongSeriesCompare::SetMaxValue(Ulong MaxVal_)
{
  BuiltInSeriesCompare<Ulong>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
UlongSeriesCompare& UlongSeriesCompare::operator = (const UlongSeriesCompare& Obj_)
{
  BuiltInSeriesCompare<Ulong>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
UlongSeriesCompare& UlongSeriesCompare::FindQuotient()
{
  BuiltInSeriesCompare<Ulong>::FindQuotient();
  return *this;
}

/****************************************************************************/
#if BLTINCMP_DEBUG
#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERT_H
#endif
int main()
{
#if BLTINCMP_INTSERIES
  cout <<"int series -- ";
  intSeriesCompare IntCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MAX, new_ints(3, INT_MAX - 100, 50, 51), 3);
  IntCmp_.FindQuotient();
  assert(IntCmp_.Result() < 0 && IntCmp_.Unit() == INT_MAX && IntCmp_.Quotient() == 1 && IntCmp_.Remainder() == 1);
  IntCmp_ = intSeriesCompare::DifferenceOfSeriesCmp(INT_MIN, new_ints(3, INT_MAX - 100, 50, 51), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == INT_MIN && IntCmp_.Quotient() == 1 && IntCmp_.Remainder() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MAX, new_ints(3, INT_MAX - 100, 50, 50), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == INT_MAX && IntCmp_.Quotient() == 1 && IntCmp_.Remainder() == 0);
  IntCmp_ = intSeriesCompare::DifferenceOfSeriesCmp(INT_MIN, new_ints(3, INT_MAX - 100, 50, 50), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() < 0 && IntCmp_.Unit() == INT_MIN && IntCmp_.Quotient() == 0 && IntCmp_.Remainder() == INT_MAX);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(INT_MAX, new_ints(3, INT_MAX - 100, 50, 49), 3);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() > 0 && IntCmp_.Unit() == INT_MAX && IntCmp_.Quotient() == 0 && IntCmp_.Remainder() == INT_MAX - 1);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(2, 1, 1), 2);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() < 0 && IntCmp_.Unit() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(6, 1, 2, 3, 4, -5, -5), 6);
  IntCmp_.FindQuotient();
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(2, 1, -1), 2);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == 0);
  IntCmp_ = intSeriesCompare::SumOfSeriesCmp(0, new_ints(2, 0, 0), 2);
  IntCmp_.FindQuotient();  
  assert(IntCmp_.Result() == 0 && IntCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif

#if BLTINCMP_UINTSERIES
  cout <<"Uint series -- ";
  UintSeriesCompare UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(UINT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 51), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == UINT_MAX && UIntCmp_.Quotient() == 1 && UIntCmp_.Remainder() == 1);
  UIntCmp_ = UintSeriesCompare::DifferenceOfSeriesCmp(UINT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 51), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() > 0 && UIntCmp_.Unit() == UINT_MAX && UIntCmp_.Quotient() == -1 && UIntCmp_.Remainder() == Uint(-1));
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(INT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 50), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == INT_MAX && UIntCmp_.Quotient() == 2 && UIntCmp_.Remainder() == 1);
  UIntCmp_ = UintSeriesCompare::DifferenceOfSeriesCmp(UINT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 50), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() > 0 && UIntCmp_.Unit() == UINT_MAX && UIntCmp_.Quotient() == -1 && UIntCmp_.Remainder() == 0);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(INT_MAX, new_unsigned_ints(3, UINT_MAX - 100, 50, 49), 3);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == INT_MAX && UIntCmp_.Quotient() == 2 && UIntCmp_.Remainder() == 0);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(0, new_unsigned_ints(2, 1, 1), 2);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() < 0 && UIntCmp_.Unit() == 0);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(21, new_unsigned_ints(6, 1, 2, 3, 4, 5, 6), 6);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() == 0 && UIntCmp_.Unit() == 21 && UIntCmp_.Quotient() == 1 && UIntCmp_.Remainder() == 0);
  UIntCmp_ = UintSeriesCompare::ProductOfSeriesCmp(8193, new_unsigned_ints(3, 64, 32, 4), 3);
  assert(UIntCmp_.Result() > 0 && UIntCmp_.Unit() == 8193);
  UIntCmp_ = UintSeriesCompare::SumOfSeriesCmp(0, new_unsigned_ints(2, 0, 0), 2);
  UIntCmp_.FindQuotient();  
  assert(UIntCmp_.Result() == 0 && UIntCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif

#if BLTINCMP_SHORTSERIES
  cout <<"short series -- ";
  shortSeriesCompare ShortCmp_ = shortSeriesCompare::SumOfSeriesCmp(SHRT_MAX, new_shorts(3, SHRT_MAX - 100, 50, 51), 3);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() < 0 && ShortCmp_.Unit() == SHRT_MAX && ShortCmp_.Quotient() == 1 && ShortCmp_.Remainder() == 1);
  ShortCmp_ = shortSeriesCompare::DifferenceOfSeriesCmp(SHRT_MIN, new_shorts(3, SHRT_MAX - 100, 50, 51), 3);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() == 0 && ShortCmp_.Unit() == SHRT_MIN && ShortCmp_.Quotient() == 1 && ShortCmp_.Remainder() == 0);
  ShortCmp_ = shortSeriesCompare::SumOfSeriesCmp(SHRT_MAX, new_shorts(3, SHRT_MAX - 100, 50, 50), 3);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() == 0 && ShortCmp_.Unit() == SHRT_MAX && ShortCmp_.Quotient() == 1 && ShortCmp_.Remainder() == 0);
  ShortCmp_ = shortSeriesCompare::DifferenceOfSeriesCmp(SHRT_MIN, new_shorts(3, SHRT_MAX - 100, 50, 50), 3);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() < 0 && ShortCmp_.Unit() == SHRT_MIN && ShortCmp_.Quotient() == 0 && ShortCmp_.Remainder() == SHRT_MAX);
  ShortCmp_ = shortSeriesCompare::SumOfSeriesCmp(SHRT_MAX, new_shorts(3, SHRT_MAX - 100, 50, 49), 3);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() > 0 && ShortCmp_.Unit() == SHRT_MAX && ShortCmp_.Quotient() == 0 && ShortCmp_.Remainder() == SHRT_MAX - 1);
  ShortCmp_ = shortSeriesCompare::SumOfSeriesCmp(0, new_shorts(2, 10, 10), 2);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() < 0 && ShortCmp_.Unit() == 0);
  ShortCmp_ = shortSeriesCompare::SumOfSeriesCmp(0, new_shorts(6, 1, 2, 3, 4, -5, -5), 6);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() == 0 && ShortCmp_.Unit() == 0);
  ShortCmp_ = shortSeriesCompare::SumOfSeriesCmp(0, new_shorts(2, 1, -1), 2);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() == 0 && ShortCmp_.Unit() == 0);
  ShortCmp_ = shortSeriesCompare::SumOfSeriesCmp(0, new_shorts(2, 0, 0), 2);
  ShortCmp_.FindQuotient();  
  assert(ShortCmp_.Result() == 0 && ShortCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif

#if BLTINCMP_USHORTSERIES
  cout <<"Ushort series -- ";
  UshortSeriesCompare UShortCmp_ = UshortSeriesCompare::SumOfSeriesCmp(USHRT_MAX, new_unsigned_shorts(3, USHRT_MAX - 100, 50, 51), 3);
  UShortCmp_.FindQuotient();  
  assert(UShortCmp_.Result() < 0 && UShortCmp_.Unit() == USHRT_MAX && UShortCmp_.Quotient() == 1 && UShortCmp_.Remainder() == 1);
  UShortCmp_ = UshortSeriesCompare::DifferenceOfSeriesCmp(USHRT_MAX, new_unsigned_shorts(3, USHRT_MAX - 100, 50, 51), 3);
  UShortCmp_.FindQuotient();  
  assert(UShortCmp_.Result() > 0 && UShortCmp_.Unit() == USHRT_MAX && UShortCmp_.Quotient() == -1 && UShortCmp_.Remainder() == Ushort(-1));
  UShortCmp_ = UshortSeriesCompare::SumOfSeriesCmp(SHRT_MAX, new_unsigned_shorts(3, USHRT_MAX - 100, 50, 50), 3);
  UShortCmp_.FindQuotient();  
  assert(UShortCmp_.Result() < 0 && UShortCmp_.Unit() == SHRT_MAX && UShortCmp_.Quotient() == 2 && UShortCmp_.Remainder() == 1);
  UShortCmp_ = UshortSeriesCompare::DifferenceOfSeriesCmp(USHRT_MAX, new_unsigned_shorts(3, USHRT_MAX - 100, 50, 50), 3);
  UShortCmp_.FindQuotient();  
  assert(UShortCmp_.Result() > 0 && UShortCmp_.Unit() == USHRT_MAX && UShortCmp_.Quotient() == -1 && UShortCmp_.Remainder() == 0);
  UShortCmp_ = UshortSeriesCompare::SumOfSeriesCmp(SHRT_MAX, new_unsigned_shorts(3, USHRT_MAX - 100, 50, 49), 3);
  UShortCmp_.FindQuotient();  
  assert(UShortCmp_.Result() < 0 && UShortCmp_.Unit() == SHRT_MAX && UShortCmp_.Quotient() == 2 && UShortCmp_.Remainder() == 0);
  UShortCmp_ = UshortSeriesCompare::SumOfSeriesCmp(0, new_unsigned_shorts(2, 1, 1), 2);
  UShortCmp_.FindQuotient();
  assert(UShortCmp_.Result() < 0 && UShortCmp_.Unit() == 0);
  UShortCmp_ = UshortSeriesCompare::SumOfSeriesCmp(21, new_unsigned_shorts(6, 1, 2, 3, 4, 5, 6), 6);
  UShortCmp_.FindQuotient();  
  assert(UShortCmp_.Result() == 0 && UShortCmp_.Unit() == 21 && UShortCmp_.Quotient() == 1 && UShortCmp_.Remainder() == 0);
  UShortCmp_ = UshortSeriesCompare::ProductOfSeriesCmp(16385, new_unsigned_shorts(4, 4, 8, 16, 32), 4);
  assert(UShortCmp_.Result() > 0 && UShortCmp_.Unit() == 16385);
  UShortCmp_ = UshortSeriesCompare::SumOfSeriesCmp(0, new_unsigned_shorts(2, 0, 0), 2);
  UShortCmp_.FindQuotient();  
  assert(UShortCmp_.Result() == 0 && UShortCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif

#if BLTINCMP_LONGSERIES
  cout <<"long series -- ";
  longSeriesCompare LongCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_longs(3, LONG_MAX - 100, 50L, 51L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() < 0 && LongCmp_.Unit() == LONG_MAX && LongCmp_.Quotient() == 1 && LongCmp_.Remainder() == 1);
  LongCmp_ = longSeriesCompare::DifferenceOfSeriesCmp(LONG_MIN, new_longs(3, LONG_MAX - 100, 50L, 51L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == LONG_MIN && LongCmp_.Quotient() == 1 && LongCmp_.Remainder() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_longs(3, LONG_MAX - 100, 50L, 50L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == LONG_MAX && LongCmp_.Quotient() == 1 && LongCmp_.Remainder() == 0);
  LongCmp_ = longSeriesCompare::DifferenceOfSeriesCmp(LONG_MIN, new_longs(3, LONG_MAX - 100, 50L, 50L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() < 0 && LongCmp_.Unit() == LONG_MIN && LongCmp_.Quotient() == 0 && LongCmp_.Remainder() == LONG_MAX);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_longs(3, LONG_MAX - 100, 50L, 49L), 3);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() > 0 && LongCmp_.Unit() == LONG_MAX && LongCmp_.Quotient() == 0 && LongCmp_.Remainder() == LONG_MAX - 1);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(2, 20L, 20L), 2);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() < 0 && LongCmp_.Unit() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(6, 1L, 2L, 3L, 4L, -5L, -5L), 6);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(2, 1L, -1L), 2);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == 0);
  LongCmp_ = longSeriesCompare::SumOfSeriesCmp(0, new_longs(2, 0L, 0L), 2);
  LongCmp_.FindQuotient();  
  assert(LongCmp_.Result() == 0 && LongCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif

#if BLTINCMP_ULONGSERIES
  cout <<"Ulong series -- ";
  UlongSeriesCompare ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(ULONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 51UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == ULONG_MAX && ULongCmp_.Quotient() == 1 && ULongCmp_.Remainder() == 1);
  ULongCmp_ = UlongSeriesCompare::DifferenceOfSeriesCmp(ULONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 51UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() > 0 && ULongCmp_.Unit() == ULONG_MAX && ULongCmp_.Quotient() == -1 && ULongCmp_.Remainder() == Ulong(-1));
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 50UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == LONG_MAX && ULongCmp_.Quotient() == 2 && ULongCmp_.Remainder() == 1);
  ULongCmp_ = UlongSeriesCompare::DifferenceOfSeriesCmp(ULONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 50UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() > 0 && ULongCmp_.Unit() == ULONG_MAX && ULongCmp_.Quotient() == -1 && ULongCmp_.Remainder() == 0);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(LONG_MAX, new_unsigned_longs(3, ULONG_MAX - 100, 50UL, 49UL), 3);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == LONG_MAX && ULongCmp_.Quotient() == 2 && ULongCmp_.Remainder() == 0);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(0, new_unsigned_longs(2, 1UL, 1UL), 2);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() < 0 && ULongCmp_.Unit() == 0);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(21, new_unsigned_longs(6, 1UL, 2UL, 3UL, 4UL, 5UL, 6UL), 6);
  ULongCmp_.FindQuotient();  
  assert(ULongCmp_.Result() == 0 && ULongCmp_.Unit() == 21 && ULongCmp_.Quotient() == 1 && ULongCmp_.Remainder() == 0);
  ULongCmp_ = UlongSeriesCompare::ProductOfSeriesCmp(134217728UL, new_unsigned_longs(6, 4UL, 8UL, 16UL, 32UL, 64UL, 128UL), 6);
  assert(ULongCmp_.Result() == 0 && ULongCmp_.Unit() == 134217728UL);
  ULongCmp_ = UlongSeriesCompare::SumOfSeriesCmp(0, new_unsigned_longs(2, 0UL, 0UL), 2);
  ULongCmp_.FindQuotient();    
  assert(ULongCmp_.Result() == 0 && ULongCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif

#if BLTINCMP_CHARSERIES
  char Buffer_[10];
  cout <<"char series -- ";
  Buffer_[0] = CHAR_MAX - 100; Buffer_[1] = Buffer_[2] = 50; ++Buffer_[2];
  charSeriesCompare CharCmp_ = charSeriesCompare::SumOfSeriesCmp(CHAR_MAX, new_char_array(3, Buffer_), 3);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() < 0 && CharCmp_.Unit() == CHAR_MAX && CharCmp_.Quotient() == 1 && CharCmp_.Remainder() == 1);
  CharCmp_ = charSeriesCompare::DifferenceOfSeriesCmp(CHAR_MIN, new_char_array(3, Buffer_), 3);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() == 0 && CharCmp_.Unit() == CHAR_MIN && CharCmp_.Quotient() == 1 && CharCmp_.Remainder() == 0);
  --Buffer_[2];
  CharCmp_ = charSeriesCompare::SumOfSeriesCmp(CHAR_MAX, new_char_array(3, Buffer_), 3);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() == 0 && CharCmp_.Unit() == CHAR_MAX && CharCmp_.Quotient() == 1 && CharCmp_.Remainder() == 0);
  CharCmp_ = charSeriesCompare::DifferenceOfSeriesCmp(CHAR_MIN, new_char_array(3, Buffer_), 3);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() < 0 && CharCmp_.Unit() == CHAR_MIN && CharCmp_.Quotient() == 0 && CharCmp_.Remainder() == CHAR_MAX);
  --Buffer_[2];
  CharCmp_ = charSeriesCompare::SumOfSeriesCmp(CHAR_MAX, new_char_array(3, Buffer_), 3);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() > 0 && CharCmp_.Unit() == CHAR_MAX && CharCmp_.Quotient() == 0 && CharCmp_.Remainder() == CHAR_MAX - 1);
  Buffer_[0] = Buffer_[1] = 5;
  CharCmp_ = charSeriesCompare::SumOfSeriesCmp(0, new_char_array(2, Buffer_), 2);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() < 0 && CharCmp_.Unit() == 0);
  Buffer_[0] = 1; Buffer_[1] = 2; Buffer_[2] = 3; Buffer_[3] = 4; Buffer_[4] = Buffer_[5] = -5;
  CharCmp_ = charSeriesCompare::SumOfSeriesCmp(0, new_char_array(6, Buffer_), 6);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() == 0 && CharCmp_.Unit() == 0);
  Buffer_[0] = 1; Buffer_[1] = -1;
  CharCmp_ = charSeriesCompare::SumOfSeriesCmp(0, new_char_array(2, Buffer_), 2);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() == 0 && CharCmp_.Unit() == 0);
  Buffer_[0] = Buffer_[1] = 0;
  CharCmp_ = charSeriesCompare::SumOfSeriesCmp(0, new_char_array(2, Buffer_), 2);
  CharCmp_.FindQuotient();    
  assert(CharCmp_.Result() == 0 && CharCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif
  
#if BLTINCMP_UCHARSERIES
  cout <<"Uchar series -- ";
  unsigned char* UBuffer_ = (unsigned char*)Buffer_;
  UBuffer_[0] = UCHAR_MAX - 100; UBuffer_[1] = UBuffer_[2] = 50; ++UBuffer_[2];
  UcharSeriesCompare UCharCmp_ = UcharSeriesCompare::SumOfSeriesCmp(UCHAR_MAX, new_unsigned_char_array(3, UBuffer_), 3);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() < 0 && UCharCmp_.Unit() == UCHAR_MAX && UCharCmp_.Quotient() == 1 && UCharCmp_.Remainder() == 1);
  UCharCmp_ = UcharSeriesCompare::DifferenceOfSeriesCmp(UCHAR_MAX, new_unsigned_char_array(3, UBuffer_), 3);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() > 0 && UCharCmp_.Unit() == UCHAR_MAX && UCharCmp_.Quotient() == -1 && UCharCmp_.Remainder() == Uchar(-1));
  --UBuffer_[2];
  UCharCmp_ = UcharSeriesCompare::SumOfSeriesCmp(CHAR_MAX, new_unsigned_char_array(3, UBuffer_), 3);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() < 0 && UCharCmp_.Unit() == CHAR_MAX && UCharCmp_.Quotient() == 2 && UCharCmp_.Remainder() == 1);
  UCharCmp_ = UcharSeriesCompare::DifferenceOfSeriesCmp(UCHAR_MAX, new_unsigned_char_array(3, UBuffer_), 3);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() > 0 && UCharCmp_.Unit() == UCHAR_MAX && UCharCmp_.Quotient() == -1 && UCharCmp_.Remainder() == 0);
  --UBuffer_[2];
  UCharCmp_ = UcharSeriesCompare::SumOfSeriesCmp(CHAR_MAX, new_unsigned_char_array(3, UBuffer_), 3);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() < 0 && UCharCmp_.Unit() == CHAR_MAX && UCharCmp_.Quotient() == 2 && UCharCmp_.Remainder() == 0);
  UBuffer_[0] = UBuffer_[1] = 1;
  UCharCmp_ = UcharSeriesCompare::SumOfSeriesCmp(0, new_unsigned_char_array(2, UBuffer_), 2);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() < 0 && UCharCmp_.Unit() == 0);
  UBuffer_[0] = 1; UBuffer_[1] = 2; UBuffer_[2] = 3; UBuffer_[3] = 4; UBuffer_[4] = 5; UBuffer_[5] = 6;
  UCharCmp_ = UcharSeriesCompare::SumOfSeriesCmp(21, new_unsigned_char_array(6, UBuffer_), 6);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() == 0 && UCharCmp_.Unit() == 21 && UCharCmp_.Quotient() == 1 && UCharCmp_.Remainder() == 0);
  UBuffer_[0] = UBuffer_[1] = 11;
  UCharCmp_ = UcharSeriesCompare::ProductOfSeriesCmp(122, new_unsigned_char_array(2, UBuffer_), 2);
  assert(UCharCmp_.Result() > 0 && UCharCmp_.Unit() == 122);
  UBuffer_[0] = UBuffer_[1] = 0;
  UCharCmp_ = UcharSeriesCompare::SumOfSeriesCmp(0, new_unsigned_char_array(2, UBuffer_), 2);
  UCharCmp_.FindQuotient();    
  assert(UCharCmp_.Result() == 0 && UCharCmp_.Unit() == 0);
  cout <<"TEST SUCCESSFULL" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif





