#ifndef CONTAINER_H
#define CONTAINER_H
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef SUBSCRIPT_H
  #include "subscrpt.h"
#endif

#define CONTAINER_ZEROVALUEFNCS_DEFN(Type)              \
friend Boolean _AtZero(const Type& Obj_)                \
{ return Obj_.IsEmpty(); }                              \
friend Boolean _NotZero(const Type& Obj_)               \
{ return !Obj_.IsEmpty(); }

class Container
{
  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  public:
    // data member destruction method
    virtual void Flush() 				= 0;

    virtual size_t Size() const 			= 0;
    static size_t MaxSize()
	{ return (SIZET_MAX - 1); }

    // container size / limit accessors
    virtual size_t Limit() const 			= 0;

    // container full / empty test methods
    virtual Boolean IsFull() const 			= 0;
    virtual Boolean IsEmpty() const 			= 0;

    // container iterator methods
    virtual void DestroyFromIterator() 			= 0;
    virtual Boolean RegisterIterator()
	{ return FALSE; }
    virtual Boolean UnRegisterIterator()
	{ return FALSE; }
    virtual size_t IteratorsActive()
	{ return 0; }
};

// Implemented in derived classes
#define STL_CONTAINER_DECL(Type, iterator, reverse_iterator)	\
    const iterator begin() const;       	\
    iterator begin();				\
    const iterator end() const;			\
    iterator end();				\
						\
    const reverse_iterator rbegin() const;      \
    reverse_iterator rbegin();                	\
    const reverse_iterator rend() const;	\
    reverse_iterator rend();			\
						\
    Type& front();				\
    const Type& front() const;			\
    Type& back();				\
    const Type& back() const;			\
						\
    void push_back(const Type& x);		\
    void pop_back();				\
    iterator insert(iterator pos, const Type& x=Type());			\
    void insert(iterator pos, size_t n, const Type& x);                         \
    void insert(iterator pos, const iterator first, const iterator last);	\
    void erase(iterator pos);			\
    void erase(iterator first, iterator last);

// Implemented in derived classes
#define STL_PTRCONTAINER_DECL(Type, iterator, reverse_iterator)	\
    const iterator begin() const;       	\
    iterator begin();				\
    const iterator end() const;			\
    iterator end();				\
						\
    const reverse_iterator rbegin() const;      \
    reverse_iterator rbegin();                	\
    const reverse_iterator rend() const;	\
    reverse_iterator rend();			\
						\
    Type* front();				\
    const Type* front() const;			\
    Type* back();				\
    const Type* back() const;			\
						\
    void push_back(Type* x);			\
    void pop_back();					\
    iterator insert(iterator pos, Type* x=NULL);	\
    void insert(iterator pos, size_t n, Type* x);       \
    void insert(iterator pos, const iterator first, const iterator last);	\
    void erase(iterator pos);			\
    void erase(iterator first, iterator last);

#endif





