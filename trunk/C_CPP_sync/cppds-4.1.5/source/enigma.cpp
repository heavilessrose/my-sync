/****************************************************************************/
/*		          String Encryption Module
**
** Contains procedures which encrypts strings using shuffling and carrier
** string methods.
*/
/****************************************************************************/
#ifndef ENIGMA_C
#define ENIGMA_C
#ifndef ENIGMA_H
  #include "enigma.h"
#endif

static char simple_enctable[MAXSIMPLE + 1] =
{
   76,  49,  75,  51,  74,  68,  86,  54,  71,  67,
   83,  57,  81,  66,  50,  80,  56,  90,  87,  84,
   85,  48,  79,  52,  72,  69,  88,  65,  89,  70,
   53,  77,  78,  55,  82,  73,
};

static char simple_datatable[MAXSIMPLE + 1] =
{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z'
};

static char encryption_table[MAXENCCHAR + 1] =
{
   76, 119,  49, 103, 116,  75,  46, 112,  60,  51,
   74,  68, 102,  86,  96,  32, 125,  62,  54, 123,
   58,  61, 101,  71, 107,  39, 113,  67, 118,  83,
   94, 121,  57, 106, 108,  44,  81,  33,  63,  66,
   50, 111,  80,  95,  40, 110,  56,  90, 124,  38,
  114, 120,  64,  87, 122,  35,  41,  84,  85,  48,
   79,  59,  52, 100,  72, 104,  69,  88,  43,  65,
   34,  89,  36,  93,  37, 105,  70,  53,  45,  42,
   77,  97,  91, 115,  47,  92, 109, 117,  98,  99,
   78, 126,  55,  82,  73, 0
};

static char dataset_table[MAXENCCHAR + 1] =
{
   32,  33,  34,  35,  36,  37,  38,  39,  40,  41,
   42,  43,  44,  45,  46,  47,  48,  49,  50,  51,
   52,  53,  54,  55,  56,  57,  58,  59,  60,  61,
   62,  63,  64,  65,  66,  67,  68,  69,  70,  71,
   72,  73,  74,  75,  76,  77,  78,  79,  80,  81,
   82,  83,  84,  85,  86,  87,  88,  89,  90,  91,
   92,  93,  94,  95,  96,  97,  98,  99, 100, 101,
  102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
  112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
  122, 123, 124, 125, 126, 0
};

/****************************************************************************/
char* EnigmaCrypt::ConvertWhiteSpace(char* hold)
{
  size_t len = strlen(hold);
  size_t x;

  for (x = 0; x < len; x++)
    if (isspace(hold[x]))
      hold[x] = SPACE_CHAR;

  return hold;
}

/****************************************************************************/
char* EnigmaCrypt::Shuffle(char* substr1, char* substr2)
{
  size_t index = 0;
  static char* result = NULL;

  free(result);
  result = (char*)calloc(strlen(substr1) + strlen(substr2) + 1, sizeof(char));

  if (!result)
    return NULL;

  while (*substr1 && *substr2)
  {
    result[index++] = *substr1++;
    result[index++] = *substr2++;
  }

  if (result[index - 1] == EXTRA_CHAR)
    --index;
  result[index] = '\0';

  return result;
}

/****************************************************************************/
char* EnigmaCrypt::Zap(char* str)
{
  size_t len, odd = 0;
  static char* substr1 = NULL;
  static char* substr2 = NULL;

  len = strlen(str);
  if (!str || len == 0)
    return str;

  odd = len % 2 != 0;
  if (odd)
    ++len;

  len /= 2;

  free(substr1);
  free(substr2);
  substr1 = (char*)calloc(len + 1, sizeof(char));
  substr2 = (char*)calloc(len + 1, sizeof(char));
  
  if (!substr1 || !substr2)
    return NULL;

  strncpy(substr1, str, len);
  strcpy(substr2, str + len);
  
  if (odd)
  {
    substr2[strlen(substr2)+1] = '\0';
    substr2[strlen(substr2)] = EXTRA_CHAR;
  }

  return Shuffle(substr1, substr2);
}

/****************************************************************************/
char* EnigmaCrypt::UnZap(char* str)
{
  size_t index = 0;
  char* substr1 = str;
  char* substr2 = str + 1;
  static char* result = NULL;

  if (!str || strlen(str) == 0)
    return str;

  free(result);
  result = (char*)calloc(strlen(str) + 1, sizeof(char));

  if (!result)
    return NULL;

  if (*substr1)
    for (;;)
    {
      result[index++] = *substr1;
      if (strlen(substr1) <= 2)
        break;
      substr1 += 2;
    }

  if (*substr2)
    for (;;)
    {
      result[index++] = *substr2;
      if (strlen(substr2) <= 2)
        break;
      substr2 += 2;
    }

  result[index] = '\0';
  return result;
}

