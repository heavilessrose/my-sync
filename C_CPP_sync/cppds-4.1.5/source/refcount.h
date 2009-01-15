#ifndef REFERENCECOUNT_H
#define REFERENCECOUNT_H
#ifndef BUILTIN_H
  #include "builtin.h"
#endif
#ifndef BOOLEAN_H
  #include "Boolean.h"
#endif

/****************************************************************************/
class ReferenceCount
{
  public:
    ReferenceCount();                                  // Create with count of 1
    ReferenceCount(const ReferenceCount&);             // Copy and increment count
    ReferenceCount& operator=(const ReferenceCount&);  // Assign; decrement lhs count, increment rhs
    ~ReferenceCount();                                 // Decrement count; delete if 0

    Boolean Unique() const;                            // True if count is 1

    void Decrement();                                  // Decrement count; delete if 0
    void Increment();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

  private:
    unsigned int* p_refcnt;                            // Pointer to actual count
};

/****************************************************************************/
#endif




