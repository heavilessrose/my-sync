// ChrString Class
//
#ifndef CHRSTRING_H
#define CHRSTRING_H

#ifndef STRBASE_H
  #include "strbase.h"
#endif
#ifndef STRTOKEN_H
  #include "strtoken.h"
#endif

#define STRING_DEBUG			            0
#define STRING_SHOW_DEBUG_OUTPUT                    0
#define STRING_NOTIFY_SUCCESS		            0

class istream;
class ostream;

/****************************************************************************/
class ChrStringAcceptor : public ObjectAcceptor
{
  friend class ChrString;

  protected:
    union
    {
      const ChrString* _ObjPtr;
      ChrString* _VarPtr;
    };

    ChrStringAcceptor(const ChrString* Ptr_);

  public:
    // Data transfer methods
    virtual void AcceptDataPtr(const void* Ptr_);
    virtual const void* GiveDataPtr() const;

    // ChrString class specific acceptor methods
    virtual Boolean IsString() const;
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
class ChrStringData : public StringData
{
  public:
    ChrStringData(Subscript Sz_);
    ChrStringData(char* Str_, Subscript Len_, Subscript Sz_);

    virtual StringData* NewInstance(Subscript Sz_);
    virtual StringData* NewInstance(char* Str_, Subscript Len_, Subscript Sz_);

    virtual void Length(Subscript Value_);
    virtual Subscript Length() const;

    static ChrStringData* Make(Subscript Sz_);
    static ChrStringData* Make(char* Str_, Subscript Len_, Subscript Sz_);
};

/****************************************************************************/
class ChrStringEncLetter;
class ConstChrStringPtr;
class ChrStringPtr;

class ChrString : public StringImp
{
  friend class ChrStringAcceptor;
  friend class ChrStringEncLetter;
  friend class ConstChrStringPtr;
  friend class ChrStringPtr;

  friend Object& RootObject(ChrString& Str_)
	{ return *((Object*)(&Str_)); }
  friend const Object& RootConstObject(const ChrString& Str_)
	{ return *((const Object*)(&Str_)); }

  protected:
    virtual void TerminateString(size_t Sz_);

    virtual Boolean IsEqualToChrString(const ChrString* Ptr_) const;
    virtual Boolean IsLesserToChrString(const ChrString* Ptr_) const;
    virtual Boolean IsGreaterToChrString(const ChrString* Ptr_) const;

  public:
    // Constructors
    ChrString();                           // Null string
    ChrString(const char*);                // From built-in string
    ChrString(const char*, Subscript n);   // ... for at most n characters
    ChrString(char c, Subscript n=1);      // From single character * n times
    ChrString(const StringImp& s);         // From another ChrString

    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      ChrString(const ChrString& s);
    #endif

    // Virtual constructor and assignment methods
    static ChrString* Make();
    static ChrString* Make(char Ch_, Subscript n);
    static ChrString* Make(const char* Str_);
    static ChrString* Make(const char* Str_, Subscript n);
    static ChrString* Make(const Object& Obj_);

    virtual Object& Assign(const Object& Obj_, ObjectAcceptor* ObjComp_=NULL);
    virtual Object* Clone(ObjectAcceptor* ObjComp_=NULL) const;

    // Automatically update pointers to the string
    static void AutoUpdatePtrs(Boolean Flag_=TRUE);

    // Static null object accessor methods
    static ChrString& GiveNullObject();

    // Null object accessors
    virtual const Object& NULLOBJECT() const;
    virtual Object& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;
    virtual long StorableFormWidth () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::STRING_TYPE; }
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
    Boolean StrniComp(const ChrString& s2, size_t max) const
	{ return ::StrniComp(_StrData->data, s2._StrData->data, max); }
    Boolean StriComp(const ChrString& s2) const
	{ return ::StriComp(_StrData->data, s2._StrData->data); }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

    // Case Conversion
    ChrString& ToUpper(void);		// convert to uppercase
    ChrString& ToLower(void);		// convert to lowercase