/****************************************************************************/
size_t EnigmaCrypt::FindDatasetIndex(char Ch_)
{
  char* Pos_ = strchr(_DatasetTable, Ch_);
  
  if (Pos_)
    return size_t(Pos_ - _DatasetTable);

  return 0;
}

/****************************************************************************/
int EnigmaCrypt::InDataset(char ch)
{
  if (!strchr(_DatasetTable, ch))
    return 0;

  return 1;
}

/****************************************************************************/
int EnigmaCrypt::InDataset(char* hold)
{
  size_t len = strlen(hold);
  size_t i;

  for (i = 0; i < len; i++)
    if (!strchr(_DatasetTable, hold[i]))
      return 0;

  return 1;
}

/****************************************************************************/
void EnigmaCrypt::FilterTableHelper(char* SrceTable_,
                                    char* DestTable_)
{
  int Valid_[20];
  size_t i, x, y, len;
  int InitialVal_;
  
  len = strlen(SrceTable_);
  InitialVal_ = (_Filter & POS_FILTER) ? 0:1;
  
  for (y = 0; y < 10; y++)
    Valid_[y] = InitialVal_;
  for (y = 10; y < 20; y++)
    Valid_[y] = 1;

  if (_Filter)
  {
    for (x = i = 0; i < len; i++)
    {
      if (_Filter & ALPHANUM & ~POS_FILTER)
        Valid_[0] = isalnum(SrceTable_[i]);
      else if (_Filter & NOT_ALPHANUM & ~NEG_FILTER)
        Valid_[10] = !isalnum(SrceTable_[i]);

      if (_Filter & ALPHA & ~POS_FILTER)
        Valid_[1] = isalpha(SrceTable_[i]);
      else if (_Filter & NOT_ALPHA & ~NEG_FILTER)
        Valid_[11] = !isalpha(SrceTable_[i]);

      if (_Filter & DIGIT & ~POS_FILTER)
        Valid_[2] = isdigit(SrceTable_[i]);
      else if (_Filter & NOT_DIGIT & ~NEG_FILTER)
        Valid_[12] = !isdigit(SrceTable_[i]);

      if (_Filter & LOWER & ~POS_FILTER)
        Valid_[3] = islower(SrceTable_[i]);
      else if (_Filter & NOT_LOWER & ~NEG_FILTER)
        Valid_[13] = !islower(SrceTable_[i]);

      if (_Filter & UPPER & ~POS_FILTER)
        Valid_[4] = isupper(SrceTable_[i]);
      else if (_Filter & NOT_UPPER & ~NEG_FILTER)
        Valid_[14] = !isupper(SrceTable_[i]);

      if (_Filter & GRAPH & ~POS_FILTER)
        Valid_[5] = isgraph(SrceTable_[i]);
      else if (_Filter & NOT_GRAPH & ~NEG_FILTER)
        Valid_[15] = !isgraph(SrceTable_[i]);

      if (_Filter & PRINT & ~POS_FILTER)
        Valid_[6] = isprint(SrceTable_[i]);
      else if (_Filter & NOT_PRINT & ~NEG_FILTER)
        Valid_[16] = !isprint(SrceTable_[i]);

      if (_Filter & PUNCT & ~POS_FILTER)
        Valid_[7] = ispunct(SrceTable_[i]);
      else if (_Filter & NOT_PUNCT & ~NEG_FILTER)
        Valid_[17] = !ispunct(SrceTable_[i]);

      if (_Filter & SPACE & ~POS_FILTER)
        Valid_[8] = isspace(SrceTable_[i]) && (SrceTable_[i] == SPACE_CHAR);
      else if (_Filter & NOT_SPACE & ~NEG_FILTER)
        Valid_[18] = !isspace(SrceTable_[i]);

      Valid_[9] = Valid_[0] || Valid_[1] || Valid_[2] || Valid_[3] ||
                  Valid_[4] || Valid_[5] || Valid_[6] || Valid_[7] || Valid_[8];
      Valid_[19] = Valid_[10] && Valid_[11] && Valid_[12] && Valid_[13] &&
                   Valid_[14] && Valid_[15] && Valid_[16] && Valid_[17] && Valid_[18];

      if (Valid_[9] && Valid_[19])
        DestTable_[x++] = SrceTable_[i];

      for (y = 0; y < 10; y++)
        Valid_[y] = InitialVal_;
      for (y = 10; y < 20; y++)
        Valid_[y] = 1;
    }
  }
  else
  {
    x = strlen(SrceTable_);
    memcpy(DestTable_, SrceTable_,
           strlen(SrceTable_) + 1);
  }

  DestTable_[x] = 0;
}

