#ifndef TXTGRAPH_CPP
#define TXTGRAPH_CPP
#ifndef TXTGRAPH_H
  #include "txtgraph.h"
#endif

/****************************************************************************/
// Standard text screen dimensions
const short TextGraphics::SCREENLEN	= 80;
const short TextGraphics::HALFLEN	= 40;
const short TextGraphics::SCREENHT	= 25;

// single border line characters
const TextGraphics::ExtChar_t TextGraphics::SVL	= 179;
const TextGraphics::ExtChar_t TextGraphics::SHL	= 196;
const TextGraphics::ExtChar_t TextGraphics::SRTC	= 191;
const TextGraphics::ExtChar_t TextGraphics::SLBC	= 192;
const TextGraphics::ExtChar_t TextGraphics::SRBC	= 217;
const TextGraphics::ExtChar_t TextGraphics::SLTC	= 218;
const TextGraphics::ExtChar_t TextGraphics::SBT	= 193;
const TextGraphics::ExtChar_t TextGraphics::STT	= 194;
const TextGraphics::ExtChar_t TextGraphics::SLT	= 195;
const TextGraphics::ExtChar_t TextGraphics::SRT	= 180;
const TextGraphics::ExtChar_t TextGraphics::SXC	= 197;

// double border line characters
const TextGraphics::ExtChar_t TextGraphics::DVL	= 186;
const TextGraphics::ExtChar_t TextGraphics::DHL	= 205;
const TextGraphics::ExtChar_t TextGraphics::DLBC	= 200;
const TextGraphics::ExtChar_t TextGraphics::DLTC	= 201;
const TextGraphics::ExtChar_t TextGraphics::DRTC	= 187;
const TextGraphics::ExtChar_t TextGraphics::DRBC	= 188;
const TextGraphics::ExtChar_t TextGraphics::DRT	= 185;
const TextGraphics::ExtChar_t TextGraphics::DLT	= 204;
const TextGraphics::ExtChar_t TextGraphics::DBT	= 202;
const TextGraphics::ExtChar_t TextGraphics::DTT	= 203;
const TextGraphics::ExtChar_t TextGraphics::DXC	= 206;

// double horizontal, single vertical characters
const TextGraphics::ExtChar_t TextGraphics::DHSVLBC	= 212;
const TextGraphics::ExtChar_t TextGraphics::DHSVLTC	= 213;
const TextGraphics::ExtChar_t TextGraphics::DHSVRTC	= 184;
const TextGraphics::ExtChar_t TextGraphics::DHSVRBC	= 190;
const TextGraphics::ExtChar_t TextGraphics::DHSVRT	= 181;
const TextGraphics::ExtChar_t TextGraphics::DHSVLT	= 198;
const TextGraphics::ExtChar_t TextGraphics::DHSVBT	= 207;
const TextGraphics::ExtChar_t TextGraphics::DHSVTT	= 209;
const TextGraphics::ExtChar_t TextGraphics::DHSVXC	= 216;

// single horizontal, double vertical characters
const TextGraphics::ExtChar_t TextGraphics::SHDVLBC	= 211;
const TextGraphics::ExtChar_t TextGraphics::SHDVLTC	= 214;
const TextGraphics::ExtChar_t TextGraphics::SHDVRTC	= 183;
const TextGraphics::ExtChar_t TextGraphics::SHDVRBC	= 189;
const TextGraphics::ExtChar_t TextGraphics::SHDVRT	= 182;
const TextGraphics::ExtChar_t TextGraphics::SHDVLT	= 199;
const TextGraphics::ExtChar_t TextGraphics::SHDVBT	= 208;
const TextGraphics::ExtChar_t TextGraphics::SHDVTT	= 210;
const TextGraphics::ExtChar_t TextGraphics::SHDVXC	= 215;

const TextGraphics::GraphTable_t TextGraphics::_GraphTable =
{
  {TextGraphics::SHL,
   TextGraphics::SVL,
   TextGraphics::SLTC,
   TextGraphics::SLBC,
   TextGraphics::SRTC,
   TextGraphics::SRBC,
   TextGraphics::SLT,
   TextGraphics::SRT,
   TextGraphics::STT,
   TextGraphics::SBT,
   TextGraphics::SXC},

  {TextGraphics::DHL,
   TextGraphics::DVL,
   TextGraphics::DLTC,
   TextGraphics::DLBC,
   TextGraphics::DRTC,
   TextGraphics::DRBC,
   TextGraphics::DLT,
   TextGraphics::DRT,
   TextGraphics::DTT,
   TextGraphics::DBT,
   TextGraphics::DXC},

  {TextGraphics::DHL,
   TextGraphics::SVL,
   TextGraphics::DHSVLTC,
   TextGraphics::DHSVLBC,
   TextGraphics::DHSVRTC,
   TextGraphics::DHSVRBC,
   TextGraphics::DHSVLT,
   TextGraphics::DHSVRT,
   TextGraphics::DHSVTT,
   TextGraphics::DHSVBT,
   TextGraphics::DHSVXC},

  {TextGraphics::SHL,
   TextGraphics::DVL,
   TextGraphics::SHDVLTC,
   TextGraphics::SHDVLBC,
   TextGraphics::SHDVRTC,
   TextGraphics::SHDVRBC,
   TextGraphics::SHDVLT,
   TextGraphics::SHDVRT,
   TextGraphics::SHDVTT,
   TextGraphics::SHDVBT,
   TextGraphics::SHDVXC}
};

/****************************************************************************/
const TextGraphics::ExtChar_t TextGraphics::GRAPHTABLE(GraphType_t Type_, GraphChar_t Char_)
{
  return ((Type_ == NONE) ? 0:_GraphTable[Type_][Char_]);
}

/****************************************************************************/
/****************************************************************************/
#if TXTGRAPH_DEBUG
//#include <conio.h>
#include <iostream.h>
void main()
{
//  clrscr();
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::SINGLE, TextGraphics::HL) <<endl;
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::SINGLE, TextGraphics::VL) <<endl;
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::DOUBLE, TextGraphics::HL) <<endl;
  cout <<TextGraphics::GRAPHTABLE(TextGraphics::DOUBLE, TextGraphics::VL) <<endl;

  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 11; y++)
      cout <<TextGraphics::GRAPHTABLE((TextGraphics::GraphType_t)x, (TextGraphics::GraphChar_t)y);
    cout <<endl;
  }
}
#endif
/****************************************************************************/
#endif





