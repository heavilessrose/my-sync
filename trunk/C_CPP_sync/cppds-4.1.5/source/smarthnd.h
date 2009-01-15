#ifndef SMARTHND_H
#define SMARTHND_H
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif
#ifndef GENDECLS_H
  #include "gendecls.h"
#endif
#ifndef BINDINGS_H
  #include "bindings.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif
#ifndef REFCOUNT_H
  #include "refcount.h"
#endif

/****************************************************************************/
// Object updater class interface. Uses object-method bindings feature of C++
// to update class objects. For further details refer to page 24.
// Building portable C++ applications with YACL.
//
class ObjectUpdater
{
  friend class UpdaterBindingList;
  friend class DeleterBindingList;

  ROOTOBJECTFNCS_STUB(ObjectUpdater, ObjectUpdater)

  friend int WhatIs(const ObjectUpdater& Obj_)
        { return Obj_.ClassID(); }

  friend Boolean operator == (const ObjectUpdater& r1, const ObjectUpdater& r2)
        { return r1.IsEqual(r2); }
  friend Boolean operator != (const ObjectUpdater& r1, const ObjectUpdater& r2)  
        { return !r1.IsEqual(r2); }

  friend Boolean _IsEqualTo(const ObjectUpdater& Trg_, const ObjectUpdater& Src_)
        { return Trg_.IsEqual(Src_); }

  friend ObjectUpdater* _Clone(const ObjectUpdater& Obj_)
	{ return ((ObjectUpdater*)Obj_.Clone()); }
  friend const ObjectUpdater* _ConstClone(const ObjectUpdater& Obj_)
	{ return &Obj_; }
  friend ObjectUpdater* _NonConstClone(ObjectUpdater& Obj_)
	{ return &Obj_; }

  friend void SetNext(ObjectUpdater* Trg_, ObjectUpdater* Ptr_)
        { if (!Ptr_ || Trg_ &&
              (Trg_->IsUpdater() && Ptr_->IsUpdater() ||
               Trg_->IsDeleter() && Ptr_->IsDeleter()))
            Trg_->SetNextPtr(Ptr_); }
  friend ObjectUpdater* GetNext(ObjectUpdater* Src_)
        { return (Src_ ? Src_->GetNextPtr():NULL); }

  friend Boolean _IsMatchable(const ObjectUpdater& Obj_)
	{ return TRUE; }
            
  protected:
    virtual void SetNextPtr(ObjectUpdater* Ptr_) = 0;
    virtual ObjectUpdater* GetNextPtr() = 0;

    virtual Boolean Destroy();

  public:
    virtual ~ObjectUpdater() {}
    virtual Boolean DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_) = 0;
    virtual Boolean DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const = 0;

    static int StaticID()
        { return TypeInfo::OBJECTUPDATER_TYPE; }
    virtual int ClassID() const;

    virtual ObjectUpdater* Clone() const = 0;
    virtual ObjectUpdater* AddNext(ObjectUpdater* Next_, Boolean* Done_=NULL) = 0;
    virtual ObjectUpdater* TransferBasePtr(ObjectUpdater*) = 0;

    virtual Boolean IsUpdater() = 0;
    virtual Boolean IsDeleter() = 0;

    virtual Boolean IsEqual(const ObjectUpdater& Obj_) const;
};

/****************************************************************************/
class UpdaterBindings : public ObjectUpdater
{
  friend void SetNext(ObjectUpdater* Trg_, ObjectUpdater* Ptr_);
  friend ObjectUpdater* GetNext(ObjectUpdater* Src_);

  protected:
    virtual void SetToNull() = 0;
    virtual const ReferenceCount& GiveRefCount() const = 0;

  public:
    virtual ~UpdaterBindings() {}

    // UpdaterBindings methods
    virtual Boolean Unique() const = 0;
    virtual Boolean IsNull() const = 0;
    virtual Boolean Update(VOIDPTR Obj_) = 0;
    virtual Boolean Update(const VOIDPTR Obj_) const = 0;
    virtual UpdaterBindings* Remove(ObjectUpdater* Ptr_, Boolean* Done_=NULL) = 0;
    virtual Boolean Includes(ObjectUpdater* Ptr_) = 0;
    virtual UpdaterBindings* HaltOnFirstFalse(Boolean Flag_=TRUE) = 0;
};

/****************************************************************************/
class DeleterBindings : public UpdaterBindings
{
  friend void SetNext(ObjectUpdater* Trg_, ObjectUpdater* Ptr_);
  friend ObjectUpdater* GetNext(ObjectUpdater* Src_);

  public:
    virtual ~DeleterBindings() {}
    
    // DeleterBindings methods
    virtual Boolean Erase(VOIDPTR Obj_) = 0;
    virtual Boolean Erase(const VOIDPTR Obj_) const = 0;
};

/****************************************************************************/
class UpdaterBindingList : public UpdaterBindings
{
  ROOTOBJECTFNCS_DEFN(ObjectUpdater, UpdaterBindingList)

  protected:
    ObjectUpdater* _Head;
    Boolean _HaltOnFalse;
    ReferenceCount refCount; // Number of pointers to heap object    

    // class constructors
    UpdaterBindingList();
    UpdaterBindingList(ObjectUpdater* Ptr_);
    UpdaterBindingList(const UpdaterBindingList& Obj_);

    // Helper methods
    Boolean HelpIncludes(ObjectUpdater* SrchPtr_, ObjectUpdater* Ptr_);    
    Boolean HelpRemove(ObjectUpdater* Prev_,
                       ObjectUpdater* Next_,
                       ObjectUpdater* Ptr_);

    // Updater binding methods
    virtual void SetToNull();
    virtual const ReferenceCount& GiveRefCount() const;

