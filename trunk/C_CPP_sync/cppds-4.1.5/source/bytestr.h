#ifndef BYTESTR_H
#define BYTESTR_H
#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif

#define BYTESTRING_DEBUG		         0
#define BYTESTRING_SHOW_DEBUG_OUTPUT	0
#define BYTESTRING_NOTIFY_SUCCESS	   0

class istream;
class ostream;

/****************************************************************************/
class ByteStringAcceptor : public ObjectAcceptor
{
  friend class ByteString;

  protected:
    union
    {
      const ByteString* _ObjPtr;
      ByteString* _VarPtr;
    };

    ByteStringAcceptor(const ByteString* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // ByteString class specific acceptor methods
    virtual Boolean IsByteString() const;
    virtual Boolean IsStringImp() const;

    virtual void* CreateFromByteString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromByteString(const ObjectAcceptor& Obj_);
    virtual void* CreateFromChrString(const ObjectAcceptor& Obj_);
    virtual void* AssignFromChrString(const ObjectAcceptor& Obj_);

    virtual Boolean IsEqualToByteString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToByteString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToByteString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsEqualToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsLesserToChrString(const ObjectAcceptor& Obj_) const;
    virtual Boolean IsGreaterToChrString(const ObjectAcceptor& Obj_) const;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);
#endif
};

/****************************************************************************/
class ByteStringData : public StringData
{
  public:
    Subscript _Length;

    ByteStringData(Subscript Sz_);
    ByteStringData(char* Str_, Subscript Len_, Subscript Sz_);

    virtual StringData* NewInstance(Subscript Sz_);
    virtual StringData* NewInstance(char* Str_, Subscript Len_, Subscript Sz_);

    virtual void Length(Subscript Value_);
    virtual Subscript Length() const;

    static ByteStringData* Make(Subscript Sz_);
    static ByteStringData* Make(char* Str_, Subscript Len_, Subscript Sz_);
};

/****************************************************************************/
class ByteStringEncLetter;
class ConstByteStringPtr;
class ByteStringPtr;

class ByteString : public StringImp
{
  friend class ByteStringAcceptor;
  friend class ByteStringEncLetter;
  friend class ConstByteStringPtr;
  friend class ByteStringPtr;

  friend Object& RootObject(ByteString& Obj_)
	{ return *((Object*)(&Obj_)); }
  friend const Object& RootConstObject(const ByteString& Obj_)
	{ return *((const Object*)(&Obj_)); }

  protected:
    virtual void TerminateString(size_t Sz_);

    virtual Boolean IsEqualToByteString(const ByteString* Ptr_) const;
    virtual Boolean IsLesserToByteString(const ByteString* Ptr_) const;
    virtual Boolean IsGreaterToByteString(const ByteString* Ptr_) const;

  public:
    // Constructors
    ByteString();                           	// Null string
    ByteString(const char*, Subscript Size_);   // ... for at most n characters
    ByteString(char c, Subscript n=1);      	// From single character * n times
    ByteString(const StringImp& s);             // From another ByteString

    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      ByteString(const ByteString& s);
    #endif

