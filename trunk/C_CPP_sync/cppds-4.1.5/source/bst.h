//-------------------------------------------------------------------------
//
//    BST.H  --  Declarations for a Binary Search Tree Class
//
//    Written July 96   - N. Dadoun
//
//    Modification History:
//
//
//
//-------------------------------------------------------------------------
//
#ifndef BST_H
#define BST_H

#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
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

#define ERRMSG_NOMEMBST 	"Out of memory replacing a value in the bst!\n"

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
class Bst;

template <class KEY, class VALUE>
class BstAcceptor : public ObjectAcceptor
{
  friend class Bst<KEY, VALUE>;

  protected:
    union
    {
      const Bst<KEY, VALUE>* _ObjPtr;
      Bst<KEY, VALUE>* _VarPtr;
    };

    BstAcceptor(const Bst<KEY, VALUE>* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // Bst class specific acceptor methods
    virtual Boolean IsBst() const;
    
    virtual void* CreateFromBst(const ObjectAcceptor& Obj_);
    virtual void* AssignFromBst(const ObjectAcceptor& Obj_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
struct BstNode
{
  KEY key;
  DataStorePtr<VALUE> value;

  BstNode<KEY, VALUE>* left;
  BstNode<KEY, VALUE>* right;

  virtual int IncBalance()
      { return 0; }
  virtual int DecBalance()
      { return 0; }

  virtual void SetBalance(int bal){}
  virtual int GetBalance()
      { return 0; }

  BstNode();
};

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
class Bst : public ObjectSkeleton
{
  public:
    typedef int(*CompFuncType)(const KEY&, const KEY&);

  protected:
    static CompFuncType _CompFunc;
    BstNode<KEY, VALUE>* _root;
    size_t _Size;
    Boolean _Found;

    size_t Pow2(size_t Exp_) const;
    long MaxNodes() const;

    void CopyNode(BstNode<KEY, VALUE>*);
    virtual BstNode<KEY, VALUE>* DelNode(const KEY&, BstNode<KEY, VALUE>*);

    void FreeNode(BstNode<KEY, VALUE>*&);
    void FreeTree(BstNode<KEY, VALUE>*&);

    size_t SubHeight(BstNode<KEY, VALUE>* node) const;
    size_t CalcSizeHelper(BstNode<KEY, VALUE>* node) const;

    virtual BstNode<KEY, VALUE>* MakeNode(const KEY&, VALUE*, int);
    virtual BstNode<KEY, VALUE>* MakeNode(const KEY&, const VALUE&);    
    virtual BstNode<KEY, VALUE>* MakeNode(const KEY&, const DataStorePtr<VALUE>&);

    virtual BstNode<KEY, VALUE>* InsertNode(const KEY&, VALUE*, int, BstNode<KEY, VALUE>*, int&);
    virtual BstNode<KEY, VALUE>* InsertNode(const KEY&, const VALUE&, BstNode<KEY, VALUE>*, int&);
    virtual BstNode<KEY, VALUE>* InsertNode(const KEY&, const DataStorePtr<VALUE>&, BstNode<KEY, VALUE>*, int&);

    virtual void Insert(const KEY&, const DataStorePtr<VALUE>&);

    ostream& DumpNodeHelper(BstNode<KEY, VALUE>* Node_, ostream& Os_,
                            size_t Level_, char* Branch_,
                            void(*travfnc)(BstNode<KEY, VALUE>*, ostream&)) const;

  public:
    Bst();
    Bst(const Bst<KEY, VALUE>& tree);
    virtual ~Bst();

    // Set comparison function method
    static void SetComparisonFunction(int(*CmpFnc_)(const KEY&, const KEY&));

    // object cloning method
    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual Bst<KEY, VALUE>* CloneBst() const;

    // Static null object accessor methods
    static Bst<KEY, VALUE>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth () const;
    virtual long DataSize() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::BST_TYPE; }
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

    void Clear();

    Bst<KEY, VALUE>& operator = (const Bst<KEY, VALUE>& tree);
    
    size_t Size() const;
    Boolean IsFound() const
        { return _Found; }

    virtual void Insert(const KEY& insertkey, const VALUE& insertvalue);
    virtual void Insert(const KEY& inskey_, VALUE* insvalptr_, int StoreAttrib_=DataStorageAttributes::ACTIVE);
    
    virtual void Del(const KEY&);

    virtual int HasNode(const KEY&, BstNode<KEY, VALUE>*);
    virtual VALUE* SearchNode(const KEY&, BstNode<KEY, VALUE>*);

    void PreOrder(BstNode<KEY, VALUE>* tnode, ostream& Os_, void(*travfnc)(BstNode<KEY, VALUE>*, ostream&));
    void InOrder(BstNode<KEY, VALUE>* tnode, ostream& Os_, void(*travfnc)(BstNode<KEY, VALUE>*, ostream&));
    void PostOrder(BstNode<KEY, VALUE>* tnode, ostream& Os_, void(*travfnc)(BstNode<KEY, VALUE>*, ostream&));

    size_t CalcHeight(BstNode<KEY, VALUE>* node) const;
    size_t CalcSize(BstNode<KEY, VALUE>* node) const;

    VALUE* GetSmallest(BstNode<KEY, VALUE>* tree);
    VALUE* GetLargest(BstNode<KEY, VALUE>* tree);

    BstNode<KEY, VALUE>* Root();

    ostream& DumpNode(BstNode<KEY, VALUE>* Node_, ostream& Os_) const;
    ostream& DumpNode(BstNode<KEY, VALUE>* Node_, ostream& Os_,
                      void(*travfnc)(BstNode<KEY, VALUE>*, ostream&)) const;

    BstNode<KEY, VALUE>** TreeToNodeArray() const;
    void NodeArrayToTree(BstNode<KEY, VALUE>** Array_, long Len_);    
};

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
class TreeWalker
{
  public:
    typedef void(*WalkFuncType)(BstNode<KEY, VALUE>*);
    
  protected:
    enum
    {
      NOTVISITED        = 1,
      LEFT_NOTVISITED   = 2,
      RIGHT_NOTVISITED  = 4
    };
  
    Bst<KEY, VALUE>* _Tree;
    BstNode<KEY, VALUE>* _Here;
    BstNode<KEY, VALUE>* _This;
    TreeWalker<KEY, VALUE>* _Left;
    TreeWalker<KEY, VALUE>* _Right;
    WalkFuncType _WalkFunc;
    int _Status;    

    BstNode<KEY, VALUE>* Visit();
    virtual void SetBranches(Bst<KEY, VALUE>* TreePtr_) = 0;
    virtual void Clear();

    TreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*));

