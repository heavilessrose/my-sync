#ifndef KEYREAD_CPP
#define KEYREAD_CPP
#ifndef KEYREAD_H
  #include "keyread.h"
#endif

//**************************************************************************//
//**************************************************************************//
//			    Keyboard Reader Class			    //
//**************************************************************************//
//**************************************************************************//
short KeyboardReader::_Instances = 0;

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Get extended keystroke BIOS service (INT 16h, service 10h).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 10h
//
// POST:
//   scan        : Scan code to write
//   ascii       : ASCII code to write
//
//   registers:
//       AH      : Keyboard scan code
//       AL      : ASCII value of keystroke
//**************************************************************************//
Word_t KeyboardReader::ReadKeyboard(Byte_t& ScanCode_, Byte_t& AsciiCode_)
{
  REGS InRegs_, OutRegs_;
  SREGS SegRegs_;

  InRegs_.h.ah = 0;

  int86x(0x16, &InRegs_, &OutRegs_, &SegRegs_);

  ScanCode_ = OutRegs_.h.ah;
  AsciiCode_ = OutRegs_.h.al;

  return OutRegs_.x.ax;
}

/****************************************************************************/
Word_t KeyboardReader::GetKey()
{
      Word_t i;

      switch (i = (Word_t)getch())
      {
      case 0xe0:
#ifdef MSDOS
	    return i;
#endif
      case 0:
	    return 256 + (Word_t)getch();
      default:
	    return i;
      }
}

/****************************************************************************/
const KeyboardReader& KeyboardReader::Controller()
{
  static KeyboardReader Reader_;
  return Reader_;
}

/****************************************************************************/
const ControllerCode KeyboardReader::ReadScan() const
{
  return ReadControllerCode();
}

/****************************************************************************/
void KeyboardReader::InitController()
{
  InitControllerCode();
}

/****************************************************************************/
void KeyboardReader::ExitController()
{
  ExitControllerCode();
}

/****************************************************************************/
KeyboardReader::KeyboardReader()
{
  if (!_Instances)
    InitController();

  ++_Instances;
}

/****************************************************************************/
KeyboardReader::~KeyboardReader()
{
  --_Instances;

  if (_Instances == 0)
  {
    ExitController();
    _Instances = 0;
  }
}

/****************************************************************************/
//**************************************************************************//
//			    Keyboard Writer Class			    //
//**************************************************************************//
/****************************************************************************/
void KeyboardWriter::PrintCntrCodes(ostream& Out_, const ControllerCode& CntrCode_)
{
  char Delimeter_[] = "(";

  for (short Index_ = 0; Index_ < CntrCode_.MAXLENGTH; Index_++)
  {
    if (CntrCode_._KeyArray[Index_])
    {
      Out_ <<Delimeter_ <<Index_;
      *Delimeter_ = ' ';
    }

    if (CntrCode_._ExtKeyArray[Index_])
    {
      Out_ <<Delimeter_ <<EXTBYTE <<"/" <<Index_;
      *Delimeter_ = ' ';
    }
  }
  Out_ <<")";
}

/****************************************************************************/
void KeyboardWriter::PrintCntrKeys(ostream& Out_, const ControllerCode& CntrCode_)
{
  for (short Index_ = 0; Index_ < CntrCode_.MAXLENGTH; Index_++)
  {
    if (CntrCode_._KeyArray[Index_])
      Out_ <<CNTRTABLE(Index_).LABEL <<" ";

    if (CntrCode_._ExtKeyArray[Index_])
      Out_ <<CNTRTABLE(SETHIGHBYTE(Index_, EXTBYTE)).LABEL <<" ";
  }
}

/****************************************************************************/
// PURPOSE:
//   Method to show information on the key that was pressed which includes:
//
//	- The ASCII code
//	- The Scan code
//	- And the description of the key in plain text
//
// PRE:
//   Byte_t ScanCode_	: The scan code of the key
//   Byte_t AsciiCode_	: The ASCII code of the key
//
// POST:
//   The scan and ASCII code of the key is displayed.
//   A text description of the key is also displayed.
//
void KeyboardWriter::ShowKey(ostream& Out_,
			     const ControllerCode& CntrCode_,
			     Byte_t ScanCode_, Byte_t AsciiCode_)
{
  short Ypos_, Index_;

  Ypos_ = wherey();
  gotoxy(1, Ypos_);

  clreol();
  Out_ <<"<CAPS LOCK " <<(GetBiosShiftState()->caps_on ? "ON":"OFF") <<">" <<endl;
  clreol();
  Out_ <<"<NUM LOCK " <<(GetBiosShiftState()->num_on ? "ON":"OFF") <<">" <<endl;
  clreol();
  Out_ <<"<SCROLL LOCK " <<(GetBiosShiftState()->scroll_on ? "ON":"OFF") <<">" <<endl;
  clreol();
  Out_ <<"<" <<(GetBiosShiftState()->ins_on ? "INSERT":"OVERWRITE") <<" MODE>" <<endl;

  clreol();
  Out_ <<"ASCII Code = " <<short(AsciiCode_) <<endl;
  clreol();
  Out_ <<"Scan Code  = " <<short(ScanCode_) <<" ";
  PrintCntrCodes(Out_, CntrCode_); Out_ <<endl;

  clreol();
  Out_ <<"BIOS Keyboard Keystroke  = " <<CODETABLE(AsciiCode_, ScanCode_).LABEL <<endl;
  clreol();
  Out_ <<"Keyboard Controller Keys = ";
  PrintCntrKeys(Out_, CntrCode_); Out_ <<endl;
  clreol();

  gotoxy(1, Ypos_);
}

