#ifndef FULLDATE_H
#define FULLDATE_H
#ifndef DATE_H
  #include "date.h"
#endif

/****************************************************************************/
class FullDate : public Date
{
  public:
    FullDate();
    FullDate(const ChrString& s);
    FullDate(int y, int m, int d);
    FullDate(const Date& Obj_);

    Boolean IsValid() const
        { return (IsEmpty() || DateStuff::IsValidYMDay(_Year, _Month, _Day)); }
    long ToJul() const
        { return DateStuff::GregToJul(_Year, _Month, _Day); }
};

/****************************************************************************/
#endif




