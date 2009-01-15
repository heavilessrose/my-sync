#ifndef QUICKINT_H
  #include "quickint.h"
#endif

Parser* Parser::_ParserObjects[];

/****************************************************************************/
FieldInfo::FieldInfo():
_Field(NULL),
_Wspace(0),
_StartDelim(NULL),
_EndDelim(NULL),
_Code(0),
_Required(TRUE),
_SkipWs(TRUE),
_Transfer(FALSE)
{}

/****************************************************************************/
FieldInfo::FieldInfo(char* Start_, char* End_, int Code_, int Required_, int SkipWs_):
_Field(NULL),
_Wspace(0),
_StartDelim(Start_ ? new_char_string(Start_):NULL),
_EndDelim(End_ ? new_char_string(End_):NULL),
_Code(Code_),
_Required(Required_),
_SkipWs(SkipWs_),
_Transfer(FALSE)
{}

/****************************************************************************/
FieldInfo::~FieldInfo()
{
  ::DeleteArray(_StartDelim);
  ::DeleteArray(_EndDelim);

  _StartDelim =
  _EndDelim = NULL;

  if (!_Transfer)
  {
    ::DeleteArray(_Field);
    _Field = NULL;
  }
}

/****************************************************************************/
FieldInfo& FieldInfo::SetField(char* Field_)
{
  if (Field_ && (Field_ != _Field))
  {
    if (!_Transfer)
      ::DeleteArray(_Field);
      
    _Field = Field_ ? new_char_string(Field_):NULL;
  }

  return *this;
}

/****************************************************************************/
FieldInfo& FieldInfo::SetWspace(int Value_)
{
  _Wspace = Value_;
  return *this;
}

/****************************************************************************/
FieldInfo& FieldInfo::SetStartDelim(char* Start_)
{
  if (Start_ != _StartDelim)
  {
    ::DeleteArray(_StartDelim);
    _StartDelim = Start_ ? new_char_string(Start_):NULL;
  }

  return *this;
}

/****************************************************************************/
FieldInfo& FieldInfo::SetEndDelim(char* End_)
{
  if (End_ != _EndDelim)
  {
    ::DeleteArray(_EndDelim);
    _EndDelim = End_ ? new_char_string(End_):NULL;
  }

  return *this;
}

/****************************************************************************/
FieldInfo& FieldInfo::SetCode(int Code_)
{
  _Code = Code_;
  return *this;
}

/****************************************************************************/
FieldInfo& FieldInfo::SetRequired(int Req_)
{
  _Required = Req_;
  return *this;
}

/****************************************************************************/
FieldInfo& FieldInfo::SetSkipWhiteSpace(int Skipws_)
{
  _SkipWs = Skipws_;
  return *this;
}

/****************************************************************************/
char* FieldInfo::GetField(Boolean Transfer_)
{
  _Transfer = Transfer_;
  return _Field;
}

/****************************************************************************/
/****************************************************************************/
TokenInfo::TokenInfo():
_Wspace(0),
_Token(NULL),
_RetCode(0),
_SkipSpaces(TRUE),
_Required(TRUE),
_CaseSensitive(TRUE)
{}

/****************************************************************************/
TokenInfo::TokenInfo(char* NewToken_, int RetCode_, int SkipSpaces_, int Required_, int Sensitive_):
_Wspace(0),
_Token(NewToken_ ? new_char_string(NewToken_):NULL),
_RetCode(RetCode_),
_SkipSpaces(SkipSpaces_),
_Required(Required_),
_CaseSensitive(Sensitive_)
{}

/****************************************************************************/
TokenInfo::~TokenInfo()
{
  ::DeleteArray(_Token);
  _Token = NULL;
}

/****************************************************************************/
TokenInfo& TokenInfo::SetToken(char* NewToken_)
{
  if (NewToken_ != _Token)
  {
    ::DeleteArray(_Token);
    _Token = NewToken_ ? new_char_string(NewToken_):NULL;
  }

  return *this;
}

/****************************************************************************/
TokenInfo& TokenInfo::SetWspace(int Value_)
{
  _Wspace = Value_;
  return *this;
}

/****************************************************************************/
TokenInfo& TokenInfo::SetRetCode(int RetCode_)
{
  _RetCode = RetCode_;
  return *this;
}

/****************************************************************************/
TokenInfo& TokenInfo::SetSkipSpaces(int Skip_)
{
  _SkipSpaces = Skip_;
  return *this;
}

/****************************************************************************/
TokenInfo& TokenInfo::SetRequired(int Req_)
{
  _Required = Req_;
  return *this;
}

/****************************************************************************/
TokenInfo& TokenInfo::SetCaseSensitive(int Sensitive_)
{
  _CaseSensitive = Sensitive_;
  return *this;
}

/****************************************************************************/
/****************************************************************************/
ParseSymbolType::~ParseSymbolType()
{
  ::DeleteArray(Symbol);
  ::DeleteArray(Definition);
}

/****************************************************************************/
void ParseSymbolType::DefineSymbol(char* Symbol_, char* Def_)
{
  Symbol = new_char_string(Symbol_);
  Definition = new_char_string(Def_);
}

/****************************************************************************/
/****************************************************************************/
ParseTokenType::ParseTokenType():
TokenType(0),
DataType(0),
IntValue(0),
FloatValue(0),
Validity(0, NumberSign::PLUS),
StrValue(NULL),
Token(NULL),
Next(NULL)
{}

/****************************************************************************/
ParseTokenType::~ParseTokenType()
{
  ::DeleteArray(StrValue);
  ::DeleteArray(Token);
  delete Next;
}

/****************************************************************************/  
ParseTokenType& ParseTokenType::SetToken(char* Str_)
{
  Token = new_char_string(Str_);
  return SetValidity(SafeStrLen(Str_) != 0, NumberSign::PLUS);
}

/****************************************************************************/  
ParseTokenType& ParseTokenType::SetTokenType(int Val_)
{
  TokenType = Val_;
  return *this;
}

/****************************************************************************/  
ParseTokenType& ParseTokenType::SetValidity(Ulong Val_, int Sign_)
{
  LongNumber NewVal_(Val_, Sign_);
  Validity = NewVal_;
  if (!Validity)
    DataType = NO_TYPE;
  
  return *this;  
}

/****************************************************************************/  
ParseTokenType& ParseTokenType::SetValidity(LongNumber Val_)
{
  Validity = Val_;
  if (!Validity)
    DataType = NO_TYPE;
  
  return *this;
}

/****************************************************************************/  
ParseTokenType& ParseTokenType::SetIntValue(int Val_)
{
  Validity.SetValue(1UL, NumberSign::PLUS);
  DataType = INT_TYPE;
  IntValue = Val_;  
  return *this;
}

/****************************************************************************/  
ParseTokenType& ParseTokenType::SetFloatValue(double Val_)
{
  Validity.SetValue(1UL, NumberSign::PLUS);
  DataType = FLOAT_TYPE;
  FloatValue = Val_;
  return *this;
}

/****************************************************************************/  
ParseTokenType& ParseTokenType::SetStrValue(char* Val_)
{
  Validity.SetValue(1UL, NumberSign::PLUS);
  DataType = STRING_TYPE;
  StrValue = Val_;
  return *this;
}

/****************************************************************************/  
ParseResultInfo* ParseTokenType::SetNext()
{
  if (!Next)
    Next = new ParseResultInfo();
    
  return Next;
}

/****************************************************************************/
int ParseTokenType::HasToken() const
{
  return (Token != ((char*)NULL));
}

/****************************************************************************/
const char* ParseTokenType::GiveType() const
{
  if (TokenType == Parser::LITERAL)
    return "LITERAL";
  else if (TokenType == Parser::SYMBOL)
    return "SYMBOL";
  else if (TokenType == Parser::OPTION)
    return "OPTION";
  else if (TokenType == Parser::GROUP)
    return "GROUP";
  else if (TokenType == Parser::PARSER_OPTION)
    return "PARSER_OPTION";

  return "NO_TOKEN";
}

/****************************************************************************/
const char* ParseTokenType::GiveToken() const
{
  return Token;
}

/****************************************************************************/
ostream& ParseTokenType::ShowData(ostream& os) const
{
  if (DataType == INT_TYPE)
    os <<IntValue;
  else if (DataType == STRING_TYPE)
    os <<StrValue;
  else if (DataType == FLOAT_TYPE)
    os <<FloatValue;

  return os;
}

/****************************************************************************/
/****************************************************************************/
ParseResultInfo::ParseResultInfo():
_Index(0),
_Current(0),
_Size(10),
Tokens((ParseTokenType**)RawAllocateWith(MEMMATRIX, sizeof(ParseTokenType*) * 10)),
OrBranch(NULL)
{
  int i;
  for (i = 0; i < _Size; i++)
    Tokens[i] = new ParseTokenType();
}

/****************************************************************************/
void ParseResultInfo::Clear()
{
  int i;
  for (i = 0; i < _Size; i++)
    delete Tokens[i];
    
  ::DeleteArray(Tokens);
  delete OrBranch;
}

/****************************************************************************/
ParseResultInfo::~ParseResultInfo()
{
  Clear();
}

/****************************************************************************/
void ParseResultInfo::Reset()
{
  Clear();
  
  _Index = 0;
  _Current = 0;
  _Size = 10;
  Tokens = (ParseTokenType**)RawAllocateWith(MEMMATRIX, sizeof(ParseTokenType*) * 10);
  OrBranch = NULL;

  int i;
  for (i = 0; i < _Size; i++)
    Tokens[i] = new ParseTokenType();  
}

/****************************************************************************/
void ParseResultInfo::Grow()
{
  int i;
  int OldSize_ = _Size;
  ParseTokenType** OldPtr_ = Tokens;

  _Size += 10;
  Tokens = (ParseTokenType**)RawAllocateWith(MEMMATRIX, sizeof(ParseTokenType*) * _Size);

  for (i = 0; i < OldSize_; i++)
    Tokens[i] = OldPtr_[i];

  for (;i < _Size; i++)
    Tokens[i] = new ParseTokenType();

  ::DeleteArray(OldPtr_);
}

/****************************************************************************/
int ParseResultInfo::Branches() const
{
  return _Branches;
}

/****************************************************************************/
int ParseResultInfo::Size() const
{
  return _Size;
}

/****************************************************************************/
int ParseResultInfo::Index() const
{
  return _Index;
}

/****************************************************************************/
int ParseResultInfo::HasTokens() const
{
  return (Tokens[0] != ((ParseTokenType*)NULL));
}

/****************************************************************************/
void ParseResultInfo::IncrementTokenCount()
{
  _Current = _Index;
  _Index++;
}

/****************************************************************************/
void ParseResultInfo::InsertToken(char* Str_)
{
  if (_Index >= _Size)
    Grow();

  Tokens[_Index]->SetToken(Str_);
  IncrementTokenCount();
}

/****************************************************************************/    
ParseTokenType* ParseResultInfo::GetCurrent()
{  
  return Tokens[_Current];
}

/****************************************************************************/    
ParseTokenType* ParseResultInfo::GetNext()
{
  if (_Index >= _Size)
    Grow();

  return Tokens[_Index];
}

/****************************************************************************/
void ParseResultInfo::SetNumBranches(int Branches_)
{
  _Branches = Branches_;
}

/****************************************************************************/
ParseResultInfo* ParseResultInfo::SetBranch()
{
  if (!OrBranch)
    OrBranch = new ParseResultInfo();

  return OrBranch;
}

/****************************************************************************/
/****************************************************************************/
QuickParseStatus::QuickParseStatus():
ParseError(0),
NestingLevel(0),
CurrentToken(0),
GroupLevel(0),
ParseGroup(0),
InGroup(32),
OptionLevel(0),
ParseOption(0),
InOption(32),
ParseSymbol(0),
InSymbol(32),
ParseLiteral(0),
InLiteral(32),
InParserOption(0),
Repeatable(0),
TokenType(0),
TerminatorFound(0),
SemiTerminatorFound(0),
StackDisplayLevel(0),
DeepestLevel(0),
ParserFlags(32),
OldSyntaxStr(NULL),
NewSyntaxStr(NULL),
NewInputStr(NULL)
{
  ParserFlags.SetBit(CASE_SENSITIVE);
}

/****************************************************************************/
int QuickParseStatus::CalcNestingLevel()
{
  return (StackDisplayLevel + GroupLevel + OptionLevel);
}

/****************************************************************************/
void QuickParseStatus::Assign(QuickParseStatus* Status_)
{
   ParseError = Status_->ParseError;
   NestingLevel = Status_->NestingLevel;
   CurrentToken = Status_->CurrentToken;
   GroupLevel = Status_->GroupLevel;
   ParseGroup = Status_->ParseGroup;
   InGroup = Status_->InGroup;
   OptionLevel = Status_->OptionLevel;
   ParseOption = Status_->ParseOption;
   InOption = Status_->InOption;
   ParseSymbol = Status_->ParseSymbol;
   InSymbol = Status_->InSymbol;
   ParseLiteral = Status_->ParseLiteral;
   InLiteral = Status_->InLiteral;
   InParserOption = Status_->InParserOption;
   Repeatable = Status_->Repeatable;
   TokenType = Status_->TokenType;
   TerminatorFound = Status_->TerminatorFound;
   SemiTerminatorFound = Status_->SemiTerminatorFound;
   StackDisplayLevel = Status_->StackDisplayLevel;
   DeepestLevel = Status_->DeepestLevel;
   ParserFlags = Status_->ParserFlags;
   OldSyntaxStr = Status_->OldSyntaxStr;
   NewSyntaxStr = Status_->NewSyntaxStr;
   NewInputStr = Status_->NewInputStr;
}

