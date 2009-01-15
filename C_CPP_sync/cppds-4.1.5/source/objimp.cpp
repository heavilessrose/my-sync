//////////////////////////////////////////////////////////////////////////////
// FILE          : OBJIMP.H
// PURPOSE       : Base object implementation class type
//
// WRITTEN BY    : Joseph Wong
//
// MODIFIED BY:         LAST MODIFIED:  COMMENTS:
// ------------         --------------  ---------
// Joseph Wong          Jan 6 1999     Original Writing
//
//////////////////////////////////////////////////////////////////////////////
#ifndef OBJECTIMP_CPP
#define OBJECTIMP_CPP
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif

/****************************************************************************/
ObjectImp::ObjectImp():
_ObjectID(_Counter.GiveCount()),
_IsNull(FALSE),
_IsNewed(0),
_IOflag(IO_CLEAR)
{}

/****************************************************************************/
ObjectImp::ObjectImp(const Object& Obj_):
_ObjectID(Obj_.ObjectID()),
_IsNull(Obj_.IsNullObject()),
_IsNewed(0),
_IOflag(IO_CLEAR)
{}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
ObjectImp::ObjectImp(const ObjectImp& Obj_):
_ObjectID(Obj_.ObjectID()),
_IsNull(Obj_.IsNullObject()),
_IsNewed(0),
_IOflag(IO_CLEAR)
{}
#endif

/****************************************************************************/
// Assignment method
ObjectImp& ObjectImp::operator = (const Object& Obj_)
{
  if (this != &Obj_)
  {
    _ObjectID = Obj_.ObjectID();
    _IsNull = Obj_.IsNullObject();
  }

  return *this;
}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
// Assignment method
ObjectImp& ObjectImp::operator = (const ObjectImp& Obj_)
{
  if (this != &Obj_)
  {
    _ObjectID = Obj_.ObjectID();
    _IsNull = Obj_.IsNullObject();
  }

  return *this;
}
#endif
/****************************************************************************/
Object* ObjectImp::Dispatch()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return (_IsNull ? &NULLOBJECT():this);
}

/****************************************************************************/
const Object* ObjectImp::Dispatch() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return (_IsNull ? &NULLOBJECT():this);
}

