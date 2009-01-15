//--------------------------------------------------------------------------//
//                                BIOSLIB.CPP                               //
// BIOS services library                                                    //
//                                                                          //
//   by Joseph Wong                                                         //
//   Last update 26/12/97                                                   //
//**************************************************************************//
#ifndef BIOSLIB_CPP
#define BIOSLIB_CPP
#ifndef BIOSLIB_H
  #include "bioslib.h"
#endif

//**************************************************************************//
BiosCursor_t& BiosCursor_t::operator ()
(
  Byte_t BegScan_,
  Byte_t EndScan_,
  Byte_t CursorRow_,
  Byte_t CursorCol_
)
{
  _BegScan = BegScan_;
  _EndScan = EndScan_;
  _CursorRow = CursorRow_;
  _CursorCol = CursorCol_;

  return *this;
}

//**************************************************************************//
BiosPen_t& BiosPen_t::operator ()
(
  Byte_t Trigger_,
  Byte_t PixCol_,
  Byte_t PixRow_,
  Byte_t PenRow_,
  Byte_t PenCol_
)
{
  _Trigger = Trigger_;
  _PixCol = PixCol_;
  _PixRow = PixRow_;
  _PenRow = PenRow_;
  _PenCol = PenCol_;

  return *this;
}

//**************************************************************************//
BiosWindow_t& BiosWindow_t::operator ()
(
  Byte_t Lines_,
  Byte_t Disp_,
  Byte_t Top_,
  Byte_t Left_,
  Byte_t Bottom_,
  Byte_t Right_
)
{
  _Lines = Lines_;
  _Disp = Disp_;
  _Top = Top_;
  _Left = Left_;
  _Bottom = Bottom_;
  _Right = Right_;

  return *this;
}

//**************************************************************************//
BiosVideoState_t& BiosVideoState_t::operator ()
(
  Byte_t ScreenWidth_,
  Byte_t DisplayMode_,
  Byte_t ActivePage_
)
{
  _ScreenWidth = ScreenWidth_;
  _DisplayMode = DisplayMode_;
  _ActivePage = ActivePage_;

  return *this;
}

//**************************************************************************//
BiosString_t& BiosString_t::operator ()
(
  char* Message_,
  Byte_t VideoMode_,
  Byte_t Page_,
  Byte_t CharAttr_,
  Word_t Length_,
  Byte_t Row_,
  Byte_t Column_
)
{
  _Message = Message_;
  _VideoMode = VideoMode_;
  _Page = Page_;
  _CharAttr = CharAttr_;
  _Length = Length_;
  _Row = Row_;
  _Column = Column_;

  return *this;
}

//**************************************************************************//
BiosDriveStatus_t& BiosDriveStatus_t::operator ()
(
  Byte_t TracksPerSide_,
  Byte_t SectorsPerTrack_,
  Byte_t Sides_,
  Byte_t DrivesAttached_
)
{
  _TracksPerSide = TracksPerSide_;
  _SectorsPerTrack = SectorsPerTrack_;
  _Sides = Sides_;
  _DrivesAttached = DrivesAttached_;

  return *this;
}

//**************************************************************************//
BiosCommStatus_t& BiosCommStatus_t::operator ()
(
  Byte_t LineStatus_,
  Byte_t ModemStatus_,
  Byte_t ModemControl_
)
{
  _LineStatus = LineStatus_;
  _ModemStatus = ModemStatus_;
  _ModemControl = ModemControl_;

  return *this;
}

//**************************************************************************//
BiosCommInfo_t& BiosCommInfo_t::operator ()
(
  Byte_t ComPort_,
  Byte_t BreakFlag_,
  Byte_t ParityFlag_,
  Byte_t StopBits_,
  Byte_t DataBits_,
  Word_t Bps_
)
{
  _ComPort = ComPort_;
  _BreakFlag = BreakFlag_;
  _ParityFlag = ParityFlag_;
  _StopBits = StopBits_;
  _DataBits = DataBits_;
  _Bps = Bps_;

  return *this;
}

//**************************************************************************//
BiosJoystick_t& BiosJoystick_t::operator ()
(
  Byte_t Switch_,
  Word_t Xpos1_,
  Word_t Ypos1_,
  Word_t Xpos2_,
  Word_t Ypos2_
)
{
  _Switch = Switch_;
  _Xpos1 = Xpos1_;
  _Ypos1 = Ypos1_;
  _Xpos2 = Xpos2_;
  _Ypos2 = Ypos2_;

  return *this;
}

//**************************************************************************//
BiosProtMode_t& BiosProtMode_t::operator ()
(
  Byte_t IRQ0off_,
  Byte_t IRQ8off_,
  Word_t CSoff_,
  Word_t GDTseg_,
  Word_t GDToff_
)
{
  _IRQ0off = IRQ0off_;
  _IRQ8off = IRQ8off_;
  _CSoff = CSoff_;
  _GDTseg = GDTseg_;
  _GDToff = GDToff_;

  return *this;
}

//**************************************************************************//
BiosRealClock_t& BiosRealClock_t::operator ()
(
  Byte_t Hours_,
  Byte_t Minutes_,
  Byte_t Seconds_,
  Byte_t AlarmOrDaylight_
)
{
  _Hours = Hours_;
  _Minutes = Minutes_;
  _Seconds = Seconds_;
  _Alarm = AlarmOrDaylight_;

  return *this;
}

//**************************************************************************//
BiosClockCount_t& BiosClockCount_t::operator ()
(
  Byte_t Midnight_,
  Word_t ClockHiWord_,
  Word_t ClockLoWord_
)
{
  _Midnight = Midnight_;
  _ClockHiWord = ClockHiWord_;
  _ClockLoWord = ClockLoWord_;

  return *this;
}

//**************************************************************************//
BiosRealDate_t& BiosRealDate_t::operator ()
(
  Byte_t Century_,
  Byte_t Year_,
  Byte_t Month_,
  Byte_t Day_
)
{
  _Century = Century_;
  _Year = Year_;
  _Month = Month_;
  _Day = Day_;

  return *this;
}

