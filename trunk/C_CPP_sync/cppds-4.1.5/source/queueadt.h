#ifndef QUEUEADT_H
#define QUEUEADT_H
#ifndef LISTADT_H
  #include "listadt.h"
#endif

/****************************************************************************/
template <class BaseData>
class QueueAdt: private ListAdt<BaseData>
{
  private:
    size_t _Limit;
  
  public:
    QueueAdt();
    QueueAdt(size_t Size_);
    QueueAdt(const QueueAdt<BaseData>& Obj_);
    ~QueueAdt() {}

    QueueAdt<BaseData>& operator = (const QueueAdt<BaseData>& Obj_);

    BaseData Front();

    bool Empty();
    bool Full();

    void Enqueue(const BaseData& Item_);
    void Dequeue();
};

/****************************************************************************/
template <class BaseData>
QueueAdt<BaseData>::QueueAdt():
_Limit(0)
{}

/****************************************************************************/
template <class BaseData>
QueueAdt<BaseData>::QueueAdt(size_t Size_):
_Limit(Size_)
{}

/****************************************************************************/
template <class BaseData>
QueueAdt<BaseData>::QueueAdt(const QueueAdt<BaseData>& Obj_):
ListAdt<BaseData>(Obj_),
_Limit(Obj_._Limit)
{}

/****************************************************************************/
template <class BaseData>
QueueAdt<BaseData>& QueueAdt<BaseData>::operator = (const QueueAdt<BaseData>& Obj_)
{
  ListAdt<BaseData>::operator = (Obj_);
  return *this;
}

/****************************************************************************/
template <class BaseData>
BaseData QueueAdt<BaseData>::Front()
{
  First();
  return Examine();
}

/****************************************************************************/
template <class BaseData>
bool QueueAdt<BaseData>::Empty()
{
  return (Count() == 0);
}

/****************************************************************************/
template <class BaseData>
bool QueueAdt<BaseData>::Full()
{
  if (!_Limit)
    return false;

  return (Count() >= _Limit);
}

/****************************************************************************/
template <class BaseData>
void QueueAdt<BaseData>::Enqueue(const BaseData& Item_)
{
  Last();
  InsertAfter(Item_);  
}

/****************************************************************************/
template <class BaseData>
void QueueAdt<BaseData>::Dequeue()
{
  First();
  Remove();
}

/****************************************************************************/
#endif
