#include <iostream.h>
#include <conio.h>
#include <dos.h>

#include "bioslib.h"

void main()
{
    char ch;
    BiosLib_t Bios_;
    Byte_t ScanCode_, AsciiCode_, Shift_;

    Bios_.ReadExtKeyboardChar(ScanCode_, AsciiCode_);

    cout <<(int)ScanCode_ <<"\t" <<AsciiCode_ <<endl;
    cin >>ch;

/*
    for (;;)
    {
      Bios_.ReadExtKeyboardStatus(ScanCode_, AsciiCode_);
      Shift_ = Bios_.ReadExtKeyboardShift();

      cout <<Shift_ <<"\t"
	   <<ScanCode_ <<"\t"
	   <<AsciiCode_ <<endl;

      if (ScanCode_ == 0 && AsciiCode_ == 27)
	break;
    }
*/
}
