#ifndef BINDINGSOBJECTENVELOPE_H
#define BINDINGSOBJECTENVELOPE_H
#ifndef OBJECTENVELOPE_H
  #include "objenvelope.h"
#endif
#ifndef SMARTHND_H
  #include "smarthnd.h"
#endif

/****************************************************************************/
// Envelope class
// General rule: methods that deals with data internal to an object should
//               be dispatched, if the method deals with the entire object
//               rather than just the data stored within it then it should
//               not be dispatched.
//

class BindingsObjectEnvelope : public ObjectEnvelope
{
  protected:
//  sample template data member:
//  UpdatePtr<T> _Bindings;

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

  public:
    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);

    // Object storage method
    virtual ByteStream& ReadObjectWith(ByteStream& Bstrm_);

    // Bindings methods
    virtual Boolean Update() const = 0;
    virtual Boolean Validate() const = 0;

    virtual UpdaterBindings* GetUpdater() const = 0;
    virtual UpdaterBindings* GetValidator() const = 0;

    virtual BindingsObjectEnvelope& CopyBindings(BindingsObjectEnvelope& rhs) = 0;
};

// Methods Not Dispatched
//
// Virtual object destruction methods:
//    virtual Boolean Destroy() = 0;
//    virtual Boolean Destroy() const = 0;
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





