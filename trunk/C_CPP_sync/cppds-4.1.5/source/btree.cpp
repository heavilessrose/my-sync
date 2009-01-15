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
//             + visits the nodes of the B-tree.                   //
//                                                                 //
//                                                                 //
/////////////////////////////////////////////////////////////////////
#ifndef BTREE_H
  #include "btree.h"
#endif

////////////////////////////////////////////////////////////////////
//
template<class T>
Btree<T>::Btree(const char* filename)
//
// Purpose: initializes the instances of a B-tree.  The
// constructor allocates dynamic memory for the array that tracks
// the deleted nodes.
//
// Parameters:
//
//     input: filename - the name of the supporting file.
//
{
    root = BTREE_NIL;
    numData = 0;
    numNodes = 0;
    numDelNodes = 0;
    // create the array that tracks the deleted nodes
    countDelNodeArray = BTREE_MIN_DEL_ARRAY;
    delNodeArray = new unsigned[countDelNodeArray];
    
    if (!delNodeArray)
      xAlloc();
    
    // assign BTREE_NIL to the elements of the array delNodeArray
    for (unsigned i = 0; i < countDelNodeArray; i++)
      *(delNodeArray + i) = BTREE_NIL;
    errMsg[0] = '\0';
    nodeSize = sizeof(Bstruct<T>);
    // open the stream for binary I/O
    f.open(filename, ios::binary | ios::in | ios::out);
    if (!f.good())
      xFileOpen();
}

//////////////////////////////////////////////////////////////////////
template <class T>
Boolean Btree<T>::showTree(const Btree<T>& t, ostream& os_) const
{
  T str;
  Boolean ok = ((Btree<T>&)t).getFirstNode(str);

  if (!ok)
    return FALSE;

  os_ << " (tree has " << ((Btree<T>&)t).getNumNodes() << " nodes)"
      << "\n\n";
      
  while (ok)
  {
    cout <<str <<"\n";
    ok = ((Btree<T>&)t).getNextNode(str);
  }

  return TRUE;
}

//////////////////////////////////////////////////////////////////////
template <class T>
Btree<T>& Btree<T>::GiveNullObject()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  static Btree<T> _NULLOBJECT("temp.btr");
  _NULLOBJECT.SetAsNullObject();
  return _NULLOBJECT;
}

//////////////////////////////////////////////////////////////////////
template <class T>    
const Object& Btree<T>::NULLOBJECT() const
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Btree<T>::GiveNullObject();
}

//////////////////////////////////////////////////////////////////////
template <class T>    
Object& Btree<T>::NULLOBJECT()
{
  // The NULLOBJECT method must be overwritten in the derived classes to
  // actually return an instance of the respective class with the null byte
  // set to null otherwise mismatched class objects will be returned.
  return Btree<T>::GiveNullObject();
}

//////////////////////////////////////////////////////////////////////
template <class T>
int Btree<T>::ClassID() const
{
  return TypeInfo::BTREE_TYPE;
}

