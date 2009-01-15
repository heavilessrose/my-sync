#ifndef DYNARRAYBASE_H
#define DYNARRAYBASE_H
#ifndef CONTAIN_H
  #include "contain.h"
#endif
#ifndef ITERATOR_CPP
  #include "iterator.cpp"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif
#ifndef INDEX_H
  #include "index.h"
#endif
#ifndef SORTS_CPP
  #include "sorts.cpp"
#endif

/****************************************************************************/
#define DYNARRAY_IMPLEMENT_STLCONTAINER		1
#define DYNARRAY_IMPLEMENT_STLITERATOR		1

template <class T>
class DynArrayBase : public Container
{
  protected:
    T* _Ptr;
    size_t _Len, _Res;
    size_t _IteratorsActive;
    Boolean _DestroyFromIterator;

    DynArrayBase():
	_IteratorsActive(0),
	_DestroyFromIterator(FALSE) {}
    DynArrayBase(size_t n, capacity c):
	_IteratorsActive(0),
	_DestroyFromIterator(FALSE) {}
    DynArrayBase(const DynArrayBase<T>& x):
	_IteratorsActive(0),
	_DestroyFromIterator(FALSE) {}
    DynArrayBase(const T& x, size_t n=1):
	_IteratorsActive(0),
	_DestroyFromIterator(FALSE) {}
    DynArrayBase(const T* s, size_t n):
	_IteratorsActive(0),
	_DestroyFromIterator(FALSE) {}

    virtual void _Grow(size_t n, const T* s=0, Boolean Trim_=0) {}// = 0;
    void _Tidy(Boolean Constructed_=0);

    void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }
    void _Xinv() const
	{ THROW (FallibleBase::DynArrayInvalidArgumentErr()); }
    void _Xlen() const
	{ THROW (FallibleBase::DynArrayLengthErr()); }
    void _Xran() const
	{ THROW (FallibleBase::DynArrayOutOfRangeErr()); }

  public:
    virtual ~DynArrayBase();

    DynArrayBase<T>& operator = (const DynArrayBase<T>& r);
    DynArrayBase<T>& Append(const T& x, size_t n=1)
	{ return (Append(&x, n, 0)); }
    DynArrayBase<T>& Append(const T* s, size_t n=1, size_t d=1);
    DynArrayBase<T>& operator += (const DynArrayBase<T>& r)
	{ return (Append(r.Base(), r.RunLength())); }
    DynArrayBase<T>& operator += (const T& r)
	{ return (Append(r)); }

    DynArrayBase<T>& AssignArg(const T& x, size_t n=1)
	{ return (AssignArg(&x, n, 0)); }
    DynArrayBase<T>& AssignArg(const T* s, size_t n=1, size_t d=1);

    DynArrayBase<T>& Insert(size_t p, const DynArrayBase<T>& x)
	{ return (Insert(p, x.Base(), x.RunLength())); }
    DynArrayBase<T>& Insert(size_t p, const T& x, size_t n=1)
	{ return (Insert(p, &x, n, 0)); }
    DynArrayBase<T>& Insert(size_t p, const T* s, size_t n=1, size_t d=1);

    DynArrayBase<T>& Remove(size_t p=0, size_t n=SIZET_MAX);

    DynArrayBase<T>& Sub_Array(DynArrayBase<T>& x, size_t p, size_t n=SIZET_MAX)
	{ if (_Len < p)
	    _Xran();
	  if (_Len - p < n)
	    n = _Len - p;
	  return (this == &x ? (Remove(p + n), Remove(0, p)):x.AssignArg(&_Ptr[p], n));
	}

    void Swap(DynArrayBase<T>& x)
	{ T* _Tp = _Ptr; _Ptr = x._Ptr; x._Ptr = _Tp;
	  size_t _Tl = _Len; _Len = x._Len; x._Len = _Tl;
	  size_t _Tr = _Res; _Res = x._Res; x._Res = _Tr;
	}

    const T& Get_At(size_t i) const
	{ if (_Len <= i)
	    _Xran();
	  return (_Ptr[i]);
	}
    void Put_At(size_t i, const T& x)
	{ if (_Len <= i)
	    _Xran();
	  _Ptr[i] = x;
	}
    T& operator [] (size_t i)
	{ return (_Ptr[i]); }
    const T& operator [] (size_t i) const
	{ return (_Ptr[i]); }
    T& operator [] (const Index& i)
	{ return (_Ptr[size_t(i)]); }
    const T& operator [] (const Index& i) const
	{ return (_Ptr[size_t(i)]); }        
    T* Base()
	{ return (_Len != 0 ? _Ptr:0); }
    const T* Base() const
	{ return (_Len != 0 ? _Ptr:0); }
    virtual size_t Size() const;
    size_t RunLength() const
	{ return (_Len); }
    void Resize(size_t n);
    void Resize(size_t n, const T& x);
    virtual void Flush();
    size_t Reserve() const
	{ return (_Res); }
    void Reserve(size_t r)
	{ if (_Ptr == 0)
	    _Res = r; }

    // container size / limit accessors
    virtual size_t Limit() const;

    // container full / empty test methods
    virtual Boolean IsFull() const;
    virtual Boolean IsEmpty() const;

    // container iterator methods
    virtual Boolean IsNewed() const = 0;
    virtual void DestroyFromIterator() = 0;
    virtual Boolean RegisterIterator() = 0;
    virtual Boolean UnRegisterIterator() = 0;
    virtual size_t IteratorsActive() = 0;

#if OVERLOAD_NEW
    // new & delete operators
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

    // new & delete operators for arrays
#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
#endif





