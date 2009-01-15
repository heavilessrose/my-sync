#ifndef GENDECLS_H
#define GENDECLS_H

/****************************************************************************/
#if OVERLOAD_NEW & HAS_ARRAY_NEW
  #define MEMORYOPS_DECL(Type) \
    void* operator new (size_t Bytes_);       \
    void operator delete (void* Space_);      \
    void* operator new[] (size_t Bytes_);     \
    void operator delete[] (void* Space_);    
  #define MEMORYOPS_DEFN(Type) \
    void* Type::operator new (size_t Bytes_)       \
    {                                              \
      return MemMatrix::Matrix().Allocate(Bytes_); \
    }                                              \
    void Type::operator delete (void* Space_)      \
    {                                              \
      MemMatrix::Matrix().Deallocate(Space_);      \
    }                                              \
    void* Type::operator new[] (size_t Bytes_)     \
    {                                              \
      return MemMatrix::Matrix().Allocate(Bytes_); \
    }                                              \
    void Type::operator delete[] (void* Space_)    \
    {                                              \
      MemMatrix::Matrix().Deallocate(Space_);      \
    }
  #define MEMORYOPS_TEMPLATE_DEFN(Type) \
    template <class T>                             \
    void* Type<T>::operator new (size_t Bytes_)    \
    {                                              \
      return MemMatrix::Matrix().Allocate(Bytes_); \
    }                                              \
    template <class T>                             \
    void Type<T>::operator delete (void* Space_)   \
    {                                              \
      MemMatrix::Matrix().Deallocate(Space_);      \
    }                                              \
    template <class T>                             \
    void* Type<T>::operator new[] (size_t Bytes_)  \
    {                                              \
      return MemMatrix::Matrix().Allocate(Bytes_); \
    }                                              \
    template <class T>                             \
    void Type<T>::operator delete[] (void* Space_) \
    {                                              \
      MemMatrix::Matrix().Deallocate(Space_);      \
    }    
#else
  #if OVERLOAD_NEW
    #define MEMORYOPS_DECL(Type) \
      void* operator new (size_t Bytes_);       \
      void operator delete (void* Space_);
    #define MEMORYOPS_DEFN(Type) \
      void* Type::operator new (size_t Bytes_)       \
      {                                              \
        return MemMatrix::Matrix().Allocate(Bytes_); \
      }                                              \
      void Type::operator delete (void* Space_)      \
      {                                              \
        MemMatrix::Matrix().Deallocate(Space_);      \
      }
    #define MEMORYOPS_TEMPLATE_DEFN(Type) \
      template <class T>                             \
      void* Type<T>::operator new (size_t Bytes_)    \
      {                                              \
        return MemMatrix::Matrix().Allocate(Bytes_); \
      }                                              \
      template <class T>                             \
      void Type<T>::operator delete (void* Space_)   \
      {                                              \
        MemMatrix::Matrix().Deallocate(Space_);      \
      }      
  #else
    #define MEMORYOPS_DECL(Type)
    #define MEMORYOPS_DEFN(Type)
    #define MEMORYOPS_TEMPLATE_DEFN(Type)
  #endif
#endif

/****************************************************************************/
#define ROOTOBJECTFNCS_DEFN(Root, Type)			\
friend Root& RootObject(Type& Obj_)                     \
{ return *((Root*)(&Obj_)); }                           \
friend const Root& RootConstObject(const Type& Obj_)    \
{ return *((const Root*)(&Obj_)); }

#define ROOTOBJECTFNCS_STUB(Root, Type)			\
friend Root& RootObject(Type& Obj_)			\
{ return Obj_; }                        		\
friend const Root& RootConstObject(const Type& Obj_)    \
{ return Obj_; }

#define EXTERNALCLASSFNCS_STUB(Root, Type)		\
friend Root& RootObject(Type& Obj_)			\
{ return Obj_; }                        		\
friend const Root& RootConstObject(const Type& Obj_)    \
{ return Obj_; }                                        \
friend int _WhatIs(const Type& Obj_)	                \
{ return TypeInfo::EXTERNALCLASS; }

/****************************************************************************/
#define WHATISFNC_DEFN(Type)		                \
friend int _WhatIs(const Type& Obj_)	                \
{ return Obj_.ClassID(); }

#define WHATISFNC_STUB(Type)		                \
friend int _WhatIs(const Type& Obj_)	                \
{ return TypeInfo::EXTERNALCLASS; }

#define WHATISFNC_TEMPLATE_DEFN(Type)		        \
template <class T>                                      \
int WhatIs(const Type<T>& Obj_)	                        \
{ return Obj_.ClassID(); }

