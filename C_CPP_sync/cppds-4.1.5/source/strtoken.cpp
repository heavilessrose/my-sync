#ifndef STRTOKEN_CPP
#define STRTOKEN_CPP
#ifndef STRTOKEN_H
  #include "strtoken.h"
#endif

/****************************************************************************/
/*************************** Key Word Token Class ***************************/
/****************************************************************************/
StringToken::StringToken():
_SearchStr(NULL),
_Delims(NULL),
_WhiteSpace(NULL),
_Index(0),
_Start(0)
{}

/****************************************************************************/
StringToken::StringToken(const StringToken& Stok_):
_SearchStr(Stok_.GetString()),
_Delims(Stok_.GetDelimiters()),
_WhiteSpace(Stok_.GetWhiteSpace()),
_Index(Stok_.GetIndex()),
_Start(Stok_.GetStartIndex())
{}

/****************************************************************************/
StringToken& StringToken::operator = (const StringToken& Stok_)
{
  if (&Stok_ != this)
  {
    _SearchStr = Stok_.GetString();
    _Delims = Stok_.GetDelimiters();
    _WhiteSpace = Stok_.GetWhiteSpace();
    _Index = Stok_.GetIndex();
    _Start = Stok_.GetStartIndex();
  }

  return *this;
}

/****************************************************************************/
StringToken::StringToken(const char* Str_, size_t Start_, const char* Delims_, const char* Wspc_):
_SearchStr(Str_),
_Delims(Delims_),
_WhiteSpace(Wspc_),
_Index(Start_),
_Start(Start_)
{}

/****************************************************************************/
size_t StringToken::FindToken(char* Token_)
{
  const char* Delimiters_ = (_Delims == NULL) ? " ":_Delims;

  size_t NextPos_;
  size_t CopyLen_ = FindNextWord(_SearchStr, Delimiters_, _Index, NextPos_);

  if (CopyLen_)
  {
    strncpy(Token_, &_SearchStr[_Index], CopyLen_);
    Token_[CopyLen_] = 0;

    if (_WhiteSpace)
    {
      RemovePadding(Token_, _WhiteSpace);
      CopyLen_ = strlen(Token_);
    }

    _Index = NextPos_;
  }

  return CopyLen_;
}

/****************************************************************************/
size_t StringToken::FindToken(char*& Token_, Boolean AllocMemory_)
{
  const char* Delimiters_ = (_Delims == NULL) ? " ":_Delims;

  size_t NextPos_;
  size_t CopyLen_ = FindNextWord(_SearchStr, Delimiters_, _Index, NextPos_);

  if (CopyLen_)
  {
    if (AllocMemory_)
#     if OVERLOAD_NEW
	     Token_ = (char*)RawAllocateWith(MEMMATRIX, CopyLen_ + 1);
#     else
	     Token_ = (char*)RawAllocateWith(STD_NEW, CopyLen_ + 1);
#     endif

    strncpy(Token_, &_SearchStr[_Index], CopyLen_);
    Token_[CopyLen_] = 0;

    if (_WhiteSpace)
    {
      RemovePadding(Token_, _WhiteSpace);
      CopyLen_ = strlen(Token_);
    }

    _Index = NextPos_;
  }

  return CopyLen_;
}

/****************************************************************************/
StringToken& StringToken::SetStartIndex(size_t Index_)
{
  _Start = Index_;
  return *this;
}

/****************************************************************************/
StringToken& StringToken::ResetIndex()
{
  _Index = _Start;
  return *this;
}

/****************************************************************************/
StringToken& StringToken::SetDelimiters(const char* Str_)
{
  _Delims = Str_;
  return *this;
}

/****************************************************************************/
StringToken& StringToken::SetWhiteSpace(const char* Str_)
{
  _WhiteSpace = Str_;
  return *this;
}

/****************************************************************************/
StringToken& StringToken::SetString(const char* Str_)
{
  _SearchStr = Str_;
  return *this;
}

/****************************************************************************/
#if OVERLOAD_NEW
void* StringToken::operator new (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}

/****************************************************************************/
void StringToken::operator delete (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}

/****************************************************************************/
#if	HAS_ARRAY_NEW
void* StringToken::operator new[] (size_t Bytes_)
{
  return MemMatrix::Matrix().Allocate(Bytes_);
}
#endif

/****************************************************************************/
#if	HAS_ARRAY_NEW
void StringToken::operator delete[] (void* Space_)
{
  MemMatrix::Matrix().Deallocate(Space_);
}
#endif
#endif
/****************************************************************************/
/****************************************************************************/
#endif




