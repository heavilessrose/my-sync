#ifndef SINGLELIST_H
#define SINGLELIST_H
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef COMPARE_CPP
  #include "compare.cpp"
#endif
#ifndef DATASTORE_CPP
  #include "datastore.cpp"
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef CONTAINER_H
  #include "contain.h"
#endif
#ifndef ITERATOR_CPP
  #include "iterator.cpp"
#endif
#ifndef LISTDEFS_H
  #include "listdefs.h"
#endif
#ifndef SIMPLEARRAY_CPP
  #include "simplearray.cpp"
#endif

// double list debug switches
#define SINGLELIST_DEBUG	                0
#define SINGLELIST_STANDARD_DEBUG	        0
#define SINGLELIST_GENERAL_DEBUG                0
#define SINGLELIST_NOTIFY_SUCCESS	        1
#define SINGLELIST_CLONE_TEST                   1
#define SINGLELIST_SHOW_DEBUG_OUTPUT            1

// double list implementation options
#define SINGLELIST_IMPLEMENT_STLCONTAINER       1
#define SINGLELIST_IMPLEMENT_STLITERATOR        1

/****************************************************************************/
template <class T>
class SingleList;

template <class T>
class SingleSublist;

template <class T>
class SingleListIterator;

template <class T>
class SingleListAcceptor : public ObjectAcceptor
{
  friend class SingleList<T>;

  protected:
    union
    {
      const SingleList<T>* _ObjPtr;
      SingleList<T>* _VarPtr;
    };

    SingleListAcceptor(const SingleList<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // SingleList class specific acceptor methods
    virtual Boolean IsSingleList() const;
    
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleSublist(const ObjectAcceptor& Obj_);
    
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleSublist(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToSingleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToSingleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToSingleList(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class SingleListNode : public ObjectSkeleton
{
  ROOTOBJECTFNCS_DEFN(Object, SingleListNode<T>)
  
  friend class SingleList<T>;
  friend class SingleSublist<T>;

  protected:
    DataStorePtr<T> _Data;
    SingleListNode<T>* _Next;

    inline void Xnull() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

    SingleListNode<T>* ReplaceData(SingleListNode<T>* Node_);

  public:
    SingleListNode();
    SingleListNode(const T& Data_);
    SingleListNode(T* Destination_, int StoreFlags_);
    SingleListNode(const SingleListNode<T>& Node_);
    SingleListNode(const SingleListNode<T>& Node_, int, int);    
    virtual ~SingleListNode();

    static size_t NumNodes(const SingleListNode<T>* Node_);

    static SingleListNode<T>* Make();
    static SingleListNode<T>* Make(const T& Data_);
    static SingleListNode<T>* Make(T* Destination_, int StoreFlags_);
    static SingleListNode<T>* Make(const SingleListNode<T>& Node_);
    static SingleListNode<T>* Make(const SingleListNode<T>& Node_, int, int);

    // Static null object accessor methods
    static SingleListNode<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELISTNODE_TYPE; }
    virtual int ClassID() const;

    // object cloning method
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;        
    virtual SingleListNode<T>* CloneNode() const;

    SingleListNode<T>& operator = (const SingleListNode<T>& Node_);

    // list node add/replace/clone methods
    virtual SingleListNode<T>* Add(SingleSublist<T>* Sublist_, int Relation_);
    virtual SingleListNode<T>* Add(SingleListNode<T>* Node_, int Relation_);

    // list node item methods
    virtual SingleListNode<T>* Erase();
    virtual SingleListNode<T>* SetItem(T* Ptr_, int StoreFlags_);
    virtual SingleListNode<T>* SetItem(const T& Obj_);
    virtual const T* GetItem() const;
    virtual T* GetItem();

    virtual int CompareItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const;
    virtual const T* FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const;
    virtual T* FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&));
    
    virtual int ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const;
    virtual int ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_);
    virtual int ApplyUnary(int(*Fnc_)(T*));
    virtual int ApplyUnary(T(*Fnc_)(T));

    // listnode sublisting methods
    virtual SingleListNode<T>* DetachNode();

    // storage attribute method
    virtual int GetStorageAttributes() const;

    // List links methods
    SingleListNode<T>* SetNext(SingleListNode<T>* Next_);

    const SingleListNode<T>* Next(size_t Nodes_=1) const;
    SingleListNode<T>* Next(size_t Nodes_=1);
    const SingleListNode<T>* Last() const;
    SingleListNode<T>* Last();

