#ifndef CHRSTRING_H
  #include "chrstring.h"
#endif

// Definition of number of bits in a byte
#define BITS_BYTE CHAR_BIT

// Input formatting extension
short _Ipfx(istream& Is_, int noskip);
short _Ipfx(istream& Is_);

// Function to extract into a string of text bits
ChrString _Bitsxstr(istream& Is_, Subscript Num_);




