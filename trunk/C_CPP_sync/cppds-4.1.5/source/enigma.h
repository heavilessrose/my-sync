/****************************************************************************/
/*		          String Encryption Module
**
** Contains procedures which encrypts strings using shuffling and carrier
** string methods.
*/
/****************************************************************************/
#ifndef ENIGMA_H
#define ENIGMA_H

#ifndef INCL_STDIO_H
  #include <stdio.h>
  #define INCL_STDIO_H
#endif
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef INCL_TIME_H
  #include <time.h>
  #define INCL_TYPE_H
#endif
#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERT_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif

#ifndef RANDOMIZER_H
  #include "randomizer.h"
#endif

/****************************************************************************/
#define DEBUG_ENIGMA	  0

#define MAXSIMPLE      36
#define MAXENCCHAR	  95
#define EXTRA_CHAR	  '\1'
#define SPACE_CHAR     32

/****************************************************************************/
// Enigma string encryptor class
class EnigmaCrypt
{
  protected:
    Randomizer<long double> _Randomizer;
    
    int _RandomizerType;
    size_t _MaxEncChar;
    unsigned _Seed;
    int _Filter;
    int _SimpleDataset;
    int _OneToOneEncode;

    char _DatasetTable[MAXENCCHAR + 1];
    char _EncryptionTable[MAXENCCHAR + 1];
    char* _EnctablePtr;

    void SetTable(char* tp);
    void FilterTable();
    void FilterTableHelper(char* SrceTable_, char* DestTable_);
    size_t FindDatasetIndex(char Ch_);
    int InDataset(char* hold);
    int InDataset(char ch);

    char* Zap(char* str);
    char* UnZap(char* str);
    static char* Shuffle(char* substr1, char* substr2);
    static char* ConvertWhiteSpace(char* hold);    

    char* Encrypt_String(char* str, int rot);
    char* Decrypt_String(char* str, int rot);

  public:
    enum
    {
      ALPHANUM       = 0x00001001,
      ALPHA          = 0x00001002,
      DIGIT          = 0x00001004,
      LOWER          = 0x00001008,
      UPPER          = 0x00001010,
      GRAPH          = 0x00001020,
      PRINT          = 0x00001040,
      PUNCT          = 0x00001080,
      SPACE          = 0x00001100,

      NOT_ALPHANUM   = 0x10010000,
      NOT_ALPHA      = 0x10020000,
      NOT_DIGIT      = 0x10040000,
      NOT_LOWER      = 0x10080000,
      NOT_UPPER      = 0x10100000,
      NOT_GRAPH      = 0x10200000,
      NOT_PRINT      = 0x10400000,
      NOT_PUNCT      = 0x10800000,
      NOT_SPACE      = 0x11000000,

      NEG_FILTER     = 0x10000000,
      POS_FILTER     = 0x00001000
    };

    enum
    {
      LEHMER = 0,
      ANSI   = 1,
      
      SIMPLE = 2
    };
  
    EnigmaCrypt();
    EnigmaCrypt(unsigned seed, int Attribs_);

    void SetOneToOneEncode(int Flag_);
    int GetOneToOneEncode() const;
    void SetSimpleDataset(int Flag_);
    int GetSimpleDataset() const;
    void SetRandomizerType(int Type_);
    int GetRandomizerType() const;    
    void SetFilter(int Attribs_);
    void AppendFilter(int Attribs_);
    void ClearFilter(int Attribs_);
    void ClearFilter();
    int GetFilter() const;

    char* MakeSimpleTable(unsigned seed);
    char* MakeSimpleTable();
    char* MakeTable(unsigned seed);
    char* MakeTable();
    char* GetTable();

    char* Encrypt(char* hold, int rot);
    char* Decrypt(char* hold, int rot);
};

/****************************************************************************/
#endif





