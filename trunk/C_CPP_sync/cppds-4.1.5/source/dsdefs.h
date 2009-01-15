/****************************************************************************/
// Author	: Joseph Wong
// Date		: May 20
//
// Data structures definitions file
/****************************************************************************/
#ifndef DSDEFS_H
#define DSDEFS_H
/****************************************************************************/
/**************************** Standard Libraries ****************************/
#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERT_H
#endif
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif

//#define DSDEFS_TRACE
#ifdef DSDEFS_TRACE
#  define DSDEFSMARKER(x) (cerr <<x <<endl);
#  define DSDEFSTRACE(x)  (cerr <<(#x " = ") <<x <<endl);
#else
#  define DSDEFSMARKER(x)
#  define DSDEFSTRACE(x)
#endif

/***************************** User Libraries *******************************/
/***************** General Declarations For Data Structures *****************/
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef GENDECLS_H
  #include "gendecls.h"
#endif

/****************************************************************************/
// Type Information Class
class TypeInfo
{
  public:
    enum
    {
      NOTYPE = 0,
      UNDEFINED,
      EXTERNALCLASS,
      VOID_POINTER,
      DOUBLE_VOID_POINTER,
      TRIPLE_VOID_POINTER,
      NULL_POINTER,
#if HAS_BOOL
      BUILTIN_BOOL,
#endif
      BUILTIN_CHAR,
      BUILTIN_UNSIGNED_CHAR,
      BUILTIN_INT,
      BUILTIN_UNSIGNED_INT,
      BUILTIN_SHORT,
      BUILTIN_UNSIGNED_SHORT,
      BUILTIN_LONG,
      BUILTIN_UNSIGNED_LONG,
      BUILTIN_FLOAT,
      BUILTIN_DOUBLE,
      BUILTIN_LONG_DOUBLE,
      OBJECT_TYPE,
      OBJECTIMP_TYPE,
      NULLOBJECT_TYPE,
      OBJECTIOFORMATTER_TYPE,
      ADVFORLOOP_TYPE,
      INTEGER_TYPE,
      UINTEGER_TYPE,
      SHORTINT_TYPE,
      USHORTINT_TYPE,
      LONGINT_TYPE,
      ULONGINT_TYPE,
      FLOAT_TYPE,
      DOUBLE_TYPE,
      LONGDOUBLE_TYPE,
      CHARACTER_TYPE,
      COMPLEXNUMBER_TYPE,
      LONGNUMBER_TYPE,
      STRING_TYPE,
      STRINGBROWSER_TYPE,
      LONESTRING_TYPE,
      BYTESTRING_TYPE,
      STREAMREADER_TYPE,
      FILEREADER_TYPE,
      FILE_TYPE,
      SLOTTEDFILE_TYPE,
      VARLENGTHFILE_TYPE,
      INIFILE_TYPE,
      BITS_TYPE,
      BITSTRING_TYPE,
      ARGARRAY_TYPE,
      PTRARRAY_TYPE,
      DYNARRAY_TYPE,
      PTRDYNARRAY_TYPE,
      HASHABLE_TYPE,
      ASSOCIATION_TYPE,
      REFPTR_TYPE,
      MEMPTR_TYPE,
      STATICPTR_TYPE,
      OBJECTUPDATER_TYPE,
      BUILTINDELETER_TYPE,
      BUILTINUPDATER_TYPE,
      TYPEDOBJECTDELETER_TYPE,
      TYPEDOBJECTUPDATER_TYPE,
      DELETERBINDINGLIST_TYPE,
      UPDATERBINDINGLIST_TYPE,
      CONSTCOPIEDVOIDPTR_TYPE,
      CONSTCOPIEDBUILTINPTR_TYPE,
      CONSTCOPIEDOBJPTR_TYPE,
      CONSTCOUNTEDVOIDPTR_TYPE,
      CONSTCOUNTEDBUILTINPTR_TYPE,
      CONSTCOUNTEDOBJPTR_TYPE,
      CONSTUPDATEPTR_TYPE,
      CONSTARRAYPTR_TYPE,
      CONSTSTATICOBJPTR_TYPE,
      COPIEDVOIDPTR_TYPE,
      COPIEDBUILTINPTR_TYPE,
      COPIEDOBJPTR_TYPE,
      COUNTEDVOIDPTR_TYPE,
      COUNTEDBUILTINPTR_TYPE,
      COUNTEDOBJPTR_TYPE,
      UPDATEPTR_TYPE,
      ARRAYPTR_TYPE,
      STATICOBJPTR_TYPE,
      CONSTPROXYPTRIMP_TYPE,
      CONSTPROXYPOINTERPTR_TYPE,
      CONSTPROXYOBJECTPTR_TYPE,
      PROXYPTRIMP_TYPE,
      PROXYPOINTERPTR_TYPE,
      PROXYOBJECTPTR_TYPE,
      FORWARD_ITERATOR_TYPE,
      REVERSE_ITERATOR_TYPE,
      BIDIRECTIONAL_ITERATOR_TYPE,
      REVERSE_BIDIRECTIONAL_ITERATOR_TYPE,
      RANDOMACCESS_ITERATOR_TYPE,
      REVERSE_RANDOMACCESS_ITERATOR_TYPE,
      ARGARRAYITERATOR_TYPE,
      PTRARRAYITERATOR_TYPE,
      DYNARRAYITERATOR_TYPE,
      PTRDYNARRAYITERATOR_TYPE,
      DOUBLELISTITERATOR_TYPE,
      SINGLELISTITERATOR_TYPE,
      IMAGEPTR_TYPE,
      IMAGESTACKPTR_TYPE,
      PTRIMAGE_TYPE,
      CONSTTEMPOBJECT_TYPE,
      TEMPOBJECT_TYPE,
      BITVECTOR_TYPE,
      DATASTOREPTR_TYPE,
      ARRAYSTOREPTR_TYPE,
      STRINGSTOREPTR_TYPE,
      SIMPLESTRING_TYPE,
      DOUBLELIST_TYPE,
      DOUBLELISTNODE_TYPE,
      SINGLELIST_TYPE,
      SINGLELISTNODE_TYPE,
      STACK_TYPE,
      QUEUE_TYPE,
      DEQUE_TYPE,
      SORTEDLIST_TYPE,
      PRIORITYQUEUE_TYPE,
      SET_TYPE,
      FUZZYSET_TYPE,
      NUMERICARRAY_TYPE,
      STATARRAY_TYPE,
      INTEGERARRAY_TYPE,
      BST_TYPE,
      AVLTREE_TYPE,
      GRID_TYPE,      
      MATRIX_TYPE,
      POLYNOMIAL_TYPE,
      ARRAYBINTREE_TYPE,
      BTREE_TYPE,
      CHARSET_TYPE,
      YEAR_TYPE,
      YMONTH_TYPE,
      DATE_TYPE,
      TIMEOFDAY_TYPE,
      RATIONAL_TYPE,
      POINT_TYPE,
      RECTANGLE_TYPE,
      TYPEID_TOTAL
    };

