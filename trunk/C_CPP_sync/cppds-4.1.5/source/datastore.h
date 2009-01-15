#ifndef DATASTORE_H
#define DATASTORE_H
#ifndef DATASTOREATTRIB_H
  #include "datastoreattrib.h"
#endif
#ifndef STRUTIL_H
  #include "strutil.h"
#endif
#ifndef SMARTPTR_CPP
  #include "smartptr.cpp"
#endif

#define DATASTOREPTR_DEBUG                      0
#define DATASTOREPTR_SHOW_DEBUG_OUTPUT          0
#define DATASTOREPTR_NOTIFY_SUCCESS             0

/****************************************************************************/
template <class T>
class ArrayStorePtr;

template <class T>
class DataStorePtr : private DataStorageAttributes
{
  protected:
    int _PtrNum;

    union
    {
      T* _RawPtr;
      const T* _ConstRawPtr;
      CopiedBuiltInPtr<T>* _CpPrimPtr;
      CopiedObjPtr<T>* _CpObjPtr;
      CountedBuiltInPtr<T>* _CntPrimPtr;
      CountedObjPtr<T>* _CntObjPtr;
    };

    void SetTypeFlags(int& Act_, int& Deep_, int& Dest_);
    void SetupPointer(const DataStorePtr<T>& Obj_);
    void SetupNewedPointer(T* Ptr_);
    void SetupPointer(T* Ptr_);
  
  public:
    DataStorePtr();
    DataStorePtr(const T& Obj_);
    DataStorePtr(T* Ptr_, int Flags_);
    DataStorePtr(const DataStorePtr<T>& Obj_);
    DataStorePtr(const ArrayStorePtr<T>& Obj_);
    virtual ~DataStorePtr();

    static DataStorePtr<T>* Make();
    static DataStorePtr<T>* Make(const T& Obj_);
    static DataStorePtr<T>* Make(T* Ptr_, int Flags_);
    static DataStorePtr<T>* Make(const DataStorePtr<T>& Obj_);
    static DataStorePtr<T>* Make(const ArrayStorePtr<T>& Obj_);
    
    virtual DataStorePtr<T>* Clone();
    virtual void Destroy();

    static int StaticID()
	{ return TypeInfo::DATASTOREPTR_TYPE; }
    virtual int ClassID() const;                        

    virtual DataStorePtr<T>& operator = (const DataStorePtr<T>& rhs);
    virtual DataStorePtr<T>& operator = (const ArrayStorePtr<T>& rhs);
    virtual DataStorePtr<T>& AssignPtr(T* Ptr_, int Flags_);
    virtual DataStorePtr<T>& AssignObject(const T& Obj_);

    virtual Boolean operator ! () const;
    virtual Boolean IsNull() const;

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual T* Pointee();
    virtual const T* Pointee() const;
    virtual const T* ConstPointee() const;

    virtual T* ReleaseControl();

    Boolean IsActive() const;
    Boolean ShouldDeepCopy() const;
    Boolean ShouldDestroy() const;
    int GetStorageAttributes() const;    

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
template <class T>
class ArrayStorePtr : private DataStorageAttributes
{
  protected:
    size_t _Length;
    int _PtrNum;

    union
    {
      T* _RawPtr;
      const T* _ConstRawPtr;
      CopiedBuiltInPtr<T>* _CpPrimPtr;
      CopiedObjPtr<T>* _CpObjPtr;
      CountedBuiltInPtr<T>* _CntPrimPtr;
      CountedObjPtr<T>* _CntObjPtr;
    };

    void SetTypeFlags(int& Act_, int& Deep_, int& Dest_);
    void SetupPointer(const ArrayStorePtr<T>& Obj_);
    void SetupNewedPointer(T* Ptr_);
    void SetupPointer(T* Ptr_, size_t Len_);
  