/****************************************************************************/
Boolean ObjectImp::Destroy()
{
  if (IsNewed())
  {
    delete this;
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
Object& ObjectImp::EnclosedObject()
{
  // There is no enclosed letter object so return itself
  return *this;
}

/****************************************************************************/
const Object& ObjectImp::EnclosedObject() const
{
  // There is no enclosed letter object so return itself
  return *this;
}

/****************************************************************************/
// Class Identification methods
int ObjectImp::ClassID() const
{
  return TypeInfo::OBJECTIMP_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* ObjectImp::GiveObjectAcceptor() const
{
  return NULL;
}

/****************************************************************************/
// Object Identification method
size_t ObjectImp::ObjectID() const
{
  return _ObjectID;
}

/****************************************************************************/
long ObjectImp::DataSize() const
{
  return 0;
}

/****************************************************************************/
long ObjectImp::StorableFormWidth () const
{
  OBJXMARKER("start/end: Object::StorableFormWidth() const", cout)
  return (long(sizeof(long) +
               sizeof(long) +
	       sizeof(Byte_t)) +
	  DataSize());
}

/****************************************************************************/
const char* ObjectImp::GiveHashableData() const
{
  return NULL;
}

/****************************************************************************/
long ObjectImp::GiveHashableDataLength() const
{
  return 0;
}

/****************************************************************************/
Boolean ObjectImp::IstreamCheck(ByteStream& Is_, Boolean* Flag_, Boolean Finalize_) const
{
  istream& RealIs_ = (istream&)Is_;
  SetIOstate(RealIs_.good(), IO_STREAMERROR, Finalize_);
  Boolean Chk_ = !IsStreamError();

  if (Flag_)
    *Flag_ = Chk_;

  return Chk_;
}

/****************************************************************************/
Boolean ObjectImp::OstreamCheck(ByteStream& Os_, Boolean* Flag_, Boolean Finalize_) const
{
  ostream& RealOs_ = (ostream&)Os_;
  SetIOstate(RealOs_.good(), IO_STREAMERROR, Finalize_);
  Boolean Chk_ = !IsStreamError();

  if (Flag_)
    *Flag_ = Chk_;

  return Chk_;
}

/****************************************************************************/
Boolean ObjectImp::IstreamCheck(istream& Is_, Boolean* Flag_, Boolean Finalize_) const
{
  SetIOstate(Is_.good(), IO_STREAMERROR, Finalize_);
  Boolean Chk_ = !IsStreamError();

  if (Flag_)
    *Flag_ = Chk_;

  return Chk_;
}

/****************************************************************************/
Boolean ObjectImp::OstreamCheck(ostream& Os_, Boolean* Flag_, Boolean Finalize_) const
{
  SetIOstate(Os_.good(), IO_STREAMERROR, Finalize_);
  Boolean Chk_ = !IsStreamError();

  if (Flag_)
    *Flag_ = Chk_;

  return Chk_;
}

/****************************************************************************/
Boolean ObjectImp::ReturnIOValid(ByteStream* Is_, ByteStream* Os_, Boolean* Ok_) const
{
  istream* RealIs_;
  ostream* RealOs_;

  if (!Is_)
    RealIs_ = ((istream*)NULL);
  else
    RealIs_ = &(Is_->GetInStream());

  if (!Os_)
    RealOs_ = ((ostream*)NULL);
  else
    RealOs_ = &(Os_->GetOutStream());

  return ReturnIOValid(RealIs_, RealOs_, Ok_);
}

/****************************************************************************/
Boolean ObjectImp::ReturnIOValid(istream* Is_, ostream* Os_, Boolean* Ok_) const
{
  ObjectImp *const LocalThis_ = (ObjectImp *const)this;
  Boolean VerifiedAsGood_ = !IsIOError() && IsIOstateFinal();

  if (VerifiedAsGood_)
    LocalThis_->UnsetIOVerify();

  if (Is_)
    IstreamCheck(*Is_, Ok_);

  if (Os_)
    OstreamCheck(*Os_, Ok_);

  if (Ok_)
    *Ok_ = !IsIOError();

  LocalThis_->UnsetIOVerify();
  return !IsIOError();
}

/****************************************************************************/
void ObjectImp::SetIOstate(Boolean Flag_, Object::IOstate StatBits_, Boolean Finalize_) const
{
  ObjectImp *const LocalThis_ = (ObjectImp *const)this;

  if (IsIOstateFinal())
  {
    if (Flag_ && StatBits_ == IO_RESET && !Finalize_)
      LocalThis_->_IOflag = IO_CLEAR;
    else if (!Flag_)
      if (IsIOError())
	     LocalThis_->_IOflag |= StatBits_;
  }
  else
  {
    if (!Flag_)
    {
      if (IsIOError())
	     LocalThis_->_IOflag |= (StatBits_ | IO_VERIFY);
      else
	     LocalThis_->_IOflag = (StatBits_ | IO_INVALID | IO_VERIFY);
    }
    else
    {
      LocalThis_->_IOflag = IO_CLEAR;
      if (Finalize_)
	     LocalThis_->_IOflag |= IO_VERIFY;
    }
  }
}

/****************************************************************************/
Object::IOstate ObjectImp::GetIOstate() const
{
  return _IOflag;
}

/****************************************************************************/
Object& ObjectImp::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (!ObjComp_)
    return (ObjectImp::operator = (Obj_));

  return *this;
}

/****************************************************************************/
Object& ObjectImp::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  return *this;
}

/****************************************************************************/
Object* ObjectImp::Clone(ObjectAcceptor* ObjComp_) const
{
  if (!ObjComp_)
    return (new ObjectImp(*this));

  return NULL;
}

/****************************************************************************/
Object* ObjectImp::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (!ObjComp_)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
const Object* ObjectImp::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (!ObjComp_)
    return this;

  return Clone(ObjComp_);
}

