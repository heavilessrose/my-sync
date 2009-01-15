#ifndef TRAITS_H
#define TRAITS_H
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif

/****************************************************************************/
template <class T>
class SummableTraits
{
  public:
    virtual SummableTraits<T>* Clone() const = 0;
    virtual T& Minus(T& Trg_, const T& Src_) = 0;
    virtual T& Add(T& Trg_, const T& Src_) = 0;
};

template <class T>
class SumUsingBuiltInOperators : public SummableTraits<T>
{
  private:
    SumUsingBuiltInOperators(){}

  public:
    virtual SummableTraits<T>* Clone() const;
    virtual T& Minus(T& Trg_, const T& Src_);
    virtual T& Add(T& Trg_, const T& Src_);

    static SummableTraits<T>* Trait()
	{ return (new SumUsingBuiltInOperators<T>); }
};

template <class T>
class SumUsingTemplateFunctions : public SummableTraits<T>
{
  private:
    SumUsingTemplateFunctions(){}

  public:
    virtual SummableTraits<T>* Clone() const;
    virtual T& Minus(T& Trg_, const T& Src_);
    virtual T& Add(T& Trg_, const T& Src_);

    static SummableTraits<T>* Trait()
	{ return (new SumUsingTemplateFunctions<T>); }
};

template <class T>
class SumUsingUserDefinedFunctor : public SummableTraits<T>
{
  friend class SummableTraits<T>;

  private:
    SummableTraits<T>* _FnPtr;

