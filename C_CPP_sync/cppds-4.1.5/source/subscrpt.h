#ifndef SUBSCRPT_H
#define SUBSCRPT_H

#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif

typedef unsigned short int Dimension;
typedef ptrdiff_t          Subscript;  // Signed, machine-dependent int; from <stddef.h>
typedef Subscript          SizeType;

#endif