/****************************************************************************/
#if defined(__TURBOC__) | defined (__BORLANDC__)
#define IOFNCS_DEFN(Type)							\
friend ostream& Write(ostream& os, const Type& obj, Boolean* Ok_=NULL)		\
{ return obj.BinaryWrite(os, Ok_); }                                  		\
friend istream& Ignore(istream& is, Type& obj, Boolean* Ok_=NULL)             	\
{ return obj.BinaryIgnore(is, Ok_); }                                  		\
friend istream& Read(istream& is, Type& obj, Boolean* Ok_=NULL)               	\
{ return obj.BinaryRead(is, Ok_); }                                    		\
friend ByteStream& Write(ByteStream& os, const Type& obj, Boolean* Ok_=NULL)	\
{ return obj.BinaryWrite(os, Ok_); }                                            \
friend ByteStream& Ignore(ByteStream& is, Type& obj, Boolean* Ok_=NULL)         \
{ return obj.BinaryIgnore(is, Ok_); }                                           \
friend ByteStream& Read(ByteStream& is, Type& obj, Boolean* Ok_=NULL)           \
{ return obj.BinaryRead(is, Ok_); }
#else
#define IOFNCS_DEFN(Type)							\
friend ostream& _Write(ostream& os, const Type& obj, Boolean* Ok_)          	\
{ return obj.BinaryWrite(os, Ok_); }                                   		\
friend istream& _Ignore(istream& is, Type& obj, Boolean* Ok_)               	\
{ return obj.BinaryIgnore(is, Ok_); }                                  		\
friend istream& _Read(istream& is, Type& obj, Boolean* Ok_)                 	\
{ return obj.BinaryRead(is, Ok_); }                                    		\
friend ByteStream& _Write(ByteStream& os, const Type& obj, Boolean* Ok_)    	\
{ return obj.BinaryWrite(os, Ok_); }                                   		\
friend ByteStream& _Ignore(ByteStream& is, Type& obj, Boolean* Ok_)         	\
{ return obj.BinaryIgnore(is, Ok_); }                                  		\
friend ByteStream& _Read(ByteStream& is, Type& obj, Boolean* Ok_)           	\
{ return obj.BinaryRead(is, Ok_); }
#endif

#if defined(__TURBOC__) | defined (__BORLANDC__)
#define IOFNCS_STUB(Type)							\
friend ostream& Write(ostream& os, const Type& obj, Boolean* Ok_=NULL)		\
{ return os; }                                  				\
friend istream& Ignore(istream& is, Type& obj, Boolean* Ok_=NULL)             	\
{ return is; }                                                                  \
friend istream& Read(istream& is, Type& obj, Boolean* Ok_=NULL)               	\
{ return is; }                                                                  \
friend ByteStream& Write(ByteStream& os, const Type& obj, Boolean* Ok_=NULL)	\
{ return os; }                                                                  \
friend ByteStream& Ignore(ByteStream& is, Type& obj, Boolean* Ok_=NULL)         \
{ return is; }                                                                  \
friend ByteStream& Read(ByteStream& is, Type& obj, Boolean* Ok_=NULL)           \
{ return is; }
#else
#define IOFNCS_STUB(Type)							\
friend ostream& _Write(ostream& os, const Type& obj, Boolean* Ok_=NULL)		\
{ return os; }                                  				\
friend istream& _Ignore(istream& is, Type& obj, Boolean* Ok_=NULL)             	\
{ return is; }                                                                  \
friend istream& _Read(istream& is, Type& obj, Boolean* Ok_=NULL)               	\
{ return is; }                                                                  \
friend ByteStream& _Write(ByteStream& os, const Type& obj, Boolean* Ok_=NULL)	\
{ return os; }                                                                  \
friend ByteStream& _Ignore(ByteStream& is, Type& obj, Boolean* Ok_=NULL)        \
{ return is; }                                                                  \
friend ByteStream& _Read(ByteStream& is, Type& obj, Boolean* Ok_=NULL)          \
{ return is; }
#endif

#define IOFNCS_INLINE_DEFN(Type)                                                \
_FNC_INLINE ostream& _Write(ostream& os, const Type& obj, Boolean* Ok_=NULL)    \
{ os.write((const char*)(&obj), SizeOfData(obj));                               \
  if (Ok_)                                                                      \
    *Ok_ = os.good();                                                           \
  return os; }                                                                  \
_FNC_INLINE istream& _Ignore(istream& is, Type& obj, Boolean* Ok_=NULL)         \
{ Type* ObjPtr_ = new Type(obj);                                                \
  if (ObjPtr_)                                                                  \
  {                                                                             \
    istream& StrmRef_ = ::Read(is, *ObjPtr_, Ok_);                              \
    delete ObjPtr_;                                                             \
    return StrmRef_;                                                            \
  }                                                                             \
  return is; }                                                                  \
_FNC_INLINE istream& _Read(istream& is, Type& obj, Boolean* Ok_=NULL)           \
{ is.read((char*)(&obj), SizeOfData(obj));                                      \
  if (Ok_)                                                                      \
    *Ok_ = is.good();                                                           \
  return is; }                                                                  \
_FNC_INLINE ByteStream& _Write(ByteStream& os, const Type& obj, Boolean* Ok_=NULL)  \
{ os.owrite((const char*)(&obj), SizeOfData(obj));                              \
  if (Ok_)                                                                      \
    *Ok_ = os.GetOutStream().good();                                            \
  return os; }                                                                  \
