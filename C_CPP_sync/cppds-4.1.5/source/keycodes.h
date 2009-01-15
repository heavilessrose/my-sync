/****************************************************************************
* FILE		: KEYCODES.H
* PURPOSE	: Keyboard codes definitions class header file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: Dec 17 1997
****************************************************************************/
#ifndef KEYCODES_H
#define KEYCODES_H
#ifndef INCL_STDDEF_H
  #include <stddef.h>
  #define INCL_STDDEF_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif
#ifndef INCL_DOS_H
  #include <dos.h>
  #define INCL_DOS_H
#endif
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

#define KEYCODES_DEBUG		0
#define EXTBYTE			0xE0

/****************************************************************************/
class KeyboardCodes
{
  public:

    typedef struct                     /* Keyboard status structure */
    {
      unsigned int
      right_shift_down  : 1,           /* Right Shift key depressed */
      left_shift_down   : 1,            /* Left Shift key depressed */
      ctrl_shift_down   : 1,                  /* Ctrl key depressed */
      alt_shift_down    : 1,                   /* Alt key depressed */
      scroll_on         : 1,                   /* Scroll Lock is on */
      num_on            : 1,                      /* Num Lock is on */
      caps_on           : 1,                     /* Caps Lock is on */
      ins_on            : 1,              /* Insert state is active */
      filler            : 3,           /* Filler for word alignment */
      ctrl_numloc       : 1,                   /* Suspend key is on */
      scroll_down       : 1,           /* Scroll Lock key depressed */
      num_down          : 1,              /* Num Lock key depressed */
      caps_down         : 1,             /* Caps Lock key depressed */
      ins_down          : 1;                /* Insert key depressed */
    } BiosShiftState;

    // Key information structure
    struct KeyInfo
    {
	Byte_t ASCII;			// Ascii code of key
	Word_t SCAN;			// Scan code of key
	const char* LABEL;		// Name label of key
    };

    // Keyboard table type definition and table limit constant
    enum
    {
      MAXKEYCODE = 249,		// Maximum number of key code entries
      MAXCNTRCODE = 101		// Maximum number of controller code entries
    };

    // Type definintion of keyboard and controller code tables
    typedef KeyInfo KeyTable[MAXKEYCODE + 1];
    typedef KeyInfo CntrTable[MAXCNTRCODE + 1];

