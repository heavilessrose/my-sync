#ifndef SAFESET_CPP
#define SAFESET_CPP
#ifndef SAFESET_H
  #include "safeset.h"
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
SafeSetIterator<T>::SafeSetIterator(SetBase::Iterator* Iter_):
_Iter(Iter_)
{}

/****************************************************************************/
/****************************************************************************/
// Create SafeSet Iterator with iterator argument set to a newly allocated
// instance of an internal iterator that is passed the base class object of
// "this" set -- SetBase.
//
template <class T>
SafeSetIterator<T>* SafeSet<T>::GiveIterator()
{
  return new SafeSetIterator<T>(ProvideIterator());
}

/****************************************************************************/
template <class T>
SafeSet<T>& SafeSet<T>::operator += (T* object)
{
  AddObject(object);
  return *this;
}

/****************************************************************************/
template <class T>
SafeSet<T>& SafeSet<T>::operator -= (T* object)
{
  RemoveObject(object);
  return *this;
}

/****************************************************************************/
/****************************************************************************/
#if SAFESET_DEBUG
int main()
{
  SafeSet<short> Sset_;
  short Num13_ = 13, Num26_ = 26, Num37_ = 37, Num81_ = 81, Num78_ = 78;

  Sset_ += &Num13_;
  Sset_ += &Num26_;
  Sset_ += &Num37_;
  Sset_ += &Num81_;
  Sset_ += &Num78_;

  SafeSetIterator<short>* IterPtr_ = Sset_.GiveIterator();

  while (IterPtr_->More())
    cout <<*IterPtr_->Next() <<" ";
  cout <<endl;

  cout <<(Sset_ >= &Num37_) <<endl;
  Sset_ -= &Num37_;
  cout <<(Sset_ >= &Num37_) <<endl;

  delete IterPtr_;
  IterPtr_ = Sset_.GiveIterator();

  while (IterPtr_->More())
    cout <<*IterPtr_->Next() <<" ";
  cout <<endl;

  delete IterPtr_;
  return 0;
}
#endif
/****************************************************************************/
#endif





