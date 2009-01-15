#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#ifndef DSDEFS_H
  #include "dsdefs.h"
#endif

/****************************************************************************/
// Timestamp class (page 186)
class TimeStamp
{
  private:
    static size_t _LastTime; // Used to assign a number
    size_t _Stamp;

  public:
    TimeStamp();

    Boolean operator > (TimeStamp Ts_) const
	{ return _Stamp > Ts_._Stamp; }
    Boolean operator >= (TimeStamp Ts_) const
	{ return _Stamp >= Ts_._Stamp; }
    Boolean operator < (TimeStamp Ts_) const
	{ return _Stamp < Ts_._Stamp; }
    Boolean operator <= (TimeStamp Ts_) const
	{ return _Stamp <= Ts_._Stamp; }
    Boolean operator == (TimeStamp Ts_) const
	{ return _Stamp == Ts_._Stamp; }
    Boolean operator != (TimeStamp Ts_) const
	{ return _Stamp != Ts_._Stamp; }

    size_t Stamp() const
	{ return _Stamp; }
};

/****************************************************************************/
#endif