_FNC_INLINE ByteStream& _Ignore(ByteStream& is, Type& obj, Boolean* Ok_=NULL)   \
{ Type* ObjPtr_ = new Type(obj);                                                \
  if (ObjPtr_)                                                                  \
  {                                                                             \
    ByteStream& StrmRef_ = ::Read(is, *ObjPtr_, Ok_);                           \
    delete ObjPtr_;                                                             \
    return StrmRef_;                                                            \
  }                                                                             \
  return is; }                                                                  \
_FNC_INLINE ByteStream& _Read(ByteStream& is, Type& obj, Boolean* Ok_=NULL)     \
{ is.iread((char*)(&obj), SizeOfData(obj));                                     \
  if (Ok_)                                                                      \
    *Ok_ = is.GetInStream().good();                                             \
  return is; }

#define IOFNCS_TEMPLATE_DEFN(Type)                                              \
template <class T>                                      			\
ostream& Write(ostream& os, const Type<T>& obj, Boolean* Ok_=NULL)              \
{ os.write((const char*)(&obj), SizeOfData(obj));                               \
  if (Ok_)                                                                      \
    *Ok_ = os.good();                                                           \
  return os; }                                                                  \
template <class T>                                      			\
istream& Ignore(istream& is, Type<T>& obj, Boolean* Ok_=NULL)                   \
{ Type<T>* ObjPtr_ = new Type<T>(obj);                                          \
  if (ObjPtr_)                                                                  \
  {                                                                             \
    istream& StrmRef_ = ::Read(is, *ObjPtr_, Ok_);                              \
    delete ObjPtr_;                                                             \
    return StrmRef_;                                                            \
  }                                                                             \
  return is; }                                                                  \
template <class T>                                      			\
istream& Read(istream& is, Type<T>& obj, Boolean* Ok_=NULL)                     \
{ is.read((char*)(&obj), SizeOfData(obj));                                      \
  if (Ok_)                                                                      \
    *Ok_ = is.good();                                                           \
  return is; }                                                                  \
template <class T>                                      			\
ByteStream& Write(ByteStream& os, const Type<T>& obj, Boolean* Ok_=NULL)        \
{ os.owrite((const char*)(&obj), SizeOfData(obj));                              \
  if (Ok_)                                                                      \
    *Ok_ = os.GetOutStream().good();                                            \
  return os; }                                                                  \
template <class T>                                      			\
ByteStream& Ignore(ByteStream& is, Type<T>& obj, Boolean* Ok_=NULL)             \
{ Type<T>* ObjPtr_ = new Type<T>(obj);                                          \
  if (ObjPtr_)                                                                  \
  {                                                                             \
    ByteStream& StrmRef_ = ::Read(is, *ObjPtr_, Ok_);                           \
    delete ObjPtr_;                                                             \
    return StrmRef_;                                                            \
  }                                                                             \
  return is; }                                                                  \
template <class T>                                      			\
ByteStream& Read(ByteStream& is, Type<T>& obj, Boolean* Ok_=NULL)               \
{ is.iread((char*)(&obj), SizeOfData(obj));                                     \
  if (Ok_)                                                                      \
    *Ok_ = is.GetInStream().good();                                             \
  return is; }

/****************************************************************************/
#define CLONEFNCS_DEFN(Type)				\
friend Type* _Clone(const Type& Obj_)                   \
{ return ((Type*)Obj_.Clone()); }                       \
friend const Type* _ConstClone(const Type& Obj_)        \
{ return ((const Type*)Obj_.ShallowClone()); }          \
friend Type* _NonConstClone(Type& Obj_)                 \
{ return ((Type*)Obj_.ShallowClone()); }

#define CLONEFNCS_STUB(Type)				\
friend Type* _Clone(const Type& Obj_)                   \
{ return NULL; }                       			\
friend const Type* _ConstClone(const Type& Obj_)        \
{ return &Obj_; }          				\
friend Type* _NonConstClone(Type& Obj_)                 \
{ return &Obj_; }

#define CLONEFNCS_INLINE_DEFN(Type)          		\
_FNC_INLINE Type* _Clone(const Type& Obj_)     		\
{ return (new Type(Obj_)); }                   		\
_FNC_INLINE const Type* _ConstClone(const Type& Obj_)   \
{ return &Obj_; }                                       \
_FNC_INLINE Type* _NonConstClone(Type& Obj_)    	\
{ return &Obj_; }

#define CLONEFNCS_TEMPLATE_DEFN(Type)          		\
template <class T>                                      \
Type<T>* Clone(const Type<T>& Obj_)			\
{ return (new Type<T>(Obj_)); }             		\
template <class T>                                      \
const Type<T>* ConstClone(const Type<T>& Obj_)    	\
{ return &Obj_; }                                       \
template <class T>                                      \
Type<T>* NonConstClone(Type<T>& Obj_)    		\
{ return &Obj_; }

