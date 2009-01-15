#ifndef STATARRAY_H
#define STATARRAY_H
#ifndef DYNARRAY_CPP
  #include "dynarray.cpp"
#endif
#ifndef SORTS_CPP
  #include "sorts.cpp"
#endif
#ifndef INDEX_H
  #include "index.h"
#endif

/****************************************************************************/
template <class T>
class StatArray;

template <class T>
class StatArrayAcceptor : public ObjectAcceptor
{
  friend class StatArray<T>;

  protected:
    union
    {
      const StatArray<T>* _ObjPtr;
      StatArray<T>* _VarPtr;
    };

    StatArrayAcceptor(const StatArray<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Integer class specific acceptor methods
    virtual Boolean IsStatArray() const;
    virtual Boolean IsDynamicArray() const;
    
    virtual void* CreateFromStatArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDynamicArray(const ObjectAcceptor& Obj_);

    virtual void* AssignFromStatArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDynamicArray(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class StatArray : public DynArray<T>
{
  friend Object& RootObject(StatArray<T>& Arr_)
	{ return *((Object*)(&Arr_)); }
  friend const Object& RootConstObject(const StatArray<T>& Arr_)
	{ return *((const Object*)(&Arr_)); }

  protected:
    MUTABLE int _ErrorStat;

    void XInCompat() const
	{ THROW (FallibleBase::IncompatableArraysErr()); }

  public:
    enum
    {
      NO_ERROR = 0,
      EMPTY_ARRAY = 1
    };
  
    StatArray();
    StatArray(size_t n, capacity c);
    StatArray(const StatArray<T>& x);
    StatArray(const DynArray<T>& x);
    StatArray(const T& x, size_t n=1);
    StatArray(const T* s, size_t n);

    static StatArray<T>* Make()
	{ return (new StatArray<T>); }
    static StatArray<T>* Make(size_t n, capacity c)
	{ return (new StatArray<T>(n, c)); }
    static StatArray<T>* Make(const Object& Obj_)
	{ StatArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._VarPtr;
	}
    static StatArray<T>* Make(const T& x, size_t n=1)
	{ return (new StatArray<T>(x, n)); }
    static StatArray<T>* Make(const T* s, size_t n)
	{ return (new StatArray<T>(s, n)); }

    static const StatArray<T>* MakeConst(const Object& Obj_)
	{ StatArrayAcceptor<T> TrgComp_(NULL);
	  Obj_.Clone(&TrgComp_);
	  return TrgComp_._ObjPtr;
	}
    static const StatArray<T>* MakeConst(const T& x, size_t n=1)
	{ return (new StatArray<T>(x, n)); }
    static const StatArray<T>* MakeConst(const T* s, size_t n)
	{ return (new StatArray<T>(s, n)); }

    // comparison methods
    StatArray<Boolean> operator == (const StatArray<T>& Obj2_) const;
    StatArray<Boolean> operator != (const StatArray<T>& Obj2_) const;
    StatArray<Boolean> operator > (const StatArray<T>& Obj2_) const;
    StatArray<Boolean> operator < (const StatArray<T>& Obj2_) const;
    StatArray<Boolean> operator >= (const StatArray<T>& Obj2_) const;
    StatArray<Boolean> operator <= (const StatArray<T>& Obj2_) const;

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);

    static DynArray<T>& GiveNullObject();
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::STATARRAY_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Statistics function with file data input
    static T Sum(ifstream& fin, Boolean& Error_);
    static T Product(ifstream& fin, Boolean& Error_);
    static T AvgMean(ifstream& fin, Boolean& Error_);
    static T Variance(ifstream& fin, Boolean& Error_);
    static T AvgDeviation(ifstream& fin, Boolean& Error_);
    static T StdDeviation(ifstream& fin, Boolean& Error_);
    static T Skew(ifstream& fin, Boolean& Error_);
    static T Kurt(ifstream& fin, Boolean& Error_);

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    StatArray<T>& operator = (const StatArray<T>& Arr_);
    StatArray<T>& operator = (const DynArray<T>& Arr_);
    
    StatArray<T>& operator += (const StatArray<T>& Arr_);
    StatArray<T>& operator += (const DynArray<T>& Arr_);

    // minimum, maximum and range calculation
    T Min() const;
    T Max() const;
    void MinMax(T& minimum, T& maximum) const;
    T RangeOf() const;

    // series calculation
    T Sum() const;
    T Product() const;

    // calculate moments of distribution
    T Mean() const;
    T Median() const;
    T** Mode() const;    
    T MedianSort();    

    T Variance() const;

    T StdDeviation() const;
    T AvgDeviation() const;

    T Skew() const;
    T Kurt() const;

    // calculate several values at once
    void Moment(T& Mean_, T& AvgDev_, T& StdDev_, T& Var_, T& Skew_, T& Kurt_) const;

    // calculate standard scores (z-scores)
    T ZScore(const Index& i) const;
    T ZScore(int i) const;
    StatArray<T> ZScore() const;

    // calculate Pearson's coefficient of correlation
    // for a pair of arrays
    T Correlation(const StatArray<T>& dsa);

    void FillRandom(T low, T high);

    int ErrorStatus()
        { return _ErrorStat; }
    void ClearError()
        { _ErrorStatus = NO_ERROR; }

#if OVERLOAD_NEW
    // new & delete operators
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

    // new & delete operators for arrays
#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif        
};

/****************************************************************************/
#endif




