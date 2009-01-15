#ifndef MEMPOOL_CPP
#define MEMPOOL_CPP
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

/****************************************************************************/
// Memory list relational operators
BOOLTYPE operator == (const MemList& List1_, const MemList& List2_)
{
  return List1_.IsEqual(List2_);
}

/****************************************************************************/
BOOLTYPE operator != (const MemList& List1_, const MemList& List2_)
{
  return (!List1_.IsEqual(List2_));
}

/****************************************************************************/
BOOLTYPE operator >= (const MemList& List1_, const MemList& List2_)
{
  return (List1_.IsEqual(List2_) || List1_.IsGreater(List2_));
}

/****************************************************************************/
BOOLTYPE operator > (const MemList& List1_, const MemList& List2_)
{
  return List1_.IsGreater(List2_);
}

/****************************************************************************/
BOOLTYPE operator <= (const MemList& List1_, const MemList& List2_)
{
  return (List1_.IsEqual(List2_) || List1_.IsLesser(List2_));
}

/****************************************************************************/
BOOLTYPE operator < (const MemList& List1_, const MemList& List2_)
{
  return List1_.IsLesser(List2_);
}

/****************************************************************************/
/****************************************************************************/
// Memory pool equality operator
BOOLTYPE operator == (const MemPool& RefPool_, const MemPool& SrcPool_)
{
  return RefPool_.IsEqual(SrcPool_);
}