/****************************************************************************/
#define SWAPFNC_DEFN(Type)		\
friend void _Swap(Type& x, Type& y)     \
{ Type* temp = (Type*)x.Clone();        \
  x.Assign(y);                          \
  y.Assign(*temp);                      \
  delete temp; }

#define SWAPFNC_STUB(Type)		\
friend void _Swap(Type& x, Type& y)     \
{}

#define SWAPFNC_INLINE_DEFN(Type)		\
_FNC_INLINE void _Swap(Type& x, Type& y)        \
{ Type Temp_ = x;                               \
  x = y;                                        \
  y = Temp_; }

#define SWAPFNC_TEMPLATE_DEFN(Type) 		\
template <class T>                              \
void Swap(Type<T>& x, Type<T>& y)               \
{ Type<T> Temp_ = x;                            \
  x = y;                                        \
  y = Temp_; }

/****************************************************************************/
#define SIZEOFDATAFNC_DEFN(Type)			\
friend long _SizeOfData(const Type& Obj_)		\
{ return Obj_.DataSize(); }

#define SIZEOFDATAFNC_STUB(Type)			\
friend long _SizeOfData(const Type& Obj_)		\
{ return sizeof(Obj_); }

#define SIZEOFDATAFNC_INLINE_DEFN(Type)		        \
_FNC_INLINE long _SizeOfData(const Type& Obj_)		\
{ return sizeof(Obj_); }

#define SIZEOFDATAFNC_TEMPLATE_DEFN(Type)		\
template <class T>                              	\
long SizeOfData(const Type<T>& Obj_)			\
{ return sizeof(Obj_); }

/****************************************************************************/
#define FORMWIDTHOFOBJECTFNC_DEFN(Type)			\
friend long _FormWidthOfObject(const Type& Obj_)	\
{ return Obj_.StorableFormWidth(); }

#define FORMWIDTHOFOBJECTFNC_STUB(Type)			\
friend long _FormWidthOfObject(const Type& Obj_)	\
{ return (long(sizeof(long)) + long(sizeof(Obj_))); }

#define FORMWIDTHOFOBJECTFNC_INLINE_DEFN(Type)		\
_FNC_INLINE long _FormWidthOfObject(const Type& Obj_)	\
{ return (long(sizeof(long)) + long(sizeof(Obj_))); }

#define FORMWIDTHOFOBJECTFNC_TEMPLATE_DEFN(Type)	\
template <class T>                                      \
long FormWidthOfObject(const Type<T>& Obj_)		\
{ return (long(sizeof(long)) + long(sizeof(Obj_))); }

/****************************************************************************/
#define ASSIGNFNC_DEFN(Type)				\
friend Type& _Assign(Type& trg, const Type& src)	\
{ trg.Assign(src);                                      \
  return trg; }

#define ASSIGNFNC_STUB(Type)				\
friend Type& _Assign(Type& trg, const Type& src)	\
{ return trg; }

#define ASSIGNFNC_INLINE_DEFN(Type)			\
_FNC_INLINE Type& _Assign(Type& trg, const Type& src)	\
{ trg = src;                                            \
  return trg; }

#define ASSIGNFNC_TEMPLATE_DEFN(Type)			\
template <class T>                                      \
Type<T>& Assign(Type<T>& trg, const Type<T>& src)	\
{ trg = src;                                            \
  return trg; }

/****************************************************************************/
#define NONCONSTASSIGNFNC_DEFN(Type)			\
friend Type& _NonConstAssign(Type& Trg_, Type& Src_)	\
{ Trg_.ShallowAssign(Src_);                             \
  return Trg_; }

#define NONCONSTASSIGNFNC_STUB(Type)			\
friend Type& _NonConstAssign(Type& Trg_, Type& Src_)	\
{ return Trg_; }

#define NONCONSTASSIGNFNC_INLINE_DEFN(Type)			\
_FNC_INLINE Type& _NonConstAssign(Type& Trg_, Type& Src_)	\
{ Trg_ = Src_;                             			\
  return Trg_; }

#define NONCONSTASSIGNFNC_TEMPLATE_DEFN(Type)		\
template <class T>                                      \
Type<T>& NonConstAssign(Type<T>& Trg_, Type<T>& Src_)	\
{ Trg_ = Src_;                             		\
  return Trg_; }

/****************************************************************************/
#define COMPARISONFNCS_DEFN(Type)                                        \
friend Boolean _IsGreaterThan(const Type& Trg_, const Type& Src_)        \
{ return Trg_.IsGreater(Src_); }                                         \
friend Boolean _IsLessThan(const Type& Trg_, const Type& Src_)           \
{ return Trg_.IsLesser(Src_); }                                          \
friend Boolean _IsEqualTo(const Type& Trg_, const Type& Src_)            \
{ return Trg_.IsEqual(Src_); }

