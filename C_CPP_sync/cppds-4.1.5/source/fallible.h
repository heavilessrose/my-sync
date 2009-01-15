/*
Modified
Example fallible class from the book Scientific and Engineering Programming
in C++: An Introduction with Advanced Techniques and Examples,
*/
#ifndef FALLIBLE_H
#define FALLIBLE_H

#include "langsupp.h"
#include "boolean.h"
#include "scierr.h"
#include "asserts.h"

#define FALLIBLE_DEBUG		0

#if HAS_EXCEPTIONS
  #define THROW(x)      \
    throw x
#else
  #define THROW(x)      \
    CATCH(x)
#endif

#define CATCH(excls)	\
  CatchEx(excls)

void CatchEx(const SciEngErr& ExCls_);

class FallibleBase
{
  private:
    Boolean is_valid;

  protected:
    void throwErr() const;
    void SetValid(Boolean state)
	{ is_valid = state; }

  public:
    FallibleBase(Boolean state) : is_valid(state) {}

    Boolean failed() const { return !is_valid;          }   // True if invalid.
    Boolean valid()  const { return is_valid;           }   // True if valid.
    void invalidate()      { is_valid = BoolClass::False; }   // Make invalid.

    class DivideByZeroErr : public SciEngErr
    {
      public:
	DivideByZeroErr() {}
	DivideByZeroErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class TypeOverflowErr : public SciEngErr
    {
      public:
	TypeOverflowErr() {}
	TypeOverflowErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class TypeUnderflowErr : public SciEngErr
    {
      public:
	TypeUnderflowErr() {}
	TypeUnderflowErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class TypeConversionErr : public SciEngErr
    {
      public:
        TypeConversionErr() {}
        TypeConversionErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class IOFailureErr : public SciEngErr
    {
      public:
	IOFailureErr() {}
	IOFailureErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class UsedInInvalidStateErr : public SciEngErr
    {
      public:
	UsedInInvalidStateErr() {}
	UsedInInvalidStateErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class IncompatibleTypeErr : public SciEngErr
    {
      public:
	IncompatibleTypeErr() {}
	IncompatibleTypeErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class BitsInvalidArgumentErr : public SciEngErr
    {
      public:
	BitsInvalidArgumentErr() {}
	BitsInvalidArgumentErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class BitsOverFlowErr : public SciEngErr
    {
      public:
	BitsOverFlowErr() {}
	BitsOverFlowErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class BitsOutOfRangeErr : public SciEngErr
    {
      public:
	BitsOutOfRangeErr() {}
	BitsOutOfRangeErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class BitStringInvalidArgumentErr : public SciEngErr
    {
      public:
	BitStringInvalidArgumentErr() {}
	BitStringInvalidArgumentErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class BitStringLengthErr : public SciEngErr
    {
      public:
	BitStringLengthErr() {}
	BitStringLengthErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class BitStringOutOfRangeErr : public SciEngErr
    {
      public:
	BitStringOutOfRangeErr() {}
	BitStringOutOfRangeErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class ArrayIndexRangeErr : public SciEngErr
    {
      public:
	ArrayIndexRangeErr() {}
	ArrayIndexRangeErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class ArrayLengthErr : public SciEngErr
    {
      public:
	ArrayLengthErr() {}
	ArrayLengthErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class ArrayInvalidArgumentErr : public SciEngErr
    {
      public:
	ArrayInvalidArgumentErr() {}
	ArrayInvalidArgumentErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class DynArrayInvalidArgumentErr : public SciEngErr
    {
      public:
	DynArrayInvalidArgumentErr() {}
	DynArrayInvalidArgumentErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class DynArrayLengthErr : public SciEngErr
    {
      public:
	DynArrayLengthErr() {}
	DynArrayLengthErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class DynArrayOutOfRangeErr : public SciEngErr
    {
      public:
	DynArrayOutOfRangeErr() {}
	DynArrayOutOfRangeErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class DerefNullPointerErr : public SciEngErr
    {
      public:
	DerefNullPointerErr() {}
	DerefNullPointerErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class DerefNullLetterObjectErr : public SciEngErr
    {
      public:
	DerefNullLetterObjectErr() {}
	DerefNullLetterObjectErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class OutOfMemoryErr : public SciEngErr
    {
      public:
	OutOfMemoryErr() {}
	OutOfMemoryErr(ostream& os_):
		SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class FullHashTableErr : public SciEngErr
    {
      public:
        FullHashTableErr() {}
        FullHashTableErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class IncompatableArraysErr : public SciEngErr
    {
      public:
	IncompatableArraysErr() {}
	IncompatableArraysErr(ostream& os_):
        	SciEngErr(os_) {}
	virtual ChrString message() const;
    };

    class FuzzyRangeErr : public SciEngErr
    {
      public:
        FuzzyRangeErr() {}
        FuzzyRangeErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class MatrixAllocationErr : public SciEngErr
    {
      public:
        MatrixAllocationErr() {}
        MatrixAllocationErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class IncompatibleMatrixErr : public SciEngErr
    {
      public:
        IncompatibleMatrixErr() {}
        IncompatibleMatrixErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class InvalidIndexErr : public SciEngErr
    {
      public:
        InvalidIndexErr() {}
        InvalidIndexErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class SingularMatrixErr : public SciEngErr
    {
      public:
        SingularMatrixErr() {}
        SingularMatrixErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class MatrixTooBigErr : public SciEngErr
    {
      public:
        MatrixTooBigErr() {}
        MatrixTooBigErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class ZeroDimensionErr : public SciEngErr
    {
      public:
        ZeroDimensionErr() {}
        ZeroDimensionErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class ZeroDivisionErr : public SciEngErr
    {
      public:
        ZeroDivisionErr() {}
        ZeroDivisionErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class PolynomialAllocationErr : public SciEngErr
    {
      public:
        PolynomialAllocationErr() {}
        PolynomialAllocationErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class PolynomialRangeErr : public SciEngErr
    {
      public:
        PolynomialRangeErr() {}
        PolynomialRangeErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };    

    class PolynomialTooSmallErr : public SciEngErr
    {
      public:
        PolynomialTooSmallErr() {}
        PolynomialTooSmallErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class IncompatiblePolynomialErr : public SciEngErr
    {
      public:
        IncompatiblePolynomialErr() {}
        IncompatiblePolynomialErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class PolynomialOverflowErr : public SciEngErr
    {
      public:
        PolynomialOverflowErr() {}
        PolynomialOverflowErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class FileOpenErr : public SciEngErr
    {
      public:
        FileOpenErr() {}
        FileOpenErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class DateRangeErr : public SciEngErr
    {
      public:
        DateRangeErr() {}
        DateRangeErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };        

    class InvalidMonthErr : public SciEngErr
    {
      public:
        InvalidMonthErr() {}
        InvalidMonthErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class InvalidYearErr : public SciEngErr
    {
      public:
        InvalidYearErr() {}
        InvalidYearErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class InvalidDayErr : public SciEngErr
    {
      public:
        InvalidDayErr() {}
        InvalidDayErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class InvalidDateErr : public SciEngErr
    {
      public:
        InvalidDateErr() {}
        InvalidDateErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class InvalidTimeErr : public SciEngErr
    {
      public:
        InvalidTimeErr() {}
        InvalidTimeErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class RootNotBracketedErr : public SciEngErr
    {
      public:
        RootNotBracketedErr() {}
        RootNotBracketedErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class RootFindMaxIterationsExceededErr : public SciEngErr
    {
      public:
        RootFindMaxIterationsExceededErr() {}
        RootFindMaxIterationsExceededErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };

    class NoComplexRootsErr : public SciEngErr
    {
      public:
        NoComplexRootsErr() {}
        NoComplexRootsErr(ostream& os_):
                SciEngErr(os_) {}
        virtual ChrString message() const;
    };    
};

template <class T>
class Fallible : private FallibleBase
{
  private:
    T _Object;
    T* instance;

  public:
    Fallible() :
	FallibleBase(BoolClass::False),
	instance(&_Object) {}   		// Invalid.
    Fallible(const T& t, T* p) :
	FallibleBase(BoolClass::True),
	_Object(t),
	instance((p == NULL) ? &_Object:p) {}     // Valid
    Fallible(const Fallible<T>& Fall_) :	// Copy constructor
	FallibleBase(Fall_.valid()),
	_Object(Fall_._Object),
	instance(Fall_.LocalCopy() ? &_Object:Fall_.instance) {}

    FallibleBase::failed;
    FallibleBase::valid;
    FallibleBase::invalidate;

    Boolean LocalCopy() const
	{ return (instance == &_Object); }

    Fallible<T>& SetAsReference(T* p);
    Fallible<T>& SetAsCopy(const T& t);
    Fallible<T>& operator = (const T& Val_);
    Fallible<T>& operator = (const Fallible<T>& Fall_);

    operator T& ();
    T elseDefaultTo(const T& default_value) const; // Value if valid, else default_value

    // These can't be used ambiguity between const and non-const versions
//    operator T () const;
//    T& elseDefaultTo(T& default_value);
};

template <class T>
Fallible<T>& Fallible<T>::SetAsReference(T* p)
{
  SetValid(BoolClass::True);
  instance = p;

  return *this;
}

template <class T>
Fallible<T>& Fallible<T>::SetAsCopy(const T& t)
{
  SetValid(BoolClass::True);
  _Object = t;
  instance = &_Object;

  return *this;
}

template <class T>
Fallible<T>& Fallible<T>::operator = (const Fallible<T>& Fall_)
{
  SetValid(Fall_.valid());
  _Object = Fall_._Object;
  instance = Fall_.LocalCopy() ? &_Object:Fall_.instance;

  return *this;
}

template <class T>
Fallible<T>& Fallible<T>::operator = (const T& Val_)
{
  if (instance)
    *instance = Val_;
  else
  {
    _Object = Val_;
    instance = &_Object;
  }
  
  return *this;
}

template <class T>
inline Fallible<T>::operator T& ()
{
    if (failed())
      throwErr();
    return *instance;
}

template <class T>
inline T Fallible<T>::elseDefaultTo(const T& default_value) const
{
    return valid() ? *instance : default_value;
}

#endif




