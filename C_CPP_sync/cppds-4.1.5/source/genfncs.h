#ifndef GENFNCS_H
#define GENFNCS_H
#ifndef MATHFNCS_H
  #include "mathfncs.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif

/****************************************************************************/
// Generic Template Functions
/****************************************************************************/
/****************************************************************************/
template <class T>
long _SizeOfData(const T& Obj_)
{
  return long(sizeof(Obj_));
}

/****************************************************************************/
template <class T>
long SizeOfData(const T& Obj_)
{
  return _SizeOfData(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
long _FormWidthOfObject(const T& Obj_)
{
  return (long(sizeof(long) +
	       sizeof(Byte_t)) +
	  SizeOfData(Obj_));
}

/****************************************************************************/
template <class T>
long FormWidthOfObject(const T& Obj_)
{
  return _FormWidthOfObject(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
const char* _HashableData(const T& Obj_)
{
  return ((const char*)&Obj_);
}

/****************************************************************************/
template <class T>
const char* HashableData(const T& Obj_)
{
  return _HashableData(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
long _HashableDataLength(const T& Obj_)
{
  return sizeof(Obj_);
}

/****************************************************************************/
template <class T>
long HashableDataLength(const T& Obj_)
{
  return _HashableDataLength(RootConstObject(Obj_));
}

/****************************************************************************/
// Nested pointer determination function.
// Seems only to work (compile) under Turbo C and MicroSoft C
#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(_MSC_VER)
template <class T>
Boolean IsNestedPtr(const T&, size_t Level_)
{ return (Level_ > 1); }

template <class T>
Boolean IsNestedPtr(const T&)
{ return FALSE; }

template <class T>
Boolean IsNestedPtr(T* Ptr_, size_t Level_)
{
  if (Level_)
    return TRUE;

  return (Ptr_ ? IsNestedPtr(*Ptr_, size_t(Level_ + 1)):FALSE);
}

template <class T>
Boolean IsNestedPtr(T* Ptr_)
{
  return (Ptr_ ? IsNestedPtr(*Ptr_, size_t(1)):FALSE);
}

_FNC_INLINE Boolean IsNestedPtr(void*, size_t Level_)
{ return (Level_ > 0); }

_FNC_INLINE Boolean IsNestedPtr(void*)
{ return FALSE; }
#endif

/****************************************************************************/
template <class T>
T* _Clone(const T& Obj_)
{
  return (new T(Obj_));
}

/****************************************************************************/
template <class T>
T* Clone(const T& Obj_)
{
  return ((T*)_Clone(RootConstObject(Obj_)));
}

/****************************************************************************/
template <class T>
const T* _ConstClone(const T& Obj_)
{
  return &Obj_;
}

/****************************************************************************/
template <class T>
const T* ConstClone(const T& Obj_)
{
  return ((const T*)_ConstClone(RootConstObject(Obj_)));
}

/****************************************************************************/
template <class T>
T* _NonConstClone(T& Obj_)
{
  return &Obj_;
}

/****************************************************************************/
template <class T>
T* NonConstClone(T& Obj_)
{
  return ((T*)_NonConstClone(RootObject(Obj_)));
}

/****************************************************************************/
template <class T>
T& _Assign(T& trg, const T& src)
{
  trg = src;
  return trg;
}

/****************************************************************************/
template <class T>
T& Assign(T& trg, const T& src)
{
  _Assign(RootObject(trg), RootConstObject(src));
  return trg;
}

/****************************************************************************/
template <class T>
T& _NonConstAssign(T& Trg_, T& Src_)
{
  Trg_ = Src_;
  return Trg_;
}

/****************************************************************************/
template <class T>
T& NonConstAssign(T& Trg_, T& Src_)
{
  _NonConstAssign(RootObject(Trg_), RootObject(Src_));
  return Trg_;
}

/****************************************************************************/
/// Object ID functions
template <class T>
size_t _CountOf(const T& Obj_, Boolean* Determinable_)
{
  if (Determinable_)
    *Determinable_ = FALSE;

  return 0;
}

/****************************************************************************/
template <class T>
size_t CountOf(const T& Obj_, Boolean* Determinable_)
{
  return _CountOf(RootConstObject(Obj_), Determinable_);
}

/****************************************************************************/
template <class T>
size_t CountOf(const T& Obj_)
{
  return _CountOf(RootConstObject(Obj_), ((Boolean*)0));
}

/****************************************************************************/
template <class T>
Boolean _IsNullObject(const T& Obj_, Boolean* Determinable_)
{
  if (Determinable_)
    *Determinable_ = FALSE;

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsNullObject(const T& Obj_, Boolean* Determinable_)
{
  return _IsNullObject(RootConstObject(Obj_), Determinable_);
}

/****************************************************************************/
template <class T>
Boolean IsNullObject(const T& Obj_)
{
  return _IsNullObject(RootConstObject(Obj_), ((Boolean*)0));
}

/****************************************************************************/
/// Object information function
template <class T>
ostream& _ShowInfo(ostream& Os_, const T& Obj_)
{
  Os_ <<endl <<"Class Name  : " <<NameOf(Obj_)
      <<endl <<"Class I.D.  : " <<WhatIs(Obj_)
      <<endl <<"Object I.D. : Indeterminate";

  return Os_;
}

/****************************************************************************/
template <class T>
ostream& ShowInfo(ostream& Os_, const T& Obj_)
{
  return _ShowInfo(Os_, RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsSummable(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsSummable(const T& Obj_)
{
  return _IsSummable(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsMultipliable(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsMultipliable(const T& Obj_)
{
  return _IsMultipliable(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsOrdinal(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsOrdinal(const T& Obj_)
{
  return _IsOrdinal(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsInvertable(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsInvertable(const T& Obj_)
{
  return _IsInvertable(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsLogical(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsLogical(const T& Obj_)
{
  return _IsLogical(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsBitwiseable(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsBitwiseable(const T& Obj_)
{
  return _IsBitwiseable(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsStreamable(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsStreamable(const T& Obj_)
{
  return _IsStreamable(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsSortable(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsSortable(const T& Obj_)
{
  return _IsSortable(RootConstObject(Obj_));
}

/****************************************************************************/
template <class T>
Boolean _IsMatchable(const T& Obj_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsMatchable(const T& Obj_)
{
  return _IsMatchable(RootConstObject(Obj_));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& _Minus(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& Minus(T& Trg_, const T& Src_)
{
  return ((T&)(::IsSummable(Trg_) ? _Minus(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
T& _Add(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& Add(T& Trg_, const T& Src_)
{
  return ((T&)(::IsSummable(Trg_) ? _Add(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
T& _Divide(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& Divide(T& Trg_, const T& Src_)
{
  return ((T&)(::IsMultipliable(Trg_) ? _Divide(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
T& _Modulo(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& Modulo(T& Trg_, const T& Src_)
{
  return ((T&)((::IsMultipliable(Trg_) && ::IsOrdinal(Trg_)) ? _Modulo(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
T& _Multiply(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& Multiply(T& Trg_, const T& Src_)
{
  return ((T&)(::IsMultipliable(Trg_) ? _Multiply(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
const T& _Increment(T& Trg_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
const T& Increment(T& Trg_)
{
  return ((const T&)(::IsOrdinal(Trg_) ? _Increment(Trg_):Trg_));
}

/****************************************************************************/
template <class T>
const T& _Decrement(T& Trg_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
const T& Decrement(T& Trg_)
{
  return ((const T&)(::IsOrdinal(Trg_) ? _Decrement(Trg_):Trg_));
}

/****************************************************************************/
template <class T>
Boolean _AtZero(const T& Trg_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean AtZero(const T& Trg_)
{
  return _AtZero(RootConstObject(Trg_));
}

/****************************************************************************/
template <class T>
Boolean _NotZero(const T& Trg_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean NotZero(const T& Trg_)
{
  return _NotZero(RootConstObject(Trg_));
}

/****************************************************************************/
template <class T>
T& _Invert(T& Trg_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& Invert(T& Trg_)
{
  return ((T&)(::IsInvertable(Trg_) ? _Invert(Trg_):Trg_));
}

/****************************************************************************/
template <class T>
Boolean _LogicalAnd(const T& Trg_, const T& Src_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean LogicalAnd(const T& Trg_, const T& Src_)
{
  return ((T&)(::IsLogical(Trg_) ? _LogicalAnd(Trg_, Src_):FALSE));
}

/****************************************************************************/
template <class T>
Boolean _LogicalOr(const T& Trg_, const T& Src_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean LogicalOr(const T& Trg_, const T& Src_)
{
  return ((T&)(::IsLogical(Trg_) ? _LogicalOr(Trg_, Src_):FALSE));
}

/****************************************************************************/
template <class T>
Boolean _LogicalNot(const T& Src_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean LogicalNot(const T& Src_)
{
  return ((T&)(::IsLogical(Src_) ? _LogicalNot(Src_):FALSE));
}

/****************************************************************************/
template <class T>
T& _BitwiseAnd(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& BitwiseAnd(T& Trg_, const T& Src_)
{
  return ((T&)(::IsBitwiseable(Trg_) ? _BitwiseAnd(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
T& _BitwiseOr(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& BitwiseOr(T& Trg_, const T& Src_)
{
  return ((T&)(::IsBitwiseable(Trg_) ? _BitwiseOr(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
T& _BitwiseXor(T& Trg_, const T& Src_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& BitwiseXor(T& Trg_, const T& Src_)
{
  return ((T&)(::IsBitwiseable(Trg_) ? _BitwiseXor(Trg_, Src_):Trg_));
}

/****************************************************************************/
template <class T>
T& _BitwiseInvert(T& Trg_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& BitwiseInvert(T& Trg_)
{
  return ((T&)(::IsBitwiseable(Trg_) ? _BitwiseInvert(Trg_):Trg_));
}

/****************************************************************************/
template <class T>
T& _BitwiseLeftShift(T& Trg_, size_t Shift_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& BitwiseLeftShift(T& Trg_, size_t Shift_)
{
  return ((T&)(::IsBitwiseable(Trg_) ? _BitwiseLeftShift(Trg_, Shift_):Trg_));
}

/****************************************************************************/
template <class T>
T& _BitwiseRightShift(T& Trg_, size_t Shift_)
{
  return Trg_;
}

/****************************************************************************/
template <class T>
T& BitwiseRightShift(T& Trg_, size_t Shift_)
{
  return ((T&)(::IsBitwiseable(Trg_) ? _BitwiseRightShift(Trg_, Shift_):Trg_));
}

/****************************************************************************/
template <class T>
Boolean _IsGreaterThan(const T& Trg_, const T& Src_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsGreaterThan(const T& Trg_, const T& Src_)
{
  return (::IsSortable(Trg_) ? _IsGreaterThan(RootConstObject(Trg_), RootConstObject(Src_)):FALSE);
}

/****************************************************************************/
template <class T>
Boolean _IsLessThan(const T& Trg_, const T& Src_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsLessThan(const T& Trg_, const T& Src_)
{
  return (::IsSortable(Trg_) ? _IsLessThan(RootConstObject(Trg_), RootConstObject(Src_)):FALSE);
}

/****************************************************************************/
template <class T>
Boolean _IsEqualTo(const T& Trg_, const T& Src_)
{
  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean IsEqualTo(const T& Trg_, const T& Src_)
{
  return (::IsMatchable(Trg_) ? _IsEqualTo(RootConstObject(Trg_), RootConstObject(Src_)):FALSE);
}

/****************************************************************************/
template <class T>
ostream& _WriteToOutput(ostream& os, const T& Obj_)
{
  os.write((char*)(&Obj_), sizeof(Obj_));
  return os;
}

/****************************************************************************/
template <class T>
ostream& WriteToOutput(ostream& os, const T& Obj_)
{
  return (::IsStreamable(Obj_) ? _WriteToOutput(os, RootConstObject(Obj_)):os);
}

/****************************************************************************/
template <class T>
istream& _ReadFromInput(istream& is, T& Obj_)
{
  is.read((char*)(&Obj_), sizeof(Obj_));
  return is;
}

/****************************************************************************/
template <class T>
istream& ReadFromInput(istream& is, T& Obj_)
{
  return (::IsStreamable(Obj_) ? _ReadFromInput(is, RootObject(Obj_)):is);
}

/****************************************************************************/
template <class T>
T& _Sine(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& Sine(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _Sine(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _Cosine(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& Cosine(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _Cosine(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _Tangent(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& Tangent(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _Tangent(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _ArcSine(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& ArcSine(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _ArcSine(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _ArcCosine(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& ArcCosine(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _ArcCosine(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _ArcTangent(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& ArcTangent(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _ArcTangent(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _ArcTangent2(T& Val1_, const T& Val2_)
{
  return Val1_;
}

/****************************************************************************/
template <class T>
T& ArcTangent2(T& Val1_, const T& Val2_)
{
  return ((T&)(::IsMultipliable(Val1_) ? _ArcTangent2(Val1_, Val2_):Val1_));
}

/****************************************************************************/
template <class T>
T& _Exponent(T& Base_, const T& Exp_)
{
  return Base_;
}

/****************************************************************************/
template <class T>
T& Exponent(T& Base_, const T& Exp_)
{
  return ((T&)(::IsMultipliable(Base_) ? _Exponent(Base_, Exp_):Base_));
}

/****************************************************************************/
template <class T>
T& _HyperSine(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& HyperSine(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _Hypersine(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _HyperCosine(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& HyperCosine(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _HyperCosine(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _HyperTangent(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& HyperTangent(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _HyperTangent(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _NaturalExp(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& NaturalExp(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _NaturalExp(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _NaturalLog(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& NaturalLog(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _NaturalLog(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _Log10(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& Log10(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _Log10(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _Ceiling(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& Ceiling(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _Ceiling(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _Floor(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& Floor(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _Floor(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _AbsoluteValue(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& AbsoluteValue(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _AbsoluteValue(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _SquareRoot(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& SquareRoot(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _SquareRoot(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _FractionPart(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& FractionPart(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _FractionPart(Val_):Val_));
}

/****************************************************************************/
template <class T>
T& _IntegerPart(T& Val_)
{
  return Val_;
}

/****************************************************************************/
template <class T>
T& IntegerPart(T& Val_)
{
  return ((T&)(::IsMultipliable(Val_) ? _IntegerPart(Val_):Val_));
}

/****************************************************************************/
#endif