  public:
    ArrayStorePtr();
    ArrayStorePtr(const T* Ptr_, size_t Len_);
    ArrayStorePtr(T* Ptr_, size_t Len_, int Flags_);
    ArrayStorePtr(const DataStorePtr<T>& Obj_);
    ArrayStorePtr(const ArrayStorePtr<T>& Obj_);
    virtual ~ArrayStorePtr();

    static ArrayStorePtr<T>* Make();
    static ArrayStorePtr<T>* Make(const T* Ptr_, size_t Len_);
    static ArrayStorePtr<T>* Make(T* Ptr_, size_t Len_, int Flags_);
    static ArrayStorePtr<T>* Make(const DataStorePtr<T>& Obj_);
    static ArrayStorePtr<T>* Make(const ArrayStorePtr<T>& Obj_);
    
    virtual ArrayStorePtr<T>* Clone();
    virtual void Destroy();

    static int StaticID()
	{ return TypeInfo::ARRAYSTOREPTR_TYPE; }        
    virtual int ClassID() const;

    size_t Length() const
        { return _Length; }

    virtual T& operator [] (ptrdiff_t Index_);
    virtual const T& operator [] (ptrdiff_t Index_) const;

    virtual T* operator () (ptrdiff_t Index_);
    virtual const T* operator () (ptrdiff_t Index_) const;

    virtual ArrayStorePtr<T>& operator = (const DataStorePtr<T>& rhs);
    virtual ArrayStorePtr<T>& operator = (const ArrayStorePtr<T>& rhs);
    virtual ArrayStorePtr<T>& AssignPtr(T* Ptr_, size_t Len_, int Flags_);
    virtual ArrayStorePtr<T>& AssignObject(const T* Ptr_, size_t Len_);

    virtual Boolean operator ! () const;
    virtual Boolean IsNull() const;

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual T* Pointee();
    virtual const T* Pointee() const;
    virtual const T* ConstPointee() const;

    virtual T* ReleaseControl();

    Boolean IsActive() const;
    Boolean ShouldDeepCopy() const;
    Boolean ShouldDestroy() const;
    int GetStorageAttributes() const;    

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
#if DATASTOREPTR_DEBUG
class Sample
{
  DEFAULT_EXTERNALCLASS_DEFN(Sample)

  friend ostream& operator << (ostream& os, const Sample& obj)
    { os <<"x=" <<obj.Getx() <<", y=" <<obj.Gety(); return os; }

  private:
    char* _own;
    int _x;
    int _y;

  public:
    Sample(int x, int y):
      _own(new_char_string("(NONE)")),
      _x(x), _y(y) {}
    Sample(const Sample& Obj_):
      _own(new_char_string(Obj_._own)),
      _x(Obj_._x), _y(Obj_._y) {}
    ~Sample()
    { cout <<"Sample class destructor called, owned by: " <<_own <<endl;
      ::Delete(_own); }
      
    int Getx() const { return _x; }
    int Gety() const { return _y; }
    void Setxy(int x, int y) { _x = x; _y = y; }
    void SetOwner(const char* Str_)
    { ::Delete(_own);
      _own = new_char_string(Str_); }

    Sample& operator = (const Sample& Obj_)
    { if (this != &Obj_) {
        _x = Obj_._x;
        _y = Obj_._y;
        ::Delete(_own);
        _own = new_char_string(Obj_._own);
      }
      return *this; }

    MEMORYOPS_DEFN(Sample)
};

template <class T>
class SimpleContainer
{
  DEFAULT_EXTERNALCLASS_DEFN(SimpleContainer)

  private:
    char* _name;
    DataStorePtr<T> _dsptr;

    static char* GetCount();

  public:
    SimpleContainer();
    SimpleContainer(T* Data_, int Flags_);
    SimpleContainer(const SimpleContainer<T>& cont_);
    ~SimpleContainer();

    SimpleContainer<T>& operator = (const SimpleContainer<T>& cont_);

    void SetData(T* Data_, int Flags_);
    DataStorePtr<T>& GetData();
    const DataStorePtr<T>& GetData() const;

    MEMORYOPS_DEFN(SimpleContainer)
};
#endif
/****************************************************************************/
#endif




