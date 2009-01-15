// String Class
//
#ifndef LONESTRING_H
#define LONESTRING_H

#ifndef LANGSUPP_H
  #include "langsupp.h"
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef INCL_IOMANIP_H
  #include <iomanip.h>
  #define INCL_IOMANIP_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif

#if USE_STRSTREAM
  #if HAS_LONGFILENAME
    #ifndef INCL_STRSTREAM_H
      #include <strstream.h>
      #define INCL_STRSTREAM_H
    #endif
  #else
    #ifndef INCL_STRSTREAM_H
      #include <strstrea.h>
      #define INCL_STRSTREAM_H
    #endif
  #endif
#else
  #ifndef INCL_SSTREAM_H
    #include <sstream>
    #define INCL_SSTREAM_H
  #endif
#endif

#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_ERRNO_H
  #include <errno.h>
  #define INCL_ERRNO_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef SUBSCRPT_H
  #include "subscrpt.h"
#endif
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
#ifndef STRUTIL_H
  #include "strutil.h"
#endif
#ifndef LONEFALL_H
  #include "lonefall.h"
#endif
#ifndef STRTOKEN_H
  #include "strtoken.h"
#endif

#define LONESTRING_DEBUG        0

class istream;
class ostream;

/****************************************************************************/
class LoneString
{
  friend LoneString& RootObject(LoneString& Obj_)
	{ return Obj_; }
  friend const LoneString& RootConstObject(const LoneString& Obj_)
	{ return Obj_; }
  friend int _WhatIs(const LoneString& Obj_)
        { return Obj_.ClassID(); }        

  friend Boolean operator == (const LoneString& obj1, const LoneString& obj2);
  friend Boolean operator != (const LoneString& obj1, const LoneString& obj2);
  friend Boolean operator > (const LoneString& obj1, const LoneString& obj2);
  friend Boolean operator < (const LoneString& obj1, const LoneString& obj2);
  friend Boolean operator >= (const LoneString& obj1, const LoneString& obj2);
  friend Boolean operator <= (const LoneString& obj1, const LoneString& obj2);

  friend Boolean _IsGreaterThan(const LoneString& Trg_, const LoneString& Src_)
	{ return Trg_.IsGreater(Src_); }
  friend Boolean _IsLessThan(const LoneString& Trg_, const LoneString& Src_)
	{ return Trg_.IsLesser(Src_); }
  friend Boolean _IsEqualTo(const LoneString& Trg_, const LoneString& Src_)
	{ return Trg_.IsEqual(Src_); }  

#if defined(__TURBOC__) | defined (__BORLANDC__)
  friend ostream& Write(ostream& os, const LoneString& obj, Boolean* Ok_=NULL)
	{ return obj.BinaryWrite(os, Ok_); }
  friend istream& Ignore(istream& is, LoneString& obj, Boolean* Ok_=NULL)
	{ return obj.BinaryIgnore(is, Ok_); }
  friend istream& Read(istream& is, LoneString& obj, Boolean* Ok_=NULL)
	{ return obj.BinaryRead(is, Ok_); }
#else
  friend ostream& _Write(ostream& os, const LoneString& obj, Boolean* Ok_)
	 { return obj.BinaryWrite(os, Ok_); }
  friend istream& _Ignore(istream& is, LoneString& obj, Boolean* Ok_)
	 { return obj.BinaryIgnore(is, Ok_); }
  friend istream& _Read(istream& is, LoneString& obj, Boolean* Ok_)
	 { return obj.BinaryRead(is, Ok_); }
#endif

  friend ostream& operator << (ostream& os, const LoneString& obj)
	 { return obj.TextWrite(os); }
  friend istream& operator >> (istream& is, LoneString& obj)
	 { return obj.TextRead(is); }

  friend ostream& _WriteToOutput(ostream& os, const LoneString& Obj_)
	{ os <<Obj_; return os; }
  friend istream& _ReadFromInput(istream& is, LoneString& Obj_)
	{ is >>Obj_; return is; }

  friend LoneString* _Clone(const LoneString& Obj_)
	{ return Obj_.Clone(); }
  friend const LoneString* _ConstClone(const LoneString& Obj_)
	{ return &Obj_; }
  friend LoneString* _NonConstClone(LoneString& Obj_)
	{ return &Obj_; }

  friend void _Swap(LoneString& x, LoneString& y)
	{ LoneString Temp_ = x;
	  x = y;
	  y = Temp_; }
  friend long _SizeOfData(const LoneString& Obj_)
	{ return Obj_.DataSize(); }
  friend long _FormWidthOfObject(const LoneString& Obj_)
	{ return Obj_.StorableFormWidth(); }
  friend LoneString& _Assign(LoneString& trg, const LoneString& src)
	{ trg = src;
	  return trg; }
  friend LoneString& _NonConstAssign(LoneString& Trg_, LoneString& Src_)
	{ Trg_ = Src_;
	  return Trg_; }

