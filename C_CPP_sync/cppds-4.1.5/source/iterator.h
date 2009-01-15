#ifndef ITERATOR_H
#define ITERATOR_H
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef LONEFALL_H
  #include "lonefall.h"
#endif
#ifndef SUBSCRPT_H
  #include "subscrpt.h"
#endif
#ifndef STL_ITERATOR_H
  #include "stliter.h"
#endif

/****************************************************************************/
#define UNIDIRECTIONAL_ITER_CLASSDEFN(StlRoot, Type) \
template <class T>                    \
class Type : public StlRoot<T, SizeType> \
{ \
  typedef Type<T> _Self; \
                         \
  friend Type<T>& RootObject(Type<T>& Obj_) \
	{ return Obj_; }                    \
  friend const Type<T>& RootConstObject(const Type<T>& Obj_) \
	{ return Obj_; }                                     \
                                                             \
  friend Boolean operator == (const Type<T>& r1, const Type<T>& r2); \
  friend Boolean operator != (const Type<T>& r1, const Type<T>& r2); \
  friend Boolean operator < (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator > (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator <= (const Type<T>& r1, const Type<T>& r2); \
  friend Boolean operator >= (const Type<T>& r1, const Type<T>& r2);  \
  friend ptrdiff_t operator - (const Type<T>& r1, const Type<T>& r2); \
                                                                      \
  friend Boolean _IsGreaterThan(const Type<T>& Trg_, const Type<T>& Src_) \
	{ return Trg_.IsGreater(Src_); }                                  \
  friend Boolean _IsLessThan(const Type<T>& Trg_, const Type<T>& Src_)    \
	{ return Trg_.IsLesser(Src_); }                                   \
  friend Boolean _IsEqualTo(const Type<T>& Trg_, const Type<T>& Src_)     \
	{ return Trg_.IsEqual(Src_); }                                    \
                                                                          \
  friend const Type<T>& _Increment(Type<T>& Trg_)                         \
        { return (++Trg_); }                                              \
                                                                          \
  friend Type<T>* _Clone(const Type<T>& Obj_)                             \
	{ return ((Type<T>*)Obj_.Clone()); }                              \
  friend const Type<T>* _ConstClone(const Type<T>& Obj_)                  \
	{ return &Obj_; }                                                 \
  friend Type<T>* _NonConstClone(Type<T>& Obj_)                           \
	{ return &Obj_; }                                                 \
                                                                          \
  friend int _WhatIs(const Type<T>& Obj_)                                 \
	{ return Obj_.ClassID(); }                                        \
                                                                          \
  friend Boolean _IsSortable(const Type<T>& Obj_)                         \
	{ return TRUE; }                                                  \
  friend Boolean _IsMatchable(const Type<T>& Obj_)                        \
	{ return TRUE; }                                                  \
  friend Boolean _IsOrdinal(const Type<T>& Obj_)                          \
	{ return TRUE; }                                                  \
                                                                          \
  protected:                                                              \
    union                                                                 \
    {                                                                     \
      const Type<T>* _ObjPtr;                                             \
      Type<T>* _VarPtr;                                                   \
    };                                                                    \
                                                                          \
    void Xnull() const                                                    \
	{ THROW (LoneFallibleBase::DerefNullPointerErr()); }              \
                                                                          \
    virtual Type<T>* Dispatch();                                          \
    virtual const Type<T>* Dispatch() const;                              \
                                                                          \
    virtual const void* Address() const                                   \
	{ return Dispatch()->Address(); }                                 \
                                                                          \
  public:                                                                 \
    Type():                                                               \
	_ObjPtr(this) {}                                                  \
    Type(const Type<T>& Boss_):                                           \
	_ObjPtr(Boss_.GiveIterator().Clone()) {}                          \
    Type(const Type<T>* Boss_):                                           \
	_ObjPtr(Boss_) {}                                                 \
                                                                          \
    virtual ~Type();                                                      \
                                                                          \
    static int StaticID();                                                \
    virtual int ClassID() const;                                          \
                                                                          \
    virtual Type<T>* Clone()                                              \
	{ return ((this == _ObjPtr) ?                                     \
			(new Type<T>):                                    \
			(new Type<T>(*this))); }                          \
    virtual const Type<T>* Clone() const                                  \
	{ return ((this == _ObjPtr) ?                                     \
			(new Type<T>):                                    \
			(new Type<T>(*this))); }                          \
                                                                          \
    virtual Boolean IsValid(SizeType Index_) const                        \
	{ return Dispatch()->IsValid(Index_); }                           \
    virtual Boolean IsValid() const                                       \
	{ return Dispatch()->IsValid(); }                                 \
    virtual Boolean More() const                                          \
	{ return Dispatch()->More(); }                                    \
    virtual void Reset() const                                            \
	{ Dispatch()->Reset(); }                                          \
    virtual void SetToLast() const                                        \
	{ Dispatch()->SetToLast(); }                                      \
    virtual void SetToEnd() const                                         \
	{ Dispatch()->SetToEnd(); }                                       \
    virtual SizeType Position() const                                     \
	{ return Dispatch()->Position(); }                                \
                                                                          \
    virtual T& operator * ()                                              \
	{ return **Dispatch(); }                                          \
    virtual const T& operator * () const                                  \
	{ return **Dispatch(); }                                          \
                                                                          \
    virtual const T* ConstPointee() const                                 \
        { return Dispatch()->ConstPointee(); }                            \
    virtual const T* Pointee() const                                      \
        { return Dispatch()->Pointee(); }                                 \
    virtual T* Pointee()                                                  \
        { return Dispatch()->Pointee(); }                                 \
                                                                          \
    virtual Type<T>& GotoHead()                                           \
        { return Dispatch()->GotoHead(); }                                \
    virtual Type<T>& GotoTail()                                           \
        { return Dispatch()->GotoTail(); }                                \
                                                                          \
    virtual const Type<T>& GotoHead() const                               \
        { return Dispatch()->GotoHead(); }                                \
    virtual const Type<T>& GotoTail() const                               \
        { return Dispatch()->GotoTail(); }                                \
                                                                          \
    virtual Boolean AtHead() const                                        \
        { return Dispatch()->AtHead(); }                                  \
    virtual Boolean AtTail() const                                        \
        { return Dispatch()->AtTail(); }                                  \
                                                                          \
    virtual Type<T>& operator ++ ()                                       \
	{ return ++(*Dispatch()); }                                       \
    virtual Type<T> operator ++ (int)                                     \
	{ return (*Dispatch())++; }                                       \
                                                                          \
    virtual const Type<T>& operator ++ () const                           \
	{ return ++(*Dispatch()); }                                       \
    virtual const Type<T> operator ++ (int) const                         \
	{ return (*Dispatch())++; }                                       \
                                                                          \
    virtual Type<T> operator + (SizeType Dist_)                           \
	{ return (*Dispatch() + Dist_); }                                 \
    virtual const Type<T> operator + (SizeType Dist_) const               \
	{ return (*Dispatch() + Dist_); }                                 \
                                                                          \
    virtual Type<T>& operator += (SizeType Dist_)                         \
	{ return (*Dispatch() += Dist_); }                                \
    virtual const Type<T>& operator += (SizeType Dist_) const             \
	{ return (*Dispatch() += Dist_); }                                \
                                                                          \
    virtual Type<T>& GiveIterator()                                       \
	{ return *(Dispatch()); }                                         \
    virtual const Type<T>& GiveIterator() const                           \
	{ return *(Dispatch()); }                                         \
                                                                          \
    virtual Boolean IsEqual(const Type<T>& Obj_) const;                   \
    virtual Boolean IsLesser(const Type<T>& Obj_) const;                  \
    virtual Boolean IsGreater(const Type<T>& Obj_) const;                 \
                                                                          \
    Type<T>& operator = (T* Ptr_);                                        \
    Type<T>& operator = (const Type<T>& Rhs_);

/****************************************************************************/
#define REVERSEUNIDIRECTIONAL_ITER_CLASSDEFN(Root, Type) \
template <class T>                                      \
class Type : public Root<T>                             \
{                                                       \
  typedef Type<T> _Self; \
                         \
  friend Root<T>& RootObject(Type<T>& Obj_)                  \
	{ return *((Root<T>*)&Obj_); }                       \
  friend const Root<T>& RootConstObject(const Type<T>& Obj_) \
	{ return *((const Root<T>*)&Obj_); }                 \
                                                             \
  friend Boolean operator == (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator != (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator < (const Type<T>& r1, const Type<T>& r2);   \
  friend Boolean operator > (const Type<T>& r1, const Type<T>& r2);   \
  friend Boolean operator <= (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator >= (const Type<T>& r1, const Type<T>& r2);  \
  friend ptrdiff_t operator - (const Type<T>& r1, const Type<T>& r2); \
                                                                      \
  public:                       \
    Type() {}                   \
    Type(const Root<T>& Boss_): \
	Root<T>(Boss_) {}       \
    Type(const Root<T>* Boss_): \
	Root<T>(Boss_) {}                               \
                                                        \
    static int StaticID();                              \
    virtual int ClassID() const;                        \
                                                        \
    virtual Root<T>* Clone()                            \
	{ return ((this == _ObjPtr) ?                   \
			(new Type<T>):                  \
			(new Type<T>(*this))); }        \
    virtual const Root<T>* Clone() const                \
	{ return ((this == _ObjPtr) ?                   \
			(new Type<T>):                  \
			(new Type<T>(*this))); }        \
                                                        \
    virtual void Reset() const                          \
	{ Dispatch()->SetToLast(); }                    \
    virtual void SetToLast() const                      \
	{ Dispatch()->Reset(); }                        \
                                                        \
    virtual Root<T>& GotoHead()                         \
        { return Dispatch()->GotoHead(); }              \
    virtual Root<T>& GotoTail()                         \
        { return Dispatch()->GotoTail(); }              \
                                                        \
    virtual const Root<T>& GotoHead() const             \
        { return Dispatch()->GotoHead(); }              \
    virtual const Root<T>& GotoTail() const             \
        { return Dispatch()->GotoTail(); }              \
                                                        \
    virtual Root<T>& operator ++ ()                     \
    { --(*Dispatch());                                  \
      return *this; }                                   \
    virtual Root<T> operator ++ (int)                   \
    { Root<T> RevRef_(Clone());                         \
      --(*Dispatch());                                  \
      return RevRef_; }                                 \
                                                        \
    virtual const Root<T>& operator ++ () const         \
    { --(*Dispatch());                                  \
      return *this; }                                   \
    virtual const Root<T> operator ++ (int) const       \
    { Root<T> RevRef_(Clone());                         \
      --(*Dispatch());                                  \
      return RevRef_; }                                 \
                                                        \
    virtual Root<T> operator + (SizeType Diff_)         \
    { Root<T> Ref_(Clone());                            \
      Ref_ += Diff_;                                    \
      return Ref_; }                                    \
    virtual const Root<T> operator + (SizeType Diff_) const    \
    { Root<T> Ref_(Clone());                                   \
      Ref_ += Diff_;                                           \
      return Ref_; }                                           \
                                                               \
    virtual Root<T>& operator += (SizeType Diff_)              \
    { *Dispatch() -= Diff_;                                    \
      return *this; }                                          \
    virtual const Root<T>& operator += (SizeType Diff_) const  \
    { *Dispatch() -= Diff_;                                    \
      return *this; }

/****************************************************************************/
#define BIDIRECTIONAL_ITER_CLASSDEFN(StlRoot, Type) \
template <class T>                    \
class Type : public StlRoot<T, SizeType> \
{ \
  typedef Type<T> _Self; \
                         \
  friend Type<T>& RootObject(Type<T>& Obj_) \
	{ return Obj_; }                    \
  friend const Type<T>& RootConstObject(const Type<T>& Obj_) \
	{ return Obj_; }                                     \
                                                             \
  friend Boolean operator == (const Type<T>& r1, const Type<T>& r2); \
  friend Boolean operator != (const Type<T>& r1, const Type<T>& r2); \
  friend Boolean operator < (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator > (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator <= (const Type<T>& r1, const Type<T>& r2); \
  friend Boolean operator >= (const Type<T>& r1, const Type<T>& r2);  \
  friend ptrdiff_t operator - (const Type<T>& r1, const Type<T>& r2); \
                                                                      \
  friend Boolean _IsGreaterThan(const Type<T>& Trg_, const Type<T>& Src_) \
	{ return Trg_.IsGreater(Src_); }                                  \
  friend Boolean _IsLessThan(const Type<T>& Trg_, const Type<T>& Src_)    \
	{ return Trg_.IsLesser(Src_); }                                   \
  friend Boolean _IsEqualTo(const Type<T>& Trg_, const Type<T>& Src_)     \
	{ return Trg_.IsEqual(Src_); }                                    \
                                                                          \
  friend const Type<T>& _Increment(Type<T>& Trg_)                         \
		{ return (++Trg_); }											  \
  friend const Type<T>& _Decrement(Type<T>& Trg_)                         \
		{ return (--Trg_); }											  \
                                                                          \
  friend Type<T>* _Clone(const Type<T>& Obj_)                             \
	{ return ((Type<T>*)Obj_.Clone()); }                              \
  friend const Type<T>* _ConstClone(const Type<T>& Obj_)                  \
	{ return &Obj_; }                                                 \
  friend Type<T>* _NonConstClone(Type<T>& Obj_)                           \
	{ return &Obj_; }                                                 \
                                                                          \
  friend int _WhatIs(const Type<T>& Obj_)                                 \
	{ return Obj_.ClassID(); }                                        \
                                                                          \
  friend Boolean _IsSortable(const Type<T>& Obj_)                         \
	{ return TRUE; }                                                  \
  friend Boolean _IsMatchable(const Type<T>& Obj_)                        \
	{ return TRUE; }                                                  \
  friend Boolean _IsOrdinal(const Type<T>& Obj_)                          \
	{ return TRUE; }                                                  \
                                                                          \
  protected:                                                              \
    union                                                                 \
    {                                                                     \
      const Type<T>* _ObjPtr;                                             \
      Type<T>* _VarPtr;                                                   \
    };                                                                    \
                                                                          \
    void Xnull() const                                                    \
	{ THROW (LoneFallibleBase::DerefNullPointerErr()); }              \
                                                                          \
    virtual Type<T>* Dispatch();                                          \
    virtual const Type<T>* Dispatch() const;                              \
                                                                          \
    virtual const void* Address() const                                   \
	{ return Dispatch()->Address(); }                                 \
                                                                          \
  public:                                                                 \
    Type():                                                               \
	_ObjPtr(this) {}                                                  \
    Type(const Type<T>& Boss_):                                           \
	_ObjPtr(Boss_.GiveIterator().Clone()) {}                          \
    Type(const Type<T>* Boss_):                                           \
	_ObjPtr(Boss_) {}                                                 \
                                                                          \
    virtual ~Type();                                                      \
                                                                          \
    static int StaticID();                                                \
    virtual int ClassID() const;                                          \
                                                                          \
    virtual Type<T>* Clone()                                              \
	{ return ((this == _ObjPtr) ?                                     \
			(new Type<T>):                                    \
			(new Type<T>(*this))); }                          \
    virtual const Type<T>* Clone() const                                  \
	{ return ((this == _ObjPtr) ?                                     \
			(new Type<T>):                                    \
			(new Type<T>(*this))); }                          \
                                                                          \
    virtual Boolean IsValid(SizeType Index_) const                        \
	{ return Dispatch()->IsValid(Index_); }                           \
    virtual Boolean IsValid() const                                       \
	{ return Dispatch()->IsValid(); }                                 \
    virtual Boolean More() const                                          \
	{ return Dispatch()->More(); }                                    \
    virtual void Reset() const                                            \
	{ Dispatch()->Reset(); }                                          \
    virtual void SetToLast() const                                        \
	{ Dispatch()->SetToLast(); }                                      \
    virtual void SetToEnd() const                                         \
	{ Dispatch()->SetToEnd(); }                                       \
    virtual SizeType Position() const                                     \
	{ return Dispatch()->Position(); }                                \
                                                                          \
    virtual T& operator * ()                                              \
	{ return **Dispatch(); }                                          \
    virtual const T& operator * () const                                  \
	{ return **Dispatch(); }                                          \
                                                                          \
    virtual const T* ConstPointee() const                                 \
        { return Dispatch()->ConstPointee(); }                            \
    virtual const T* Pointee() const                                      \
        { return Dispatch()->Pointee(); }                                 \
    virtual T* Pointee()                                                  \
        { return Dispatch()->Pointee(); }                                 \
                                                                          \
    virtual const Type<T>& GotoHead() const                               \
        { return Dispatch()->GotoHead(); }                                \
    virtual const Type<T>& GotoTail() const                               \
        { return Dispatch()->GotoTail(); }                                \
                                                                          \
    virtual Type<T>& GotoHead()                                           \
        { return Dispatch()->GotoHead(); }                                \
    virtual Type<T>& GotoTail()                                           \
        { return Dispatch()->GotoTail(); }                                \
                                                                          \
    virtual Boolean AtHead() const                                        \
        { return Dispatch()->AtHead(); }                                  \
    virtual Boolean AtTail() const                                        \
        { return Dispatch()->AtTail(); }                                  \
                                                                          \
    virtual Type<T>& operator ++ ()                                       \
	{ return ++(*Dispatch()); }                                       \
    virtual Type<T> operator ++ (int)                                     \
	{ return (*Dispatch())++; }                                       \
                                                                          \
    virtual const Type<T>& operator ++ () const                           \
	{ return ++(*Dispatch()); }                                       \
    virtual const Type<T> operator ++ (int) const                         \
	{ return (*Dispatch())++; }                                       \
                                                                          \
    virtual Type<T>& operator -- ()                                       \
	{ return --(*Dispatch()); }                                       \
    virtual Type<T> operator -- (int)                                     \
	{ return (*Dispatch())--; }                                       \
                                                                          \
    virtual const Type<T>& operator -- () const                           \
	{ return --(*Dispatch()); }                                       \
    virtual const Type<T> operator -- (int) const                         \
	{ return (*Dispatch())--; }                                       \
                                                                          \
    virtual Type<T> operator + (SizeType Dist_)                           \
	{ return (*Dispatch() + Dist_); }                                 \
    virtual const Type<T> operator + (SizeType Dist_) const               \
	{ return (*Dispatch() + Dist_); }                                 \
    virtual Type<T> operator - (SizeType Dist_)                           \
	{ return (*Dispatch() - Dist_); }                                 \
    virtual const Type<T> operator - (SizeType Dist_) const               \
	{ return (*Dispatch() - Dist_); }                                 \
                                                                          \
    virtual Type<T>& operator += (SizeType Dist_)                         \
	{ return (*Dispatch() += Dist_); }                                \
    virtual const Type<T>& operator += (SizeType Dist_) const             \
	{ return (*Dispatch() += Dist_); }                                \
    virtual Type<T>& operator -= (SizeType Dist_)                         \
	{ return (*Dispatch() -= Dist_); }                                \
    virtual const Type<T>& operator -= (SizeType Dist_) const             \
	{ return (*Dispatch() -= Dist_); }                                \
                                                                          \
    virtual Type<T>& GiveIterator()                                       \
	{ return *(Dispatch()); }                                         \
    virtual const Type<T>& GiveIterator() const                           \
	{ return *(Dispatch()); }                                         \
                                                                          \
    virtual Boolean IsEqual(const Type<T>& Obj_) const;                   \
    virtual Boolean IsLesser(const Type<T>& Obj_) const;                  \
    virtual Boolean IsGreater(const Type<T>& Obj_) const;                 \
                                                                          \
    Type<T>& operator = (T* Ptr_);                                        \
    Type<T>& operator = (const Type<T>& Rhs_);

/****************************************************************************/
#define RANDOMACCESS_ITER_CLASSDEFN(StlRoot, Type) \
BIDIRECTIONAL_ITER_CLASSDEFN(StlRoot, Type)        \
  public:                                       \
    virtual Type<T>& operator [] (SizeType Index_) \
        { GotoHead();                                          \
          return (*Dispatch() += Index_); }                    \
    virtual const Type<T>& operator [] (SizeType Index_) const \
        { GotoHead();                                          \
          return (*Dispatch() += Index_); }

/****************************************************************************/
#define DERIVED_ITER_COMMONDEFN(Root, Type) \
template <class T>                                      \
class Type : public Root<T>                             \
{                                                       \
  typedef Type<T> _Self; \
                         \
  friend Root<T>& RootObject(Type<T>& Obj_)                  \
	{ return *((Root<T>*)&Obj_); }                       \
  friend const Root<T>& RootConstObject(const Type<T>& Obj_) \
	{ return *((const Root<T>*)&Obj_); }                 \
                                                             \
  friend Boolean operator == (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator != (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator < (const Type<T>& r1, const Type<T>& r2);   \
  friend Boolean operator > (const Type<T>& r1, const Type<T>& r2);   \
  friend Boolean operator <= (const Type<T>& r1, const Type<T>& r2);  \
  friend Boolean operator >= (const Type<T>& r1, const Type<T>& r2);  \
  friend ptrdiff_t operator - (const Type<T>& r1, const Type<T>& r2); \
                                                                      \
  public:                       \
    Type() {}                   \
    Type(const Root<T>& Boss_): \
	Root<T>(Boss_) {}       \
    Type(const Root<T>* Boss_): \
	Root<T>(Boss_) {}                               \
                                                        \
    static int StaticID();                              \
    virtual int ClassID() const;                        \
                                                        \
    virtual Root<T>* Clone()                            \
	{ return ((this == _ObjPtr) ?                   \
			(new Type<T>):                  \
			(new Type<T>(*this))); }        \
    virtual const Root<T>* Clone() const                \
	{ return ((this == _ObjPtr) ?                   \
			(new Type<T>):                  \
			(new Type<T>(*this))); }        \
                                                        \
    virtual void Reset() const                          \
	{ Dispatch()->SetToLast(); }                    \
    virtual void SetToLast() const                      \
	{ Dispatch()->Reset(); }                        \
                                                        \
    virtual Root<T>& GotoHead()                         \
        { return Dispatch()->GotoHead(); }              \
    virtual Root<T>& GotoTail()                         \
        { return Dispatch()->GotoTail(); }              \
                                                        \
    virtual const Root<T>& GotoHead() const             \
        { return Dispatch()->GotoHead(); }              \
    virtual const Root<T>& GotoTail() const             \
        { return Dispatch()->GotoTail(); }              \
                                                        \
    virtual Root<T>& operator ++ ()                     \
    { --(*Dispatch());                                  \
      return *this; }                                   \
    virtual Root<T> operator ++ (int)                   \
    { Root<T> RevRef_(Clone());                         \
      --(*Dispatch());                                  \
      return RevRef_; }                                 \
                                                        \
    virtual const Root<T>& operator ++ () const         \
    { --(*Dispatch());                                  \
      return *this; }                                   \
    virtual const Root<T> operator ++ (int) const       \
    { Root<T> RevRef_(Clone());                         \
      --(*Dispatch());                                  \
      return RevRef_; }                                 \
                                                        \
    virtual Root<T>& operator -- ()                     \
    { ++(*Dispatch());                                  \
      return *this; }                                   \
    virtual Root<T> operator -- (int)                   \
    { Root<T> RevRef_(Clone());                         \
      ++(*Dispatch());                                  \
      return RevRef_; }                                 \
                                                        \
    virtual const Root<T>& operator -- () const         \
    { ++(*Dispatch());                                  \
      return *this; }                                   \
    virtual const Root<T> operator -- (int) const       \
    { Root<T> RevRef_(Clone());                         \
      ++(*Dispatch());                                  \
      return RevRef_; }                                 \
                                                        \
    virtual Root<T> operator + (SizeType Diff_)         \
    { Root<T> Ref_(Clone());                            \
      Ref_ += Diff_;                                    \
      return Ref_; }                                    \
    virtual const Root<T> operator + (SizeType Diff_) const    \
    { Root<T> Ref_(Clone());                                   \
      Ref_ += Diff_;                                           \
      return Ref_; }                                           \
    virtual Root<T> operator - (SizeType Diff_)                \
    { Root<T> Ref_(Clone());                                   \
      Ref_ -= Diff_;                                           \
      return Ref_; }                                           \
    virtual const Root<T> operator - (SizeType Diff_) const    \
    { Root<T> Ref_(Clone());                                   \
      Ref_ -= Diff_;                                           \
      return Ref_; }                                           \
                                                               \
    virtual Root<T>& operator += (SizeType Diff_)              \
    { *Dispatch() -= Diff_;                                    \
      return *this; }                                          \
    virtual const Root<T>& operator += (SizeType Diff_) const  \
    { *Dispatch() -= Diff_;                                    \
      return *this; }                                          \
    virtual Root<T>& operator -= (SizeType Diff_)              \
    { *Dispatch() += Diff_;                                    \
      return *this; }                                          \
    virtual const Root<T>& operator -= (SizeType Diff_) const  \
    { *Dispatch() += Diff_;                                    \
      return *this; }    

/****************************************************************************/
#define REVERSEBIDIRECTIONAL_ITER_CLASSDEFN(Root, Type) \
DERIVED_ITER_COMMONDEFN(Root, Type)

/****************************************************************************/
#define REVERSERANDOMACCESS_ITER_CLASSDEFN(Root, Type) \
DERIVED_ITER_COMMONDEFN(Root, Type)                    \
  public:                                              \
    virtual Root<T>& operator [] (SizeType Index_)     \
        { GotoTail();                                          \
          return (*Dispatch() -= Index_); }                    \
    virtual const Root<T>& operator [] (SizeType Index_) const \
        { GotoTail();                                          \
          return (*Dispatch() -= Index_); }

#define END_CLASSDEFN() };

/****************************************************************************/
UNIDIRECTIONAL_ITER_CLASSDEFN(Forward_Iterator, ForwardIterator)
END_CLASSDEFN()

REVERSEUNIDIRECTIONAL_ITER_CLASSDEFN(ForwardIterator, ReverseIterator)
END_CLASSDEFN()

BIDIRECTIONAL_ITER_CLASSDEFN(Bidirectional_Iterator, BidirectionalIterator)
END_CLASSDEFN()

REVERSEBIDIRECTIONAL_ITER_CLASSDEFN(BidirectionalIterator, ReverseBidirectionalIterator)
END_CLASSDEFN()

RANDOMACCESS_ITER_CLASSDEFN(Random_Access_Iterator, RandomAccessIterator)
END_CLASSDEFN()

REVERSERANDOMACCESS_ITER_CLASSDEFN(RandomAccessIterator, ReverseRandomAccessIterator)
END_CLASSDEFN()

/****************************************************************************/
#define ITERATOR_COMP_DEFN(Type) \
template <class T>   \
Boolean operator == (const Type<T>& r1, const Type<T>& r2) \
{ \
  return \
  ( \
    r1.Position() == r2.Position() && \
    r1.Address() == r2.Address()      \
  );                                  \
}                                     \
                                      \
template <class T>                    \
Boolean operator != (const Type<T>& r1, const Type<T>& r2) \
{ \
  return \
  ( \
    r1.Position() != r2.Position() || \
    r1.Address() != r2.Address()      \
  );                                  \
}                                     \
                                      \
template <class T>                    \
Boolean operator < (const Type<T>& r1, const Type<T>& r2) \
{ \
  return \
  ( \
    r1.Address() == r2.Address() && \
    r1.Position() < r2.Position()   \
  );                                \
}                                   \
                                    \
template <class T>                  \
Boolean operator > (const Type<T>& r1, const Type<T>& r2) \
{ \
  return \
  ( \
    r1.Address() == r2.Address() && \
    r1.Position() > r2.Position()   \
  );                                \
}                                   \
                                    \
template <class T> \
Boolean operator <= (const Type<T>& r1, const Type<T>& r2) \
{ \
  return \
  ( \
    r1.Address() == r2.Address() && \
    r1.Position() <= r2.Position()  \
  );                                \
}                                   \
                                    \
template <class T>                  \
Boolean operator >= (const Type<T>& r1, const Type<T>& r2) \
{ \
  return \
  ( \
    r1.Address() == r2.Address() && \
    r1.Position() >= r2.Position()  \
  );                                \
}                                   \
                                    \
template <class T>                  \
ptrdiff_t operator - (const Type<T>& r1, const Type<T>& r2) \
{ \
  SizeType LhsPos_ = r1.Position(); \
  return (LhsPos_ - r2.Position()); \
}

/****************************************************************************/
ITERATOR_COMP_DEFN(ForwardIterator)
ITERATOR_COMP_DEFN(BidirectionalIterator)
ITERATOR_COMP_DEFN(RandomAccessIterator)

ITERATOR_COMP_DEFN(ReverseIterator)
ITERATOR_COMP_DEFN(ReverseBidirectionalIterator)
ITERATOR_COMP_DEFN(ReverseRandomAccessIterator)

/****************************************************************************/
#endif





