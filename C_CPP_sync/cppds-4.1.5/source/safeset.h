#ifndef SAFESET_H
#define SAFESET_H
#ifndef SETBASE_H
  #include "setbase.h"
#endif

#define SAFESET_DEBUG	0

/****************************************************************************/
template <class T>
class SafeSetIterator
{
  private:
    SetBase::Iterator* _Iter;	// Internal iterator object set to "this set".

  public:
    SafeSetIterator(SetBase::Iterator* Iter_);
    ~SafeSetIterator()	// destructor added to delete internal member.
	{ delete _Iter; }

    // same as internal's More method
    Boolean More()
	{ return _Iter->More(); }

    // same as internal's Next method with return void pointer cast to the
    // parametric type pointer.
    T* Next()
	{ return (T*)(_Iter->Next()); }
};

/****************************************************************************/
// Safe set class is implemented for the sole purpose of providing
// type specific safety through template definitions
//
template <class T>
class SafeSet : private SetBase
{
  public:
    // Default no-argument and copy constructors are OK
    // Default operator= is OK
    SafeSet<T>& operator += (T* object);
    SafeSet<T>& operator -= (T* object);
    Boolean operator >= (T* object)
	{ return Exists(object); }
    SafeSetIterator<T>* GiveIterator();
};

/****************************************************************************/
#endif





