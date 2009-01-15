#ifndef KEYCODES_CPP
#define KEYCODES_CPP
#ifndef KEYCODES_H
  #include "keycodes.h"
#endif
/****************************************************************************/
// Keyboard code table initialization
const KeyboardCodes::KeyTable KeyboardCodes::_CodeTable =
{
	{0, 0, "BREAK (CTRL-PAUSE)"},
	{27, 1, "ESC"},
	{33, 2, "! (SHIFT-1)"},
	{64, 3, "@ (SHIFT-2)"},
	{0, 3, "CTRL-SHIFT-2"},
	{35, 4, "# (SHIFT-3)"},
	{36, 5, "$ (SHIFT-4)"},
	{37, 6, "% (SHIFT-5)"},
	{94, 7, "^ (SHIFT-6)"},
	{30, 7, "CTRL-SHIFT-6"},
	{38, 8, "& (SHIFT-7)"},
	{42, 9, "* (SHIFT-8)"},
	{40, 10, "( (SHIFT-9)"},
	{41, 11, ") (SHIFT-0)"},
	{45, 12, "-"},
	{95, 12, "_ (SHIFT-MINUS)"},
	{31, 12, "CTRL-SHIFT-MINUS"},
	{61, 13, "="},
	{43, 13, "+ (SHIFT-=)"},
	{8, 14, "BACKSPACE"},
	{127, 14, "CTRL-BACKSPACE"},
	{9, 15, "TAB"},
	{0, 15, "SHIFT-TAB"},
	{0, 16, "ALT-Q"},
	{17, 16, "CTRL-Q"},
	{0, 17, "ALT-W"},
	{23, 17, "CTRL-W"},
	{0, 18, "ALT-E"},
	{5, 18, "CTRL-E"},
	{0, 19, "ALT-R"},
	{18, 19, "CTRL-R"},
	{0, 20, "ALT-T"},
	{20, 20, "CTRL-T"},
	{0, 21, "ALT-Y"},
	{25, 21, "CTRL-Y"},
	{0, 22, "ALT-U"},
	{21, 22, "CTRL-U"},
	{0, 23, "ALT-I"},
	{9, 23, "CTRL-I"},
	{0, 24, "ALT-O"},
	{15, 24, "CTRL-O"},
	{0, 25, "ALT-P"},
	{16, 25, "CTRL-P"},
	{91, 26, "["},
	{123, 26, "{ (SHIFT-[)"},
	{27, 26, "CTRL-["},
	{93, 27, "]"},
	{125, 27, "} (SHIFT-])"},
	{29, 27, "CTRL-]"},
	{13, 28, "ENTER"},
	{0, 30, "ALT-A"},
	{1, 30, "CTRL-A"},
	{0, 31, "ALT-S"},
	{19, 31, "CTRL-S"},
	{0, 32, "ALT-D"},
	{4, 32, "CTRL-D"},
	{0, 33, "ALT-F"},
	{6, 33, "CTRL-F"},
	{0, 34, "ALT-G"},
	{7, 34, "CTRL-G"},
	{0, 35, "ALT-H"},
	{8, 35, "CTRL-H"},
	{0, 36, "ALT-J"},
	{10, 36, "CTRL-J"},
	{0, 37, "ALT-K"},
	{11, 37, "CTRL-K"},
	{0, 38, "ALT-L"},
	{12, 38, "CTRL-L"},
	{59, 39, ";"},
	{58, 39, ": (SHIFT-;)"},
	{39, 40, "\'"},
	{34, 40, "\" (SHIFT-\')"},
	{96, 41, "`"},
	{126, 41, "~ (SHIFT-`)"},
	{92, 43, "\\"},
	{124, 43, "| (SHIFT-\\)"},
	{28, 43, "CTRL-\\"},
	{0, 44, "ALT-Z"},
	{26, 44, "CTRL-Z"},
	{0, 45, "ALT-X"},
	{24, 45, "CTRL-X"},
	{0, 46, "ALT-C"},
	{3, 46, "CTRL-C"},
	{0, 47, "ALT-V"},
	{22, 47, "CTRL-V"},
	{0, 48, "ALT-B"},
	{2, 48, "CTRL-B"},
	{0, 49, "ALT-N"},
	{14, 49, "CTRL-N"},
	{0, 50, "ALT-M"},
	{13, 50, "CTRL-M"},
	{44, 51, ","},
	{60, 51, "< (SHIFT-,)"},
	{46, 52, "."},
	{62, 52, "> (SHIFT-.)"},
	{47, 53, "/"},
	{63, 53, "? (SHIFT-/)"},
	{42, 55, "* (KEYPAD)"},
	{0, 55, "ALT-PAUSE"},
	{0, 56, "ALT-BREAK (ALT-CTRL-PAUSE)"},
	{32, 57, "SPACE"},
	{0, 58, "CAPS-LOCK"},
	{0, 59, "F1"},
	{0, 60, "F2"},
	{0, 61, "F3"},
	{0, 62, "F4"},
	{0, 63, "F5"},
	{0, 64, "F6"},
	{0, 65, "F7"},
	{0, 66, "F8"},
	{0, 67, "F9"},
	{0, 68, "F10"},
	{0, 69, "NUM-LOCK"},
	{0, 70, "SCROLL-LOCK"},
	{0, 71, "HOME"},
	{55, 71, "7 (KEYPAD)"},
	{0, 72, "UP ARROW"},
	{56, 72, "8 (KEYPAD)"},
	{0, 73, "PAGE UP"},
	{57, 73, "9 (KEYPAD)"},
	{45, 74, "- (KEYPAD)"},
	{0, 75, "LEFT ARROW"},
	{52, 75, "4 (KEYPAD)"},
	{0, 76, "CENTERKEY"},
	{53, 76, "5 (KEYPAD)"},
	{0, 77, "RIGHT ARROW"},
	{54, 77, "6 (KEYPAD)"},
	{43, 78, "+ (KEYPAD)"},
	{0, 79, "END"},
	{49, 79, "1 (KEYPAD)"},
	{0, 80, "DOWN ARROW"},
	{50, 80, "2 (KEYPAD)"},
	{0, 81, "PAGE DOWN"},
	{51, 81, "3 (KEYPAD)"},
	{0, 82, "INSERT"},
	{48, 82, "0 (KEYPAD)"},
	{0, 83, "DELETE"},
	{46, 83, ". (KEYPAD)"},
	{0, 84, "SHIFT-F1"},
	{0, 85, "SHIFT-F2"},
	{0, 86, "SHIFT-F3"},
	{0, 87, "SHIFT-F4"},
	{0, 88, "SHIFT-F5"},
	{0, 89, "SHIFT-F6"},
	{0, 90, "SHIFT-F7"},
	{0, 91, "SHIFT-F8"},
	{0, 92, "SHIFT-F9"},
	{0, 93, "SHIFT-F10"},
	{0, 94, "CTRL-F1"},
	{0, 95, "CTRL-F2"},
	{0, 96, "CTRL-F3"},
	{0, 97, "CTRL-F4"},
	{0, 98, "CTRL-F5"},
	{0, 99, "CTRL-F6"},
	{0, 100, "CTRL-F7"},
	{0, 101, "CTRL-F8"},
	{0, 102, "CTRL-F9"},
	{0, 103, "CTRL-F10"},
	{0, 104, "ALT-F1"},
	{0, 105, "ALT-F2"},
	{0, 106, "ALT-F3"},
	{0, 107, "ALT-F4"},
	{0, 108, "ALT-F5"},
	{0, 109, "ALT-F6"},
	{0, 110, "ALT-F7"},
	{0, 111, "ALT-F8"},
	{0, 112, "ALT-F9"},
	{0, 113, "ALT-F10"},
	{0, 114, "CTRL-PRTSC"},
	{0, 115, "CTRL-LEFT ARROW"},
	{0, 116, "CTRL-RIGHT ARROW"},
	{0, 117, "CTRL-END"},
	{0, 118, "CTRL-PAGE DOWN"},
	{0, 119, "CTRL-HOME"},
	{0, 120, "ALT-1 (KEYBOARD)"},
	{0, 121, "ALT-2 (KEYBOARD)"},
	{0, 122, "ALT-3 (KEYBOARD)"},
	{0, 123, "ALT-4 (KEYBOARD)"},
	{0, 124, "ALT-5 (KEYBOARD)"},
	{0, 125, "ALT-6 (KEYBOARD)"},
	{0, 126, "ALT-7 (KEYBOARD)"},
	{0, 127, "ALT-8 (KEYBOARD)"},
	{0, 128, "ALT-9 (KEYBOARD)"},
	{0, 129, "ALT-0 (KEYBOARD)"},
	{0, 130, "ALT-- (ALT-MINUS KEYBOARD)"},
	{0, 131, "ALT-= (ALT-EQUAL KEYBOARD)"},
	{0, 132, "CTRL-PAGE UP"},

	// Upper Case Letters
	{'A', 30, "A"},
	{'B', 48, "B"},
	{'C', 46, "C"},
	{'D', 32, "D"},
	{'E', 18, "E"},
	{'F', 33, "F"},
	{'G', 34, "G"},
	{'H', 35, "H"},
	{'I', 23, "I"},
	{'J', 36, "J"},
	{'K', 37, "K"},
	{'L', 38, "L"},
	{'M', 50, "M"},
	{'N', 49, "N"},
	{'O', 24, "O"},
	{'P', 25, "P"},
	{'Q', 16, "Q"},
	{'R', 19, "R"},
	{'S', 31, "S"},
	{'T', 20, "T"},
	{'U', 22, "U"},
	{'V', 47, "V"},
	{'W', 17, "W"},
	{'X', 45, "X"},
	{'Y', 21, "Y"},
	{'Z', 44, "Z"},

	// Lower Case Letters
	{'a', 30, "a"},
	{'b', 48, "b"},
	{'c', 46, "c"},
	{'d', 32, "d"},
	{'e', 18, "e"},
	{'f', 33, "f"},
	{'g', 34, "g"},
	{'h', 35, "h"},
	{'i', 23, "i"},
	{'j', 36, "j"},
	{'k', 37, "k"},
	{'l', 38, "l"},
	{'m', 50, "m"},
	{'n', 49, "n"},
	{'o', 24, "o"},
	{'p', 25, "p"},
	{'q', 16, "q"},
	{'r', 19, "r"},
	{'s', 31, "s"},
	{'t', 20, "t"},
	{'u', 22, "u"},
	{'v', 47, "v"},
	{'w', 17, "w"},
	{'x', 45, "x"},
	{'y', 21, "y"},
	{'z', 44, "z"},

	// Decimal Digits
	{'0', 11, "0"},
	{'1', 2, "1"},
	{'2', 3, "2"},
	{'3', 4, "3"},
	{'4', 5, "4"},
	{'5', 6, "5"},
	{'6', 7, "6"},
	{'7', 8, "7"},
	{'8', 9, "8"},
	{'9', 10, "9"},

	// NULL Dummy Entry
	{0, 0, 0}
};

