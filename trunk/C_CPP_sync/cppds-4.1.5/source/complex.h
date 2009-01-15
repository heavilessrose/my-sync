/****************************************************************************/
// FILE: complex.h
// PURPOSE:
// Complex number class
/****************************************************************************/
#ifndef COMPLEX_H
#define COMPLEX_H
#ifndef FPCONST_H
  #include "fpconst.h"
#endif
#ifndef FPUTIL_H
  #include "fputil.h"
#endif
#ifndef COMPLEXMEMBER_H
  #include "complexmember.h"
#endif
#ifndef TYPEWRAPPER_H
  #include "typewrap.h"
#endif

#define COMPLEX_DEBUG           0
#define COMPLEX_TESTINPUT       0

/****************************************************************************/
template <class T>
class ComplexNumber;

template <class T>
class ComplexNumberAcceptor : public ObjectAcceptor
{
  friend class ComplexNumber<T>;

  protected:
    union
    {
      const ComplexNumber<T>* _ObjPtr;
      ComplexNumber<T>* _VarPtr;
    };

    ComplexNumberAcceptor(const ComplexNumber<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // ComplexNumber class specific acceptor methods
    virtual Boolean IsComplexNumber() const;
    virtual Boolean IsTypeWrapper() const;
    virtual void* CreateFromComplexNumber(const ObjectAcceptor& Obj_);
    virtual void* AssignFromComplexNumber(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToComplexNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToComplexNumber(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToComplexNumber(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class ComplexNumber : public RealType, public ComplexNumberMember
{
  friend class ComplexNumberAcceptor<T>;

  friend Object& RootObject(ComplexNumber<T>& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const ComplexNumber<T>& Obj_)
	{ return *((const Object*)(&Obj_)); }

  friend ComplexNumber<T> operator + (const ComplexNumber<T>& Num1_, const ComplexNumber<T>& Num2_)
      { return (ComplexNumber<T>(Num1_) += Num2_); }
  friend ComplexNumber<T> operator - (const ComplexNumber<T>& Num1_, const ComplexNumber<T>& Num2_)
      { return (ComplexNumber<T>(Num1_) -= Num2_); }
  friend ComplexNumber<T> operator * (const ComplexNumber<T>& Num1_, const ComplexNumber<T>& Num2_)
      { return (ComplexNumber<T>(Num1_) *= Num2_); }
  friend ComplexNumber<T> operator / (const ComplexNumber<T>& Num1_, const ComplexNumber<T>& Num2_)
      { return (ComplexNumber<T>(Num1_) /= Num2_); }

  friend ComplexNumber<T> operator + (const ComplexNumber<T>& Num1_, long Num2_)
      { return (ComplexNumber<T>(Num1_) += Num2_); }
  friend ComplexNumber<T> operator - (const ComplexNumber<T>& Num1_, long Num2_)
      { return (ComplexNumber<T>(Num1_) -= Num2_); }
  friend ComplexNumber<T> operator * (const ComplexNumber<T>& Num1_, long Num2_)
      { return (ComplexNumber<T>(Num1_) *= Num2_); }
  friend ComplexNumber<T> operator / (const ComplexNumber<T>& Num1_, long Num2_)
      { return (ComplexNumber<T>(Num1_) /= Num2_); }      

  friend ComplexNumber<T> operator + (long Num1_, const ComplexNumber<T>& Num2_)
      { return (ComplexNumber<T>(Num2_) += Num1_); }
  friend ComplexNumber<T> operator * (long Num1_, const ComplexNumber<T>& Num2_)
      { return (ComplexNumber<T>(Num2_) *= Num1_); }

  protected:
    T _Re;
    T _Im;

    int GetTypePrec() const;

    virtual Boolean IsEqualToComplexNumber(const ComplexNumber<T>* Ptr_) const;
    virtual Boolean IsLesserToComplexNumber(const ComplexNumber<T>* Ptr_) const;
    virtual Boolean IsGreaterToComplexNumber(const ComplexNumber<T>* Ptr_) const;

  public:
    ComplexNumber();
    ComplexNumber(const T& Real_, const T& Img_ = T(0));
    ComplexNumber(const ChrString& Str_);
    ComplexNumber(const ComplexNumber& Obj_);
    virtual ~ComplexNumber() {}

    static const ComplexNumber<T>& CNI(); //(0.0, 1.0);
    static const ComplexNumber<T>& CN1(); //(1.0, 0.0);
    static const ComplexNumber<T>& CNZ(); //(0.0, 0.0);

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;

    // Virtual constructor and assignment methods
    static ComplexNumber<T>* Make(const T& Real_=0, const T& Img_=0);
    static ComplexNumber<T>* Make(const Object& Obj_);
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static ComplexNumber<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    static Boolean IsValue(const ChrString& Str_);
    static ComplexNumber<T> ValueOf(const ChrString& Str_);
    static ChrString ToString(const ComplexNumber<T>& Dbl_);
    static ChrString ToTruncString(const ComplexNumber<T>& Dbl_);
    virtual ChrString ToString(int = 10) const;
    virtual ChrString ToTruncString(int = 10) const;
    virtual ComplexNumber<T>& FromString(const ChrString& Str_);

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::COMPLEXNUMBER_TYPE; }
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

    // Special value methods
    virtual Boolean IsNan() const;
    virtual Boolean IsInfinite() const;
    virtual Boolean AtZero() const;

    // Rounding methods
    virtual ComplexNumber<T>& Ceiling();
    virtual ComplexNumber<T>& Floor();
    virtual ComplexNumber<T>& Round();

    // Unary operators
    virtual Boolean operator ! () const;
    ComplexNumber<T> operator - () const;
    ComplexNumber<T> operator + () const;

    // Assignment methods
    virtual ComplexNumber<T>& operator = (const ChrString& Str_);
    virtual ComplexNumber<T>& operator = (const ComplexNumber& Obj_);

    // Arithmetic assignment operators    
    virtual ComplexNumber<T>& operator += (const ComplexNumber& Rhs_);
    virtual ComplexNumber<T>& operator -= (const ComplexNumber& Rhs_);
    virtual ComplexNumber<T>& operator *= (const ComplexNumber& Rhs_);
    virtual ComplexNumber<T>& operator /= (const ComplexNumber& Rhs_);

    // Arithmetic assignment operators
    virtual ComplexNumber<T>& operator += (long Rhs_);
    virtual ComplexNumber<T>& operator -= (long Rhs_);
    virtual ComplexNumber<T>& operator *= (long Rhs_);
    virtual ComplexNumber<T>& operator /= (long Rhs_);

    // Arithmetic assignment operators
    virtual ComplexNumber<T>& operator += (UnsignedIntValue Val_);
    virtual ComplexNumber<T>& operator -= (UnsignedIntValue Val_);
    virtual ComplexNumber<T>& operator *= (UnsignedIntValue Val_);
    virtual ComplexNumber<T>& operator /= (UnsignedIntValue Val_);    

    virtual void SetRealVal(const T& Obj_);
    virtual void SetImagVal(const T& Obj_);
    T RealVal() const
	{ return _Re; }
    T ImagVal() const
	{ return _Im; }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

// type definitions
typedef ComplexNumber<float>      FComplex;
typedef ComplexNumber<double>     DComplex;
typedef ComplexNumber<Ldouble>    LComplex;

//-----------------------
// Set significant digits
//-----------------------
template <class T>
ComplexNumber<T> SigDig(const ComplexNumber<T>& c, size_t n)
{
  return ComplexNumber<T>(SigDig(c.RealVal(), n), SigDig(c.ImagVal(), n));
}

//------------------
// Complex functions
//------------------
/****************************************************************************/
// complex conjugate
template <class T>
ComplexNumber<T> conj(const ComplexNumber<T> & c)
{
  return ComplexNumber<T>(c.RealVal(), -c.ImagVal());
}

/****************************************************************************/
// square of the magnitude
template <class T>
T norm(const ComplexNumber<T> & c)
{
  return (c.RealVal() * c.RealVal() + c.ImagVal() * c.ImagVal());
}

/****************************************************************************/
// angle in the plane
template <class T>
T arg(const ComplexNumber<T>& c)
{
  return atan2(c.ImagVal(), c.RealVal());
}

/****************************************************************************/
// conversion to polar coordinates
template <class T>
ComplexNumber<T> cpolar(const T& radius, const T& theta)
{
  return ComplexNumber<T>(radius * cos(theta),
                          radius * sin(theta));
}

/****************************************************************************/
// logarithmic functions
template <class T>
ComplexNumber<T> exp(const ComplexNumber<T>& c)
{
  return cpolar(exp(c.RealVal()), c.ImagVal());
}

/****************************************************************************/
template <class T>
ComplexNumber<T> log(const ComplexNumber<T>& c)
{
  return ComplexNumber<T>(log(abs(c)), arg(c));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> log10(const ComplexNumber<T>& c)
{
  return ComplexNumber<T>(N_DLOG10SQE * log(norm(c)), arg(c));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> pow(const ComplexNumber<T> & c, const T& ex)
{
    return pow(c,ComplexNumber<T>(ex, 0));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> pow(const T& c, const ComplexNumber<T> & ex)
{
    return pow(ComplexNumber<T>(ex),c);
}

/****************************************************************************/
// trigonometric functions
template <class T>
ComplexNumber<T> sin(const ComplexNumber<T> & c)
{
    return ComplexNumber<T>(sin(c.RealVal()) * cosh(c.ImagVal()),
                    cos(c.RealVal()) * sinh(c.ImagVal()));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> cos(const ComplexNumber<T> & c)
{
    return ComplexNumber<T>( cos(c.RealVal()) * cosh(c.ImagVal()),
                    -sin(c.RealVal()) * sinh(c.ImagVal()));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> tan(const ComplexNumber<T> & c)
{
    T r = c.RealVal() + c.RealVal();
    T i = c.ImagVal() + c.ImagVal();
    T t = 1.0 / (cos(r) + cosh(i));

    return ComplexNumber<T>(t * sin(r), t * sinh(i));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> sinh(const ComplexNumber<T> & c)
{
    return ComplexNumber<T>(cos(c.ImagVal()) * sinh(c.RealVal()),
                    sin(c.ImagVal()) * cosh(c.RealVal()));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> cosh(const ComplexNumber<T> & c)
{
    return ComplexNumber<T>(cos(c.ImagVal()) * cosh(c.RealVal()),
                    sin(c.ImagVal()) * sinh(c.RealVal()));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> tanh(const ComplexNumber<T> & c)
{
    T r = c.RealVal() + c.RealVal();
    T i = c.ImagVal() + c.ImagVal();
    T t = 1.0 / (cosh(r) + cos(i));

    return ComplexNumber<T>(t * sinh(r), t * sin(i));
}

/****************************************************************************/
template <class T>
ComplexNumber<T> atanh(const ComplexNumber<T> & c)
{
    ComplexNumber<T> x(atan(ComplexNumber<T>(-c.ImagVal(), c.RealVal())));
    return ComplexNumber<T>(x.ImagVal(), -x.RealVal());
}

/****************************************************************************/
template <class T>
ComplexNumber<T> asinh(const ComplexNumber<T> & c)
{
    ComplexNumber<T> x(asin(ComplexNumber<T>(-c.ImagVal(), c.RealVal())));
    return ComplexNumber<T>(x.ImagVal(), -x.RealVal());
}

/****************************************************************************/
template <class T>
ComplexNumber<T> acosh(const ComplexNumber<T> & c)
{
    ComplexNumber<T> x(acos(ComplexNumber<T>(c.RealVal(), -c.ImagVal())));
    return ComplexNumber<T>(x.ImagVal(), x.RealVal());
}

/****************************************************************************/
#endif




