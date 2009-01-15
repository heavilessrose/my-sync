#ifndef COMPARE_CPP
#define COMPARE_CPP
#ifndef COMPARE_H
  #include "compare.h"
#endif

/****************************************************************************/
/****************** Pointer Argument Comparison Base Class ******************/
/****************************************************************************/
// PRE:
//   const T* Ptr1_ : the pointer to the first object to be compared
//   const T* Ptr2_ : the pointer to the second object to be compared
//
template <class T>
PtrCompareImp<T>::PtrCompareImp(const T* Ptr1_, const T* Ptr2_):
_Pointer1(Ptr1_),
_Pointer2(Ptr2_)
{}

/****************************************************************************/
// Cast to BOOLTYPE operator where BOOLTYPE is the predefined boolean type
//
// POST:
//   if both data members are false then returns a false BOOLTYPE.
//   otherwise dereferences the pointers and compares the objects via the
//   Compare method
//
template <class T>
PtrCompareImp<T>::operator BOOLTYPE () const
{
  return ((_Pointer1 == NULL || _Pointer2 == NULL) ?
		BOOLTYPE(FALSE):
		Compare(*_Pointer1, *_Pointer2));
}

/****************************************************************************/
// Method accepts pointers to objects and assigns them to the data members
// then calls the cast to BOOLTYPE method to return the result of comparing
// the two objects.
//
template <class T>
BOOLTYPE PtrCompareImp<T>::operator () (const T* Ptr1_, const T* Ptr2_)
{
  _Pointer1 = Ptr1_;
  _Pointer2 = Ptr2_;
  return BOOLTYPE(*this);
}

/****************************************************************************/
/************************** Equality Test Functor ***************************/
/****************************************************************************/
// object comparison default class constructor uses
// MatchUsingBuiltInOperators as comparison method
//
template <class T>
PtrEqual<T>::PtrEqual():
PtrCompareImp<T>(NULL, NULL),
_Comp(RelationalEnum::EQUAL),
_Comparer(MatchUsingBuiltInOperators<T>::Trait())
{}

/****************************************************************************/
// object comparison class constructor accepting matchable traits object as a
// way of specifying the comparison algorithm and method.
//
template <class T>
PtrEqual<T>::PtrEqual(MatchableTraits<T>* MatchTraits_):
PtrCompareImp<T>(NULL, NULL),
_Comp(RelationalEnum::EQUAL),
_Comparer(MatchTraits_)
{}

/****************************************************************************/
// object comparison class constructor accepting matchable traits object as a
// way of specifying the comparison algorithm and method. Also accepting the
// pointer to objects to be compared.
//
template <class T>
PtrEqual<T>::PtrEqual(const T* Ptr1_, const T* Ptr2_, MatchableTraits<T>* MatchTraits_):
PtrCompareImp<T>(Ptr1_, Ptr2_),
_Comp(RelationalEnum::EQUAL),
_Comparer(MatchTraits_)
{}

/****************************************************************************/
template <class T>
PtrEqual<T>::PtrEqual(const PtrEqual<T>& Obj_):
PtrCompareImp<T>(((PtrCompareImp<T>&)Obj_)),
_Comp(Obj_._Comp),
_Comparer(Obj_._Comparer->Clone())
{}

/****************************************************************************/
template <class T>
PtrEqual<T>::~PtrEqual()
{
  delete _Comparer;
  _Comparer = NULL;
}

/****************************************************************************/
template <class T>
BOOLTYPE PtrEqual<T>::Compare(const T& Obj1_, const T& Obj2_) const
{
  return ((_Comp == RelationalEnum::EQUAL) ?
		_Comparer->Equal(Obj1_, Obj2_):
		_Comparer->NotEqual(Obj1_, Obj2_));
}

/****************************************************************************/
template <class T>
PtrEqual<T>::operator BOOLTYPE () const
{
  return PtrCompareImp<T>::operator BOOLTYPE();
}

/****************************************************************************/
template <class T>
BOOLTYPE PtrEqual<T>::operator () (const T* Ptr1_, const T* Ptr2_)
{
  return PtrCompareImp<T>::operator () (Ptr1_, Ptr2_);
}

