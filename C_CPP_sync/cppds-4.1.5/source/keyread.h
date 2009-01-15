#ifndef KEYREAD_H
#define KEYREAD_H
#ifndef KEYCODES_H
  #include "keycodes.h"
#endif
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif
#ifndef INCL_STDIO_H
  #include <stdio.h>
  #define INCL_STDIO_H
#endif
#ifndef INCL_DOS_H
  #include <dos.h>
  #define INCL_DOS_H
#endif
#ifndef INCL_CONIO_H
  #include <conio.h>
  #define INCL_CONIO_H
#endif
#ifndef INCL_IOSTREAM_H
  #include <iostream.h>
  #define INCL_IOSTREAM_H
#endif
#ifndef KEYWATCH_H
  #include "keywatch.h"
#endif

#define KEYREAD_DEBUG		0
#define KEYREAD_DEBUG1		0
#define KEYREAD_DEBUG2		0
#define KEYREAD_DEBUG3		0

/****************************************************************************/
class KeyboardReader : public KeyboardCodes
{
  private:
    static short _Instances;

    static void InitController();
    static void ExitController();

  public:
    KeyboardReader();
    ~KeyboardReader();

    // Keyboard code interpreter BIOS call
    static Word_t ReadKeyboard(Byte_t& ScanCode_, Byte_t& AsciiCode_);
    static Word_t GetKey();

    static const KeyboardReader& Controller();
    const ControllerCode ReadScan() const;
};

/****************************************************************************/
class KeyboardWriter : public KeyboardReader
{
  private:
    // Show keyboard controller codes and labels
    static void PrintCntrCodes(ostream& Out_, const ControllerCode& CntrCode_);
    static void PrintCntrKeys(ostream& Out_, const ControllerCode& CntrCode_);

    // Show keyboard code method
    static void ShowKey(ostream& Out_,
			const ControllerCode& CntrCode_,
			Byte_t ScanCode_, Byte_t AsciiCode_);

  public:
    // Keyboard reader method with output stream argument
    static void ReadKey(ostream& Out_);

    // Keyboard reader method using cout as default output stream
    static void ReadKey()
	{ ReadKey(cout); }

    static void ShowBiosShiftState();
};

/****************************************************************************/
#endif