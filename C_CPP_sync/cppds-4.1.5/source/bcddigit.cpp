//
// BcdDigit.cpp
//
#ifndef BCDDIGIT_CPP
#define BCDDIGIT_CPP
#ifndef BCDDIGIT_H
  #include "bcddigit.h"
#endif

/****************************************************************************/
BcdDigit operator * (const BcdDigit& Opr1_, const BcdDigit& Opr2_)
{
  BcdDigit Temp_ = Opr1_;
  return Temp_.Multiply(Opr2_);
}

/****************************************************************************/
BcdDigit operator + (const BcdDigit& Opr1_, const BcdDigit& Opr2_)
{
  BcdDigit Temp_ = Opr1_;
  return Temp_.Add(Opr2_);
}

/****************************************************************************/
BcdDigit operator - (const BcdDigit& Opr1_, const BcdDigit& Opr2_)
{
  BcdDigit Temp_ = Opr1_;
  return Temp_.Subtract(Temp_, Opr2_, 1);
}

/****************************************************************************/
BcdDigit::BcdDigit():
_Digit(0),
_Carry(0)
{}

/****************************************************************************/
BcdDigit::BcdDigit(Ulong Digit_, Ulong Exponent_):
_Digit((Digit_ >> ((Exponent_ % DigitsPerLong()) * 4)) & 0x0F),
_Carry(0)
{}

/****************************************************************************/
BcdDigit::BcdDigit(const BcdDigit& Obj_):
_Digit(Obj_._Digit),
_Carry(Obj_._Carry)
{}

/****************************************************************************/
Ulong BcdDigit::DigitsPerLong()
{
  return (BYTES_PER_LONG * 2);
}

/****************************************************************************/
Ulong BcdDigit::BitsPerLong()
{
  return (BYTES_PER_LONG * 8);
}

/****************************************************************************/
BcdDigit& BcdDigit::operator = (const BcdDigit& Obj_)
{
  if (this != &Obj_)
  {
    _Digit = Obj_._Digit;
    _Carry = Obj_._Carry;
  }

  return *this;
}

/****************************************************************************/
BcdDigit& BcdDigit::AddCarry(const BcdDigit& Obj1_, const BcdDigit& Obj2_)
{
  _Digit = (_Carry + Obj1_._Digit + Obj2_._Digit);
  _Carry = _Digit / 10;  
  _Digit %= 10;

  return *this;
}

/****************************************************************************/
BcdDigit& BcdDigit::Add(const BcdDigit& Obj_)
{
  _Digit += Obj_._Digit;
  _Carry = _Digit / 10;
  _Digit %= 10;

  return *this;
}

/****************************************************************************/
BcdDigit& BcdDigit::Multiply(const BcdDigit& Obj_)
{
  _Digit *= Obj_._Digit;
  _Carry = _Digit / 10;
  _Digit %= 10;

  return *this;
}

/****************************************************************************/
BcdDigit& BcdDigit::Subtract(const BcdDigit& Obj1_, const BcdDigit& Obj2_, int OnesCol_)
{
  BcdDigit Subt_(Obj2_);
  Subt_.SetCarry(0);
  Subt_.Complement();
  
  if (OnesCol_)
    Subt_.Add(BcdDigit(1));
  
  AddCarry(Obj1_, Subt_);
  _Carry += Subt_.Carry();
  
  return *this;
}

/****************************************************************************/
BcdDigit& BcdDigit::Complement()
{
  _Digit = 9 - _Digit;
  return *this;
}

/****************************************************************************/
#if BCDCLASS_DEBUG
#if defined(__TURBOC__) | defined(__DJGPP__)
  #include <conio.h>
#endif
#include <iostream.h>

void main()
{
#if defined(__TURBOC__) | defined(__DJGPP__)
  clrscr();
#endif

  BcdDigit d1(0x8L << 28, 7);           // 8
  BcdDigit d2(0x5L << 28, 7);           // 5
  BcdDigit d3;

  d3 = d1 * d2;
  cout <<"Digit = " <<d3.Digit() <<" Carry = " <<d3.Carry() <<endl;
  d3 = d1 + d2;
  cout <<"Digit = " <<d3.Digit() <<" Carry = " <<d3.Carry() <<endl;
  d3 = d1 - d2;
  cout <<"Digit = " <<d3.Digit() <<" Carry = " <<d3.Carry() <<endl;
  d3 = d2 - d1;
  cout <<"Digit = " <<d3.Digit() <<" Carry = " <<d3.Carry() <<endl;
}
#endif
/****************************************************************************/
#endif