/****************************************************************************/
NullObject& ObjectImp::GiveNullObject()
{
  return NullObject::GiveNullObject();
}

/****************************************************************************/
Object& ObjectImp::SetAsNullObject(Boolean Flag_)
{
  _IsNull = Flag_;
  return *this;
}

/****************************************************************************/
const Object& ObjectImp::SetAsNullObject(Boolean Flag_) const
{
  ObjectImp *const LocalThis_ = (ObjectImp *const)this;
  LocalThis_->_IsNull = Flag_;

  return *this;
}

/****************************************************************************/
Object& ObjectImp::SetObjectID(size_t Id_)
{
  _ObjectID = Id_;
  return *this;
}

/****************************************************************************/
const Object& ObjectImp::SetObjectID(size_t Id_) const
{
  ObjectImp *const LocalThis_ = (ObjectImp *const)this;
  LocalThis_->_ObjectID = Id_;

  return *this;
}

/****************************************************************************/
Boolean ObjectImp::IsNullObject() const
{
  return (_IsNull != FALSE);
}

/****************************************************************************/
const Object& ObjectImp::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return NullObject::GiveNullObject();
}

/****************************************************************************/
Object& ObjectImp::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return NullObject::GiveNullObject();
}

/****************************************************************************/
istream& ObjectImp::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Object* ObjPtr_ = Clone();

  if (ObjPtr_)
  {
    istream& StrmRef_ = ObjPtr_->BinaryRead(Is_, Ok_);
    delete ObjPtr_;
    return StrmRef_;
  }

  return Is_;
}

/****************************************************************************/
// ----- Saving and restoration in binary form ----
istream& ObjectImp::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& ObjectImp::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& ObjectImp::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Object* ObjPtr_ = Clone();

  if (ObjPtr_)
  {
    ByteStream& StrmRef_ = ObjPtr_->BinaryRead(Is_, Ok_);
    delete ObjPtr_;
    return StrmRef_;
  }

  return Is_;
}

/****************************************************************************/
// ----- Saving and restoration in binary form ----
ByteStream& ObjectImp::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& ObjectImp::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
istream& ObjectImp::TextIgnore(istream& Is_, Boolean* Ok_)
{
  Object* ObjPtr_ = Clone();

  if (ObjPtr_)
  {
    istream& StrmRef_ = ObjPtr_->TextRead(Is_, Ok_);
    delete ObjPtr_;
    return StrmRef_;
  }

  return Is_;
}

