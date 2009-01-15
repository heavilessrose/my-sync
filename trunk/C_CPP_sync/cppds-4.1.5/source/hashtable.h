/*****************************************************************************
*			Joseph Wong / Nov 29 1994			     *
* Hashtable header file						     	     *
* Declarations for a Hashtable with container classes, Iterator classes,     *
* and associate classes							     *
*****************************************************************************/
#include <iostream.h>
#include <iomanip.h>
#ifndef ASSOCIATION_CPP
  #include "assoc.cpp"
#endif
#ifndef PRIMETBL_H
  #include "primetbl.h"
#endif

#ifndef HASHTABLE_H
#define HASHTABLE_H
//****************************************************************************
#define DEBUG_HASHTABLE       0

//****************************************************************************
// the hashable class contains a key value pair with the template member
// being the value the key member is of integer type this is used as the
// array index into the hash table
//
// The store_by_string class contains a stringkey member this is used as the
// default key data for calculating the integer key in the hashable base class
// After this calculation is done the hashable base class of the store by
// string class contains the correct hash index for the given string key the
// data member of the hashable class can be set to any arbitrary value. The
// end result is that the hashable class can used to store pair values or
// associations of strings with any arbitrary value.
//
// The hashtable class do not need to contain the store_by_string members
// since this can be calculated using one instance of this class. The
// calculated values are located in the hashable base class of the store_
// by_string class and this is passed to the hashtable class to be stored
// within the array.
//
// The array_by_string class contains one pointer to an instance of the
// store_by_string class. This pointer is instantiated as neccessary to
// calculate the resulting hashable base class and it containing key & data
// members. Appropriate methods from the hashable class is then called to
// either lookup or store the resulting hashable object.
//
// The Borland container class method of dealing with hashable object is to
// create a hashValue method for each class to return the hash index to the
// hashtable. This has its drawbacks since hash indexes can only be calculated
// by calling the hashValue method only objects within the container class or
// derived from it can be hashed. User defined class or built in types
// cannot be hash since they may not contain the hashValue method for
// calculating indexes.
//
/****************************************************************************/
class HashTableScheme
{
  public:
    enum
    {
      CHAIN,
      DOUBLEHASH
    };
};

template <class KEY, class VALUE>
class HashTableIterator;

template <class KEY, class VALUE>
class ArrayReference;

template <class KEY, class VALUE>
class ArrayPtrReference;

template <class KEY, class VALUE>
class HashTable : public HashTableScheme
{
  friend class ArrayReference<KEY, VALUE>;
  friend class ArrayPtrReference<KEY, VALUE>;
  friend class HashTableIterator<KEY, VALUE>;

  protected:
    enum
    {
      OWNS_KEY   = 1,
      OWNS_VALUE = 2
    };
  
    VALUE _DefaultValue;
    Ulong _TblSize;
    Ulong _ItemCount;    
    Boolean _FullTable;
    Boolean _Found;
    unsigned _OwnsPtr;
    int _StorageScheme;

    HashableClass<KEY, VALUE>** _Table;

    static Ulong NextPrime(Ulong Size_);

    void EraseTable();
    HashableClass<KEY, VALUE>** AllocTable(Ulong Size_);

