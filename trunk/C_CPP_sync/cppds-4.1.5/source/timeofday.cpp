#ifndef TIMEOFDAY_CPP
#define TIMEOFDAY_CPP
#ifndef TIMEOFDAY_H
  #include "timeofday.h"
#endif

/****************************************************************************/
TimeOfDay::TimeOfDay():
_Days(0),
_Hour(0),
_Minute(0),
_Second(0)
{}

/****************************************************************************/
//  arguments: 1  - Base hours (0 -> 24)
//             2  - Base minutes (0 -> 59)
//             3  - Base seconds (0 -> 59)
//
TimeOfDay::TimeOfDay(short hour, short minute, short second):
_Days(0),
_Hour(hour),
_Minute(minute),
_Second(second)
{
  RollOver();
  
//  assert(IsValidTime());
  if (!IsValidTime())
    xTimeError();
}

/****************************************************************************/
//  arguments: 1  - Base hours (0 -> 24)
//             2  - Base minutes (0 -> 59)
//             3  - Base seconds (0 -> 59)
//
TimeOfDay::TimeOfDay(const ChrString& s):
_Days(0),
_Hour(0),
_Minute(0),
_Second(0)
{
  char* Buffer_ = DownCase(new_char_string(s.c_str()));
  char* Token_;

  Token_ = strtok(Buffer_, ":");
  _Hour = atoi(Token_);
  Token_ = strtok(NULL, ":");
  
  if (strchr(Token_, 'm'))
  {
    char* Space_ = NULL;
    
    if (Space_ = strchr(Token_, 'p'))
    {
      _Hour += 12;
      --Space_;
    }
    else if (Space_ = strchr(Token_, 'a'))
      --Space_;

    if (Space_ && isspace(*Space_))
      *Space_ = 0;

    _Minute = atoi(Token_);
  }
  else
  {
    _Minute = atoi(Token_);
    Token_ = strtok(NULL, ":");
    _Second = atoi(Token_);
  }

  ::DeleteArray(Buffer_);
  RollOver();

//  assert(IsValidTime());
  if (!IsValidTime())
    xTimeError();
}

/****************************************************************************/
//  arguments: 1  - Base hours (0 -> 24)
//             2  - Base minutes (0 -> 59)
//             3  - Base seconds (0 -> 59)
//
TimeOfDay::TimeOfDay(long nsecs):
_Days(nsecs / 86400),
_Hour((nsecs % 86400) / 3600),
_Minute(((nsecs % 86400) % 3600) / 60),
_Second(((nsecs % 86400) % 3600) % 60)
{
  RollOver();

//  assert(IsValidTime());
  if (!IsValidTime())
    xTimeError();
}

/****************************************************************************/
TimeOfDay::TimeOfDay(const TimeOfDay& Obj_):
_Days(Obj_._Days),
_Hour(Obj_._Hour),
_Minute(Obj_._Minute),
_Second(Obj_._Second)
{}

/****************************************************************************/
TimeOfDay* TimeOfDay::Make()
{
  return (new TimeOfDay());
}

/****************************************************************************/
TimeOfDay* TimeOfDay::Make(short hour, short minute, short second)
{
  return (new TimeOfDay(hour, minute, second));
}

/****************************************************************************/
TimeOfDay* TimeOfDay::Make(const ChrString& s)
{
  return (new TimeOfDay(s));
}

/****************************************************************************/
TimeOfDay* TimeOfDay::Make(long nsecs)
{
  return (new TimeOfDay(nsecs));
}

/****************************************************************************/
TimeOfDay* TimeOfDay::Make(const TimeOfDay& Obj_)
{
  return (new TimeOfDay(Obj_));
}

/****************************************************************************/
long TimeOfDay::ToSeconds() const
{
  long tval_ = _Hour * 3600L + _Minute * 60L + _Second;
  return tval_;
}

/****************************************************************************/
Boolean TimeOfDay::IsValidTime(short hour, short minute, short second)
{
  return
  (
    (0 <= hour && hour <= 24) &&
    (0 <= minute && minute <= 59) &&
    (0 <= second && second <= 59)
  );
}

