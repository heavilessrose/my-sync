#include "datestuff.h"
#include <iostream.h>

const char* DayText[] =
{ "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
  "Saturday", "Sunday" };

int main()
{
  int year = 1997;
  int month = 4;
  int day = 1;

//  cout.setf(ios::boolalpha);
  cout <<"Date: " <<month <<"/" <<day <<"/" <<year <<endl;
  cout <<"Leap Year? " <<DateStuff::IsLeap(year) <<endl;
  cout <<"Day of week: " <<DayText[DateStuff::DayOfWeek(year, month, day)] <<endl;
  cout <<"Day of year: " <<DateStuff::DayOfYear(year, month, day) <<endl;
  cout <<"Days in month: " <<DateStuff::EndOfMonth(year, month) <<endl;
  cout <<"Days in previous month: " <<DateStuff::DaysInPrevMonth(year, month) <<endl;
  cout <<"Days in next month: " <<DateStuff::DaysInNextMonth(year, month) <<endl;
  cout <<"3rd Saturday of month: " <<DateStuff::NthWeekDay(3, DateStuff::SATURDAY, year, month) <<endl;
  long jday = DateStuff::GregToJul(year, month, day);
  cout <<"Julian day number: " <<jday <<endl;
  int year2, month2, day2;
  DateStuff::JulToGreg(jday + 100, year2, month2, day2);
  cout <<"100 days frm now: " <<month2 <<"/" <<day2 <<"/" <<year2 <<endl;

  return 0;
}
