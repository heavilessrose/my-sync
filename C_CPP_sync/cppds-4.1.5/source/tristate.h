#ifndef TRISTATE_H
#define TRISTATE_H
#ifndef PORTABLE_H
  #include "portable.h"
#endif

#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif

#define TRISTATE_DEBUG	        0

static int icmpstr(const char* s1, const char* s2);

class Tristate
{
  private:
    const char _Def;
    char v;

    Tristate(int, int):
	_Def(1),
	v(Indeterminate) {}

  public:
    // Constants
    enum constants { Indeterminate = -1, False = 0, True = 1 };

    // Construction.
    Tristate():
	_Def(0),
	v(Indeterminate) {}   // Construct uninitialized.
    Tristate(const Tristate& Ts_):
	_Def(0),
	v(Ts_.v) {}
    Tristate(int i):
	_Def(0),
	v(i < 0 ? Indeterminate:
	  i > 0 ? True:False) {}   // Construct and initialize to (i != 0).

    static Tristate& Default();

    static Tristate TristateDouble(double d)
	{ return Tristate(int(d)); }   // Construct and initialize to (d != 0).

    // Conversion.
    operator int() const
	{ return v; }

    Tristate& operator = (const Tristate& Ts_);

    static const char* ToString(const Tristate& Val_)
	{ return Val_.ToString(); }
    const char* ToString() const
	{ return ((v < 0) ? "INDETERMINATE":
		  (v > 0) ? "TRUE":"FALSE"); }

    // Negation.
    Tristate operator ! () const
	{ return ((v > 0) ? False:
		  (v < 0) ? Indeterminate:True); }

    // Is default object?
    int IsDefault() const
	{ return _Def; }
};

// I/O
ostream& operator << (ostream& s, Tristate  b);
istream& operator >> (istream& s, Tristate& b);

// Equality
Tristate operator == (const Tristate Val1_, const Tristate Val2_);

#endif




