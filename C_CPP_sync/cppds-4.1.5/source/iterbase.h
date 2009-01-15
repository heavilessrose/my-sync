#ifndef ITERBASE_H
#define ITERBASE_H
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef LONEFALL_H
  #include "lonefall.h"
#endif
#ifndef STL_ITERATOR_H
  #include "stliter.h"
#endif

/****************************************************************************/
template <class T>
class RandomAccessIteratorProxy;

template <class T>
class RandomAccessIterator : public Random_Access_Iterator<T, size_t>
{
  typedef RandomAccessIterator<T> _Self;

  friend Boolean operator == (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator != (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);

  friend Boolean operator < (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator > (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator <= (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend Boolean operator >= (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);
  friend ptrdiff_t operator - (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2);

  protected:
    union
    {
      const RandomAccessIterator<T>* _ObjPtr;
      RandomAccessIterator<T>* _VarPtr;
    };

    void Xnull() const
	{ THROW (LoneFallibleBase::DerefNullPointerErr()); }

    // This method does the actual dispatching work
    // This isn't called directly, but is overwritten in the
    // envelope class. Since the dispatched methods were originally
    // designated as pure abstract, calling this dispatch method through
    // them is flagged as a fatal error because it means they were never
    // reimplemented in the derived classes as a "concrete" implementation.
    virtual RandomAccessIterator<T>* Dispatch();
    virtual const RandomAccessIterator<T>* Dispatch() const;

    virtual const void* Address() const
	{ return Dispatch()->Address(); }
    virtual size_t Position() const
	{ return Dispatch()->Position(); }

  public:
    RandomAccessIterator():
	_ObjPtr(this) {}
    RandomAccessIterator(const RandomAccessIterator<T>& Boss_):
	_ObjPtr(Boss_.GiveIterator().Clone()) {}
    RandomAccessIterator(const RandomAccessIterator<T>* Boss_):
	_ObjPtr(Boss_) {}

    virtual ~RandomAccessIterator()
	{ delete _VarPtr; }

    virtual RandomAccessIterator<T>* Clone()
	{ return (new RandomAccessIterator<T>(*this)); }
    virtual const RandomAccessIterator<T>* Clone() const
	{ return (new RandomAccessIterator<T>(*this)); }

    virtual Boolean IsValid(size_t Index_) const
	{ return Dispatch()->IsValid(Index_); }
    virtual Boolean IsValid() const
	{ return Dispatch()->IsValid(); }
    virtual Boolean More() const
	{ return Dispatch()->More(); }
    virtual void Reset() const
	{ Dispatch()->Reset(); }
    virtual void SetToEnd() const
	{ Dispatch()->SetToEnd(); }

    virtual T& operator * ()
	{ return Dispatch()->operator * (); }
    virtual const T& operator * () const
	{ return Dispatch()->operator * (); }
    virtual T& operator [] (size_t Index_)
	{ return Dispatch()->operator [] (Index_); }
    virtual const T& operator [] (size_t Index_) const
	{ return Dispatch()->operator [] (Index_); }

    virtual RandomAccessIterator<T>& operator ++ ()
	{ return Dispatch()->operator ++ (); }
    virtual RandomAccessIteratorProxy<T> operator ++ (int)
	{ return (*Dispatch())++; }

    virtual const RandomAccessIterator<T>& operator ++ () const
	{ return Dispatch()->operator ++ (); }
    virtual const RandomAccessIteratorProxy<T> operator ++ (int) const
	{ return (*Dispatch())++; }

    virtual RandomAccessIterator<T>& operator -- ()
	{ return Dispatch()->operator -- (); }
    virtual RandomAccessIteratorProxy<T> operator -- (int)
	{ return (*Dispatch())--; }

    virtual const RandomAccessIterator<T>& operator -- () const
	{ return Dispatch()->operator -- (); }
    virtual const RandomAccessIteratorProxy<T> operator -- (int) const
	{ return (*Dispatch())--; }

    virtual RandomAccessIteratorProxy<T> operator + (size_t Dist_)
	{ return Dispatch()->operator + (Dist_); }
    virtual const RandomAccessIteratorProxy<T> operator + (size_t Dist_) const
	{ return Dispatch()->operator + (Dist_); }
    virtual RandomAccessIteratorProxy<T> operator - (size_t Dist_)
	{ return Dispatch()->operator - (Dist_); }
    virtual const RandomAccessIteratorProxy<T> operator - (size_t Dist_) const
	{ return Dispatch()->operator - (Dist_); }

    virtual RandomAccessIterator<T>& operator += (size_t Dist_)
	{ return Dispatch()->operator += (Dist_); }
    virtual const RandomAccessIterator<T>& operator += (size_t Dist_) const
	{ return Dispatch()->operator += (Dist_); }
    virtual RandomAccessIterator<T>& operator -= (size_t Dist_)
	{ return Dispatch()->operator -= (Dist_); }
    virtual const RandomAccessIterator<T>& operator -= (size_t Dist_) const
	{ return Dispatch()->operator -= (Dist_); }

    virtual RandomAccessIterator<T>& GiveIterator()
	{ return *(Dispatch()); }
    virtual const RandomAccessIterator<T>& GiveIterator() const
	{ return *(Dispatch()); }

    virtual RandomAccessIterator<T>& operator = (const RandomAccessIterator<T>& Rhs_);
};

/****************************************************************************/
template <class T>
class ReverseRandomAccessIterator : public RandomAccessIterator<T>
{
  public:
    ReverseRandomAccessIterator(const RandomAccessIterator<T>& Boss_):
	RandomAccessIterator<T>(Boss_) {}
    ReverseRandomAccessIterator(const RandomAccessIterator<T>* Boss_):
	RandomAccessIterator<T>(Boss_) {}

    virtual RandomAccessIterator<T>* Clone()
	{ return (new ReverseRandomAccessIterator<T>(*this)); }
    virtual const RandomAccessIterator<T>* Clone() const
	{ return (new ReverseRandomAccessIterator<T>(*this)); }

    virtual RandomAccessIterator<T>& operator ++ ();
    virtual RandomAccessIteratorProxy<T> operator ++ (int);

    virtual const RandomAccessIterator<T>& operator ++ () const;
    virtual const RandomAccessIteratorProxy<T> operator ++ (int) const;

    virtual RandomAccessIterator<T>& operator -- ();
    virtual RandomAccessIteratorProxy<T> operator -- (int);

    virtual const RandomAccessIterator<T>& operator -- () const;
    virtual const RandomAccessIteratorProxy<T> operator -- (int) const;

    virtual RandomAccessIteratorProxy<T> operator + (size_t Diff_);
    virtual const RandomAccessIteratorProxy<T> operator + (size_t Diff_) const;
    virtual RandomAccessIteratorProxy<T> operator - (size_t Diff_);
    virtual const RandomAccessIteratorProxy<T> operator - (size_t Diff_) const;

    virtual RandomAccessIterator<T>& operator += (size_t Diff_);
    virtual const RandomAccessIterator<T>& operator += (size_t Diff_) const;
    virtual RandomAccessIterator<T>& operator -= (size_t Diff_);
    virtual const RandomAccessIterator<T>& operator -= (size_t Diff_) const;
};


/****************************************************************************/
template <class T>
Boolean operator == (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2)
{
  return
  (
    r1.Position() == r2.Position() &&
    r1.Address() == r2.Address()
  );
}

/****************************************************************************/
template <class T>
Boolean operator != (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2)
{
  return
  (
    r1.Position() != r2.Position() ||
    r1.Address() != r2.Address()
  );
}

/****************************************************************************/
template <class T>
Boolean operator < (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2)
{
  return
  (
    r1.Address() == r2.Address() &&
    r1.Position() < r2.Position()
  );
}

/****************************************************************************/
template <class T>
Boolean operator > (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2)
{
  return
  (
    r1.Address() == r2.Address() &&
    r1.Position() > r2.Position()
  );
}

/****************************************************************************/
template <class T>
Boolean operator <= (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2)
{
  return
  (
    r1.Address() == r2.Address() &&
    r1.Position() <= r2.Position()
  );
}

/****************************************************************************/
template <class T>
Boolean operator >= (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2)
{
  return
  (
    r1.Address() == r2.Address() &&
    r1.Position() >= r2.Position()
  );
}

/****************************************************************************/
template <class T>
ptrdiff_t operator - (const RandomAccessIterator<T>& r1, const RandomAccessIterator<T>& r2)
{
  size_t LhsPos_ = r1.Position();
  return (LhsPos_ - r2.Position());
}

/****************************************************************************/
#endif



