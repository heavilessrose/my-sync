#ifndef RECT_CPP
#define RECT_CPP
#ifndef RECT_H
  #include "rect.h"
#endif

/****************************************************************************/
template <class T>
Rectangle<T>::Rectangle():
_Left(0),
_Top(0),
_Right(0),
_Bottom(0)
{}

/****************************************************************************/
template <class T>
Rectangle<T>::Rectangle(const T& Left_, const T& Top_, const T& Right_, const T& Bottom_):
_Left(Left_),
_Top(Top_),
_Right(Right_),
_Bottom(Bottom_)
{}

/****************************************************************************/
template <class T>
Rectangle<T>::Rectangle(const Rectangle<T>& Obj_):
_Left(Obj_._Left),
_Top(Obj_._Top),
_Right(Obj_._Right),
_Bottom(Obj_._Bottom)
{}

/****************************************************************************/
template <class T>
Rectangle<T>* Rectangle<T>::Make()
{
  return (new Rectangle<T>());
}

/****************************************************************************/
template <class T>
Rectangle<T>* Rectangle<T>::Make(const T& Left_, const T& Top_, const T& Right_, const T& Bottom_)
{
  return (new Rectangle<T>(Left_, Top_, Right_, Bottom_));
}

/****************************************************************************/
template <class T>
Rectangle<T>* Rectangle<T>::Make(const Rectangle<T>& Obj_)
{
  return (new Rectangle<T>(Obj_));
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::operator = (const Rectangle<T>& Obj_)
{
  if (this != &Obj_)
  {
    _Left = Obj_._Left;
    _Top = Obj_._Top;
    _Right = Obj_._Right;
    _Bottom = Obj_._Bottom;
  }

  return *this;
}

/****************************************************************************/
template <class T>
Object& Rectangle<T>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  RectangleAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromRectangle(TrgComp_)));
}

/****************************************************************************/
template <class T>
Object* Rectangle<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Rectangle<T>(*this));

  RectangleAcceptor<T> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromRectangle(TrgComp_)));
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Rectangle<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class T>
const Object& Rectangle<T>::NULLOBJECT() const
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>    
Object& Rectangle<T>::NULLOBJECT()
{
  return GiveNullObject();
}

/****************************************************************************/
template <class T>    
int Rectangle<T>::ClassID() const
{
  return TypeInfo::RECTANGLE_TYPE;
}

/****************************************************************************/
template <class T>    
const ObjectAcceptor* Rectangle<T>::GiveObjectAcceptor() const
{
  return (new RectangleAcceptor<T>(this));
}

/****************************************************************************/
template <class T>    
long Rectangle<T>::DataSize () const
{
  return (sizeof(T) * 4);
}

/****************************************************************************/
template <class T>    
istream& Rectangle<T>::BinaryIgnore(istream& Is_, Boolean* Ok_)
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
istream& Rectangle<T>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Left), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Top), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Right), sizeof(short)).good()))
        if (*Ok_ && (*Ok_ = Is_.read((char*)(&_Bottom), sizeof(short)).good()))    
          SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>        
ostream& Rectangle<T>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Left), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Top), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Os_.write((char*)(&_Right), sizeof(short)).good()))
        *Ok_ = Os_.write((char*)(&_Bottom), sizeof(short)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
ByteStream& Rectangle<T>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
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
ByteStream& Rectangle<T>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);
  if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Left), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Top), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Right), sizeof(short)).good()))
        if (*Ok_ && (*Ok_ = Is_.iread((char*)(&_Bottom), sizeof(short)).good()))
          SetIOstate(TRUE, IO_CLEAR, TRUE);

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

/****************************************************************************/
template <class T>        
ByteStream& Rectangle<T>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);
  if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Left), sizeof(short)).good()))
    if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Top), sizeof(short)).good()))
      if (*Ok_ && (*Ok_ = Os_.owrite((char*)(&_Right), sizeof(short)).good()))
        *Ok_ = Os_.owrite((char*)(&_Bottom), sizeof(short)).good();

  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

/****************************************************************************/
template <class T>
istream& Rectangle<T>::TextRead(istream& Is_, Boolean* Ok_)
{
  T Left_, Top_, Right_, Bottom_;

  Is_ >>Left_ >>Top_ >>Right_ >>Bottom_;
  *this = Rectangle<T>(Left_, Top_, Right_, Bottom_);

  return ObjectImp::TextRead(Is_, Ok_);
}

/****************************************************************************/
template <class T>
ostream& Rectangle<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Os_ <<"(" <<_Left <<", " <<_Top <<"), "
      <<"(" <<_Right <<", " <<_Bottom <<")";
  return ObjectImp::TextWrite(Os_, Ok_);
}

