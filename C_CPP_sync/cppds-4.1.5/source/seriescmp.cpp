#ifndef SERIESCOMPARE_CPP
#define SERIESCOMPARE_CPP
#ifndef SERIESCOMPARE_H
  #include "seriescmp.h"
#endif

/****************************************************************************/
template <class T>
SeriesCmpData<T>::SeriesCmpData(const T& Unit_, const T& ZeroValue_,
				long Qty_, const T& Fraction_, short Result_,
				Boolean Zero_, int Scheme_,
				const T& ValOffset_, Boolean Flipped_):
_Unit(Unit_),
_ZeroValue(ZeroValue_),
_Quantity(Qty_),
_Fraction(Fraction_),
_Result(Result_),
_ValOffset(ValOffset_),
_StatusBits((Zero_ ? ZERO_BIT:0) |
	    (Flipped_ ? FLIPPED_BIT:0) |
	    (NumberingScheme::IsNegRangeLarger(Scheme_) ? NEGSCALE_BIT:0) |
	    (NumberingScheme::IsEqualRanges(Scheme_) ? EQUALRANGE_BIT:0))
{}

/****************************************************************************/
template <class T>
SeriesCompare<T>::SeriesCompare(const T& Unit_, const T& ZeroValue_, Boolean Zero_, int Scheme_):
SeriesCmpData<T>
(
  LengthValue(Unit_, ZeroValue_, NumberingScheme::IsNegRangeLarger(Scheme_)),
  ZeroValue_,
  ((Unit_ > ZeroValue_) ? 1:
   (Unit_ < ZeroValue_) ? -1:0),
  ZeroValue_,
  0,
  Zero_,
  Scheme_,
  (Zero_ ? LengthValue(Unit_, ZeroValue_, NumberingScheme::IsNegRangeLarger(Scheme_)):ZeroValue_),
  (Unit_ != LengthValue(Unit_, ZeroValue_, NumberingScheme::IsNegRangeLarger(Scheme_)))
)
{}

/****************************************************************************/
template <class T>
const T& SeriesCompare<T>::Magnitude() const
{
  return ((_Fraction == _ZeroValue) ? _Unit:_Fraction);
}

/****************************************************************************/
template <class T>
T SeriesCompare<T>::LengthValue(const T& Val_, const T& Zero_, Boolean NegRangeLarger_)
{
  return (NegRangeLarger_ ? InvAbsoluteValue(Val_, Zero_):
			    AbsoluteValue(Val_, Zero_));
}