/****************************************************************************/
void QuickParseStatus::ResetParseFlags()
{
   ParseError = 0;
   ParseLiteral = 0;
   ParseSymbol = 0;
   ParseGroup = 0;
   ParseOption = 0;
   Repeatable = 0;
   InParserOption = 0;
}

/****************************************************************************/
int QuickParseStatus::ResetStatusFlag()
{
  if (!NestingLevel || ParseError || InParserOption)
  {
    ResetParseFlags();
    return CurrentToken;
  }

  NestingLevel--;
  Ulong Val_;
  BitVector Mask_;

  switch (CurrentToken)
  {
    case Parser::LITERAL:
      Val_ = ~(1UL << Ulong(NestingLevel));
      Mask_.FromIntValue(Val_);
      InLiteral &= Mask_;
      break;
    
    case Parser::SYMBOL:
      Val_ = ~(1UL << Ulong(NestingLevel));
      Mask_.FromIntValue(Val_);      
      InSymbol &= Mask_;
      break;
    
    case Parser::GROUP:
      Val_ = ~(1UL << Ulong(NestingLevel));
      Mask_.FromIntValue(Val_);      
      InGroup &= Mask_;
      break;
    
    case Parser::OPTION:
      Val_ = ~(1UL << Ulong(NestingLevel));
      Mask_.FromIntValue(Val_);      
      InOption &= Mask_;
      break;
  }

  Ulong Index_ = NestingLevel;
  if (Index_)
    --Index_;

  CurrentToken = (InLiteral.GetBit(Index_)) ? Parser::LITERAL:
                 (InSymbol.GetBit(Index_)) ? Parser::SYMBOL:
                 (InGroup.GetBit(Index_)) ? Parser::GROUP:
                 (InOption.GetBit(Index_)) ? Parser::OPTION:0;
                 
  ResetParseFlags();
  return CurrentToken;
}

/****************************************************************************/
void QuickParseStatus::SetError()
{
  ResetParseFlags();
  ParseError = 1;
}

/****************************************************************************/
void QuickParseStatus::SetInParserOption()
{
  ResetParseFlags();
  InParserOption = 1;
}

/****************************************************************************/
void QuickParseStatus::SetParseLiteral()
{
  ResetParseFlags();
  ParseLiteral = 1;
}

/****************************************************************************/
void QuickParseStatus::SetParseSymbol()
{
  ResetParseFlags();
  ParseSymbol = 1;
}

/****************************************************************************/
void QuickParseStatus::SetParseGroup()
{
  ResetParseFlags();
  ParseGroup = 1;
}

/****************************************************************************/
void QuickParseStatus::SetParseOption()
{
  ResetParseFlags();
  ParseOption = 1;
}

/****************************************************************************/
void QuickParseStatus::SetParseRepOption()
{
  ResetParseFlags();
  ParseOption = 1;
  Repeatable = 1;
}

/****************************************************************************/
void QuickParseStatus::SetInLiteral()
{
  if (!ParseError)
  {
    Ulong Val_ = (1UL << Ulong(NestingLevel));
    BitVector Mask_(0, NULL, Val_);
    InLiteral |= Mask_;
    NestingLevel++;
    CurrentToken = Parser::LITERAL;
  }
}

/****************************************************************************/
void QuickParseStatus::SetInSymbol()
{
  if (!ParseError)
  {
    Ulong Val_ = (1UL << Ulong(NestingLevel));
    BitVector Mask_(0, NULL, Val_);    
    InSymbol |= Mask_;
    NestingLevel++;
    CurrentToken = Parser::SYMBOL;
  }
}

/****************************************************************************/
void QuickParseStatus::SetInGroup()
{
  if (!ParseError)
  {
    Ulong Val_ = (1UL << Ulong(NestingLevel));
    BitVector Mask_(0, NULL, Val_);    
    InGroup |= Mask_;
    NestingLevel++;
    CurrentToken = Parser::GROUP;
  }
}

/****************************************************************************/
void QuickParseStatus::SetInOption()
{
  if (!ParseError)
  {
    Ulong Val_ = (1UL << Ulong(NestingLevel));
    BitVector Mask_(0, NULL, Val_);
    InOption |= Mask_;
    NestingLevel++;
    CurrentToken = Parser::OPTION;
  }
}

/****************************************************************************/
void QuickParseStatus::SetCaseSensitive(int BoolVal_)
{
  if (BoolVal_)
    ParserFlags.SetBit(CASE_SENSITIVE);
  else  
    ParserFlags.UnSetBit(CASE_SENSITIVE);
}

/****************************************************************************/
void QuickParseStatus::Reset()
{
    ParseError = 0;
    NestingLevel = 0;
    CurrentToken = 0;
    GroupLevel = 0;
    ParseGroup = 0;
    InGroup.UnSetAllBits();
    OptionLevel = 0;
    ParseOption = 0;
    InOption.UnSetAllBits();
    ParseSymbol = 0;
    InSymbol.UnSetAllBits();
    ParseLiteral = 0;
    InLiteral.UnSetAllBits();
    InParserOption = 0;
    Repeatable = 0;
    TokenType = 0;
    TerminatorFound = 0;
    SemiTerminatorFound = 0;
    StackDisplayLevel = 0;
    DeepestLevel = 0;
    ParserFlags.SetBit(CASE_SENSITIVE);
    OldSyntaxStr = NULL;
    NewSyntaxStr = NULL;
    NewInputStr = NULL;
}

/****************************************************************************/
/****************************************************************************/
int Parser::FindTokensHelper(char* str_, TokenInfo* Info_, size_t ArrayLen_, size_t CmpLen_, size_t Index_)
{
  if (Index_ < ArrayLen_)
  {
    int NotMatch_ = 0;

    if (SafeStrLen(Info_[Index_].GetToken()) >= CmpLen_ && SafeStrLen(str_) >= CmpLen_)
    {
      if (Info_[Index_].IsCaseSensitive())
        NotMatch_ = strncmp(str_, Info_[Index_].GetToken(), CmpLen_);
      else
        NotMatch_ = StrniComp(str_, Info_[Index_].GetToken(), CmpLen_);
    }
    else
      NotMatch_ = TRUE;
      
    if (NotMatch_)
      return FindTokensHelper(str_, Info_, ArrayLen_, CmpLen_, Index_ + 1);
    else
      return (Index_ + 1);            
  }

  return 0;
}

/****************************************************************************/
int Parser::FindTokens(char*& str_, TokenInfo* Info_, size_t ArrayLen_, size_t CmpLen_, int& Nil_)
{
  size_t Code_ = 0;
  int Found_ = FindTokensHelper(str_, Info_, ArrayLen_, CmpLen_, 0);

  if (!Found_)
    Nil_ = 1;
  else
  {
    int i = Found_ - 1;
    Found_ = SafeStrLen(Info_[i].GetToken()) == CmpLen_;
    
    if (!Found_)
      Code_ = 0;
    else
    {
      Code_ = Info_[i].GetRetCode();
      str_ += CmpLen_;
    }
  }

  return Code_;
}

/****************************************************************************/
int Parser::FindToken(char*& str, char* Token_, int RetCode_, int SkipSpaces_, int CaseSensitive_, int Consume_, int TokenWspace_)
{
  if ((TokenWspace_ & 1) && (!str || !isspace(*str)))
    return 0;

  char* OldStr_ = str;
  int Match_ = 0;
  size_t TokLen_ = SafeStrLen(Token_);    

  if (SkipSpaces_)
    str = StripLeadingSpaces(str);

  size_t Len_ = SafeStrLen(str);

  if (str && SafeStrLen(str))
  {
    if (CaseSensitive_)
      Match_ = Len_ >= TokLen_ && !strncmp(str, Token_, TokLen_);
    else
      Match_ = Len_ >= TokLen_ && !StrniComp(str, Token_, TokLen_);
  }

  if (Match_)
  {
    if ((TokenWspace_ & 2) && (!str || !isspace(str[TokLen_])))
      Match_ = 0;
  
    if (Match_ && Consume_)
      str += TokLen_;
  }

  if (!Match_)
  {
    RetCode_ = 0;
    str = OldStr_;
  }

  return RetCode_;
}

/****************************************************************************/
int Parser::RepeatFindTokenHelper(char*& str, TokenInfo* Info_, size_t Index_, size_t Len_, int Code_)
{
  if (Index_ < Len_)
  {
    int RetVal_ = FindToken(str, Info_[Index_].GetToken(),
                            Info_[Index_].GetRetCode(),
                            Info_[Index_].GetSkipSpaces(),
                            Info_[Index_].IsCaseSensitive(), TRUE,
                            Info_[Index_].GetWspace());
    Code_ |= RetVal_;
    
    if (RetVal_ || !Info_[Index_].IsRequired())
      return RepeatFindTokenHelper(str, Info_, Index_ + 1, Len_, Code_);
  }

  return Code_;
}

/****************************************************************************/
int Parser::RepeatFindToken(char*& str, TokenInfo* Info_, size_t Len_)
{
  int Code_ = RepeatFindTokenHelper(str, Info_, 0, Len_, 0);
  return Code_;
}

/****************************************************************************/
char* Parser::FindWord(char*& str, int& RetVal_, int Code_)
{
  int Found_ = str && SafeStrLen(str);
  char* Field_ = NULL;
  char* OldStr_ = str;
  size_t Len_, NextPos_;

  str = StripLeadingSpaces(str);
  Len_ = SafeStrLen(str);
  for (NextPos_ = 0; NextPos_ < Len_ && !isspace(str[NextPos_]); NextPos_++);
  Found_ = NextPos_ != Len_;

  if (Found_)
  {
    Len_ = NextPos_;
    Field_ = Len_ ? strncpy(new_char_array(Len_ + 1, NULL), str, Len_):NULL;
    
    if (Field_)
      Field_[Len_] = 0;
       
    RetVal_ |= Code_;
    str = &str[NextPos_];
  }

  if (!Found_)
  {
    RetVal_ = 0;
    str = OldStr_;
  }

  return Field_;  
}

/****************************************************************************/
char* Parser::FindField(char*& str, const char* StartDelim_, const char* EndDelim_,
                        int& RetVal_, int Code_, int SkipWs_, int TokenWspace_)
{
  int Found_ = str && SafeStrLen(str);
  char* Field_ = NULL;
  char* OldStr_ = str;
  char Delim_[2];
  size_t NextPos_, Len_, Pos_, DelimLen_;
  Delim_[1] = 0;

  if ((TokenWspace_ & 1) && (!str || !isspace(*str)))
    Found_ = FALSE;

  if (SkipWs_ && Found_)
    str = StripLeadingSpaces(str);

  if (StartDelim_ && Found_)
  {
    if (!SafeStrLen(str))
      Found_ = FALSE;
    else
    {
      DelimLen_ = SafeStrLen(StartDelim_);
      Found_ = (SafeStrLen(str) >= DelimLen_) &&
               !strncmp(StartDelim_, str, DelimLen_);
      
      if (Found_)
        str += DelimLen_;
    }
  }
        
  if (Found_)
  {
    if (!EndDelim_)
    {    
      RetVal_ |= Code_;
      return Field_;
    }

    Len_ = SafeStrLen(str);    
    for (NextPos_ = 0; NextPos_ < Len_ && !strchr(EndDelim_, str[NextPos_]); NextPos_++);
    Found_ = NextPos_ != Len_;

    if (Found_)
    {
      DelimLen_ = SafeStrLen(EndDelim_);
      Found_ = (SafeStrLen(&str[NextPos_]) >= DelimLen_) &&
               !strncmp(EndDelim_, &str[NextPos_], DelimLen_);
               
      if (Found_)
        Pos_ = NextPos_ + DelimLen_;
    }

    if (Found_)
    {
       Len_ = NextPos_;
       Field_ = Len_ ? strncpy(new_char_array(Len_ + 1, NULL), str, Len_):SafeStrCpy(new_char(0), "");
       if (Field_)
         Field_[Len_] = 0;
       
       RetVal_ |= Code_;
       str = &str[Pos_];

       if ((TokenWspace_ & 2) && (!str || !isspace(*str)))
       {
         Found_ = FALSE;
         ::DeleteArray(Field_);
         Field_ = NULL;
       }
    }
  }

  if (!Found_)
  {
    RetVal_ = 0;
    str = OldStr_;
  }

  return Field_;
}

/****************************************************************************/
int Parser::RepeatFindFieldHelper(char*& str, FieldInfo* Array_, size_t Index_, size_t Len_, int Code_)
{
  if (Index_ < Len_)
  {
    int RetCode_ = Code_;
    Array_[Index_].SetField(
      FindField(str,
                Array_[Index_].GetStartDelim(),
                Array_[Index_].GetEndDelim(),
                RetCode_,
                (Array_[Index_].GetCode() | Code_),
                Array_[Index_].SkipWhiteSpace(),
                Array_[Index_].GetWspace()));

    if (RetCode_ || !Array_[Index_].IsRequired())
    {
      if (RetCode_)
        Code_ = RetCode_;
        
      return RepeatFindFieldHelper(str, Array_, Index_ + 1, Len_, Code_);
    }
    else
      Code_ = RetCode_;
  }

  return Code_;
}

/****************************************************************************/
int Parser::RepeatFindField(char*& str, FieldInfo* Array_, size_t Len_)
{
  int RetVal_ = RepeatFindFieldHelper(str, Array_, 0, Len_, 0);
  return RetVal_;
}

/****************************************************************************/
char* Parser::TrimSpaces(char* str)
{
  if (str)
  {
    str = StripLeadingSpaces(str);
  
    char* ptr = &str[SafeStrLen(str) - 1];
    while (ptr >= str && isspace(*ptr))
      *ptr-- = 0;
  }

  return str;  
}