//////////////////////////////////////////////////////////////////////
template <class T>
ostream& Btree<T>::TextWrite(ostream& Os_, Boolean* Ok_) const
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
char* Btree<T>::getErrorMessage()
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
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::writeNode(Bstruct<T> *node, unsigned nodeNum)
//
// Purpose: writes a node to the stream.
//
// Parameters:
//
//    input: node - the node written to the stream.
//           nodeNum - the node index
//
{
  f.seekg(nodeNum * nodeSize);
  // write node in one swoop
  f.write((char*) node, nodeSize);
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::readNode(Bstruct<T> *node, unsigned nodeNum)
//
// Purpose: reads a node from the stream.
//
// Parameters:
//
//    input: node - the node read from the stream.
//           nodeNum - the node index
//
{
  f.seekg(nodeNum * nodeSize);
  // read the node in one swoop
  f.read((char*) node, nodeSize);
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::expandDelNodeArray()
//
// Purpose: expands the size of the delNodeArray.
//
{
    unsigned *newArray;
    unsigned newSize = countDelNodeArray + BTREE_DEL_ARRAY_INCR;

    // create new array
    newArray = new unsigned[newSize];
    // copy values from the old array
    for (unsigned i = 0; i < countDelNodeArray; i++)
      *(newArray + i) = *(delNodeArray + i);
    // delete the old array
    delete [] delNodeArray;
    // assign delNodeArray pointer to access the new array
    delNodeArray = newArray;
    // update the current size of the delNodeArray
    countDelNodeArray = newSize;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
unsigned Btree<T>::getNodeIndex()
//
// Purpose: returns the index for the next node.
//
{
  // are there no deleted node?
  if (numDelNodes == 0) {
    highNodeIndex = (numNodes > 0) ? highNodeIndex + 1 : 0;
    return highNodeIndex;
  }
  else
    // get node index from the delNodeArray
    return *(delNodeArray + numDelNodes-- - 1);
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::insert(T& x)
//
// Purpose: performs node insertion in a B-tree.
//
//  Parameters:
//
//    input: x - the inserted data
//
{
  T r;  // data for the node that is inserted as the new root
  unsigned p, rRST; // the right subtree of 'r'
  Bstruct<T> *buf;

  // if item x is in the tree exit
  if (search(x))
    return FALSE;

  // did the tree grow in height?
  if (pushDown(x, root, r, rRST)) {
     // get the index of a new node
     p = getNodeIndex();
     // make new root
     buf = new Bstruct<T>;
     numNodes++;
     buf->count = 1;
     buf->data[1] = r;
     buf->nodeLink[0] = root;
     buf->nodeLink[1] = rRST;
     for (unsigned i = 2; i <= BTREE_MAX; i++)
       buf->nodeLink[i] = BTREE_NIL;
     root = p;
     // save the new node
     writeNode(buf, p);
     delete buf;
  }
  numData++;
  return TRUE;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::search(T& x)
//
// Purpose: searches for a specific data in the B-tree.
// The function returns TRUE if item x is in the tree.  Otherwise,
// the function yields FALSE.
//
//  Parameters:
//
//   input: x - the search data data
//
{
  unsigned dummyIndex;
  unsigned dummyPtr;

  return searchBtree(x, root, dummyPtr, dummyIndex);
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::searchBtree(T& x,
                              unsigned rootPtr,
                              unsigned &nodePtr,
                              unsigned &pos)
//
// Purpose: recursive function that searches for a specific data
// in the B-tree.  The function returns TRUE if item x
// is in the tree.  Otherwise, the function yields FALSE.
//
//  Parameters:
//
//   input: x - the search
//          rootPtr - the index to the root of a subtree
//          nodePtr - the index to the node that contains item x
//          pos - the index of the matching element
//
{
  Boolean iterate = TRUE;
  Boolean found = FALSE;
  Bstruct<T> *buf = new Bstruct<T>;

  while (iterate) {
    if (rootPtr == BTREE_NIL)
      iterate = FALSE;
    else {
      readNode(buf, rootPtr);
      found = searchNode(x, buf, pos);
      if (found) {
        nodePtr = rootPtr;
        iterate = FALSE;
      }
      else
        rootPtr = buf->nodeLink[pos];
    }
  }
  delete buf;
  return found;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::searchNode(T& x,
                             Bstruct<T> *ptr,
                             unsigned &pos)
//
// Purpose: searches a node for an element that matches item x.  The
// function returns TRUE if x is found in the searched node.
// Otherwise, the function yields FALSE.
//
//  Parameters:
//
//   input: x - the search data data
//          ptr - the pointer to the searched node
//
//   output:
//          pos - the index of the node element that matches item x
//
// Comment:  the value returned by the reference parameter pos is
// signficant only if the function result is TRUE.
//
{
    // is item x in the node at all?
    if (x < ptr->data[1]) {
      pos = 0;
      return FALSE;
    }
    else {
      // begin the serial search in the elements of the node
      pos = ptr->count;
      while (x < ptr->data[pos] && pos > 1)
        pos--;
      // return the function's result
      return (x == ptr->data[pos]) ? TRUE : FALSE;
    }
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::pushDown(T& x,
                           unsigned ptr,
                           T& item,
                           unsigned &itemRST)
//
// Purpose:  recursive function that inserts item x in the B-tree.
// The function returns TRUE if a the height of the B-tree needs
// to be increased.  Otherwise, the function yields FALSE.
//
// Parameters:
//
//    input: x - the inserted data
//           ptr - the index to the manipulated node
//
//    output:
//           item - the median element
//           itemRST - the index to the right subtree of the node
//             which contains item
//
{
  unsigned i;
  Bstruct<T> *buf;

  if (ptr == BTREE_NIL) {
    // cannot insert into an empty tree
    item = x;
    itemRST = BTREE_NIL;
    return TRUE;
  }
  else {
    buf = new Bstruct<T>;
    readNode(buf, ptr);
    // attempt to isnert a duplicate key in the current node?
    if (searchNode(x, buf, i)) {
      strcpy(errMsg, "Cannot insert duplicates");
      return FALSE;
    }
    // reinsert the median element
    if (pushDown(x, buf->nodeLink[i], item, itemRST)) {
      if (buf->count < BTREE_MAX) {
        pushIn(item, itemRST, buf, i);
        writeNode(buf, ptr);
        delete buf;
        return FALSE;
      }
      else {
        cutNode(item, itemRST, ptr, i, item, itemRST);
        delete buf;
        return TRUE;
      }
    }
    else {
      delete buf;
      return FALSE;
   }
  }
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::pushIn(T& x,
                      unsigned xRST,
                      Bstruct<T> *ptr,
                      unsigned pos)
//
// Purpose: inserts the item x and the index xRST in the node
// accessed by index ptr and in the position pos.
//
// Parameters:
//
//    input: x - the inserted data
//           xRST - the inserted node index
//           ptr - the index to the manipulated node
//           pos - the index of the inserted element in the
//             manipulated node
//
{
    // move the elements and links in the manipulated node
    for (unsigned i = ptr->count; i >= (pos + 1); i--) {
      ptr->data[i+1] = ptr->data[i];
      ptr->nodeLink[i+1] = ptr->nodeLink[i];
    }
    // insert item x
    ptr->data[pos+1] = x;
    // insert index xRST
    ptr->nodeLink[pos+1] = xRST;
    // increment count of node elements
    ptr->count++;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::cutNode(T& x,
                      unsigned xRST,
                      unsigned ptr,
                      unsigned pos,
                      T& y,
                      unsigned &yRST)
//
// Purpose: divides the node accessed by index ptr that contains
// item x and index xRST at index pos.  The new nodes are accessed
// by pointers ptr and yRST.  The median element is y.
//
// Parameters:
//
//    input: x - the inserted data
//           xRST - the inserted index associated with item x
//           ptr - the index to the manipulated node
//           pos - the index of the dividing line
//
//    output:
//           y - the new median
//           yRST - the index to the other node.
//
{
   unsigned median, i;
   Bstruct<T> *buf1, *buf2;

   buf1 = new Bstruct<T>;
   buf2 = new Bstruct<T>;
   readNode(buf1, ptr);
   // calculate the median element which also determines if
   // the new inserted item x is placed in the new left or the
   // new right nodes
   median = (pos <= BTREE_MIN) ? BTREE_MIN : BTREE_MIN + 1;
   // create a new tree node and put it on the right
   yRST = getNodeIndex();
   for (i = 0; i <= BTREE_MAX; i++)
     buf2->nodeLink[i] = BTREE_NIL;
   numNodes++;

   // loop to move half of the keys
   for (i = median + 1; i <= BTREE_MAX; i++) {
     buf2->data[i - median] = buf1->data[i];
     buf2->nodeLink[i - median] = buf1->nodeLink[i];
   }
   buf2->count = BTREE_MAX - median;
   buf1->count = median;
   // push in the new data
   if (pos <= BTREE_MIN)
     pushIn(x, xRST, buf1, pos);
   else
     pushIn(x, xRST, buf2, pos - median);
   y = buf1->data[buf1->count];
   buf2->nodeLink[0] = buf1->nodeLink[buf1->count--];
   writeNode(buf1, ptr);
   writeNode(buf2, yRST);
   delete buf1;
   delete buf2;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::remove(T& x)
//
//
// Purpose: performs node deletion.  The function returns TRUE if
// item x was deleted from the B-tree.
//
//  Parameters:
//
//    input: x - the data to be removed
//
{
    Bstruct<T> *buf = new Bstruct<T>;
    unsigned p;
    Boolean found;

     recDelete(x, root, found);
    if (found) {
      numData--; // decrement data counter
      readNode(buf, root);
      // is the node empty?
      if (buf->count == 0) {
        p = root;
        root = buf->nodeLink[0];
        // delete node at index p
        numDelNodes++; // increment the counter for the number
                       // of deleted nodes
        // does the array delNodeArray need to expand?
        if (numDelNodes == countDelNodeArray)
          expandDelNodeArray();
        // assign index q to an element of array delNodeArray
        *(delNodeArray + numDelNodes - 1) = p;
        // decrement the node counter
        numNodes--;
      }
    }
    delete buf;
    return found;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::recDelete(T& x,
                         unsigned ptr,
                         Boolean& found)
//
//
// Purpose: function that performs node deletion.
//
//  Parameters:
//
//    input: x - the data to be removed
//           ptr - the
//
//    output:
//           found - flag that indicates whether item x was found
//
{
    Bstruct<T> *buf, *buf2;
    unsigned i;

    if (ptr == BTREE_NIL)
       found = FALSE;
    else {
      buf = new Bstruct<T>;
      readNode(buf, ptr);
      // search for x in the current node
      found = (searchNode(x, buf, i)) ? TRUE : FALSE;
      // found an element that matches item x?
      if (found) {
        // does ptr point to a leaf node?
        if (buf->nodeLink[i - 1] == BTREE_NIL) {
          // remove element at index i
          trim(buf, i);
          writeNode(buf, ptr);
        }
        else {
          // replace data[i] with its successor in node ptr
          successor(buf, i);
          writeNode(buf, ptr);
          recDelete(buf->data[i], buf->nodeLink[i], found);
        }
      }
      else
        recDelete(x, buf->nodeLink[i], found);

      if (buf->nodeLink[i] != BTREE_NIL) {
        buf2 = new Bstruct<T>;
        readNode(buf2, buf->nodeLink[i]);
        if (buf2->count < BTREE_MIN) {
          restore(buf, i);
          writeNode(buf, ptr);
        }
        delete buf2;
      }
      delete buf;
    }
}


////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::trim(Bstruct<T> *ptr, unsigned pos)
//
//
// Purpose: removes the element at index pos from the node accessed
// by index ptr.
//
//  Parameters:
//
//    input: ptr - the pointer of the trimmed node
//           pos - the index of the trimmed element
//
{
    for (unsigned i = pos + 1; i <= ptr->count; i++) {
      ptr->data[i - 1] = ptr->data[i];
      ptr->nodeLink[i - 1] = ptr->nodeLink[i];
    }
    // decrement node counter
    ptr->count--;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::successor(Bstruct<T> *ptr, unsigned pos)
//
//
// Purpose: replaces ptr->data[pos] by its immediate successor.
//
//  Parameters:
//
//    input: ptr - the pointer to the node with the replaced data
//           pos - the index of the replaced data
//
{
   Bstruct<T> *bufP;
   unsigned p, n;

   bufP = new Bstruct<T>;
   p = ptr->nodeLink[pos];
   readNode(bufP, p);
   while (bufP->nodeLink[0] != BTREE_NIL) {
     n = bufP->nodeLink[0];
     readNode(bufP, n);
   }
   ptr->data[pos] = bufP->data[1];
   delete bufP;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::restore(Bstruct<T> *ptr, unsigned pos)
//
//
// Purpose: locates an item and inserts into nodeLink[pos] of
// node ptr in order to restore the minimum number of items in
// the targeted node.
//
//  Parameters:
//
//    input: ptr - the pointer to the manipulated node
//           pos - the index of the inserted element
//
{
  Bstruct<T> *childBuf, *childBuf2;

  childBuf = new Bstruct<T>;
  if (pos == 0) {
    readNode(childBuf, ptr->nodeLink[1]);
    // restore leftmost element in the current node?
    if (childBuf->count > BTREE_MIN)
      shiftLeft(ptr, 1);
    else
      compact(ptr, 1);
  }
  // restore the rightmost element in the current node?
  else if (pos == ptr->count) {
    readNode(childBuf, ptr->nodeLink[pos - 1]);
    if (childBuf->count > BTREE_MIN)
      shiftRight(ptr, pos);
    else
      compact(ptr, pos);
  }
  // restore other internal elements in the current node
  else {
    childBuf2 = new Bstruct<T>;
    readNode(childBuf, ptr->nodeLink[pos - 1]);
    readNode(childBuf2, ptr->nodeLink[pos + 1]);
    if (childBuf->count > BTREE_MIN)
      shiftRight(ptr, pos);
    else if (childBuf2->count > BTREE_MIN)
      shiftLeft(ptr, pos + 1);
    else
      compact(ptr, pos);
    delete childBuf2;
  }
  delete childBuf;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::shiftRight(Bstruct<T> *ptr, unsigned pos)
//
//
// Purpose: moves a data element to the right in a node.
//
//  Parameters:
//
//    input: ptr - the pointer to the manipulated node
//           pos - the index of the shifted data element
//
{
    Bstruct<T> *bufP = new Bstruct<T>;
    Bstruct<T> *bufQ = new Bstruct<T>;
    unsigned p, q;

    p = ptr->nodeLink[pos];
    readNode(bufP, p);
    // move all the data elements to the right by one position
    for (unsigned i = bufP->count; i > 0; i--) {
      bufP->data[i + 1] = bufP->data[i];
      bufP->nodeLink[i + 1] = bufP->nodeLink[i];
    }
    // shift the data element from parent node and into right node
    bufP->nodeLink[1] = bufP->nodeLink[0];
    bufP->count++;
    bufP->data[1] = ptr->data[pos];
    writeNode(bufP, p);

    // move the last data of the left node into the parent node
    p = ptr->nodeLink[pos - 1];
    readNode(bufP, p);
    ptr->data[pos] = bufP->data[bufP->count];
    q = ptr->nodeLink[pos];
    readNode(bufQ, q);
    bufQ->nodeLink[0] = bufP->nodeLink[bufP->count];
    bufP->count--;
    // update nodes p and ptr in the stream
    writeNode(bufP, p);
    writeNode(bufQ, q);
    delete bufP;
    delete bufQ;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::shiftLeft(Bstruct<T> *ptr, unsigned pos)
//
//
// Purpose: moves a data element into the left in a node.
//
//  Parameters:
//
//    input: ptr - the index to the manipulated node
//           pos - the index of the shifted data element
//
{
    Bstruct<T> *bufP = new Bstruct<T>;
    Bstruct<T> *bufQ = new Bstruct<T>;
    unsigned p, q;

    // shift data element from parent node into the left node
    p = ptr->nodeLink[pos - 1];
    readNode(bufP, p);
    bufP->count++;
    bufP->data[bufP->count] = ptr->data[pos];
    q = ptr->nodeLink[pos];
    readNode(bufQ, q);
    bufP->nodeLink[bufP->count] = bufQ->nodeLink[0];
    writeNode(bufP, p);

    // shift the data element from the right node into
    // the parent node
    p = ptr->nodeLink[pos];
    readNode(bufP, p);
    ptr->data[pos] = bufP->data[1];
    bufP->nodeLink[0] = bufP->nodeLink[1];
    bufP->count--;
    // loop that shifts all of the data elements in the right node
    // by one position upward
    for (unsigned i = 1; i <= bufP->count; i++) {
      bufP->data[i] = bufP->data[i + 1];
      bufP->nodeLink[i] = bufP->nodeLink[i + 1];
    }
    // update nodes p and ptr in the stream
    writeNode(bufP, p);
    delete bufP;
    delete bufQ;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::compact(Bstruct<T> *ptr, unsigned pos)
//
// Purpose: combines adjacent nodes.
//
//  Parameters:
//
//    input: ptr - the pointer to the manipulated node
//           pos - the index of the merged right node
//
{
    Bstruct<T> *bufP = new Bstruct<T>;
    Bstruct<T> *bufQ = new Bstruct<T>;
    unsigned p, q, i;

    p = ptr->nodeLink[pos - 1];
    readNode(bufP, p);
    // q is the index to the right node that will be made vacant
    // and then deleted
    q = ptr->nodeLink[pos];
    readNode(bufQ, q);

    // process the left node and insert data elements from
    // the parent node
    bufP->count++;
    bufP->data[bufP->count] = ptr->data[pos];
    bufP->nodeLink[bufP->count] = bufQ->nodeLink[0];
    // inserts all data elements from the right node
    for (i = 1; i <= bufQ->count; i++) {
      bufP->count++;
      bufP->data[bufP->count] = bufQ->data[i];
      bufP->nodeLink[bufP->count] = bufQ->nodeLink[i];
    }

    // remove data elements from the parent node
    for (i = pos; i < ptr->count; i++) {
      ptr->data[i] = ptr->data[i + 1];
      ptr->nodeLink[i] = ptr->nodeLink[i + 1];
    }
    ptr->count--;
    // remove the empty right node
    numDelNodes++; // increment the counter for the number
                   // of deleted nodes
    // does the array delNodeArray need to expand?
    if (numDelNodes == countDelNodeArray)
      expandDelNodeArray();
    // assign index q to an element of array delNodeArray
    *(delNodeArray + numDelNodes - 1) = q;
    numNodes--;
    writeNode(bufP, p);
    delete bufQ;
    delete bufP;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::clearMarks(unsigned rootPtr)
//
// Purpose: function used to traverse the B-tree and set the
// marked member of each node to 0.
//
//  Parameters:
//
//    input: rootPtr - index to the root of the B-tree.
//
{
    Bstruct<T> *buf;

    if (rootPtr != BTREE_NIL) {
        buf = new Bstruct<T>;
        // write to all the pages sytematically
        for (unsigned i = 0; i <= highNodeIndex; i++) {
          readNode(buf, i);
          buf->marked = 0;
          // update data in the stream
          writeNode(buf, i);
        }
        delete buf;
    }
}

////////////////////////////////////////////////////////////////////
//
template<class T>
void Btree<T>::visitTree(unsigned rootPtr,
                         unsigned &node)
//
// Purpose: recursive function used to traverse the B-tree.
//
//  Parameters:
//
//    input: rootPtr - index to the root of the B-tree.
//
//    output: node - the index to the next node visited.
//
{
    Bstruct<T> *buf = new Bstruct<T>;
    unsigned n;

    readNode(buf, rootPtr);
    n = buf->marked;

    // terminate recursive traversal?
    if (n > buf->count) {
      delete buf;
      // all nodes have been visited
      return;
    }

    if (buf->nodeLink[n] != BTREE_NIL)
      visitTree(buf->nodeLink[n], node);

    if (seekNode && n < buf->count) {
      buf->marked++;
      node = rootPtr;
      visitIndex = buf->marked;
      seekNode = FALSE;
      writeNode(buf, rootPtr);
    }
    delete buf;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::getFirstNode(T& x)
//
// Purpose: visits the root of the B-tree.  The function
// returns TRUE if the B-tree object has a root.
//
//  Parameters:
//
//   input: x - the index to the data associated with the root of
//           the B-tree.
{
    Bstruct<T> *buf;

    if (root != BTREE_NIL) {
        seekNode = TRUE;
        clearMarks(root);
        visitTree(root, nodePtr);
        buf = new Bstruct<T>;
        readNode(buf, nodePtr);
        x = buf->data[visitIndex];
        delete buf;
    }
    return (root != BTREE_NIL) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////
//
template<class T>
Boolean Btree<T>::getNextNode(T& x)
//
// Purpose: visits the next node in the B-tree.  The function
// returns TRUE if there was a next node to visit.
//
//  Parameters:
//
//    input: x - the index to the data associated with the visited
//             node.
//
{
    Bstruct<T> *buf;

    seekNode = TRUE;
    if (root != BTREE_NIL) {
        visitTree(root, nodePtr);
        buf = new Bstruct<T>;
        readNode(buf, nodePtr);
        if (!seekNode)
            x = buf->data[visitIndex];
        delete buf;
    }
    return (!seekNode) ? TRUE : FALSE;
}