    // ObjectUpdater methods
    virtual Boolean DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_)
        { return _Head->DoUpdate(Obj_, HaltOnFalse_); }
    virtual Boolean DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const
        { return _Head->DoUpdate(Obj_, HaltOnFalse_); }

    virtual void SetNextPtr(ObjectUpdater* Ptr_)
        { _Head->SetNextPtr(Ptr_); }
    virtual ObjectUpdater* GetNextPtr()
        { return _Head->GetNextPtr(); }    

  public:
    virtual ~UpdaterBindingList();

    // Object construction methods
    static UpdaterBindingList* Make()
        { return (new UpdaterBindingList()); }
    static UpdaterBindingList* Make(ObjectUpdater* Ptr_)
        { return (new UpdaterBindingList(Ptr_)); }
    static UpdaterBindingList* Make(const UpdaterBindingList& Obj_)
        { return (new UpdaterBindingList(Obj_)); }

    // Assignment operators
    UpdaterBindingList& operator = (ObjectUpdater* Ptr_);
    UpdaterBindingList& operator = (const UpdaterBindingList& Obj_);

    // ObjectUpdater methods
    virtual UpdaterBindingList* Clone() const
        { return Make(*this); }        
    virtual UpdaterBindingList* AddNext(ObjectUpdater* Next_, Boolean* Done_=NULL);
    virtual UpdaterBindingList* TransferBasePtr(ObjectUpdater* Ptr_);
    virtual Boolean IsUpdater()
        { return TRUE; }
    virtual Boolean IsDeleter()
        { return FALSE; }
        
    // UpdaterBinding methods
    virtual Boolean Unique() const;
    virtual Boolean IsNull() const
        { return !_Head; }
    virtual Boolean Update(VOIDPTR Obj_)
        { return (_Head ? DoUpdate(Obj_, _HaltOnFalse):FALSE); }
    virtual Boolean Update(const VOIDPTR Obj_) const
        { return (_Head ? DoUpdate(Obj_, _HaltOnFalse):FALSE); }
    virtual UpdaterBindingList* Remove(ObjectUpdater* Ptr_, Boolean* Done_=NULL);
    virtual Boolean Includes(ObjectUpdater* Ptr_);
    virtual UpdaterBindingList* HaltOnFirstFalse(Boolean Flag_=TRUE);

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
class DeleterBindingList : public DeleterBindings
{
  ROOTOBJECTFNCS_DEFN(ObjectUpdater, DeleterBindingList)

  protected:
    ObjectUpdater* _Head;
    Boolean _HaltOnFalse;
    ReferenceCount refCount; // Number of pointers to heap object

    // class constructors
    DeleterBindingList();
    DeleterBindingList(ObjectUpdater* Ptr_);
    DeleterBindingList(const DeleterBindingList& Obj_);

    // Helper methods
    Boolean HelpIncludes(ObjectUpdater* SrchPtr_, ObjectUpdater* Ptr_);    
    Boolean HelpRemove(ObjectUpdater* Prev_,
                       ObjectUpdater* Next_,
                       ObjectUpdater* Ptr_);

    // Updater binding methods
    virtual void SetToNull();
    virtual const ReferenceCount& GiveRefCount() const;

    // ObjectUpdater methods
    virtual Boolean DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_)
        { return _Head->DoUpdate(Obj_, HaltOnFalse_); }
    virtual Boolean DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const
        { return _Head->DoUpdate(Obj_, HaltOnFalse_); }

    virtual void SetNextPtr(ObjectUpdater* Ptr_)
        { _Head->SetNextPtr(Ptr_); }
    virtual ObjectUpdater* GetNextPtr()
        { return _Head->GetNextPtr(); }        

  public:
    virtual ~DeleterBindingList();

    // Object construction methods
    static DeleterBindingList* Make()
        { return (new DeleterBindingList()); }
    static DeleterBindingList* Make(ObjectUpdater* Ptr_)
        { return (new DeleterBindingList(Ptr_)); }
    static DeleterBindingList* Make(const DeleterBindingList& Obj_)
        { return (new DeleterBindingList(Obj_)); }

    // Assignment operators
    DeleterBindingList& operator = (ObjectUpdater* Ptr_);
    DeleterBindingList& operator = (const DeleterBindingList& Obj_);

    // ObjectUpdater methods
    virtual DeleterBindingList* Clone() const
        { return Make(*this); }
    virtual DeleterBindingList* AddNext(ObjectUpdater* Next_, Boolean* Done_=NULL);
    virtual DeleterBindingList* TransferBasePtr(ObjectUpdater* Ptr_);
    virtual Boolean IsUpdater()
        { return TRUE; }
    virtual Boolean IsDeleter()
        { return TRUE; }
    
    // UpdaterBinding methods
    virtual Boolean Unique() const;    
    virtual Boolean IsNull() const
        { return !_Head; }
    virtual Boolean Update(VOIDPTR Obj_)
        { return (_Head ? DoUpdate(Obj_, _HaltOnFalse):FALSE); }
    virtual Boolean Update(const VOIDPTR Obj_) const
        { return (_Head ? DoUpdate(Obj_, _HaltOnFalse):FALSE); }
    virtual DeleterBindingList* Remove(ObjectUpdater* Ptr_, Boolean* Done_=NULL);
    virtual Boolean Includes(ObjectUpdater* Ptr_);    
    virtual DeleterBindingList* HaltOnFirstFalse(Boolean Flag_=TRUE);

    // DeleterBinding methods
    virtual Boolean Erase(VOIDPTR Obj_)
	{ return (_Head ? DoUpdate(Obj_, _HaltOnFalse):FALSE); }
    virtual Boolean Erase(const VOIDPTR Obj_) const
	{ return (_Head ? DoUpdate(Obj_, _HaltOnFalse):FALSE); }

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





