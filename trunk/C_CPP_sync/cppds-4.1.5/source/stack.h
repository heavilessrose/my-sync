#ifndef STACK_H
#define STACK_H
#ifndef SINGLELIST_CPP
  #include "slnklist.cpp"
#endif

#define STACK_DEBUG                     0
#define STACK_NOTIFY_SUCCESS            0

/****************************************************************************/
template <class T>
class Stack;

template <class T>
class StackAcceptor : public ObjectAcceptor
{
  friend class Stack<T>;

  protected:
    union
    {
      const Stack<T>* _ObjPtr;
      Stack<T>* _VarPtr;
    };

    StackAcceptor(const Stack<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Stack class specific acceptor methods
    virtual Boolean IsStack() const;
    
    virtual void* CreateFromStack(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleSublist(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromStack(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleSublist(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToStack(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToStack(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToStack(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class StackTypeDefn
{
  public:
    typedef SingleListNode<T> StackNode;
};

template <class T>
class Stack : public SingleList<T>
{
  ROOTOBJECTFNCS_DEFN(Object, Stack<T>)

  friend class SingleListNode<T>;
  friend class StackAcceptor<T>;

  friend Stack<T> operator + (const Stack<T>& List1_, const Stack<T>& List2_)
      { return (Stack<T>(List1_) += List2_); }

  protected:
    virtual Boolean IsEqualToStack(const Stack<T>* Ptr_) const;
    virtual Boolean IsLesserToStack(const Stack<T>* Ptr_) const;
    virtual Boolean IsGreaterToStack(const Stack<T>* Ptr_) const;

  public:
    Stack();
    Stack(Stack<T>* List_, int StoreFlags_);
    Stack(SingleList<T>* Sublist_, int StoreFlags_);
    Stack(SingleSublist<T>* Sublist_, int StoreFlags_);
    Stack(const Stack<T>& List_);
    Stack(const SingleList<T>& Sublist_);
    Stack(const SingleSublist<T>& Sublist_);    
    virtual ~Stack() {}

    // Virtual constructor and assignment methods
    static Stack<T>* Make();
    static Stack<T>* Make(Stack<T>* List_, int StoreFlags_);
    static Stack<T>* Make(SingleList<T>* Sublist_, int StoreFlags_);
    static Stack<T>* Make(SingleSublist<T>* Sublist_, int StoreFlags_);
    static Stack<T>* Make(const Stack<T>& List_);
    static Stack<T>* Make(const SingleList<T>& Sublist_);
    static Stack<T>* Make(const SingleSublist<T>& Sublist_);

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Stack<T>* CloneStack() const;

    // Static null object accessor methods
    static Stack<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELIST_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    Stack<T>& operator = (const Stack<T>& List_);
    Stack<T>& AssignListNode(SingleListNode<T>* Node_, int StoreFlags_);
    
    virtual Boolean Push(SingleSublist<T>* List_, Boolean DelArgList_=TRUE);
    virtual Boolean Push(SingleListNode<T>* Node_);
    virtual Boolean Push(const T& Obj_);

    virtual SingleListNode<T>* PopNode();
    virtual T* PopItem();
    virtual T* PeekItem();

    virtual SingleList<T>& Shrink(size_t Num_);

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




