#ifndef MMLIST_H
#define MMLIST_H
#ifndef COMPARE_CPP
  #include "compare.cpp"
#endif

#define MMLIST_DEBUG	0

/****************************************************************************/
template <class T>
struct MMNode
{
  T* _Object;
  MMNode* _Next;

  MMNode(T* Obj_, MMNode<T>* Next_ = NULL);
  virtual ~MMNode();

  void Insert(MMNode<T>* Node_);
};

/****************************************************************************/
template <class T>
class MMList
{
  private:
    MMList(const MMList<T>& List_) {}
    MMList<T>& operator = (const MMList<T>& SimpList_)
	{ return *this; }

  protected:
    MMNode<T>* _List;

    MMList(MMNode<T>* NodePtr_); 

    MMNode<T>* FindPrevious(const MMNode<T>* Ptr_) const;
    T* PopNode();

  public:
    MMList();
    virtual ~MMList();

    static T* Deref(MMNode<T>* Ptr_)
	{ return (Ptr_ ? Ptr_->_Object:((T*)NULL)); }

    MMNode<T>* Head() const
	{ return _List; }

    // Find nearest node with matching object
    MMNode<T>* Find(const T* Obj_, PtrCompare<T>& Compare_) const;
    MMNode<T>* Find(const T* Obj_) const;

    // Item/Node Removal and Insertion methods
    void Remove(MMNode<T>* Ptr_);
    void InsertAfter(MMNode<T>* Ptr_, T* NewPtr_);
    void InsertBefore(MMNode<T>* Ptr_, T* NewPtr_);

    // Calls the Push method to add the node at the head of the list.
    MMList<T>& AppendHead(T* Obj_);

    // Calls the AddNode method to add the node at the tail of the list.
    MMList<T>& AppendTail(T* Obj_);

    // Delete all nodes
    void DeleteAll();

    // Returns TRUE if list is empty
    BOOLTYPE Empty() const;
};

template <class T>
class MMListBrowser : public MMList<T>
{
  public:
    MMListBrowser(MMNode<T>* Ptr_);
    MMListBrowser(const MMList<T>& Alist_);
    virtual ~MMListBrowser();

    MMListBrowser<T>& operator = (MMNode<T>* Ptr_);
    MMListBrowser<T>& operator = (const MMList<T>& Alist_);

    // Find next adjacent node with the same matching object
    MMNode<T>* NextMatch(const T* Obj_, PtrCompare<T>& Compare_);
    MMNode<T>* NextMatch(const T* Obj_);

    MMNode<T>* IndexNode(size_t Index_);

    MMNode<T>* operator ++ ();
    MMNode<T>* operator ++ (int);

    MMNode<T>* CurrentNode()
	{ return _List; }
};

/****************************************************************************/
#endif