/****************************************************************************/
template <class T>
T SeriesCompare<T>::LengthValue(const T& Val_) const
{
  return LengthValue(Val_, _ZeroValue, NegScale());
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::Decrement(const T& Obj_)
{
  if (Obj_ == _ZeroValue)
    return;

  T Value_(LengthValue(Obj_));

  if (_Fraction != _ZeroValue)
  {
    if (IsLesserLen(Value_, _Fraction))
      _Fraction -= Value_;
    else
    {
      Value_ -= _Fraction;
      _Fraction = _ZeroValue;
      Decrement(Value_);
    }
  }
  else
  {
    while (IsLesserLen(_Unit, Value_) || _Unit == Value_)
    {
      --_Quantity;
      Value_ -= _Unit;
    }

    if (IsGreaterLen(Value_, _ZeroValue))
    {
      T UnitVal_(_Unit);
      UnitVal_ -= Value_;
      _Fraction = UnitVal_;
      --_Quantity;
    }
  }
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::Increment(const T& Obj_)
{
  if (Obj_ == _ZeroValue)
    return;

  T Value_(LengthValue(Obj_));

  if (_Fraction != _ZeroValue)
  {
    T Remain_(_Unit - _Fraction);

    if (IsLesserLen(Value_, Remain_))
      _Fraction += Value_;
    else
    {
      ++_Quantity;
      Value_ -= Remain_;
      _Fraction = _ZeroValue;
      Increment(Value_);
    }
  }
  else
  {
    while (IsLesserLen(_Unit, Value_) || _Unit == Value_)
    {
      ++_Quantity;
      Value_ -= _Unit;
    }

    if (IsGreaterLen(Value_, _ZeroValue))
      _Fraction += Value_;
  }
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::SumOfSeriesCmpHelper(T** List_)
{
  size_t Index_ = 0;

  if (!CalcDiffOfSeries())
  {
    for (;List_[Index_]; ++Index_)
      if (*List_[Index_] < _ZeroValue)
	Increment(*List_[Index_]);
      else if (*List_[Index_] > _ZeroValue)
	Decrement(*List_[Index_]);
  }
  else
  {
    for (;List_[Index_]; ++Index_)
      if (*List_[Index_] > _ZeroValue)
	Increment(*List_[Index_]);
      else if (*List_[Index_] < _ZeroValue)
	Decrement(*List_[Index_]);
  }
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::SumOfSeriesCmpResult()
{
  ShiftBackToZero();

  _Result =
	(_Quantity == 0 && _Fraction == _ZeroValue) ? 0:
	(_Quantity >= 0) ? 1:-1;
}

/****************************************************************************/
template <class T>
short SeriesCompare<T>::GiveSumOfSeriesCmp(T** List_)
{
  SumOfSeriesCmpHelper(List_);
  SumOfSeriesCmpResult();

  return _Result;
}

/****************************************************************************/
template <class T>
short SeriesCompare<T>::GiveProductOfSeriesCmp(T** List_)
{
  short Sign_;
  size_t Sz_ = SizeOfList(List_);
  Boolean Zero_ = PrepareForMult(List_, Sz_, Sign_);

  if (Sz_)
  {
    ShiftBackToZero();

    if (Zero_)
    {
      _Result =
	Negative() ? -1:
	Zero() ? 0:1;
    }
    else if ((Negative() || Zero()) && Sign_ == 1)
      _Result = -1;
    else if ((!Negative() || Zero()) && Sign_ == -1)
      _Result = 1;
    else
    {
      if (Sz_ > 1)
	SortList(List_, Sz_);

      T ListLenVal1_ = LengthValue(*List_[0]);
      T UnitLenVal_ = LengthValue(_Unit);

      if (IsLenOne(ListLenVal1_))
	_Result = (IsLenOne(UnitLenVal_) ? 0:((Sign_ == 1) ? 1:-1));
      else if (Sz_ == 1)
	_Result = (IsLesserLen(UnitLenVal_, ListLenVal1_) ? -1:
		   IsGreaterLen(UnitLenVal_, ListLenVal1_) ? 1:0);
      else
      {
	T Product_ = ListLenVal1_;
	T Quotient_ = _ZeroValue;

	size_t Index_;
	for (Index_ = 1; Index_ < Sz_; ++Index_)
	{
	  Quotient_ = UnitLenVal_ / Product_;
	  if (IsLesserLen(LengthValue(*List_[Index_]), LengthValue(Quotient_)) ||
	      LengthValue(*List_[Index_]) == LengthValue(Quotient_))
	    Product_ *= AbsoluteValue(*List_[Index_], _ZeroValue);
	  else
	    break;
	}

	_Result = (Sign_ == 1) ?
		    ((Index_ == Sz_) ? ((Product_ == UnitLenVal_) ? 0:1):-1):
		    ((Index_ == Sz_) ? ((Product_ == UnitLenVal_) ? 0:-1):1);
      }
    }
  }
  else
    return 0;

  _Quantity = 0;
  _Fraction = _ZeroValue;

  UnflipNegRange();
  UnflipPosRange();

  return _Result;
}

/****************************************************************************/
template <class T>
Boolean SeriesCompare<T>::PrepareForMult(T** List_, size_t Sz_, short& Sign_)
{
  Sign_ = 1;

  for (size_t Index_ = 0; Index_ < Sz_; ++Index_)
    if (*List_[Index_] == _ZeroValue)
      return TRUE;
    else if (*List_[Index_] < _ZeroValue)
      Sign_ = -Sign_;

  return FALSE;
}

/****************************************************************************/
template <class T>
size_t SeriesCompare<T>::SizeOfList(T** List_)
{
  size_t cnt;
  for (cnt = 0; List_[cnt]; ++cnt);
  return cnt;
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::SortList(T** List_, size_t Size_)
{
  for (size_t gap = Size_/2; 0 < gap; gap /= 2)
    for (size_t i = gap; i < Size_; ++i)
      if (i >= gap)
      {
	size_t j = i - gap;

	for (;;)
	{
	  // Sort descending
	  if (IsGreaterLen(LengthValue(*List_[j+gap]), LengthValue(*List_[j])))
	  {
	    T* Temp_ = List_[j];
	    List_[j] = List_[j+gap];
	    List_[j+gap] = Temp_;
	  }

	  if (j >= gap)
	    j -= gap;
	  else
	    break;
	}
      }
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::ShiftBackToZero()
{
  if (Zero())
  {
    if (Negative())
      Increment(_ValOffset);
    else
      Decrement(_ValOffset);

    _Unit = _ZeroValue;
  }
}

/****************************************************************************/
template <class T>
Boolean SeriesCompare<T>::IsFlippable() const
{
  return
    (Flipped() &&
      (EqualRanges() ||
       (NegScale() && MaxValSet() && _Unit >= InvAbsoluteValue(_MaxVal, _ZeroValue)) ||
       (!NegScale() && MinValSet() && _Unit <= AbsoluteValue(_MinVal, _ZeroValue))));
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::UnflipNegRange()
{
  if (IsFlippable())
  {
    _Unit = -_Unit;
    _Fraction = -_Fraction;
    UnsetStatusBit(FLIPPED_BIT);
  }
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::UnflipPosRange()
{
  if (IsFlippable())
  {
    _Unit = -_Unit;
    _Fraction = -_Fraction;
    UnsetStatusBit(FLIPPED_BIT);
  }
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::ReverseScalePos()
{
  if (_Fraction == _ZeroValue)
  {
/*
    if (ReversedScale())
    {
      if (_Quantity <= 0)
	_Quantity = -(_Quantity - 1);
      else
	_Quantity = -_Quantity + 1;
    }
    else
    {
*/
      if (_Quantity > 0)
	_Quantity = -(_Quantity - 1);
      else
	_Quantity = -_Quantity + 1;
//  }
  }
  else
  {
    _Quantity = -_Quantity;
    T UnitVal_(_Unit);
    _Fraction = UnitVal_ - _Fraction;
  }
/*
  if (ReversedScale())
    UnsetStatusBit(REVERSEDSCALE_BIT);
  else
    SetStatusBit(REVERSEDSCALE_BIT);
*/
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::ReverseScaleNeg()
{
  if (_Fraction == _ZeroValue)
    _Quantity = -(_Quantity + 1);
  else
  {
    _Quantity = -_Quantity - 2;
    T UnitVal_(_Unit);
    _Fraction = UnitVal_ - _Fraction;
  }
/*
  if (ReversedScale())
    UnsetStatusBit(REVERSEDSCALE_BIT);
  else
    SetStatusBit(REVERSEDSCALE_BIT);
*/
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::FindQuotientNeg()
{
  if (_Quantity >= -1)
  {
    _Quantity = _Quantity + 1;
    if (_Fraction < _ZeroValue && !Flipped())
      _Fraction = -_Fraction;
  }
  else
  {
    ReverseScaleNeg();
    _Quantity = -_Quantity;
    if (_Fraction > _ZeroValue && Flipped())
      _Fraction = -_Fraction;
  }
}

/****************************************************************************/
template <class T>
void SeriesCompare<T>::FindQuotientPos()
{
  if (_Quantity > 1 || _Quantity == 1 && _Fraction != _ZeroValue)
  {
    _Quantity = -_Quantity + 1;
    if (_Fraction > _ZeroValue && !Flipped())
      _Fraction = -_Fraction;
  }
  else if (_Quantity < 1 || _Quantity == 1 && _Fraction == _ZeroValue)
  {
    ReverseScalePos();
    if (_Fraction < _ZeroValue && Flipped())
      _Fraction = -_Fraction;
  }
}

/****************************************************************************/
template <class T>
SeriesCompare<T>& SeriesCompare<T>::FindQuotient()
{
  if (Zero())
  {
    SetStatusBit(DIVIDEBYZERO_BIT);
    return *this;
  }

  UnsetStatusBit(DIVIDEBYZERO_BIT);
  UnflipNegRange();
  UnflipPosRange();

  if (Negative())
    FindQuotientNeg();
  else
    FindQuotientPos();

  return *this;
}

/****************************************************************************/
template <class T>
SeriesCompare<T>& SeriesCompare<T>::SetMinValue(const T& MinVal_)
{
  _MinVal = MinVal_;

#if SERCMP_BOUNDS_CHECK
  if (MaxValSet())
    if (EqualRanges())
      assert(!_MinVal && !_MaxVal || _MinVal % _MaxVal == _ZeroValue);
    else if (NegScale())
      assert(!_MaxVal || (_MinVal % _MaxVal != _ZeroValue));
    else if (!NegScale())
      assert(!_MinVal || (_MaxVal % _MinVal != _ZeroValue));
#endif

  SetStatusBit(MINVALSET_BIT);
  return *this;
}

/****************************************************************************/
template <class T>
SeriesCompare<T>& SeriesCompare<T>::SetMaxValue(const T& MaxVal_)
{
  _MaxVal = MaxVal_;

#if SERCMP_BOUNDS_CHECK
  if (MinValSet())
    if (EqualRanges())
      assert(!_MinVal && !_MaxVal || _MinVal % _MaxVal == _ZeroValue);
    else if (NegScale())
      assert(!_MaxVal || (_MinVal % _MaxVal != _ZeroValue));
    else if (!NegScale())
      assert(!_MinVal || (_MaxVal % _MinVal != _ZeroValue));
#endif

  SetStatusBit(MAXVALSET_BIT);
  return *this;
}

/****************************************************************************/
template <class T>
SeriesCompare<T>& SeriesCompare<T>::SetCalcDiff()
{
  SetStatusBit(CALCDIFF_BIT);
  return *this;
}

/****************************************************************************/
template <class T>
SeriesCompare<T>& SeriesCompare<T>::operator = (const SeriesCompare<T>& Obj_)
{
  if (this != &Obj_)
  {
    _Unit = Obj_._Unit;
    _ZeroValue = Obj_._ZeroValue;
    _Quantity = Obj_._Quantity;
    _Fraction = Obj_._Fraction;
    _Result = Obj_._Result;
    _MinVal = Obj_._MinVal;
    _MaxVal = Obj_._MaxVal;
    _ValOffset = Obj_._ValOffset;
    _StatusBits = Obj_._StatusBits;
  }

  return *this;
}

/****************************************************************************/
/****************************************************************************/
#if SERCMP_DEBUG
int main()
{
#if SERCMP_DEBUG1
  short* List_[25];
  int i, RetVal_;

  for (i = 0; i < 10; ++i)
    List_[i] = new short(i+1);
  List_[10] = NULL;

  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(11, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(55, List_, 0, SHRT_MIN, SHRT_MAX).Result() == 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(77, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(1, List_, 0, SHRT_MIN, SHRT_MAX, TRUE).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-11, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-55, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-77, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::DifferenceOfSignedSeriesCmp(-11, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::DifferenceOfSignedSeriesCmp(-55, List_, 0, SHRT_MIN, SHRT_MAX).Result() == 0);
  assert(SeriesCompare<short>::DifferenceOfSignedSeriesCmp(-77, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);

  for (i = 0; i < 10; ++i)
    *List_[i] = -(*List_[i]);

  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(11, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(55, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(77, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(1, List_, 0, SHRT_MIN, SHRT_MAX, TRUE).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-11, List_, 0, SHRT_MIN, SHRT_MAX).Result() > 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-55, List_, 0, SHRT_MIN, SHRT_MAX).Result() == 0);
  assert(SeriesCompare<short>::SumOfSignedSeriesCmp(-77, List_, 0, SHRT_MIN, SHRT_MAX).Result() < 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(11, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(55, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(77, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(1, List_, 0, NumberingScheme::SIGNED_2SCOMP).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(-11, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() > 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(-55, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() == 0);
  assert(SeriesCompare<short>::SumOfSeriesCmp(-77, List_, 0, NumberingScheme::EQUAL_LEN_RANGES).Result() < 0);

  for (i = 0; i < 10; ++i)
    delete List_[i];

  List_[0] = new short(SHRT_MAX - 100);
  List_[1] = new short(50);
  List_[2] = new short(51);
  List_[3] = NULL;
  List_[4] = new short(*List_[0]);
  List_[5] = new short(50);
  List_[6] = new short(50);
  List_[7] = NULL;
  List_[8] = new short(SHRT_MIN + 100);
  List_[9] = new short(-50);
  List_[10] = new short(-51);
  List_[11] = NULL;
  List_[12] = new short(*List_[8]);
  List_[13] = new short(-50);
  List_[14] = new short(-50);
  List_[15] = NULL;
  List_[16] = new short(SHRT_MAX);
  List_[17] = new short(SHRT_MIN);
  List_[18] = new short(SHRT_MAX - 500);
  List_[19] = new short(500);
  List_[20] = new short(1);
  List_[21] = NULL;
  List_[22] = new short(100);
  List_[23] = new short(SHRT_MAX);

  SeriesCompare<short> Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, List_, 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() < 0);
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[4], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  assert(Sobj_.Result() == 0);
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;

  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MIN, &List_[8], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() > 0);
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MIN, &List_[12], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() == 0);

  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[16], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() == 0);

  delete List_[20];
  List_[20] = List_[22];
  List_[22] = NULL;
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[16], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() < 0);

  delete List_[20];
  List_[20] = List_[23];
  List_[23] = NULL;
  Sobj_ = SeriesCompare<short>::SumOfSignedSeriesCmp(SHRT_MAX, &List_[16], 0, SHRT_MIN, SHRT_MAX);
  Sobj_.FindQuotient();  
  cout <<Sobj_.Result() <<" " <<Sobj_.Unit() <<" " <<Sobj_.Quotient() <<" " <<Sobj_.Remainder() <<" " <<endl;
  assert(Sobj_.Result() < 0);

  for (i = 0; i < 24; ++i)
    delete List_[i];
#endif

#if SERCMP_DEBUG2
  short* nList_[25];
  int x;

  for (x = 0; x < 10; ++x)
    nList_[x] = new short(x+1);
  nList_[10] = NULL;
  SeriesCompare<short> Pobj_ = SeriesCompare<short>::ProductOfSignedSeriesCmp(1000, nList_, 0, SHRT_MIN, SHRT_MAX);
  cout <<Pobj_.Result() <<" " <<Pobj_.Unit() <<endl;
  assert(Pobj_.Result() < 0);

  for (x = 0; x < 10; ++x)
    if (x % 2)
      *nList_[x] = -(*nList_[x]);
  Pobj_ = SeriesCompare<short>::ProductOfSignedSeriesCmp(1000, nList_, 0, SHRT_MIN, SHRT_MAX);
  cout <<Pobj_.Result() <<" " <<Pobj_.Unit() <<endl;
  assert(Pobj_.Result() > 0);

  for (x = 0; x < 10; ++x)
    if (!(x % 2))
      *nList_[x] = -(*nList_[x]);
  Pobj_ = SeriesCompare<short>::ProductOfSignedSeriesCmp(1000, nList_, 0, SHRT_MIN, SHRT_MAX);
  cout <<Pobj_.Result() <<" " <<Pobj_.Unit() <<endl;
  assert(Pobj_.Result() < 0);

  *nList_[5] = 0;
  Pobj_ = SeriesCompare<short>::ProductOfSignedSeriesCmp(1000, nList_, 0, SHRT_MIN, SHRT_MAX);
  cout <<Pobj_.Result() <<" " <<Pobj_.Unit() <<endl;
  assert(Pobj_.Result() > 0);

  for (x = 0; x < 6; ++x)
    *nList_[x] = (x + 1) * 2;

  delete nList_[6];
  nList_[6] = NULL;

  Pobj_ = SeriesCompare<short>::ProductOfSignedSeriesCmp(1000, nList_, 0, SHRT_MIN, SHRT_MAX);
  cout <<Pobj_.Result() <<" " <<Pobj_.Unit() <<endl;
  assert(Pobj_.Result() < 0);

  for (x = 0; x < 10; ++x)
    delete nList_[x];

  long* lList_[7];
  for (x = 0; x < 6; ++x)
    lList_[x] = new long((x + 1) * 3);
  lList_[6] = NULL;

  SeriesCompare<long> Lpobj_ = SeriesCompare<long>::ProductOfSignedSeriesCmp(1000, lList_, 0, LONG_MIN, LONG_MAX);
  cout <<Lpobj_.Result() <<" " <<Lpobj_.Unit() <<endl;
  assert(Lpobj_.Result() < 0);

  for (x = 0; x < 6; ++x)
    delete lList_[x];
#endif

  cout <<endl <<"SUCCESS Testing Series Compare Class" <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif






