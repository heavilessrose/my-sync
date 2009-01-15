//-------------------------------------------------------------------------
//
//    BST.CPP  -- Implementation of a Binary Search Tree Class
//
//    Written July 96   - N. Dadoun, (but modified by J. Wong)
//
//    Modification History:
//      templatized
//      NULLOBJ object used as dummy return value for invalid data
//	non-recursive tree search method
//      preorder, inorder, postorder tree traversals
//	get largest item method
//	get smallest item method
//	get root node pointer method
//
//-------------------------------------------------------------------------
#ifndef BST_CPP
#define BST_CPP
#ifndef BST_H
  #include "bst.h"
#endif

/****************************************************************************/
template <class KEY, class VALUE>
int(*Bst<KEY, VALUE>::_CompFunc)(const KEY&, const KEY&) = NULL;

//-------------------------------------------------------------------------
//
//  BstNode        - constructor initializes value datastore
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>::BstNode():
left(NULL),
right(NULL)
{}

//-------------------------------------------------------------------------
//
//  Bst        - constructor initializes root to NULL.
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Bst<KEY, VALUE>::Bst():
_root(NULL),
_Size(0),
_Found(FALSE)
{
  Assert(_CompFunc != NULL, "Comparison function not set for bst structure");
}

//-------------------------------------------------------------------------
//
//  ~Bst        - destructor uses freeTree to deallocate entire tree structure
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Bst<KEY, VALUE>::~Bst()
{
  FreeTree(_root);
  _root = NULL;
}

