#ifndef REFCOUNT_CPP
#define REFCOUNT_CPP
#ifndef REFCOUNT_H
  #include "refcount.h"
#endif

/****************************************************************************/
Boolean ReferenceCount::Unique() const
{
  return (p_refcnt != NULL && *p_refcnt == 1);
}

/****************************************************************************/
ReferenceCount::ReferenceCount():
p_refcnt(new_unsigned_int(1))
{}

/****************************************************************************/
ReferenceCount::ReferenceCount(const ReferenceCount& anRC):
p_refcnt(anRC.p_refcnt)
{
  Increment();
}

/****************************************************************************/
void ReferenceCount::Increment()
{
  if (p_refcnt)
    ++*p_refcnt;
}

/****************************************************************************/
void ReferenceCount::Decrement()
{
  if (Unique())
  {
    RawDelete(p_refcnt);
    p_refcnt = NULL;
  }
  else if (p_refcnt)
    --*p_refcnt;
}

/****************************************************************************/
ReferenceCount::~ReferenceCount()
{
  Decrement();
}

/****************************************************************************/
ReferenceCount& ReferenceCount::operator = (const ReferenceCount& rhs)
{
    if (rhs.p_refcnt)
      ++*rhs.p_refcnt;

    Decrement();
    p_refcnt = rhs.p_refcnt;
    return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* ReferenceCount::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void ReferenceCount::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* ReferenceCount::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif
/****************************************************************************/
#if	HAS_ARRAY_NEW
void ReferenceCount::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
#endif





