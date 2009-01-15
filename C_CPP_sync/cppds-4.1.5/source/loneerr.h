#ifndef LONEERR_H
#define LONEERR_H

class ostream;

class LoneErr
{
  private:
    ostream& _Stream;

  public:
    LoneErr():
	_Stream(cerr) {}
    LoneErr(ostream& os_):
	_Stream(os_) {}

    ostream& Stream() const
	{ return _Stream; }
    virtual const char* message() const = 0;
    virtual ~LoneErr();
};

ostream& operator << (ostream& s, const LoneErr& e);

#endif





