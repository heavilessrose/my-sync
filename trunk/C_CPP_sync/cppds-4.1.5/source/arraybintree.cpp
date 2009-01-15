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
//             + visit the tree nodes.                             //
//                                                                 //
//                                                                 //
/////////////////////////////////////////////////////////////////////
#ifndef ARRAYBINTREE_H
  #include "arraybintree.h"
#endif

////////////////////////////////////////////////////////////////////
//
template<class T>
ArrayBinTree<T>::ArrayBinTree()
{
    unsigned arraySize = 0;
    T emptyElement = T();
    unsigned i;

    arrSize = (arraySize < MIN_ARRAY_SIZE) ? MIN_ARRAY_SIZE :
                                             arraySize;
    nodes = new ArBinstruct<T>[arrSize];
    numNodes = 0;

    if (!nodes)
      xAlloc();

    errMsg[0] = '\0';
    numNodes = 0;
    root = NIL;
    // initialize the right pointers to set the chain
    // of vacant elements
    for (i = 0; i < (arrSize - 1); i++) {
      nodes[i].rightPtr = i + 1;
    }
    nodes[arrSize - 1].rightPtr = NIL;
    first = 0; // first available vacant element
    last = arrSize - 1; // last available vacant element
    emptyElem = emptyElement;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
ArrayBinTree<T>::ArrayBinTree(unsigned arraySize,
                              T emptyElement)
//
// Purpose: initializes the instances of an binary tree.
//
// Parameters:
//
//     input: arraySize - the maximum number of nodes in the binary
//             tree
//            emptyElement - an instance of the template class T
//             that represents an empty or null element.
//
{
    unsigned i;

    arrSize = (arraySize < MIN_ARRAY_SIZE) ? MIN_ARRAY_SIZE :
                                             arraySize;
    nodes = new ArBinstruct<T>[arrSize];
    numNodes = 0;

    if (!nodes)
      xAlloc();

    errMsg[0] = '\0';
    numNodes = 0;
    root = NIL;
    // initialize the right pointers to set the chain
    // of vacant elements
    for (i = 0; i < (arrSize - 1); i++) {
      nodes[i].rightPtr = i + 1;
    }
    nodes[arrSize - 1].rightPtr = NIL;
    first = 0; // first available vacant element
    last = arrSize - 1; // last available vacant element
    emptyElem = emptyElement;
}

//////////////////////////////////////////////////////////////////////
template <class T>
Boolean ArrayBinTree<T>::showTree(const ArrayBinTree<T>& t, ostream& os_) const
{
  T str;
  Boolean ok = ((ArrayBinTree<T>&)t).getFirstNode(str);

  if (!ok)
    return FALSE;

  cout <<"\n\n";
  while (ok)
  {
    os_ <<str <<"\n";
    ok = ((ArrayBinTree<T>&)t).getNextNode(str);
  }

  return TRUE;
}

//////////////////////////////////////////////////////////////////////
template <class T>
Object* ArrayBinTree<T>::Clone(ObjectAcceptor* ObjComp_) const
{
  if (ObjComp_ == NULL)
    return (new ArrayBinTree<T>(*this));

  return NULL;
}

//////////////////////////////////////////////////////////////////////
template <class T>    
ArrayBinTree<T>* ArrayBinTree<T>::CloneArrayBinTree() const
{
  return (new ArrayBinTree<T>(*this));
}

//////////////////////////////////////////////////////////////////////
template <class T>
ArrayBinTree<T>& ArrayBinTree<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static ArrayBinTree<T> _NULLOBJECT;
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//////////////////////////////////////////////////////////////////////
template <class T>    
const Object& ArrayBinTree<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ArrayBinTree<T>::GiveNullObject();
}

//////////////////////////////////////////////////////////////////////
template <class T>    
Object& ArrayBinTree<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return ArrayBinTree<T>::GiveNullObject();
}

//////////////////////////////////////////////////////////////////////
template <class T>
int ArrayBinTree<T>::ClassID() const
{
  return TypeInfo::ARRAYBINTREE_TYPE;
}

