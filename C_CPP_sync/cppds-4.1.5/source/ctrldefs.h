#ifndef CTRLDEFS_H
#define CTRLDEFS_H

// Standard libraries
#ifndef INCL_STDLIB_H
  #include <stdlib.h>
  #define INCL_STDLIB_H
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef INCL_CTYPE_H
  #include <ctype.h>
  #define INCL_CTYPE_H
#endif
#ifndef INCL_STRING_H
  #include <string.h>
  #define INCL_STRING_H
#endif
#ifndef INCL_CONIO_H
  #include <conio.h>
  #define INCL_CONIO_H
#endif
#ifndef INCL_ASSERT_H
  #include <assert.h>
  #define INCL_ASSERT_H
#endif
#if defined(__DJGPP__)
  #ifndef INCL_UNISTD_H
    #include <unistd.h>
    #define INCL_UNISTD_H
  #endif
#endif

// User defined libraries
#ifndef KEYCODES_H
  #include "keycodes.h"
#endif
#ifndef KEYMAP_H
  #include "keymap.h"
#endif
#ifndef USERMSG_H
  #include "usermsg.h"
#endif
#ifndef ASSERTS_H
  #include "asserts.h"
#endif

// Compiler specific commands, redefine these if you need to.
#if defined(__TURBOC__) | defined(__BORLANDC__) | defined(__DJGPP__)
  #define CLRSCR clrscr		// Clears the screen
  #define GOTOXY gotoxy		// set cursor at specified x,y position
  #define CLREOL clreol		// clears text from cursor to end of line
  #define GETCH getch		// gets character, direct input from console
  #define PUTCH putch		// puts character, direct output to console
  #define CPUTS cputs		// puts string, direct output to console
  #define WHEREX wherex		// x position of cursor on screen
  #define WHEREY wherey		// y position of cursor on screen
#endif

/****************************************************************************/
class ControlWindow;
class ControlInfo;

struct ColorInfo
{
  enum HighLightMode
  {
    NORMAL_COLORS,
    HIGHLIGHT_COLORS,
    BACKGROUND_COLOR
  };

  Boolean* _UseColor;
  int* _NormalColors;
  int* _HighLightColors;
  int* _HkColor;
  int* _BackGroundColor;

  ColorInfo();
  ~ColorInfo();

  void ClearColors();
  void ClearAll();

  void SetupColors(int Fg_, int Bg_);
  Boolean SetupColors(int Mode_);

  void UseColor(Boolean Flag_);
  void SetNormalColors(int FgCol_, int BgCol_);
  void SetHighLightColors(int FgCol_, int BgCol_);
  void SetHotKeyColor(int Col_);
  void SetBackGroundColor(int Col_);

  Boolean UsesColor() const;
  int* NormalColors() const;
  int* HighLightColors() const;
  int HotKeyColor() const;
  int BackGroundColor() const;
};

class TextControl
{
  friend ControlInfo;

  protected:
    // Text color methods
    static ColorInfo& GetDefaultColorInfo();

  public:
    enum KeyAction
    {
      ESCAPE = 0,	// extended code == 27
      ENTER,		// extended code == 13, CTRL-M
      NEXT,		// extended code == 9, CTRL-I
      BACK,             // extended code == 8, CTRL-H
      DOWN,             // extended code == 336
      UP,               // extended code == 328
      RIGHT,            // extended code == 333
      LEFT,             // extended code == 331
      PAGEUP,           // extended code == 329
      PAGEDOWN,         // extended code == 337
      INSERT,           // extended code == 338
      DELETE,           // extended code == 339
      HOME,             // extended code == 327
      END,              // extended code == 335
      TOTAL_ACTIONS
    };

    enum HotKeyType
    {
      NONE = 0,
      HOTKEY,
      SHORTCUT
    };

    TextControl();
    virtual ~TextControl() {}

    // prompt box manipulators
    virtual void Show()				= 0;
    virtual void ShowInput()			= 0;
    virtual void Hide()				= 0;
    virtual int Prompt(int status_)		= 0;
    virtual void Erase()			= 0;

    // Accelerator key methods
    virtual int GetHotKey() const	= 0;
    virtual int GetShortCut() const	= 0;
    virtual int GetHotKeyType() const	= 0;

