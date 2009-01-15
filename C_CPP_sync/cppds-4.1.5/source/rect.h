#ifndef RECT_H
#define RECT_H

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

#define RECTANGLE_DEBUG         0

/****************************************************************************/
template <class T>
class Rectangle;

template <class T>
class RectangleAcceptor : public ObjectAcceptor
{
  friend class Rectangle<T>;

  protected:
    union
    {
      const Rectangle<T>* _ObjPtr;
      Rectangle<T>* _VarPtr;
    };

    RectangleAcceptor(const Rectangle<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Rectangle class specific acceptor methods
    virtual Boolean IsRectangle() const;
    
    virtual void* CreateFromRectangle(const ObjectAcceptor& Obj_);
    virtual void* AssignFromRectangle(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToRectangle(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToRectangle(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToRectangle(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class Rectangle : public ObjectSkeleton
{
  friend class RectangleAcceptor<T>;

  friend Object& RootObject(Rectangle<T>& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const Rectangle<T>& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:
    T _Left;
    T _Top;
    T _Right;
    T _Bottom;

    virtual Boolean IsEqualToRectangle(const Rectangle<T>* Ptr_) const;
    virtual Boolean IsLesserToRectangle(const Rectangle<T>* Ptr_) const;
    virtual Boolean IsGreaterToRectangle(const Rectangle<T>* Ptr_) const;
    
  public:
    Rectangle();
    Rectangle(const T& Left_, const T& Top_,
              const T& Right_, const T& Bottom_);
    Rectangle(const Rectangle<T>& Obj_);

    static Rectangle<T>* Make();
    static Rectangle<T>* Make(const T& Left_, const T& Top_,
                              const T& Right_, const T& Bottom_);
    static Rectangle<T>* Make(const Rectangle<T>& Obj_);

    Rectangle<T>& operator = (const Rectangle<T>& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static Rectangle<T>& GiveNullObject();

    // Null object methods
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();    

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::RECTANGLE_TYPE; }
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
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;

    Rectangle<T>& SetLeft(const T& x)
        { _Left = x; return *this; }
    Rectangle<T>& SetTop(const T& y)
        { _Top = y; return *this; }
    Rectangle<T>& SetRight(const T& x)
        { _Right = x; return *this; }
    Rectangle<T>& SetBottom(const T& y)
        { _Bottom = y; return *this; }

    T GetLeft() const
        { return _Left; }
    T GetTop() const
        { return _Top; }
    T GetRight() const
        { return _Right; }
    T GetBottom() const
        { return _Bottom; }

    Rectangle<T>& DragLeftToLeft(const T& x);
    Rectangle<T>& DragLeftToRight(const T& x);
    Rectangle<T>& DragTopToTop(const T& y);
    Rectangle<T>& DragTopToBottom(const T& y);    
    Rectangle<T>& DragRightToLeft(const T& x);
    Rectangle<T>& DragRightToRight(const T& x);
    Rectangle<T>& DragBottomToTop(const T& y);
    Rectangle<T>& DragBottomToBottom(const T& y);

    Rectangle<T>& MoveToLeft(const T& x);
    Rectangle<T>& MoveToRight(const T& x);
    Rectangle<T>& MoveToTop(const T& y);
    Rectangle<T>& MoveToBottom(const T& y);

    Rectangle<T>& Rotate90();

    T Area() const;
    T Perimeter() const;
    T Width() const;
    T Height() const;    
    T LongSide() const;
    T ShortSide() const;

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