/****************************************************************************/
// Method inverts the comparison so EQUAL will be NOT EQUAL and vice versa
// example: comp.Invert().Compare(ptr1, ptr2) will compare for NOT EQUAL.
//
template <class T>
PtrCompare<T>& PtrEqual<T>::Invert()
{
  _Comp = (_Comp == RelationalEnum::EQUAL) ? RelationalEnum::NOT_EQUAL:RelationalEnum::EQUAL;
  return *this;
}

/****************************************************************************/
template <class T>
PtrEqual<T>& PtrEqual<T>::operator = (const PtrEqual<T>& Obj_)
{
  PtrCompareImp<T>::operator = ((PtrCompareImp<T>&)Obj_);
  _Comp = Obj_._Comp;  
  delete _Comparer;
  _Comparer = Obj_._Comparer->Clone();
  
  return *this;
}

/****************************************************************************/
/****************************************************************************/
// ObjEqual class is equivalent to PtrEqual class, but accepts objects instead
// of pointer to objects.
//
template <class T>
ObjEqual<T>::ObjEqual():
_Comparer(MatchUsingBuiltInOperators<T>::Trait())
{}

/****************************************************************************/
// ObjEqual class is equivalent to PtrEqual class, but accepts objects instead
// of pointer to objects.
//
template <class T>
ObjEqual<T>::ObjEqual(MatchableTraits<T>* MatchTraits_):
_Comparer(MatchTraits_)
{}

/****************************************************************************/
template <class T>
ObjEqual<T>::ObjEqual(const T& Obj1_, const T& Obj2_, MatchableTraits<T>* MatchTraits_):
_Comparer(&Obj1_, &Obj2_, MatchTraits_)
{}

/****************************************************************************/
template <class T>
ObjEqual<T>::operator BOOLTYPE () const
{
  return BOOLTYPE(_Comparer);
}

/****************************************************************************/
template <class T>
BOOLTYPE ObjEqual<T>::operator () (const T& Obj1_, const T& Obj2_)
{
  return _Comparer(&Obj1_, &Obj2_);
}

/****************************************************************************/
template <class T>
ObjCompare<T>& ObjEqual<T>::Invert()
{
  _Comparer.Invert();
  return *this;
}

/****************************************************************************/
/************************ Relational Test Functor ***************************/
/****************************************************************************/
// object comparison default class constructor uses
// SortUsingBuiltInOperators as comparison method.
//
template <class T>
PtrRelation<T>::PtrRelation():
PtrCompareImp<T>(NULL, NULL),
_Comp(RelationalEnum::EQUAL),
_Comparer(SortUsingBuiltInOperators<T>::Trait())
{}

/****************************************************************************/
// object comparison class constructor accepting sortable traits object as a
// way of specifying the comparison algorithm and method.
//
template <class T>
PtrRelation<T>::PtrRelation(SortableTraits<T>* SortTraits_):
PtrCompareImp<T>(NULL, NULL),
_Comp(RelationalEnum::EQUAL),
_Comparer(SortTraits_)
{}

/****************************************************************************/
template <class T>
PtrRelation<T>::PtrRelation(int Comp_, SortableTraits<T>* SortTraits_):
PtrCompareImp<T>(NULL, NULL),
_Comp(Comp_),
_Comparer(SortTraits_)
{}

/****************************************************************************/
template <class T>
PtrRelation<T>::PtrRelation(const T* Ptr1_, int Comp_, const T* Ptr2_, SortableTraits<T>* SortTraits_):
PtrCompareImp<T>(Ptr1_, Ptr2_),
_Comp(Comp_),
_Comparer(SortTraits_)
{}

/****************************************************************************/
template <class T>
PtrRelation<T>::PtrRelation(const PtrRelation<T>& Obj_):
PtrCompareImp<T>(((PtrCompareImp<T>&)Obj_)),
_Comp(Obj_._Comp),
_Comparer(Obj_._Comparer->Clone())
{}

/****************************************************************************/
template <class T>
PtrRelation<T>::~PtrRelation()
{
  delete _Comparer;
  _Comparer = NULL;
}

/****************************************************************************/
template <class T>
PtrRelation<T>& PtrRelation<T>::operator = (const PtrRelation<T>& Obj_)
{
  PtrCompareImp<T>::operator = ((PtrCompareImp<T>&)Obj_);
  _Comp = Obj_._Comp;  
  delete _Comparer;
  _Comparer = (SortableTraits<T>*)Obj_._Comparer->Clone();
  
  return *this;
}