//**************************************************************************//
// PURPOSE:
//   Bios Library class constructor
//
// PRE:
//   SREGS _SegRegs	: Segment registers data member
//
// POST:
//   _SegRegs.ds and _SegRegs.es is set to the segment address of the current
//   object
//
//**************************************************************************//
BiosLib_t::BiosLib_t()
{
  segread(&_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Printer Services
//
// PURPOSE:
//   Print screen BIOS service (INT 5h).
//   Interface to C language calling convention.
//**************************************************************************//
void BiosLib_t::PrintScreen()
{
  int86x(0x5, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Set video mode BIOS service (INT 10h, service 0).
//   Interface to C language calling convention.
//
// PRE:
//   mode : desired video mode
//
//   registers:
//     AH : 0
//     AL : desired video mode
//**************************************************************************//
void BiosLib_t::SetVideoMode(Byte_t Mode_)
{
  _InRegs.h.ah = 0;
  _InRegs.h.al = Mode_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Set cursor size BIOS service (INT 10h, service 1).
//   Interface to C language calling convention.
//
// PRE:
//   begscan : beginning scan line of cursor
//   endscan : ending scan line of cursor
//
//   registers:
//     AH : 1
//     CH : beginning scan line of cursor
//     CL : ending scan line of cursor
//**************************************************************************//
void BiosLib_t::SetCursorSize(Byte_t BegScan_, Byte_t EndScan_)
{
  _InRegs.h.ah = 1;
  _InRegs.h.ch = BegScan_;
  _InRegs.h.cl = EndScan_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Set cursor position BIOS service (INT 10h, service 2).
//   Interface to C language calling convention.
//
// PRE:
//   pg : video page number
//   row : cursor row
//   col : cursor column
//
//   registers:
//     AH : 2
//     BH : video page number
//     DH : cursor row
//     DL : cursor column
//**************************************************************************//
void BiosLib_t::SetCursorPos(Byte_t Pg_, Byte_t Row_, Byte_t Col_)
{
  _InRegs.h.ah = 2;
  _InRegs.h.bh = Pg_;
  _InRegs.h.dh = Row_;
  _InRegs.h.dl = Col_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   read cursor position and size BIOS service (INT 10h, service 3).
//   Interface to C language calling convention.
//
// PRE:
//   pg : video page number
//
//   registers:
//     AH : 3
//     BH : video page number
//
// POST:
//   info : information structure
//	   <1st field> : beginning scan line of cursor
//	   <2nd field> : ending scan line of cursor
//	   <3rd field> : cursor row
//	   <4th field> : cursor column
//
//   registers:
//     CH     : beginning scan line of cursor
//     CL     : ending scan line of cursor
//     DH     : cursor row
//     DL     : cursor column
//**************************************************************************//
void BiosLib_t::ReadCursor(Byte_t Pg_, BiosCursor_t& Info_)
{
  _InRegs.h.ah = 3;
  _InRegs.h.bh = Pg_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._BegScan = _OutRegs.h.ch;
  Info_._EndScan = _OutRegs.h.cl;
  Info_._CursorRow = _OutRegs.h.dh;
  Info_._CursorCol = _OutRegs.h.dl;
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   read light pen position BIOS service (INT 10h, service 4).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH : 4
//
// POST:
//   data in registers are saved in info structure -- info
//
//   info : information structure
//	   <1st field> : light pen trigger status
//	   <2nd field> : pixel column
//	   <3rd field> : Raster line (pixel row)
//	   <4th field> : light pen row
//	   <5th field> : light pen column
//
//   registers:
//       AH : Light pen trigger status
//       BX : Pixel column
//       CX : Raster line (pixel row)
//       DH : Light pen row
//       DL : Light pen column
//**************************************************************************//
void BiosLib_t::ReadPen(BiosPen_t& Info_)
{
  _InRegs.h.ah = 4;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._Trigger = _OutRegs.h.ah;
  Info_._PixCol  = _OutRegs.x.bx;
  Info_._PixRow  = _OutRegs.x.cx;
  Info_._PenRow  = _OutRegs.h.dh;
  Info_._PenCol  = _OutRegs.h.dl;
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Set active display page BIOS service (INT 10h, service 5).
//   Interface to C language calling convention.
//
// PRE:
//   page : page number of desired active display page
//
//   registers:
//       AH : 5
//       AL : Desired display page
//**************************************************************************//
void BiosLib_t::SetDisplayPage(Byte_t Pg_)
{
  _InRegs.h.ah = 5;
  _InRegs.h.al = Pg_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Scroll window up BIOS service (INT 10h, service 6).
//   Interface to C language calling convention.
//
// PRE:
//   info        : information structure
//		: <1st field> : number of lines to scroll
//		: <2nd field> : display attributes for blank lines
//		: <3rd field> : row for upper left corner of window
//		: <4th field> : column for upper left cornder of window
//		: <5th field> : row for lower right corner of window
//		: <6th field> : column for lower right corner of window
//
//   registers:
//       AH      : 6
//       AL      : number of lines to scroll
//       BH      : display attribute for blank lines
//       CH      : row for upper left corner of window
//       CL      : column for upper left corner of window
//       DH      : row for lower right corner of window
//       DL      : column for lower right corner of window
//
//**************************************************************************//
void BiosLib_t::ScrollWinUp(BiosWindow_t& Info_)
{
  _InRegs.h.ah = 6;
  _InRegs.h.al = Info_._Lines;
  _InRegs.h.bh = Info_._Disp;
  _InRegs.h.ch = Info_._Top;
  _InRegs.h.cl = Info_._Left;
  _InRegs.h.dh = Info_._Bottom;
  _InRegs.h.dl = Info_._Right;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Scroll window down BIOS service (INT 10h, service 7).
//   Interface to C language calling convention.
//
// PRE:
//   info	: information structure
//   		  <1st field> : number of lines to scroll
//   		  <2nd field> : display attribute for blank lines
//   		  <3rd field> : row for upper left corner of window
//   		  <4th field> : column for upper left corner of window
//   		  <5th field> : row for lower right corner of window
//   		  <6th field> : column for lower right corner of window
//
//   registers:
//       AH      : 7
//       AL      : number of lines to scroll
//       BH      : display attribute for blank lines
//       CH      : row for upper left corner of window
//       CL      : column for upper left corner of window
//       DH      : row for lower right corner of window
//       DL      : column for lower right corner of window
//
//**************************************************************************//
void BiosLib_t::ScrollWinDown(BiosWindow_t& Info_)
{
  _InRegs.h.ah = 7;
  _InRegs.h.al = Info_._Lines;
  _InRegs.h.bh = Info_._Disp;
  _InRegs.h.ch = Info_._Top;
  _InRegs.h.cl = Info_._Left;
  _InRegs.h.dh = Info_._Bottom;
  _InRegs.h.dl = Info_._Right;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Read character and attribute BIOS service (INT 10h, service 8).
//   Interface to C language calling convention.
//
// PRE:
//   pg          : video page number
//
//   registers:
//       AH      : 8
//       BH      : video page number
//
// POST:
//   attr	: ptr to attribute byte
//   ascode	: ptr to ASCII character code
//
//   registers:
//       AH      : Attribute byte
//       AL      : ASCII character code
//
//**************************************************************************//
Word_t BiosLib_t::ReadCharAttr(Byte_t Pg_, Byte_t& Attr_, Byte_t& AscCode_)
{
  _InRegs.h.ah = 8;
  _InRegs.h.bh = Pg_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);

  Attr_ = _OutRegs.h.ah;
  AscCode_ = _OutRegs.h.al;

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Write character and attribute BIOS service (INT 10h, service 9).
//   Interface to C language calling convention.
//
// PRE:
//   pg          : video page number
//   asch        : ASCII character code
//   attr        : attribute of character
//   count       : number of character/attribute words to display
//
//   registers:
//       AH      : 9
//       AL      : ASCII character code
//       BH      : video page number
//       BL      : video attribute of character in AL
//       CX      : Number of character/attribute words to display
//**************************************************************************//
void BiosLib_t::WriteCharAttr(Byte_t Pg_, Byte_t Ch_, Byte_t Attr_, Word_t Count_)
{
  _InRegs.h.ah = 9;
  _InRegs.h.al = Ch_;
  _InRegs.h.bh = Pg_;
  _InRegs.h.bl = Attr_;
  _InRegs.x.cx = Count_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Write character and attribute BIOS service (INT 10h, service 0Ah).
//   Interface to C language calling convention.
//
// PRE:
//   pg          : video page number
//   asch        : ASCII character code
//   attr        : attribute of character (in graphics mode only)
//   count       : number of character/attribute words to display
//
//   registers:
//       AH      : 0Ah
//       AL      : ASCII character code
//       BH      : video page number
//       BL      : video attribute of character in AL (in graphics mode only)
//       CX      : Number of character/attribute words to display
//**************************************************************************//
void BiosLib_t::WritGraphCharAttr(Byte_t Pg_, Byte_t Ch_, Byte_t Attr_, Word_t Count_)
{
  _InRegs.h.ah = 0x0A;
  _InRegs.h.al = Ch_;
  _InRegs.h.bh = Pg_;
  _InRegs.h.bl = Attr_;
  _InRegs.x.cx = Count_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Set color palette BIOS service (INT 10h, service 0Bh).
//   Interface to C language calling convention.
//
// PRE:
//   palid       : Palette ID
//   color       : Palette ID color value
//
//   registers:
//       AH      : 0Bh
//       BH      : Palette ID
//       BL      : Palette ID color value
//**************************************************************************//
void BiosLib_t::SetPalette(Byte_t Palid_, Byte_t Color_)
{
  _InRegs.h.ah = 0x0B;
  _InRegs.h.bh = Palid_;
  _InRegs.h.bl = Color_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Write pixel dot BIOS service (INT 10h, service 0Ch).
//   Interface to C language calling convention.
//
// PRE:
//   pixval      : Pixel value
//   pixcol      : Pixel column
//   pixrow      : Raster line (pixel row)
//
//   registers:
//       AH      : OCh
//       AL      : Pixel value
//       CX      : Pixel column
//       DX      : Raster line (pixel row)
//**************************************************************************//
void BiosLib_t::WritePixel(Byte_t PixVal_, Word_t PixCol_, Word_t PixRow_)
{
  _InRegs.h.ah = 0x0C;
  _InRegs.h.al = PixVal_;
  _InRegs.x.cx = PixCol_;
  _InRegs.x.dx = PixRow_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Read pixel dot BIOS service (INT 10h, service 0Dh).
//   Interface to C language calling convention.
//
// PRE:
//   pixcol      : Pixel column
//   pixrow      : Raster line (pixel row)
//
//   registers:
//       AH      : ODh
//       CX      : Pixel column
//       DX      : Raster line (pixel row)
//
// POST:
//   registers:
//       AL      : Pixel value
//**************************************************************************//
Byte_t BiosLib_t::ReadPixel(Word_t PixCol_, Word_t PixRow_)
{
  _InRegs.h.ah = 0x0D;
  _InRegs.x.cx = PixCol_;
  _InRegs.x.dx = PixRow_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.al;
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   TTY character output BIOS service (INT 10h, service 0Eh).
//   Interface to C language calling convention.
//
// PRE:
//   asch        : ASCII character code
//   pg          : video page number
//   color       : Character color (graphics foreground)
//
//   registers:
//       AH      : OEh
//       AL      : ASCII character code
//       BH      : Video page number
//       BL      : Character color (graphics foreground)
//**************************************************************************//
void BiosLib_t::WriteTTYChar(Byte_t Char_, Byte_t Pg_, Byte_t Color_)
{
  _InRegs.h.ah = 0x0E;
  _InRegs.h.al = Char_;
  _InRegs.h.bh = Pg_;
  _InRegs.h.bl = Color_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Get current video state BIOS service (INT 10h, service 0Fh).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : OEh
//
// POST:
//   data in registers are saved in info structure -- info
//
//   info	: information structure
//   		  <1st field> : Screen width
//   		  <2nd field> : Display mode
//   		  <3rd field> : Active display page
//
//   registers:
//       AH      : Screen width
//       AL      : Display mode
//       BH      : Active display page
//**************************************************************************//
void BiosLib_t::GetVideoState(BiosVideoState_t& Info_)
{
  _InRegs.h.ah = 0x0F;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._ScreenWidth = _OutRegs.h.ah;
  Info_._DisplayMode = _OutRegs.h.al;
  Info_._ActivePage  = _OutRegs.h.bh;
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA palette registers BIOS service (INT 10h, service 10h).
//   Set border color register (subfunction 01h)
//   Interface to C language calling convention.
//
// PRE:
//   color       : Color value
//
//   registers:
//	AH	: 10h
//      AL      : 01h
//      BH      : Color value
//**************************************************************************//
void BiosLib_t::SetBorder(Byte_t Color_)
{
  _InRegs.h.ah = 0x10;
  _InRegs.h.al = 0x01;
  _InRegs.h.bh = Color_;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 12h
//       BL      : (subfunction #)
//       AL      : (subfunction options)
//
// POST:
//   registers:
//       subfunction 10h (setting in BL)
//       BH      : BIOS video mode (0 color / 1 mono)
//       BL      : Size of EGA RAM (0=64K to 3=256K)
//       AL      : 12h if service is valid in all cases
//**************************************************************************//
void BiosLib_t::AltVideo()
{
  _InRegs.h.ah = 0x12;

  int86x(0x10, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
//   Return EGA information (subfunction 10h)
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 12h
//       BL      : 10h
//
// POST:
//   vidmode     : BIOS video mode (0 color / 1 mono)
//   egaram      : Size of EGA RAM (0=64K to 3=256K)
//
//   registers:
//       AH, BH  : BIOS video mode (0 color / 1 mono)
//       AL, BL  : Size of EGA RAM (0=64K to 3=256K)
//**************************************************************************//
Word_t BiosLib_t::GetEgaInfo(Word_t& VidMode_, Word_t& EgaRam_)
{
  _InRegs.h.bl = 0x10;

  AltVideo();

  VidMode_ = _OutRegs.h.bh;
  EgaRam_ = _OutRegs.h.bl;

  return _OutRegs.x.bx;
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
//   select scan lines (subfunction 30h)
//   Interface to C language calling convention.
//
// PRE:
//   lines       : number of scan lines
//
//   registers:
//       AH      : 12h
//       BL      : 30h
//       AL      : 00h 200 lines
//               : 01h 350 lines
//               : 02h 400 lines
//**************************************************************************//
void BiosLib_t::SetScanLines(Byte_t Lines_)
{
  _InRegs.h.bl = 0x30;
  _InRegs.h.al = Lines_;

  AltVideo();
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
//   load palette when setting mode (subfunction 31h)
//   Interface to C language calling convention.
//
// PRE:
//   flg         : 00h = enable, 01h = disable
//
//   registers:
//       AH      : 12h
//       BL      : 31h
//       AL      : enable=00h / disable=01h
//**************************************************************************//
void BiosLib_t::LoadPalette(Byte_t Flag_)
{
  _InRegs.h.bl = 0x31;
  _InRegs.h.al = Flag_;

  AltVideo();
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
//   Video on/off (subfunction 32h)
//   Interface to C language calling convention.
//
// PRE:
//   flg         : 00h = enable, 01h = disable
//
//   registers:
//       AH      : 12h
//       BL      : 32h
//       AL      : enable=00h / disable=01h
//**************************************************************************//
void BiosLib_t::ToggleVideo(Byte_t Flag_)
{
  _InRegs.h.bl = 0x32;
  _InRegs.h.al = Flag_;

  AltVideo();
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
//   Sum color to gray scale (subfunction 33h)
//   Interface to C language calling convention.
//
// PRE:
//   flg         : 00h = enable, 01h = disable
//
//   registers:
//       AH      : 12h
//       BL      : 33h
//       AL      : enable=00h / disable=01h
//**************************************************************************//
void BiosLib_t::SelectGrayScale(Byte_t Flag_)
{
  _InRegs.h.bl = 0x33;
  _InRegs.h.al = Flag_;

  AltVideo();
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
//   Cursor emulation on/off (subfunction 34h)
//   Interface to C language calling convention.
//
// PRE:
//   flg         : 00h = enable, 01h = disable
//
//   registers:
//       AH      : 12h
//       BL      : 34h
//       AL      : enable=00h / disable=01h
//**************************************************************************//
void BiosLib_t::ToggleCursorEmulation(Byte_t Flag_)
{
  _InRegs.h.bl = 0x34;
  _InRegs.h.al = Flag_;

  AltVideo();
}

//**************************************************************************//
// CATEGORY:     Video Services
//
// PURPOSE:
//   Write string BIOS service (INT 10h, service 13h).
//   Interface to C language calling convention.
//
// PRE:
//   info	: information structure
//   		  <1st field> : msg
//   		  <2nd field> : video mode
//		  <3rd field> : video page number
//		  <4th field> : character attribute
//		  <5th field> : length of string
//		  <6th field> : cursor row where string is to be displayed
//		  <7th field> : cursor column where string is to be displayed
//		  <8th field> : offset address of string
//		  <9th field> : segment address of string
//
//   registers:
//       AH      : 13h
//       AL      : video mode
//       BH      : video page number
//       BL      : character attribute (depending on AL)
//       CX      : length of string
//       DH      : cursor row where string is to be displayed
//       DL      : cursor column where string is to be displayed
//       BP      : offset address of string
//       ES      : segment address of string
//**************************************************************************//
void BiosLib_t::WriteString(BiosString_t& Info_)
{
  _RegPack.r_ax = (0x13 << 8) | Info_._VideoMode;
  _RegPack.r_bx = (Info_._Page << 8) | Info_._CharAttr;
  _RegPack.r_cx = Info_._Length;
  _RegPack.r_dx = (Info_._Row << 8) | Info_._Column;

  _RegPack.r_si = FP_OFF(Info_._Message);
  _SegRegs.es = _SegRegs.ds = FP_SEG(Info_._Message);
  _RegPack.r_bp = FP_OFF(MK_FP(_SegRegs.es, _RegPack.r_si));

  intr(0x10, &_RegPack);
}

//**************************************************************************//
// CATEGORY:     System Services
//
// PURPOSE:
//   Get equipment status BIOS service (INT 11h).
//   Interface to C language calling convention.
//
// POST:
//   registers:
//       AX      : Equipment status
//
// Bit meanings for equipment status word returned by interrupt 11h.
//
// Bits
// FEDCBA98      76543210        Meaning of bits
// *************************************--------
// xx                            Number of printers attached
//   x                           Not used
//    0                          Game adapter not installed
//    1                          Game adapted installed
//     xxx                       Number of serial cards attached
//        x                      Not used
//               00              1 disk drive attached (if bit 0=1)
//               01              2 disk drives attached (if bit 0=1)
//               10              3 disk drives attached (if bit 0=1)
//               11              4 disk drives attached (if bit 0=1)
//                 01            Initial video mode -- 40 x 25 BW/color card
//                 10            Initial video mode -- 80 x 25 BW/color card
//                 11            Initial video mode -- 80 x 25 BW/mono card
//                   00          16K system board RAM
//                   01          32K system board RAM
//                   10          48K system board RAM
//                   11          64K system board RAM
//                     0         Math coprocessor not installed
//                     1         Math coprocessor installed
//                      0        No disk drives installed (bits 6-7 insignificant)
//                      0        Disk drives installed (bits 6-7 significant)
//**************************************************************************//
Word_t BiosLib_t::GetEquipStatus()
{
  int86x(0x11, &_InRegs, &_OutRegs, &_SegRegs);
  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     System Services
//
// PURPOSE:
//   Get memory size BIOS service (INT 12h).
//   Interface to C language calling convention.
//
// POST:
//   registers:
//       AX      : Memory blocks
//
//**************************************************************************//
Word_t BiosLib_t::GetMemorySize()
{
  int86x(0x12, &_InRegs, &_OutRegs, &_SegRegs);
  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Disk Services
//
// PURPOSE:
//   Get floppy disk status BIOS service (INT 13h, service 01h).
//   Interface to C language calling convention.
//
// PRE:
//   drvnum      : Drive number
//
//   registers:
//       AH      : 1
//       DL      : Drive number (0 bit based),
//                 bit 7 = 0 for a diskette
//
// POST:
//   registers:
//       AH      : Status byte
//
// Meaning of the status byte returned by service 13/1
//
// Bits
// 76543210      Hex     Decimal         Meaning of bits
// *************************************----------------
// 1             80      128             Time out - drive did not respond
//  1            40      64              Seek failure - couldn't move to
//                                       requested track
//   1           20      32              Controller malfunction
//    1          10      16              Bad CRC detected on disk read
//     1  1      9       9               DMA error - 64L boundary crossed
//     1         8       8               DMA overrun
//      1        4       4               Bad sector / sector not found
//       11      3       3               Write protect error
//       1       2       2               Bad sector ID (address) mark
//        1      1       1               Bad command
//**************************************************************************//
Byte_t BiosLib_t::GetDiskStatus(Byte_t DriveNum_)
{
  _InRegs.h.ah = 1;
  _InRegs.h.dl = DriveNum_;

  int86x(0x13, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     Disk Services
//
// PURPOSE:
//   Get floppy disk status BIOS service (INT 13h, service 08h).
//   Interface to C language calling convention.
//
// PRE:
//   drvnum      : Drive number
//
//   registers:
//       AH      : 8
//       DL      : Drive number (0 bit based),
//                 bit 7 = 1 for a fixed disk
//
// POST:
//   info	: information structure
//		: <1st field> : number of tracks/side
//		: <2nd field> : number of sectors/track
//		: <3rd field> : number of sides
//		: <4th field> : number of consecutive drives attached
//
//   registers:
//       CH    	: Number of tracks/side
//       CL  	: Number sector/track
//       DH     : Number of sides
//       DL     : Number of consecutive drives attached
//**************************************************************************//
void BiosLib_t::GetDriveParameters(Byte_t DriveNum_, BiosDriveStatus_t& Info_)
{
  _InRegs.h.ah = 8;
  _InRegs.h.dl = DriveNum_;

  int86x(0x13, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._TracksPerSide 		= _OutRegs.h.ch;
  Info_._SectorsPerTrack 	= _OutRegs.h.cl;
  Info_._Sides 			= _OutRegs.h.dh;
  Info_._DrivesAttached 	= _OutRegs.h.dl;
}

//**************************************************************************//
// CATEGORY:     Disk Services
//
// PURPOSE:
//   Park heads BIOS service (INT 13h, service 19h).
//   Interface to C language calling convention.
//
// PRE:
//   drvnum      : Drive number
//
//   registers:
//       AH      : 19h
//       DL      : Drive number (0 bit based),
//                 bit 7 = 1 for a fixed disk
//**************************************************************************//
void BiosLib_t::ParkHeads(Byte_t DriveNum_)
{
  _InRegs.h.ah = 0x19;
  _InRegs.h.dl = DriveNum_;

  int86x(0x13, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Communications Services
//
// PURPOSE:
//   Initialize communications port BIOS service (INT 14h, service 0).
//   Interface to C language calling convention.
//
// PRE:
//   iparm       : initialization parameter
//   port        : com port
//
//   registers:
//       AH      : 0
//       AL      : Initialization parameter
//       DX      : com port
//
// Meaning of AL bits for initialization parameter
//
// Bits
// 76543210      Meaning
// ---------------------
// 000           110 baud
// 001           150 baud
// 010           300 baud
// 011           600 baud
// 100           1200 baud
// 101           2400 baud
// 110           4800 baud
// 111           9600 baud
//    00         No parity
//    01         Odd parity
//    10         No parity
//    11         Even parity
//      0        1 stopbit
//      1        2 stopbit
//       10      7-bit data length
//       11      8-bit data length
//
// POST:
//   linestat	: line status
//   modstat	: modem status
//
//   registers:
//       AH      : Line status
//       AL      : Modem status
//
// Meaning of AH bits for line status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Time-out error
//  1            Transfer shift register (TSR) empty
//   1           Transfer holding register (THR) empty
//    1          Break interrupt detected
//     1         Framing error
//      1        Parity error
//       1       Overrun error
//        1      Data ready
//
// Meaning of AL bits for modem status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Receive line-signal detect
//  1            Ring indicator
//   1           Data set ready (DSR)
//    1          Clear to send (CTS)
//     1         Delta receive line-signal detect
//      1        Trailing-edge ring detected
//       1       Delta data-set ready (DDSR)
//        1      Delta clear to send (DCTS)
//**************************************************************************//
Word_t BiosLib_t::InitCommPort(Byte_t InitParm_, Word_t Port_, BiosCommStatus_t& Info_)
{
  _InRegs.h.al = InitParm_;
  _InRegs.x.dx = Port_;
  _InRegs.h.ah = 0;

  int86x(0x14, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._LineStatus = _OutRegs.h.ah;
  Info_._ModemStatus = _OutRegs.h.al;

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Communications Services
//
// PURPOSE:
//   Transmit character BIOS service (INT 14h, service 01h).
//   Interface to C language calling convention.
//
// PRE:
//   asch        : ASCII character
//   port        : com port
//
//   registers:
//       AH      : 1
//       AL      : ASCII character
//       DX      : com port
//
// POST:
//   registers:
//       AH      : Return code
//                 If successful then (AH < 80h)
//                 If unsuccessful then (AH > 7Fh)
//
// Meaning of AH bits for line status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Time-out error
//  1            Transfer shift register (TSR) empty
//   1           Transfer holding register (THR) empty
//    1          Break interrupt detected
//     1         Framing error
//      1        Parity error
//       1       Overrun error
//        1      Data ready
//**************************************************************************//
Byte_t BiosLib_t::SendChar(Byte_t Char_, Word_t Port_)
{
  _InRegs.h.al = Char_;
  _InRegs.x.dx = Port_;
  _InRegs.h.ah = 1;

  int86x(0x14, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     Communications Services
//
// PURPOSE:
//   Receive character BIOS service (INT 14h, service 02h).
//   Interface to C language calling convention.
//
// PRE:
//   port        : com port
//
//   registers:
//       AH      : 2
//       DX      : com port
//
// POST:
//   registers:
//       AH      : Return code
//                 If successful then (AH < 80h)
//                 If unsuccessful then (AH > 7Fh)
//
// Meaning of AH bits for line status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Time-out error
//  1            Transfer shift register (TSR) empty
//   1           Transfer holding register (THR) empty
//    1          Break interrupt detected
//     1         Framing error
//      1        Parity error
//       1       Overrun error
//        1      Data ready
//**************************************************************************//
Byte_t BiosLib_t::ReceiveChar(Word_t Port_)
{
  _InRegs.h.ah = 2;
  _InRegs.x.dx = Port_;

  int86x(0x14, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     Communications Services
//
// PURPOSE:
//   Get communications port status BIOS service (INT 14h, service 03h).
//   Interface to C language calling convention.
//
// PRE:
//   port        : com port
//
//   registers:
//       AH      : 3
//       DX      : com port
//
// POST:
//   linestat	: line status
//   modstat	: modem status
//
//   registers:
//       AH      : Line status
//       AL      : Modem status
//
// Meaning of AH bits for line status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Time-out error
//  1            Transfer shift register (TSR) empty
//   1           Transfer holding register (THR) empty
//    1          Break interrupt detected
//     1         Framing error
//      1        Parity error
//       1       Overrun error
//        1      Data ready
//
// Meaning of AL bits for modem status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Receive line-signal detect
//  1            Ring indicator
//   1           Data set ready (DSR)
//    1          Clear to send (CTS)
//     1         Delta receive line-signal detect
//      1        Trailing-edge ring detected
//       1       Delta data-set ready (DDSR)
//        1      Delta clear to send (DCTS)
//**************************************************************************//
Word_t BiosLib_t::GetComStatus(Word_t Port_, BiosCommStatus_t& Info_)
{
  _InRegs.h.ah = 3;
  _InRegs.x.dx = Port_;

  int86x(0x14, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._LineStatus = _OutRegs.h.ah;
  Info_._ModemStatus = _OutRegs.h.al;

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Communications Services
//
// PURPOSE:
//   Extended communications port initialization BIOS service (INT 14h, service 04h).
//   Interface to C language calling convention.
//
// PRE:
//   info	: information structure
//		: <1st field> : com port
//		: <2nd field> : break flag
//		: <3rd field> : parity flag
//		: <4th field> : stop bits
//		: <5th field> : data bits
//		: <6th field> : bits per second
//
//   registers:
//       AH      : 4
//       AL      : 00h   = no break
//               : 01h   = break
//       BH      : 00h   = no parity
//               : 01h   = odd parity
//               : 02h   = even parity
//               : 03h   = mark parity
//               : 04h   = space parity
//       BL      : 00h   = one stop bit
//               : 01h   = two stop bits
//       CH      : 00h   = 5 data bits
//               : 01h   = 6 data bits
//               : 02h   = 7 data bits
//               : 03h   = 8 data bits
//       CL      : 00h   = 110 BPS
//               : 01h   = 150 BPS
//               : 02h   = 300 BPS
//               : 03h   = 600 BPS
//               : 04h   = 1200 BPS
//               : 05h   = 2400 BPS
//               : 06h   = 4800 BPS
//               : 07h   = 9600 BPS
//               : 08h   = 19200 BPS
//       DX      : com port
//
// POST:
//   linestat	: line status
//   modstat	: modem status
//
//   registers:
//       AH      : Line status
//       AL      : Modem status
//
// Meaning of AH bits for line status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Time-out error
//  1            Transfer shift register (TSR) empty
//   1           Transfer holding register (THR) empty
//    1          Break interrupt detected
//     1         Framing error
//      1        Parity error
//       1       Overrun error
//        1      Data ready
//
// Meaning of AL bits for modem status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Receive line-signal detect
//  1            Ring indicator
//   1           Data set ready (DSR)
//    1          Clear to send (CTS)
//     1         Delta receive line-signal detect
//      1        Trailing-edge ring detected
//       1       Delta data-set ready (DDSR)
//        1      Delta clear to send (DCTS)
//**************************************************************************//
Word_t BiosLib_t::ExtInitCommPort(BiosCommInfo_t& Input_, BiosCommStatus_t& Output_)
{
  _InRegs.h.ah = 4;
  _InRegs.h.al = Input_._ComPort;
  _InRegs.h.bh = Input_._BreakFlag;
  _InRegs.h.bl = Input_._ParityFlag;
  _InRegs.h.ch = Input_._StopBits;
  _InRegs.h.cl = Input_._DataBits;
  _InRegs.x.dx = Input_._Bps;

  int86x(0x14, &_InRegs, &_OutRegs, &_SegRegs);

  Output_._LineStatus = _OutRegs.h.ah;
  Output_._ModemStatus = _OutRegs.h.al;

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Communications Services
//
// PURPOSE:
//   Extended communications port control BIOS service (INT 14h, service 05h).
//   Interface to C language calling convention.
//
// PRE:
//   rdwr        : Read/Write status
//   mcr         : New mcr content if write enabled
//   port        : Serial port
//
//   registers:
//       AH      : 05h
//       AL      : 00h, Read modem control register
//               : 01h, Write modem control register
//       BL      : New MCR content if AL = 01h
//       DX      : Serial port (0 = COM1, etc.)
//
// POST:
//   linestat	: line status
//   modstat	: modem status
//   modctrl	: modem control
//
//   registers:
//       AH      : Line status
//       AL      : Modem status
//       DL, BL  : Modem control register
//
// Meaning of BL bits for modem control
//
// Bits
// 76543210      Meaning
// ---------------------
//        1      Data terminal ready (DTR)
//       1       Request to send (RTS)
//      1        User 1
//     1         User 2
//    1          Loopback test
// xxx           Reserved
//
// Meaning of AH bits for line status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Time-out error
//  1            Transfer shift register (TSR) empty
//   1           Transfer holding register (THR) empty
//    1          Break interrupt detected
//     1         Framing error
//      1        Parity error
//       1       Overrun error
//        1      Data ready
//
// Meaning of AL bits for modem status
//
// Bits
// 76543210      Meaning
// ---------------------
// 1             Receive line-signal detect
//  1            Ring indicator
//   1           Data set ready (DSR)
//    1          Clear to send (CTS)
//     1         Delta receive line-signal detect
//      1        Trailing-edge ring detected
//       1       Delta data-set ready (DDSR)
//        1      Delta clear to send (DCTS)
//**************************************************************************//
void BiosLib_t::ExtCommCtrl(Byte_t RdWr_, Byte_t Mcr_, Byte_t Port_, BiosCommStatus_t& Info_)
{
  _InRegs.h.ah = 5;
  _InRegs.h.al = RdWr_;
  _InRegs.h.bl = Mcr_;
  _InRegs.x.dx = Port_;

  int86x(0x14, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._LineStatus = _OutRegs.h.ah;
  Info_._ModemStatus = _OutRegs.h.al;
  Info_._ModemControl = _OutRegs.h.bl;
}

//**************************************************************************//
// CATEGORY:     Miscellaneous Services
//
// PURPOSE:
//   Joystick support BIOS service (INT 15h, service 84h).
//   Interface to C language calling convention.
//
// PRE:
//   rdflg       : 00h = Read switches
//               : 01h = Read joystick position
//
//   registers:
//       AH      : 84h
//       DX      : 00h = Read switches
//               : 01h = Read joystick position
//
// POST:
//   info        : information structure
//		: <1st field> : switch settings
//		: <2nd field> : A(X) value
//		: <3rd field> : A(Y) value
//		: <4th field> : B(X) value
//		: <5th field> : B(Y) value
//
//   registers:
//
//   The joystick data is saved in information structure -- info
//
//   if reading switches (DX=0):
//       AL = Switch settings (bits 4-7)
//   if reading position (DX=1):
//       AX = A(X) value
//       BX = A(Y) value
//       CX = B(X) value
//       DX = B(Y) value
//**************************************************************************//
void BiosLib_t::GetJoystick(Word_t ReadFlag_, BiosJoystick_t& Info_)
{
  _InRegs.h.ah = 0x84;
  _InRegs.x.dx = ReadFlag_;

  int86x(0x15, &_InRegs, &_OutRegs, &_SegRegs);

  if (ReadFlag_)
  {
    Info_._Xpos1 = _OutRegs.x.ax;
    Info_._Ypos1 = _OutRegs.x.bx;
    Info_._Xpos2 = _OutRegs.x.cx;
    Info_._Ypos2 = _OutRegs.x.dx;
  }
  else
    Info_._Switch = _OutRegs.h.al;
}

//**************************************************************************//
// CATEGORY:     Miscellaneous Services
//
// PURPOSE:
//   Move block to/from extended memory BIOS service (INT 15h, service 87h).
//   Interface to C language calling convention.
//
// PRE:
//   numwords    : number of words to move
//   segaddr     : segment address of GDT
//   offaddr     : Offset address of GDT
//
//   registers:
//       AH      : 87h
//       CX      : Number of words to move
//       ES      : Segment address of GDT
//       SI      : Offset address of GDT
//
// POST:
//   registers:
//       AH      : Return code
//
// Format of global descriptor table
//
// Offset        Description
// -------------------------
// 00h           Dummy, set to zero
// 08h           GDT data-segment location, set to zero
// 10h           Source GDT, points to 8-byte GDT for source memory block
// 18h           Target GDT, points to 8-byte GDT for target memory block
// 20h           Pointer to BIOS code segment, initially zero
// 28h           Pointer to BIOS stack segment, initially zero
//
//                       source/target GDT layouts
//
// 00h           Segment limit
// 02h           24-bit segment physical address
// 05h           Data access rights
// 06h           Reserved word, must be zero
//**************************************************************************//
Byte_t BiosLib_t::ExtMemBlockMove(Word_t NumWords_, Word_t SegAddr_, Word_t OffAddr_)
{
  _InRegs.h.ah = 0x87;
  _InRegs.x.cx = NumWords_;
  _InRegs.x.si = OffAddr_;
  _SegRegs.es = SegAddr_;

  int86x(0x15, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     Miscellaneous Services
//
// PURPOSE:
//   Size extended memory BIOS service (INT 15h, service 88h).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 88h
//
// POST:
//   registers:
//       AX      : Number of contiguous 1K blocks of RAM above 100000h
//**************************************************************************//
Word_t BiosLib_t::GetExtMemSize()
{
  _InRegs.h.ah = 0x88;

  int86x(0x15, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Miscellaneous Services
//
// PURPOSE:
//   Protected mode switch BIOS service (INT 15h, service 88h).
//   Interface to C language calling convention.
//
// PRE:
//   info	: information structure
//		: <1st field> : IRQ0 vector offset
//		: <2nd field> : IRQ8 vector offset
//		: <3rd field> : Offset into protected-mode CS to jump to
//		: <4th field> : Segment address of GDT
//		: <5th field> : Offset address of GDT
//
//   registers:
//       AH      : 89h
//       BL      : IRQ0 vector offset
//       BH      : IRQ8 vector offset
//       CX      : Offset into protected-mode CS to jump to
//       ES      : Segment address of GDT
//       SI      : Offset address of GDT
//
// POST:
//   registers:
//       AH      : Return code
//
// Format of global descriptor table
//
// Offset        Description
// -------------------------
// 00h           Dummy, set to zero
// 08h           GDT data-segment location, set to zero
// 10h           Source GDT, points to 8-byte GDT for source memory block
// 18h           Target GDT, points to 8-byte GDT for target memory block
// 20h           Pointer to BIOS code segment, initially zero
// 28h           Pointer to BIOS stack segment, initially zero
//
//                       source/target GDT layouts
//
// 00h           Segment limit
// 02h           24-bit segment physical address
// 05h           Data access rights
// 06h           Reserved word, must be zero
//**************************************************************************//
Byte_t BiosLib_t::SetProtMode(BiosProtMode_t& Info_)
{
  _InRegs.h.ah = 0x89;
  _InRegs.h.bl = Info_._IRQ0off;
  _InRegs.h.bh = Info_._IRQ8off;
  _InRegs.x.cx = Info_._CSoff;
  _InRegs.x.si = Info_._GDToff;
  _SegRegs.es  = Info_._GDTseg;

  int86x(0x15, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     PS/2 Services
//
// PURPOSE:
//   Get system configuration BIOS service (INT 15h, service C0h).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : C0h
//
// POST:
//   syscfgoff   : Offset address of system-configuration table
//   syscfgseg   : Segment address of system-configuration table
//
//   registers:
//       AX, BX  : Offset address of system-configuration table
//       DX, ES  : Segment address of system-configuration table
//
// System descriptor table
//
// Offset        Meaning
// ---------------------
// 00h           Byte count of subsequent data (minimum 8)
// 02h           Model byte (See system model table)
// 03h           Submodel byte (See system model table)
// 04h           BIOS revision level (00 = first release)
// 05h           Feature information (See feature info table)
// 06-09h        Reserved
//
// Feature information table
//
// Bits
// 76543210      Hex     Decimal         Meaning of bits
// *************************************----------------
// 1             80      128             DMA channel 3 used by hard disk BIOS
//  1            40      64              Second interrupt chip present
//   1           20      32              Real-time clock present
//    1          10      16              Keyboard intercept called by INT 09h
//     1         8       8               Wait for external event is supported
//      1        4       4               Extended BIOS data area allocated
//       1       2       2               Micro channel architecture
//       0                               PC bus I/O channel
//        x                              Reserved
//
// System model identification
//
//                 Model                         BIOS
//                 Byte          Submodel        Revision
// Computer        (Offset       (Offset         (Offset         BIOS
// Type             02h)         03h)             04h)           Date
// *************************************-----------------------------
// PC              FFh
// PC XT           FEh
// PC XT           FBh           00h             01h             1/10/86
// PC XT           FBh           00h             02h             5/09/86
// PCjr            FDh
// AT              FCh
// AT              FCh           00h             01h             6/10/85
// AT, COMPAQ 286  FCh           01h             00h             11/15/85
// PC XT 286       FCh           02h             00h
// PC Convertible  F9h           00h             00h
// PS/2 Model 30   FAh           00h             00h
// PS/2 Model 50   FCh           04h             00h
// PS/2 Model 60   FCh           05h             00h
// PS/2 Model 80   F8h           00h             00h
//**************************************************************************//
void BiosLib_t::GetSysConfig(Word_t& SysCfgOff_, Word_t& SysCfgSeg_)
{
  _RegPack.r_ax = 0xC0 << 8;

  intr(0x15, &_RegPack);

  SysCfgOff_ = _RegPack.r_bx;
  SysCfgSeg_ = _RegPack.r_es;
}

//**************************************************************************//
// CATEGORY:     PS/2 Services
//
// PURPOSE:
//   Get extended BIOS address BIOS service (INT 15h, service C1h).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : C1h
//
// POST:
//   retcode	: return code
//   extaddr	: Extended BIOS Data Area's segment address
//
//   registers:
//       AH      : Return code
//       DX, ES  : Extended BIOS Data Area's segment address
//**************************************************************************//
void BiosLib_t::GetExtBiosAddr(Byte_t& RetCode_, Word_t& ExtBiosAddr_)
{
  _RegPack.r_ax = 0xC1 << 8;

  intr(0x15, &_RegPack);

  RetCode_ = (_RegPack.r_ax) >> 8;
  ExtBiosAddr_ = _RegPack.r_es;
}

//**************************************************************************//
// CATEGORY:     PS/2 Services
//
// PURPOSE:
//   Programmable option select BIOS service (INT 15h, service C4h).
//   Interface to C language calling convention.
//
// PRE:
//   option      : Programmable option
//
//   registers:
//       AH      : C4h
//       AL      : 00h = Return base POS register address
//               : 01h = Enable slot for setup
//               : 02h = Enable adapter
//
// POST:
//   basepos	: Base POS register port address (if AL=0)
//   slotnum	: Slot number (if AL=0)
//
//   registers:
//       DL      : Base POS register port address (if AL=0)
//       AL, BL  : Slot number (if AL=0)
//**************************************************************************//
void BiosLib_t::ProgramOptionSelect(Byte_t Option_, Byte_t& BasePOSaddr_, Byte_t& SlotNum_)
{
  _InRegs.h.al = Option_;
  _InRegs.h.ah = 0xC4;

  int86x(0x15, &_InRegs, &_OutRegs, &_SegRegs);

  BasePOSaddr_ = _OutRegs.h.dl;
  SlotNum_ = _OutRegs.h.bl;
}

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Read keyboard character BIOS service (INT 16h, service 0).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 0
//
// POST:
//   scancode	: Keyboard scan code
//   asciicode	: ASCII value of keystroke
//
//   registers:
//       AH      : Keyboard scan code
//       AL      : ASCII value of keystroke
//**************************************************************************//
Word_t BiosLib_t::ReadKeyboardChar(Byte_t& ScanCode_, Byte_t& AsciiCode_)
{
  _InRegs.h.ah = 0;

  int86x(0x16, &_InRegs, &_OutRegs, &_SegRegs);

  ScanCode_ = _OutRegs.h.ah;
  AsciiCode_ = _OutRegs.h.al;

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Read keyboard status BIOS service (INT 16h, service 1).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 1
//
// POST:
//   scancode	: Keyboard scan code
//   asciicode	: ASCII value of keystroke
//
//   registers:
//       AH      : Keyboard scan code
//       AL      : ASCII value of keystroke
//
//   flags:
//       zero flag cleared if key is available
//       zero flag set if key is unavailable
//**************************************************************************//
Word_t BiosLib_t::ReadKeyboardStatus(Byte_t& ScanCode_, Byte_t& AsciiCode_)
{
  _RegPack.r_ax = 1 << 8;

  intr(0x16, &_RegPack);

  if (_RegPack.r_flags & 64) // 6th bit of flags register is zero flag
    ScanCode_ = AsciiCode_ = _OutRegs.x.ax = 0;
  else
  {
    ScanCode_ = _OutRegs.h.ah;
    AsciiCode_ = _OutRegs.h.al;
  }

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Read keyboard shift status BIOS service (INT 16h, service 2).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 2
//
// POST:
//   registers:
//       AL      : Shift status
//
// Keyboard shift-status values
//
// Bits
// 76543210      Hex     Decimal         Meaning of bits
// *************************************----------------
// 1             80      128             Insert on
//  1            40      64              Caps lock on
//   1           20      32              Num lock on
//    1          10      16              Scroll lock on
//     1         8       8               Alt key down
//      1        4       4               Ctrl key down
//       1       2       2               Left shift key down
//        1      1       1               Right shift key down
//**************************************************************************//
Byte_t BiosLib_t::ReadKeyboardShift()
{
  _InRegs.h.ah = 2;

  int86x(0x16, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.al;
}

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Adjust repeat rate BIOS service (INT 16h, service 3).
//   Interface to C language calling convention.
//
// PRE:
//   delay       : repeat delay
//   rate        : repeat rate
//
//   registers:
//       AH      : 3
//       AL      : 05h
//       BH      : Repeat delay (0-3 x 250 ms)
//       BL      : Repeat rate (0-31, lower values are faster)
//**************************************************************************//
void BiosLib_t::SetRepRate(Byte_t Delay_, Byte_t Rate_)
{
  _InRegs.h.ah = 3;
  _InRegs.h.al = 0x05;
  _InRegs.h.bh = Delay_;
  _InRegs.h.bl = Rate_;

  int86x(0x16, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Write to keyboard buffer BIOS service (INT 16h, service 5).
//   Interface to C language calling convention.
//
// PRE:
//   scan        : Scan code to write
//   ascii       : ASCII code to write
//
//   registers:
//       AH      : 5
//       CH      : Scan code to write
//       CL      : ASCII code to write
//
// POST:
//   registers:
//       AL      : 01h if buffer is full
//**************************************************************************//
Byte_t BiosLib_t::WriteKeyBuffer(Byte_t ScanCode_, Byte_t AsciiCode_)
{
  _InRegs.h.ah = 5;
  _InRegs.h.ch = ScanCode_;
  _InRegs.h.cl = AsciiCode_;

  int86x(0x16, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.al;
}

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
Word_t BiosLib_t::ReadExtKeyboardChar(Byte_t& ScanCode_, Byte_t& AsciiCode_)
{
  _InRegs.h.ah = 0x10;

  int86x(0x16, &_InRegs, &_OutRegs, &_SegRegs);

  ScanCode_ = _OutRegs.h.ah;
  AsciiCode_ = _OutRegs.h.al;

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Check extended keyboard status BIOS service (INT 16h, service 11h).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 11h
//
// POST:
//   scan        : Scan code to write
//   ascii       : ASCII code to write
//
//   registers:
//       AH      : Keyboard scan code
//       AL      : ASCII value of keystroke
//**************************************************************************//
Word_t BiosLib_t::ReadExtKeyboardStatus(Byte_t& ScanCode_, Byte_t& AsciiCode_)
{
  _RegPack.r_ax = 0x11 << 8;

  intr(0x16, &_RegPack);

  if (_RegPack.r_flags & 64) // 6th bit of flags register is zero flag
    ScanCode_ = AsciiCode_ = _OutRegs.x.ax = 0;
  else
  {
    ScanCode_ = _OutRegs.h.ah;
    AsciiCode_ = _OutRegs.h.al;
  }

  return _OutRegs.x.ax;
}

//**************************************************************************//
// CATEGORY:     Keyboard Services
//
// PURPOSE:
//   Read extended keyboard shift status BIOS service (INT 16h, service 2).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 2
//
// POST:
//   registers:
//       AL      : Shift status
//
// Keyboard shift-status values
//
// Bits
// 76543210      Hex     Decimal         Meaning of bits
// *************************************----------------
// 1             80      128             Insert on
//  1            40      64              Caps lock on
//   1           20      32              Num lock on
//    1          10      16              Scroll lock on
//     1         8       8               Alt key down
//      1        4       4               Ctrl key down
//       1       2       2               Left shift key down
//        1      1       1               Right shift key down
//**************************************************************************//
Byte_t BiosLib_t::ReadExtKeyboardShift()
{
  _InRegs.h.ah = 0x12;

  int86x(0x16, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.al;
}

//**************************************************************************//
// CATEGORY:     Printer Services
//
// PURPOSE:
//   Print char BIOS service (INT 17h, service 0).
//   Interface to C language calling convention.
//
// PRE:
//   asch        : character to print
//   printer     : printer to be used
//
//   registers:
//       AH      : 0
//       AL      : Character to print
//       DX      : Printer to be used
//
// POST:
//   registers:
//       AH      : Printer status
//
// Meaning of bits returned in AH for service 17/0, 17/1, and 17/2
//
// Bits
// 76543210      Meaning of bits
// -----------------------------
// 1             Printer not busy
// 0             Printer busy
//  1            Printer acknowledgment
//   1           Out of paper
//    1          Printer selected
//     1         I/O error
//      ??       Unused
//        1      Time-out
//**************************************************************************//
Byte_t BiosLib_t::PrintChar(Byte_t Char_, Word_t Printer_)
{
  _InRegs.h.ah = 0;
  _InRegs.h.al = Char_;
  _InRegs.x.dx = Printer_;

  int86x(0x17, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     Printer Services
//
// PURPOSE:
//   Initialize printer BIOS service (INT 17h, service 1).
//   Interface to C language calling convention.
//
// PRE:
//   printer     : printer to be used
//
//   registers:
//       AH      : 1
//       DX      : Printer to be used
//
// POST:
//   registers:
//       AH      : Printer status
//
// Meaning of bits returned in AH for service 17/0, 17/1, and 17/2
//
// Bits
// 76543210      Meaning of bits
// -----------------------------
// 1             Printer not busy
// 0             Printer busy
//  1            Printer acknowledgment
//   1           Out of paper
//    1          Printer selected
//     1         I/O error
//      ??       Unused
//        1      Time-out
//**************************************************************************//
Byte_t BiosLib_t::InitPrinter(Word_t Printer_)
{
  _InRegs.h.ah = 1;
  _InRegs.x.dx = Printer_;

  int86x(0x17, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     Printer Services
//
// PURPOSE:
//   Get printer status BIOS service (INT 17h, service 2).
//   Interface to C language calling convention.
//
// PRE:
//   printer     : printer to be used
//
//   registers:
//       AH      : 2
//       DX      : Printer to be used
//
// POST:
//   registers:
//       AH      : Printer status
//
// Meaning of bits returned in AH for service 17/0, 17/1, and 17/2
//
// Bits
// 76543210      Meaning of bits
// -----------------------------
// 1             Printer not busy
// 0             Printer busy
//  1            Printer acknowledgment
//   1           Out of paper
//    1          Printer selected
//     1         I/O error
//      ??       Unused
//        1      Time-out
//**************************************************************************//
Byte_t BiosLib_t::GetPrinterStatus(Word_t Printer_)
{
  _InRegs.h.ah = 2;
  _InRegs.x.dx = Printer_;

  int86x(0x17, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.h.ah;
}

//**************************************************************************//
// CATEGORY:     System Services
//
// PURPOSE:
//   Warm boot BIOS service (INT 19h).
//   Interface to C language calling convention.
//**************************************************************************//
void BiosLib_t::WarmBoot()
{
  int86x(0x19, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Get clock counter BIOS service (INT 1Ah, service 0).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//       AH      : 0
//
// POST:
//   info        : information structure
//		: <1st field> : Midnight flag
//		: <2nd field> : Clock count high-order word
//		: <3rd field> : Clock count low-order word
//
//   registers:
//       AL      : Midnight flag
//       CX      : Clock count high-order word
//       DX      : Clock count low-order word
//**************************************************************************//
void BiosLib_t::GetClockCount(BiosClockCount_t& Info_)
{
  _InRegs.h.ah = 0;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._Midnight = _OutRegs.h.al;
  Info_._ClockHiWord = _OutRegs.x.cx;
  Info_._ClockLoWord = _OutRegs.x.dx;
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Set clock counter BIOS service (INT 1Ah, service 1).
//   Interface to C language calling convention.
//
// PRE:
//   clkhi	: Clock count high-order word
//   clklo	: Clock count low-order word
//
//   registers:
//       AH      : 1
//       CX      : Clock count high-order word
//       DX      : Clock count low-order word
//**************************************************************************//
void BiosLib_t::SetClockCount(Word_t ClockHi_, Word_t ClockLo_)
{
  _InRegs.x.cx = ClockHi_;
  _InRegs.x.dx = ClockLo_;
  _InRegs.h.ah = 1;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Read real-time clock BIOS service (INT 1Ah, service 2).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//	AH	: 2
//
// POST:
//   info        : information structure
//		: <1st field> : Hours (BCD)
//		: <2nd field> : Minutes (BCD)
//		: <3rd field> : Seconds (BCD)
//
//   registers:
//	AH, CH	: Hours (BCD)
//	AL, CL	: Minutes (BCD)
//	DH	: Seconds (BCD)
//**************************************************************************//
void BiosLib_t::ReadRealClock(BiosRealClock_t& Info_)
{
  _InRegs.h.ah = 2;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._Hours = _OutRegs.h.ch;
  Info_._Minutes = _OutRegs.h.cl;
  Info_._Seconds = _OutRegs.h.dh;
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Set real-time clock BIOS service (INT 1Ah, service 3).
//   Interface to C language calling convention.
//
// PRE:
//   info        : information structure
//		: <1st field> : Hours (BCD)
//		: <2nd field> : Minutes (BCD)
//		: <3rd field> : Seconds (BCD)
//		: <4th field> : Daylight savings time
//
//   registers:
//	AH	: 3
//	CH	: Hours (BCD)
//	CL	: Minutes (BCD)
//	DH	: Seconds (BCD)
//	DL	: Daylight savings time
//**************************************************************************//
void BiosLib_t::SetRealClock(BiosRealClock_t& Info_)
{
  _InRegs.h.ah = 3;

  _InRegs.h.ch = Info_._Hours;
  _InRegs.h.cl = Info_._Minutes;
  _InRegs.h.dh = Info_._Seconds;
  _InRegs.h.dl = Info_._Daylight;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Read date from real-time clock BIOS service (INT 1Ah, service 4).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//	AH	: 4
//
// POST:
//   info        : information structure
//		: <1st field> : Century (BCD)
//		: <2nd field> : Year (BCD)
//		: <3rd field> : Month (BCD)
//		: <4th field> : Day (BCD)
//
//   registers:
//	AH, CH	: Century (BCD)
//	AL, CL	: Year (BCD)
//	DH	: Month (BCD)
//	DL	: Day (BCD)
//**************************************************************************//
void BiosLib_t::ReadRealDate(BiosRealDate_t& Info_)
{
  _InRegs.h.ah = 4;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._Century = _OutRegs.h.ch;
  Info_._Year = _OutRegs.h.cl;
  Info_._Month = _OutRegs.h.dh;
  Info_._Day = _OutRegs.h.dl;
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Read date from real-time clock BIOS service (INT 1Ah, service 5).
//   Interface to C language calling convention.
//
// PRE:
//   info        : information structure
//		: <1st field> : Century (BCD)
//		: <2nd field> : Year (BCD)
//		: <3rd field> : Month (BCD)
//		: <4th field> : Day (BCD)
//
//   registers:
//	AH	: 5
//	CH	: Century (BCD)
//	CL	: Year (BCD)
//	DH	: Month (BCD)
//	DL	: Day (BCD)
//**************************************************************************//
void BiosLib_t::SetRealDate(BiosRealDate_t& Info_)
{
  _InRegs.h.ah = 5;

  _InRegs.h.ch = Info_._Century;
  _InRegs.h.cl = Info_._Year;
  _InRegs.h.dh = Info_._Month;
  _InRegs.h.dl = Info_._Day;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Set alarm BIOS service (INT 1Ah, service 6).
//   Interface to C language calling convention.
//
//   The BIOS alarm function simply generates an interrupt signal after the
//   appropriate period of time has elapsed. The address of the routine you
//   want to perform should be vectored to interrupt 4Ah
//
// PRE:
//   info        : information structure
//		: <1st field> : hours (BCD)
//		: <2nd field> : minutes (BCD)
//		: <3rd field> : seconds (BCD)
//
//   registers:
//	AH	: 6
//	CH	: Hours (BCD)
//	CL	: Minutes (BCD)
//	DH	: Seconds (BCD)
//**************************************************************************//
void BiosLib_t::SetAlarm(BiosRealClock_t& Info_)
{
  _InRegs.h.ah = 6;

  _InRegs.h.ch = Info_._Hours;
  _InRegs.h.cl = Info_._Minutes;
  _InRegs.h.dh = Info_._Seconds;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Disable alarm BIOS service (INT 1Ah, service 7).
//   Interface to C language calling convention.
//
//   The BIOS alarm function simply generates an interrupt signal after the
//   appropriate period of time has elapsed. The address of the routine you
//   want to perform should be vectored to interrupt 4Ah
//
// PRE:
//   registers:
//	AH	: 7
//**************************************************************************//
void BiosLib_t::DisableAlarm()
{
  _InRegs.h.ah = 7;
  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Read alarm BIOS service (INT 1Ah, service 9).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//	AH	: 9
//
// POST:
//   info        : information structure
//		: <1st field> : hours (BCD)
//		: <2nd field> : minutes (BCD)
//		: <3rd field> : seconds (BCD)
//		: <4th field> : alarm status
//
//   registers:
//	AH, CH	: Hours (BCD)
//	AL, CL	: Minutes (BCD)
//	DH	: Seconds (BCD)
//	DL	: Alarm status:
//		     00 = Not enabled
//		     01 = Enabled, no power on
//		     02 = Enabled, will power system on when alarm triggers
//**************************************************************************//
void BiosLib_t::ReadAlarm(BiosRealClock_t& Info_)
{
  _InRegs.h.ah = 9;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);

  Info_._Hours = _OutRegs.h.ch;
  Info_._Minutes = _OutRegs.h.cl;
  Info_._Seconds = _OutRegs.h.dh;
  Info_._Alarm = _OutRegs.h.dl;
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Get day count BIOS service (INT 1Ah, service 0Ah).
//   Interface to C language calling convention.
//
// PRE:
//   registers:
//	AH	: 0Ah
//
// POST:
//   registers:
//	AX, CX	: Total count of days since 01/01/80
//**************************************************************************//
Word_t BiosLib_t::GetDayCount()
{
  _InRegs.h.ah = 0x0A;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);

  return _OutRegs.x.cx;
}

//**************************************************************************//
// CATEGORY:     Date/time Services
//
// PURPOSE:
//   Set day count BIOS service (INT 1Ah, service 0Bh).
//   Interface to C language calling convention.
//
// PRE:
//   dcnt	: Total count of days since 01/01/80
//
//   registers:
//	AH	: 0Bh
//	CX	: Total count of days since 01/01/80
//**************************************************************************//
void BiosLib_t::SetDayCount(Word_t DayCount_)
{
  _InRegs.h.ah = 0x0B;
  _InRegs.x.cx = DayCount_;

  int86x(0x1A, &_InRegs, &_OutRegs, &_SegRegs);
}

//**************************************************************************//
#endif