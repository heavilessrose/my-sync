#ifndef COMPARE_H
#define COMPARE_H
#ifndef TRAITS_CPP
  #include "traits.cpp"
#endif

#define COMPARE_DEBUG	0
#define COMPARE_DEBUG1	0
#define COMPARE_DEBUG2	0

/****************************************************************************/
template <class T>
Boolean CompareWith(const T& obj1, const T& obj2, int RelEnum_, SortableTraits<T>* Trait_)
{
  return
  (
    (RelEnum_ == EQUAL) 		? Trait_.Equal(obj1, obj2):
    (RelEnum_ == NOT_EQUAL) 		? Trait_.NotEqual(obj1, obj2):
    (RelEnum_ == LESS_THAN) 		? Trait_.LessThan(obj1, obj2):
    (RelEnum_ == GREATER_THAN) 		? Trait_.GreaterThan(obj1, obj2):
    (RelEnum_ == LESS_THAN_OR_EQUAL) 	? (Trait_.LessThan(obj1, obj2) ||
					   Trait_.Equal(obj1, obj2)):
    (RelEnum_ == GREATER_THAN_OR_EQUAL) ? (Trait_.GreaterThan(obj1, obj2) ||
					   Trait_.Equal(obj1, obj2_)):
					  FALSE
  );
}

/****************************************************************************/
template <class T>
Boolean MatchWith(const T& obj1, const T& obj2, int RelEnum_, MatchableTraits<T>* Trait_)
{
  return
  (
    (RelEnum_ == EQUAL) 		? Trait_.Equal(obj1, obj2):
    (RelEnum_ == NOT_EQUAL) 		? Trait_.NotEqual(obj1, obj2):
					  FALSE
  );
}

/****************************************************************************/
// Abstract base class for pointer contents comparison functors
//
template <class T>
class PtrCompare
{
  protected:
    virtual BOOLTYPE Compare(const T& Obj1_, const T& Obj2_) const	= 0;

  public:
    virtual operator BOOLTYPE ()	const				= 0;
    virtual BOOLTYPE operator () (const T* Ptr1_, const T* Ptr2_)	= 0;

    virtual PtrCompare<T>& Relation(int)
	{ return *this; }
    virtual PtrCompare<T>& Invert()
	{ return *this; }
};

template <class T>
class ObjCompare
{
  public:
    virtual operator BOOLTYPE () const					= 0;
    virtual BOOLTYPE operator () (const T& Ptr1_, const T& Ptr2_)	= 0;

    virtual ObjCompare<T>& Relation(int)
	{ return *this; }
    virtual ObjCompare<T>& Invert()
	{ return *this; }
};

/****************************************************************************/
// Implementation base class for pointer contents comparison functors
//
template <class T>
class PtrCompareImp : virtual public PtrCompare<T>
{
  protected:
    const T* _Pointer1;
    const T* _Pointer2;

    virtual BOOLTYPE Compare(const T& Obj1_, const T& Obj2_) const
	{ return FALSE; }

  public:
    PtrCompareImp(const T* Ptr1_, const T* Ptr2_);

    virtual operator BOOLTYPE () const;
    virtual BOOLTYPE operator () (const T* Ptr1_, const T* Ptr2_);
};

/****************************************************************************/
// Functor which returns TRUE if both pointers contain equivalent contents
// otherwise returns FALSE.
//
template <class T>
class PtrRelation;

template <class T>
class PtrEqual : virtual public PtrCompare<T>, private PtrCompareImp<T>
{
  protected:
    int _Comp;
    MatchableTraits<T>* _Comparer;

    virtual BOOLTYPE Compare(const T& Obj1_, const T& Obj2_) const;

  public:
    PtrEqual();
    PtrEqual(MatchableTraits<T>* MatchTraits_);
    PtrEqual(const T* Ptr1_, const T* Ptr2_, MatchableTraits<T>* MatchTraits_);
    PtrEqual(const PtrEqual<T>& Obj_);
    ~PtrEqual();

    virtual operator BOOLTYPE () const;
    virtual BOOLTYPE operator () (const T* Ptr1_, const T* Ptr2_);
    virtual PtrEqual<T>& operator = (const PtrEqual<T>& Obj_);

