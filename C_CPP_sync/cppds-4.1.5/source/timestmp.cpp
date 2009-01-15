#ifndef TIMESTAMP_CPP
#define TIMESTAMP_CPP
#ifndef TIMESTAMP_H
  #include "timestmp.h"
#endif

size_t TimeStamp::_LastTime = 0;

/****************************************************************************/
TimeStamp::TimeStamp():
_Stamp(++_LastTime)
{}

/****************************************************************************/
#endif