/****************************************************************************/
Boolean TimeOfDay::StringToTime(const ChrString& s)
{
  _Hour = 0;
  _Minute = 0;
  _Second = 0;
  _Days = 0;

  char* Buffer_ = DownCase(new_char_string(s.c_str()));
  char* Token_;

  Token_ = strtok(Buffer_, ":");
  _Hour = atoi(Token_);
  Token_ = strtok(NULL, ":");
  
  if (strchr(Token_, 'm'))
  {
    char* Space_ = NULL;
    
    if (Space_ = strchr(Token_, 'p'))
    {
      _Hour += 12;
      --Space_;
    }
    else if (Space_ = strchr(Token_, 'a'))
      --Space_;

    if (Space_ && isspace(*Space_))
      *Space_ = 0;

    _Minute = atoi(Token_);
  }
  else
  {
    _Minute = atoi(Token_);
    Token_ = strtok(NULL, ":");
    _Second = atoi(Token_);
  }

  ::DeleteArray(Buffer_);
  RollOver();

  return IsValidTime();
}

/****************************************************************************/
//  arguments: 1  - Base hours (0 -> 24)
//             2  - Base minutes (0 -> 59)
//             3  - Base seconds (0 -> 59)
//
TimeOfDay& TimeOfDay::operator = (const ChrString& s)
{
  StringToTime(s);
  
  if (!IsValidTime())
    xTimeError();
  
  return *this;
}

/****************************************************************************/
//  arguments: 1  - Base hours (0 -> 24)
//             2  - Base minutes (0 -> 59)
//             3  - Base seconds (0 -> 59)
//
TimeOfDay& TimeOfDay::operator = (long nsecs)
{
  _Days = (nsecs / 86400);
  _Hour = ((nsecs % 86400) / 3600);
  _Minute = (((nsecs % 86400) % 3600) / 60);
  _Second = (((nsecs % 86400) % 3600) % 60);

  RollOver();

  if (!IsValidTime())
    xTimeError();  

  return *this;
}

/****************************************************************************/
TimeOfDay& TimeOfDay::operator = (const TimeOfDay& Obj_)
{
  _Hour = Obj_._Hour;
  _Minute = Obj_._Minute;
  _Second = Obj_._Second;
  _Days = Obj_._Days;

  return *this;
}

/****************************************************************************/
TimeOfDay& TimeOfDay::operator += (long numsecs)
{
  TimeOfDay Temp_(numsecs);
  AddTime(Temp_._Hour, Temp_._Minute, Temp_._Second);
  return *this;
}
    
/****************************************************************************/
TimeOfDay& TimeOfDay::operator -= (long numsecs)
{
  TimeOfDay Temp_(numsecs);
  SubtractTime(Temp_._Hour, Temp_._Minute, Temp_._Second);
  return *this;
}

/****************************************************************************/
TimeOfDay& TimeOfDay::AddTime(short hour, short minute, short second)
{
  AddTime(_Hour, _Minute, _Second,
          hour, minute, second,
          &_Hour, &_Minute, &_Second, &_Days);

  RollOver();

  return *this;
}

/****************************************************************************/
TimeOfDay& TimeOfDay::SubtractTime(short hour, short minute, short second)
{
  AddTime(_Hour, _Minute, _Second,
          -hour, -minute, -second,
          &_Hour, &_Minute, &_Second, &_Days);

  RollOver();

  return *this;
}

/****************************************************************************/
/*
**  ADDTIME.C - Add a time period to a base time, normalizing the result
**
**  arguments: 1  - Base hours (0 -> 24)
**             2  - Base minutes (0 -> 59)
**             3  - Base seconds (0 -> 59)
**             4  - Span hours
**             5  - Span minutes
**             6  - Span seconds
**             7  - Address of returned hours (0 -> 23)
**             9  - Address of returned minutes (0 -> 59)
**             10 - Address of returned seconds (0 -> 59)
**             11 - Address of number of days to add to result
**
**  returns: 0 if no error, non-zero if base time range error
**
**  Notes: 1) Span values may be negative.
**         2) Overflowing midnight will cause a positive number of days to be
**            returned.
**         3) Underflowing midnight will cause a negative number of days to be
**            returned.
**
**  Original Copyright 1994 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  This subset version is donated to the public domain.
*/
int TimeOfDay::AddTime(short basehrs, short basemins, short basesecs,
                       short spanhrs, short spanmins, short spansecs,
                       short *hrs, short *mins, short *secs, short *days)
{
      short h, m, s;
      div_t r;

      if (basehrs > 24 || basemins > 59 || basesecs > 59)
            return -1;

      if (24 == basehrs)
            basehrs = 0;
      h = (short)basehrs  + spanhrs;
      m = (short)basemins + spanmins;
      s = (short)basesecs + spansecs;

      r = div(s, 60);
      if (s < 0)
      {
            r.rem += 60;
            --r.quot;
      }
      *secs = r.rem;
      m += r.quot;

      r = div(m, 60);
      if (m < 0)
      {
            r.rem += 60;
            --r.quot;
      }
      *mins = r.rem;
      h += r.quot;

      r = div(h, 24);
      if (h < 0)
      {
            r.rem += 24;
            --r.quot;
      }
      *hrs = r.rem;
      *days = r.quot;

      return 0;
}

