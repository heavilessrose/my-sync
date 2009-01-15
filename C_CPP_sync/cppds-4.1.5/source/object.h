//////////////////////////////////////////////////////////////////////////////
// FILE          : OBJECT.H
// PURPOSE       : Base object class type. This is the base super class
//                 for which all other classes are derived from.
//                 Contains procedures, functions and operations common
//                 to all classes which can instantiate objects.
//
// WRITTEN BY    : Joseph Wong
//
// MODIFIED BY:         LAST MODIFIED:  COMMENTS:
// ------------         --------------  ---------
// Joseph Wong          Dec 21 1996     Original Writing
//			June 4 1998	Change names to match style convention
//
//////////////////////////////////////////////////////////////////////////////
// Notes:
// - Do not try to factor out the I/O stream checking and I/O operations
//   from the main Object class. Although this makes sense in design. This
//   was attempted before and it causes the compiler to fail. Leave it as
//   it is even though this would result in a fatter interface.
// - Do not reimplement the inline functions as friends to the class. This
//   does not work and causes problems with the compiler. Another problem is
//   that all these functions must be specialized for every class derived
//   from Object, so that calls to these functions operates on the appropriate
//   type through the template specialization mechanism. Making these
//   functions friends to Object will not do this since friendship is never
//   inherited.
// - Do not combine overloaded inline functions by making a single inline
//   function which accepts default arguments. Again, although this makes
//   sense in design, it has the practical problem of causing some compilers
//   to fail.
// - The cloning and swap functions must be declared as friends to the object
//   class and inlined in the class declaration for them to work properly
// - The other template specialization functions (starting with "_") must
//   also be declared as friends and MUST BE inlined in the header file for
//   them to work.
// - Because of the new behaviour defined in the ANSI C++ standard:
//   1) No implicit conversion from derived to base type in ctors and
//      assignment operators derived classes
//   2) implicit conversion from derived to base types only performed in
//      derived classes of polymorphic (must have virtual methods) base
//      classes
//   - explicit cast to base types may have to be done in some constructors
//     in derived classes. Or extra constructors may need to be defined
//     which explicitly takes the derived class object as an argument.
//
//     The exception to this rule would be the copy constructor with the
//     signature of X(const X&) which would not need any type casts.
//     A default public copy constructor would be generated if one is not
//     explicitly declared. This constructor would do default memberwise
//     initialization
//     Another exception would be the assignment operator which would
//     automatically convert derived types to base types if called from a
//     derived class method or operator. If no assignment operator is
//     declared one is generated for you which does memberwise initialization.
//     The assignment operator must accept the same argument type as the
//     class that declares it as in -- X& operator = (const X&)
//   - Explicit invocation of the base class may be needed in some situations
//     such as explicitly invoking the ObjectImp ctors. The compiler may
//     fail to determine which version of the ObjectImp constructors to
//     invoke or fail to invoke any of the explicitly defined ones.
//////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_H
#define OBJECT_H
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef STREAM_H
  #include "stream.h"
#endif
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef OBJECTACCEPTOR_H
  #include "objaccpt.h"
#endif

/****************************************************************************/
/****************************************************************************/
class ObjectEnvelope;
class BindingsObjectEnvelope;

struct ObjectData
{
  long _IDvalue;
  Ulong _ObjectID;
  Byte_t _NullStatus;
};

class Object
{
  friend class ObjectEnvelope;
  friend class BindingsObjectEnvelope;

  friend Boolean operator == (const Object& obj1, const Object& obj2);
  friend Boolean operator != (const Object& obj1, const Object& obj2);
  friend Boolean operator > (const Object& obj1, const Object& obj2);
  friend Boolean operator < (const Object& obj1, const Object& obj2);
  friend Boolean operator >= (const Object& obj1, const Object& obj2);
  friend Boolean operator <= (const Object& obj1, const Object& obj2);

  // Internal object data read/write methods
  friend Boolean ReadObjectData(Object& Obj_, istream& Is_, Boolean Ignore_);
  friend Boolean WriteObjectData(const Object& Obj_, ostream& Os_);
  friend Boolean ReadObjectData(Object& Obj_, ByteStream& Is_, Boolean Ignore_);
  friend Boolean WriteObjectData(const Object& Obj_, ByteStream& Os_);

