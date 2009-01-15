#ifndef IMAGEPTR_CPP
#define IMAGEPTR_CPP
#ifndef IMAGEPTR_H
  #include "imageptr.h"
#endif

/****************************************************************************/
/****************************************************************************/
template <class T>
ImagePtr<T>::ImagePtr():
_Undo(NULL),
_Current(new T)
{}

/****************************************************************************/
template <class T>
ImagePtr<T>::ImagePtr(T* Ptr_):
_Undo(NULL),
_Current(new T(*Ptr_))
{}

/****************************************************************************/
template <class T>
ImagePtr<T>::ImagePtr(const ImagePtr<T>& Ptr_):
_Current(new T(*(Ptr_._Current))),
_Undo(NULL)
{}

/****************************************************************************/
template <class T>
ImagePtr<T>::~ImagePtr()
{
  ::Delete(_Current);
  ::Delete(_Undo);
}

/****************************************************************************/
template <class T>
int ImagePtr<T>::ClassID() const
{
  return TypeInfo::IMAGEPTR_TYPE;
}

/****************************************************************************/
template <class T>
ImagePtr<T>* ImagePtr<T>::Clone() const
{
  return (new ImagePtr<T>(*this));
}

/****************************************************************************/
template <class T>
ImagePtr<T>& ImagePtr<T>::operator = (const ImagePtr<T>& Ptr_)
{
  if (this != &Ptr_)
  {
    ::Delete(_Current);
    _Current = new T(*(Ptr_._Current));
  }

  return *this;
}

