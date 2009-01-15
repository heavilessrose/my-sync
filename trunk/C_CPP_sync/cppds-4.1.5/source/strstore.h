#ifndef STRINGSTORE_H
#define STRINGSTORE_H
#ifndef DATASTORE_CPP
  #include "datastore.cpp"
#endif

#define STRINGSTORE_DEBUG               0
#define STRINGSTORE_NOTIFY_SUCCESS      0

/****************************************************************************/
class StringStorePtr : public ArrayStorePtr<char>
{
  public:
    StringStorePtr();
    StringStorePtr(const char* Ptr_);
    StringStorePtr(char* Ptr_, int Flags_);
    StringStorePtr(const DataStorePtr<char>& Obj_);
    StringStorePtr(const ArrayStorePtr<char>& Obj_);
    StringStorePtr(const StringStorePtr& Obj_);

    static StringStorePtr* Make();
    static StringStorePtr* Make(const char* Ptr_);
    static StringStorePtr* Make(char* Ptr_, int Flags_);
    static StringStorePtr* Make(const DataStorePtr<char>& Obj_);
    static StringStorePtr* Make(const ArrayStorePtr<char>& Obj_);
    static StringStorePtr* Make(const StringStorePtr& Obj_);
    
    virtual ArrayStorePtr<char>* Clone();

    static int StaticID()
	{ return TypeInfo::STRINGSTOREPTR_TYPE; }
    virtual int ClassID() const;

    size_t StrLen() const
        { return (_Length - 1); }

    virtual StringStorePtr& operator = (const DataStorePtr<char>& rhs);
    virtual StringStorePtr& operator = (const ArrayStorePtr<char>& rhs);
    virtual StringStorePtr& operator = (const StringStorePtr& rhs);    
    virtual StringStorePtr& AssignPtr(char* Ptr_, int Flags_);
    virtual StringStorePtr& AssignObject(const char* Ptr_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif  
};

/****************************************************************************/
#endif