// Keyboard controller table -- the ASCII code field is insignificant
const KeyboardCodes::CntrTable KeyboardCodes::_CntrTable =
{
	// Keyboard Controller Labels
	{27, 1, "ESC"},
	{'1', 2, "1"},
	{'2', 3, "2"},
	{'3', 4, "3"},
	{'4', 5, "4"},
	{'5', 6, "5"},
	{'6', 7, "6"},
	{'7', 8, "7"},
	{'8', 9, "8"},
	{'9', 10, "9"},
	{'0', 11, "0"},
	{45, 12, "-"},
	{61, 13, "="},
	{8, 14, "BACKSPACE"},
	{9, 15, "TAB"},
	{'a', 30, "a"},
	{'b', 48, "b"},
	{'c', 46, "c"},
	{'d', 32, "d"},
	{'e', 18, "e"},
	{'f', 33, "f"},
	{'g', 34, "g"},
	{'h', 35, "h"},
	{'i', 23, "i"},
	{'j', 36, "j"},
	{'k', 37, "k"},
	{'l', 38, "l"},
	{'m', 50, "m"},
	{'n', 49, "n"},
	{'o', 24, "o"},
	{'p', 25, "p"},
	{'q', 16, "q"},
	{'r', 19, "r"},
	{'s', 31, "s"},
	{'t', 20, "t"},
	{'u', 22, "u"},
	{'v', 47, "v"},
	{'w', 17, "w"},
	{'x', 45, "x"},
	{'y', 21, "y"},
	{'z', 44, "z"},
	{91, 26, "["},
	{93, 27, "]"},
	{13, 28, "ENTER"},
	{0, SETHIGHBYTE(28,EXTBYTE), "ENTER (KEYPAD)"},
	{59, 39, ";"},
	{39, 40, "\'"},
	{96, 41, "`"},
	{92, 43, "\\"},
	{44, 51, ","},
	{46, 52, "."},
	{47, 53, "/"},
	{0, SETHIGHBYTE(53,EXTBYTE), "/ (KEYPAD)"},
	{42, 55, "* (KEYPAD)"},
	{32, 57, "SPACE"},
	{0, 59, "F1"},
	{0, 60, "F2"},
	{0, 61, "F3"},
	{0, 62, "F4"},
	{0, 63, "F5"},
	{0, 64, "F6"},
	{0, 65, "F7"},
	{0, 66, "F8"},
	{0, 67, "F9"},
	{0, 68, "F10"},
	{0, 71, "HOME (KEYPAD)"},
	{0, 72, "UP ARROW (KEYPAD)"},
	{0, 73, "PAGE UP (KEYPAD)"},
	{0, 74, "- (KEYPAD)"},
	{0, 75, "LEFT ARROW (KEYPAD)"},
	{0, 76, "5 (KEYPAD)"},
	{0, 77, "RIGHT ARROW (KEYPAD)"},
	{0, 78, "+ (KEYPAD)"},
	{0, 79, "END (KEYPAD)"},
	{0, 80, "DOWN ARROW (KEYPAD)"},
	{0, 81, "PAGE DOWN (KEYPAD)"},
	{0, 82, "INSERT (KEYPAD)"},
	{0, 83, "DELETE (KEYPAD)"},
	{0, 29, "CTRL"},
	{0, SETHIGHBYTE(29,EXTBYTE), "RIGHT CTRL"},
	{0, SETHIGHBYTE(69,29), "PAUSE"},
	{0, 42, "LEFT SHIFT"},
	{0, SETHIGHBYTE(52,EXTBYTE), "PRINT SCREEN"},
	{0, SETHIGHBYTE(71,EXTBYTE), "HOME (MIDDLE)"},
	{0, SETHIGHBYTE(72,EXTBYTE), "UP ARROW (MIDDLE)"},
	{0, SETHIGHBYTE(73,EXTBYTE), "PAGE UP (MIDDLE)"},
	{0, SETHIGHBYTE(75,EXTBYTE), "LEFT ARROW (MIDDLE)"},
	{0, SETHIGHBYTE(77,EXTBYTE), "RIGHT ARROW (MIDDLE)"},
	{0, SETHIGHBYTE(79,EXTBYTE), "END (MIDDLE)"},
	{0, SETHIGHBYTE(80,EXTBYTE), "DOWN ARROW (MIDDLE)"},
	{0, SETHIGHBYTE(81,EXTBYTE), "PAGE DOWN (MIDDLE)"},
	{0, SETHIGHBYTE(82,EXTBYTE), "INSERT (MIDDLE)"},
	{0, SETHIGHBYTE(83,EXTBYTE), "DELETE (MIDDLE)"},
	{0, 54, "RIGHT SHIFT"},
	{0, 56, "ALT"},
	{0, SETHIGHBYTE(56,EXTBYTE), "RIGHT ALT"},
	{0, 58, "CAPS LOCK"},
	{0, 69, "NUM LOCK"},
	{0, 70, "SCROLL LOCK"},
	{0, 87, "F11"},
	{0, 88, "F12"},

	// NULL Dummy Entry
	{0, 0, 0}
};

