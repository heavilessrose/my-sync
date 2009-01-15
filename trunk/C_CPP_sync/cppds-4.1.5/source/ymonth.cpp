#ifndef YMONTH_CPP
#define YMONTH_CPP
#ifndef YMONTH_H
  #include "ymonth.h"
#endif

/****************************************************************************/
YMonth::YMonth()
{
  // Get current year
  time_t tval = time(0);
  struct tm* tmp = localtime(&tval);
  _Month = int(tmp->tm_mon + 1);
}

/****************************************************************************/
YMonth::YMonth(int y, int m):
Year(y)
{
  _Month = m;
  if (!IsEmpty() && !IsValid())
    xMonthError();
}

/****************************************************************************/
YMonth::YMonth(const ChrString& s):
Year(s)
{
  char Buffer_[10];
  strncpy(Buffer_, s(4), 2);
  Buffer_[2] = 0;
  
  _Month = int(atoi(Buffer_));
  if (!IsEmpty() && !IsValid())
    xMonthError();
}

/****************************************************************************/
YMonth::YMonth(const YMonth& Obj_):
Year(Obj_),
_Month(Obj_._Month)
{}

/****************************************************************************/
YMonth* YMonth::Make()
{
  return (new YMonth());
}

/****************************************************************************/
YMonth* YMonth::Make(int y, int m)
{
  return (new YMonth(y, m));
}

/****************************************************************************/
YMonth* YMonth::Make(const ChrString& Str_)
{
  return (new YMonth(Str_));
}

/****************************************************************************/
YMonth* YMonth::Make(const YMonth& Obj_)
{
  return (new YMonth(Obj_));
}

/****************************************************************************/
Boolean YMonth::IsEqualToYMonth(const YMonth* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) == 0);

  return FALSE;
}

/****************************************************************************/
Boolean YMonth::IsLesserToYMonth(const YMonth* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) < 0);

  return FALSE;
}

/****************************************************************************/
Boolean YMonth::IsGreaterToYMonth(const YMonth* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) > 0);

  return FALSE;
}

/****************************************************************************/
Object* YMonth::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new YMonth(*this));

  YMonthAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromYMonth(TrgComp_)));
}

/****************************************************************************/
Object& YMonth::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  YMonthAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromYMonth(TrgComp_)));
}

/****************************************************************************/
YMonth* YMonth::CloneYMonth() const
{
  return (new YMonth(*this));
}

/****************************************************************************/
YMonth& YMonth::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static YMonth _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& YMonth::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
Object& YMonth::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
int YMonth::ClassID() const
{
  return TypeInfo::YMONTH_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* YMonth::GiveObjectAcceptor() const
{
  return (new YMonthAcceptor(this));
}

/****************************************************************************/
long YMonth::DataSize() const
{
  return sizeof(int);
}

/****************************************************************************/
Boolean YMonth::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  YMonthAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToLongInt(TrgComp_);
}

/****************************************************************************/
Boolean YMonth::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  YMonthAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToLongInt(TrgComp_);
}

/****************************************************************************/
Boolean YMonth::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  YMonthAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToLongInt(TrgComp_);
}

/****************************************************************************/
istream& YMonth::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& YMonth::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.read((char*)(&_Month), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& YMonth::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.write((char*)(&_Month), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& YMonth::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& YMonth::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.iread((char*)(&_Month), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& YMonth::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.owrite((char*)(&_Month), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& YMonth::TextRead(istream& Is_, Boolean* Ok_)
{
  int Data_;

  Is_ >>Data_;
  _Month = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& YMonth::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Month;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
int YMonth::Compare(const YMonth& right) const
{
  int diff = Super::Compare(right);
  return ((diff == 0) ? (_Month - right._Month):diff);
}

/****************************************************************************/
DateStuff::Duration YMonth::Age() const
{
  return AgeBetween(*this, YMonth());
}

/****************************************************************************/
int YMonth::EndOfYear() const
{
  if (!IsValid())
    xMonthError();
  return DateStuff::EndOfYear(_Year);
}

/****************************************************************************/
int YMonth::EndOfMonth() const
{
  if (!IsValid())
    xMonthError();
  return DateStuff::EndOfMonth(_Year, _Month);
}

/****************************************************************************/
int YMonth::DaysInPrevMonth() const
{
  if (!IsValid())
    xMonthError();
  return DateStuff::DaysInPrevMonth(_Year, _Month);
}

/****************************************************************************/
int YMonth::DaysInNextMonth() const
{
  if (!IsValid())
    xMonthError();
  return DateStuff::DaysInNextMonth(_Year, _Month);
}

/****************************************************************************/
YMonth& YMonth::operator = (const YMonth& Obj_)
{
  if (this != &Obj_)
  {
    Year::operator = (Obj_);
    _Month = Obj_._Month;
  }
    
  return *this;
}

/****************************************************************************/
void YMonth::AddMonths(long months)
{
  Resolve(_Year * 12L + _Month + months);
}

/****************************************************************************/
void YMonth::SubtractMonths(long months)
{
  Resolve(_Year * 12L + _Month - months);
}

/****************************************************************************/
long YMonth::MonthsBetween(const YMonth& Obj_) const
{
  long diff = int(Obj_ - *this);
  return (diff >= 0 ? diff:-diff);
}

/****************************************************************************/
ChrString YMonth::ToString() const
{
  char Buffer_[23];
  sprintf(Buffer_, "%04d%02d", _Year, _Month);
  return (IsEmpty() ? ChrString():ChrString(Buffer_));
}

//=============================================================
#if OVERLOAD_NEW
void* YMonth::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
void YMonth::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//=============================================================
#if HAS_ARRAY_NEW
void* YMonth::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
void YMonth::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
Boolean YMonthAcceptor::IsYMonth() const
{
  return TRUE;
}

/****************************************************************************/
Boolean YMonthAcceptor::IsYear() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(YMonthAcceptor::YMonthAcceptor, YMonth)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(YMonthAcceptor::AcceptDataPtr, YMonth)
OBJACCEPTOR_GIVEDATA_TOPDEFN(YMonthAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(YMonthAcceptor::CreateFromYMonth, YMonth, YMonth)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(YMonthAcceptor::AssignFromYMonth, YMonth)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(YMonthAcceptor::IsLesserToYMonth, _ObjPtr->IsLesserToYMonth, YMonth)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(YMonthAcceptor::IsEqualToYMonth, _ObjPtr->IsEqualToYMonth, YMonth)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(YMonthAcceptor::IsGreaterToYMonth, _ObjPtr->IsGreaterToYMonth, YMonth)
MEMORYOPS_DEFN(YMonthAcceptor)
OBJACCEPTOR_CREATEFNC_TOPDEFN(YMonthAcceptor::CreateFromChrString, YMonth, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(YMonthAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(YMonthAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToYMonth, YMonth, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(YMonthAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToYMonth, YMonth, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(YMonthAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToYMonth, YMonth, ChrString)

/****************************************************************************/
#endif