/****************************************************************************/
char* Parser::StripLeadingSpaces(char* str)
{
  if (str)
    while (*str && isspace(*str))
      ++str;
  
  return str;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to compare two strings for equality. If both strings are
//   NULL then procedure returns 0, otherwise the strings are compared
//   character by character up to max characters same as strncmp, but
//   without case sensitivity.
//
// PRE:
//   char* s1 : the first string
//   char* s2 : the second string
//   int max : the maximum number of characters to compare in each string
//
// POST:
//   Return Value:
//   These routines return an int value based on the result of comparing
//   s1 (or part of it) to s2 (or part of it):
//   þ <  0  if s1 <  s2
//   þ == 0  if s1 == s2
//   þ >  0  if s1 >  s2
//
int Parser::StrniComp(const char* s1, const char* s2, size_t max)
{
  size_t index;
  int diff;

  if (!s1 || !s2)
    return (s1 ? 1:
	    s2 ? -1:0);

  for (index = 0, diff = 0;
       index < max &&
       !(diff = tolower(s1[index]) - tolower(s2[index])) &&
       s1[index] && s2[index];
       index++);

  return diff;
}

/****************************************************************************/
// PURPOSE:
//   Procedure to compare two strings for equality. If both strings are
//   NULL then procedure returns 0, otherwise the strings are compared
//   character by character until a NULL characters is found in either string
//   same as strcmp, but without case sensitivity.
//
// PRE:
//   char* s1 : the first string
//   char* s2 : the second string
//   int max : the maximum number of characters to compare in each string
//
// POST:
//   Return Value:
//   These routines return an int value based on the result of comparing
//   s1 (or part of it) to s2 (or part of it):
//   þ <  0  if s1 <  s2
//   þ == 0  if s1 == s2
//   þ >  0  if s1 >  s2
//
int Parser::StriComp(const char* s1, const char* s2)
{
  if (!s1 || !s2)
    return (s1 ? 1:
	    s2 ? -1:0);

  int diff;
  for (diff;
       !(diff = tolower(*s1) - tolower(*s2)) && *s1 && *s2;
       s1++, s2++);

  return diff;
}

/****************************************************************************/
int Parser::MaxOf(int Value1_, int Value2_)
{
  return ((Value1_ > Value2_) ? Value1_:Value2_);
}

/****************************************************************************/
Parser::Parser(long ParserNumber_):
_ParserNumber(ParserNumber_),
_NumSymbols(0),
_MaxParseSymbols(MAX_PARSE_SYMBOLS),
_ParseSymbols(NULL),
_ReadLine(NULL),
_WorkLine(NULL),
_TerminatorFound(0),
_SemiTerminatorCnt(0),
_ParseError(0),
_LineNumber(0),
_StartQuote(new_char_string("\"")),
_EndQuote(new_char_string("\"")),
_Assignment(new_char_string("=")),
_Terminator(new_char_string(":")),
_SemiTerminator(new_char_string(";"))
{
  long i, Max_;

  if (_ParserNumber && _ParserNumber < 256)
    _ParserObjects[_ParserNumber] = this;
  else
  {
    #if QUICKINT_DEBUG
      _Fout1 = new ofstream("interpret1.out", ios::out);
      _Fout2 = new ofstream("interpret2.out", ios::out);
    #endif
  
    for (i = 0; i < 256; i++)
      _ParserObjects[i] = NULL;
  }

  // Array limit for storage of user defined symbols
  Max_ = _MaxParseSymbols;
  
  _ParseSymbols = (ParseSymbolType**)RawAllocateWith(MEMMATRIX, sizeof(ParseSymbolType*) * Max_);
  _NumSymbols = 0;
  
  for (i = 0; i < Max_; i++)
    _ParseSymbols[i] = NULL;

  ResetVariables();
}

/****************************************************************************/
Parser::~Parser()
{
  ::DeleteArray(_StartQuote);
  ::DeleteArray(_EndQuote);
  ::DeleteArray(_Assignment);
  ::DeleteArray(_Terminator);
  ::DeleteArray(_SemiTerminator);

  long i;
  for (i = 0; i < _NumSymbols; i++)
    delete _ParseSymbols[i];
    
  ::DeleteArray(_ParseSymbols);

  if (!_ParserNumber)
  {
    for (i = 1; i < 256; i++)
      if (_ParserObjects[i])
      {
        delete _ParserObjects[i];
        _ParserObjects[i] = NULL;
      }
      else
        break;
  }
}

/****************************************************************************/
void Parser::DeleteParserObject(long MaxParsers_)
{
  if (_ParserNumber <= MaxParsers_)
  {
    long i = _ParserNumber;
    delete _ParserObjects[_ParserNumber];
    _ParserObjects[i] = NULL;

    for (;i < MaxParsers_; i++)
    {
      _ParserObjects[i] = _ParserObjects[i+1];
      _ParserObjects[i]->_ParserNumber--;
    }
    
    _ParserObjects[i] = NULL;
  }  
}

/****************************************************************************/
int Parser::DetermineTokenType(const char* Char_)
{
  if (*Char_ == '\"')
    return LITERAL;
  else if (*Char_ == '<')
    return SYMBOL;
  else if (*Char_ == '[')
    return OPTION;
  else if (*Char_ == '(')
    return GROUP;
  else if (*Char_ == '|')
    return OR_BRANCH;
  else if (*Char_ == '{')
  {
    if (Char_[1] == '#')
      return PARSER_OPTION;
    else
      return REPEATABLE;
  }

  return NO_TOKEN;
}

/****************************************************************************/
char* Parser::StripGrouping(char* Str_, const char* Startb_, const char* Endb_)
{
  int Len_ = SafeStrLen(Startb_);
  Str_ = StripLeadingSpaces(Str_);

  if (SafeStrLen(Str_) >= Len_ && strncmp(Str_, Startb_, Len_) == 0)  
  {
    memmove(Str_, Str_ + Len_, SafeStrLen(&Str_[Len_]) + 1);
    if (SafeStrLen(Str_) >= Len_ && strncmp(&Str_[SafeStrLen(Str_) - Len_], Endb_, Len_) == 0)
      Str_[SafeStrLen(Str_) - Len_] = 0;
  }

  return Str_;
}

/****************************************************************************/
char* Parser::FindGrouping(char*& Str_, const char* Startb_, const char* Endb_)
{
  char* StartPt_ = Str_;
  int Balance_ = 1;
  int InQuote_ = 0;
  int Len_ = SafeStrLen(Startb_);
  int x;

  for (++Str_; *Str_; Str_++)
  {
    if (InQuote_)
    {
      if (*Str_ == '\"')
        InQuote_ = 0;
    }
    else
    {    
      if (SafeStrLen(Str_) >= Len_ && strncmp(Str_, Startb_, Len_) == 0)
      {
        ++Balance_;
        for (x = 1; x < Len_ && *Str_; x++, Str_++);
      }

      if (SafeStrLen(Str_) >= Len_ && strncmp(Str_, Endb_, Len_) == 0)
      {
        --Balance_;
        for (x = 1; x < Len_ && *Str_; x++, Str_++);
      }

      if (*Str_ == '\"')
        InQuote_ = 1;
    }

    if (Balance_ == 0)
      break;
  }

  if (Balance_ != 0)
    return NULL;

  Len_ = (Str_ - StartPt_) + 1;
  char* Token_ = strncpy(new_char_array(Len_ + 1, NULL), StartPt_, Len_);
  Token_[Len_] = 0;
  ++Str_;
  
  return Token_;
}

/****************************************************************************/
char* Parser::FindNextBranch(char* Str_)
{
  int Balance_ = 0;
  int InQuote_ = 0;
  int FirstBranch_ = 0;

  while (*Str_)
  {
    if (InQuote_)
    {
      if (*Str_ == '\"')
        InQuote_ = 0;
    }
    else
    {
      if (*Str_ == '(' || *Str_ == '[')
        ++Balance_;

      if (*Str_ == ')' || *Str_ == ']')
        --Balance_;

      if (*Str_ == '\"')
        InQuote_ = 1;
    }

    if (!isspace(*Str_))
    {
      if (Balance_ == 0 && !InQuote_)
      {
        if (DetermineTokenType(Str_) == OR_BRANCH)
        {
          if (FirstBranch_)
          {
            ++Str_;
            return Str_;
          }
          else
            return NULL;
        }
        else
          FirstBranch_ = 1;
      }
      else
        FirstBranch_ = 1;
    }

    Str_++;
  }

  return NULL;
}

/****************************************************************************/
int Parser::FindNumBranches(char* Str_)
{
  char* OldStr_ = Str_;
  
  int SepCnt;
  int Balance_ = 0;
  int InQuote_ = 0;
  int FirstBranch_ = 0;

  for (SepCnt = 0; *Str_; Str_++)
  {
    if (InQuote_)
    {
      if (*Str_ == '\"')
        InQuote_ = 0;
    }
    else
    {
      if (*Str_ == '(' || *Str_ == '[')
        ++Balance_;

      if (*Str_ == ')' || *Str_ == ']')
        --Balance_;

      if (*Str_ == '\"')
        InQuote_ = 1;
    }

    if (!isspace(*Str_))
    {
      if (Balance_ == 0 && !InQuote_)
      {
        if (DetermineTokenType(Str_) == OR_BRANCH)
        {
          if (FirstBranch_)
            SepCnt++;
          else
            return 0;
        }
        else
          FirstBranch_ = 1;
      }
      else
        FirstBranch_ = 1;
    }
  }
  
  if (SafeStrLen(OldStr_))
    return (SepCnt + 1);

  return 0;  
}

/****************************************************************************/
void Parser::SetSyntaxSymbols(char* Assignment_, char* Terminator_, char* SemiTerminator_)
{
  _Assignment = new_char_string(Assignment_);
  _Terminator = new_char_string(Terminator_);
  _SemiTerminator = new_char_string(SemiTerminator_);
}

/****************************************************************************/
int Parser::FindOrBranch(char* Str_, QuickParseStatus* Status_)
{
  int Balance_ = 1;
  int InQuote_ = 0;

  for (++Str_; *Str_; Str_++)
  {
    if (InQuote_)
    {
      if (*Str_ == '\"')
        InQuote_ = 0;
    }
    else
    {
      if (*Str_ == '(' || *Str_ == '[')
        ++Balance_;

      if (*Str_ == ')' || *Str_ == ']')
        --Balance_;

      if (*Str_ == '\"')
        InQuote_ = 1;
    }

    if (Balance_ == 1 && !InQuote_)
    {
      if (DetermineTokenType(Str_) == OR_BRANCH)
      {
        Status_->NewSyntaxStr = Str_;
        return 1;
      }
    }

    if (Balance_ == 0)
      break;
  }

  return 0;
}

/****************************************************************************/
int Parser::CheckParseError(ParseResultInfo* Results_, char* Text_, char* SyntaxStr_, QuickParseStatus* Status_)
{
  int TokType_;

  if (Status_->NewSyntaxStr)
  {
    Status_->NewSyntaxStr = StripLeadingSpaces(Status_->NewSyntaxStr);  
    SyntaxStr_ = Status_->NewSyntaxStr;
  }
  else
    SyntaxStr_ = StripLeadingSpaces(SyntaxStr_);  

  Text_ = Status_->NewInputStr;

  // Or branch cannot occur within an option bracket so grouping bracket must be most recently encountered
  int BranchCond_ = (Status_->GroupLevel != 0) && (Status_->InGroup > Status_->InOption);
  int TermFound_ = (Status_->TerminatorFound || Status_->SemiTerminatorFound);
                
  if (SafeStrLen(SyntaxStr_))
  {
    TokType_ = DetermineTokenType(SyntaxStr_);
    Status_->TokenType = TokType_;
        
    if (TokType_ == OR_BRANCH)
    {
      if (!BranchCond_)
        return 0;
      else
      {
        if (Results_->OrBranch)
          return 1;
        else
          return (SafeStrLen(Text_) == 0 || TermFound_);
      }
    }
    else
    {    
      if (BranchCond_)
        if (!Results_->OrBranch)
        {      
          if (FindOrBranch(SyntaxStr_, Status_))
            Results_->SetBranch();
          else
            return 0;
        }
      
      return BranchCond_;
    }
  }
  else if (SafeStrLen(Text_))
  {
    // guard against case in which syntax string runs out before input string
    // but in middle of group and input string is still valid for entire parsing process.
    if (Status_->CalcNestingLevel() && SafeStrLen(Status_->OldSyntaxStr))
      return 1;
    else
      return (Status_->InSymbol.Any() || TermFound_);
  }

  return 1;
}

/****************************************************************************/
int Parser::GrabToken(ParseResultInfo* Results_, char* Text_, char* SyntaxStr_,
                      QuickParseStatus* Status_, char* Token_, int PrevValid_)
{
  if (Status_->NewSyntaxStr)
  {
    Status_->NewSyntaxStr = StripLeadingSpaces(Status_->NewSyntaxStr);  
    SyntaxStr_ = Status_->NewSyntaxStr;
  }
  else
    SyntaxStr_ = StripLeadingSpaces(SyntaxStr_);
  
  FieldInfo Finfo_;
  char* PrevSyntaxStr_;
  int TokType_;
  int RetCode_;
  int RetVal_ = 1;

  Finfo_
    .SetEndDelim("\"")
    .SetCode(1);

  Token_ = NULL;
  TokType_ = DetermineTokenType(SyntaxStr_);
  Status_->TokenType = TokType_;                // new token type to parse

  switch (TokType_)
  {
    // new, parser options inserted here
    case PARSER_OPTION:
      Token_ = FindGrouping(SyntaxStr_, "{#", "#}");
      if (Token_)
      {
        Status_->SetInParserOption();
        if (!QuickParse(Results_, Text_, SyntaxStr_, Status_, Token_, PrevValid_))
          RetVal_ = 0;
        else if (!CheckParseError(Results_, Text_, SyntaxStr_, Status_))
          RetVal_ = 0;
      }
      else
        RetVal_ = 0;
      break;
  
    case LITERAL:
      ++SyntaxStr_;
      RetCode_ = RepeatFindField(SyntaxStr_, &Finfo_, 1);
      if (!RetCode_)
      {
        RetVal_ = 0;
        break;
      }

      Token_ = Finfo_.GetField(TRUE);      
      if (Token_)
      {
        Status_->SetParseLiteral();
        if (!QuickParse(Results_, Text_, SyntaxStr_, Status_, Token_, PrevValid_))
          RetVal_ = 0;
        else if (!CheckParseError(Results_, Text_, SyntaxStr_, Status_))
          RetVal_ = 0;
      }
      else
        RetVal_ = 0;
      break;

        
    case SYMBOL:
      Token_ = FindGrouping(SyntaxStr_, "<", ">");
      if (Token_)
      {
        Status_->SetParseSymbol();
        if (!QuickParse(Results_, Text_, SyntaxStr_, Status_, Token_, PrevValid_))
          RetVal_ = 0;
        else if (!CheckParseError(Results_, Text_, SyntaxStr_, Status_))
          RetVal_ = 0;
      }
      else
        RetVal_ = 0;
      break;

    case REPEATABLE:
      PrevSyntaxStr_ = SyntaxStr_;
      Token_ = FindGrouping(SyntaxStr_, "{", "}");
      
      if (Token_)
      {
        Status_->NewSyntaxStr = PrevSyntaxStr_;

        // in case syntax string runs out before input string in middle of option
        if (Status_->CalcNestingLevel() == 0)
          Status_->OldSyntaxStr = SyntaxStr_;
        
        ++Status_->OptionLevel;
        Status_->SetParseRepOption();
        if (!QuickParse(Results_, Text_, SyntaxStr_, Status_, Token_, PrevValid_))
          RetVal_ = 0;
        else if (!CheckParseError(Results_, Text_, SyntaxStr_, Status_))
          RetVal_ = 0;
      }
      else
        RetVal_ = 0;
      break;

    case OPTION:
      Token_ = FindGrouping(SyntaxStr_, "[", "]");
      if (Token_)
      {
        // in case syntax string runs out before input string in middle of option
        if (Status_->CalcNestingLevel() == 0)
          Status_->OldSyntaxStr = SyntaxStr_;
      
        ++Status_->OptionLevel;
        Status_->SetParseOption();
        if (!QuickParse(Results_, Text_, SyntaxStr_, Status_, Token_, PrevValid_))
          RetVal_ = 0;
        else if (!CheckParseError(Results_, Text_, SyntaxStr_, Status_))
          RetVal_ = 0;
      }      
      else
        RetVal_ = 0;
      break;

    case GROUP:
      Token_ = FindGrouping(SyntaxStr_, "(", ")");
      if (Token_)
      {
        // in case syntax string runs out before input string in middle of group
        if (Status_->CalcNestingLevel() == 0)
          Status_->OldSyntaxStr = SyntaxStr_;
      
        ++Status_->GroupLevel;
        Status_->SetParseGroup();
        if (!QuickParse(Results_, Text_, SyntaxStr_, Status_, Token_, PrevValid_))
          RetVal_ = 0;
        else if (!CheckParseError(Results_, Text_, SyntaxStr_, Status_))
          RetVal_ = 0;
      }
      else
        RetVal_ = 0;
      break;

    // Or branch cannot occur within an option bracket so grouping bracket must be most recently encountered
    case OR_BRANCH:    
      if (!Status_->GroupLevel || !Status_->InGroup || (Status_->InGroup < Status_->InOption))
        return 0;
        
      Results_->SetBranch();
      break;

    default:
      return 0;
  }

  ::DeleteArray(Token_);
  return RetVal_;
}

/****************************************************************************/
ParseResultInfo* Parser::ResolveGroup(ParseResultInfo* Results_, int Branches_)
{
  ParseResultInfo* Next_ =  Results_->GetCurrent()->Next;
  LongNumber High_;
  LongNumber** Validity_ = (LongNumber**)RawAllocateWith(MEMMATRIX, sizeof(LongNumber*) * Branches_);
  int i, Max_;
  int Valid_ = 0;
  int BranchNum_ = 1;
  int BranchCnt_ = Branches_;

  for (i = 0; i < Branches_; i++)
    Validity_[i] = new LongNumber;
  
  --Validity_;
  
  while (Next_ && Next_->HasTokens())
  {
    Next_->SetNumBranches(BranchCnt_);
    Max_ = Next_->Index();
    Valid_ = Max_ ? 1:0;
    High_ = LongNumber(0, NumberSign::PLUS);

    #if QUICKINT_QUICKPARSE_DEBUG2
      cout <<"High   = " <<High_.ToTruncString().c_str() <<endl;
    #endif
    
    for (i = 0; Valid_ && i < Max_; i++)
      if (Next_->Tokens[i])
        if (Next_->Tokens[i]->TokenType == OPTION ||
            Next_->Tokens[i]->TokenType == REPEATABLE)
        {
          if (Next_->Tokens[i]->Validity)
            High_ += (LongNumber(1000) * Next_->Tokens[i]->Validity);
          else if (High_ % LongNumber(1000) == LongNumber(0))
            High_ += 1;

          #if QUICKINT_QUICKPARSE_DEBUG2
            cout <<"Token  = " <<Next_->Tokens[i]->Token <<endl;
            cout <<"Valid1 = " <<Next_->Tokens[i]->Validity.ToTruncString().c_str() <<endl;
            cout <<"High   = " <<High_.ToTruncString().c_str() <<endl;
          #endif
        }
        else
        {
          Valid_ = Valid_ && Next_->Tokens[i]->Validity;
          if (Valid_)
          {
            High_ += (LongNumber(1000) * Next_->Tokens[i]->Validity);

            #if QUICKINT_QUICKPARSE_DEBUG2
              cout <<"Token  = " <<Next_->Tokens[i]->Token <<endl;
              cout <<"Valid1 = " <<Next_->Tokens[i]->Validity.ToTruncString().c_str() <<endl;
              cout <<"High   = " <<High_.ToTruncString().c_str() <<endl;
            #endif
          }
        }

    *(Validity_[BranchNum_]) = (Valid_ ? High_:LongNumber(0, NumberSign::PLUS));
    #if QUICKINT_QUICKPARSE_DEBUG2    
      cout <<"Valid2 = " <<Validity_[BranchNum_]->ToTruncString().c_str() <<endl;
    #endif
    
    Next_ = Next_->OrBranch;
    BranchNum_++;
    BranchCnt_--;
  }

  Max_ = BranchNum_;
  High_ = LongNumber(0, NumberSign::PLUS);
  
  for (i = BranchNum_ = 1; i < Max_; i++)
    if (*(Validity_[i]) > High_)
    {
      High_ = *(Validity_[i]);
      BranchNum_ = i;
    }

  if (High_)
  {
    Results_->GetCurrent()->SetIntValue(BranchNum_);
    Results_->GetCurrent()->SetValidity(High_);

    #if QUICKINT_QUICKPARSE_DEBUG2    
      cout <<"High   = " <<High_.ToTruncString().c_str() <<endl;
    #endif
  }
  else
    Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

  ++Validity_;
  for (i = 0; i < Branches_; i++)
    delete Validity_[i];
  
  ::DeleteArray(Validity_);
  return Results_;
}

/****************************************************************************/
ParseResultInfo* Parser::ResolveOption(ParseResultInfo* Results_)
{
  ParseResultInfo* Next_ =  Results_->GetCurrent()->Next;
  LongNumber High_;
  LongNumber Validity_;  
  int i, Max_;
  int Valid_ = 0;

  if (Next_ && Next_->HasTokens())
  {
    Max_ = Next_->Index();
    Valid_ = Max_ ? 1:0;  
    
    for (i = 0; Valid_ && i < Max_; i++)
      if (Next_->Tokens[i])
        if (Next_->Tokens[i]->TokenType == OPTION ||
            Next_->Tokens[i]->TokenType == REPEATABLE)
        {
          if (Next_->Tokens[i]->Validity)
            High_ += (LongNumber(1000) * Next_->Tokens[i]->Validity);
          else if (High_ % LongNumber(1000) == LongNumber(0))
            High_ += 1;

          #if QUICKINT_QUICKPARSE_DEBUG2
            cout <<"Token  = " <<Next_->Tokens[i]->Token <<endl;
            cout <<"Valid1 = " <<Next_->Tokens[i]->Validity.ToTruncString().c_str() <<endl;
            cout <<"High   = " <<High_.ToTruncString().c_str() <<endl;
          #endif
        }
        else
        {
          Valid_ = Valid_ && Next_->Tokens[i]->Validity;
          if (Valid_)
          {
            High_ += (LongNumber(1000) * Next_->Tokens[i]->Validity);

            #if QUICKINT_QUICKPARSE_DEBUG2
              cout <<"Token  = " <<Next_->Tokens[i]->Token <<endl;
              cout <<"Valid1 = " <<Next_->Tokens[i]->Validity.ToTruncString().c_str() <<endl;
              cout <<"High   = " <<High_.ToTruncString().c_str() <<endl;
            #endif
          }
        }

    Validity_ = (Valid_ ? High_:LongNumber(0, NumberSign::PLUS));
    #if QUICKINT_QUICKPARSE_DEBUG2    
      cout <<"Valid2 = " <<Validity_.ToTruncString().c_str() <<endl;
    #endif
  }

  #if QUICKINT_QUICKPARSE_DEBUG2
    cout <<"Valid2 = " <<Validity_.ToTruncString().c_str() <<endl;
  #endif
  Results_->GetCurrent()->SetValidity(Validity_);    
  return Results_;
}

/****************************************************************************/
ParseResultInfo* Parser::ResolveFloat(ParseResultInfo* DefnInfo_, ParseResultInfo* TokenInfo_)
{
  ParseResultInfo* Next_ =  DefnInfo_->GetCurrent()->Next;

  // <integer> | <integer>\".\"<integer> | \".\"<integer>
  if (DefnInfo_->GetCurrent()->Validity &&
      DefnInfo_->GetCurrent()->DataType == ParseTokenType::INT_TYPE)
    if (DefnInfo_->GetCurrent()->IntValue == 1)
      TokenInfo_->GetCurrent()->SetFloatValue(Next_->Tokens[0]->IntValue);
    else
    {
      int Whole1_, Whole2_;
      
      if (DefnInfo_->GetCurrent()->IntValue == 2)
      {
        Next_ = Next_->OrBranch;
        Whole1_ = Next_->Tokens[0]->IntValue;
        Whole2_ = Next_->Tokens[2]->IntValue;
      }

      if (DefnInfo_->GetCurrent()->IntValue == 3)
      {
        Next_ = Next_->OrBranch->OrBranch;
        Whole1_ = 0;
        Whole2_ = Next_->Tokens[1]->IntValue;
      }

      double Frac_ = Whole2_;
      int Div_ = 1;

      while (Whole2_)
      {
        Whole2_ /= 10;
        Div_ *= 10;
      }      

      Frac_ = Whole1_ + (Frac_ / Div_);      
      TokenInfo_->GetCurrent()->SetFloatValue(Frac_);
    }

  return TokenInfo_;
}

/****************************************************************************/
char* Parser::ExtractStrTokenFnc(char*& Text_, char* SyntaxStr_, int Quoted_, char* SymbolType_)
{
  FieldInfo Finfo_;
  char* Field_;
  char* SavedText_ = Text_;
  char* Token_ = NULL;
  char* Buffer_ = NULL;
  int TokType_, RetCode_;

  if (!Text_ || !SafeStrLen(Text_) || !SyntaxStr_ || !SafeStrLen(SyntaxStr_))
    return NULL;

  Finfo_
    .SetEndDelim("\"")
    .SetCode(1);

  SyntaxStr_ = StripLeadingSpaces(SyntaxStr_);
  TokType_ = DetermineTokenType(SyntaxStr_);

  if (TokType_ == OPTION || TokType_ == REPEATABLE)
  {
    #if QUICKINT_QUICKPARSE_DEBUG3
      cout <<"Parsing Token Type: Option" <<endl;
    #endif  
    Token_ = FindGrouping(SyntaxStr_, "[", "]");
    
    if (Token_)
    {
      #if QUICKINT_QUICKPARSE_DEBUG3    
        cout <<"Extracting Token: " <<Token_ <<endl;
      #endif
    
      Token_ = StripGrouping(Token_, "[", "]");
      Buffer_ = ExtractStrTokenFnc(Text_, Token_, Quoted_, SymbolType_);

      if (!Buffer_)
      {
        #if QUICKINT_QUICKPARSE_DEBUG3
          cout <<"Extracting Token: " <<SyntaxStr_ <<endl;
        #endif
      
        Text_ = SavedText_;
        Buffer_ = ExtractStrTokenFnc(Text_, SyntaxStr_, Quoted_, SymbolType_);

        if (!Buffer_)
        {
          #if QUICKINT_QUICKPARSE_DEBUG3        
            cout <<"Extract Token: " <<Token_ <<": Failed" <<endl;
          #endif          
          Text_ = SavedText_;
        }
      }
    }
    else
      Buffer_ = NULL;
  }
  else if (TokType_ == GROUP)
  {
    #if QUICKINT_QUICKPARSE_DEBUG3  
      cout <<"Parsing Token Type: Group" <<endl;
    #endif  
    Token_ = FindGrouping(SyntaxStr_, "(", ")");

    if (Token_)
    {
      char* NewToken_;
      NewToken_ = Token_ = StripGrouping(Token_, "(", ")");
      int Branches_ = FindNumBranches(Token_);

      #if QUICKINT_QUICKPARSE_DEBUG3
        cout <<"Extracting Token: " <<Token_ <<endl;
      #endif      
      Buffer_ = ExtractStrTokenFnc(Text_, Token_, Quoted_, SymbolType_);

      if (!Buffer_)
      {
        for (--Branches_; Branches_ && NewToken_ && !Buffer_; Branches_--)
        {        
          NewToken_ = FindNextBranch(NewToken_);
          Text_ = SavedText_;
          
          if (NewToken_)
          {
            #if QUICKINT_QUICKPARSE_DEBUG3          
              cout <<"Extracting Token: " <<NewToken_ <<endl;
            #endif          
            Buffer_ = ExtractStrTokenFnc(Text_, NewToken_, Quoted_, SymbolType_);
          }
        }

        if (!Buffer_)
        {
          #if QUICKINT_QUICKPARSE_DEBUG3        
            cout <<"Extract Token: " <<Token_ <<": Failed" <<endl;
          #endif
          Text_ = SavedText_;
        }
      }
    }
  }
  else if (TokType_ == OR_BRANCH)
  {
    // if reached the end of OR-branch then, try out all predefined symbols as token separators
    const char* Temp_ = "(<assignment> | <quoted string> | <single quoted string> | <double quoted string> | <terminator> | <semi-terminator> | <whitespace>)";
    char* SpareSep_ = new_char_string(Temp_);

    if (SpareSep_)
    {
      char* RetVal_ = ExtractStrTokenFnc(Text_, SpareSep_, Quoted_, SymbolType_);
      ::DeleteArray(SpareSep_);
      return RetVal_;
    }

    return NULL;  
  }
  else if (TokType_ == SYMBOL)
  {
    #if QUICKINT_QUICKPARSE_DEBUG3  
      cout <<"Parsing Token Type: Symbol" <<endl;
    #endif
  
    const char* Delim_ = NULL;
    Token_ = FindGrouping(SyntaxStr_, "<", ">");
    
    if (strncmp("<assignment>", Token_, SafeStrLen("<assignment>")) == 0)
      Delim_ = _Assignment;
    else if (strncmp("<quoted string>", Token_, SafeStrLen("<quoted string>")) == 0)
      Delim_ = _StartQuote;
    else if (strncmp("<single quoted string>", Token_, SafeStrLen("<single quoted string>")) == 0)
      Delim_ = "\'";
    else if (strncmp("<double quoted string>", Token_, SafeStrLen("<double quoted string>")) == 0)
      Delim_ = "\"";
    else if (strncmp("<terminator>", Token_, SafeStrLen("<terminator>")) == 0)
      Delim_ = _Terminator;
    else if (strncmp("<semi-terminator>", Token_, SafeStrLen("<semi-terminator>")) == 0)
      Delim_ = _SemiTerminator;
    else if (strncmp("<whitespace>", Token_, SafeStrLen("<whitespace>")) == 0)
      Delim_ = " ";
    else
    {
      // user defined symbols
      char* Defn_ = GroupToken(FindDefinition(Token_), "(", ")");
      
      if (Defn_)
      {
        Buffer_ = ExtractStrTokenFnc(Text_, Defn_, Quoted_, SymbolType_);
        ::DeleteArray(Defn_);
      }
    }

    if (Delim_)
    {
      if (isspace(*Delim_))
        Field_ = FindWord(Text_, RetCode_, 1);
      else
        Field_ = FindField(Text_, NULL, Delim_, RetCode_, 1, FALSE);

      #if QUICKINT_QUICKPARSE_DEBUG3
        cout <<"Finding Delimiter: " <<Delim_ <<endl;
        cout <<"Field Extracted: " <<Field_ <<endl;
      #endif        

      int TypeCode_ = (strncmp("<unquoted string>", SymbolType_, SafeStrLen("<unquoted string>")) == 0) ? 1:
                      (strncmp("<integer>", SymbolType_, SafeStrLen("<integer>")) == 0) ? 2:0;

      int IntValue_;
      int ValidField_ =
      (Field_ && SafeStrLen(Field_) && RetCode_) ?
         ((TypeCode_ == 1) ? IsValidStrField(Field_, Quoted_, SymbolType_):
          (TypeCode_ == 2) ? IsValidIntField(Field_, IntValue_):0):0;          

      if (ValidField_)
      {
        Buffer_ = new_char_string(Field_);
        if (!isspace(*Delim_))
          Text_ -= SafeStrLen(Delim_);        
      }
      else
      {
        #if QUICKINT_QUICKPARSE_DEBUG3      
          cout <<"Field Extraction Failed" <<endl;
        #endif
      
        Text_ = SavedText_;
        Buffer_ = NULL;
      }
    }    
  }
  else if (TokType_ == LITERAL)
  {
    #if QUICKINT_QUICKPARSE_DEBUG3  
      cout <<"Parsing Token Type: Literal" <<endl;
    #endif
  
    ++SyntaxStr_;
    RetCode_ = RepeatFindField(SyntaxStr_, &Finfo_, 1);

    if (RetCode_)
    {
      Token_ = Finfo_.GetField(TRUE);

      if (Token_)
      {      
        if (isspace(*Token_))
          Field_ = FindWord(Text_, RetCode_, 1);
        else
          Field_ = FindField(Text_, NULL, Token_, RetCode_, 1, FALSE);

        #if QUICKINT_QUICKPARSE_DEBUG3
          cout <<"Finding Delimiter: " <<Token_ <<endl;
          cout <<"Field Extracted: " <<Field_ <<endl;
        #endif          

        int TypeCode_ = (strncmp("<unquoted string>", SymbolType_, SafeStrLen("<unquoted string>")) == 0) ? 1:
                        (strncmp("<integer>", SymbolType_, SafeStrLen("<integer>")) == 0) ? 2:0;
                        
        int IntValue_;
        int ValidField_ =
        (Field_ && SafeStrLen(Field_) && RetCode_) ?
           ((TypeCode_ == 1) ? IsValidStrField(Field_, Quoted_, SymbolType_):
            (TypeCode_ == 2) ? IsValidIntField(Field_, IntValue_):0):0;

        if (ValidField_)
        {
          Buffer_ = new_char_string(Field_);
          if (!isspace(*Token_))
            Text_ -= SafeStrLen(Token_);
        }
        else
        {
          #if QUICKINT_QUICKPARSE_DEBUG3
            cout <<"Field Extraction Failed" <<endl;
          #endif
        
          Text_ = SavedText_;
          Buffer_ = NULL;
        }
      }
    }
  }

  ::DeleteArray(Token_);
  return Buffer_;
}

/****************************************************************************/
char* Parser::ExtractStrToken(char*& Text_, char* SyntaxStr_, int Quoted_, char* SymbolType_)
{
  // if syntax string has ran out, try out all predefined symbols as token separators  
  if (!SyntaxStr_ || !SafeStrLen(SyntaxStr_))
  {
    const char* Temp_ = "(<assignment> | <quoted string> | <single quoted string> | <double quoted string> | <terminator> | <semi-terminator> | <whitespace>)";
    char* SpareSep_ = new_char_string(Temp_);

    if (SpareSep_)
    {
      char* RetVal_ = ExtractStrTokenFnc(Text_, SpareSep_, Quoted_, SymbolType_);
      ::DeleteArray(SpareSep_);
      return RetVal_;
    }

    return NULL;
  }

  return ExtractStrTokenFnc(Text_, SyntaxStr_, Quoted_, SymbolType_);
}

/****************************************************************************/
int Parser::IsValidStrField(char* Field_, int Quoted_, char* Type_)
{
  int i, Max, Len_;

  for (i = 0, Max = SafeStrLen(Field_); i < Max; i++)
  {
    if (!Quoted_)
    {  
      // no assignment characters
      Len_ = SafeStrLen(_Assignment);
      if (SafeStrLen(&Field_[i]) >= Len_)
        if (strncmp(&Field_[i], _Assignment, Len_) == 0)
          return 0;

      // no terminator characters
      Len_ = SafeStrLen(_Terminator);
      if (SafeStrLen(&Field_[i]) >= Len_)
        if (strncmp(&Field_[i], _Terminator, Len_) == 0)
          return 0;

      // no semi-terminator characters
      Len_ = SafeStrLen(_SemiTerminator);
      if (SafeStrLen(&Field_[i]) >= Len_)
        if (strncmp(&Field_[i], _SemiTerminator, Len_) == 0)
          return 0;

      // no quote characters
      Len_ = SafeStrLen(_StartQuote);
      if (SafeStrLen(&Field_[i]) >= Len_)
        if (strncmp(&Field_[i], _StartQuote, Len_) == 0)
          return 0;

      Len_ = SafeStrLen(_EndQuote);
      if (SafeStrLen(&Field_[i]) >= Len_)
        if (strncmp(&Field_[i], _EndQuote, Len_) == 0)
          return 0;

      // no actual quote character
      if (Field_[i] == '\"')
        return 0;

      // no white space
      if (isspace(Field_[i]))
        return 0;          
    }
    else if (Type_)
    {
      if (strcmp(Type_, "<quoted string>") == 0)
      {
        // no quote characters
        Len_ = SafeStrLen(_StartQuote);
        if (SafeStrLen(&Field_[i]) >= Len_)
          if (strncmp(&Field_[i], _StartQuote, Len_) == 0)
            return 0;

        Len_ = SafeStrLen(_EndQuote);
        if (SafeStrLen(&Field_[i]) >= Len_)
          if (strncmp(&Field_[i], _EndQuote, Len_) == 0)
            return 0;

        // no actual quote character
        if (Field_[i] == '\"')
          return 0;
      }
      else if (strcmp(Type_, "<double quoted string>") == 0)
      {
        // double quote character must be escaped
        if (Field_[i] && Field_[i] == '\"')
          if (Max < 2 || Field_[i-1] != '\\')
            return 0;      
      }
      else if (strcmp(Type_, "<single quoted string>") == 0)
      {
        // single quote character must be escaped
        if (Field_[i] && Field_[i] == '\'')
          if (Max < 2 || Field_[i-1] != '\\')
            return 0;      
      }      
    }
  }

  return 1;
}

/****************************************************************************/
int Parser::IsValidIntField(char* Field_, int& IntValue_)
{
  int i, Max;

  for (i = 0, Max = SafeStrLen(Field_); i < Max; i++)
  {
    // no non-digit characters
    if (SafeStrLen(&Field_[i]))
      if (!isdigit(Field_[i]))
        return 0;

  }

  IntValue_ = atoi(Field_);
  return 1;
}

/****************************************************************************/
int Parser::NotInQuotes(char* Text_, char* Token_, int Whitespace_)
{
  int SingleQuote_ = 0;
  int DoubleQuote_ = 0;
  int i, Max, Len_, TokenVerified_;
  
  TokenVerified_ = 0;
  
  if (Whitespace_)
    TokenVerified_ = isspace(*Text_);
  else
    TokenVerified_ = strncmp(Text_, Token_, SafeStrLen(Token_)) == 0;

  if (TokenVerified_)
  {
    for (i = 0, Max = SafeStrLen(Text_); i < Max; i++)
    {
      Len_ = SafeStrLen(_StartQuote);
      if (SafeStrLen(&Text_[i]) >= Len_)
        if (strncmp(&Text_[i], _StartQuote, Len_) == 0)
          return 1;

      Len_ = SafeStrLen(_EndQuote);
      if (SafeStrLen(&Text_[i]) >= Len_)
        if (strncmp(&Text_[i], _EndQuote, Len_) == 0)
          return 0;

      if (Text_[i] == '\"')
      {
        if ((i && Text_[i-1] == '\\') || SingleQuote_)
          continue;
        else
        {
          if (!DoubleQuote_)
            DoubleQuote_++;
          else
            DoubleQuote_--;
        }
      }
      else if (Text_[i] == '\'')
      {
        if ((i && Text_[i-1] == '\\') || DoubleQuote_)
          continue;
        else
        {
          if (!SingleQuote_)
            SingleQuote_++;
          else
            SingleQuote_--;
        }      
      }        
    }

    return (!SingleQuote_ && !DoubleQuote_);
  }

  return 0;
}

/****************************************************************************/
void Parser::ShowParseState(QuickParseStatus* Status_, char* Token_, int AtStart_, int AtRoot_, ParseResultInfo* Results_)
{
  ParseTokenType* Current_ = Results_->GetCurrent();
  char Buffer_[128];
  int i, Max;

  Buffer_[0] = 0;
  Max = Status_->CalcNestingLevel();
  if (!AtRoot_)
    Max++;

  if (Max)
  {
    SafeStrCpy(Buffer_, "--");
    for (i = 1; i < Max; i++)
      SafeStrCat(Buffer_, "--");
  }

  if (AtStart_)
    cout <<Buffer_ <<" Begin Parsing: " <<Token_
                   <<" | IG:" <<Status_->InGroup <<" | IO:" <<Status_->InOption
                   <<" | IS:" <<Status_->InSymbol <<" | IL:" <<Status_->InLiteral
                   <<" | IP:" <<Status_->InParserOption <<" | R:" <<Status_->Repeatable
                   <<" | T:" <<Status_->TerminatorFound <<"  | ST:" <<Status_->SemiTerminatorFound
                   <<endl;
  else
    cout <<Buffer_ <<" End Parsing: " <<Token_
                   <<" (Valid=" <<Current_->Validity.ToTruncString().c_str() <<", Int=" <<Current_->IntValue
                   <<", Float=" <<Current_->FloatValue <<", Str=" <<Current_->StrValue <<")"
                   <<" | IG:" <<Status_->InGroup <<" | IO:" <<Status_->InOption
                   <<" | IS:" <<Status_->InSymbol <<" | IL:" <<Status_->InLiteral
                   <<" | IP:" <<Status_->InParserOption <<" | R:" <<Status_->Repeatable
                   <<" | T:" <<Status_->TerminatorFound <<"  | ST:" <<Status_->SemiTerminatorFound
                   <<endl;  
}

/****************************************************************************/
void Parser::ShowResults(ParseResultInfo* Results_, int Level_)
{
  char Buffer_[128];
  int i, Max;

  Buffer_[0] = 0;
  Max = Level_;

  if (Max)
  {
    SafeStrCpy(Buffer_, "--");
    for (i = 1; i < Max; i++)
      SafeStrCat(Buffer_, "--");
  }    

  for (i = 0, Max = Results_->Index(); i < Max; i++)
  {
    cout <<Buffer_
         <<"TokenType: " <<Results_->Tokens[i]->GiveType()
         <<" | Token: " <<Results_->Tokens[i]->GiveToken()
         <<" | Data: "; Results_->Tokens[i]->ShowData(cout);
    cout <<" | Valid: " <<Results_->Tokens[i]->Validity.ToTruncString().c_str();
    cout <<endl;

    if (Results_->Tokens[i]->Next)
      ShowResults(Results_->Tokens[i]->Next, Level_ + 1);
  }

  if (Results_->OrBranch)
  {
    cout <<Buffer_ <<"*** Or Branch ***" <<endl;
    ShowResults(Results_->OrBranch, Level_);
  }
}

/****************************************************************************/
ParseResultInfo* Parser::QuickParse(ParseResultInfo* Results_, char* Text_, char* SyntaxStr_,
                                    QuickParseStatus* Status_, char* Token_, int PrevValid_)
{
  if (Status_->NewInputStr)
    Text_ = Status_->NewInputStr;

  if (Status_->ParseGroup)
  {
    if (!PrevValid_)
    {
      Status_->SetError();
      Results_->GetNext()->SetTokenType(GROUP);
      Results_->InsertToken(Token_);
      Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 0, 0, Results_);
      #endif

      --Status_->GroupLevel;    
    }
    else
    {
      Results_->GetNext()->SetTokenType(GROUP);
      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 1, 0, Results_);
        char* SavedToken_ = new_char_string(Token_);
      #endif
  
      Status_->SetInGroup();
      Status_->ParseGroup = 0;
    
      Results_->InsertToken(Token_);
      Token_ = StripGrouping(Token_, "(", ")");
    
      char* NestedToken_ = NULL;
      QuickParseStatus NestedStatus_;
      NestedStatus_.Assign(Status_);
      NestedStatus_.NewSyntaxStr = NULL;
      NestedStatus_.NewInputStr = NULL;
      NestedStatus_.StackDisplayLevel++;       // to show symbols nested within the group at a different level
    
      ParseResultInfo* NestedResults_ = Results_->GetCurrent()->SetNext();

      int i;
      int High_ = MaxOf(Status_->DeepestLevel, Status_->GroupLevel + Status_->OptionLevel);
      int Valid_ = 0;
      int BranchNum_ = 0;
      int Branches_ = FindNumBranches(Token_);

      if (Status_->DeepestLevel < High_)
        Status_->DeepestLevel = High_;

      QuickParseStatus** CompStatus_ = (QuickParseStatus**)RawAllocateWith(MEMMATRIX, sizeof(QuickParseStatus*) * Branches_);
      for (BranchNum_ = 0; BranchNum_ < Branches_; BranchNum_++)
      {
        CompStatus_[BranchNum_] = new QuickParseStatus;
        CompStatus_[BranchNum_]->Reset();
      }

      BranchNum_ = 0;

      for (;;)
      {
        Valid_ = GrabToken(NestedResults_, Text_, Token_, &NestedStatus_, NestedToken_, 1);

        if (Valid_)
        {
          if (!NestedResults_->OrBranch)
          {
            CompStatus_[BranchNum_]->Assign(&NestedStatus_);
            NestedStatus_.NewInputStr = NULL;
            NestedStatus_.SemiTerminatorFound = NestedStatus_.TerminatorFound = 0;
            break;
          }
          else
          {
            CompStatus_[BranchNum_]->Assign(&NestedStatus_);
            NestedStatus_.NewInputStr = NULL;
            NestedStatus_.SemiTerminatorFound = NestedStatus_.TerminatorFound = 0;
          
            NestedResults_ = NestedResults_->OrBranch;
            BranchNum_++;
            NestedStatus_.NewSyntaxStr++;
          }
        }
        else
          break;
      }

      for (i = 0; i < Branches_; i++)
      {
        int Level_ = CompStatus_[i]->GroupLevel + CompStatus_[i]->OptionLevel;
        if (CompStatus_[i]->DeepestLevel > Level_)
          Level_ = CompStatus_[i]->DeepestLevel;
      
        if (Level_ > High_)
          High_ = Level_;
      }

      if (Valid_)
      {
        Results_ = ResolveGroup(Results_, Branches_);

        if (Results_->GetCurrent()->Validity)
        {
          BranchNum_ = Results_->GetCurrent()->IntValue - 1;
          Text_ = Status_->NewInputStr = CompStatus_[BranchNum_]->NewInputStr;

          if (CompStatus_[BranchNum_]->TerminatorFound)
            Status_->TerminatorFound = CompStatus_[BranchNum_]->TerminatorFound;
          
          if (CompStatus_[BranchNum_]->SemiTerminatorFound)
            Status_->SemiTerminatorFound = CompStatus_[BranchNum_]->SemiTerminatorFound;
        }
      }
      else
        Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

      for (BranchNum_ = 0; BranchNum_ < Branches_; BranchNum_++)
        delete CompStatus_[BranchNum_];
      ::DeleteArray(CompStatus_);
    
      Status_->ParseGroup = 1;
      Status_->DeepestLevel = High_;

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, SavedToken_, 0, 0, Results_);
        ::DeleteArray(SavedToken_);
      #endif

      --Status_->GroupLevel;
    }
  }
  else if (Status_->ParseOption)
  {
    if (!PrevValid_)
    {
      Status_->SetError();
      if (Status_->Repeatable)
        Results_->GetNext()->SetTokenType(REPEATABLE);     // set repeatable option
      else
        Results_->GetNext()->SetTokenType(OPTION);
      
      Results_->InsertToken(Token_);
      Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

      // set previous SyntaxStr into current SyntaxStr if repeatable option is set
      if (Status_->Repeatable)
      {
        SyntaxStr_ = Status_->NewSyntaxStr;
        Status_->NewSyntaxStr = NULL;
      }

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 0, 0, Results_);
      #endif

      --Status_->OptionLevel;    
    }
    else
    {
      if (Status_->Repeatable)
        Results_->GetNext()->SetTokenType(REPEATABLE);     // set repeatable option
      else
        Results_->GetNext()->SetTokenType(OPTION);
      
      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 1, 0, Results_);
        char* SavedToken_ = new_char_string(Token_);
      #endif

      int Valid_ = 0;
      int High_ = MaxOf(Status_->DeepestLevel, Status_->GroupLevel + Status_->OptionLevel);
    
      if (Status_->DeepestLevel < High_)
        Status_->DeepestLevel = High_;
    
      Status_->SetInOption();
      Status_->ParseOption = 0;
    
      Results_->InsertToken(Token_);
      if (Status_->Repeatable)
        Token_ = StripGrouping(Token_, "{", "}");       // grouping brackets for repeatable option
      else
        Token_ = StripGrouping(Token_, "[", "]");

      char* NestedToken_ = NULL;
      QuickParseStatus NestedStatus_;
      NestedStatus_.Assign(Status_);
      NestedStatus_.NewSyntaxStr = NULL;
      NestedStatus_.NewInputStr = NULL;
      NestedStatus_.StackDisplayLevel++;       // to show symbols nested within the option at a different level
      ParseResultInfo* NestedResults_ = Results_->GetCurrent()->SetNext();

      Valid_ = GrabToken(NestedResults_, Text_, Token_, &NestedStatus_, NestedToken_, 1);

      int Level_ = NestedStatus_.GroupLevel + NestedStatus_.OptionLevel;
      if (NestedStatus_.DeepestLevel > Level_)
        Level_ = NestedStatus_.DeepestLevel;
      
      if (Level_ > High_)
        High_ = Level_;
    
      if (Valid_)
      {
        Results_ = ResolveOption(Results_);
      
        if (Results_->GetCurrent()->Validity)
        {
          Text_ = Status_->NewInputStr = NestedStatus_.NewInputStr;

          if (NestedStatus_.TerminatorFound)
            Status_->TerminatorFound = NestedStatus_.TerminatorFound;
          
          if (NestedStatus_.SemiTerminatorFound)
            Status_->SemiTerminatorFound = NestedStatus_.SemiTerminatorFound;
        }
      }
      else
        Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

      Status_->ParseOption = 1;
      Status_->DeepestLevel = High_;

      // set previous SyntaxStr into current SyntaxStr if repeatable option is set
      if (Status_->Repeatable)
      {
        SyntaxStr_ = Status_->NewSyntaxStr;
        Status_->NewSyntaxStr = NULL;
      }

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, SavedToken_, 0, 0, Results_);
        ::DeleteArray(SavedToken_);
      #endif

      --Status_->OptionLevel;
    }
  }
  else if (Status_->InParserOption)
  {
    // set parser options
    Results_->GetNext()->SetTokenType(PARSER_OPTION);
    #if QUICKINT_QUICKPARSE_DEBUG1
      ShowParseState(Status_, Token_, 1, 0, Results_);
    #endif

    Token_ = TrimSpaces(Token_);
    char* SavedToken_ = new_char_string(Token_);
    
    Token_ = StripGrouping(Token_, "{#", "#}");       // grouping brackets for parser options
    if (strncmp("set_icase", Token_, SafeStrLen("set_icase")) == 0)
    {
      Results_->InsertToken(SavedToken_);
      Status_->SetCaseSensitive(0);
    }
    else if (strncmp("set_case", Token_, SafeStrLen("set_case")) == 0)
    {    
      Results_->InsertToken(SavedToken_);
      Status_->SetCaseSensitive(1);      
    }

    #if QUICKINT_QUICKPARSE_DEBUG1
      ShowParseState(Status_, Token_, 0, 0, Results_);
    #endif

    ::DeleteArray(SavedToken_);
    Status_->InParserOption = 0;
  }  
  else if (Status_->ParseSymbol)
  {
    if (!PrevValid_)
    {
      Status_->SetError();
      Results_->GetNext()->SetTokenType(SYMBOL);
      Results_->InsertToken(Token_);
      Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 0, 0, Results_);
      #endif    
    }
    else
    {
      Status_->SetInSymbol();
      Results_->GetNext()->SetTokenType(SYMBOL);
      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 1, 0, Results_);
      #endif
  
      if (strncmp("<whitespace>", Token_, SafeStrLen("<whitespace>")) == 0)
      {
        Results_->InsertToken(Token_);
    
        if (isspace(*Text_) && NotInQuotes(Text_, NULL, 1))
          Text_ = StripLeadingSpaces(Text_);
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else if (strncmp("<assignment>", Token_, SafeStrLen("<assignment>")) == 0)
      {
        int Len_ = SafeStrLen(_Assignment);
        Results_->InsertToken(Token_);
            
        if (strncmp(Text_, _Assignment, Len_) == 0 && NotInQuotes(Text_, _Assignment, 0))
          Text_ += Len_;
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else if (strncmp("<double quoted string>", Token_, SafeStrLen("<double quoted string>")) == 0)
      {
        Results_->InsertToken(Token_);
      
        if (strncmp(Text_, "\"", 1) == 0)
        {
          int RetVal_;
          char* OldStr_ = Text_;
          char* Field_ = FindField(Text_, "\"", "\"", RetVal_, 1, FALSE);
        
          if (RetVal_ && IsValidStrField(Field_, 1, Token_))
            Results_->GetCurrent()->SetStrValue(Field_);
          else
          {
            Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
            Text_ = OldStr_;
          }
        }
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else if (strncmp("<single quoted string>", Token_, SafeStrLen("<single quoted string>")) == 0)
      {
        Results_->InsertToken(Token_);
      
        if (strncmp(Text_, "\'", 1) == 0)
        {
          int RetVal_;
          char* OldStr_ = Text_;
          char* Field_ = FindField(Text_, "\'", "\'", RetVal_, 1, FALSE);
        
          if (RetVal_ && IsValidStrField(Field_, 1, Token_))
            Results_->GetCurrent()->SetStrValue(Field_);
          else
          {
            Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
            Text_ = OldStr_;
          }
        }
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else if (strncmp("<quoted string>", Token_, SafeStrLen("<quoted string>")) == 0)
      {
        int Len_ = SafeStrLen(_StartQuote);
        Results_->InsertToken(Token_);
      
        if (strncmp(Text_, _StartQuote, Len_) == 0)
        {
          int RetVal_;
          char* OldStr_ = Text_;
          char* Field_ = FindField(Text_, _StartQuote, _EndQuote, RetVal_, 1, FALSE);
        
          if (RetVal_ && IsValidStrField(Field_, 1, Token_))
            Results_->GetCurrent()->SetStrValue(Field_);
          else
          {
            Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
            Text_ = OldStr_;
          }
        }
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else if (strncmp("<unquoted string>", Token_, SafeStrLen("<unquoted string>")) == 0)
      {
        Results_->InsertToken(Token_);

        // We not actually parsing the syntax string, only temporarily using it to find the
        // separator token used to extract out the string value
        char* SyntaxBuf_ = new_char_string(SyntaxStr_);
        char* Field_ = ExtractStrToken(Text_, SyntaxBuf_, 0, Token_);

        if (Field_)
          Results_->GetCurrent()->SetStrValue(Field_);
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

        ::DeleteArray(SyntaxBuf_);
      }
      else if (strncmp("<integer>", Token_, SafeStrLen("<integer>")) == 0)
      {
        Results_->InsertToken(Token_);

        // We not actually parsing the syntax string, only temporarily using it to find the
        // separator token used to extract out the string value
        char* SyntaxBuf_ = new_char_string(SyntaxStr_);
        char* Field_ = ExtractStrToken(Text_, SyntaxBuf_, 0, Token_);
        int IntValue_;

        if (Field_ && IsValidIntField(Field_, IntValue_))
          Results_->GetCurrent()->SetIntValue(IntValue_);
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

        ::DeleteArray(SyntaxBuf_);
      }
      else if (strncmp("<terminator>", Token_, SafeStrLen("<terminator>")) == 0)
      {
        int Len_ = SafeStrLen(_Terminator);
        Results_->InsertToken(Token_);
            
        if (strncmp(Text_, _Terminator, Len_) == 0 && NotInQuotes(Text_, _Terminator, 0))
        {
          Text_ += Len_;
          Status_->TerminatorFound = 1;
        }
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else if (strncmp("<semi-terminator>", Token_, SafeStrLen("<semi-terminator>")) == 0)
      {
        int Len_ = SafeStrLen(_SemiTerminator);
        Results_->InsertToken(Token_);
            
        if (strncmp(Text_, _SemiTerminator, Len_) == 0 && NotInQuotes(Text_, _SemiTerminator, 0))
        {
          Text_ += Len_;
          Status_->SemiTerminatorFound = 1;
        }
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else
      {
        // user defined symbols
        char* Defn_ = GroupToken(FindDefinition(Token_), "(", ")");
      
        if (Defn_)
        {
          Results_->InsertToken(Token_);
          Status_->ParseSymbol = 0;

          int Valid_;
          char* DefnToken_ = NULL;
          QuickParseStatus DefnStatus_;
          DefnStatus_.Assign(Status_);
          DefnStatus_.NewSyntaxStr = NULL;
          DefnStatus_.NewInputStr = NULL;
          DefnStatus_.StackDisplayLevel++;       // to show symbols nested within the group at a different level
          ParseResultInfo* DefnResults_ = Results_->GetCurrent()->SetNext();

          Valid_ = GrabToken(DefnResults_, Text_, Defn_, &DefnStatus_, DefnToken_, 1);

          if (Valid_)
          {
            if (strcmp(Token_, "<float>") == 0)
              Results_ = ResolveFloat(DefnResults_, Results_);

            Results_->GetCurrent()->SetValidity(DefnResults_->GetCurrent()->Validity);
            Text_ = Status_->NewInputStr = DefnStatus_.NewInputStr;

            if (DefnStatus_.TerminatorFound)
              Status_->TerminatorFound = DefnStatus_.TerminatorFound;
          
            if (DefnStatus_.SemiTerminatorFound)
              Status_->SemiTerminatorFound = DefnStatus_.SemiTerminatorFound;
          }
          else
            Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

          // Add one more level down for tokens defined under the aggregate symbol
          DefnStatus_.DeepestLevel++;
          if (DefnStatus_.DeepestLevel > Status_->DeepestLevel)
            Status_->DeepestLevel = DefnStatus_.DeepestLevel;

          Status_->ParseSymbol = 1;
          ::DeleteArray(Defn_);
        }
      }

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 0, 0, Results_);
      #endif
    }
  }
  else if (Status_->ParseLiteral)
  {
    if (!PrevValid_)
    {
      Status_->SetError();
      Results_->GetNext()->SetTokenType(LITERAL);
      Results_->InsertToken(Token_);
      Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 0, 0, Results_);
      #endif
    }
    else
    {
      Status_->SetInLiteral();
      Results_->GetNext()->SetTokenType(LITERAL);
      Results_->InsertToken(Token_);
    
      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 1, 0, Results_);
      #endif

      // Grab string until end of literal
      if (Status_->ParserFlags.GetBit(QuickParseStatus::CASE_SENSITIVE))
      {
        // {set_case} parser option
        if (strncmp(Text_, Token_, SafeStrLen(Token_)) == 0)
        {
          Text_ += SafeStrLen(Token_);
          Results_->GetCurrent()->SetValidity(SafeStrLen(Token_), NumberSign::PLUS);
        }
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }
      else
      {
        // {set_icase} parser option
        if (StrniComp(Text_, Token_, SafeStrLen(Token_)) == 0)
        {
          Text_ += SafeStrLen(Token_);
          Results_->GetCurrent()->SetValidity(SafeStrLen(Token_), NumberSign::PLUS);
        }
        else
          Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);
      }

      #if QUICKINT_QUICKPARSE_DEBUG1
        ShowParseState(Status_, Token_, 0, 0, Results_);
      #endif
    }
  }

  // update input and syntax string after current parse cycle has been completed.
  Status_->NewInputStr = Text_;
  Status_->NewSyntaxStr = SyntaxStr_;

  // in case syntax string runs out before input string in middle of option or group
  if (Status_->CalcNestingLevel() == 0)
    Status_->OldSyntaxStr = SyntaxStr_;

  // determine if branch condition is valid and if terminator symbol was found
  // Or branch cannot occur within an option bracket so grouping bracket must be most recently encountered  
  int BranchCond_ = (Status_->GroupLevel != 0) && (Status_->InGroup > Status_->InOption);
  int TermFound_ = (Status_->TerminatorFound || Status_->SemiTerminatorFound);
  int Optional_ = (Status_->InParserOption || Status_->ParseOption);

  // if current non-optional token is parsed as invalid and branch condition is invalid
  // then return whole token as invalid
  if (!Results_->GetCurrent()->Validity && !Optional_ && !BranchCond_)
  {
    Status_->ResetStatusFlag();         // reset parser flags since we're done parsing current token
    return NULL;
  }

  // parse next token if syntax string and text input line has not been completely read
  if (SafeStrLen(Text_) && SafeStrLen(SyntaxStr_) && !TermFound_)
  {
    // determine whether previous token was optional, if it isn't and is parsed as invalid
    // then next token is set to be invalid.
    int NextValid_ = Results_->GetCurrent()->Validity || Optional_;

    // reset parser flags since we're done parsing current token
    Status_->ResetStatusFlag();    
  
    if (!GrabToken(Results_, Text_, SyntaxStr_, Status_, Token_, NextValid_))
      return NULL;
    else
      return Results_;                      // return result after token is determined valid.
  }

  // If either the input string has ran out or the syntax string has ran out or the terminator has
  // been found then skip the step of retrieving the next token and return results.
  Status_->ResetStatusFlag();           // reset parser flags since we're done parsing current token
  return Results_;                      // return result after token is determined valid.
}

