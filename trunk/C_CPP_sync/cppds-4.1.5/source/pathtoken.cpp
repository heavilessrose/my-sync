// getpath class, source file
//
// PURPOSE:
//   for getting the path or similar environment variable and seperating each individual path entry
//   for storage in a string array as a member of the class
//
// FILE:
//   getpath.cpp
//
#ifndef PATHTOKEN_CPP
#define PATHTOKEN_CPP

#ifndef PATHTOKEN_H
  #include "pathtoken.h"
#endif

/******************************************************************************/  
PathTokenizer::PathTokenizer():
_Max(0),
_Size(10),
_TokenArray((char**)MemMatrix::Matrix().Allocate(10 * sizeof(char*))),
#if (defined(__linux__) | defined(__unix__)) & !defined(__DJGPP__)
  _TokenSeparator(":"),
  _DirSeparator("/")
#else
  _TokenSeparator(";"),
  _DirSeparator("\\")
#endif
{}

/******************************************************************************/  
PathTokenizer::PathTokenizer(const char* VarName_, int SysType_):
_Max(0),
_Size(10),
_TokenArray((char**)MemMatrix::Matrix().Allocate(10 * sizeof(char*))),
_TokenSeparator((SysType_ == DOS_PATH) ? ";":":"),
_DirSeparator((SysType_ == DOS_PATH) ? "\\":"/")
{
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = NewString(EnvStrVal_);
    Tokenize(NewStr_);
    ::DeleteArray(NewStr_);
  }
}

/******************************************************************************/
PathTokenizer::PathTokenizer(const char* VarName_, const char* TokSep_, const char* DirSep_):
_Max(0),
_Size(10),
_TokenArray((char**)MemMatrix::Matrix().Allocate(10 * sizeof(char*))),
_TokenSeparator(TokSep_),
_DirSeparator(DirSep_)
{
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = NewString(EnvStrVal_);
    Tokenize(NewStr_);
    ::DeleteArray(NewStr_);
  }
}

/******************************************************************************/
PathTokenizer::~PathTokenizer()
{
  size_t i;
  for (i = 0; i < _Max; i++)
    ::DeleteArray(_TokenArray[i]);

  ::DeleteArray(_TokenArray);
}

/******************************************************************************/
void PathTokenizer::Tokenize(char* Str_)
{
  // testing string tokenizing
  char* Buffer_ = new_char_array(300, NULL);
  StringToken Tok_(Str_, 0, _TokenSeparator, NULL);
  size_t i;

  for (;;)
  {
    for (i = 0; i < 10 && Tok_.FindToken(Buffer_); i++)
      _TokenArray[_Max++] = NewString(Buffer_);
    
    if (i >= 10)
    {
      char** OldArray_ = _TokenArray;
      size_t OldSize_ = _Size;
      size_t x;

      _Size += 10;
      _TokenArray = (char**)MemMatrix::Matrix().Allocate(_Size * sizeof(char*));
    
      for (x = 0; x < OldSize_; x++)
        _TokenArray[x] = OldArray_[x];

      ::DeleteArray(OldArray_);
    }
    else
      break;
  }

  ::DeleteArray(Buffer_);
}

/******************************************************************************/  
void PathTokenizer::SetEnvVar(const char* VarName_, const char* TokSep_, const char* DirSep_)
{
  _TokenSeparator = TokSep_;
  _DirSeparator = DirSep_;
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = NewString(EnvStrVal_);
    Tokenize(NewStr_);
    ::DeleteArray(NewStr_);
  }
}

/******************************************************************************/  
void PathTokenizer::SetEnvVar(const char* VarName_, int SysType_)
{
  _TokenSeparator = (SysType_ == DOS_PATH) ? ";":":";
  _DirSeparator = (SysType_ == DOS_PATH) ? "\\":"/";
  char* EnvStrVal_ = getenv(VarName_);
  
  if (EnvStrVal_)
  {
    char* NewStr_ = NewString(EnvStrVal_);
    Tokenize(NewStr_);
    ::DeleteArray(NewStr_);
  }
}

/******************************************************************************/
size_t PathTokenizer::Max() const
{
  return _Max;
}

/******************************************************************************/
const char* PathTokenizer::TokenSeparator() const
{
  return _TokenSeparator;
}

/******************************************************************************/
const char* PathTokenizer::DirSeparator() const
{
  return _DirSeparator;
}

/******************************************************************************/
void PathTokenizer::Dump(ostream& Out_) const
{
  size_t x;
  for (x = 0; x < _Max; x++)
    Out_ <<_TokenArray[x] <<endl;
}

/******************************************************************************/
const char* PathTokenizer::operator [] (size_t Index_) const
{
  return ((Index_ < _Max) ? _TokenArray[Index_]:NULL);
}

/****************************************************************************/
void PathTokenizer::CallMethod(PathTokenizer& Obj_, const char* MethodName_, const char* VarName_, int SysType_)
{
  if (!strcmp(MethodName_, "SetEnvVar(const char*, int)"))
    Obj_.SetEnvVar(VarName_, SysType_);
}

/******************************************************************************/
/******************************************************************************/
#if PATHTOKEN_DEBUG
void main(int argc, char* argv[])
{
  PathTokenizer gp;
  int x;
  
  if (argc == 3)
  {
    if (!strcmp(argv[2], "unix") || !strcmp(argv[2], "UNIX"))
      gp.SetEnvVar(argv[1], PathTokenizer::UNIX_PATH);
    else
      gp.SetEnvVar(argv[1], PathTokenizer::DOS_PATH);
  }
  else
  {
    cout <<"PATH environment variable style\n";
    cout <<"1. UNIX\t\t2. DOS\n";
    cin >>x;

    if (x == 1)
      gp.SetEnvVar("PATH", PathTokenizer::UNIX_PATH);
    else
      gp.SetEnvVar("PATH", PathTokenizer::DOS_PATH);
  }

  size_t Max_ = gp.Max();
  for (x = 0; x < Max_; x++)
    cout <<gp[x] <<endl;    
}

#endif
/******************************************************************************/
#endif




