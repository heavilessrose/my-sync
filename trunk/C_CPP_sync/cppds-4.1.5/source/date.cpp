#ifndef DATE_CPP
#define DATE_CPP
#ifndef DATE_H
  #include "date.h"
#endif

/****************************************************************************/
Date::Date(int y, int m, int d):
_Year(y),
_Month(m),
_Day(d)
{
  if (!IsEmpty() && !IsValid())
    xDateError();
}

/****************************************************************************/
Date::Date(const ChrString& s):
_Year(0),
_Month(0),
_Day(0)
{
  if (s.strlen() > 8)
    xDateError();

  char Buffer_[10];
  Buffer_[0] = 0;
  
  if (s.strlen() >= 4)
  {
    strncpy(Buffer_, s.c_str(), 4);
     Buffer_[4] = 0;
    _Year = atoi(Buffer_);
  }

  if (s.strlen() >= 6)
  {
    strncpy(Buffer_, s(4), 2);
    Buffer_[2] = 0;
    _Month = atoi(Buffer_);
  }

  if (s.strlen() == 8)
  {
    strncpy(Buffer_, s(6), 2);
    Buffer_[2] = 0;
    _Day = atoi(Buffer_);
  }

  if (!IsEmpty() && !IsValid())
    xDateError();    
}

/****************************************************************************/
Date::Date(const Year& Obj_):
_Year(Obj_.GetYear()),
_Month(0),
_Day(0)
{}

/****************************************************************************/
Date::Date(const YMonth& Obj_):
_Year(Obj_.GetYear()),
_Month(Obj_.GetMonth()),
_Day(0)
{}

/****************************************************************************/
Date::Date(const Date& Obj_):
_Year(Obj_.GetYear()),
_Month(Obj_.GetMonth()),
_Day(Obj_.GetDay())
{}

/****************************************************************************/
Date* Date::Make(int y, int m, int d)
{
  return (new Date(y, m, d));
}

/****************************************************************************/
Date* Date::Make(const ChrString& s)
{
  return (new Date(s));
}

/****************************************************************************/
Date* Date::Make(const Year& Obj_)
{
  return (new Date(Obj_));
}

/****************************************************************************/
Date* Date::Make(const YMonth& Obj_)
{
  return (new Date(Obj_));
}

/****************************************************************************/
Date* Date::Make(const Date& Obj_)
{
  return (new Date(Obj_));
}

/****************************************************************************/
Object& Date::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  DateAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromDate(TrgComp_)));
}

/****************************************************************************/
Object* Date::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Date(*this));

  DateAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromDate(TrgComp_)));
}

/****************************************************************************/
Date& Date::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Date _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& Date::NULLOBJECT() const
{
  return Date::GiveNullObject();
}

/****************************************************************************/
Object& Date::NULLOBJECT()
{
  return Date::GiveNullObject();
}

/****************************************************************************/
Date& Date::operator = (const ChrString& s)
{
  if (s.strlen() > 8)
    xDateError();

  char Buffer_[10];
  Buffer_[0] = 0;

  _Year = 0;
  _Month = 0;
  _Day = 0;

  if (s.strlen() >= 4)
  {
    strncpy(Buffer_, s.c_str(), 4);
    Buffer_[4] = 0;
    _Year = atoi(Buffer_);
  }

  if (s.strlen() >= 6)
  {  
    strncpy(Buffer_, s(4), 2);
    Buffer_[2] = 0;
    _Month = atoi(Buffer_);
  }

  if (s.strlen() == 8)
  {  
    strncpy(Buffer_, s(6), 2);
    Buffer_[2] = 0;
    _Day = atoi(Buffer_);
  }

  if (!IsEmpty() && !IsValid())
    xDateError();

  return *this;
}

/****************************************************************************/
Date& Date::operator = (const Year& Obj_)
{
  assert(Obj_.IsEmpty() || Obj_.IsValid());
  _Year = Obj_.GetYear();
  _Month = 0;
  _Day = 0;

  return *this;
}

