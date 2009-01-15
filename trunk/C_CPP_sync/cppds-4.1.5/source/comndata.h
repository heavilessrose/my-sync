/*======================================================================

                 Copyright (c) 1992 Namir Clement Shammas

    VERSION:  2.0.0                                     DATE 5/2/92

    PURPOSE: defines common data types.

    UPDATE HISTORY:

======================================================================*/

#ifndef COMNDATA_H
#define COMNDATA_H

enum answerType { no, yes, maybe };
enum arrayorder { no_order, in_order, in_reverse };

const unsigned NOT_FOUND = 0xffff;
#define ALLOCATE_ERROR "Dynamic allocation error"

typedef unsigned char byte;
typedef unsigned int word;
typedef double real;
typedef char string2[3];
typedef char string3[4];
typedef char string4[5];
typedef char string8[9];
typedef char string10[11];
typedef char string12[13];
typedef char string40[41];
typedef char string80[81];
typedef char string255[256];
typedef char string1K[1025];

#endif





