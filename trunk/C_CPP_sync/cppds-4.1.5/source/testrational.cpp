//==========================================================
//  Numerics Library
//==========================================================
//  TestRational.cpp
//
//  A test module for rational numbers (fractions).
//
//  Copyright 1992-96 Scott Robert Ladd. All rights reserved
//==========================================================

#include "rational.cpp"

void TestRational(ostream & buffer)
{
    Rational<long> r1, r2, r3;

    r1(1l,2l);
    r2(2l,3l);

    r3 = r1 + r2;
    buffer << r1 << " + " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 - r2;
    buffer << r1 << " - " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 * r2;
    buffer << r1 << " * " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 / r2;
    buffer << r1 << " / " << r2 << " = " << r3 << "\r\n"; 
    buffer << "\r\n";

    r1(-1l,2l);
    r2(2l,3l);

    r3 = r1 + r2;
    buffer << r1 << " + " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 - r2;
    buffer << r1 << " - " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 * r2;
    buffer << r1 << " * " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 / r2;
    buffer << r1 << " / " << r2 << " = " << r3 << "\r\n"; 
    buffer << "\r\n";

    r1(-1l,2l);
    r2(2l,-3l);

    r3 = r1 + r2;
    buffer << r1 << " + " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 - r2;
    buffer << r1 << " - " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 * r2;
    buffer << r1 << " * " << r2 << " = " << r3 << "\r\n"; 

    r3 = r1 / r2;
    buffer << r1 << " / " << r2 << " = " << r3 << "\r\n"; 
    buffer << "\r\n";

    r1(-1l,2l);
    r2(2l,3l);

    buffer << r1 << " = " << float(r1) << "\r\n";
    buffer << r1 << " = " << double(r1) << "\r\n";
    buffer << r2 << " = " << float(r2) << "\r\n";
    buffer << r2 << " = " << double(r2) << "\r\n";
    buffer << "\r\n";

    r1 = -r1;
    r2 = -r2;

    buffer << r1 << "\r\n";
    buffer << r2 << "\r\n";
}

int main()
{
  TestRational(cout);
  cout <<endl <<"SUCCESS Testing Rational" <<endl;
  return 0;
}