/****************************************************************************/
Date& Date::operator = (const YMonth& Obj_)
{
  assert(Obj_.IsEmpty() || Obj_.IsValid());
  _Year = Obj_.GetYear();
  _Month = Obj_.GetMonth();
  _Day = 0;
  
  return *this;
}

/****************************************************************************/
Date& Date::operator = (const Date& Obj_)
{
  if (this != &Obj_)
  {
    assert(Obj_.IsEmpty() || Obj_.IsValid());
    _Year = Obj_.GetYear();
    _Month = Obj_.GetMonth();
    _Day = Obj_.GetDay();
  }
  
  return *this;
}

/****************************************************************************/
Boolean Date::IsValid() const
{
  return
  (
    IsEmpty() ||
    (_Day == 0 && _Month == 0 && IsValidYear()) ||
    (_Day == 0 && IsValidYMonth()) ||
    IsValidYMDay()
  );
}

/****************************************************************************/
Boolean Date::IsEmpty() const
{
  return (_Day == 0 && _Month == 0 && _Year == 0);
}

/****************************************************************************/
long Date::DataSize () const
{
  return (sizeof(int) * 3);
}

/****************************************************************************/
int Date::ClassID() const
{
  return TypeInfo::DATE_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* Date::GiveObjectAcceptor() const
{
  return (new DateAcceptor(this));
}

/****************************************************************************/
istream& Date::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.seekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& Date::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Year), sizeof(int)).good()))
    if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Month), sizeof(int)).good()))
      if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Day), sizeof(int)).good()))  
        SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& Date::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Year), sizeof(int)).good()))
    if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Month), sizeof(int)).good()))
      *Ok_ = Os_.write((char*)(&_Day), sizeof(int)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& Date::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.iseekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& Date::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Year), sizeof(int)).good()))
    if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Month), sizeof(int)).good()))
      if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Day), sizeof(int)).good()))
        SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& Date::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Year), sizeof(int)).good()))
    if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Month), sizeof(int)).good()))
      *Ok_ = Os_.owrite((char*)(&_Day), sizeof(int)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& Date::TextRead(istream& Is_, Boolean* Ok_)
{
  int y, m, d;

  Is_ >>y >>m >>d;
  *this = Date(y, m, d);

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& Date::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Year <<"/t" <<_Month <<"/t" <<_Day;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
int Date::Compare(const Date& Obj_) const
{
  int ydiff = _Year - Obj_._Year;
  int mdiff = _Month - Obj_._Month;
  return ((ydiff == 0) ? ((mdiff == 0) ? (_Day - Obj_._Day) : mdiff):ydiff);
}

/****************************************************************************/
Boolean Date::IsEqualToDate(const Date* Ptr_) const
{
  return
  (
    _Year == Ptr_->_Year &&
    _Month == Ptr_->_Month &&
    _Day == Ptr_->_Day
  );
}

/****************************************************************************/
Boolean Date::IsLesserToDate(const Date* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) < 0);

  return FALSE;
}

/****************************************************************************/
Boolean Date::IsGreaterToDate(const Date* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) > 0);

  return FALSE;
}

/****************************************************************************/
Boolean Date::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DateAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToDate(TrgComp_);
}

/****************************************************************************/
Boolean Date::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DateAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToDate(TrgComp_);
}

/****************************************************************************/
Boolean Date::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  DateAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToDate(TrgComp_);
}

/****************************************************************************/
void Date::AddYears(int years)
{
  if (IsValidYMDay())
    DateStuff::AddYears(years, _Year, _Month, _Day);
  else if (IsValidYMonth() || IsValidYear())
  {
    Year y(_Year);
    y.AddYears(years);
    Date::operator = (y);
  }
  else
    xDateError();
}

