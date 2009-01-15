#ifndef CNTPTR_H
#define CNTPTR_H
#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERT_H
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

/****************************************************************************/
template <class T>
class CountedPtr
{
  private:
    int* _Count;
    T* _Data;

    static int* IncrementCnt(int* CntPtr_);
    static int* DecrementCnt(int* CntPtr_);

  public:
    CountedPtr();
    CountedPtr(T* just_newed);
    CountedPtr(const CountedPtr<T>& Obj_);
    ~CountedPtr();

    virtual CountedPtr<T>* Clone() const;

    CountedPtr<T>& operator = (CountedPtr<T>& Obj_);
    CountedPtr<T>& operator = (T* rhs);

    T& operator * ();
    const T& operator * () const;
    
    virtual const T* ConstPointee() const;
    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual T* ReleaseControl();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif    
};

/****************************************************************************/
template <class T>
CountedPtr<T>::CountedPtr():
_Count(NULL),
_Data(NULL)
{}

/****************************************************************************/
template <class T>
CountedPtr<T>::CountedPtr(T* just_newed):
_Count(new int(1)),
_Data(just_newed)
{}

/****************************************************************************/
template <class T>
CountedPtr<T>::CountedPtr(const CountedPtr<T>& Obj_):
_Count(CountedPtr<T>::IncrementCnt(Obj_._Count)),
_Data(Obj_._Data)
{}

/****************************************************************************/
template <class T>
CountedPtr<T>::~CountedPtr()
{
  if (DecrementCnt(_Count) && *_Count == 0)
  {
    delete _Data;
    delete _Count;
  }
}

/****************************************************************************/
template <class T>
int* CountedPtr<T>::DecrementCnt(int* CntPtr_)
{
  if (CntPtr_ && *CntPtr_ > 0)
    --(*CntPtr_);
    
  return CntPtr_;
}

/****************************************************************************/
template <class T>
int* CountedPtr<T>::IncrementCnt(int* CntPtr_)
{
  if (CntPtr_)
    ++(*CntPtr_);
    
  return CntPtr_;
}

/****************************************************************************/
template <class T>
CountedPtr<T>* CountedPtr<T>::Clone() const
{
  return (new CountedPtr<T>(*this));
}

/****************************************************************************/
template <class T>
CountedPtr<T>& CountedPtr<T>::operator = (CountedPtr<T>& Obj_)
{
  if (this != &Obj_)
  {
    if (DecrementCnt(_Count) && *_Count == 0)
    {
      delete _Data;
      delete _Count;
    }

    _Data = Obj_._Data;
    _Count = IncrementCnt(Obj_._Count);
  }

  return *this;
}

/****************************************************************************/
template <class T>
CountedPtr<T>& CountedPtr<T>::operator = (T* rhs)
{
  if (DecrementCnt(_Count) && *_Count == 0)
  {
    delete _Data;
    delete _Count;
  }

  _Data = rhs;
  _Count = new int(1);

  return *this;
}

/****************************************************************************/
template <class T>
T& CountedPtr<T>::operator * ()
{
  if (!_Data)
    assert(_Data != NULL);
  
  return *_Data;
}

/****************************************************************************/
template <class T>
const T& CountedPtr<T>::operator * () const
{
  if (!_Data)
    assert(_Data != NULL);
  
  return *_Data;
}

/****************************************************************************/
template <class T>
const T* CountedPtr<T>::ConstPointee() const
{
  return _Data;
}

/****************************************************************************/
template <class T>
const T* CountedPtr<T>::Pointee() const
{
  return _Data;
}

/****************************************************************************/
template <class T>
T* CountedPtr<T>::Pointee()
{
  return _Data;
}

/****************************************************************************/
template <class T>
T* CountedPtr<T>::ReleaseControl()
{
  T* Saved_ = _Data;
  
  _Data = NULL;
  DecrementCnt(_Count);

  if (_Count && *_Count == 0)
  {
    delete _Count;
    _Count = NULL;
  }

  return Saved_;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* CountedPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CountedPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
    
/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* CountedPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void CountedPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif




