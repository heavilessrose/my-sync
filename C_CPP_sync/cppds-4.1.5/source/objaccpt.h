#ifndef OBJECTACCEPTOR_H
#define OBJECTACCEPTOR_H
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

/****************************************************************************/
class ObjectAcceptor
{
  public:
    virtual void AcceptDataPtr(const void* Ptr_)			= 0;
    virtual const void* GiveDataPtr() const				= 0;

    // String representation of data
    virtual const char* GiveDataAsString() const;
    virtual void AcceptDataAsString(const char* Ptr_);

    // Null Object class specific acceptor methods
    virtual Boolean IsNullObject() const;
    virtual void* CreateFromNullObject(const ObjectAcceptor&);
    virtual void* AssignFromNullObject(const ObjectAcceptor&);
    virtual Boolean IsEqualToNullObject(const ObjectAcceptor&) const;

    // Type wrapper acceptor methods
    virtual Boolean IsTypeWrapper() const;

    // Integer class specific acceptor methods
    virtual Boolean IsInteger() const;
    virtual void* CreateFromInteger(const ObjectAcceptor&);
    virtual void* AssignFromInteger(const ObjectAcceptor&);
    virtual Boolean IsEqualToInteger(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToInteger(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToInteger(const ObjectAcceptor&) const;

    // UInteger class specific acceptor methods
    virtual Boolean IsUInteger() const;
    virtual void* CreateFromUInteger(const ObjectAcceptor&);
    virtual void* AssignFromUInteger(const ObjectAcceptor&);

    // ShortInt class specific acceptor methods
    virtual Boolean IsShortInt() const;
    virtual void* CreateFromShortInt(const ObjectAcceptor&);
    virtual void* AssignFromShortInt(const ObjectAcceptor&);
    virtual Boolean IsEqualToShortInt(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToShortInt(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToShortInt(const ObjectAcceptor&) const;

    // UShortInt class specific acceptor methods
    virtual Boolean IsUShortInt() const;
    virtual void* CreateFromUShortInt(const ObjectAcceptor&);
    virtual void* AssignFromUShortInt(const ObjectAcceptor&);

    // LongInt class specific acceptor methods
    virtual Boolean IsLongInt() const;
    virtual void* CreateFromLongInt(const ObjectAcceptor&);
    virtual void* AssignFromLongInt(const ObjectAcceptor&);
    virtual Boolean IsEqualToLongInt(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToLongInt(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToLongInt(const ObjectAcceptor&) const;

    // ULongInt class specific acceptor methods
    virtual Boolean IsULongInt() const;
    virtual void* CreateFromULongInt(const ObjectAcceptor&);
    virtual void* AssignFromULongInt(const ObjectAcceptor&);

    // Float class specific acceptor methods
    virtual Boolean IsFloat() const;
    virtual void* CreateFromFloat(const ObjectAcceptor&);
    virtual void* AssignFromFloat(const ObjectAcceptor&);
    virtual Boolean IsEqualToFloat(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToFloat(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToFloat(const ObjectAcceptor&) const;

    // Double class specific acceptor methods
    virtual Boolean IsDouble() const;
    virtual void* CreateFromDouble(const ObjectAcceptor&);
    virtual void* AssignFromDouble(const ObjectAcceptor&);
    virtual Boolean IsEqualToDouble(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToDouble(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToDouble(const ObjectAcceptor&) const;

    // LongDouble class specific acceptor methods
    virtual Boolean IsLongDouble() const;
    virtual void* CreateFromLongDouble(const ObjectAcceptor&);
    virtual void* AssignFromLongDouble(const ObjectAcceptor&);
    virtual Boolean IsEqualToLongDouble(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToLongDouble(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToLongDouble(const ObjectAcceptor&) const;

    // Character class specific acceptor methods
    virtual Boolean IsCharacter() const;
    virtual void* CreateFromCharacter(const ObjectAcceptor&);
    virtual void* AssignFromCharacter(const ObjectAcceptor&);
    virtual Boolean IsEqualToCharacter(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToCharacter(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToCharacter(const ObjectAcceptor&) const;

    // String base implementation class method
    virtual Boolean IsStringImp() const;

    // String class specific acceptor methods
    virtual Boolean IsChrString() const;
    virtual void* CreateFromChrString(const ObjectAcceptor&);
    virtual void* AssignFromChrString(const ObjectAcceptor&);
    virtual Boolean IsEqualToChrString(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToChrString(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor&) const;

    // ByteString class specific acceptor methods
    virtual Boolean IsByteString() const;
    virtual void* CreateFromByteString(const ObjectAcceptor&);
    virtual void* AssignFromByteString(const ObjectAcceptor&);
    virtual Boolean IsEqualToByteString(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToByteString(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToByteString(const ObjectAcceptor&) const;

    // String browser specific acceptor methods
    virtual Boolean IsStringBrowser() const;
    virtual void* CreateFromStringBrowser(const ObjectAcceptor&);
    virtual void* AssignFromStringBrowser(const ObjectAcceptor&);

    // Standard C++ stream methods
    virtual Boolean Isistream() const;
    virtual Boolean Isostream() const;
    virtual Boolean Isifstream() const;
    virtual Boolean Isofstream() const;
    virtual Boolean Isios() const;

    // StreamReader class specific acceptor methods
    virtual Boolean IsStreamReader() const;
    virtual void* CreateFromStreamReader(const ObjectAcceptor&);
    virtual void* AssignFromStreamReader(const ObjectAcceptor&);

    // FileReader class specific acceptor methods
    virtual Boolean IsFileReader() const;
    virtual void* CreateFromFileReader(const ObjectAcceptor&);
    virtual void* AssignFromFileReader(const ObjectAcceptor&);

    // File class specific acceptor methods
    virtual Boolean IsFile() const;
    virtual void* CreateFromFile(const ObjectAcceptor&);
    virtual void* AssignFromFile(const ObjectAcceptor&);

    // Slotted file class specific acceptor methods
    virtual Boolean IsSlottedFile() const;
    virtual void* CreateFromSlottedFile(const ObjectAcceptor&);
    virtual void* AssignFromSlottedFile(const ObjectAcceptor&);

    // Variable length file class specific acceptor methods
    virtual Boolean IsVarLengthFile() const;
    virtual void* CreateFromVarLengthFile(const ObjectAcceptor&);
    virtual void* AssignFromVarLengthFile(const ObjectAcceptor&);

    // Bits class specific acceptor methods
    virtual Boolean IsBits() const;
    virtual Boolean IsEqvBits(int) const;
    virtual void* CreateFromBits(const ObjectAcceptor&);
    virtual void* AssignFromBits(const ObjectAcceptor&);
    virtual Boolean IsEqualToBits(const ObjectAcceptor&) const;

    // BitString class specific acceptor methods
    virtual Boolean IsBitString() const;
    virtual void* CreateFromBitString(const ObjectAcceptor&);
    virtual void* AssignFromBitString(const ObjectAcceptor&);
    virtual Boolean IsEqualToBitString(const ObjectAcceptor&) const;

    // Typed array specific acceptor method
    virtual Boolean IsTypedArray() const;

    // Argument array specific acceptor methods
    virtual Boolean IsArgArray() const;
    virtual void* CreateFromArgArray(const ObjectAcceptor&);
    virtual void* AssignFromArgArray(const ObjectAcceptor&);
    virtual Boolean IsEqualToArgArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToArgArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToArgArray(const ObjectAcceptor& Obj_) const;

    // Pointer array specific acceptor methods
    virtual Boolean IsPtrArray() const;
    virtual void* CreateFromPtrArray(const ObjectAcceptor&);
    virtual void* AssignFromPtrArray(const ObjectAcceptor&);
    virtual Boolean IsEqualToPtrArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToPtrArray(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToPtrArray(const ObjectAcceptor& Obj_) const;

    // Dynamic array specific acceptor methods
    virtual Boolean IsDynArray() const;
    virtual void* CreateFromDynArray(const ObjectAcceptor&);
    virtual void* AssignFromDynArray(const ObjectAcceptor&);
    virtual Boolean IsEqualToDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToDynArray(const ObjectAcceptor&) const;

    // Dynamic pointer array specific acceptor methods
    virtual Boolean IsPtrDynArray() const;
    virtual void* CreateFromPtrDynArray(const ObjectAcceptor&);
    virtual void* AssignFromPtrDynArray(const ObjectAcceptor&);
    virtual Boolean IsEqualToPtrDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToPtrDynArray(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToPtrDynArray(const ObjectAcceptor&) const;

    // Hashable class specific acceptor methods
    virtual Boolean IsHashable() const;
    virtual Boolean IsEqvHashable(int, size_t) const;
    virtual void* CreateFromHashable(const ObjectAcceptor&);
    virtual void* AssignFromHashable(const ObjectAcceptor&);
    virtual Boolean IsEqualToHashable(const ObjectAcceptor&) const;

    // Association class specific acceptor methods
    virtual Boolean IsAssociation() const;
    virtual Boolean IsEqvAssociation(int, size_t, int, size_t) const;
    virtual void* CreateFromAssociation(const ObjectAcceptor&);
    virtual void* AssignFromAssociation(const ObjectAcceptor&);
    virtual Boolean IsEqualToAssociation(const ObjectAcceptor&) const;

    // Wrapper pointer class specific acceptor methods
    virtual Boolean IsWrapper() const;
    virtual void* CreateFromWrapper(const ObjectAcceptor&);
    virtual void* AssignFromWrapper(const ObjectAcceptor&);
    virtual Boolean IsEqualToWrapper(const ObjectAcceptor&) const;
    virtual int BaseID() const;

    // Advanced for loop class specific acceptor methods
    virtual Boolean IsAdvForLoop() const;
    virtual void* CreateFromAdvForLoop(const ObjectAcceptor&);
    virtual void* AssignFromAdvForLoop(const ObjectAcceptor&);

    // DoubleList class specific acceptor methods
    virtual Boolean IsDoubleList() const;
    virtual void* CreateFromDoubleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDoubleSublist(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDoubleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDoubleSublist(const ObjectAcceptor& Obj_);
    virtual Boolean IsEqualToDoubleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDoubleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToDoubleList(const ObjectAcceptor& Obj_) const;

    // SingleList class specific acceptor methods
    virtual Boolean IsSingleList() const;
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);    
    virtual void* CreateFromSingleSublist(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);    
    virtual void* AssignFromSingleSublist(const ObjectAcceptor& Obj_);    
    virtual Boolean IsEqualToSingleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToSingleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToSingleList(const ObjectAcceptor& Obj_) const;

    // Stack specific methods
    virtual Boolean IsStack() const;
    virtual void* CreateFromStack(const ObjectAcceptor& Obj_);
    virtual void* AssignFromStack(const ObjectAcceptor& Obj_);    
    virtual Boolean IsEqualToStack(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToStack(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToStack(const ObjectAcceptor& Obj_) const;    

    // Queue specific methods
    virtual Boolean IsQueue() const;
    virtual void* CreateFromQueue(const ObjectAcceptor& Obj_);
    virtual void* AssignFromQueue(const ObjectAcceptor& Obj_);    
    virtual Boolean IsEqualToQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToQueue(const ObjectAcceptor& Obj_) const;

    // Deque specific methods
    virtual Boolean IsDeque() const;
    virtual void* CreateFromDeque(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDeque(const ObjectAcceptor& Obj_);    
    virtual Boolean IsEqualToDeque(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDeque(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToDeque(const ObjectAcceptor& Obj_) const;

    // Sorted list specific methods
    virtual Boolean IsSortedList() const;
    virtual void* CreateFromSortedList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSortedList(const ObjectAcceptor& Obj_);        
    virtual Boolean IsEqualToSortedList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToSortedList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToSortedList(const ObjectAcceptor& Obj_) const;

    // Priority Queue specific methods
    virtual Boolean IsPriorityQueue() const;
    virtual void* CreateFromPriorityQueue(const ObjectAcceptor& Obj_);
    virtual void* AssignFromPriorityQueue(const ObjectAcceptor& Obj_);        
    virtual Boolean IsEqualToPriorityQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToPriorityQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToPriorityQueue(const ObjectAcceptor& Obj_) const;    

    // specialized array type
    virtual Boolean IsNumericArray() const;
    virtual Boolean IsStatArray() const;
    virtual Boolean IsIntegerArray() const;    
    virtual void* CreateFromNumericArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromStatArray(const ObjectAcceptor& Obj_);
    virtual void* CreateFromIntegerArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromNumericArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromStatArray(const ObjectAcceptor& Obj_);
    virtual void* AssignFromIntegerArray(const ObjectAcceptor& Obj_);

    // Bst class specific methods
    virtual Boolean IsBst() const;    
    virtual void* CreateFromBst(const ObjectAcceptor& Obj_);
    virtual void* AssignFromBst(const ObjectAcceptor& Obj_);

    // AvlTree class specific methods
    virtual Boolean IsAvlTree() const;
    virtual void* CreateFromAvlTree(const ObjectAcceptor& Obj_);
    virtual void* AssignFromAvlTree(const ObjectAcceptor& Obj_);

    // ComplexNumber class specific acceptor methods
    virtual Boolean IsComplexNumber() const;
    virtual void* CreateFromComplexNumber(const ObjectAcceptor&);
    virtual void* AssignFromComplexNumber(const ObjectAcceptor&);
    virtual Boolean IsEqualToComplexNumber(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToComplexNumber(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToComplexNumber(const ObjectAcceptor&) const;

    // Year class specific acceptor methods
    virtual Boolean IsYear() const;
    virtual void* CreateFromYear(const ObjectAcceptor&);
    virtual void* AssignFromYear(const ObjectAcceptor&);

    // YMonth class specific acceptor methods
    virtual Boolean IsYMonth() const;
    virtual void* CreateFromYMonth(const ObjectAcceptor&);
    virtual void* AssignFromYMonth(const ObjectAcceptor&);

    // Date class specific acceptor methods
    virtual Boolean IsDate() const;
    virtual void* CreateFromDate(const ObjectAcceptor&);
    virtual void* AssignFromDate(const ObjectAcceptor&);
    virtual Boolean IsEqualToDate(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToDate(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToDate(const ObjectAcceptor&) const;

    // TimeOfDay class specific acceptor methods
    virtual Boolean IsTimeOfDay() const;
    virtual void* CreateFromTimeOfDay(const ObjectAcceptor& Obj_);
    virtual void* AssignFromTimeOfDay(const ObjectAcceptor& Obj_);
    virtual Boolean IsEqualToTimeOfDay(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToTimeOfDay(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToTimeOfDay(const ObjectAcceptor& Obj_) const;

    // Rational class specific acceptor methods
    virtual Boolean IsRational() const;
    virtual void* CreateFromRational(const ObjectAcceptor&);
    virtual void* AssignFromRational(const ObjectAcceptor&);
    virtual Boolean IsEqualToRational(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToRational(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToRational(const ObjectAcceptor&) const;

    // Point class specific acceptor methods
    virtual Boolean IsPoint() const;
    virtual void* CreateFromPoint(const ObjectAcceptor&);
    virtual void* AssignFromPoint(const ObjectAcceptor&);
    virtual Boolean IsEqualToPoint(const ObjectAcceptor&) const;

    // Rectangle class specific acceptor methods
    virtual Boolean IsRectangle() const;
    virtual void* CreateFromRectangle(const ObjectAcceptor&);
    virtual void* AssignFromRectangle(const ObjectAcceptor&);
    virtual Boolean IsEqualToRectangle(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToRectangle(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToRectangle(const ObjectAcceptor&) const;

    // LongNumber class specific acceptor methods
    virtual Boolean IsLongNumber() const;
    virtual void* CreateFromLongNumber(const ObjectAcceptor&);
    virtual void* AssignFromLongNumber(const ObjectAcceptor&);
    virtual Boolean IsEqualToLongNumber(const ObjectAcceptor&) const;
    virtual Boolean IsLesserToLongNumber(const ObjectAcceptor&) const;
    virtual Boolean IsGreaterToLongNumber(const ObjectAcceptor&) const;    
};

/****************************************************************************/
#define OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(Method, Host)      \
 Method (const Host *Ptr_):                                \
_ObjPtr(Ptr_)                                              \
{}

#define OBJACCEPTOR_ACCEPTDATA_TOPDEFN(Method, Host)	        \
void Method (const void* Ptr_)                                  \
{ _ObjPtr = (const Host *)Ptr_; }

#define OBJACCEPTOR_GIVEDATA_TOPDEFN(Method)	                \
const void* Method () const                                     \
{ return _ObjPtr; }

#define OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(Method, FncName, Host)	        \
Boolean Method (const ObjectAcceptor& Obj_) const                       \
{ return FncName ((const Host *)Obj_.GiveDataPtr()); }


#define OBJACCEPTOR_CREATEFNC_TOPDEFN(Method, Host, Target)        \
void* Method (const ObjectAcceptor& Obj_)                          \
{ _VarPtr = new Host (*((const Target *)Obj_.GiveDataPtr()));      \
  return _VarPtr; }

#define OBJACCEPTOR_ASSIGNFNC_TOPDEFN(Method, Target)              \
void* Method (const ObjectAcceptor& Obj_)                          \
{ const Target *StrPtr_ = (const Target *)Obj_.GiveDataPtr();      \
  *_VarPtr = *StrPtr_;                                             \
  return _VarPtr; }  

#define OBJACCEPTOR_COMPFNC_TOPDEFN(Method, FncName, Host, Target)	\
Boolean Method (const ObjectAcceptor& Obj_) const                       \
{ Host TrgObj_(*((const Target *)Obj_.GiveDataPtr()));                  \
  return FncName (&TrgObj_); }

#define OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(Method, Host)         \
void* Method (const ObjectAcceptor& Obj_)                               \
{ const TypeWrapper* TrgPtr_ = (const TypeWrapper*)Obj_.GiveDataPtr();  \
  _VarPtr = new Host (TrgPtr_->ToString());                             \
  return _VarPtr; }

#define OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(Method)               \
void* Method (const ObjectAcceptor& Obj_)                               \
{ const TypeWrapper* StrPtr_ = (const TypeWrapper*)Obj_.GiveDataPtr();  \
  *_VarPtr = StrPtr_->ToString();                                       \
  return _VarPtr; }

#define OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(Method, FncName, Host)  \
Boolean Method (const ObjectAcceptor& Obj_) const                       \
{ const TypeWrapper* TrgPtr_ = (const TypeWrapper*)Obj_.GiveDataPtr();  \
  Host TrgObj_(TrgPtr_->ToString());                                    \
  return FncName (&TrgObj_); }

#define OBJACCEPTOR_CREATEFROMFLOATTYPE_TOPDEFN(Method, Host, Prec)     \
void* Method (const ObjectAcceptor& Obj_)                               \
{ const TypeWrapper* TrgPtr_ = (const TypeWrapper*)Obj_.GiveDataPtr();  \
  _VarPtr = new Host (TrgPtr_->ToString(Prec));                         \
  return _VarPtr; }

#define OBJACCEPTOR_ASSIGNFROMFLOATTYPE_TOPDEFN(Method, Prec)           \
void* Method (const ObjectAcceptor& Obj_)                               \
{ const TypeWrapper* StrPtr_ = (const TypeWrapper*)Obj_.GiveDataPtr();  \
  *_VarPtr = StrPtr_->ToString(Prec);                                   \
  return _VarPtr; }

#define OBJACCEPTOR_COMPFROMFLOATTYPE_TOPDEFN(Method, FncName, Host, Prec)  \
Boolean Method (const ObjectAcceptor& Obj_) const                           \
{ const TypeWrapper* TrgPtr_ = (const TypeWrapper*)Obj_.GiveDataPtr();      \
  Host TrgObj_(TrgPtr_->ToString(Prec));                                    \
  return FncName (&TrgObj_); }
  
#define OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN(Method, Host)                      \
void* Method (const ObjectAcceptor& Obj_)                                         \
{ const RationalRealType* TrgPtr_ = (const RationalRealType*)Obj_.GiveDataPtr();  \
  _VarPtr = new Host (TrgPtr_->FloatValue());                                     \
  return _VarPtr; }

#define OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN(Method)                            \
void* Method (const ObjectAcceptor& Obj_)                                         \
{ const RationalRealType* StrPtr_ = (const RationalRealType*)Obj_.GiveDataPtr();  \
  *_VarPtr = StrPtr_->FloatValue();                                               \
  return _VarPtr; }

#define OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(Method, FncName, Host)               \
Boolean Method (const ObjectAcceptor& Obj_) const                                 \
{ const RationalRealType* TrgPtr_ = (const RationalRealType*)Obj_.GiveDataPtr();  \
  Host TrgObj_(TrgPtr_->FloatValue());                                            \
  return FncName (&TrgObj_); }  

/****************************************************************************/
// Method Definitions
/****************************************************************************/
#define OBJACCEPTOR_CONSTRUCTOR_METHODDEFN(Host)      \
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(Host##Acceptor::##Host##Acceptor, Host)

#define OBJACCEPTOR_ACCEPTDATA_METHODDEFN(Host)		                        \
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(Host##Acceptor::AcceptDataPtr, Host)

#define OBJACCEPTOR_GIVEDATA_METHODDEFN(Host)		                        \
OBJACCEPTOR_GIVEDATA_TOPDEFN(Host##Acceptor::GiveDataPtr)

#define OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN(Prefix, Host)		        \
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(Host##Acceptor::##Prefix##Host, _ObjPtr->##Prefix##Host, Host)


#define OBJACCEPTOR_CREATEFNC_METHODDEFN(Host, Target)		        \
OBJACCEPTOR_CREATEFNC_TOPDEFN(Host##Acceptor::CreateFrom##Target, Host, Target)

#define OBJACCEPTOR_ASSIGNFNC_METHODDEFN(Host, Target)		        \
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(Host##Acceptor::AssignFrom##Target, Target)

#define OBJACCEPTOR_COMPFNC_METHODDEFN(Prefix, Host, Target)		\
OBJACCEPTOR_COMPFNC_TOPDEFN(Host##Acceptor::##Prefix##Target, _ObjPtr->##Prefix##Host, Host, Target)


#define OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN(Host, Target)		        \
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(Host##Acceptor::CreateFrom##Target, Host)

#define OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN(Host, Target)		        \
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(Host##Acceptor::AssignFrom##Target)

#define OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN(Prefix, Host, Target)		\
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(Host##Acceptor::##Prefix##Target, _ObjPtr->##Prefix##Host, Host)


#define OBJACCEPTOR_CREATEFROMRATIONAL_METHODDEFN(Host, Target)		        \
OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN(Host##Acceptor::CreateFrom##Target, Host)

#define OBJACCEPTOR_ASSIGNFROMRATIONAL_METHODDEFN(Host, Target)		        \
OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN(Host##Acceptor::AssignFrom##Target)

#define OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN(Prefix, Host, Target)		\
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(Host##Acceptor::##Prefix##Target, _ObjPtr->##Prefix##Host, Host)

/****************************************************************************/
// Template Method Definitions
/****************************************************************************/
#define OBJACCEPTORTEMP_CONSTRUCTOR_METHODDEFN(Host)                            \
template <class T>                                                              \
OBJACCEPTOR_CONSTRUCTOR_TOPDEFN(Host##Acceptor<T>::##Host##Acceptor, Host##<T>)

#define OBJACCEPTORTEMP_ACCEPTDATA_METHODDEFN(Host)		                \
template <class T>                                                              \
OBJACCEPTOR_ACCEPTDATA_TOPDEFN(Host##Acceptor<T>::AcceptDataPtr, Host##<T>)

#define OBJACCEPTORTEMP_GIVEDATA_METHODDEFN(Host)		                \
template <class T>                                                              \
OBJACCEPTOR_GIVEDATA_TOPDEFN(Host##Acceptor<T>::GiveDataPtr)

#define OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN(Prefix, Host)		        \
template <class T>                                                              \
OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN(Host##Acceptor<T>::##Prefix##Host, _ObjPtr->##Prefix##Host, Host##<T>)

#define OBJACCEPTORTEMP_HOSTCREATEFNC_METHODDEFN(Host)		        \
template <class T>                                                      \
OBJACCEPTOR_CREATEFNC_TOPDEFN(Host##Acceptor<T>::CreateFrom##Host, Host##<T>, Host##<T>)

#define OBJACCEPTORTEMP_HOSTASSIGNFNC_METHODDEFN(Host)		        \
template <class T>                                                      \
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(Host##Acceptor<T>::AssignFrom##Host, Host##<T>)


#define OBJACCEPTORTEMP_CREATEFNC_METHODDEFN(Host, Target)		        \
template <class T>                                                              \
OBJACCEPTOR_CREATEFNC_TOPDEFN(Host##Acceptor<T>::CreateFrom##Target, Host##<T>, Target)

#define OBJACCEPTORTEMP_ASSIGNFNC_METHODDEFN(Host, Target)		        \
template <class T>                                                              \
OBJACCEPTOR_ASSIGNFNC_TOPDEFN(Host##Acceptor<T>::AssignFrom##Target, Target)

#define OBJACCEPTORTEMP_COMPFNC_METHODDEFN(Prefix, Host, Target)		\
template <class T>                                                              \
OBJACCEPTOR_COMPFNC_TOPDEFN(Host##Acceptor<T>::##Prefix##Target, _ObjPtr->##Prefix##Host, Host##<T>, Target)


#define OBJACCEPTORTEMP_CREATEFROMTYPEWRAPPER_METHODDEFN(Host, Target)		        \
template <class T>                                                                      \
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN(Host##Acceptor<T>::CreateFrom##Target, Host##<T>)

#define OBJACCEPTORTEMP_ASSIGNFROMTYPEWRAPPER_METHODDEFN(Host, Target)		        \
template <class T>                                                                      \
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN(Host##Acceptor<T>::AssignFrom##Target)

#define OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN(Prefix, Host, Target)		\
template <class T>                                                                      \
OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN(Host##Acceptor<T>::##Prefix##Target, _ObjPtr->##Prefix##Host, Host##<T>)


#define OBJACCEPTORTEMP_CREATEFROMRATIONAL_METHODDEFN(Host, Target)		        \
template <class T>                                                                      \
OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN(Host##Acceptor<T>::CreateFrom##Target, Host##<T>)

#define OBJACCEPTORTEMP_ASSIGNFROMRATIONAL_METHODDEFN(Host, Target)		        \
template <class T>                                                                      \
OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN(Host##Acceptor<T>::AssignFrom##Target)

#define OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN(Prefix, Host, Target)		\
template <class T>                                                                      \
OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN(Host##Acceptor<T>::##Prefix##Target, _ObjPtr->##Prefix##Host, Host##<T>)

/****************************************************************************/
// Grouped Methods Definitions
/****************************************************************************/
#define OBJACCEPTOR_DISPATCHFUNCS_DEFN(Host, Target)		\
OBJACCEPTOR_CREATEFNC_METHODDEFN(Host, Target)                  \
OBJACCEPTOR_ASSIGNFNC_METHODDEFN(Host, Target)                  \
OBJACCEPTOR_COMPFNC_METHODDEFN(IsLesserTo, Host, Target)        \
OBJACCEPTOR_COMPFNC_METHODDEFN(IsEqualTo, Host, Target)         \
OBJACCEPTOR_COMPFNC_METHODDEFN(IsGreaterTo, Host, Target)

#define OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN(Host, Target)	\
OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN(Host, Target)              \
OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN(Host, Target)              \
OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN(IsLesserTo, Host, Target)    \
OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN(IsEqualTo, Host, Target)     \
OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN(IsGreaterTo, Host, Target)

#define OBJACCEPTOR_RATIONAL_DISPATCHFUNCS_DEFN(Host, Target)	     \
OBJACCEPTOR_CREATEFROMRATIONAL_METHODDEFN(Host, Target)              \
OBJACCEPTOR_ASSIGNFROMRATIONAL_METHODDEFN(Host, Target)              \
OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN(IsLesserTo, Host, Target)    \
OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN(IsEqualTo, Host, Target)     \
OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN(IsGreaterTo, Host, Target)

#define OBJACCEPTOR_HOSTCLASS_DEFN(Host)                        \
OBJACCEPTOR_CONSTRUCTOR_METHODDEFN(Host)                        \
OBJACCEPTOR_ACCEPTDATA_METHODDEFN(Host)		                \
OBJACCEPTOR_GIVEDATA_METHODDEFN(Host)		                \
OBJACCEPTOR_CREATEFNC_METHODDEFN(Host, Host)                    \
OBJACCEPTOR_ASSIGNFNC_METHODDEFN(Host, Host)                    \
OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN(IsLesserTo, Host)            \
OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN(IsEqualTo, Host)             \
OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN(IsGreaterTo, Host)           \
MEMORYOPS_DEFN(Host##Acceptor)

// Template Definitions
#define OBJACCEPTOR_TEMPLATE_DISPATCHFUNCS_DEFN(Host, Target)		\
OBJACCEPTORTEMP_CREATEFNC_METHODDEFN(Host, Target)                      \
OBJACCEPTORTEMP_ASSIGNFNC_METHODDEFN(Host, Target)                      \
OBJACCEPTORTEMP_COMPFNC_METHODDEFN(IsLesserTo, Host, Target)            \
OBJACCEPTORTEMP_COMPFNC_METHODDEFN(IsEqualTo, Host, Target)             \
OBJACCEPTORTEMP_COMPFNC_METHODDEFN(IsGreaterTo, Host, Target)

#define OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN(Host, Target)		\
OBJACCEPTORTEMP_CREATEFROMTYPEWRAPPER_METHODDEFN(Host, Target)                          \
OBJACCEPTORTEMP_ASSIGNFROMTYPEWRAPPER_METHODDEFN(Host, Target)                          \
OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN(IsLesserTo, Host, Target)                \
OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN(IsEqualTo, Host, Target)                 \
OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN(IsGreaterTo, Host, Target)

#define OBJACCEPTOR_TEMPLATE_RATIONAL_DISPATCHFUNCS_DEFN(Host, Target)		\
OBJACCEPTORTEMP_CREATEFROMRATIONAL_METHODDEFN(Host, Target)                          \
OBJACCEPTORTEMP_ASSIGNFROMRATIONAL_METHODDEFN(Host, Target)                          \
OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN(IsLesserTo, Host, Target)                \
OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN(IsEqualTo, Host, Target)                 \
OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN(IsGreaterTo, Host, Target)

#define OBJACCEPTOR_TEMPLATE_HOSTCLASS_DEFN(Host)                   \
OBJACCEPTORTEMP_CONSTRUCTOR_METHODDEFN(Host)                        \
OBJACCEPTORTEMP_ACCEPTDATA_METHODDEFN(Host)		                    \
OBJACCEPTORTEMP_GIVEDATA_METHODDEFN(Host)		                       \
OBJACCEPTORTEMP_HOSTCREATEFNC_METHODDEFN(Host)                      \
OBJACCEPTORTEMP_HOSTASSIGNFNC_METHODDEFN(Host)                      \
OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN(IsLesserTo, Host)            \
OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN(IsEqualTo, Host)             \
OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN(IsGreaterTo, Host)           \
MEMORYOPS_TEMPLATE_DEFN(Host##Acceptor)
/****************************************************************************/
#endif