/****************************************************************************/
void Date::SubtractYears(int years)
{
  if (IsValidYMDay())
    DateStuff::SubtractYears(years, _Year, _Month, _Day);
  else if (IsValidYMonth() || IsValidYear())
  {
    Year y(_Year);
    y.SubtractYears(years);
    Date::operator = (y);
  }
  else
    xDateError();
}

/****************************************************************************/
int Date::YearsBetween(const Date& Obj_)
{
  if (!IsValid())
    xDateError();

  Year me(_Year);
  Year you(Obj_._Year);
  return me.YearsBetween(you);
}

/****************************************************************************/
void Date::AddMonths(long months)
{
  if (IsValidYMDay())
    DateStuff::AddMonths(months, _Year, _Month, _Day);
  else if (IsValidYMonth())
    DateStuff::ResolveMonths(_Year * 12L + _Month + months, _Year, _Month);
  else
    xDateError();
}

/****************************************************************************/
void Date::SubtractMonths(long months)
{
  if (IsValidYMDay())
    DateStuff::SubtractMonths(months, _Year, _Month, _Day);
  else if (IsValidYMonth())
    DateStuff::ResolveMonths(_Year * 12L + _Month - months, _Year, _Month);
  else
    xDateError();
}

/****************************************************************************/
long Date::MonthsBetween(const Date& Obj_) const
{
  if (!IsValidYMonth() || !Obj_.IsValidYMonth())
    xDateError();

  return DateStuff::MonthsBetween(_Year, _Month, Obj_._Year, Obj_._Month);
}

/****************************************************************************/
void Date::AddDays(long days)
{
  if (!IsValidYMDay())
    xDateError();

  DateStuff::AddDays(days, _Year, _Month, _Day);
}

/****************************************************************************/
void Date::SubtractDays(long days)
{
  if (!IsValidYMDay())
    xDateError();

  DateStuff::SubtractDays(days, _Year, _Month, _Day);
}

/****************************************************************************/
long Date::DaysBetween(const Date& Obj_) const
{
  if (!IsValidYMDay() || !Obj_.IsValidYMDay())
    xDateError();

  return DateStuff::DaysBetween(_Year, _Month, _Day,
                                Obj_._Year, Obj_._Month, Obj_._Day);
}

/****************************************************************************/
void Date::AddWeeks(long weeks)
{
  if (!IsValidYMDay())
    xDateError();

  DateStuff::AddWeeks(weeks, _Year, _Month, _Day);
}

/****************************************************************************/
void Date::SubtractWeeks(long weeks)
{
  if (!IsValidYMDay())
    xDateError();

  DateStuff::SubtractWeeks(weeks, _Year, _Month, _Day);
}

/****************************************************************************/
long Date::WeeksBetween(const Date& Obj_) const
{
  if (!IsValidYMDay() || !Obj_.IsValidYMDay())
    xDateError();

  return DateStuff::WeeksBetween(_Year, _Month, _Day,
                                 Obj_._Year, Obj_._Month, Obj_._Day);
}

/****************************************************************************/
void Date::AddWeekDays(long days)
{
  if (!IsValidYMDay())
    xDateError();

  DateStuff::AddWeekDays(days, _Year, _Month, _Day);
}

/****************************************************************************/
void Date::SubtractWeekDays(long days)
{
  if (!IsValidYMDay())
    xDateError();

  DateStuff::SubtractWeekDays(days, _Year, _Month, _Day);
}

/****************************************************************************/
long Date::WeekDaysBetween(const Date& Obj_) const
{
  if (!IsValidYMDay() || !Obj_.IsValidYMDay())
    xDateError();

  return DateStuff::WeekDaysBetween(_Year, _Month, _Day,
                                    Obj_._Year, Obj_._Month, Obj_._Day);
}

/****************************************************************************/
DateStuff::Duration Date::operator - (const Date& Obj_) const
{
  return AgeBetween(Obj_);
}

/****************************************************************************/
int Date::DayOfWeek() const
{
  if (!IsValidYMDay())
    xDateError();

  return DateStuff::DayOfWeek(_Year, _Month, _Day);
}

