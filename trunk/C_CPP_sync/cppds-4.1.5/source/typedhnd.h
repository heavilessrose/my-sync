#ifndef TYPEDSMARTHND_H
#define TYPEDSMARTHND_H
#ifndef BUILTINHND_H
  #include "bltinhnd.h"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif

/****************************************************************************/
template <class T>
class TypedObjectDeleter : public ObjectUpdater
{
  friend ObjectUpdater& RootObject(TypedObjectDeleter<T>& Obj_)
	{ return *((ObjectUpdater*)(&Obj_)); }
  friend const ObjectUpdater& RootConstObject(const TypedObjectDeleter<T>& Obj_)
	{ return *((const ObjectUpdater*)(&Obj_)); }

  protected:
    ObjectUpdater* _Next;

    TypedObjectDeleter();
    TypedObjectDeleter(const TypedObjectDeleter<T>& Obj_);

    virtual Boolean DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_);
    virtual Boolean DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const;

    virtual void SetNextPtr(ObjectUpdater* Ptr_);
    virtual ObjectUpdater* GetNextPtr();

  public:
    virtual ~TypedObjectDeleter();

    static TypedObjectDeleter<T>* Make()
	{ return (new TypedObjectDeleter<T>); }
    static TypedObjectDeleter<T>* Make(TypedObjectDeleter<T>* Ptr_)
	{ return (Ptr_ ? (new TypedObjectDeleter<T>(*Ptr_)):NULL); }

    virtual TypedObjectDeleter<T>* Clone() const;
    virtual ObjectUpdater* AddNext(ObjectUpdater* Next_, Boolean* Done_=NULL);
    virtual ObjectUpdater* TransferBasePtr(ObjectUpdater* Ptr_);
    virtual Boolean TypedErase(T* Obj_) const;

    virtual Boolean IsUpdater();
    virtual Boolean IsDeleter();

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
template <class CLIENT, class T>
class TypedObjectUpdater : public ObjectUpdater
{
  friend ObjectUpdater& RootObject(TypedObjectUpdater<CLIENT,T>& Obj_)
	{ return *((ObjectUpdater*)(&Obj_)); }
  friend const ObjectUpdater& RootConstObject(const TypedObjectUpdater<CLIENT,T>& Obj_)
	{ return *((const ObjectUpdater*)(&Obj_)); }

  protected:  
    CLIENT* _BasePtr;
    BindingsUnion<CLIENT, T, Boolean> _Bindings;

    int _Type;
    long _NotificationCode;
    ObjectUpdater* _Next;    

    TypedObjectUpdater();
    TypedObjectUpdater(TypedObjectUpdater<CLIENT, T>* Ptr_);
    TypedObjectUpdater(const TypedObjectUpdater<CLIENT, T>& Obj_);
    TypedObjectUpdater(CLIENT* Base_, TYPENAME ClassBinding0<CLIENT>::Type Method_);
    TypedObjectUpdater(CLIENT* Base_, TYPENAME ClassBinding1<CLIENT, long>::Type Method_, long IdCode_=0);
    TypedObjectUpdater(CLIENT* Base_, TYPENAME ClassBinding2<CLIENT, T>::Type Method_, long IdCode_=0);