    SumUsingUserDefinedFunctor(SummableTraits<T>* FnPtr_);
    SumUsingUserDefinedFunctor(const SumUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(SummableTraits<T>* FnPtr_);

  public:  
    virtual ~SumUsingUserDefinedFunctor();
    virtual SummableTraits<T>* Clone() const;
    virtual T& Minus(T& Trg_, const T& Src_);
    virtual T& Add(T& Trg_, const T& Src_);

    static SummableTraits<T>* Trait(SummableTraits<T>* FnPtr_)
	{ SumUsingUserDefinedFunctor<T> Instance_ =
          new SumUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
template <class T>
class MultipliableTraits
{
  public:
    virtual MultipliableTraits<T>* Clone() const = 0;
    virtual T& Divide(T& Trg_, const T& Src_) = 0;
    virtual T& Multiply(T& Trg_, const T& Src_) = 0;
};

template <class T>
class MultiplyUsingBuiltInOperators : public MultipliableTraits<T>
{
  private:
    MultiplyUsingBuiltInOperators(){}

  public:
    virtual MultipliableTraits<T>* Clone() const;
    virtual T& Divide(T& Trg_, const T& Src_);
    virtual T& Multiply(T& Trg_, const T& Src_);

    static MultipliableTraits<T>* Trait()
	{ return (new MultiplyUsingBuiltInOperators<T>); }
};

template <class T>
class MultiplyUsingTemplateFunctions : public MultipliableTraits<T>
{
  private:
    MultiplyUsingTemplateFunctions(){}

  public:
    virtual MultipliableTraits<T>* Clone() const;
    virtual T& Divide(T& Trg_, const T& Src_);
    virtual T& Multiply(T& Trg_, const T& Src_);

    static MultipliableTraits<T>* Trait()
	{ return (new MultiplyUsingTemplateFunctions<T>); }
};

template <class T>
class MultiplyUsingUserDefinedFunctor : public MultipliableTraits<T>
{
  friend class MultipliableTraits<T>;

  private:
    MultipliableTraits<T>* _FnPtr;

    MultiplyUsingUserDefinedFunctor(MultipliableTraits<T>* FnPtr_);
    MultiplyUsingUserDefinedFunctor(const MultiplyUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(MultipliableTraits<T>* FnPtr_);

  public:  
    virtual ~MultiplyUsingUserDefinedFunctor();
    virtual MultipliableTraits<T>* Clone() const;
    virtual T& Divide(T& Trg_, const T& Src_);
    virtual T& Multiply(T& Trg_, const T& Src_);

    static MultipliableTraits<T>* Trait(MultipliableTraits<T>* FnPtr_)
	{ MultiplyUsingUserDefinedFunctor<T> Instance_ =
          new MultiplyUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
template <class T>
class ModuloTraits : public MultipliableTraits<T>
{
  public:
    virtual ModuloTraits<T>* Clone() const = 0;
    virtual T& Modulo(T& Trg_, const T& Src_) = 0;
};

template <class T>
class ModuloUsingBuiltInOperators : public ModuloTraits<T>
{
  private:
    ModuloUsingBuiltInOperators(){}

  public:
    virtual ModuloTraits<T>* Clone() const;
    virtual T& Divide(T& Trg_, const T& Src_);
    virtual T& Multiply(T& Trg_, const T& Src_);    
    virtual T& Modulo(T& Trg_, const T& Src_);

    static ModuloTraits<T>* Trait()
	{ return (new ModuloUsingBuiltInOperators<T>); }
};

template <class T>
class ModuloUsingTemplateFunctions : public ModuloTraits<T>
{
  private:
    ModuloUsingTemplateFunctions(){}

  public:
    virtual ModuloTraits<T>* Clone() const;
    virtual T& Divide(T& Trg_, const T& Src_);
    virtual T& Multiply(T& Trg_, const T& Src_);        
    virtual T& Modulo(T& Trg_, const T& Src_);

    static ModuloTraits<T>* Trait()
	{ return (new ModuloUsingTemplateFunctions<T>); }
};

template <class T>
class ModuloUsingUserDefinedFunctor : public ModuloTraits<T>
{
  friend class ModuloTraits<T>;

  private:
    ModuloTraits<T>* _FnPtr;

    ModuloUsingUserDefinedFunctor(ModuloTraits<T>* FnPtr_);
    ModuloUsingUserDefinedFunctor(const ModuloUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(ModuloTraits<T>* FnPtr_);

  public:  
    virtual ~ModuloUsingUserDefinedFunctor();
    virtual ModuloTraits<T>* Clone() const;
    virtual T& Divide(T& Trg_, const T& Src_);
    virtual T& Multiply(T& Trg_, const T& Src_);            
    virtual T& Modulo(T& Trg_, const T& Src_);

    static ModuloTraits<T>* Trait(ModuloTraits<T>* FnPtr_)
	{ ModuloUsingUserDefinedFunctor<T> Instance_ =
          new ModuloUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
template <class T>
class OrdinalTraits
{
  public:
    virtual OrdinalTraits<T>* Clone() const = 0;
    virtual T& Increment(T& x) = 0;
    virtual T& Decrement(T& x) = 0;
};

template <class T>
class IncrementUsingBuiltInOperators : public OrdinalTraits<T>
{
  private:
    IncrementUsingBuiltInOperators(){}

  public:
    virtual OrdinalTraits<T>* Clone() const;
    virtual T& Increment(T& x);
    virtual T& Decrement(T& x);

    static OrdinalTraits<T>* Trait()
	{ return (new IncrementUsingBuiltInOperators<T>); }
};

template <class T>
class IncrementUsingTemplateFunctions : public OrdinalTraits<T>
{
  private:
    IncrementUsingTemplateFunctions(){}

  public:
    virtual OrdinalTraits<T>* Clone() const;
    virtual T& Increment(T& x);
    virtual T& Decrement(T& x);

    static OrdinalTraits<T>* Trait()
	{ return (new IncrementUsingTemplateFunctions<T>); }
};

template <class T>
class IncrementUsingUserDefinedFunctor : public OrdinalTraits<T>
{
  friend class OrdinalTraits<T>;

  private:
    OrdinalTraits<T>* _FnPtr;

    IncrementUsingUserDefinedFunctor(OrdinalTraits<T>* FnPtr_);
    IncrementUsingUserDefinedFunctor(const IncrementUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(OrdinalTraits<T>* FnPtr_);

  public:  
    virtual ~IncrementUsingUserDefinedFunctor();
    virtual OrdinalTraits<T>* Clone() const;  
    virtual T& Increment(T& x);
    virtual T& Decrement(T& x);

    static OrdinalTraits<T>* Trait(OrdinalTraits<T>* FnPtr_)
	{ IncrementUsingUserDefinedFunctor<T> Instance_ =
          new IncrementUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
template <class T>
class InvertableTraits
{
  public:
    virtual InvertableTraits<T>* Clone() const = 0;
    virtual T& Invert(T& x) = 0;
};

template <class T>
class InvertUsingBuiltInOperators : public InvertableTraits<T>
{
  private:
    InvertUsingBuiltInOperators(){}

  public:
    virtual InvertableTraits<T>* Clone() const;
    virtual T& Invert(T& x);

    static InvertableTraits<T>* Trait()
	{ return (new InvertUsingBuiltInOperators<T>); }
};

template <class T>
class InvertUsingTemplateFunctions : public InvertableTraits<T>
{
  private:
    InvertUsingTemplateFunctions(){}

  public:
    virtual InvertableTraits<T>* Clone() const;
    virtual T& Invert(T& x);

    static InvertableTraits<T>* Trait()
	{ return (new InvertUsingTemplateFunctions<T>); }
};

template <class T>
class InvertUsingUserDefinedFunction : public InvertableTraits<T>
{
  friend class InvertableTraits<T>;

  private:
    T&(*_FnPtr)(T&);
  
    InvertUsingUserDefinedFunction(T&(*FnPtr_)(T&));
    void SetFnPtr(T&(*FnPtr_)(T&));

  public:
    virtual InvertableTraits<T>* Clone() const;  
    virtual T& Invert(T& x);

    static InvertableTraits<T>* Trait(T&(*FnPtr_)(T&))
	{ InvertUsingUserDefinedFunction<T> Instance_ =
          new InvertUsingUserDefinedFunction<T>(FnPtr_);
	  return Instance_; }
};

template <class T>
class InvertUsingUserDefinedFunctor : public InvertableTraits<T>
{
  friend class InvertableTraits<T>;

  private:
    InvertableTraits<T>* _FnPtr;

    InvertUsingUserDefinedFunctor(InvertableTraits<T>* FnPtr_);
    InvertUsingUserDefinedFunctor(const InvertUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(InvertableTraits<T>* FnPtr_);

  public:  
    virtual ~InvertUsingUserDefinedFunctor();
    virtual InvertableTraits<T>* Clone() const;  
    virtual T& Invert(T& x);

    static InvertableTraits<T>* Trait(InvertableTraits<T>* FnPtr_)
	{ InvertUsingUserDefinedFunctor<T> Instance_ =
          new InvertUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
template <class T>
class MatchableTraits
{
  public:
    virtual MatchableTraits<T>* Clone() const = 0;
    virtual Boolean NotEqual(const T& x, const T& y) const = 0;
    virtual Boolean Equal(const T& x, const T& y) const = 0;
};

template <class T>
class MatchUsingBuiltInOperators : public MatchableTraits<T>
{
  private:
    MatchUsingBuiltInOperators(){}

  public:
    virtual MatchableTraits<T>* Clone() const;
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean Equal(const T& x, const T& y) const;

    static MatchableTraits<T>* Trait()
	{ return (new MatchUsingBuiltInOperators<T>); }
};

template <class T>
class MatchUsingTemplateFunctions : public MatchableTraits<T>
{
  private:
    MatchUsingTemplateFunctions(){}

  public:
    virtual MatchableTraits<T>* Clone() const;
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean Equal(const T& x, const T& y) const;

    static MatchableTraits<T>* Trait()
	{ return (new MatchUsingTemplateFunctions<T>); }
};

template <class T>
class MatchUsingUserDefinedFunction : public MatchableTraits<T>
{
  friend class MatchableTraits<T>;

  private:
    Boolean(*_FnPtr)(const T&, const T&);
  
    MatchUsingUserDefinedFunction(Boolean(*FnPtr_)(const T&, const T&));
    void SetFnPtr(Boolean(*FnPtr_)(const T&, const T&));

  public:
    virtual MatchableTraits<T>* Clone() const;  
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean Equal(const T& x, const T& y) const;

    static MatchableTraits<T>* Trait(Boolean(*FnPtr_)(const T&, const T&))
	{ MatchUsingUserDefinedFunction<T> Instance_ =
          new MatchUsingUserDefinedFunction<T>(FnPtr_);
	  return Instance_; }
};

template <class T>
class MatchUsingUserDefinedFunctor : public MatchableTraits<T>
{
  friend class MatchableTraits<T>;

  private:
    MatchableTraits<T>* _FnPtr;

    MatchUsingUserDefinedFunctor(MatchableTraits<T>* FnPtr_);
    MatchUsingUserDefinedFunctor(const MatchUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(MatchableTraits<T>* FnPtr_);

  public:  
    virtual ~MatchUsingUserDefinedFunctor();  
    virtual MatchableTraits<T>* Clone() const;  
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean Equal(const T& x, const T& y) const;

    static MatchableTraits<T>* Trait(MatchableTraits<T>* FnPtr_)
	{ MatchUsingUserDefinedFunctor<T> Instance_ =
          new MatchUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
template <class T>
class SortableTraits : public MatchableTraits<T>
{
  public:
    virtual MatchableTraits<T>* Clone() const = 0;
    virtual Boolean LessThan(const T& x, const T& y) const = 0;
    virtual Boolean GreaterThan(const T& x, const T& y) const = 0;
};

template <class T>
class SortUsingBuiltInOperators : public SortableTraits<T>
{
  private:
    SortUsingBuiltInOperators(){}

  public:
    virtual MatchableTraits<T>* Clone() const;
    virtual Boolean Equal(const T& x, const T& y) const;
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean LessThan(const T& x, const T& y) const;
    virtual Boolean GreaterThan(const T& x, const T& y) const;

    static SortableTraits<T>* Trait()
	{ return (new SortUsingBuiltInOperators<T>); }
};

template <class T>
class SortUsingTemplateFunctions : public SortableTraits<T>
{
  private:
    SortUsingTemplateFunctions(){}

  public:
    virtual MatchableTraits<T>* Clone() const;
    virtual Boolean Equal(const T& x, const T& y) const;
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean LessThan(const T& x, const T& y) const;
    virtual Boolean GreaterThan(const T& x, const T& y) const;

    static SortableTraits<T>* Trait()
	{ return (new SortUsingTemplateFunctions<T>); }
};

template <class T>
class SortUsingUserDefinedFunction : public SortableTraits<T>
{
  friend class SortableTraits<T>;

  private:
    Boolean(*_FnPtr)(const T&, const T&);
    
    SortUsingUserDefinedFunction(Boolean(*FnPtr_)(const T&, const T&));
    void SetFnPtr(Boolean(*FnPtr_)(const T&, const T&));

  public:
    virtual MatchableTraits<T>* Clone() const;
    virtual Boolean Equal(const T& x, const T& y) const;
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean LessThan(const T& x, const T& y) const;
    virtual Boolean GreaterThan(const T& x, const T& y) const;

    static SortableTraits<T>* Trait(Boolean(*FnPtr_)(const T&, const T&))
	{ SortUsingUserDefinedFunction<T>* Instance_ =
          new SortUsingUserDefinedFunction<T>(FnPtr_);
	  return Instance_; }
};

template <class T>
class SortUsingUserDefinedFunctor : public SortableTraits<T>
{
  friend class SortableTraits<T>;

  private:
    SortableTraits<T>* _FnPtr;

    SortUsingUserDefinedFunctor(SortableTraits<T>* FnPtr_);
    SortUsingUserDefinedFunctor(const SortUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(SortableTraits<T>* FnPtr_);

  public:  
    virtual ~SortUsingUserDefinedFunctor();  
    virtual MatchableTraits<T>* Clone() const;
    virtual Boolean Equal(const T& x, const T& y) const;
    virtual Boolean NotEqual(const T& x, const T& y) const;
    virtual Boolean LessThan(const T& x, const T& y) const;
    virtual Boolean GreaterThan(const T& x, const T& y) const;

    static SortableTraits<T>* Trait(SortableTraits<T>* FnPtr_)
	{ SortUsingUserDefinedFunctor<T>* Instance_ =
          new SortUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
template <class T>
class StreamableTraits
{
  public:
    virtual StreamableTraits<T>* Clone() const = 0;
    virtual T& Write(ostream& Os_, const T& Src_) = 0;
    virtual T& Read(istream& Is_, T& Trg_) = 0;
};

template <class T>
class StreamUsingBuiltInOperators : public StreamableTraits<T>
{
  private:
    StreamUsingBuiltInOperators(){}

  public:
    virtual StreamableTraits<T>* Clone() const;
    virtual T& Write(ostream& Os_, const T& Src_);
    virtual T& Read(istream& Is_, T& Trg_);

    static StreamableTraits<T>* Trait()
	{ return (new StreamUsingBuiltInOperators<T>); }
};

template <class T>
class StreamUsingTemplateFunctions : public StreamableTraits<T>
{
  private:
    StreamUsingTemplateFunctions(){}

  public:
    virtual StreamableTraits<T>* Clone() const;
    virtual T& Write(ostream& Os_, const T& Src_);
    virtual T& Read(istream& Is_, T& Trg_);

    static StreamableTraits<T>* Trait()
	{ return (new StreamUsingTemplateFunctions<T>); }
};

template <class T>
class StreamUsingUserDefinedFunctor : public StreamableTraits<T>
{
  friend class StreamableTraits<T>;

  private:
    StreamableTraits<T>* _FnPtr;

    StreamUsingUserDefinedFunctor(StreamableTraits<T>* FnPtr_);
    StreamUsingUserDefinedFunctor(const StreamUsingUserDefinedFunctor<T>& Obj_);
    void SetFnPtr(StreamableTraits<T>* FnPtr_);

  public:  
    virtual ~StreamUsingUserDefinedFunctor();
    virtual StreamableTraits<T>* Clone() const;
    virtual T& Write(ostream& Os_, const T& Src_);
    virtual T& Read(istream& Is_, T& Trg_);    

    static StreamableTraits<T>* Trait(StreamableTraits<T>* FnPtr_)
	{ StreamUsingUserDefinedFunctor<T> Instance_ =
          new StreamUsingUserDefinedFunctor<T>(FnPtr_);
	  return Instance_; }
};

/****************************************************************************/
#endif




