#ifndef DATE_H
#define DATE_H
#ifndef YMONTH_H
  #include "ymonth.h"
#endif
#ifndef STRUTIL_H
  #include "strutil.h"
#endif

/****************************************************************************/
class DateAcceptor : public ObjectAcceptor
{
  friend class Date;

  protected:
    union
    {
      const Date* _ObjPtr;
      Date* _VarPtr;
    };

    DateAcceptor(const Date* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Date class specific acceptor methods
    virtual Boolean IsDate() const;
    
    virtual void* CreateFromDate(const ObjectAcceptor& Obj_);
    virtual void* CreateFromYear(const ObjectAcceptor& Obj_);
    virtual void* CreateFromYMonth(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromDate(const ObjectAcceptor& Obj_);
    virtual void* AssignFromYear(const ObjectAcceptor& Obj_);
    virtual void* AssignFromYMonth(const ObjectAcceptor& Obj_);    
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToDate(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToYear(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToYMonth(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsGreaterToDate(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsGreaterToYear(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToYMonth(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;
    
    virtual Boolean IsLesserToDate(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToYear(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToYMonth(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class Date : public ObjectSkeleton
{
  friend class DateAcceptor;

  friend Object& RootObject(Date& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const Date& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:
    int _Year;
    int _Month;
    int _Day;

    static void xDateError()
        { THROW (FallibleBase::InvalidDateErr()); }

    virtual Boolean IsEqualToDate(const Date* Ptr_) const;
    virtual Boolean IsLesserToDate(const Date* Ptr_) const;
    virtual Boolean IsGreaterToDate(const Date* Ptr_) const;

  public:
    Date(int = 0, int = 0, int = 0);
    Date(const ChrString& s);
    Date(const Year& Obj_);
    Date(const YMonth& Obj_);
    Date(const Date& Obj_);

    static Date* Make(int = 0, int = 0, int = 0);
    static Date* Make(const ChrString& s);
    static Date* Make(const Year& Obj_);
    static Date* Make(const YMonth& Obj_);
    static Date* Make(const Date& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Date& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Additional assignment (for efficiency):
    Date& operator = (const ChrString& s);
    Date& operator = (const Year& Obj_);
    Date& operator = (const YMonth& Obj_);
    Date& operator = (const Date& Obj_);

    // Accessors
    int GetYear() const
        { return _Year; }
    int GetMonth() const
        { return _Month; }
    int GetDay() const
        { return _Day; }

    // Tests:
    Boolean IsValidYear() const
        { return DateStuff::IsValidYear(_Year); }
    Boolean IsValidYMonth() const
        { return DateStuff::IsValidYMonth(_Year, _Month); }
    Boolean IsValidYMDay() const
        { return DateStuff::IsValidYMDay(_Year, _Month, _Day); }
    virtual Boolean IsValid() const;
    Boolean IsEmpty() const;

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::DATE_TYPE; }
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
    int Compare(const Date& Obj_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;    

    // Basic Calculations:
    void AddYears(int);
    void SubtractYears(int);
    int YearsBetween(const Date&);
    void AddMonths(long);
    void SubtractMonths(long);
    long MonthsBetween(const Date&) const;
    void AddDays(long);
    void SubtractDays(long);
    long DaysBetween(const Date&) const;
    void AddWeeks(long weeks);
    void SubtractWeeks(long weeks);
    long WeeksBetween(const Date&) const;
    void AddWeekDays(long days);
    void SubtractWeekDays(long days);
    long WeekDaysBetween(const Date& right) const;
    DateStuff::Duration operator - (const Date&) const;

    // Special Requests:
    int DayOfWeek() const;
    int DayOfYear() const;
    int EndOfMonth() const;
    int EndOfYear() const;
    DateStuff::Duration AgeBetween(const Date&) const;
    DateStuff::Duration Age() const;
    static Date Today();

    // Conversions:
    ChrString ToString() const;

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