/****************************************************************************/
ParseResultInfo* Parser::QuickParse_Main(ParseResultInfo* Results_, QuickParseStatus* Status_, char* TagName_, char* Text_, char* SyntaxStr_)
{
  char* Token_ = NULL;

  Results_->GetNext()->SetTokenType(SYMBOL);
  #if QUICKINT_QUICKPARSE_DEBUG1
    ShowParseState(Status_, TagName_, 1, 1, Results_);
  #endif      
  
  Results_->InsertToken(TagName_);
  if (!SyntaxStr_)
    SyntaxStr_ = FindDefinition(TagName_);
  
  if (!GrabToken(Results_->GetCurrent()->SetNext(), Text_, SyntaxStr_, Status_, Token_, 1))
    Results_->GetCurrent()->SetValidity(0, NumberSign::PLUS);

  #if QUICKINT_QUICKPARSE_DEBUG1
    ShowParseState(Status_, TagName_, 0, 1, Results_);
  #endif

  Status_->DeepestLevel++;
  return Results_;
}

/****************************************************************************/
char* Parser::FindDefinition(char* Token_)
{
  int Low_ = 0;
  int High_ = _NumSymbols;
  int Mid_;  
  int Result_;

  while (Low_ <= High_ && (Result_ = strcmp(Token_, _ParseSymbols[Mid_=(Low_+High_)/2]->Symbol)))
  {    
    if (Result_ < 0)
      High_ = Mid_ - 1;
    else if (Result_ > 0)
      Low_ = Mid_ + 1;
  }

  if (Result_)
    return NULL;

  return _ParseSymbols[Mid_]->Definition;
}