//////////////////////////////////////////////////////////////////////
template <class T>
ostream& ArrayBinTree<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
{
  Boolean Dum_;
  if (!Ok_)
    Ok_ = &Dum_;

  *Ok_ = showTree(*this, Os_);
  return Os_;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
char* ArrayBinTree<T>::getErrorMessage()
//
// Purpose: returns the contents of the errMsg data member
// and assigns an empty string to that member.
//
//
{
    string80 s;

    strcpy(s, errMsg);
    strcpy(errMsg, "");
    return s;
};

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean ArrayBinTree<T>::insert(T& x)
//
// Purpose: performs node insertion in an binary tree.
//
//  Parameters:
//
//    input: x - the inserted data.
//
{
  unsigned index = root, nextPtr;
  Boolean notFound = TRUE;

  // is tree full?
  if (numNodes == arrSize)
    return FALSE;

  // insert first element
  if (root == NIL) {
    // initialize data for inserting the first node
    root = 0;
    notFound = FALSE;
    index = 0;
    first = 1;
  }

  // start searching for the insertion location
  while (notFound) {
    if (nodes[index].dataNode < x) {
      if (nodes[index].rightPtr != NIL)
        index = nodes[index].rightPtr;
      else {
        nextPtr = nodes[first].rightPtr;
        nodes[index].rightPtr = first;
        index = first;
        first = nextPtr;
        notFound = FALSE;
      }
    }
    else {
      if (nodes[index].leftPtr != NIL)
        index = nodes[index].leftPtr;
      else {
        nextPtr = nodes[first].rightPtr;
        nodes[index].leftPtr = first;
        index = first;
        first = nextPtr;
        notFound = FALSE;
      }
    }
  }
  nodes[index].dataNode = x;
  nodes[index].rightPtr = NIL;
  nodes[index].leftPtr = NIL;
  numNodes++;
  return TRUE;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean ArrayBinTree<T>::search(T& x)
//
// Purpose: searches for a specific key in the binary tree.  The
// function returns a TRUE if item x is in the tree.  Otherwise,
// the function yields FALSE if no match was found.
//
//  Parameters:
//
//   input: x - the search key data.
//
{
  Boolean found = FALSE;
  unsigned index = root;

  if (numNodes == 0)
    return FALSE;

  while (index != NIL && !found)
  {
    if (index != NIL && nodes[index].dataNode < x)
      index = nodes[index].rightPtr;
    if (index != NIL && nodes[index].dataNode > x)
      index = nodes[index].leftPtr;
    if (index != NIL && nodes[index].dataNode == x)
      found = TRUE;
   }

   if (found)
     x = nodes[index].dataNode;
   return found;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean ArrayBinTree<T>::remove(T& x)
//
//
// Purpose: performs node deletion.
//
//  Parameters:
//
//    input: x - the data to be removed
//
{
   unsigned index = root;
   unsigned lastIndex = NIL;
   unsigned direction = NIL;
   unsigned nextPtr;
   Boolean found = FALSE;

   // does tree have one node?
   if (numNodes == 0)
     return FALSE;

   while (index != NIL && !found) {
     if (nodes[index].dataNode == x)
       found = TRUE;
     else {
       if (nodes[index].dataNode < x) {
         lastIndex = index;
         direction = RIGHT;
         index = nodes[index].rightPtr;
       }
       else {
         lastIndex = index;
         direction = LEFT;
         index = nodes[index].leftPtr;
       }
     }
   }

   if (!found)
     return FALSE;

   // delete matching item
   nodes[index].dataNode = emptyElem;
   // reconnect left subtree?
   if (nodes[index].rightPtr == NIL) {
     if (lastIndex != NIL) { // delete non-root node
       if (direction == LEFT)
         nodes[lastIndex].leftPtr = nodes[index].leftPtr;
       else
         nodes[lastIndex].rightPtr = nodes[index].leftPtr;
     }
     else // delete the root
       root = nodes[index].leftPtr;
     nodes[index].leftPtr = NIL;
   }
   // reconnect right sub-tree?
   else if (nodes[index].leftPtr == NIL) {
     if (lastIndex != NIL) { // delete non-root node
       if (direction == LEFT)
         nodes[lastIndex].leftPtr = nodes[index].rightPtr;
       else
         nodes[lastIndex].rightPtr = nodes[index].rightPtr;
     }
     else // delete the root
       root = nodes[index].rightPtr;
     nodes[index].rightPtr = NIL;
   }
   else {
     nextPtr = nodes[index].rightPtr;
     while (nodes[nextPtr].leftPtr != NIL)
       nextPtr = nodes[nextPtr].leftPtr;
     nodes[nextPtr].leftPtr = nodes[index].leftPtr;
     if (lastIndex != NIL) { // delete non-root node
       if (direction == LEFT)
         nodes[lastIndex].leftPtr = nodes[index].rightPtr;
       else
         nodes[lastIndex].rightPtr = nodes[index].rightPtr;
     }
     else // delete the root
       root = nodes[index].rightPtr;
     nodes[index].leftPtr = NIL;
     nodes[index].rightPtr = NIL;
   }
   nodes[last].rightPtr = index;
   last = index;
   numNodes--;
   if (numNodes == 0) {
     first = 0;
     root = NIL;
   }
   return TRUE;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void ArrayBinTree<T>::clearMarks()
//
// Purpose: clears the marked members of each binary tree node.
//
{
    for (unsigned i = 0; i < arrSize; i++)
      nodes[i].marked = FALSE;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void ArrayBinTree<T>::visitTree(unsigned rootPtr,
                                unsigned &node)
//
// Purpose: recursive function used to traverse the binary tree.
//
//  Parameters:
//
//    input: rootPtr - index to the root of the binary tree.
//
//    output: node - the index to the next node visited.
//
{
    if (seekNode && rootPtr != NIL) {
        visitTree(nodes[rootPtr].leftPtr, node);
        if (seekNode && !nodes[rootPtr].marked) {
            seekNode = FALSE;
            nodes[rootPtr].marked = TRUE;
            node = rootPtr;
            return;
        }
        visitTree(nodes[rootPtr].rightPtr, node);
    }
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void ArrayBinTree<T>::clear()
//
// Purpose: clears the binary tree.
//
// Parameters:
//
//     input: firstItem - the first item inserted in the binary tree
//
{
    unsigned i;

   for (i = 0; i < arrSize; i++)
   {
      nodes[i].marked = FALSE;
      nodes[i].dataNode = emptyElem;
   }
   root = NIL;
   numNodes = 0;
   first = 0;
   errMsg[0] = '\0';
   // initialize the right pointers to set the chain
   // of vacant elements
   for (i = 1; i < (arrSize - 1); i++)
   {
     nodes[i].rightPtr = i + 1;
   }
   nodes[arrSize - 1].rightPtr = NIL;
   last = arrSize - 1; // last available vacant element
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean ArrayBinTree<T>::getFirstNode(T& x)
//
// Purpose: visits the root of the binary tree.  The function
// returns TRUE if the binary tree object has a root.
//
//  Parameters:
//
//   input: x - the pointer to the data associated with the root of
//           the binary tree.
{
    if (numNodes > 0) {
        seekNode = TRUE;
        clearMarks();
        visitTree(root, nodePtr);
        x = nodes[nodePtr].dataNode;
    }
    return (numNodes > 0) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean ArrayBinTree<T>::getNextNode(T& x)
//
// Purpose: visits the next node in the binary tree.  The function
// returns TRUE if there was a next node to visit.
//
//  Parameters:
//
//    input: x - the pointer to the data associated with the visited
//             node.
//
{
    seekNode = TRUE;
    nodePtr = NIL;
    if (numNodes > 0) {
        visitTree(root, nodePtr);
        if (!seekNode)
            x = nodes[nodePtr].dataNode;
    }
    return (!seekNode) ? TRUE : FALSE;
}

///////////////////////////////////////////////////////////////////
//
template<class T>
Boolean ArrayBinTree<T>::probeLeftChild(T& x)
//
// Purpose: visits the left child of the current node.  The function
// returns TRUE if the current node has a left child.
//
//  Parameters:
//
//    input: x - the data associated with the left child node.
{
    unsigned n;

    if (nodePtr != NIL && nodes[nodePtr].leftPtr != NIL) {
        n = nodes[nodePtr].leftPtr;
        x = nodes[n].dataNode;
        return TRUE;
    }
    else
        return FALSE;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean ArrayBinTree<T>::probeRightChild(T& x)
//
// Purpose: visits the right child of the current node.  The
// function returns TRUE if the current node has right child.
//
//  Parameters:
//
//   input: x - the data associated with the right child node.
//
{
    unsigned n;

    if (nodePtr != NIL && nodes[nodePtr].rightPtr != NIL) {
        n = nodes[nodePtr].rightPtr;
        x = nodes[n].dataNode;
        return TRUE;
    }
    else
        return FALSE;
}