  friend Boolean StoreObjectData(ObjectData& Obj_, istream& Is_);
  friend Boolean StoreObjectData(ObjectData& Obj_, ByteStream& Is_);

  friend ostream& operator << (ostream& os, const Object& obj);
  friend istream& operator >> (istream& is, Object& obj);
  friend ByteStream& operator << (ByteStream& os, const Object& obj);
  friend ByteStream& operator >> (ByteStream& is, Object& obj);

  friend Ulong AssociatedIndex(const Object& Obj_, Ulong TableSize_);

  ROOTOBJECTFNCS_STUB(Object, Object)
  GENERIC_FNC_DEFN(Object)

  public:
    typedef Byte_t IOstate;

    // I/O flag toggle settings
    enum
    {
      IO_VERIFY 		= 0x80,
      IO_INVALID		= 0x40,
      IO_CLEAR			= 0x00,
      IO_STREAMERROR		= 0x01,
      IO_OBJECTIDERROR  	= 0x02,
      IO_OBJECTDATAERROR	= 0x04,
      IO_NOTDONE		= 0x08
    };

    enum
    {
      NEWED_VERIFY		= 0xF0,
      NEWED_FLAG		= 0x0F
    };

  protected:
    enum
    {
      IO_RESET			= 0x10,
    };

    // Used in an envelope class for accessing the enclosed letter object
    virtual Object* Dispatch() = 0;
    virtual const Object* Dispatch() const = 0;

    // Virtual object destruction method
    virtual Boolean Destroy() = 0;

    // Not Implemented message
    void NotImplemented(const char* MethodName_) const
	{ ::NotImplemented(ClassName(), MethodName_); }

    // Stream validation methods
    virtual Boolean IstreamCheck(ByteStream& Is_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const = 0;
    virtual Boolean OstreamCheck(ByteStream& Os_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const = 0;

    virtual Boolean IstreamCheck(istream& Is_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const = 0;
    virtual Boolean OstreamCheck(ostream& Os_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const = 0;

    // Read\Write operation validation methods
    virtual void SetIOstate(Boolean Flag_, Object::IOstate StatBits_, Boolean Finalize_=FALSE) const = 0;
    virtual Boolean ReturnIOValid(ByteStream* Is_, ByteStream* Os_, Boolean* Ok_=NULL) const = 0;
    virtual Boolean ReturnIOValid(istream* Is_, ostream* Os_, Boolean* Ok_=NULL) const       = 0;

    inline Boolean ReturnReadValid(ByteStream& Is_, Boolean* Ok_=NULL) const
	{ return ReturnIOValid(&Is_, NULL, Ok_); }
    inline Boolean ReturnReadValid(istream& Is_, Boolean* Ok_=NULL) const
	{ return ReturnIOValid(&Is_, NULL, Ok_); }
    inline Boolean ReturnWriteValid(ByteStream& Os_, Boolean* Ok_=NULL) const
	{ Os_.oflush(); return ReturnIOValid(NULL, &Os_, Ok_); }
    inline Boolean ReturnWriteValid(ostream& Os_, Boolean* Ok_=NULL) const
	{ Os_.flush(); return ReturnIOValid(NULL, &Os_, Ok_); }

    // Null object methods
    virtual Object& SetAsNullObject(Boolean Flag_=TRUE) = 0;
    virtual const Object& SetAsNullObject(Boolean Flag_=TRUE) const = 0;

    // Object identity methods
    virtual Object& SetObjectID(size_t Id_) = 0;
    virtual const Object& SetObjectID(size_t Id_) const = 0;

  public:
    // virtual destructor
    virtual ~Object() = 0;

    // object property methods
    virtual Boolean IsSummableType() const;
    virtual Boolean IsMultipliableType() const;
    virtual Boolean IsOrdinalType() const;
    virtual Boolean IsInvertableType() const;
    virtual Boolean IsLogicalType() const;
    virtual Boolean IsBitwiseableType() const;
    virtual Boolean IsStreamableType() const;
    virtual Boolean IsSortableType() const;
    virtual Boolean IsMatchableType() const;

    // Used in an envelope class for accessing the enclosed letter object
    virtual Object& EnclosedObject() = 0;
    virtual const Object& EnclosedObject() const = 0;

    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL) = 0;
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL) = 0;
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const = 0;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) = 0;
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const = 0;

    // Null object methods
    virtual Boolean IsNullObject() const = 0;
    virtual const Object& NULLOBJECT() const = 0;
    virtual Object& NULLOBJECT() = 0;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::OBJECT_TYPE; }
    Boolean IsCommonClass(int IDvalue_) const;
    virtual int ClassID() const;
    const char* ClassName() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const = 0;

