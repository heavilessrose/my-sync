;----------------------------------------------------------------------------;
;                                BIOSLIB.ASM                                 ;
; BIOS services library                                                      ;
;                                                                            ;
;   by Joseph Wong                                                           ;
;   TASM                                                                     ;
;   Last update 10/01/96                                                     ;
;----------------------------------------------------------------------------;
.MODEL SMALL,C

LOCALS
;----------------------------------------------------------------------------;
;                              DATA SEGMENT                                  ;
;----------------------------------------------------------------------------;
.DATA
	PUBLIC C BEG_SCAN, C END_SCAN, C CUR_ROW, C CUR_COL, C TRIGGER
	PUBLIC C PIXEL_ROW, C PIXEL_COL, C PEN_ROW, C PEN_COL, C ATTRIBUTE
	PUBLIC C ASCIICODE, C SCREEN_WIDTH, C DISPLAY_MODE, C ACTIVE_PAGE
	PUBLIC C VIDMODE, C EGARAM, C TRACKSPERSIDE, C SECTORSPERTRACK
	PUBLIC C SIDES, C DRIVESATTACHED, C LINESTATUS, C MODEMSTATUS
	PUBLIC C MODEMCONTROL, C JSTKSWITCH, C JSTKPOSX1, C JSTKPOSY1
	PUBLIC C JSTKPOSX2, C JSTKPOSY2, C SYSCFGOFF, C SYSCFGSEG
	PUBLIC C RETURNCODE, C EXTBIOSADDR, C BASEPOS, C SLOTNUMBER
	PUBLIC C MIDNITE, C HICLKCNT, C LOCLKCNT, C HOURS, C MINUTES
	PUBLIC C SECONDS, C CENTURY, C YEAR, C MONTH, C DAY, C ALARMSTATUS
	PUBLIC C SCANCODE

	BEG_SCAN		DB	0
	END_SCAN		DB	0
	CUR_ROW			DB	0
	CUR_COL			DB	0

	TRIGGER			DB	0
	PIXEL_COL		DW	0
	PIXEL_ROW		DW	0
	PEN_ROW			DB	0
	PEN_COL			DB	0

	ATTRIBUTE		DB	0
	ASCIICODE		DB	0
	SCANCODE		DB	0

	SCREEN_WIDTH		DB	0
	DISPLAY_MODE		DB	0
	ACTIVE_PAGE		DB	0

	VIDMODE			DB	0
	EGARAM			DB	0

	TRACKSPERSIDE		DB	0
	SECTORSPERTRACK		DB	0
	SIDES			DB	0
	DRIVESATTACHED		DB	0

	LINESTATUS		DB	0
	MODEMSTATUS		DB	0
	MODEMCONTROL		DB	0

	JSTKPOSX1		DW	0
	JSTKPOSY1		DW	0
	JSTKPOSX2		DW	0
	JSTKPOSY2		DW	0
	JSTKSWITCH		DB	0

	SYSCFGOFF		DW	0
	SYSCFGSEG		DW	0

	RETURNCODE		DB	0
	EXTBIOSADDR		DW	0

	BASEPOS			DB	0
	SLOTNUMBER		DB	0

	MIDNITE			DB	0
	HICLKCNT		DW	0
	LOCLKCNT		DW	0

	HOURS			DB	0
	MINUTES			DB	0
	SECONDS			DB	0
	ALARMSTATUS		DB	0

	CENTURY			DB	0
	YEAR			DB	0
	MONTH			DB	0
	DAY			DB	0

;----------------------------------------------------------------------------;
;                               CODE SEGMENT                                 ;
;----------------------------------------------------------------------------;
.CODE
  PUBLIC C prtscr, C setvidmode, C setcursz, C setcurpos, C rdcur, C rdpen
  PUBLIC C setdisplaypg, C scrwinup, C scrwindn, C rdchattr, C wrchattr
  PUBLIC C wrgrchattr, C setpalette, C wrpixel, C rdpixel, C wrttych
  PUBLIC C getvidstate, C setborder, C altvideo, C getegainfo, C setscanlines
  PUBLIC C selloadpal, C selvideo, C selgrayscale, C wrstr, C getequipstat
  PUBLIC C getmemsz, C getdiskstat, C getdrvparm, C parkhds, C initcom
  PUBLIC C sendch, C recch, C getcomstat, C extinitcom, C extcomctrl
  PUBLIC C getjoystk, C extblkmove, C getextmemsz, C setprotmode, C getsyscfg
  PUBLIC C getextbiosaddr, C programsel, C rdkeybch, C rdkeybstat
  PUBLIC C rdkeybshft, C setreprate, C wrkeybuf, C rdextkeybch
  PUBLIC C rdextkeybstat, C prtch, C initprt, C getprtstat, C warmboot
  PUBLIC C getclkcnt, C setclkcnt, C rdrealclk, C setrealclk, C rdrealdate
  PUBLIC C setrealdate, C setalarm, C disablealarm, C rdalarm, C getdaycnt
  PUBLIC C setdaycnt

;----------------------------------------------------------------------------;
; CATEGORY:     Printer Services
;
; PURPOSE:
;   Print screen BIOS service (INT 5h).
;   Interface to C language calling convention.
;----------------------------------------------------------------------------;
prtscr PROC C
	INT     5h
	RET
prtscr ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Set video mode BIOS service (INT 10h, service 0).
;   Interface to C language calling convention.
;
; PRE:
;   mode : desired video mode
;
;   registers:
;     AH : 0
;     AL : desired video mode
;----------------------------------------------------------------------------;
setvidmode PROC C
ARG @@mode:BYTE
USES AX
	MOV     AH, 0
	MOV     AL,[@@mode]
	INT     10h
	RET
setvidmode ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Set cursor size BIOS service (INT 10h, service 1).
;   Interface to C language calling convention.
;
; PRE:
;   begscan : beginning scan line of cursor
;   endscan : ending scan line of cursor
;
;   registers:
;     AH : 1
;     CH : beginning scan line of cursor
;     CL : ending scan line of cursor
;----------------------------------------------------------------------------;
setcursz PROC C
ARG @@begscan:BYTE,@@endscan:BYTE
USES AX, CX
	MOV 	AH,1
	MOV 	CH,[@@begscan]
	MOV 	CL,[@@endscan]
	INT 	10h
	RET
setcursz ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Set cursor position BIOS service (INT 10h, service 2).
;   Interface to C language calling convention.
;
; PRE:
;   pg : video page number
;   row : cursor row
;   col : cursor column
;
;   registers:
;     AH : 2
;     BH : video page number
;     DH : cursor row
;     DL : cursor column
;----------------------------------------------------------------------------;
setcurpos PROC C
ARG @@pg:BYTE,@@row:BYTE,@@col:BYTE
USES AX,BX,DX
	MOV 	AH,2
	MOV 	BH,[@@pg]
	MOV 	DH,[@@row]
	MOV 	DL,[@@col]
	INT 	10h
	RET
setcurpos ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   read cursor position and size BIOS service (INT 10h, service 3).
;   Interface to C language calling convention.
;
; PRE:
;   pg : video page number
;
;   registers:
;     AH : 3
;     BH : video page number
;
; POST:
;	   BEG_SCAN : beginning scan line of cursor
;	   END_SCAN : ending scan line of cursor
;	   CUR_ROW : cursor row
;	   CUR_COL : cursor column
;
;   registers:
;     CH     : beginning scan line of cursor
;     CL     : ending scan line of cursor
;     DH     : cursor row
;     DL     : cursor column
;----------------------------------------------------------------------------;
rdcur PROC C
ARG @@pg:BYTE
USES AX,BX,CX,DX
	MOV 	AH,3
	MOV 	BH,[@@pg]
	INT 	10h

	MOV	BEG_SCAN,CH
	MOV	END_SCAN,CL
	MOV	CUR_ROW,DH
	MOV	CUR_COL,DL

	RET