/****************************************************************************/
template <class T>
BOOLTYPE PtrRelation<T>::Compare(const T& Obj1_, const T& Obj2_) const
{
  return
  (
	(_Comp == RelationalEnum::EQUAL) ?
		_Comparer->Equal(Obj1_, Obj2_):
	(_Comp == RelationalEnum::NOT_EQUAL) ?
		_Comparer->NotEqual(Obj1_, Obj2_):
	(_Comp == RelationalEnum::LESS_THAN) ?
		_Comparer->LessThan(Obj1_, Obj2_):
	(_Comp == RelationalEnum::LESS_THAN_OR_EQUAL) ?
		(_Comparer->LessThan(Obj1_, Obj2_) ||
		 _Comparer->Equal(Obj1_, Obj2_)):
	(_Comp == RelationalEnum::GREATER_THAN)	?
		_Comparer->GreaterThan(Obj1_, Obj2_):
	(_Comp == RelationalEnum::GREATER_THAN_OR_EQUAL) ?
		(_Comparer->GreaterThan(Obj1_, Obj2_) ||
		 _Comparer->Equal(Obj1_, Obj2_)):
		FALSE
  );
}

/****************************************************************************/
// Method to specify the comparison operation to be done.
//
template <class T>
PtrCompare<T>& PtrRelation<T>::Relation(int Comp_)
{
  _Comp = Comp_;
  return *this;
}

/****************************************************************************/
// Invert the comparison operation
//
template <class T>
PtrCompare<T>& PtrRelation<T>::Invert()
{
  _Comp =
  (_Comp == RelationalEnum::EQUAL) ?
	RelationalEnum::NOT_EQUAL:
  (_Comp == RelationalEnum::LESS_THAN) ?
	RelationalEnum::GREATER_THAN_OR_EQUAL:
  (_Comp == RelationalEnum::LESS_THAN_OR_EQUAL) ?
	RelationalEnum::GREATER_THAN:
  (_Comp == RelationalEnum::GREATER_THAN) ?
	RelationalEnum::LESS_THAN_OR_EQUAL:
  (_Comp == RelationalEnum::GREATER_THAN_OR_EQUAL) ?
	RelationalEnum::LESS_THAN:
	RelationalEnum::EQUAL;

  return *this;
}

/****************************************************************************/
template <class T>
PtrRelation<T>::operator BOOLTYPE () const
{
  return PtrCompareImp<T>::operator BOOLTYPE();
}

/****************************************************************************/
template <class T>
BOOLTYPE PtrRelation<T>::operator () (const T* Ptr1_, const T* Ptr2_)
{
  return PtrCompareImp<T>::operator () (Ptr1_, Ptr2_);
}

/****************************************************************************/
template <class T>
const T* PtrRelation<T>::Min(const T* x1, const T* x2, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  return ((BOOLTYPE)PtrRelation<T>(x1, RelationalEnum::LESS_THAN, x2, SortTraits_) ? x1:x2);
}

/****************************************************************************/
template <class T>
const T* PtrRelation<T>::Max(const T* x1, const T* x2, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  return ((BOOLTYPE)PtrRelation<T>(x1, RelationalEnum::GREATER_THAN, x2, SortTraits_) ? x1:x2);
}

/****************************************************************************/
template <class T>
BOOLTYPE PtrRelation<T>::InRange(const T* x, const T* min, const T* max, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  SortableTraits<T>* SortTraits2_ = (SortableTraits<T>*)SortTraits_->Clone();
  BOOLTYPE Con1_ = PtrRelation<T>(x, RelationalEnum::GREATER_THAN_OR_EQUAL, min, SortTraits_);
  BOOLTYPE Con2_ = PtrRelation<T>(x, RelationalEnum::LESS_THAN_OR_EQUAL, max, SortTraits2_);

  return (Con1_ && Con2_);
}