/****************************************************************************/
char* Parser::GroupToken(char* Token_, char* StartBracket_, char* EndBracket_)
{
  if (Token_)
    return(
      SafeStrCat(
        SafeStrCat(
          SafeStrCpy(
            new_char_array(SafeStrLen(Token_) + 3, NULL),
            StartBracket_
          ),
          Token_
        ),
        EndBracket_
      )
    );

  return NULL;
}

/****************************************************************************/
void Parser::DefineSymbol(char* Symbol_, char* Definition_)
{
  if (_NumSymbols < _MaxParseSymbols)
  {
    if (!_NumSymbols)
    {
      _ParseSymbols[0] = new ParseSymbolType;
      _ParseSymbols[0]->DefineSymbol(Symbol_, Definition_);
      _NumSymbols++;          
    }
    else
    {
      int Pos_ = _NumSymbols - 1;
      int Result_ = 0;
      int Found_ = 0;
      
      while (Pos_ > 0 && (Result_ = strcmp(Symbol_, _ParseSymbols[Pos_]->Symbol)) < 0)
        Pos_--;

      if (Pos_ > 0)
      {
        if (Result_ > 0)
          Pos_++;
        else
          Found_ = 1;
      }
      else if (Pos_ == 0)
      {
        Result_ = strcmp(Symbol_, _ParseSymbols[0]->Symbol);
        
        if (Result_ < 0)
          Pos_ = 0;
        else if (Result_ > 0)
          Pos_ = 1;
        else
          Found_ = 1;
      }

      if (!Found_)
      {
        memmove(&_ParseSymbols[Pos_+1], &_ParseSymbols[Pos_], (_NumSymbols - Pos_) * sizeof(ParseSymbolType*));
        _ParseSymbols[Pos_] = new ParseSymbolType;
        _ParseSymbols[Pos_]->DefineSymbol(Symbol_, Definition_);
        _NumSymbols++;
      }
    }    
  }
  else
  {
    GrowSymbolTable();
    DefineSymbol(Symbol_, Definition_);
  }
}

