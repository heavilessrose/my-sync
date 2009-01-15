/*****************************************************************************
*	       Sorting/Searching Procedures Library Source File		     *
* USES:									     *
*   General Purpose Sorting Library					     *
*****************************************************************************/
#ifndef SORTS_CPP
#define SORTS_CPP
/******************************* Header Files *******************************/
#ifndef SORTS_H
  #include "sorts.h"
#endif

/****************************************************************************/
template <class VT, class DOT>
int ObjectSortHelper<VT, DOT>::Compare(const DOT& Obj1_, const DOT& Obj2_)
{
  return
  (
    (Obj1_ < Obj2_) ? -1:
    (Obj1_ > Obj2_) ? 1:0
  );
}

/****************************************************************************/
template <class VT, class DOT>
DOT& ObjectSortHelper<VT, DOT>::Dereference(VT* VectPtr_)
{
  return (*VectPtr_);
}

/****************************************************************************/
template <class VT, class DOT>
int PointerSortHelper<VT, DOT>::Compare(const DOT& Obj1_, const DOT& Obj2_)
{
  return
  (
    (Obj1_ < Obj2_) ? -1:
    (Obj1_ > Obj2_) ? 1:0
  );
}

/****************************************************************************/
template <class VT, class DOT>
DOT& PointerSortHelper<VT, DOT>::Dereference(VT* VectPtr_)
{
  return (**VectPtr_);
}

/****************************************************************************/
/*			 sortclass_t class definition			    */
/****************************************************************************/
/**************************** Class Constructor *****************************/
/****************************************************************************/
template <class VT, class DOT>
ItemSorter<VT, DOT>::ItemSorter(int sortmod):
_OrderModifier(sortmod)
{}

/************************ object comparison method **************************/
/****************************************************************************/
template <class VT, class DOT>
int ItemSorter<VT, DOT>::Compare(VT* Ptr1_, VT* Ptr2_)
{
  return
  (
    _SortHelper->Compare(
      _SortHelper->Dereference(Ptr1_),
      _SortHelper->Dereference(Ptr2_)) * GetOrder()
  );
}

/****************************** Swap Functions ******************************/
/****************************************************************************/
// procedure for swapping pointer elements in an array
// PASSED:
//   list : the array of pointer elements
//   index1, index2 : array indexex of elements to be swapped
// CHANGED:
//   the elements in positions index1 and index2 in the array is swapped
//
template <class VT, class DOT>
void ItemSorter<VT, DOT>::Swap(VT* list, ArraySize index1, ArraySize index2)
{
  if (index1 != index2 && (&list[index1] != &list[index2]))
  {  
    VT Temp_ = list[index1];
    list[index1] = list[index2];
    list[index2] = Temp_;
  }
}

/****************************************************************************/
// Procedure for swapping elements in the array
//
template <class VT, class DOT>
void ItemSorter<VT, DOT>::Swap(VT& element1, VT& element2)
{
  if (&element1 != &element2)
  {
    VT Temp_ = element1;
    element1 = element2;
    element2 = Temp_;
  }
}

/*************************** Assignment Function ****************************/
/****************************************************************************/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::Assign(VT& element1, VT& element2)
{
  if (&element1 != &element2)
    element1 = element2;
}

/*********************** Quick Sort Helper Functions ************************/
/*****************************************************************************
procedure for quick sort partitioning
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  uses the comparison function assigned to the comp function pointer
CHANGED:
  the array of pointers is sorted into order
RETURNED:
  returns the pivot position of the array
*/
template <class VT, class DOT>
ArraySize ItemSorter<VT, DOT>::Partition
(
  VT* list,
  ArraySize lo,
  ArraySize hi
)
{
  short dir = -1;
  VT pivot = list[lo]; // pivot element

  while (lo < hi)
  {
    switch (dir)
    {
      case -1 : while ((Compare(&list[hi], &pivot) >= 0) && lo < hi)
		  hi += dir;
		list[lo] = list[hi];
		break;

      case 1 : while ((Compare(&list[lo], &pivot) <= 0) && lo < hi)
		 lo += dir;
	       list[hi] = list[lo];
	       break;
    }

    dir *= -1; /* change direction of pointer movement */
  }

  list[lo] = pivot;
  return lo;
}

