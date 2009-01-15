#ifndef SIMPLELIST_H
#define SIMPLELIST_H
#ifndef BUILTIN_H
  #include "builtin.h"
#endif
#ifndef OBJECTCLONER_H
  #include "objectcloner.h"
#endif

#define SIMPLELIST_DEBUG	0

/****************************************************************************/
template <class T>
class SimpleList;

template <class T>
class SearchableList;

template <class T>
struct SimpleNode;

template <class T>
class ConstListIterator
{
  protected:
      const SimpleList<T>* _ConstList;
      const SimpleNode<T>* _ConstHere;
      size_t _Index;      

  public:
    ConstListIterator(const SimpleList<T>* List_);

    Boolean More() const;
    Boolean Less() const;
    
    const SimpleNode<T>* Next();
    const SimpleNode<T>* Prev();

    const SimpleNode<T>* operator ++ (int);
    const SimpleNode<T>* operator ++ ()
	{ return Next(); }

    const SimpleNode<T>* operator -- (int);
    const SimpleNode<T>* operator -- ()
	{ return Prev(); }

    const SimpleList<T>* List() const
	{ return _List; }
    SimpleList<T>* List()
	{ return _List; }        
    const SimpleNode<T>* Here() const
	{ return _Here; }
    SimpleNode<T>* Here()
	{ return _Here; }        

    ConstListIterator<T>& Head();
    ConstListIterator<T>& Tail();

    const SimpleNode<T>* IndexNode(size_t Index_);

    operator const SimpleNode<T>* () const
	{ return Here(); }
    operator SimpleNode<T>* ()
	{ return Here(); }

    size_t Position() const;
    size_t Count() const;                

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

template <class T>
class SimpleListIterator
{
  protected:
      SimpleList<T>* _List;
      SimpleNode<T>* _Here;
      size_t _Index;

  public:
    SimpleListIterator(SimpleList<T>* List_);

    Boolean More() const;
    Boolean Less() const;
    
    SimpleNode<T>* Next();
    SimpleNode<T>* Prev();

    SimpleNode<T>* operator ++ (int);
    SimpleNode<T>* operator ++ ()
	{ return Next(); }

    SimpleNode<T>* operator -- (int);
    SimpleNode<T>* operator -- ()
	{ return Prev(); }

    const SimpleList<T>* List() const
	{ return _List; }
    SimpleList<T>* List()
	{ return _List; }        
    const SimpleNode<T>* Here() const
	{ return _Here; }
    SimpleNode<T>* Here()
	{ return _Here; }

    SimpleListIterator<T>& Head();
    SimpleListIterator<T>& Tail();

    SimpleNode<T>* IndexNode(size_t Index_);
    SimpleNode<T>* Update(size_t Index_, T* Data_);

    void Remove();
    void InsertAfter(T* NewPtr_)
	{ _List->InsertAfter(_Here, NewPtr_); }
    void InsertBefore(T* NewPtr_)
	{ _List->InsertBefore(_Here, NewPtr_); }

    operator const SimpleNode<T>* () const
	{ return Here(); }
    operator SimpleNode<T>* ()
	{ return Here(); }

    size_t Position() const;
    size_t Count() const;        

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
struct SimpleNode
{
  public:
    T* _Object;
    SimpleNode* _Next;

    SimpleNode(T* Obj_, SimpleNode<T>* Next_ = NULL);
    virtual ~SimpleNode();

    void Insert(SimpleNode<T>* Node_);

    T* Value()
       { return _Object; };
    const T* Value() const
       { return _Object; };

    T* Next()
       { return _Next; };
    const T* Next() const
       { return _Next; };

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
class SimpleList
{
  protected:
    ObjectCloner<T>* _Cloner;
    size_t _Count;
    union
    {
      SimpleNode<T>* _List;
      const SimpleNode<T>* _ConstList;
    };
    
    SimpleList(const SimpleNode<T>* NodePtr_, ObjectCloner<T>* ClonerPtr_=NULL);

    SimpleNode<T>* FindPrevious(SimpleNode<T>* Ptr_);
    T* PopNode();

