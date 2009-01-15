//////////////////////////////////////////////////////////////////////////////
// FILE          : OBJECTIMP.H
// PURPOSE       : Base object implementation class type
//
// WRITTEN BY    : Joseph Wong
//
// MODIFIED BY:         LAST MODIFIED:  COMMENTS:
// ------------         --------------  ---------
// Joseph Wong          Jan 6 1999     	Original Writing
//			April 8 2000	Moved DataSize and StorableFormWidth
//					methods implementation to ObjectImp
//					class
//					Made IsNewed method a virtual method
//					of the root Object class
//
//////////////////////////////////////////////////////////////////////////////
// - Because of the new behaviour defined in the ANSI C++ standard:
//   1) No implicit conversion from derived to base type in ctors and
//      assignment operators of derived classes
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
//     invoke or fail to invoke any of the explicitly defined ones
//     because of ambiguity between different ctors.
//////////////////////////////////////////////////////////////////////////////
#ifndef OBJECTIMP_H
#define OBJECTIMP_H
#ifndef OBJECTCOUNTER_H
  #include "objcount.h"
#endif
#ifndef OBJECT_H
  #include "object.h"
#endif
#ifndef GENFNCS2_H
  #include "genfncs2.h"
#endif

#define OBJIMP_DEBUG    0
#define OBJIMP_DEBUG2	0	// Debug ObjectImp class
#define OBJIMP_DEBUG3	0	// Debug template functions

#define OBJIMP_SHOW_DEBUG_OUTPUT	0

#define OBJECTIMP_TRACE	0
#if OBJECTIMP_TRACE
#  define OBJXMARKER(x, strm)	XMARKER(x, strm)
#else
#  define OBJXMARKER(x, strm)
#endif

#if OBJIMP_DEBUG3
struct SomeDumStruct
{
  DEFAULT_ROOTOBJECT_DEFN(SomeDumStruct)
};
#endif

class NullObject;

/****************************************************************************/
class NullObjectAcceptor : public ObjectAcceptor
{
  friend class NullObject;

  protected:
    union
    {
      const NullObject* _ObjPtr;
      NullObject* _VarPtr;
    };