/****************************************************************************/
void Parser::GrowSymbolTable()
{
  ParseSymbolType** OldTable_ = _ParseSymbols;
  int OldMax_ = _MaxParseSymbols;
  int i = 0;
  
  _MaxParseSymbols += MAX_PARSE_SYMBOLS;
  _ParseSymbols = (ParseSymbolType**)RawAllocateWith(MEMMATRIX, sizeof(ParseSymbolType*) * _MaxParseSymbols);

  for (;i < OldMax_; i++)
    _ParseSymbols[i] = OldTable_[i];

  for (;i < _MaxParseSymbols; i++)
    _ParseSymbols[i] = NULL;

  ::DeleteArray(OldTable_);
}

/****************************************************************************/
char* Parser::GetResultCodeString(ParseResultInfo* Results_, char* Buffer_, int Level_, int SpecLevel_)
{
  int i, Max, x;
  char TempStr_[64];

  if (Level_ < SpecLevel_ && Level_)
    SafeStrCat(Buffer_, "-");  

  for (i = 0, Max = Results_->Index(); i < Max; i++)
  {
    if (Level_ == SpecLevel_)
    {
      sprintf(&TempStr_[0], "%d", Level_);
      SafeStrCat(TempStr_, ":");
      x = SafeStrLen(TempStr_);
      TempStr_[x++] = Results_->Tokens[i]->GiveType()[0];
      TempStr_[x++] = ':';
      TempStr_[x] = 0;
      x = SafeStrLen(TempStr_);
      sprintf(&TempStr_[x], "%s", Results_->Tokens[i]->Validity.ToTruncString().c_str());
      SafeStrCat(Buffer_, TempStr_);
    }
    
    if (Results_->Tokens[i]->Next)
    {
      int IsOpt_ = 0;
      int IsRep_ = 0;
      int IsGrp_ = 0;
      int IsSym_ = 0;
      
      if (Results_->Tokens[i]->TokenType == Parser::OPTION)
        IsOpt_ = 1;
      else if (Results_->Tokens[i]->TokenType == Parser::REPEATABLE)
        IsRep_ = 1;
      else if (Results_->Tokens[i]->TokenType == Parser::GROUP)
        IsGrp_ = 1;
      else if (Results_->Tokens[i]->TokenType == Parser::SYMBOL)
        IsSym_ = 1;

      if (IsOpt_)
        SafeStrCat(Buffer_, "[");
      else if (IsGrp_)
        SafeStrCat(Buffer_, "(");        
      else if (IsRep_)
        SafeStrCat(Buffer_, "{");
      else if (IsSym_)
        SafeStrCat(Buffer_, "<");

      if (Level_ < SpecLevel_)
        GetResultCodeString(Results_->Tokens[i]->Next, Buffer_, Level_ + 1, SpecLevel_);
      else
        SafeStrCat(Buffer_, "+");

      if (IsOpt_)
        SafeStrCat(Buffer_, "]");
      else if (IsGrp_)
        SafeStrCat(Buffer_, ")");
      else if (IsRep_)
        SafeStrCat(Buffer_, "}");
      else if (IsSym_)
        SafeStrCat(Buffer_, ">");
    }

    if (Level_ == SpecLevel_)
    {
      if (i < Max - 1)
        SafeStrCat(Buffer_, ",");
    }
  }

  if (Results_->OrBranch)
  {
    SafeStrCat(Buffer_, "|");
    GetResultCodeString(Results_->OrBranch, Buffer_, Level_, SpecLevel_);
  }

  return Buffer_;
}

