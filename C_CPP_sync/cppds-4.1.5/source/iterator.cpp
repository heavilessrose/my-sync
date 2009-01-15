#ifndef ITERATOR_CPP
#define ITERATOR_CPP
#ifndef ITERATOR_H
  #include "iterator.h"
#endif

/****************************************************************************/
#define ITER_DISPATCH_FNC(Type) \
template <class T>              \
Type<T>* Type<T>::Dispatch()    \
{                               \
  if (_VarPtr == NULL)          \
    Xnull();                    \
  return _VarPtr;               \
}                               \
                                \
template <class T>              \
const Type<T>* Type<T>::Dispatch() const \
{                                        \
  if (_ObjPtr == NULL)                   \
    Xnull();                             \
  return _ObjPtr;                        \
}

#define ITER_DESTRUCTOR_FNC(Type) \
template <class T>                \
Type<T>::~Type()                  \
{                                 \
  if (this != _VarPtr)            \
  {                               \
    delete _VarPtr;               \
    _VarPtr = NULL;               \
  }                               \
}

#define ITER_CLASSID_FNC(Type, IDTAG) \
template <class T>                    \
int Type<T>::StaticID()               \
{                                     \
  return IDTAG;                       \
}                                     \
                                      \
template <class T>                    \
int Type<T>::ClassID() const          \
{                                     \
  return IDTAG;                       \
}

#define ITER_ASSIGNMENT_FNC(Type)     \
template <class T>                    \
Type<T>& Type<T>::operator = (const Type<T>& Rhs_) \
{                                                  \
  if (this != &Rhs_)                               \
  {                                                \
    delete _VarPtr;                                \
    _ObjPtr = Rhs_.GiveIterator().Clone();         \
  }                                                \
  return *this;                                    \
}                                                  \
                                                   \
template <class T>                                 \
Type<T>& Type<T>::operator = (T* Ptr_)             \
{                                                  \
  *_ObjPtr = Ptr_;                                 \
  return *this;                                    \
}


#define ITER_COMPARISON_FNC(Type)    \
template <class T>                   \
Boolean Type<T>::IsEqual(const Type<T>& Obj_) const \
{                                                   \
  return                                            \
  (                                                 \
    Position() == Obj_.Position() &&                \
    Address() == Obj_.Address()                     \
  );                                                \
}                                                   \
                                                    \
template <class T>                                   \
Boolean Type<T>::IsLesser(const Type<T>& Obj_) const \
{                                                    \
  return                                             \
  (                                                  \
    Address() == Obj_.Address() &&                   \
    Position() < Obj_.Position()                     \
  );                                                 \
}                                                    \
                                                     \
template <class T>                                    \
Boolean Type<T>::IsGreater(const Type<T>& Obj_) const \
{                                                     \
  return                                              \
  (                                                   \
    Address() == Obj_.Address() &&                    \
    Position() > Obj_.Position()                      \
  );                                                  \
}

#define ITER_BASECLASS_DEFN(Type, IDTAG) \
ITER_DISPATCH_FNC(Type)                  \
ITER_DESTRUCTOR_FNC(Type)                \
ITER_CLASSID_FNC(Type, IDTAG)            \
ITER_ASSIGNMENT_FNC(Type)                \
ITER_COMPARISON_FNC(Type)

#define ITER_DERIVEDCLASS_DEFN(Type, IDTAG) \
ITER_CLASSID_FNC(Type, IDTAG)

/****************************************************************************/
ITER_BASECLASS_DEFN(ForwardIterator, TypeInfo::FORWARD_ITERATOR_TYPE)
ITER_BASECLASS_DEFN(BidirectionalIterator, TypeInfo::BIDIRECTIONAL_ITERATOR_TYPE)
ITER_BASECLASS_DEFN(RandomAccessIterator, TypeInfo::RANDOMACCESS_ITERATOR_TYPE)

ITER_DERIVEDCLASS_DEFN(ReverseIterator, TypeInfo::REVERSE_ITERATOR_TYPE)
ITER_DERIVEDCLASS_DEFN(ReverseBidirectionalIterator, TypeInfo::REVERSE_BIDIRECTIONAL_ITERATOR_TYPE)
ITER_DERIVEDCLASS_DEFN(ReverseRandomAccessIterator, TypeInfo::REVERSE_RANDOMACCESS_ITERATOR_TYPE)

/****************************************************************************/
#endif





