#ifndef MEMPOOL_H
#define MEMPOOL_H
#ifndef INCL_NEW_H
  #include <new.h>
  #define INCL_NEW_H
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif
#ifndef MMLIST_CPP
  #include "mmlist.cpp"
#endif
#ifndef GENFNCS_H
  #include "genfncs.h"
#endif

//
// Tested Successfully On:
// - BC++ 3.1 for DOS
// - DJGPP with GCC 2.95.3, GCC 3.33
// - BC++ 5.01
// - VC++ 5.0, 6.0, .NET
//
// The enumerated memory pools in the MemPool  class should stay. Even though
// only one block of memory is allocated for each pool, the enumerated header
// provides a convenient way for each pool to indicate it is in use. The
// header can be set to 0 if it is not in use and set to the size of the block
// if it is in use. Although this setup is more expensive than a single byte
// flag, it provides the side benefit of having information on the actual
// memory usage of the block. This information can be used as memory
// statistics for the general program or for future enhancements to the memory
// manager.
//
// the memory manager allocate memory lists in increasing order of fibonacci
// numbers for storage of the blocks. To find the matching memory list for
// each block the search procedures of the MMList class are modified to take
// advantage of the RelationalEnum type value passed to the PtrCompare<T>
// functor in each of the search procedures which then calls the appropriate
// relational operator for each object of the parametric type T. To make this
// work the MemList class would have to be modified to include the various
// relational operators that can be called from the PtrCompare<T> functor.
// Once a matching block is found it is initialized with the actual memory
// usage size passed to the allocate method of the memory manager rather than
// the actual allocated pool size stated in the memory list. This would
// initialize the size header of the pool object returned by the memory list
// with the actual usage size of the block of memory occupying it.
//
// USING TREES
// -----------
// Trees were considered as a more efficient replacement for linked lists
// since the chunk sizes of each list would be sorted in order if placed in a
// tree structure. But the problem with using a tree to store the lists
// according to their allocatable pool sizes is that the search criteria for a
// memory block is to find a matching list with pool sizes equal or greater
// than the block size. This creates a problem if the lists were placed in a
// tree structure since the search for the matching list may not neccessarily
// begin with the smallest list. In most cases of searching through a tree,
// the search would begin with a node that is located somewhere in the middle
// in terms of its magnitude in relation with other nodes. Because of this
// peculiar property of searching in trees the fittest memory list in terms of
// the memory size that is the smallest able to enclose the requested block
// may be bypassed for any block that could provide a fit though not
// neccessarily a close fit. This would waste space that would otherwise be
// better spent enclosing memory blocks closest to its allocatable size. A
// linked list of lists would not have this problem since the search for a
// matching list would begin with the smallest list and progress in size as
// you search up the list. The list with the closest size that could enclose
// the requested memory block would be returned.
//
// The memory manager also has the following addition features:
//
// ANSI memory manipulation functions in the MemPool class
// Range checking for all memory block manipulation methods
// PreAllocation To selected minimum number of pools for every memory list
// Ability to reallocate memory to a bigger/smaller size pool with the
//   option of keeping the original memory block address if possible
// Memory statistics provided for:
//	  Block Size
//	  Total Usage
//	  Total Wastage (gaps of unused memory in each memory pool)
//	  Total Free Space
//	  Total Allocated
//
// The memory management scheme used by the MemMatrix memory manager is as
// follows:
//
// 1 2 3 5 8 13 21 34 55 ...
//       | |  |  |  |  |
//       | |  |  |  |  |
//       | |  |  |  |  |
//         |     |     |
//         |     |
//               |
//
// The series of numbers represents the prespecified chunksize for the memory
// list. Each number with the downward chain of "|" represents a memory list
// contained within the memory manager. Each "|" represents a memory pool
// stored within the given memory list. Notice that each memory list contains
// a minimum of 3 pools or 0 pools. This would occur if the
// MemMatrix::MemMatrix(size_t) constructor is given a default value of 3.
// This specifies a minimum of 3 memory pools to maintained for any list in
// the memory manager containing allocated blocks. If the memory list is not
// actually used (no allocated blocks within it) it has 0 memory pools since
// it doesn't need to maintain them. An allocated block that is no longer used
// is marked as unused by the memory manager when the MemMatrix::Deallocate
// method is called. The block is marked, but not actually deleted. It is
// left as an available free block for future memory requests. For optimizing
// the memory manager the MemMatrix::Preallocate method could be called to
// preallocate a series of pools up to the maximum size pool specified.
//
// For example: if you want to Preallocate up to or including a 80 sized
// memory pool with a minimum of 3 pools maintained within each memory list:
//
// first change the default argument to the MemMatrix::MemMatrix(size_t)
// constructor to 3 then call MemMatrix::Preallocate method with an
// argument of 80 (If the tweak file feature is turned on you can do this
// by specifying the field values in the tweak file)
//
// The resulting memory manager would look like the following:
//
// 1 2 3 5 8 13 21 34 55 89
// | | | | |  |  |  |  |  |
// | | | | |  |  |  |  |  |
// | | | | |  |  |  |  |  |
//
// A minimum of three pre-sized memory pools for each list is preallocated.
// Notice that there is a size 89 pool, but no size 80 pool. This is because
// 89 is included in the Fib series but 80 isn't. This is okay because a size
// 80 allocated block can fit within a size 89 memory pool. The memory manager
// has an algorithm to find the "best fit" pool for any given allocated block
//
// --------------------------------------------------------------------------
// Revision Jan 27 1999	: Fixed bug for traversing through linked list of
//			  memory pools in MemList::SetMemoryCap method.
// 			  Revised the linked list class to include a list
//			  browser class for this purpose.
// Revision Jan 27 1999 : Fixed bug in MemList::Allocate method. The
//			  _RealTotal member for keeping track of total pools
//			  should be incremented after the memory pool has
//			  been validated and added into the linked list not
//		          at the end of the for loop
// Revision Feb 26 1999 : Added HasThis method for determining whether given
//			  address is an allocated memory block stored in the
//			  memory manager.
// Revision Feb 27 1999 : Revised Deallocate method to return a unsigned
//			  (as boolean) value indicating whether the given
//			  memory block has been successfully deallocated.
//                        If the memory block cannot be located within the
//			  memory manager then the method returns 0 (FALSE).
// Revision March 5 1999 : Revised Reallocate method to accept the unsigned
//			   parameter: Moveable_
//	                   The Moveable_ parameter can be used to specify
//			   whether the contents of the original memory block
// 			   can be moved to a new memory block with a different
//			   address.
// Revision March 21 1999 : Added 5 new methods:
//
//      Suspend/Restore memory handler methods:
//
//         // Returns TRUE if handler is in active state
//         BOOLTYPE MemMatrix::IsNewHandlerActive()
//
//	   // Set new handler into active state
//	   void MemMatrix::SetNewHandlerActive()
//
//	   // Set new handler into inactive state
//	   void MemMatrix::SetNewHandlerInactive()
//
// 	Memory handler setting method:
//
// 	   // Sets new handler and returns previously used memory handler
//         MemoryHandlerType MemMatrix::SetNewHandler(MemoryHandlerType NewHandler_)
//
//      Direct call to memory handler method:
//
//         // Method to call memory handler directly to handle memory faults
//         void MemMatrix::NoMemory();
//
// - Added MemMatrix::_CustomHandler member for users to set a custom
//   memory handler
// - Added MemMatrix::_HandlerActive member as a flag to indicate active
//   state of handler
// - Revised MemMatrix::Allocate and MemMatrix::Reallocate methods to
//   work with these changes.
//
// Revision March 22 1999 : Revised MemMatrix::Reallocate method to
//			    allocate brand new memory to the passed address
//			    by calling MemMatrix::Allocate if address is NULL
//			    or cannot be found within the memory manager.
//
//			    Changed MemList::Reallocate to accept unsigned&
//			    argument to indicate whether the specified
//			    address points to an existing memory pool in the
//			    memory manager.
// Revision May 1 1999 : Fixed bug in loop for finding number of freeblocks
//			 in MemList::SetMemoryCap method.
//		         Previous implementation would overcount by 1 since
//		         condition of loop is tested after incrementing count
//
//			 Optimized loop for counting up to minimum sized list
//			 in MemList::SetMemoryCap method by eliminating
//			 redundant loop terminating condition
// Revision May 9 1999 : Fixed bug in MemMatrix::Deallocate and
//			 MemMatrix::HasThis methods for null addresses.
//			 Null memory addresses passed to these methods should
//			 not be processed since they will never be found as a
//			 valid memory block address. Both methods should
//			 return 0 for address not found.
// Revision May 10 1999 : Fixed bugs in MemMatrix::Reallocate method. All
//			  exceptional values passed as parameters are handled
//			  - If 0 bytes specified for reallocation then free
//			    memory instead by calling the deallocate method
//			  - If null address is given then allocate new memory
//			  - If unrecognized address is given then if the
//			    Moveable_ flag is specified then allocate new
//			    memory, otherwise return the original specified
//			    address.
//			  Fixed bug in MemMatrix::Allocate method.
//			  - If given a zero value for bytes to allocate then
//			    return NULL instead. Zero bytes cannot be
//			    allocated.
// Revision May 16 1999 : Added MemMatrix::ReleaseAll and MemList::ReleaseAll
//			  methods. The ReleaseAll method allows the user to
//			  explicitly "release" all memory blocks stored within
//			  the memory manager without need to pass the address
//			  of any specified block.
//
//                        Added increment operators to the list browser:
//                        - MMListBrowser::operator ++ ()
//                        - MMListBrowser::operator ++ (int)
//
//			  Added node indexing method to the list browser:
//                        - MMListBrowser::IndexNode(size_t)
//
//			  Added current node accessor to the list browser:
//			  - MMNode<T>* MMListBrowser::CurrentNode()
// Revision June 15 1999 : Added MemMatrix::Preallocate and
//			   MemList::Preallocate methods. These methods allow
//			   the user to preallocate a series of memory pools
//			   up to the specified maximum memory pool size.
//			   These pools are contained within each memory list
//                         with equal sized pools respective to each list.
//			   The minimum number of pools specified by the
//			   MemMatrix::_MinPools data member is then allocated
//			   for each memory list.
//
//			   Doing this relieves the memory manager from doing
//			   requesting memory from the system and have it
//			   simply return the preallocated memory to the
//			   calling program. But judge carefully because
//			   preallocating too much may be a drain on memory
//			   resources.
// Revision June 17 1999 : Added the BOOLTYPE typedef for specifying the
//                         Boolean type. This is located in the boolean.h
//			   header file. The boolean.h header contains a
//			   custom boolean class which could also be used.
//
//			   Added support for a tweak file for fine tuning
//			   the memory manager by specifying minimum number
//			   of maintainable pools or preallocation maximum
//			   pool size through a initialization text file.
// Revision June 18 1999 : Changed #define MMTWEAK_FILE to MM_TWEAK_FILE
//		           Added #define MM_MINPOOLS and used as default
//			   1st argument to MemMatrix::MemMatrix constructor
//			   The 1st argument is used to specify the minimum
//			   number of maintainable pools per list
//			   Added #define MM_PREALLOC and used as default
//			   2nd argument to MemMatrix::MemMatrix constructor
//			   The 2nd argument is used to specify the maximum
//			   sized pool to preallocate up to.
//
//			   Optimize equality method MemPool::IsEqual for
//			   finding equal memory block addresses.
//
//			   Created another method MemRequest::IsEqual for
//		           testing for an available free memory block.
//			   The test was originally mixed in with the
//			   MemPool::IsEqual method, but this slows down
//			   comparisons for equal addresses.
//
//			   Added PreAlloc_ parameter to the MemMatrix
//			   constructor. The value of this parameter is
//			   used as the argument to the MemMatrix::Preallocate
//			   method. This method is called everytime an instance
//			   of the MemMatrix class is constructed.
//			   Alternatively this value can be set through the
//			   fields values in the tweak file in which case the
//			   default value is overwritten with the value in the
//			   tweak file. If the PreAlloc_ value is non-zero the
//			   MemMatrix::Preallocate method is called.
// Revision July 28 1999 : Added user selectable allocation methods
//
//		VOIDPTR RawAllocateWith(MemAllocMethod Method_, size_t Bytes_)
//     		VOIDPTR RawReallocateWith(MemAllocMethod Method_, VOIDPTR Address_,
//				     size_t Bytes_, BOOLTYPE Moveable_)
//
// 			   Depending on what the user selects for Method_
//			   the procedure will either user the standard C++
//			   new/delete operators for memory operations or
//			   the memory manager methods MemMatrix::Allocate and
//			   MemMatrix::Deallocate.
//
//			   If Method_ is set to STD_NEW then the standard C++
//			   operators are used. Otherwise the MemMatrix::
//		           operators are used.
// Revision August 24 1999 : Added MM_USE_EXCLUSIVELY flag. This is used to
//                           prevent confusion when releasing memory that
//                           was previously allocated with either MemMatrix
//                           memory methods or global ::new operator.
//                           see comments for details
//
//                           Added universal deletion procedure for arrays
//                           This is the same as the ::Delete procedure, but
//                           used for arrays. It uses the more appropriate
//                           ::delete[] operator if MM_USE_EXCLUSIVELY is not
//                           active instead of of the ::delete operator which
//                           is what the ::Delete procedure uses.
//
//                           replaced void* with the VOIDPTR typedef. This is
//                           done because some compilers doesn't use the
//                           void* pointer correctly as a pointer to generic
//                           objects eg.) MSC++, MetroWerks. For these
//                           compilers (there may be others) the generic
//                           pointer support can be implemented using the
//                           older and less reliable unsigned char* instead
//                           of void*.
// Revision August 31 1999 : Added multithreading support for POSIX and
//                           Win32 threads (untested)
// Revision October 5 1999 : Added universal allocation functor class
//			     New. The "New" class is a template class which
//			     accepts either no argument or an array length
//			     argument. A cast to T* pointer operator is
//			     provided for automatic casting to the storage
//			     type immediately after construction of the
//			     functor class. Optional method argument is
//			     also provided to allow alternative allocation
//			     strategies. The assumption of the template
//			     class defined with default constructor is made.
// Revision Feb 15 2000 : Removed static instance of memory matrix object
//			  because it resides in file scope. This may cause
//		          initialization order problems in projects with
//		          many translation units. Instead placed a static
//		          local variable of MemMatrix type in static method
//			  of MemMatrix class. Calling MemMatrix::Matrix()
//		          would return the static local variable.
// Revision Feb 18 2000 : Renamed Delete to RawDelete, DeleteArray to
//			  RawDeleteArray and New to RawNew to differentiate
//			  raw memory allocation/deallocation methods from
//		          "smart" functions which differs in the way memory
//			  is acquired/released depending on the type of the
//			  object passed to the function.
//			  Added PlacementNew and PlacementCopy template
//			  functions for creating an object at a specified
//			  memory buffer.
// Revision Feb 19 2000 : Added extra information for the header section of
//		          each memory pool object. This is used to
//			  differentiate between memory allocated through
//                        the usual methods of calling the memory manager
//			  allocation/reallocation methods directly and memory
//			  allocated through an explicit call to the
//			  PlacementNew, PlacementCopy functions which are
//                        used to allocated memory through the new (void*) T
//			  placement new operator. This may be useful when
//			  choosing which method of deallocation to use.
// Revision Feb 21 2000 : Added convenience method Callocate(size_t, BOOLTYPE)
//                        This method calls the MemMatrix::Allocate method to
//		          allocate the new memory block then if successful
//		          initializes the new memory block to 0. Returns the
//			  return value from the MemMatrix::Allocate method.
// Revision Dec 30 2004 : Cast pointers to char* before deletion to avoid
//                        undefined deletion warning.
/****************************************************************************/
// MultiThreading support
#if defined(_MSC_VER)
#   ifdef _MT
#     define __MM_WIN32THREADS
#   endif
#endif