// Flags needed for caps and num lock setting methods
volatile unsigned char _far* KeyboardCodes::_KeyFlags =
	(unsigned char _far *)0x00400017;

// Flags needed for reading the BIOS shift state
volatile KeyboardCodes::BiosShiftState _far* KeyboardCodes::_KbdStatus =
	(BiosShiftState _far *)0x00000417L;

/****************************************************************************/
// PURPOSE:
//   Method to test for the existence of an extended keyboard.
//
// POST:
//   Returns 1 if extended keyboard is present, otherwise returns 0.
//
int KeyboardCodes::IsExtKeyboard()
{
      union REGS rg;
      unsigned kbdflags;

      rg.h.ah = 0x02;               /* check BIOS supports enhanced kbd */
      int86(0x16, &rg, &rg);        /* get kbd flags                    */
      kbdflags = rg.h.al;

      /* mess 'em up, get enhanced flags */
      rg.x.ax = 0x1200 + kbdflags ^ 0xff;
      int86(0x16, &rg, &rg);
      if (rg.h.al == kbdflags)      /* BIOS supports enhanced keyboard  */
      {
	    /* if bit 4 at 40:96h is set machine has an enhanced kbd */
	    if ((*(( char far *) 0x400096L) & 0x10))
		  return 1;         /* enhanced keyboard present        */
      }
      return 0;                     /* don't use enhanced keyboard calls */
}