    static const char* GiveClassName(int ClassId_);

  private:
    static void BuildNameList();
    static char* NewString(const char* Str_)
	{ return strcpy(new char[strlen(Str_) + 1], Str_); }

    friend void DestroyNameList(void);

    static char** _Names;
    static int _Initialized;
};

// capacity information enum type
// Used in draft standard C++ libraries
enum capacity { default_size, reserve };

/// Class declaration macros for support of identification methods
/****************************************************************************/
#if defined(__TURBOC__) | defined(__BORLANDC__)
template <class T>
T& RootObject(T& Obj_)
{
  return Obj_;
}

/****************************************************************************/
template <class T>
const T& RootConstObject(const T& Obj_)
{
  return Obj_;
}

/****************************************************************************/
template <class T>
int _WhatIs(const T& Obj_)
{
  return TypeInfo::EXTERNALCLASS_TYPE;
}

#define DEFAULT_ROOTOBJECT_DEFN(Type)

/****************************************************************************/
#else
/*
template <class T>
T& RootObject(T& Obj_)
{
  return Obj_;
}
template <class T>
const T& RootConstObject(const T& Obj_)
{
  return Obj_;
}
*/
#define DEFAULT_ROOTOBJECT_DEFN(Type)         \
ROOTOBJECTFNCS_STUB(Type, Type)

#define DEFAULT_EXTERNALCLASS_DEFN(Type)      \
EXTERNALCLASSFNCS_STUB(Type, Type)

#endif
/****************************************************************************/
/// Root class functions
#if HAS_BOOL
_FNC_INLINE bool& RootObject(bool& Obj_)
{ return Obj_; }
#endif

_FNC_INLINE char& RootObject(char& Obj_)
{ return Obj_; }

_FNC_INLINE unsigned char& RootObject(unsigned char& Obj_)
{ return Obj_; }

_FNC_INLINE int& RootObject(int& Obj_)
{ return Obj_; }