/****************************************************************************/
istream& ObjectImp::TextRead(istream& Is_, Boolean* Ok_)
{
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& ObjectImp::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& ObjectImp::WriteObjectWith(ByteStream& Bstrm_) const
{
  return Bstrm_.WriteWithThisObject(this);
}

/****************************************************************************/
ByteStream& ObjectImp::ReadObjectWith(ByteStream& Bstrm_)
{
  return Bstrm_.ReadWithThisObject(this);
}

/****************************************************************************/
Boolean ObjectImp::IsNewed(const void* Space_)
{
  void* Address_ = ((void*)Space_);
#if OVERLOAD_NEW
  return MemMatrix::Matrix().HasThis(Address_);
#else
  // This will lead to memory leaks, but its better than crashing
  // from deleting non-heap objects
  return FALSE;
#endif
}

/****************************************************************************/
Boolean ObjectImp::IsNewed() const
{
  if (!NewedVerified())
  {
    SetNewedVerify();
    if (ObjectImp::IsNewed((Object*)this))
      SetNewedFlag();
  }

  return NewedFlag();
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ObjectImp::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ObjectImp::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
void* ObjectImp::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if HAS_ARRAY_NEW
void ObjectImp::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
ObjectSkeleton::ObjectSkeleton():
ObjectImp()
{}

/****************************************************************************/
ObjectSkeleton::ObjectSkeleton(const Object& Obj_):
ObjectImp(Obj_)
{}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
ObjectSkeleton::ObjectSkeleton(const ObjectSkeleton& Obj_):
ObjectImp(Obj_)
{}
#endif

/****************************************************************************/
// Assignment method
ObjectSkeleton& ObjectSkeleton::operator = (const Object& Obj_)
{
  if (this != &Obj_)
    ObjectImp::operator = (Obj_);

  return *this;
}

/****************************************************************************/
#if !defined(__BORLANDC__) & !defined(__TURBOC__)
// Assignment method
ObjectSkeleton& ObjectSkeleton::operator = (const ObjectSkeleton& Obj_)
{
  if (this != &Obj_)
    ObjectImp::operator = (Obj_);  

  return *this;
}
#endif
/****************************************************************************/
istream& ObjectSkeleton::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& ObjectSkeleton::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& ObjectSkeleton::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& ObjectSkeleton::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& ObjectSkeleton::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& ObjectSkeleton::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
/****************************************************************************/
NullObject::NullObject():
ObjectImp()
{
  SetAsNullObject();
}

/****************************************************************************/
NullObject::NullObject(const NullObject& Obj_):
ObjectImp(Obj_)
{
  SetAsNullObject();
}

/****************************************************************************/
NullObject* NullObject::Make()
{
  return (new NullObject());
}

/****************************************************************************/
NullObject* NullObject::Make(const NullObject& Obj_)
{
  return (new NullObject(Obj_));
}

/****************************************************************************/
Object& NullObject::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  NullObjectAcceptor TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromNullObject(TrgComp_)));
}

/****************************************************************************/
Object& NullObject::ShallowAssign(Object& Obj_, ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return *this;

  NullObjectAcceptor TrgComp_(this);
  return *((Object*) (ObjComp_->AssignFromNullObject(TrgComp_)));
}

/****************************************************************************/
Object* NullObject::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new NullObject(*this));

  NullObjectAcceptor TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromNullObject(TrgComp_)));
}

/****************************************************************************/
Object* NullObject::ShallowClone(ObjectAcceptor* ObjComp_)
{
  if (ObjComp_ == NULL)
    return this;

  return Clone();
}

/****************************************************************************/
const Object* NullObject::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return this;

  return Clone();
}

