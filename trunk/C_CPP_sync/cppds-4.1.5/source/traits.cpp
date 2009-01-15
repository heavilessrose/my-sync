#ifndef TRAITS_CPP
#define TRAITS_CPP
#ifndef TRAITS_H
  #include "traits.h"
#endif

/****************************************************************************/
template <class T>    
T& StreamUsingBuiltInOperators<T>::Write(ostream& Os_, const T& Src_)
{
  Os_ << Src_;
  return Os_;
}

/****************************************************************************/
template <class T>
T& StreamUsingBuiltInOperators<T>::Read(istream& Is_, T& Trg_)
{
  Is_ >> Trg_;
  return Is_;
}

/****************************************************************************/
template <class T>
StreamableTraits<T>* StreamUsingBuiltInOperators<T>::Clone() const
{
  return (new StreamUsingBuiltInOperators<T>);
}

/****************************************************************************/
/****************************************************************************/
template <class T>    
T& StreamUsingTemplateFunctions<T>::Write(ostream& Os_, const T& Src_)
{
  WriteToOutput(Os_, Src_);
  return Os_;
}

/****************************************************************************/
template <class T>
T& StreamUsingTemplateFunctions<T>::Read(istream& Is_, T& Trg_)
{
  ReadFromInput(Is_, Trg_);
  return Is_;
}

/****************************************************************************/
template <class T>
StreamableTraits<T>* StreamUsingTemplateFunctions<T>::Clone() const
{
  return (new StreamUsingTemplateFunctions<T>);
}

