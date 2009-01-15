#ifndef ARRAYCLASS_CPP
#define ARRAYCLASS_CPP
#ifndef ARRAYDEF_H
  #include "arraydef.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef CONTAINER_H
  #include "contain.h"
#endif

/****************************************************************************/
class SubArray;
class RefInfo;

// Abstract array class
class ArrayClass : public Container
{
  friend class SubArray;
  friend class RefInfo;

  protected:
    // TSubArray maintenance methods
    virtual void EraseNullChain()	      		= 0;
    virtual void SetTail(SubArray* Ptr_)		= 0;
    virtual void SetRunLength(size_t Size_)		= 0;

    // Total occupied (non-void) cells : set, increment, decrement.
    virtual void SetSize(size_t Size_)			= 0;
    virtual void IncrementSize(size_t Incr_)		= 0;
    virtual void DecrementSize(size_t Decr_)		= 0;

    // Total cells allocated : increment, decrement.
    virtual void IncrementTotal(size_t Incr_)		= 0;
    virtual void DecrementTotal(size_t Decr_)		= 0;

    // Find number of non-null elements in vector
    virtual void FindSize()				= 0;

  public:
    virtual ~ArrayClass() {}

    // Array shrinking methods
    virtual void Shrink(size_t Size_) 			= 0;
    virtual void ShrinkTo(size_t Size_) 	     	= 0;

    // Size and total data accessors
    virtual size_t RunLength() const			= 0;
    virtual size_t Total() const			= 0;

    // Self deleting/shrinking option methods
    virtual ArrayClass& EraseIfNull() 			= 0;
    virtual ArrayClass& KeepIfNull() 			= 0;
    virtual Boolean ShouldEraseIfNull() const		= 0;

    // Array bound methods
    virtual ArrayClass& Bound(size_t Limit_)		= 0;
    virtual ArrayClass& Unbound()			= 0;
};

/****************************************************************************/
#endif





