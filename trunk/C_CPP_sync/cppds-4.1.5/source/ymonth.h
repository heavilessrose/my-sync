#ifndef YMONTH_H
#define YMONTH_H
#ifndef YEAR_H
  #include "year.h"
#endif

/****************************************************************************/
class YMonthAcceptor : public ObjectAcceptor
{
  friend class YMonth;

  protected:
    union
    {
      const YMonth* _ObjPtr;
      YMonth* _VarPtr;
    };

    YMonthAcceptor(const YMonth* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // YMonth class specific acceptor methods
    virtual Boolean IsYMonth() const;
    virtual Boolean IsYear() const;

    virtual void* CreateFromYMonth(const ObjectAcceptor& Obj_);
    virtual void* AssignFromYMonth(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);    

    virtual Boolean IsEqualToYMonth(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToYMonth(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToYMonth(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;    

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class YMonth : public Year
{
  friend class YMonthAcceptor;

  private:
    typedef Year Super;
    
    void Resolve(long months)
        { DateStuff::ResolveMonths(months, _Year, _Month); }
    static void xMonthError()
        { THROW (FallibleBase::InvalidMonthErr()); }

  protected:
    int _Month;

    virtual Boolean IsEqualToYMonth(const YMonth* Ptr_) const;
    virtual Boolean IsLesserToYMonth(const YMonth* Ptr_) const;
    virtual Boolean IsGreaterToYMonth(const YMonth* Ptr_) const;

  public:
    YMonth();
    YMonth(int y, int m);
    YMonth(const ChrString& s);
    YMonth(const YMonth& Obj_);

    static YMonth* Make();
    static YMonth* Make(int y, int m);
    static YMonth* Make(const ChrString& s);
    static YMonth* Make(const YMonth& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual YMonth* CloneYMonth() const;

    // Static null object accessor methods
    static YMonth& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::YMONTH_TYPE; }
    virtual int ClassID() const;    
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Comparisons:
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

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

    // storing/restoring from binary form
    long DataSize() const;

    // Accessors
    int GetMonth() const
        { return _Month; }

    // Comparison
    int Compare(const YMonth& right) const;
    Boolean operator == (const YMonth& right) const
        { return (Compare(right) == 0); }
    Boolean operator != (const YMonth& right) const
        { return (Compare(right) != 0); }
    Boolean operator < (const YMonth& right) const
        { return (Compare(right) < 0); }
    Boolean operator > (const YMonth& right) const
        { return (Compare(right) > 0); }
    Boolean operator <= (const YMonth& right) const
        { return (Compare(right) <= 0); }
    Boolean operator >= (const YMonth& right) const
        { return (Compare(right) >= 0); }

    // Arithmetic
    int operator - (const YMonth& right) const
        { return ((_Year - right._Year) * 12L + _Month - right._Month); }
    virtual YMonth& operator = (const YMonth& Obj_);

    // Other operations
    virtual void AddMonths(long months);
    virtual void SubtractMonths(long months);
    long MonthsBetween(const YMonth& right) const;
    DateStuff::Duration AgeBetween(const YMonth& right) const
        { return AgeBetween(right, *this); }
    DateStuff::Duration Age() const;
    int EndOfYear() const;
    int EndOfMonth() const;
    int DaysInPrevMonth() const;
    int DaysInNextMonth() const;

    ChrString ToString() const;
    Boolean IsValid() const
        { return (Super::IsValid() && (1 <= _Month && _Month <= 12)); }
    Boolean IsEmpty() const
        { return (Super::IsEmpty() && _Month == 0); }

    static DateStuff::Duration AgeBetween(const YMonth& first,
                                          const YMonth& last)
        { return DateStuff::AgeBetween(first._Year, first._Month, 0,
                                       last._Year, last._Month, 0); }

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




