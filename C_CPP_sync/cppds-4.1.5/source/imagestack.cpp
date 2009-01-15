#ifndef IMAGESTACK_CPP
#define IMAGESTACK_CPP
#ifndef IMAGESTACK_H
  #include "imagestack.h"
#endif

/****************************************************************************/
/******************************** Image Node ********************************/
/****************************************************************************/
template <class T>
ImageNode<T>::ImageNode(T* Obj_, ImageNode<T>* Next_):
_Object(Obj_),
_Next(Next_)
{}

/****************************************************************************/
template <class T>
ImageNode<T>::~ImageNode()
{
  delete _Next;
  _Next = NULL;

  ::Delete(_Object);
  _Object = NULL;
}

/****************************************************************************/
template <class T>
void ImageNode<T>::Insert(ImageNode<T>* Node_)
{
  Node_->_Next = _Next;
  _Next = Node_;
}

/****************************************************************************/
/******************************** Image Stack *******************************/
/****************************************************************************/
template <class T>
ImageStack<T>::ImageStack():
_List(NULL)
{}

/****************************************************************************/
template <class T>
ImageStack<T>::ImageStack(const ImageStack<T>& Stck_):
_List(NULL)
{
  for (ImageNode<T>* Node_ = Stck_._List; Node_ != NULL; Node_ = Node_->_Next)
    AddNode(Node_->_Object);
}

/****************************************************************************/
template <class T>
ImageStack<T>::~ImageStack()
{
  delete _List;
  _List = NULL;
}

/****************************************************************************/
template <class T>
ImageStack<T>& ImageStack<T>::operator = (const ImageStack<T>& Stck_)
{
  if (this != &Stck_)
  {
    delete _List;
    _List = NULL;

    for (ImageNode<T>* Node_ = Stck_._List; Node_ != NULL; Node_ = Node_->_Next)
      AddNode(Node_->_Object);
  }

  return *this;
}

/****************************************************************************/
template <class T>
Boolean ImageStack<T>::Empty() const
{
  return (_List == NULL);
}

/****************************************************************************/
template <class T>
ImageNode<T>* ImageStack<T>::HasNode(T* Obj_, PtrCompare<T>& Compare_) const
{
  ImageNode<T>* Node_ = _List;

  if (Obj_)
    while (Node_ && !Compare_(Obj_, Node_->_Object))
      Node_ = Node_->_Next;

  return ((Obj_ && Node_) ? Node_:NULL);
}

/****************************************************************************/
template <class T>
ImageNode<T>* ImageStack<T>::HasNode(T* Obj_) const
{
  PtrEqual<T> Comp_;
  return HasNode(Obj_, Comp_);
}

/****************************************************************************/
template <class T>
void ImageStack<T>::AddNode(T* Obj_)
{
  ImageNode<T>* Node_ = _List;

  if (Obj_)
    if (_List)
    {
      while (Node_->_Next)
	Node_ = Node_->_Next;
      Node_->_Next = new ImageNode<T>(Obj_, Node_->_Next);
    }
    else
      _List = new ImageNode<T>(Obj_, _List);
}

/****************************************************************************/
template <class T>
void ImageStack<T>::Push(T* Obj_)
{
  _List = new ImageNode<T>(Obj_, _List);
}

/****************************************************************************/
template <class T>
T* ImageStack<T>::Pop()
{
  ImageNode<T>* Node_ = _List;

  if (Node_ == NULL)
    return NULL;

  T* Object_ = Node_->_Object;
  Node_->_Object = NULL; 	// So node won't try to delete it
  _List = Node_->_Next;
  Node_->_Next = NULL; 		// So it won't delete the list

  delete Node_;
  return Object_;
}

/****************************************************************************/
template <class T>
void ImageStack<T>::DeleteAll()
{
  delete _List;
  _List = NULL;
}

/****************************************************************************/
#endif