    NullObjectAcceptor(const NullObject* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // NullObject class specific acceptor methods
    virtual Boolean IsNullObject() const;
    virtual void* CreateFromNullObject(const ObjectAcceptor&);
    virtual void* AssignFromNullObject(const ObjectAcceptor&);
    virtual Boolean IsEqualToNullObject(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class ObjectImp : public Object
{
  friend class ObjectIOformatter;

  friend Object& RootObject(ObjectImp& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const ObjectImp& Obj_)
	{ return *((const Object*)(&Obj_)); }

  private:
    ObjectCounter _Counter;
    size_t _ObjectID;
    Byte_t _IsNull;
    Byte_t _IsNewed;
    Object::IOstate _IOflag;

  protected:
    // I/O flag set/unset methods
    inline void SetValidIO()
	{ _IOflag &= ~IO_INVALID; }
    inline void SetInvalidIO()
	{ _IOflag |= IO_INVALID; }
    inline void UnsetIOVerify()
	{ _IOflag &= ~IO_VERIFY; }
    inline void SetIOVerify()
	{ _IOflag |= IO_VERIFY; }

    // Newed flag set/verify methods
    inline Byte_t NewedVerified() const
	{ return (NEWED_VERIFY & _IsNewed); }
    inline Byte_t NewedFlag() const
	{ return (NEWED_FLAG & _IsNewed); }
    inline void SetNewedVerify() const
	{ ((ObjectImp *const)this)->_IsNewed |= NEWED_VERIFY; }
    inline void SetNewedFlag() const
	{ ((ObjectImp *const)this)->_IsNewed |= NEWED_FLAG; }

    // Used in an envelope class for accessing the enclosed letter object
    virtual Object* Dispatch();
    virtual const Object* Dispatch() const;

    // Virtual object destruction method
    virtual Boolean Destroy();

    // Stream validation methods
    virtual Boolean IstreamCheck(ByteStream& Is_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const;
    virtual Boolean OstreamCheck(ByteStream& Os_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const;

    virtual Boolean IstreamCheck(istream& Is_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const;
    virtual Boolean OstreamCheck(ostream& Os_, Boolean* Flag_=NULL, Boolean Finalize_=FALSE) const;

    // Read\Write operation validation methods
    virtual void SetIOstate(Boolean Flag_, Object::IOstate StatBits_, Boolean Finalize_=FALSE) const;
    virtual Boolean ReturnIOValid(ByteStream* Is_, ByteStream* Os_, Boolean* Ok_=NULL) const;
    virtual Boolean ReturnIOValid(istream* Is_, ostream* Os_, Boolean* Ok_=NULL) const;

    // Null object methods
    virtual Object& SetAsNullObject(Boolean Flag_=TRUE);
    virtual const Object& SetAsNullObject(Boolean Flag_=TRUE) const;

    // Object identity methods
    virtual Object& SetObjectID(size_t Id_);
    virtual const Object& SetObjectID(size_t Id_) const;

    // Object implementation default constructor
    // No objects of this class are allowed to be constructed
    ObjectImp();

  public:
    // Class Constructors
    ObjectImp(const Object& Obj_);
    #if !defined(__BORLANDC__) & !defined(__TURBOC__)
      ObjectImp(const ObjectImp& Obj_);
    #endif

    virtual ~ObjectImp() {}

    virtual Object& EnclosedObject();
    virtual const Object& EnclosedObject() const;

    // Assignment method
    ObjectImp& operator = (const Object& Obj_);
    #if !defined(__BORLANDC__) & !defined(__TURBOC__)
      ObjectImp& operator = (const ObjectImp& Obj_);
    #endif

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::OBJECTIMP_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL);
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static NullObject& GiveNullObject();

    // Null object accessors
    virtual Boolean IsNullObject() const;
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Object Identification method
    virtual size_t ObjectID() const;

    // ----- Saving and restoration in binary form ----
    virtual long DataSize() const;
    virtual long StorableFormWidth () const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;

    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Read\Write operation validation methods
    virtual Object::IOstate GetIOstate() const;

    // Object storage method
    virtual ByteStream& WriteObjectWith(ByteStream& Bstrm_) const;
    virtual ByteStream& ReadObjectWith(ByteStream& Bstrm_);

    // Methods to check if object(s) is dynamically allocated by searching
    // through the custom memory manager (If option is set)
    virtual Boolean IsNewed() const;
    static Boolean IsNewed(const void* Space_);

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
class ObjectSkeleton : public ObjectImp
{
  protected:
    // Object implementation default constructor
    // No objects of this class are allowed to be constructed
    ObjectSkeleton();
  
  public:
    // Class Constructors
    ObjectSkeleton(const Object& Obj_);
    #if !defined(__BORLANDC__) & !defined(__TURBOC__)
      ObjectSkeleton(const ObjectSkeleton& Obj_);
    #endif

    // Assignment method
    ObjectSkeleton& operator = (const Object& Obj_);
    #if !defined(__BORLANDC__) & !defined(__TURBOC__)
      ObjectSkeleton& operator = (const ObjectSkeleton& Obj_);
    #endif
    
    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;  
};

/****************************************************************************/
class NullObject : public ObjectImp
{
  friend class NullObjectAcceptor;

  friend Object& RootObject(NullObject& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const NullObject& Obj_)
	{ return *((const Object*)(&Obj_)); }

  public:
    // Object implementation class constructors
    NullObject();
    NullObject(const NullObject& Obj_);

    // Virtual constructor and assignment methods
    static NullObject* Make();
    static NullObject* Make(const NullObject& Obj_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::NULLOBJECT_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Virtual constructor and assignment methods
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_ = NULL);
    virtual Object& ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_ = NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_ = NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_ = NULL);
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_ = NULL) const;

    // Assignment method
    virtual NullObject& operator = (const Object& Obj_);

    // Static null object accessor methods
    static NullObject& GiveNullObject();

    // Null object methods
    virtual Boolean IsNullObject() const;
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor*) const;

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
class ObjectIOformatter : public ObjectImp
{
  friend Object& RootObject(ObjectIOformatter& Obj_)
	{ return *Obj_._ObjPtr; }
  friend const Object& RootConstObject(const ObjectIOformatter& Obj_)
	{ return *Obj_._ObjPtr; }

  private:
    ObjectIOformatter();

  protected:
    union
    {
      Object* _ObjPtr;
      const Object* _ConstObjPtr;
    };

    // Used in an envelope class for accessing the enclosed letter object
    virtual Object* Dispatch();
    virtual const Object* Dispatch() const;
    
  public:
    ObjectIOformatter(const Object* Ptr_);
    ObjectIOformatter(const ObjectIOformatter& Obj_);

    // Virtual constructor and assignment methods
    static ObjectIOformatter* Make(const Object* Ptr_);
    static ObjectIOformatter* Make(const ObjectIOformatter& Obj_);

    virtual Object& EnclosedObject();
    virtual const Object& EnclosedObject() const;

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;    

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::OBJECTIOFORMATTER_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Virtual constructor and assignment methods
    virtual Object* Clone(ObjectAcceptor* ObjComp_ = NULL) const;
    virtual Object* ShallowClone(ObjectAcceptor* ObjComp_ = NULL);
    virtual const Object* ShallowClone(ObjectAcceptor* ObjComp_ = NULL) const;    

    ObjectIOformatter& operator = (const Object& Obj_);
    #if !defined(__BORLANDC__) & !defined(__TURBOC__)
      ObjectIOformatter& operator = (const ObjectIOformatter& Obj_);
    #endif

    // Static null object accessor methods
    static ObjectIOformatter& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Text input and output
    virtual istream& TextIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor*) const;    
  
    // IO Format methods
    virtual void ResetFormat(istream& is, ostream& os);
    virtual void SetFormat(IosFormat& IosForm_, istream& is, ostream& os);
    virtual void ResetFormat(ByteStream& is, ByteStream& os);
    virtual void SetFormat(IosFormat& IosForm_, ByteStream& is, ByteStream& os);

    // stream reset procedure
    virtual void ResetStreams(istream& is, ostream& os);
    virtual void ResetStreams(ByteStream& is, ByteStream& os);
};

/****************************************************************************/
#endif