/****************************************************************************/
/****************************************************************************/
template <class T>
StreamUsingUserDefinedFunctor<T>::StreamUsingUserDefinedFunctor(StreamableTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
StreamUsingUserDefinedFunctor<T>::StreamUsingUserDefinedFunctor(const StreamUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? Obj_._FnPtr->Clone():Obj_._FnPtr)
{}

/****************************************************************************/
template <class T>         
void StreamUsingUserDefinedFunctor<T>::SetFnPtr(StreamableTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>    
T& StreamUsingUserDefinedFunctor<T>::Write(ostream& Os_, const T& Src_)
{
  return _FnPtr->Write(Os_, Src_);
}

/****************************************************************************/
template <class T>
T& StreamUsingUserDefinedFunctor<T>::Read(istream& Is_, T& Trg_)
{
  return _FnPtr->Read(Is_, Trg_);
}

/****************************************************************************/
template <class T>
StreamableTraits<T>* StreamUsingUserDefinedFunctor<T>::Clone() const
{
  return (new StreamUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
StreamUsingUserDefinedFunctor<T>::~StreamUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& InvertUsingBuiltInOperators<T>::Invert(T& x)
{
  x = !x;
  return x;
}

/****************************************************************************/
template <class T>
InvertableTraits<T>* InvertUsingBuiltInOperators<T>::Clone() const
{
  return (new InvertUsingBuiltInOperators<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& InvertUsingTemplateFunctions<T>::Invert(T& x)
{
  ::Invert(x);
  return x;
}

/****************************************************************************/
template <class T>
InvertableTraits<T>* InvertUsingTemplateFunctions<T>::Clone() const
{
  return (new InvertUsingTemplateFunctions<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
InvertUsingUserDefinedFunction<T>::InvertUsingUserDefinedFunction(T&(*FnPtr_)(T&)):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
void InvertUsingUserDefinedFunction<T>::SetFnPtr(T&(*FnPtr_)(T&))
{
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
T& InvertUsingUserDefinedFunction<T>::Invert(T& x)
{
  return ((*_FnPtr)(x));
}

/****************************************************************************/
template <class T>
InvertableTraits<T>* InvertUsingUserDefinedFunction<T>::Clone() const
{
  return (new InvertUsingUserDefinedFunction<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
InvertUsingUserDefinedFunctor<T>::InvertUsingUserDefinedFunctor(InvertableTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
InvertUsingUserDefinedFunctor<T>::InvertUsingUserDefinedFunctor(const InvertUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? Obj_._FnPtr->Clone():Obj_._FnPtr)
{}

/****************************************************************************/
template <class T>         
void InvertUsingUserDefinedFunctor<T>::SetFnPtr(InvertableTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
T& InvertUsingUserDefinedFunctor<T>::Invert(T& x)
{
  return _FnPtr->Invert(x);
}

/****************************************************************************/
template <class T>
InvertableTraits<T>* InvertUsingUserDefinedFunctor<T>::Clone() const
{
  return (new InvertUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
InvertUsingUserDefinedFunctor<T>::~InvertUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& IncrementUsingBuiltInOperators<T>::Increment(T& x)
{
  ++x;
  return x;
}

/****************************************************************************/
template <class T>
T& IncrementUsingBuiltInOperators<T>::Decrement(T& x)
{
  --x;
  return x;
}

/****************************************************************************/
template <class T>
OrdinalTraits<T>* IncrementUsingBuiltInOperators<T>::Clone() const
{
  return (new IncrementUsingBuiltInOperators<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& IncrementUsingTemplateFunctions<T>::Increment(T& x)
{
  ::Increment(x);
  return x;
}

/****************************************************************************/
template <class T>
T& IncrementUsingTemplateFunctions<T>::Decrement(T& x)
{
  ::Decrement(x);
  return x;
}

/****************************************************************************/
template <class T>
OrdinalTraits<T>* IncrementUsingTemplateFunctions<T>::Clone() const
{
  return (new IncrementUsingTemplateFunctions<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
IncrementUsingUserDefinedFunctor<T>::IncrementUsingUserDefinedFunctor(OrdinalTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
IncrementUsingUserDefinedFunctor<T>::IncrementUsingUserDefinedFunctor(const IncrementUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? Obj_._FnPtr->Clone():Obj_._FnPtr)
{}

/****************************************************************************/
template <class T>         
void IncrementUsingUserDefinedFunctor<T>::SetFnPtr(OrdinalTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
T& IncrementUsingUserDefinedFunctor<T>::Increment(T& x)
{
  return _FnPtr->Increment(x);
}

/****************************************************************************/
template <class T>
T& IncrementUsingUserDefinedFunctor<T>::Decrement(T& x)
{
  return _FnPtr->Decrement(x);
}

/****************************************************************************/
template <class T>
OrdinalTraits<T>* IncrementUsingUserDefinedFunctor<T>::Clone() const
{
  return (new IncrementUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
IncrementUsingUserDefinedFunctor<T>::~IncrementUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& SumUsingBuiltInOperators<T>::Add(T& x, const T& y)
{
  x += y;
  return x;
}

/****************************************************************************/
template <class T>
T& SumUsingBuiltInOperators<T>::Minus(T& x, const T& y)
{
  x -= y;
  return x;
}

/****************************************************************************/
template <class T>
SummableTraits<T>* SumUsingBuiltInOperators<T>::Clone() const
{
  return (new SumUsingBuiltInOperators<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& SumUsingTemplateFunctions<T>::Add(T& x, const T& y)
{
  ::Add(x, y);
  return x;
}

/****************************************************************************/
template <class T>
T& SumUsingTemplateFunctions<T>::Minus(T& x, const T& y)
{
  ::Minus(x, y);
  return x;
}

/****************************************************************************/
template <class T>
SummableTraits<T>* SumUsingTemplateFunctions<T>::Clone() const
{
  return (new SumUsingTemplateFunctions<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
SumUsingUserDefinedFunctor<T>::SumUsingUserDefinedFunctor(SummableTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
SumUsingUserDefinedFunctor<T>::SumUsingUserDefinedFunctor(const SumUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? Obj_._FnPtr->Clone():Obj_._FnPtr)
{}

/****************************************************************************/
template <class T>         
void SumUsingUserDefinedFunctor<T>::SetFnPtr(SummableTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
T& SumUsingUserDefinedFunctor<T>::Add(T& x, const T& y)
{
  return _FnPtr->Add(x, y);
}

/****************************************************************************/
template <class T>
T& SumUsingUserDefinedFunctor<T>::Minus(T& x, const T& y)
{
  return _FnPtr->Minus(x, y);
}

/****************************************************************************/
template <class T>
SummableTraits<T>* SumUsingUserDefinedFunctor<T>::Clone() const
{
  return (new SumUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
SumUsingUserDefinedFunctor<T>::~SumUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& MultiplyUsingBuiltInOperators<T>::Multiply(T& x, const T& y)
{
  x *= y;
  return x;
}

/****************************************************************************/
template <class T>
T& MultiplyUsingBuiltInOperators<T>::Divide(T& x, const T& y)
{
  x /= y;
  return x;
}

/****************************************************************************/
template <class T>
MultipliableTraits<T>* MultiplyUsingBuiltInOperators<T>::Clone() const
{
  return (new MultiplyUsingBuiltInOperators<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& MultiplyUsingTemplateFunctions<T>::Multiply(T& x, const T& y)
{
  ::Multiply(x, y);
  return x;
}

/****************************************************************************/
template <class T>
T& MultiplyUsingTemplateFunctions<T>::Divide(T& x, const T& y)
{
  ::Divide(x, y);
  return x;
}

/****************************************************************************/
template <class T>
MultipliableTraits<T>* MultiplyUsingTemplateFunctions<T>::Clone() const
{
  return (new MultiplyUsingTemplateFunctions<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
MultiplyUsingUserDefinedFunctor<T>::MultiplyUsingUserDefinedFunctor(MultipliableTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
MultiplyUsingUserDefinedFunctor<T>::MultiplyUsingUserDefinedFunctor(const MultiplyUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? Obj_._FnPtr->Clone():Obj_._FnPtr)
{}

/****************************************************************************/
template <class T>         
void MultiplyUsingUserDefinedFunctor<T>::SetFnPtr(MultipliableTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
T& MultiplyUsingUserDefinedFunctor<T>::Multiply(T& x, const T& y)
{
  return _FnPtr->Multiply(x, y);
}

/****************************************************************************/
template <class T>
T& MultiplyUsingUserDefinedFunctor<T>::Divide(T& x, const T& y)
{
  return _FnPtr->Divide(x, y);
}

/****************************************************************************/
template <class T>
MultipliableTraits<T>* MultiplyUsingUserDefinedFunctor<T>::Clone() const
{
  return (new MultiplyUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
MultiplyUsingUserDefinedFunctor<T>::~MultiplyUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& ModuloUsingBuiltInOperators<T>::Multiply(T& x, const T& y)
{
  x *= y;
  return x;
}

/****************************************************************************/
template <class T>
T& ModuloUsingBuiltInOperators<T>::Modulo(T& x, const T& y)
{
  x %= y;
  return x;
}

/****************************************************************************/
template <class T>
T& ModuloUsingBuiltInOperators<T>::Divide(T& x, const T& y)
{
  x /= y;
  return x;
}

/****************************************************************************/
template <class T>
ModuloTraits<T>* ModuloUsingBuiltInOperators<T>::Clone() const
{
  return (new ModuloUsingBuiltInOperators<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
T& ModuloUsingTemplateFunctions<T>::Multiply(T& x, const T& y)
{
  ::Multiply(x, y);
  return x;
}

/****************************************************************************/
template <class T>
T& ModuloUsingTemplateFunctions<T>::Modulo(T& x, const T& y)
{
  ::Modulo(x, y);
  return x;
}

/****************************************************************************/
template <class T>
T& ModuloUsingTemplateFunctions<T>::Divide(T& x, const T& y)
{
  ::Divide(x, y);
  return x;
}

/****************************************************************************/
template <class T>
ModuloTraits<T>* ModuloUsingTemplateFunctions<T>::Clone() const
{
  return (new ModuloUsingTemplateFunctions<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
ModuloUsingUserDefinedFunctor<T>::ModuloUsingUserDefinedFunctor(ModuloTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
ModuloUsingUserDefinedFunctor<T>::ModuloUsingUserDefinedFunctor(const ModuloUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? Obj_._FnPtr->Clone():Obj_._FnPtr)
{}

/****************************************************************************/
template <class T>         
void ModuloUsingUserDefinedFunctor<T>::SetFnPtr(ModuloTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
T& ModuloUsingUserDefinedFunctor<T>::Multiply(T& x, const T& y)
{
  return _FnPtr->Multiply(x, y);
}

/****************************************************************************/
template <class T>
T& ModuloUsingUserDefinedFunctor<T>::Modulo(T& x, const T& y)
{
  return _FnPtr->Modulo(x, y);
}

/****************************************************************************/
template <class T>
T& ModuloUsingUserDefinedFunctor<T>::Divide(T& x, const T& y)
{
  return _FnPtr->Divide(x, y);
}

/****************************************************************************/
template <class T>
ModuloTraits<T>* ModuloUsingUserDefinedFunctor<T>::Clone() const
{
  return (new ModuloUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
ModuloUsingUserDefinedFunctor<T>::~ModuloUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean MatchUsingBuiltInOperators<T>::Equal(const T& x, const T& y) const
{
  return (x == y);
}

/****************************************************************************/
template <class T>
Boolean MatchUsingBuiltInOperators<T>::NotEqual(const T& x, const T& y) const
{
  return (x != y);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* MatchUsingBuiltInOperators<T>::Clone() const
{
  return (new MatchUsingBuiltInOperators<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean MatchUsingTemplateFunctions<T>::Equal(const T& x, const T& y) const
{
  return IsEqualTo(x, y);
}

/****************************************************************************/
template <class T>
Boolean MatchUsingTemplateFunctions<T>::NotEqual(const T& x, const T& y) const
{
  return !IsEqualTo(x, y);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* MatchUsingTemplateFunctions<T>::Clone() const
{
  return (new MatchUsingTemplateFunctions<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
MatchUsingUserDefinedFunction<T>::MatchUsingUserDefinedFunction(Boolean(*FnPtr_)(const T&, const T&)):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
void MatchUsingUserDefinedFunction<T>::SetFnPtr(Boolean(*FnPtr_)(const T&, const T&))
{
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
Boolean MatchUsingUserDefinedFunction<T>::Equal(const T& x, const T& y) const
{
  return ((*_FnPtr)(x, y) == 0);
}

/****************************************************************************/
template <class T>
Boolean MatchUsingUserDefinedFunction<T>::NotEqual(const T& x, const T& y) const
{
  return ((*_FnPtr)(x, y) != 0);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* MatchUsingUserDefinedFunction<T>::Clone() const
{
  return (new MatchUsingUserDefinedFunction<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
MatchUsingUserDefinedFunctor<T>::MatchUsingUserDefinedFunctor(MatchableTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
MatchUsingUserDefinedFunctor<T>::MatchUsingUserDefinedFunctor(const MatchUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? Obj_._FnPtr->Clone():Obj_._FnPtr)
{}

/****************************************************************************/
template <class T>         
void MatchUsingUserDefinedFunctor<T>::SetFnPtr(MatchableTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
Boolean MatchUsingUserDefinedFunctor<T>::Equal(const T& x, const T& y) const
{
  return _FnPtr->Equal(x, y);
}

/****************************************************************************/
template <class T>
Boolean MatchUsingUserDefinedFunctor<T>::NotEqual(const T& x, const T& y) const
{
  return _FnPtr->NotEqual(x, y);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* MatchUsingUserDefinedFunctor<T>::Clone() const
{
  return (new MatchUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
MatchUsingUserDefinedFunctor<T>::~MatchUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean SortUsingBuiltInOperators<T>::Equal(const T& x, const T& y) const
{
  return (x == y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingBuiltInOperators<T>::NotEqual(const T& x, const T& y) const
{
  return (x != y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingBuiltInOperators<T>::LessThan(const T& x, const T& y) const
{
  return (x < y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingBuiltInOperators<T>::GreaterThan(const T& x, const T& y) const
{
  return (x > y);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* SortUsingBuiltInOperators<T>::Clone() const
{
  return (new SortUsingBuiltInOperators<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
Boolean SortUsingTemplateFunctions<T>::Equal(const T& x, const T& y) const
{
  return IsEqualTo(x, y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingTemplateFunctions<T>::NotEqual(const T& x, const T& y) const
{
  return !IsEqualTo(x, y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingTemplateFunctions<T>::LessThan(const T& x, const T& y) const
{
  return IsLessThan(x, y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingTemplateFunctions<T>::GreaterThan(const T& x, const T& y) const
{
  return IsGreaterThan(x, y);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* SortUsingTemplateFunctions<T>::Clone() const
{
  return (new SortUsingTemplateFunctions<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
SortUsingUserDefinedFunction<T>::SortUsingUserDefinedFunction(Boolean(*FnPtr_)(const T&, const T&)):
_FnPtr(FnPtr_)
{}

/****************************************************************************/         
template <class T>
void SortUsingUserDefinedFunction<T>::SetFnPtr(Boolean(*FnPtr_)(const T&, const T&))
{
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunction<T>::Equal(const T& x, const T& y) const
{
  return ((*_FnPtr)(x, y) == 0);
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunction<T>::NotEqual(const T& x, const T& y) const
{
  return ((*_FnPtr)(x, y) != 0);
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunction<T>::LessThan(const T& x, const T& y) const
{
  return ((*_FnPtr)(x, y) < 0);
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunction<T>::GreaterThan(const T& x, const T& y) const
{
  return ((*_FnPtr)(x, y) > 0);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* SortUsingUserDefinedFunction<T>::Clone() const
{
  return (new SortUsingUserDefinedFunction<T>(*this));
}

/****************************************************************************/
/****************************************************************************/
template <class T>
SortUsingUserDefinedFunctor<T>::SortUsingUserDefinedFunctor(SortableTraits<T>* FnPtr_):
_FnPtr(FnPtr_)
{}

/****************************************************************************/
template <class T>         
SortUsingUserDefinedFunctor<T>::SortUsingUserDefinedFunctor(const SortUsingUserDefinedFunctor<T>& Obj_):
_FnPtr(Obj_._FnPtr ? ((SortableTraits<T>*)Obj_._FnPtr->Clone()):((SortableTraits<T>*)Obj_._FnPtr))
{}

/****************************************************************************/
template <class T>         
void SortUsingUserDefinedFunctor<T>::SetFnPtr(SortableTraits<T>* FnPtr_)
{
  delete _FnPtr;
  _FnPtr = FnPtr_;
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunctor<T>::Equal(const T& x, const T& y) const
{
  return _FnPtr->Equal(x, y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunctor<T>::NotEqual(const T& x, const T& y) const
{
  return _FnPtr->NotEqual(x, y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunctor<T>::LessThan(const T& x, const T& y) const
{
  return _FnPtr->LessThan(x, y);
}

/****************************************************************************/
template <class T>
Boolean SortUsingUserDefinedFunctor<T>::GreaterThan(const T& x, const T& y) const
{
  return _FnPtr->GreaterThan(x, y);
}

/****************************************************************************/
template <class T>
MatchableTraits<T>* SortUsingUserDefinedFunctor<T>::Clone() const
{
  return (new SortUsingUserDefinedFunctor<T>(*this));
}

/****************************************************************************/
template <class T>
SortUsingUserDefinedFunctor<T>::~SortUsingUserDefinedFunctor()
{
  delete _FnPtr;
  _FnPtr = NULL;
}

/****************************************************************************/
#endif