#define COMPARISONFNCS_STUB(Type)                                        \
friend Boolean _IsGreaterThan(const Type& Trg_, const Type& Src_)        \
{ return FALSE; }                                                        \
friend Boolean _IsLessThan(const Type& Trg_, const Type& Src_)           \
{ return FALSE; }                                                        \
friend Boolean _IsEqualTo(const Type& Trg_, const Type& Src_)            \
{ return FALSE; }

#define COMPARISONFNCS_INLINE_DEFN(Type)                                        \
_FNC_INLINE Boolean _IsGreaterThan(const Type& Trg_, const Type& Src_)          \
{ return (Trg_ > Src_); }                                                       \
_FNC_INLINE Boolean _IsLessThan(const Type& Trg_, const Type& Src_)             \
{ return (Trg_ < Src_); }                                                       \
_FNC_INLINE Boolean _IsEqualTo(const Type& Trg_, const Type& Src_)              \
{ return (Trg_ == Src_); }

#define COMPARISONFNCS_TEMPLATE_DEFN(Type)                                      \
template <class T>                                                              \
Boolean IsGreaterThan(const Type<T>& Trg_, const Type<T>& Src_)                 \
{ return Trg_.IsGreater(Src_); }                                                \
template <class T>                                                              \
Boolean IsLessThan(const Type<T>& Trg_, const Type<T>& Src_)                    \
{ return Trg_.IsLesser(Src_); }                                                 \
template <class T>                                                              \
Boolean IsEqualTo(const Type<T>& Trg_, const Type<T>& Src_)                     \
{ return Trg_.IsEqual(Src_); }

/****************************************************************************/
#define REDIRECTFNCS_DEFN(Type)                                          \
friend ostream& _WriteToOutput(ostream& os, const Type& Obj_)            \
{ os <<Obj_; return os; }                                                \
friend istream& _ReadFromInput(istream& is, Type& Obj_)                  \
{ is >>Obj_; return is; }

#define REDIRECTFNCS_STUB(Type)                                          \
friend ostream& _WriteToOutput(ostream& os, const Type& Obj_)            \
{ return os; }                                                           \
friend istream& _ReadFromInput(istream& is, Type& Obj_)                  \
{ return is; }

#define REDIRECTFNCS_INLINE_DEFN(Type)                                     \
_FNC_INLINE ostream& _WriteToOutput(ostream& os, const Type& Obj_)         \
{ os <<Obj_; return os; }                                                  \
_FNC_INLINE istream& _ReadFromInput(istream& is, Type& Obj_)               \
{ is >>Obj_; return is; }

#define REDIRECTFNCS_TEMPLATE_DEFN(Type)                                 \
template <class T>                                                       \
ostream& WriteToOutput(ostream& os, const Type<T>& Obj_)                 \
{ os <<Obj_; return os; }                                                \
template <class T>                                                       \
istream& ReadFromInput(istream& is, Type<T>& Obj_)                       \
{ is >>Obj_; return is; }

/****************************************************************************/
#define COUNTOFFNC_DEFN(Type)                                            \
friend size_t _CountOf(const Type& Obj_, Boolean* Determinable_)         \
{ if (Determinable_)                                                     \
    *Determinable_ = TRUE;                                               \
  return Obj_.ObjectID(); }

#define COUNTOFFNC_STUB(Type)                                            \
friend size_t _CountOf(const Type& Obj_, Boolean* Determinable_)         \
{ if (Determinable_)                                                     \
    *Determinable_ = FALSE;                                              \
  return 0; }

#define COUNTOFFNC_INLINE_DEFN(Type)                                     \
_FNC_INLINE size_t _CountOf(const Type& Obj_, Boolean* Determinable_)    \
{ if (Determinable_)                                                     \
    *Determinable_ = FALSE;                                              \
  return 0; }  

#define COUNTOFFNC_TEMPLATE_DEFN(Type)                                   \
template <class T>                                                       \
size_t CountOf(const Type<T>& Obj_, Boolean* Determinable_)              \
{ if (Determinable_)                                                     \
    *Determinable_ = TRUE;                                               \
  return Obj_.ObjectID(); }  

/****************************************************************************/
#define ISNULLOBJECTFNC_DEFN(Type)                                       \
friend Boolean _IsNullObject(const Type& Obj_, Boolean* Determinable_)   \
{ if (Determinable_)                                                     \
    *Determinable_ = TRUE;                                               \
  return Obj_.IsNullObject(); }

#define ISNULLOBJECTFNC_STUB(Type)                                       \
friend Boolean _IsNullObject(const Type& Obj_, Boolean* Determinable_)   \
{ if (Determinable_)                                                     \
    *Determinable_ = FALSE;                                              \
  return FALSE; }

#define ISNULLOBJECTFNC_INLINE_DEFN(Type)                                     \
_FNC_INLINE Boolean _IsNullObject(const Type& Obj_, Boolean* Determinable_)   \
{ if (Determinable_)                                                          \
    *Determinable_ = FALSE;                                                   \
  return FALSE; }

#define ISNULLOBJECTFNC_TEMPLATE_DEFN(Type)                                   \
template <class T>                                                            \
Boolean IsNullObject(const Type<T>& Obj_, Boolean* Determinable_)             \
{ if (Determinable_)                                                          \
    *Determinable_ = TRUE;                                                    \
  return Obj_.IsNullObject(); }

