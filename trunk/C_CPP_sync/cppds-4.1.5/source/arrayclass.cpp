#ifndef ARRAYCLASS_CPP
#define ARRAYCLASS_CPP
#ifndef ARRAYDEF_H
  #include "arraydef.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif

/****************************************************************************/
class SubArray;
class RefInfo;

// Abstract array class
class ArrayClass
{
  friend class SubArray;
  friend class RefInfo;

  protected:
    // TSubArray maintenance methods
    virtual void EraseNullChain()	      		= 0;
    virtual void SetTail(SubArray* Ptr_)		= 0;
    virtual void SetRunLength(Ushort Size_)		= 0;

    // Total occupied (non-void) cells : set, increment, decrement.
    virtual void SetSize(Ushort Size_)			= 0;
    virtual void IncrementSize(Ushort Incr_)		= 0;
    virtual void DecrementSize(Ushort Decr_)		= 0;

    // Total cells allocated : increment, decrement.
    virtual void IncrementTotal(Ushort Incr_)		= 0;
    virtual void DecrementTotal(Ushort Decr_)		= 0;

    // Find total occupied (non-void) cells.
    virtual void FindSize()				= 0;

  public:
    virtual ~ArrayClass() {}

    // Array shrinking methods
    virtual void Shrink(Ushort Size_) 			= 0;
    virtual void ShrinkTo(Ushort Size_) 	     	= 0;

    // Size and total data accessors
    virtual Ushort Size() const				= 0;
    virtual Ushort RunLength() const			= 0;
    virtual Ushort Total() const			= 0;

    // Self deleting/shrinking option methods
    virtual ArrayClass& EraseIfNull() 			= 0;
    virtual ArrayClass& KeepIfNull() 			= 0;
    virtual Boolean ShouldEraseIfNull() const		= 0;

    // Array bound methods
    virtual ArrayClass& Bound(Ushort Limit_)		= 0;
    virtual ArrayClass& Unbound()			= 0;
    virtual Ushort GetLimit() const			= 0;

    // Array element data deletion methods
    virtual ArrayClass& IgnoreWhenDiscardItem()		= 0;
    virtual ArrayClass& DeleteWhenDiscardItem()		= 0;
    virtual Boolean ShouldDeleteItem()			= 0;
    virtual void DeleteAll()				= 0;
};

/****************************************************************************/
#endif

