#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef SHORTINT_H
  #include "shortint.h"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

/****************************************************************************/
void main()
{
  ShortInt* Obj1_ = new ShortInt(10);
  ShortInt* Obj2_ = new ShortInt(20);
  MemPool* Pool1_ = new MemPool(sizeof(ShortInt));

  char* String_ = (char*)MemMatrix::Matrix.Allocate(sizeof(char) * 10);
  strcpy(String_, "Joseph");
  cout <<String_ <<endl;

  delete Obj1_;
  delete Obj2_;

  String_ = (char*)MemMatrix::Matrix.Reallocate(String_, sizeof(char) * 22);
  strcat(String_, "    , Wong");
  cout <<String_ <<endl;

  delete Pool1_;
  MemMatrix::Matrix.Deallocate(String_);
}

/****************************************************************************/
