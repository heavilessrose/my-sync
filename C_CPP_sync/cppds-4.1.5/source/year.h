#ifndef YEAR_H
#define YEAR_H
#ifndef DATESTUFF_H
  #include "datestuff.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif

/****************************************************************************/
class YearAcceptor : public ObjectAcceptor
{
  friend class Year;

  protected:
    union
    {
      const Year* _ObjPtr;
      Year* _VarPtr;
    };

    YearAcceptor(const Year* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Year class specific acceptor methods
    virtual Boolean IsYear() const;
    
    virtual void* CreateFromYear(const ObjectAcceptor& Obj_);
    virtual void* AssignFromYear(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);    
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);    
    
    virtual Boolean IsEqualToYear(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToYear(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToYear(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;    

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class Year : public ObjectSkeleton
{
  friend class YearAcceptor;

  protected:
    int _Year;

    static void xDateRangeError()
        { THROW (FallibleBase::DateRangeErr()); }
    static void xYearError()
        { THROW (FallibleBase::InvalidYearErr()); }

    virtual Boolean IsEqualToYear(const Year* Ptr_) const;
    virtual Boolean IsLesserToYear(const Year* Ptr_) const;
    virtual Boolean IsGreaterToYear(const Year* Ptr_) const;        
  
  public:
    Year();
    Year(int Val_);
    Year(const ChrString& Str_);
    Year(const Year& Obj_);

    static Year* Make();
    static Year* Make(int Val_);
    static Year* Make(const ChrString& Str_);
    static Year* Make(const Year& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Year* CloneYear() const;    

    // Static null object accessor methods
    static Year& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::YEAR_TYPE; }
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

    // storing/restoring from binary from
    long DataSize() const;
    
    int GetYear() const
        { return _Year; }

    // Comparison methods    
    int Compare(const Year& Yr_) const
        { return (_Year - Yr_._Year); }
    Boolean operator == (const Year& Yr1_) const
        { return (Compare(Yr1_) == 0); }
    Boolean operator != (const Year& Yr1_) const
        { return (Compare(Yr1_) != 0); }
    Boolean operator < (const Year& Yr1_) const
        { return (Compare(Yr1_) < 0); }
    Boolean operator > (const Year& Yr1_) const
        { return (Compare(Yr1_) > 0); }
    Boolean operator <= (const Year& Yr1_) const
        { return (Compare(Yr1_) <= 0); }
    Boolean operator >= (const Year& Yr1_) const
        { return (Compare(Yr1_) >= 0); }

    // Arithmetic
    int operator - (const Year& Yr1_) const
        { return Compare(Yr1_); }
    virtual Year& operator = (const Year& Obj_);

    // Other operations
    virtual void AddYears(int Years_);
    virtual void SubtractYears(int Years_);
    int YearsBetween(const Year& Obj_) const;
    DateStuff::Duration AgeBetween(const Year& Obj_) const
        { return DateStuff::Duration(YearsBetween(Obj_)); }
    DateStuff::Duration Age() const;
    int EndOfYear() const;

    ChrString ToString() const;
    Boolean IsValid() const
        { return (DateStuff::MIN_YEAR <= _Year && _Year <= DateStuff::MAX_YEAR); }
    Boolean IsEmpty() const
        { return (_Year == 0); }

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




