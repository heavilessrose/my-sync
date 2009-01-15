#ifndef OBJECTENVELOPE_H
#define OBJECTENVELOPE_H
#ifndef OBJECT_H
  #include "object.h"
#endif

/****************************************************************************/
// Envelope class
// General rule: methods that deals with data internal to an object should
//               be dispatched, if the method deals with the entire object
//               rather than just the data stored within it then it should
//               not be dispatched.
//

class ObjectEnvelope : public Object
{
  protected:
    // This method does the actual dispatching work
    // This isn't called directly, but is overwritten in the
    // envelope class. Since the dispatched methods were originally
    // designated as pure abstract, calling this dispatch method through
    // them is flagged as a fatal error because it means they were never
    // reimplemented in the derived classes as a "concrete" implementation.
    virtual Object* Dispatch();
    virtual const Object* Dispatch() const;

    // virtual self-destruction method
    virtual Boolean Destroy();

    // Stream validation methods
    virtual Boolean IstreamCheck(ByteStream& Is_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const
	{ return Dispatch()->IstreamCheck(Is_, Flag_, Finalize_); }
    virtual Boolean OstreamCheck(ByteStream& Os_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const
	{ return Dispatch()->OstreamCheck(Os_, Flag_, Finalize_); }

    virtual Boolean IstreamCheck(istream& Is_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const
	{ return Dispatch()->IstreamCheck(Is_, Flag_, Finalize_); }
    virtual Boolean OstreamCheck(ostream& Os_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const
	{ return Dispatch()->OstreamCheck(Os_, Flag_, Finalize_); }

    // Read\Write operation validation methods
    virtual void SetIOstate(Boolean Flag_, Object::IOstate StatBits_, Boolean Finalize_=FALSE) const
	{ Dispatch()->SetIOstate(Flag_, StatBits_, Finalize_); }
    virtual Boolean ReturnIOValid(ByteStream* Is_, ByteStream* Os_, Boolean* Ok_=NULL) const
	{ return Dispatch()->ReturnIOValid(Is_, Os_, Ok_); }
    virtual Boolean ReturnIOValid(istream* Is_, ostream* Os_, Boolean* Ok_=NULL) const
	{ return Dispatch()->ReturnIOValid(Is_, Os_, Ok_); }

    // Null object methods
    virtual Object& SetAsNullObject(Boolean Flag_=TRUE) = 0;
    virtual const Object& SetAsNullObject(Boolean Flag_=TRUE) const = 0;

    // Object identity methods
    virtual Object& SetObjectID(size_t Id_)
        { return Dispatch()->SetObjectID(Id_); }
    virtual const Object& SetObjectID(size_t Id_) const
        { return Dispatch()->SetObjectID(Id_); }

  public:
    virtual ~ObjectEnvelope() {}

    virtual Object& EnclosedObject()
	{ return *Dispatch(); }
    virtual const Object& EnclosedObject() const
	{ return *Dispatch(); }

    virtual Object* operator -> ()
	{ return Dispatch(); }
    virtual const Object* operator -> () const
	{ return Dispatch(); }

    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL)
	{ return Dispatch()->Assign(Obj_, ObjComp_); }
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL)
	{ return Dispatch()->ShallowAssign(Obj_, ObjComp_); }
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const = 0;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) = 0;
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const = 0;

    // Null object methods
    virtual Boolean IsNullObject() const = 0;
    virtual const Object& NULLOBJECT() const = 0;
    virtual Object& NULLOBJECT() = 0;

    // Class Identification methods
    virtual int ClassID() const
        { return Dispatch()->ClassID(); }
    virtual const ObjectAcceptor* GiveObjectAcceptor() const
	{ return Dispatch()->GiveObjectAcceptor(); }

    // Object Identification method
    virtual size_t ObjectID() const
        { return Dispatch()->ObjectID(); }

    // ----- Saving and restoration in binary form ----
    virtual long DataSize() const
	{ return Dispatch()->DataSize(); }
    virtual long StorableFormWidth () const
	{ return Dispatch()->StorableFormWidth(); }

    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL)
	{ return Dispatch()->BinaryIgnore(Is_, Ok_); }
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL)
	{ return Dispatch()->BinaryRead(Is_, Ok_); }
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const
	{ return Dispatch()->BinaryWrite(Os_, Ok_); }

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL)
	{ return Dispatch()->BinaryIgnore(Is_, Ok_); }
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL)
	{ return Dispatch()->BinaryRead(Is_, Ok_); }
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const
	{ return Dispatch()->BinaryWrite(Os_, Ok_); }

    // Text input and output
    virtual istream& TextIgnore(istream& Is_, Boolean* Ok_=NULL)
	{ return Dispatch()->TextIgnore(Is_, Ok_); }
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL)
	{ return Dispatch()->TextRead(Is_, Ok_); }
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const
	{ return Dispatch()->TextWrite(Os_, Ok_); }

    // Read\Write operation validation methods
    virtual Object::IOstate GetIOstate() const
	{ return Dispatch()->GetIOstate(); }

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_ = NULL) const
	{ return Dispatch()->IsEqual(Obj_, ObjComp_); }
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_ = NULL) const
	{ return Dispatch()->IsLesser(Obj_, ObjComp_); }
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_ = NULL) const
	{ return Dispatch()->IsGreater(Obj_, ObjComp_); }

    // Object storage method
    virtual ByteStream& WriteObjectWith(ByteStream& Bstrm_) const
	{ return Dispatch()->WriteObjectWith(Bstrm_); }
    virtual ByteStream& ReadObjectWith(ByteStream& Bstrm_)
	{ return Dispatch()->ReadObjectWith(Bstrm_); }

    // Methods to check if object(s) is dynamically allocated by searching
    // through the custom memory manager (If option is set)
    virtual Boolean IsNewed() const = 0;        
};

// Methods Not Dispatched
//
// Virtual object destruction methods:
//    virtual Boolean Destroy() = 0;
//
// Not Implemented message
//    void NotImplemented(const char* MethodName_) const
//	{ ::NotImplemented(ClassName(), MethodName_); }
//
// Class Identification methods:
//    static int StaticID()
//	{ return TypeInfo::OBJECT_TYPE; }
//    Boolean IsCommonClass(int IDvalue_) const;
//    const char* ClassName() const;
//
// Saving and restoration in binary form:
//    Boolean CheckClassType(int IDvalue_) const;
//
// Read\Write operation validation methods:
//    void ClearIOstate() const;
//    inline Boolean IsIOstateFinal() const
//	{ return (GetIOstate() & IO_VERIFY); }
//    inline Boolean IsIOError() const
//	{ return (GetIOstate() & IO_INVALID); }
//    inline Boolean IsStreamError() const
//	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_STREAMERROR)); }
//    inline Boolean IsObjectIdError() const
//	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_OBJECTIDERROR)); }
//    inline Boolean IsObjectDataError() const
//	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_OBJECTDATAERROR)); }
//    inline Boolean IsIONotDone() const
//	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_NOTDONE)); }
//
// Methods to check if object(s) is dynamically allocated by searching
// through the custom memory manager (If option is set):
//    virtual Boolean IsNewed() const
//	{ return Dispatch()->IsNewed(); }
//
/****************************************************************************/
#endif





