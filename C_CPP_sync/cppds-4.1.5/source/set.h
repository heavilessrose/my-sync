#ifndef SET_H
#define SET_H
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef SIMPLELIST_CPP
  #include "simplelist.cpp"
#endif

#define SET_DEBUG               0
#define SET_NOTIFY_SUCCESS      0

/****************************************************************************/
template <class T>
class Set : public ObjectSkeleton
{
  public:
    typedef int(*EqualFuncType)(const T&, const T&);

  protected:
    SearchableList<T> _SetData;
    static EqualFuncType _EqualFunc;  // Comparison function for list items

  public:
    Set();                      // An empty set
    Set(const Set<T>& Data_);   // Duplicate the set
    Set(T* Ptr_);               // A set with one initial T

    static Set<T>* Make();
    static Set<T>* Make(const Set<T>& Data_);
    static Set<T>* Make(T* Ptr_);

    // List destruction method
    virtual void Flush();

    // list size method
    virtual size_t Size() const;

    // item search method
    virtual const T* FindItem(const T& Obj_) const;
    virtual T* FindItem(const T& Obj_);

    virtual Boolean IsEmpty() const;

    virtual Boolean IsNewed() const;

    // Set comparison function method
    static void SetComparisonFunction(int(*EqFnc_)(const T&, const T&));

    Set<T>& operator = (const Set<T>& Data_);

    // Static null object accessor methods
    static Set<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SET_TYPE; }
    virtual int ClassID() const;

    // object cloning method
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;        
    virtual Set<T>* CloneSet() const;

    // Binary operations and relations on (Set,Set)
    // (also |=, &=, -=, <, <= variants)
    Set<T> operator | (const Set<T>& Data_) const; // Union of sets
    Set<T> operator & (const Set<T>& Data_) const; // Intersection
    Set<T> operator - (const Set<T>& Data_) const; // Set difference

    Set<T>& operator |= (const Set<T>& Data_);     // Union of sets
    Set<T>& operator &= (const Set<T>& Data_);     // Intersection
    Set<T>& operator -= (const Set<T>& Data_);     // Set difference
    
    Boolean operator > (const Set<T>& Data_) const; // True if
	// this is a proper superset of the argument
    Boolean operator >= (const Set<T>& Data_) const; // True if
	// this is a superset of the argument
    Boolean operator < (const Set<T>& Data_) const; // True if
	// the argument is a proper subset of this
    Boolean operator <= (const Set<T>& Data_) const; // True if
	// the argument is a subset of this
    Boolean operator == (const Set<T>& Data_) const; // True if
	// the sets have the same content
    Boolean operator != (const Set<T>& Data_) const; // True if
	// the sets does not have the same content
    Boolean Disjoint(const Set<T>& Data_) const; // True if
        // the sets do not share any common elements

	// Binary operations, relations on (Set,T*)
	// (also |=, -= variants)
    Set<T> operator | (T* Ptr_) const; // Add T to this
    Set<T> operator - (T* Ptr_) const; // this minus the T

    Set<T>& operator |= (T* Ptr_); // Add T to this
    Set<T>& operator -= (T* Ptr_); // this minus the T
    
    Boolean operator > (const T* Ptr_) const; // True if
	// T is in the set but not the only T
    Boolean operator >= (const T* Ptr_) const; // True if
	// T is in the set
    Boolean operator == (const T* Ptr_) const; // True if
	// T is the only T in the set

    SearchableList<T>& GiveSetData();
    const SearchableList<T>& GiveSetData() const;

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