/*****************************************************************************
procedure for quick sort
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  uses partition to sort each subarray of the array
  uses quicksort to sort recursively
CHANGED:
  the array is sorted into order
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::_QuickSort(VT* list, ArraySize lo, ArraySize hi)
{
  ArraySize newx;

  if (lo < hi)
  {
    newx = Partition(list, lo, hi);  /* partition array */
    _QuickSort(list, lo, newx-1);    /* sort left subarray */
    _QuickSort(list, newx+1, hi);    /* sort right subarray */
  }
}

/*************************** Quick Sort Procedure ***************************/
/*****************************************************************************
quicksort wrapper procedure for checking pointers
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  _quicksort to call the quicksort sorting procedure
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::QuickSort(VT* list, ArraySize lo, ArraySize hi)
{
  ASSERT(list!=NULL, ERRMSG_NULLARRAY);		// check for NULL pointers
  _QuickSort(list,lo,hi);			// execute quick sort procedure
}

/*********************** Shell Sort Helper Functions ************************/
/*****************************************************************************
Procedure called by shell sort procedure to swap elements of the array into
sorted order
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
  spread : the gap between elements of the array
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::ShellSwap
(
  VT* list,
  ArraySize lo,
  ArraySize hi,
  ArraySize spread
)
{
  Boolean done;
  ArraySize i;

  do{
    for (i=lo, done=TRUE; i<=hi-spread; i++)
      if (Compare(&list[i], &list[i+spread]) > 0){
	Swap(list,i,i+spread);
	done = FALSE;
      }
  }while (!done);
}

/*************************** Shell Sort Procedure ***************************/
/*****************************************************************************
procedure for shell sort
PASSED:
  list : the array of pointers
  lo : lower limit of array
  hi : upper limit of array
USES:
  uses shellswap to swap elements of the array into sorted order
CHANGED:
  the array is sorted into order
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::ShellSort(VT* list, ArraySize lo, ArraySize hi)
{
  ArraySize spread;	// gap between array elements

  ASSERT(list!=NULL, ERRMSG_NULLARRAY);		// check for NULL pointers
  for (spread=(hi-lo+1)/2;spread;)
  {
    ShellSwap(list,lo,hi,spread);
    spread /= 2;
  }
}

/*************************** Merge Sort Procedure ***************************/
/*****************************************************************************
Procedure to sort an array of pointers using merge sort
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::MergeSort(VT* list, ArraySize lo, ArraySize hi)
{
  ArraySize mid;

  if (lo<hi)
  {
    mid = (lo+hi)/2;
    MergeSort(list,lo,mid);
    MergeSort(list,mid+1,hi);
    Merge(list,lo,hi,mid);
  }
}

/************************ Heap Sort Helper Functions ************************/
/****************************************************************************/
// Given a complete binary tree stored in locations root through n of array
// list with left and right subtrees that are heaps. This procedure converts
// this tree into a heap
//
template <class VT, class DOT>
void ItemSorter<VT, DOT>::SwapDown(VT* list, ArraySize root, ArraySize hi)
{
  ArraySize child = 2 * root;
  Boolean finished = FALSE;

  while (!finished && child <= hi)
  {
    // find largest child
    if (child < hi && Compare(&list[child], &list[child + 1]) < 0)
      ++child;

    // swap node and largest child if neccessary,
    // and move down to the next subtree.
    if (Compare(&list[root], &list[child]) < 0)
    {
      Swap(list[root], list[child]);
      root = child;
      child = 2 * root;
    }
    else
      finished = TRUE;
  }
}

/****************************************************************************/
// Procedure to convert a complete binary tree stored in positions lo
// through hi of array list into a heap.
//
template <class VT, class DOT>
void ItemSorter<VT, DOT>::Heapify(VT* list, ArraySize lo, ArraySize hi)
{
  for (ArraySize root = hi/2; root >= lo; root--)
    SwapDown(list, root, hi);
}

