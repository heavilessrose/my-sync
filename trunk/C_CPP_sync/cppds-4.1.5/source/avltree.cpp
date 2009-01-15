// Name		: Joseph Wong
// Course	: cpsc 335
// Section	: 1
// Instructor 	: Nou Dadoun
//
// AVL tree ADT class -- source file
//
#ifndef AVLTREE_CPP
#define AVLTREE_CPP
#ifndef AVLTREE_H
  #include "avltree.h"
#endif

/****************************************************************************/
template <class KEY, class VALUE>
AvlNode<KEY, VALUE>::AvlNode():
_balancefactor(0)
{}

/************************* AVL tree helper methods **************************/
/****************************************************************************/
template <class KEY, class VALUE>
AvlTree<KEY, VALUE>::AvlTree():
Bst<KEY, VALUE>()
{}

/****************************************************************************/
template <class KEY, class VALUE>    
AvlTree<KEY, VALUE>::AvlTree(const Bst<KEY, VALUE>& tree):
Bst<KEY, VALUE>(tree)
{}

/****************************************************************************/
template <class KEY, class VALUE>
AvlTree<KEY, VALUE>::AvlTree(const AvlTree<KEY, VALUE>& tree):
Bst<KEY, VALUE>(tree)
{}

/****************************************************************************/
template <class KEY, class VALUE>
Object& AvlTree<KEY, VALUE>::Assign(const Object& Obj_, ObjectAcceptor* ObjComp_)
{
  AvlTreeAcceptor<KEY, VALUE> TrgComp_(this);

  if (ObjComp_ == NULL)
  {
    ((Object*)&Obj_)->Assign(*this, &TrgComp_);
    return *TrgComp_._VarPtr;
  }

  return *((Object*) (ObjComp_->AssignFromAvlTree(TrgComp_)));
}

/****************************************************************************/
template <class KEY, class VALUE>
Object* AvlTree<KEY, VALUE>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new AvlTree<KEY, VALUE>(*this));

  AvlTreeAcceptor<KEY, VALUE> TrgComp_(this);
  return ((Object*) (ObjComp_->CreateFromAvlTree(TrgComp_)));
}

/****************************************************************************/
template <class KEY, class VALUE>
AvlTree<KEY, VALUE>* AvlTree<KEY, VALUE>::CloneAvlTree() const
{
  return (new AvlTree<KEY, VALUE>(*this));
}

/****************************************************************************/
template <class KEY, class VALUE>
AvlTree<KEY, VALUE>& AvlTree<KEY, VALUE>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static AvlTree<KEY, VALUE> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

/****************************************************************************/
template <class KEY, class VALUE>
const Object& AvlTree<KEY, VALUE>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return AvlTree<KEY, VALUE>::GiveNullObject();
}

/****************************************************************************/
template <class KEY, class VALUE>
Object& AvlTree<KEY, VALUE>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return AvlTree<KEY, VALUE>::GiveNullObject();
}

/****************************************************************************/
template <class KEY, class VALUE>
int AvlTree<KEY, VALUE>::ClassID() const
{
  return TypeInfo::AVLTREE_TYPE;
}

/****************************************************************************/
template <class KEY, class VALUE>
const ObjectAcceptor* AvlTree<KEY, VALUE>::GiveObjectAcceptor() const
{
  return (new AvlTreeAcceptor<KEY, VALUE>(this));
}

/****************************************************************************/
template <class KEY, class VALUE>
int AvlTree<KEY, VALUE>::CalcBalance(BstNode<KEY, VALUE>* tree)
{
  if (!tree)
    return 0;

  int ht = CalcHeight(tree->left);
  return (ht - CalcHeight(tree->right));
}

