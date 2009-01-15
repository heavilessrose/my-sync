/////////////////////////////////////////////////////////////////////
//                                                                 //
//                 Copyright (c) 1992 Namir Clement Shammas        //
//                                                                 //
//  Version: 1.0.0                                  Date 8/9/92    //
//                                                                 //
//  Purpose: implements class of array-based binary trees that     //
//           have the following functionality:                     //
//                                                                 //
//             + insert data.                                      //
//             + search for data.                                  //
//             + delete data.                                      //
//             + nodes with duplicate keys are optional.           //
//                                                                 //
//                                                                 //
/////////////////////////////////////////////////////////////////////
#ifndef COMNDATA_H
  #include "comndata.h"
#endif
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif

#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif

#ifndef ARRAYBINTREE_H
#define ARRAYBINTREE_H

#define NIL 0xffff
#define LEFT 0
#define RIGHT 1
#define MIN_ARRAY_SIZE 20

template<class T>
struct ArBinstruct
{
    T              dataNode;  // node data
    unsigned       leftPtr,   // left child index
                   rightPtr;  // right child index
    Boolean        marked;    // visit flag
};

template<class T>
class ArrayBinTree : public ObjectSkeleton
{
  protected:
    string80   errMsg;       // error message
    unsigned   arrSize,
               numNodes,      // number of nodes
               root,          // index to root
               first,         // first available vacant member
               last,          // last available vacant member
               nodePtr;
    ArBinstruct<T> *nodes;
    T emptyElem;
    Boolean seekNode;

    void xAlloc() const
	{ THROW (FallibleBase::OutOfMemoryErr()); }
    void clearMarks();
    void visitTree(unsigned rootPtr, unsigned &node);
    Boolean showTree(const ArrayBinTree<T>& t, ostream& os_) const;
                   
  public:
    // ************** state manipulation methods **************
    ArrayBinTree();
    ArrayBinTree(unsigned arraySize,
                 T emptyElement);
    ~ArrayBinTree()
    { clear();
      delete [] nodes; }

    // object cloning method
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;
    virtual ArrayBinTree<T>* CloneArrayBinTree() const;

    // Static null object accessor methods
    static ArrayBinTree<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::ARRAYBINTREE_TYPE; }
    virtual int ClassID() const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL)
	{ return Is_; }
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    char* getErrorMessage();
    unsigned getNodeCount() const
    { return numNodes; }

    // ************** object manipulation methods *************
    virtual Boolean insert(T& x);
    virtual Boolean search(T& x);
    virtual Boolean remove(T& x);
    virtual Boolean getFirstNode(T& x);
    virtual Boolean getNextNode(T& x);
    virtual void clear();
    Boolean probeLeftChild(T& x);
    Boolean probeRightChild(T& x);
};

#endif