//-------------------------------------------------------------------------
//
//  Bst        - copy constructor to copy entire tree structure using recursive
//               copyNode routine.
//
//  tree       - pointer to _root of tree to copy from.
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Bst<KEY, VALUE>::Bst(const Bst<KEY, VALUE>& tree):
_root(NULL),
_Size(0),
_Found(FALSE)
{
   Assert(_CompFunc != NULL, "Comparison function not set for bst structure");
   CopyNode(tree._root);
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
size_t Bst<KEY, VALUE>::Pow2(size_t Exp_) const
{
  size_t x;
  size_t Val_ = 1;
  
  for (x = 0; x < Exp_; x++)
    Val_ *= 2;

  return Val_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
long Bst<KEY, VALUE>::MaxNodes() const
{
  size_t x;
  size_t Ht_ = CalcHeight(_root);
  size_t NumNodes_ = 0;

  for (x = 0; x < Ht_; x++)
    NumNodes_ += Pow2(x);

  return NumNodes_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Object& Bst<KEY, VALUE>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  BstAcceptor<KEY, VALUE> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromBst(TrgComp_)));
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Object* Bst<KEY, VALUE>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new Bst<KEY, VALUE>(*this));

  BstAcceptor<KEY, VALUE> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromBst(TrgComp_)));
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Bst<KEY, VALUE>* Bst<KEY, VALUE>::CloneBst() const
{
  return (new Bst<KEY, VALUE>(*this));
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Bst<KEY, VALUE>& Bst<KEY, VALUE>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Bst<KEY, VALUE> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
const Object& Bst<KEY, VALUE>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Bst<KEY, VALUE>::GiveNullObject();
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Object& Bst<KEY, VALUE>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Bst<KEY, VALUE>::GiveNullObject();
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
long Bst<KEY, VALUE>::StorableFormWidth () const
{
  return (long(sizeof(long) +
               sizeof(long) +
	       sizeof(Byte_t) +               
	       sizeof(long)) +
	  DataSize());
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
long Bst<KEY, VALUE>::DataSize() const
{
  return (MaxNodes() * (sizeof(Byte_t) + sizeof(KEY) + sizeof(VALUE)));
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
int Bst<KEY, VALUE>::ClassID() const
{
  return TypeInfo::BST_TYPE;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
const ObjectAcceptor* Bst<KEY, VALUE>::GiveObjectAcceptor() const
{
  return (new BstAcceptor<KEY, VALUE>(this));
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>** Bst<KEY, VALUE>::TreeToNodeArray() const
{
  size_t Index_ = 1;
  size_t Max_ = MaxNodes();
  
  BstNode<KEY, VALUE>** NodeArray_ =
  (BstNode<KEY, VALUE>**)MemMatrix::Matrix().Allocate((Max_ + 1) * sizeof(BstNode<KEY, VALUE>*));
  
  BstNode<KEY, VALUE>* Node_ = _root;
  NodeArray_[Index_] = Node_;

  while (Index_ <= Max_)
  {
    if (Index_ * 2 <= Max_)
      NodeArray_[Index_ * 2] = !Node_ ? NULL:Node_->left;
      
    if (Index_ * 2 + 1 <= Max_)
      NodeArray_[Index_ * 2 + 1] = !Node_ ? NULL:Node_->right;
      
    ++Index_;
    Node_ = NodeArray_[Index_];    
  }

  return NodeArray_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::NodeArrayToTree(BstNode<KEY, VALUE>** Array_, long Len_)
{
  long Index_ = 1;
  BstNode<KEY, VALUE>* Node_ = Array_[Index_];

  FreeTree(_root);  
  _root = Node_;

  while (Index_ <= Len_)
  {
    if (Index_ * 2 <= Len_)
      Node_->left = Array_[Index_ * 2];
      
    if (Index_ * 2 + 1 <= Len_)
      Node_->right = Array_[Index_ * 2 + 1];
      
    ++Index_;
    Node_ = Array_[Index_];
  }
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
istream& Bst<KEY, VALUE>::BinaryIgnore(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Len_;
  
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
    if (Is_.read((char*)(&Len_), sizeof(long)).good())
    {
      Len_ *= (sizeof(Byte_t) + sizeof(KEY) + sizeof(VALUE));
      if (Is_.seekg(Len_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);    
    }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
istream& Bst<KEY, VALUE>::BinaryRead(istream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  KEY DumKey_;
  VALUE DumVal_;
  
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {  
    Byte_t Confirm_;
    long Index_;
    long Len_;
  
    if (Is_.read((char*)(&Len_), sizeof(long)).good())
    {
      BstNode<KEY, VALUE>** NodeArray_ =
      (BstNode<KEY, VALUE>**)MemMatrix::Matrix().Allocate((Len_ + 1) * sizeof(BstNode<KEY, VALUE>*));
    
      for (Index_ = 1; Index_ <= Len_; Index_++)
        if (Is_.read((char*)(&Confirm_), sizeof(Byte_t)).good())
        {
          Read(Is_, DumKey_, Ok_);
        
          if (*Ok_)
          {
            Read(Is_, DumVal_, Ok_);
          
            if (*Ok_)
              if (Confirm_)
                NodeArray_[Index_] = MakeNode(DumKey_, DumVal_);
              else
                NodeArray_[Index_] = NULL;
          }
        }

      NodeArrayToTree(NodeArray_, Len_);
      ::Delete(NodeArray_);
    }
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
ostream& Bst<KEY, VALUE>::BinaryWrite(ostream& Os_, Boolean* Ok_) const
{
  BstNode<KEY, VALUE>** NodeArray_ = TreeToNodeArray();
  KEY DumKey_;
  VALUE DumVal_;

  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Byte_t Confirm_;
    long Index_;
    
    long Len_ = (long)MaxNodes();
    Os_.write((char*)(&Len_), sizeof(long));

    if (OstreamCheck(Os_, Ok_) && Len_)
      for (Index_ = 1; Index_ <= Len_; Index_++)
      {
        Confirm_ = NodeArray_[Index_] != NULL;
        Os_.write((char*)(&Confirm_), sizeof(Byte_t));
      
        if (Confirm_)
        {
          Write(Os_, NodeArray_[Index_]->key, Ok_);
          if (*Ok_)
            Write(Os_, *NodeArray_[Index_]->value, Ok_);
        }
        else
        {
          Write(Os_, DumKey_, Ok_);
          if (*Ok_)
            Write(Os_, DumVal_, Ok_);        
        }
      }
  }

  ::Delete(NodeArray_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
ByteStream& Bst<KEY, VALUE>::BinaryIgnore(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  long Len_;
  
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
    if (Is_.iread((char*)(&Len_), sizeof(long)).good())
    {
      Len_ *= (sizeof(Byte_t) + sizeof(KEY) + sizeof(VALUE));
      if (Is_.iseekg(Len_, ios::cur).good())
	SetIOstate(TRUE, IO_CLEAR, TRUE);    
    }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
ByteStream& Bst<KEY, VALUE>::BinaryRead(ByteStream& Is_, Boolean* Ok_)
{
  Boolean RdChk_;
  KEY DumKey_;
  VALUE DumVal_;
  
  if (!Ok_)
    Ok_ = &RdChk_;

  *Ok_ = ReadObjectData(*this, Is_, FALSE);

  if (*Ok_)
  {
    Byte_t Confirm_;
    long Index_;
    long Len_;
  
    if (Is_.iread((char*)(&Len_), sizeof(long)).good())
    {
      BstNode<KEY, VALUE>** NodeArray_ =
      (BstNode<KEY, VALUE>**)MemMatrix::Matrix().Allocate((Len_ + 1) * sizeof(BstNode<KEY, VALUE>*));
    
      for (Index_ = 1; Index_ <= Len_; Index_++)
        if (Is_.iread((char*)(&Confirm_), sizeof(Byte_t)).good())
        {      
          Read(Is_, DumKey_, Ok_);
          
          if (*Ok_)
          {
            Read(Is_, DumVal_, Ok_);
            
            if (*Ok_)
              if (Confirm_)
                NodeArray_[Index_] = MakeNode(DumKey_, DumVal_);
              else
                NodeArray_[Index_] = NULL;
          }
        }

      NodeArrayToTree(NodeArray_, Len_);
      ::Delete(NodeArray_);
    }
  }

  SetIOstate(FALSE, IO_STREAMERROR);
  ReturnReadValid(Is_, Ok_);
  return Is_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
ByteStream& Bst<KEY, VALUE>::BinaryWrite(ByteStream& Os_, Boolean* Ok_) const
{
  BstNode<KEY, VALUE>** NodeArray_ = TreeToNodeArray();
  KEY DumKey_;
  VALUE DumVal_;

  Boolean WrChk_;
  if (!Ok_)
    Ok_ = &WrChk_;

  *Ok_ = WriteObjectData(*this, Os_);

  if (*Ok_)
  {
    Byte_t Confirm_;
    long Index_;
    
    long Len_ = (long)MaxNodes();
    Os_.owrite((char*)(&Len_), sizeof(long));

    if (OstreamCheck(((ostream&)Os_), Ok_) && Len_)
      for (Index_ = 1; Index_ <= Len_; Index_++)
      {
        Confirm_ = NodeArray_[Index_] != NULL;
        Os_.owrite((char*)(&Confirm_), sizeof(Byte_t));
      
        if (Confirm_)
        {
          Write(Os_, NodeArray_[Index_]->key, Ok_);
          if (*Ok_)
            Write(Os_, *NodeArray_[Index_]->value, Ok_);
        }
        else
        {
          Write(Os_, DumKey_, Ok_);
          if (*Ok_)
            Write(Os_, DumVal_, Ok_);
        }
      }
  }

  ::Delete(NodeArray_);
  ReturnWriteValid(Os_, Ok_);
  return Os_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
ostream& Bst<KEY, VALUE>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  DumpNode(_root, Os_);
  return Os_;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::Clear()
{
   FreeTree(_root);
   _root = NULL;
}

//-------------------------------------------------------------------------
//
//  operator = - assignment operator to copy entire tree structure using recursive
//               copyNode routine.
//
//  tree       - pointer to _root of tree to copy from.
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
Bst<KEY, VALUE>& Bst<KEY, VALUE>::operator = (const Bst<KEY, VALUE>& tree)
{
   if (this != &tree)               // Don't need copy to yourself.
   {
      if (_root)                    // if this Bst already has something
      {                             // deallocate it before copying.
	 FreeTree(_root);
         
	 _root = NULL;
        _Size = 0;
        _Found = FALSE;
      }

      CopyNode(tree._root);         // start copying from the _root.
   }

   return (*this);
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::SetComparisonFunction(int(*CmpFnc_)(const KEY&, const KEY&))
{
  _CompFunc = CmpFnc_;
}

//-------------------------------------------------------------------------
//
//  copyNode   - Private method to recursively copy nodes from src tree to
//               'this' tree.
//
//  src        - pointer to source tree to copy
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::CopyNode(BstNode<KEY, VALUE>* src)
{
   if (src == NULL)                           // Quit if source is empty.
      return;

   Insert(src->key, src->value);         // Copy node to destination tree.

   CopyNode(src -> left);
   CopyNode(src -> right);                   // Recursively do left & right subtrees.
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
size_t Bst<KEY, VALUE>::Size() const
{
  return _Size;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::MakeNode(const KEY& inskey_, VALUE* insvalptr_, int StoreAttrib_)
{
  BstNode<KEY, VALUE>* newNode;               // Pointer to new node.
  newNode = new BstNode<KEY, VALUE>;

  if (newNode == NULL)
    cerr << "Out of memory creating a node in the Bst!\n";

  StoreAttrib_ |= DataStorageAttributes::ACTIVE;
  newNode->value.AssignPtr(insvalptr_, StoreAttrib_);

  if (newNode->value.Pointee() == NULL)
    cerr << "Out of memory creating a node value in the Bst!\n";

  newNode->key = inskey_;
  newNode->left = NULL;                               // and initialize subtrees.
  newNode->right = NULL;

  ++_Size;
  return newNode;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::MakeNode(const KEY& inskey_, const DataStorePtr<VALUE>& insval_)
{
   BstNode<KEY, VALUE>* newNode;               // Pointer to new node.
   newNode = new BstNode<KEY, VALUE>;

   if (newNode == NULL)
     cerr << "Out of memory creating a node in the Bst!\n";

   newNode->value = insval_;

   if (newNode->value.Pointee() == NULL)
      cerr << "Out of memory creating a node value in the Bst!\n";

   newNode->key = inskey_;
   newNode->left = NULL;                               // and initialize subtrees.
   newNode->right = NULL;

   ++_Size;
   return newNode;
}

//-------------------------------------------------------------------------
//
//  makeNode   - Private method to create a binary search tree node with the given key and value
//               and return its address.
//
//
//  makekey    - key of node to insert
//  makevalue  - value of node to insert
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::MakeNode(const KEY& makekey, const VALUE& makevalue)
{
   BstNode<KEY, VALUE>* newNode;               // Pointer to new node.
   newNode = new BstNode<KEY, VALUE>;

   if (newNode == NULL)
     cerr << "Out of memory creating a node in the Bst!\n";

   newNode->value.AssignObject(makevalue);

   if (newNode->value.Pointee() == NULL)
      cerr << "Out of memory creating a node value in the Bst!\n";

   newNode->key = makekey;
   newNode->left = NULL;                               // and initialize subtrees.
   newNode->right = NULL;

   ++_Size;
   return newNode;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::InsertNode(const KEY& inskey_, VALUE* insvalptr_, int StoreAttrib_, BstNode<KEY, VALUE>* tree, int& Confirm_)
{
  if (tree == NULL)                                     // Case (i).
  {
    Confirm_ = 1;
    return MakeNode(inskey_, insvalptr_, StoreAttrib_);
  }

  if ((*_CompFunc)(inskey_, tree->key) < 0)           // Case (ii).
    tree->left = InsertNode(inskey_, insvalptr_, StoreAttrib_, tree->left, Confirm_);

  else if ((*_CompFunc)(inskey_, tree->key) > 0)      // Case (iii).
    tree->right = InsertNode(inskey_, insvalptr_, StoreAttrib_, tree->right, Confirm_);

  else                                                  // Case (iv).
  {
    Confirm_ = 0;
    StoreAttrib_ |= DataStorageAttributes::ACTIVE;
    tree->value.AssignPtr(insvalptr_, StoreAttrib_);

    if (tree->value.Pointee() == NULL)
      cerr << "Out of memory replacing a value in the Bst!\n";
  }

  return tree;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::InsertNode(const KEY& inskey_, const DataStorePtr<VALUE>& insval_, BstNode<KEY, VALUE>* tree, int& Confirm_)
{
   if (tree == NULL)                                     // Case (i).
   {
      Confirm_ = 1;
      return MakeNode(inskey_, insval_);
   }

   if ((*_CompFunc)(inskey_, tree->key) < 0)           // Case (ii).
      tree->left = InsertNode(inskey_, insval_, tree->left, Confirm_);

   else if ((*_CompFunc)(inskey_, tree->key) > 0)      // Case (iii).
      tree->right = InsertNode(inskey_, insval_, tree->right, Confirm_);

   else                                                  // Case (iv).
   {
      Confirm_ = 0;
      tree->value = insval_;

      if (tree->value.Pointee() == NULL)
	 cerr << "Out of memory replacing a value in the Bst!\n";
   }

   return tree;
}

//-------------------------------------------------------------------------
//
//  insertNode - Private method to insert a new node with the given key and value
//               into the tree using a recursive search.
//
//               4 Cases:
//               (i)   current tree is empty: return pointer to new node with key/value
//               (ii)  insert key < current key: insert in left subtree
//               (iii) insert key > current key: insert in right subtree
//               (iv)  insert key = current key: replace tree node's value
//
//  insertkey   - key of node to insert
//  insertvalue - value of node to insert
//  tree        - pointer to _root of subtree to traverse.
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::InsertNode(const KEY& insertkey, const VALUE& insertvalue, BstNode<KEY, VALUE>* tree, int& Confirm_)
{
   if (tree == NULL)                                     // Case (i).
   {
      Confirm_ = 1;
      return MakeNode(insertkey, insertvalue);
   }

   if ((*_CompFunc)(insertkey, tree->key) < 0)           // Case (ii).
      tree->left = InsertNode(insertkey, insertvalue, tree->left, Confirm_);

   else if ((*_CompFunc)(insertkey, tree->key) > 0)      // Case (iii).
      tree->right = InsertNode(insertkey, insertvalue, tree->right, Confirm_);

   else                                                  // Case (iv).
   {
      Confirm_ = 0;
      tree->value.AssignObject(insertvalue);
      
      if (tree->value.Pointee() == NULL)
	 cerr << "Out of memory replacing a value in the Bst!\n";
   }

   return tree;
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::Insert(const KEY& inskey_, VALUE* insvalptr_, int StoreAttrib_)
{
   int confirm_ = 0;
  _root = InsertNode(inskey_, insvalptr_, StoreAttrib_, _root, confirm_);
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::Insert(const KEY& inskey_, const DataStorePtr<VALUE>& insval_)
{
   int confirm_ = 0;
   _root = InsertNode(inskey_, insval_, _root, confirm_);
}

//-------------------------------------------------------------------------
//
//  insert     - Public method to insert a new node with the given key and value
//               into the tree using the private method insertNode to perform a recursive
//               search.  This routine starts insertNode at the _root.
//
//  insertkey   - key of node to insert
//  insertvalue - value of node to insert
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::Insert(const KEY& insertkey, const VALUE& insertvalue)
{
   int confirm_ = 0;
   _root = InsertNode(insertkey, insertvalue, _root, confirm_);
}

/****************************************************************************/
//
//  searchNode - Private method to search for the given key in the binary
//		 search tree using a recursive search.
//
//  4 Cases:
//    (i)   current tree is empty: return empty string
//    (ii)  search key < current key: search in left subtree
//    (iii) search key > current key: search in right subtree
//    (iv)  search key = current key: found - return tree node's value
//
//  searchkey   - key of node to search for
//  tree        - pointer to root of subtree to traverse.
//
template <class KEY, class VALUE>
VALUE* Bst<KEY, VALUE>::SearchNode(const KEY& searchkey, BstNode<KEY, VALUE>* tree)
{
  while (tree && (*_CompFunc)(searchkey, tree->key) != 0)
  {
    if ((*_CompFunc)(searchkey, tree->key) < 0)
      tree = tree->left;

    else if ((*_CompFunc)(searchkey, tree->key) > 0)
      tree = tree->right;
  }

  if (!tree)
  {
    _Found = FALSE;
    return NULL;
  }

  _Found = TRUE;
  return tree->value.Pointee();
}

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
int Bst<KEY, VALUE>::HasNode(const KEY& searchkey, BstNode<KEY, VALUE>* tree)
{
  while (tree && (*_CompFunc)(searchkey, tree->key) != 0)
  {
    if ((*_CompFunc)(searchkey, tree->key) < 0)
      tree = tree->left;

    else if ((*_CompFunc)(searchkey, tree->key) > 0)
      tree = tree->right;
  }

  if (!tree)
  {
    _Found = FALSE;  
    return 0;
  }

  _Found = TRUE;
  return 1;
}

//-------------------------------------------------------------------------
//
//  del        - Public method to remove the node with the given key from
//               the tree using the private method delNode to perform a recursive
//               search.  This routine starts delNode at the _root.
//
//  deletekey  - key of node to remove
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::Del(const KEY& deletekey)
{
   _root = DelNode(deletekey, _root);
}

//-------------------------------------------------------------------------
//
//  delNode    - Private method to remove the node with the given key from
//               the tree using a recursive search.
//
//  deletekey  - key of node to remove
//  tree       - pointer to _root of subtree to traverse.
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::DelNode(const KEY& deletekey, BstNode<KEY, VALUE>* tree)
{
   BstNode<KEY, VALUE> * pred;                           // Successor of deleted node.
   BstNode<KEY, VALUE> * subroot;                        // Root node to return.

   if (tree == NULL)                          // Quit if searching empty tree.   
     return NULL;

   if ((*_CompFunc)(tree->key, deletekey) > 0)          // See if node to delete is in one
   {                                                    // one of the subtrees.
      tree->left = DelNode(deletekey, tree->left);
      return tree;
   }
   else if ((*_CompFunc)(tree->key, deletekey) < 0)
   {
      tree->right = DelNode(deletekey, tree->right);
      return tree;
   }

   // At this point we know that tree->key == deletekey. There are 3 cases:
   //
   //  (i)   Node being deleted is only node in the tree: return NULL.
   //  (ii)  Node being deleted has one child, replace node with its child.
   //  (iii) Node being deleted has two children, find the successor of the right
   //        subtree and append it to the left subtree.

   if ( !tree->right && !tree->left)     // Case (i)
   {
      subroot = NULL;
      FreeNode(tree);
   }
   else if (!tree -> right)                // Case (ii), right subtree empty
   {
      subroot = tree -> left;
      FreeNode(tree);
   }

   else if (!tree -> left )                // Case (ii), left subtree empty
   {
      subroot = tree -> right;
      FreeNode(tree);
   }
   else                                   // Case (iii), both subtrees exist,
   {                                      // find smallest element in right subtree,
      pred = tree -> right;
      while (pred -> left != NULL)
        pred = pred -> left;

      pred -> left = tree -> left;        // Make tree's left subtree the
	                                  // left child of the right subtree's
      subroot = tree -> right;            // smallest element.
      FreeNode(tree);
   }

   return subroot;
}

//-------------------------------------------------------------------------
//
//  freeTree   - Private method to recursively free the binary search tree.
//
//  tree       - tree to free/deallocate.
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::FreeTree(BstNode<KEY, VALUE>*& tree)
{
   if (tree == NULL)                          // Can't delete empty tree.
     return;

   FreeTree(tree -> left);                   // Delete subtrees, then tree.
   FreeTree(tree -> right);
   FreeNode(tree);
}

//-------------------------------------------------------------------------
//
//  freeNode   - Private method to free a binary search tree node.
//
//  node       - node to free/deallocate.
//
//-------------------------------------------------------------------------
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::FreeNode(BstNode<KEY, VALUE>*& node)
{
  if (node)
  {
    node->value.Destroy();

    delete node;
    node = NULL;
    --_Size;
  }
}

/****************************************************************************/
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::PreOrder(BstNode<KEY, VALUE>* tnode, ostream& Os_, void(*travfnc)(BstNode<KEY, VALUE>*, ostream&))
{
  if (tnode)
  {
    travfnc(tnode, Os_);
    PreOrder(tnode->left, Os_, travfnc);
    PreOrder(tnode->right, Os_, travfnc);
  }
}

/****************************************************************************/
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::InOrder(BstNode<KEY, VALUE>* tnode, ostream& Os_, void(*travfnc)(BstNode<KEY, VALUE>*, ostream&))
{
  if (tnode)
  {
    InOrder(tnode->left, Os_, travfnc);
    travfnc(tnode, Os_);
    InOrder(tnode->right, Os_, travfnc);
  }
}

/****************************************************************************/
template <class KEY, class VALUE>
void Bst<KEY, VALUE>::PostOrder(BstNode<KEY, VALUE>* tnode, ostream& Os_, void(*travfnc)(BstNode<KEY, VALUE>*, ostream&))
{
  if (tnode)
  {
    PostOrder(tnode->left, Os_, travfnc);
    PostOrder(tnode->right, Os_, travfnc);
    travfnc(tnode, Os_);
  }
}

/****************************************************************************/
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* Bst<KEY, VALUE>::Root()
{
  return _root;
}

/****************************************************************************/
// PURPOSE:
//   Method to return the largest item in the tree
//
// PRE:
//   tree : root of the tree
//
// POST:
//   Returns the largest item in the tree
//
template <class KEY, class VALUE>
VALUE* Bst<KEY, VALUE>::GetLargest(BstNode<KEY, VALUE>* tree)
{
  if (!tree)
    return NULL;

  while (tree->right)
    tree = tree->right;

  return tree->value;
}

/****************************************************************************/
// PURPOSE:
//   Method to return the smallest item in the tree
//
// PRE:
//   tree : root of the tree
//
// POST:
//   Returns the smallest item in the tree
//
template <class KEY, class VALUE>
VALUE* Bst<KEY, VALUE>::GetSmallest(BstNode<KEY, VALUE>* tree)
{
  if (!tree)
    return NULL;

  while (tree->left)
    tree = tree->left;

  return tree->value;
}

/****************************************************************************/
// PURPOSE:
//   Method to get the height of a tree given the _root of it.
//
// PRE:
//   BstNode<T>* the _root of the binary tree
//
// POST:
//   Returns height of tree
//
template <class KEY, class VALUE>
size_t Bst<KEY, VALUE>::CalcHeight(BstNode<KEY, VALUE>* node) const
{
  size_t max = 0;

  if (!node)
    return 0;

  if (node->left && node->right)
    max = SubHeight(node);
  else if (node->left)
    max = CalcHeight(node->left);
  else if (node->right)
    max = CalcHeight(node->right);

  return (max + 1);
}

/****************************************************************************/
// PURPOSE:
//   Method to get the highest of two branchest of a tree given the _root
//   of it.
//
// PRE:
//   BstNode<KEY, VALUE>* the _root of the binary tree
//
// POST:
//   Returns highest of the two branches of a tree
//
template <class KEY, class VALUE>
size_t Bst<KEY, VALUE>::SubHeight(BstNode<KEY, VALUE>* node) const
{
  if (!node)
    return 0;

  return ObjRelation<int>::Max(CalcHeight(node->left), CalcHeight(node->right));
}

/****************************************************************************/
// PURPOSE:
//   Method to get the size of a tree given the _root of it.
//
// PRE:
//   BstNode<KEY, VALUE>* the _root of the binary tree
//
// POST:
//   Returns size of tree
//
template <class KEY, class VALUE>
size_t Bst<KEY, VALUE>::CalcSizeHelper(BstNode<KEY, VALUE>* node) const
{
  if (!node)
    return 0;

  return
  (
    1 + CalcSize(node->left)
      +	CalcSize(node->right)
  );    
}

/****************************************************************************/
template <class KEY, class VALUE>
size_t Bst<KEY, VALUE>::CalcSize(BstNode<KEY, VALUE>* node) const
{
  _Size = CalcSizeHelper(node);
  return _Size;
}

/****************************************************************************/
template <class KEY, class VALUE>
ostream& Bst<KEY, VALUE>::DumpNodeHelper(BstNode<KEY, VALUE>* Node_, ostream& Os_,
                                         size_t Level_, char* Branch_,
                                         void(*travfnc)(BstNode<KEY, VALUE>*, ostream&)) const
{
  if (Node_)
  {
    size_t x;
    
    if (travfnc)
      travfnc(Node_, Os_);
    else
    {
      for (x = 0; x < Level_; x++)
        Os_ <<"  ";
      
      Os_ <<Branch_;
      Os_ <<"Key: " <<Node_->key << "  Value: " <<*Node_->value <<endl;
    }
    
    DumpNodeHelper(Node_->left, Os_, Level_ + 1, "L.", travfnc);
    DumpNodeHelper(Node_->right, Os_, Level_ + 1, "R.", travfnc);
  }

  return Os_;
}

/****************************************************************************/
template <class KEY, class VALUE>
ostream& Bst<KEY, VALUE>::DumpNode(BstNode<KEY, VALUE>* Node_, ostream& Os_,
                                   void(*travfnc)(BstNode<KEY, VALUE>*, ostream&)) const
{
  DumpNodeHelper(Node_, Os_, 0, "*", travfnc);
  return Os_;
}

/****************************************************************************/
template <class KEY, class VALUE>
ostream& Bst<KEY, VALUE>::DumpNode(BstNode<KEY, VALUE>* Node_, ostream& Os_) const
{
  DumpNodeHelper(Node_, Os_, 0, "*", NULL);
  return Os_;
}

/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
BstAcceptor<KEY, VALUE>::BstAcceptor(const Bst<KEY, VALUE>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class KEY, class VALUE>
void BstAcceptor<KEY, VALUE>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const Bst<KEY, VALUE>*)Ptr_;
}

/****************************************************************************/
template <class KEY, class VALUE>
const void* BstAcceptor<KEY, VALUE>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean BstAcceptor<KEY, VALUE>::IsBst() const
{
  return TRUE;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* BstAcceptor<KEY, VALUE>::CreateFromBst(const ObjectAcceptor& Obj_)
{
  _VarPtr = new Bst<KEY, VALUE>(*((const Bst<KEY, VALUE>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* BstAcceptor<KEY, VALUE>::AssignFromBst(const ObjectAcceptor& Obj_)
{
  const Bst<KEY, VALUE>* TrgPtr_ = (const Bst<KEY, VALUE>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class KEY, class VALUE>
void* BstAcceptor<KEY, VALUE>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class KEY, class VALUE>
void BstAcceptor<KEY, VALUE>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif

/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
TreeWalker<KEY, VALUE>::TreeWalker(Bst<KEY, VALUE>* TreePtr_):
_Tree(TreePtr_),
_Here(TreePtr_->Root()),
_This(TreePtr_->Root()),
_Left(NULL),
_Right(NULL),
_WalkFunc(NULL),
_Status(NOTVISITED | LEFT_NOTVISITED | RIGHT_NOTVISITED)
{}

/****************************************************************************/
template <class KEY, class VALUE>
TreeWalker<KEY, VALUE>::TreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*)):
_Tree(TreePtr_),
_Here(TreePtr_->Root()),
_This(TreePtr_->Root()),
_Left(NULL),
_Right(NULL),
_WalkFunc(travfnc),
_Status(NOTVISITED | LEFT_NOTVISITED | RIGHT_NOTVISITED)
{}

/****************************************************************************/
template <class KEY, class VALUE>
TreeWalker<KEY, VALUE>::TreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*)):
_Tree(TreePtr_),
_Here(TreeNode_),
_This(TreeNode_),
_Left(NULL),
_Right(NULL),
_WalkFunc(travfnc),
_Status(NOTVISITED | LEFT_NOTVISITED | RIGHT_NOTVISITED)
{}

/****************************************************************************/
template <class KEY, class VALUE>
TreeWalker<KEY, VALUE>::TreeWalker(const TreeWalker<KEY, VALUE>& Obj_):
_Tree(Obj_.Tree()),
_Here(Obj_.Tree()->Root()),
_This(Obj_.Tree()->Root()),
_Left(NULL),
_Right(NULL),
_WalkFunc(NULL),
_Status(NOTVISITED | LEFT_NOTVISITED | RIGHT_NOTVISITED)
{}

/****************************************************************************/
template <class KEY, class VALUE>
void TreeWalker<KEY, VALUE>::Clear()
{
  delete _Left;
  delete _Right;
  _Left = _Right = NULL;
}

/****************************************************************************/
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* TreeWalker<KEY, VALUE>::Visit()
{
  if (_Status & NOTVISITED)
    _Status &= ~NOTVISITED;

  if (_WalkFunc && _This)
    (*_WalkFunc)(_This);

  return _This;
}

/****************************************************************************/
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* TreeWalker<KEY, VALUE>::operator ++ (int)
{
  BstNode<KEY, VALUE>* RetPtr_ = _Here;
  Next();

  return RetPtr_;
}

/****************************************************************************/
template <class KEY, class VALUE>
TreeWalker<KEY, VALUE>& TreeWalker<KEY, VALUE>::Reset()
{
  Clear();
  _Here = _Tree->Root();
  _Status = NOTVISITED | LEFT_NOTVISITED | RIGHT_NOTVISITED;
  SetBranches(_Tree);

  return *this;
}

/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
PreOrderTreeWalker<KEY, VALUE>::PreOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_):
TreeWalker<KEY, VALUE>(TreePtr_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PreOrderTreeWalker<KEY, VALUE>::PreOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*)):
TreeWalker<KEY, VALUE>(TreePtr_, travfnc)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PreOrderTreeWalker<KEY, VALUE>::PreOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*)):
TreeWalker<KEY, VALUE>(TreePtr_, TreeNode_, travfnc)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PreOrderTreeWalker<KEY, VALUE>::PreOrderTreeWalker(const PreOrderTreeWalker<KEY, VALUE>& Obj_):
TreeWalker<KEY, VALUE>(Obj_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PreOrderTreeWalker<KEY, VALUE>::PreOrderTreeWalker(const TreeWalker<KEY, VALUE>& Obj_):
TreeWalker<KEY, VALUE>(Obj_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PreOrderTreeWalker<KEY, VALUE>::~PreOrderTreeWalker()
{
  Clear();
}

/****************************************************************************/
template <class KEY, class VALUE>
void PreOrderTreeWalker<KEY, VALUE>::SetBranches(Bst<KEY, VALUE>* TreePtr_)
{
  if (_Here->left)
    _Left = new PreOrderTreeWalker<KEY, VALUE>(TreePtr_, _Here->left, _WalkFunc);
  else
    _Status &= ~LEFT_NOTVISITED;

  if (_Here->right)
    _Right = new PreOrderTreeWalker<KEY, VALUE>(TreePtr_, _Here->right, _WalkFunc);
  else
    _Status &= ~RIGHT_NOTVISITED;
}

/****************************************************************************/
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* PreOrderTreeWalker<KEY, VALUE>::Next()
{
  if (_Status & NOTVISITED)
    return Visit();
  else if (_Status & LEFT_NOTVISITED)
  {
    _Here = _Left->Next();
    if (_Here)
      return _Here;
    else
    {
      _Status &= ~LEFT_NOTVISITED;
      _Here = Next();
      return _Here;
    }
  }
  else if (_Status & RIGHT_NOTVISITED)
  {
    _Here = _Right->Next();
    if (_Here)
      return _Here;
    else
    {
      _Status &= ~RIGHT_NOTVISITED;
      _Here = Next();
      return _Here;
    }
  }

  if (!_Status)
    return NULL;  
}

/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
InOrderTreeWalker<KEY, VALUE>::InOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_):
TreeWalker<KEY, VALUE>(TreePtr_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
InOrderTreeWalker<KEY, VALUE>::InOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*)):
TreeWalker<KEY, VALUE>(TreePtr_, travfnc)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
InOrderTreeWalker<KEY, VALUE>::InOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*)):
TreeWalker<KEY, VALUE>(TreePtr_, TreeNode_, travfnc)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
InOrderTreeWalker<KEY, VALUE>::InOrderTreeWalker(const InOrderTreeWalker<KEY, VALUE>& Obj_):
TreeWalker<KEY, VALUE>(Obj_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
InOrderTreeWalker<KEY, VALUE>::InOrderTreeWalker(const TreeWalker<KEY, VALUE>& Obj_):
TreeWalker<KEY, VALUE>(Obj_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
InOrderTreeWalker<KEY, VALUE>::~InOrderTreeWalker()
{
  Clear();
}

/****************************************************************************/
template <class KEY, class VALUE>
void InOrderTreeWalker<KEY, VALUE>::SetBranches(Bst<KEY, VALUE>* TreePtr_)
{
  if (_Here->left)
    _Left = new InOrderTreeWalker<KEY, VALUE>(TreePtr_, _Here->left, _WalkFunc);
  else
    _Status &= ~LEFT_NOTVISITED;

  if (_Here->right)
    _Right = new InOrderTreeWalker<KEY, VALUE>(TreePtr_, _Here->right, _WalkFunc);
  else
    _Status &= ~RIGHT_NOTVISITED;
}

/****************************************************************************/
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* InOrderTreeWalker<KEY, VALUE>::Next()
{
  if (_Status & LEFT_NOTVISITED)
  {
    _Here = _Left->Next();
    if (_Here)
      return _Here;
    else
    {
      _Status &= ~LEFT_NOTVISITED;
      _Here = Next();
      return _Here;
    }
  }
  else if (_Status & NOTVISITED)
    return Visit();        
  else if (_Status & RIGHT_NOTVISITED)
  {
    _Here = _Right->Next();
    if (_Here)
      return _Here;
    else
    {
      _Status &= ~RIGHT_NOTVISITED;
      _Here = Next();
      return _Here;
    }
  }

  if (!_Status)
    return NULL;  
}

/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
PostOrderTreeWalker<KEY, VALUE>::PostOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_):
TreeWalker<KEY, VALUE>(TreePtr_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PostOrderTreeWalker<KEY, VALUE>::PostOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*)):
TreeWalker<KEY, VALUE>(TreePtr_, travfnc)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PostOrderTreeWalker<KEY, VALUE>::PostOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*)):
TreeWalker<KEY, VALUE>(TreePtr_, TreeNode_, travfnc)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PostOrderTreeWalker<KEY, VALUE>::PostOrderTreeWalker(const PostOrderTreeWalker<KEY, VALUE>& Obj_):
TreeWalker<KEY, VALUE>(Obj_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PostOrderTreeWalker<KEY, VALUE>::PostOrderTreeWalker(const TreeWalker<KEY, VALUE>& Obj_):
TreeWalker<KEY, VALUE>(Obj_)
{
  SetBranches(TreePtr_);
}

/****************************************************************************/
template <class KEY, class VALUE>
PostOrderTreeWalker<KEY, VALUE>::~PostOrderTreeWalker()
{
  Clear();
}

/****************************************************************************/
template <class KEY, class VALUE>
void PostOrderTreeWalker<KEY, VALUE>::SetBranches(Bst<KEY, VALUE>* TreePtr_)
{
  if (_Here->left)
    _Left = new PostOrderTreeWalker<KEY, VALUE>(TreePtr_, _Here->left, _WalkFunc);
  else
    _Status &= ~LEFT_NOTVISITED;

  if (_Here->right)
    _Right = new PostOrderTreeWalker<KEY, VALUE>(TreePtr_, _Here->right, _WalkFunc);
  else
    _Status &= ~RIGHT_NOTVISITED;
}

/****************************************************************************/
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* PostOrderTreeWalker<KEY, VALUE>::Next()
{
  if (_Status & LEFT_NOTVISITED)
  {
    _Here = _Left->Next();
    if (_Here)
      return _Here;
    else
    {
      _Status &= ~LEFT_NOTVISITED;
      _Here = Next();
      return _Here;      
    }
  }
  else if (_Status & RIGHT_NOTVISITED)
  {
    _Here = _Right->Next();
    if (_Here)
      return _Here;
    else
    {
      _Status &= ~RIGHT_NOTVISITED;
      _Here = Next();
      return _Here;      
    }
  }
  else if (_Status & NOTVISITED)
    return Visit();
    
  if (!_Status)
    return NULL;  
}

/****************************************************************************/
#endif






