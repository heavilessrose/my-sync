#ifndef IMAGEPTR_H
#define IMAGEPTR_H
#ifndef IMAGESTACK_CPP
  #include "imagestack.cpp"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif

#define IMAGEPTR_DEBUG	0

/****************************************************************************/
// Simple image pointer (pages 199-200)
template <class T>
class ImagePtr
{
  friend ImagePtr<T>& RootObject(ImagePtr<T>& Obj_)
	{ return &Obj_; }
  friend const ImagePtr<T>& RootConstObject(const ImagePtr<T>& Obj_)
	{ return &Obj_; }

  friend Boolean operator == (const ImagePtr<T>& r1, const ImagePtr<T>& r2)
        { return r1.IsEqual(r2); }
  friend Boolean operator != (const ImagePtr<T>& r1, const ImagePtr<T>& r2)
        { return !r1.IsEqual(r2); }

  friend Boolean _IsEqualTo(const ImagePtr<T>& Trg_, const ImagePtr<T>& Src_)
	{ return Trg_.IsEqual(Src_); }

  friend ImagePtr<T>* _Clone(const ImagePtr<T>& Obj_)
	{ return ((ImagePtr<T>*)Obj_.Clone()); }
  friend const ImagePtr<T>* _ConstClone(const ImagePtr<T>& Obj_)
        { return &Obj_; }  
  friend ImagePtr<T>* _NonConstClone(ImagePtr<T>& Obj_)
        { return &Obj_; }  
        
  friend int _WhatIs(const ImagePtr<T>& Obj_)
	{ return Obj_.ClassID(); }

  friend Boolean _IsMatchable(const ImagePtr<T>& Obj_)
	{ return TRUE; }  

  private:
    T* _Current; 	// Current image presented to client
    T* _Undo; 		// Previous image

  public:
    ImagePtr();
    ImagePtr(T* Ptr_);
    ImagePtr(const ImagePtr<T>& Ptr_);
    ~ImagePtr();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::IMAGEPTR_TYPE; }
    virtual int ClassID() const;

    ImagePtr<T>* Clone() const;
    ImagePtr<T>& operator = (const ImagePtr<T>& Ptr_);
    ImagePtr<T>& operator = (T* Ptr_);

    Boolean operator ! () const
	{ return _Current == NULL; }

    const T* Pointee() const
	{ return _Current; }
    T* Pointee()
	{ return _Current; }

    const T* operator->() const
	{ return _Current; }
    T* operator->()
	{ return _Current; }

    void Snapshot();
    void Commit();
    void Rollback();

    virtual Boolean IsEqual(const ImagePtr<T>& Ptr_) const;    

    Boolean HaveImage() const
	{ return (_Undo != NULL); }

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
// Image stack pointer (pages 201-202)
// Rollback() algorithm is considerably simpler here.
template <class T>
class ImageStackPtr
{
  friend ImageStackPtr<T>& RootObject(ImageStackPtr<T>& Obj_)
	{ return &Obj_; }
  friend const ImageStackPtr<T>& RootConstObject(const ImageStackPtr<T>& Obj_)
	{ return &Obj_; }

  friend Boolean operator == (const ImageStackPtr<T>& r1, const ImageStackPtr<T>& r2)
        { return r1.IsEqual(r2); }
  friend Boolean operator != (const ImageStackPtr<T>& r1, const ImageStackPtr<T>& r2)
        { return !r1.IsEqual(r2); }  

  friend Boolean _IsEqualTo(const ImageStackPtr<T>& Trg_, const ImageStackPtr<T>& Src_)
	{ return Trg_.IsEqual(Src_); }

  friend ImageStackPtr<T>* _Clone(const ImageStackPtr<T>& Obj_)
	{ return ((ImageStackPtr<T>*)Obj_.Clone()); }
  friend const ImageStackPtr<T>* _ConstClone(const ImageStackPtr<T>& Obj_)
        { return &Obj_; }
  friend ImageStackPtr<T>* _NonConstClone(ImageStackPtr<T>& Obj_)
        { return &Obj_; }
        