    virtual PtrCompare<T>& Invert();
};

template <class T>
class ObjEqual : public ObjCompare<T>
{
  protected:
    PtrEqual<T> _Comparer;

  public:
    ObjEqual();
    ObjEqual(MatchableTraits<T>* MatchTraits_);
    ObjEqual(const T& Obj1_, const T& Obj2_, MatchableTraits<T>* MatchTraits_);

    virtual operator BOOLTYPE () const;
    virtual BOOLTYPE operator () (const T& Obj1_, const T& Obj2_);

    virtual ObjCompare<T>& Invert();
};

/****************************************************************************/
// Functor which returns TRUE if contents of both pointers agree with the
// comparison operation done on them, otherwise returns FALSE.
//
template <class T>
class PtrRelation : virtual public PtrCompare<T>, private PtrCompareImp<T>
{
  protected:
    int _Comp;
    SortableTraits<T>* _Comparer;

    virtual BOOLTYPE Compare(const T& Obj1_, const T& Obj2_) const;

  public:
    PtrRelation();
    PtrRelation(SortableTraits<T>* SortTraits_);
    PtrRelation(int Comp_, SortableTraits<T>* SortTraits_);
    PtrRelation(const T* Ptr1_, int Comp_, const T* Ptr2_, SortableTraits<T>* SortTraits_);    
    PtrRelation(const PtrRelation<T>& Obj_);
    ~PtrRelation();

    virtual operator BOOLTYPE () const;
    virtual BOOLTYPE operator () (const T* Ptr1_, const T* Ptr2_);
    virtual PtrRelation<T>& operator = (const PtrRelation<T>& Obj_);

    virtual PtrCompare<T>& Relation(int Comp_);
    virtual PtrCompare<T>& Invert();

    static const T* Min(const T* x1, const T* x2, SortableTraits<T>* SortTraits_=NULL);
    static const T* Max(const T* x1, const T* x2, SortableTraits<T>* SortTraits_=NULL);

    static BOOLTYPE InRange(const T* x, const T* min, const T* max, SortableTraits<T>* SortTraits_=NULL);
    static BOOLTYPE XInRange(const T* x, const T* min, const T* max, SortableTraits<T>* SortTraits_=NULL);
};

template <class T>
class ObjRelation : public ObjCompare<T>
{
  protected:
    PtrRelation<T> _Comparer;

  public:
    ObjRelation();
    ObjRelation(SortableTraits<T>* SortTraits_);
    ObjRelation(int Comp_, SortableTraits<T>* SortTraits_);
    ObjRelation(const T& Obj1_, int Comp_, const T& Obj2_, SortableTraits<T>* SortTraits_);

    virtual operator BOOLTYPE () const;
    virtual BOOLTYPE operator () (const T& Ptr1_, const T& Ptr2_);

    virtual ObjCompare<T>& Relation(int Comp_);
    virtual ObjCompare<T>& Invert();

    static const T& Min(const T& x1, const T& x2, SortableTraits<T>* SortTraits_=NULL);
    static const T& Max(const T& x1, const T& x2, SortableTraits<T>* SortTraits_=NULL);

    static BOOLTYPE InRange(const T& x, const T& min, const T& max, SortableTraits<T>* SortTraits_=NULL);
    static BOOLTYPE XInRange(const T& x, const T& min, const T& max, SortableTraits<T>* SortTraits_=NULL);
};


#if COMPARE_DEBUG
struct Dumstruct
{ DEFAULT_ROOTOBJECT_DEFN(Dumstruct) };

class Somestruct
{
  DEFAULT_ROOTOBJECT_DEFN(Somestruct)

  friend Boolean _IsMatchable(const Somestruct& Obj_)
      { return TRUE; }
  friend Boolean _IsEqualTo(const Somestruct& Src_, const Somestruct& Trg_)
      { return (Src_.xval == Trg_.xval); }
  
  public:
    int xval;
    void Set(int x_) { xval = x_; }
};
#endif

/****************************************************************************/
#endif