    // data storage release method
    void ReleaseControl()
        { _Data.ReleaseControl(); }    

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
template <class T>
class SingleListTypeDefn
{
  public:
    typedef SingleListNode<T> NodePtr;
    typedef DataStorePtr<NodePtr> NodeStore;
};

template <class T>
class SingleSublist : public ObjectSkeleton
{
  friend class SingleListNode<T>;
  friend class SingleList<T>;

  protected:
    SingleListNode<T>* _Head;
    SingleListNode<T>* _Tail;
    size_t _NumNodes;
    
    TYPENAME SingleListTypeDefn<T>::NodeStore _Data;

    inline void Xnull() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }        

    // Quick Sort Helper Functions
    void Relink(SingleListNode<T>** List_);
    void _QuickSort(SingleListNode<T>** List_, size_t lo, size_t hi,
                    int Order_, int(*CmpFnc_)(const T&, const T&));
    size_t Partition(SingleListNode<T>** List_, size_t lo, size_t hi,
                     int Order_, int(*CmpFnc_)(const T&, const T&));
    void QuickSort(SingleListNode<T>** List_, size_t lo, size_t hi,
                   int Order_, int(*CmpFnc_)(const T&, const T&));

    void IncreaseSize(size_t Ext_)
        { _NumNodes += Ext_; }
    void DecreaseSize(size_t Ext_)
        { _NumNodes = ((Ext_ <= _NumNodes) ? (_NumNodes - Ext_):0); }

    // List destruction method
    virtual Boolean Destroy();

  public:
    SingleSublist();
    SingleSublist(SingleSublist<T>* Sublist_, int StoreFlags_);
    SingleSublist(const SingleSublist<T>& Sublist_);
    virtual ~SingleSublist(){}

    static SingleSublist<T>* Make();
    static SingleSublist<T>* Make(SingleSublist<T>* Sublist_, int StoreFlags_);
    static SingleSublist<T>* Make(const SingleSublist<T>& Sublist_);

    // object cloning method
    virtual SingleSublist<T>* CloneSublist() const;

    // comparison methods
    int CompareSublist(const SingleSublist<T>& Sublist_, int(*CmpFnc_)(const T&, const T&)) const;

    // assignment methods
    SingleSublist<T>& operator = (const SingleSublist<T>& Sublist_);
    SingleSublist<T>& AssignListNode(SingleListNode<T>* Node_, int StoreFlags_);
    SingleSublist<T>& AddSublist(SingleSublist<T>* Sublist_, int Relation_);

    // item search method
    const SingleListNode<T>* FindSublistItem(const T& Obj_, size_t& Index_, int(*CmpFnc_)(const T&, const T&)) const;
    SingleListNode<T>* FindSublistItem(const T& Obj_, size_t& Index_, int(*CmpFnc_)(const T&, const T&));

    // function application method
    virtual int ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const;
    virtual int ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_);
    virtual int ApplyUnary(int(*Fnc_)(T*));    
    virtual int ApplyUnary(T(*Fnc_)(T));

    // storage attribute method
    virtual int GetStorageAttributes() const;

    // List links methods
    const SingleListNode<T>* Tail() const
        { return _Tail; }
    SingleListNode<T>* Tail()
        { return _Tail; }

    const SingleListNode<T>* Head() const
        { return _Head; }
    SingleListNode<T>* Head()
        { return _Head; }

    const SingleListNode<T>* GetNode(size_t Index_) const;
    SingleListNode<T>* GetNode(size_t Index_);

    size_t NumNodes() const;

    // sorting method
    void SortSublist(int Order_, int(*CmpFnc_)(const T&, const T&));

    // data storage release method
    void ReleaseControl()
        { _Data.ReleaseControl(); }    

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
template <class T>
class SingleList : public Container, public SingleSublist<T>
{
  ROOTOBJECTFNCS_DEFN(Object, SingleList<T>)

  friend class SingleListNode<T>;
  friend class SingleListAcceptor<T>;

  friend SingleList<T> operator + (const SingleList<T>& List1_, const SingleList<T>& List2_)
      { return (SingleList<T>(List1_) += List2_); }
  friend SimpleArray<int> SeriesListCompare(const SingleSublist<T>& Sublist1_,
                                            const SingleSublist<T>& Sublist2_);

  public:
    typedef int(*CompFuncType)(const T&, const T&);

  protected:
    size_t _IteratorsActive;	          // Number of iterators active
    Boolean _DestroyFromIterator;	  // Destroy container from iterator
    size_t _Limit;                        // List limit
    static CompFuncType _CompFunc;        // Comparison function for list items