/****************************************************************************/
// PURPOSE:
//   Method to return the alternate of 2 extended keyboard indicator values
//
// PRE
//   Byte_t HiByte_	: the high byte value of a given scan code
//
// POST:
//   Returns 1 of 2 alternate scan code values to indicate the existence of
//   an extended keyboard depending on the value of the argument passed.
//
Byte_t KeyboardCodes::Alternate(Byte_t HiByte_)
{
  return ((HiByte_ == 0x2A) ? 0xE0:0x2A);
}

/****************************************************************************/
// PURPOSE:
//   Scan code equality test method.
//   Test for whether 2 scan codes are equivalent or not.
//
// PRE:
//   Word_t Scan1_	: 1st scan code
//   Word_t Scan2_	: 2nd scan code
//
// POST:
//   Returns whether or not the two given scan codes are equivalent. If they
//   have exactly the same value then or the high byte are set to either 1 of
//   2 alternate values then they are equivalent.
//
int KeyboardCodes::CompareCode(Word_t Scan1_, Word_t Scan2_)
{
  return ((HIGHBYTE(Scan1_) != 0) ?
		((Scan1_ == Scan2_) ||
			(LOWBYTE(Scan1_) == LOWBYTE(Scan2_) &&
			 HIGHBYTE(Scan1_) == Alternate(HIGHBYTE(Scan2_)))):
		(Scan1_ == Scan2_));
}