_FNC_INLINE unsigned int& RootObject(unsigned int& Obj_)
{ return Obj_; }

_FNC_INLINE short int& RootObject(short int& Obj_)
{ return Obj_; }

_FNC_INLINE unsigned short int& RootObject(unsigned short int& Obj_)
{ return Obj_; }

_FNC_INLINE long int& RootObject(long int& Obj_)
{ return Obj_; }

_FNC_INLINE unsigned long int& RootObject(unsigned long int& Obj_)
{ return Obj_; }

_FNC_INLINE float& RootObject(float& Obj_)
{ return Obj_; }

_FNC_INLINE double& RootObject(double& Obj_)
{ return Obj_; }

_FNC_INLINE long double& RootObject(long double& Obj_)
{ return Obj_; }

_FNC_INLINE void* RootObject(void* Obj_)
{ return Obj_; }

_FNC_INLINE void** RootObject(void** Obj_)
{ return Obj_; }

_FNC_INLINE void*** RootObject(void*** Obj_)
{ return Obj_; }

/// Const root class functions
#if HAS_BOOL
_FNC_INLINE const bool& RootConstObject(const bool& Obj_)
{ return Obj_; }
#endif

_FNC_INLINE const char& RootConstObject(const char& Obj_)
{ return Obj_; }

_FNC_INLINE const unsigned char& RootConstObject(const unsigned char& Obj_)
{ return Obj_; }

_FNC_INLINE const int& RootConstObject(const int& Obj_)
{ return Obj_; }

_FNC_INLINE const unsigned int& RootConstObject(const unsigned int& Obj_)
{ return Obj_; }

_FNC_INLINE const short int& RootConstObject(const short int& Obj_)
{ return Obj_; }

_FNC_INLINE const unsigned short int& RootConstObject(const unsigned short int& Obj_)
{ return Obj_; }

_FNC_INLINE const long int& RootConstObject(const long int& Obj_)
{ return Obj_; }

_FNC_INLINE const unsigned long int& RootConstObject(const unsigned long int& Obj_)
{ return Obj_; }

_FNC_INLINE const float& RootConstObject(const float& Obj_)
{ return Obj_; }

_FNC_INLINE const double& RootConstObject(const double& Obj_)
{ return Obj_; }

_FNC_INLINE const long double& RootConstObject(const long double& Obj_)
{ return Obj_; }

_FNC_INLINE void* RootConstObject(void* Obj_)
{ return Obj_; }

_FNC_INLINE void** RootConstObject(void** Obj_)
{ return Obj_; }

_FNC_INLINE void*** RootConstObject(void*** Obj_)
{ return Obj_; }

/****************************************************************************/
/// Object name functions
template <class T>
const char* NameOf(const T& Obj_)
{ return TypeInfo::GiveClassName(WhatIs(Obj_)); }

template <class T>
const char* NameOfPtr(T* Ptr_)
{ return (Ptr_ ? NameOf(*Ptr_):
		 TypeInfo::GiveClassName(TypeInfo::NULL_POINTER)); }

_FNC_INLINE const char* NameOfPtr(void* Ptr_)
{ return (Ptr_ ? TypeInfo::GiveClassName(TypeInfo::VOID_POINTER):
		 TypeInfo::GiveClassName(TypeInfo::NULL_POINTER)); }

_FNC_INLINE const char* NameOfPtr(void** Ptr_)
{ return (Ptr_ ? TypeInfo::GiveClassName(TypeInfo::DOUBLE_VOID_POINTER):
		 TypeInfo::GiveClassName(TypeInfo::NULL_POINTER)); }

_FNC_INLINE const char* NameOfPtr(void*** Ptr_)
{ return (Ptr_ ? TypeInfo::GiveClassName(TypeInfo::TRIPLE_VOID_POINTER):
		 TypeInfo::GiveClassName(TypeInfo::NULL_POINTER)); }

/****************************************************************************/
/// Type Id comparison functions
template <class T>
Boolean OfSameType(const T& Obj1_, const T& Obj2_)
{ return (WhatIs(Obj1_) == WhatIs(Obj2_)); }

template <class T>
Boolean OfSameTypePtr(T* Ptr1_, T* Ptr2_)
{ return (Ptr1_ && Ptr2_ ? OfSameType(*Ptr1_, *Ptr2_):FALSE); }

_FNC_INLINE Boolean OfSameTypePtr(void* Ptr1_, void* Ptr2_)
{ return (Ptr1_ && Ptr2_); }

_FNC_INLINE Boolean OfSameTypePtr(void** Ptr1_, void** Ptr2_)
{ return (Ptr1_ && Ptr2_); }

