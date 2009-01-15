#ifndef LONEFALL_H
#define LONEFALL_H

#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef LANGSUPP_H
  #include "langsupp.h"
#endif
#ifndef BOOLEAN_H
  #include "boolean.h"
#endif
#ifndef LONEERR_H
  #include "loneerr.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif

#define FALLIBLE_DEBUG		0

#if HAS_EXCEPTIONS
  #define THROW(x)      \
    throw x
#else
  #define THROW(x)      \
    CATCH(x)
#endif

#define CATCH(excls)    \
  CatchEx(excls)

void CatchEx(const LoneErr& ExCls_);

class LoneFallibleBase
{
  private:
    Boolean is_valid;

  protected:
    void throwErr() const;
    void SetValid(Boolean state)
	{ is_valid = state; }

  public:
    LoneFallibleBase(Boolean state) : is_valid(state) {}

    Boolean failed() const { return !is_valid;          }   // True if invalid.
    Boolean valid()  const { return is_valid;           }   // True if valid.
    void invalidate()      { is_valid = BoolClass::False; }   // Make invalid.

    class IOFailureErr : public LoneErr
    {
      public:
	IOFailureErr() {}
	IOFailureErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class UsedInInvalidStateErr : public LoneErr
    {
      public:
	UsedInInvalidStateErr() {}
	UsedInInvalidStateErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class IncompatibleTypeErr : public LoneErr
    {
      public:
	IncompatibleTypeErr() {}
	IncompatibleTypeErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class BitsInvalidArgumentErr : public LoneErr
    {
      public:
	BitsInvalidArgumentErr() {}
	BitsInvalidArgumentErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class BitsOverFlowErr : public LoneErr
    {
      public:
	BitsOverFlowErr() {}
	BitsOverFlowErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class BitsOutOfRangeErr : public LoneErr
    {
      public:
	BitsOutOfRangeErr() {}
	BitsOutOfRangeErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class BitStringInvalidArgumentErr : public LoneErr
    {
      public:
	BitStringInvalidArgumentErr() {}
	BitStringInvalidArgumentErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class BitStringLengthErr : public LoneErr
    {
      public:
	BitStringLengthErr() {}
	BitStringLengthErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class BitStringOutOfRangeErr : public LoneErr
    {
      public:
	BitStringOutOfRangeErr() {}
	BitStringOutOfRangeErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class ArrayIndexRangeErr : public LoneErr
    {
      public:
	ArrayIndexRangeErr() {}
	ArrayIndexRangeErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class ArrayLengthErr : public LoneErr
    {
      public:
	ArrayLengthErr() {}
	ArrayLengthErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class ArrayInvalidArgumentErr : public LoneErr
    {
      public:
	ArrayInvalidArgumentErr() {}
	ArrayInvalidArgumentErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class DerefNullPointerErr : public LoneErr
    {
      public:
	DerefNullPointerErr() {}
	DerefNullPointerErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class NotSameBasePointerErr : public LoneErr
    {
      public:
	NotSameBasePointerErr() {}
	NotSameBasePointerErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class OutOfMemoryErr : public LoneErr
    {
      public:
	OutOfMemoryErr() {}
	OutOfMemoryErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class IncompatibleHtmlTagsErr : public LoneErr
    {
      public:
	IncompatibleHtmlTagsErr() {}
	IncompatibleHtmlTagsErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class NotImplementedErr : public LoneErr
    {
      public:
	NotImplementedErr() {}
	NotImplementedErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class InvalidBoundsErr : public LoneErr
    {
      public:
	InvalidBoundsErr() {}
        InvalidBoundsErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };

    class BoundsUnderFlowErr : public LoneErr
    {
      public:
	BoundsUnderFlowErr() {}
        BoundsUnderFlowErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };
    
    class BoundsOverFlowErr : public LoneErr
    {
      public:
	BoundsOverFlowErr() {}
        BoundsOverFlowErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };    

    class AdditionOutOfRangeErr : public LoneErr
    {
      public:
	AdditionOutOfRangeErr() {}
        AdditionOutOfRangeErr(ostream& os_):
		LoneErr(os_) {}
	virtual const char* message() const;
    };        
};

template <class T>
class LoneFallible : private LoneFallibleBase
{
  private:
    T _Object;
    T* instance;

  public:
    LoneFallible() :
	LoneFallibleBase(BoolClass::False),
	instance(&_Object) {}   		// Invalid.
    LoneFallible(const T& t, T* p) :
	LoneFallibleBase(BoolClass::True),
	_Object(t),
	instance((p == NULL) ? &_Object:p) {}     // Valid
    LoneFallible(const LoneFallible<T>& Fall_) :	// Copy constructor
	LoneFallibleBase(Fall_.valid()),
	_Object(Fall_._Object),
	instance(Fall_.LocalCopy() ? &_Object:Fall_.instance) {}

    LoneFallibleBase::failed;
    LoneFallibleBase::valid;
    LoneFallibleBase::invalidate;

    Boolean LocalCopy() const
	{ return (instance == &_Object); }

    LoneFallible<T>& SetAsReference(T* p);
    LoneFallible<T>& SetAsCopy(const T& t);
    LoneFallible<T>& operator = (const LoneFallible<T>& Fall_);

    operator T& ();
    T elseDefaultTo(const T& default_value) const; // Value if valid, else default_value

    // These can't be used ambiguity between const and non-const versions
//    operator T () const;
//    T& elseDefaultTo(T& default_value);
};

template <class T>
LoneFallible<T>& LoneFallible<T>::SetAsReference(T* p)
{
  SetValid(BoolClass::True);
  instance = p;

  return *this;
}

template <class T>
LoneFallible<T>& LoneFallible<T>::SetAsCopy(const T& t)
{
  SetValid(BoolClass::True);
  _Object = t;
  instance = &_Object;

  return *this;
}

template <class T>
LoneFallible<T>& LoneFallible<T>::operator = (const LoneFallible<T>& Fall_)
{
  SetValid(Fall_.valid());
  _Object = Fall_._Object;
  instance = Fall_.LocalCopy() ? &_Object:Fall_.instance;

  return *this;
}

template <class T>
inline LoneFallible<T>::operator T& ()
{
    if (failed())
      throwErr();
    return *instance;
}

template <class T>
inline T LoneFallible<T>::elseDefaultTo(const T& default_value) const
{
    return valid() ? *instance : default_value;
}

#endif