/****************************************************************************/
BOOLTYPE operator != (const MemPool& RefPool_, const MemPool& SrcPool_)
{
  return (!RefPool_.IsEqual(SrcPool_));
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Fibonacci counter constructor
//
// PRE:
//   size_t Value_ 	: The starting value to set the counter
//
// POST:
//   The counter is set to the specified starting value
//
MMCounter::MMCounter(size_t Value_):
_Count(0),
_Previous(0)
{
  // if a initial value is specified then find the closest approximation in
  // the Fib series to that value
  if (Value_)
    FindFib(Value_);
}

/****************************************************************************/
// PURPOSE:
//   increment Fib counter to next succeeding number
//
// POST:
//   The counter is set to the next succeeding value
//
size_t MMCounter::Increment()
{
  if (!_Count)
    _Count = 1;			// The 1st Fib number
  else
  {
    size_t Temp_ = _Count;	// Add previous number to current to
    _Count += _Previous;     	// get the next Fib number in the series
    _Previous = Temp_;
  }

  return _Count;
}

/****************************************************************************/
// PURPOSE:
//   Decrement Fib counter to preceeding number
//
// POST:
//   The counter is set to the preceeding value
//
size_t MMCounter::Decrement()
{
  if (!_Previous)
    _Count = 0;				// The 0th Fib number
  else
  {
    size_t Temp_ = _Previous;		// Subtract the previous number
    _Previous = _Count - _Previous;	// from the current to get the
    _Count = Temp_;			// preceeding number in the series
  }

  return _Count;
}

/****************************************************************************/
// PURPOSE:
//   Find closest Fibonacci number that is less than the specified value
//
// PRE:
//   size_t Value_	: The specified value
//
// POST:
//   Returns the closest number that is < Value_
//
size_t MMCounter::FindLesser(size_t Value_)
{
  FindGreaterOrEqual(Value_);
  Decrement();

  return _Count;
}

/****************************************************************************/
// PURPOSE:
//   Find closest Fibonacci number that is greater than the specified value
//
// PRE:
//   size_t Value_	: The specified value
//
// POST:
//   Returns the closest number that is > Value_
//
size_t MMCounter::FindGreater(size_t Value_)
{
  FindLesserOrEqual(Value_);
  Increment();

  return _Count;
}

/****************************************************************************/
// PURPOSE:
//   Find closest Fibonacci number that is less than or equal to the
//   specified value
//
// PRE:
//   size_t Value_	: The specified value
//
// POST:
//   Returns the closest number that is <= Value_
//
size_t MMCounter::FindLesserOrEqual(size_t Value_)
{

  FindGreaterOrEqual(Value_);
  if (Counter() > Value_)
    Decrement();

  return _Count;
}

/****************************************************************************/
// PURPOSE:
//   Find closest Fibonacci number that is greater than or equal to the
//   specified value
//
// PRE:
//   size_t Value_	: The specified value
//
// POST:
//   Returns the closest number that is >= Value_
//
size_t MMCounter::FindGreaterOrEqual(size_t Value_)
{
  for (Reset(); Value_ > Counter(); Increment());
  return _Count;
}

/****************************************************************************/
// PURPOSE:
//   Find the Fib number at the given position indexed in numerical order
//   starting at 0.
//
// PRE:
//   The specified index position of the Fib number to find and return
//
// POST:
//   Returns the Fib number located at the specified index.
//
size_t MMCounter::FindFib(size_t Value_)
{
  Reset();	// Resets the Fib counter

  for (size_t Index_ = 0; Index_ < Value_; Index_++, Increment());

  return _Count;
}

/****************************************************************************/
// PURPOSE:
//   Postfix version of Fib counter increment method
//
// POST:
//   Returns the current value of the Fib counter while incrementing to its
//   next succeeding value
//
size_t MMCounter::operator ++ (int)
{
  size_t Count_ = _Count;
  Increment();
  return Count_;
}

/****************************************************************************/
// PURPOSE:
//   Postfix version of Fib counter decrement method
//
// POST:
//   Returns the current value of the Fib counter while decrementing to its
//   preceeding value
//
size_t MMCounter::operator -- (int)
{
  size_t Count_ = _Count;
  Decrement();
  return Count_;
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   special memory pool constructor used by memory reference and memory
//   request objects to bypass memory block header initialization
//
// PRE:
//   VOIDPTR Address_	: Address of memory block
//   BOOLTYPE Init_	: Initialization flag for memory block header
//
// POST:
//   Creates a memory pool with the given memory address. If the
//   initialization flag is specified TRUE then header of memory block is
//   initialized, otherwise no initialization occurs.
//
MemPool::MemPool(VOIDPTR Address_, BOOLTYPE Init_):
_Space((Byte_t*)Address_)
{
  // if initialization flag is specified then set the header of the
  // memory block to 0 size then increment the memory block pointer to the
  // actual storage area
  if (Init_)
  {
    *((size_t*)_Space) = 0;	// set header of block to 0 to indicate
				// amount of usage

    // increment pointer to actual storage area
    _Space += (sizeof(size_t) + sizeof(MEMPOOL_HEADER_BUFFER));
  }
}

/****************************************************************************/
// PURPOSE:
//   memory pool standard constructor
//
// PRE:
//   size_t Size_ 	: Size of memory block to create
//
// POST:
//   A memory pool is created with a memory block of the specified size.
//   The header of the memory block is initialized with given size.
//
MemPool::MemPool(size_t Size_):
_Space(new Byte_t[Size_ + sizeof(size_t) + sizeof(MEMPOOL_HEADER_BUFFER)])
{
  *((size_t*)_Space) = 0;	// set header of block to 0 to indicate
				// amount of usage

  // increment pointer to actual storage area
  _Space += (sizeof(size_t) + sizeof(MEMPOOL_HEADER_BUFFER));
}

/****************************************************************************/
// PURPOSE:
//   Memory block destructor
//
// POST:
//   deletes the enclosed memory block of this memory pool object
//
MemPool::~MemPool()
{
  if (_Space)
  {
    // decrement to beginning of memory block
    _Space -= (sizeof(size_t) + sizeof(MEMPOOL_HEADER_BUFFER));

    delete[] ((char*)_Space);		// delete memory block and set NULL
    _Space = NULL;
  }
}

/****************************************************************************/
// PURPOSE:
//   Class identity method. Returns whether or not class object is a
//   memory request object.
//
// POST:
//   Returns TRUE if the class object is a memory request object, otherwise
//   returns FALSE.
//
BOOLTYPE MemPool::IsMemRequest() const __VOLATILE
{
  return FALSE;
}

/****************************************************************************/
// PURPOSE
//   Memory pool equality method
//
// PRE:
//   const MemPool& Pool_	: memory pool to be compared with
//
// POST:
//   Returns whether this memory pool object is equivalent the given memory
//   pool object. If one memory pool is a memory request object the other
//   pool must have a free memory block to be equivalent. If neither pools
//   are memory request objects then the address of their memory blocks are
//   compared.
//
BOOLTYPE MemPool::IsEqual(const __VOLATILE MemPool& Pool_) const __VOLATILE
{
/*
  // For matching address ranges -- Doesn't Work!
  VOIDPTR UpperBound_ = _Space + MemSize();
  return
  (
    (_Space <= Pool_._Space && Pool_._Space < UpperBound_) ||
    Pool_.IsMemRequest() && IsFree()
  );
*/
  // matching exact addresses instead
  return
  (
    BOOLTYPE(_Space == Pool_._Space) ||
    Pool_.IsMemRequest() && IsFree()
  );
}

/****************************************************************************/
// PURPOSE:
//   Memory pool resizing method
//
// PRE:
//   size_t Size_	: The size this memory pool object should be resized
//			  to
//
// POST:
//   If the memory block is non-null and the block allocated for use is
//   greater than zero in size then the memory pool is resized to the
//   specified size.
//
VOIDPTR MemPool::Resize(size_t Size_) __VOLATILE
{
  // if memory block is in-use then it is allowed to be resized
  // otherwise return the memory block without doing anything
  if (_Space && MemSize() != 0)
    SetSize(Size_);

  return ((VOIDPTR)_Space);
}

/****************************************************************************/
// PURPOSE:
//   Memory pool usage checkout method
//   All memory requests to use a memory pool object must call this method
//   to check the memory pool out before using it.
//
// PRE:
//   size_t Size_	: The amount of space in the memory pool to be used
//
// POST
//   The memory block header is initialized to the amount of space used
//   then returned. If Size_ is 0 then the pool is not considered in use.
//
VOIDPTR MemPool::Use(size_t Size_, BOOLTYPE Placed_) __VOLATILE
{
  // if memory block is available (0 usage) then it is allowed to be
  // checked-out, otherwise return the memory block without doing anything
  if (_Space && MemSize() == 0)
  {
    SetSize(Size_);
    if (Placed_)
      SetPlaced(TRUE);
  }

  return ((VOIDPTR)_Space);
}

/****************************************************************************/
// PURPOSE:
//   Method to set the memory block header with the give size information
//
// PRE:
//   size_t Size_	: Amount space used of the current memory pool
//
// POST:
//   Memory block header is initialized to the given amount of space used
//
void MemPool::SetSize(size_t Size_) __VOLATILE
{
  // Set the size by assigning a value to the header section of the block
  size_t* Header_ = (size_t*)(_Space - (sizeof(size_t) + sizeof(MEMPOOL_HEADER_BUFFER)));
  *Header_ = Size_;
}

/****************************************************************************/
// PURPOSE:
//   Method to set the memory block buffer with the given
//   placement new information
//
// PRE:
//   Byte_t Placed_	: Flag indicating whether or not block was allocated
//			  through the placement new operator
//
// POST:
//   Memory block buffer is initialized TRUE if block is placed FALSE
//   otherwise.
//
void MemPool::SetPlaced(Byte_t Placed_) __VOLATILE
{
  // Set the placement flag by assigning a value to the buffer
  // section of the block
  Byte_t* PlacementFlag_ = (Byte_t*)(_Space - sizeof(MEMPOOL_HEADER_BUFFER));
  *PlacementFlag_ = Placed_;
}

/****************************************************************************/
BOOLTYPE MemPool::IsPlaced() const __VOLATILE
{
  // Return the size of the memory block by returning the value assigned
  // to the header section of the block
  Byte_t* Header_ = (Byte_t*)(_Space - sizeof(MEMPOOL_HEADER_BUFFER));
  return (*Header_ != 0);
}

/****************************************************************************/
// PURPOSE:
//   Returns the amount of space used of the current memory pool by returning
//   the header portion of the memory block
//
// POST:
//   Returns memory block header as size_t.
//
size_t MemPool::MemSize() const __VOLATILE
{
  // Return the size of the memory block by returning the value assigned
  // to the header section of the block
  size_t* Header_ = (size_t*)(_Space - (sizeof(size_t) + sizeof(MEMPOOL_HEADER_BUFFER)));
  return *Header_;
}

/****************************************************************************/
// PURPOSE:
//   Memory block size accessor method
//   Returns the recorded in-use size of the given memory block
//
// PRE:
//   VOIDPTR Address_	: The given memory block
//
// POST:
//   Returns the recorded in-use size of the given memory block
//
size_t MemPool::SizeOf(VOIDPTR Address_)
{
  // Refer to an allocated piece of memory and return its initialized size
  MemRef Pool_(Address_);
  return Pool_.MemSize();
}

/****************************************************************************/
// PURPOSE:
//   Method to determine if the current memory block fits within the given
//   number of bytes
//
// PRE:
//   size_t Bytes_	: Given number of bytes
//
// POST:
//   If the current memory block fits within the given number of bytes then
//   the current memory block is returned, otherwise NULL is returned
//
VOIDPTR MemPool::MemFit(size_t Bytes_) const __VOLATILE
{
  return ((MemSize() >= Bytes_) ? ((VOIDPTR)_Space):NULL);
}

/****************************************************************************/
// PURPOSE:
//   Method to determine if the given memory block fits within the given
//   number of bytes
//
// PRE:
//   VOIDPTR Address_	: Given memory block
//   size_t Bytes_	: Given number of bytes
//
// POST:
//   If the given memory block fits within the given number of bytes then the
//   given memory block is returned, otherwise NULL is returned
//
VOIDPTR MemPool::MemFit(VOIDPTR Address_, size_t Bytes_)
{
  return ((SizeOf(Address_) >= Bytes_) ? Address_:NULL);
}

/****************************************************************************/
// PURPOSE:
//   ANSI memmove procedure redefined to use memory blocks
//
// PRE:
//   const VOIDPTR Address_	: Source memory block to be moved
//   size_t Bytes_		: The number of bytes to be moved
//
// POST:
//   If the target memory blocks fits within the given number of bytes
//   and the source memory block is non-null then the memmove ANSI C
//   procedure is called to move the memory from the source block to this
//   block. The current memory block is returned if successful, otherwise
//   NULL is returned
//
VOIDPTR MemPool::MemMove(const VOIDPTR Address_, size_t Bytes_) __VOLATILE
{
  return ((MemFit(Bytes_) && Address_) ?
		memmove((VOIDPTR)_Space, Address_, Bytes_):
		NULL);
}

/****************************************************************************/
// PURPOSE:
//   ANSI memchr procedure redefined to use memory blocks
//
// PRE:
//   int Char_	 	: The character to search for
//   size_t Bytes_	: The number of bytes within the memory block to be
//			  searched
//
// POST:
//   If the target memory blocks fits within the given number of bytes
//   then the memchr ANSI C procedure is called to search for the given
//   character in the first Bytes_ bytes of the current memory block
//   The current memory block is returned if successful, otherwise NULL is
//   returned
//
VOIDPTR MemPool::MemChr(int Char_, size_t Bytes_) const __VOLATILE
{
  return (MemFit(Bytes_) ?
		memchr((VOIDPTR)_Space, Char_, Bytes_):
		NULL);
}

/****************************************************************************/
// PURPOSE:
//   ANSI memset procedure redefined to use memory blocks
//
// PRE:
//   int Char_	 	: The character used for setting
//   size_t Bytes_	: The number of bytes within the memory block to be
//			  set with the given character
//
// POST:
//   If the target memory blocks fits within the given number of bytes
//   then the memset ANSI C procedure is called to set the first Bytes_ bytes
//   of the current memory block to the given character. The current memory
//   block is returned if successful, otherwise NULL is returned.
//
VOIDPTR MemPool::MemSet(int Char_, size_t Bytes_) __VOLATILE
{
  return (MemFit(Bytes_) ?
		memset((VOIDPTR)_Space, Char_, Bytes_):
		NULL);
}

/****************************************************************************/
// PURPOSE:
//   ANSI memcmp procedure redefined to use memory blocks
//
// PRE:
//   const VOIDPTR Str2_ 	: Memory block to be compared with
//   size_t Bytes_	: The number of bytes of each block to compare
//
// POST:
//   If the target memory blocks fits within the given number of bytes
//   then the memcmp ANSI C procedure is called to compare the given memory
//   block to the current memory block.
//
//   returns:
//   	þ  < 0 if s1 <  s2
//   	þ  = 0 if s1 == s2
//   	þ  > 0 if s1 >  s2
//
//   otherwise returns 0.
//
int MemPool::MemCmp(const void *Str2_, size_t Bytes_) const __VOLATILE
{
  return ((MemFit(Bytes_) && Str2_) ?
		memcmp((VOIDPTR)_Space, Str2_, Bytes_):
		0);
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Memory reference class constructor
//
// PRE:
//   VOIDPTR Address_	: Address of the memory block to reference
//
// POST:
//   Returns a memory reference object initialized to the given memory
//   block.
//
MemRef::MemRef(VOIDPTR Address_):
MemPool(Address_, FALSE)
{}

/****************************************************************************/
// PURPOSE:
//   Memory reference destructor
//
// POST:
//   Sets the memory block to NULL since the referenced memory block is never
//   intended to be deleted.
//
MemRef::~MemRef()
{
  _Space = NULL;
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Memory request constructor
//
// POST:
//   Essentially the same as a memory reference object with the memory block
//   to be referenced set to NULL since objects of this class are used as
//   free memory pool requesters.
//
MemRequest::MemRequest():
MemRef(NULL)
{}

/****************************************************************************/
// PURPOSE:
//   Class identity method. Returns whether or not class object is a
//   memory request object.
//
// POST:
//   Returns TRUE if the class object is a memory request object, otherwise
//   returns FALSE.
//
BOOLTYPE MemRequest::IsMemRequest() const __VOLATILE
{
  return TRUE;
}

/****************************************************************************/
// PURPOSE
//   Memory request test method
//
// PRE:
//   const MemPool& Pool_	: memory pool to be examined
//
// POST:
//   Returns whether this memory pool object is a free block available for
//   allocation. If one memory pool is a memory request object the other
//   pool must have a free memory block to be equivalent.
//
BOOLTYPE MemRequest::IsEqual(const __VOLATILE MemPool& Pool_) const __VOLATILE
{
  return Pool_.IsFree();
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Memory list constructor.
//
// PRE:
//   MemMatrix* Matrix_ 	: Pointer to enclosing memory matrix
//   size_t Size_		: The size of the memory pools the list
//			          is to allocate
//
// POST:
//   A memory list is created with the specified memory matrix which
//   encloses it. Every memory pool allocated by the memory list has the
//   indicated Size_
//
MemList::MemList(MemMatrix* Matrix_, size_t Size_):
_Matrix(Matrix_),	// pointer to the enclosing memory matrix
_RealTotal(0),		// actual number of pools allocated
_ChunkSize(Size_)	// Prespecified size of pool
{}

/****************************************************************************/
// PURPOSE:
//   Memory statistics method.
//
// PRE:
//   MemStats& Stats_	: Memory statistics structure
//
// POST:
//   Get statistics about the memory manager and stores it in the statistics
//   structure. Statistics includes: Usage, Wastage, Total Space and
//   Free Space.
//
void MemList::MemoryStats(MemStats& Stats_)
{
  size_t Allocated_, Used_, Wasted_, Free_, UseSize_, Overhead_, Header_;
  MMNode<MemPool>* Nodep_ = Head();

  // Amount of space used in each mempool header
  Header_ = (sizeof(size_t) + sizeof(MEMPOOL_HEADER_BUFFER));

  for (Allocated_ = Used_ = Overhead_ = Wasted_ = Free_ = 0;
       Nodep_; Nodep_ = Nodep_->_Next)
  {
    UseSize_ = Nodep_->_Object->MemSize();

    if (UseSize_)
    {
      Used_ += UseSize_;
      Wasted_ += _ChunkSize - UseSize_;
    }
    else
      Free_ += _ChunkSize;

    Overhead_ += Header_;
    Allocated_ += _ChunkSize + Header_;
  }

  Stats_._Allocated 	+= Allocated_;
  Stats_._Used 		+= Used_;
  Stats_._BookKeeping   += Overhead_;
  Stats_._Wasted     	+= Wasted_;
  Stats_._Free 		+= Free_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to validate the given memory pool
//
// PRE:
//   MemPool* Pool_	: The memory pool to be validated
//
// POST:
//   Returns whether or not the memory pool is valid and available for use
//
BOOLTYPE MemList::Validate(MemPool* Pool_)
{
  // Set low memory flag in the memory matrix if pool or its enclosed
  // memory block cannot be allocated
  _Matrix->SetLowMemory(!Pool_ || Pool_->IsNULL());

  // Delete the current pool if it exists
  if (_Matrix->IsLowMemory())
    delete Pool_;

  // return whether low memory flag is set
  return !_Matrix->IsLowMemory();
}

/****************************************************************************/
// PURPOSE:
//   Method to delete a specified number of memory pools from the memory
//   memory list.
//
// PRE:
//   size_t Max_	: The specified number of memory pools to delete
//
// POST:
//   The specified number of memory pools are deleted from the memory list
//
void MemList::DeleteBlocks(size_t Max_)
{
  for (MemRequest Pool_; Max_; Max_--)
  {
    MMNode<MemPool>* Nodep_ = Find(&Pool_);
    Remove(Nodep_);
  }
}

/****************************************************************************/
// PURPOSE:
//   Delete unused memory blocks up to the lowest possible increment of
//   blocks that still contains unused memory
//
// POST:
//   Unused memory blocks are deleted up to the lowest possible increment of
//   the Fib number series which still contain unused memory. This is done to
//   conserve memory that would otherwise be taking up unused space or space
//   that was once used but no longer necessary.
//
size_t MemList::SetMemoryCap()
{
  // find the initial free memory block in the list
  MemRequest Pool_;
  MMNode<MemPool>* Nodep_ = Find(&Pool_);
  MMListBrowser<MemPool> ListBrowser_(Nodep_);

  if (Nodep_)
  {
    // traverse down the list from the initial free memory block to get
    // all existing free blocks
    size_t FreeBlocks_ = 1;
    for (;;)
      if (Nodep_ = ListBrowser_.NextMatch(&Pool_))
	FreeBlocks_++;
      else
	break;

    // Get the total used blocks by subtracting the total allocated by the
    // total free blocks
    size_t UsedBlocks_ = _RealTotal - FreeBlocks_;

    // Find the minimum Fib number in the series which can enclose all
    // in-use blocks, but is less than the total number of blocks allocated
    for (_Counter.Reset(); _Counter.Counter() <= UsedBlocks_; ++_Counter);

    // Delete all excess free blocks if any and modify the running total
    size_t Diff_ = _Counter.Counter() - UsedBlocks_;
    if (FreeBlocks_ > Diff_)
    {
      DeleteBlocks(FreeBlocks_ - Diff_);
      _RealTotal = _Counter.Counter();
    }

    // return the total number of in-use blocks
    return UsedBlocks_;
  }

  // if no free blocks are found then all allocated blocks must be in-use
  // therefore return the actual running total
  return _RealTotal;
}

/****************************************************************************/
// PURPOSE:
//   Method to reallocate a memory block to its new size.
//
// PRE:
//   VOIDPTR Address_	: The address of the given memory block
//   size_t Bytes_	: The new size to reallocate the given block to
//   BOOLTYPE Moveable_ : The contents of the given memory block could be
//			  moved to another block of similar size, but with
//			  a different address: 1 for (TRUE), 0 for (FALSE)
//
// POST:
//   The given memory block is reallocated to its new size. This could either
//   be smaller or larger to its present block size. The reallocated block
//   is then returned.
//
//   WARNING: since the allocatable block size for each memory list is
//   predefined to a particular size at the time of construction of the list
//   the new reallocated block depending on its size may be transferred to
//   another memory block in another list for efficiency and book keeping
//   reasons so the address of the memory block cannot be guaranteed to stay
//   constant if a reallocation operation is done.
//
//   You can specify the Moveable_ flag as FALSE to ensure that the
//   reallocated block has the same address as the original block.
//   If this can't be done then the Reallocate function will return NULL.
//
VOIDPTR MemList::Reallocate(VOIDPTR Address_, size_t Bytes_, BOOLTYPE Moveable_, BOOLTYPE& Found_)
{
  size_t BufSize_ = 0;		// new block size

  // create a memory reference to the specified memory pool and
  // search for it in the memory list
  MemRef DestPool_(Address_);
  MemPool* Result_ = MMList<MemPool>::Deref(Find(&DestPool_));
  Found_ = Result_ != NULL;

  if (Found_)
  {
    size_t OldSize_ = Result_->MemSize();	// old block size

    // if the new block size is less than or equal to the prespecified
    // memory block size then. No reallocation of zero-sized blocks allowed.
    if (Bytes_ <= _ChunkSize)
    {
      MMCounter SizeCount_;  		// the Fib size count

      // if the given memory block is specified as moveable and
      // if the new block size is less than the old block size and it fits
      // in a smaller prespecified memory block in the Fib series then the
      // existing block should be transferred over to the smaller memory block
      if (Moveable_ &&
          ObjRelation<size_t>::XInRange(Bytes_, size_t(0), OldSize_, SortUsingBuiltInOperators<size_t>::Trait()) &&
          SizeCount_.FindGreaterOrEqual(Bytes_) < _ChunkSize)
        BufSize_ = Bytes_;
      else
        // If the size of the new block is specified as a non-zero value
        // then do a resize operation on the existing block and return it
        // otherwise check in the old memory block to the memory manager and
        // return NULL
        if (Bytes_)
          return Result_->Resize(Bytes_);
        else
          Result_->Return();
    }
    else
      // otherwise the new block size is the same as the old block size
      BufSize_ = OldSize_;
  }

  // if the new non-zero block size has been specified and the given memory
  // block is specified as moveable then
  if (BufSize_ && Moveable_)
  {
    // allocate a new memory block from the memory matrix with the new size
    VOIDPTR NewSpace_ = _Matrix->Allocate(Bytes_);

    // if allocation was successful then transfer the contents of the old
    // memory block to the new memory block. The old memory block is checked
    // in to the memory manager. The new memory block is checked out of the
    // memory manager and returned to the caller.
    if (NewSpace_)
    {
      memmove(NewSpace_, Address_, BufSize_);
      Result_->Return();
      return NewSpace_;
    }
  }

  // otherwise if a new non-zero block size has not been specified or
  // the contents of the memory block cannot be moved to a new address
  // then return NULL
  return NULL;
}

/****************************************************************************/
// PURPOSE:
//   This method preallocates the minimum number of pools specified by the
//   MemMatrix::_MinPools data member for this memory list.
//
//   Doing this relieves the memory manager from doing requesting memory
//   from the system and have it simply return the preallocated memory to
//   the calling program. But judge carefully because preallocating too much
//   may be a drain on memory resources and defeats the purpose of the
//   memory manager.
//
// POST:
//   Returns TRUE if preallocation was successful, otherwise returns FALSE
//
BOOLTYPE MemList::Preallocate()
{
  // create a memory request for an available memory block
  // then search for an available memory block
  MemRequest NullPool_;
  MemPool* Result_ = MMList<MemPool>::Deref(Find(&NullPool_));

  // if the search was unsuccessful and the memory low flag is not set
  // allocate additional memory pools
  if (Result_ == NULL && !_Matrix->IsLowMemory())
  {
    // Count up to the minimum number of pools to maintain
    // or count up to the next Fib number in the series
    do
      ++_Counter;
    while (_Counter.Counter() < _Matrix->MinPools());

    // while the actual running total for memory pools maintained is less
    // than the Fib counter value
    for (BOOLTYPE Capped_ = FALSE; _RealTotal < _Counter.Counter();)
    {
      // allocate a new memory pool with the prespecified block size
      MemPool* NewPool_ = new MemPool(_ChunkSize);

      // validate the new memory pool and append it to the head of the list
      if (Validate(NewPool_))
      {
	AppendHead(NewPool_);
	++_RealTotal;
      }

      // if the memory pool is invalid then if memory capping has not been
      // done then do a memory cap operation, otherwise break from loop
      else if (!Capped_)
      {
	_Matrix->SetMemoryCap();	// cap the memory in the memory matrix
	Capped_ = TRUE;			// memory cap operation has been done
      }
      else
	break;
    }

    // try again to find an available memory block
    Result_ = MMList<MemPool>::Deref(Find(&NullPool_));
  }

  // if an available memory block has been found then return TRUE
  if (Result_)
    return TRUE;

  // otherwise return FALSE to indicate memory allocation failure
  return FALSE;
}

/****************************************************************************/
// PURPOSE:
//   Method to allocate a block of memory from the list's growing collection
//   of same sized memory pools and check it out for use.
//
// PRE:
//   size_t Bytes_	: The size of the memory block to be allocated.
//   BOOLTYPE Placed_	: Flag indicating whether to set the block as
//                        being allocated through the placement new operator
//
// POST:
//   Returns the allocated and checked out block of memory if successful,
//   otherwise NULL is returned. The placement new flag in the memory block
//   is set if the Placed_ argument is set as TRUE.
//
VOIDPTR MemList::Allocate(size_t Bytes_, BOOLTYPE Placed_)
{
  // create a memory request for an available memory block
  // then search for an available memory block
  MemRequest NullPool_;
  MemPool* Result_ = MMList<MemPool>::Deref(Find(&NullPool_));

  // if the search was unsuccessful and the memory low flag is not set
  // allocate additional memory pools
  if (Result_ == NULL && !_Matrix->IsLowMemory())
  {
    // Count up to the minimum number of pools to maintain
    // or count up to the next Fib number in the series
    do
      ++_Counter;
    while (_Counter.Counter() < _Matrix->MinPools());

    // while the actual running total for memory pools maintained is less
    // than the Fib counter value
    for (BOOLTYPE Capped_ = FALSE; _RealTotal < _Counter.Counter();)
    {
      // allocate a new memory pool with the prespecified block size
      MemPool* NewPool_ = new MemPool(_ChunkSize);

      // validate the new memory pool and append it to the head of the list
      if (Validate(NewPool_))
      {
	AppendHead(NewPool_);
	++_RealTotal;
      }

      // if the memory pool is invalid then if memory capping has not been
      // done then do a memory cap operation, otherwise break from loop
      else if (!Capped_)
      {
	_Matrix->SetMemoryCap();	// cap the memory in the memory matrix
	Capped_ = TRUE;			// memory cap operation has been done
      }
      else
	break;
    }

    // try again to find an available memory block
    Result_ = MMList<MemPool>::Deref(Find(&NullPool_));
  }

  // if an available memory block has been found then
  // check it out for use and return it
  if (Result_)
    return Result_->Use(Bytes_, Placed_);

  // otherwise return NULL to indicate memory allocation failure
  return NULL;
}

/****************************************************************************/
// PURPOSE:
//   Method to deallocate a memory pool that has been checked out for use by
//   the Allocate method. The deallocated pool is checked in and maintained
//   by the memory list for future use.
//
// PRE:
//   MemPool* Pool_	: The memory pool that is no longer used and is
//			  to be dealocated and checked in.
//
// POST:
//   The given memory pool is checked in and maintained by the current
//   memory list for future use.
//
BOOLTYPE MemList::Deallocate(MemPool* Pool_)
{
  // Search for the given memory pool in the list
  MemPool* Result_ = MMList<MemPool>::Deref(Find(Pool_));

  // if the memory pool was found then return it to the list as a free block
  if (Result_)
  {
    Result_->Return();
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
// PURPOSE:
//   Method to check the memory manager for an allocated block with the
//   specified address.
//
// PRE:
//   VOIDPTR Address_	: The memory block to search for
//
// POST:
//   The memory manager is searched for an allocated block of the given
//   address. If no blocks matches the given address the function returns
//   0 (FALSE), otherwise it returns 1 (TRUE).
//
BOOLTYPE MemList::HasThis(MemPool* Pool_, BOOLTYPE& IsPlaced_)
{
  // Search for the given memory pool in the list
  MemPool* Result_ = MMList<MemPool>::Deref(Find(Pool_));

  // if the memory pool was found then return 1 (TRUE), otherwise
  // return 0 (FALSE).
  if (Result_)
  {
    IsPlaced_ = Result_->IsPlaced();
    return TRUE;
  }

  return FALSE;
}

/****************************************************************************/
// PURPOSE:
//   Method to release all memory within the memory list without need to
//   reference the address of memory blocks. The released blocks stays within
//   the memory manager encoded with zero usage size but are not actually
//   deleted.
//
//   Note:
//     Use with caution. If "released" memory is still in use the memory
//     manager will still treat it as a free memory block. Memory management
//     will be corrupted and tracking of free and in-use blocks impossible.
//
// PRE:
//   Memory list in its current state.
//   In-use and free memory blocks are encoded with their usage size.
//
// POST:
//   All memory within the memory list is freed. All free blocks are
//   encoded with zero usage size.
//
Ulong MemList::ReleaseAll()
{
  Ulong TotalReleased_ = 0;
  MMListBrowser<MemPool> ListBrowser_(Head());

  for (size_t Index_ = 0; Index_ < _RealTotal; Index_++)
  {
    MMNode<MemPool>* Nodep_ = ListBrowser_++;

    if (Nodep_)
    {
      MemPool* PoolPtr_ = MMList<MemPool>::Deref(Nodep_);
      size_t PoolSize_;

      if (PoolPtr_ && (PoolSize_ = PoolPtr_->MemSize()))
      {
	TotalReleased_ += PoolSize_;
	PoolPtr_->Return();
      }
    }
  }

  return TotalReleased_;
}

/****************************************************************************/
// PURPOSE:
//   Method to delete all memory blocks from the current memory lists
//
// POST:
//   All memory blocks maintained by the current memory list is removed from
//   the list and deleted. The memory list counter variables are reset to
//   their initial values.
//
void MemList::DeleteAll()
{
  // While the list is not empty continue removing memory pools from the list
  while (!Empty())
    Remove(Head());

  _RealTotal = 0;	// Reset the running total to 0
  _Counter.Reset();	// Reset the Fib counter to 0
}

/****************************************************************************/
/****************************************************************************/
// nth number in Fibonacci series to start the memory list counter at
const size_t MemMatrix::FIBSTARTNUM = 2;

// Error message string to be displayed in case of memory allocation failure
const char* MemMatrix::ERRMSG_MEMALLOCFAIL = "Memory Allocation Failure: MemMatrix Memory Manager";

// Win32 CRITICAL_SECTION Object initialization flag
#ifdef __MM_WIN32THREADS
  Boolean MemMatrix::Lock_Initialized = FALSE;
#endif

/****************************************************************************/
// PURPOSE:
//   Memory failure handler, New operator handler
//
// POST:
//   Displays an error message in case of memory allocation failure and
//   exits the program.
//
void MemAllocFailHandler()
{
  SHOWFATAL(MemMatrix::ERRMSG_MEMALLOCFAIL);
}

/****************************************************************************/
// PURPOSE:
//   Universal delete procedure. Tests the address given as to whether it
//   is stored in the memory manager. If it is a valid address stored within
//   the memory manager then it is deallocated through the
//   MemMatrix::Deallocate method of the memory manager. If the given address
//   is not stored within the memory manager then it is deleted using the
//   standard C++ delete operator. (if MM_USE_EXCLUSIVELY flag is not active)
//
//   Note: This function is best used with primitive types, but not for
//         classes or other aggregate structures. The latter could handle
//         memory deallocation through overriding the operator delete
//         definition within its class. The MemMatrix::Deallocate function
//	   would then be called from the operator delete method definition.
//         For primitive types which does not allow for operator overloading,
//	   this function can be called to deallocate memory allocated from an
//	   indeterminate source.
//
// PRE:
//   VOIDPTR Address_ 	: The address of an allocated memory block
//
// POST:
//   If the allocated memory block was allocated through the memory manager
//   then it is released back to the memory manager using the
//   MemMatrix::Deallocate method. If the given address is not found within
//   the memory manager then it is deleted using the standard C++ delete
//   operator. The function returns TRUE if the given address is found
//   within the memory manager, otherwise it returns FALSE.
//
BOOLTYPE RawDelete(VOIDPTR Address_)
{
#if OVERLOAD_NEW
  if (!MemMatrix::Matrix().Deallocate(Address_))
  {
    #if !MM_USE_EXCLUSIVELY
      delete ((char*)Address_);
    #endif
    return FALSE;
  }
#else
  delete ((char*)Address_);
#endif

  return TRUE;
}

/****************************************************************************/
// PURPOSE:
//   Universal delete procedure. Tests the address given as to whether it
//   is stored in the memory manager. If it is a valid address stored within
//   the memory manager then it is deallocated through the
//   MemMatrix::Deallocate method of the memory manager. If the given address
//   is not stored within the memory manager then it is deleted using the
//   standard C++ delete operator. (if MM_USE_EXCLUSIVELY flag is not active)
//
//   Note: This function is best used with primitive types, but not for
//         classes or other aggregate structures. The latter could handle
//         memory deallocation through overriding the operator delete
//         definition within its class. The MemMatrix::Deallocate function
//	   would then be called from the operator delete method definition.
//         For primitive types which does not allow for operator overloading,
//	   this function can be called to deallocate memory allocated from an
//	   indeterminate source.
//
// PRE:
//   VOIDPTR Address_ 	: The address of an allocated memory block
//
// POST:
//   If the allocated memory block was allocated through the memory manager
//   then it is released back to the memory manager using the
//   MemMatrix::Deallocate method. If the given address is not found within
//   the memory manager then it is deleted using the standard C++ delete
//   operator. The function returns TRUE if the given address is found
//   within the memory manager, otherwise it returns FALSE.
//
BOOLTYPE RawDeleteArray(VOIDPTR Address_)
{
#if OVERLOAD_NEW
  if (!MemMatrix::Matrix().Deallocate(Address_))
  {
    #if !MM_USE_EXCLUSIVELY
      delete[] ((char*)Address_);
    #endif
    return FALSE;
  }
#else
  delete[] ((char*)Address_);
#endif

  return TRUE;
}

/****************************************************************************/
// PURPOSE:
//   User selectable allocation method. Depending on what was passed for
//   Method_ for the allocation method, the procedure either allocates memory
//   using the standard C++ new operator or the MemMatrix::Allocate memory
//   allocation method.
//
// PRE:
//   size_t Bytes_ : The size of the block to be allocated in bytes.
//
// POST:
//   If Method_ == STD_NEW then the standard C++ new operator is used to
//   allocate Bytes_ worth of memory. Otherwise the MemMatrix::Allocate
//   method is used to allocate Bytes_ worth of memory.
//
VOIDPTR RawAllocateWith(MemAllocMethod Method_, size_t Bytes_)
{
#if OVERLOAD_NEW
  if (Method_ == STD_NEW)
    return (Bytes_ ? (new Byte_t[Bytes_]):NULL);

  return MemMatrix::Matrix().Allocate(Bytes_);
#else
  return (Bytes_ ? (new Byte_t[Bytes_]):NULL);
#endif
}

/****************************************************************************/
VOIDPTR RawReallocateWith(MemAllocMethod Method_, VOIDPTR Address_, size_t Bytes_, BOOLTYPE Moveable_)
{
#if OVERLOAD_NEW
  if (Method_ == STD_NEW)
    if (Moveable_)
    {
      if (Bytes_)
      {
	VOIDPTR Temp_ = (Bytes_ > 1) ? (new Byte_t[Bytes_]):(new Byte_t);
	if (Temp_)
	  memmove(Temp_, Address_, Bytes_);
	if (!RawDelete(Address_))
	  delete ((char*)Address_);
	Address_ = Temp_;
      }
      return Address_;
    }
    else
      return NULL;

  return MemMatrix::Matrix().Reallocate(Address_, Bytes_, Moveable_);

#else
  if (Bytes_)
  {
    VOIDPTR Temp_ = (Bytes_ > 1) ? (new Byte_t[Bytes_]):(new Byte_t);
    if (Temp_)
      memmove(Temp_, Address_, Bytes_);
    RawDelete(Address_);
    Address_ = Temp_;
  }
  return Address_;
#endif
}

/****************************************************************************/
#if DEFINE_PLACEMENT_NEW
void* operator new (size_t, void* Addr_)
{
  return Addr_;
}
#endif

/******************************************************************************/
/******************************************************************************/
MMPathTokenizer::StringTokenizer::StringTokenizer():
_SearchStr(NULL),
_Delims(NULL),
_WhiteSpace(NULL),
_Index(0),
_Start(0)
{}

/****************************************************************************/
MMPathTokenizer::StringTokenizer::StringTokenizer(const char* Str_, size_t Start_, const char* Delims_, const char* Wspc_):
_SearchStr(Str_),
_Delims(Delims_),
_WhiteSpace(Wspc_),
_Index(Start_),
_Start(Start_)
{}

/****************************************************************************/
char* MMPathTokenizer::StringTokenizer::RemovePadding(char* str, const char* chset)
{
  size_t x, l;

  for (l = 0; str[l] && InCharSet(str[l], chset); l++);
  for (x = 0; (str[x] = str[l]); x++, l++);

  for (size_t l = strlen(str); l;)
    if (InCharSet(str[--l], chset))
      str[l] = '\0';
    else
      break;

  return str;    
}

/****************************************************************************/
Boolean MMPathTokenizer::StringTokenizer::InCharSet(char ch, const char* chset)
{
  if (chset == NULL || *chset == 0)
    return FALSE;

  if (*chset == '0' && ch == 0)
    return TRUE;

  if (*chset == '-' || *chset == '0')
    ++chset;

  for (;*chset; chset++)
    if (ch == *chset)
      return TRUE;

  return FALSE;
}

/****************************************************************************/
size_t MMPathTokenizer::StringTokenizer::FindNextWord(const char* Str_, const char* Delims_, size_t& Index_, size_t& NextPos_)
{
  size_t RunLen_ = (Str_ ? ::strlen(Str_):0);

  if (!Str_)
    return 0;

  for (;Index_ < RunLen_ && InCharSet(Str_[Index_], Delims_); Index_++);
  NextPos_ = Index_;

  for (;NextPos_ < RunLen_; NextPos_++)
    if (InCharSet(Str_[NextPos_], Delims_))
      break;

  return (NextPos_ - Index_);
}

/****************************************************************************/
size_t MMPathTokenizer::StringTokenizer::FindToken(char*& Token_, Boolean AllocMemory_)
{
  const char* Delimiters_ = (_Delims == NULL) ? " ":_Delims;

  size_t NextPos_;
  size_t CopyLen_ = FindNextWord(_SearchStr, Delimiters_, _Index, NextPos_);

  if (CopyLen_)
  {
    if (AllocMemory_)
      Token_ = new char[CopyLen_ + 1];

    strncpy(Token_, &_SearchStr[_Index], CopyLen_);
    Token_[CopyLen_] = 0;

    if (_WhiteSpace)
    {
      RemovePadding(Token_, _WhiteSpace);
      CopyLen_ = strlen(Token_);
    }

    _Index = NextPos_;
  }

  return CopyLen_;
}

/******************************************************************************/
/******************************************************************************/
MMPathTokenizer::MMPathTokenizer():
_Max(0),
_Size(10),
_TokenArray(new char*[10]),
#if (defined(__linux__) | defined(__unix__)) & !defined(__DJGPP__)
  _TokenSeparator(":"),
  _DirSeparator("/")
#else
  _TokenSeparator(";"),
  _DirSeparator("\\")
#endif
{}

/******************************************************************************/  
MMPathTokenizer::MMPathTokenizer(const char* VarName_, int SysType_):
_Max(0),
_Size(10),
_TokenArray(new char*[10]),
_TokenSeparator((SysType_ == DOS_PATH) ? ";":":"),
_DirSeparator((SysType_ == DOS_PATH) ? "\\":"/")
{
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = strcpy(new char[strlen(EnvStrVal_) + 1], EnvStrVal_);
    Tokenize(NewStr_);
    delete[] NewStr_;
  }
}

/******************************************************************************/
MMPathTokenizer::MMPathTokenizer(const char* VarName_, const char* TokSep_, const char* DirSep_):
_Max(0),
_Size(10),
_TokenArray(new char*[10]),
_TokenSeparator(TokSep_),
_DirSeparator(DirSep_)
{
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = strcpy(new char[strlen(EnvStrVal_) + 1], EnvStrVal_);
    Tokenize(NewStr_);
    delete[] NewStr_;
  }
}

/******************************************************************************/
MMPathTokenizer::~MMPathTokenizer()
{
  size_t i;
  for (i = 0; i < _Max; i++)
    delete[] _TokenArray[i];

  delete[] _TokenArray;
}

/******************************************************************************/
void MMPathTokenizer::Tokenize(char* Str_)
{
  // testing string tokenizing
  char* Buffer_ = new char[300];
  MMPathTokenizer::StringTokenizer Tok_(Str_, 0, _TokenSeparator, NULL);
  size_t i;

  for (;;)
  {
    for (i = 0; i < 10 && Tok_.FindToken(Buffer_); i++)
      _TokenArray[_Max++] = strcpy(new char[strlen(Buffer_) + 1], Buffer_);
    
    if (i >= 10)
    {
      char** OldArray_ = _TokenArray;
      size_t OldSize_ = _Size;
      size_t x;

      _Size += 10;
      _TokenArray = new char*[_Size];
    
      for (x = 0; x < OldSize_; x++)
        _TokenArray[x] = OldArray_[x];

      delete[] OldArray_;
    }
    else
      break;
  }

  delete[] Buffer_;
}

/******************************************************************************/  
void MMPathTokenizer::SetEnvVar(const char* VarName_, const char* TokSep_, const char* DirSep_)
{
  _TokenSeparator = TokSep_;
  _DirSeparator = DirSep_;
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = strcpy(new char[strlen(EnvStrVal_) + 1], EnvStrVal_);
    Tokenize(NewStr_);
    delete[] NewStr_;
  }
}

/******************************************************************************/  
void MMPathTokenizer::SetEnvVar(const char* VarName_, int SysType_)
{
  _TokenSeparator = (SysType_ == DOS_PATH) ? ";":":";
  _DirSeparator = (SysType_ == DOS_PATH) ? "\\":"/";
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = strcpy(new char[strlen(EnvStrVal_) + 1], EnvStrVal_);
    Tokenize(NewStr_);
    delete[] NewStr_;
  }
}

/******************************************************************************/
size_t MMPathTokenizer::Max() const
{
  return _Max;
}

/******************************************************************************/
const char* MMPathTokenizer::TokenSeparator() const
{
  return _TokenSeparator;
}

/******************************************************************************/
const char* MMPathTokenizer::DirSeparator() const
{
  return _DirSeparator;
}

/******************************************************************************/
void MMPathTokenizer::Dump(ostream& Out_) const
{
  size_t x;
  for (x = 0; x < _Max; x++)
    Out_ <<_TokenArray[x] <<endl;
}

/******************************************************************************/
const char* MMPathTokenizer::operator [] (size_t Index_) const
{
  return ((Index_ < _Max) ? _TokenArray[Index_]:NULL);
}

/****************************************************************************/
void MMPathTokenizer::CallMethod(MMPathTokenizer& Obj_, const char* MethodName_, const char* VarName_, int SysType_)
{
  if (!strcmp(MethodName_, "SetEnvVar(const char*, int)"))
    Obj_.SetEnvVar(VarName_, SysType_);
}

/****************************************************************************/
/****************************************************************************/
// PURPOSE:
//   Procedure to trim string of leading and trailing spaces and convert to
//   lower case all other characters.
//
// PRE:
//   char* str	: String to be trimmed and converted
//
// POST:
//   Returns the string trimmed of leading and trailing white spaces and
//   converted to lower case.
//
char* MemMatrix::TrimAndDnCase(char* str)
{
  size_t x, l;

  for (l = strlen(str); l;)
    if (isspace(str[--l]))
      str[l] = '\0';
    else
      break;

  for (l = 0; str[l] && isspace(str[l]); l++);
  for (x = 0; (str[x] = tolower(str[l])); x++, l++);

  return str;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to extract character from the input stream until either the
//   delimiter character is detected or end of file is detected. If the
//   input buffer is filled and end of file or the delimiter character has
//   not been retrieved then the function will read and throw away characters
//   until either the delimiter is read or end of file is detected.
//
// PRE:
//   istream& In_	: The input stream
//   char* Dest_	: The input buffer
//   int Size_		: The size of the input buffer
//   char Delimiter_	: The delimiter character
//
// POST:
//   Characters are extracted and placed into the input buffer until either
//   the delimiter character is read or end of file is detected. If the
//   input buffer is filled then character will be read and thrown away.
//   returns the input buffer
//
char* MemMatrix::RdStr(istream& In_, char* Dest_, int Size_, char Delimiter_)
{
  int EndOfRead_;

  In_.getline(Dest_, Size_, Delimiter_);
  if (strlen(Dest_) == size_t(Size_ - 1))
    EndOfRead_ = In_.eof();
  else
    EndOfRead_ = 1;

  if (!EndOfRead_)
    for (size_t Count_ = 0; !In_.eof() && In_.get() != Delimiter_; Count_++);

  return Dest_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to get the field value after the field name in the tweak file
//
// PRE:
//   char* Buffer_	: characater buffer
//   ifstream Fin_	: Input stream
//
// POST:
//   Reads string after "=" sign separating field name/field value pair and
//   converts it into an unsigned long value and returns it.
//
Ulong MemMatrix::GetFieldValue(ifstream& Fin_, char* Buffer_)
{
  Ulong FieldVal_;
  char *ValStr_, *Endp_;

  ValStr_ = TrimAndDnCase(RdStr(Fin_, Buffer_, 80));

  if (!(ValStr_ &&
       (FieldVal_ = strtoul(ValStr_, &Endp_, 10)) != 0 &&
       ((isspace(*Endp_) || *Endp_ == '\0') && ValStr_ != Endp_)))
    FieldVal_ = 0;

  return FieldVal_;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to read the tweak file and parse field value arguments from
//   it. The field values include the preallocation maximum pool size and
//   the minimum maintainable pools per memory list.
//
// PRE:
//   char* Fname_		: File name of the tweak file
//   size_t& PreAllocArg_       : The preallocation maximum pool size
//   size_t& MinPoolsArg_	: Minimum number of maintainable pools per
//				  memory list.
//
// POST:
//   Stores into PreAllocArg_ and MinPoolsArg_ their respective field values
//   parsed from the tweak file. If no valid field values are retrieved for
//   a particular field the field is assigned a value of 0 by default.
//
void MemMatrix::ReadTweakFile(char* Fname_, size_t& PreAllocArg_, size_t& MinPoolsArg_)
{
  char Buffer_[80];
  ifstream Fin_;
    
  MMPATHTOKENIZER(Pathtok_, "PATH");  
  size_t Max_ = Pathtok_.Max();
  size_t i;

  char* AbsPath_ = new char[300 + strlen(Fname_)];
  strcpy(AbsPath_, Fname_);
  Fin_.open(AbsPath_, ios::in);

  for (i = 0; !Fin_.good() && i < Max_; i++)
  {
    strcpy(AbsPath_, Pathtok_[i]);
    strcat(AbsPath_, Pathtok_.DirSeparator());
    strcat(AbsPath_, Fname_);
    Fin_.clear();
    Fin_.open(AbsPath_, ios::in);
  }

  if (!Fin_.good())
  {
    delete[] AbsPath_;

    PreAllocArg_ = 100;
    MinPoolsArg_ = 5;
    return;
  }

  char* Field_;
  int FieldNo_ = 0;
  BOOLTYPE FieldDone_[2];

  PreAllocArg_ = MinPoolsArg_ = 0;
  FieldDone_[0] = FieldDone_[1] = FALSE;

  if (Fin_.good() && !Fin_.eof())
    if (strcmp(TrimAndDnCase(RdStr(Fin_, Buffer_, 80)), "[memmatrix]") == 0)
      while (!Fin_.eof() && FieldNo_ < 2)
      {
	     Field_ = TrimAndDnCase(RdStr(Fin_, Buffer_, 80, '='));

	     if (!strcmp(Field_, "preallocate" ) && FieldDone_[0] == FALSE)
	     {
	       PreAllocArg_ = GetFieldValue(Fin_, Buffer_);
	       FieldDone_[0] = TRUE;
	       ++FieldNo_;
        }
	     else if (!strcmp(Field_, "minpools" ) && FieldDone_[1] == FALSE)
	     {
	       MinPoolsArg_ = GetFieldValue(Fin_, Buffer_);
	       FieldDone_[1] = TRUE;
	       ++FieldNo_;
        }
	     else
	       RdStr(Fin_, Buffer_, 80);
      }

  delete[] AbsPath_;      
}

/****************************************************************************/
// PURPOSE:
//   Memory matrix constructor
//
// PRE:
//   size_t MinPools_	: The minimum number of pools each memory list should
//			  have. (Might be ignored if Fname_ is non-NULL)
//   char* Fname_	: The tweak file name. This files contains the
//			  arguments given to the memory manager such as
//			  minimum pool size per list and preallocation max
//			  pool size. Used for fine tuning the pool for
//			  optimal performance.
//
// POST:
//   Constructs an instance of the memory matrix class
//   Each memory list constructed by this instance of the memory matrix will
//   have the specified minimum number of memory pools MinPools_ once
//   they are constructed. The new handler is also replaced with a NULL
//   handler. If the original new handler is NULL then a default handler
//   will be assigned that will display a message and end the program in
//   case of memory allocation failure.
//
MemMatrix::MemMatrix(size_t MinPools_, size_t PreAlloc_, char* Fname_):
_LowMemory(FALSE),              // Memory low indicator flag
_Counter(FIBSTARTNUM),		// Initialize counter to 2nd Fib number
_MinPools(MinPools_),		// Initialize the minimum pools to maintain
_TotalLists(0),			// Initialize the total number of lists
_DefaultHandler(NULL),		// Set the default handler to NULL
_CustomHandler(NULL),		// Set the custom handler to NULL
_DefaultHandlerExists(FALSE),	// Default handler exists flag
_HandlerActive(TRUE),		// Memory handler active flag
_AllocCalls(0),			// Count number of calls to Allocate method
_ReallocCalls(0),		// Count number of calls to Reallocate method
_DeallocCalls(0),		// Count number of calls to Deallocate method
_ReleaseAllCalls(0)		// Count number of calls to ReleaseAll method
{
  // If tweak file name exists then read tweak file
  if (Fname_)
  {
    size_t PreAllocArg_;	// Preallocation maximum pool size
    size_t MinPoolsArg_;	// Minimum maintainable pools per list

    // Read tweak file and parse field value into variables
    ReadTweakFile(Fname_, PreAllocArg_, MinPoolsArg_);

    // Replace value for _MinPools if field value exists and non-zero
    if (MinPoolsArg_)
      _MinPools = MinPoolsArg_;

    // Do preallocation of memory pools if field value exists and non-zero
    if (PreAllocArg_)
      PreAlloc_ = PreAllocArg_;
  }

  // If the PreAllocation value is non-zero then preallocate a series of
  // memory pools up to the maximum sized pool specified by PreAlloc_
  if (PreAlloc_)
    Preallocate(PreAlloc_);

  // set new handler to NULL and return old handler
  _DefaultHandler = set_new_handler(0);

  // if no default handler exists then set it to custom handler
  if (!_DefaultHandler)
  {
    _DefaultHandler = MemAllocFailHandler;
    _DefaultHandlerExists = FALSE;
  }
  else
    _DefaultHandlerExists = TRUE;
}

/****************************************************************************/
// PURPOSE:
//   Memory matrix destructor
//
// POST:
//   Destroys the memory matrix and reassigns the original handler if exists
//
MemMatrix::~MemMatrix()
{
  // if the default handler exists then reset the new handler to it,
  // otherwise reset the new handler to NULL
  if (_DefaultHandlerExists)
    set_new_handler(_DefaultHandler);
  else
    set_new_handler(0);
}

/****************************************************************************/
// PURPOSE:
//   Memory list validation method
//
// PRE:
//   MemList* List_	: The newly allocated memory list to be validated
//
// POST:
//   If the memory list pointer is non-null then the memory list is
//   successfully allocated and the method returns TRUE. If the memory list
//   pointer is NULL then allocation of the memory list had failed and the
//   method returns FALSE.
//
BOOLTYPE MemMatrix::Validate(MemList* List_)
{
  // Low memory condition is true if the memory list fail in allocation
  _LowMemory = !List_;
  return !_LowMemory;
}

/****************************************************************************/
// PURPOSE:
//   Method to cap the memory resources of the memory matrix by freeing up
//   unused memory blocks.
//
// POST:
//   The maximum amount of unused memory blocks are released to conserve
//   memory used by the memory manager.
//
void MemMatrix::SetMemoryCap()
{
  // for each memory list contain in the memory matrix call its
  // memory capping method to conserve memory
  for (MMNode<MemList>* Nodep_ = Head();
       Nodep_; Nodep_ = Nodep_->_Next)
  {
    MemList* Listp_ = MMList<MemList>::Deref(Nodep_);

    if (Listp_)
    {
      size_t Usage_ = Listp_->SetMemoryCap();

      // if the total amount usage of the allocated memory blocks of the
      // given list is 0 then remove all of them from the list
      if (Usage_ == 0)
	Listp_->DeleteAll();
    }
  }

  // set the low memory flag to FALSE
  _LowMemory = FALSE;
}

/****************************************************************************/
// PURPOSE:
//   Method to search the memory matrix for the memory list which contains
//   the given memory block size
//
// PRE:
//   MemList* TestList_	: A test list which matches the target list that
//			  needs to be found from the memory matrix.
//   BOOLTYPE ExactSize_	: Specifies whether the target list that needs to be
//			  found must have a memory block size which matches
//			  exactly with the test list size or
//
// POST:
//   Returns the matching memory list if the search operation was successful,
//   otherwise NULL is returned
//
MemList* MemMatrix::HasList(MemList* TestList_, BOOLTYPE ExactSize_)
{
  // set up a relation object given the parameter indicating whether to
  // find a memory block of exact size or one that fits that is less than
  // or equal to the specified size
  int Comp_ = ExactSize_ ?
	RelationalEnum::EQUAL:
	RelationalEnum::LESS_THAN_OR_EQUAL;
  PtrRelation<MemList> Relation_(Comp_, SortUsingBuiltInOperators<MemList>::Trait());

  // Do a search operation in the memory matrix to find the memory
  // list containing memory blocks of the specified size
  return MMList<MemList>::Deref(Find(TestList_, Relation_));
}

/****************************************************************************/
// PURPOSE:
//   Method to reallocate the given memory block to a new size. The new size
//   of the memory block could be larger or smaller than its original size
//
// PRE:
//   VOIDPTR Address_	: The memory block to be reallocated
//   size_t Bytes_	: The new size the given memory block should be
//			  reallocated to.
//   BOOLTYPE& Moveable_ : Specify whether memory can be relocated into
//			   another block that may not be contiguous with the
//			   old block
//
// POST:
//   The given memory block is reallocated to the new specified size and
//   returned. If the given memory block could not be found then NULL is
//   returned.
//
VOIDPTR MemMatrix::Reallocate(VOIDPTR Address_, size_t Bytes_, BOOLTYPE Moveable_)
{
  // acquire lock
#ifndef _NOTHREADS
  /* REFERENCED */
  Lock The_Lock_;	// mutex auto create/destroy object
#endif /* _NOTHREADS */

  // If reallocate to 0 bytes then deallocate memory instead, return NULL.
  if (Bytes_ == 0)
  {
    Deallocate(Address_);
    return NULL;
  }

  // Address is null, assume new memory is requested so call Allocate method
  if (Address_ == NULL)
    return Allocate(Bytes_);

  // find the size from the given memory block
  size_t OldSize_ = MemRef::SizeOf(Address_);

  // Create a test list with the same pool size as the memory block
  // then search the memory matrix for the actual list given the test list
  MemList List_(this, OldSize_);
  MemList* Result_ = HasList(&List_, FALSE);
  BOOLTYPE Found_ = Result_ != NULL;

  // Increment counter to reallocate method
  ++_ReallocCalls;

  // if the searched for list is found then reallocate the given memory block
  // which should be contained within it to its new size
  if (Found_)
  {
    VOIDPTR NewBlock_ = Result_->Reallocate(Address_, Bytes_, Moveable_, Found_);
    if (NewBlock_)
      return NewBlock_;
  }

  // If address is not found then if Moveable_ flag specified then allocate
  // new memory, otherwise original specified address is returned.
  if (!Found_)
    return (Moveable_ ? Allocate(Bytes_):Address_);

  // if memory allocation failed there is the option of calling the
  // default memory handler directly, otherwise return NULL
  #if HANDLE_MEMORY_FAULT
    if (_HandlerActive)
      NoMemory();
  #endif
  return NULL;
}

/****************************************************************************/
// PURPOSE:
//   This methods allow the user to preallocate a series of memory pools
//   up to the specified maximum memory pool size. These pools are contained
//   within each memory list with equal sized pools respective to each list.
//   The minimum number of pools specified by the MemMatrix::_MinPools data
//   member is then allocated for each memory list.
//
//   Doing this relieves the memory manager from doing requesting memory
//   from the system and have it simply return the preallocated memory to
//   the calling program. But judge carefully because preallocating too much
//   may be a drain on memory resources, and defeats the purpose of the
//   memory manager.
//
// PRE:
//   size_t Bytes_ :	The maximum memory pool size to allocate in the
//			series of memory lists
//
// POST:
//   Returns TRUE if preallocation was successful, otherwise returns FALSE
//
BOOLTYPE MemMatrix::Preallocate(size_t Bytes_)
{
  // Cannot allocate zero bytes, return FALSE
  if (Bytes_ == 0)
    return FALSE;

  // Create a test list with the specified pool size in Bytes_
  // then search the memory matrix for the actual list given the test list
  MemList List_(this, Bytes_);
  MemList* Result_ = HasList(&List_, FALSE);

  // if the searched for memory list is not found and the low memory
  // indicator is not set then allocate a new memory list containing
  // the specified memory pool size
  if (Result_ == NULL && !_LowMemory)
  {
    BOOLTYPE Capped_ = FALSE;	// Memory capping operation done flag
    BOOLTYPE Done_ = FALSE;	// Memory list allocation done flag

    while (!Done_)
    {
      // Allocate a new memory list with its memory pool size equal
      // to the current Fib counter value
      MemList* NewList_ = new MemList(this, _Counter.Counter());

      if (Validate(NewList_) && NewList_->Preallocate())
      {
	// if the list is valid then append it to the tail of the matrix,
	// increment to running total for memory lists, test if the current
	// Fib counter value fits the size of the memory block to be
	// allocated. If this is true then the appropriate memory list has
	// been allocated. The last step is to increment the Fib counter.
	AppendTail(NewList_);
	++_TotalLists;
	Done_ = _Counter.Counter() >= Bytes_;
	++_Counter;
      }

      // If memory list allocation has failed then perform memory capping
      // if not done already, otherwise break from the loop
      else if (!Capped_)
      {
	SetMemoryCap();
	Capped_ = TRUE;
      }
      else
	break;
    }

    // Try again to search the matrix for the appropriately sized list
    Result_ = HasList(&List_, FALSE);
  }

  // If memory list allocation is successfull then return TRUE
  if (Result_)
    return TRUE;

  // if memory list allocation failed then return FALSE
  return FALSE;
}

/****************************************************************************/
// PURPOSE:
//   Method to allocate a new memory block and check it out for use
//
// PRE:
//   size_t Bytes_ :	The size of the allocated block in bytes
//   BOOLTYPE Placed_	: Flag indicating whether to set the block as
//                        being allocated through the placement new operator
//
// POST:
//   If the memory block is allocated successfully it is returned,
//   otherwise NULL is returned. The placement new flag in the memory block
//   is set if the Placed_ argument is set as TRUE.
//
VOIDPTR MemMatrix::Allocate(size_t Bytes_, BOOLTYPE Placed_)
{
  // Cannot allocate zero bytes, return NULL
  if (Bytes_ == 0)
    return NULL;

  // acquire lock
#ifndef _NOTHREADS
  /* REFERENCED */
  Lock The_Lock_;	// mutex auto create/destroy object
#endif /* _NOTHREADS */

  // Create a test list with the specified pool size in Bytes_
  // then search the memory matrix for the actual list given the test list
  MemList List_(this, Bytes_);
  MemList* Result_ = HasList(&List_, FALSE);

  // if the searched for memory list is not found and the low memory
  // indicator is not set then allocate a new memory list containing
  // the specified memory pool size
  if (Result_ == NULL && !_LowMemory)
  {
    BOOLTYPE Capped_ = FALSE;	// Memory capping operation done flag
    BOOLTYPE Done_ = FALSE;	// Memory list allocation done flag

    while (!Done_)
    {
      // Allocate a new memory list with its memory pool size equal
      // to the current Fib counter value
      MemList* NewList_ = new MemList(this, _Counter.Counter());

      if (Validate(NewList_))
      {
	// if the list is valid then append it to the tail of the matrix,
	// increment to running total for memory lists, test if the current
	// Fib counter value fits the size of the memory block to be
	// allocated. If this is true then the appropriate memory list has
	// been allocated. The last step is to increment the Fib counter.
	AppendTail(NewList_);
	++_TotalLists;
	Done_ = _Counter.Counter() >= Bytes_;
	++_Counter;
      }

      // If memory list allocation has failed then perform memory capping
      // if not done already, otherwise break from the loop
      else if (!Capped_)
      {
	SetMemoryCap();
	Capped_ = TRUE;
      }
      else
	break;
    }

    // Try again to search the matrix for the appropriately sized list
    Result_ = HasList(&List_, FALSE);
  }

  // Increment counter to Allocate method
  ++_AllocCalls;

  // Call the memory pool allocation method
  // given the appropriate memory list
  if (Result_)
  {
    VOIDPTR NewBlock_ = Result_->Allocate(Bytes_, Placed_);
    if (NewBlock_)
      return NewBlock_;
  }

  // if memory allocation failed there is the option of calling the
  // default memory handler directly, otherwise return NULL
  #if HANDLE_MEMORY_FAULT
    if (_HandlerActive)
      NoMemory();
  #endif
  return NULL;
}

/****************************************************************************/
// PURPOSE:
//   Same as the Allocate method, but clears memory block to 0 if
//   successfully allocated. This is similars to C's calloc method hence
//   the method name Callocate.
//
// PRE:
//   size_t Bytes_ :	The size of the allocated block in bytes
//   BOOLTYPE Placed_	: Flag indicating whether to set the block as
//                        being allocated through the placement new operator
//
// POST:
//   If the memory block is allocated successfully it is cleared to 0 and
//   returned, otherwise NULL is returned. The placement new flag in the
//   memory block is set if the Placed_ argument is set as TRUE.
//
VOIDPTR MemMatrix::Callocate(size_t Bytes_, BOOLTYPE Placed_)
{
  VOIDPTR NewBlock_ = Allocate(Bytes_, Placed_);

  if (NewBlock_)
    memset((VOIDPTR)NewBlock_, 0, Bytes_);

  return NewBlock_;
}

/****************************************************************************/
// PURPOSE:
//   Method to deallocate a memory block that was previously allocated
//
// PRE:
//   VOIDPTR Address_	: The memory block that was previously allocated
//			  using the Allocate method
//
// POST:
//   The memory block is checked in and stored in the memory manager as a
//   free block, but is not deallocated. It is reinitialized for use when
//   a new memory allocation request is made.
//
BOOLTYPE MemMatrix::Deallocate(VOIDPTR Address_)
{
  // Null addresses are not processed, return 0 for address not found
  if (Address_ == NULL)
    return FALSE;

  // acquire lock
#ifndef _NOTHREADS
  /* REFERENCED */
  Lock The_Lock_;	// mutex auto create/destroy object
#endif /* _NOTHREADS */

  // Create a test list with the same pool size as the given memory block
  // then search for the actual list in the matrix given the test list
  MemList List_(this, MemPool::SizeOf(Address_));
  MemList* Result_ = HasList(&List_, FALSE);

  // if the actual memory list with its prespecified pool size equal to
  // the given memory block is found then deallocate the memory block from
  // the resulting memory list assuming it is present there
  if (Result_)
  {
    MemRef Pool_(Address_);

    if (Result_->Deallocate(&Pool_))
    {
      ++_DeallocCalls;
      return TRUE;
    }
  }

  return FALSE;
}

/****************************************************************************/
// PURPOSE:
//   Method to check the memory manager for an allocated block with the
//   specified address.
//
// PRE:
//   VOIDPTR Address_	 : The memory block to search for
//   BOOLTYPE& IsPlaced_ : Flag indicating whether the memory block was
//			   allocated through the placement new operator
//
// POST:
//   The memory manager is searched for an allocated block of the given
//   address. If no blocks matches the given address the function returns
//   0 (FALSE), otherwise it returns 1 (TRUE).
//
BOOLTYPE MemMatrix::HasThis(VOIDPTR Address_, BOOLTYPE& IsPlaced_)
{
  // Initialize placement flag to FALSE
  IsPlaced_ = FALSE;

  // Null addresses are not processed, return 0 for address not found
  if (Address_ == NULL)
    return FALSE;

  // acquire lock
#ifndef _NOTHREADS
  /* REFERENCED */
  Lock The_Lock_;	// mutex auto create/destroy object
#endif /* _NOTHREADS */

  // Create a test list with the same pool size as the given memory block
  // then search for the actual list in the matrix given the test list
  MemList List_(this, MemPool::SizeOf(Address_));
  MemList* Result_ = HasList(&List_, FALSE);

  // if the actual memory list with its prespecified pool size equal to
  // the given memory block is found then search the memory list for the
  // actual memory block address to match the given address.
  if (Result_)
  {
    MemRef Pool_(Address_);
    return Result_->HasThis(&Pool_, IsPlaced_);
  }

  return FALSE;
}

/****************************************************************************/
// PURPOSE:
//   Same as HasThis method, but without the placement flag parameter.
//
BOOLTYPE MemMatrix::HasThis(VOIDPTR Address_)
{
  BOOLTYPE Dummy_;
  return HasThis(Address_, Dummy_);
}

/****************************************************************************/
// PURPOSE:
//   Method to release all memory within the memory manager without need to
//   reference the address of memory blocks. The released blocks stays within
//   the memory manager encoded with zero usage size but are not actually
//   deleted.
//
//   Note:
//     Use with caution. If "released" memory is still in use the memory
//     manager will still treat it as a free memory block. Memory management
//     will be corrupted and tracking of free and in-use blocks impossible.
//
// PRE:
//   Memory manager in its current state.
//   In-use and free memory blocks are encoded with their usage size.
//
// POST:
//   All memory within the memory manager is freed. All free blocks are
//   encoded with zero usage size.
//
Ulong MemMatrix::ReleaseAll()
{
  // acquire lock
#ifndef _NOTHREADS
  /* REFERENCED */
  Lock The_Lock_;	// mutex auto create/destroy object
#endif /* _NOTHREADS */

  if (_TotalLists)
  {
    Ulong TotalReleased_ = 0;		// Total bytes released
    MMCounter ListCount_(FIBSTARTNUM);	// Fib counter

    for (size_t Index_ = 0; Index_ < _TotalLists; Index_++, ListCount_++)
    {
      // Create a test list with the same pool size as the Fib counter value
      // then search for the actual list in the matrix given the test list
      MemList List_(this, ListCount_.Counter());
      MemList* Result_ = HasList(&List_, TRUE);

      // If the memory manager has the searched for list then
      // release all memory within the memory list.
      if (Result_)
	TotalReleased_ += Result_->ReleaseAll();
    }

    // Increment number of times this procedure was called
    ++_ReleaseAllCalls;

    // Return the total number of bytes released
    return TotalReleased_;
  }

  return 0;	// Return 0 if no memory blocks were released.
}

/****************************************************************************/
// PURPOSE:
//   Method to call the memory handler directly to handle memory faults
//   The memory handler will be called regardless of the status of the
//   _HandlerActive flag.
//
// POST:
//   The custom memory handler is called if it is not NULL otherwise
//   the default memory handler is called if it is not NULL otherwise
//   no action is taken and the method returns.
//
void MemMatrix::NoMemory()
{
  if (_CustomHandler)
    (*_CustomHandler)();
  else if (_DefaultHandler)
    (*_DefaultHandler)();
}

/****************************************************************************/
// PURPOSE:
//   Return static instance of memory matrix. The object is made function
//   static rather than class static to avoid initialization order problems
//   associated with global static and class static variables
//
// POST:
//   Returns the static instance of a memory matrix object.
//
MemMatrix& MemMatrix::Matrix()
{
  static MemMatrix* TheMatrixPtr_ = NULL;

  // Do a lock-out during construction of memory matrix object (just in case)
#ifndef _NOTHREADS
  static Boolean ObjectInitialized_ = FALSE;
  if (!ObjectInitialized_)
  {
    __MEMMATRIX_LOCK;
#endif

    static MemMatrix TheMatrix_;
    TheMatrixPtr_ = &TheMatrix_;

  // Do a lock-out during construction of memory matrix object (just in case)
#ifndef _NOTHREADS
    ObjectInitialized_ = TRUE;
    __MEMMATRIX_UNLOCK;
  }
#endif

  return *TheMatrixPtr_;
}

/****************************************************************************/
#ifdef __MM_PTHREADS
pthread_mutex_t* MemMatrix::GiveLockObject()
{
  static pthread_mutex_t LockObject_ = PTHREAD_MUTEX_INITIALIZER;
  return &LockObject_;
}
#endif

#ifdef __MM_WIN32THREADS
CRITICAL_SECTION* MemMatrix::GiveLockObject()
{
  static CRITICAL_SECTION LockObject_;

  if (!Lock_Initialized)
  {
    InitializeCriticalSection(&LockObject_);
    Lock_Initialized = TRUE;
  }

  return &LockObject_;
}
#endif

/****************************************************************************/
// PURPOSE:
//   Method to collect statistics on the memory manager.
//   Statistics that are collected include:
//	- Total allocated memory contained in the memory manager
//      - The amount of allocated memory that is in use
//      - The amount of overhead used as book keeping data
//      - The amount of allocated memory that is wasted
//	  (inexact matching of in-use memory and the prespecified block size)
//      - The amount of allocated memory that is still available for use.
//
// POST:
//   The statistics for the memory manager is stored in a local MemStats
//   structure which is then returned by value to the calling program
//
MemStats MemMatrix::Statistics()
{
  // acquire lock
#ifndef _NOTHREADS
  /* REFERENCED */
  Lock The_Lock_;	// mutex auto create/destroy object
#endif /* _NOTHREADS */

  // Create a memory statistics structure and
  // find the head node of the memory matrix
  MemStats Stats_;
  MMNode<MemList>* Nodep_ = Head();

  // initialize all members of the statistics structure to 0
  Stats_._Allocated 	=
  Stats_._Used 		=
  Stats_._BookKeeping	=
  Stats_._Wasted 	=
  Stats_._Free          = 0;

  Stats_._AllocCalls 		= _AllocCalls;
  Stats_._DeallocCalls  	= _DeallocCalls;
  Stats_._ReallocCalls  	= _ReallocCalls;
  Stats_._ReleaseAllCalls       = _ReleaseAllCalls;

  // starting from the initial memory list, traverse down the memory matrix
  // calling the statistics method of each memory list to find to accumulated
  // memory statistics of the entire memory matrix
  for (;Nodep_; Nodep_ = Nodep_->_Next)
    Nodep_->_Object->MemoryStats(Stats_);

  // return the local memory statistics structure by value
  return Stats_;
}

/****************************************************************************/
ostream& MemMatrix::ShowStats(ostream& os, const MemStats& Stats_)
{
    os <<"Allocated           : " <<Stats_._Allocated <<endl;
    os <<"Used                : " <<Stats_._Used <<endl;
    os <<"BookKeeping         : " <<Stats_._BookKeeping <<endl;
    os <<"Wasted              : " <<Stats_._Wasted <<endl;
    os <<"Free                : " <<Stats_._Free <<endl;
    os <<"AllocCalls          : " <<Stats_._AllocCalls <<endl;
    os <<"DeallocCalls        : " <<Stats_._DeallocCalls <<endl;
    os <<"ReallocCalls        : " <<Stats_._ReallocCalls <<endl;
    os <<"ReleaseAllCalls     : " <<Stats_._ReleaseAllCalls <<endl;

    return os;    
}

/****************************************************************************/
#endif