  public:
    TreeWalker(Bst<KEY, VALUE>* TreePtr_);
    TreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*));
    TreeWalker(const TreeWalker<KEY, VALUE>& Obj_);
    virtual ~TreeWalker(){};
  
    void SetTreePtr(Bst<KEY, VALUE>* TreePtr_)
        { _Tree = TreePtr_; }
    void ApplyFunction(void(*travfnc)(BstNode<KEY, VALUE>*, ostream&))
        { _WalkFunc = travfnc; }

    virtual BstNode<KEY, VALUE>* Next() = 0;

    BstNode<KEY, VALUE>* operator ++ ()
        { return Next(); }
    BstNode<KEY, VALUE>* operator ++ (int);

    Bst<KEY, VALUE>* Tree()
	{ return _Tree; }
    BstNode<KEY, VALUE>* Here()
        { return _Here; }

    operator BstNode<KEY, VALUE>* ()
	{ return Here(); }

    TreeWalker<KEY, VALUE>& Reset();
};

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
class PreOrderTreeWalker : public TreeWalker<KEY, VALUE>
{
  protected:
    virtual void SetBranches(Bst<KEY, VALUE>* TreePtr_);
    PreOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*));

  public:
    PreOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_);
    PreOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*));
    PreOrderTreeWalker(const PreOrderTreeWalker<KEY, VALUE>& Obj_);
    PreOrderTreeWalker(const TreeWalker<KEY, VALUE>& Obj_);
    virtual ~PreOrderTreeWalker();

    virtual BstNode<KEY, VALUE>* Next();
};

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
class InOrderTreeWalker : public TreeWalker<KEY, VALUE>
{
  protected:
    virtual void SetBranches(Bst<KEY, VALUE>* TreePtr_);
    InOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*));

  public:
    InOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_);
    InOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*));
    InOrderTreeWalker(const InOrderTreeWalker<KEY, VALUE>& Obj_);
    InOrderTreeWalker(const TreeWalker<KEY, VALUE>& Obj_);
    virtual ~InOrderTreeWalker();

    virtual BstNode<KEY, VALUE>* Next();
};

//-------------------------------------------------------------------------
template <class KEY, class VALUE>
class PostOrderTreeWalker : public TreeWalker<KEY, VALUE>
{
  protected:
    virtual void SetBranches(Bst<KEY, VALUE>* TreePtr_);
    PostOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, BstNode<KEY, VALUE>* TreeNode_, void(*travfnc)(BstNode<KEY, VALUE>*));

  public:
    PostOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_);
    PostOrderTreeWalker(Bst<KEY, VALUE>* TreePtr_, void(*travfnc)(BstNode<KEY, VALUE>*));
    PostOrderTreeWalker(const PostOrderTreeWalker<KEY, VALUE>& Obj_);
    PostOrderTreeWalker(const TreeWalker<KEY, VALUE>& Obj_);
    virtual ~PostOrderTreeWalker();

    virtual BstNode<KEY, VALUE>* Next();
};

//-------------------------------------------------------------------------
#endif




