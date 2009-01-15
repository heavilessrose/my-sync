#ifndef FUZZYSET_H
#define FUZZYSET_H
#ifndef LANGSUPP_H
  #include "langsupp.h"
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef SIMPLELIST_CPP
  #include "simplelist.cpp"
#endif

#define FUZZYSET_DEBUG               0
#define FUZZYSET_NOTIFY_SUCCESS      0

/****************************************************************************/
template <class KEY, class VALUE>
struct FuzzyNode
{
  friend FuzzyNode<KEY,VALUE>& RootObject(FuzzyNode<KEY,VALUE>& Obj_)
      { return Obj_; }
  friend const FuzzyNode<KEY,VALUE>& RootConstObject(const FuzzyNode<KEY,VALUE>& Obj_)
      { return Obj_; }
  friend int _WhatIs(const FuzzyNode<KEY,VALUE>& Obj_)
      { return TypeInfo::EXTERNALCLASS; }

  friend ostream& operator << (ostream& os, const FuzzyNode<KEY, VALUE>& obj)
      { os <<obj._Key <<"\t" <<obj._Value <<endl;
        return os; }
  friend istream& operator >> (istream& is, FuzzyNode<KEY, VALUE>& obj)
      { is >>obj._Key >>obj._Value;
        return is; }

  KEY _Key;
  VALUE _Value;

  FuzzyNode():
    _Key(KEY()), _Value(VALUE()) {}
  FuzzyNode(const FuzzyNode<KEY, VALUE>& Obj_):
    _Key(Obj_._Key), _Value(Obj_._Value) {}
  FuzzyNode(const KEY& x, const VALUE& y):
    _Key(x), _Value(y) {}

  FuzzyNode<KEY,VALUE>& operator = (const FuzzyNode<KEY,VALUE>& Obj_);

  void* operator new (size_t Bytes_);
  void operator delete (void* Space_);
  void* operator new[] (size_t Bytes_);
  void operator delete[] (void* Space_);
};

