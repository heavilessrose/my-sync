#ifndef DATESTUFF_CPP
#define DATESTUFF_CPP
#ifndef DATESTUFF_H
  #include "datestuff.h"
#endif

const int DateStuff::DaysInMonth[2][13] =
{
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

const int DateStuff::DaysToDate[2][13] =
{
  {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
  {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

/****************************************************************************/
Boolean DateStuff::IsLeap(int y)
{
  return
  (
    (y >= 1582) ?
      (y % 4 == 0  &&  y % 100 != 0  ||  y % 400 == 0 ):
      (y % 4 == 0)
  );  
}

/****************************************************************************/
Boolean DateStuff::IsValidYear(int y)
{
  return (MIN_YEAR <= y && y <= MAX_YEAR);
}

/****************************************************************************/
Boolean DateStuff::IsValidMonth(int m)
{
  return (1 <= m && m <= 12);  
}

/****************************************************************************/
Boolean DateStuff::IsValidYMonth(int y, int m)
{
  return (IsValidYear(y) && IsValidMonth(m));
}

/****************************************************************************/
Boolean DateStuff::IsValidDay(int m, int d)
{
  return (IsValidMonth(m) &&
          1 <= d && d <= GetDaysInMonth(TRUE, m));
}

/****************************************************************************/
Boolean DateStuff::IsValidYMDay(int y, int m, int d)
{
  return (IsValidYMonth(y, m) &&
          1 <= d && d <= EndOfMonth(y, m));
}

/****************************************************************************/
int DateStuff::DayOfWeek(long jd)
{
  return int(jd % 7L);
}

/****************************************************************************/
int DateStuff::DayOfWeek(int y, int m, int d)
{
  CheckYMD(y, m, d);
  return int(GregToJul(y, m, d) % 7);
}

/****************************************************************************/
int DateStuff::DayOfYear(long jd)
{
  int y, m, d;
  JulToGreg(jd, y, m, d);
  return DayOfYear(y, m, d);
}

/****************************************************************************/
int DateStuff::DayOfYear(int y, int m, int d)
{
  CheckYMD(y, m, d);
  return int(GetDaysToDate(IsLeap(y), m) + d);
}

/****************************************************************************/
int DateStuff::EndOfMonth(int y, int m)
{
  CheckYM(y, m);
  return GetDaysInMonth(IsLeap(y), m);
}

/****************************************************************************/
int DateStuff::EndOfYear(int y)
{
  CheckY(y);
  return GetDaysToDate(IsLeap(y), 13);
}

/****************************************************************************/
int DateStuff::AbsoluteWeek(int d)
{
  assert(1 <= d && d <= 366);
  return ((d - 1) / 7 + 1);
}

/****************************************************************************/
int DateStuff::AbsoluteWeekOfYear(int y, int m, int d)
{
  CheckYMD(y, m, d);
  return AbsoluteWeek(DayOfYear(y, m, d));
}

/****************************************************************************/
int DateStuff::AbsoluteWeekOfMonth(int y, int m, int d)
{
  CheckYMD(y, m, d);
  return AbsoluteWeek(d);
}

/****************************************************************************/
int DateStuff::CommonWeekOfMonth(int y, int m, int d)
{
  CheckYMD(y, m, d);
  return CommonWeek(FirstSatOfMonth(y, m), d);
}

/****************************************************************************/
int DateStuff::CommonWeekOfYear(int y, int m, int d)
{
  CheckYMD(y, m, d);
  return CommonWeek(FirstSatOfYear(y), DayOfYear(y, m, d));
}

/****************************************************************************/
int DateStuff::FirstSatOfYear(int y)
{
  CheckY(y);
  return FirstSat(DayOfWeek(y, 1, 1));
}

/****************************************************************************/
int DateStuff::FirstSatOfMonth(int y, int m)
{
  CheckYM(y, m);
  return FirstSat(DayOfWeek(y, m, 1));
}

/****************************************************************************/
int DateStuff::Compare(int y1, int m1, int d1, int y2, int m2, int d2)
{
  // returns the # of days between two dates (signed)
  return (GregToJul(y1, m1, d1) - GregToJul(y2, m2, d2));
}

/****************************************************************************/
long DateStuff::DaysBetween(int y1, int m1, int d1, int y2, int m2, int d2)
{
  // A synonym for Compare
  return labs(Compare(y1, m1, d1, y2, m2, d2));
}

/****************************************************************************/
int DateStuff::NumCommonWeeks(int y)
{
  CheckY(y);
  return ((EndOfYear(y) - FirstSatOfYear(y) - 1) / 7 + 2);
}

/****************************************************************************/
long DateStuff::MonthsBetween(int y1, int m1, int y2, int m2)
{
  CheckYM(y1, m1);
  CheckYM(y2, m2);
  return labs((y1 - y2) * 12L + m1 - m2);
}

/****************************************************************************/
void DateStuff::AddDays(int days, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  JulToGreg(GregToJul(y, m, d) + days, y, m, d);
}

/****************************************************************************/
void DateStuff::SubtractDays(int days, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  JulToGreg(GregToJul(y, m, d) - days, y, m, d);
}

/****************************************************************************/
void DateStuff::AddWeeks(int wks, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  AddDays(wks * 7, y, m, d);
}

/****************************************************************************/
void DateStuff::SubtractWeeks(int wks, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  SubtractDays(wks * 7, y, m, d);
}

/****************************************************************************/
long DateStuff::WeeksBetween(int y1, int m1, int d1, int y2, int m2, int d2)
{
  CheckYMD(y1, m1, d1);
  CheckYMD(y2, m2, d2);
  return labs(DaysBetween(y1, m1, d1, y2, m2, d2) / 7L);
}

/****************************************************************************/
int DateStuff::GetDaysInMonth(Boolean IsLeap_, int m)
{
  int Index_ = IsLeap_ ? 1:0;
  if (!IsValidMonth(m))
    xMonthError();

  return DaysInMonth[Index_][m];
}

/****************************************************************************/
int DateStuff::GetDaysToDate(Boolean IsLeap_, int m)
{
  int Index_ = IsLeap_ ? 1:0;
  if (m < 1 || 13 < m)
    xMonthError();

  return DaysToDate[Index_][m - 1];
}

/****************************************************************************/
long DateStuff::GregToJul(int year, int month, int day)
{
  CheckYMD(year, month, day);
  long m = long(month);
  long d = long(day);
  long y = long(year);

  return
  (
    d - 32075L
    + 1461L * (y + 4800 + (m - 14L)/12L) / 4L
    + 367L * (m - 2L - (m - 14L)/12L * 12L) / 12L
    - 3L * ((y + 4900L + (m - 14L)/12L) / 100L) / 4L
  );
}

/****************************************************************************/
void DateStuff::JulToGreg(long jday, int& year, int& month, int& day)
{
  long t1 = jday + 68569L;
  long t2 = 4L * t1 / 146097L;
  t1 -= (146097L * t2 + 3L) / 4L;

  long y = 4000L * (t1 + 1) / 1461001L;
  t1 = t1 - 1461L * y / 4L + 31;

  long m = 80L * t1 / 2447L;
  day = int(t1 - 2447L * m / 80L);

  t1 = m / 11L;
  month = int(m + 2L - 12L * t1);

  year = int(100L * (t2 - 49L) + y + t1);
}

/****************************************************************************/
void DateStuff::NthDay(int n, int year, int& month, int& day)
{
  CheckY(year);
  if (n < 1 || 366 < n)
    xDayError();

  int row = IsLeap(year);
  if (n > DaysToDate[row][12])
    xDateRangeError();

  for (month = 0; month < 13; ++month)
    if (DaysToDate[row][month] >= n)
      break;

  if (DaysToDate[row][month] > n)
    day = int(n - DaysToDate[row][month - 1]);
  else
    day = int(DaysToDate[row][month] - DaysToDate[row][month-1]);
}

/****************************************************************************/
int DateStuff::NthWeekDay(int n, int weekDay, int year, int month)
{
  CheckYM(year, month);
  if (!(1 <= n && n <= 5) ||
      !(MONDAY <= weekDay && weekDay <= SUNDAY))
    xDayError();

  long jday = GregToJul(year, month, 1);
  while (int(jday % 7L) != weekDay)
    ++jday;

  int weekno;
  for (weekno = 1; weekno < n; weekno++)
    jday += 7L;

  int day;
  int tempmonth = month;
  JulToGreg(jday, year, month, day);
  
  if (month != tempmonth)
    xDateRangeError();

  return day;
}

/****************************************************************************/
int DateStuff::DaysInPrevMonth(int year, int month)
{
  CheckYM(year, month);
  if (month == 1)
  {
    --year;
    month = 12;
  }
  else
    --month;

  int Index_ = IsLeap(year) ? 1:0;
  return DaysInMonth[Index_][month];
}

/****************************************************************************/
int DateStuff::DaysInNextMonth(int year, int month)
{
  CheckYM(year, month);
  if (month == 12)
  {
    ++year;
    month = 1;
  }
  else
    ++month;

  int Index_ = IsLeap(year) ? 1:0;
  return DaysInMonth[Index_][month];
}

/****************************************************************************/
int DateStuff::FirstSat(int dow)
{
  assert(MONDAY <= dow && dow <= SUNDAY);
  if (dow == SUNDAY)
    return 7;
  else if (dow == SATURDAY)
    return 1;
  else
    return (SATURDAY - dow + 1);
}

/****************************************************************************/
int DateStuff::CommonWeek(int s, int d)
{
  assert(1 <= s && s <= 7);
  assert(1 <= d && d <= 365);
  if (d <= s)
    return 1;
  else
    return (2 + (d - s - 1) / 7);
}

/****************************************************************************/
void DateStuff::Today(int& y, int& m, int& d)
{
  time_t tval = time(0);
  struct tm* tmp = localtime(&tval);

  d = tmp->tm_mday;
  m = tmp->tm_mon + 1;
  y = tmp->tm_year + 1900;
}

/****************************************************************************/
void DateStuff::NthCommonWeek(int n, int y, int& doy1, int& doy2)
{
  if (!(1 <= doy1 && doy1 <= EndOfYear(y)) ||
      !(1 <= doy2 && doy2 <= EndOfYear(y)))
    xDayError();

  int s = FirstSatOfYear(y);
  if (n <= 1)
  {
    doy1 = 1;
    doy2 = s;
  }
  else
  {
    int nc = NumCommonWeeks(y);
    if (n > nc)
      n = nc;
     
    doy1 = s + 1 + 7 * (n - 2);
    doy2 = (n == nc) ? EndOfYear(y) : s + 7 * (n - 1);
  }
}

/****************************************************************************/
void DateStuff::AddYears(int yrs, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  if (y > MAX_YEAR - yrs)
    xDateRangeError();

  y += yrs;
  int eom_ = EndOfMonth(y, m);  
  d = (d < eom_) ? d:eom_;
}

/****************************************************************************/
void DateStuff::SubtractYears(int yrs, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  if (y < MIN_YEAR + yrs)
    xDateRangeError();

  y -= yrs;
  int eom_ = EndOfMonth(y, m);  
  d = (d < eom_) ? d:eom_;  
}

/****************************************************************************/
void DateStuff::AddMonths(int mths, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  ResolveMonths(y * 12 + m + mths, y, m);

  int eom_ = EndOfMonth(y, m);  
  d = (d < eom_) ? d:eom_;
}

/****************************************************************************/
void DateStuff::SubtractMonths(int mths, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  ResolveMonths(y * 12 + m - mths, y, m);

  int eom_ = EndOfMonth(y, m);  
  d = (d < eom_) ? d:eom_;  
}

/****************************************************************************/
void DateStuff::AddWeekDays(int wdays, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  // NOTE: this algorithm uses the fact the MONDAY == 0

  // Back-up to nearest weekday
  int dayno = DayOfWeek(y,m,d);
  if (dayno > FRIDAY)
  {
    SubtractDays(dayno - FRIDAY, y, m, d);
    dayno = FRIDAY;
  }

  if (wdays >= 5)
    AddWeeks(wdays / 5, y, m, d);

  int extra = int(wdays % 5);
  if (dayno + extra > FRIDAY)
    extra += 2;
  if (extra > 0)
    AddDays(extra, y, m, d);    
}

/****************************************************************************/
void DateStuff::SubtractWeekDays(int wdays, int& y, int& m, int& d)
{
  CheckYMD(y, m, d);
  // NOTE: this algorithm uses the fact the MONDAY == 0

  // Advance to nearest weekday
  int dayno = DayOfWeek(y, m, d);
  if (dayno > FRIDAY)
  {
    AddDays(7 - dayno, y, m, d);
    dayno = MONDAY;
  }

  // Subtract weeks first
  SubtractWeeks(wdays / 5, y, m, d);

  // Now back up the rest of the days
  int extra = int(wdays % 5);
  if (dayno - extra < MONDAY)
    extra += 2;

  SubtractDays(extra, y, m, d);
}

/****************************************************************************/
long DateStuff::WeekDaysBetween(int y1, int m1, int d1,
                                int y2, int m2, int d2)
{
  CheckYMD(y1, m1, d1);
  CheckYMD(y2, m2, d2);

  int dayno1 = DayOfWeek(y1, m1, d1);
  if (dayno1 > FRIDAY)
  {
    SubtractDays(dayno1 - FRIDAY, y1, m1, d1);
    dayno1 = FRIDAY;
  }

  int dayno2 = DayOfWeek(y2, m2, d2);
  if (dayno2 > FRIDAY)
  {
    SubtractDays(dayno2 - FRIDAY, y2, m2, d2);
    dayno2 = FRIDAY;
  }

  long weeks = WeeksBetween(y1, m1, d1, y2, m2, d2);
  int extra = int(dayno2 - dayno1);
  long days = weeks * 5 + extra;
  return labs(days);
}

/****************************************************************************/
DateStuff::Duration DateStuff::Age(int y, int m, int d)
{
  int y2, m2, d2;
  Today(y2, m2, d2);
  return AgeBetween(y, m, d, y2, m2, d2);
}

/****************************************************************************/
DateStuff::Duration DateStuff::AgeBetween(int y1, int m1, int d1,
                                          int y2, int m2, int d2)
{
  // Find order of two dates:
  int order = Compare(y1, m1, d1, y2, m2, d2);
  if (order == 0)
    return Duration(0, 0, 0);
  else if (order > 0)
  {
    // Make date1 precede date2:
    Swap(y1, y2);
    Swap(m1, m2);
    Swap(d1, d2);
  }

  int years = y2 - y1;
  int months = m2 - m1;
  int days = d2 - d1;
  assert(years > 0 || years == 0 && months > 0 ||
         years == 0 && months == 0 && days > 0);

  int lastMonth = m2;
  int lastYear = y2;
  
  while (days < 0)
  {
    assert(months > 0);
    days += DaysInPrevMonth(lastYear, lastMonth--);
    --months;
    
    // This is a loop in case one is borrowing from
    // February, and therefore doesn't get enough days
    // to make 'days' go non-negative, This loop
    // will never iterate more than twice
  }

  if (months < 0)
  {
    assert(years > 0);
    months += 12;
    --years;
  }

  return Duration(abs(years), abs(months), abs(days));
}

/****************************************************************************/
void DateStuff::ResolveMonths(long months, int& y, int& m)
{
  assert(months > 1582 * 12);

  if (months < 0)
    months = -months;

  y = int(months / 12);
  m = int(months % 12);

  if (m == 0)
    m = 12;

  if (!IsValidYMonth(y, m))
    xDateRangeError();
}

/****************************************************************************/
void DateStuff::FromString(const ChrString& s, int& y, int& m, int& d)
{
  char Buffer_[10];
  
  strncpy(Buffer_, s.c_str(), 4);
  Buffer_[4] = 0;  
  y = atoi(Buffer_);
  strncpy(Buffer_, s(4), 2);
  Buffer_[2] = 0;
  m = atoi(Buffer_);
  strncpy(Buffer_, s(6), 2);
  Buffer_[2] = 0;
  d = atoi(Buffer_);
  if (!IsValidYMDay(y, m, d))
    xDateError();
}

/****************************************************************************/
void DateStuff::CheckYMD(int y, int m, int d)
{
  if (!IsValidYMDay(y, m, d))
    xDateError();
}

/****************************************************************************/
void DateStuff::CheckYM(int y, int m)
{
  if (!IsValidYMonth(y, m))
    xDateError();
}

/****************************************************************************/
void DateStuff::CheckY(int y)
{
  if (!IsValidYear(y))
    xDateError();
}

/****************************************************************************/
ChrString DateStuff::ToString(int y, int m, int d)
{
  char Buffer_[10];
  sprintf(Buffer_, "%04d%02d%02d", y, m, d);
  return ChrString(Buffer_);
}

/****************************************************************************/
unsigned DateStuff::MonthsToDays(unsigned month)
{
   return (month * 3057 - 3007) / 100;
}

/****************************************************************************/
long DateStuff::YearsToDays(unsigned yr)
{
   return yr * 365L + yr / 4 - yr / 100 + yr / 400;
}

/****************************************************************************/
long DateStuff::YmdToScalar(unsigned yr, unsigned mo, unsigned day)
{
   long scalar;
   scalar = day + MonthsToDays(mo);
   if ( mo > 2 )                         /* adjust if past February */
      scalar -= IsLeap(yr) ? 1 : 2;
   yr--;
   scalar += YearsToDays(yr);
   return scalar;
}

/****************************************************************************/
void DateStuff::ScalarToYmd(long scalar, unsigned *yr, unsigned *mo, unsigned *day)
{
   unsigned n;                /* compute inverse of years_to_days() */

   for ( n = (unsigned)((scalar * 400L) / 146097); YearsToDays(n) < scalar;)
      n++;                          /* 146097 == years_to_days(400) */
   *yr = n;
   n = (unsigned)(scalar - YearsToDays(n-1));
   if ( n > 59 ) {                       /* adjust if past February */
      n += 2;
      if ( IsLeap(*yr) )
         n -= n > 62 ? 1 : 2;
   }
   *mo = (n * 100 + 3007) / 3057;    /* inverse of months_to_days() */
   *day = n - MonthsToDays(*mo);
}

/****************************************************************************/
/***
 *
 *  Function getdtime :   Get current time and date
 *
 *  Parameters        :   Pointer to time & date structure
 *
 ***/
void DateStuff::GetDtime(struct tm *dtime)
{
  time_t timer;
  struct tm *tmp;

  time( &timer );          /* get time of day */

  tmp = localtime( &timer );   /* converts date/time to a structure */
  memcpy( dtime , tmp , sizeof(struct tm) );
}

/****************************************************************************/
#endif