/****************************************************************************/
int Date::DayOfYear() const
{
  if (!IsValidYMDay())
    xDateError();

  return DateStuff::DayOfYear(_Year, _Month, _Day);
}

/****************************************************************************/
int Date::EndOfMonth() const
{
  if (!IsValidYMonth())
    xDateError();

  return DateStuff::EndOfMonth(_Year, _Month);
}

/****************************************************************************/
int Date::EndOfYear() const
{
  if (!IsValidYear())
    xDateError();

  return DateStuff::EndOfYear(_Year);
}

/****************************************************************************/
DateStuff::Duration Date::AgeBetween(const Date& Obj_) const
{
  if (!IsValid() || !Obj_.IsValid())
    xDateError();

  if (IsValidYMDay() && Obj_.IsValidYMDay())
    return DateStuff::AgeBetween(_Year, _Month, _Day,
                                 Obj_._Year, Obj_._Month, Obj_._Day);
  else if (IsValidYMonth() && Obj_.IsValidYMonth())
    return DateStuff::AgeBetween(_Year, _Month, 0,
                                 Obj_._Year, Obj_._Month, 0);
  else if (IsValidYear() && Obj_.IsValidYear())
    return DateStuff::AgeBetween(_Year, 0, 0,
                                 Obj_._Year, 0, 0);
  else
  {
    assert(0);
    return DateStuff::Duration(-1, -1, -1);
  }
}

/****************************************************************************/
DateStuff::Duration Date::Age() const
{
  return AgeBetween(Today());
}

/****************************************************************************/
Date Date::Today()
{
  int y, m, d;
  DateStuff::Today(y, m, d);
  return Date(y, m, d);
}

/****************************************************************************/
ChrString Date::ToString() const
{
  return (IsEmpty() ? ChrString():DateStuff::ToString(_Year, _Month, _Day));
}

/****************************************************************************/
#if OVERLOAD_NEW
void* Date::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void Date::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* Date::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void Date::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
Boolean DateAcceptor::IsDate() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(DateAcceptor::DateAcceptor, Date)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(DateAcceptor::AcceptDataPtr, Date)
OBJACCEPTOR_GIVEDATA_TOPDEFN(DateAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(DateAcceptor::CreateFromDate, Date, Date)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(DateAcceptor::AssignFromDate, Date)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(DateAcceptor::IsLesserToDate, _ObjPtr->IsLesserToDate, Date)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(DateAcceptor::IsEqualToDate, _ObjPtr->IsEqualToDate, Date)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(DateAcceptor::IsGreaterToDate, _ObjPtr->IsGreaterToDate, Date)
MEMORYOPS_DEFN(DateAcceptor)

OBJACCEPTOR_CREATEFNC_TOPDEFN(DateAcceptor::CreateFromChrString, Date, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(DateAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToDate, Date, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToDate, Date, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToDate, Date, ChrString)
OBJACCEPTOR_CREATEFNC_TOPDEFN(DateAcceptor::CreateFromYear, Date, Year)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(DateAcceptor::AssignFromYear, Year)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsLesserToYear, _ObjPtr->IsLesserToDate, Date, Year)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsEqualToYear, _ObjPtr->IsEqualToDate, Date, Year)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsGreaterToYear, _ObjPtr->IsGreaterToDate, Date, Year)
OBJACCEPTOR_CREATEFNC_TOPDEFN(DateAcceptor::CreateFromYMonth, Date, YMonth)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(DateAcceptor::AssignFromYMonth, YMonth)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsLesserToYMonth, _ObjPtr->IsLesserToDate, Date, YMonth)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsEqualToYMonth, _ObjPtr->IsEqualToDate, Date, YMonth)
OBJACCEPTOR_COMPFNC_TOPDEFN(DateAcceptor::IsGreaterToYMonth, _ObjPtr->IsGreaterToDate, Date, YMonth)

/****************************************************************************/
#endif




