/*****************************************************************************
*	       Sorting/Searching Procedures Library Header File		     *
*****************************************************************************/
#ifndef SORTS_H
#define SORTS_H
/**************************** Standard Libraries ****************************/
//  #ifndef INCL_ALLOC_H
//    #include <alloc.h>
//    #define INCL_ALLOC_H
//  #endif
#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERT_H
#endif
#ifndef INCL_FSTREAM_H
  #include <fstream.h>
  #define INCL_FSTREAM_H
#endif
#ifndef INCL_STDIO_H
  #include <stdio.h>
  #define INCL_STDIO_H
#endif
/**************************** Include Libraries *****************************/
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef GENFNCS2_H
  #include "genfncs2.h"
#endif
#ifndef RAND_H
  #include "rand.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif
/******************************** Constants *********************************/
/* message constants */
#define ERRMSG_INVALIDLIMITS	"ERROR: Invalid upper and lower array limits"
#define ERRMSG_NULLARRAY	"ERROR: Null array pointer"

#define ITEMSORTER_DEBUG                0
#define ITEMSORTER_OBJECT_DEBUG         0
#define ITEMSORTER_POINTER_DEBUG        0

/***************************** Type Definitions *****************************/
typedef long ArraySize;

/****************************************************************************/
// VT : vector type
// DOT : dereferenced object type

template <class VT, class DOT>
class SortingHelper
{
  public:
    virtual int Compare(const DOT&, const DOT&) = 0;
    virtual DOT& Dereference(VT*) = 0;
};

template <class VT, class DOT>
class ObjectSortHelper : public SortingHelper<VT, DOT>
{
  public:
    ObjectSortHelper(){}
    static ObjectSortHelper<VT, DOT>* Make()
        { return (new ObjectSortHelper<VT, DOT>()); }
  
    virtual int Compare(const DOT&, const DOT&);
    virtual DOT& Dereference(VT*);
};

template <class VT, class DOT>
class PointerSortHelper : public SortingHelper<VT, DOT>
{
  public:
    PointerSortHelper(){}
    static PointerSortHelper<VT, DOT>* Make()
        { return (new PointerSortHelper<VT, DOT>()); }
    
    virtual int Compare(const DOT&, const DOT&);
    virtual DOT& Dereference(VT*);
};

template <class VT, class DOT>
class ItemSorter
{
  protected:
    // order modifier factor
    int _OrderModifier;

    // Sorting helper object
    SortingHelper<VT, DOT>* _SortHelper;

    // object swapping methods
    void Swap(VT* list, ArraySize index1, ArraySize index2);
    void Swap(VT& element1, VT& element2);

    // object assignment methods
    void Assign(VT& element1, VT& element2);

    // object comparison method
    virtual int Compare(VT* Ptr1_, VT* Ptr2_);

    // Quick Sort Helper Functions
    void _QuickSort(VT* list, ArraySize lo, ArraySize hi);
    ArraySize Partition(VT* list, ArraySize lo, ArraySize hi);

    // Heap sort helper functions
    void SwapDown(VT* list, ArraySize root, ArraySize hi);
    void Heapify(VT* list, ArraySize lo, ArraySize hi);

    // Shell sort helper functions
    void ShellSwap(VT* list, ArraySize lo, ArraySize hi, ArraySize spread);

    // Merge Sort Helper Functions
    virtual Boolean SplitFile(char* filename, char* subname1, char* subname2);
    virtual void MergeFile(char* filename, char* subname1, char* subname2);

    // Merge Procedure
    void Merge(VT* list, ArraySize lo, ArraySize hi, ArraySize mid);
    void Merge(VT* mainlist,
	       VT* losublist, ArraySize losize,
	       VT* hisublist, ArraySize hisize);

  public:
    ItemSorter(int sortmod=SortOrder::INCREASING);

    // sorting order procedures
    void SetOrder(int so)
	{ _OrderModifier = so; }
    int GetOrder()
	{ return _OrderModifier; }

    // Set sorter helper object
    void SetSortHelper(SortingHelper<VT, DOT>* Ptr_);

    // ItemSorter Procedures
    void QuickSort(VT* list, ArraySize lo, ArraySize hi);
    void ShellSort(VT* list, ArraySize lo, ArraySize hi);
    void HeapSort(VT* list, ArraySize lo, ArraySize hi);
    void MergeSort(VT* list, ArraySize lo, ArraySize hi);
    void SelectSort(VT* list, ArraySize lo, ArraySize hi);
    void MergeSort(char* filename, char* subname1, char* subname2);

    // Searching Procedures
    Boolean LinearSearch(VT* list, ArraySize lo, ArraySize hi, VT& item, ArraySize& index);
    Boolean BinarySearch(VT* list, ArraySize lo, ArraySize hi, VT& item, ArraySize& index);
};

/****************************************************************************/
#endif