    // Virtual constructor and assignment methods
    static ByteString* Make();
    static ByteString* Make(char Ch_, Subscript n);
    static ByteString* Make(const char* Str_, Subscript n);
    static ByteString* Make(const Object& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Automatically update pointers to the string
    static void AutoUpdatePtrs(Boolean Flag_=TRUE);

    // Static null object accessor methods
    static ByteString& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long StorableFormWidth() const;
    virtual long DataSize() const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::BYTESTRING_TYPE; }
    virtual int ClassID() const;
    virtual const ObjectAcceptor* GiveObjectAcceptor() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    virtual ByteStream& BinaryIgnore(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryRead(ByteStream& Is_, Boolean* Ok_=NULL);
    virtual ByteStream& BinaryWrite(ByteStream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Comparison methods
    virtual Boolean IsEqual(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsLesser(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    virtual Boolean IsGreater(const Object& Obj_, const ObjectAcceptor* ObjComp_=NULL) const;
    Boolean MemiComp(const ByteString& s2) const
	{ return ::MemiComp(_StrData->data, s2._StrData->data, strlen()); }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

    // Case Conversion
    ByteString& ToUpper(void);			// convert to uppercase
    ByteString& ToLower(void);			// convert to lowercase

    // Formatting
    ByteString& LeftJustify(Subscript FieldWidth_, char Pad_=' ');
    ByteString& RightJustify(Subscript FieldWidth_, char Pad_=' ');
    ByteString& CenterJustify(Subscript FieldWidth_, char Pad_=' ');

    // Duplication, Insertion, deletion and replacement
    ByteString& Duplicate(Subscript Num_);
    ByteString& Insert(Subscript pos, const ByteString& Str_);
    ByteString& Append(const ByteString& rhs, Subscript RunLen_);
    ByteString& OverWrite(Subscript pos, const ByteString& Str_);
    ByteString& Delete(Subscript pos, Subscript len = 1);
    ByteString& SetAllBytesTo(char Pad_);
    Subscript Replace(Subscript pos,
		      const ByteString& from,
		      const ByteString& to,
		      Subscript count=-1);

    // character set functions
    ByteString& MakeCharSet(int(*iscsetfn)(int));
    ByteString& MakeCharSet(const char* Src_, size_t RunLen_=0);

    // Assignment
    virtual ByteString& operator = (char Chr_);            		// ... from single character
    virtual ByteString& operator = (const StringImp& Str_);   		// Assign from ByteString
    virtual ByteString& AssignStr(const char* Str_, Subscript Len_);    // ... from built-in string

    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      virtual ByteString& operator = (const ByteString& Str_);
    #endif

    // Indexing
    Subscript RestrictIndex(Subscript Index_) const;
    Boolean IndexInRange(Subscript Index_) const;
    virtual Fallible<char> operator [] (Subscript i) const;
    virtual Fallible<char> operator [] (Subscript i);
    virtual const char* operator () (Subscript i) const;
    virtual char* operator () (Subscript i);
    virtual char operator * () const;
    virtual char& operator * ();

    // Concatenation
    virtual ByteString& operator += (const ByteString& rhs);
    virtual ByteString& operator -= (const ByteString& rhs);
    virtual ByteString& Delete(const ByteString& rhs);

    friend ByteString operator + (const ByteString& lhs, const ByteString& rhs);
    friend ByteString operator - (const ByteString& lhs, const ByteString& rhs);
    friend ByteString Delete(const ByteString& lhs, const ByteString& rhs);

    // Searches
    virtual Fallible<Subscript> find(char c)            const;  		// First occurence of c
    virtual Fallible<Subscript> findlast(char c)        const;  		// Last occurence of c
    virtual Fallible<Subscript> findnext(Subscript pos, char c)   const;    	// Next occurence of s
    virtual Fallible<Subscript> findprevious(Subscript pos, char c)   const;	// Last occurence of s

    Fallible<Subscript> find(const ByteString& s)   const;  	// First occurence of s
    Fallible<Subscript> findlast(const ByteString& s)   const;  	// Last occurence of s
    Fallible<Subscript> findnext(Subscript pos, const ByteString& s)   const;  	// Next occurence of s
    Fallible<Subscript> findprevious(Subscript pos, const ByteString& s)   const;  	// Last occurence of s
    Fallible<Subscript> brk(const ByteString& s)    const;  // First occurence of any character in s
    Subscript           span(const ByteString& s)   const;  // Size of initial span of characters in s
    Subscript           cspan(const ByteString& s)  const;  // Size of initial span of characters not in s

    // Sub-strings
    ByteString subString(Subscript start) const;  		// From start
    ByteString subString(Subscript start, Subscript n) const;  	// ... for n characters
    ByteString& Left(Subscript len);				// left   len chars
    ByteString& Right(Subscript len);				// right  len chars
    ByteString& Mid(Subscript pos,	        		// middle len chars from pos
		    Subscript len);

    // Conversion into numbers
    virtual Fallible<double>        strtod()  const;    // Conversion to double
    virtual Fallible<long>          strtol()  const;    // Conversion to long
    virtual Fallible<unsigned long> strtoul() const;    // Conversion to unsigned long

    // String termination
    virtual StringImp& NullTerminate();

    // Miscellaneous
    ChrString AsString() const
	{ return ChrString(_StrData->data, _StrData->Length()); }
    ByteString& StrRev();

    // Iterators
    class Iterator; // Forward declaration

    class Browser
    {
      public:
	Browser(const ByteString& s);

	Boolean more() const;
	void    advance();
	char    current() const;

	Browser(const Iterator&);

      private:
	const char* cur;
	const char* endp;
    };

    class Iterator
    {
      public:
	Iterator(ByteString& s);

	Boolean more() const;
	void    advance();
	char&   current() const;

      private:
	friend Browser::Browser(const Iterator&);
	char* cur;
	const char* endp;
    };

  private:
    ByteString(const ByteString&, const ByteString&);
    ByteString(ByteString& Obj_, int, int);
    ByteString(char*, int, int);

    void AssignLetter(ByteString& Obj_);
    Boolean IsOld() const;
};

/****************************************************************************/
class ByteStringToken
{
  private:
    ByteString _SearchStr;	// The byte string to search for
    const char* _Delims;		// the delimiters in the byte string
    const char* _WhiteSpace;		// Whitespace in the search string

    size_t _Start;
    size_t _Index;

  public:
    ByteStringToken();
    ByteStringToken(const ByteStringToken& Stok_);
    ByteStringToken(const ByteString& Str_, size_t Start_, const char* Delims_, const char* Wspc_);

    ByteStringToken& operator = (const ByteStringToken& Stok_);

    inline const ByteString& GetString() const
	{ return _SearchStr; }
    inline size_t GetStartIndex() const
	{ return _Start; }
    inline size_t GetIndex() const
	{ return _Index; }
    inline const char* GetDelimiters() const
	{ return _Delims; }
    inline const char* GetWhiteSpace() const
	{ return _WhiteSpace; }

    ByteStringToken& SetStartIndex(size_t Index_);
    ByteStringToken& ResetIndex();
    ByteStringToken& SetDelimiters(const char* Str_);
    ByteStringToken& SetWhiteSpace(const char* Str_);
    ByteStringToken& SetString(const ByteString& Str_);

    size_t FindToken(ByteString& Token_);

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
// ByteStringPtr class forward declaration
class ByteStringPtr;

// Enclosed object of ByteStringPtr class
class ByteStringEncLetter : public ByteString
{
  friend class ByteStringPtr;

  private:
    ByteStringEncLetter();
    ByteStringEncLetter(ByteString& Obj_);
    ByteStringEncLetter(const ByteStringEncLetter& Obj_);

    const ByteStringEncLetter& Truncate(const ByteString& rhs) const;

  public:
    // Case Conversion
    const ByteStringEncLetter& ToUpper(void) const;
    const ByteStringEncLetter& ToLower(void) const;

    // Duplication, Insertion, deletion and replacement
    const ByteStringEncLetter& Delete(Subscript pos, Subscript len = 1) const;

    virtual ByteString& Delete(const ByteString& rhs);
    virtual const ByteStringEncLetter& Delete(const ByteString& rhs) const;

    // Miscellaneous
    const ByteStringEncLetter& StrRev() const;
};

/****************************************************************************/
// ByteStringPtr class -- functions in much the same way as a pointer
// the ByteString class. The internal ptr to the actual string can be
// manipulated to store different characters, but the ptr itself cannot be
// changed to point to another address.
// similar to returning char *const which means constant pointer to a
// non-const array of chars
class ByteStringPtr
{
  protected:
    const ByteStringEncLetter* _LetterPtr;
    ByteStringEncLetter _Letter;

  public:
    ByteStringPtr();
    ByteStringPtr(ByteString& Obj_);
    ByteStringPtr(const ByteStringPtr& Obj_);

    ByteStringPtr& operator = (ByteString& Obj_);
    ByteStringPtr& operator = (const ByteStringPtr& Obj_);
    ByteStringPtr& operator -= (const ByteString& rhs);

    // Indexing
    Subscript RestrictIndex(Subscript Index_) const;
    Boolean IndexInRange(Subscript Index_) const;
    Fallible<char> operator [] (Subscript i) const;
    Fallible<char> operator [] (Subscript i);
    const char* operator () (Subscript i) const;
    char* operator () (Subscript i);
    char operator * () const;
    char& operator * ();

    // conversion to C built in string type
    operator const char* () const;
    operator char* ();

    // return pointer to enclosed ByteStringEncLetter object
    const ByteStringEncLetter* operator -> () const;
    const ByteString& EnclosedObject() const;
    Boolean IsOld() const;
};

/****************************************************************************/
// ByteStringPtr class -- functions in much the same way as a const pointer to
// a const ByteString Object. neither the internal data nor the the pointer to
// the actual string of characters can be altered. Also assignment "=" is
// disallowed and only const methods are provided
// similar to returning const char *const which means constant pointer to a
// const array of chars
// Objects of this class should only be declared as const objects
class ConstByteStringPtr
{
  private:
    ConstByteStringPtr& operator = (const ByteString& Obj_)
	{ return *this; }
    ConstByteStringPtr& operator = (const ConstByteStringPtr& Obj_)
	{ return *this; }

  protected:
    const ByteString* _LetterPtr;
    ByteString _Letter;

  public:
    ConstByteStringPtr(const ByteString& Obj_);
    ConstByteStringPtr(const ConstByteStringPtr& Obj_);

    // Indexing
    Subscript RestrictIndex(Subscript Index_) const;
    Boolean IndexInRange(Subscript Index_) const;
    Fallible<char> operator [] (Subscript i) const;
    const char* operator () (Subscript i) const;
    char operator * () const;

    // conversion to C built in string type
    operator const char* () const;

    // return pointer to enclosed ByteStringEncLetter object
    const ByteString* operator -> () const;
    const ByteString& EnclosedObject() const;
    Boolean IsOld() const;
};

/****************************************************************************/
//// overloaded operators ////
inline ByteString operator + (const ByteString& lhs, const char* rhs)
	{ return (lhs + ByteString(rhs, ::strlen(rhs))); }
inline ByteString operator + (const char* lhs, const ByteString& rhs)
	{ return (rhs + ByteString(lhs, ::strlen(lhs))); }

inline ByteString operator - (const ByteString& lhs, const char* rhs)
	{ return (lhs - ByteString(rhs, ::strlen(rhs))); }
inline ByteString operator - (const char* lhs, const ByteString& rhs)
	{ return (ByteString(lhs, ::strlen(lhs)) - rhs); }

inline ByteString Delete(const ByteString& lhs, const char* rhs)
	{ return ByteString(lhs).Delete(ByteString(rhs, ::strlen(rhs))); }
inline ByteString Delete(const char* lhs, const ByteString& rhs)
	{ return ByteString(lhs, ::strlen(lhs)).Delete(rhs); }

inline Boolean operator == (const ByteString& Lhs_, const char* Rhs_)
	{ return (Lhs_ == ByteString(Rhs_, ::strlen(Rhs_))); }
inline Boolean operator == (const char* Lhs_, const ByteString& Rhs_)
	{ return (ByteString(Lhs_, ::strlen(Lhs_)) == Rhs_); }

inline Boolean operator != (const ByteString& Lhs_, const char* Rhs_)
	{ return (Lhs_ != ByteString(Rhs_, ::strlen(Rhs_))); }
inline Boolean operator != (const char* Lhs_, const ByteString& Rhs_)
	{ return (ByteString(Lhs_, ::strlen(Lhs_)) != Rhs_); }

inline Boolean operator <= (const ByteString& Lhs_, const char* Rhs_)
	{ return (Lhs_ <= ByteString(Rhs_, ::strlen(Rhs_))); }
inline Boolean operator <= (const char* Lhs_, const ByteString& Rhs_)
	{ return (ByteString(Lhs_, ::strlen(Lhs_)) <= Rhs_); }

inline Boolean operator >= (const ByteString& Lhs_, const char* Rhs_)
	{ return (Lhs_ >= ByteString(Rhs_, ::strlen(Rhs_))); }
inline Boolean operator >= (const char* Lhs_, const ByteString& Rhs_)
	{ return (ByteString(Lhs_, ::strlen(Lhs_)) >= Rhs_); }

inline Boolean operator < (const ByteString& Lhs_, const char* Rhs_)
	{ return (Lhs_ < ByteString(Rhs_, ::strlen(Rhs_))); }
inline Boolean operator < (const char* Lhs_, const ByteString& Rhs_)
	{ return (ByteString(Lhs_, ::strlen(Lhs_)) < Rhs_); }

inline Boolean operator > (const ByteString& Lhs_, const char* Rhs_)
	{ return (Lhs_ > ByteString(Rhs_, ::strlen(Rhs_))); }
inline Boolean operator > (const char* Lhs_, const ByteString& Rhs_)
	{ return (ByteString(Lhs_, ::strlen(Lhs_)) > Rhs_); }

//// one-liner member methods ////
inline Subscript ByteString::RestrictIndex(Subscript Index_) const
{ return StringImp::RestrictIndex(Index_, 1); }

inline Boolean ByteString::IndexInRange(Subscript Index_) const
{ return StringImp::IndexInRange(Index_, 1); }

inline Boolean ByteString::IsOld() const
{ return StringImp::IsOld(); }

inline void ByteString::AssignLetter(ByteString& Obj_)
{ StringImp::AssignLetter(Obj_); }

//// Browser methods ////
inline ByteString::Browser::Browser(const ByteString& s) : cur(s.c_str()), endp(cur + s.strlen()) {}
inline Boolean ByteString::Browser::more() const { return cur < endp; }
inline void    ByteString::Browser::advance()    { ++cur; }
inline char    ByteString::Browser::current() const { return *cur; }

//// Iterator methods ////
inline ByteString::Iterator::Iterator(ByteString& s) : cur(s.c_str()), endp(cur + s.strlen()) {}
inline Boolean ByteString::Iterator::more() const { return cur < endp; }
inline void    ByteString::Iterator::advance()    { ++cur; }
inline char& ByteString::Iterator::current() const { return *cur; }

// Browser constructor accepting iterator argument
inline ByteString::Browser::Browser(const ByteString::Iterator& iter):
cur(iter.cur), endp(iter.endp) {}

// I/O
template<class T>
ByteString& operator << (ByteString& s, const T& obj); // Format into string object

// Declare commonly used cases of << to avoid proliferation of functions
ByteString& operator << (ByteString&, const char*);  // Special case

ByteString& operator << (ByteString&, const char&);
ByteString& operator << (ByteString&, const int&);
ByteString& operator << (ByteString&, const unsigned int&);
ByteString& operator << (ByteString&, const long int&);
ByteString& operator << (ByteString&, const unsigned long int&);
ByteString& operator << (ByteString&, const float&);
ByteString& operator << (ByteString&, const double&);

#if USE_STRSTREAM
template <class T>
ByteString& operator << (ByteString& s, const T& obj)
{
  ostrstream os;
  os << obj;
  char* p = os.str();      // Get pointer to string and "freeze" buffer
  s += ByteString(p, os.pcount());
  os.rdbuf()->freeze(0);   // "Thaw" buffer so it will be deleted when os is destroyed
  return s;
}
#endif

#endif