    // Object factory type definition and data member.
    // Used for polymorphic creation of typed objects given a type code
    typedef T*(*ObjMaker)(int);
    ObjMaker _Factory;

    virtual Boolean IsEqualToSingleList(const SingleSublist<T>* Ptr_) const;
    virtual Boolean IsLesserToSingleList(const SingleSublist<T>* Ptr_) const;
    virtual Boolean IsGreaterToSingleList(const SingleSublist<T>* Ptr_) const;

    void IgnoreElement(istream& Is_, Boolean* Ok_);
    void ReadElement(istream& Is_, SingleListNode<T>* ListPtr_, Boolean* Ok_);
    void WriteElement(ostream& Os_, const SingleListNode<T>* ListPtr_, Boolean* Ok_) const;

    void IgnoreElement(ByteStream& Is_, Boolean* Ok_);
    void ReadElement(ByteStream& Is_, SingleListNode<T>* ListPtr_, Boolean* Ok_);
    void WriteElement(ByteStream& Os_, const SingleListNode<T>* ListPtr_, Boolean* Ok_) const;
  
  public:
    SingleList();
    SingleList(SingleList<T>* List_, int StoreFlags_);
    SingleList(SingleSublist<T>* Sublist_, int StoreFlags_);
    SingleList(const SingleList<T>& List_);
    SingleList(const SingleSublist<T>& Sublist_);
    virtual ~SingleList() {}

    // Virtual constructor and assignment methods
    static SingleList<T>* Make();
    static SingleList<T>* Make(SingleList<T>* List_, int StoreFlags_);
    static SingleList<T>* Make(SingleSublist<T>* Sublist_, int StoreFlags_);
    static SingleList<T>* Make(const SingleList<T>& List_);
    static SingleList<T>* Make(const SingleSublist<T>& Sublist_);

    // List destruction method
    virtual void Flush();

    // list size method
    virtual size_t Size() const;

    // Set comparison function method
    static void SetComparisonFunction(int(*CmpFnc_)(const T&, const T&));

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual SingleList<T>* CloneList() const;