    // text control selection/deselection mutators
    virtual void Select()			= 0;
    virtual void Deselect()			= 0;

    // Empty, valid and selected Text Control test
    virtual Boolean IsSelectable()      = 0;
    virtual Boolean IsSelected()	= 0;
    virtual Boolean IsEmpty()		= 0;
    virtual Boolean IsEscaped()		= 0;
    virtual Boolean IsValid()		= 0;
    virtual Boolean IsActive()		= 0;
    virtual Boolean IsInFocus()		= 0;

    // Activation/Deactivation for this text widget
    virtual void Activate()		= 0;
    virtual void Deactivate()		= 0;

    // Control Ownership methods
    virtual void SetOwner(ControlWindow* Pwin_, TextControl* TxtCtrl_) 	= 0;
    virtual Boolean IsOwner(ControlWindow* Pwin_)	= 0;

    // Text color methods
    virtual void UseColor(Boolean Flag_=TRUE) = 0;
    virtual void SetNormalColors(int FgCol_, int BgCol_) = 0;
    virtual void SetHighLightColors(int FgCol_, int BgCol_) = 0;
    virtual void SetHotKeyColor(int Col_) = 0;
    virtual void SetBackGroundColor(int Col_) = 0;
    virtual void UseDefaultColors() = 0;

    virtual Boolean UsesColor() const = 0;
    virtual int* NormalColors() const = 0;
    virtual int* HighLightColors() const = 0;
    virtual int HotKeyColor() const = 0;
    virtual int BackGroundColor() const = 0;

    // Keyboard mapping methods
    static KeyMapArray& GetKeyMap();
    
    // Accelerator key methods
    static char* SetMessage_Method(char* str, int& ShortCut_, int& HotKey_, int& HotIndex_);
};

/****************************************************************************/
class ControlWindow
{
  protected:
    // Text Control accessor/mutator methods
    virtual void Control(int Index_, TextControl* TxtCtrl_)	= 0;
    virtual TextControl* Control(int Index_)			= 0;

    // Dialog controls check methods
    virtual Boolean ValidityCheck()			= 0;
    virtual Boolean AnyActive()				= 0;

    // data member destruction method
    virtual void Clear()				= 0;

  public:
    // prompt window data mutators
    virtual void MaxControl(int max)			= 0;
    virtual void Grow(int extra)     			= 0;

    // data member accessors
    virtual int MaxControl()				= 0;
    virtual void RemoveControl(TextControl* TxtCtrl_)	= 0;
    virtual void DeleteControl(TextControl* TxtCtrl_)   = 0;

    // prompt window manipulators
    virtual void Show()					= 0;
    virtual void Hide()					= 0;
    virtual int Prompt(int StartIndex_=0)		= 0;
    virtual void Erase()				= 0;
    virtual void Delete()				= 0;
    virtual void Remove()				= 0;

    // Text color methods
    virtual void UseColor(Boolean Flag_=TRUE) 			= 0;
    virtual void SetNormalColors(int FgCol_, int BgCol_) 	= 0;
    virtual void SetHighLightColors(int FgCol_, int BgCol_) 	= 0;
    virtual void SetHotKeyColor(int Col_) 			= 0;
    virtual void UseDefaultColors() 				= 0;

    // Retrieve control information object
    virtual ControlInfo* GetControlInfo()		= 0;
};

/****************************************************************************/
class ControlInfo
{
  protected:
    int* _HotKeys;
    int* _ShortCuts;
    int _Max;
    int _Index;
    int _HotKeyPressed;

    ControlInfo(const ControlInfo&){}

    // Array extending methods
    int* GrowArray(int* Array_, int Sz_);
    void Grow();

  public:
    ControlInfo(int Max_=0);
    virtual ~ControlInfo();

    // Hot key setting method
    void SetHotKeys(int Index_, TextControl* TxtCtrl_);

    // Hot key testing method
    int TestHotKey(int Key_);

    // Reset data members
    void ResetHotKeys();

    // class data member accessors
    int GetIndex() const
	{ return _Index; }
    int HotKeyPressed() const
	{ return _HotKeyPressed; }
};

/****************************************************************************/
#endif