/****************** External File Sorting Using Merge Sort ******************/
/*****************************************************************************
Sub procedure of merge sort for merging two subfiles into one main file.
Procedure is used in merge sort algorithm for sorting external files.
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::MergeFile(char* fname, char* subname1, char* subname2)
{
  DOT objlo, objhi;		// objects read/write from or to files
  fstream file,			// source data file
	  hisubf,       	// higher subfile
	  losubf;       	// lower subfile

  // opening binary files
  file.open(fname,ios::binary|ios::out);
  losubf.open(subname1,ios::binary|ios::in);
  hisubf.open(subname2,ios::binary|ios::in);

  // read first object from both subfiles
  ::Read(losubf, objlo);
  ::Read(hisubf, objhi);

  // repeat the following as long as elements are left in both the
  // lower and upper portions to be merged together
  while (losubf.good() && !losubf.eof() &&
         hisubf.good() && !hisubf.eof())
  {
    if ((_SortHelper->Compare(objlo, objhi) * GetOrder()) < 0)
    {
      ::Write(file, objlo);
      ::Read(losubf, objlo);
    }
    else
    {
      ::Write(file, objhi);
      ::Read(hisubf, objhi);
    }
  }

  // one of the following loops will be executed
  while (losubf.good() && !losubf.eof())
  {
      ::Write(file, objlo);
      ::Read(losubf, objlo);
  }

  while (hisubf.good() && !hisubf.eof())
  {  
      ::Write(file, objhi);
      ::Read(hisubf, objhi);
  }

  // closing data file and subfiles
  file.close();
  losubf.close();
  hisubf.close();
}

/*****************************************************************************
Sub procedure of merge sort for splitting the main file into subfiles.
Procedure is used in merge sort algorithm for sorting external files.
*/
template <class VT, class DOT>
Boolean ItemSorter<VT, DOT>::SplitFile(char* fname, char* subname1, char* subname2)
{
  DOT objlo, objhi; 		        // objects read/write from or to files
  ifstream file;		        // source data file
  ofstream *hisubf = new ofstream;      // pointer to higher subfile
  ofstream *losubf = new ofstream;      // pointer to lower subfile
  ofstream *temp;		        // pointer to temporary stream variable
  Boolean done;

  // opening binary files
  file.open(fname,ios::binary|ios::in);
  losubf->open(subname1,ios::binary|ios::out);
  hisubf->open(subname2,ios::binary|ios::out);

  if (!file.good() || !losubf->good() || !hisubf->good())
    return FALSE;

  // initializing temp stream variable to lower subfile
  temp = losubf;

  // splitting main file into subfiles
  ::Read(file, objlo);
  
  while (file.good() && !file.eof())
  {
    ::Write(*losubf, objlo);
    ::Read(file, objhi);

    if (file.good() && !file.eof())
    {
      if ((_SortHelper->Compare(objlo, objhi) * GetOrder()) > 0)
      {
	temp = losubf;
	losubf = hisubf;
	hisubf = temp;
      }
      objlo = objhi;
    }
  }

  // closing data file and subfiles
  file.close();
  losubf->close();
  hisubf->close();

  // done if temp stream variable is unchanged from its initialized value
  done = temp == losubf;
  delete losubf;	// delete pointers to files streams
  delete hisubf;

  return done;
}

