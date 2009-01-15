#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#ifndef SINGLELIST_CPP
  #include "slnklist.cpp"
#endif

#define PRIORITYQUEUE_DEBUG                     0
#define PRIORITYQUEUE_NOTIFY_SUCCESS            0

/****************************************************************************/
template <class T>
class PriorityQueue;

template <class T>
class PriorityQueueAcceptor : public ObjectAcceptor
{
  friend class PriorityQueue<T>;

  protected:
    union
    {
      const PriorityQueue<T>* _ObjPtr;
      PriorityQueue<T>* _VarPtr;
    };

    PriorityQueueAcceptor(const PriorityQueue<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // PriorityQueue class specific acceptor methods
    virtual Boolean IsPriorityQueue() const;
    
    virtual void* CreateFromPriorityQueue(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleSublist(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromPriorityQueue(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleSublist(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToPriorityQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToPriorityQueue(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToPriorityQueue(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class PriorityQueueTypeDefn
{
  public:
    typedef SingleListNode<T> PriorityQueueNode;
};

template <class T>
class PriorityQueue : public SingleList<T>
{
  ROOTOBJECTFNCS_DEFN(Object, PriorityQueue<T>)

  friend class SingleListNode<T>;
  friend class PriorityQueueAcceptor<T>;

  friend PriorityQueue<T> operator + (const PriorityQueue<T>& List1_, const PriorityQueue<T>& List2_)
      { return (PriorityQueue<T>(List1_) += List2_); }

  public:
    typedef int(*PriorityFuncType)(const T&);

  protected:
    PriorityFuncType _PriorityFunc;
  
    virtual Boolean IsEqualToPriorityQueue(const PriorityQueue<T>* Ptr_) const;
    virtual Boolean IsLesserToPriorityQueue(const PriorityQueue<T>* Ptr_) const;
    virtual Boolean IsGreaterToPriorityQueue(const PriorityQueue<T>* Ptr_) const;

  public:
    PriorityQueue();
    PriorityQueue(PriorityQueue<T>* List_, int StoreFlags_);
    PriorityQueue(SingleList<T>* Sublist_, int StoreFlags_);
    PriorityQueue(SingleSublist<T>* Sublist_, int StoreFlags_);
    PriorityQueue(const PriorityQueue<T>& List_);
    PriorityQueue(const SingleList<T>& Sublist_);
    PriorityQueue(const SingleSublist<T>& Sublist_);    
    virtual ~PriorityQueue() {}

    // Virtual constructor and assignment methods
    static PriorityQueue<T>* Make();
    static PriorityQueue<T>* Make(PriorityQueue<T>* List_, int StoreFlags_);
    static PriorityQueue<T>* Make(SingleList<T>* Sublist_, int StoreFlags_);
    static PriorityQueue<T>* Make(SingleSublist<T>* Sublist_, int StoreFlags_);
    static PriorityQueue<T>* Make(const PriorityQueue<T>& List_);
    static PriorityQueue<T>* Make(const SingleList<T>& Sublist_);
    static PriorityQueue<T>* Make(const SingleSublist<T>& Sublist_);

    // Set comparison function method
    void SetPriorityFunction(int(*PriorityFnc_)(const T&))
         { _PriorityFunc = PriorityFnc_; }

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual PriorityQueue<T>* ClonePriorityQueue() const;

    // Static null object accessor methods
    static PriorityQueue<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELIST_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    PriorityQueue<T>& operator = (const PriorityQueue<T>& List_);
    PriorityQueue<T>& AssignListNode(SingleListNode<T>* Node_, int StoreFlags_);
    
    virtual Boolean Insert(SingleSublist<T>* List_, Boolean DelArgList_=TRUE);
    virtual Boolean Insert(SingleListNode<T>* Node_);
    virtual Boolean Insert(const T& Obj_);

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




