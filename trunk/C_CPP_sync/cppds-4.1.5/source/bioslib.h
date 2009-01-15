//**************************************************************************//
//                                BIOSLIB.CPP                               //
// BIOS services library                                                    //
//                                                                          //
//   by Joseph Wong                                                         //
//   Last update 26/12/97                                                   //
//**************************************************************************//
#ifndef BIOSLIB_H
#define BIOSLIB_H
#ifndef INCL_DOS_H
  #include <dos.h>
  #define INCL_DOS_H
#endif
#ifndef TYPEDEFS_H
  #include "typedefs.h"
#endif

//**************************************************************************//
struct BiosCursor_t
{
  Byte_t _BegScan;
  Byte_t _EndScan;
  Byte_t _CursorRow;
  Byte_t _CursorCol;

  BiosCursor_t& operator ()
  (
    Byte_t BegScan_,
    Byte_t EndScan_,
    Byte_t CursorRow_,
    Byte_t CursorCol_
  );
};

//**************************************************************************//
struct BiosPen_t
{
  Byte_t _Trigger;
  Word_t _PixCol;
  Word_t _PixRow;
  Byte_t _PenRow;
  Byte_t _PenCol;

  BiosPen_t& operator ()
  (
    Byte_t Trigger_,
    Byte_t PixCol_,
    Byte_t PixRow_,
    Byte_t PenRow_,
    Byte_t PenCol_
  );
};

//**************************************************************************//
struct BiosWindow_t
{
  Byte_t _Lines;
  Byte_t _Disp;
  Byte_t _Top;
  Byte_t _Left;
  Byte_t _Bottom;
  Byte_t _Right;

  BiosWindow_t& operator ()
  (
    Byte_t Lines_,
    Byte_t Disp_,
    Byte_t Top_,
    Byte_t Left_,
    Byte_t Bottom_,
    Byte_t Right_
  );
};

//**************************************************************************//
struct BiosVideoState_t
{
  Byte_t _ScreenWidth;
  Byte_t _DisplayMode;
  Byte_t _ActivePage;

  BiosVideoState_t& operator ()
  (
    Byte_t ScreenWidth_,
    Byte_t DisplayMode_,
    Byte_t ActivePage_
  );
};

//**************************************************************************//
struct BiosString_t
{
  char* _Message;
  Byte_t _VideoMode;
  Byte_t _Page;
  Byte_t _CharAttr;
  Word_t _Length;
  Byte_t _Row;
  Byte_t _Column;

  BiosString_t& operator ()
  (
    char* Message_,
    Byte_t VideoMode_,
    Byte_t Page_,
    Byte_t CharAttr_,
    Word_t Length_,
    Byte_t Row_,
    Byte_t Column_
  );
};

//**************************************************************************//
struct BiosDriveStatus_t
{
  Byte_t _TracksPerSide;
  Byte_t _SectorsPerTrack;
  Byte_t _Sides;
  Byte_t _DrivesAttached;

  BiosDriveStatus_t& operator ()
  (
    Byte_t TracksPerSide_,
    Byte_t SectorsPerTrack_,
    Byte_t Sides_,
    Byte_t DrivesAttached_
  );
};

//**************************************************************************//
struct BiosCommStatus_t
{
  Byte_t _LineStatus;
  Byte_t _ModemStatus;
  Byte_t _ModemControl;

  BiosCommStatus_t& operator ()
  (
    Byte_t LineStatus_,
    Byte_t ModemStatus_,
    Byte_t ModemControl_
  );
};

//**************************************************************************//
struct BiosCommInfo_t
{
  Byte_t _ComPort;
  Byte_t _BreakFlag;
  Byte_t _ParityFlag;
  Byte_t _StopBits;
  Byte_t _DataBits;
  Word_t _Bps;

  BiosCommInfo_t& operator ()
  (
    Byte_t ComPort_,
    Byte_t BreakFlag_,
    Byte_t ParityFlag_,
    Byte_t StopBits_,
    Byte_t DataBits_,
    Word_t Bps_
  );
};

//**************************************************************************//
struct BiosJoystick_t
{
  Byte_t _Switch;
  Word_t _Xpos1;
  Word_t _Ypos1;
  Word_t _Xpos2;
  Word_t _Ypos2;

  BiosJoystick_t& operator ()
  (
    Byte_t Switch_,
    Word_t Xpos1_,
    Word_t Ypos1_,
    Word_t Xpos2_,
    Word_t Ypos2_
  );
};

//**************************************************************************//
struct BiosProtMode_t
{
  Byte_t _IRQ0off;
  Byte_t _IRQ8off;
  Word_t _CSoff;
  Word_t _GDTseg;
  Word_t _GDToff;

  BiosProtMode_t& operator ()
  (
    Byte_t IRQ0off_,
    Byte_t IRQ8off_,
    Word_t CSoff_,
    Word_t GDTseg_,
    Word_t GDToff_
  );
};

//**************************************************************************//
struct BiosRealClock_t
{
  Byte_t _Hours;
  Byte_t _Minutes;
  Byte_t _Seconds;

  union
  {
    Byte_t _Alarm;
    Byte_t _Daylight;
  };

  BiosRealClock_t& operator ()
  (
    Byte_t Hours_,
    Byte_t Minutes_,
    Byte_t Seconds_,
    Byte_t AlarmOrDaylight_
  );
};

//**************************************************************************//
struct BiosClockCount_t
{
  Byte_t _Midnight;
  Word_t _ClockHiWord;
  Word_t _ClockLoWord;

  BiosClockCount_t& operator ()
  (
    Byte_t Midnight_,
    Word_t ClockHiWord_,
    Word_t ClockLoWord_
  );
};