/****************************************************************************/
template <class KEY, class VALUE>
void* FuzzyNode<KEY,VALUE>::operator new (size_t Bytes_)
{
    return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class KEY, class VALUE>
void FuzzyNode<KEY,VALUE>::operator delete (void* Space_)
{
    MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
template <class KEY, class VALUE>
void* FuzzyNode<KEY,VALUE>::operator new[] (size_t Bytes_)
{
    return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class KEY, class VALUE>
void FuzzyNode<KEY,VALUE>::operator delete[] (void* Space_)
{
    MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean operator == (const FuzzyNode<KEY, VALUE>& x, const FuzzyNode<KEY, VALUE>& y)
{
  return (x._Value == y._Value);
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean operator != (const FuzzyNode<KEY, VALUE>& x, const FuzzyNode<KEY, VALUE>& y)
{
  return (x._Value != y._Value);
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean operator < (const FuzzyNode<KEY, VALUE>& x, const FuzzyNode<KEY, VALUE>& y)
{
  return (x._Value < y._Value);
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean operator > (const FuzzyNode<KEY, VALUE>& x, const FuzzyNode<KEY, VALUE>& y)
{
  return (x._Value > y._Value);
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean operator <= (const FuzzyNode<KEY, VALUE>& x, const FuzzyNode<KEY, VALUE>& y)
{
  return (x._Value <= y._Value);
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean operator >= (const FuzzyNode<KEY, VALUE>& x, const FuzzyNode<KEY, VALUE>& y)
{
  return (x._Value >= y._Value);
}

/****************************************************************************/
template <class T>
class FuzzySetTypeDefn
{
  public:
    typedef FuzzyNode<double,T> NodePtr;
    typedef SearchableList<NodePtr> NodeStore;
    typedef SimpleNode<NodePtr> NodeListNode;
    typedef SimpleListIterator<NodePtr> NodeIter;
    typedef ConstListIterator<NodePtr> ConstIter;
};

template <class T>
class FuzzySet : public ObjectSkeleton
{
  public:
    typedef int(*EqualFuncType)(const T&, const T&);

  protected:
    TYPENAME FuzzySetTypeDefn<T>::NodeStore _SetData;
    static EqualFuncType _EqualFunc;  // Comparison function for list items

    inline void XFuzzyRange() const
	{ THROW (FallibleBase::FuzzyRangeErr()); }
  
  public:
    FuzzySet();                      // An empty set
    FuzzySet(const FuzzySet<T>& Data_);   // Duplicate the set
    FuzzySet(FuzzyNode<double,T>* Ptr_);

    static FuzzySet<T>* Make();
    static FuzzySet<T>* Make(const FuzzySet<T>& Data_);
    static FuzzySet<T>* Make(FuzzyNode<double,T>* Ptr_);

    // List destruction method
    virtual void Flush();

    // list size method
    virtual size_t Size() const;

    // item search method
    virtual const FuzzyNode<double,T>* FindItem(const FuzzyNode<double,T>& Obj_) const;
    virtual FuzzyNode<double,T>* FindItem(const FuzzyNode<double,T>& Obj_);

    // set empty method
    virtual Boolean IsEmpty() const;

    // memory allocation for object?
    virtual Boolean IsNewed() const;

    // Set comparison function method
    static void SetComparisonFunction(int(*EqFnc_)(const T&, const T&));

    // Fuzzy set assignment operator
    FuzzySet<T>& operator = (const FuzzySet<T>& Data_);

    // Static null object accessor methods
    static FuzzySet<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::FUZZYSET_TYPE; }
    virtual int ClassID() const;

    // object cloning method
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;        
    virtual FuzzySet<T>* CloneSet() const;

    // Binary operations and relations on (Set,Set)
    // (also |=, &=, -=, <, <= variants)
    FuzzySet<T> operator | (const FuzzySet<T>& Data_) const; // Union of sets
    FuzzySet<T> operator & (const FuzzySet<T>& Data_) const; // Intersection
    FuzzySet<T> operator - (const FuzzySet<T>& Data_) const; // Set difference

    FuzzySet<T>& operator |= (const FuzzySet<T>& Data_);     // Union of sets
    FuzzySet<T>& operator &= (const FuzzySet<T>& Data_);     // Intersection
    FuzzySet<T>& operator -= (const FuzzySet<T>& Data_);     // Set difference
    
    Boolean operator > (const FuzzySet<T>& Data_) const; // True if
	// this is a proper superset of the argument
    Boolean operator >= (const FuzzySet<T>& Data_) const; // True if
	// this is a superset of the argument
    Boolean operator < (const FuzzySet<T>& Data_) const; // True if
	// the argument is a proper subset of this
    Boolean operator <= (const FuzzySet<T>& Data_) const; // True if
	// the argument is a subset of this
    Boolean operator == (const FuzzySet<T>& Data_) const; // True if
	// the sets have the same content
    Boolean operator != (const FuzzySet<T>& Data_) const; // True if
	// the sets does not have the same content
    Boolean Disjoint(const FuzzySet<T>& Data_) const; // True if
        // the sets do not share any common elements

	// Binary operations, relations on (Set,T*)
	// (also |=, -= variants)
    FuzzySet<T> operator | (FuzzyNode<double,T>* Ptr_) const; // Add T to this
    FuzzySet<T> operator - (FuzzyNode<double,T>* Ptr_) const; // this minus the T

    FuzzySet<T>& operator |= (FuzzyNode<double,T>* Ptr_); // Add T to this
    FuzzySet<T>& operator -= (FuzzyNode<double,T>* Ptr_); // this minus the T
    
    Boolean operator > (const FuzzyNode<double,T>* Ptr_) const; // True if
	// T is in the set but not the only T
    Boolean operator >= (const FuzzyNode<double,T>* Ptr_) const; // True if
	// T is in the set
    Boolean operator == (const FuzzyNode<double,T>* Ptr_) const; // True if
	// T is the only T in the set

    // give complement of set
    FuzzySet<T> Complement() const;

    TYPENAME FuzzySetTypeDefn<T>::NodeStore& GiveSetData();
    const TYPENAME FuzzySetTypeDefn<T>::NodeStore& GiveSetData() const;

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




