/*
modified
Example error class from the book Scientific and Engineering Programming
in C++: An Introduction with Advanced Techniques and Examples,
*/
#ifndef SCIERR_H
#define SCIERR_H

class ChrString;
class ostream;

class SciEngErr
{
  private:
    ostream& _Stream;

  public:
    SciEngErr():
	_Stream(cerr) {}
    SciEngErr(ostream& os_):
	_Stream(os_) {}

    ostream& Stream() const
	{ return _Stream; }

    virtual ChrString message() const = 0;
    virtual ~SciEngErr();
};

ostream& operator << (ostream& s, const SciEngErr& e);

#endif