/****************************************************************************/
#define SHOWINFOFNC_DEFN(Type)                                           \
friend ostream& _ShowInfo(ostream& Os_, const Type& Obj_)                \
{ Os_ <<endl <<"Class Name  : " <<Obj_.ClassName()                       \
      <<endl <<"Class I.D.  : " <<Obj_.ClassID()                         \
      <<endl <<"Object I.D. : " <<Obj_.ObjectID();                       \
  return Os_; }

#define SHOWINFOFNC_STUB(Type)                                              \
friend ostream& _ShowInfo(ostream& Os_, const Type& Obj_)                   \
{ Os_ <<endl <<"Class Name  : " <<TypeInfo::GiveClassName(::WhatIs(Obj_))   \
      <<endl <<"Class I.D.  : " <<::WhatIs(Obj_)                            \
      <<endl <<"Object I.D. : Indeterminate";                               \
  return Os_; }

#define SHOWINFOFNC_INLINE_DEFN(Type)                                       \
_FNC_INLINE ostream& _ShowInfo(ostream& Os_, const Type& Obj_)              \
{ Os_ <<endl <<"Class Name  : " <<TypeInfo::GiveClassName(::WhatIs(Obj_))   \
      <<endl <<"Class I.D.  : " <<::WhatIs(Obj_)                            \
      <<endl <<"Object I.D. : " <<0;                                        \
  return Os_; }

#define SHOWINFOFNC_TEMPLATE_DEFN(Type)                                  \
template <class T>                                                       \
ostream& ShowInfo(ostream& Os_, const Type<T>& Obj_)                     \
{ Os_ <<endl <<"Class Name  : " <<Obj_.ClassName()                       \
      <<endl <<"Class I.D.  : " <<Obj_.ClassID()                         \
      <<endl <<"Object I.D. : " <<Obj_.ObjectID();                       \
  return Os_; }

/****************************************************************************/
#define HASHINGFNCS_DEFN(Type)                                           \
friend const char* _HashableData(const Type& Obj_)                       \
{ return Obj_.GiveHashableData(); }                                      \
friend long _HashableDataLength(const Type& Obj_)                        \
{ return Obj_.GiveHashableDataLength(); }

#define HASHINGFNCS_STUB(Type)                                           \
friend const char* _HashableData(const Type& Obj_)                       \
{ return ((char*)&Obj_); }                                               \
friend long _HashableDataLength(const Type& Obj_)                        \
{ return sizeof(Obj_); }

#define HASHINGFNCS_INLINE_DEFN(Type)                                    \
_FNC_INLINE const char* _HashableData(const Type& Obj_)                  \
{ return ((char*)&Obj_); }                                               \
_FNC_INLINE long _HashableDataLength(const Type& Obj_)                   \
{ return sizeof(Obj_); }

#define HASHINGFNCS_TEMPLATE_DEFN(Type)                                  \
template <class T>                                                       \
const char* _HashableData(const Type& Obj_)                              \
{ return Obj_.GiveHashableData(); }                                      \
template <class T>                                                       \
long _HashableDataLength(const Type& Obj_)                               \
{ return Obj_.GiveHashableDataLength(); }

/****************************************************************************/
#define OBJECTPROPERTIESFNCS_DEFN(Type)                                \
friend Boolean _IsSummable(const Type& Obj_)                           \
{ return Obj_.IsSummableType(); }                                      \
friend Boolean _IsMultipliable(const Type& Obj_)                       \
{ return Obj_.IsMultipliableType(); }                                  \
friend Boolean _IsOrdinal(const Type& Obj_)                            \
{ return Obj_.IsOrdinalType(); }                                       \
friend Boolean _IsInvertable(const Type& Obj_)                         \
{ return Obj_.IsInvertableType(); }                                    \
friend Boolean _IsLogical(const Type& Obj_)                            \
{ return Obj_.IsLogicalType(); }                                       \
friend Boolean _IsBitwiseable(const Type& Obj_)                        \
{ return Obj_.IsBitwiseableType(); }                                   \
friend Boolean _IsStreamable(const Type& Obj_)                         \
{ return Obj_.IsStreamableType(); }                                    \
friend Boolean _IsSortable(const Type& Obj_)                           \
{ return Obj_.IsSortableType(); }                                      \
friend Boolean _IsMatchable(const Type& Obj_)                          \
{ return Obj_.IsMatchableType(); }

