#ifndef STRBASE_H
#define STRBASE_H

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
    #ifndef INCL_SSTREAM_H
      #include <sstream>
      #define INCL_SSTREAM_H
    #endif
  #else
    #ifndef INCL_SSTREAM_H
      #include <sstream>
      #define INCL_SSTREAM_H
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
#ifndef FALLIBLE_H
  #include "fallible.h"
#endif
#ifndef OBJECTIMP_H
  #include "objimp.h"
#endif
#ifndef OBJECTACCEPTOR_H
  #include "objaccpt.h"
#endif
#ifndef STRUTIL_H
  #include "strutil.h"
#endif

#define STR_TRACE       0
#if STR_TRACE
#  define STRMARKER(x, strm)    XMARKER(x, strm)
#else
#  define STRMARKER(x, strm)
#endif

class StringImp;
/****************************************************************************/
class StringData
{
  friend class StringImp;
  DEFAULT_ROOTOBJECT_DEFN(StringData)

  protected:
    StringData(Subscript Sz_);
    StringData(char* Str_, Subscript Sz_);

    void DecShareCnt();
    void IncShareCnt();

  public:
    char* data; 		// Pointer to built-in character string
    Subscript _Size;
    Subscript _ShareCnt;	// Share count used for instance sharing
    Boolean _Old;

    static Boolean _AutoUpdatePtrs;
    static char _DummyData;

    virtual ~StringData();

    inline Boolean IsShareSafe() const
	{ return (_ShareCnt == 1 || _AutoUpdatePtrs); }
    inline Boolean IsOld() const
	{ return _Old; }

    static char* AllocateStr(Subscript Bytes_);
    static char* ReallocateStr(char* Addr_, Subscript Bytes_);
    static void DeallocateStr(char* Addr_);

    static void SetAutoUpdate(Boolean Flag_);
    static char* DummyData();

    virtual StringData* NewInstance(Subscript Sz_) = 0;
    virtual StringData* NewInstance(char* Str_, Subscript Len_, Subscript Sz_) = 0;

    virtual void Length(Subscript Value_) = 0;
    virtual Subscript Length() const = 0;

    char& operator [] (Subscript i)
	{ return data[i]; }

    StringData* ShareInstance();

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
class StringImp : public ObjectImp
{
  protected:
    StringData* _StrData;

    virtual void TerminateString(size_t Sz_) = 0;
    virtual void SetStr(const char* p,
			Subscript pos = 0,
			Subscript len = 10000);

    StringData* ShouldDisOwnData();
    void DisOwnSharedData(StringData* DisOwnedData_);

    char* ReallocateData(Subscript Bytes_);
    void AssignLetter(StringImp& Obj_);
    Boolean IsOld() const;

  public:
    // Constructors
    StringImp(StringData* Data_);
    StringImp(const StringImp& Obj_, StringData* Data_);

    // Destructor
    virtual ~StringImp();

    // Hashing methods
    virtual const char* GiveHashableData() const;
    virtual long GiveHashableDataLength() const;

    // Case Conversion
    void ToUpper(void);		// convert to uppercase
    void ToLower(void);		// convert to lowercase

    // Formatting
    void LeftJustify(Subscript FieldWidth_, char Pad_);
    void RightJustify(Subscript FieldWidth_, char Pad_);
    void CenterJustify(Subscript FieldWidth_, char Pad_);

    // Duplication, Insertion, deletion and replacement
    void Duplicate(Subscript Num_);
    void Insert(Subscript pos, const StringImp& Str_);
    void Append(const StringImp& rhs, Subscript RunLen_);
    void OverWrite(Subscript pos, const StringImp& Str_);
    void Delete(Subscript pos, Subscript len = 1);

    // character set functions
    void MakeCharSet(int(*iscsetfn)(int));
    void MakeCharSet(const char* Src_, size_t RunLen_=0);
    Boolean InCharSet(char ch) const;

    // Assignment
    void Assignment(const StringImp& Str_);   // Assign from StringImp
    void Assignment(const char* Str_, Subscript Len_);
    void Assignment(char Chr_);

    // Concatenation
    void Addition(const StringImp& rhs);
    void Subtraction(const StringImp& rhs);

    // Indexing
    Boolean IndexInRange(Subscript Index_, size_t ExcludeNull_) const;
    Subscript RestrictIndex(Subscript Index_, size_t ExcludeNull_) const;
    virtual Fallible<char> operator [] (Subscript i) const = 0;
    virtual Fallible<char> operator [] (Subscript i) = 0;
    virtual const char* operator () (Subscript i) const = 0;
    virtual char* operator () (Subscript i) = 0;
    virtual char operator * () const = 0;
    virtual char& operator * () = 0;

    // base conversion functions
    int IsIntBin() const;
    int IsIntOct() const;
    int IsIntDec() const;
    int IsIntHex() const;

    int BinToHex();
    int BinToCmp2Bin(int NumBytes_=0);
    int Cmp2BinToBin();
    int BytesInCmp2Bin() const;

    // number to string conversion functions
    void LongFloatToStr(Ldouble value, int width, int prec);
    void FloatToStr(double value, int width, int prec);
    void UlongToStr(unsigned long value, int radix=10);
    void LongToStr(long value, int radix=10);
    void UshortToStr(unsigned short value, int radix=10);
    void ShortToStr(short value, int radix=10);
    void UintToStr(unsigned int value, int radix=10);
    void IntToStr(int value, int radix=10);
    void CharToStr(unsigned char ch);

    void AddPosSign();
    void AddRadixPrefix(int radix);

    // Filter functions
    void RemoveLeading(const char* chset);
    void RemoveTrailing(const char* chset);
    void RemovePadding(const char* chset);
    void RemoveChar(const char* chset);

    // Searches
    virtual Fallible<Subscript> find(char c) const = 0; 			// First occurence of c
    virtual Fallible<Subscript> findlast(char c) const = 0;			// Last occurence of c
    virtual Fallible<Subscript> findnext(Subscript pos, char c) const = 0;	// Next occurence of s
    virtual Fallible<Subscript> findprevious(Subscript pos, char c) const = 0;	// Last occurence of s

    // Sub-strings
    void Left(Subscript len)
	{ SetStr(_StrData->data, 0, len); }
    void Right(Subscript len)
	{ SetStr(_StrData->data, strlen() - len, len); }
    void Mid(Subscript pos, Subscript len)
	{ SetStr(_StrData->data, pos, len); }

    // String termination
    virtual StringImp& NullTerminate() = 0;

    // Miscellaneous
    void StrRev();
    Subscript   strlen() const;
    const char* c_str()  const;
    char*       c_str();
    Boolean HasNullData() const;

    // Conversion into numbers
    virtual Fallible<double>        strtod()  const = 0;    // Conversion to double
    virtual Fallible<long>          strtol()  const = 0;    // Conversion to long
    virtual Fallible<unsigned long> strtoul() const = 0;    // Conversion to unsigned long

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

inline Boolean StringImp::IsOld() const
{ return _StrData->_Old; }

inline void StringImp::StrRev()
{ ByteStrReverse(_StrData->data, strlen()); }

inline const char* StringImp::c_str() const
{ return _StrData->data; }

inline char* StringImp::c_str()
{ return _StrData->data; }

inline Boolean StringImp::HasNullData() const
{ return !_StrData->data; }

inline Subscript StringImp::strlen() const
{ return _StrData->Length(); }

/****************************************************************************/
#endif




