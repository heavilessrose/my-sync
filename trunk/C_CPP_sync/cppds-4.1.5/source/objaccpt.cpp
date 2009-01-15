#ifndef OBJECTACCEPTOR_CPP
#define OBJECTACCEPTOR_CPP
#ifndef OBJECTACCEPTOR_H
  #include "objaccpt.h"
#endif

// String representation of data
/****************************************************************************/
const char* ObjectAcceptor::GiveDataAsString() const
{ return NULL; }

/****************************************************************************/
void ObjectAcceptor::AcceptDataAsString(const char* Ptr_)
{}

// NullObject class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsNullObject() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromNullObject(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromNullObject(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToNullObject(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// Type wrapper class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsTypeWrapper() const
{ return FALSE; }
/****************************************************************************/

// Integer class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsInteger() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromInteger(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromInteger(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToInteger(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToInteger(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToInteger(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// UInteger class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsUInteger() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromUInteger(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromUInteger(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/

// ShortInt class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsShortInt() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromShortInt(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromShortInt(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToShortInt(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToShortInt(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToShortInt(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// UShortInt class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsUShortInt() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromUShortInt(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromUShortInt(const ObjectAcceptor&)
{ return NULL; }

// LongInt class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsLongInt() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromLongInt(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromLongInt(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToLongInt(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToLongInt(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToLongInt(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// ULongInt class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsULongInt() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromULongInt(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromULongInt(const ObjectAcceptor&)
{ return NULL; }

// Float class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsFloat() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromFloat(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromFloat(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToFloat(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToFloat(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToFloat(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// Double class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsDouble() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromDouble(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromDouble(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToDouble(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToDouble(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToDouble(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// Long Double class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsLongDouble() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromLongDouble(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromLongDouble(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToLongDouble(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToLongDouble(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToLongDouble(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// Character class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsCharacter() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromCharacter(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromCharacter(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToCharacter(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToCharacter(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToCharacter(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/

// String base implementation class method
/****************************************************************************/
Boolean ObjectAcceptor::IsStringImp() const
{ return FALSE; }

// String class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsChrString() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromChrString(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromChrString(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToChrString(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToChrString(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToChrString(const ObjectAcceptor&) const
{ return FALSE; }

// ByteString class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsByteString() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromByteString(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromByteString(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToByteString(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToByteString(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToByteString(const ObjectAcceptor&) const
{ return FALSE; }

// StringBrowser class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsStringBrowser() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromStringBrowser(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromStringBrowser(const ObjectAcceptor&)
{ return NULL; }

// Standard C++ stream methods
/****************************************************************************/
Boolean ObjectAcceptor::Isistream() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::Isostream() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::Isifstream() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::Isofstream() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::Isios() const
{ return FALSE; }

// Stream Reader class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsStreamReader() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromStreamReader(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromStreamReader(const ObjectAcceptor&)
{ return NULL; }

// File Reader class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsFileReader() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromFileReader(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromFileReader(const ObjectAcceptor&)
{ return NULL; }

// File Reader class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsFile() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromFile(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromFile(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/

// Slotted file class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsSlottedFile() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromSlottedFile(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromSlottedFile(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/

// Variable length file class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsVarLengthFile() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromVarLengthFile(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromVarLengthFile(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/

// Bits class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsBits() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqvBits(int) const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromBits(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromBits(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToBits(const ObjectAcceptor&) const
{ return FALSE; }

// BitString class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsBitString() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromBitString(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromBitString(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToBitString(const ObjectAcceptor&) const
{ return FALSE; }

// Typed Array specific method
/****************************************************************************/
Boolean ObjectAcceptor::IsTypedArray() const
{ return FALSE; }

// Argument Array specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsArgArray() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromArgArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromArgArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToArgArray(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToArgArray(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToArgArray(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// Pointer array specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsPtrArray() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromPtrArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromPtrArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToPtrArray(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToPtrArray(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToPtrArray(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// Dynamic Array specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsDynArray() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromDynArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromDynArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToDynArray(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToDynArray(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToDynArray(const ObjectAcceptor&) const
{ return FALSE; }

// Dynamic pointer array specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsPtrDynArray() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromPtrDynArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromPtrDynArray(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToPtrDynArray(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToPtrDynArray(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToPtrDynArray(const ObjectAcceptor&) const
{ return FALSE; }

// Hashable class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsHashable() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqvHashable(int, size_t) const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromHashable(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromHashable(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToHashable(const ObjectAcceptor&) const
{ return FALSE; }

