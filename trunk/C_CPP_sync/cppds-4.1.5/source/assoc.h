#ifndef ASSOCIATION_H
#define ASSOCIATION_H
#ifndef HASHABLE_CPP
  #include "hashable.cpp"
#endif

#define ASSOCIATION_DEBUG		0

/****************************************************************************/
template <class KEY, class VALUE>
class Association;

template <class KEY, class VALUE>
class AssociationAcceptor : public ObjectAcceptor
{
  friend class Association<KEY, VALUE>;

  protected:
    union
    {
      const Association<KEY, VALUE>* _ObjPtr;
      Association<KEY, VALUE>* _VarPtr;
    };

    AssociationAcceptor(const Association<KEY, VALUE>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Association class specific acceptor methods
    virtual Boolean IsAssociation() const;
    virtual Boolean IsEqvAssociation(int KeyID_, size_t KeySize_,
                                     int ValID_, size_t ValSize_) const;
    virtual void* CreateFromAssociation(const ObjectAcceptor& Obj_);
    virtual void* AssignFromAssociation(const ObjectAcceptor& Obj_);
    virtual Boolean IsEqualToAssociation(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

//****************************************************************************
template <class KEY, class VALUE>
class Association : public HashableClass<KEY, VALUE>
{
  friend class AssociationAcceptor<KEY, VALUE>;
  
  friend Object& RootObject(Association<KEY, VALUE>& Obj_)
      { return ((Object&)Obj_); }
  friend const Object& RootConstObject(const Association<KEY, VALUE>& Obj_)
      { return ((const Object&)Obj_); }

  protected:
    DataStorePtr<VALUE> _Value;
    DataStorageAttributes _ValueStoreAttr;

    Boolean IsEqualToAssociation(const Association<KEY, VALUE>* hb) const;
    Boolean IsEqualValue(const Association<KEY, VALUE>* hb) const;

    virtual const DataStorePtr<VALUE>& GetValueStorePtr() const;

  public:
    Association(int Flags_=DataStorageAttributes::DEEP_COPY);
    Association(const Association<KEY, VALUE>& hb);
    virtual ~Association();

    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Association<KEY, VALUE>& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::ASSOCATION_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;    
    virtual Association<KEY, VALUE>& operator = (const Association<KEY, VALUE>& hb);
    
    virtual const VALUE* GetData() const;
    virtual VALUE* GetData();    
    virtual const VALUE* GetValue() const;
    virtual VALUE* GetValue();

    virtual Association<KEY, VALUE>& SetValue(VALUE* ptr);
    virtual Association<KEY, VALUE>& SetData(VALUE* ptr);    
    virtual Association<KEY, VALUE>& SetPair(KEY* Key_, VALUE* Value_);
    
    virtual void DestroyData();
    virtual void DestroyValue();
    virtual void DestroyPair();
    
    virtual Association<KEY, VALUE>& SetValueStorageAttributes(int Flags_);
    virtual const DataStorageAttributes& GetValueStorageAttributes() const;
            
    virtual Boolean IsAssociation() const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

//****************************************************************************
#endif