    // Formatting
    ChrString& LeftJustify(Subscript FieldWidth_, char Pad_=' ');
    ChrString& RightJustify(Subscript FieldWidth_, char Pad_=' ');
    ChrString& CenterJustify(Subscript FieldWidth_, char Pad_=' ');

    // Duplication, Insertion, deletion and replacement
    ChrString& Duplicate(Subscript Num_);
    ChrString& Insert(Subscript pos, const ChrString& Str_);
    ChrString& Append(const ChrString& rhs, Subscript RunLen_);
    ChrString& OverWrite(Subscript pos, const ChrString& Str_);
    ChrString& Delete(Subscript pos, Subscript len = 1);
    Subscript Replace(Subscript pos,
		      const ChrString& from,
		      const ChrString& to,
		      Subscript count=-1);

    // character set functions
    ChrString& MakeCharSet(int(*iscsetfn)(int));
    ChrString& MakeCharSet(const char* Src_, size_t RunLen_=0);

    // Assignment
    virtual ChrString& operator = (char Chr_);            // ... from single character
    virtual ChrString& operator = (const char* Str_);     // ... from built-in string
    virtual ChrString& operator = (const StringImp& Str_);   // Assign from ChrString
    
    #if !defined(__TURBOC__) & !defined(__BORLANDC__)
      virtual ChrString& operator = (const ChrString& Str_);
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
    virtual ChrString& operator += (const ChrString& rhs);
    virtual ChrString& operator -= (const ChrString& rhs);
    virtual ChrString& Delete(const ChrString& rhs);

    friend ChrString operator + (const ChrString& lhs, const ChrString& rhs);
    friend ChrString operator - (const ChrString& lhs, const ChrString& rhs);
    friend ChrString Delete(const ChrString& lhs, const ChrString& rhs);

    // Searches
    virtual Fallible<Subscript> find(char c) const;  				// First occurence of c
    virtual Fallible<Subscript> findlast(char c) const;  			// Last occurence of c
    virtual Fallible<Subscript> findnext(Subscript pos, char c) const;  	// Next occurence of s
    virtual Fallible<Subscript> findprevious(Subscript pos, char c) const;  	// Last occurence of s

    Fallible<Subscript> find(const ChrString& s) const;  			// First occurence of s
    Fallible<Subscript> findlast(const ChrString& s) const;  			// Last occurence of s
    Fallible<Subscript> findnext(Subscript pos, const ChrString& s) const;  	// Next occurence of s
    Fallible<Subscript> findprevious(Subscript pos, const ChrString& s) const;  // Last occurence of s
    Fallible<Subscript> brk(const ChrString& s) const;  			// First occurence of any character in s
    Subscript           span(const ChrString& s) const;  			// Size of initial span of characters in s
    Subscript           cspan(const ChrString& s) const;  			// Size of initial span of characters not in s

    // Sub-strings
    ChrString subString(Subscript start) const;  		// From start
    ChrString subString(Subscript start, Subscript n) const;  	// ... for n characters
    ChrString& Left(Subscript len);				// left   len chars
    ChrString& Right(Subscript len);				// right  len chars
    ChrString& Mid(Subscript pos,				// middle len chars from pos
		Subscript len);

    // Conversion into numbers
    virtual Fallible<double>        strtod()  const;    // Conversion to double
    virtual Fallible<long>          strtol()  const;    // Conversion to long
    virtual Fallible<unsigned long> strtoul() const;    // Conversion to unsigned long

    // String termination
    virtual StringImp& NullTerminate();

    // Miscellaneous
    ChrString& StrRev();

    // Iterators
    class Iterator; // Forward declaration

    class Browser
    {
      public:
	Browser(const ChrString& s);

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
	Iterator(ChrString& s);

	Boolean more() const;
	void    advance();
	char&   current() const;

      private:
	friend Browser::Browser(const Iterator&);
	char* cur;
	const char* endp;
    };

  private:
    ChrString(const ChrString&, const ChrString&);
    ChrString(ChrString& Obj_, int, int);
    ChrString(char*, int, int);

    void AssignLetter(ChrString& Obj_);
    Boolean IsOld() const;
};

/****************************************************************************/
// ChrStringPtr class forward declaration
class ChrStringPtr;