_FNC_INLINE Boolean OfSameTypePtr(void*** Ptr1_, void*** Ptr2_)
{ return (Ptr1_ && Ptr2_); }

/****************************************************************************/
/// Object identification functions
// Note: Weird behaviour in Turbo C++ compiler -- the template function
// parameter name must be specified even though it is not used otherwise
// Turbo C++ will complain that the parameter to the function is not defined
// in specialization functions to the template function. Leave the parameter
// name on the template function and ignore warnings of it not being used
template <class T>
int WhatIs(const T& Obj_);

#if HAS_BOOL
_FNC_INLINE int WhatIs(const bool&)
{ return TypeInfo::BUILTIN_BOOL; }
#endif

_FNC_INLINE int WhatIs(const char&)
{ return TypeInfo::BUILTIN_CHAR; }

_FNC_INLINE int WhatIs(const unsigned char&)
{ return TypeInfo::BUILTIN_UNSIGNED_CHAR; }

_FNC_INLINE int WhatIs(const int&)
{ return TypeInfo::BUILTIN_INT; }

_FNC_INLINE int WhatIs(const unsigned int&)
{ return TypeInfo::BUILTIN_UNSIGNED_INT; }

_FNC_INLINE int WhatIs(const short int&)
{ return TypeInfo::BUILTIN_SHORT; }

_FNC_INLINE int WhatIs(const unsigned short int&)
{ return TypeInfo::BUILTIN_UNSIGNED_SHORT; }

_FNC_INLINE int WhatIs(const long int&)
{ return TypeInfo::BUILTIN_LONG; }

_FNC_INLINE int WhatIs(const unsigned long int&)
{ return TypeInfo::BUILTIN_UNSIGNED_LONG; }

_FNC_INLINE int WhatIs(const float&)
{ return TypeInfo::BUILTIN_FLOAT; }

_FNC_INLINE int WhatIs(const double&)
{ return TypeInfo::BUILTIN_DOUBLE; }

_FNC_INLINE int WhatIs(const long double&)
{ return TypeInfo::BUILTIN_LONG_DOUBLE; }

template <class T>
int _WhatIs(const T& Obj_)
{ return TypeInfo::UNDEFINED; }

template <class T>
int WhatIs(const T& Obj_)
{ return _WhatIs(RootConstObject(Obj_)); }

/****************************************************************************/
/// Pointer identification functions
template <class T>
int WhatIsPtr(T* Ptr_)
{ return (Ptr_ ? WhatIs(*Ptr_):TypeInfo::NULL_POINTER); }

_FNC_INLINE int WhatIsPtr(void* Ptr_)
{ return (Ptr_ ? TypeInfo::VOID_POINTER:TypeInfo::NULL_POINTER); }

_FNC_INLINE int WhatIsPtr(void** Ptr_)
{ return (Ptr_ ? TypeInfo::DOUBLE_VOID_POINTER:TypeInfo::NULL_POINTER); }

_FNC_INLINE int WhatIsPtr(void*** Ptr_)
{ return (Ptr_ ? TypeInfo::TRIPLE_VOID_POINTER:TypeInfo::NULL_POINTER); }

/****************************************************************************/
/// Built-in types identification functions
template <class T>
Boolean IsBuiltInTypeImpl(const T& Obj_)
{
  int Id_ = WhatIs(Obj_);
  return
  (
#if HAS_BOOL
    Id_ == TypeInfo::BUILTIN_BOOL ||
#endif
    Id_ == TypeInfo::BUILTIN_CHAR ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_CHAR ||
    Id_ == TypeInfo::BUILTIN_INT ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_INT ||
    Id_ == TypeInfo::BUILTIN_SHORT ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_SHORT ||
    Id_ == TypeInfo::BUILTIN_LONG ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_LONG ||
    Id_ == TypeInfo::BUILTIN_FLOAT ||
    Id_ == TypeInfo::BUILTIN_DOUBLE ||
    Id_ == TypeInfo::BUILTIN_LONG_DOUBLE
  );
}

template <class T>
Boolean IsFloatTypeImpl(const T& Obj_)
{
  int Id_ = WhatIs(Obj_);
  return
  (
    Id_ == TypeInfo::BUILTIN_FLOAT ||
    Id_ == TypeInfo::BUILTIN_DOUBLE ||
    Id_ == TypeInfo::BUILTIN_LONG_DOUBLE
  );
}