/****************************************************************************/
void EnigmaCrypt::FilterTable()
{
  FilterTableHelper(encryption_table, _EncryptionTable);
  FilterTableHelper(dataset_table, _DatasetTable);
  _MaxEncChar = strlen(_EncryptionTable);
}

/****************************************************************************/
char* EnigmaCrypt::MakeSimpleTable(unsigned seed)
{
  size_t index, cindex, tindex, found;
  char ch;
  _Seed = seed;

  if (_RandomizerType == ANSI)
    srand(_Seed);
  else
    _Randomizer.SetSeed(_Seed);

  strcpy(_EncryptionTable, simple_enctable);
  strcpy(_DatasetTable, simple_datatable);
  _MaxEncChar = strlen(_EncryptionTable);    

  for (index = 0; index < _MaxEncChar; index++)
  {
    do
    {
      if (_RandomizerType == ANSI)    
        cindex = rand() % _MaxEncChar;
      else
        cindex = _Randomizer.RAND() % _MaxEncChar;
      
      ch = _DatasetTable[cindex];
      found = 0;

      for (tindex = 0; tindex < index && !found; tindex++)
        found = _EncryptionTable[tindex] == ch;
    }
    while (found);
    
    _EncryptionTable[index] = ch;
  }

  _EnctablePtr = _EncryptionTable;
  return _EnctablePtr;
}

/****************************************************************************/
char* EnigmaCrypt::MakeSimpleTable()
{
  return MakeSimpleTable(time(NULL));
}

/****************************************************************************/
char* EnigmaCrypt::MakeTable(unsigned seed)
{
  size_t index, cindex, tindex, found;
  char ch;  
  _Seed = seed;

  if (_RandomizerType == ANSI)
    srand(_Seed);
  else
    _Randomizer.SetSeed(_Seed);
    
  FilterTable();

  for (index = 0; index < _MaxEncChar; index++)
  {
    do
    {
      if (_RandomizerType == ANSI)
        cindex = rand() % _MaxEncChar;
      else
        cindex = _Randomizer.RAND() % _MaxEncChar;
        
      ch = _DatasetTable[cindex];
      found = 0;

      for (tindex = 0; tindex < index && !found; tindex++)
        found = _EncryptionTable[tindex] == ch;
    }
    while (found);
    
    _EncryptionTable[index] = ch;
  }

  _EnctablePtr = _EncryptionTable;
  return _EnctablePtr;
}

/****************************************************************************/
char* EnigmaCrypt::MakeTable()
{
  return MakeTable(time(NULL));
}

/****************************************************************************/
void EnigmaCrypt::SetTable(char* tp)
{
  _EnctablePtr = tp;
}

/****************************************************************************/
char* EnigmaCrypt::GetTable(void)
{
  return _EnctablePtr;
}

/****************************************************************************/
char* EnigmaCrypt::Encrypt_String(char* str, int rot)
{
  char* tabptr;
  char* indexptr;
  unsigned ch;
  size_t sindex, tindex;
  size_t len = strlen(str);
  int OneToOne_ = GetOneToOneEncode();

  rot %= _MaxEncChar;
  tabptr = (rot >= 0) ?
  (_EnctablePtr + rot) : (&_EnctablePtr[strlen(_EnctablePtr)-1] + rot + 1);

  if (!OneToOne_ || InDataset(str))
    for (sindex = 0; sindex < len; sindex++)
    {
      ch = (unsigned)str[sindex];
      indexptr = tabptr;      
      
      if (OneToOne_ || InDataset(str[sindex]))
      {
        tindex = FindDatasetIndex(ch);

        if (tindex >= strlen((char*)indexptr))
        {
          tindex -= strlen((char*)indexptr);
          indexptr = _EnctablePtr;
          tindex %= strlen((char*)indexptr);
        }
      
        str[sindex] = indexptr[tindex];
      }
    }

  return str;
}