/****************************************************************************/
template <class T>
Boolean Rectangle<T>::IsEqualToRectangle(const Rectangle<T>* Ptr_) const
{
  if (Ptr_)
    return (Area() == Ptr_->Area());

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean Rectangle<T>::IsLesserToRectangle(const Rectangle<T>* Ptr_) const
{
  if (Ptr_)
    return (Area() < Ptr_->Area());

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean Rectangle<T>::IsGreaterToRectangle(const Rectangle<T>* Ptr_) const
{
  if (Ptr_)
    return (Area() > Ptr_->Area());

  return FALSE;
}

/****************************************************************************/
template <class T>
Boolean Rectangle<T>::IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  RectangleAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsEqual(Obj_, &TrgComp_);

  return ObjComp_->IsEqualToRectangle(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean Rectangle<T>::IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  RectangleAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsLesser(Obj_, &TrgComp_);

  return ObjComp_->IsLesserToRectangle(TrgComp_);
}

/****************************************************************************/
template <class T>
Boolean Rectangle<T>::IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_) const
{
  RectangleAcceptor<T> TrgComp_(this);

  if (ObjComp_ == NULL)
    return Obj_.IsGreater(Obj_, &TrgComp_);

  return ObjComp_->IsGreaterToRectangle(TrgComp_);
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragLeftToLeft(const T& x)
{
  _Left -= x;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragLeftToRight(const T& x)
{
  _Left += x;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragTopToTop(const T& y)
{
  _Top -= y;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragTopToBottom(const T& y)
{
  _Top += y;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragRightToLeft(const T& x)
{
  _Right -= x;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragRightToRight(const T& x)
{
  _Right += x;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragBottomToTop(const T& y)
{
  _Bottom -= y;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::DragBottomToBottom(const T& y)
{
  _Bottom += y;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::MoveToLeft(const T& x)
{
  _Left -= x;
  _Right -= x;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::MoveToRight(const T& x)
{
  _Left += x;
  _Right += x;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::MoveToTop(const T& y)
{
  _Top -= y;
  _Bottom -= y;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::MoveToBottom(const T& y)
{
  _Top += y;
  _Bottom += y;
  return *this;
}

/****************************************************************************/
template <class T>
Rectangle<T>& Rectangle<T>::Rotate90()
{
  T Ht_ = Height();
  T Wd_ = Width();
  T Ls_ = LongSide();

  if (Ls_ == Wd_)
  {
    T Incr_ = (Wd_ - Ht_) / 2;
    DragTopToTop(Incr_);
    DragLeftToRight(Incr_);
    DragBottomToBottom(Incr_);
    DragRightToLeft(Incr_);
  }
  else
  {
    T Incr_ = (Ht_ - Wd_) / 2;
    DragTopToBottom(Incr_);
    DragLeftToLeft(Incr_);
    DragBottomToTop(Incr_);
    DragRightToRight(Incr_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
T Rectangle<T>::Area() const
{
  if (IsIntegerType(&_Left))
    return T(labs(_Right - _Left) * labs(_Bottom - _Top));

  return T(fabs(_Right - _Left) * fabs(_Bottom - _Top));
}

/****************************************************************************/
template <class T>
T Rectangle<T>::Perimeter() const
{
  if (IsIntegerType(&_Left))
    return T(labs(_Right - _Left) * 2 + labs(_Bottom - _Top) * 2);

  return T(fabs(_Right - _Left) * 2 + fabs(_Bottom - _Top) * 2);
}

/****************************************************************************/
template <class T>
T Rectangle<T>::Width() const
{
  if (IsIntegerType(&_Left))
    return T(labs(_Right - _Left));

  return T(fabs(_Right - _Left));
}

/****************************************************************************/
template <class T>
T Rectangle<T>::Height() const
{
  if (IsIntegerType(&_Left))
    return T(labs(_Bottom - _Top));

  return T(fabs(_Bottom - _Top));
}

/****************************************************************************/
template <class T>
T Rectangle<T>::LongSide() const
{
  T Ht_ = Height();
  T Wd_ = Width();
  return ((Ht_ > Wd_) ? Ht_:Wd_);
}

/****************************************************************************/
template <class T>
T Rectangle<T>::ShortSide() const
{
  T Ht_ = Height();
  T Wd_ = Width();
  return ((Ht_ < Wd_) ? Ht_:Wd_);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* Rectangle<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Rectangle<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if HAS_ARRAY_NEW
template <class T>
void* Rectangle<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void Rectangle<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
RectangleAcceptor<T>::RectangleAcceptor(const Rectangle<T>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class T>
Boolean RectangleAcceptor<T>::IsRectangle() const
{
  return TRUE;
}

/****************************************************************************/
template <class T>
void RectangleAcceptor<T>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Rectangle<T>*)Ptr_;
}

/****************************************************************************/
template <class T>
const void* RectangleAcceptor<T>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class T>
void* RectangleAcceptor<T>::CreateFromRectangle(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Rectangle<T>(*((const Rectangle<T>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class T>
void* RectangleAcceptor<T>::AssignFromRectangle(const ObjectAcceptor& Obj_)
{
  const Rectangle<T>* TrgPtr_ = (const Rectangle<T>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class T>
Boolean RectangleAcceptor<T>::IsEqualToRectangle(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsEqualToRectangle((const Rectangle<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean RectangleAcceptor<T>::IsLesserToRectangle(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsLesserToRectangle((const Rectangle<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
template <class T>
Boolean RectangleAcceptor<T>::IsGreaterToRectangle(const ObjectAcceptor& Obj_) const
{
  return _ObjPtr->IsGreaterToRectangle((const Rectangle<T>*)Obj_.GiveDataPtr());
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* RectangleAcceptor<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void RectangleAcceptor<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
/****************************************************************************/
#if RECTANGLE_DEBUG
int main()
{
  Rectangle<int> r1(1, 1, 21, 41);
  Rectangle<int> r2(30, 30, 60, 90);
  Rectangle<int> r3(r1);
  Rectangle<int> r4;

  assert(r1.Area() == 800);
  assert(r2.Area() == 1800);
  assert(r2.Perimeter() == 180);
  assert(r1 == r3);
  assert(r1 < r2);
  assert(r2 > r3);
  assert(r2.Width() == 30);
  assert(r2.Height() == 60);
  r2.Rotate90();
  cout <<r2 <<endl;
  return 0;
}
#endif
/****************************************************************************/
#endif