#ifdef _PTHREADS
#   define __MM_PTHREADS
#endif

// Headers for MultiThreading support
#ifdef __MM_WIN32THREADS
#  include <windows.h>
#else
#  ifdef __MM_PTHREADS
#    include <pthread.h>
#  endif
#endif

// No thread support if not defined
#if !defined(__MM_PTHREADS) & !defined(__MM_WIN32THREADS)
#   define _NOTHREADS
#endif

# ifdef _NOTHREADS
//  Thread-unsafe
#   define __MEMMATRIX_LOCK
#   define __MEMMATRIX_UNLOCK
#   ifndef __VOLATILE
#     define __VOLATILE volatile
#   endif
# endif

# ifdef __MM_PTHREADS
    // POSIX Threads
#   include <pthread.h>
#   define __MEMMATRIX_LOCK \
	pthread_mutex_lock(GiveLockObject())
#   define __MEMMATRIX_UNLOCK \
	pthread_mutex_unlock(GiveLockObject())
#   define __VOLATILE volatile
# endif

# ifdef __MM_WIN32THREADS
    // The lock needs to be initialized by constructing an allocator
    // objects of the right type.
#   define __MEMMATRIX_LOCK \
	EnterCriticalSection(GiveLockObject())
#   define __MEMMATRIX_UNLOCK \
	LeaveCriticalSection(GiveLockObject())
