#ifndef POINT_H
#define POINT_H

#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif

#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif
#ifndef RATIONAL_CPP
  #include "rational.cpp"
#endif

#define POINT_DEBUG     0

/****************************************************************************/
template <class T>
class Point;

template <class T>
class PointAcceptor : public ObjectAcceptor
{
  friend class Point<T>;

  protected:
    union
    {
      const Point<T>* _ObjPtr;
      Point<T>* _VarPtr;
    };

    PointAcceptor(const Point<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Point class specific acceptor methods
    virtual Boolean IsPoint() const;
    
    virtual void* CreateFromPoint(const ObjectAcceptor& Obj_);
    virtual void* AssignFromPoint(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToPoint(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class Point : public ObjectSkeleton
{
  friend class PointAcceptor<T>;

  friend Object& RootObject(Point<T>& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const Point<T>& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:
    T _Xpos;
    T _Ypos;

    virtual Boolean IsEqualToPoint(const Point<T>* Ptr_) const;
    
  public:
    Point();
    Point(const T& x, const T& y);
    Point(const Point<T>& Obj_);

    static Point<T>* Make();
    static Point<T>* Make(const T& x, const T& y);
    static Point<T>* Make(const Point<T>& Obj_);

    Point<T>& operator = (const Point<T>& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Point<T>& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();    

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::POINT_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;            

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

    // Comparisons:
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    Point<T>& Setx(const T& x)
        { _Xpos = x; return *this; }
    Point<T>& Sety(const T& y)
        { _Ypos = y; return *this; }
    
    T Getx() const
        { return _Xpos; }
    T Gety() const
        { return _Ypos; }

    Point<T>& SlideX(const T& x)
        { _Xpos -= x; return *this; }
    Point<T>& SlideY(const T& y)
        { _YPos -= y; return *this; }

    Point<T>& Shrink(const T& x)
        { _Xpos *= x; return *this; }
    Point<T>& Stretch(const T& y)
        { _Ypos *= y; return *this; }

    Point<T>& FlipVertical()
        { _Xpos *= T(-1); return *this; }
    Point<T>& FlipHorizontal()
        { _Ypos *= T(-1); return *this; }

    double Distance(const Point& Obj_) const;
    Rational<T> Slope(const Point& Obj_) const;
    Rational<T> Xintercept(const Point& Obj_) const;
    Rational<T> Yintercept(const Point& Obj_) const;
    Boolean IsColinear(const Point<T>& pt1, const Point<T>& pt2) const;

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
#endif