  friend Boolean _IsStreamable(const LoneString& Obj_)
	{ return TRUE; }
  friend Boolean _IsSortable(const LoneString& Obj_)
	{ return TRUE; }
  friend Boolean _IsMatchable(const LoneString& Obj_)
	{ return TRUE; }          

  private:
    Byte_t _IsNull;

  protected:
    virtual Boolean IsEqualString(const LoneString* Ptr_) const;
    virtual Boolean IsLesserString(const LoneString* Ptr_) const;
    virtual Boolean IsGreaterString(const LoneString* Ptr_) const;

    virtual void SetStr(const char* p,
			Subscript pos = 0,
			Subscript len = 10000);

#if OVERLOAD_NEW
    static char* AllocateStr(Subscript Bytes_)
	{ return (char*)MemMatrix::Matrix().Allocate(sizeof(char) * Bytes_); }
    static char* ReallocateStr(char* Addr_, Subscript Bytes_)
	{ return (char*)MemMatrix::Matrix().Reallocate(Addr_, sizeof(char) * Bytes_); }
    static void DeallocateStr(char* Addr_)
	{ MemMatrix::Matrix().Deallocate(Addr_); }
#else
    static char* AllocateStr(Subscript Bytes_)
	{ return (new char[Bytes_]); }
    static char* ReallocateStr(char* Addr_, Subscript Bytes_)
	{ return ResizeString(Addr_, Bytes_, STD_NEW); }
    static void DeallocateStr(char* Addr_)
	{ delete[] Addr_; }
#endif

    int CheckClassType(int Id_) const
	{ return (Id_ == TypeInfo::LONESTRING_TYPE); }

    Boolean ReadObjectData(istream& Is_, Boolean Ignore_);
    Boolean WriteObjectData(ostream& Os_) const;

  public:
    // Constructors
    LoneString();                           // Null string
    LoneString(const char*);                // From built-in string
    LoneString(const char*, Subscript n);   // ... for at most n characters
    LoneString(char c, Subscript n=1);      // From single character * n times
    LoneString(const LoneString&);              // From another String

    // Virtual constructor and assignment methods
    static LoneString* Make();
    static LoneString* Make(char Ch_, Subscript n);
    static LoneString* Make(const char* Str_);
    static LoneString* Make(const char* Str_, Subscript n);
    static LoneString* Make(const LoneString& Obj_);

    virtual LoneString* Clone() const;

    // Static null object accessor methods
    static LoneString& GiveNullObject();

    // Null object methods
    virtual Boolean IsNullObject() const;
    virtual const LoneString& SetAsNullObject() const;
    virtual LoneString& SetAsNullObject();
    virtual const LoneString& NULLOBJECT() const;
    virtual LoneString& NULLOBJECT();

    // ----- Saving and restoration in binary form ----
    virtual long DataSize () const;
    virtual long StorableFormWidth () const;

    // Class Identification methods
    static int StaticID()
	{ return TypeInfo::LONESTRING_TYPE; }
    virtual int ClassID() const;
    virtual const char* ClassName() const;

    // ----- Saving and restoration in binary form ----
    virtual istream& BinaryIgnore(istream& Is_, Boolean* Ok_=NULL);
    virtual istream& BinaryRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& BinaryWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Text input and output
    virtual istream& TextRead(istream& Is_, Boolean* Ok_=NULL);
    virtual ostream& TextWrite(ostream& Os_, Boolean* Ok_=NULL) const;

    // Comparison methods
    virtual Boolean IsEqual(const LoneString& Obj_) const;
    virtual Boolean IsLesser(const LoneString& Obj_) const;
    virtual Boolean IsGreater(const LoneString& Obj_) const;
    
    Boolean StrniComp(const LoneString& s2, size_t max) const
	{ return ::StrniComp(data, s2.data, max); }
    Boolean StriComp(const LoneString& s2) const
	{ return ::StriComp(data, s2.data); }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

    // Case Conversion
    LoneString& ToUpper(void);			// convert to uppercase
    LoneString& ToLower(void);			// convert to lowercase

    // Formatting
    LoneString& LeftJustify(Subscript FieldWidth_, char Pad_=' ');
    LoneString& RightJustify(Subscript FieldWidth_, char Pad_=' ');
    LoneString& CenterJustify(Subscript FieldWidth_, char Pad_=' ');

    // Duplication, Insertion, deletion and replacement
    LoneString& Duplicate(Subscript Num_);
    LoneString& Insert(Subscript pos, const LoneString& Str_);
    LoneString& Append(const LoneString& rhs, Subscript RunLen_);
    LoneString& OverWrite(Subscript pos, const LoneString& Str_);
    LoneString& Delete(Subscript pos, Subscript len = 1);
    Subscript Replace(Subscript pos,
		      const LoneString& from,
		      const LoneString& to,
		      Subscript count=-1);

