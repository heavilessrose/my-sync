#ifndef TIMEOFDAY_H
#define TIMEOFDAY_H

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
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif

#define TIMEOFDAY_DEBUG         0

/****************************************************************************/
class TimeOfDay;

class TimeOfDayAcceptor : public ObjectAcceptor
{
  friend class TimeOfDay;

  protected:
    union
    {
      const TimeOfDay* _ObjPtr;
      TimeOfDay* _VarPtr;
    };

    TimeOfDayAcceptor(const TimeOfDay* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // TimeOfDay class specific acceptor methods
    virtual Boolean IsTimeOfDay() const;
    
    virtual void* CreateFromTimeOfDay(const ObjectAcceptor& Obj_);
    virtual void* AssignFromTimeOfDay(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);    
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);    

    virtual Boolean IsEqualToTimeOfDay(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToTimeOfDay(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToTimeOfDay(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;    

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class TimeOfDay : public ObjectSkeleton
{
  friend class TimeOfDayAcceptor;

  friend Object& RootObject(TimeOfDay& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const TimeOfDay& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:
    short _Days;  
    short _Hour;
    short _Minute;
    short _Second;

    static void xTimeError()
        { THROW (FallibleBase::InvalidTimeErr()); }

    Boolean StringToTime(const ChrString& s);
    void RollOver()
        { if (_Hour >= 24) _Hour = 0; }    
    
    static Boolean IsValidTime(short hour, short minute, short second);
    Boolean IsValidTime()
        { return IsValidTime(_Hour, _Minute, _Second); }

    virtual Boolean IsEqualToTimeOfDay(const TimeOfDay* Ptr_) const;
    virtual Boolean IsLesserToTimeOfDay(const TimeOfDay* Ptr_) const;
    virtual Boolean IsGreaterToTimeOfDay(const TimeOfDay* Ptr_) const;

  public:
    enum { TIME_NORMAL, TIME_MILITARY };

    TimeOfDay();
    TimeOfDay(short hour, short minute, short second);
    TimeOfDay(const ChrString& s);
    TimeOfDay(long nsecs);
    TimeOfDay(const TimeOfDay& Obj_);

    static TimeOfDay* Make();
    static TimeOfDay* Make(short hour, short minute, short second);
    static TimeOfDay* Make(const ChrString& s);
    static TimeOfDay* Make(long nsecs);
    static TimeOfDay* Make(const TimeOfDay& Obj_);

    TimeOfDay& operator = (const ChrString& s);
    TimeOfDay& operator = (long nsecs);
    TimeOfDay& operator = (const TimeOfDay& Obj_);

    TimeOfDay& operator += (long numsecs);
    TimeOfDay& operator -= (long numsecs);
    TimeOfDay& AddTime(short hour, short minute, short second);
    TimeOfDay& SubtractTime(short hour, short minute, short second);
    static int AddTime(short basehrs, short basemins, short basesecs,
                       short spanhrs, short spanmins, short spansecs,
                       short *hrs, short *mins, short *secs, short *days);        

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static TimeOfDay& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();    

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::TIMEOFDAY_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;    

    // Comparisons:
    long Compare(const TimeOfDay& Obj_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // return a time object containing the current time
    static TimeOfDay Now();
    static long ParseAndConvert(const ChrString& s);
    static void GetDtime(struct tm *dtime);
    
    short Hour() const
        { return _Hour; }
    short Minute() const
        { return _Minute; }
    short Second() const
        { return _Second; }
    short Days() const
        { return _Days; }

    ChrString AsString() const;
    ChrString PrintString(int Form_=TIME_NORMAL) const;

    Boolean IsBetween(const TimeOfDay& t1, const TimeOfDay& t2);
    long ToSeconds() const;    
    
    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;    

    // arithmetic operations
    long operator - (const TimeOfDay& Obj_) const;
    TimeOfDay operator + (long numsecs) const;
    TimeOfDay operator - (long numsecs) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
#endif




