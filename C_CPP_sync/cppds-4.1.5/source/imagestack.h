#ifndef IMAGESTACK_H
#define IMAGESTACK_H
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef COMPARE_CPP
  #include "compare.cpp"
#endif
#ifndef BUILTIN_H
  #include "builtin.h"
#endif

/****************************************************************************/
template <class T>
struct ImageNode
{
  T* _Object;
  ImageNode* _Next;

  ImageNode(T* Obj_, ImageNode<T>* Next_ = NULL);
  virtual ~ImageNode();

  void Insert(ImageNode<T>* Node_);
};

/****************************************************************************/
// Stack template for use with image stacks (page 200)
template <class T>
class ImageStack
{
  protected:
    ImageNode<T>* _List;

    ImageNode<T>* HasNode(T* Obj_, PtrCompare<T>& Compare_) const;
    ImageNode<T>* HasNode(T* Obj_) const;

    void AddNode(T* Obj_);

  public:
    ImageStack();
    ImageStack(const ImageStack<T>& s);
    virtual ~ImageStack();

    static T* Deref(ImageNode<T>* Ptr_)
	{ return (Ptr_ ? Ptr_->_Object:((T*)NULL)); }

    ImageStack<T>& operator = (const ImageStack<T>& s);

    Boolean Empty() const;

    void Push(T* Obj_);
    T* Pop();
    void DeleteAll();
};

/****************************************************************************/
#endif