/****************************************************************************/
template <class T>
ImagePtr<T>& ImagePtr<T>::operator = (T* Ptr_)
{
  if (Ptr_ != NULL)
  {
    ::Delete(_Current);
    _Current = new T(*Ptr_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
void ImagePtr<T>::Snapshot()
{
  ::Delete(_Undo); // In case there was an old one
  _Undo = _Current;
  _Current = new T(*_Undo);
}

/****************************************************************************/
template <class T>
void ImagePtr<T>::Commit()
{
  ::Delete(_Undo);
  _Undo = NULL;
}

/****************************************************************************/
template <class T>
void ImagePtr<T>::Rollback()
{
  if (_Undo != NULL)
  {
    ::Delete(_Current);
    _Current = _Undo;
    _Undo = NULL;
  }
}

/****************************************************************************/
template <class T>
Boolean ImagePtr<T>::IsEqual(const ImagePtr<T>& Ptr_) const
{
  return (*_Current == *Ptr_._Current);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ImagePtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ImagePtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ImagePtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ImagePtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
ImageStackPtr<T>::ImageStackPtr():
_Current(new T)
{}

/****************************************************************************/
template <class T>
ImageStackPtr<T>::ImageStackPtr(T* Ptr_):
_Current(new T(*Ptr_))
{}

/****************************************************************************/
template <class T>
ImageStackPtr<T>::ImageStackPtr(const ImageStackPtr<T>& Ptr_):
_Current(new T(*(Ptr_._Current)))
{}

/****************************************************************************/
template <class T>
ImageStackPtr<T>::~ImageStackPtr()
{
  ::Delete(_Current);
}

/****************************************************************************/
template <class T>
int ImageStackPtr<T>::ClassID() const
{
  return TypeInfo::IMAGESTACKPTR_TYPE;
}

/****************************************************************************/
template <class T>
ImageStackPtr<T>* ImageStackPtr<T>::Clone() const
{
  return (new ImageStackPtr<T>(*this));
}

/****************************************************************************/
template <class T>
ImageStackPtr<T>& ImageStackPtr<T>::operator = (const ImageStackPtr<T>& Ptr_)
{
  if (this != &Ptr_)
  {
    ::Delete(_Current);
    _Current = new T(*(Ptr_._Current));
  }

  return *this;
}

/****************************************************************************/
template <class T>
ImageStackPtr<T>& ImageStackPtr<T>::operator = (T* Ptr_)
{
  if (Ptr_ != NULL)
  {
    ::Delete(_Current);
    _Current = new T(*Ptr_);
  }

  return *this;
}

/****************************************************************************/
template <class T>
void ImageStackPtr<T>::PushImage()
{
  _History.Push(_Current);
  _Current = new T(*_Current);
}

/****************************************************************************/
template <class T>
void ImageStackPtr<T>::PopImage() // Roll back one level
{
  if (!_History.Empty())
  {
    ::Delete(_Current);
    _Current = _History.Pop();
  }
}

/****************************************************************************/
template <class T>
void ImageStackPtr<T>::Rollback() // Roll back to oldest image
{
  while (!_History.Empty())
  {
    ::Delete(_Current);
    _Current = _History.Pop();
  }
}

/****************************************************************************/
template <class T>
Boolean ImageStackPtr<T>::IsEqual(const ImageStackPtr<T>& Ptr_) const
{
  return (*_Current == *Ptr_._Current);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* ImageStackPtr<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void ImageStackPtr<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* ImageStackPtr<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void ImageStackPtr<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
template <class T>
PtrImage<T>::PtrImage():
_HaveImage(FALSE),
_Current(NULL),
_Undo(NULL)
{}

/****************************************************************************/
template <class T>
PtrImage<T>::PtrImage(T* Ptr_):
_HaveImage(FALSE),
_Current(Ptr_),
_Undo(NULL)
{}

/****************************************************************************/
template <class T>
PtrImage<T>::PtrImage(const PtrImage<T>& Ptr_):
_HaveImage(FALSE),
_Current(Ptr_._Current),
_Undo(NULL)
{}

/****************************************************************************/
template <class T>
int PtrImage<T>::ClassID() const
{
  return TypeInfo::PTRIMAGE_TYPE;
}

/****************************************************************************/
template <class T>
PtrImage<T>* PtrImage<T>::Clone() const
{
  return (new PtrImage<T>(*this));
}

/****************************************************************************/
template <class T>
PtrImage<T>& PtrImage<T>::operator = (const PtrImage<T>& Ptr_)
{
  if (this != &Ptr_)
    _Current = Ptr_._Current;

  return *this;
}

/****************************************************************************/
template <class T>
PtrImage<T>& PtrImage<T>::operator = (T* Ptr_)
{
  _Current = Ptr_;
  return *this;
}

/****************************************************************************/
template <class T>
void PtrImage<T>::Snapshot()
{
  _Undo = _Current;
  _HaveImage = TRUE;
}

/****************************************************************************/
template <class T>
void PtrImage<T>::Commit()
{
  _Undo = NULL;
  _HaveImage = FALSE;
}

/****************************************************************************/
template <class T>
void PtrImage<T>::Rollback()
{
  if (_HaveImage)
  {
    _Current = _Undo;
    _HaveImage = FALSE;
  }
}

/****************************************************************************/
template <class T>
Boolean PtrImage<T>::IsEqual(const PtrImage<T>& Ptr_) const
{
  return (_Current == Ptr_._Current);
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class T>
void* PtrImage<T>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class T>
void PtrImage<T>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void* PtrImage<T>::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
template <class T>
void PtrImage<T>::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#if IMAGEPTR_DEBUG
class YoClass
{
  DEFAULT_ROOTOBJECT_DEFN(YoClass)

  private:
    int x;

  public:
    int Getx() const
	{ return x; }
    void Setx(int p)
	{ x = p; }
};

int operator == (const YoClass& y1, const YoClass& y2)
	{ return (y1.Getx() == y2.Getx()); }

int operator < (const YoClass& y1, const YoClass& y2)
	{ return (y1.Getx() < y2.Getx()); }

int operator > (const YoClass& y1, const YoClass& y2)
	{ return (y1.Getx() < y2.Getx()); }

int main()
{
  YoClass Samp_;
  YoClass Samp2_;
  YoClass Samp3_;
  YoClass Samp4_;

  ImagePtr<YoClass> Yo1_;
  PtrImage<YoClass> Yo2_;
  ImageStackPtr<YoClass> Yo3_;
  ImagePtr<YoClass> Yo4_;
  PtrImage<YoClass> Yo5_;
  ImageStackPtr<YoClass> Yo6_;

  Samp_.Setx(10);
  Samp2_.Setx(5);
  Samp3_.Setx(15);
  Samp4_.Setx(25);

  Yo1_ = &Samp_;
  Yo1_.Snapshot();
  cout <<Yo1_.HaveImage() <<" ";
  cout <<Yo1_->Getx() <<" ";
  Yo1_ = &Samp2_;
  cout <<Yo1_->Getx() <<" ";
  Yo1_.Rollback();
  cout <<Yo1_->Getx() <<" ";
  Yo1_.Snapshot();
  Yo1_ = &Samp2_;
  Yo1_.Commit();
  Yo1_.Rollback();
  cout <<Yo1_->Getx() <<endl;

  Yo2_ = &Samp_;
  Yo2_.Snapshot();
  cout <<Yo2_.HaveImage() <<" ";
  cout <<Yo2_->Getx() <<" ";
  Yo2_ = &Samp2_;
  cout <<Yo2_->Getx() <<" ";
  Yo2_.Rollback();
  cout <<Yo2_->Getx() <<" ";
  Yo2_.Snapshot();
  Yo2_ = &Samp2_;
  Yo2_.Commit();
  Yo2_.Rollback();
  cout <<Yo2_->Getx() <<endl;

  Yo3_ = &Samp_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  cout <<Yo3_.HaveImage() <<" ";
  Yo3_ = &Samp2_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  Yo3_ = &Samp3_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  Yo3_ = &Samp4_;
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PopImage();
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PopImage();
  cout <<Yo3_->Getx() <<" ";
  Yo3_.PushImage();
  Yo3_ = &Samp4_;
  Yo3_.Commit();
  Yo3_.Rollback();
  cout <<Yo3_->Getx() <<endl;

  Yo4_ = &Samp_;
  Yo1_ = &Samp_;
  cout <<(Yo1_ == Yo4_) <<" ";
  cout <<(Yo1_ != Yo4_) <<endl;

  Yo5_ = &Samp2_;
  Yo2_ = &Samp2_;
  cout <<(Yo2_ == Yo5_) <<" ";
  cout <<(Yo2_ != Yo5_) <<endl;

  Yo6_ = &Samp3_;
  Yo3_ = &Samp3_;
  cout <<(Yo3_ == Yo6_) <<" ";
  cout <<(Yo3_ != Yo6_) <<endl;

  return 0;
}
#endif
/****************************************************************************/
#endif





