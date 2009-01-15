// Node common definition file
//
#ifndef LISTDEFS_H
#define LISTDEFS_H
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif
/****************************************************************************/
struct ListRelation
{
  public:
    enum
    {
      HEAD,
      PREVIOUS,
      THIS,
      NEXT,
      TAIL
    };
};

/****************************************************************************/
#endif