#   define __VOLATILE volatile
# endif /* WIN32THREADS */

// No global placement new operator, so explicitly define it
#if defined(_MSC_VER) | defined(__DJGPP__) | defined(__GNUC__)
#  define DEFINE_PLACEMENT_NEW  0
#else
#  define DEFINE_PLACEMENT_NEW	1
#endif

// Flag to call memory handler in case of memory allocation failure
#define HANDLE_MEMORY_FAULT	1

// Flag for enabling memory manager tweak file
#define TWEAK_MEMMATRIX		0

// The format of the tweak file follows the style of a Windows .INI file
// and is declared as follows:
//
// [MemMatrix]
// Preallocate = 100
// MinPools = 5
//
// The header section "[MemMatrix]" must be present. This is read in by the
// memory manager to confirm it is reading from a valid tweak file
//
// The "Preallocate" field is read as the maximum size pool in the series of
// memory pools to allocate. A value of 0 specifies no preallocation
//
// The "MinPools" field is read as the minimum number of pools to maintained
// within each memory list. A value of 0 specifies using the default argument
// passed to the MemMatrix::MemMatrix(size_t) constructor
//
// The "Preallocate" and "MinPools" fields can go in any order after the
// "[MemMatrix]" header and case is insensitive. Undefined fields are
// ignored. Spaces between field names and the "=" sign or field values are
// optional, but are ignored while parsing.
//
#if TWEAK_MEMMATRIX
  #define MM_TWEAK_FILE "mmtweak.dat"
