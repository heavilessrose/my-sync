/*
Example programs from the book Scientific and Engineering Programming
in C++: An Introduction with Advanced Techniques and Examples,
Addison-Wesley, 1994.

		 (c) COPYRIGHT INTERNATIONAL BUSINESS MACHINES
		 CORPORATION 1994.  ALL RIGHTS RESERVED.

See README file for further details.
*/
#include "boolean.h"

int icmpstr(const char* s1, const char* s2)
{
  if (!s1 || !s2)
    return (s1 ? 1:
	    s2 ? -1:0);

  int diff;
  for (diff = 0;
       !(diff = tolower(*s1) - tolower(*s2)) && *s1 && *s2;
       s1++, s2++);

  return diff;
}

ostream& operator<<(ostream& s, BoolClass b)
{
  return s << (b ? "TRUE" : "FALSE");
}

istream& operator>>(istream& s, BoolClass& b)
{
  const size_t bufsize = 1 + sizeof("FALSE");
  char inbuf[bufsize];

  s.width(int(bufsize));     // Set maximum # of characters to be read
  s >> inbuf;		     // Read character string (also resets width)

  if (icmpstr(inbuf, "TRUE") == 0)
    b = BoolClass::True;
  else if (icmpstr(inbuf, "FALSE") == 0)
    b = BoolClass::False;
  else
    s.clear(ios::failbit);    // Indicate extraction failed

  return s;
}

BoolClass operator == (const BoolClass Val1_, const BoolClass Val2_)
{
  return (int(Val1_) == int(Val2_));
}




