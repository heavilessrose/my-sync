#ifndef DEQUE_H
#define DEQUE_H
#ifndef SINGLELIST_CPP
  #include "slnklist.cpp"
#endif

#define DEQUE_DEBUG                     0
#define DEQUE_NOTIFY_SUCCESS            0

/****************************************************************************/
template <class T>
class Deque;

template <class T>
class DequeAcceptor : public ObjectAcceptor
{
  friend class Deque<T>;

  protected:
    union
    {
      const Deque<T>* _ObjPtr;
      Deque<T>* _VarPtr;
    };

    DequeAcceptor(const Deque<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Deque class specific acceptor methods
    virtual Boolean IsDeque() const;
    
    virtual void* CreateFromDeque(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleSublist(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromDeque(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleSublist(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToDeque(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDeque(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToDeque(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class DequeTypeDefn
{
  public:
    typedef SingleListNode<T> DequeNode;
};

template <class T>
class Deque : public SingleList<T>
{
  ROOTOBJECTFNCS_DEFN(Object, Deque<T>)

  friend class SingleListNode<T>;
  friend class DequeAcceptor<T>;

  friend Deque<T> operator + (const Deque<T>& List1_, const Deque<T>& List2_)
      { return (Deque<T>(List1_) += List2_); }

  protected:
    virtual Boolean IsEqualToDeque(const Deque<T>* Ptr_) const;
    virtual Boolean IsLesserToDeque(const Deque<T>* Ptr_) const;
    virtual Boolean IsGreaterToDeque(const Deque<T>* Ptr_) const;

  public:
    Deque();
    Deque(Deque<T>* List_, int StoreFlags_);
    Deque(SingleList<T>* Sublist_, int StoreFlags_);
    Deque(SingleSublist<T>* Sublist_, int StoreFlags_);
    Deque(const Deque<T>& List_);
    Deque(const SingleList<T>& Sublist_);
    Deque(const SingleSublist<T>& Sublist_);    
    virtual ~Deque() {}

    // Virtual constructor and assignment methods
    static Deque<T>* Make();
    static Deque<T>* Make(Deque<T>* List_, int StoreFlags_);
    static Deque<T>* Make(SingleList<T>* Sublist_, int StoreFlags_);
    static Deque<T>* Make(SingleSublist<T>* Sublist_, int StoreFlags_);
    static Deque<T>* Make(const Deque<T>& List_);
    static Deque<T>* Make(const SingleList<T>& Sublist_);
    static Deque<T>* Make(const SingleSublist<T>& Sublist_);

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Deque<T>* CloneDeque() const;

    // Static null object accessor methods
    static Deque<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELIST_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    Deque<T>& operator = (const Deque<T>& List_);
    Deque<T>& AssignListNode(SingleListNode<T>* Node_, int StoreFlags_);
    
    virtual Boolean Push(SingleSublist<T>* List_, int Relation_, Boolean DelArgList_=TRUE);
    virtual Boolean Push(SingleListNode<T>* Node_, int Relation_);
    virtual Boolean Push(const T& Obj_, int Relation_);

    virtual SingleListNode<T>* PopNode(int Relation_);
    virtual T* PopItem(int Relation_);
    virtual T* PeekItem(int Relation_);

    virtual SingleList<T>& Shrink(size_t Num_, int Relation_);
    virtual SingleList<T>& ShrinkTo(size_t Num_, int Relation_);

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