    // character set functions
    LoneString& MakeCharSet(int(*iscsetfn)(int))
	{ return (*this = ::MakeCharSet(iscsetfn)); }
    Boolean InCharSet(char ch)
	{ return ::InCharSet(ch, c_str()); }

    // Assignment
    virtual LoneString& operator = (char Chr_);            // ... from single character
    virtual LoneString& operator = (const char* Str_);     // ... from built-in LoneString
    virtual LoneString& operator = (const LoneString& Str_);   // Assign from LoneString

    // Destructor
    virtual ~LoneString();

    // Indexing
    Subscript RestrictIndex(Subscript Index_) const;
    Boolean IndexInRange(Subscript Index_) const;
    LoneFallible<char> operator[](Subscript i) const;
    LoneFallible<char> operator[](Subscript i);
    const char* operator()(Subscript i) const;
    char* operator()(Subscript i);
    char operator * () const;
    char& operator * ();

    // Concatenation
    virtual LoneString& operator += (const LoneString& rhs);
    virtual LoneString& operator -= (const LoneString& rhs);
    virtual LoneString& Delete(const LoneString& rhs);

    friend LoneString operator + (const LoneString& lhs, const LoneString& rhs);
    friend LoneString operator - (const LoneString& lhs, const LoneString& rhs);
    friend LoneString Delete(const LoneString& lhs, const LoneString& rhs);

    // Searches
    LoneFallible<Subscript> find(const LoneString& s)   const;  	              // First occurence of s
    LoneFallible<Subscript> find(char c)            const;  	                      // First occurence of c
    LoneFallible<Subscript> findlast(const LoneString& s)   const;  	              // Last occurence of s
    LoneFallible<Subscript> findlast(char c)        const;  	                      // Last occurence of c
    LoneFallible<Subscript> findnext(Subscript pos, char c)   const;  		      // Next occurence of s
    LoneFallible<Subscript> findnext(Subscript pos, const LoneString& s) const;       // Next occurence of s
    LoneFallible<Subscript> findprevious(Subscript pos, char c)   const;  	      // Last occurence of s
    LoneFallible<Subscript> findprevious(Subscript pos, const LoneString& s) const;   // Last occurence of s
    LoneFallible<Subscript> brk(const LoneString& s)    const;                        // First occurence of any character in s
    Subscript           span(const LoneString& s)   const;                            // Size of initial span of characters in s
    Subscript           cspan(const LoneString& s)  const;                            // Size of initial span of characters not in s

    // Sub-strings
    LoneString subString(Subscript start             ) const;  // From start
    LoneString subString(Subscript start, Subscript n) const;  // ... for n characters
    LoneString& Left(Subscript len);		// left   len chars
    LoneString& Right(Subscript len);		// right  len chars
    LoneString& Mid(Subscript pos,			// middle len chars from pos
		Subscript len);

    // Miscellaneous
    LoneString& StrRev();
    Subscript   strlen() const;  // Length of string (not including null)
    const char* c_str()  const;  // Pointer to underlying built-in string
    char*       c_str();         // Pointer to underlying built-in string

    // Conversion into numbers
    LoneFallible<double>        strtod()  const;    // Conversion to double
    LoneFallible<long>          strtol()  const;    // Conversion to long
    LoneFallible<unsigned long> strtoul() const;    // Conversion to unsigned long

    // Iterators
    class Iterator; // Forward declaration

    class Browser
    {
      public:
	Browser(const LoneString& s);

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
	Iterator(LoneString& s);

	Boolean more() const;
	void    advance();
	char&   current() const;

      private:
	friend Browser::Browser(const Iterator&);
	char* cur;
	const char* endp;
    };

  private:
    char* data; // Pointer to built-in character string
    Subscript _Size;

    LoneString(const LoneString&, const LoneString&);
};

/****************************************************************************/
int CompLoneString(const LoneString& obj1_, const LoneString& obj2_);
LoneString* Clone(const LoneString& obj);

inline LoneString operator + (const LoneString& lhs, const char* rhs)
	{ return (lhs + LoneString(rhs)); }
inline LoneString operator + (const char* lhs, const LoneString& rhs)
	{ return (rhs + LoneString(lhs)); }

inline LoneString operator - (const LoneString& lhs, const char* rhs)
	{ return (lhs - LoneString(rhs)); }
inline LoneString operator - (const char* lhs, const LoneString& rhs)
	{ return (LoneString(lhs) - rhs); }

inline LoneString Delete(const LoneString& lhs, const char* rhs)
	{ return LoneString(lhs).Delete(LoneString(rhs)); }
