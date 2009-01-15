#ifndef BOOLEAN_H
#define BOOLEAN_H
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

// Boolean constants
#if !defined(TRUE)
  #define TRUE		!(0)
#endif
#if !defined(FALSE)
  #define FALSE		0
#endif

static int icmpstr(const char* s1, const char* s2);

class BoolClass
{
  private:
    char v;

  public:
    // Constants
    enum constants { False = 0, True = 1 };

    // Construction.
    BoolClass()                      {}   // Construct uninitialized.
    BoolClass(int i) :    v(i != 0)  {}   // Construct and initialize to (i != 0).

    static BoolClass BooleanFloat(float f)
	{ return BoolClass(f != 0); }   // Construct and initialize to (f != 0).
    static BoolClass BooleanDouble(double d)
	{ return BoolClass(d != 0); }   // Construct and initialize to (d != 0).
    static BoolClass BooleanVoid(void* p)
	{ return BoolClass(p != 0); }   // Construct and initialize to (p != 0).

    // Conversion.
    operator int() const
	{ return v; }   // To allow "if (boolean-value)..."

    static const char* ToString(const BoolClass& Val_)
	{ return Val_.ToString(); }
    const char* ToString() const
	{ return ((v != 0) ? "TRUE":"FALSE"); }

    // Negation.
    BoolClass operator!() const
	{ return !v; }
};

// I/O
ostream& operator << (ostream& s, BoolClass  b);
istream& operator >> (istream& s, BoolClass& b);

// Equality
BoolClass operator == (const BoolClass Val1_, const BoolClass Val2_);

//typedef bool BOOLTYPE;
//typedef BoolClass BOOLTYPE;
typedef unsigned BOOLTYPE;

//typedef bool Boolean;
//typedef BoolClass Boolean;
typedef unsigned Boolean;

#endif