/****************************************************************************/
void Parser::CopyParseSymbols(const Parser& Obj_)
{
  int x;
  for (x = 0; x < Obj_._NumSymbols; x++)
    DefineSymbol(Obj_._ParseSymbols[x]->Symbol, Obj_._ParseSymbols[x]->Definition);
}

/****************************************************************************/
void Parser::ReadDefinitionFile(const char* FileName_)
{
  size_t Len_, Max_;
  int LineBufSize_ = 512;
  char *Ptr_, *SymPtr_, *DefBuf_;  
  char* LineBuf_ = new_char_array(LineBufSize_, NULL);
  ifstream Fin_;

  PATHTOKENIZER(Pathtok_, "PATH");
  Max_ = Pathtok_.Max();
  size_t i;

  char* AbsPath_ = new_char_array(300 + strlen(FileName_), FileName_);    
  Fin_.open(AbsPath_, ios::in);
  
  for (i = 0; !Fin_.good() && i < Max_; i++)
  {
    strcpy(AbsPath_, Pathtok_[i]);
    strcat(AbsPath_, Pathtok_.DirSeparator());
    strcat(AbsPath_, FileName_);
    Fin_.clear();
    Fin_.open(AbsPath_, ios::in);
  }
  
  Max_ = 512;

  if (Fin_.good())
  {
    DefBuf_ = new_char_array(Max_, NULL);
  
    while (!Fin_.eof())
    {
      Ptr_ = LineBuf_;
      DefBuf_[0] = 0;
      Len_ = 0;
      
      for (;;)
      {        
        LineBuf_ = ReadStringUntil(Fin_, LineBuf_, LineBufSize_, '\n');
        Ptr_ = LineBuf_;
        Ptr_ = TrimSpaces(Ptr_);

        if (!strlen(Ptr_) || strncmp(Ptr_, "//", 2) == 0)
        {
          if (Fin_.eof())
            break;
        
          Ptr_ = LineBuf_;
          continue;
        }

        Len_ += strlen(Ptr_);

        if (Len_ >= Max_)
        {
          while (Len_ >= Max_)
            Max_ += 512;
            
          DefBuf_ = ResizeString(DefBuf_, Max_);
        }

        strcat(DefBuf_, Ptr_);

        if (Ptr_[strlen(Ptr_) - 1] == ';')
        {
          DefBuf_[strlen(DefBuf_) - 1] = 0;
          break;
        }
        else
          Ptr_ = LineBuf_;

        if (Fin_.eof())
          break;                    
      }

      if (Ptr_ = strstr(DefBuf_, "::="))
      {
        --Ptr_;
        *Ptr_ = 0;
        Ptr_ += 4;
        
        SymPtr_ = DefBuf_;        
        SymPtr_ = TrimSpaces(SymPtr_);
        Ptr_ = TrimSpaces(Ptr_);        
        
        DefineSymbol(SymPtr_, Ptr_);        
      }
    }

    ::DeleteArray(DefBuf_);
  }

  ::DeleteArray(AbsPath_);
  ::DeleteArray(LineBuf_);
  Fin_.close();
}