rdcur ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   read light pen position BIOS service (INT 10h, service 4).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH : 4
;
; POST:
;	   TRIGGER 	: light pen trigger status
;	   PIXEL_COL 	: pixel column
;	   PIXEL_ROW 	: Raster line (pixel row)
;	   PEN_ROW 	: light pen row
;	   PEN_COL 	: light pen column
;
;   registers:
;       AH : Light pen trigger status
;       BX : Pixel column
;       CX : Raster line (pixel row)
;       DH : Light pen row
;       DL : Light pen column
;----------------------------------------------------------------------------;
rdpen   PROC C
USES	AX,BX,CX,DX
	MOV     AH,4            ;Specify service 4
	INT     10h             ;BIOS video interrupt

	MOV     TRIGGER,AH
	MOV     PIXEL_COL,BX
	MOV     PIXEL_ROW,CX
	MOV     PEN_ROW,DH
	MOV	PEN_COL,DL

	RET
rdpen   ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Set active display page BIOS service (INT 10h, service 5).
;   Interface to C language calling convention.
;
; PRE:
;   page : page number of desired active display page
;
;   registers:
;       AH : 5
;       AL : Desired display page
;----------------------------------------------------------------------------;
setdisplaypg PROC C
ARG @@pg:BYTE
USES AX
	MOV 	AH,5
	MOV 	AL,[@@pg]
	INT 	10h
	RET
setdisplaypg ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Scroll window up BIOS service (INT 10h, service 6).
;   Interface to C language calling convention.
;
; PRE:
;		lines     	: number of lines to scroll
;		attr		: display attributes for blank lines
;		top 		: row for upper left corner of window
;		left		: column for upper left cornder of window
;		bottom		: row for lower right corner of window
;		right		: column for lower right corner of window
;
;   registers:
;       AH      : 6
;       AL      : number of lines to scroll
;       BH      : display attribute for blank lines
;       CH      : row for upper left corner of window
;       CL      : column for upper left corner of window
;       DH      : row for lower right corner of window
;       DL      : column for lower right corner of window
;
;----------------------------------------------------------------------------;
scrwinup        PROC C
ARG	@@lines:BYTE,@@attr:BYTE,@@top:BYTE,@@left:BYTE,@@bottom:BYTE,@@right:BYTE
USES	AX,BX,CX,DX
	MOV     AH,6
	MOV     AL,[@@lines]
	MOV     BH,[@@attr]
	MOV     CH,[@@top]
	MOV     CL,[@@left]
	MOV     DH,[@@bottom]
	MOV     DL,[@@right]
	INT     10h
	RET
scrwinup        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Scroll window down BIOS service (INT 10h, service 7).
;   Interface to C language calling convention.
;
; PRE:
;		lines     	: number of lines to scroll
;		attr		: display attributes for blank lines
;		top 		: row for upper left corner of window
;		left		: column for upper left cornder of window
;		bottom		: row for lower right corner of window
;		right		: column for lower right corner of window
;
;   registers:
;       AH      : 7
;       AL      : number of lines to scroll
;       BH      : display attribute for blank lines
;       CH      : row for upper left corner of window
;       CL      : column for upper left corner of window
;       DH      : row for lower right corner of window
;       DL      : column for lower right corner of window
;
;----------------------------------------------------------------------------;
scrwindn        PROC    C
ARG	@@lines:BYTE,@@attr:BYTE,@@top:BYTE,@@left:BYTE,@@bottom:BYTE,@@right:BYTE
USES	AX,BX,CX,DX
	MOV     AH,7
	MOV     AL,[@@lines]
	MOV     BH,[@@attr]
	MOV     CH,[@@top]
	MOV     CL,[@@left]
	MOV     DH,[@@bottom]
	MOV     DL,[@@right]
	INT     10h
	RET
scrwindn        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Read character and attribute BIOS service (INT 10h, service 8).
;   Interface to C language calling convention.
;
; PRE:
;   pg          : video page number
;
;   registers:
;       AH      : 8
;       BH      : video page number
;
; POST:
;	ATTRIBUTE	: character attribute
;	ASCIICODE	: character ascii code
;
;   registers:
;       AH      : Attribute byte
;       AL      : ASCII character code
;
;----------------------------------------------------------------------------;
rdchattr        PROC C
ARG     @@pg:BYTE
USES	BX,ES
	MOV     AH,8
	MOV     BH,[@@pg]
	INT     10h

	MOV	ATTRIBUTE,AH
	MOV	ASCIICODE,AL
	RET
rdchattr        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Write character and attribute BIOS service (INT 10h, service 9).
;   Interface to C language calling convention.
;
; PRE:
;   pg          : video page number
;   asch        : ASCII character code
;   attr        : attribute of character
;   count       : number of character/attribute words to display
;
;   registers:
;       AH      : 9
;       AL      : ASCII character code
;       BH      : video page number
;       BL      : video attribute of character in AL
;       CX      : Number of character/attribute words to display
;----------------------------------------------------------------------------;
wrchattr        PROC C
ARG     @@pg:BYTE,@@asch:BYTE,@@attr:BYTE,@@count:WORD
USES	AX,BX,CX
	MOV     AH,9
	MOV     AL,[@@asch]
	MOV     BH,[@@pg]
	MOV     BL,[@@attr]
	MOV     CX,[@@count]
	INT     10h
	RET
wrchattr        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Write character and attribute BIOS service (INT 10h, service 0Ah).
;   Interface to C language calling convention.
;
; PRE:
;   pg          : video page number
;   asch        : ASCII character code
;   attr        : attribute of character (in graphics mode only)
;   count       : number of character/attribute words to display
;
;   registers:
;       AH      : 0Ah
;       AL      : ASCII character code
;       BH      : video page number
;       BL      : video attribute of character in AL (in graphics mode only)
;       CX      : Number of character/attribute words to display
;----------------------------------------------------------------------------;
wrgrchattr      PROC C
ARG     @@pg:BYTE,@@asch:BYTE,@@attr:BYTE,@@count:WORD
USES	AX,BX,CX
	MOV     AH,0Ah
	MOV     AL,[@@asch]
	MOV     BH,[@@pg]
	MOV     BL,[@@attr]
	MOV     CX,[@@count]
	INT     10h
	RET
wrgrchattr       ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Set color palette BIOS service (INT 10h, service 0Bh).
;   Interface to C language calling convention.
;
; PRE:
;   palid       : Palette ID
;   color       : Palette ID color value
;
;   registers:
;       AH      : 0Bh
;       BH      : Palette ID
;       BL      : Palette ID color value
;----------------------------------------------------------------------------;
setpalette      PROC C
ARG     @@palid:BYTE,@@color:BYTE
USES	AX,BX
	MOV     AH,0Bh
	MOV     BH,[@@palid]
	MOV     BL,[@@color]
	INT     10h
	RET
setpalette      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Write pixel dot BIOS service (INT 10h, service 0Ch).
;   Interface to C language calling convention.
;
; PRE:
;   pixval      : Pixel value
;   pixcol      : Pixel column
;   pixrow      : Raster line (pixel row)
;
;   registers:
;       AH      : OCh
;       AL      : Pixel value
;       CX      : Pixel column
;       DX      : Raster line (pixel row)
;----------------------------------------------------------------------------;
wrpixel         PROC C
ARG     @@pixval:BYTE,@@pixcol:WORD,@@pixrow:WORD
USES	AX,CX,DX
	MOV     AH,0Ch
	MOV     AL,[@@pixval]
	MOV     CX,[@@pixcol]
	MOV     DX,[@@pixrow]
	INT     10h
	RET
wrpixel         ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Read pixel dot BIOS service (INT 10h, service 0Dh).
;   Interface to C language calling convention.
;
; PRE:
;   pixcol      : Pixel column
;   pixrow      : Raster line (pixel row)
;
;   registers:
;       AH      : ODh
;       CX      : Pixel column
;       DX      : Raster line (pixel row)
;
; POST:
;   registers:
;       AL      : Pixel value
;----------------------------------------------------------------------------;
rdpixel         PROC C
ARG     @@pixcol:WORD,@@pixrow:WORD
USES	CX,DX
	MOV     AH,0Dh
	MOV     CX,[@@pixcol]
	MOV     DX,[@@pixrow]
	INT     10h
	XOR     AH,AH
	RET