/**************************** Heap Sort Procedure ***************************/
/*****************************************************************************
Procedure to sort an array of objects using heap sort
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::HeapSort(VT* list, ArraySize lo, ArraySize hi)
{
  if (!lo)
  {
    list -= 1;
    ++lo;
    ++hi;
  }

  Heapify(list, lo, hi);

  for (ArraySize i = hi; i > lo; i--)
  {
    Swap(list[lo], list[i]);
    SwapDown(list, lo, i - 1);
  }
}

/************************* Selection Sort Procedure *************************/
/*****************************************************************************
Procedure to sort an array of objects using selection sort
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::SelectSort(VT* list, ArraySize lo, ArraySize hi)
{
  ArraySize elementpos;

  for (ArraySize i = lo; i < hi; i++)
  {
    elementpos = i;
    for (ArraySize i2 = i + 1; i2 <= hi; i2++)
      if (Compare(&list[elementpos], &list[i2]) > 0)
	elementpos = i2;

    Swap(list, i, elementpos);
  }
}

/*****************************************************************************
Merge sort procedure for sorting external files
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::MergeSort(char* filename, char* subfile1, char* subfile2)
{
  Boolean done = FALSE;	// sort done flag

  // sorting file using merge sort
  while (!done)
  {
    done = SplitFile(filename, subfile1, subfile2);
    MergeFile(filename, subfile1, subfile2);
  }

  // deleting sub files
  remove(subfile1);
  remove(subfile2);
}

/***************************** Merge Procedures *****************************/
/*****************************************************************************
Procedure to merge subarray partitions from the merge sort procedure
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::Merge
(
  VT* list,
  ArraySize lo,
  ArraySize hi,
  ArraySize mid
)
{
  ArraySize n = hi-lo+1;
  VT* temp = new VT[n];	        // temporary array for the sort
  ArraySize lopos = lo,
	      hipos = mid+1,
	      index = 0;

  // repeat the following as long as elements are left in both the
  // lower and upper portions to be merged together
  for (;lopos <= mid && hipos <= hi; index++)
    if (Compare(&list[lopos], &list[hipos]) < 0)
    {
      temp[index] = list[lopos];
      lopos++;
    }
    else
    {
      temp[index] = list[hipos];
      hipos++;
    }

  // one of the following loops will be executed
  for (;lopos <= mid; lopos++)
    temp[index++] = list[lopos];

  for (;hipos <= hi; hipos++)
    temp[index++] = list[hipos];

  // copy the temporary array into the original list
  for (index = 0; lo <= hi; lo++, index++)
    Swap(list[lo], temp[index]);

  delete[] temp;		// delete temporary array
}

/*****************************************************************************
Procedure to merge subarray partitions from arrays passed to the function
*/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::Merge
(
  VT* mainlist,
  VT* losublist,
  ArraySize losize,
  VT* hisublist,
  ArraySize hisize
)
{
  ArraySize index = 0,
	      lopos = 0,
	      hipos = 0;

  // repeat the following as long as elements are left in both the
  // lower and upper portions to be merged together
  while (lopos < losize && hipos < hisize)
    if (compare(losublist[lopos], hisublist[hipos]) < 0)
      mainlist[index++] = losublist[lopos++];
    else
      mainlist[index++] = hisublist[hipos++];

  // one of the following loops will be executed
  while (lopos < losize)
    mainlist[index++] = losublist[lopos++];

  while (hipos < hisize)
    mainlist[index++] = hisublist[hipos++];
}

/*************************** Searching Procedures ***************************/
/*****************************************************************************
Procedure to linearly search an array for a specific data item
PASSED:
  list : array of element pointers
  lo : lower limit of array
  hi : upper limit of array
  item : item to be found
READ:
  list is read from start to end for elements matching the data item
RETURNED:
  if the item is found, returns TRUE, otherwise returns FALSE
*/
template <class VT, class DOT>
Boolean ItemSorter<VT, DOT>::LinearSearch
(
  VT* list,
  ArraySize lo,
  ArraySize hi,
  VT& item,
  ArraySize& index
)
{
  ASSERT(list != NULL, ERRMSG_NULLARRAY);	// check for NULL pointers
  ASSERT(lo <= hi, ERRMSG_INVALIDLIMITS);

  for (;lo <= hi; lo++)
    if (!Compare(&list[lo], &item))
    {
      index = lo;
      return TRUE;
    }

  return FALSE;
}

/*****************************************************************************
Procedure to do a binary search of an array for a specific data item
PASSED:
  list : sorted array of element pointers
  lo : lower limit of array
  hi : upper limit of array
  item : item to be found
READ:
  list is read in binary fashion for elements matching the data item
RETURNED:
  if the item is found, returns TRUE, otherwise returns FALSE
*/
template <class VT, class DOT>
Boolean ItemSorter<VT, DOT>::BinarySearch
(
  VT* list,
  ArraySize lo,
  ArraySize hi,
  VT& item,
  ArraySize& index
)
{
  int cmpres;  /* data match flag */
  ArraySize mid; /* array midpoint index */

  ASSERT(list != NULL, ERRMSG_NULLARRAY);	// check for NULL pointers
  ASSERT(lo <= hi, ERRMSG_INVALIDLIMITS);

  while (lo <= hi && (cmpres = Compare(&item, &list[mid=(lo+hi)/2])))
    if (cmpres > 0)
      lo = mid + (1 * GetOrder());
    else
      hi = mid - (1 * GetOrder());

  if (cmpres)
    return FALSE;

  index = mid;
  return TRUE;
}

