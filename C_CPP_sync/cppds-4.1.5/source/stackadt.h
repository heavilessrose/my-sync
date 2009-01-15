#ifndef STACKADT_H
#define STACKADT_H
#ifndef LISTADT_H
  #include "listadt.h"
#endif

/****************************************************************************/
template <class BaseData>
class StackAdt: private ListAdt<BaseData>
{
  private:
    size_t _Limit;
  
  public:
    StackAdt();
    StackAdt(size_t Size_);
    StackAdt(const StackAdt<BaseData>& Obj_);
    ~StackAdt() {}

    StackAdt<BaseData>& operator = (const StackAdt<BaseData>& Obj_);

    BaseData Top();

    bool Empty();
    bool Full();

    void Push(const BaseData& Item_);
    void Pop();
};

/****************************************************************************/
template <class BaseData>
StackAdt<BaseData>::StackAdt():
_Limit(0)
{}

/****************************************************************************/
template <class BaseData>
StackAdt<BaseData>::StackAdt(size_t Size_):
_Limit(Size_)
{}

/****************************************************************************/
template <class BaseData>
StackAdt<BaseData>::StackAdt(const StackAdt<BaseData>& Obj_):
ListAdt<BaseData>(Obj_),
_Limit(Obj_._Limit)
{}

/****************************************************************************/
template <class BaseData>
StackAdt<BaseData>& StackAdt<BaseData>::operator = (const StackAdt<BaseData>& Obj_)
{
  ListAdt<BaseData>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
template <class BaseData>
BaseData StackAdt<BaseData>::Top()
{
  First();
  return Examine();
}

/****************************************************************************/
template <class BaseData>
bool StackAdt<BaseData>::Empty()
{
  return (Count() == 0);
}

/****************************************************************************/
template <class BaseData>
bool StackAdt<BaseData>::Full()
{
  if (!_Limit)
    return false;

  return (Count() >= _Limit);
}

/****************************************************************************/
template <class BaseData>
void StackAdt<BaseData>::Push(const BaseData& Item_)
{
  First();
  InsertBefore(Item_);
}

/****************************************************************************/
template <class BaseData>
void StackAdt<BaseData>::Pop()
{
  First();
  Remove();
}

/****************************************************************************/
#endif
