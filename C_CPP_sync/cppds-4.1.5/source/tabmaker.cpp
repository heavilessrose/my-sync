#include <ctype.h>
#include <iostream.h>
#define MAXENCCHAR 95

static unsigned char encryption_table[MAXENCCHAR + 1] =
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

int main()
{
  int i, x, len;
  len = strlen((char*)encryption_table);
  unsigned char new_table[MAXENCCHAR + 1];
  int new_max = 0;

  for (x = i = 0; i < len; i++)
  {
    if (isdigit(encryption_table[i]) ||
        isalpha(encryption_table[i]))
      new_table[x++] = encryption_table[i];
    if (i && i % 10 == 0)
      cout <<endl;
  }

  new_max = x;
  cout <<"table_alphanum = {\n";
  for (i = 0; i < new_max; i++)
  {
    cout <<((int)encryption_table[i]) <<((i == new_max - 1) ? " ":", ");
    if (i && i % 10 == 0)
      cout <<endl;    
  }
  cout <<"};" <<endl <<endl;

  return 0;
}
