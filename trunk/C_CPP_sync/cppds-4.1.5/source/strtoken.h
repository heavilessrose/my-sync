#ifndef STRTOKEN_H
#define STRTOKEN_H
#ifndef STRUTIL_H
  #include "strutil.h"
#endif

/****************************************************************************/
class StringToken
{
  private:
    const char* _SearchStr;	// The character string the search on
    const char* _Delims;	// Delimiters in the search string
    const char* _WhiteSpace;	// whitespace in the search string

    size_t _Start;
    size_t _Index;

  public:
    StringToken();
    StringToken(const StringToken& Stok_);
    StringToken(const char* Str_, size_t Start_, const char* Delims_, const char* Wspc_);

    StringToken& operator = (const StringToken& Stok_);

    inline const char* GetString() const
	{ return _SearchStr; }
    inline size_t GetStartIndex() const
	{ return _Start; }
    inline size_t GetIndex() const
	{ return _Index; }
    inline const char* GetDelimiters() const
	{ return _Delims; }
    inline const char* GetWhiteSpace() const
	{ return _WhiteSpace; }

    StringToken& SetStartIndex(size_t Index_);
    StringToken& ResetIndex();
    StringToken& SetDelimiters(const char* Str_);
    StringToken& SetWhiteSpace(const char* Str_);
    StringToken& SetString(const char* Str_);

    size_t FindToken(char* Token_);
    size_t FindToken(char*& Token_, Boolean AllocMemory_);

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




