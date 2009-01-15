#ifndef SIMPLEARRAY_H
#define SIMPLEARRAY_H
#ifndef BUILTIN_H
  #include "builtin.h"
#endif
#ifndef INDEX_H
  #include "index.h"
#endif

#define SIMPLEARRAY_DEBUG       0

#if SIMPLEARRAY_DEBUG
  #ifndef SORTS_CPP
    #include "sorts.cpp"
  #endif
#endif

/****************************************************************************/
template <class T>
class SimpleArray
{
  friend SimpleArray<Boolean> SeriesArrayCompare(const SimpleArray<T> &ia1, const SimpleArray<T> &ia2);
  friend SimpleArray<T> Apply(const SimpleArray<T> & ia, T (* func)(T));

  friend Boolean operator == (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_);
  friend Boolean operator != (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_);
  friend Boolean operator > (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_);
  friend Boolean operator < (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_);
  friend Boolean operator >= (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_);
  friend Boolean operator <= (const SimpleArray<T>& Obj1_, const SimpleArray<T>& Obj2_);

  public:
    typedef int(*CompFuncType)(const T&, const T&);

  protected:
    static CompFuncType _CompFunc;

    Boolean _Error;
    size_t _Size;
    T* _Buffer;

  public:
    // constructors
    SimpleArray();
    SimpleArray(size_t n);
    SimpleArray(const T* a, size_t n);
    SimpleArray(const SimpleArray<T>& Arr_, size_t start_, size_t end_);
    SimpleArray(const SimpleArray<T>& Arr_);
    
    // concatenate constructor
    SimpleArray(const SimpleArray<T> & a1,
                const SimpleArray<T> & a2);

    virtual ~SimpleArray();

    // item comparison function pointer setting method
    static void SetComparisonFunction(int(*CompFunc_)(const T&, const T&));

    // error set method
    void SetError(Boolean Flag_)
        { _Error = Flag_; }
    
    // assignment operator
    SimpleArray<T>& operator = (const SimpleArray<T>& a);

    // conversion operator
    operator const T* () const;

    // raw C array retrieval methods
    T* Base()
        { return _Buffer; }
    const T* Base() const
        { return _Buffer; }
    
    // get number of elements
    size_t GetCount() const;

    // index operators
    T& operator [] (const Index & i);
    T& operator [] (int i);
    const T& operator [] (const Index & i) const;
    const T& operator [] (int i) const;

    const T Read(const Index & i) const;
    const T Read(int i) const;

    // comparison operators
    SimpleArray<Boolean> SeriesArrayCompare(const SimpleArray<T> &ia);
    Boolean IsEqual(const SimpleArray<T> & ia) const;
    Boolean IsLesser(const SimpleArray<T> & ia) const;
    Boolean IsGreater(const SimpleArray<T> & ia) const;

    // apply function to elements
    SimpleArray<T>& Apply(T (* func)(T));

    // fill functions
    void Fill(T i);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif    
};

template <class T>
class SimpleArrayIter
{
  protected:
    SimpleArray<T>& _Target; // reference to target array
    T* _ElemFirst;     // address of first element
    T* _ElemLast;      // address of last  element
    T* _ElemPtr;       // pointer to element
    
  public:
    // constructors
    SimpleArrayIter(SimpleArray<T>& a);
    SimpleArrayIter(const SimpleArrayIter<T>& aptr);

    // assignment operators
    SimpleArrayIter<T> & operator = (const SimpleArrayIter<T> & aptr);
    SimpleArrayIter<T> & operator = (const Index& i);
    SimpleArrayIter<T> & operator = (size_t i);

    // conversion operator
    T & operator * ();

    // increment and decrement
    SimpleArrayIter<T> & operator ++ ();          // prefix
    SimpleArrayIter<T>   operator ++ (int dummy); // postfix
    SimpleArrayIter<T> & operator -- ();          // prefix
    SimpleArrayIter<T>   operator -- (int dummy); // postfix

    // set to first or last element
    void SetFirst();
    void SetLast();

    // value check functions
    int IsFirst();
    int IsLast();

    // comparison operators
    int operator == (const SimpleArrayIter<T> & aptr);
    int operator != (const SimpleArrayIter<T> & aptr);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif    
};

#if SIMPLEARRAY_DEBUG
class SimpleStr
{
  DEFAULT_ROOTOBJECT_DEFN(SimpleStr)

  friend ostream & operator << (ostream & buffer, SimpleStr& s)
      { buffer <<s.Str(); return buffer; }

  private:
    char* _str;

  public:
    SimpleStr():
      _str(NULL) {}
    SimpleStr(const char* str_):
      _str(new_char_string(str_)) {}
    ~SimpleStr()
      { DeleteArray(_str); }

    SimpleStr& operator = (const char* str_)
      { DeleteArray(_str);
        _str = new_char_string(str_);
        return *this; }

    char* Str()
      { return _str; }
};
#endif
/****************************************************************************/
#endif