    // Object Identification method
    virtual size_t ObjectID() const = 0;

    // ----- Saving and restoration in binary form ----
    Boolean CheckClassType(int IDvalue_) const;
    virtual long DataSize() const = 0;
    virtual long StorableFormWidth () const = 0;

    // Hashing methods
    virtual const char* GiveHashableData() const = 0;
    virtual long GiveHashableDataLength() const = 0;

    // I/O Stream methods
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL) = 0;
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL) = 0;
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const = 0;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL) = 0;
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL) = 0;
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const = 0;

    // Text input and output
    virtual istream& TextIgnore(istream& Is_, Boolean* Ok_=NULL) = 0;
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL) = 0;
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const = 0;

    // Read\Write operation validation methods
    virtual Object::IOstate GetIOstate() const = 0;
    void ClearIOstate() const;
    inline Boolean IsIOstateFinal() const
	{ return (GetIOstate() & IO_VERIFY); }
    inline Boolean IsIOError() const
	{ return (GetIOstate() & IO_INVALID); }
    inline Boolean IsStreamError() const
	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_STREAMERROR)); }
    inline Boolean IsObjectIdError() const
	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_OBJECTIDERROR)); }
    inline Boolean IsObjectDataError() const
	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_OBJECTDATAERROR)); }
    inline Boolean IsIONotDone() const
	{ return ((GetIOstate() & IO_INVALID) && (GetIOstate() & IO_NOTDONE)); }

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* = NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* = NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* = NULL) const;

    // Object storage method
    virtual ByteStream& WriteObjectWith(ByteStream& Bstrm_) const = 0;
    virtual ByteStream& ReadObjectWith(ByteStream& Bstrm_) = 0;

    // Methods to check if object(s) is dynamically allocated by searching
    // through the custom memory manager (If option is set)
    virtual Boolean IsNewed() const = 0;
};

/****************************************************************************/
class IOstateString
{
  friend IOstateString GetIOstateString(Object::IOstate Status_);

  private:
    char _Buffer[256];

    IOstateString();
    IOstateString(const char* Str_);
    IOstateString(const IOstateString& Obj_);

    IOstateString& operator += (const char* Str_);

  public:
    operator const char* ();
};

/****************************************************************************/
class IosFormat
{
  protected:
    long _FormatFlags;
    int _Precision;
    int _Width;
    char _FillCh;

  public:
    long SetFormat(long Flags_);
    int SetPrecision(int Prec_);
    int SetWidth(int Width_);
    char SetFill(char FillCh_);

    inline long GetFormat()
	{ return _FormatFlags; }
    inline int GetPrecision()
	{ return _Precision; }
    inline int GetWidth()
	{ return _Width; }
    inline char GetFill()
	{ return _FillCh; }
};

// manipulator storage class
// stores information needed for the manipulator to perform
class IosResetFunctor
{
  public:
    // hidden manipulator function type definition
    typedef ios&(*FncPtr)(ios&);

  protected:
    FncPtr _Fptr;  	    	// hidden manipulator function pointer

  public:
    // manipulator storage class constructor
    IosResetFunctor(FncPtr Fparg_)
	: _Fptr(Fparg_) {}

    // public interface
    FncPtr GetFncPtr()
	{ return _Fptr; }
};

class IosSetFunctor
{
  public:
    typedef ios&(*FncPtr)(ios&, IosFormat&);