#else
  #define MM_TWEAK_FILE NULL
#endif

// Minimum maintainable number of memory pools per memory list
#define MM_MINPOOLS	5

// Maximum sized memory pool to preallocate up to
#define MM_PREALLOC	100

// Use exclusively flag, switched on for exclusive usage of MemMatrix memory
// manager. This also prevents errors when releasing memory. If flag is
// active then ::delete, ::delete[] operators will not be called if
// MemMatrix::Deallocate method fails to find the specified memory block
// If mixed memory management strategy is used (not recommended) in which
// both the global ::new, ::delete operators and MemMatrix memory methods are
// used then turn this flag off. Then the ::delete operator will be called to
// release memory if MemMatrix::Deallocate fails to find the specified memory
// block.
#define MM_USE_EXCLUSIVELY      1

// Fibonacci counter class
// Returns succeeding and preceeding fibonacci numbers
// Also finds closest fibonacci number in relation to a given number
class MMCounter
{
  protected:
    size_t _Previous;		// Previous fibonacci number
    size_t _Count;		// Current fibonacci number

    size_t Decrement();		// Decrement to preceeding number
    size_t Increment();		// Increment to succeeding number

  public:
    // Fibonacci class constructor
    MMCounter(size_t Value_ = 0);

    // Resets counter to 0
    void Reset()
	{ _Count = _Previous = 0; }

