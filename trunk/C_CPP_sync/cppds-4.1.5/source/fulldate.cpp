#ifndef FULLDATE_CPP
#define FULLDATE_CPP
#ifndef FULLDATE_H
  #include "fulldate.h"
#endif

/****************************************************************************/
FullDate::FullDate()
{
  DateStuff::Today(_Year, _Month, _Day);
}

/****************************************************************************/
FullDate::FullDate(const ChrString& s)
{
  DateStuff::FromString(s, _Year, _Month, _Day);
  
  if (!IsEmpty() && !IsValidYMDay())
    xDateError();    
}

/****************************************************************************/
FullDate::FullDate(int year, int month, int day)
{
  _Year = year;
  _Month = month;
  _Day = day;

  if (!IsEmpty() && !IsValidYMDay())
    xDateError();
}

/****************************************************************************/
FullDate::FullDate(const Date& Obj_)
{
  _Year = Obj_.GetYear();
  _Month = Obj_.GetMonth();
  _Day = Obj_.GetDay();

  assert(IsValid());
}

/****************************************************************************/
#endif