//**************************************************************************//
struct BiosRealDate_t
{
  Byte_t _Century;
  Byte_t _Year;
  Byte_t _Month;
  Byte_t _Day;

  BiosRealDate_t& operator ()
  (
    Byte_t Century_,
    Byte_t Year_,
    Byte_t Month_,
    Byte_t Day_
  );
};

//**************************************************************************//
class BiosLib_t
{
  protected:
    REGS _InRegs;
    REGS _OutRegs;
    SREGS _SegRegs;
    REGPACK _RegPack;

  public:
    BiosLib_t();

    // Video Services
    void PrintScreen();
    void SetVideoMode(Byte_t Mode_);
    void SetCursorSize(Byte_t BegScan_, Byte_t EndScan_);
    void SetCursorPos(Byte_t Pg_, Byte_t Row_, Byte_t Col_);
    void ReadCursor(Byte_t Pg_, BiosCursor_t& Info_);
    void ReadPen(BiosPen_t& Info_);
    void SetDisplayPage(Byte_t Pg_);
    void ScrollWinUp(BiosWindow_t& Info_);
    void ScrollWinDown(BiosWindow_t& Info_);
    Word_t ReadCharAttr(Byte_t Pg_, Byte_t& Attr_, Byte_t& AscCode_);
    void WriteCharAttr(Byte_t Pg_, Byte_t Ch_, Byte_t Attr_, Word_t Count_);
    void WritGraphCharAttr(Byte_t Pg_, Byte_t Ch_, Byte_t Attr_, Word_t Count_);
    void SetPalette(Byte_t Palid_, Byte_t Color_);
    void WritePixel(Byte_t PixVal_, Word_t PixCol_, Word_t PixRow);
    Byte_t ReadPixel(Word_t PixCol_, Word_t PixRow_);
    void WriteTTYChar(Byte_t Char_, Byte_t Pg_, Byte_t Color_);
    void GetVideoState(BiosVideoState_t& Info_);
    void SetBorder(Byte_t Color_);
    void AltVideo();
    Word_t GetEgaInfo(Word_t& VidMode_, Word_t& EgaRam_);
    void SetScanLines(Byte_t Lines_);
    void LoadPalette(Byte_t Flag_);
    void ToggleVideo(Byte_t Flag_);
    void SelectGrayScale(Byte_t Flag_);
    void ToggleCursorEmulation(Byte_t Flag_);
    void WriteString(BiosString_t& Info_);

    // System Services
    Word_t GetEquipStatus();
    Word_t GetMemorySize();
    void WarmBoot();

    // Disk Services
    Byte_t GetDiskStatus(Byte_t DriveNum_);
    void GetDriveParameters(Byte_t DriveNum_, BiosDriveStatus_t& Info_);
    void ParkHeads(Byte_t DriveNum_);

    // Communication Services
    Word_t InitCommPort(Byte_t InitParm_, Word_t Port_, BiosCommStatus_t& Info_);
    Byte_t SendChar(Byte_t Char_, Word_t Port_);
    Byte_t ReceiveChar(Word_t Port_);
    Word_t GetComStatus(Word_t Port_, BiosCommStatus_t& Info_);
    Word_t ExtInitCommPort(BiosCommInfo_t& Input_, BiosCommStatus_t& Output_);
    void ExtCommCtrl(Byte_t RdWr_, Byte_t Mcr_, Byte_t Port_, BiosCommStatus_t& Info_);

    // Miscellaneous Services
    void GetJoystick(Word_t ReadFlag_, BiosJoystick_t& Info_);
    Byte_t ExtMemBlockMove(Word_t NumWords_, Word_t SegAddr_, Word_t OffAddr_);
    Word_t GetExtMemSize();
    Byte_t SetProtMode(BiosProtMode_t& Info_);

    // PS/2 Services
    void GetSysConfig(Word_t& SysCfgOff_, Word_t& SysCfgSeg_);
    void GetExtBiosAddr(Byte_t& RetCode_, Word_t& ExtBiosAddr_);
    void ProgramOptionSelect(Byte_t Option_, Byte_t& BasePOSaddr_, Byte_t& SlotNum_);

    // Keyboard Services
    Word_t ReadKeyboardChar(Byte_t& ScanCode_, Byte_t& AsciiCode_);
    Word_t ReadKeyboardStatus(Byte_t& ScanCode_, Byte_t& AsciiCode_);
    Byte_t ReadKeyboardShift();
    void SetRepRate(Byte_t Delay_, Byte_t Rate_);
    Byte_t WriteKeyBuffer(Byte_t ScanCode_, Byte_t AsciiCode_);
    Word_t ReadExtKeyboardChar(Byte_t& ScanCode_, Byte_t& AsciiCode_);
    Word_t ReadExtKeyboardStatus(Byte_t& ScanCode_, Byte_t& AsciiCode_);
    Byte_t ReadExtKeyboardShift();

    // Printer Services
    Byte_t PrintChar(Byte_t Char_, Word_t Printer_);
    Byte_t InitPrinter(Word_t Printer_);
    Byte_t GetPrinterStatus(Word_t Printer_);

    // Date/time Services
    void GetClockCount(BiosClockCount_t& Info_);
    void SetClockCount(Word_t ClockHi_, Word_t ClockLo_);
    void ReadRealClock(BiosRealClock_t& Info_);
    void SetRealClock(BiosRealClock_t& Info_);
    void ReadRealDate(BiosRealDate_t& Info_);
    void SetRealDate(BiosRealDate_t& Info_);
    void SetAlarm(BiosRealClock_t& Info_);
    void DisableAlarm();
    void ReadAlarm(BiosRealClock_t& Info_);
    Word_t GetDayCount();
    void SetDayCount(Word_t DayCount_);
};

//**************************************************************************//
#endif