    // Key Label enumerated type, each entry refers to the keyboard key
    enum KeyLabel_t
    {
	BREAK,
	ESC,
	EXCLAMATION,
	AT,
	CTRL_SHIFT_2,
	POUND,
	DOLLAR,
	PERCENT,
	CARET,
	CTRL_SHIFT_6,
	AMPERSAND,
	ASTERISK,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	MINUS,
	UNDERSCORE,
	CTRL_SHIFT_MINUS,
	EQUAL,
	PLUS,
	BACKSPACE,
	CTRL_BACKSPACE,
	TAB,
	SHIFT_TAB,
	ALT_Q,
	CTRL_Q,
	ALT_W,
	CTRL_W,
	ALT_E,
	CTRL_E,
	ALT_R,
	CTRL_R,
	ALT_T,
	CTRL_T,
	ALT_Y,
	CTRL_Y,
	ALT_U,
	CTRL_U,
	ALT_I,
	CTRL_I,
	ALT_O,
	CTRL_O,
	ALT_P,
	CTRL_P,
	LEFTBRACKET,
	LEFTBRACE,
	CTRL_LEFTBRACKET,
	RIGHTBRACKET,
	RIGHTBRACE,
	CTRL_RIGHTBRACKET,
	ENTER,
	ALT_A,
	CTRL_A,
	ALT_S,
	CTRL_S,
	ALT_D,
	CTRL_D,
	ALT_F,
	CTRL_F,
	ALT_G,
	CTRL_G,
	ALT_H,
	CTRL_H,
	ALT_J,
	CTRL_J,
	ALT_K,
	CTRL_K,
	ALT_L,
	CTRL_L,
	SEMICOLON,
	COLON,
	APOSTROPHE,
	QUOTE,			// SHIFT-APOSTROPHE
	BACKQUOTE,
	TILDE,			// SHIFT-BACKQUOTE
	BACKSLASH,
	PIPE,                   // SHIFT-BACKSLASH
	CTRL_BACKSLASH,
	ALT_Z,
	CTRL_Z,
	ALT_X,
	CTRL_X,
	ALT_C,
	CTRL_C,
	ALT_V,
	CTRL_V,
	ALT_B,
	CTRL_B,
	ALT_N,
	CTRL_N,
	ALT_M,
	CTRL_M,
	COMMA,
	LESSTHAN, 	        // SHIFT-COMMA
	PERIOD,
	GREATERTHAN,		// SHIFT-PERIOD
	FORWARDSLASH,
	QUESTION,		// SHIFT-FORWARDSLASH
	KEYPAD_ASTERISK,
	ALT_PAUSE,
	ALT_BREAK,
	SPACE,
	CAPS_LOCK,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	NUM_LOCK,
	SCROLL_LOCK,
	HOME,
	KEYPAD_7,
	UPARROW,
	KEYPAD_8,
	PAGEUP,
	KEYPAD_9,
	KEYPAD_MINUS,
	LEFTARROW,
	KEYPAD_4,
	CENTERKEY,
	KEYPAD_5,
	RIGHTARROW,
	KEYPAD_6,
	KEYPAD_PLUS,
	END,
	KEYPAD_1,
	DOWNARROW,
	KEYPAD_2,
	PAGEDOWN,
	KEYPAD_3,
	INSERT,
	KEYPAD_0,
	DELETE,
	KEYPAD_PERIOD,
	SHIFT_F1,
	SHIFT_F2,
	SHIFT_F3,
	SHIFT_F4,
	SHIFT_F5,
	SHIFT_F6,
	SHIFT_F7,
	SHIFT_F8,
	SHIFT_F9,
	SHIFT_F10,
	CTRL_F1,
	CTRL_F2,
	CTRL_F3,
	CTRL_F4,
	CTRL_F5,
	CTRL_F6,
	CTRL_F7,
	CTRL_F8,
	CTRL_F9,
	CTRL_F10,
	ALT_F1,
	ALT_F2,
	ALT_F3,
	ALT_F4,
	ALT_F5,
	ALT_F6,
	ALT_F7,
	ALT_F8,
	ALT_F9,
	ALT_F10,
	CTRL_PRTSC,
	CTRL_LEFTARROW,
	CTRL_RIGHTARROW,
	CTRL_END,
	CTRL_PAGEDOWN,
	CTRL_HOME,
	ALT_1,
	ALT_2,
	ALT_3,
	ALT_4,
	ALT_5,
	ALT_6,
	ALT_7,
	ALT_8,
	ALT_9,
	ALT_0,
	ALT_MINUS,
	ALT_EQUAL,
	CTRL_PAGEUP,
	UPPER_A,
	UPPER_B,
	UPPER_C,
	UPPER_D,
	UPPER_E,
	UPPER_F,
	UPPER_G,
	UPPER_H,
	UPPER_I,
	UPPER_J,
	UPPER_K,
	UPPER_L,
	UPPER_M,
	UPPER_N,
	UPPER_O,
	UPPER_P,
	UPPER_Q,
	UPPER_R,
	UPPER_S,
	UPPER_T,
	UPPER_U,
	UPPER_V,
	UPPER_W,
	UPPER_X,
	UPPER_Y,
	UPPER_Z,
	LOWER_A,
	LOWER_B,
	LOWER_C,
	LOWER_D,
	LOWER_E,
	LOWER_F,
	LOWER_G,
	LOWER_H,
	LOWER_I,
	LOWER_J,
	LOWER_K,
	LOWER_L,
	LOWER_M,
	LOWER_N,
	LOWER_O,
	LOWER_P,
	LOWER_Q,
	LOWER_R,
	LOWER_S,
	LOWER_T,
	LOWER_U,
	LOWER_V,
	LOWER_W,
	LOWER_X,
	LOWER_Y,
	LOWER_Z,
	KEYBOARD_0,
	KEYBOARD_1,
	KEYBOARD_2,
	KEYBOARD_3,
	KEYBOARD_4,
	KEYBOARD_5,
	KEYBOARD_6,
	KEYBOARD_7,
	KEYBOARD_8,
	KEYBOARD_9,
    };