/****************************************************************************/
template <class VT, class DOT>
void ItemSorter<VT, DOT>::SetSortHelper(SortingHelper<VT, DOT>* Ptr_)
{
  _SortHelper = Ptr_;
}

/****************************************************************************/
/****************************************************************************/
#if ITEMSORTER_DEBUG
#if ITEMSORTER_OBJECT_DEBUG
void ShowArray(int* arr, int size_)
{
  int i;
  for (i = 0; i < size_; i++)
    cout <<arr[i] <<",";
  cout <<endl;
}
#endif

#if ITEMSORTER_POINTER_DEBUG
void ShowArray(int** arr, int size_)
{
  int i;
  for (i = 0; i < size_; i++)
    cout <<*(arr[i]) <<",";
  cout <<endl;
}
#endif

int main()
{
#if ITEMSORTER_OBJECT_DEBUG
  char filename1[] = "testfile.bin";
  char tempfile1[] = "tempf1.bin";
  char tempfile2[] = "tempf2.bin";

  fstream file;
  int arr[100];
  int i, valid;

  RANDOMIZE();
  ItemSorter<int, int> Sorter_;
  Sorter_.SetSortHelper(ObjectSortHelper<int, int>::Make());

  // testing quicksort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  Sorter_.QuickSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing shellsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.ShellSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing heapsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.HeapSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing mergesort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.MergeSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing selectsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.SelectSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing mergesort for files
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  file.open(filename1, ios::binary|ios::out);
  file.seekp(0);
  for (i=0;i<100;i++)
    ::Write(file, arr[i]);
  file.close();

  Sorter_.MergeSort(filename1, tempfile1, tempfile2);

  file.open(filename1,ios::binary|ios::in);
  file.seekg(0);
  for (i = 0 ; i < 100; i++)
    ::Read(file, arr[i]);
  file.close();
  
  for (i = 0; i < 99; i++)
    assert(arr[i] <= arr[i+1]);

  // testing linear search
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  i = RANDOM(100);
  ArraySize out1;

  if (Sorter_.LinearSearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);

  // testing binary search
  Sorter_.QuickSort(arr, 0, 99);
  i = RANDOM(100);

  if (Sorter_.BinarySearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);

/// Reverse Order
  Sorter_.SetOrder(SortOrder::DECREASING);

  // testing quicksort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  Sorter_.QuickSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing shellsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.ShellSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing heapsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.HeapSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing mergesort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.MergeSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing selectsort
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);

  Sorter_.SelectSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing mergesort for files
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  file.open(filename1, ios::binary|ios::out);
  file.seekp(0);
  for (i=0;i<100;i++)
    ::Write(file, arr[i]);
  file.close();

  Sorter_.MergeSort(filename1, tempfile1, tempfile2);

  file.open(filename1,ios::binary|ios::in);
  file.seekg(0);
  for (i = 0 ; i < 100; i++)
    ::Read(file, arr[i]);
  file.close();
  
  for (i = 0; i < 99; i++)
    assert(arr[i] >= arr[i+1]);

  // testing linear search
  for (i = 0; i < 100; i++)
    arr[i] = (int)(RANDOM(1000) + 1);
  
  i = RANDOM(100);

  if (Sorter_.LinearSearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);

  // testing binary search
  Sorter_.QuickSort(arr, 0, 99);  
  i = RANDOM(100);

  if (Sorter_.BinarySearch(arr, 0, 99, arr[i], out1))
    valid = (arr[out1] == arr[i]);
  else
    valid = FALSE;

  assert(valid);
  cout <<"SUCCESS testing <ItemSorter> with objects" <<endl;
#endif