/****************************************************************************/
// PURPOSE:
//   Method to rebalance tree after insertion
//
// PRE:
//   BstNode<KEY, VALUE>* tree		: _root of tree
//
// POST:
//   rebalances tree and return new _root of tree
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::Rebalance(BstNode<KEY, VALUE>* tree)
{
  if (tree)
  {
    int balance = CalcBalance(tree);
    tree->SetBalance(balance);

    if (abs(balance) >= 2)
      if (balance >= 2)
      {
	if (tree->left->GetBalance() > 0)
	  tree = LeftOfLeft(tree);
	else
	  tree = RightOfLeft(tree);
      }
      else
      {
	if (tree->right->GetBalance() < 0)
	  tree = RightOfRight(tree);
	else
	  tree = LeftOfRight(tree);
      }
  }

  return tree;
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   removes _root, furthest right child of left subtree becomes new _root.
//
// PRE:
//   BstNode<KEY, VALUE>* _root of tree
//
// POST:
//   returns new node of tree after deletion
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RmLeftSubr(BstNode<KEY, VALUE>* tree, BstNode<KEY, VALUE>* piv)
{
  BstNode<KEY, VALUE>* newpiv;

  if (tree->right->right)
  {
    newpiv = RmLeftSubr(tree->right, piv);
    tree->right = Rebalance(tree->right);
    return newpiv;
  }

  newpiv = tree->right;
  tree->right = newpiv->left;
  newpiv->left = piv->left;
  newpiv->right = piv->right;

  newpiv->left = Rebalance(newpiv->left);

  return Rebalance(newpiv);
}

/****************************************************************************/
// PURPOSE:
//   Wrapper procedure for rmrightsubl, used for suBstituting the furthest
//   right child of the left subtree as the new pivot in deletions
//
// PRE:
//   BstNode<KEY, VALUE>* : the current pivot node
//
// POST:
//   Returns the new pivot after the deletion is completed.
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RmLeftRight(BstNode<KEY, VALUE>* tree)
{
  return RmLeftSubr(tree->left, tree);
}

/****************************************************************************/
// PURPOSE:
//   removes _root, furthest left child of left subtree becomes new _root.
//
// PRE:
//   BstNode<KEY, VALUE>* _root of tree
//
// POST:
//   returns new node of tree after deletion
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RmRightSubl(BstNode<KEY, VALUE>* tree, BstNode<KEY, VALUE>* piv)
{
  BstNode<KEY, VALUE>* newpiv;

  if (tree->left->left)
  {
    newpiv = RmRightSubl(tree->left, piv);
    tree->left = Rebalance(tree->left);
    return newpiv;
  }

  newpiv = tree->left;
  tree->left = newpiv->right;
  newpiv->left = piv->left;
  newpiv->right = piv->right;

  newpiv->right = Rebalance(newpiv->right);

  return Rebalance(newpiv);
}

/****************************************************************************/
// PURPOSE:
//   Wrapper procedure for rmrightsubl, used for suBstituting the furthest
//   left child of the right subtree as the new pivot in deletions
//
// PRE:
//   BstNode<KEY, VALUE>* : the current pivot node
//
// POST:
//   Returns the new pivot after the deletion is completed.
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RmRightLeft(BstNode<KEY, VALUE>* tree)
{
  return RmRightSubl(tree->right, tree);
}

/****************************************************************************/
// PURPOSE:
//   deletion method used when left subtree has no right child and
//   right subtree has no left child. Forming V shape tree.
//   removes _root. Left subtree becomes new _root.
//
// PRE:
//   BstNode<KEY, VALUE>* _root of tree
//
// POST:
//   returns new node of tree after deletion
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RmLeft(BstNode<KEY, VALUE>* tree)
{
  BstNode<KEY, VALUE>* tnode = tree;

  tree = tnode->left;
  tree->right = tnode->right;

  return Rebalance(tree);
}

/****************************************************************************/
// PURPOSE:
//   deletion method used when left subtree has no right child and
//   right subtree has no left child. Forming V shape tree.
//   removes _root. Right subtree becomes new _root.
//
// PRE:
//   BstNode<KEY, VALUE>* _root of tree
//
// POST:
//   returns new node of tree after deletion
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RmRight(BstNode<KEY, VALUE>* tree)
{
  BstNode<KEY, VALUE>* tnode = tree;

  tree = tnode->right;
  tree->left = tnode->left;

  return Rebalance(tree);
}

/****************************************************************************/
// PURPOSE:
//   Deletion method used when tree has both left and right subtrees
//
// PRE:
//   Calls appropriate methods to perform deletions
//
// POST:
//   Returns new _root of tree after deletion.
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RmHasBoth(BstNode<KEY, VALUE>* tree)
{
  int htl, htlr, htr, htrl, maxht;

  htl = ((htlr = CalcHeight(tree->left->right)) ? 0:CalcHeight(tree->left));
  htr = ((htrl = CalcHeight(tree->right->left)) ? 0:CalcHeight(tree->right));

  maxht = ObjRelation<int>::Max(ObjRelation<int>::Max(htlr, htrl),
                                ObjRelation<int>::Max(htl, htr));

  return
  (
    (maxht == htl) ? RmLeft(tree):
    (maxht == htr) ? RmRight(tree):
    (maxht == htlr) ? RmLeftRight(tree):
		      RmRightLeft(tree)
  );
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Left of left rotation
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::LeftOfLeft(BstNode<KEY, VALUE>* tree)
{
  BstNode<KEY, VALUE> *tlc, *tlrc;

  // begin by altering the necessary pointers
  tlc = tree->left;
  tlrc = tlc->right;
  tlc->right = tree;
  tree->left = tlrc;
  tree = tlc;

  tree->SetBalance(0);
  tree->right->SetBalance(0);

  return tree;
}

/****************************************************************************/
// PURPOSE:
//   Right of left rotation
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RightOfLeft(BstNode<KEY, VALUE>* tree)
{
  int bal = tree->left->right->GetBalance();

  // First adjust pointers by performing AVL rotations
  tree->left = RightOfRight(tree->left);
  tree = LeftOfLeft(tree);

  // The reset the balance factors according to the subcase
  if (bal == 0)
  { // subcase 1
    tree->left->SetBalance(0);
    tree->right->SetBalance(0);
  }
  else if (bal == 1)
  { // subcase 2
    tree->SetBalance(0);
    tree->left->SetBalance(0);
    tree->right->SetBalance(-1);
  }
  else
  { // subcase 3
    tree->SetBalance(0);
    tree->left->SetBalance(1);
    tree->right->SetBalance(0);
  }

  return tree;
}

/****************************************************************************/
// PURPOSE:
//   Right of right rotation
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::RightOfRight(BstNode<KEY, VALUE>* tree)
{
  BstNode<KEY, VALUE> *trc, *trlc;

  // begin by altering the necessary pointers
  trc = tree->right;
  trlc = trc->left;
  trc->left = tree;
  tree->right = trlc;
  tree = trc;

  tree->SetBalance(0);
  tree->left->SetBalance(0);

  return tree;
}

/****************************************************************************/
// PURPOSE:
//   Left of right rotation
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::LeftOfRight(BstNode<KEY, VALUE>* tree)
{
  int bal = tree->right->left->GetBalance();

  // First adjust pointers by performing AVL rotations
  tree->right = LeftOfLeft(tree->right);
  tree = RightOfRight(tree);

  // The reset the balance factors according to the subcase
  if (bal == 0)
  { // subcase 1
    tree->left->SetBalance(0);
    tree->right->SetBalance(0);
  }
  else if (bal == 1)
  { // subcase 2
    tree->SetBalance(0);
    tree->left->SetBalance(0);
    tree->right->SetBalance(-1);
  }
  else
  { // subcase 3
    tree->SetBalance(0);
    tree->left->SetBalance(1);
    tree->right->SetBalance(0);
  }

  return tree;
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Node removal procedure
//
// PRE:
//   int deletekey 	: the key of the node to delete
//   BstNode<KEY, VALUE>* tree	: the _root of the tree
//
// POST:
//   deletes node with specified key and returns new _root of tree
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::DelNode(const KEY& deletekey, BstNode<KEY, VALUE>* tree)
{
  BstNode<KEY, VALUE>* retp;                        // returned pointer to delete.

  if (tree == NULL)                     // Quit if searching empty tree.
    return NULL;

  if (tree -> key > deletekey)          // See if node to delete is in one
  {                                     // one of the subtrees.
    tree->left = DelNode(deletekey, tree->left);
    return Rebalance(tree);
  }
  else if (tree -> key < deletekey)
  {
    tree->right = DelNode(deletekey, tree->right);
    return Rebalance(tree);
  }

  if (tree->right && tree->left)
  {
    retp = tree;
    tree = RmHasBoth(tree);
  }
  else if (tree->left)
  {
    retp = tree;
    tree = retp->left;
  }
  else if (tree->right)
  {
    retp = tree;
    tree = retp->right;
  }
  else
  {
    retp = tree;
    tree = NULL;
  }

  if (retp)
    delete retp;

  return tree;
}

/****************************************************************************/
// PURPOSE:
//   Method to make a new node
//
// PRE:
//   int makekey		: the key of the node to make
//   int makevalue		: the value of the node to make
//
// POST:
//   Returns new node
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::MakeNode(const KEY& makekey, const VALUE& makevalue)
{
  AvlNode<KEY, VALUE>* newNode;       // Pointer to new node.

  newNode = new AvlNode<KEY, VALUE>;

  if (newNode == NULL)
    cerr << "Out of memory creating a node in the Bst!\n";

  newNode->value.AssignObject(makevalue);

  if (newNode->value.Pointee() == NULL)
    cerr << "Out of memory creating a node value in the Bst!\n";

  newNode -> key = makekey;
  newNode -> left = NULL;                               // and initialize subtrees.
  newNode -> right = NULL;
  newNode -> _balancefactor = 0;

  ++_Size;
  return newNode;
}

/****************************************************************************/
// PURPOSE:
//   Method to make a new node
//
// PRE:
//   int makekey		: the key of the node to make
//   int makevalue		: the value of the node to make
//
// POST:
//   Returns new node
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::MakeNode(const KEY& makekey, const DataStorePtr<VALUE>& makevalue)
{
  AvlNode<KEY, VALUE>* newNode;       // Pointer to new node.

  newNode = new AvlNode<KEY, VALUE>;

  if (newNode == NULL)
    cerr << "Out of memory creating a node in the Bst!\n";

  newNode->value = makevalue;

  if (newNode->value.Pointee() == NULL)
    cerr << "Out of memory creating a node value in the Bst!\n";

  newNode -> key = makekey;
  newNode -> left = NULL;                               // and initialize subtrees.
  newNode -> right = NULL;
  newNode -> _balancefactor = 0;

  ++_Size;
  return newNode;
}

/****************************************************************************/
// PURPOSE:
//   Method to make a new node
//
// PRE:
//   int makekey		: the key of the node to make
//   int makevalue		: the value of the node to make
//
// POST:
//   Returns new node
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::MakeNode(const KEY& makekey, VALUE* makevalue, int StoreAttrib_)
{
  AvlNode<KEY, VALUE>* newNode;       // Pointer to new node.

  newNode = new AvlNode<KEY, VALUE>;

  if (newNode == NULL)
    cerr << "Out of memory creating a node in the Bst!\n";

  StoreAttrib_ |= DataStorageAttributes::ACTIVE;
  newNode->value.AssignPtr(makevalue, StoreAttrib_);

  if (newNode->value.Pointee() == NULL)
    cerr << "Out of memory creating a node value in the Bst!\n";

  newNode -> key = makekey;
  newNode -> left = NULL;                               // and initialize subtrees.
  newNode -> right = NULL;
  newNode -> _balancefactor = 0;

  ++_Size;
  return newNode;
}

/****************************************************************************/
// PURPOSE:
//   Method to insert a new node into a tree or update an existing node
//   with new data.
//
// PRE:
//   int insertkey		: the key of the node to insert
//   int insertvalue		: the value of the node to insert
//   BstNode<KEY, VALUE>* tree		: _root of tree
//
// POST:
//   rebalances tree and return new _root of tree after performing insertion.
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::InsertNode
(const KEY& insertkey, const VALUE& insertvalue, BstNode<KEY, VALUE>* tree, int& confirm)
{
  if (tree == NULL)
  {
    confirm = 1;
    return MakeNode(insertkey, insertvalue);
  }

  if (insertkey < tree->key)
    tree->left = InsertNode(insertkey, insertvalue, tree->left, confirm);
  else if (insertkey > tree->key)
    tree->right = InsertNode(insertkey, insertvalue, tree->right, confirm);
  else
  {
    confirm = 0;
    tree->value.AssignObject(insertvalue);
    
    if (tree->value.Pointee() == NULL)
      cerr <<ERRMSG_NOMEMBST;
  }

  if (confirm)
    return Rebalance(tree);

  return tree;
}

/****************************************************************************/
// PURPOSE:
//   Method to insert a new node into a tree or update an existing node
//   with new data.
//
// PRE:
//   int insertkey		: the key of the node to insert
//   int insertvalue		: the value of the node to insert
//   BstNode<KEY, VALUE>* tree		: _root of tree
//
// POST:
//   rebalances tree and return new _root of tree after performing insertion.
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::InsertNode
(const KEY& insertkey, const DataStorePtr<VALUE>& insertvalue, BstNode<KEY, VALUE>* tree, int& confirm)
{
  if (tree == NULL)
  {
    confirm = 1;
    return MakeNode(insertkey, insertvalue);
  }

  if (insertkey < tree->key)
    tree->left = InsertNode(insertkey, insertvalue, tree->left, confirm);
  else if (insertkey > tree->key)
    tree->right = InsertNode(insertkey, insertvalue, tree->right, confirm);
  else
  {
    confirm = 0;
    tree->value = insertvalue;
    
    if (tree->value.Pointee() == NULL)
      cerr <<ERRMSG_NOMEMBST;
  }

  if (confirm)
    return Rebalance(tree);

  return tree;
}

/****************************************************************************/
// PURPOSE:
//   Method to insert a new node into a tree or update an existing node
//   with new data.
//
// PRE:
//   int insertkey		: the key of the node to insert
//   int insertvalue		: the value of the node to insert
//   BstNode<KEY, VALUE>* tree		: _root of tree
//
// POST:
//   rebalances tree and return new _root of tree after performing insertion.
//
template <class KEY, class VALUE>
BstNode<KEY, VALUE>* AvlTree<KEY, VALUE>::InsertNode
(const KEY& insertkey, VALUE* insertvalue, int StoreAttrib_, BstNode<KEY, VALUE>* tree, int& confirm)
{
  if (tree == NULL)
  {
    confirm = 1;
    return MakeNode(insertkey, insertvalue, StoreAttrib_);
  }

  if (insertkey < tree->key)
    tree->left = InsertNode(insertkey, insertvalue, StoreAttrib_, tree->left, confirm);
  else if (insertkey > tree->key)
    tree->right = InsertNode(insertkey, insertvalue, StoreAttrib_, tree->right, confirm);
  else
  {
    confirm = 0;  
    StoreAttrib_ |= DataStorageAttributes::ACTIVE;
    tree->value.AssignPtr(insertvalue, StoreAttrib_);
    
    if (tree->value.Pointee() == NULL)
      cerr <<ERRMSG_NOMEMBST;
  }

  if (confirm)
    return Rebalance(tree);

  return tree;
}

/*********************** deletion / insertion methods ***********************/
/****************************************************************************/
// PURPOSE:
//   Method to inser a node into the tree
//
// PRE:
//   Calls insertNode to insert the node at the _root of tree and search
//   down for insertion point.
//
template <class KEY, class VALUE>
void AvlTree<KEY, VALUE>::Insert(const KEY& insertkey, const VALUE& insertvalue)
{
   int confirm = 0;
   _root = InsertNode(insertkey, insertvalue, _root, confirm);
}

/****************************************************************************/
// PURPOSE:
//   Method to inser a node into the tree
//
// PRE:
//   Calls insertNode to insert the node at the _root of tree and search
//   down for insertion point.
//
template <class KEY, class VALUE>
void AvlTree<KEY, VALUE>::Insert(const KEY& insertkey, const DataStorePtr<VALUE>& insertvalue)
{
   int confirm = 0;
   _root = InsertNode(insertkey, insertvalue, _root, confirm);
}

/****************************************************************************/
template <class KEY, class VALUE>
void AvlTree<KEY, VALUE>::Insert(const KEY& inskey_, VALUE* insval_, int StoreAttrib_)
{
   int confirm = 0;
   _root = InsertNode(inskey_, insval_, StoreAttrib_, _root, confirm);
}

/****************************************************************************/
/****************************************************************************/
template <class KEY, class VALUE>
AvlTreeAcceptor<KEY, VALUE>::AvlTreeAcceptor(const AvlTree<KEY, VALUE>* Ptr_):
_ObjPtr(Ptr_)
{}

/****************************************************************************/
template <class KEY, class VALUE>
void AvlTreeAcceptor<KEY, VALUE>::AcceptDataPtr(const void* Ptr_)
{
  _ObjPtr = (const AvlTree<KEY, VALUE>*)Ptr_;
}

/****************************************************************************/
template <class KEY, class VALUE>
const void* AvlTreeAcceptor<KEY, VALUE>::GiveDataPtr() const
{
  return _ObjPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean AvlTreeAcceptor<KEY, VALUE>::IsAvlTree() const
{
  return TRUE;
}

/****************************************************************************/
template <class KEY, class VALUE>
Boolean AvlTreeAcceptor<KEY, VALUE>::IsBst() const
{
  return TRUE;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* AvlTreeAcceptor<KEY, VALUE>::CreateFromAvlTree(const ObjectAcceptor& Obj_)
{
  _VarPtr = new AvlTree<KEY, VALUE>(*((const AvlTree<KEY, VALUE>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* AvlTreeAcceptor<KEY, VALUE>::CreateFromBst(const ObjectAcceptor& Obj_)
{
  _VarPtr = new AvlTree<KEY, VALUE>(*((const Bst<KEY, VALUE>*)Obj_.GiveDataPtr()));
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* AvlTreeAcceptor<KEY, VALUE>::AssignFromAvlTree(const ObjectAcceptor& Obj_)
{
  const AvlTree<KEY, VALUE>* TrgPtr_ = (const AvlTree<KEY, VALUE>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
template <class KEY, class VALUE>
void* AvlTreeAcceptor<KEY, VALUE>::AssignFromBst(const ObjectAcceptor& Obj_)
{
  const Bst<KEY, VALUE>* TrgPtr_ = (const Bst<KEY, VALUE>*)Obj_.GiveDataPtr();
  *_VarPtr = *TrgPtr_;
  return _VarPtr;
}

/****************************************************************************/
#if OVERLOAD_NEW
template <class KEY, class VALUE>
void* AvlTreeAcceptor<KEY, VALUE>::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
template <class KEY, class VALUE>
void AvlTreeAcceptor<KEY, VALUE>::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
/****************************************************************************/
#endif




