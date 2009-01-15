#ifndef DOUBLELIST_H
#define DOUBLELIST_H
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
#ifndef STACK_CPP
  #include "stack.cpp"
#endif
#ifndef QUEUE_CPP
  #include "queue.cpp"
#endif
#ifndef DEQUE_CPP
  #include "deque.cpp"
#endif
#ifndef SORTEDLIST_CPP
  #include "sortlist.cpp"
#endif
#ifndef SIMPLEARRAY_CPP
  #include "simplearray.cpp"
#endif

// double list debug switches
#define DOUBLELIST_DEBUG	                0
#define DOUBLELIST_STANDARD_DEBUG	        0
#define DOUBLELIST_GENERAL_DEBUG                0
#define DOUBLELIST_NOTIFY_SUCCESS	        1
#define DOUBLELIST_CLONE_TEST                   1
#define DOUBLELIST_SHOW_DEBUG_OUTPUT            1

// double list implementation options
#define DOUBLELIST_IMPLEMENT_STLCONTAINER       1
#define DOUBLELIST_IMPLEMENT_STLITERATOR        1

/****************************************************************************/
template <class T>
class DoubleList;

template <class T>
class DoubleSublist;

template <class T>
class DoubleListIterator;

template <class T>
class DoubleListAcceptor : public ObjectAcceptor
{
  friend class DoubleList<T>;

  protected:
    union
    {
      const DoubleList<T>* _ObjPtr;
      DoubleList<T>* _VarPtr;
    };