// Enclosed object of ChrStringPtr class
class ChrStringEncLetter : public ChrString
{
  friend class ChrStringPtr;

  private:
    ChrStringEncLetter();
    ChrStringEncLetter(ChrString& Obj_);
    ChrStringEncLetter(const ChrStringEncLetter& Obj_);

    const ChrStringEncLetter& Truncate(const ChrString& rhs) const;

  public:
    // Case Conversion
    const ChrStringEncLetter& ToUpper(void) const;
    const ChrStringEncLetter& ToLower(void) const;

    // Duplication, Insertion, deletion and replacement
    const ChrStringEncLetter& Delete(Subscript pos, Subscript len = 1) const;

    virtual ChrString& Delete(const ChrString& rhs);
    virtual const ChrStringEncLetter& Delete(const ChrString& rhs) const;

    // Miscellaneous
    const ChrStringEncLetter& StrRev() const;
};

/****************************************************************************/
// ChrStringPtr class -- functions in much the same way as a pointer
// the ChrString class. The internal ptr to the actual string can be
// manipulated to store different characters, but the ptr itself cannot be
// changed to point to another address.
// similar to returning char *const which means constant pointer to a
// non-const array of chars
class ChrStringPtr
{
  protected:
    const ChrStringEncLetter* _LetterPtr;
    ChrStringEncLetter _Letter;

  public:
    ChrStringPtr();
    ChrStringPtr(ChrString& Obj_);
    ChrStringPtr(const ChrStringPtr& Obj_);

    ChrStringPtr& operator = (ChrString& Obj_);
    ChrStringPtr& operator = (const ChrStringPtr& Obj_);
    ChrStringPtr& operator -= (const ChrString& rhs);

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

    // return pointer to enclosed ChrStringEncLetter object
    const ChrStringEncLetter* operator -> () const;
    const ChrString& EnclosedObject() const;
    Boolean IsOld() const;
};

/****************************************************************************/
// ChrStringPtr class -- functions in much the same way as a const pointer to
// a const ChrString Object. neither the internal data nor the the pointer to
// the actual string of characters can be altered. Also assignment "=" is
// disallowed and only const methods are provided
// similar to returning const char *const which means constant pointer to a
// const array of chars
// Objects of this class should only be declared as const objects
class ConstChrStringPtr
{
  private:
    ConstChrStringPtr& operator = (const ChrString& Obj_)
	{ return *this; }
    ConstChrStringPtr& operator = (const ConstChrStringPtr& Obj_)
	{ return *this; }

  protected:
    const ChrString* _LetterPtr;
    ChrString _Letter;

  public:
    ConstChrStringPtr(const ChrString& Obj_);
    ConstChrStringPtr(const ConstChrStringPtr& Obj_);

    // Indexing
    Subscript RestrictIndex(Subscript Index_) const;
    Boolean IndexInRange(Subscript Index_) const;
    Fallible<char> operator [] (Subscript i) const;
    const char* operator () (Subscript i) const;
    char operator * () const;

    // conversion to C built in string type
    operator const char* () const;

    // return pointer to enclosed ChrStringEncLetter object
    const ChrString* operator -> () const;
    const ChrString& EnclosedObject() const;
    Boolean IsOld() const;
};

/****************************************************************************/
// comparison function
int CompString(const ChrString& lhs, const ChrString& rhs);

//// overloaded operators ////
inline ChrString operator + (const ChrString& lhs, const char* rhs)
	{ return (lhs + ChrString(rhs)); }
inline ChrString operator + (const char* lhs, const ChrString& rhs)
	{ return (rhs + ChrString(lhs)); }

inline ChrString operator - (const ChrString& lhs, const char* rhs)
	{ return (lhs - ChrString(rhs)); }
inline ChrString operator - (const char* lhs, const ChrString& rhs)
	{ return (ChrString(lhs) - rhs); }

inline ChrString Delete(const ChrString& lhs, const char* rhs)
	{ return ChrString(lhs).Delete(ChrString(rhs)); }
inline ChrString Delete(const char* lhs, const ChrString& rhs)
	{ return ChrString(lhs).Delete(rhs); }

