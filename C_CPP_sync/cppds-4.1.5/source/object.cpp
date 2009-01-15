//////////////////////////////////////////////////////////////////////////////
// FILE          : OBJECT.HD
// PURPOSE       : Base object class type. This is the base super class
//                 for which all other classes are derived from.
//                 Contains procedures, functions and operations common
//                 to all classes which can instantiate objects.
//
// WRITTEN BY    :  Joseph Wong
//
// MODIFIED BY:         LAST MODIFIED:  COMMENTS:
// ------------         --------------  ---------
// Joseph Wong          Dec 21 1996     Original Writing
//
//////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_CPP
#define OBJECT_CPP
#ifndef OBJECT_H
  #include "object.h"
#endif

/****************************************************************************/
long IosFormat::SetFormat(long Flags_)
{
  long Old_ = _FormatFlags;
  _FormatFlags = Flags_;
  return Old_;
}

/****************************************************************************/
int IosFormat::SetPrecision(int Prec_)
{
  int Old_ = _Precision;
  _Precision = Prec_;
  return Old_;
}

/****************************************************************************/
int IosFormat::SetWidth(int Width_)
{
  int Old_ = _Width;
  _Width = Width_;
  return Old_;
}

/****************************************************************************/
char IosFormat::SetFill(char FillCh_)
{
  int Old_ = _FillCh;
  _FillCh = FillCh_;
  return Old_;
}

/****************************************************************************/
/****************************************************************************/
ios& operator << (ios& IosObj_, IosSetFunctor Form_)
{
  // invoke hidden function and pass data member to it
  return ((*Form_.GetFncPtr())(IosObj_, Form_.GetIosFormat()));
}

/****************************************************************************/
ios& operator >> (ios& IosObj_, IosSetFunctor Form_)
{
  // invoke hidden function and pass data member to it
  return ((*Form_.GetFncPtr())(IosObj_, Form_.GetIosFormat()));
}

/****************************************************************************/
ios& operator << (ios& IosObj_, IosResetFunctor Form_)
{
  // invoke hidden function and pass data member to it
  return ((*Form_.GetFncPtr())(IosObj_));
}

/****************************************************************************/
ios& operator >> (ios& IosObj_, IosResetFunctor Form_)
{
  // invoke hidden function and pass data member to it
  return ((*Form_.GetFncPtr())(IosObj_));
}

/****************************************************************************/
ios& IosReset_Hidden(ios& IosObj_)
{
  IosObj_.setf(ios::skipws | ios::dec);
  IosObj_.precision(6);
  IosObj_.width(0);
  IosObj_.fill(' ');

  return IosObj_;
}

/****************************************************************************/
ios& IosSet_Hidden(ios& IosObj_, IosFormat& IosForm_)
{
#if (!defined(__DJGPP__) & !defined(__linux__) & !defined(__unix__) & !defined(__GNUC__))
  IosObj_.setf(IosForm_.GetFormat());
#else
  IosObj_.setf((std::_Ios_Fmtflags)IosForm_.GetFormat());
#endif
  IosObj_.precision(IosForm_.GetPrecision());
  IosObj_.width(IosForm_.GetWidth());
  IosObj_.fill(IosForm_.GetFill());

  return IosObj_;
}

/****************************************************************************/
IosResetFunctor IosReset()
{
  return IosResetFunctor(IosReset_Hidden);
}

/****************************************************************************/
IosSetFunctor IosSet(IosFormat& IosForm_)
{
  return IosSetFunctor(IosSet_Hidden, IosForm_);
}

/****************************************************************************/
/****************************************************************************/
IOstateString::IOstateString()
{
  memset(_Buffer, 0, 256);
}

/****************************************************************************/
IOstateString::IOstateString(const IOstateString& Obj_)
{
  strcpy(_Buffer, Obj_._Buffer);
}

/****************************************************************************/
IOstateString::IOstateString(const char* Str_)
{
  strcpy(_Buffer, Str_);
}

/****************************************************************************/
IOstateString& IOstateString::operator += (const char* Str_)
{
  strcat(_Buffer, Str_);
  return *this;
}

/****************************************************************************/
IOstateString::operator const char* ()
{
  return _Buffer;
}

/****************************************************************************/
/****************************************************************************/
IOstateString GetIOstateString(Object::IOstate Status_)
{
  IOstateString IOstr_;

  if (Status_ | Object::IO_INVALID)
  {
    if (Status_ | Object::IO_STREAMERROR)
      IOstr_ += "\nIO Stream Error";

    if (Status_ | Object::IO_OBJECTIDERROR)
      IOstr_ += "\nObject ID Error";

    if (Status_ | Object::IO_OBJECTDATAERROR)
      IOstr_ += "\nObject Data Error";

    if (Status_ | Object::IO_NOTDONE)
      IOstr_ += "\nIO Not Done";
  }
  else
    IOstr_ += "\nNo IO Errors";

  return IOstr_;
}

