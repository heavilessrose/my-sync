//
// Bcddigit.h
//
#ifndef BCDDIGIT_H
#define BCDDIGIT_H

/****************************************************************************/
typedef unsigned short Ushort;
typedef unsigned long Ulong;
typedef unsigned char Byte_t;

#define BCDCLASS_DEBUG          0

class BcdDigit
{
  friend BcdDigit operator * (const BcdDigit& Opr1_, const BcdDigit& Opr2_);
  friend BcdDigit operator + (const BcdDigit& Opr1_, const BcdDigit& Opr2_);
  friend BcdDigit operator - (const BcdDigit& Opr1_, const BcdDigit& Opr2_);

  protected:
    char _Digit;
    char _Carry;

  public:
    enum { BYTES_PER_LONG = 4 };
  
    BcdDigit();
    BcdDigit(Ulong Digit_, Ulong Exponent_=0);
    BcdDigit(const BcdDigit& Obj_);

    static Ulong DigitsPerLong();
    static Ulong BitsPerLong();

    BcdDigit& operator = (const BcdDigit& Obj_);

    BcdDigit& Add(const BcdDigit& Obj_);
    BcdDigit& AddCarry(const BcdDigit& Obj1_, const BcdDigit& Obj2_=BcdDigit());
    BcdDigit& Subtract(const BcdDigit& Obj1_, const BcdDigit& Obj2_, int OnesCol_);
    BcdDigit& Multiply(const BcdDigit& Obj_);
    BcdDigit& Complement();

    Ushort Carry() const
	{ return (_Carry & 0x0F); }
    Ushort Digit() const
	{ return (_Digit & 0x0F); }

    void SetCarry(Ushort Carry_)
	{ _Carry = Carry_; }
    void SetDigit(Ushort Digit_)
	{ _Digit = Digit_; }
};

/****************************************************************************/
#endif




