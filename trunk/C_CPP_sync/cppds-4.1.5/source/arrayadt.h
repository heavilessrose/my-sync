#ifndef ARRAYADT_H
#define ARRAYADT_H
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif

/****************************************************************************/
template <class IndexType, class BaseData>
class ArrayAdt
{
  private:
    BaseData* arrayData;
    int loIndex;
    int hiIndex;
    bool outOfRange(IndexType i);

    inline void Xindex() const
        { THROW (FallibleBase::ArrayIndexRangeErr()); }

  public:
    ArrayAdt();
    ArrayAdt(IndexType lo, IndexType hi);
    ArrayAdt(const ArrayAdt<IndexType, BaseData>& Obj_);
    ~ArrayAdt();

    void SetRange(IndexType lo, IndexType hi);
    ArrayAdt<IndexType, BaseData>& operator = (const ArrayAdt<IndexType, BaseData>& Obj_);

    void Assign(IndexType i, const BaseData& val);
    BaseData Retrieve(IndexType i);
    BaseData& operator [] (IndexType i);

    IndexType LowIndex() const
        { return loIndex; }
    IndexType HighIndex() const
        { return hiIndex; }
};

/****************************************************************************/
template <class IndexType, class BaseData>
ArrayAdt<IndexType, BaseData>::ArrayAdt()
{
  arrayData = new BaseData[10];
  assert(arrayData != 0);

  loIndex = 0;
  hiIndex = 10;
}

/****************************************************************************/
template <class IndexType, class BaseData>
ArrayAdt<IndexType, BaseData>::ArrayAdt(IndexType lo, IndexType hi)
{
  arrayData = new BaseData[hi - lo + 1];
  assert(arrayData != 0);

  loIndex = lo;
  hiIndex = hi;
}

/****************************************************************************/
template <class IndexType, class BaseData>
ArrayAdt<IndexType, BaseData>::ArrayAdt(const ArrayAdt<IndexType, BaseData>& Obj_)
{
  arrayData = new BaseData[Obj_.hiIndex - Obj_.loIndex + 1];
  assert(arrayData != 0);

  loIndex = Obj_.loIndex;
  hiIndex = Obj_.hiIndex;

  int i;
  for (i = loIndex; i <= hiIndex; i++)
    arrayData[i] = Obj_.arrayData[i];
}

/****************************************************************************/
template <class IndexType, class BaseData>
ArrayAdt<IndexType, BaseData>::~ArrayAdt()
{
  delete[] arrayData;
}

/****************************************************************************/
template <class IndexType, class BaseData>
void ArrayAdt<IndexType, BaseData>::SetRange(IndexType lo, IndexType hi)
{
  delete[] arrayData;

  arrayData = new BaseData[hi - lo + 1];
  assert(arrayData != 0);

  loIndex = lo;
  hiIndex = hi;  
}

/****************************************************************************/
template <class IndexType, class BaseData>
ArrayAdt<IndexType, BaseData>&
ArrayAdt<IndexType, BaseData>::operator = (const ArrayAdt<IndexType, BaseData>& Obj_)
{
  arrayData = new BaseData[Obj_.hiIndex - Obj_.loIndex + 1];
  assert(arrayData != 0);

  loIndex = Obj_.loIndex;
  hiIndex = Obj_.hiIndex;

  int i;
  for (i = loIndex; i <= hiIndex; i++)
    arrayData[i] = Obj_.arrayData[i];

  return *this;
}

/****************************************************************************/
template <class IndexType, class BaseData>
bool ArrayAdt<IndexType, BaseData>::outOfRange(IndexType i)
{
  if ((i < loIndex) || (i > hiIndex))
  {
    cerr <<"Index " <<i <<" out of range" <<endl;
    return true;
  }

  return false;
}

/****************************************************************************/
template <class IndexType, class BaseData>
void ArrayAdt<IndexType, BaseData>::Assign(IndexType i, const BaseData& val)
{
  if (outOfRange(i))
    Xindex();
  else
    arrayData[i - loIndex] = val;
}

/****************************************************************************/
template <class IndexType, class BaseData>
BaseData& ArrayAdt<IndexType, BaseData>::operator [] (IndexType i)
{
  if (outOfRange(i))
    Xindex();

  return (arrayData[i - loIndex]);
}

/****************************************************************************/
template <class IndexType, class BaseData>
BaseData ArrayAdt<IndexType, BaseData>::Retrieve(IndexType i)
{
  if (outOfRange(i))
    Xindex();

  return (arrayData[i - loIndex]);
}

/****************************************************************************/
#endif
