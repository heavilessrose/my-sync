#ifndef OBJECTCOUNTER_CPP
#define OBJECTCOUNTER_CPP
#ifndef OBJECTCOUNTER_H
  #include "objcount.h"
#endif

size_t ObjectCounter::_Count = 0;

/****************************************************************************/
ObjectCounter::ObjectCounter()
{
  ++_Count;
}

/****************************************************************************/
ObjectCounter::ObjectCounter(const ObjectCounter&)
{
  ++_Count;
}

/****************************************************************************/
ObjectCounter::~ObjectCounter()
{
  --_Count;
}

/****************************************************************************/
size_t ObjectCounter::GiveCount()
{
  return _Count;
}

/****************************************************************************/
#endif