/****************************************************************************/
Object& TimeOfDay::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  TimeOfDayAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromTimeOfDay(TrgComp_)));
}

/****************************************************************************/
Object* TimeOfDay::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new TimeOfDay(*this));

  TimeOfDayAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromTimeOfDay(TrgComp_)));
}

/****************************************************************************/
TimeOfDay& TimeOfDay::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static TimeOfDay _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& TimeOfDay::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
Object& TimeOfDay::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
int TimeOfDay::ClassID() const
{
  return TypeInfo::TIMEOFDAY_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* TimeOfDay::GiveObjectAcceptor() const
{
  return (new TimeOfDayAcceptor(this));
}

/****************************************************************************/
istream& TimeOfDay::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& TimeOfDay::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Hour), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Minute), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Second), sizeof(short)).good()))
        if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Days), sizeof(short)).good()))
          SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& TimeOfDay::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Hour), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Minute), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Second), sizeof(short)).good()))
        *Ok_ = Os_.write((char*)(&_Days), sizeof(short)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& TimeOfDay::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& TimeOfDay::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Hour), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Minute), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Second), sizeof(short)).good()))
        if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Days), sizeof(short)).good()))
          SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& TimeOfDay::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Hour), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Minute), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Second), sizeof(short)).good()))
        *Ok_ = Os_.owrite((char*)(&_Days), sizeof(short)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& TimeOfDay::TextRead(istream& Is_, Boolean* Ok_)
{
  int h, m, s;

  Is_ >>h >>m >>s;
  *this = TimeOfDay(h, m, s);

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& TimeOfDay::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Hour <<":" <<_Minute <<":" <<_Second <<" -- days:" <<_Days;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
long TimeOfDay::Compare(const TimeOfDay& Obj_) const
{
  long TmVal1_ = ToSeconds();
  long TmVal2_ = Obj_.ToSeconds();
  return (TmVal1_ - TmVal2_);
}

/****************************************************************************/
Boolean TimeOfDay::IsEqualToTimeOfDay(const TimeOfDay* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) == 0);

  return FALSE;
}

/****************************************************************************/
Boolean TimeOfDay::IsLesserToTimeOfDay(const TimeOfDay* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) < 0);

  return FALSE;
}

/****************************************************************************/
Boolean TimeOfDay::IsGreaterToTimeOfDay(const TimeOfDay* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) > 0);

  return FALSE;
}

/****************************************************************************/
Boolean TimeOfDay::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  TimeOfDayAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToTimeOfDay(TrgComp_);
}

/****************************************************************************/
Boolean TimeOfDay::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  TimeOfDayAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToTimeOfDay(TrgComp_);
}

/****************************************************************************/
Boolean TimeOfDay::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  TimeOfDayAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToTimeOfDay(TrgComp_);
}

/****************************************************************************/
TimeOfDay TimeOfDay::Now()
{
  struct tm dtime_;
  GetDtime(&dtime_);
  
  short Hour_ = dtime_.tm_hour;
  short Minute_ = dtime_.tm_min;
  short Second_ = dtime_.tm_sec;
  
  return TimeOfDay(Hour_, Minute_, Second_);
}

/****************************************************************************/
long TimeOfDay::ParseAndConvert(const ChrString& s)
{
  TimeOfDay TmpTime_;
  Boolean Result_ = TmpTime_.StringToTime(s);

  if (!Result_)
    return -1;

  return TmpTime_.ToSeconds();
}

/****************************************************************************/
void TimeOfDay::GetDtime(struct tm *dtime)
{
  time_t timer;
  struct tm *tmp;

  time(&timer);                 // get time of day

  tmp = localtime(&timer);      // converts date/time to a structure
  memcpy(dtime ,tmp ,sizeof(struct tm));
}

