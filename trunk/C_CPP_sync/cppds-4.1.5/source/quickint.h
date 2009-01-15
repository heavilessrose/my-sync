#ifndef QUICKINT_H
#define QUICKINT_H

#ifndef INCL_STDIO_H
  #include <stdio.h>
  #define INCL_STDIO_H
#endif
#ifndef INCL_FSTREAM_H
  #include <fstream.h>
  #define INCL_FSTREAM_H
#endif
#ifndef INCL_IOMANIP_H
  #include <iomanip.h>
  #define INCL_IOMANIP_H
#endif
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif

#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef SAFESTR_H
  #include "safestr.h"
#endif
#ifndef BITVECTOR_H
  #include "bitvect.h"
#endif
#ifndef STRUTIL_H
  #include "strutil.h"
#endif
#ifndef LONGNUM_H
  #include "longnum.h"
#endif

#define QUICKINT_DEBUG                 0
#define QUICKINT_NICE_OUTPUT           0
#define QUICKINT_QUICKPARSE_DEBUG1     0
#define QUICKINT_QUICKPARSE_DEBUG2     0
#define QUICKINT_QUICKPARSE_DEBUG3     0

class TokenInfo
{
  protected:
    char* _Token;
    int _Wspace;
    int _RetCode;
    int _SkipSpaces;
    int _Required;
    int _CaseSensitive;

  public:
    enum
    {
      NONE = 0,
      PRE = 1,
      POST = 2
    };
  
    TokenInfo();
    TokenInfo(char* NewToken_, int RetCode_, int SkipSpaces_, int Required_, int Sensitive_);
    ~TokenInfo();

    char* GetToken()
        { return _Token; }
    int GetWspace() const
        { return _Wspace; }
    int GetRetCode() const
        { return _RetCode; }
    int GetSkipSpaces() const
        { return _SkipSpaces; }
    int IsRequired() const
        { return _Required; }
    int IsCaseSensitive() const
        { return _CaseSensitive; }