/****************************************************************************/
istream& operator >> (istream& is, Object& obj)
{
  return obj.TextRead(is);
}

/****************************************************************************/
ostream& operator << (ostream& os, const Object& obj)
{
  return obj.TextWrite(os);
}

/****************************************************************************/
ByteStream& operator >> (ByteStream& is, Object& obj)
{
  return obj.BinaryRead(is);
}

/****************************************************************************/
ByteStream& operator << (ByteStream& os, const Object& obj)
{
  return obj.BinaryWrite(os);
}

/****************************************************************************/
Boolean StoreObjectData(ObjectData& Obj_, istream& Is_)
{
  Is_.read((char*)(&Obj_._IDvalue), sizeof(long));
  if (Is_.good())
  {
    Is_.read((char*)(&Obj_._ObjectID), sizeof(long));
    if (Is_.good())
      Is_.read((char*)(&Obj_._NullStatus), sizeof(Byte_t));
  }

  if (Is_.good())
    Is_.seekg(-long(sizeof(long) +
                    sizeof(long) +
                    sizeof(Byte_t)),
              ios::cur);

  return Is_.good();
}

/****************************************************************************/
Boolean StoreObjectData(ObjectData& Obj_, ByteStream& Is_)
{
  Is_.iread((char*)(&Obj_._IDvalue), sizeof(long));
  if (Is_.Good())
  {
    Is_.iread((char*)(&Obj_._ObjectID), sizeof(long));
    if (Is_.Good())
      Is_.iread((char*)(&Obj_._NullStatus), sizeof(Byte_t));
  }

  if (Is_.Good())
    Is_.iseekg(-long(sizeof(long) +
                     sizeof(long) +
                     sizeof(Byte_t)),
               ios::cur);

  return Is_.Good();
}

/****************************************************************************/
// ----- Saving and restoration in binary form ----
Boolean ReadObjectData(Object& Obj_, istream& Is_, Boolean Ignore_)
{
  Boolean StreamOk_;
  Ulong ObjectID_;
  Byte_t IsNull_;
  long IDvalue_;

  if (Obj_.IsIOstateFinal())
    return Obj_.IsIOError();

  Obj_.ClearIOstate();  
  Is_.read((char*)(&IDvalue_), sizeof(long));

  if ((StreamOk_ = Obj_.IstreamCheck(Is_)) && Obj_.CheckClassType(IDvalue_))
  {
    Is_.read((char*)(&ObjectID_), sizeof(long));
    
    if (Is_.good())
    {
      if (!Ignore_)
        Obj_.SetObjectID(ObjectID_);
        
      Is_.read((char*)(&IsNull_), sizeof(Byte_t));
      if (Is_.good() && !Ignore_)
        Obj_.SetAsNullObject(IsNull_);
    }
  }
  else
  {
    if (StreamOk_)
      Is_.seekg(-long(sizeof(long)), ios::cur);

    return FALSE;
  }

  return Obj_.IstreamCheck(Is_);
}

/****************************************************************************/
Boolean WriteObjectData(const Object& Obj_, ostream& Os_)
{
  Byte_t IsNull_ = Obj_.IsNullObject();
  long IDvalue_ = Obj_.ClassID();
  Ulong ObjectID_ = Obj_.ObjectID();

  if (Obj_.IsIOstateFinal())
    return Obj_.IsIOError();

  Obj_.ClearIOstate();
  Os_.write((char*)(&IDvalue_), sizeof(long));

  if (Obj_.OstreamCheck(Os_))
  {
    Os_.write((char*)(&ObjectID_), sizeof(long));
    if (Obj_.OstreamCheck(Os_))
      Os_.write((char*)(&IsNull_), sizeof(Byte_t));
  }

  return Obj_.OstreamCheck(Os_);
}