    virtual Boolean DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_);
    virtual Boolean DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const;

    virtual void SetNextPtr(ObjectUpdater* Ptr_);
    virtual ObjectUpdater* GetNextPtr();

  public:
    virtual ~TypedObjectUpdater();

    static TypedObjectUpdater<CLIENT, T>* Make()
        { return (new TypedObjectUpdater<CLIENT, T>); }
    static TypedObjectUpdater<CLIENT, T>* Make(TypedObjectUpdater<CLIENT, T>* Ptr_)
	{ return (Ptr_ ? (new TypedObjectUpdater<CLIENT, T>(*Ptr_)):NULL); }        
    static TypedObjectUpdater<CLIENT, T>* Make(CLIENT* Base_, TYPENAME ClassBinding0<CLIENT>::Type Method_)
	{ return (new TypedObjectUpdater<CLIENT, T>(Base_, Method_)); }                
    static TypedObjectUpdater<CLIENT, T>* Make(CLIENT* Base_, TYPENAME ClassBinding1<CLIENT, long>::Type Method_, long IdCode_=0)
	{ return (new TypedObjectUpdater<CLIENT, T>(Base_, Method_, IdCode_)); }        
    static TypedObjectUpdater<CLIENT, T>* Make(CLIENT* Base_, TYPENAME ClassBinding2<CLIENT, T>::Type Method_, long IdCode_=0)
	{ return (new TypedObjectUpdater<CLIENT, T>(Base_, Method_, IdCode_)); }        

    virtual TypedObjectUpdater<CLIENT, T>* Clone() const;
    virtual ObjectUpdater* AddNext(ObjectUpdater* Next_, Boolean* Done_=NULL);
    virtual ObjectUpdater* TransferBasePtr(ObjectUpdater* Ptr_);
    virtual Boolean TypedUpdate(const T* Obj_) const;

    virtual Boolean IsUpdater();
    virtual Boolean IsDeleter();    

    virtual void SetBinding(TYPENAME ClassBinding0<CLIENT>::Type Method_);
    virtual void SetBinding(TYPENAME ClassBinding1<CLIENT, long>::Type Method_, long IdCode_=0);
    virtual void SetBinding(TYPENAME ClassBinding2<CLIENT, T>::Type Method_, long IdCode_=0);
    
    virtual void SetBasePtr(CLIENT* Base_);
    virtual CLIENT* GetBasePtr();
    virtual const CLIENT* GetBasePtr() const;

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
template <class CLIENT, class T>
class BuiltInUpdater : public ObjectUpdater
{
  friend ObjectUpdater& RootObject(BuiltInUpdater<CLIENT, T>& Obj_)
	{ return *((ObjectUpdater*)(&Obj_)); }
  friend const ObjectUpdater& RootConstObject(const BuiltInUpdater<CLIENT, T>& Obj_)
	{ return *((const ObjectUpdater*)(&Obj_)); }

  protected:
    CLIENT* _BasePtr;
    ObjectUpdater* _Next;
  
    BuiltInUpdater();
    BuiltInUpdater(CLIENT* Base_);    
    BuiltInUpdater(BuiltInUpdater<CLIENT, T>* Ptr_);
    BuiltInUpdater(const BuiltInUpdater<CLIENT, T>& Obj_);

    virtual Boolean DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_);
    virtual Boolean DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const;

    virtual void SetNextPtr(ObjectUpdater* Ptr_);
    virtual ObjectUpdater* GetNextPtr();

  public:
    virtual ~BuiltInUpdater();

    static BuiltInUpdater<CLIENT, T>* Make()
	{ return (new BuiltInUpdater<CLIENT, T>); }
    static BuiltInUpdater<CLIENT, T>* Make(CLIENT* Base_)
	{ return (new BuiltInUpdater<CLIENT, T>(Base_)); }
    static BuiltInUpdater<CLIENT, T>* Make(BuiltInUpdater<CLIENT, T>* Ptr_)
	{ return (Ptr_ ? (new BuiltInUpdater<CLIENT, T>(*Ptr_)):NULL); }

    virtual BuiltInUpdater<CLIENT, T>* Clone() const;
    virtual ObjectUpdater* AddNext(ObjectUpdater* Next_, Boolean* Done_=NULL);
    virtual ObjectUpdater* TransferBasePtr(ObjectUpdater* Ptr_);
    virtual Boolean TypedUpdate(const T* Obj_) const;

    virtual Boolean IsUpdater();
    virtual Boolean IsDeleter();    

    virtual void SetBasePtr(CLIENT* Base_);
    virtual CLIENT* GetBasePtr();
    virtual const CLIENT* GetBasePtr() const;

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