    // Keyboard Controller Labels.
    // enumerated types prepended with CNTR are controller codes.
    enum CntrLabel_t
    {
	CNTR_ESC,
	CNTR_1,
	CNTR_2,
	CNTR_3,
	CNTR_4,
	CNTR_5,
	CNTR_6,
	CNTR_7,
	CNTR_8,
	CNTR_9,
	CNTR_0,
	CNTR_MINUS,
	CNTR_EQUAL,
	CNTR_BACKSPACE,
	CNTR_TAB,
	CNTR_A,
	CNTR_B,
	CNTR_C,
	CNTR_D,
	CNTR_E,
	CNTR_F,
	CNTR_G,
	CNTR_H,
	CNTR_I,
	CNTR_J,
	CNTR_K,
	CNTR_L,
	CNTR_M,
	CNTR_N,
	CNTR_O,
	CNTR_P,
	CNTR_Q,
	CNTR_R,
	CNTR_S,
	CNTR_T,
	CNTR_U,
	CNTR_V,
	CNTR_W,
	CNTR_X,
	CNTR_Y,
	CNTR_Z,
	CNTR_LEFTBRACKET,
	CNTR_RIGHTBRACKET,
	CNTR_ENTER,
	CNTR_KEYPAD_ENTER,
	CNTR_SEMICOLON,
	CNTR_APOSTROPHE,
	CNTR_BACKQUOTE,
	CNTR_BACKSLASH,
	CNTR_COMMA,
	CNTR_PERIOD,
	CNTR_FORWARDSLASH,
	CNTR_KEYPAD_FORWARDSLASH,
	CNTR_KEYPAD_ASTERISK,
	CNTR_SPACE,
	CNTR_F1,
	CNTR_F2,
	CNTR_F3,
	CNTR_F4,
	CNTR_F5,
	CNTR_F6,
	CNTR_F7,
	CNTR_F8,
	CNTR_F9,
	CNTR_F10,
	CNTR_KEYPAD_HOME,
	CNTR_KEYPAD_UPARROW,
	CNTR_KEYPAD_PAGEUP,
	CNTR_KEYPAD_MINUS,
	CNTR_KEYPAD_LEFTARROW,
	CNTR_KEYPAD_5,
	CNTR_KEYPAD_RIGHTARROW,
	CNTR_KEYPAD_PLUS,
	CNTR_KEYPAD_END,
	CNTR_KEYPAD_DOWNARROW,
	CNTR_KEYPAD_PAGEDOWN,
	CNTR_KEYPAD_INSERT,
	CNTR_KEYPAD_DELETE,
	CNTR_CTRL,
	CNTR_RIGHT_CTRL,
	CNTR_PAUSE,
	CNTR_LEFT_SHIFT,
	CNTR_PRINT_SCREEN,
	CNTR_MIDDLE_HOME,
	CNTR_MIDDLE_UPARROW,
	CNTR_MIDDLE_PAGEUP,
	CNTR_MIDDLE_LEFTARROW,
	CNTR_MIDDLE_RIGHTARROW,
	CNTR_MIDDLE_END,
	CNTR_MIDDLE_DOWNARROW,
	CNTR_MIDDLE_PAGEDOWN,
	CNTR_MIDDLE_INSERT,
	CNTR_MIDDLE_DELETE,
	CNTR_RIGHT_SHIFT,
	CNTR_ALT,
	CNTR_RIGHT_ALT,
	CNTR_CAPS_LOCK,
	CNTR_NUM_LOCK,
	CNTR_SCROLL_LOCK,
	F11,
	F12
    };

    // Keyboard table method
    static const KeyInfo& CODETABLE(KeyLabel_t Label_)
	{ return _CodeTable[Label_]; }

    // Keyboard table method
    static const KeyInfo& CODETABLE(Byte_t AsciiCode_, Word_t ScanCode_);

    // Controller table method
    static Byte_t SCANCODE(CntrLabel_t Label_, Byte_t* ExtByte_=NULL);
    static const KeyInfo& CNTRTABLE(CntrLabel_t Label_)
	{ return _CntrTable[Label_]; }

    // Controller table method
    static const KeyInfo& CNTRTABLE(Word_t ScanCode_);

    // extended keyboard existence test
    static int IsExtKeyboard();

    // Keyboard locks set and clear methods
    static void SetCapsLock(void)
	{ *_KeyFlags |= 0x40; }

    static void ClrCapsLock(void)
	{ *_KeyFlags &= ~0x40; }

    static void SetNumLock(void)
	{ *_KeyFlags |= 0x20; }

    static void ClrNumLock(void)
	{ *_KeyFlags &= ~0x20; }

    // Keyboard BIOS state retrieval method
    static volatile BiosShiftState _far* GetBiosShiftState()
	{ return _KbdStatus; }

  protected:
    // Keyboard code table
    static const KeyTable _CodeTable;
    static const CntrTable _CntrTable;

  private:
    // Bios shift state and key lock flags
    static volatile BiosShiftState _far *_KbdStatus;
    static volatile unsigned char _far *_KeyFlags;

    // code comparison methods
    static int CompareCode(Word_t Scan1_, Word_t Scan2_);
    static Byte_t Alternate(Byte_t HiByte_);
};

/****************************************************************************/
#endif