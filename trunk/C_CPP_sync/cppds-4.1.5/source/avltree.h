//
// AVL tree ADT class -- header file
//
#ifndef AVLTREE_H
#define AVLTREE_H
#ifndef BST_CPP
  #include "bst.cpp"
#endif
#ifndef MATHFNCS_H
  #include "mathfncs.h"
#endif
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif

/****************************************************************************/
template <class KEY, class VALUE>
class AvlTree;

template <class KEY, class VALUE>
class AvlTreeAcceptor : public ObjectAcceptor
{
  friend class AvlTree<KEY, VALUE>;

  protected:
    union
    {
      const AvlTree<KEY, VALUE>* _ObjPtr;
      AvlTree<KEY, VALUE>* _VarPtr;
    };

    AvlTreeAcceptor(const AvlTree<KEY, VALUE>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // AvlTree class specific acceptor methods
    virtual Boolean IsAvlTree() const;
    virtual Boolean IsBst() const;
    
    virtual void* CreateFromAvlTree(const ObjectAcceptor& Obj_);
    virtual void* CreateFromBst(const ObjectAcceptor& Obj_);
    virtual void* AssignFromAvlTree(const ObjectAcceptor& Obj_);
    virtual void* AssignFromBst(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
template <class KEY, class VALUE>
struct AvlNode : public BstNode<KEY, VALUE>
{
  int _balancefactor;

  virtual int IncBalance()
	{ return ++_balancefactor; }
  virtual int DecBalance()
	{ return --_balancefactor; }

  virtual void SetBalance(int bal)
	{ _balancefactor = bal; }
  virtual int GetBalance()
	{ return _balancefactor; }

  AvlNode();
};

template <class KEY, class VALUE>
class AvlTree : public Bst<KEY, VALUE>
{
  protected:
    int CalcBalance(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* Rebalance(BstNode<KEY, VALUE>* tree);

    BstNode<KEY, VALUE>* LeftOfLeft(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* RightOfRight(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* RightOfLeft(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* LeftOfRight(BstNode<KEY, VALUE>* tree);

    BstNode<KEY, VALUE>* RmLeftSubr(BstNode<KEY, VALUE>* tree, BstNode<KEY, VALUE>* piv);
    BstNode<KEY, VALUE>* RmRightSubl(BstNode<KEY, VALUE>* tree, BstNode<KEY, VALUE>* piv);
    BstNode<KEY, VALUE>* RmLeftRight(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* RmRightLeft(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* RmRight(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* RmLeft(BstNode<KEY, VALUE>* tree);
    BstNode<KEY, VALUE>* RmHasBoth(BstNode<KEY, VALUE>* tree);

    virtual BstNode<KEY, VALUE>* DelNode(const KEY& deletekey, BstNode<KEY, VALUE>* tree);

    virtual BstNode<KEY, VALUE>* InsertNode(const KEY& insertkey, const VALUE& insertvalue, BstNode<KEY, VALUE>* tree, int& confirm);
    virtual BstNode<KEY, VALUE>* InsertNode(const KEY& insertkey, const DataStorePtr<VALUE>& insertvalue, BstNode<KEY, VALUE>* tree, int& confirm);
    virtual BstNode<KEY, VALUE>* InsertNode(const KEY& insertkey, VALUE* insertvalue, int StoreAttrib_, BstNode<KEY, VALUE>* tree, int& confirm);
    
    virtual BstNode<KEY, VALUE>* MakeNode(const KEY& makekey, const VALUE& makevalue);
    virtual BstNode<KEY, VALUE>* MakeNode(const KEY& makekey, const DataStorePtr<VALUE>& makevalue);
    virtual BstNode<KEY, VALUE>* MakeNode(const KEY& makekey, VALUE* makevalue, int StoreAttrib_);

    virtual void Insert(const KEY& insertkey, const DataStorePtr<VALUE>& insertvalue);    

  public:
    AvlTree();
    AvlTree(const Bst<KEY, VALUE>& tree);
    AvlTree(const AvlTree<KEY, VALUE>& tree);
  
    virtual void Insert(const KEY& insertkey, const VALUE& insertvalue);
    virtual void Insert(const KEY& inskey_, VALUE* insval_, int StoreAttrib_=DataStorageAttributes::ACTIVE);

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual AvlTree<KEY, VALUE>* CloneAvlTree() const;

    // Static null object accessor methods
    static AvlTree<KEY, VALUE>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::AVLTREE_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;
};

/****************************************************************************/
#endif





