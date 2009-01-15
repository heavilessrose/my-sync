#ifndef HASHABLE_H
#define HASHABLE_H
#ifndef BYTESTR_H
  #include "bytestr.h"
#endif
#ifndef HASHFUNC_H
  #include "hashfunc.h"
#endif
#ifndef DATASTORE_CPP
  #include "datastore.cpp"
#endif

#define HASHABLE_DEBUG		0

/****************************************************************************/
template <class T>
Ulong HashValue(Ulong(*HashFunc_)(const char*, size_t, Ulong), const T* Ptr_, Ulong TableSize_)
{
  return (Ptr_ ? ((*HashFunc_)(HashableData(*Ptr_), SizeOfData(*Ptr_), TableSize_)):0);
}

/****************************************************************************/
template <class KEY, class VALUE>
class HashableClass;

template <class KEY, class VALUE>
class HashableAcceptor : public ObjectAcceptor
{
  friend class HashableClass<KEY, VALUE>;

  protected:
    union
    {
      const HashableClass<KEY, VALUE>* _ObjPtr;
      HashableClass<KEY, VALUE>* _VarPtr;
    };

    HashableAcceptor(const HashableClass<KEY, VALUE>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // HashableClass class specific acceptor methods
    virtual Boolean IsHashable() const;
    virtual Boolean IsEqvHashable(int ID_, size_t Size_) const;
    virtual void* CreateFromHashable(const ObjectAcceptor& Obj_);
    virtual void* AssignFromHashable(const ObjectAcceptor& Obj_);
    virtual Boolean IsEqualToHashable(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class KEY, class VALUE>
class HashableClass : public ObjectSkeleton
{
  friend class HashableAcceptor<KEY, VALUE>;
  
  friend Object& RootObject(HashableClass<KEY, VALUE>& Obj_)
      { return ((Object&)Obj_); }
  friend const Object& RootConstObject(const HashableClass<KEY, VALUE>& Obj_)
      { return ((const Object&)Obj_); }

  protected:
    typedef Ulong(*HashFunction)(const char*, size_t, Ulong);
    typedef Ulong(*DblHashFunction)(Ulong, Ulong);

    DataStorePtr<KEY> _Data;
    Ulong _HashIndex;
    Ulong _ProbeIncr;    
    HashFunction _HashFn;
    DblHashFunction _DblHashFn;
    HashableClass<KEY, VALUE>* _Next;
    Boolean _Nil;
    Boolean _Visited;
    DataStorageAttributes _KeyStoreAttr;

    ByteString StringKey() const;
    inline void Xnull() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

    inline void SetIndex(Ulong Key_)
        { _HashIndex = Key_; }
    inline void SetProbeIncr(Ulong Incr_)
        { _ProbeIncr = Incr_; }
    inline Ulong DblHashValue(Ulong(*DblHashFunc_)(Ulong, Ulong), Ulong Key_, Ulong TableSize_)
        { return ((*DblHashFunc_)(Key_, TableSize_)); }        

    virtual Ulong MakeProbeIncr(Ulong Tabsize_);
    virtual const DataStorePtr<KEY>& GetKeyStorePtr() const;

  public:
    HashableClass(int Flags_=DataStorageAttributes::DEEP_COPY);
    HashableClass(const HashableClass<KEY, VALUE>& hb);
    virtual ~HashableClass();

    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    // Static null object accessor methods
    static HashableClass<KEY, VALUE>& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Size of contained data
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::HASHABLE_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    
    virtual HashableClass<KEY, VALUE>& operator = (const HashableClass<KEY, VALUE>& hb);
    virtual HashableClass<KEY, VALUE>& SetData(VALUE* ptr);
    virtual HashableClass<KEY, VALUE>& SetKey(KEY* ptr);
    virtual const VALUE* GetData() const;
    virtual VALUE* GetData();
    virtual const KEY* GetKey() const;
    virtual KEY* GetKey();
    virtual void DestroyKey();
    virtual void DestroyData();
    virtual HashableClass<KEY, VALUE>& SetKeyStorageAttributes(int Flags_);
    virtual const DataStorageAttributes& GetKeyStorageAttributes() const;    

    HashableClass<KEY, VALUE>& SetVisited(Boolean Flag_=TRUE);
    HashableClass<KEY, VALUE>& SetNil(Boolean Flag_=TRUE);
    HashableClass<KEY, VALUE>& SetPrimaryHashFunction(HashFunction Fn_);
    HashableClass<KEY, VALUE>& SetDoubleHashFunction(DblHashFunction Fn_);    
    HashableClass<KEY, VALUE>& ProbeAhead(Ulong Tabsize_);
    HashableClass<KEY, VALUE>& SetNext(HashableClass<KEY, VALUE>* hb);

    Boolean IsVisited() const
        { return _Visited; }
    Boolean IsNil() const
        { return _Nil; }
    Boolean IsReady() const
        { return (_Data.Pointee() && !_Nil); }
    Boolean IsEqualToHashable(const HashableClass<KEY, VALUE>* hb) const;
    Boolean CollidesWith(const HashableClass<KEY, VALUE>* hb) const;
    Boolean IsEmptySlot(const HashableClass<KEY, VALUE>* hb) const
        { return (!hb || (hb->IsNil() && GetIndex() == hb->GetIndex())); }
    Ulong GetIndex() const;
    Ulong GetProbeIncr() const;
    const HashableClass<KEY, VALUE>* GetNext() const
        { return _Next; }        
    HashableClass<KEY, VALUE>* GetNext()
        { return _Next; }
    
    virtual Ulong MakeIndex(Ulong Tabsize_);
    virtual Boolean IsHashable() const;

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
class Hashable : public HashableClass<T, T>
{
  friend class HashableAcceptor<T, T>;
  
  friend Object& RootObject(Hashable<T>& Obj_)
      { return ((Object&)Obj_); }
  friend const Object& RootConstObject(const Hashable<T>& Obj_)
      { return ((const Object&)Obj_); }

  public:
    Hashable(int Flags_=DataStorageAttributes::DEEP_COPY);
    Hashable(const Hashable<T>& hb);
    virtual ~Hashable();

    virtual HashableClass<T, T>& SetData(T* ptr);    
    virtual const T* GetData() const;
    virtual T* GetData();
    virtual void DestroyData();
    
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