/****************************************************************************/
// ----- Saving and restoration in binary form ----
Boolean ReadObjectData(Object& Obj_, ByteStream& Is_, Boolean Ignore_)
{
  Boolean StreamOk_;
  Ulong ObjectID_;
  Byte_t IsNull_;
  long IDvalue_;

  if (Obj_.IsIOstateFinal())
    return Obj_.IsIOError();

  Obj_.ClearIOstate();
  Is_.iread((char*)(&IDvalue_), sizeof(long));

  if ((StreamOk_ = Obj_.IstreamCheck(Is_)) && Obj_.CheckClassType(IDvalue_))
  {
    Is_.iread((char*)(&ObjectID_), sizeof(long));

    int Good_ = Is_.Good();
    if (Good_)
    {
      if (!Ignore_)    
        Obj_.SetObjectID(ObjectID_);
        
      Is_.iread((char*)(&IsNull_), sizeof(Byte_t));
      Good_ = Is_.Good();
      if (Good_ && !Ignore_)
        Obj_.SetAsNullObject(IsNull_);
    }
  }
  else
  {
    if (StreamOk_)
      Is_.iseekg(-long(sizeof(long)), ios::cur);

    return FALSE;
  }

  return Obj_.IstreamCheck(Is_);
}

/****************************************************************************/
Boolean WriteObjectData(const Object& Obj_, ByteStream& Os_)
{
  Byte_t IsNull_ = Obj_.IsNullObject();
  long IDvalue_ = Obj_.ClassID();
  Ulong ObjectID_ = Obj_.ObjectID();

  if (Obj_.IsIOstateFinal())
    return Obj_.IsIOError();

  Obj_.ClearIOstate();
  Os_.owrite((char*)(&IDvalue_), sizeof(long));

  if (Obj_.OstreamCheck(Os_))
  {
    Os_.owrite((char*)(&ObjectID_), sizeof(long));
    if (Obj_.OstreamCheck(Os_))
      Os_.owrite((char*)(&IsNull_), sizeof(Byte_t));
  }

  return Obj_.OstreamCheck(Os_);
}

/****************************************************************************/
/****************************************************************************/
Boolean operator == (const Object& obj1, const Object& obj2)
{
  return obj1.IsEqual(obj2);
}

/****************************************************************************/
Boolean operator != (const Object& obj1, const Object& obj2)
{
  return !obj1.IsEqual(obj2);
}

/****************************************************************************/
Boolean operator > (const Object& obj1, const Object& obj2)
{
  return obj1.IsGreater(obj2);
}

/****************************************************************************/
Boolean operator < (const Object& obj1, const Object& obj2)
{
  return obj1.IsLesser(obj2);
}

/****************************************************************************/
Boolean operator >= (const Object& obj1, const Object& obj2)
{
  return (obj1.IsGreater(obj2) || obj1.IsEqual(obj2));
}

/****************************************************************************/
Boolean operator <= (const Object& obj1, const Object& obj2)
{
  return (obj1.IsLesser(obj2) || obj1.IsEqual(obj2));
}

/****************************************************************************/
Ulong AssociatedIndex(const Object& Obj_, Ulong TableSize_)
{
  return ((Obj_.ObjectID() >= TableSize_) ?
	  (Obj_.ObjectID() % TableSize_):Obj_.ObjectID());
}

/****************************************************************************/
/****************************************************************************/
Object::~Object()
{}

/****************************************************************************/
Boolean Object::IsSummableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean Object::IsMultipliableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean Object::IsOrdinalType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean Object::IsInvertableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean Object::IsLogicalType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean Object::IsBitwiseableType() const
{
  return FALSE;
}

/****************************************************************************/
Boolean Object::IsStreamableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean Object::IsSortableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean Object::IsMatchableType() const
{
  return TRUE;
}

/****************************************************************************/
Boolean Object::CheckClassType(int IDvalue_) const
{
  Boolean Match_ = IsCommonClass(IDvalue_);
  SetIOstate(Match_, IO_OBJECTIDERROR);
  return Match_;
}

/****************************************************************************/
Boolean Object::IsCommonClass(int IDvalue_) const
{
  return (IDvalue_ == ClassID());
}

/****************************************************************************/
int Object::ClassID() const
{
  return TypeInfo::OBJECT_TYPE;
}

/****************************************************************************/
const char* Object::ClassName() const
{
  return NameOf(*this);
}

/****************************************************************************/
Boolean Object::IsEqual(const Object& Obj_, const ObjectAcceptor*) const
{
  return (this == &Obj_);
}

/****************************************************************************/
Boolean Object::IsLesser(const Object& Obj_, const ObjectAcceptor*) const
{
  return (this < &Obj_);
}

/****************************************************************************/
Boolean Object::IsGreater(const Object& Obj_, const ObjectAcceptor*) const
{
  return (this > &Obj_);
}

/****************************************************************************/
void Object::ClearIOstate() const
{
  SetIOstate(TRUE, IO_RESET, FALSE);
}

/****************************************************************************/
#endif