    HashableClass<KEY, VALUE>* StoreItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_);
    HashableClass<KEY, VALUE>* FindItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_);
    HashableClass<KEY, VALUE>* ChainStoreItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_);
    HashableClass<KEY, VALUE>* ChainFindItem(HashableClass<KEY, VALUE>* Target_, HashableClass<KEY, VALUE>* Source_);
    
    HashableClass<KEY, VALUE>* Add(HashableClass<KEY, VALUE>* HbPtr_, Ulong& Index_, Boolean& Done_);
    HashableClass<KEY, VALUE>* LookUp(HashableClass<KEY, VALUE>* HbPtr_, Ulong& Index_, Boolean& Done_);
    HashableClass<KEY, VALUE>* Remove(HashableClass<KEY, VALUE>* HbPtr_, Ulong& Index_, Boolean& Done_);
    VALUE& GiveDefaultValue();
    const VALUE& GiveDefaultValue() const;

    void SetFound(Boolean Flag_)
	{ _Found = Flag_; }
    void SetFullTable(Boolean Flag_)
	{ _FullTable = Flag_; }

    void Xfull() const
	{ THROW (FallibleBase::FullHashTableErr()); }
        
  public:  
    HashTable(Ulong Size_, const VALUE& DefaultVal_, int Scheme_);
    HashTable(const HashTable<KEY, VALUE>& ht);
    virtual ~HashTable();

    HashTable<KEY, VALUE>& operator = (const HashTable<KEY, VALUE>& Htable_);
    HashTable<KEY, VALUE>& SetOwnsValuePtr(Boolean Flag_);
    HashTable<KEY, VALUE>& SetOwnsKeyPtr(Boolean Flag_);

    ArrayReference<KEY, VALUE> operator [] (const HashableClass<KEY, VALUE>& Destination_);
    ArrayReference<KEY, VALUE> operator [] (const KEY& Destination_);

    ArrayPtrReference<KEY, VALUE> operator () (HashableClass<KEY, VALUE>* Destination_);
    ArrayPtrReference<KEY, VALUE> operator () (KEY* Destination_);

    Ulong GetTableSize() const;
    Ulong GetItemCount() const;
    Boolean OwnsValuePtr() const
        { return (_OwnsPtr & OWNS_VALUE); }
    Boolean OwnsKeyPtr() const
        { return (_OwnsPtr & OWNS_KEY); }
    Boolean IsFound() const
        { return _Found; }
    Boolean IsFullTable() const
	{ return _FullTable; }
};

//****************************************************************************
template <class KEY, class VALUE>
class HashTableIterator
{
  protected:
    union
    {
      HashTable<KEY, VALUE>* _ArrPtr;
      const HashTable<KEY, VALUE>* _ConstArrPtr;
    };
    
    Ulong _IterIndex;
    Ulong _ArraySize;
    Boolean _PostIncValid;

  public:
    HashTableIterator(const HashTable<KEY, VALUE>& Target_);

    void Attach(HashTable<KEY, VALUE>& Target_);
    void Attach(const HashTable<KEY, VALUE>& Target_) const;
    void Reset()
	{ _IterIndex = _ArraySize; }    
    Ulong GetPos() const
	{ return _IterIndex; }

    const HashableClass<KEY, VALUE>* operator ++ () const;
    const HashableClass<KEY, VALUE>* operator ++ (int) const;

    HashableClass<KEY, VALUE>* operator ++ ();
    HashableClass<KEY, VALUE>* operator ++ (int);    
};

//****************************************************************************
template <class KEY, class VALUE>
class ArrayReference
{
  friend class HashTable<KEY, VALUE>;

  private:
    ReferenceCount refCount; // Number of pointers to heap object      
    HashTable<KEY, VALUE>* _HtPtr;
    HashableClass<KEY, VALUE>* _HbPtr;

    ArrayReference(HashTable<KEY, VALUE>& MyBoss_, HashableClass<KEY, VALUE>* MyData_);

  public:
    ~ArrayReference();

    Boolean Erase();
    VALUE& Store();    
    VALUE& operator = (const VALUE& Val_);
    operator const VALUE& () const;
    operator VALUE& ();
};

//****************************************************************************
template <class KEY, class VALUE>
class ArrayPtrReference
{
  friend class HashTable<KEY, VALUE>;

  private:
    ReferenceCount refCount; // Number of pointers to heap object        
    HashTable<KEY, VALUE>* _HtPtr;
    HashableClass<KEY, VALUE>* _HbPtr;

    ArrayPtrReference(HashTable<KEY, VALUE>& MyBoss_, HashableClass<KEY, VALUE>* MyData_);

  public:
    ~ArrayPtrReference();

    Boolean Erase();
    VALUE* Store();
    VALUE* operator = (VALUE* Val_);
    operator const VALUE* () const;
    operator VALUE* ();
};

//****************************************************************************
#endif




