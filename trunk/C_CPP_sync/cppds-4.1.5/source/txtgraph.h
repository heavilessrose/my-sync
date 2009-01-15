/*****************************************************************************
* FILE		: TGRDEFS.H
* PURPOSE	: Text graphics definitions class header file
*
* AUTHOR	: Joseph Wong
* MODIFIED BY	:
* LAST MODIFIED	: Dec 17 1997
*****************************************************************************/
#ifndef TEXTGRAPHICS_H
#define TEXTGRAPHICS_H

#define	TXTGRAPH_DEBUG  0

/******************************** Constants *********************************/
class TextGraphics
{
  public:
    // Extended character set type definition
    typedef unsigned char ExtChar_t;
    typedef ExtChar_t GraphTable_t[4][11];

    // Enumerated types for indexing characters from the graphics table
    typedef enum {SINGLE, DOUBLE, HDOUBLE, VDOUBLE, NONE} GraphType_t;
    typedef enum {HL, VL, LTC, LBC, RTC, RBC, LT, RT, TT, BT, XC} GraphChar_t;

    // Standard text screen dimensions
    static const short SCREENLEN;
    static const short HALFLEN;
    static const short SCREENHT;

    // single border line characters
    static const ExtChar_t SVL;
    static const ExtChar_t SHL;
    static const ExtChar_t SRTC;
    static const ExtChar_t SLBC;
    static const ExtChar_t SRBC;
    static const ExtChar_t SLTC;
    static const ExtChar_t SBT;
    static const ExtChar_t STT;
    static const ExtChar_t SLT;
    static const ExtChar_t SRT;
    static const ExtChar_t SXC;

    // double border line characters
    static const ExtChar_t DVL;
    static const ExtChar_t DHL;
    static const ExtChar_t DLBC;
    static const ExtChar_t DLTC;
    static const ExtChar_t DRTC;
    static const ExtChar_t DRBC;
    static const ExtChar_t DRT;
    static const ExtChar_t DLT;
    static const ExtChar_t DBT;
    static const ExtChar_t DTT;
    static const ExtChar_t DXC;

    // double horizontal, single vertical characters
    static const ExtChar_t DHSVLBC;
    static const ExtChar_t DHSVLTC;
    static const ExtChar_t DHSVRTC;
    static const ExtChar_t DHSVRBC;
    static const ExtChar_t DHSVRT;
    static const ExtChar_t DHSVLT;
    static const ExtChar_t DHSVBT;
    static const ExtChar_t DHSVTT;
    static const ExtChar_t DHSVXC;

    // single horizontal, double vertical characters
    static const ExtChar_t SHDVLBC;
    static const ExtChar_t SHDVLTC;
    static const ExtChar_t SHDVRTC;
    static const ExtChar_t SHDVRBC;
    static const ExtChar_t SHDVRT;
    static const ExtChar_t SHDVLT;
    static const ExtChar_t SHDVBT;
    static const ExtChar_t SHDVTT;
    static const ExtChar_t SHDVXC;

    // Graphics table accessor function
    static const ExtChar_t GRAPHTABLE(GraphType_t Type_, GraphChar_t Char_);

  protected:
    static const GraphTable_t _GraphTable;
};

/****************************************************************************/
#endif