/****************************************************************************/
char* EnigmaCrypt::Decrypt_String(char* str, int rot)
{
  char* tabptr;
  char* indexptr;
  size_t sindex, tindex, tabpos;
  size_t len = strlen(str);
  int OneToOne_ = GetOneToOneEncode();

  rot %= _MaxEncChar;
  tabptr = (rot >= 0) ?
  (_EnctablePtr + rot) : (&_EnctablePtr[strlen(_EnctablePtr)-1] + rot + 1);

  if (!OneToOne_ || InDataset(str))  
    for (sindex = 0; sindex < len; sindex++)
    {
      if (OneToOne_ || InDataset(str[sindex]))
        for (tabpos = 0; tabpos < _MaxEncChar; tabpos++)
        {
          indexptr = tabptr;
          tindex = tabpos;

          if (tindex >= strlen((char*)indexptr))
          {
            tindex -= strlen((char*)indexptr);
            indexptr = _EnctablePtr;
            tindex %= strlen((char*)indexptr);
          }
        
          if (str[sindex] == indexptr[tindex])
          {
            str[sindex] = _DatasetTable[tabpos];
            break;
          }
        }
    }

  return str;
}

/****************************************************************************/
char* EnigmaCrypt::Encrypt(char* hold, int rot)
{
  if (!GetOneToOneEncode() || InDataset(hold))
  {
      _EnctablePtr = _EncryptionTable;
      strcpy(hold, Zap(hold));
      if (rot % 2)
        _EnctablePtr = Zap(_EnctablePtr);
      strcpy(hold, Encrypt_String(hold, rot));
  }

  return hold;
}

/****************************************************************************/
char* EnigmaCrypt::Decrypt(char* hold, int rot)
{
  if (!GetOneToOneEncode() || InDataset(hold))
  {
      _EnctablePtr = _EncryptionTable;
      strcpy(hold, UnZap(hold));
      if (rot % 2)
        _EnctablePtr = Zap(_EnctablePtr);
      strcpy(hold, Decrypt_String(hold, rot));
  }

  return hold;
}

/****************************************************************************/
void EnigmaCrypt::SetFilter(int Attribs_)
{
  _Filter = Attribs_;

  if (_SimpleDataset)
  {
    if (_Seed)
      MakeSimpleTable(_Seed);
    else
      MakeSimpleTable();
  }
  else
  {
    if (_Seed)
      MakeTable(_Seed);
    else
      MakeTable();
  }
}

/****************************************************************************/
void EnigmaCrypt::AppendFilter(int Attribs_)
{
  SetFilter(_Filter | Attribs_);
}

/****************************************************************************/    
void EnigmaCrypt::ClearFilter()
{
  _Filter = 0;

  if (_SimpleDataset)
  {
    if (_Seed)
      MakeSimpleTable(_Seed);
    else
      MakeSimpleTable();  
  }
  else
  {
    if (_Seed)
      MakeTable(_Seed);
    else
      MakeTable();
  }
}

/****************************************************************************/    
void EnigmaCrypt::ClearFilter(int Attribs_)
{
  _Filter &= ~Attribs_;

  if (_SimpleDataset)
  {
    if (_Seed)
      MakeSimpleTable(_Seed);
    else
      MakeSimpleTable();  
  }
  else
  {
    if (_Seed)
      MakeTable(_Seed);
    else
      MakeTable();
  }
}

/****************************************************************************/    
int EnigmaCrypt::GetFilter() const
{
  return _Filter;
}

/****************************************************************************/
void EnigmaCrypt::SetRandomizerType(int Type_)
{
  _RandomizerType = Type_;

  if (_SimpleDataset)
  {
    if (_Seed)
      MakeSimpleTable(_Seed);
    else
      MakeSimpleTable();  
  }
  else
  {
    if (_Seed)
      MakeTable(_Seed);
    else
      MakeTable();
  }
}

/****************************************************************************/
int EnigmaCrypt::GetRandomizerType() const
{
  return _RandomizerType;
}

/****************************************************************************/
void EnigmaCrypt::SetSimpleDataset(int Flag_)
{
  _SimpleDataset = Flag_;
  _OneToOneEncode = !Flag_;

  if (_SimpleDataset)
  {
    if (_Seed)
      MakeSimpleTable(_Seed);
    else
      MakeSimpleTable();  
  }
  else
  {
    if (_Seed)
      MakeTable(_Seed);
    else
      MakeTable();
  }  
}

/****************************************************************************/
int EnigmaCrypt::GetSimpleDataset() const
{
  return _SimpleDataset;  
}

/****************************************************************************/
void EnigmaCrypt::SetOneToOneEncode(int Flag_)
{
  int TurnOff_ = _OneToOneEncode == 1 && !Flag_;
  _OneToOneEncode = Flag_;
  
  if (TurnOff_)
  {
    if (_SimpleDataset)
    {
      if (_Seed)
        MakeSimpleTable(_Seed);
      else
        MakeSimpleTable();
    }
    else
    {
      if (_Seed)
        MakeTable(_Seed);
      else
        MakeTable();
    }
  }
}

