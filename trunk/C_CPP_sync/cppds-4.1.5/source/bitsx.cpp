#ifndef BITSX_H
  #include "bitsx.h"
#endif

/****************************************************************************/
short _Ipfx(istream& Is_, int noskip)
{
  if (Is_.good())
  {
    if (Is_.tie() != 0)
      Is_.tie()->flush();

    if (!noskip && Is_.flags() & ios::skipws)
    {
      int ch;
      while (isspace(ch = Is_.rdbuf()->sbumpc()))
	;
      if (ch != EOF)
	Is_.rdbuf()->sputbackc(ch);
    }

    if (Is_.good())
      return  1;
  }

  Is_.clear(ios::failbit);
  return 0;
}

/****************************************************************************/
short _Ipfx(istream& Is_)
{
  return _Ipfx(Is_, 0);
}

/****************************************************************************/
ChrString _Bitsxstr(istream& Is_, Subscript Num_)
{
  Boolean Changed_ = FALSE;
  ChrString Str_('\0', Num_);

  if (Num_ == PTRDIFFT_MAX)
    --Num_;

  if (_Ipfx(Is_))
  {
    int ch;
    while (0 < Num_ && (ch = Is_.rdbuf()->sbumpc()) != EOF)
      if (ch != '0' && ch != '1')
      {
	Is_.rdbuf()->sputbackc(ch);
	break;
      }
      else
      {
	Str_ += ch;
	Changed_ = TRUE;
	--Num_;
      }
  }

  if (!Changed_)
    Is_.clear(ios::failbit);

  if (!_Ipfx(Is_))
    THROW (FallibleBase::IOFailureErr());

  return Str_;
}

/****************************************************************************/




