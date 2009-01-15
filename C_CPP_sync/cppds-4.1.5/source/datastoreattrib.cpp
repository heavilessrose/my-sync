#ifndef DATASTOREATTRIB_CPP
#define DATASTOREATTRIB_CPP
#ifndef DATASTOREATTRIB_H
  #include "datastoreattrib.h"
#endif

/****************************************************************************/
DataStorageAttributes::DataStorageAttributes():
_Attr(0)
{}

/****************************************************************************/
DataStorageAttributes::DataStorageAttributes(int Flags_):
_Attr(0)
{
  SetStorageAttributes(Flags_);
}

/****************************************************************************/
DataStorageAttributes::DataStorageAttributes(const DataStorageAttributes& Obj_):
_Attr(Obj_._Attr)
{}

/****************************************************************************/
DataStorageAttributes& DataStorageAttributes::SetStorageAttributes(const DataStorageAttributes& Obj_)
{
  _Attr = Obj_._Attr;
  return *this;
}

/****************************************************************************/
DataStorageAttributes& DataStorageAttributes::SetStorageAttributes(int Flags_)
{
  int All_ = (ACTIVE | DEEP_COPY | DESTROY);
  
  if (Flags_ == DEEP_COPY || Flags_ == (ACTIVE | DEEP_COPY))
    _Attr = All_;
  else
    _Attr = Flags_ & All_;

  if (_Attr)
    _Attr |= ACTIVE;

  return *this;
}

/****************************************************************************/
DataStorageAttributes& DataStorageAttributes::SetActive(Boolean Flag_)
{
  if (Flag_)
    _Attr |= ACTIVE;
  else
    _Attr = 0;
  
  return *this;
}

/****************************************************************************/
DataStorageAttributes& DataStorageAttributes::SetDeepCopy(Boolean Flag_)
{
  if (Flag_)
    _Attr |= (DEEP_COPY | ACTIVE);
  else
    _Attr &= ~DEEP_COPY;
  
  return *this;
}

/****************************************************************************/
DataStorageAttributes& DataStorageAttributes::SetDestroy(Boolean Flag_)
{
  if (Flag_)
    _Attr |= (DESTROY | ACTIVE);
  else
    _Attr &= ~DESTROY;
  
  return *this;
}

/****************************************************************************/
Boolean DataStorageAttributes::ShouldDeepCopy() const
{
  return ((_Attr & DEEP_COPY) != 0 &&
          (_Attr & ACTIVE) != 0);
}

/****************************************************************************/
Boolean DataStorageAttributes::ShouldDestroy() const
{
  return ((_Attr & DESTROY) != 0 &&
          (_Attr & ACTIVE) != 0);
}

/****************************************************************************/
Boolean DataStorageAttributes::IsActive() const
{
  return ((_Attr & ACTIVE) != 0);
}

/****************************************************************************/
int DataStorageAttributes::GetStorageAttributes() const
{
  return _Attr;
}

/****************************************************************************/
#endif