/****************************************************************************/
ChrString TimeOfDay::AsString() const
{
  long Hour_ = _Hour;
  char Buffer_[128];
  size_t Len_;
  
  ShortToStr(Hour_, Buffer_);
  Len_ = SafeStrLen(Buffer_);  
  if (Hour_ < 10)
  {
    Buffer_[Len_] = Buffer_[Len_ - 1];
    Buffer_[Len_ - 1] = '0';
    ++Len_;
  }
  Buffer_[Len_] = ':';
  ShortToStr(_Minute, &Buffer_[Len_ + 1]);
  Len_ = SafeStrLen(Buffer_);
  if (_Minute < 10)
  {
    Buffer_[Len_] = Buffer_[Len_ - 1];
    Buffer_[Len_ - 1] = '0';
    ++Len_;    
  }  
  Buffer_[Len_] = ':';
  ShortToStr(_Second, &Buffer_[Len_ + 1]);
  Len_ = SafeStrLen(Buffer_);
  if (_Second < 10)
  {
    Buffer_[Len_] = Buffer_[Len_ - 1];
    Buffer_[Len_ - 1] = '0';
    Buffer_[++Len_] = 0;
  }

  return ChrString(Buffer_);
}

/****************************************************************************/
ChrString TimeOfDay::PrintString(int Form_) const
{
  short Hour_ = _Hour;
  const char* Mod_ = (_Hour >= 12) ? "pm":"am";

  if (Form_ == TIME_MILITARY)
    return AsString();

  char Buffer_[128];
  size_t Len_;

  if (Hour_ > 12)
    Hour_ -= 12;
  
  ShortToStr(Hour_, Buffer_);
  Len_ = SafeStrLen(Buffer_);  
  if (_Hour < 10)
  {
    Buffer_[Len_] = Buffer_[Len_ - 1];
    Buffer_[Len_ - 1] = '0';
    ++Len_;
  }
  Buffer_[Len_] = ':';
  ShortToStr(_Minute, &Buffer_[Len_ + 1]);
  Len_ = SafeStrLen(Buffer_);
  if (_Minute < 10)
  {
    Buffer_[Len_] = Buffer_[Len_ - 1];
    Buffer_[Len_ - 1] = '0';
    ++Len_;    
  }  

  Buffer_[Len_] = ' ';
  Buffer_[++Len_] = 0;
  strcat(Buffer_, Mod_);

  return ChrString(Buffer_);
}

/****************************************************************************/
Boolean TimeOfDay::IsBetween(const TimeOfDay& t1, const TimeOfDay& t2)
{
  return (t1 <= *this && *this <= t2);
}

/****************************************************************************/
long TimeOfDay::DataSize () const
{
  return (sizeof(short) * 4);
}

/****************************************************************************/
long TimeOfDay::operator - (const TimeOfDay& Obj_) const
{
  return Compare(Obj_);
}

/****************************************************************************/
TimeOfDay TimeOfDay::operator + (long numsecs) const
{
  return TimeOfDay(ToSeconds() + numsecs);
}

/****************************************************************************/
TimeOfDay TimeOfDay::operator - (long numsecs) const
{
  numsecs = ToSeconds() - numsecs;
  if (numsecs < 0)
    xTimeError();

  return TimeOfDay(numsecs);
}

/****************************************************************************/
#if OVERLOAD_NEW
void* TimeOfDay::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void TimeOfDay::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* TimeOfDay::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void TimeOfDay::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
TimeOfDayAcceptor::TimeOfDayAcceptor(const TimeOfDay* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
Boolean TimeOfDayAcceptor::IsTimeOfDay() const
{
  return TRUE;
}

/****************************************************************************/
void TimeOfDayAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const TimeOfDay*)Ptr_;
}