/****************************************************************************/
template <class T>
BOOLTYPE PtrRelation<T>::XInRange(const T* x, const T* min, const T* max, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  SortableTraits<T>* SortTraits2_ = (SortableTraits<T>*)SortTraits_->Clone();
  BOOLTYPE Con1_ = PtrRelation<T>(x, RelationalEnum::GREATER_THAN, min, SortTraits_);
  BOOLTYPE Con2_ = PtrRelation<T>(x, RelationalEnum::LESS_THAN, max, SortTraits2_);

  return (Con1_ && Con2_);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
ObjRelation<T>::ObjRelation():
_Comparer(SortUsingBuiltInOperators<T>::Trait())
{}

/****************************************************************************/
template <class T>
ObjRelation<T>::ObjRelation(SortableTraits<T>* SortTraits_):
_Comparer(SortTraits_)
{}

/****************************************************************************/
template <class T>
ObjRelation<T>::ObjRelation(int Comp_, SortableTraits<T>* SortTraits_):
_Comparer(Comp_, SortTraits_)
{}

/****************************************************************************/
template <class T>
ObjRelation<T>::ObjRelation(const T& Obj1_, int Comp_, const T& Obj2_, SortableTraits<T>* SortTraits_):
_Comparer(&Obj1_, Comp_, &Obj2_, SortTraits_)
{}

/****************************************************************************/
template <class T>
ObjRelation<T>::operator BOOLTYPE () const
{
  return BOOLTYPE(_Comparer);
}

/****************************************************************************/
template <class T>
BOOLTYPE ObjRelation<T>::operator () (const T& Obj1_, const T& Obj2_)
{
  return _Comparer(&Obj1_, &Obj2_);
}

/****************************************************************************/
template <class T>
ObjCompare<T>& ObjRelation<T>::Relation(int Comp_)
{
  _Comparer.Relation(Comp_);
  return *this;
}

/****************************************************************************/
template <class T>
ObjCompare<T>& ObjRelation<T>::Invert()
{
  _Comparer.Invert();
  return *this;
}

/****************************************************************************/
template <class T>
const T& ObjRelation<T>::Min(const T& x1, const T& x2, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  return ((BOOLTYPE)ObjRelation<T>(x1, RelationalEnum::LESS_THAN, x2, SortTraits_) ? x1:x2);
}

/****************************************************************************/
template <class T>
const T& ObjRelation<T>::Max(const T& x1, const T& x2, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  return ((BOOLTYPE)ObjRelation<T>(x1, RelationalEnum::GREATER_THAN, x2, SortTraits_) ? x1:x2);
}

/****************************************************************************/
template <class T>
BOOLTYPE ObjRelation<T>::InRange(const T& x, const T& min, const T& max, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  SortableTraits<T>* SortTraits2_ = (SortableTraits<T>*)SortTraits_->Clone();
  BOOLTYPE Con1_ = ObjRelation<T>(x, RelationalEnum::GREATER_THAN_OR_EQUAL, min, SortTraits_);
  BOOLTYPE Con2_ = ObjRelation<T>(x, RelationalEnum::LESS_THAN_OR_EQUAL, max, SortTraits2_);
  
  return (Con1_ && Con2_);
}

/****************************************************************************/
template <class T>          
BOOLTYPE ObjRelation<T>::XInRange(const T& x, const T& min, const T& max, SortableTraits<T>* SortTraits_)
{
  if (!SortTraits_)
    SortTraits_ = SortUsingBuiltInOperators<T>::Trait();

  SortableTraits<T>* SortTraits2_ = (SortableTraits<T>*)SortTraits_->Clone();
  BOOLTYPE Con1_ = ObjRelation<T>(x, RelationalEnum::GREATER_THAN, min, SortTraits_);
  BOOLTYPE Con2_ = ObjRelation<T>(x, RelationalEnum::LESS_THAN, max, SortTraits2_);
  
  return (Con1_ && Con2_);
}

/****************************************************************************/
/****************************************************************************/
// Generic conditional function
template <class T>
inline T Conditional(BOOLTYPE Cond_, T Val1_, T Val2_)
{
  return (Cond_ ? Val1_:Val2_);
}

/****************************************************************************/
/****************************************************************************/
#if COMPARE_DEBUG
#include <iostream.h>
#if defined(__TURBOC__)
  #include <conio.h>
#endif
int main()
{
  Dumstruct dum1;
  Dumstruct dum2;

  Somestruct some1;
  Somestruct some2;
  some1.Set(5);
  some2.Set(5);

  int v1 = 1, v4 = 4, v7 = 7;
  int a1 = 1, a4 = 4, a7 = 7;

#if defined(__TURBOC__)
  clrscr();
#endif

#if COMPARE_DEBUG1
//  PtrEqual<int> eqcmp(MatchUsingBuiltInOperators<int>::Trait());
  PtrEqual<int> eqcmp;
  PtrRelation<int> relcmp(SortUsingUserDefinedFunctor<int>::Trait(SortUsingBuiltInOperators<int>::Trait()));

  eqcmp(&v1, &a1);
  assert((BOOLTYPE)eqcmp);

  relcmp.Relation(RelationalEnum::LESS_THAN);
  assert((BOOLTYPE)relcmp(&v1, &a4));
  assert(!relcmp(&v4, &a1));
  relcmp.Relation(RelationalEnum::GREATER_THAN);
  assert((BOOLTYPE)relcmp(&v7, &a4));
  assert(!relcmp(&v4, &a7));
  relcmp.Relation(RelationalEnum::EQUAL);
  assert((BOOLTYPE)relcmp(&v7, &a7));
  assert(!relcmp(&v1, &a7));

  assert((BOOLTYPE)relcmp.InRange(&a4, &v1, &v7, SortUsingBuiltInOperators<int>::Trait()));
  assert(!relcmp.XInRange(&a1, &v1, &v7, SortUsingBuiltInOperators<int>::Trait()));
  assert((BOOLTYPE)relcmp.InRange(&a1, &v1, &v7, SortUsingBuiltInOperators<int>::Trait()));
  assert(!PtrRelation<int>::InRange(&a7, &v1, &v4, SortUsingBuiltInOperators<int>::Trait()));

  PtrRelation<Dumstruct> dumrelcmp(SortUsingTemplateFunctions<Dumstruct>::Trait());
  assert(!dumrelcmp(&dum1, &dum2));

  PtrEqual<Somestruct> somerelcmp(MatchUsingTemplateFunctions<Somestruct>::Trait());
  assert((BOOLTYPE)somerelcmp(&some1, &some2));
  some1.Set(0);
  assert(!somerelcmp(&some1, &some2));

  cout <<"SUCCESS Testing PtrEqual, PtrRelation classes" <<endl;
#endif

#if COMPARE_DEBUG2
//  ObjEqual<int> eqcmp2(MatchUsingBuiltInOperators<int>::Trait());
  ObjEqual<int> eqcmp2;
  ObjRelation<int> relcmp2(SortUsingUserDefinedFunctor<int>::Trait(SortUsingBuiltInOperators<int>::Trait()));

  eqcmp2(v1, a1);
  eqcmp2.Invert();
  assert(!eqcmp2);

  relcmp2.Relation(RelationalEnum::LESS_THAN);
  relcmp2.Invert();
  assert(!relcmp2(v1, a4));
  assert((BOOLTYPE)relcmp2(v4, a1));
  relcmp2.Relation(RelationalEnum::GREATER_THAN);
  relcmp2.Invert();
  assert(!relcmp2(v7, a4));
  assert((BOOLTYPE)relcmp2(v4, a7));
  relcmp2.Relation(RelationalEnum::EQUAL);
  relcmp2.Invert();
  assert(!relcmp2(v7, a7));
  assert((BOOLTYPE)relcmp2(v1, a7));

  assert((BOOLTYPE)relcmp2.InRange(a4, v1, v7, SortUsingBuiltInOperators<int>::Trait()));
  assert(!relcmp2.XInRange(a1, v1, v7, SortUsingBuiltInOperators<int>::Trait()));
  assert((BOOLTYPE)relcmp2.InRange(a1, v1, v7, SortUsingBuiltInOperators<int>::Trait()));
  assert(!ObjRelation<int>::InRange(a7, v1, v4, SortUsingBuiltInOperators<int>::Trait()));

  ObjRelation<Dumstruct> dumrelcmp(SortUsingTemplateFunctions<Dumstruct>::Trait());
  assert(!dumrelcmp(dum1, dum2));

  ObjEqual<Somestruct> somerelcmp(MatchUsingTemplateFunctions<Somestruct>::Trait());
  assert((BOOLTYPE)somerelcmp(some1, some2));
  some1.Set(0);
  assert(!somerelcmp(some1, some2));

  cout <<"SUCCESS Testing ObjEqual, ObjRelation classes" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