/****************************************************************************/
// PURPOSE:
//   Methods which returns the scan code given the name of the key.
//   if the Extended Byte pointer is not null then it is set to the high
//   byte of the scan code which may contain the extended code 0xEO which
//   are often returned by extended 101 keyboards.
//
// PRE:
//   CntrLabel_t Label_		: The name of the key
//   Byte_t* ExtByte_		: Extended keyboard byte
//
// POST:
//   Scan code of the given key is returned. the extended byte pointer
//   may or may not be set depending on whether it is NULL and whether the
//   keyboard support extended keys.
//
Byte_t KeyboardCodes::SCANCODE(CntrLabel_t Label_, Byte_t* ExtByte_)
{
  Word_t ScanCode_ = CNTRTABLE(Label_).SCAN;

  if (ExtByte_)
    *ExtByte_ = HIGHBYTE(ScanCode_);

  return LOWBYTE(ScanCode_);
}

/****************************************************************************/
// PURPOSE:
//   Controller table accessor method. returns a constant KeyInfo structure
//   containing the scan code and name of the key given the scancode
//
// PRE:
//   Word_t ScanCode_	: The scan code passed to the controller table method
//
// POST:
//   Returns a constant KeyInfo structure containing both the scan code and
//   the name of the key.
//
const KeyboardCodes::KeyInfo& KeyboardCodes::CNTRTABLE(Word_t ScanCode_)
{
  // search from the beginning of the controller code table to the end
  // call the code comparison method to test the given scan code with the
  // currently indexed one in the table if a match is found then break from
  // the loop and return the KeyInfo entry from the table as const reference.
  for (CntrLabel_t Index_ = 0; Index_ < MAXCNTRCODE; Index_++)
    if (CompareCode(CNTRTABLE(Index_).SCAN, ScanCode_))
      break;

  return CNTRTABLE(Index_);
}

/****************************************************************************/
// PURPOSE:
//   Code table lookup method which returns the KeyInfo structure for the
//   given ascii/scan code pairs.
//
// PRE:
//   Byte_t AsciiCode_	: The ASCII code given
//   Word_t ScanCode_	: The scan code given
//
// POST:
//   Returns a constant KeyInfo structure containing the name of the key
//   and the given Ascii/Scan code pairs
//
const KeyboardCodes::KeyInfo& KeyboardCodes::CODETABLE(Byte_t AsciiCode_, Word_t ScanCode_)
{
  // loop through the table and look for entries with the same scan/ascii
  // code pairs as the arguments
  for (KeyLabel_t Index_ = 0; Index_ < MAXKEYCODE; Index_++)
    if (CODETABLE(Index_).SCAN == ScanCode_ &&
	CODETABLE(Index_).ASCII == AsciiCode_)
      break;

  return CODETABLE(Index_);
}

/****************************************************************************/
#if KEYCODES_DEBUG
#include <conio.h>
#include <iostream.h>
void main()
{
  clrscr();

  cout <<(int)KeyboardCodes::CODETABLE(KeyboardCodes::CTRL_F1).ASCII <<endl
       <<(int)KeyboardCodes::CODETABLE(KeyboardCodes::CTRL_F1).SCAN <<endl
       <<KeyboardCodes::CODETABLE(KeyboardCodes::CTRL_F1).LABEL <<endl
       <<KeyboardCodes::CODETABLE(KeyboardCodes::UPPER_G).LABEL <<endl;

  KeyboardCodes::KeyInfo Info_;
  Info_.ASCII = 'm';
  Info_.SCAN = 50;

  Info_.LABEL = KeyboardCodes::CODETABLE(Info_.ASCII, Info_.SCAN).LABEL;
  cout <<Info_.LABEL <<endl;
}
#endif
/****************************************************************************/
#endif