rdpixel         ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   TTY character output BIOS service (INT 10h, service 0Eh).
;   Interface to C language calling convention.
;
; PRE:
;   asch        : ASCII character code
;   pg          : video page number
;   color       : Character color (graphics foreground)
;
;   registers:
;       AH      : OEh
;       AL      : ASCII character code
;       BH      : Video page number
;       BL      : Character color (graphics foreground)
;----------------------------------------------------------------------------;
wrttych         PROC C
ARG     @@asch:BYTE,@@pg:BYTE,@@color:BYTE
USES	AX,BX
	MOV     AH,0Eh
	MOV     AL,[@@asch]
	MOV     BH,[@@pg]
	MOV     BL,[@@color]
	INT     10h
	RET
wrttych         ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Get current video state BIOS service (INT 10h, service 0Fh).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : OEh
;
; POST:
;   		  SCREEN_WIDTH 	: Screen width
;   		  DISPLAY_MODE	: Display mode
;   		  ACTIVE_PAGE 	: Active display page
;
;   registers:
;       AH      : Screen width
;       AL      : Display mode
;       BH      : Active display page
;----------------------------------------------------------------------------;
getvidstate     PROC C
ARG	@@info:WORD
USES	AX,BX
	MOV     AH,0Fh                  ;Specify service 0Fh
	INT     10h                     ;BIOS video interrupt

	MOV	SCREEN_WIDTH, AH       ;Save number of columns
	MOV	DISPLAY_MODE, AL	;Save display mode
	MOV	ACTIVE_PAGE, BH       ;Save display page
	RET
getvidstate     ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA palette registers BIOS service (INT 10h, service 10h).
;   Set border color register (subfunction 01h)
;   Interface to C language calling convention.
;
; PRE:
;   color       : Color value
;
;   registers:
;	AH	: 10h
;       AL      : 01h
;       BH      : Color value
;----------------------------------------------------------------------------;
setborder       PROC C
ARG     @@color:BYTE
USES    AX,BX
	MOV     AH,10h
	MOV     AL,01h
	MOV     BH,[@@color]
	INT     10h
	RET
