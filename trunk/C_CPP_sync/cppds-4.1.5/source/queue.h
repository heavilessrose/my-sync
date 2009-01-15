#ifndef QUEUE_H
#define QUEUE_H
#ifndef SINGLELIST_CPP
  #include "slnklist.cpp"
#endif

#define QUEUE_DEBUG                     0
#define QUEUE_NOTIFY_SUCCESS            0

/****************************************************************************/
template <class T>
class Queue;

template <class T>
class QueueAcceptor : public ObjectAcceptor
{
  friend class Queue<T>;

  protected:
    union
    {
      const Queue<T>* _ObjPtr;
      Queue<T>* _VarPtr;
    };

    QueueAcceptor(const Queue<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Queue class specific acceptor methods
    virtual Boolean IsQueue() const;
    
    virtual void* CreateFromQueue(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleSublist(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromQueue(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleSublist(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToQueue(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class QueueTypeDefn
{
  public:
    typedef SingleListNode<T> QueueNode;
};

template <class T>
class Queue : public SingleList<T>
{
  ROOTOBJECTFNCS_DEFN(Object, Queue<T>)

  friend class SingleListNode<T>;
  friend class QueueAcceptor<T>;

  friend Queue<T> operator + (const Queue<T>& List1_, const Queue<T>& List2_)
      { return (Queue<T>(List1_) += List2_); }

  protected:
    virtual Boolean IsEqualToQueue(const Queue<T>* Ptr_) const;
    virtual Boolean IsLesserToQueue(const Queue<T>* Ptr_) const;
    virtual Boolean IsGreaterToQueue(const Queue<T>* Ptr_) const;

  public:
    Queue();
    Queue(Queue<T>* List_, int StoreFlags_);
    Queue(SingleList<T>* Sublist_, int StoreFlags_);
    Queue(SingleSublist<T>* Sublist_, int StoreFlags_);
    Queue(const Queue<T>& List_);
    Queue(const SingleList<T>& Sublist_);
    Queue(const SingleSublist<T>& Sublist_);    
    virtual ~Queue() {}

    // Virtual constructor and assignment methods
    static Queue<T>* Make();
    static Queue<T>* Make(Queue<T>* List_, int StoreFlags_);
    static Queue<T>* Make(SingleList<T>* Sublist_, int StoreFlags_);
    static Queue<T>* Make(SingleSublist<T>* Sublist_, int StoreFlags_);
    static Queue<T>* Make(const Queue<T>& List_);
    static Queue<T>* Make(const SingleList<T>& Sublist_);
    static Queue<T>* Make(const SingleSublist<T>& Sublist_);

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Queue<T>* CloneQueue() const;

    // Static null object accessor methods
    static Queue<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELIST_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    Queue<T>& operator = (const Queue<T>& List_);
    Queue<T>& AssignListNode(SingleListNode<T>* Node_, int StoreFlags_);
    
    virtual Boolean Enqueue(SingleSublist<T>* List_, Boolean DelArgList_=TRUE);
    virtual Boolean Enqueue(SingleListNode<T>* Node_);
    virtual Boolean Enqueue(const T& Obj_);

    virtual SingleListNode<T>* PopNode();
    virtual T* PopItem();
    virtual T* PeekItem();

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