  protected:
    FncPtr _Fptr;	// hidden manipulator function pointer
    IosFormat _IosForm;

  public:
    // manipulator storage class constructor
    IosSetFunctor(FncPtr Fparg_, IosFormat& IosForm_)
	: _Fptr(Fparg_),
	  _IosForm(IosForm_) {}

    FncPtr GetFncPtr()
	{ return _Fptr; }

    IosFormat& GetIosFormat()
	{ return _IosForm; }
};

ios& operator << (ios& IosObj_, IosResetFunctor Form_);
ios& operator >> (ios& IosObj_, IosResetFunctor Form_);

ios& operator << (ios& IosObj_, IosSetFunctor Form_);
ios& operator >> (ios& IosObj_, IosSetFunctor Form_);

ios& IosReset_Hidden(ios& IosObj_);
ios& IosSet_Hidden(ios& IosObj_, IosFormat& IosForm_);

IosResetFunctor IosReset();
IosSetFunctor IosSet(IosFormat& IosForm_);

/****************************************************************************/
#define OBJECTIMP_GIVEOBJECTACCEPTOR_TOPDEFN(Method, ClassName)     \
const ObjectAcceptor* Method () const                               \
{ return (new ClassName (this)); }

#define OBJECTIMP_ASSIGN_TOPDEFN(Method, ClassName, FncName)        \
Object& Method (const Object& Obj_, ObjectAcceptor* ObjComp_)       \
{ ClassName TrgComp_(this);                                         \
  if (ObjComp_ == NULL)                                             \
  { ((Object*)&Obj_)->Assign(*this, &TrgComp_);                     \
    return *TrgComp_._VarPtr; }                                     \
  return *((Object*)( FncName (TrgComp_))); }

#define OBJECTIMP_CLONE_TOPDEFN(Method, HostClass, AcceptClass, FncName)    \
Object* Method (ObjectAcceptor* ObjComp_) const                             \
{ if (ObjComp_ == NULL)                                                     \
    return (new HostClass (*this));                                         \
  AcceptClass TrgComp_(this);                                               \
  return ((Object*) ( FncName (TrgComp_))); }

#define OBJECTIMP_GIVENULLOBJECT_TOPDEFN(Method, Host)                      \
LongInt& Method ()                                                          \
{ static Host _NULLOBJECT;                                                  \
  _NULLOBJECT.SetAsNullObject();                                            \
  return _NULLOBJECT; }

#define OBJECTIMP_NULLOBJECT_TOPDEFN(Method, FncName)                      \
const Object& Method () const                                              \
{ return FncName (); }                                                     \
Object& Method ()                                                          \
{ return FncName (); }

#define OBJECTIMP_COMPFNC_TOPDEFN(Method, AcceptClass, FncCall, FncDisp)   \
Boolean Method (const Object& Obj_, const ObjectAcceptor* ObjComp_) const  \
{ AcceptClass TrgComp_(this);                                              \
  if (ObjComp_ == NULL)                                                    \
    return FncCall (Obj_, &TrgComp_);                                      \
  return FncDisp (TrgComp_); }

