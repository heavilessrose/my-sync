//-----------------------------------------------------------
//  TOOLS
//    C++ Templates & Tools, 2nd Edition
//-----------------------------------------------------------
//
//      TestPoly.cpp
//
//      Procedures for testing polynomial classes.
//
//-----------------------------------------------------------
//  Copyright 1996 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include <sstream>
#include <iomanip.h>
#include <limits.h>

#include "polynomial.h"

// utility routine to display a matrix
template <class T>
void ShowPoly(ostream & buffer, const Polynomial<T> p)
{
        const size_t D = p.Degree();

        buffer << "[ ";

        size_t i = D - 1;

        while(1)
        {
            buffer << p.Get(i) << " ";

            if (i == 0)
                break;

            --i;
        }

        buffer << "]";
}

template <class T>
void BinaryOut(ostream & str, T x)
{
        T mask = 1 << (CHAR_BIT * sizeof(T) - 1);

        while (1)
        {
            str << ((x & mask) ? "1" : "0");

            mask >>= 1;

            if (mask == 0)
                break;
        }
}

//-----------------
// Test Polynomials
//-----------------

void TestPolynomials(ostream& buffer)
{
    buffer << "Polynomial test\r\n===============\r\n\r\n";

    Polynomial<int> p1(4), p2(4), p3(2), p4(4), p5(8);

    p1[0U] =  1; p2[0U] =  2; p3[0U] =  2;
    p1[1U] =  2; p2[1U] = -1; p3[1U] = -1;
    p1[2U] =  0; p2[2U] =  0;
    p1[3U] = -3; p2[3U] =  0;

    buffer << "p1 = "; ShowPoly(buffer,p1); buffer << "\r\n";
    buffer << "p2 = "; ShowPoly(buffer,p2); buffer << "\r\n";
    buffer << "p3 = "; ShowPoly(buffer,p3); buffer << "\r\n";

    buffer << "p1 + p2 = "; ShowPoly(buffer,p1 + p2); buffer << "\r\n";
    buffer << "p1 + p3 = "; ShowPoly(buffer,p1 + p3.Stretch(4)); buffer << "\r\n";

    p4 = p1;
    p4 += p2;
    buffer << "p1 += p2 = "; ShowPoly(buffer,p4); buffer << "\r\n";

    p5 = p1 * p2;
    buffer << "p5 = p1 * p2"; ShowPoly(buffer,p5); buffer << "\r\n";
    buffer << "p1(3) = " << p1(3) << "\r\n";
    buffer << "p5(2) = " << p5(2) << "\r\n";

    int a1[] = { 9, -10, 7,  6};
    int a2[] = {-5,   4, 0, -2};

    Polynomial<int> x1(4,a1), x2(4), x3(7);
    x2 = a2;

    buffer << "x1 = "; ShowPoly(buffer,x1); buffer << "\r\n";
    buffer << "x2 = "; ShowPoly(buffer,x2); buffer << "\r\n";

    x3 = x1 * x2;
    buffer << "x3 = x1 * x2"; ShowPoly(buffer,x3); buffer << "\r\n\r\n";

    int finit[] = {-6,0,1,-3};
    int ginit[] = {1,1,0,5};
    int hinit[] = {1,2,3,4,5};
    Polynomial<int> f(4,finit), g(4,ginit), h(5,hinit);

    buffer << "f = "; ShowPoly(buffer,f); buffer << "\r\n";
    buffer << "g = "; ShowPoly(buffer,g); buffer << "\r\n";
    buffer << "h = "; ShowPoly(buffer,h); buffer << "\r\n";

    buffer << "g + f = "; ShowPoly(buffer,g + f); buffer << "\r\n";
    buffer << "f - g = "; ShowPoly(buffer,f - g); buffer << "\r\n";
    buffer << "f * g = "; ShowPoly(buffer,f * g); buffer << "\r\n";
    buffer << "f - h = "; ShowPoly(buffer,f - h); buffer << "\r\n";
    buffer << "h - f = "; ShowPoly(buffer,h - f); buffer << "\r\n";
    buffer << "f + h = "; ShowPoly(buffer,f + h); buffer << "\r\n";
    buffer << "h + f = "; ShowPoly(buffer,h + f); buffer << "\r\n\r\n";

    // test FFT multiply
    double px1init[] = { 9.0, -10.0, 7.0,  6.0};
    double px2init[] = {-5.0,   4.0, 0.0, -2.0};
    DPoly px1(4,px1init);
    DPoly px2(4,px2init);

    buffer << "px1 = "; ShowPoly(buffer,px1); buffer << "\r\n";
    buffer << "px2 = "; ShowPoly(buffer,px2); buffer << "\r\n";

    buffer << "\r\n           px1 * px2 = ";
    ShowPoly(buffer,px1 * px2);
    buffer << "\r\n";

    buffer << "FFTMultiply(px1,px2) = ";
    ShowPoly(buffer,FFTMultiply(px1,px2));
    buffer << "\r\n\r\n";

    double z1init[] = { 1.0, 1.1, 2.0, 2.2, 3.0, 3.3, 4.0, 4.4,
                        5.0, 5.5, 6.0, 6.6, 7.0, 7.7, 8.0, 8.8 };

    double z2init[] = { 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8,
                        2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8 };

    DPoly z1(16,z1init);
    DPoly z2(16,z2init);

    buffer << "z1 = "; ShowPoly(buffer,z1); buffer << "\r\n";
    buffer << "z2 = "; ShowPoly(buffer,z2); buffer << "\r\n";

    buffer << "\r\n           z1 * z2 = ";
    ShowPoly(buffer,z1 * z2);
    buffer << "\r\n";

    buffer << "FFTMultiply(z1,z2) = ";
    ShowPoly(buffer,FFTMultiply(z1,z2));
    buffer << "\r\n";
}

int main()
{
  TestPolynomials(cout);
  return 0;
}