template <class T>
Boolean IsIntegerTypeImpl(const T& Obj_)
{
  int Id_ = WhatIs(Obj_);
  return
  (
#if HAS_BOOL
    Id_ == TypeInfo::BUILTIN_BOOL ||
#endif
    Id_ == TypeInfo::BUILTIN_CHAR ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_CHAR ||
    Id_ == TypeInfo::BUILTIN_INT ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_INT ||
    Id_ == TypeInfo::BUILTIN_SHORT ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_SHORT ||
    Id_ == TypeInfo::BUILTIN_LONG ||
    Id_ == TypeInfo::BUILTIN_UNSIGNED_LONG
  );
}

template <class T>
Boolean IsBuiltInType(T* Ptr_)
{ return (Ptr_ ? IsBuiltInTypeImpl(*Ptr_):FALSE); }

_FNC_INLINE Boolean IsBuiltInType(void*)
{ return FALSE; }

_FNC_INLINE Boolean IsBuiltInType(void**)
{ return FALSE; }

_FNC_INLINE Boolean IsBuiltInType(void***)
{ return FALSE; }

// ----------
template <class T>
Boolean IsFloatType(T* Ptr_)
{ return (Ptr_ ? IsFloatTypeImpl(*Ptr_):FALSE); }

_FNC_INLINE Boolean IsFloatType(void*)
{ return FALSE; }

_FNC_INLINE Boolean IsFloatType(void**)
{ return FALSE; }

_FNC_INLINE Boolean IsFloatType(void***)
{ return FALSE; }

// ----------
template <class T>
Boolean IsIntegerType(T* Ptr_)
{ return (Ptr_ ? IsIntegerTypeImpl(*Ptr_):FALSE); }

_FNC_INLINE Boolean IsIntegerType(void*)
{ return FALSE; }

_FNC_INLINE Boolean IsIntegerType(void**)
{ return FALSE; }

_FNC_INLINE Boolean IsIntegerType(void***)
{ return FALSE; }

/****************************************************************************/
/// Class library types identification functions
template <class T>
Boolean IsObjectTypeImpl(const T& Obj_)
{
  int Id_ = WhatIs(Obj_);
  return
  (
#if HAS_BOOL
    Id_ != TypeInfo::BUILTIN_BOOL &&
#endif
    Id_ != TypeInfo::BUILTIN_CHAR &&
    Id_ != TypeInfo::BUILTIN_UNSIGNED_CHAR &&
    Id_ != TypeInfo::BUILTIN_INT &&
    Id_ != TypeInfo::BUILTIN_UNSIGNED_INT &&
    Id_ != TypeInfo::BUILTIN_SHORT &&
    Id_ != TypeInfo::BUILTIN_UNSIGNED_SHORT &&
    Id_ != TypeInfo::BUILTIN_LONG &&
    Id_ != TypeInfo::BUILTIN_UNSIGNED_LONG &&
    Id_ != TypeInfo::BUILTIN_FLOAT &&
    Id_ != TypeInfo::BUILTIN_DOUBLE &&
    Id_ != TypeInfo::BUILTIN_LONG_DOUBLE &&
    Id_ != TypeInfo::UNDEFINED &&
    Id_ != TypeInfo::EXTERNALCLASS
  );
}

template <class T>
Boolean IsObjectType(T* Ptr_)
{ return (Ptr_ ? IsObjectTypeImpl(*Ptr_):FALSE); }

_FNC_INLINE Boolean IsObjectType(void*)
{ return FALSE; }

_FNC_INLINE Boolean IsObjectType(void**)
{ return FALSE; }

_FNC_INLINE Boolean IsObjectType(void***)
{ return FALSE; }

/****************************************************************************/
/// External class types identification functions
template <class T>
Boolean IsExternalType(T* Ptr_)
{ return (Ptr_ ? (WhatIs(*Ptr_) == TypeInfo::EXTERNALCLASS):FALSE); }

_FNC_INLINE Boolean IsExternalType(void*)
{ return FALSE; }

_FNC_INLINE Boolean IsExternalType(void**)
{ return FALSE; }

_FNC_INLINE Boolean IsExternalType(void***)
{ return FALSE; }

/****************************************************************************/
#if HAS_MUTABLE
  #define MUTABLE mutable
  #define ASSIGN_CONST_MEMBER(ClassType, Member, Val)           \
  { Member = Val; }
#else
  #define MUTABLE
  #define ASSIGN_CONST_MEMBER(ClassType, Member, Val)           \
  { ClassType* const LocalThis_ = (ClassType* const)this;       \
    LocalThis_->Member = Val; }
#endif

/****************************************************************************/
#endif