/****************************************************************************/
// Method Definitions
/****************************************************************************/
#define OBJECTIMP_GIVEOBJECTACCEPTOR_METHODDEFN(Host)               \
OBJECTIMP_GIVEOBJECTACCEPTOR_TOPDEFN(Host##::GiveObjectAcceptor, Host##Acceptor)

#define OBJECTIMP_ASSIGN_METHODDEFN(Host)                           \
OBJECTIMP_ASSIGN_TOPDEFN(Host##::Assign, Host##Acceptor, ObjComp_->AssignFrom##Host)

#define OBJECTIMP_CLONE_METHODDEFN(Host)                            \
OBJECTIMP_CLONE_TOPDEFN(Host##::Clone, Host, Host##Acceptor, ObjComp_->CreateFrom##Host)

#define OBJECTIMP_GIVENULLOBJECT_METHODDEFN(Host)                            \
OBJECTIMP_GIVENULLOBJECT_TOPDEFN(Host##::GiveNullObject, Host)

#define OBJECTIMP_NULLOBJECT_METHODDEFN(Host)                                \
OBJECTIMP_NULLOBJECT_TOPDEFN(Host##::NULLOBJECT, Host##::GiveNullObject)

#define OBJECTIMP_COMPFNC_METHODDEFN(Prefix, Host)		\
OBJACCEPTOR_COMPFNC_TOPDEFN(Host##::##Prefix, Host##Acceptor, Obj_.##Prefix, ObjComp_->##Prefix##To##Host)

/****************************************************************************/
// Template Method Definitions
/****************************************************************************/
#define OBJECTIMPTEMP_GIVEOBJECTACCEPTOR_METHODDEFN(Host)           \
template <class T>                                                  \
OBJECTIMP_GIVEOBJECTACCEPTOR_TOPDEFN(Host##<T>::GiveObjectAcceptor, Host##Acceptor<T>)  

#define OBJECTIMPTEMP_ASSIGN_METHODDEFN(Host)                       \
template <class T>                                                  \
OBJECTIMP_ASSIGN_TOPDEFN(Host##<T>::Assign, Host##Acceptor<T>, ObjComp_->AssignFrom##Host)

#define OBJECTIMPTEMP_CLONE_METHODDEFN(Host)                        \
template <class T>                                                  \
OBJECTIMP_CLONE_TOPDEFN(Host##<T>::Clone, Host##<T>, Host##Acceptor<T>, ObjComp_->CreateFrom##Host)

#define OBJECTIMPTEMP_GIVENULLOBJECT_METHODDEFN(Host)                        \
template <class T>                                                           \
OBJECTIMP_GIVENULLOBJECT_TOPDEFN(Host##<T>::GiveNullObject, Host##<T>)

#define OBJECTIMPTEMP_NULLOBJECT_METHODDEFN(Host)                            \
template <class T>                                                           \
OBJECTIMP_NULLOBJECT_TOPDEFN(Host##<T>::NULLOBJECT, Host##<T>::GiveNullObject)

#define OBJECTIMPTEMP_COMPFNC_METHODDEFN(Prefix, Host)		\
template <class T>                                              \
OBJACCEPTOR_COMPFNC_TOPDEFN(Host##<T>::##Prefix, Host##Acceptor<T>, Obj_.##Prefix, ObjComp_->##Prefix##To##Host)

/****************************************************************************/
// Grouped Methods Definitions
/****************************************************************************/
#define OBJECTIMP_COMMONFUNCS_DEFN(Host)		    \
OBJECTIMP_GIVEOBJECTACCEPTOR_METHODDEFN(Host)               \
OBJECTIMP_ASSIGN_METHODDEFN(Host)                           \
OBJECTIMP_CLONE_METHODDEFN(Host)                            \
OBJECTIMP_GIVENULLOBJECT_METHODDEFN(Host)                   \
OBJECTIMP_NULLOBJECT_METHODDEFN(Host)                       \
OBJECTIMP_COMPFNC_METHODDEFN(IsEqual, Host)                 \
OBJECTIMP_COMPFNC_METHODDEFN(IsLesser, Host)                \
OBJECTIMP_COMPFNC_METHODDEFN(IsGreater, Host)

// Template Definitions
#define OBJECTIMPTEMP_COMMONFUNCS_DEFN(Host)		    \
OBJECTIMPTEMP_GIVEOBJECTACCEPTOR_METHODDEFN(Host)           \
OBJECTIMPTEMP_ASSIGN_METHODDEFN(Host)                       \
OBJECTIMPTEMP_CLONE_METHODDEFN(Host)                        \
OBJECTIMPTEMP_GIVENULLOBJECT_METHODDEFN(Host)               \
OBJECTIMPTEMP_NULLOBJECT_METHODDEFN(Host)                   \
OBJECTIMPTEMP_COMPFNC_METHODDEFN(IsEqual, Host)             \
OBJECTIMPTEMP_COMPFNC_METHODDEFN(IsLesser, Host)            \
OBJECTIMPTEMP_COMPFNC_METHODDEFN(IsGreater, Host)

/****************************************************************************/
#endif