    // Increment and decrement operators,
    // both prefix and postfix provided
    size_t operator ++ ()
	{ return Increment(); }
    size_t operator -- ()
	{ return Decrement(); }
    size_t operator ++ (int);
    size_t operator -- (int);

    // Returns current count of series
    size_t Counter()
	{ return _Count; }

    // Returns nearest fibonacci number
    // in relation to the given number
    size_t FindLesser(size_t Value_);
    size_t FindGreater(size_t Value_);
    size_t FindLesserOrEqual(size_t Value_);
    size_t FindGreaterOrEqual(size_t Value_);
    size_t FindFib(size_t Value_);
};

/****************************************************************************/
// Buffer area of mempool object used for storing
// extra book keeping information
#define MEMPOOL_HEADER_BUFFER	Byte_t

// Pool-based memory allocation
class MemPool
{
  friend MemPool& RootObject(MemPool& Obj_)
	{ return Obj_; }
  friend const MemPool& RootConstObject(const MemPool& Obj_)
	{ return Obj_; }

  // equality operators for memory pools
  friend BOOLTYPE operator == (const MemPool& Pool1_, const MemPool& Pool2_);
  friend BOOLTYPE operator != (const MemPool& Pool1_, const MemPool& Pool2_);

  friend Boolean _IsEqualTo(const MemPool& Trg_, const MemPool& Src_)
	{ return Trg_.IsEqual(Src_); }
  friend Boolean _IsMatchable(const MemPool& Obj_)
	{ return TRUE; }        

  protected:
    // Next byte to allocate, starting address of unused byte array memory.
    __VOLATILE Byte_t* _Space;	// byte array memory pool

    // special memory pool constructor used by memory request objects
    // to bypass memory block header initialization
    MemPool(VOIDPTR Address_, BOOLTYPE Init_);

    // Method to set the size of the memory pool
    void SetSize(size_t Size_) __VOLATILE;
    void SetPlaced(Byte_t Placed_) __VOLATILE;

    // Returns TRUE if object is a memory request otherwise returns FALSE
    virtual BOOLTYPE IsMemRequest() const __VOLATILE;

  public:
    // next available memory space initialized to starting address of byte
    // array at construction time of Pool structure.
    MemPool(size_t Size_);
    virtual ~MemPool();

    // Returns the size of the memory pool
    size_t MemSize() const __VOLATILE;
    static size_t SizeOf(VOIDPTR Address_);

    // returns this memory pool if it fits the requested size,
    // otherwise returns NULL
    VOIDPTR MemFit(size_t Bytes_) const __VOLATILE;
    static VOIDPTR MemFit(VOIDPTR Address_, size_t Bytes_);

    // returns whether pool is NULL
    BOOLTYPE IsNULL() const __VOLATILE
	{ return (_Space == NULL); }

    // returns whether or not pool was allocated through placement new
    BOOLTYPE IsPlaced() const __VOLATILE;

    // method to resize pool
    VOIDPTR Resize(size_t Size_) __VOLATILE;

    // Use and return method of the given pool,
    // must be called upon request and release of memory pool
    VOIDPTR Use(size_t Size_, BOOLTYPE Placed_=0) __VOLATILE;
    void Return() __VOLATILE
	{ Resize(0); }

    // Object equality method and Memory pool availability method
    virtual BOOLTYPE IsEqual(const __VOLATILE MemPool& Pool_) const __VOLATILE;
    BOOLTYPE IsFree() const __VOLATILE
	{ return (!IsMemRequest() && MemSize() == 0); }

    // ANSI memory manipulation functions
    VOIDPTR MemMove(const VOIDPTR Address_, size_t Bytes_) __VOLATILE;
    VOIDPTR MemChr(int Char_, size_t Bytes_) const __VOLATILE;
    VOIDPTR MemSet(int Char_, size_t Bytes_) __VOLATILE;
    int MemCmp (const void *Str2_, size_t Bytes_) const __VOLATILE;
};

// Memory reference class, derived from memory pool class
class MemRef : public MemPool
{
  public:
    MemRef(VOIDPTR Address_);	// Initialize a memory reference
    virtual ~MemRef();
};

class MemList;	// Memory list forward declaration

// Memory request class, used for memory pool requests
// within the memory manager
class MemRequest : public MemRef
{
  friend class MemList;

  protected:
    MemRequest();

