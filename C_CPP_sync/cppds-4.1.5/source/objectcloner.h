#ifndef OBJECTCLONER_H
#define OBJECTCLONER_H

/****************************************************************************/
template <class T>
class ObjectCloner
{
  public:
    virtual ObjectCloner<T>* Clone() = 0;
    virtual T* Duplicate(T& Object_) = 0;
    virtual const T* Duplicate(const T& Object_) const = 0;
};

template <class T>
class FakeCloner : public ObjectCloner<T>
{
  private:
    FakeCloner();
  
  public:
    virtual ObjectCloner<T>* Clone();
    virtual T* Duplicate(T& Object_);
    virtual const T* Duplicate(const T& Object_) const;
    static FakeCloner<T>* Make();
};

template <class T>
class RealCloner : public ObjectCloner<T>
{
  private:
    RealCloner();

  public:
    virtual ObjectCloner<T>* Clone();
    virtual T* Duplicate(T& Object_);
    virtual const T* Duplicate(const T& Object_) const;    
    static RealCloner<T>* Make();
};

/****************************************************************************/
/****************************************************************************/
template <class T>
FakeCloner<T>::FakeCloner()
{}

/****************************************************************************/
template <class T>
ObjectCloner<T>* FakeCloner<T>::Clone()
{
  return (new FakeCloner());
}

/****************************************************************************/
template <class T>
T* FakeCloner<T>::Duplicate(T& Object_)
{
  return &Object_;
}

/****************************************************************************/
template <class T>
const T* FakeCloner<T>::Duplicate(const T& Object_) const
{
  return &Object_;
}

/****************************************************************************/
template <class T>
FakeCloner<T>* FakeCloner<T>::Make()
{
  return (new FakeCloner<T>());
}

/****************************************************************************/
/****************************************************************************/
template <class T>
RealCloner<T>::RealCloner()
{}

/****************************************************************************/
template <class T>
ObjectCloner<T>* RealCloner<T>::Clone()
{
  return (new RealCloner());
}

/****************************************************************************/
template <class T>
T* RealCloner<T>::Duplicate(T& Object_)
{
  return ::Clone(Object_);
}

/****************************************************************************/
template <class T>
const T* RealCloner<T>::Duplicate(const T& Object_) const
{
  return ::Clone(Object_);
}

/****************************************************************************/
template <class T>
RealCloner<T>* RealCloner<T>::Make()
{
  return (new RealCloner<T>());
}

/****************************************************************************/
#endif


