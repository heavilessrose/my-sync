#ifndef DSDEFS_CPP
#define DSDEFS_CPP
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif

char** TypeInfo::_Names = NULL;
int TypeInfo::_Initialized = 0;

/****************************************************************************/
void DestroyNameList(void)
{
  DSDEFSMARKER("start: DestroyNameList()")

  int MaxNames_ = TypeInfo::TYPEID_TOTAL;
  int Index_;

  if (TypeInfo::_Names)
  {
    for (Index_ = 0; Index_ < MaxNames_; ++Index_)
      delete TypeInfo::_Names[Index_];

    delete TypeInfo::_Names;
    TypeInfo::_Names = NULL;
  }

  DSDEFSMARKER("end: DestroyNameList()")
  DSDEFSTRACE(TypeInfo::_Names)
}

/****************************************************************************/
void TypeInfo::BuildNameList()
{
  DSDEFSMARKER("start: TypeInfo::BuildNameList()")

  _Names = new char*[TYPEID_TOTAL];

  _Names[NOTYPE] = NewString("No Type");
  _Names[UNDEFINED] = NewString("Undefined Type");
  _Names[EXTERNALCLASS] = NewString("ExternalClass");
  _Names[VOID_POINTER] = NewString("void*");
  _Names[DOUBLE_VOID_POINTER] = NewString("void**");
  _Names[TRIPLE_VOID_POINTER] = NewString("void***");
  _Names[NULL_POINTER] = NewString("NULL");
#if HAS_BOOL
  _Names[BUILTIN_BOOL] = NewString("bool");
#endif
  _Names[BUILTIN_CHAR] = NewString("char");
  _Names[BUILTIN_UNSIGNED_CHAR] = NewString("unsigned char");
  _Names[BUILTIN_INT] = NewString("int");
  _Names[BUILTIN_UNSIGNED_INT] = NewString("unsigned int");
  _Names[BUILTIN_SHORT] = NewString("short");
  _Names[BUILTIN_UNSIGNED_SHORT] = NewString("unsigned short");
  _Names[BUILTIN_LONG] = NewString("long");
  _Names[BUILTIN_UNSIGNED_LONG] = NewString("unsigned long");
  _Names[BUILTIN_FLOAT] = NewString("float");
  _Names[BUILTIN_DOUBLE] = NewString("double");
  _Names[BUILTIN_LONG_DOUBLE] = NewString("long double");
  _Names[OBJECT_TYPE] = NewString("Object");
  _Names[OBJECTIMP_TYPE] = NewString("ObjectImp");
  _Names[NULLOBJECT_TYPE] = NewString("NullObject");
  _Names[OBJECTIOFORMATTER_TYPE] = NewString("Object IO Formatter");
  _Names[ADVFORLOOP_TYPE] = NewString("AdvForLoop");
  _Names[INTEGER_TYPE] = NewString("Integer");
  _Names[UINTEGER_TYPE] = NewString("UInteger");
  _Names[SHORTINT_TYPE] = NewString("ShortInt");
  _Names[USHORTINT_TYPE] = NewString("UShortInt");
  _Names[LONGINT_TYPE] = NewString("LongInt");
  _Names[ULONGINT_TYPE] = NewString("ULongInt");
  _Names[FLOAT_TYPE] = NewString("Float");
  _Names[DOUBLE_TYPE] = NewString("Double");
  _Names[LONGDOUBLE_TYPE] = NewString("LongDouble");
  _Names[CHARACTER_TYPE] = NewString("Character");
  _Names[COMPLEXNUMBER_TYPE] = NewString("ComplexNumber");
  _Names[LONGNUMBER_TYPE] = NewString("LongNumber");
  _Names[STRING_TYPE] = NewString("ChrString");
  _Names[STRINGBROWSER_TYPE] = NewString("ChrStringBrowser");
  _Names[LONESTRING_TYPE] = NewString("LoneString");
  _Names[BYTESTRING_TYPE] = NewString("ByteString");
  _Names[STREAMREADER_TYPE] = NewString("StreamReader");
  _Names[FILEREADER_TYPE] = NewString("FileReader");
  _Names[FILE_TYPE] = NewString("File");
  _Names[SLOTTEDFILE_TYPE] = NewString("SlottedFile");
  _Names[VARLENGTHFILE_TYPE] = NewString("VarLengthFile");
  _Names[INIFILE_TYPE] = NewString("IniFile");
  _Names[BITS_TYPE] = NewString("Bits");
  _Names[BITSTRING_TYPE] = NewString("BitString");
  _Names[ARGARRAY_TYPE] = NewString("ArgArray");
  _Names[PTRARRAY_TYPE] = NewString("PtrArray");
  _Names[DYNARRAY_TYPE] = NewString("DynArray");
  _Names[PTRDYNARRAY_TYPE] = NewString("PtrDynArray");
  _Names[HASHABLE_TYPE] = NewString("Hashable");
  _Names[ASSOCIATION_TYPE] = NewString("Association");
  _Names[REFPTR_TYPE] = NewString("RefPtr");
  _Names[MEMPTR_TYPE] = NewString("MemPtr");
  _Names[STATICPTR_TYPE] = NewString("StaticPtr");
  _Names[OBJECTUPDATER_TYPE] = NewString("ObjectUpdater");
  _Names[BUILTINDELETER_TYPE] = NewString("BuiltInDeleter");
  _Names[BUILTINUPDATER_TYPE] = NewString("BuiltInUpdater");
  _Names[TYPEDOBJECTDELETER_TYPE] = NewString("TypedObjectDeleter");
  _Names[TYPEDOBJECTUPDATER_TYPE] = NewString("TypedObjectUpdater");
  _Names[UPDATERBINDINGLIST_TYPE] = NewString("UpdaterBindingList");
  _Names[DELETERBINDINGLIST_TYPE] = NewString("DeleterBindingList");
  _Names[CONSTCOPIEDVOIDPTR_TYPE] = NewString("ConstCopiedVoidPtr");
  _Names[CONSTCOPIEDBUILTINPTR_TYPE] = NewString("ConstCopiedBuiltInPtr");
  _Names[CONSTCOPIEDOBJPTR_TYPE] = NewString("ConstCopiedObjPtr");
  _Names[CONSTCOUNTEDVOIDPTR_TYPE] = NewString("ConstCountedVoidPtr");
  _Names[CONSTCOUNTEDBUILTINPTR_TYPE] = NewString("ConstCountedBuiltInPtr");
  _Names[CONSTCOUNTEDOBJPTR_TYPE] = NewString("ConstCountedObjPtr");
  _Names[CONSTUPDATEPTR_TYPE] = NewString("ConstUpdatePtr");
  _Names[CONSTARRAYPTR_TYPE] = NewString("ConstArrayPtr");
  _Names[CONSTSTATICOBJPTR_TYPE] = NewString("ConstStaticPtr");
  _Names[COPIEDVOIDPTR_TYPE] = NewString("CopiedVoidPtr");
  _Names[COPIEDBUILTINPTR_TYPE] = NewString("CopiedBuiltInPtr");
  _Names[COPIEDOBJPTR_TYPE] = NewString("CopiedObjPtr");
  _Names[COUNTEDVOIDPTR_TYPE] = NewString("CountedVoidPtr");
  _Names[COUNTEDBUILTINPTR_TYPE] = NewString("CountedBuiltInPtr");
  _Names[COUNTEDOBJPTR_TYPE] = NewString("CountedObjPtr");
  _Names[UPDATEPTR_TYPE] = NewString("UpdatePtr");
  _Names[ARRAYPTR_TYPE] = NewString("ArrayPtr");
  _Names[STATICOBJPTR_TYPE] = NewString("StaticObjPtr");
  _Names[CONSTPROXYPTRIMP_TYPE] = NewString("ConstProxyPtrImp");
  _Names[CONSTPROXYPOINTERPTR_TYPE] = NewString("ConstProxyPointerPtr");
  _Names[CONSTPROXYOBJECTPTR_TYPE] = NewString("ConstProxyObjectPtr");
  _Names[PROXYPTRIMP_TYPE] = NewString("ProxyPtrImp");
  _Names[PROXYPOINTERPTR_TYPE] = NewString("ProxyPointerPtr");
  _Names[PROXYOBJECTPTR_TYPE] = NewString("ProxyObjectPtr");
  _Names[FORWARD_ITERATOR_TYPE] = NewString("ForwardIterator");
  _Names[REVERSE_ITERATOR_TYPE] = NewString("ReverseIterator");
  _Names[BIDIRECTIONAL_ITERATOR_TYPE] = NewString("BidirectionalIterator");
  _Names[REVERSE_BIDIRECTIONAL_ITERATOR_TYPE] = NewString("ReverseBidirectionalAccessIterator");
  _Names[RANDOMACCESS_ITERATOR_TYPE] = NewString("RandomAccessIterator");
  _Names[REVERSE_RANDOMACCESS_ITERATOR_TYPE] = NewString("ReverseRandomAccessIterator");
  _Names[ARGARRAYITERATOR_TYPE] = NewString("ArgArrayIterator");
  _Names[PTRARRAYITERATOR_TYPE] = NewString("PtrArrayIterator");
  _Names[DYNARRAYITERATOR_TYPE] = NewString("DynArrayIterator");
  _Names[PTRDYNARRAYITERATOR_TYPE] = NewString("PtrDynArrayIterator");
  _Names[DOUBLELISTITERATOR_TYPE] = NewString("DoubleListIterator");
  _Names[SINGLELISTITERATOR_TYPE] = NewString("SingleListIterator");
  _Names[IMAGEPTR_TYPE] = NewString("ImagePtr");
  _Names[IMAGESTACKPTR_TYPE] = NewString("ImageStackPtr");
  _Names[PTRIMAGE_TYPE] = NewString("PtrImage");
  _Names[CONSTTEMPOBJECT_TYPE] = NewString("ConstTempObject");
  _Names[TEMPOBJECT_TYPE] = NewString("TempObject");
  _Names[BITVECTOR_TYPE] = NewString("BitVector");
  _Names[DATASTOREPTR_TYPE] = NewString("DataStorePtr");
  _Names[ARRAYSTOREPTR_TYPE] = NewString("ArrayStorePtr");
  _Names[STRINGSTOREPTR_TYPE] = NewString("StringStorePtr");
  _Names[SIMPLESTRING_TYPE] = NewString("SimpleString");
  _Names[DOUBLELIST_TYPE] = NewString("DoubleList");
  _Names[DOUBLELISTNODE_TYPE] = NewString("DoubleListNode");
  _Names[SINGLELIST_TYPE] = NewString("SingleList");
  _Names[SINGLELISTNODE_TYPE] = NewString("SingleListNode");
  _Names[STACK_TYPE] = NewString("Stack");
  _Names[QUEUE_TYPE] = NewString("Queue");
  _Names[DEQUE_TYPE] = NewString("Deque");
  _Names[SORTEDLIST_TYPE] = NewString("SortedList");
  _Names[PRIORITYQUEUE_TYPE] = NewString("PriorityQueue");  
  _Names[SET_TYPE] = NewString("Set");
  _Names[FUZZYSET_TYPE] = NewString("FuzzySet");
  _Names[NUMERICARRAY_TYPE] = NewString("NumericArray");
  _Names[STATARRAY_TYPE] = NewString("StatArray");
  _Names[INTEGERARRAY_TYPE] = NewString("IntegerArray");
  _Names[BST_TYPE] = NewString("Bst");
  _Names[AVLTREE_TYPE] = NewString("AvlTree");
  _Names[GRID_TYPE] = NewString("Grid");
  _Names[MATRIX_TYPE] = NewString("Matrix");  
  _Names[POLYNOMIAL_TYPE] = NewString("Polynomial");
  _Names[ARRAYBINTREE_TYPE] = NewString("ArrayBinTree");
  _Names[BTREE_TYPE] = NewString("Btree");
  _Names[CHARSET_TYPE] = NewString("CharSet");
  _Names[YEAR_TYPE] = NewString("Year");
  _Names[YMONTH_TYPE] = NewString("YMonth");
  _Names[DATE_TYPE] = NewString("Date");
  _Names[TIMEOFDAY_TYPE] = NewString("TimeOfDay");
  _Names[RATIONAL_TYPE] = NewString("Rational");
  _Names[POINT_TYPE] = NewString("Point");
  _Names[RECTANGLE_TYPE] = NewString("Rectangle");

  _Initialized = 1;
  atexit(DestroyNameList);

  DSDEFSMARKER("end: TypeInfo::BuildNameList()")
  DSDEFSTRACE(TypeInfo::_Names)
}

/****************************************************************************/
const char* TypeInfo::GiveClassName(int ClassId_)
{
  if (!_Names || !_Initialized)
    BuildNameList();

  return _Names[ClassId_];
}

/****************************************************************************/
#endif




