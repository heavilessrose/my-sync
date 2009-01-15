#include "tristate.h"

Tristate& Tristate::Default()
{
  static Tristate _State(0, 0);

  _State.v = Indeterminate;
  return _State;
}

Tristate& Tristate::operator = (const Tristate& Ts_)
{
  if (this != &Ts_ && !IsDefault())
    v = Ts_.v;

  return *this;
}

int icmpstr(const char* s1, const char* s2)
{
  if (!s1 || !s2)
    return (s1 ? 1:
	    s2 ? -1:0);

  int diff;
  for (diff;
       !(diff = tolower(*s1) - tolower(*s2)) && *s1 && *s2;
       s1++, s2++);

  return diff;
}

ostream& operator << (ostream& s, Tristate b)
{
  s << b.ToString();
  return s;
}

istream& operator >> (istream& s, Tristate& b)
{
  const size_t bufsize = 1 + sizeof("INDETERMINATE");
  char inbuf[bufsize];

  s.width(int(bufsize));     // Set maximum # of characters to be read
  s >> inbuf;		     // Read character string (also resets width)

  if (icmpstr(inbuf, "TRUE") == 0)
    b = Tristate::True;
  else if (icmpstr(inbuf, "FALSE") == 0)
    b = Tristate::False;
  else if (icmpstr(inbuf, "INDETERMINATE") == 0)
    b = Tristate::Indeterminate;
  else
    s.clear(ios::failbit);    // Indicate extraction failed

  return s;
}

Tristate operator == (const Tristate Val1_, const Tristate Val2_)
{
  return (int(Val1_) == int(Val2_));
}

#if TRISTATE_DEBUG
void Function1(Tristate& Ts_ = Tristate::Default())
{
  if ((int)Ts_ == Tristate::Indeterminate)
    cout <<"Indeterminate State" <<endl;

  cout <<"STATE : " <<Ts_.ToString() <<endl;
}

void main()
{
  Tristate Ts1_ = Tristate::True;

  Function1();
  Function1(Ts1_);
}
#endif





