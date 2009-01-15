/////////////////////////////////////////////////////////////////////
//                                                                 //
//             Copyright (c) 1992 Namir Clement Shammas            //
//                                                                 //
//  Version: 1.0.0                                 Date 8/12/92    //
//                                                                 //
//  Purpose: implements class of B-trees that have the             //
//           following functionality:                              //
//                                                                 //
//             + inserts data.                                     //
//             + searches for data.                                //
//             + deletes data.                                     //
//             + visits the nodes of the multi-way tree.           //
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

#ifndef INCL_FSTREAM_H
  #include <fstream.h>
  #define INCL_FSTREAM_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif

#ifndef BTREE_H
#define BTREE_H

const unsigned BTREE_NIL = 0xffff;
const unsigned BTREE_ORDER = 5;
const unsigned BTREE_MAX = BTREE_ORDER - 1;
const unsigned BTREE_MIN = BTREE_ORDER / 2;
const unsigned BTREE_MIN_DEL_ARRAY = 50;
const unsigned BTREE_DEL_ARRAY_INCR = 20;

template<class T>
struct Bstruct
{
    unsigned count;
    T data[BTREE_MAX+1];
    unsigned nodeLink[BTREE_MAX+1];
    unsigned marked; // visit flag
};

template<class T>
class Btree : public ObjectSkeleton
{

  protected:
    string80 errMsg;            // error message
    unsigned numData,           // number of data items in B-tree
             numNodes,          // number of nodes in B-tree
             nodeSize,          // size of each node
             visitIndex,        // visit index
             nodePtr,           // visit node index
             root,              // index to root page
             numDelNodes,       // number of deleted nodes
             countDelNodeArray, // number of elements in delNodeArray
             highNodeIndex;     // highest node index
    unsigned* delNodeArray;     // array of deleted node indices
    Boolean seekNode;           // search flag
    fstream f;                  // supporting stream

    void xFileOpen() const
        { THROW (FallibleBase::FileOpenErr()); }
    void xAlloc() const
	{ THROW (FallibleBase::OutOfMemoryErr()); }    

    Boolean searchBtree(T& x,
                        unsigned rootPtr,
                        unsigned &nodePtr,
                        unsigned &pos);
    Boolean searchNode(T& x,
                       Bstruct<T> *ptr,
                       unsigned &pos);
    Boolean pushDown(T& x, unsigned ptr,
                     T& item, unsigned &itemRST);
    void pushIn(T& x, unsigned xRST,
                Bstruct<T> *ptr, unsigned pos);
    void cutNode(T& x, unsigned xRST,
                 unsigned ptr, unsigned pos,
                 T& y, unsigned &yRST);
    void recDelete(T& x, unsigned ptr, Boolean& found);
    void trim(Bstruct<T> *ptr, unsigned pos);
    void successor(Bstruct<T> *ptr, unsigned pos);
    void restore(Bstruct<T> *ptr, unsigned pos);
    void shiftRight(Bstruct<T> *ptr, unsigned pos);
    void shiftLeft(Bstruct<T> *ptr, unsigned pos);
    void compact(Bstruct<T> *ptr, unsigned pos);

    void clearMarks(unsigned rootPtr);
    void visitTree(unsigned rootPtr, unsigned &node);

    void writeNode(Bstruct<T> *node, unsigned nodeNum);
    void readNode(Bstruct<T> *node, unsigned nodeNum);
    void expandDelNodeArray();
    unsigned getNodeIndex();

    Boolean showTree(const Btree<T>& t, ostream& os_) const;

  public:
    // ************** state manipulation methods **************
    Btree(const char* filename);
    ~Btree()
      { clear(); }

    // Static null object accessor methods
    static Btree<T>& GiveNullObject();

    // Null object accessors    
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::BTREE_TYPE; }
    virtual int ClassID() const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL)
	{ return Is_; }
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    char* getErrorMessage();
    unsigned getNumData() const
      { return numData; }
    unsigned getNumNodes() const
      { return numNodes; }
    void getPage(Bstruct<T> *node, unsigned pageNum)
      { readNode(node, pageNum); }

    // ************** object manipulation methods *************
    virtual Boolean insert(T& x);
    virtual Boolean search(T& x);
    virtual Boolean remove(T& x);
    virtual Boolean getFirstNode(T& x);
    virtual Boolean getNextNode(T& x);
    virtual void clear()
      {
        delete [] delNodeArray;
        f.close();
      }
};

#endif