/****************************************************************************/
int EnigmaCrypt::GetOneToOneEncode() const
{
  return _OneToOneEncode;
}

/****************************************************************************/
EnigmaCrypt::EnigmaCrypt():
_RandomizerType(LEHMER),
_MaxEncChar(MAXENCCHAR),
_Seed(0),
_Filter(0),
_SimpleDataset(0),
_OneToOneEncode(1)
{
  MakeTable(time(NULL));
}

/****************************************************************************/
EnigmaCrypt::EnigmaCrypt(unsigned seed, int Attribs_):
_RandomizerType(LEHMER),
_MaxEncChar(MAXENCCHAR),
_Seed(0),
_Filter(Attribs_),
_SimpleDataset(Attribs_ == SIMPLE),
_OneToOneEncode(Attribs_ != SIMPLE)
{
  if (_SimpleDataset)
  {
    if (seed)
      MakeSimpleTable(seed);
    else
      MakeSimpleTable();
  }
  else
  {
    if (seed)
      MakeTable(seed);
    else
      MakeTable();
  }
}

/****************************************************************************/
#if DEBUG_ENIGMA
#if defined(__TURBOC__) | defined(__DJGPP__)
  #include <conio.h>
#endif
int main()
{
  char hold[128];
  char original[128] =
  {"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890 "
   "`!@#$%^&*()-=_+[]{}\\|;:\'\",<.>/?"};
  char original_ns[128] =
  {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
   "`!@#$%^&*()-=_+[]{}\\|;:\'\",<.>/?"};
  char original_an[128] =
  {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};
  char original_ud[128] =
  {"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};
  char original_p[128] =
  {"`!@#$%^&*()-=_+[]{}\\|;:\'\",<.>/?"};
  char original_a[128] =
  {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  char original_d[128] =
  {"1234567890"};
  

#if defined(__TURBOC__) | defined(__DJGPP__)
  clrscr();
#else
  system("clear");
#endif

  EnigmaCrypt Crypto_;
  Crypto_.SetRandomizerType(EnigmaCrypt::ANSI);

  strcpy(hold, original);
  printf("original = %s\n", hold);

  strcpy(hold, Crypto_.Encrypt(hold, -10));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, -10));
  printf("decrypt  = %s\n", hold);


  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, -15));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, -15));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, -20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, -20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, 10));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 10));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, 15));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 15));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.MakeTable();
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original));

  Crypto_.SetFilter(EnigmaCrypt::NOT_SPACE);
  printf("\n*** space character excluded ***\n", hold);

  strcpy(hold, original);
  printf("incorrect data = %s\n", hold);
  
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, original_ns);
  printf("original = %s\n", hold);

  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);  

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original_ns));

  Crypto_.SetFilter(EnigmaCrypt::ALPHANUM);
  printf("\n*** alphanumeric characters only ***\n", hold);

  strcpy(hold, original);
  printf("incorrect data = %s\n", hold);
  
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);  
  
  strcpy(hold, original_an);
  printf("original = %s\n", hold);
  
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original_an));

//  Crypto_.SetFilter(EnigmaCrypt::UPPER | EnigmaCrypt::DIGIT);
  Crypto_.SetSimpleDataset(1);  
  printf("\n*** upper case characters and digits only ***\n", hold);

  strcpy(hold, original_ud);
  printf("original = %s\n", hold);
  
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original_ud));

  Crypto_.SetSimpleDataset(0);
  Crypto_.SetFilter(EnigmaCrypt::ALPHA);
  printf("\n*** letters only ***\n", hold);
  
  strcpy(hold, original_a);
  printf("original = %s\n", hold);
  
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original_a));

  Crypto_.SetFilter(EnigmaCrypt::DIGIT);
  printf("\n*** digits only ***\n", hold);
  
  strcpy(hold, original_d);
  printf("original = %s\n", hold);
  
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original_d));

  Crypto_.SetFilter(EnigmaCrypt::PUNCT | EnigmaCrypt::NOT_SPACE);
  printf("\n*** punctuation characters only ***\n", hold);
  
  strcpy(hold, original_p);
  printf("original = %s\n", hold);
  
  strcpy(hold, Crypto_.Encrypt(hold, 20));
  printf("encrypt  = %s\n", hold);

  strcpy(hold, Crypto_.Decrypt(hold, 20));
  printf("decrypt  = %s\n", hold);
  assert(!strcmp(hold, original_p));

  return 0;
}
#endif

/****************************************************************************/
#endif