/****************************************************************************/
// PURPOSE:
//   Method to read a keystroke from the keyboard, interpret the key and then
//   show information for that key.
//
// POST:
//   For every keystroke read from the keyboard information on that key is
//   displayed. This process continues until two consecutive ESC keystrokes
//   are detected.
//
void KeyboardWriter::ReadKey(ostream& Out_)
{
  short EscFlag_, Ypos_;
  Byte_t ScanCode_;
  Byte_t AsciiCode_;
  ControllerCode CntrCode_;

  Out_ <<endl <<"Keyboard Reader"
       <<endl <<"---------------"
       <<endl;
  Out_ <<(IsExtKeyboard() ? "Enhanced Keyboard supported":
			    "Enhanced Keyboard NOT supported") <<endl;

  Ypos_ = wherey();
  Out_ <<"<CAPS LOCK " <<(GetBiosShiftState()->caps_on ? "ON":"OFF") <<">" <<endl
       <<"<NUM LOCK " <<(GetBiosShiftState()->num_on ? "ON":"OFF") <<">" <<endl
       <<"<SCROLL LOCK " <<(GetBiosShiftState()->scroll_on ? "ON":"OFF") <<">" <<endl
       <<"<" <<(GetBiosShiftState()->ins_on ? "INSERT":"OVERWRITE") <<" MODE>" <<endl;
  Out_ <<"ASCII Code = " <<endl
       <<"Scan Code  = " <<endl
       <<"BIOS Keyboard Keystroke  = " <<endl
       <<"Keyboard Controller Keys = <Enter A Key>" <<endl;
  gotoxy(1, Ypos_);

  _setcursortype(_NOCURSOR);

  for (EscFlag_ = 0;;)
  {
    CntrCode_ = Controller().ReadScan();
    ReadKeyboard(ScanCode_, AsciiCode_);

    if (ScanCode_ == CODETABLE(ESC).SCAN &&
	AsciiCode_ == CODETABLE(ESC).ASCII)
      if (!EscFlag_)
	++EscFlag_;
      else
	break;
    else
      EscFlag_ = 0;

    ShowKey(Out_, CntrCode_, ScanCode_, AsciiCode_);
  }

  _setcursortype(_NORMALCURSOR);
}

/****************************************************************************/
void KeyboardWriter::ShowBiosShiftState()
{
	while (1)
	{
	    gotoxy(1, 1);
	    printf("right_shift_down = %d\n",GetBiosShiftState()->right_shift_down);
	    printf("left_shift_down = %d\n",GetBiosShiftState()->left_shift_down);
	    printf("ctrl_shift_down = %d\n",GetBiosShiftState()->ctrl_shift_down);
	    printf("alt_shift_down = %d\n",GetBiosShiftState()->alt_shift_down);
	    printf("scroll_on = %d\n",GetBiosShiftState()->scroll_on);
	    printf("num_on = %d\n",GetBiosShiftState()->num_on);
	    printf("caps_on = %d\n",GetBiosShiftState()->caps_on);
	    printf("ins_on = %d\n",GetBiosShiftState()->ins_on);
	    printf("filler = %d\n",GetBiosShiftState()->filler);
	    printf("ctrl_numloc = %d\n",GetBiosShiftState()->ctrl_numloc);
	    printf("scroll_down = %d\n",GetBiosShiftState()->scroll_down);
	    printf("num_down = %d\n",GetBiosShiftState()->num_down);
	    printf("caps_down = %d\n",GetBiosShiftState()->caps_down);
	    printf("ins_down = %d\n",GetBiosShiftState()->ins_down);
	    printf("\nPress F10 To Break\n");

	    if (Controller().ReadScan()._KeyArray[KeyboardCodes::SCANCODE(KeyboardCodes::CNTR_F10)])
	      break;
	}
}

/****************************************************************************/
#if KEYREAD_DEBUG
void main()
{
#if KEYREAD_DEBUG1
  clrscr();
  KeyboardWriter::ReadKey();
  clrscr();
#endif

#if KEYREAD_DEBUG2
  clrscr();
  cout <<"Set CapsLock" <<endl; getch();
  KeyboardCodes::SetCapsLock();
  cout <<"Clr CapsLock" <<endl; getch();
  KeyboardCodes::ClrCapsLock();

  cout <<"Set NumLock" <<endl; getch();
  KeyboardCodes::SetNumLock();
  cout <<"Clr NumLock" <<endl; getch();
  KeyboardCodes::ClrNumLock();
#endif

#if KEYREAD_DEBUG3
  KeyboardWriter::ShowBiosShiftState();
  clrscr();
#endif
}
#endif
/****************************************************************************/
#endif