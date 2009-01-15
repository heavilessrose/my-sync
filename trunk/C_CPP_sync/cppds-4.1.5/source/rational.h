//==========================================================
//  Numerics Library
//==========================================================
//  Rational.h
//
//  A class for handling Rational numbers (fractions).
//
//  Copyright 1992-96 Scott Robert Ladd. All rights reserved
//==========================================================

#ifndef RATIONAL_H
#define RATIONAL_H

#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif

#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif

/****************************************************************************/
class RationalRealType : public RealType
{
  protected:
    RationalRealType(int Options_):
        RealType(Options_) {}
    RationalRealType(const TypeWrapper& Obj_):
        RealType(Obj_) {}

  public:  
    virtual double FloatValue() const = 0;
};

/****************************************************************************/
template <class T>
class Rational;

template <class T>
class RationalAcceptor : public ObjectAcceptor
{
  friend class Rational<T>;

  protected:
    union
    {
      const Rational<T>* _ObjPtr;
      Rational<T>* _VarPtr;
    };

    RationalAcceptor(const Rational<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // ComplexNumber class specific acceptor methods
    virtual Boolean IsRational() const;
    virtual Boolean IsTypeWrapper() const;

    virtual void* CreateFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);    
    virtual void* CreateFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromInteger(const ObjectAcceptor& Obj_);
    virtual void* CreateFromULongInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromUShortInt(const ObjectAcceptor& Obj_);
    virtual void* CreateFromUInteger(const ObjectAcceptor& Obj_);    
    virtual void* CreateFromFloat(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDouble(const ObjectAcceptor& Obj_);    
    virtual void* CreateFromLongDouble(const ObjectAcceptor& Obj_);
    virtual void* CreateFromRational(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromLongNumber(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromLongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromInteger(const ObjectAcceptor& Obj_);
    virtual void* AssignFromULongInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUShortInt(const ObjectAcceptor& Obj_);
    virtual void* AssignFromUInteger(const ObjectAcceptor& Obj_);
    virtual void* AssignFromFloat(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDouble(const ObjectAcceptor& Obj_);        
    virtual void* AssignFromLongDouble(const ObjectAcceptor& Obj_);
    virtual void* AssignFromRational(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToLongDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToRational(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsLesserToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToUInteger(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToLongDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToRational(const ObjectAcceptor& Obj_) const;

    virtual Boolean IsGreaterToLongNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongInt(const ObjectAcceptor& Obj_) const;    
    virtual Boolean IsGreaterToShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToULongInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUShortInt(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToUInteger(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToFloat(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToLongDouble(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToRational(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

//==========================================================
template <class T>
class Rational : public RationalRealType
{
  friend class RationalAcceptor<T>;

  // basic four operators
  friend Rational<T> operator + (const Rational<T> & r1, const Rational<T> & r2)
  {
    Rational<T> result;

    result.Denom = Rational<T>::LCM(r1.Denom,r2.Denom);

    result.Numer = r1.Numer * (result.Denom / r1.Denom)
                 + r2.Numer * (result.Denom / r2.Denom);

    result.Reduce();
    return result;
  }  
  friend Rational<T> operator - (const Rational<T> & r1, const Rational<T> & r2)
  {
    Rational<T> result;

    result.Denom = Rational<T>::LCM(r1.Denom,r2.Denom);

    result.Numer = r1.Numer * (result.Denom / r1.Denom)
                 - r2.Numer * (result.Denom / r2.Denom);

    result.Reduce();
    return result;
  }
  friend Rational<T> operator * (const Rational<T> & r1, const Rational<T> & r2)
  {
    Rational<T> result;
    result.Numer = r1.Numer * r2.Numer;
    result.Denom = r1.Denom * r2.Denom;
    result.Reduce();
    return result;
  }  
  friend Rational<T> operator / (const Rational<T> & r1, const Rational<T> & r2)
  {
    Rational<T> result;
    result.Numer = r1.Numer * r2.Denom;
    result.Denom = r1.Denom * r2.Numer;
    result.Reduce();
    return result;
  }  

  // stream input/output
  friend istream & operator >> (istream & strm, Rational<T> & r)
  {
    T n, d;
    char c;

    strm >> n >> c;
  
    if (c == '/')
      strm >> d;
    else
    {
      strm.putback(c);
      d = 1;
    }

    r = Rational<T>(n,d);

    return strm;
  }
  
  friend ostream & operator << (ostream & strm, const Rational<T> & r)
  {
    strm << r.Numer << "/" << r.Denom;
    return strm;
  }

  protected:
    T Numer;
    T Denom;

    virtual Boolean IsEqualToRational(const Rational<T>* Ptr_) const;
    virtual Boolean IsLesserToRational(const Rational<T>* Ptr_) const;
    virtual Boolean IsGreaterToRational(const Rational<T>* Ptr_) const;
    
  private:
    // Get precision of float type
    int GetTypePrec() const;
  
    // reduce fraction & adjust sign
    void Reduce();

    // convert from string
    void ConvertFromString(const ChrString& Str_);

    // greatest common divisor
    static T GCD(T x, T y);

    // least common multiple
    static T LCM(T x, T y);    

  public:
    // constructors
    Rational();
    Rational(const ChrString& Str_);
    Rational(const Rational<T>& r);
    Rational(T num, T den = 1);

    // assignment operator
    virtual Rational<T>& operator = (const ChrString& Str_);
    virtual Rational<T>& operator = (const Rational<T>& r);
    Rational<T> AssignRational(T num, T den);

    static Rational<T>* Make();
    static Rational<T>* Make(const T& num, const T& den = 1);
    static Rational<T>* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Rational<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static Boolean IsValue(const ChrString& Str_);
    static Rational<T> ValueOf(const ChrString& Str_);
    static ChrString ToString(const Rational<T>& Dbl_);
    static ChrString ToTruncString(const Rational<T>& Dbl_);
    virtual ChrString ToString(int = 10) const;
    virtual ChrString ToTruncString(int = 10) const;
    virtual Rational<T>& FromString(const ChrString& Str_);
    virtual Boolean AtZero() const;        

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::RATIONAL_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    // Rounding methods
    virtual Rational<T>& Ceiling();
    virtual Rational<T>& Floor();
    virtual Rational<T>& Round();

    // Unary operators
    virtual Boolean operator ! () const;
    Rational<T> operator - () const;
    Rational<T> operator + () const;

    // conversion operators
    virtual double FloatValue() const;
    operator double() const;

    // assignment of numerator and denominator
    Rational<T> & operator () (T num, T den = 1);

    // shorthand operators
    Rational<T> operator += (const Rational<T> & r);
    Rational<T> operator -= (const Rational<T> & r);
    Rational<T> operator *= (const Rational<T> & r);
    Rational<T> operator /= (const Rational<T> & r);

    // interrogation functions
    T GetNum() const;
    T GetDen() const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

//==========================================================
#endif




