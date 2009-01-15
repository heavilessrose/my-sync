#ifndef BUILTINSERIESCMP_CPP
#define BUILTINSERIESCMP_CPP
#ifndef BUILTINSERIESCMP_H
  #include "bltinseriescmp.h"
#endif

/****************************************************************************/
template <class T>
BuiltInSeriesCompare<T>::BuiltInSeriesCompare(T Unit_, T* List_, size_t Sz_, int Scheme_, T Zval_, T Disp_, T*(*AllocFn_)(T), Boolean DelParam_):
SeriesCompare<T>(Unit_ ? Unit_:Disp_, Zval_, (Unit_ == Zval_), Scheme_),
_AllocFn(AllocFn_),
_Size(Sz_),
#if OVERLOAD_NEW
_DataList((List_ && Sz_) ? (T**)RawAllocateWith(MEMMATRIX, sizeof(T*) * (Sz_ + 1)):NULL)
#else
_DataList((List_ && Sz_) ? (T**)RawAllocateWith(STD_NEW, sizeof(T*) * (Sz_ + 1)):NULL)
#endif
{
  _DataList[Sz_] = NULL;

  if (_Size)
  {
    size_t Index_;
    for (Index_ = 0; Index_ < Sz_; ++Index_)
      _DataList[Index_] = (*_AllocFn)(List_[Index_]);
  }

  if (DelParam_)
    if (Sz_ > 1)
      RawDeleteArray(List_);
    else
      RawDelete(List_);
}

/****************************************************************************/
template <class T>
BuiltInSeriesCompare<T>::BuiltInSeriesCompare(const BuiltInSeriesCompare<T>& Obj_):
SeriesCompare<T>(Obj_),
_AllocFn(Obj_._AllocFn),
_Size(Obj_._Size),
#if OVERLOAD_NEW
_DataList((Obj_._DataList && Obj_._Size) ? (T**)RawAllocateWith(MEMMATRIX, sizeof(T*) * (Obj_._Size + 1)):NULL)
#else
_DataList((Obj_._DataList && Obj_._Size) ? (T**)RawAllocateWith(STD_NEW, sizeof(T*) * (Obj_._Size + 1)):NULL)
#endif
{
  _DataList[_Size] = NULL;

  if (_Size)
  {
    size_t Index_;
    for (Index_ = 0; Index_ < _Size; ++Index_)
      _DataList[Index_] = Obj_._DataList[Index_] ?
	(*_AllocFn)(*Obj_._DataList[Index_]):NULL;
  }
}

/****************************************************************************/
template <class T>
void BuiltInSeriesCompare<T>::Clear()
{
  size_t Index_;
  for (Index_ = 0; Index_ < _Size; ++Index_)
    RawDelete(_DataList[Index_]);

  RawDelete(_DataList);
  _DataList = NULL;
  _Size = 0;
}

/****************************************************************************/
template <class T>
BuiltInSeriesCompare<T>::~BuiltInSeriesCompare()
{
  Clear();
}

/****************************************************************************/
template <class T>
BuiltInSeriesCompare<T>& BuiltInSeriesCompare<T>::SetMinValue(T MinVal_)
{
  SeriesCompare<T>::SetMinValue(MinVal_);
  return *this;
}

/****************************************************************************/
template <class T>
BuiltInSeriesCompare<T>& BuiltInSeriesCompare<T>::SetMaxValue(T MaxVal_)
{
  SeriesCompare<T>::SetMaxValue(MaxVal_);
  return *this;
}

/****************************************************************************/
template <class T>
BuiltInSeriesCompare<T>& BuiltInSeriesCompare<T>::operator = (const BuiltInSeriesCompare<T>& Obj_)
{
  if (this != &Obj_)
  {
    SeriesCompare<T>::operator = (Obj_);

    Clear();
    _AllocFn = Obj_._AllocFn;
    _Size = Obj_._Size;

#   if OVERLOAD_NEW
      _DataList = (Obj_._DataList && Obj_._Size) ? (T**)RawAllocateWith(MEMMATRIX, sizeof(T*) * (Obj_._Size + 1)):NULL;
#   else
      _DataList = (Obj_._DataList && Obj_._Size) ? (T**)RawAllocateWith(STD_NEW, sizeof(T*) * (Obj_._Size + 1)):NULL;
#   endif

    _DataList[_Size] = NULL;

    if (_Size)
    {
      size_t Index_;
      for (Index_ = 0; Index_ < _Size; ++Index_)
	_DataList[Index_] = Obj_._DataList[Index_] ?
	  (*_AllocFn)(*Obj_._DataList[Index_]):NULL;
    }
  }

  return *this;
}

/****************************************************************************/
template <class T>
BuiltInSeriesCompare<T>& BuiltInSeriesCompare<T>::FindQuotient()
{
  SeriesCompare<T>::FindQuotient();
  return *this;
}

/****************************************************************************/
#endif