inline LoneString Delete(const char* lhs, const LoneString& rhs)
	{ return LoneString(lhs).Delete(rhs); }

inline Boolean operator == (const LoneString& Lhs_, const char* Rhs_)
	{ return (Lhs_ == LoneString(Rhs_)); }
inline Boolean operator == (const char* Lhs_, const LoneString& Rhs_)
	{ return (LoneString(Lhs_) == Rhs_); }
inline Boolean operator == (const LoneString& Lhs_, const LoneString& Rhs_)
        { return Lhs_.IsEqual(Rhs_); }

inline Boolean operator != (const LoneString& Lhs_, const char* Rhs_)
	{ return (Lhs_ != LoneString(Rhs_)); }
inline Boolean operator != (const char* Lhs_, const LoneString& Rhs_)
	{ return (LoneString(Lhs_) != Rhs_); }
inline Boolean operator != (const LoneString& Lhs_, const LoneString& Rhs_)
	{ return !Lhs_.IsEqual(Rhs_); }

inline Boolean operator <= (const LoneString& Lhs_, const char* Rhs_)
	{ return (Lhs_ <= LoneString(Rhs_)); }
inline Boolean operator <= (const char* Lhs_, const LoneString& Rhs_)
	{ return (LoneString(Lhs_) <= Rhs_); }
inline Boolean operator <= (const LoneString& Lhs_, const LoneString& Rhs_)
        { return (Lhs_.IsEqual(Rhs_) || Lhs_.IsLesser(Rhs_)); }

inline Boolean operator >= (const LoneString& Lhs_, const char* Rhs_)
	{ return (Lhs_ >= LoneString(Rhs_)); }
inline Boolean operator >= (const char* Lhs_, const LoneString& Rhs_)
	{ return (LoneString(Lhs_) >= Rhs_); }
inline Boolean operator >= (const LoneString& Lhs_, const LoneString& Rhs_)
       { return (Lhs_.IsEqual(Rhs_) || Lhs_.IsGreater(Rhs_)); }

inline Boolean operator < (const LoneString& Lhs_, const char* Rhs_)
	{ return (Lhs_ < LoneString(Rhs_)); }
inline Boolean operator < (const char* Lhs_, const LoneString& Rhs_)
	{ return (LoneString(Lhs_) < Rhs_); }
inline Boolean operator < (const LoneString& Lhs_, const LoneString& Rhs_)
       { return Lhs_.IsLesser(Rhs_); }

inline Boolean operator > (const LoneString& Lhs_, const char* Rhs_)
	{ return (Lhs_ > LoneString(Rhs_)); }
inline Boolean operator > (const char* Lhs_, const LoneString& Rhs_)
	{ return (LoneString(Lhs_) > Rhs_); }
inline Boolean operator > (const LoneString& Lhs_, const LoneString& Rhs_)
       { return Lhs_.IsGreater(Rhs_); }
        
inline const char* LoneString::c_str() const
{
    return data;
}

inline char* LoneString::c_str()
{
    return data;
}

inline Subscript LoneString::strlen() const { return SafeStrLen(data); }

inline LoneString::Browser::Browser(const LoneString& s) : cur(s.c_str()), endp(cur + s.strlen()) {}
inline Boolean LoneString::Browser::more() const { return cur < endp; }
inline void    LoneString::Browser::advance()    { ++cur; }
inline char    LoneString::Browser::current() const { return *cur; }

inline LoneString::Iterator::Iterator(LoneString& s) : cur(s.c_str()), endp(cur + s.strlen()) {}
inline Boolean LoneString::Iterator::more() const { return cur < endp; }
inline void    LoneString::Iterator::advance()    { ++cur; }
inline char& LoneString::Iterator::current() const { return *cur; }

inline LoneString::Browser::Browser(const LoneString::Iterator& iter) : cur(iter.cur), endp(iter.endp) {}

// I/O
template<class T>
LoneString& operator << (LoneString& s, const T& obj); // Format into LoneString object

// Declare commonly used cases of << to avoid proliferation of functions
LoneString& operator << (LoneString&, const char*);  // Special case

LoneString& operator << (LoneString&, const char&);
LoneString& operator << (LoneString&, const int&);
LoneString& operator << (LoneString&, const unsigned int&);
LoneString& operator << (LoneString&, const long int&);
LoneString& operator << (LoneString&, const unsigned long int&);
LoneString& operator << (LoneString&, const float&);
LoneString& operator << (LoneString&, const double&);

template <class T>
LoneString& operator << (LoneString& s, const T& obj)
{
  std::ostringstream oss;
  std::string string;
  oss << obj;
  string = oss.str();      // Get pointer to string and "freeze" buffer
  s += LoneString(string.c_str());
  return s;
}

#endif