// Association class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsAssociation() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqvAssociation(int, size_t, int, size_t) const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromAssociation(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromAssociation(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToAssociation(const ObjectAcceptor&) const
{ return FALSE; }

// Wrapper pointer class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsWrapper() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromWrapper(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromWrapper(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToWrapper(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
int ObjectAcceptor::BaseID() const
{ return 0; }

// Advanced for loop specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsAdvForLoop() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromAdvForLoop(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromAdvForLoop(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/

// DoubleList class specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsDoubleList() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromDoubleList(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromDoubleSublist(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromDoubleList(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromDoubleSublist(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToDoubleList(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToDoubleList(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToDoubleList(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// SingleList class specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsSingleList() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromSingleList(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromSingleSublist(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromSingleList(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromSingleSublist(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToSingleList(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToSingleList(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToSingleList(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// Stack specific methods
/****************************************************************************/    
Boolean ObjectAcceptor::IsStack() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromStack(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromStack(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToStack(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToStack(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToStack(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// Queue specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsQueue() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromQueue(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromQueue(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToQueue(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToQueue(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToQueue(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// Deque specific methods
/****************************************************************************/
Boolean ObjectAcceptor::IsDeque() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromDeque(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromDeque(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToDeque(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToDeque(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToDeque(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// SortedList specific methods
/****************************************************************************/    
Boolean ObjectAcceptor::IsSortedList() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromSortedList(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromSortedList(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToSortedList(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToSortedList(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToSortedList(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// PriorityQueue specific methods
/****************************************************************************/    
Boolean ObjectAcceptor::IsPriorityQueue() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromPriorityQueue(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromPriorityQueue(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToPriorityQueue(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToPriorityQueue(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToPriorityQueue(const ObjectAcceptor& Obj_) const
{ return FALSE; }

// specialized array methods
/****************************************************************************/
Boolean ObjectAcceptor::IsNumericArray() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsStatArray() const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsIntegerArray() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromNumericArray(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromStatArray(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromIntegerArray(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromNumericArray(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromStatArray(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromIntegerArray(const ObjectAcceptor& Obj_)
{ return NULL; }

// specialized Bst methods
/****************************************************************************/
Boolean ObjectAcceptor::IsBst() const
{ return FALSE; }
/****************************************************************************/    
void* ObjectAcceptor::CreateFromBst(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/    
void* ObjectAcceptor::AssignFromBst(const ObjectAcceptor& Obj_)
{ return NULL; }

// specialized AvlTree methods
/****************************************************************************/
Boolean ObjectAcceptor::IsAvlTree() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromAvlTree(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromAvlTree(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/

// ComplexNumber class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsComplexNumber() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromComplexNumber(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromComplexNumber(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToComplexNumber(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToComplexNumber(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToComplexNumber(const ObjectAcceptor&) const
{ return FALSE; }

// Rational class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsRational() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromRational(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromRational(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToRational(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToRational(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToRational(const ObjectAcceptor&) const
{ return FALSE; }

// Point class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsPoint() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromPoint(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromPoint(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToPoint(const ObjectAcceptor&) const
{ return FALSE; }

// Rectangle class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsRectangle() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromRectangle(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromRectangle(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToRectangle(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToRectangle(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToRectangle(const ObjectAcceptor&) const
{ return FALSE; }

// Year class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsYear() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromYear(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromYear(const ObjectAcceptor&)
{ return NULL; }

// YMonth class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsYMonth() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromYMonth(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromYMonth(const ObjectAcceptor&)
{ return NULL; }

// Date class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsDate() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromDate(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromDate(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToDate(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToDate(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToDate(const ObjectAcceptor&) const
{ return FALSE; }

// TimeOfDay class specific acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsTimeOfDay() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromTimeOfDay(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromTimeOfDay(const ObjectAcceptor& Obj_)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToTimeOfDay(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToTimeOfDay(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToTimeOfDay(const ObjectAcceptor& Obj_) const
{ return FALSE; }
/****************************************************************************/

// LongNumber class specific Acceptor methods
/****************************************************************************/
Boolean ObjectAcceptor::IsLongNumber() const
{ return FALSE; }
/****************************************************************************/
void* ObjectAcceptor::CreateFromLongNumber(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
void* ObjectAcceptor::AssignFromLongNumber(const ObjectAcceptor&)
{ return NULL; }
/****************************************************************************/
Boolean ObjectAcceptor::IsEqualToLongNumber(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsLesserToLongNumber(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
Boolean ObjectAcceptor::IsGreaterToLongNumber(const ObjectAcceptor&) const
{ return FALSE; }
/****************************************************************************/
#endif