    TokenInfo& SetToken(char* NewToken_);
    TokenInfo& SetWspace(int Value_);
    TokenInfo& SetRetCode(int RetCode_);
    TokenInfo& SetSkipSpaces(int Skip_);
    TokenInfo& SetRequired(int Req_);
    TokenInfo& SetCaseSensitive(int Sensitive_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

class FieldInfo
{
  protected:
    char* _Field;
    int _Wspace;
    char* _StartDelim;
    char* _EndDelim;
    int _Code;
    int _Required;
    int _SkipWs;
    int _Transfer;

  public:
    enum
    {
      NONE = 0,
      PRE = 1,
      POST = 2
    };
    
    FieldInfo();
    FieldInfo(char* Start_, char* End_, int Code_, int Required_, int SkipWs_);
    ~FieldInfo();

    FieldInfo& SetField(char* Field_);
    FieldInfo& SetWspace(int Value_);
    FieldInfo& SetStartDelim(char* Start_);
    FieldInfo& SetEndDelim(char* End_);
    FieldInfo& SetCode(int Code_);
    FieldInfo& SetRequired(int Req_);
    FieldInfo& SetSkipWhiteSpace(int Skipws_);

    char* GetField(Boolean Transfer_);
    int GetWspace() const
        { return _Wspace; }
    char* GetStartDelim() const
        { return _StartDelim; }
    char* GetEndDelim() const
        { return _EndDelim; }
    int GetCode() const
        { return _Code; }
    int IsRequired() const
        { return _Required; }
    int SkipWhiteSpace() const
        { return _SkipWs; }

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

// BEGIN: classes for BNF quick parser
struct ParseSymbolType
{
  ParseSymbolType():
    Symbol(NULL),
    Definition(NULL) {}
  ~ParseSymbolType();

  char* Symbol;
  char* Definition;

  void DefineSymbol(char* Symbol_, char* Def_);


#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

class ParseResultInfo;

struct ParseTokenType
{
  enum
  {
    NO_TYPE,
    INT_TYPE,
    STRING_TYPE,
    FLOAT_TYPE
  };

  int TokenType;
  int DataType;
  int IntValue;
  double FloatValue;
  LongNumber Validity;
  char* StrValue;
  char* Token;
  ParseResultInfo* Next;

  ParseTokenType();
  ~ParseTokenType();

  ParseTokenType& SetTokenType(int Val_);
  ParseTokenType& SetToken(char* Str_);

  ParseTokenType& SetValidity(Ulong Val_, int Sign_);
  ParseTokenType& SetValidity(LongNumber Val_);
  ParseTokenType& SetIntValue(int Val_);
  ParseTokenType& SetFloatValue(double Val_);
  ParseTokenType& SetStrValue(char* Val_);
  int HasToken() const;  
  ostream& ShowData(ostream& os) const;
  const char* GiveType() const;
  const char* GiveToken() const;
  
  ParseResultInfo* SetNext();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

class ParseResultInfo
{
  protected:
    int _Current;
    int _Index;
    int _Size;
    int _Branches;

    void Clear();
    void IncrementTokenCount();
    
  public:
    ParseTokenType** Tokens;
    ParseResultInfo* OrBranch;

    ParseResultInfo();
    ~ParseResultInfo();

    void Reset();
    void Grow();
    int Branches() const;
    int Size() const;
    int Index() const;
    int HasTokens() const;

    void InsertToken(char* Str_);
    void SetNumBranches(int Branches_);
    ParseTokenType* GetCurrent();
    ParseTokenType* GetNext();
    ParseResultInfo* SetBranch();

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

struct QuickParseStatus
{
  // According to bit number in
  // ParserFlags bit vector
  enum
  {
    CASE_SENSITIVE = 1
  };

  QuickParseStatus();

  int CalcNestingLevel();
  void Assign(QuickParseStatus* Status_);
  void ResetParseFlags();
  int ResetStatusFlag();
  void Reset();
  void SetError();

  void SetInLiteral();
  void SetInSymbol();
  void SetInGroup();
  void SetInOption();

  void SetParseLiteral();
  void SetParseSymbol();
  void SetParseGroup();
  void SetParseOption();
  void SetParseRepOption();
  void SetInParserOption();
  void SetCaseSensitive(int BoolVal_);

  int ParseError;
  int NestingLevel;
  int CurrentToken;
  int GroupLevel;
  int ParseGroup;
  BitVector InGroup;
  int OptionLevel;
  int ParseOption;
  BitVector InOption;
  int ParseSymbol;
  BitVector InSymbol;
  int ParseLiteral;
  BitVector InLiteral;
  int InParserOption;
  int Repeatable;
  int TokenType;
  int TerminatorFound;
  int SemiTerminatorFound;
  int StackDisplayLevel;
  int DeepestLevel;
  BitVector ParserFlags;

  char* OldSyntaxStr;
  char* NewSyntaxStr;
  char* NewInputStr;

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif
};

class Parser
{
  private:
    static char* TrimSpaces(char* str);
    static char* StripLeadingSpaces(char* str);
    static int StrniComp(const char* s1, const char* s2, size_t max);
    static int StriComp(const char* s1, const char* s2);    
  
  protected:
    enum
    {
      MAX_PARSE_SYMBOLS = 256
    };
  
    int _ParserNumber;
    static Parser* _ParserObjects[256];
    
    int _NumSymbols;
    int _MaxParseSymbols;
    ParseSymbolType** _ParseSymbols;

    char* _ReadLine;
    char* _WorkLine;
    int _TerminatorFound;
    int _SemiTerminatorCnt;
    int _ParseError;
    int _LineNumber;

    char* _StartQuote;
    char* _EndQuote;
    char* _Assignment;
    char* _Terminator;
    char* _SemiTerminator;

    static char* HasChar(const char* Str_, const char* Filter_);
    static char* ReplaceChar(char* Str_, const char* Trg_, const char* Rep_);
    static int MaxOf(int Value1_, int Value2_);

    // quick parser methods
    void GrowSymbolTable();
    char* GroupToken(char* Token_, char* StartBracket_, char* EndBracket_);
    static int DetermineTokenType(const char* Char_);
    static char* FindNextBranch(char* Str_);
    static int FindNumBranches(char* Str_);
    static int FindOrBranch(char* Str_, QuickParseStatus* Status_);
    static char* FindGrouping(char*& Str_, const char* Startb_, const char* Endb_);
    static char* StripGrouping(char* Str_, const char* Startb_, const char* Endb_);
    static ParseResultInfo* ResolveGroup(ParseResultInfo* Results_, int Branches_);
    static ParseResultInfo* ResolveOption(ParseResultInfo* Results_);
    static ParseResultInfo* ResolveFloat(ParseResultInfo* DefnInfo_, ParseResultInfo* TokenInfo_);
    void SetSyntaxSymbols(char* Assignment_, char* Terminator_, char* SemiTerminator_);
    char* ExtractStrTokenFnc(char*& Text_, char* SyntaxStr_, int Quoted_, char* SymbolType_);
    char* ExtractStrToken(char*& Text_, char* SyntaxStr_, int Quoted_, char* SymbolType_);
    int IsValidIntField(char* Field_, int& IntValue_);
    int IsValidStrField(char* Field_, int Quoted_, char* Type_);
    int NotInQuotes(char* Text_, char* Token_, int Whitespace_);
    void ShowParseState(QuickParseStatus* Status_, char* Token_, int AtStart_, int AtRoot_, ParseResultInfo* Results_);
    int CheckParseError(ParseResultInfo* Results_, char* Text_, char* SyntaxStr_, QuickParseStatus* Status_);
    int GrabToken(ParseResultInfo* Results_, char* Text_, char* SyntaxStr_,
                  QuickParseStatus* Status_, char* Token_, int PrevValid_);
    ParseResultInfo* QuickParse(ParseResultInfo* Results_, char* Text_, char* SyntaxStr_,
                                QuickParseStatus* Status_, char* Token_, int PrevValid_);

    void ResetVariables();

    static long ParserSupply(int Create_, long Index_, Parser* Ptr_);
    void DeleteParserObject(long MaxParsers_);

    Parser(long ParserNumber_=0);
    ~Parser();

  public:
    enum
    {
      NO_TOKEN,
      LITERAL,
      SYMBOL,
      OPTION,
      GROUP,
      OR_BRANCH,
      REPEATABLE,
      PARSER_OPTION
    };

    void ShowLineData();

    int ParseError() const
        { return _ParseError; }
    int LineNumber() const
        { return _LineNumber; }

    void DefineSymbol(char* Symbol_, char* Definition_);
    char* FindDefinition(char* Token_);

    ParseResultInfo* QuickParse_Main(ParseResultInfo* Results_, QuickParseStatus* Status_, char* TagName_, char* Text_, char* SyntaxStr_);
    void ShowResults(ParseResultInfo* Results_, int Level_);
    char* GetResultCodeString(ParseResultInfo* Results_, char* Buffer_, int Level_, int SpecLevel_);

    void UseBrackets(Boolean Flag_);    
    void SetStringQuotes(const char* StartQuote_, const char* EndQuote_);
    void SetError(int Code_, int LineNum_=0);

    void ReadDefinitionFile(const char* FileName_);
    void CopyParseSymbols(const Parser& Obj_);

    // Token parsing methods
    static int FindTokensHelper(char* str_, TokenInfo* Info_, size_t ArrayLen_, size_t CmpLen_, size_t Index_);
    static int FindTokens(char*& str_, TokenInfo* Info_, size_t ArrayLen_, size_t CmpLen_, int& Nil_);
    
    static int FindToken(char*& str, char* Token_, int RetCode_, int SkipSpaces_, int CaseSensitive_, int Consume_, int TokenWspace_=0);
    static int RepeatFindTokenHelper(char*& str, TokenInfo* Info_, size_t Index_, size_t Len_, int Code_);
    static int RepeatFindToken(char*& str, TokenInfo* Info_, size_t Len_);

    static char* FindWord(char*& str, int& RetVal_, int Code_);                                    // new
    static char* FindField(char*& str, const char* StartDelim_, const char* EndDelim_, int& RetVal_, int Code_, int SkipWs_, int TokenWspace_=0);  // changed
    static int RepeatFindFieldHelper(char*& str, FieldInfo* Array_, size_t Index_, size_t Len_, int Code_);
    static int RepeatFindField(char*& str, FieldInfo* Array_, size_t Len_);

#if OVERLOAD_NEW
    void* operator new (size_t Bytes_);
    void operator delete (void* Space_);

#if	HAS_ARRAY_NEW
    void* operator new[] (size_t Bytes_);
    void operator delete[] (void* Space_);
#endif
#endif

  public:
    // For external C interface
    static long InitParser();
    static long MakeParser();
    static void DestroyParser(long Index_, Parser* Ptr_);
    static Parser* IndexParser(long Index_);
    static Parser* MakeParserPtr()
        { return Parser::IndexParser(MakeParser()); }    
};

// need C linkage to interface to non-C++ languages
#ifdef __cplusplus
extern "C" {
#endif

long quickint_Initialize();
long quickint_GetParser();
void quickint_EraseParser(long Index_);
void quickint_ShowLineData(long Index_);
int quickint_ParseError(long Index_);
int quickint_LineNumber(long Index_);
void quickint_UseBrackets(long Index_, Boolean Flag_);
void quickint_DefineSymbol(long Index_, char* Symbol_, char* Definition_);
char* quickint_FindDefinition(long Index_, char* Token_);

#ifdef __cplusplus
}
#endif

#endif