setborder       ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 12h
;       BL      : (subfunction #)
;       AL      : (subfunction options)
;
; POST:
;   registers:
;       subfunction 10h (setting in BL)
;       BH      : BIOS video mode (0 color / 1 mono)
;       BL      : Size of EGA RAM (0=64K to 3=256K)
;       AL      : 12h if service is valid in all cases
;----------------------------------------------------------------------------;
altvideo        PROC C
	MOV     AH,12h
	INT     10h
	RET
altvideo        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
;   Return EGA information (subfunction 10h)
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 12h
;       BL      : 10h
;
; POST:
;   VIDMODE     : BIOS video mode (0 color / 1 mono)
;   EGARAM      : Size of EGA RAM (0=64K to 3=256K)
;
;   registers:
;       AH, BH  : BIOS video mode (0 color / 1 mono)
;       AL, BL  : Size of EGA RAM (0=64K to 3=256K)
;----------------------------------------------------------------------------;
getegainfo      PROC C
USES	BX
	MOV     BL,10h
	CALL    altvideo
	MOV     AX,BX

	MOV	VIDMODE,AH
	MOV	EGARAM,AL
	RET
getegainfo      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
;   select scan lines (subfunction 30h)
;   Interface to C language calling convention.
;
; PRE:
;   lines       : number of scan lines
;
;   registers:
;       AH      : 12h
;       BL      : 30h
;       AL      : 00h 200 lines
;               : 01h 350 lines
;               : 02h 400 lines
;----------------------------------------------------------------------------;
setscanlines    PROC C
ARG     @@lines:BYTE
USES	AX,BX
	MOV     BL,30h
	MOV     AL,[@@lines]
	CALL    altvideo
	RET
setscanlines    ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
;   load palette when setting mode (subfunction 31h)
;   Interface to C language calling convention.
;
; PRE:
;   flg         : 00h = enable, 01h = disable
;
;   registers:
;       AH      : 12h
;       BL      : 31h
;       AL      : enable=00h / disable=01h
;----------------------------------------------------------------------------;
selloadpal      PROC C
ARG     @@flg:BYTE
USES	AX,BX
	MOV     BL,31h
	MOV     AL,[@@flg]
	CALL    altvideo
	RET
selloadpal      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
;   Video on/off (subfunction 32h)
;   Interface to C language calling convention.
;
; PRE:
;   flg         : 00h = enable, 01h = disable
;
;   registers:
;       AH      : 12h
;       BL      : 32h
;       AL      : enable=00h / disable=01h
;----------------------------------------------------------------------------;
selvideo        PROC C
ARG     @@flg:BYTE
USES	AX,BX
	MOV     BL,32h
	MOV     AL,[@@flg]
	CALL    altvideo
	RET
selvideo        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
;   Sum color to gray scale (subfunction 33h)
;   Interface to C language calling convention.
;
; PRE:
;   flg         : 00h = enable, 01h = disable
;
;   registers:
;       AH      : 12h
;       BL      : 33h
;       AL      : enable=00h / disable=01h
;----------------------------------------------------------------------------;
selgrayscale    PROC C
ARG     @@flg:BYTE
USES	AX,BX
	MOV     BL,33h
	MOV     AL,[@@flg]
	CALL    altvideo
	RET
selgrayscale    ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   EGA/VGA alternate video select BIOS service (INT 10h, service 12h).
;   Cursor emulation on/off (subfunction 34h)
;   Interface to C language calling convention.
;
; PRE:
;   flg         : 00h = enable, 01h = disable
;
;   registers:
;       AH      : 12h
;       BL      : 34h
;       AL      : enable=00h / disable=01h
;----------------------------------------------------------------------------;
selcuremul      PROC
ARG     @@flg:BYTE
USES	AX,BX
	MOV     BL,34h
	MOV     AL,[@@flg]
	CALL    altvideo
	RET
selcuremul      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Video Services
;
; PURPOSE:
;   Write string BIOS service (INT 10h, service 13h).
;   Interface to C language calling convention.
;
; PRE:
;   info	: information structure
;   		  msg 	      : message
;   		  mode	      : video mode
;		  pg	      : video page number
;		  attr	      : character attribute
;		  length      : length of string
;		  row	      : cursor row where string is to be displayed
;		  col	      : cursor column where string is to be displayed
;
;   registers:
;       AH      : 13h
;       AL      : video mode
;       BH      : video page number
;       BL      : character attribute (depending on AL)
;       CX      : length of string
;       DH      : cursor row where string is to be displayed
;       DL      : cursor column where string is to be displayed
;       BP      : offset address of string
;       ES      : segment address of string
;----------------------------------------------------------------------------;
wrstr   PROC C
ARG     @@msg:WORD,@@mode:BYTE,@@pg:BYTE,@@attr:BYTE,@@length:WORD,@@row:BYTE,@@col:BYTE
USES	AX,BX,CX,DX,SI,BP,ES

	MOV	SI,[@@msg]
	MOV	AL,[@@mode]
	MOV	BH,[@@pg]
	MOV	BL,[@@attr]
	MOV	CX,[@@length]
	MOV	DH,[@@row]
	MOV	DL,[@@col]

	PUSH    DS                      ;Make ES same as DS
	POP     ES

	MOV     BP,OFFSET ES:SI         ;Point to message offset
	MOV     AH,13h
	INT     10h

	RET
wrstr   ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     System Services
;
; PURPOSE:
;   Get equipment status BIOS service (INT 11h).
;   Interface to C language calling convention.
;
; POST:
;   registers:
;       AX      : Equipment status
;
; Bit meanings for equipment status word returned by interrupt 11h.
;
; Bits
; FEDCBA98      76543210        Meaning of bits
; ---------------------------------------------
; xx                            Number of printers attached
;   x                           Not used
;    0                          Game adapter not installed
;    1                          Game adapted installed
;     xxx                       Number of serial cards attached
;        x                      Not used
;               00              1 disk drive attached (if bit 0=1)
;               01              2 disk drives attached (if bit 0=1)
;               10              3 disk drives attached (if bit 0=1)
;               11              4 disk drives attached (if bit 0=1)
;                 01            Initial video mode -- 40 x 25 BW/color card
;                 10            Initial video mode -- 80 x 25 BW/color card
;                 11            Initial video mode -- 80 x 25 BW/mono card
;                   00          16K system board RAM
;                   01          32K system board RAM
;                   10          48K system board RAM
;                   11          64K system board RAM
;                     0         Math coprocessor not installed
;                     1         Math coprocessor installed
;                      0        No disk drives installed (bits 6-7 insignificant)
;                      0        Disk drives installed (bits 6-7 significant)
;----------------------------------------------------------------------------;
getequipstat    PROC C
	INT     11h
	RET
getequipstat    ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     System Services
;
; PURPOSE:
;   Get memory size BIOS service (INT 12h).
;   Interface to C language calling convention.
;
; POST:
;   registers:
;       AX      : Memory blocks
;
;----------------------------------------------------------------------------;
getmemsz        PROC C
	INT     12h
	RET
getmemsz        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Disk Services
;
; PURPOSE:
;   Get floppy disk status BIOS service (INT 13h, service 01h).
;   Interface to C language calling convention.
;
; PRE:
;   drvnum      : Drive number
;
;   registers:
;       AH      : 1
;       DL      : Drive number (0 bit based),
;                 bit 7 = 0 for a diskette
;
; POST:
;   registers:
;       AH      : Status byte
;
; Meaning of the status byte returned by service 13/1
;
; Bits
; 76543210      Hex     Decimal         Meaning of bits
; -----------------------------------------------------
; 1             80      128             Time out - drive did not respond
;  1            40      64              Seek failure - couldn't move to
;                                       requested track
;   1           20      32              Controller malfunction
;    1          10      16              Bad CRC detected on disk read
;     1  1      9       9               DMA error - 64L boundary crossed
;     1         8       8               DMA overrun
;      1        4       4               Bad sector / sector not found
;       11      3       3               Write protect error
;       1       2       2               Bad sector ID (address) mark
;        1      1       1               Bad command
;----------------------------------------------------------------------------;
getdiskstat     PROC C
ARG     @@drvnum:BYTE
USES	DX
	MOV     AH,1
	MOV     DL,[@@drvnum]
	INT     13h
	XOR     AL,AL
	RET
getdiskstat     ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Disk Services
;
; PURPOSE:
;   Get floppy disk status BIOS service (INT 13h, service 08h).
;   Interface to C language calling convention.
;
; PRE:
;   drvnum      : Drive number
;
;   registers:
;       AH      : 8
;       DL      : Drive number (0 bit based),
;                 bit 7 = 1 for a fixed disk
;
; POST:
;		TRACKSPERSIDE 	: number of tracks/side
;		SECTORSPERTRACK : number of sectors/track
;		SIDES 		: number of sides
;		DRIVESATTACHED 	: number of consecutive drives attached
;
;   registers:
;       CH  	: Number of tracks/side
;       CL  	: Number sector/track
;       DH      : Number of sides
;       DL      : Number of consecutive drives attached
;----------------------------------------------------------------------------;
getdrvparm      PROC C
ARG     @@drvnum:BYTE,@@info:WORD
USES	AX,CX,DX
	MOV     AH,8
	MOV     DL,[@@drvnum]
	INT     13h

	MOV	TRACKSPERSIDE,CH
	MOV	SECTORSPERTRACK,CL
	MOV	SIDES,DH
	MOV	DRIVESATTACHED,DL

	RET
getdrvparm      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Disk Services
;
; PURPOSE:
;   Park heads BIOS service (INT 13h, service 19h).
;   Interface to C language calling convention.
;
; PRE:
;   drvnum      : Drive number
;
;   registers:
;       AH      : 19h
;       DL      : Drive number (0 bit based),
;                 bit 7 = 1 for a fixed disk
;----------------------------------------------------------------------------;
parkhds         PROC C
ARG     @@drvnum:BYTE
USES	AX,DX
	MOV     AH,19h
	MOV     DL,[@@drvnum]
	INT     13h
	RET
parkhds         ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Communications Services
;
; PURPOSE:
;   Initialize communications port BIOS service (INT 14h, service 0).
;   Interface to C language calling convention.
;
; PRE:
;   iparm       : initialization parameter
;   port        : com port
;
;   registers:
;       AH      : 0
;       AL      : Initialization parameter
;       DX      : com port
;
; Meaning of AL bits for initialization parameter
;
; Bits
; 76543210      Meaning
; ---------------------
; 000           110 baud
; 001           150 baud
; 010           300 baud
; 011           600 baud
; 100           1200 baud
; 101           2400 baud
; 110           4800 baud
; 111           9600 baud
;    00         No parity
;    01         Odd parity
;    10         No parity
;    11         Even parity
;      0        1 stopbit
;      1        2 stopbit
;       10      7-bit data length
;       11      8-bit data length
;
; POST:
;   LINESTATUS	: line status
;   MODEMSTATUS	: modem status
;
;   registers:
;       AH      : Line status
;       AL      : Modem status
;
; Meaning of AH bits for line status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Time-out error
;  1            Transfer shift register (TSR) empty
;   1           Transfer holding register (THR) empty
;    1          Break interrupt detected
;     1         Framing error
;      1        Parity error
;       1       Overrun error
;        1      Data ready
;
; Meaning of AL bits for modem status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Receive line-signal detect
;  1            Ring indicator
;   1           Data set ready (DSR)
;    1          Clear to send (CTS)
;     1         Delta receive line-signal detect
;      1        Trailing-edge ring detected
;       1       Delta data-set ready (DDSR)
;        1      Delta clear to send (DCTS)
;----------------------------------------------------------------------------;
initcom         PROC C
ARG     @@iparm:BYTE,@@port:WORD
USES	DX
	MOV     AL,[@@iparm]
	MOV     DX,[@@port]
	MOV     AH,0
	INT     14h

	MOV	LINESTATUS,AH
	MOV	MODEMSTATUS,AL
	RET
initcom         ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Communications Services
;
; PURPOSE:
;   Transmit character BIOS service (INT 14h, service 01h).
;   Interface to C language calling convention.
;
; PRE:
;   asch        : ASCII character
;   port        : com port
;
;   registers:
;       AH      : 1
;       AL      : ASCII character
;       DX      : com port
;
; POST:
;   registers:
;       AH      : Return code
;                 If successful then (AH < 80h)
;                 If unsuccessful then (AH > 7Fh)
;
; Meaning of AH bits for line status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Time-out error
;  1            Transfer shift register (TSR) empty
;   1           Transfer holding register (THR) empty
;    1          Break interrupt detected
;     1         Framing error
;      1        Parity error
;       1       Overrun error
;        1      Data ready
;----------------------------------------------------------------------------;
sendch  PROC C
ARG     @@asch:BYTE,@@port:WORD
USES	DX
	MOV     AL,[@@asch]
	MOV     DX,[@@port]
	MOV     AH,1
	INT     14h
	XOR     AL,AL
	RET
sendch  ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Communications Services
;
; PURPOSE:
;   Receive character BIOS service (INT 14h, service 02h).
;   Interface to C language calling convention.
;
; PRE:
;   port        : com port
;
;   registers:
;       AH      : 2
;       DX      : com port
;
; POST:
;   registers:
;       AH      : Return code
;                 If successful then (AH < 80h)
;                 If unsuccessful then (AH > 7Fh)
;
; Meaning of AH bits for line status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Time-out error
;  1            Transfer shift register (TSR) empty
;   1           Transfer holding register (THR) empty
;    1          Break interrupt detected
;     1         Framing error
;      1        Parity error
;       1       Overrun error
;        1      Data ready
;----------------------------------------------------------------------------;
recch   PROC C
ARG     @@port:WORD
USES	DX
	MOV     AH,2
	MOV     DX,[@@port]
	INT     14h
	XOR     AL,AL
	RET
recch   ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Communications Services
;
; PURPOSE:
;   Get communications port status BIOS service (INT 14h, service 03h).
;   Interface to C language calling convention.
;
; PRE:
;   port        : com port
;
;   registers:
;       AH      : 3
;       DX      : com port
;
; POST:
;   LINESTATUS	: line status
;   MODEMSTATUS	: modem status
;
;   registers:
;       AH      : Line status
;       AL      : Modem status
;
; Meaning of AH bits for line status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Time-out error
;  1            Transfer shift register (TSR) empty
;   1           Transfer holding register (THR) empty
;    1          Break interrupt detected
;     1         Framing error
;      1        Parity error
;       1       Overrun error
;        1      Data ready
;
; Meaning of AL bits for modem status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Receive line-signal detect
;  1            Ring indicator
;   1           Data set ready (DSR)
;    1          Clear to send (CTS)
;     1         Delta receive line-signal detect
;      1        Trailing-edge ring detected
;       1       Delta data-set ready (DDSR)
;        1      Delta clear to send (DCTS)
;----------------------------------------------------------------------------;
getcomstat      PROC C
ARG     @@port:WORD
USES	DX
	MOV     DX,[@@port]
	MOV     AH,3
	INT     14h

	MOV	LINESTATUS,AH
	MOV	MODEMSTATUS,AL
	RET
getcomstat      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Communications Services
;
; PURPOSE:
;   Extended communications port initialization BIOS service (INT 14h, service 04h).
;   Interface to C language calling convention.
;
; PRE:
;		comport 	: com port
;		breakflag 	: break flag
;		parity 		: parity flag
;		stopbits 	: stop bits
;		databits 	: data bits
;		bps 		: bits per second
;
;   registers:
;       AH      : 4
;       AL      : 00h   = no break
;               : 01h   = break
;       BH      : 00h   = no parity
;               : 01h   = odd parity
;               : 02h   = even parity
;               : 03h   = mark parity
;               : 04h   = space parity
;       BL      : 00h   = one stop bit
;               : 01h   = two stop bits
;       CH      : 00h   = 5 data bits
;               : 01h   = 6 data bits
;               : 02h   = 7 data bits
;               : 03h   = 8 data bits
;       CL      : 00h   = 110 BPS
;               : 01h   = 150 BPS
;               : 02h   = 300 BPS
;               : 03h   = 600 BPS
;               : 04h   = 1200 BPS
;               : 05h   = 2400 BPS
;               : 06h   = 4800 BPS
;               : 07h   = 9600 BPS
;               : 08h   = 19200 BPS
;       DX      : com port
;
; POST:
;   linestat	: line status
;   modstat	: modem status
;
;   registers:
;       AH      : Line status
;       AL      : Modem status
;
; Meaning of AH bits for line status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Time-out error
;  1            Transfer shift register (TSR) empty
;   1           Transfer holding register (THR) empty
;    1          Break interrupt detected
;     1         Framing error
;      1        Parity error
;       1       Overrun error
;        1      Data ready
;
; Meaning of AL bits for modem status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Receive line-signal detect
;  1            Ring indicator
;   1           Data set ready (DSR)
;    1          Clear to send (CTS)
;     1         Delta receive line-signal detect
;      1        Trailing-edge ring detected
;       1       Delta data-set ready (DDSR)
;        1      Delta clear to send (DCTS)
;----------------------------------------------------------------------------;
extinitcom      PROC C
ARG	@@comport:BYTE,@@breakflag:BYTE,@@parity:BYTE,@@stopbits:BYTE,@@databits:BYTE,@@bps:WORD
USES	BX,CX,DX
	MOV     AH,04h
	MOV     AL,[@@comport]
	MOV     BH,[@@breakflag]
	MOV     BL,[@@parity]
	MOV     CH,[@@stopbits]
	MOV     CL,[@@databits]
	MOV     DX,[@@bps]
	INT     14h

	MOV     LINESTATUS,AH
	MOV	MODEMSTATUS,AL
	RET
extinitcom      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Communications Services
;
; PURPOSE:
;   Extended communications port control BIOS service (INT 14h, service 05h).
;   Interface to C language calling convention.
;
; PRE:
;   rdwr        : Read/Write status
;   mcr         : New mcr content if write enabled
;   port        : Serial port
;
;   registers:
;       AH      : 05h
;       AL      : 00h, Read modem control register
;               : 01h, Write modem control register
;       BL      : New MCR content if AL = 01h
;       DX      : Serial port (0 = COM1, etc.)
;
; POST:
;   LINESTATUS		: line status
;   MODEMSTATUS		: modem status
;   MODEMCONTROL	: modem control
;
;   registers:
;       AH      : Line status
;       AL      : Modem status
;       DL, BL  : Modem control register
;
; Meaning of BL bits for modem control
;
; Bits
; 76543210      Meaning
; ---------------------
;        1      Data terminal ready (DTR)
;       1       Request to send (RTS)
;      1        User 1
;     1         User 2
;    1          Loopback test
; xxx           Reserved
;
; Meaning of AH bits for line status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Time-out error
;  1            Transfer shift register (TSR) empty
;   1           Transfer holding register (THR) empty
;    1          Break interrupt detected
;     1         Framing error
;      1        Parity error
;       1       Overrun error
;        1      Data ready
;
; Meaning of AL bits for modem status
;
; Bits
; 76543210      Meaning
; ---------------------
; 1             Receive line-signal detect
;  1            Ring indicator
;   1           Data set ready (DSR)
;    1          Clear to send (CTS)
;     1         Delta receive line-signal detect
;      1        Trailing-edge ring detected
;       1       Delta data-set ready (DDSR)
;        1      Delta clear to send (DCTS)
;----------------------------------------------------------------------------;
extcomctrl      PROC C
ARG     @@rdwr:BYTE,@@mcr:BYTE,@@port:WORD,@@linestat:WORD,@@modstat:WORD,@@modctrl:WORD
USES	BX
	MOV     AL,[@@rdwr]
	MOV     BL,[@@mcr]
	MOV     DX,[@@port]
	MOV	AH,05h
	INT	14h
	XOR     BH,BH
	MOV     DX,BX

	MOV	LINESTATUS,AH
	MOV	MODEMSTATUS,AL
	MOV	MODEMCONTROL,BL
	RET
extcomctrl      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Miscellaneous Services
;
; PURPOSE:
;   Joystick support BIOS service (INT 15h, service 84h).
;   Interface to C language calling convention.
;
; PRE:
;   rdflg       : 00h = Read switches
;               : 01h = Read joystick position
;
;   registers:
;       AH      : 84h
;       DX      : 00h = Read switches
;               : 01h = Read joystick position
;
; POST:
;		: JSTKSWITCH : switch settings
;		: JSTKPOSX1 : A(X) value
;		: JSTKPOSY1 : A(Y) value
;		: JSTKPOSX2 : B(X) value
;		: JSTKPOSY2 : B(Y) value
;
;   registers:
;
;   The joystick data is saved in information structure -- info
;
;   if reading switches (DX=0):
;       AL = Switch settings (bits 4-7)
;   if reading position (DX=1):
;       AX = A(X) value
;       BX = A(Y) value
;       CX = B(X) value
;       DX = B(Y) value
;----------------------------------------------------------------------------;
getjoystk       PROC C
ARG     @@rdflg:WORD,@@info:WORD
USES	AX,BX,CX,DX
	MOV     AH,84h                  ;Joystick services
	MOV     DX,[@@rdflg]            ;read position
	INT     15h                     ;BIOS extension interrupt

	CMP     DX,0
	JE      @@READSWITCHES

	MOV     JSTKPOSX1,AX
	MOV     JSTKPOSY1,BX
	MOV     JSTKPOSX2,CX
	MOV     JSTKPOSY2,DX
	JMP     @@READPOSITION

@@READSWITCHES:
	XOR     AH,AH
	MOV     JSTKSWITCH,AL

@@READPOSITION:
	RET
getjoystk       ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Miscellaneous Services
;
; PURPOSE:
;   Move block to/from extended memory BIOS service (INT 15h, service 87h).
;   Interface to C language calling convention.
;
; PRE:
;   numwords    : number of words to move
;   segaddr     : segment address of GDT
;   offaddr     : Offset address of GDT
;
;   registers:
;       AH      : 87h
;       CX      : Number of words to move
;       ES      : Segment address of GDT
;       SI      : Offset address of GDT
;
; POST:
;   registers:
;       AH      : Return code
;
; Format of global descriptor table
;
; Offset        Description
; -------------------------
; 00h           Dummy, set to zero
; 08h           GDT data-segment location, set to zero
; 10h           Source GDT, points to 8-byte GDT for source memory block
; 18h           Target GDT, points to 8-byte GDT for target memory block
; 20h           Pointer to BIOS code segment, initially zero
; 28h           Pointer to BIOS stack segment, initially zero
;
;                       source/target GDT layouts
;
; 00h           Segment limit
; 02h           24-bit segment physical address
; 05h           Data access rights
; 06h           Reserved word, must be zero
;----------------------------------------------------------------------------;
extblkmove      PROC C
ARG     @@numwords:WORD,@@segaddr:WORD,@@offaddr:WORD
USES	CX,SI,ES
	MOV     AH,87h
	MOV     CX,[@@numwords]
	MOV     ES,[@@segaddr]
	MOV     SI,[@@offaddr]
	INT     15h
	XOR     AL,AL
	RET
extblkmove      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Miscellaneous Services
;
; PURPOSE:
;   Size extended memory BIOS service (INT 15h, service 88h).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 88h
;
; POST:
;   registers:
;       AX      : Number of contiguous 1K blocks of RAM above 100000h
;----------------------------------------------------------------------------;
getextmemsz     PROC C
	MOV     AH,88h
	INT     15h
	RET
getextmemsz     ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Miscellaneous Services
;
; PURPOSE:
;   Protected mode switch BIOS service (INT 15h, service 88h).
;   Interface to C language calling convention.
;
; PRE:
;   info	: information structure
;		: irq0off	: IRQ0 vector offset
;		: irq8off	: IRQ8 vector offset
;		: pmodeoff	: Offset into protected-mode CS to jump to
;		: gdtseg	: Segment address of GDT
;		: gdtoff	: Offset address of GDT
;
;   registers:
;       AH      : 89h
;       BL      : IRQ0 vector offset
;       BH      : IRQ8 vector offset
;       CX      : Offset into protected-mode CS to jump to
;       ES      : Segment address of GDT
;       SI      : Offset address of GDT
;
; POST:
;   registers:
;       AH      : Return code
;
; Format of global descriptor table
;
; Offset        Description
; -------------------------
; 00h           Dummy, set to zero
; 08h           GDT data-segment location, set to zero
; 10h           Source GDT, points to 8-byte GDT for source memory block
; 18h           Target GDT, points to 8-byte GDT for target memory block
; 20h           Pointer to BIOS code segment, initially zero
; 28h           Pointer to BIOS stack segment, initially zero
;
;                       source/target GDT layouts
;
; 00h           Segment limit
; 02h           24-bit segment physical address
; 05h           Data access rights
; 06h           Reserved word, must be zero
;----------------------------------------------------------------------------;
setprotmode     PROC C
ARG     @@irq0off:BYTE,@@irq8off:BYTE,@@pmodeoff:WORD,@@gdtseg:WORD,@@gdtoff:WORD
USES	BX,CX,SI,ES
	MOV     AH,89h
	MOV     BL,[@@irq0off]
	MOV	BH,[@@irq8off]
	MOV     CX,[@@pmodeoff]
	MOV     ES,[@@gdtseg]
	MOV     SI,[@@gdtoff]
	INT     15h
	XOR     AL,AL
	RET
setprotmode     ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     PS/2 Services
;
; PURPOSE:
;   Get system configuration BIOS service (INT 15h, service C0h).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : C0h
;
; POST:
;   SYSCFGOFF   : Offset address of system-configuration table
;   SYSCFGSEG   : Segment address of system-configuration table
;
;   registers:
;       AX, BX  : Offset address of system-configuration table
;       DX, ES  : Segment address of system-configuration table
;
; System descriptor table
;
; Offset        Meaning
; ---------------------
; 00h           Byte count of subsequent data (minimum 8)
; 02h           Model byte (See system model table)
; 03h           Submodel byte (See system model table)
; 04h           BIOS revision level (00 = first release)
; 05h           Feature information (See feature info table)
; 06-09h        Reserved
;
; Feature information table
;
; Bits
; 76543210      Hex     Decimal         Meaning of bits
; -----------------------------------------------------
; 1             80      128             DMA channel 3 used by hard disk BIOS
;  1            40      64              Second interrupt chip present
;   1           20      32              Real-time clock present
;    1          10      16              Keyboard intercept called by INT 09h
;     1         8       8               Wait for external event is supported
;      1        4       4               Extended BIOS data area allocated
;       1       2       2               Micro channel architecture
;       0                               PC bus I/O channel
;        x                              Reserved
;
; System model identification
;
;                 Model                         BIOS
;                 Byte          Submodel        Revision
; Computer        (Offset       (Offset         (Offset         BIOS
; Type             02h)         03h)             04h)           Date
; ------------------------------------------------------------------
; PC              FFh
; PC XT           FEh
; PC XT           FBh           00h             01h             1/10/86
; PC XT           FBh           00h             02h             5/09/86
; PCjr            FDh
; AT              FCh
; AT              FCh           00h             01h             6/10/85
; AT, COMPAQ 286  FCh           01h             00h             11/15/85
; PC XT 286       FCh           02h             00h
; PC Convertible  F9h           00h             00h
; PS/2 Model 30   FAh           00h             00h
; PS/2 Model 50   FCh           04h             00h
; PS/2 Model 60   FCh           05h             00h
; PS/2 Model 80   F8h           00h             00h
;----------------------------------------------------------------------------;
getsyscfg       PROC C
USES    BX,ES
	MOV     AH,00C0h
	INT     15h
	MOV     DX,ES
	MOV     AX,BX

	MOV	SYSCFGOFF,AX
	MOV     SYSCFGSEG,DX
	RET
getsyscfg       ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     PS/2 Services
;
; PURPOSE:
;   Get extended BIOS address BIOS service (INT 15h, service C1h).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : C1h
;
; POST:
;   RETURNCODE	: return code
;   EXTBIOSADDR	: Extended BIOS Data Area's segment address
;
;   registers:
;       AH      : Return code
;       DX, ES  : Extended BIOS Data Area's segment address
;----------------------------------------------------------------------------;
getextbiosaddr          PROC C
USES	DX,ES
	MOV     AH,00C1h
	INT     15h
	XOR     AL,AL
	MOV     DX,ES

	MOV	RETURNCODE,AH
	MOV	EXTBIOSADDR,DX

	RET
getextbiosaddr          ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     PS/2 Services
;
; PURPOSE:
;   Programmable option select BIOS service (INT 15h, service C4h).
;   Interface to C language calling convention.
;
; PRE:
;   option      : Programmable option
;
;   registers:
;       AH      : C4h
;       AL      : 00h = Return base POS register address
;               : 01h = Enable slot for setup
;               : 02h = Enable adapter
;
; POST:
;   BASEPOS	: Base POS register port address (if AL=0)
;   SLOTNUMBER	: Slot number (if AL=0)
;
;   registers:
;       DL      : Base POS register port address (if AL=0)
;       AL, BL  : Slot number (if AL=0)
;----------------------------------------------------------------------------;
programsel      PROC C
ARG     @@option:BYTE
USES	AX,BX,DX
	MOV     AL,[@@option]
	MOV     AH,00C4h
	INT     15h
	MOV     AL,BL
	XOR     AH,AH
	XOR     DH,DH

	MOV	BASEPOS,DL
	MOV	SLOTNUMBER,AL
	RET
programsel      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Keyboard Services
;
; PURPOSE:
;   Read keyboard character BIOS service (INT 16h, service 0).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 0
;
; POST:
;   registers:
;       AH      : Keyboard scan code
;       AL      : ASCII value of keystroke
;----------------------------------------------------------------------------;
rdkeybch        PROC C
	MOV     AH,0
	INT     16h

	MOV	SCANCODE,AH
	MOV	ASCIICODE,AL

	RET
rdkeybch        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Keyboard Services
;
; PURPOSE:
;   Read keyboard status BIOS service (INT 16h, service 1).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 1
;
; POST:
;   SCANCODE	: Keyboard scan code
;   ASCIICODE	: ASCII value of keystroke
;
;   registers:
;       AH      : Keyboard scan code
;       AL      : ASCII value of keystroke
;
;   flags:
;       zero flag cleared if key is available
;       zero flag set if key is unavailable
;----------------------------------------------------------------------------;
rdkeybstat      PROC C
	MOV     AH,1
	INT     16h

	JZ      @@NO_KEY

	XOR     DX,DX
	MOV	SCANCODE,AH
	MOV	ASCIICODE,AL
	JMP     @@HAS_KEY

@@NO_KEY:
	XOR     AX,AX
	XOR     DX,DX
@@HAS_KEY:
	RET
rdkeybstat      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Keyboard Services
;
; PURPOSE:
;   Read keyboard shift status BIOS service (INT 16h, service 2).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 2
;
; POST:
;   registers:
;       AL      : Shift status
;
; Keyboard shift-status values
;
; Bits
; 76543210      Hex     Decimal         Meaning of bits
; -----------------------------------------------------
; 1             80      128             Insert on
;  1            40      64              Caps lock on
;   1           20      32              Num lock on
;    1          10      16              Scroll lock on
;     1         8       8               Alt key down
;      1        4       4               Ctrl key down
;       1       2       2               Left shift key down
;        1      1       1               Right shift key down
;----------------------------------------------------------------------------;
rdkeybshft      PROC C
	MOV     AH,2
	INT     16h
	XOR     AH,AH
	RET
rdkeybshft      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Keyboard Services
;
; PURPOSE:
;   Adjust repeat rate BIOS service (INT 16h, service 3).
;   Interface to C language calling convention.
;
; PRE:
;   delay       : repeat delay
;   rate        : repeat rate
;
;   registers:
;       AH      : 3
;       AL      : 05h
;       BH      : Repeat delay (0-3 x 250 ms)
;       BL      : Repeat rate (0-31, lower values are faster)
;----------------------------------------------------------------------------;
setreprate      PROC C
ARG     @@delay:BYTE,@@rate:BYTE
USES	AX,BX
	MOV     AH,3
	MOV     AL,05h
	MOV     BH,[@@delay]
	MOV     BL,[@@rate]
	INT     16h
	RET
setreprate      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Keyboard Services
;
; PURPOSE:
;   Write to keyboard buffer BIOS service (INT 16h, service 5).
;   Interface to C language calling convention.
;
; PRE:
;   scan        : Scan code to write
;   ascii       : ASCII code to write
;
;   registers:
;       AH      : 5
;       CH      : Scan code to write
;       CL      : ASCII code to write
;
; POST:
;   registers:
;       AL      : 01h if buffer is full
;----------------------------------------------------------------------------;
wrkeybuf        PROC C
ARG     @@scan:BYTE,@@ascii:BYTE
USES	CX
	MOV     AH,5
	MOV     CH,[@@scan]
	MOV     CL,[@@ascii]
	INT     16h
	XOR     AH,AH
	RET
wrkeybuf        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Keyboard Services
;
; PURPOSE:
;   Get extended keystroke BIOS service (INT 16h, service 10h).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 10h
;
; POST:
;   scan        : Scan code to write
;   ascii       : ASCII code to write
;
;   registers:
;       AH      : Keyboard scan code
;       AL      : ASCII value of keystroke
;----------------------------------------------------------------------------;
rdextkeybch     PROC C
	MOV     AH,10h
	INT     16h

	MOV	SCANCODE,AH
	MOV	ASCIICODE,AL
	RET
rdextkeybch     ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Keyboard Services
;
; PURPOSE:
;   Check extended keyboard status BIOS service (INT 16h, service 11h).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 11h
;
; POST:
;   SCANCODE    : Scan code to write
;   ASCIICODE   : ASCII code to write
;
;   registers:
;       AH      : Keyboard scan code
;       AL      : ASCII value of keystroke
;----------------------------------------------------------------------------;
rdextkeybstat  PROC C
	MOV     AH,11h
	INT     16h

	MOV	SCANCODE,AH
	MOV	ASCIICODE,AL
	RET
rdextkeybstat  ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Printer Services
;
; PURPOSE:
;   Print char BIOS service (INT 17h, service 0).
;   Interface to C language calling convention.
;
; PRE:
;   asch        : character to print
;   printer     : printer to be used
;
;   registers:
;       AH      : 0
;       AL      : Character to print
;       DX      : Printer to be used
;
; POST:
;   registers:
;       AH      : Printer status
;
; Meaning of bits returned in AH for service 17/0, 17/1, and 17/2
;
; Bits
; 76543210      Meaning of bits
; -----------------------------
; 1             Printer not busy
; 0             Printer busy
;  1            Printer acknowledgment
;   1           Out of paper
;    1          Printer selected
;     1         I/O error
;      ??       Unused
;        1      Time-out
;----------------------------------------------------------------------------;
prtch   PROC C
ARG     @@asch:BYTE,@@printer:WORD
USES	DX
	MOV     AH,0
	MOV     AL,[@@asch]
	MOV     DX,[@@printer]
	INT     17h
	XOR     AL,AL
	RET
prtch   ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Printer Services
;
; PURPOSE:
;   Initialize printer BIOS service (INT 17h, service 1).
;   Interface to C language calling convention.
;
; PRE:
;   printer     : printer to be used
;
;   registers:
;       AH      : 1
;       DX      : Printer to be used
;
; POST:
;   registers:
;       AH      : Printer status
;
; Meaning of bits returned in AH for service 17/0, 17/1, and 17/2
;
; Bits
; 76543210      Meaning of bits
; -----------------------------
; 1             Printer not busy
; 0             Printer busy
;  1            Printer acknowledgment
;   1           Out of paper
;    1          Printer selected
;     1         I/O error
;      ??       Unused
;        1      Time-out
;----------------------------------------------------------------------------;
initprt PROC C
ARG     @@printer:WORD
USES	DX
	MOV     AH,1
	MOV     DX,[@@printer]
	INT     17h
	XOR     AL,AL
	RET
initprt ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Printer Services
;
; PURPOSE:
;   Get printer status BIOS service (INT 17h, service 2).
;   Interface to C language calling convention.
;
; PRE:
;   printer     : printer to be used
;
;   registers:
;       AH      : 2
;       DX      : Printer to be used
;
; POST:
;   registers:
;       AH      : Printer status
;
; Meaning of bits returned in AH for service 17/0, 17/1, and 17/2
;
; Bits
; 76543210      Meaning of bits
; -----------------------------
; 1             Printer not busy
; 0             Printer busy
;  1            Printer acknowledgment
;   1           Out of paper
;    1          Printer selected
;     1         I/O error
;      ??       Unused
;        1      Time-out
;----------------------------------------------------------------------------;
getprtstat      PROC C
ARG     @@printer:WORD
USES	DX
	MOV     AH,2
	MOV     DX,[@@printer]
	INT     17h
	XOR     AL,AL
	RET
getprtstat      ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     System Services
;
; PURPOSE:
;   Warm boot BIOS service (INT 19h).
;   Interface to C language calling convention.
;----------------------------------------------------------------------------;
warmboot        PROC C
	INT 19h
	RET
warmboot        ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Get clock counter BIOS service (INT 1Ah, service 0).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;       AH      : 0
;
; POST:
;   info        : information structure
;		: MIDNITE 	: Midnight flag
;		: HICLKCNT 	: Clock count high-order word
;		: LOCLKCNT 	: Clock count low-order word
;
;   registers:
;       AL      : Midnight flag
;       CX      : Clock count high-order word
;       DX      : Clock count low-order word
;----------------------------------------------------------------------------;
getclkcnt       PROC C
ARG     @@info:WORD
USES	AX,CX,DX
	MOV     AH,0
	INT     1Ah

	MOV     MIDNITE,AL
	MOV     HICLKCNT,CX
	MOV     LOCLKCNT,DX

	RET
getclkcnt       ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Set clock counter BIOS service (INT 1Ah, service 1).
;   Interface to C language calling convention.
;
; PRE:
;   clkhi	: Clock count high-order word
;   clklo	: Clock count low-order word
;
;   registers:
;       AH      : 1
;       CX      : Clock count high-order word
;       DX      : Clock count low-order word
;----------------------------------------------------------------------------;
setclkcnt       PROC C
ARG	@@clkhi:WORD,@@clklo:WORD
USES	AX,CX,DX
	MOV	CX,[@@clkhi]
	MOV	DX,[@@clklo]
	MOV	AH,1
	INT	1Ah
	RET
setclkcnt       ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Read real-time clock BIOS service (INT 1Ah, service 2).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;	AH	: 2
;
; POST:
;		HOURS 	: Hours (BCD)
;		MINUTES : Minutes (BCD)
;		SECONDS : Seconds (BCD)
;
;   registers:
;	AH, CH	: Hours (BCD)
;	AL, CL	: Minutes (BCD)
;	DH	: Seconds (BCD)
;----------------------------------------------------------------------------;
rdrealclk	PROC C
USES	CX
	MOV	AH,2
	INT	1Ah
	MOV	AX,CX
	XOR 	DL,DL

	MOV	HOURS,CH
	MOV	MINUTES,CL
	MOV	SECONDS,DH
	RET
rdrealclk	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Set real-time clock BIOS service (INT 1Ah, service 3).
;   Interface to C language calling convention.
;
; PRE:
;		hours			: Hours (BCD)
;		minutes 		: Minutes (BCD)
;		seconds 		: Seconds (BCD)
;		daylightsavings 	: Daylight savings time
;
;   registers:
;	AH	: 3
;	CH	: Hours (BCD)
;	CL	: Minutes (BCD)
;	DH	: Seconds (BCD)
;	DL	: Daylight savings time
;----------------------------------------------------------------------------;
setrealclk	PROC C
ARG	@@hours:BYTE,@@minutes:BYTE,@@seconds:BYTE,@@daylightsavings:BYTE
USES	AX,CX,DX
	MOV	AH,3
	MOV	CH,[@@hours]
	MOV	CL,[@@minutes]
	MOV	DH,[@@seconds]
	MOV	DL,[@@daylightsavings]
	INT	1Ah
	RET
setrealclk	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Read date from real-time clock BIOS service (INT 1Ah, service 4).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;	AH	: 4
;
; POST:
;		CENTURY : Century (BCD)
;		YEAR 	: Year (BCD)
;		MONTH 	: Month (BCD)
;		DAY 	: Day (BCD)
;
;   registers:
;	AH, CH	: Century (BCD)
;	AL, CL	: Year (BCD)
;	DH	: Month (BCD)
;	DL	: Day (BCD)
;----------------------------------------------------------------------------;
rdrealdate	PROC C
USES	CX
	MOV	AH,4
	INT	1Ah
	MOV	AX,CX

	MOV	CENTURY,AH
	MOV	YEAR,AL
	MOV	MONTH,DH
	MOV	DAY,DL
	RET
rdrealdate	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Read date from real-time clock BIOS service (INT 1Ah, service 5).
;   Interface to C language calling convention.
;
; PRE:
;		century     : Century (BCD)
;		year	    : Year (BCD)
;		month	    : Month (BCD)
;		day	    : Day (BCD)
;
;   registers:
;	AH	: 5
;	CH	: Century (BCD)
;	CL	: Year (BCD)
;	DH	: Month (BCD)
;	DL	: Day (BCD)
;----------------------------------------------------------------------------;
setrealdate	PROC C
ARG	@@century:BYTE,@@year:BYTE,@@month:BYTE,@@day:BYTE
USES	AX,CX,DX
	MOV	AH,5
	MOV     CH,[@@century]
	MOV	CL,[@@year]
	MOV	DH,[@@month]
	MOV	DL,[@@day]
	INT     1Ah
	RET
setrealdate	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Set alarm BIOS service (INT 1Ah, service 6).
;   Interface to C language calling convention.
;
;   The BIOS alarm function simply generates an interrupt signal after the
;   appropriate period of time has elapsed. The address of the routine you
;   want to perform should be vectored to interrupt 4Ah
;
; PRE:
;		hours 	: hours (BCD)
;		minutes	: minutes (BCD)
;		seconds	: seconds (BCD)
;
;   registers:
;	AH	: 6
;	CH	: Hours (BCD)
;	CL	: Minutes (BCD)
;	DH	: Seconds (BCD)
;----------------------------------------------------------------------------;
setalarm	PROC C
ARG     @@hours:BYTE,@@minutes:BYTE,@@seconds:BYTE
USES	AX,CX,DX
	MOV	AH,6
	MOV	CH,[@@hours]
	MOV	CL,[@@minutes]
	MOV	DH,[@@seconds]
	INT	1Ah
	RET
setalarm	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Disable alarm BIOS service (INT 1Ah, service 7).
;   Interface to C language calling convention.
;
;   The BIOS alarm function simply generates an interrupt signal after the
;   appropriate period of time has elapsed. The address of the routine you
;   want to perform should be vectored to interrupt 4Ah
;
; PRE:
;   registers:
;	AH	: 7
;----------------------------------------------------------------------------;
disablealarm	PROC C
	MOV	AH,7
	INT	1Ah
	RET
disablealarm	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Read alarm BIOS service (INT 1Ah, service 9).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;	AH	: 9
;
; POST:
;		HOURS 		: hours (BCD)
;		MINUTES 	: minutes (BCD)
;		SECONDS 	: seconds (BCD)
;		ALARMSTATUS 	: alarm status
;
;   registers:
;	AH, CH	: Hours (BCD)
;	AL, CL	: Minutes (BCD)
;	DH	: Seconds (BCD)
;	DL	: Alarm status:
;		     00 = Not enabled
;		     01 = Enabled, no power on
;		     02 = Enabled, will power system on when alarm triggers
;----------------------------------------------------------------------------;
rdalarm	PROC C
ARG	@@info:WORD
USES	CX
	MOV	AH,9
	INT	1Ah
	MOV	AX,CX

	MOV     HOURS,AH
	MOV	MINUTES,AL
	MOV	SECONDS,DH
	MOV	ALARMSTATUS,DL
	RET
rdalarm	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Get day count BIOS service (INT 1Ah, service 0Ah).
;   Interface to C language calling convention.
;
; PRE:
;   registers:
;	AH	: 0Ah
;
; POST:
;   registers:
;	AX, CX	: Total count of days since 01/01/80
;----------------------------------------------------------------------------;
getdaycnt	PROC C
USES	CX
	MOV	AH,0Ah
	INT	1Ah
	MOV	AX,CX
	RET
getdaycnt	ENDP

;----------------------------------------------------------------------------;
; CATEGORY:     Date/time Services
;
; PURPOSE:
;   Set day count BIOS service (INT 1Ah, service 0Bh).
;   Interface to C language calling convention.
;
; PRE:
;   dcnt	: Total count of days since 01/01/80
;
;   registers:
;	AH	: 0Bh
;	CX	: Total count of days since 01/01/80
;----------------------------------------------------------------------------;
setdaycnt	PROC C
ARG	@@dcnt:WORD
USES	AX,CX
	MOV	AH,0Bh
	MOV	CX,[@@dcnt]
	INT	1Ah
	RET
setdaycnt	ENDP

;----------------------------------------------------------------------------;
	END
