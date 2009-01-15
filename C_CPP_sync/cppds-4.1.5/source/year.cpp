#ifndef YEAR_CPP
#define YEAR_CPP
#ifndef YEAR_H
  #include "year.h"
#endif

/****************************************************************************/
Year::Year()
{
  // Get current year
  time_t tval = time(0);
  struct tm* tmp = localtime(&tval);
  _Year = int(tmp->tm_year + 1900);
}

/****************************************************************************/
Year::Year(int y)
{
  _Year = y;
  if (!IsEmpty() && !IsValid())
    xYearError();
}

/****************************************************************************/
Year::Year(const ChrString& s)
{
  char Buffer_[10];
  strncpy(Buffer_, s.c_str(), 4);
  Buffer_[4] = 0;
  
  _Year = int(atoi(Buffer_));
  if (!IsEmpty() && !IsValid())
    xYearError();
}

/****************************************************************************/
Year::Year(const Year& Obj_):
_Year(Obj_._Year)
{}

/****************************************************************************/
Year* Year::Make()
{
  return (new Year());
}

/****************************************************************************/
Year* Year::Make(int Val_)
{
  return (new Year(Val_));
}

/****************************************************************************/
Year* Year::Make(const ChrString& Str_)
{
  return (new Year(Str_));
}

/****************************************************************************/
Year* Year::Make(const Year& Obj_)
{
  return (new Year(Obj_));
}

/****************************************************************************/
Boolean Year::IsEqualToYear(const Year* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) == 0);

  return FALSE;
}

/****************************************************************************/
Boolean Year::IsLesserToYear(const Year* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) < 0);

  return FALSE;
}

/****************************************************************************/
Boolean Year::IsGreaterToYear(const Year* Ptr_) const
{
  if (Ptr_)
    return (Compare(*Ptr_) > 0);

  return FALSE;
}

/****************************************************************************/
Object* Year::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Year(*this));

  YearAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromYear(TrgComp_)));
}

/****************************************************************************/
Object& Year::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  YearAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromYear(TrgComp_)));
}

/****************************************************************************/
Year* Year::CloneYear() const
{
  return (new Year(*this));
}

/****************************************************************************/
Year& Year::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Year _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& Year::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
Object& Year::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
const ObjectAcceptor* Year::GiveObjectAcceptor() const
{
  return (new YearAcceptor(this));
}

/****************************************************************************/
int Year::ClassID() const
{
  return TypeInfo::YEAR_TYPE;
}

/****************************************************************************/
long Year::DataSize() const
{
  return sizeof(int);
}

/****************************************************************************/
Boolean Year::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  YearAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToLongInt(TrgComp_);
}

/****************************************************************************/
Boolean Year::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  YearAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToLongInt(TrgComp_);
}

/****************************************************************************/
Boolean Year::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  YearAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToLongInt(TrgComp_);
}

/****************************************************************************/
istream& Year::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& Year::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.read((char*)(&_Year), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& Year::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.write((char*)(&_Year), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& Year::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& Year::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && Is_.iread((char*)(&_Year), DataSize()).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& Year::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
    Os_.owrite((char*)(&_Year), DataSize());

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& Year::TextRead(istream& Is_, Boolean* Ok_)
{
  int Data_;

  Is_ >>Data_;
  _Year = Data_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
ostream& Year::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<_Year;
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
void Year::AddYears(int y)
{
  if (_Year > DateStuff::MAX_YEAR - y)
    xDateRangeError();
  _Year += y;
}

/****************************************************************************/
void Year::SubtractYears(int y)
{
  if (_Year > DateStuff::MIN_YEAR + y)
    xDateRangeError();
  _Year -= y;
}

/****************************************************************************/
DateStuff::Duration Year::Age() const
{
  return AgeBetween(Year());
}

/****************************************************************************/
int Year::EndOfYear() const
{
  if (!IsValid())
    xYearError();
  return DateStuff::EndOfYear(_Year);
}

/****************************************************************************/
Year& Year::operator = (const Year& Obj_)
{
  if (this != &Obj_)
    _Year = Obj_._Year;
  return *this;
}

/****************************************************************************/
int Year::YearsBetween(const Year& Obj_) const
{
  int diff = int(Obj_._Year - _Year);
  return int(diff >= 0 ? diff:-diff);
}

/****************************************************************************/
ChrString Year::ToString() const
{
  return (IsEmpty() ? ChrString():DateStuff::ToString(_Year, 0, 0));
}

//=============================================================
#if OVERLOAD_NEW
void* Year::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
void Year::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

//=============================================================
#if HAS_ARRAY_NEW
void* Year::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

//=============================================================
void Year::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
Boolean YearAcceptor::IsYear() const
{
  return TRUE;
}

/****************************************************************************/
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(YearAcceptor::YearAcceptor, Year)
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(YearAcceptor::AcceptDataPtr, Year)
OBJACCEPTOR_GIVEDATA_TOPDEFN(YearAcceptor::GiveDataPtr)
OBJACCEPTOR_CREATEFNC_TOPDEFN(YearAcceptor::CreateFromYear, Year, Year)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(YearAcceptor::AssignFromYear, Year)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(YearAcceptor::IsLesserToYear, _ObjPtr->IsLesserToYear, Year)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(YearAcceptor::IsEqualToYear, _ObjPtr->IsEqualToYear, Year)
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(YearAcceptor::IsGreaterToYear, _ObjPtr->IsGreaterToYear, Year)
MEMORYOPS_DEFN(YearAcceptor)
OBJACCEPTOR_CREATEFNC_TOPDEFN(YearAcceptor::CreateFromChrString, Year, ChrString)
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(YearAcceptor::AssignFromChrString, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(YearAcceptor::IsLesserToChrString, _ObjPtr->IsLesserToYear, Year, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(YearAcceptor::IsEqualToChrString, _ObjPtr->IsEqualToYear, Year, ChrString)
OBJACCEPTOR_COMPFNC_TOPDEFN(YearAcceptor::IsGreaterToChrString, _ObjPtr->IsGreaterToYear, Year, ChrString)

/****************************************************************************/
#endif