#define OBJECTPROPERTIESFNCS_STUB(Type)                                \
friend Boolean _IsSummable(const Type& Obj_)                           \
{ return FALSE; }                                                      \
friend Boolean _IsMultipliable(const Type& Obj_)                       \
{ return FALSE; }                                                      \
friend Boolean _IsOrdinal(const Type& Obj_)                            \
{ return FALSE; }                                                      \
friend Boolean _IsInvertable(const Type& Obj_)                         \
{ return FALSE; }                                                      \
friend Boolean _IsLogical(const Type& Obj_)                            \
{ return FALSE; }                                                      \
friend Boolean _IsBitwiseable(const Type& Obj_)                        \
{ return FALSE; }                                                      \
friend Boolean _IsStreamable(const Type& Obj_)                         \
{ return FALSE; }                                                      \
friend Boolean _IsSortable(const Type& Obj_)                           \
{ return FALSE; }                                                      \
friend Boolean _IsMatchable(const Type& Obj_)                          \
{ return FALSE; }

#define OBJECTPROPERTIESFNCS_INLINE_DEFN(Type)                         \
_FNC_INLINE Boolean _IsSummable(const Type& Obj_)                      \
{ return Obj_.IsSummableType(); }                                      \
_FNC_INLINE Boolean _IsMultipliable(const Type& Obj_)                  \
{ return Obj_.IsMultipliableType(); }                                  \
_FNC_INLINE Boolean _IsOrdinal(const Type& Obj_)                       \
{ return Obj_.IsOrdinalType(); }                                       \
_FNC_INLINE Boolean _IsInvertable(const Type& Obj_)                    \
friend Boolean _IsInvertable(const Type& Obj_)                         \
_FNC_INLINE Boolean _IsLogical(const Type& Obj_)                       \
{ return Obj_.IsLogicalType(); }                                       \
_FNC_INLINE Boolean _IsBitwiseable(const Type& Obj_)                   \
{ return Obj_.IsBitwiseableType(); }                                   \
_FNC_INLINE Boolean _IsStreamable(const Type& Obj_)                    \
{ return Obj_.IsStreamableType(); }                                    \
_FNC_INLINE Boolean _IsSortable(const Type& Obj_)                      \
{ return Obj_.IsSortableType(); }                                      \
_FNC_INLINE Boolean _IsMatchable(const Type& Obj_)                     \
{ return Obj_.IsMatchableType(); }

#define OBJECTPROPERTIESFNCS_TEMPLATE_DEFN(Type)                         \
template <class T>                                                       \
Boolean IsSummable(const Type<T>& Obj_)                                  \
{ return Obj_.IsSummableType(); }                                        \
template <class T>                                                       \
Boolean IsMultipliable(const Type<T>& Obj_)                              \
{ return Obj_.IsMultipliableType(); }                                    \
template <class T>                                                       \
Boolean IsOrdinal(const Type<T>& Obj_)                                   \
{ return Obj_.IsOrdinalType(); }                                         \
template <class T>                                                       \
Boolean IsInvertable(const Type<T>& Obj_)                                \
{ return Obj_.IsInvertableType(); }                                      \
template <class T>                                                       \
Boolean IsLogical(const Type<T>& Obj_)                                   \
{ return Obj_.IsLogicalType(); }                                         \
template <class T>                                                       \
Boolean IsBitwiseable(const Type<T>& Obj_)                               \
{ return Obj_.IsBitwiseableType(); }                                     \
template <class T>                                                       \
Boolean IsStreamable(const Type<T>& Obj_)                                \
{ return Obj_.IsStreamableType(); }                                      \
template <class T>                                                       \
Boolean IsSortable(const Type<T>& Obj_)                                  \
{ return Obj_.IsSortableType(); }                                        \
template <class T>                                                       \
Boolean IsMatchable(const Type<T>& Obj_)                                 \
{ return Obj_.IsMatchableType(); }

/****************************************************************************/
#define ZEROVALUEFNCS_DEFN(Type)                                  \
friend Boolean _AtZero(const Type& Obj_)                          \
{ return ::IsNullObject(Obj_); }                                  \
friend Boolean _NotZero(const Type& Obj_)                         \
{ return !::IsNullObject(Obj_); }

#define ZEROVALUEFNCS_STUB(Type)                                  \
friend Boolean _AtZero(const Type& Obj_)                          \
{ return FALSE; }                                                 \
friend Boolean _NotZero(const Type& Obj_)                         \
{ return FALSE; }

#define ZEROVALUEFNCS_INLINE_DEFN(Type)                           \
_FNC_INLINE Boolean _AtZero(const Type& Obj_)                     \
{ return (Obj_ == ((Type)0)); }                                   \
_FNC_INLINE Boolean _NotZero(const Type& Obj_)                    \
{ return (Obj_ != ((Type)0)); }

#define ZEROVALUEFNCS_TEMPLATE_DEFN(Type)                         \
template <class T>                                                \
Boolean AtZero(const Type<T>& Obj_)                               \
{ return ::IsNullObject(Obj_); }                                  \
template <class T>                                                \
Boolean NotZero(const Type<T>& Obj_)                              \
{ return !::IsNullObject(Obj_); }

