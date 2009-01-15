// datefwd.h
#ifndef DATESTUFF_H
#define DATESTUFF_H

#ifndef INCL_LIMITS_H
  #include <limits.h>
  #define INCL_LIMITS_H
#endif
#ifndef INCL_TIME_H
  #include "time.h"
  #define INCL_TIME_H
#endif

#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef GENFNCS2_H
  #include "genfncs2.h"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif

struct DateStuff
{
  static const int DaysInMonth[2][13];
  static const int DaysToDate[2][13];

  struct Duration
  {
    int _Year;
    int _Month;
    int _Day;

    Duration(int y=0, int m=0, int d=0):
      _Year(y),
      _Month(m),
      _Day(d) {}
  };

  // Constants and Data:
  enum YearLimits { MIN_YEAR = 1583, MAX_YEAR = INT_MAX };
  enum Days { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY,
              SUNDAY, DAY_ERROR };
  enum Months { MONTH_ERROR, JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE,
                JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER };

  static void xMonthError()
      { THROW (FallibleBase::InvalidMonthErr()); }
  static void xDateRangeError()
      { THROW (FallibleBase::DateRangeErr()); }
  static void xDayError()
      { THROW (FallibleBase::InvalidDayErr()); }
  static void xDateError()
      { THROW (FallibleBase::InvalidDateErr()); }

  // System Date:
  static void Today(int&, int&, int&);

  // Leap test
  static Boolean IsLeap(int y);

  // other validity checks
  static Boolean IsValidYear(int y);
  static Boolean IsValidMonth(int m);
  static Boolean IsValidYMonth(int y, int m);  
  static Boolean IsValidDay(int m, int d);
  static Boolean IsValidYMDay(int y, int m, int d);

  // various computations
  static int Compare(int y1, int m1, int d1,
                     int y2, int m2, int d2);
  static int DayOfWeek(int y, int m, int d);
  static int DayOfYear(int y, int m, int d);
  static int EndOfMonth(int y, int m);
  static int EndOfYear(int y);
  static void NthDay(int n, int y, int& m, int& d);
  static int NthWeekDay(int n, int wd, int y, int m);
  static int DaysInPrevMonth(int y, int m);
  static int DaysInNextMonth(int y, int m);
  static int GetDaysInMonth(Boolean IsLeap_, int m);
  static int GetDaysToDate(Boolean IsLeap_, int m);

  static int FirstSat(int dow);
  static int FirstSatOfMonth(int y, int m);
  static int FirstSatOfYear(int y);

  static int AbsoluteWeek(int doy);
  static int AbsoluteWeekOfMonth(int y, int m, int d);
  static int AbsoluteWeekOfYear(int y, int m, int d);
  static int CommonWeek(int s, int doyom);
  static int CommonWeekOfMonth(int y, int m, int d);
  static int CommonWeekOfYear(int y, int m, int d);
  static void NthCommonWeek(int n, int y, int& doy1, int& doy2);
  static int NumCommonWeeks(int y);

  static void AddYears(int yrs, int& y, int& m, int& d);
  static void SubtractYears(int yrs, int& y, int& m, int& d);
  static void AddMonths(int mths, int& y, int& m, int& d);
  static void SubtractMonths(int mths, int& y, int& m, int& d);
  static long MonthsBetween(int y1, int m1, int y2, int m2);
  static void AddDays(int days, int& y, int& m, int& d);
  static void SubtractDays(int days, int& y, int& m, int& d);
  static long DaysBetween(int y1, int m1, int d1, int y2, int m2, int d2);
  static void AddWeekDays(int wdays, int& y, int& m, int& d);
  static void SubtractWeekDays(int wdays, int& y, int& m, int& d);
  static long WeekDaysBetween(int y1, int m1, int d1, int y2, int m2, int d2);
  static void AddWeeks(int wks, int& y, int& m, int& d);
  static void SubtractWeeks(int wks, int& y, int& m, int& d);
  static long WeeksBetween(int y1, int m1, int d1, int y2, int m2, int d2);

  static Duration Age(int y, int m, int d);
  static Duration AgeBetween(int y1, int m1, int d1,
                             int y2, int m2, int d2);

  // Julian Day Conversions:
  static long GregToJul(int y, int m, int d);
  static void JulToGreg(long jd, int& y, int& m, int& d);
  static int DayOfWeek(long jd);
  static int DayOfYear(long jd);

  // Other conversions:
  static ChrString ToString(int y, int m, int d);
  static void FromString(const ChrString& Str_, int& y, int& m, int& d);

  // Misc.
  static void ResolveMonths(long mths, int& y, int& m);
  static void CheckY(int y);
  static void CheckYM(int y, int m);
  static void CheckYMD(int y, int m, int d);

  static unsigned MonthsToDays(unsigned month);
  static long YearsToDays(unsigned yr);
  static long YmdToScalar(unsigned yr, unsigned mo, unsigned day);
  static void ScalarToYmd(long scalar, unsigned *yr, unsigned *mo, unsigned *day);
  static void GetDtime(struct tm *dtime);
};

#endif