inline Boolean operator == (const ChrString& Lhs_, const char* Rhs_)
	{ return (Lhs_ == ChrString(Rhs_)); }
inline Boolean operator == (const char* Lhs_, const ChrString& Rhs_)
	{ return (ChrString(Lhs_) == Rhs_); }

inline Boolean operator != (const ChrString& Lhs_, const char* Rhs_)
	{ return (Lhs_ != ChrString(Rhs_)); }
inline Boolean operator != (const char* Lhs_, const ChrString& Rhs_)
	{ return (ChrString(Lhs_) != Rhs_); }

inline Boolean operator <= (const ChrString& Lhs_, const char* Rhs_)
	{ return (Lhs_ <= ChrString(Rhs_)); }
inline Boolean operator <= (const char* Lhs_, const ChrString& Rhs_)
	{ return (ChrString(Lhs_) <= Rhs_); }

inline Boolean operator >= (const ChrString& Lhs_, const char* Rhs_)
	{ return (Lhs_ >= ChrString(Rhs_)); }
inline Boolean operator >= (const char* Lhs_, const ChrString& Rhs_)
	{ return (ChrString(Lhs_) >= Rhs_); }

inline Boolean operator < (const ChrString& Lhs_, const char* Rhs_)
	{ return (Lhs_ < ChrString(Rhs_)); }
inline Boolean operator < (const char* Lhs_, const ChrString& Rhs_)
	{ return (ChrString(Lhs_) < Rhs_); }

inline Boolean operator > (const ChrString& Lhs_, const char* Rhs_)
	{ return (Lhs_ > ChrString(Rhs_)); }
inline Boolean operator > (const char* Lhs_, const ChrString& Rhs_)
	{ return (ChrString(Lhs_) > Rhs_); }

//// one-liner member methods ////
inline Subscript ChrString::RestrictIndex(Subscript Index_) const
{ return StringImp::RestrictIndex(Index_, 0); }

inline Boolean ChrString::IndexInRange(Subscript Index_) const
{ return StringImp::IndexInRange(Index_, 0); }

inline Boolean ChrString::IsOld() const
{ return StringImp::IsOld(); }

inline void ChrString::AssignLetter(ChrString& Obj_)
{ StringImp::AssignLetter(Obj_); }

//// Browser methods ////
inline ChrString::Browser::Browser(const ChrString& s):
cur(s.c_str()), endp(cur + s.strlen()) {}

inline Boolean ChrString::Browser::more() const { return cur < endp; }
inline void    ChrString::Browser::advance()    { ++cur; }
inline char    ChrString::Browser::current() const { return *cur; }

//// Iterator methods ////
inline ChrString::Iterator::Iterator(ChrString& s):
cur(s.c_str()), endp(cur + s.strlen()) {}

inline Boolean ChrString::Iterator::more() const { return cur < endp; }
inline void    ChrString::Iterator::advance()    { ++cur; }
inline char& ChrString::Iterator::current() const { return *cur; }

// Browser constructor accepting iterator argument
inline ChrString::Browser::Browser(const ChrString::Iterator& iter):
cur(iter.cur), endp(iter.endp) {}

// I/O
template<class T>
ChrString& operator << (ChrString& s, const T& obj); // Format into string object

// Declare commonly used cases of << to avoid proliferation of functions
ChrString& operator << (ChrString&, const char*);  // Special case

ChrString& operator << (ChrString&, const char&);
ChrString& operator << (ChrString&, const int&);
ChrString& operator << (ChrString&, const unsigned int&);
ChrString& operator << (ChrString&, const long int&);
ChrString& operator << (ChrString&, const unsigned long int&);
ChrString& operator << (ChrString&, const float&);
ChrString& operator << (ChrString&, const double&);

template <class T>
ChrString& operator << (ChrString& s, const T& obj)
{
  std::ostringstream os;
  os << obj;
  std::string p = os.str();      // Get pointer to string and "freeze" buffer
  s += ChrString(p, os.pcount());
  os.rdbuf()->freeze(0);   // "Thaw" buffer so it will be deleted when os is destroyed
  return s;
}

#endif