    // Returns TRUE if object is a memory request otherwise returns FALSE
    virtual BOOLTYPE IsMemRequest() const __VOLATILE;

    // Test for free block method
    virtual BOOLTYPE IsEqual(const __VOLATILE MemPool& Pool_) const __VOLATILE;
};

/****************************************************************************/
// Memory statistics structure
struct MemStats
{
  size_t _Allocated;	// total memory allocated
  size_t _Used;		// memory used within allocated blocks
  size_t _BookKeeping;	// amount of overhead used as book keeping data
  size_t _Wasted;	// memory wasted within allocated blocks
  size_t _Free;		// total memory available within allocated blocks

  size_t _AllocCalls;        	// Counts number of calls to Allocate method
  size_t _ReallocCalls;		// Counts number of calls to Reallocate method
  size_t _DeallocCalls;		// Counts number of calls to Deallocate method
  size_t _ReleaseAllCalls;	// Counts number of calls to ReleaseAll method
};

class MemMatrix;	// Memory matrix forward declaration

// Memory list class, derived from a simple linked list of memory pools
class MemList : protected MMList<MemPool>
{
  // Relational operators for memory list objects
  friend BOOLTYPE operator == (const MemList& List1_, const MemList& List2_);
  friend BOOLTYPE operator != (const MemList& List1_, const MemList& List2_);
  friend BOOLTYPE operator >= (const MemList& List1_, const MemList& List2_);
  friend BOOLTYPE operator > (const MemList& List1_, const MemList& List2_);
  friend BOOLTYPE operator <= (const MemList& List1_, const MemList& List2_);
  friend BOOLTYPE operator < (const MemList& List1_, const MemList& List2_);

  protected:
    MemMatrix* _Matrix;         // Pointer to memory matrix object
    MMCounter _Counter;		// Fibonacci counter of memory blocks
    size_t _RealTotal;		// Actual running total
    size_t _ChunkSize; 		// Size of each free block

    // Free memory pool validation method
    BOOLTYPE Validate(MemPool* Pool_);

  public:
    // Memory list constructor, Size_ indicates the size of each memory pool
    MemList(MemMatrix* Matrix_, size_t Size_);

    // Returns the specified memory pool size
    size_t PoolSize()
	{ return _ChunkSize; }

    // Memory statistics method
    void MemoryStats(MemStats& Stats_);

    // Memory capping and conservation methods
    void DeleteBlocks(size_t Max_);
    size_t SetMemoryCap();

    // Memory allocation, deallocation, reallocation methods
    BOOLTYPE Preallocate();
    VOIDPTR Allocate(size_t Bytes_, BOOLTYPE Placed_);
    VOIDPTR Reallocate(VOIDPTR Address_, size_t Bytes_, BOOLTYPE Moveable_, BOOLTYPE& Found_);
    BOOLTYPE Deallocate(MemPool* Pool_);
    BOOLTYPE HasThis(MemPool* Pool_, BOOLTYPE& IsPlaced_);
    Ulong ReleaseAll();
    void DeleteAll();

    // Object relation methods
    BOOLTYPE IsEqual(const MemList& List_) const
	{ return (_ChunkSize == List_._ChunkSize); }
    BOOLTYPE IsGreater(const MemList& List_) const
	{ return (_ChunkSize > List_._ChunkSize); }
    BOOLTYPE IsLesser(const MemList& List_) const
	{ return (_ChunkSize < List_._ChunkSize); }
};

/****************************************************************************/
class MMPathTokenizer
{
  protected:
    struct StringTokenizer
    {
      const char* _SearchStr;	// The character string the search on
      const char* _Delims;	// Delimiters in the search string
      const char* _WhiteSpace;	// whitespace in the search string
      size_t _Start;
      size_t _Index;

      StringTokenizer();
      StringTokenizer(const char* Str_, size_t Start_, const char* Delims_, const char* Wspc_);

      inline const char* GetString() const
          { return _SearchStr; }
      inline size_t GetStartIndex() const
	  { return _Start; }
      inline size_t GetIndex() const
	  { return _Index; }
      inline const char* GetDelimiters() const
	  { return _Delims; }
      inline const char* GetWhiteSpace() const
	  { return _WhiteSpace; }

      inline void SetStartIndex(size_t Index_)
          { _Start = Index_; }
      inline void ResetIndex()
          { _Index = _Start; }
      inline void SetDelimiters(const char* Str_)
          { _Delims = Str_; }
      inline void SetWhiteSpace(const char* Str_)
          { _WhiteSpace = Str_; }      
      inline void SetString(const char* Str_)
          { _SearchStr = Str_; }

      char* RemovePadding(char* str, const char* chset);
      Boolean InCharSet(char ch, const char* chset);
      size_t FindNextWord(const char* Str_, const char* Delims_, size_t& Index_, size_t& NextPos_);
      size_t FindToken(char*& Token_, Boolean AllocMemory_=FALSE);
    };
  