/****************************************************************************/
const void* TimeOfDayAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
void* TimeOfDayAcceptor::CreateFromTimeOfDay(const ObjectAcceptor& Obj_)
{
  _VarPtr = new TimeOfDay(*((const TimeOfDay*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
void* TimeOfDayAcceptor::AssignFromTimeOfDay(const ObjectAcceptor& Obj_)
{
  const TimeOfDay* TrgPtr_ = (const TimeOfDay*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
Boolean TimeOfDayAcceptor::IsEqualToTimeOfDay(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToTimeOfDay((const TimeOfDay*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean TimeOfDayAcceptor::IsLesserToTimeOfDay(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToTimeOfDay((const TimeOfDay*)Obj_.GiveDataPtr());
}

/****************************************************************************/
Boolean TimeOfDayAcceptor::IsGreaterToTimeOfDay(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToTimeOfDay((const TimeOfDay*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
void* TimeOfDayAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void TimeOfDayAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif

/****************************************************************************/
OBJACCEPTOR_CREATEFNC_TOPDEFN(TimeOfDayAcceptor::CreateFromChrString, TimeOfDay, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(TimeOfDayAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(TimeOfDayAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToTimeOfDay, TimeOfDay, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(TimeOfDayAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToTimeOfDay, TimeOfDay, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(TimeOfDayAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToTimeOfDay, TimeOfDay, ChrString)
/****************************************************************************/
/****************************************************************************/
#if TIMEOFDAY_DEBUG
int main()
{
  TimeOfDay td1_(9, 30, 10);
  TimeOfDay td2_(47700);
  TimeOfDay td3_("1:30 pm");
  TimeOfDay td4_("20:15:30");
  TimeOfDay td5_;  

  assert(td3_.IsBetween(td1_, td4_));
  assert(td2_.IsBetween(td1_, td3_));
  assert(td2_ == TimeOfDay("1:15 pm"));

  td5_ = "1:5 pm";
  assert(td5_.Hour() == 13);
  assert(td5_.Minute() == 5);
  assert(td5_.Second() == 0);
  td5_.AddTime(7, 10, 30);
  assert(td5_ == td4_);
  td5_ -= 24330;
  assert(td5_ == td3_);
  td5_ += 900;
  assert(td5_ == TimeOfDay(13, 45, 00));
  assert(td5_.IsBetween(td3_, td4_));
  assert(td3_ < td5_);
  assert(td4_ > td5_);
  td3_.SubtractTime(0, 15, 0);
  assert(td3_ == td2_);
  td3_ += 900;
  assert(td3_ == TimeOfDay(13, 30, 00));

  td5_ = td1_.Now();
  time_t timer;
  struct tm *tmp;
  time(&timer);                 // get time of day
  tmp = localtime(&timer);      // converts date/time to a structure  
  assert(td5_ == TimeOfDay(tmp->tm_hour, tmp->tm_min, tmp->tm_sec));
  assert(TimeOfDay::ParseAndConvert("3:30 pm") == TimeOfDay("15:30:00").ToSeconds());
  cout <<td5_ <<endl;
  cin >>td5_;
  cout <<td5_ <<endl;

  TimeOfDay* tdp6_ = TimeOfDay::Make("15:30:00");
  ChrString str_ = tdp6_->AsString();
  assert(!strcmp(str_.c_str(), "15:30:00"));
  str_ = tdp6_->PrintString(TimeOfDay::TIME_NORMAL);
  assert(!strcmp(str_.c_str(), "3:30 pm"));
  str_ = tdp6_->PrintString(TimeOfDay::TIME_MILITARY);
  assert(!strcmp(str_.c_str(), "15:30:00"));
  
  td5_ = *tdp6_ + (7200 + 900 + 15);
  assert(td5_ == TimeOfDay("17:45:15"));
  td5_ = td5_ - (3600 + 450 + 10);
  assert(td5_ == TimeOfDay("16:37:35"));
  long nsecs_ = td5_ - *tdp6_;
  assert(nsecs_ == 4055);

  assert(td5_.StaticID() == tdp6_->ClassID());
  assert(tdp6_->ClassID() == TypeInfo::TIMEOFDAY_TYPE);
  assert(tdp6_->NULLOBJECT() == td5_.NULLOBJECT());
  assert(td5_.NULLOBJECT().IsNullObject());

  TimeOfDay* tdp7_ = TimeOfDay::Make(tdp6_->ToSeconds());
  assert(*tdp7_ == *tdp6_);
  assert(*tdp7_ != td5_);
  delete tdp7_;
  tdp7_ = (TimeOfDay*)td5_.Clone();
  assert(*tdp7_ != *tdp6_);
  assert(*tdp7_ == td5_);
  tdp7_->Assign(td1_);
  assert(*tdp7_ != *tdp6_);
  assert(*tdp7_ != td5_);
  assert(*tdp7_ == td1_);

  ofstream fout("testing.bin");
  Write(fout, *tdp7_);
  fout.close();

  TimeOfDay* tdp8_ = TimeOfDay::Make(10, 10, 10);
  assert(tdp8_->ToSeconds() == (10 * 3600 + 10 * 60 + 10));
  assert(*tdp8_ != *tdp7_);
  ifstream fin("testing.bin");
  Read(fin, *tdp8_);
  fin.close();

  assert(*tdp7_ == *tdp8_);
  cout <<*tdp8_ <<endl;
  assert(*tdp8_ == TimeOfDay("9:30:10"));
  *tdp8_ += 86310;
  cout <<*tdp8_ <<endl;
  assert(*tdp8_ == TimeOfDay("9:28:40"));
  assert(tdp8_->Days() == 1);

  delete tdp6_;
  delete tdp7_;
  delete tdp8_;

  return 0;
}
#endif
/****************************************************************************/
#endif




