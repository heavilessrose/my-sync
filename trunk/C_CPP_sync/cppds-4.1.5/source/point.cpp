#ifndef POINT_CPP
#define POINT_CPP
#ifndef POINT_H
  #include "point.h"
#endif

/****************************************************************************/
template <class T>
Point<T>::Point():
_Xpos(T(0)),
_Ypos(T(0))
{}

/****************************************************************************/
template <class T>
Point<T>::Point(const T& x, const T& y):
_Xpos(x),
_Ypos(y)
{}

/****************************************************************************/
template <class T>
Point<T>::Point(const Point<T>& Obj_):
_Xpos(Obj_._Xpos),
_Ypos(Obj_._Ypos)
{}

/****************************************************************************/
template <class T>
Point<T>* Point<T>::Make()
{
  return (new Point<T>());
}

/****************************************************************************/
template <class T>
Point<T>* Point<T>::Make(const T& x, const T& y)
{
  return (new Point<T>(x, y));
}

/****************************************************************************/
template <class T>
Point<T>* Point<T>::Make(const Point<T>& Obj_)
{
  return (new Point<T>(Obj_));
}

/****************************************************************************/
template <class T>
Point<T>& Point<T>::operator = (const Point<T>& Obj_)
{
  if (this != &Obj_)
  {
    _Xpos = Obj_._Xpos;
    _Ypos = Obj_._Ypos;
  }

  return *this;
}

/****************************************************************************/
template <class T>
Object& Point<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  PointAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromPoint(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* Point<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Point<T>(*this));

  PointAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromPoint(TrgComp_)));
}

/****************************************************************************/
template <class T>
Point<T>& Point<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Point<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& Point<T>::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>    
Object& Point<T>::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>    
int Point<T>::ClassID() const
{
  return TypeInfo::POINT_TYPE;
}

/****************************************************************************/
template <class T>    
const ObjectAcceptor* Point<T>::GiveObjectAcceptor() const
{
  return (new PointAcceptor<T>(this));
}

/****************************************************************************/
template <class T>    
long Point<T>::DataSize () const
{
  return (sizeof(T) * 2);
}

/****************************************************************************/
template <class T>    
istream& Point<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.seekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>        
istream& Point<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Xpos), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Ypos), sizeof(short)).good()))
      SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>        
ostream& Point<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Xpos), sizeof(short)).good()))
    *Ok_ = Os_.write((char*)(&_Ypos), sizeof(short)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& Point<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, TRUE);
  if (*Ok_ && Is_.iseekg(DataSize(), ios::cur).good())
    SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>        
ByteStream& Point<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Xpos), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Ypos), sizeof(short)).good()))
      SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>        
ByteStream& Point<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Xpos), sizeof(short)).good()))
    *Ok_ = Os_.owrite((char*)(&_Ypos), sizeof(short)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
istream& Point<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  T x, y;

  Is_ >>x >>y;
  *this = Point<T>(x, y);

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
template <class T>
ostream& Point<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<"(" <<_Xpos <<", " <<_Ypos <<")";
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
template <class T>
Boolean Point<T>::IsEqualToPoint(const Point<T>* Ptr_) const
{
  if (Ptr_)
    return
    (
      Ptr_->Getx() == Getx() &&
      Ptr_->Gety() == Gety()
    );

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean Point<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  PointAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToPoint(TrgComp_);
}

/****************************************************************************/
template <class T>
double Point<T>::Distance(const Point& Obj_) const
{
  T Horz_ = Obj_.Getx() - Getx();
  T Vert_ = Obj_.Gety() - Gety();

  return sqrt((Horz_ * Horz_) + (Vert_ * Vert_));
}

/****************************************************************************/
template <class T>
Rational<T> Point<T>::Slope(const Point& Obj_) const
{
  T Horz_ = Obj_.Getx() - Getx();
  T Vert_ = Obj_.Gety() - Gety();

  return Rational<T>(Vert_, Horz_);
}

/****************************************************************************/
template <class T>
Rational<T> Point<T>::Xintercept(const Point& Obj_) const
{
  Rational<T> Yint_ = Yintercept(Obj_);

  if (Yint_.GetNum() == T(0))
    return Rational<T>(0);

  Rational<T> Slope_ = Slope(Obj_);
  Yint_ = -Yint_;
  return (Yint_ / Slope_);
}

/****************************************************************************/
template <class T>
Rational<T> Point<T>::Yintercept(const Point& Obj_) const
{
  Rational<T> Slope_ = Slope(Obj_);
  Rational<T> Yval_(_Ypos);
  Rational<T> Xval_(_Xpos);
  
  return (Yval_ - (Slope_ * Xval_));
}

/****************************************************************************/
template <class T>
Boolean Point<T>::IsColinear(const Point<T>& pt1, const Point<T>& pt2) const
{
  Rational<T> Yint_ = Yintercept(pt1);
  Rational<T> Slope_ = Slope(pt1);

  return (Rational<T>(pt2.Gety()) == Slope_ * Rational<T>(pt2.Getx()) + Yint_);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* Point<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Point<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* Point<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Point<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
PointAcceptor<T>::PointAcceptor(const Point<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
Boolean PointAcceptor<T>::IsPoint() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void PointAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Point<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* PointAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
void* PointAcceptor<T>::CreateFromPoint(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Point<T>(*((const Point<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* PointAcceptor<T>::AssignFromPoint(const ObjectAcceptor& Obj_)
{
  const Point<T>* TrgPtr_ = (const Point<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean PointAcceptor<T>::IsEqualToPoint(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToPoint((const Point<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PointAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PointAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if POINT_DEBUG
int main()
{
  Point<int> pt1(1, 8);
  Point<int> pt2(2, 11);
  Point<int> pt3(3, 14);
  Point<int> pt4(4, 17);
  Point<int> npt5(4, 19);

  double Dist_ = pt1.Distance(npt5);
  cout <<Dist_ <<" == " <<sqrt(130) <<endl;

  Dist_ = pt1.Distance(pt4);
  cout <<Dist_ <<" == " <<sqrt(90) <<endl;

  Rational<int> slp_ = pt1.Slope(pt3);
  assert(slp_.GetNum() == 3);
  assert(slp_.GetDen() == 1);
  
  Rational<int> xint_ = pt2.Xintercept(pt4);
  assert(xint_.GetNum() == -5);
  assert(xint_.GetDen() == 3);

  Rational<int> yint_ = pt2.Yintercept(pt3);
  assert(yint_.GetNum() == 5);
  assert(yint_.GetDen() == 1);

  assert(pt1.IsColinear(pt2, pt3));
  assert(pt3.IsColinear(pt2, pt4));
  assert(!pt2.IsColinear(pt1, npt5));
  assert(!pt3.IsColinear(pt2, npt5));

  Point<int>* pt11p = (Point<int>*)pt1.Clone();
  Point<int> pt12(0, 0);
  
  assert(*pt11p == pt1);
  assert(pt12 != pt2);
  pt12.Assign(pt2);
  assert(pt12 == pt2);
  assert(pt12 != pt1);

  ofstream fout("testing.bin");
  Write(fout, pt1);
  fout.close();

  ifstream fin("testing.bin");
  Read(fin, pt12);
  fin.close();

  assert(pt12 == pt1);
  return 0;
}
#endif
/****************************************************************************/
#endif