    // Static null object accessor methods
    static SingleList<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth () const;
    virtual long DataSize() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELIST_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL)
	{ return Is_; }
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Setting object maker function
    void SetObjectMaker(ObjMaker Maker_)
	{ _Factory = Maker_; }

    // Make by type code method which invokes
    // the pointer to function data member
    T* MakeByTypeCode(int Code_);

    // comparison methods
    SimpleArray<int> SeriesListCompare(const SingleSublist<T>& Sublist_) const;
    virtual int Compare(const SingleSublist<T>& Sublist_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    
    // item search method
    virtual const SingleListNode<T>* FindItem(const T& Obj_, size_t& Index_) const;
    virtual SingleListNode<T>* FindItem(const T& Obj_, size_t& Index_);

    // sort method
    virtual void Sort(int Order_=SortOrder::INCREASING);

    SingleList<T>& operator = (const SingleList<T>& List_);
    SingleList<T>& operator += (const SingleList<T>& List_);
    SingleList<T>& operator += (const SingleListNode<T>& Node_);
    
    SingleList<T>& AssignListNode(SingleListNode<T>* Node_, int StoreFlags_);
    Boolean AddList(SingleSublist<T>* List_, int Relation_, Boolean DelArgList_=TRUE);
    Boolean AddListNode(SingleListNode<T>* Node_, int Relation_);
    Boolean AddListNode(SingleListNode<T>* Node_, SingleListNode<T>* IndexNode_);
    Boolean AddListNodeAtIndex(SingleListNode<T>* Node_, size_t Index_);

    virtual SingleListNode<T>* DetachFromHead();
    virtual SingleListNode<T>* DetachFromTail();
    virtual SingleListNode<T>* DetachFromMiddle(SingleListNode<T>* IndexNode_);
    virtual SingleListNode<T>* DetachFromMiddle(size_t Index_);

    virtual Boolean IsEmpty() const;
    virtual Boolean IsFull() const;
    
    virtual size_t Limit() const;
    virtual SingleList<T>& SetLimit(size_t Limit_, Boolean Shrink_=TRUE);
    virtual SingleList<T>& IncreaseLimit(size_t Ext_, Boolean Shrink_=TRUE);
    virtual SingleList<T>& DecreaseLimit(size_t Ext_, Boolean Shrink_=TRUE);

    virtual SingleList<T>& Shrink(size_t Num_);
    virtual SingleList<T>& ShrinkTo(size_t Num_);

    virtual Boolean IsNewed() const;

    virtual void DestroyFromIterator();
    virtual Boolean RegisterIterator();
    virtual Boolean UnRegisterIterator();
    virtual size_t IteratorsActive();    

    virtual SingleList<T>& ReverseList();

    virtual T** ListToPtrArray();
    virtual T* ListToArray();
    virtual SingleList<T>& PtrArrayToList(T** Array_, size_t Size_);
    virtual SingleList<T>& ArrayToList(T* Array_, size_t Size_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

#if SINGLELIST_IMPLEMENT_STLCONTAINER
    STL_CONTAINER_DECL(T, SingleListIterator<T>, ReverseIterator<T>)
#endif
};

/****************************************************************************/
#if SINGLELIST_IMPLEMENT_STLITERATOR
template <class T>
class SingleListIterator : public ForwardIterator<T>
{
  friend ForwardIterator<T>& RootObject(SingleListIterator<T>& Obj_)
	{ return *((ForwardIterator<T>*)&Obj_); }
  friend const ForwardIterator<T>& RootConstObject(const SingleListIterator<T>& Obj_)
	{ return *((ForwardIterator<T>*)&Obj_); }

  private:
    union
    {
      const SingleList<T>* _ArrayObjPtr;
      SingleList<T>* _ArrayVarPtr;
    };

    SizeType _Index;

    virtual Boolean AtEnd() const
	{ return (_Index == SizeType(-1)); }
    virtual const void* Address() const
	{ return _ArrayObjPtr; }

    void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    SingleListIterator(const SingleList<T>* Ap_, Boolean DestroyFromIter_=FALSE);
    SingleListIterator(const SingleListIterator<T>& Ap_);
    virtual ~SingleListIterator();

    static SingleListIterator<T>* Make(SingleList<T>* Ap_);
    static SingleListIterator<T>* Make(SingleListIterator<T>& Ap_);

    static const SingleListIterator<T>* MakeConst(const SingleList<T>* Ap_);
    static const SingleListIterator<T>* MakeConst(const SingleListIterator<T>& Ap_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::SINGLELISTITERATOR_TYPE; }
    virtual int ClassID() const;            

    virtual ForwardIterator<T>* Clone();
    virtual const ForwardIterator<T>* Clone() const;

    virtual Boolean IsValid(SizeType Index_) const;
    virtual Boolean IsValid() const;
    virtual Boolean More() const;
    virtual void Reset() const;
    virtual void SetToLast() const;
    virtual void SetToEnd() const;
    virtual SizeType Position() const
	{ return _Index; }

    virtual const T* ConstPointee() const;
    virtual const T* Pointee() const;
    virtual T* Pointee();

    virtual const SingleListNode<T>* ConstNode() const;
    virtual const SingleListNode<T>* Node() const;
    virtual SingleListNode<T>* Node();

    virtual const SingleListIterator<T>& GotoHead() const;
    virtual const SingleListIterator<T>& GotoTail() const;

    virtual SingleListIterator<T>& GotoHead();
    virtual SingleListIterator<T>& GotoTail();

    virtual Boolean AtHead() const;
    virtual Boolean AtTail() const;

    virtual T& operator * ();
    virtual const T& operator * () const;

    virtual ForwardIterator<T>& operator ++ ();
    virtual ForwardIterator<T> operator ++ (int);

    virtual const ForwardIterator<T>& operator ++ () const;
    virtual const ForwardIterator<T> operator ++ (int) const;

    virtual ForwardIterator<T> operator + (SizeType Dist_);
    virtual const ForwardIterator<T> operator + (SizeType Dist_) const;

    virtual ForwardIterator<T>& operator += (SizeType Diff_);
    virtual const ForwardIterator<T>& operator += (SizeType Diff_) const;

    virtual ForwardIterator<T>& GiveIterator()
	{ return *this; }
    virtual const ForwardIterator<T>& GiveIterator() const
	{ return *this; }

    virtual SingleListIterator<T>& SetData(T* Ptr_, int StoreFlags_);
    virtual SingleListIterator<T>& SetData(const T& Obj_);
    virtual SingleListIterator<T>& Erase();

    SingleListIterator<T>& operator = (T* Ptr_);
    SingleListIterator<T>& operator = (const SingleListIterator<T>& Rhs_);
};
#endif

/****************************************************************************/
#endif