#if ITEMSORTER_POINTER_DEBUG
  char filename1[] = "testfile.bin";
  char tempfile1[] = "tempf1.bin";
  char tempfile2[] = "tempf2.bin";
  
  fstream file;
  int* arr[100];
  int i, valid;

  RANDOMIZE();
  ItemSorter<int*, int> Sorter_;
  Sorter_.SetSortHelper(PointerSortHelper<int*, int>::Make());

  // testing quicksort
  for (i = 0; i < 100; i++)
    arr[i] = new int(RANDOM(1000) + 1);
  
  Sorter_.QuickSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) <= *(arr[i+1]));

  // testing shellsort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.ShellSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) <= *(arr[i+1]));

  // testing heapsort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.HeapSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) <= *(arr[i+1]));

  // testing mergesort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.MergeSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) <= *(arr[i+1]));

  // testing selectsort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.SelectSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) <= *(arr[i+1]));

  // testing mergesort for files
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  file.open(filename1, ios::binary|ios::out);
  file.seekp(0);
  for (i=0;i<100;i++)
    ::Write(file, *(arr[i]));
  file.close();

  Sorter_.MergeSort(filename1, tempfile1, tempfile2);

  file.open(filename1,ios::binary|ios::in);
  file.seekg(0);
  for (i = 0 ; i < 100; i++)
    ::Read(file, *(arr[i]));
  file.close();
  
  for (i = 0; i < 99; i++)
    assert(*(arr[i]) <= *(arr[i+1]));

  // testing linear search
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);
  
  i = RANDOM(100);
  ArraySize out1;

  if (Sorter_.LinearSearch(arr, 0, 99, arr[i], out1))
    valid = (*(arr[out1]) == *(arr[i]));
  else
    valid = FALSE;

  assert(valid);

  // testing binary search
  Sorter_.QuickSort(arr, 0, 99);
  i = RANDOM(100);

  if (Sorter_.BinarySearch(arr, 0, 99, arr[i], out1))
    valid = (*(arr[out1]) == *(arr[i]));
  else
    valid = FALSE;

  assert(valid);

/// Reverse Order
  Sorter_.SetOrder(SortOrder::DECREASING);

  // testing quicksort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);
  
  Sorter_.QuickSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) >= *(arr[i+1]));

  // testing shellsort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.ShellSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) >= *(arr[i+1]));

  // testing heapsort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.HeapSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) >= *(arr[i+1]));

  // testing mergesort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.MergeSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) >= *(arr[i+1]));

  // testing selectsort
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);

  Sorter_.SelectSort(arr, 0, 99);

  for (i = 0; i < 99; i++)
    assert(*(arr[i]) >= *(arr[i+1]));

  // testing mergesort for files
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);
  
  file.open(filename1, ios::binary|ios::out);
  file.seekp(0);
  for (i=0;i<100;i++)
    ::Write(file, *(arr[i]));
  file.close();

  Sorter_.MergeSort(filename1, tempfile1, tempfile2);

  file.open(filename1,ios::binary|ios::in);
  file.seekg(0);
  for (i = 0 ; i < 100; i++)
    ::Read(file, *(arr[i]));
  file.close();
  
  for (i = 0; i < 99; i++)
    assert(*(arr[i]) >= *(arr[i+1]));

  // testing linear search
  for (i = 0; i < 100; i++)
    *(arr[i]) = (int)(RANDOM(1000) + 1);
  
  i = RANDOM(100);

  if (Sorter_.LinearSearch(arr, 0, 99, arr[i], out1))
    valid = (*(arr[out1]) == *(arr[i]));
  else
    valid = FALSE;

  assert(valid);

  // testing binary search
  Sorter_.QuickSort(arr, 0, 99);  
  i = RANDOM(100);

  if (Sorter_.BinarySearch(arr, 0, 99, arr[i], out1))
    valid = (*(arr[out1]) == *(arr[i]));    
  else
    valid = FALSE;

  assert(valid);

  for (i = 0; i < 100; i++)
    delete arr[i];
    
  cout <<"SUCCESS testing <ItemSorter> with pointers" <<endl;
#endif

  return 0;
}
#endif
/****************************************************************************/
#endif