/****************************************************************************/
#define GENERIC_MINIMAL_FNC_DEFN(Type)  \
WHATISFNC_DEFN(Type)			\
IOFNCS_DEFN(Type)			\
CLONEFNCS_DEFN(Type)			\
SWAPFNC_DEFN(Type)			\
SIZEOFDATAFNC_DEFN(Type)		\
FORMWIDTHOFOBJECTFNC_DEFN(Type)		\
ASSIGNFNC_DEFN(Type)			\
NONCONSTASSIGNFNC_DEFN(Type)            \
COMPARISONFNCS_DEFN(Type)               \
REDIRECTFNCS_DEFN(Type)                 \
COUNTOFFNC_DEFN(Type)                   \
ISNULLOBJECTFNC_DEFN(Type)              \
SHOWINFOFNC_DEFN(Type)                  \
HASHINGFNCS_DEFN(Type)

#define GENERIC_FNC_DEFN(Type)          \
WHATISFNC_DEFN(Type)			\
IOFNCS_DEFN(Type)			\
CLONEFNCS_DEFN(Type)			\
SWAPFNC_DEFN(Type)			\
SIZEOFDATAFNC_DEFN(Type)		\
FORMWIDTHOFOBJECTFNC_DEFN(Type)		\
ASSIGNFNC_DEFN(Type)			\
NONCONSTASSIGNFNC_DEFN(Type)            \
COMPARISONFNCS_DEFN(Type)               \
REDIRECTFNCS_DEFN(Type)                 \
COUNTOFFNC_DEFN(Type)                   \
ISNULLOBJECTFNC_DEFN(Type)              \
SHOWINFOFNC_DEFN(Type)                  \
HASHINGFNCS_DEFN(Type)                  \
OBJECTPROPERTIESFNCS_DEFN(Type)         \
ZEROVALUEFNCS_DEFN(Type)

#define GENERIC_FNC_STUB(Type)          \
WHATISFNC_STUB(Type)			\
IOFNCS_STUB(Type)               	\
CLONEFNCS_STUB(Type)			\
SWAPFNC_STUB(Type)			\
SIZEOFDATAFNC_STUB(Type)		\
FORMWIDTHOFOBJECTFNC_STUB(Type)		\
ASSIGNFNC_STUB(Type)			\
NONCONSTASSIGNFNC_STUB(Type)            \
COMPARISONFNCS_DEFN(Type)               \
REDIRECTFNCS_DEFN(Type)                 \
COUNTOFFNC_DEFN(Type)                   \
ISNULLOBJECTFNC_DEFN(Type)              \
SHOWINFOFNC_DEFN(Type)                  \
HASHINGFNCS_DEFN(Type)                  \
OBJECTPROPERTIESFNCS_DEFN(Type)         \
ZEROVALUEFNCS_DEFN(Type)

#define GENERIC_FNC_INLINE_DEFN(Type)   \
IOFNCS_INLINE_DEFN(Type)                \
CLONEFNCS_INLINE_DEFN(Type)		\
SWAPFNC_INLINE_DEFN(Type)		\
SIZEOFDATAFNC_INLINE_DEFN(Type)	        \
FORMWIDTHOFOBJECTFNC_INLINE_DEFN(Type)	\
ASSIGNFNC_INLINE_DEFN(Type)		\
NONCONSTASSIGNFNC_INLINE_DEFN(Type)     \
COMPARISONFNCS_INLINE_DEFN(Type)        \
REDIRECTFNCS_INLINE_DEFN(Type)          \
COUNTOFFNC_INLINE_DEFN(Type)            \
ISNULLOBJECTFNC_INLINE_DEFN(Type)       \
SHOWINFOFNC_INLINE_DEFN(Type)           \
HASHINGFNCS_INLINE_DEFN(Type)           \
OBJECTPROPERTIESFNCS_INLINE_DEFN(Type)  \
ZEROVALUEFNCS_INLINE_DEFN(Type)

#define GENERIC_FNC_TEMPLATE_DEFN(Type)         \
WHATISFNC_TEMPLATE_DEFN(Type)                   \
IOFNCS_TEMPLATE_DEFN(Type)              	\
CLONEFNCS_TEMPLATE_DEFN(Type)			\
SWAPFNC_TEMPLATE_DEFN(Type)			\
SIZEOFDATAFNC_TEMPLATE_DEFN(Type)		\
FORMWIDTHOFOBJECTFNC_TEMPLATE_DEFN(Type)	\
ASSIGNFNC_TEMPLATE_DEFN(Type)			\
NONCONSTASSIGNFNC_TEMPLATE_DEFN(Type)           \
COMPARISONFNCS_TEMPLATE_DEFN(Type)              \
REDIRECTFNCS_TEMPLATE_DEFN(Type)                \
COUNTOFFNC_TEMPLATE_DEFN(Type)                  \
ISNULLOBJECTFNC_TEMPLATE_DEFN(Type)             \
SHOWINFOFNC_TEMPLATE_DEFN(Type)                 \
HASHINGFNCS_TEMPLATE_DEFN(Type)                 \
OBJECTPROPERTIESFNCS_TEMPLATE_DEFN(Type)        \
ZEROVALUEFNCS_TEMPLATE_DEFN(Type)

/****************************************************************************/
#endif




