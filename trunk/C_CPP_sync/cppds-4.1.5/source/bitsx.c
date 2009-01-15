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
