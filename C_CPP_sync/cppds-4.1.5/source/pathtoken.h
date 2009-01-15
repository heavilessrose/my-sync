// getpath class, header file
//
// PURPOSE:
//   for getting the path or similar environment variable and seperating each individual path entry
//   for storage in a string array as a member of the class
//
// FILE:
//   getpath.h
//
#ifndef PATHTOKEN_H
#define PATHTOKEN_H

#ifndef STRTOKEN_H
  #include "strtoken.h"
#endif

#define PATHTOKEN_DEBUG   0

/****************************************************************************/
class PathTokenizer
{
  protected:
    size_t _Max;
    size_t _Size;
    char** _TokenArray;
    const char* _TokenSeparator;
    const char* _DirSeparator;

    void Tokenize(char* Str_);
    
  public:
    enum
    {
      DOS_PATH,
      UNIX_PATH
    };

  public:
    PathTokenizer();
    PathTokenizer(const char* VarName_, int SysType_);
    PathTokenizer(const char* VarName_, const char* TokSep_, const char* DirSep_);
    virtual ~PathTokenizer();

    void SetEnvVar(const char* VarName_, int SysType_);
    void SetEnvVar(const char* VarName_, const char* TokSep_, const char* DirSep_);

    size_t Max() const;
    const char* TokenSeparator() const;
    const char* DirSeparator() const;
    const char* operator [] (size_t Index_) const;
    void Dump(ostream& Out_) const;

    // kludge to make preprocessor macro work
    static void CallMethod(PathTokenizer& Obj_, const char* MethodName_, const char* VarName_, int SysType_);
};

#if (defined(__linux__) | defined(__unix__)) & !defined(__DJGPP__)
  #define PATHTOKENIZER(VarName, PathVar)    \
    PathTokenizer VarName;                   \
    const char* PathVarConst_ = PathVar;     \
    PathTokenizer::CallMethod(VarName, "SetEnvVar(const char*, int)", PathVarConst_, PathTokenizer::UNIX_PATH)
#else
  #define PATHTOKENIZER(VarName, PathVar)    \
    PathTokenizer VarName;                   \
    const char* PathVarConst_ = PathVar;     \
    PathTokenizer::CallMethod(VarName, "SetEnvVar(const char*, int)", PathVarConst_, PathTokenizer::DOS_PATH)
#endif

/****************************************************************************/
#endif