/****************************************************************************/
int NullObject::ClassID() const
{
  return TypeInfo::NULLOBJECT_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* NullObject::GiveObjectAcceptor() const
{
  return (new NullObjectAcceptor(this));
}

/****************************************************************************/
NullObject& NullObject::operator = (const Object& Obj_)
{
  ObjectImp::operator = (Obj_);
  SetAsNullObject();
  return *this;
}

/****************************************************************************/
NullObject& NullObject::GiveNullObject()
{
  static NullObject _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
Boolean NullObject::IsNullObject() const
{
  return TRUE;
}

/****************************************************************************/
const Object& NullObject::NULLOBJECT() const
{
  return *this;
}

/****************************************************************************/
Object& NullObject::NULLOBJECT()
{
  return *this;
}

/****************************************************************************/
istream& NullObject::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
istream& NullObject::BinaryRead(istream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ostream& NullObject::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
ByteStream& NullObject::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, TRUE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& NullObject::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  ReadObjectData(*this, Is_, FALSE);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
ByteStream& NullObject::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  WriteObjectData(*this, Os_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
Boolean NullObject::IsEqual(const Object& Obj_, const ObjectAcceptor*) const
{
  return Obj_.IsNullObject();
}

/****************************************************************************/
#if OVERLOAD_NEW
void* NullObject::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void NullObject::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* NullObject::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void NullObject::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
NullObjectAcceptor::NullObjectAcceptor(const NullObject* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
Boolean NullObjectAcceptor::IsNullObject() const
{
  return TRUE;
}

/****************************************************************************/
void NullObjectAcceptor::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const NullObject*)Ptr_;
}

/****************************************************************************/
const void* NullObjectAcceptor::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
void* NullObjectAcceptor::CreateFromNullObject(const ObjectAcceptor&)
{
  _VarPtr = (NullObject*)(_VarPtr->NULLOBJECT().Clone());
  return _VarPtr;
}

/****************************************************************************/
void* NullObjectAcceptor::AssignFromNullObject(const ObjectAcceptor&)
{
  *_VarPtr = (NullObject&)(_VarPtr->NULLOBJECT());
  return _VarPtr;
}

/****************************************************************************/
Boolean NullObjectAcceptor::IsEqualToNullObject(const ObjectAcceptor& Obj_) const
{
  return TRUE;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* NullObjectAcceptor::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void NullObjectAcceptor::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
ObjectIOformatter::ObjectIOformatter():
_ConstObjPtr(NULL)
{}

/****************************************************************************/
ObjectIOformatter::ObjectIOformatter(const Object* Ptr_):
_ConstObjPtr(Ptr_)
{
  if (!Ptr_)
    XSHOWFATAL("ERROR: Attempt To Dereference Null Pointer", cerr);
}

/****************************************************************************/
ObjectIOformatter::ObjectIOformatter(const ObjectIOformatter& Obj_):
_ObjPtr(Obj_._ObjPtr)
{}

/****************************************************************************/
Object* ObjectIOformatter::Dispatch()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return (_IsNull ? &_ObjPtr->NULLOBJECT():_ObjPtr);
}

/****************************************************************************/
const Object* ObjectIOformatter::Dispatch() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return (_IsNull ? &_ConstObjPtr->NULLOBJECT():_ConstObjPtr);
}

/****************************************************************************/
Object& ObjectIOformatter::EnclosedObject()
{
  return *_ObjPtr;
}

/****************************************************************************/
const Object& ObjectIOformatter::EnclosedObject() const
{
  return *_ConstObjPtr;
}

/****************************************************************************/
const char* ObjectIOformatter::GiveHashableData() const
{
  return _ConstObjPtr->GiveHashableData();
}

/****************************************************************************/
long ObjectIOformatter::GiveHashableDataLength() const
{
  return _ConstObjPtr->GiveHashableDataLength();
}

/****************************************************************************/
ObjectIOformatter* ObjectIOformatter::Make(const Object* Ptr_)
{
  return (new ObjectIOformatter(Ptr_));
}

/****************************************************************************/
ObjectIOformatter* ObjectIOformatter::Make(const ObjectIOformatter& Obj_)
{
  return (new ObjectIOformatter(Obj_));
}

/****************************************************************************/
Object* ObjectIOformatter::Clone(ObjectAcceptor* ObjComp_) const
{
  return (new ObjectIOformatter(*this));
}

/****************************************************************************/
Object* ObjectIOformatter::ShallowClone(ObjectAcceptor* ObjComp_)
{
  return Clone();
}

/****************************************************************************/
const Object* ObjectIOformatter::ShallowClone(ObjectAcceptor* ObjComp_) const
{
  return Clone();
}

/****************************************************************************/
int ObjectIOformatter::ClassID() const
{
  return TypeInfo::OBJECTIOFORMATTER_TYPE;
}

/****************************************************************************/
const ObjectAcceptor* ObjectIOformatter::GiveObjectAcceptor() const
{
  return NULL;
}

/****************************************************************************/
ObjectIOformatter& ObjectIOformatter::operator = (const Object& Obj_)
{
  ObjectImp::operator = (Obj_);
  _ConstObjPtr = &Obj_.EnclosedObject();
  return *this;
}

/****************************************************************************/
ObjectIOformatter& ObjectIOformatter::operator = (const ObjectIOformatter& Obj_)
{
  ObjectImp::operator = (Obj_);
  _ConstObjPtr = Obj_._ConstObjPtr;
  return *this;
}

/****************************************************************************/
ObjectIOformatter& ObjectIOformatter::GiveNullObject()
{
  static ObjectIOformatter _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
const Object& ObjectIOformatter::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return GiveNullObject();
}

/****************************************************************************/
Object& ObjectIOformatter::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return GiveNullObject();
}

/****************************************************************************/
Boolean ObjectIOformatter::IsEqual(const Object& Obj_, const ObjectAcceptor*) const
{
  return (_ConstObjPtr == &Obj_.EnclosedObject());
}

/****************************************************************************/
istream& ObjectIOformatter::TextIgnore(istream& Is_, Boolean* Ok_)
{
  Object* ObjPtr_ = _ObjPtr->Clone();

  if (ObjPtr_)
  {
    istream& StrmRef_ = ObjPtr_->TextRead(Is_, Ok_);
    delete ObjPtr_;
    return StrmRef_;
  }

  return Is_;
}

/****************************************************************************/
istream& ObjectIOformatter::TextRead(istream& Is_, Boolean* Ok_)
{
  Object* DataPtr_ = _ObjPtr->Clone();
  Is_ >>(*DataPtr_);
  *_ObjPtr = *DataPtr_;
  delete DataPtr_;

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/    
ostream& ObjectIOformatter::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<(*_ConstObjPtr);
  return ObjectImp::TextWrite(Os_, Ok_);
}
  
/****************************************************************************/
void ObjectIOformatter::ResetFormat(istream& is, ostream& os)
{
  is >>IosReset();
  os <<IosReset();
}

/****************************************************************************/
void ObjectIOformatter::ResetFormat(ByteStream& is, ByteStream& os)
{
  is.GetInStream(FALSE) >>IosReset();
  os.GetOutStream(FALSE) <<IosReset();
}

/****************************************************************************/
void ObjectIOformatter::SetFormat(IosFormat& IosForm_, istream& is, ostream& os)
{
  is >>IosSet(IosForm_);
  os <<IosSet(IosForm_);
}

/****************************************************************************/
void ObjectIOformatter::SetFormat(IosFormat& IosForm_, ByteStream& is, ByteStream& os)
{
  is.GetInStream(FALSE) >>IosSet(IosForm_);
  os.GetOutStream(FALSE) <<IosSet(IosForm_);
}

/****************************************************************************/
void ObjectIOformatter::ResetStreams(istream& is, ostream& os)
{
  (is.seekg(0) >>IosReset()).clear();
  (os.flush().seekp(0) <<IosReset()).clear();
}

/****************************************************************************/
void ObjectIOformatter::ResetStreams(ByteStream& is, ByteStream& os)
{
  (is.GetInStream().seekg(0) >>IosReset()).clear();
  (os.GetOutStream().flush().seekp(0) <<IosReset()).clear();
}

/****************************************************************************/
/****************************************************************************/
#if OBJIMP_DEBUG
int main()
{
#if OBJIMP_DEBUG2
  NullObject Obj_;
  ObjectIOformatter Formatter_(&Obj_);
  Boolean Bvar_ = FALSE;
  Boolean* Det_ = &Bvar_;
  size_t Intv_;

  ShowInfo((ostream&)cout, Intv_);
  cout <<endl;
  ShowInfo((ostream&)cout, Obj_);
  cout <<endl;

  Intv_ = CountOf(Obj_, Det_);
#if OBJIMP_SHOW_DEBUG_OUTPUT
  cout <<"CountOf(Obj_, Det_)  = " <<Intv_ <<"," <<*Det_ <<endl;
  cout <<"CountOf(Intv_) = " <<CountOf(Intv_) <<endl;
  cout <<"WhatIs() = " <<WhatIs(Obj_) <<"\tNameOf() = " <<NameOf(Obj_) <<endl;
#else
  assert(Intv_ == 1 && *Det_ == TRUE);
  assert(CountOf(Intv_) == 0);
  assert(WhatIs(Obj_) == TypeInfo::NULLOBJECT_TYPE);
  assert(!strcmp(NameOf(Obj_), "NullObject"));
#endif

  NullObject* Ptr_ = ::Clone(Obj_);
  const NullObject* ConstPtr_ = ::ConstClone(*Ptr_);
  NullObject* NonConstPtr_ = ::NonConstClone(*Ptr_);

  ShowInfo((ostream&)cout, *Ptr_);
  cout <<endl;
#if OBJIMP_SHOW_DEBUG_OUTPUT
  cout <<"null object = " <<Obj_.IsNullObject() <<endl;
  cout <<"null object = " <<Ptr_->IsNullObject() <<endl;
  cout <<"null object = " <<(*Ptr_ == Obj_) <<endl;
  cout <<"null object = " <<(*Ptr_ == Obj_.NULLOBJECT()) <<endl;
  cout <<"null object = " <<(*Ptr_ == NullObject::GiveNullObject()) <<endl;
#else
  assert(Obj_.IsNullObject());
  assert(Ptr_->IsNullObject());
  assert(*Ptr_ == Obj_);
  assert(*Ptr_ == Obj_.NULLOBJECT());
  assert(*Ptr_ == NullObject::GiveNullObject());
#endif

#if defined(__TURBOC__)
  // Testing reallocation method
  NullObject* ArrObj_ = new NullObject;
  ArrObj_ = ReallocateWith(MEMMATRIX, ArrObj_, size_t(3));
#if OBJIMP_SHOW_DEBUG_OUTPUT
  cout <<"ArrObj_[0] = " <<ArrObj_[0].IsNullObject() <<endl;
  cout <<"ArrObj_[1] = " <<ArrObj_[1].IsNullObject() <<endl;
  cout <<"ArrObj_[2] = " <<ArrObj_[2].IsNullObject() <<endl;
#else
  assert(ArrObj_[0].IsNullObject());
  assert(ArrObj_[1].IsNullObject());
  assert(ArrObj_[2].IsNullObject());
#endif
  delete[] ArrObj_;
#endif

  delete Ptr_;
#endif

#if OBJIMP_DEBUG3
  char cvar;
  int ivar;
  unsigned int uivar;
  short svar;
  Ushort usvar;
  long lvar;
  Ulong ulvar;
  float fvar;
  double dvar;
  Ldouble ldvar;
  SomeDumStruct EmptyStruct;

  int filler = 0;
  void* voidp = &filler;
  void** dvoidp = &voidp;
  int* nullp = NULL;

  assert(IsBuiltInType(&cvar) && WhatIs(cvar) == TypeInfo::BUILTIN_CHAR &&
	 !strcmp(NameOf(cvar), "char"));
  assert(IsBuiltInType(&ivar) && WhatIs(ivar) == TypeInfo::BUILTIN_INT &&
	 !strcmp(NameOf(ivar), "int"));
  assert(IsBuiltInType(&uivar) && WhatIs(uivar) == TypeInfo::BUILTIN_UNSIGNED_INT &&
	 !strcmp(NameOf(uivar), "unsigned int"));
  assert(IsBuiltInType(&svar) && WhatIs(svar) == TypeInfo::BUILTIN_SHORT &&
	 !strcmp(NameOf(svar), "short"));
  assert(IsBuiltInType(&usvar) && WhatIs(usvar) == TypeInfo::BUILTIN_UNSIGNED_SHORT &&
	 !strcmp(NameOf(usvar), "unsigned short"));
  assert(IsBuiltInType(&lvar) && WhatIs(lvar) == TypeInfo::BUILTIN_LONG &&
	 !strcmp(NameOf(lvar), "long"));
  assert(IsBuiltInType(&ulvar) && WhatIs(ulvar) == TypeInfo::BUILTIN_UNSIGNED_LONG &&
	 !strcmp(NameOf(ulvar), "unsigned long"));
  assert(IsBuiltInType(&fvar) && WhatIs(fvar) == TypeInfo::BUILTIN_FLOAT &&
	 !strcmp(NameOf(fvar), "float"));
  assert(IsBuiltInType(&dvar) && WhatIs(dvar) == TypeInfo::BUILTIN_DOUBLE &&
	 !strcmp(NameOf(dvar), "double"));
  assert(IsBuiltInType(&ldvar) && WhatIs(ldvar) == TypeInfo::BUILTIN_LONG_DOUBLE &&
	 !strcmp(NameOf(ldvar), "long double"));
  assert(!IsBuiltInType(voidp) && WhatIsPtr(voidp) == TypeInfo::VOID_POINTER &&
	 !strcmp(NameOfPtr(voidp), "void*"));
  assert(!IsBuiltInType(dvoidp) && WhatIsPtr(dvoidp) == TypeInfo::DOUBLE_VOID_POINTER &&
	 !strcmp(NameOfPtr(dvoidp), "void**"));
  assert(!IsBuiltInType(nullp) && WhatIsPtr(nullp) == TypeInfo::NULL_POINTER &&
	 !strcmp(NameOfPtr(nullp), "NULL"));
  assert(!IsBuiltInType(&EmptyStruct) && WhatIs(EmptyStruct) == TypeInfo::UNDEFINED &&
	 !strcmp(NameOf(EmptyStruct), "Undefined Type"));

  char* cvarp = Clone('z');
  int* ivarp = Clone(2);
  unsigned int* uivarp = Clone(3U);
  short* svarp = Clone(short(4));
  Ushort* usvarp = Clone(Ushort(5));
  long* lvarp = Clone(6L);
  Ulong* ulvarp = Clone(7UL);
  float* fvarp = Clone(float(8.3));
  double* dvarp = Clone(9.6);
  Ldouble* ldvarp = Clone(Ldouble(10.9));
  voidp = Clone(EmptyStruct);

#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(_MSC_VER)
  int** dintp = &ivarp;
  void** dvoidp = &voidp;
  assert(IsNestedPtr(dintp));
  assert(!IsNestedPtr(voidp));
  assert(IsNestedPtr(dvoidp));
#endif

  assert(IsStreamable(voidp));
  assert(cvarp && *cvarp == 'z');
  assert(ivarp && *ivarp == 2);
  assert(uivarp && *uivarp == 3);
  assert(svarp && *svarp == 4);
  assert(usvarp && *usvarp == 5);
  assert(lvarp && *lvarp == 6);
  assert(ulvarp && *ulvarp == 7);
  cout <<*fvarp <<" ";	// 8.3
  cout <<*dvarp <<" ";	// 9.6
  cout <<*ldvarp <<" ";	// 10.9

  // Testing template functions
  int ivar2;
  ivar = 5; ivar2 = 10;
  Swap(ivar, ivar2); assert(ivar == 10 && ivar2 == 5);

#if defined(__TURBOC__) | defined(__BORLANDC__)
  assert(SizeOfData(ivar) == 2);
  assert(FormWidthOfObject(ivar) == 6);
#else
  assert(SizeOfData(ivar) == 4);
  assert(FormWidthOfObject(ivar) == 8);
#endif

  Assign(ivar, ivar2); assert(ivar == 5 && ivar2 == 5);
  ivar = 29;
  WriteToOutput((ostream&)cout, ivar); // 29
  ivar2 = 30;
  NonConstAssign(ivar, ivar2); assert(ivar == 30 && ivar2 == 30);
  Delete(ivarp);
  ivarp = Clone(ivar);
  assert(*ivarp == 30);

  Delete(voidp);
  Delete(cvarp);
  Delete(ivarp);
  Delete(uivarp);
  Delete(svarp);
  Delete(usvarp);
  Delete(lvarp);
  Delete(ulvarp);
  Delete(fvarp);
  Delete(dvarp);
  Delete(ldvarp);

  // Testing memory manager -- these extra deletes should not cause
  // any problems, but should be ignored by the memory manager.
  Delete(voidp);
  Delete(cvarp);
  Delete(ivarp);
  Delete(uivarp);
  Delete(svarp);
  Delete(usvarp);
  Delete(lvarp);
  Delete(ulvarp);
  Delete(fvarp);
  Delete(dvarp);
  Delete(ldvarp);

  cout <<endl <<"SUCCESS Testing generic template functions" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