  protected:  
    size_t _Max;
    size_t _Size;
    char** _TokenArray;
    const char* _TokenSeparator;
    const char* _DirSeparator;    

    void Tokenize(char* Str_);    
    
  public:
    enum
    {
      DOS_PATH,
      UNIX_PATH
    };

  public:
    MMPathTokenizer();
    MMPathTokenizer(const char* VarName_, int SysType_);
    MMPathTokenizer(const char* VarName_, const char* TokSep_, const char* DirSep_);
    virtual ~MMPathTokenizer();

    void SetEnvVar(const char* VarName_, int SysType_);
    void SetEnvVar(const char* VarName_, const char* TokSep_, const char* DirSep_);

    size_t Max() const;
    const char* TokenSeparator() const;
    const char* DirSeparator() const;
    const char* operator [] (size_t Index_) const;
    void Dump(ostream& Out_) const;

    // kludge to make preprocessor macro work
    static void CallMethod(MMPathTokenizer& Obj_, const char* MethodName_, const char* VarName_, int SysType_);    
};

#if (defined(__linux__) | defined(__unix__)) & !defined(__DJGPP__)
  #define MMPATHTOKENIZER(VarName, PathVar)    \
    MMPathTokenizer VarName;                   \
    const char* PathVarConst_ = PathVar;       \
    MMPathTokenizer::CallMethod(VarName, "SetEnvVar(const char*, int)", PathVarConst_, MMPathTokenizer::UNIX_PATH)
#else
  #define MMPATHTOKENIZER(VarName, PathVar)    \
    MMPathTokenizer VarName;                   \
    const char* PathVarConst_ = PathVar;       \
    MMPathTokenizer::CallMethod(VarName, "SetEnvVar(const char*, int)", PathVarConst_, MMPathTokenizer::DOS_PATH)
#endif

/****************************************************************************/
// Memory allocation method enumerated type
enum MemAllocMethod { STD_NEW, MEMMATRIX };

// Memory handler function substitute for default handler
static void MemAllocFailHandler();

// Universal deletion procedure
BOOLTYPE RawDelete(VOIDPTR Address_);

// Universal deletion procedure for arrays
BOOLTYPE RawDeleteArray(VOIDPTR Address_);

// Universal allocation functor
// Assumption: template object has default constructor defined.
template <class T>
class RawNew
{
  private:
    T* _Ptr;

  public:
#if OVERLOAD_NEW
    RawNew(MemAllocMethod Method_=MEMMATRIX):
	_Ptr((Method_ == MEMMATRIX) ? RawAllocateWith(MEMMATRIX, sizeof(T)):(new T)) {}
    RawNew(ptrdiff_t Len_, MemAllocMethod Method_=MEMMATRIX):
	_Ptr((Method_ == MEMMATRIX) ? RawAllocateWith(MEMMATRIX, Len_ * sizeof(T)):(Len_ ? (new T[Len_]):NULL)) {}
#else
    RawNew(MemAllocMethod Method_=STD_NEW):
	_Ptr((Method_ == MEMMATRIX) ? RawAllocateWith(MEMMATRIX, sizeof(T)):(new T)) {}
    RawNew(ptrdiff_t Len_, MemAllocMethod Method_=STD_NEW):
	_Ptr((Method_ == MEMMATRIX) ? RawAllocateWith(MEMMATRIX, Len_ * sizeof(T)):(Len_ ? (new T[Len_]):NULL)) {}
#endif

    operator T* ()
	{ return _Ptr; }
};

// User selectable allocation procedures
VOIDPTR RawAllocateWith(MemAllocMethod Method_, size_t Bytes_);
VOIDPTR RawReallocateWith(MemAllocMethod Method_, VOIDPTR Address_, size_t Bytes_, BOOLTYPE Moveable_=1);

// Global placement new operator
#if DEFINE_PLACEMENT_NEW
void* operator new (size_t, void* Addr_);
#endif

class MemMatrix : protected MMList<MemList>
{
  friend class MemList;	// Everything should be accessable to MemList class

  public:
# ifdef __MM_WIN32THREADS
    static Boolean Lock_Initialized;
    static CRITICAL_SECTION* GiveLockObject();
# endif

# ifdef __MM_PTHREADS
    static pthread_mutex_t* GiveLockObject();
# endif

#ifndef _NOTHREADS
    struct Lock
    {
      Lock()
	{ __MEMMATRIX_LOCK; }
      ~Lock()	// Destory object and release mutex lock
	{ __MEMMATRIX_UNLOCK; }
    };
#endif

 private: 
    // Tweak file read and parse procedures
    static char* TrimAndDnCase(char* Str_);
    static char* RdStr(istream& In_, char* Dest_, int Size_, char Delimiter_='\n');
    static Ulong GetFieldValue(ifstream& Fin_, char* Buffer_);
    static void ReadTweakFile(char* Fname_, size_t& PreAllocArg_, size_t& MinPoolsArg_);

  protected:
    // nth number in Fibonacci series to start the memory list counter at
    static const size_t FIBSTARTNUM;