  public:
    SimpleList(ObjectCloner<T>* ClonerPtr_=NULL);
    SimpleList(const SimpleList<T>& List_);
    virtual ~SimpleList();    

    static T* Deref(SimpleNode<T>* Ptr_)
	{ return (Ptr_ ? Ptr_->_Object:((T*)NULL)); }    

    SimpleList<T>& operator = (const SimpleList<T>& SimpList_);
    SimpleList<T>& operator += (const SimpleList<T>& SimpList_);

    SimpleNode<T>* Tail();
    const SimpleNode<T>* Tail() const;
    SimpleNode<T>* Head()
	{ return _List; }
    const SimpleNode<T>* Head() const
	{ return _List; }

    // List item duplication behaviour
    SimpleList<T>& SetCloner(ObjectCloner<T>* Cloner_);

    // Item/Node Removal and Insertion methods
    void Remove(SimpleNode<T>* Ptr_);
    void InsertAfter(SimpleNode<T>* Ptr_, T* NewPtr_);
    void InsertBefore(SimpleNode<T>* Ptr_, T* NewPtr_);

    // Calls the Push method to add the node at the head of the list.
    SimpleList<T>& AppendHead(T* Obj_);

    // Calls the AddNode method to add the node at the tail of the list.
    SimpleList<T>& AppendTail(T* Obj_);

    // Delete all nodes
    void DeleteAll();

    // Returns TRUE if list is empty
    Boolean Empty(void) const;

    // Return size of list in number of elements
    size_t Size(void) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

template <class T>
class SimpleListBrowser;

template <class T>
class SearchableList : public SimpleList<T>
{
  friend class SimpleListBrowser<T>;

  protected:
    SearchableList(const SimpleNode<T>* NodePtr_);

  public:
    SearchableList();
    SearchableList(const SearchableList<T>& List_);

    SearchableList<T>& operator = (const SimpleList<T>& SimpList_);
    SearchableList<T>& operator += (const SimpleList<T>& SimpList_);
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      SearchableList<T>& operator = (const SearchableList<T>& SimpList_);
      SearchableList<T>& operator += (const SearchableList<T>& SimpList_);
    #endif

    // Find next adjacent node with the same matching object
    SimpleNode<T>* NextMatch(SimpleNode<T>* Ptr_, PtrCompare<T>& Compare_);
    SimpleNode<T>* NextMatch(SimpleNode<T>* Ptr_);

    // Find next adjacent node with the same matching object
    const SimpleNode<T>* NextMatch(const SimpleNode<T>* Ptr_, PtrCompare<T>& Compare_) const;
    const SimpleNode<T>* NextMatch(const SimpleNode<T>* Ptr_) const;

    // Find nearest node with matching object
    SimpleNode<T>* Find(T* Obj_, PtrCompare<T>& Compare_);
    SimpleNode<T>* Find(T* Obj_);

    // Find nearest node with matching object
    const SimpleNode<T>* Find(const T* Obj_, PtrCompare<T>& Compare_) const;
    const SimpleNode<T>* Find(const T* Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif    
};

template <class T>
class SimpleListBrowser : private SearchableList<T>
{
  public:
    SimpleListBrowser(const SimpleNode<T>* Ptr_);
    SimpleListBrowser(const SearchableList<T>& Alist_);
    virtual ~SimpleListBrowser();

    SimpleListBrowser<T>& operator = (const SimpleNode<T>* Ptr_);
    SimpleListBrowser<T>& operator = (const SearchableList<T>& Alist_);

    // Find next adjacent node with the same matching object
    const SimpleNode<T>* NextMatch(const T* Obj_, PtrCompare<T>& Compare_) const;
    const SimpleNode<T>* NextMatch(const T* Obj_) const;

    const SimpleNode<T>* IndexNode(size_t Index_) const;

    Boolean More() const;
    const SimpleNode<T>* operator ++ () const;
    const SimpleNode<T>* operator ++ (int) const;

    const SimpleNode<T>* Here() const
	{ return _List; }

    operator const SimpleNode<T>* () const
	{ return Here(); }

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


