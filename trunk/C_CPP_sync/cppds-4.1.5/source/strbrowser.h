#ifndef STRINGBROWSER_H
#define STRINGBROWSER_H
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif

#define STRINGBROWSER_DEBUG     0

/****************************************************************************/
class StringBrowserAcceptor : public ObjectAcceptor
{
  friend class StringBrowser;

  protected:
    union
    {
      const StringBrowser* _ObjPtr;
      StringBrowser* _VarPtr;
    };

    StringBrowserAcceptor(const StringBrowser* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // ChrString class specific acceptor methods
    virtual void* CreateFromStringBrowser(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromStringBrowser(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);

    virtual Boolean IsStringBrowser() const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class StringBrowser : public ObjectSkeleton
{
  protected:
    ChrString _Str;
    ChrString _Token;
    StringToken _Tokenizer;

  public:
    StringBrowser();                           // Null string
    StringBrowser(const char*);                // From built-in string
    StringBrowser(const char*, Subscript n);   // ... for at most n characters
    StringBrowser(char c, Subscript n=1);      // From single character * n times
    StringBrowser(const ChrString&);           // From a ChrString
    StringBrowser(const StringBrowser&);       // From another StringBrowser

    // Virtual constructor and assignment methods
    static StringBrowser* Make();
    static StringBrowser* Make(char Ch_, Subscript n);
    static StringBrowser* Make(const char* Str_);
    static StringBrowser* Make(const char* Str_, Subscript n);
    static StringBrowser* Make(const Object& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Static null object accessor methods
    static StringBrowser& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::STRINGBROWSER_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // Assignment
    virtual StringBrowser& operator = (const ChrString& Str_);
    virtual StringBrowser& operator = (const StringBrowser& Str_);

    // ChrString tokenizer accessor
    StringToken& Tokenizer()
	{ return _Tokenizer; }

    // Find token method
    ChrString& FindToken();    

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

    // new & delete operators for arrays
    // Not dispatched since memory allocation / deallocation should be
    // defined for the whole class including any envelope interface classes.
    // Set definition HAS_ARRAY_NEW to true if compiler supports it.
    // Should be omitted in classes that are directly related to the
    // memory manager or doesn't use custom memory management.
#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

/****************************************************************************/
#endif