    DoubleListAcceptor(const DoubleList<T>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // DoubleList class specific acceptor methods
    virtual Boolean IsDoubleList() const;
    
    virtual void* CreateFromDoubleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDoubleSublist(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* CreateFromStack(const ObjectAcceptor& Obj_);
    virtual void* CreateFromQueue(const ObjectAcceptor& Obj_);
    virtual void* CreateFromDeque(const ObjectAcceptor& Obj_);
    virtual void* CreateFromSortedList(const ObjectAcceptor& Obj_);

    virtual void* AssignFromDoubleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDoubleSublist(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSingleList(const ObjectAcceptor& Obj_);
    virtual void* AssignFromStack(const ObjectAcceptor& Obj_);
    virtual void* AssignFromQueue(const ObjectAcceptor& Obj_);
    virtual void* AssignFromDeque(const ObjectAcceptor& Obj_);
    virtual void* AssignFromSortedList(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToDoubleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToDoubleList(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToDoubleList(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class T>
class DoubleListNode : public ObjectSkeleton
{
  ROOTOBJECTFNCS_DEFN(Object, DoubleListNode<T>)
  
  friend class DoubleList<T>;
  friend class DoubleSublist<T>;

  protected:
    DataStorePtr<T> _Data;
    DoubleListNode<T>* _Next;
    DoubleListNode<T>* _Previous;

    inline void Xnull() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

    DoubleListNode<T>* ReplaceData(DoubleListNode<T>* Node_);
    DoubleListNode<T>* ReplaceNode(DoubleListNode<T>* Node_);

  public:
    DoubleListNode();
    DoubleListNode(const T& Data_);
    DoubleListNode(T* Destination_, int StoreFlags_);
    DoubleListNode(const DoubleListNode<T>& Node_);
    DoubleListNode(const DoubleListNode<T>& Node_, int, int);    
    virtual ~DoubleListNode();

    static size_t NumNodes(const DoubleListNode<T>* Node_);

    static DoubleListNode<T>* Make();
    static DoubleListNode<T>* Make(const T& Data_);
    static DoubleListNode<T>* Make(T* Destination_, int StoreFlags_);
    static DoubleListNode<T>* Make(const DoubleListNode<T>& Node_);
    static DoubleListNode<T>* Make(const DoubleListNode<T>& Node_, int, int);

    // Static null object accessor methods
    static DoubleListNode<T>& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::DOUBLELISTNODE_TYPE; }
    virtual int ClassID() const;

    // object cloning method
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;        
    virtual DoubleListNode<T>* CloneNode() const;

    DoubleListNode<T>& operator = (const DoubleListNode<T>& Node_);

    // list node add/replace/clone methods    
    virtual DoubleListNode<T>* Replace(DoubleSublist<T>* Sublist_);
    virtual DoubleListNode<T>* Replace(DoubleListNode<T>* Node_);
    virtual DoubleListNode<T>* Add(DoubleSublist<T>* Sublist_, int Relation_);
    virtual DoubleListNode<T>* Add(DoubleListNode<T>* Node_, int Relation_);

    // list node item methods
    virtual DoubleListNode<T>* Erase();
    virtual DoubleListNode<T>* SetItem(T* Ptr_, int StoreFlags_);
    virtual DoubleListNode<T>* SetItem(const T& Obj_);
    virtual const T* GetItem() const;
    virtual T* GetItem();

    virtual int CompareItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const;
    virtual const T* FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&)) const;
    virtual T* FindItem(const T& Obj_, int(*CmpFnc_)(const T&, const T&));
    
    virtual int ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const;
    virtual int ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_);
    virtual int ApplyUnary(int(*Fnc_)(T*));
    virtual int ApplyUnary(T(*Fnc_)(T));

    // listnode duplication method
    virtual DoubleListNode<T>* Duplicate(size_t NumNodes_);

    // listnode sublisting methods
    virtual DoubleListNode<T>* DetachNode();
    virtual DoubleSublist<T>* DetachSublist(int Relation_, size_t NumNodes_=0, Boolean ToEnd_=FALSE);
    virtual DoubleSublist<T>* NewSublist(int Relation_, size_t NumNodes_=0, Boolean ToEnd_=FALSE);

    // storage attribute method
    virtual int GetStorageAttributes() const;

    // List links methods
    DoubleListNode<T>* SetNext(DoubleListNode<T>* Next_);
    DoubleListNode<T>* SetPrevious(DoubleListNode<T>* Prev_);

    const DoubleListNode<T>* Next(size_t Nodes_=1) const;
    DoubleListNode<T>* Next(size_t Nodes_=1);
    const DoubleListNode<T>* Last() const;
    DoubleListNode<T>* Last();

    const DoubleListNode<T>* Previous(size_t Nodes_=1) const;
    DoubleListNode<T>* Previous(size_t Nodes_=1);
    const DoubleListNode<T>* First() const;
    DoubleListNode<T>* First();

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
class DoubleListTypeDefn
{
  public:
    typedef DoubleListNode<T> NodePtr;
    typedef DataStorePtr<NodePtr> NodeStore;
};

template <class T>
class DoubleSublist : public ObjectSkeleton
{
  friend class DoubleList<T>;
  friend class DoubleListNode<T>;

  protected:
    DoubleListNode<T>* _Head;
    DoubleListNode<T>* _Tail;
    size_t _NumNodes;
    
    TYPENAME DoubleListTypeDefn<T>::NodeStore _Data;

    inline void Xnull() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }        

    // Quick Sort Helper Functions
    void Relink(DoubleListNode<T>** List_);
    void _QuickSort(DoubleListNode<T>** List_, size_t lo, size_t hi,
                    int Order_, int(*CmpFnc_)(const T&, const T&));
    size_t Partition(DoubleListNode<T>** List_, size_t lo, size_t hi,
                     int Order_, int(*CmpFnc_)(const T&, const T&));
    void QuickSort(DoubleListNode<T>** List_, size_t lo, size_t hi,
                   int Order_, int(*CmpFnc_)(const T&, const T&));

    // List links methods
    void SetHead(DoubleListNode<T>* Node_);
    void SetTail(DoubleListNode<T>* Node_);

    void IncreaseSize(size_t Ext_)
        { _NumNodes += Ext_; }
    void DecreaseSize(size_t Ext_)
        { _NumNodes = ((Ext_ <= _NumNodes) ? (_NumNodes - Ext_):0); }

    // List destruction method
    virtual Boolean Destroy();

  public:
    DoubleSublist();
    DoubleSublist(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_);
    DoubleSublist(DoubleSublist<T>* Sublist_, int StoreFlags_);
    DoubleSublist(const DoubleSublist<T>& Sublist_);
    virtual ~DoubleSublist(){}

    static DoubleSublist<T>* Make();
    static DoubleSublist<T>* Make(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_);
    static DoubleSublist<T>* Make(DoubleSublist<T>* Sublist_, int StoreFlags_);
    static DoubleSublist<T>* Make(const DoubleSublist<T>& Sublist_);

    // object cloning method
    virtual DoubleSublist<T>* CloneSublist() const;

    // comparison methods
    int CompareSublist(const DoubleSublist<T>& Sublist_, int(*CmpFnc_)(const T&, const T&)) const;

    DoubleSublist<T>& operator = (const DoubleSublist<T>& Sublist_);
    DoubleSublist<T>& AssignSublist(DoubleSublist<T>* Sublist_, int StoreFlags_);
    DoubleSublist<T>& AssignListNode(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_);
    
    DoubleSublist<T>& AddSublist(DoubleSublist<T>* Sublist_, int Relation_);

    // item search method
    const DoubleListNode<T>* FindSublistItem(const T& Obj_, size_t& Index_, int(*CmpFnc_)(const T&, const T&), size_t StartIndex_=0, int Toward_=ListRelation::TAIL) const;
    DoubleListNode<T>* FindSublistItem(const T& Obj_, size_t& Index_, int(*CmpFnc_)(const T&, const T&), size_t StartIndex_=0, int Toward_=ListRelation::TAIL);

    // function application method
    virtual int ApplyBinary(int(*Fnc_)(const T&, const T&), const T& Obj_) const;
    virtual int ApplyBinary(int(*Fnc_)(T&, T&), T& Obj_);
    virtual int ApplyUnary(int(*Fnc_)(T*));
    virtual int ApplyUnary(T(*Fnc_)(T));

    // storage attribute method
    virtual int GetStorageAttributes() const;

    // List links methods
    const DoubleListNode<T>* Tail() const
        { return _Tail; }
    DoubleListNode<T>* Tail()
        { return _Tail; }

    const DoubleListNode<T>* Head() const
        { return _Head; }
    DoubleListNode<T>* Head()
        { return _Head; }

    const DoubleListNode<T>* GetNode(size_t Index_) const;
    DoubleListNode<T>* GetNode(size_t Index_);

    size_t NumNodes() const;

    void SortSublist(int Order_, int(*CmpFnc_)(const T&, const T&));

    // data storage method
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
class DoubleList : public Container, public DoubleSublist<T>
{
  ROOTOBJECTFNCS_DEFN(Object, DoubleList<T>)

  friend class DoubleListNode<T>;
  friend class DoubleListAcceptor<T>;

  friend DoubleList<T> operator + (const DoubleList<T>& List1_, const DoubleList<T>& List2_)
      { return (DoubleList<T>(List1_) += List2_); }
  friend SimpleArray<int> SeriesListCompare(const DoubleSublist<T>& Sublist1_,
                                        const DoubleSublist<T>& Sublist2_);

  public:
    typedef int(*CompFuncType)(const T&, const T&);

  protected:
    size_t _IteratorsActive;	          // Number of iterators active
    Boolean _DestroyFromIterator;	  // Destroy container from iterator
    size_t _Limit;                        // List limit
    static CompFuncType _CompFunc;  // Comparison function for list items

    // Object factory type definition and data member.
    // Used for polymorphic creation of typed objects given a type code
    typedef T*(*ObjMaker)(int);
    ObjMaker _Factory;

    virtual Boolean IsEqualToDoubleList(const DoubleSublist<T>* Ptr_) const;
    virtual Boolean IsLesserToDoubleList(const DoubleSublist<T>* Ptr_) const;
    virtual Boolean IsGreaterToDoubleList(const DoubleSublist<T>* Ptr_) const;

    void IgnoreElement(istream& Is_, Boolean* Ok_);
    void ReadElement(istream& Is_, DoubleListNode<T>* ListPtr_, Boolean* Ok_);
    void WriteElement(ostream& Os_, const DoubleListNode<T>* ListPtr_, Boolean* Ok_) const;

    void IgnoreElement(ByteStream& Is_, Boolean* Ok_);
    void ReadElement(ByteStream& Is_, DoubleListNode<T>* ListPtr_, Boolean* Ok_);
    void WriteElement(ByteStream& Os_, const DoubleListNode<T>* ListPtr_, Boolean* Ok_) const;
  
  public:
    DoubleList();
    DoubleList(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_);
    DoubleList(DoubleList<T>* List_, int StoreFlags_);
    DoubleList(DoubleSublist<T>* Sublist_, int StoreFlags_);
    DoubleList(const DoubleList<T>& List_);
    DoubleList(const DoubleSublist<T>& Sublist_);
    virtual ~DoubleList() {}

    // Virtual constructor and assignment methods
    static DoubleList<T>* Make();
    static DoubleList<T>* Make(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_);
    static DoubleList<T>* Make(DoubleList<T>* List_, int StoreFlags_);
    static DoubleList<T>* Make(DoubleSublist<T>* Sublist_, int StoreFlags_);
    static DoubleList<T>* Make(const DoubleList<T>& List_);
    static DoubleList<T>* Make(const DoubleSublist<T>& Sublist_);

    // List destruction method
    virtual void Flush();

    // list size method
    virtual size_t Size() const;

    // Set comparison function method
    static void SetComparisonFunction(int(*CmpFnc_)(const T&, const T&));

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual DoubleList<T>* CloneList() const;

    // Static null object accessor methods
    static DoubleList<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth () const;
    virtual long DataSize() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::DOUBLELIST_TYPE; }
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
    SimpleArray<int> SeriesListCompare(const DoubleSublist<T>& Sublist_) const;
    virtual int Compare(const DoubleSublist<T>& Sublist_) const;
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    
    // item search method
    virtual const DoubleListNode<T>* FindItem(const T& Obj_, size_t& Index_, size_t StartIndex_=0, int Toward_=ListRelation::TAIL) const;
    virtual DoubleListNode<T>* FindItem(const T& Obj_, size_t& Index_, size_t StartIndex_=0, int Toward_=ListRelation::TAIL);

    // sort method
    virtual void Sort(int Order_=SortOrder::INCREASING);

    DoubleList<T>& operator = (const DoubleList<T>& List_);
    DoubleList<T>& AssignList(DoubleList<T>* List_, int StoreFlags_);
    DoubleList<T>& AssignListNode(DoubleListNode<T>* Node_, size_t NumNodes_, int StoreFlags_);

    DoubleList<T>& operator += (const DoubleSublist<T>& List_);
    Boolean AddList(DoubleSublist<T>* List_, int Relation_, Boolean DelArgList_=TRUE);
    Boolean AddList(DoubleSublist<T>* List_, DoubleListNode<T>* IndexNode_, int Relation_, Boolean DelArgList_=TRUE);
    Boolean AddListAtIndex(DoubleSublist<T>* List_, size_t Index_, int Relation_, Boolean DelArgList_=TRUE);
    Boolean AddListNode(DoubleListNode<T>* Node_, int Relation_);
    Boolean AddListNode(DoubleListNode<T>* Node_, DoubleListNode<T>* IndexNode_, int Relation_);
    Boolean AddListNodeAtIndex(DoubleListNode<T>* Node_, size_t Index_, int Relation_);

    Boolean ReplaceList(DoubleSublist<T>* Sublist_, int Relation_, Boolean DelArgList_=TRUE);
    Boolean ReplaceList(DoubleSublist<T>* Sublist_, DoubleListNode<T>* IndexNode_, Boolean DelArgList_=TRUE);
    Boolean ReplaceListAtIndex(DoubleSublist<T>* Sublist_, size_t Index_, Boolean DelArgList_=TRUE);
    Boolean ReplaceListNode(DoubleListNode<T>* Node_, int Relation_);
    Boolean ReplaceListNode(DoubleListNode<T>* Node_, DoubleListNode<T>* IndexNode_);
    Boolean ReplaceListNodeAtIndex(DoubleListNode<T>* Node_, size_t Index_);

    virtual DoubleList<T>* DetachFromHead(size_t NumNodes_);
    virtual DoubleList<T>* DetachFromTail(size_t NumNodes_);
    virtual DoubleList<T>* DetachFromMiddle(DoubleListNode<T>* IndexNode_);
    virtual DoubleList<T>* DetachFromMiddle(size_t Index_, size_t NumNodes_);
    virtual DoubleList<T>* DetachFromMiddleToEnd(size_t Index_);
    virtual DoubleList<T>* DetachFromMiddleToStart(size_t Index_);

    // listnode duplication method
    virtual DoubleList<T>& Duplicate(size_t Index_, size_t NumNodes_);

    Boolean IsCircular() const;
    Boolean IsLinear() const;
    
    virtual Boolean IsEmpty() const;
    virtual Boolean IsFull() const;
    
    virtual size_t Limit() const;
    virtual DoubleList<T>& SetLimit(size_t Limit_, Boolean Shrink_=TRUE);
    virtual DoubleList<T>& IncreaseLimit(size_t Ext_, Boolean Shrink_=TRUE);
    virtual DoubleList<T>& DecreaseLimit(size_t Ext_, Boolean Shrink_=TRUE);

    virtual DoubleList<T>& Shrink(size_t Num_, int Relation_=ListRelation::TAIL);
    virtual DoubleList<T>& ShrinkTo(size_t Num_, int Relation_=ListRelation::TAIL);

    virtual DoubleList<T>& MakeCircular();
    virtual DoubleList<T>& MakeLinear();    

    virtual Boolean IsNewed() const;

    virtual void DestroyFromIterator();
    virtual Boolean RegisterIterator();
    virtual Boolean UnRegisterIterator();
    virtual size_t IteratorsActive();    

    virtual DoubleList<T>& ReverseList();

    virtual T** ListToPtrArray();
    virtual T* ListToArray();
    virtual DoubleList<T>& PtrArrayToList(T** Array_, size_t Size_);
    virtual DoubleList<T>& ArrayToList(T* Array_, size_t Size_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

#if DOUBLELIST_IMPLEMENT_STLCONTAINER
    STL_CONTAINER_DECL(T, DoubleListIterator<T>, ReverseRandomAccessIterator<T>)
#endif
};

/****************************************************************************/
#if DOUBLELIST_IMPLEMENT_STLITERATOR
template <class T>
class DoubleListIterator : public RandomAccessIterator<T>
{
  friend RandomAccessIterator<T>& RootObject(DoubleListIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }
  friend const RandomAccessIterator<T>& RootConstObject(const DoubleListIterator<T>& Obj_)
	{ return *((RandomAccessIterator<T>*)&Obj_); }

  private:
    union
    {
      const DoubleList<T>* _ArrayObjPtr;
      DoubleList<T>* _ArrayVarPtr;
    };

    SizeType _Index;

    virtual Boolean AtEnd() const
	{ return (_Index == SizeType(-1)); }
    virtual const void* Address() const
	{ return _ArrayObjPtr; }

    void Xnullp() const
	{ THROW (FallibleBase::DerefNullPointerErr()); }

  public:
    DoubleListIterator(const DoubleList<T>* Ap_, Boolean DestroyFromIter_=FALSE);
    DoubleListIterator(const DoubleListIterator<T>& Ap_);
    virtual ~DoubleListIterator();

    static DoubleListIterator<T>* Make(DoubleList<T>* Ap_);
    static DoubleListIterator<T>* Make(DoubleListIterator<T>& Ap_);

    static const DoubleListIterator<T>* MakeConst(const DoubleList<T>* Ap_);
    static const DoubleListIterator<T>* MakeConst(const DoubleListIterator<T>& Ap_);

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::DOUBLELISTITERATOR_TYPE; }
    virtual int ClassID() const;            

    virtual RandomAccessIterator<T>* Clone();
    virtual const RandomAccessIterator<T>* Clone() const;

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

    virtual const DoubleListNode<T>* ConstNode() const;
    virtual const DoubleListNode<T>* Node() const;
    virtual DoubleListNode<T>* Node();

    virtual const DoubleListIterator<T>& GotoHead() const;
    virtual const DoubleListIterator<T>& GotoTail() const;

    virtual DoubleListIterator<T>& GotoHead();
    virtual DoubleListIterator<T>& GotoTail();

    virtual Boolean AtHead() const;
    virtual Boolean AtTail() const;

    virtual T& operator * ();
    virtual const T& operator * () const;
    virtual RandomAccessIterator<T>& operator [] (SizeType Index_);
    virtual const RandomAccessIterator<T>& operator [] (SizeType Index_) const;

    virtual RandomAccessIterator<T>& operator ++ ();
    virtual RandomAccessIterator<T> operator ++ (int);

    virtual const RandomAccessIterator<T>& operator ++ () const;
    virtual const RandomAccessIterator<T> operator ++ (int) const;

    virtual RandomAccessIterator<T>& operator -- ();
    virtual RandomAccessIterator<T> operator -- (int);

    virtual const RandomAccessIterator<T>& operator -- () const;
    virtual const RandomAccessIterator<T> operator -- (int) const;

    virtual RandomAccessIterator<T> operator + (SizeType Dist_);
    virtual const RandomAccessIterator<T> operator + (SizeType Dist_) const;
    virtual RandomAccessIterator<T> operator - (SizeType Dist_);
    virtual const RandomAccessIterator<T> operator - (SizeType Dist_) const;

    virtual RandomAccessIterator<T>& operator += (SizeType Diff_);
    virtual const RandomAccessIterator<T>& operator += (SizeType Diff_) const;
    virtual RandomAccessIterator<T>& operator -= (SizeType Diff_);
    virtual const RandomAccessIterator<T>& operator -= (SizeType Diff_) const;

    virtual RandomAccessIterator<T>& GiveIterator()
	{ return *this; }
    virtual const RandomAccessIterator<T>& GiveIterator() const
	{ return *this; }

    virtual DoubleListIterator<T>& SetData(T* Ptr_, int StoreFlags_);
    virtual DoubleListIterator<T>& SetData(const T& Obj_);
    virtual DoubleListIterator<T>& Erase();

    DoubleListIterator<T>& operator = (T* Ptr_);
    DoubleListIterator<T>& operator = (const DoubleListIterator<T>& Rhs_);
};
#endif

/****************************************************************************/
#endif





