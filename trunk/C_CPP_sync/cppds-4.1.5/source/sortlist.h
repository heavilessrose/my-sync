#ifndef SORTEDLIST_H
#define SORTEDLIST_H
#ifndef SINGLELIST_CPP
  #include "slnklist.cpp"
#endif

#define SORTEDLIST_DEBUG                     0
#define SORTEDLIST_NOTIFY_SUCCESS            0

/****************************************************************************/
template <class T>
class SortedList;

template <class T>
class SortedListAcceptor : public ObjectAcceptor
{
  friend class SortedList<T>;

  protected:
    union
    {
      const SortedList<T>* _ObjPtr;
      SortedList<T>* _VarPtr;
    };

    SortedListAcceptor(const SortedList<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // SortedList class specific acceptor methods
    virtual Boolean IsSortedList() const;
    
    virtual void* CreateFromSortedList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleSublist(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromSortedList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleSublist(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToSortedList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToSortedList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToSortedList(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class SortedListTypeDefn
{
  public:
    typedef SingleListNode<T> SortedListNode;
};

template <class T>
class SortedList : public SingleList<T>
{
  ROOTOBJECTFNCS_DEFN(Object, SortedList<T>)

  friend class SingleListNode<T>;
  friend class SortedListAcceptor<T>;

  friend SortedList<T> operator + (const SortedList<T>& List1_, const SortedList<T>& List2_)
      { return (SortedList<T>(List1_) += List2_); }

  protected:
    int _SortOrder;
  
    virtual Boolean IsEqualToSortedList(const SortedList<T>* Ptr_) const;
    virtual Boolean IsLesserToSortedList(const SortedList<T>* Ptr_) const;
    virtual Boolean IsGreaterToSortedList(const SortedList<T>* Ptr_) const;

  public:
    SortedList();
    SortedList(SortedList<T>* List_, int StoreFlags_);
    SortedList(SingleList<T>* Sublist_, int StoreFlags_);
    SortedList(SingleSublist<T>* Sublist_, int StoreFlags_);
    SortedList(const SortedList<T>& List_);
    SortedList(const SingleList<T>& Sublist_);
    SortedList(const SingleSublist<T>& Sublist_);    
    virtual ~SortedList() {}

    // Virtual constructor and assignment methods
    static SortedList<T>* Make();
    static SortedList<T>* Make(SortedList<T>* List_, int StoreFlags_);
    static SortedList<T>* Make(SingleList<T>* Sublist_, int StoreFlags_);
    static SortedList<T>* Make(SingleSublist<T>* Sublist_, int StoreFlags_);
    static SortedList<T>* Make(const SortedList<T>& List_);
    static SortedList<T>* Make(const SingleList<T>& Sublist_);
    static SortedList<T>* Make(const SingleSublist<T>& Sublist_);

    // Sort order set method
    void SetSortOrder(int Order_)
        { _SortOrder = Order_; }
    int GetSortOrder()
        { return _SortOrder; }

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual SortedList<T>* CloneSortedList() const;

    // Static null object accessor methods
    static SortedList<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELIST_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    SortedList<T>& operator = (const SortedList<T>& List_);
    SortedList<T>& AssignListNode(SingleListNode<T>* Node_, int StoreFlags_);
    
    virtual Boolean Insert(SingleSublist<T>* List_, Boolean DelArgList_=TRUE);
    virtual Boolean Insert(SingleListNode<T>* Node_);
    virtual Boolean Insert(const T& Obj_);

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




