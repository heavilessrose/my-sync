#include <iostream.h>
#ifndef FULLDATE_H
  #include "fulldate.h"
#endif

void checkDate(int, int, int);
const char* DayText[] =
{
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday",
  "Sunday"
};

int main()
{
  checkDate(0, 3, 14);          // invalid year
  checkDate(1997, 0, 12);       // invalid month
  checkDate(1998, 13, 1);       // invalid month
  checkDate(97, 1, 12);         // invalid year
  checkDate(1999, 2, 29);       // invalid day
  checkDate(1900, 2, 29);       // invalid day

  Date empty(0, 0, 0);
  cout <<"empty: \"" <<empty.ToString() <<"\"\n";
  Date empty2(ChrString(""));
  cout <<"empty2: \"" <<empty2.ToString() <<"\"\n";

  Date d1 = ChrString("20000101");
  Date d2(2000, 3, 1);
  cout <<"Day of week for d1:" <<d1.ToString() <<" == "
       <<DayText[d1.DayOfWeek()] <<endl;
  cout <<"Day of week for d2:" <<d2.ToString() <<" == "
       <<DayText[d2.DayOfWeek()] <<endl;

  Date p1(1996, 2, 29);
  Date p2("20000229");

  cout <<"p1 == " <<p1.ToString() <<endl;
  cout <<"p2 == " <<p2.ToString() <<endl;

  DateStuff::Duration d = p1.AgeBetween(p2);
  cout <<"Duration between p1 and p2: "
       <<d._Year <<" years, "
       <<d._Month <<" months, "
       <<d._Day <<" days\n";

  DateStuff::Duration dd = p1.AgeBetween(d2);
  cout <<"Duration between p1 and d2 (20000301): "
       <<dd._Year <<" years, "
       <<dd._Month <<" months, "
       <<dd._Day <<" days\n";

  if (p1 < p2)
  {
    p1.AddDays(d._Day);
    p1.AddMonths(d._Month);
    p1.AddYears(d._Year);
  }
  else if (p1 > p2)
  {
    p2.AddDays(d._Day);
    p2.AddMonths(d._Month);
    p2.AddYears(d._Year);
  }

  cout <<"p1 == p2 (after adding Duration(p1,p2)): "
       <<((p1 == p2) ? "Yes":"No") <<endl;

  FullDate today;
  Date f(today);
  cout <<"Today: " <<f.ToString() <<endl;
  f.AddWeekDays(2);
  cout <<" +2 weekdays: " <<f.ToString() <<endl;
  f.AddWeekDays(10);
  cout <<" +10 weekdays: " <<f.ToString() <<endl;
  f.AddWeekDays(30);
  cout <<" +30 weekdays: " <<f.ToString() <<endl;
  f.SubtractWeekDays(30);
  cout <<" -30 weekdays: " <<f.ToString() <<endl;
  f.SubtractWeekDays(10);
  cout <<" -10 weekdays: " <<f.ToString() <<endl;
  f.SubtractWeekDays(2);
  cout <<" -2 weekdays: " <<f.ToString() <<endl;

  cout <<"WeekdaysBetween 12/11/96 and 12/13/96: "
       <<Date(1996, 12, 11).WeekDaysBetween(Date(1996, 12, 13))
       <<endl;
  cout <<"WeekdaysBetween 12/13/96 and 12/27/96: "
       <<Date(1996, 12, 13).WeekDaysBetween(Date(1996, 12, 27))
       <<endl;
  cout <<"WeekdaysBetween 12/27/96 and 2/7/97: "
       <<Date(1996, 12, 27).WeekDaysBetween(Date(1997, 2, 7))
       <<endl;

  FullDate f1(d1), f2(d2);
  DateStuff::Duration f12 = f2 - f1;
  cout <<"d1 - d1 == "
       <<f12._Year <<" years, "
       <<f12._Month <<" months, "
       <<f12._Day <<" days\n";
}

void checkDate(int year, int month, int day)
{
  try
  {
    Date(year, month, day);
    cout <<"Validity check FAILED" <<endl;    
  }
  catch (SciEngErr& x)
  {
    cout <<"Validity check PASSED" <<endl;
  }
}