    void (*_DefaultHandler)();		// Default memory handler
    void (*_CustomHandler)();		// Custom memory handler

    BOOLTYPE _DefaultHandlerExists;	// Default handler existence flag
    BOOLTYPE _HandlerActive;		// Memory handler active flag

    BOOLTYPE _LowMemory;		// Low memory flag
    MMCounter _Counter;		// Fibonacci counter of memory lists
    size_t _MinPools;		// Minimum pools for each list to allocate
    size_t _TotalLists;		// Total number of memory lists created

    size_t _AllocCalls;        	// Counts number of calls to Allocate method
    size_t _ReallocCalls;	// Counts number of calls to Reallocate method
    size_t _DeallocCalls;	// Counts number of calls to Deallocate method
    size_t _ReleaseAllCalls;	// Counts number of calls to ReleaseAll method

    // Memory list validation method
    BOOLTYPE Validate(MemList* Pool_);

    // Memory list searching method
    MemList* HasList(MemList* TestList_, BOOLTYPE ExactSize_);

    // Low memory indicator and mutator methods
    BOOLTYPE IsLowMemory()
	{ return _LowMemory; }
    void SetLowMemory(BOOLTYPE LoMem_)
	{ _LowMemory = LoMem_; }

    // Returns the specified minimum number of pools for each list
    size_t MinPools()
	{ return _MinPools; }

    // Memory capping and conservation method
    void SetMemoryCap();

    // Memory matrix contructor and destructor
    MemMatrix(size_t MinPools_ = MM_MINPOOLS, size_t PreAlloc_ = MM_PREALLOC,
	      char* Fname_ = MM_TWEAK_FILE);

  public:
    typedef void (*MemoryHandlerType)();

    static const char* ERRMSG_MEMALLOCFAIL;	// Error message for memory allocation fault
    static MemMatrix& Matrix();			// Static instance of memory matrix

    // Memory pool destructor
    ~MemMatrix();

    // Memory allocation, deallocation and reallocation methods
    BOOLTYPE Preallocate(size_t Bytes_);
    VOIDPTR Allocate(size_t Bytes_, BOOLTYPE Placed_=0);
    VOIDPTR Reallocate(VOIDPTR Address_, size_t Bytes_, BOOLTYPE Moveable_=1);
    BOOLTYPE Deallocate(VOIDPTR Address_);
    BOOLTYPE HasThis(VOIDPTR Address_, BOOLTYPE& IsPlaced_);
    BOOLTYPE HasThis(VOIDPTR Address_);
    Ulong ReleaseAll();

    // Convenience methods
    // Allocate and clear to 0
    VOIDPTR Callocate(size_t Bytes_, BOOLTYPE Placed_=0);

    // Suspend/Restore memory handler
    BOOLTYPE IsNewHandlerActive()
	{ return _HandlerActive; }
    void SetNewHandlerActive()
	{ _HandlerActive = TRUE; }
    void SetNewHandlerInactive()
	{ _HandlerActive = FALSE; }

    // Memory handler setting method
    // Returns previously used memory handler
    MemoryHandlerType SetNewHandler(MemoryHandlerType NewHandler_)
	{ MemoryHandlerType PrevHandler_ =
		_CustomHandler ? _CustomHandler:_DefaultHandler;
	  _CustomHandler = NewHandler_;
	  return PrevHandler_; }

    // Method to call memory handler directly to handle memory faults
    void NoMemory();

    // Memory statistics accessor method
    MemStats Statistics();

    // Memory statistics display method
    static ostream& ShowStats(ostream& os, const MemStats& Stats_);
};

// Placement new function
template <class T>
T* PlacementNew(T* Dest_, size_t Sz_)
{
  VOIDPTR Chunk_ = MemMatrix::Matrix().Allocate(sizeof(T) * Sz_, TRUE);

  if (Sz_ > 1)
    Dest_ = ::new (Chunk_) T[Sz_];
  else if (Sz_)
    Dest_ = ::new (Chunk_) T;
  else
    Dest_ = NULL;

  return Dest_;
}

_FNC_INLINE void* RawPlacementNew(void* Dest_, size_t Sz_)
{
  VOIDPTR Chunk_ = MemMatrix::Matrix().Allocate(sizeof(Dest_) * Sz_, TRUE);
  Dest_ = Sz_ ? Chunk_:NULL;
  return Dest_;
}

template <class T>
T* PlacementCopy(const T& Src_)
{
  VOIDPTR Chunk_ = MemMatrix::Matrix().Allocate(sizeof(Src_), TRUE);
  T* Dest_ = ::new (Chunk_) T(Src_);
  return Dest_;
}

inline void* RawPlacementCopy(const void* Src_)
{
  VOIDPTR Dest_ = MemMatrix::Matrix().Allocate(sizeof(Src_), TRUE);
  Dest_ = memmove(Dest_, Src_, sizeof(Src_));
  return Dest_;
}

/****************************************************************************/
#endif




