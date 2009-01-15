#ifndef BUILTINHND_H
#define BUILTINHND_H
#ifndef SMARTHND_H
  #include "smarthnd.h"
#endif

/****************************************************************************/
class BuiltInDeleter : public ObjectUpdater
{
  ROOTOBJECTFNCS_DEFN(ObjectUpdater, BuiltInDeleter)

  protected:
    ObjectUpdater* _Next;

    BuiltInDeleter();
    BuiltInDeleter(const BuiltInDeleter& Obj_);

    virtual Boolean DoUpdate(VOIDPTR Obj_, Boolean HaltOnFalse_);
    virtual Boolean DoUpdate(const VOIDPTR Obj_, Boolean HaltOnFalse_) const;

    virtual void SetNextPtr(ObjectUpdater* Ptr_);
    virtual ObjectUpdater* GetNextPtr();

  public:
    virtual ~BuiltInDeleter();

    static BuiltInDeleter* Make()
	{ return (new BuiltInDeleter); }
    static BuiltInDeleter* Make(BuiltInDeleter* Ptr_)
	{ return (Ptr_ ? (new BuiltInDeleter(*Ptr_)):NULL); }

    virtual BuiltInDeleter* Clone() const;
    virtual ObjectUpdater* AddNext(ObjectUpdater* Next_, Boolean* Done_=NULL);
    virtual ObjectUpdater* TransferBasePtr(ObjectUpdater*);
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
#endif