/****************************************************************************/
void Parser::SetError(int Code_, int LineNum_)
{
  _ParseError = Code_;

  if (LineNum_)
    _LineNumber = LineNum_;
}

/****************************************************************************/
void Parser::SetStringQuotes(const char* StartQuote_, const char* EndQuote_)
{
  if (_StartQuote && _EndQuote)
  {
    *_StartQuote = *StartQuote_;
    *_EndQuote = *EndQuote_;
  }
}

/****************************************************************************/
void Parser::UseBrackets(Boolean Flag_)
{
  if (_StartQuote && _EndQuote)
  {
    *_StartQuote = Flag_ ? '[':'\"';
    *_EndQuote = Flag_ ? ']':'\"';
  }
}

/****************************************************************************/
char* Parser::HasChar(const char* Str_, const char* Filter_)
{
  size_t flen_ = SafeStrLen(Filter_);
  size_t index_;
  char* pos_ = NULL;

  for (index_ = 0; index_ < flen_; ++index_)
    if (pos_ = strchr(Str_, Filter_[index_]))
      break;

  return pos_;
}

/****************************************************************************/
char* Parser::ReplaceChar(char* Str_, const char* Trg_, const char* Rep_)
{
  char* pos_;
  
  if (pos_ = HasChar(Str_, Trg_))
    *pos_ = *Rep_;

  return Str_;
}

/****************************************************************************/
void Parser::ResetVariables()
{
  _TerminatorFound = FALSE;
  _SemiTerminatorCnt = 0;
  _ReadLine =
  _WorkLine = NULL;
}

/****************************************************************************/
void Parser::ShowLineData()
{
#if QUICKINT_DEBUG
  if (_ParseError)
    HandleError();
  else
  {
#if QUICKINT_NICE_OUTPUT
    (*_Fout1) <<setw(3) <<_Codes[SCODE]
              <<setw(5) <<_Codes[TCODE]
              <<setw(3) <<_Codes[UCODE]
              <<setw(5) <<_Codes[CCODE]
              <<setw(3) <<_Codes[ICODE]
              <<setw(3) <<_Codes[VCODE]
              <<setw(3) <<_Codes[FCODE]
              <<setw(3) <<_Codes[ACODE]
              <<setw(3) <<_Codes[MCODE]
              <<setw(3) <<_Codes[PCODE]
              <<setw(3) <<_Codes[ECODE]
              <<setw(3) <<_Codes[GCODE]
              <<setw(6) <<"S :" <<_Selected
              <<setw(6) <<"C :" <<_Checked
              <<setw(6) <<"M :" <<_MaxItems
              <<setw(6) <<"Z :" <<_Size
              <<setw(6) <<"T :" <<_TerminatorFound
              <<endl;
    (*_Fout2) <<setw(12) <<"L :" <<(_Label     ? _Label:" ")
              <<setw(12) <<"FT:" <<(_FromTable ? _FromTable:" ")
              <<setw(12) <<"FF:" <<(_FromField ? _FromField:" ")
              <<setw(12) <<"TF:" <<(_TextFormat ? _TextFormat:" ")
              <<setw(12) <<"UT:" <<(_UrlTable ? _UrlTable:" ")
              <<setw(15) <<"UF:" <<(_UrlField ? _UrlField:" ")
              <<setw(15) <<"US:" <<(_UrlString ? _UrlString:" ")
              <<setw(15) <<"IS:" <<(_ImageSrcString  ? _ImageSrcString:" ")
              <<setw(15) <<"IA:" <<(_ImageAltString  ? _ImageAltString:" ")
              <<setw(15) <<"IST:" <<(_ImageSrcTable  ? _ImageSrcTable:" ")
              <<setw(15) <<"IAT:" <<(_ImageAltTable  ? _ImageAltTable:" ")
              <<setw(15) <<"ISF:" <<(_ImageSrcField  ? _ImageSrcField:" ")
              <<setw(15) <<"IAF:" <<(_ImageAltField  ? _ImageAltField:" ")
              <<setw(15) <<"AS:" <<(_AnchorString  ? _AnchorString:" ")
              <<setw(15) <<"AT:" <<(_AnchorTable  ? _AnchorTable:" ")
              <<setw(15) <<"AF:" <<(_AnchorField  ? _AnchorField:" ")
              <<setw(12) <<"TN:" <<(_TableName ? _TableName:" ")
              <<setw(30) <<"SC:" <<(_SqlCommand ? _SqlCommand:" ")
              <<setw(12) <<"FN:" <<(_FieldName ? _FieldName:" ")
              <<setw(12) <<"NL:" <<(_NumLength ? _NumLength:" ")
              <<setw(12) <<"ND:" <<(_NumDecimals ? _NumDecimals:" ")
              <<endl;
#else
    (*_Fout1) <<_Codes[SCODE]
              <<"\t" <<_Codes[TCODE]
              <<"\t" <<_Codes[UCODE]
              <<"\t" <<_Codes[CCODE]
              <<"\t" <<_Codes[ICODE]
              <<"\t" <<_Codes[VCODE]
              <<"\t" <<_Codes[FCODE]
              <<"\t" <<_Codes[ACODE]
              <<"\t" <<_Codes[MCODE]
              <<"\t" <<_Codes[PCODE]
              <<"\t" <<_Codes[ECODE]
              <<"\t" <<_Codes[GCODE]
              <<"\t" <<"S :" <<_Selected
              <<"\t" <<"C :" <<_Checked
              <<"\t" <<"M :" <<_MaxItems
              <<"\t" <<"Z :" <<_Size
              <<"\t" <<"T :" <<_TerminatorFound
              <<endl;
    (*_Fout2) <<"L :" <<(_Label     ? _Label:" ")
              <<"\t" <<"FT:" <<(_FromTable ? _FromTable:" ")
              <<"\t" <<"FF:" <<(_FromField ? _FromField:" ")
              <<"\t" <<"TF:" <<(_TextFormat ? _TextFormat:" ")
              <<"\t" <<"UT:" <<(_UrlTable ? _UrlTable:" ")
              <<"\t" <<"UF:" <<(_UrlField ? _UrlField:" ")
              <<"\t" <<"US:" <<(_UrlString ? _UrlString:" ")
              <<"\t" <<"IS:" <<(_ImageSrcString  ? _ImageSrcString:" ")
              <<"\t" <<"IA:" <<(_ImageAltString  ? _ImageAltString:" ")
              <<"\t" <<"IST:" <<(_ImageSrcTable  ? _ImageSrcTable:" ")
              <<"\t" <<"IAT:" <<(_ImageAltTable  ? _ImageAltTable:" ")
              <<"\t" <<"ISF:" <<(_ImageSrcField  ? _ImageSrcField:" ")
              <<"\t" <<"IAF:" <<(_ImageAltField  ? _ImageAltField:" ")
              <<"\t" <<"AS:" <<(_AnchorString  ? _AnchorString:" ")
              <<"\t" <<"AT:" <<(_AnchorTable  ? _AnchorTable:" ")
              <<"\t" <<"AF:" <<(_AnchorField  ? _AnchorField:" ")
              <<"\t" <<"TN:" <<(_TableName ? _TableName:" ")
              <<"\t" <<"SC:" <<(_SqlCommand ? _SqlCommand:" ")
              <<"\t" <<"FN:" <<(_FieldName ? _FieldName:" ")
              <<"\t" <<"NL:" <<(_NumLength ? _NumLength:" ")
              <<"\t" <<"ND:" <<(_NumDecimals ? _NumDecimals:" ")
              <<endl;
#endif           
  }
#endif

  ResetVariables();
}

/****************************************************************************/
long Parser::ParserSupply(int Create_, long Index_, Parser* Ptr_)
{
  static long ParserNum_ = 0;
  InitParser();
  Parser* ParserPtr_;

  if (Create_ && ParserNum_ < 255)
  {
    ParserNum_++;
    ParserPtr_ = new Parser(ParserNum_);
  }
  else if (ParserNum_)
  {  
    ParserPtr_ = Ptr_ ? Ptr_:_ParserObjects[Index_];
    ParserPtr_->DeleteParserObject(ParserNum_);
    ParserNum_--;
  }

  return ParserNum_;
}

/****************************************************************************/
Parser* Parser::IndexParser(long Index_)
{
  return _ParserObjects[Index_];
}

/****************************************************************************/
long Parser::InitParser()
{
  static Parser DummyInitObj_;
  return !DummyInitObj_.ParseError();
}

/****************************************************************************/
void Parser::DestroyParser(long Index_, Parser* Ptr_)
{
  ParserSupply(0, Index_, Ptr_);
}

/****************************************************************************/
long Parser::MakeParser()
{
  return ParserSupply(1, 0, NULL);
}

/****************************************************************************/
MEMORYOPS_DEFN(TokenInfo)
MEMORYOPS_DEFN(FieldInfo)
MEMORYOPS_DEFN(ParseSymbolType)
MEMORYOPS_DEFN(ParseTokenType)
MEMORYOPS_DEFN(ParseResultInfo)
MEMORYOPS_DEFN(QuickParseStatus)
MEMORYOPS_DEFN(Parser)

/****************************************************************************/
/****************************************************************************/
long quickint_Initialize()
{
  return Parser::InitParser();
}

/****************************************************************************/
long quickint_GetParser()
{
  return Parser::MakeParser();
}

/****************************************************************************/
void quickint_EraseParser(long Index_)
{
  Parser::DestroyParser(Index_, NULL);
}

/****************************************************************************/
void quickint_ShowLineData(long Index_)
{
  Parser* Ptr_ = Parser::IndexParser(Index_);
  Ptr_->ShowLineData();
}

/****************************************************************************/
int quickint_ParseError(long Index_)
{
  Parser* Ptr_ = Parser::IndexParser(Index_);
  return Ptr_->ParseError();
}

/****************************************************************************/
int quickint_LineNumber(long Index_)
{
  Parser* Ptr_ = Parser::IndexParser(Index_);
  return Ptr_->LineNumber();
}

/****************************************************************************/
void quickint_UseBrackets(long Index_, Boolean Flag_)
{
  Parser* Ptr_ = Parser::IndexParser(Index_);
  Ptr_->UseBrackets(Flag_);
}

/****************************************************************************/
void quickint_DefineSymbol(long Index_, char* Symbol_, char* Definition_)
{
  Parser* Ptr_ = Parser::IndexParser(Index_);
  Ptr_->DefineSymbol(Symbol_, Definition_);
}

/****************************************************************************/
char* quickint_FindDefinition(long Index_, char* Token_)
{
  Parser* Ptr_ = Parser::IndexParser(Index_);
  return Ptr_->FindDefinition(Token_);
}

/****************************************************************************/