  friend int _WhatIs(const ImageStackPtr<T>& Obj_)
	{ return Obj_.ClassID(); }

  friend Boolean _IsMatchable(const ImageStackPtr<T>& Obj_)
	{ return TRUE; }  

  private:
    T* _Current; // Current image presented to client
    ImageStack<T> _History; // Previous images

  public:
    ImageStackPtr();
    ImageStackPtr(T* Ptr_);
    ImageStackPtr(const ImageStackPtr<T>& Ptr_);
    ~ImageStackPtr();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::IMAGESTACKPTR_TYPE; }
    virtual int ClassID() const;        

    ImageStackPtr<T>* Clone() const;
    ImageStackPtr<T>& operator = (const ImageStackPtr<T>& Ptr_);
    ImageStackPtr<T>& operator = (T* Ptr_);

    Boolean operator ! () const
	{ return _Current == NULL; }

    const T* Pointee() const
	{ return _Current; }
    T* Pointee()
	{ return _Current; }

    const T* operator->() const
	{ return _Current; }
    T* operator->()
	{ return _Current; }

    void PushImage();
    void Commit()
	{ _History.DeleteAll(); }

    void PopImage(); 	// Roll back one level
    void Rollback();    // Roll back to last image

    virtual Boolean IsEqual(const ImageStackPtr<T>& Ptr_) const;    

    Boolean HaveImage() const
	{ return !_History.Empty(); }

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
// Pointer image (pages 206-207)
// Added operator! to test for NULL pointer.
template <class T>
class PtrImage
{
  friend PtrImage<T>& RootObject(PtrImage<T>& Obj_)
	{ return &Obj_; }
  friend const PtrImage<T>& RootConstObject(const PtrImage<T>& Obj_)
	{ return &Obj_; }

  friend Boolean operator == (const PtrImage<T>& r1, const PtrImage<T>& r2)
        { return r1.IsEqual(r2); }
  friend Boolean operator != (const PtrImage<T>& r1, const PtrImage<T>& r2)
        { return !r1.IsEqual(r2); }  

  friend Boolean _IsEqualTo(const PtrImage<T>& Trg_, const PtrImage<T>& Src_)
	{ return Trg_.IsEqual(Src_); }

  friend PtrImage<T>* _Clone(const PtrImage<T>& Obj_)
	{ return ((PtrImage<T>*)Obj_.Clone()); }
  friend const PtrImage<T>* _ConstClone(const PtrImage<T>& Obj_)
	{ return ((const PtrImage<T>*)Obj_.Clone()); }
  friend PtrImage<T>* _NonConstClone(PtrImage<T>& Obj_)
	{ return ((PtrImage<T>*)Obj_.Clone()); }
        
  friend int _WhatIs(const PtrImage<T>& Obj_)
	{ return Obj_.ClassID(); }

  friend Boolean _IsMatchable(const PtrImage<T>& Obj_)
	{ return TRUE; }    

  private:
    T* _Current; 	// Current image presented to client
    T* _Undo; 		// Previous image
    Boolean _HaveImage; // True if there is an image

  public:
    PtrImage();
    PtrImage(T* Ptr_);
    PtrImage(const PtrImage<T>& Ptr_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::PTRIMAGE_TYPE; }
    virtual int ClassID() const;            

    PtrImage<T>* Clone() const;
    PtrImage<T>& operator = (const PtrImage<T>& Ptr_);
    PtrImage<T>& operator = (T* Ptr_);

    Boolean operator ! () const
	{ return _Current == NULL; }

    const T* Pointee() const
	{ return _Current; }
    T* Pointee()
	{ return _Current; }

    const T* operator->() const
	{ return _Current; }
    T* operator->()
	{ return _Current; }

    void Snapshot();
    void Commit();
    void Rollback();

    virtual Boolean IsEqual(const PtrImage<T>& Ptr_) const;

    Boolean HaveImage() const
	{ return _HaveImage; }

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





