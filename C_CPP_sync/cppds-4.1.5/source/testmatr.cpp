//-----------------------------------------------------------
//  TOOLS
//    C++ Templates & Tools, 2nd Edition
//-----------------------------------------------------------
//
//      TestMatrix.cpp
//
//      Procedures for testing matrix classes
//
//-----------------------------------------------------------
//  Copyright 1996 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include <iomanip.h>
#include <limits.h>

//#include "ToolsThread.h"
#include "matrix.h"

// utility routine to display a matrix
template <class T>
void ShowMatrix(ostream& os, const Matrix<T>& m)
{
  const size_t MaxR = m.GetRows();
  const size_t MaxC = m.GetCols();
  size_t r, c;

  for (r = 0; r < MaxR; ++r)
  {
    os << "[ ";

    for (c = 0; c < MaxC; ++c)
      os << setw(3) << m.Get(r,c) << " ";

    os << "]\r\n";
  }
}

int Times2(const int & x)
{
  return x * 2;
}

//--------------
// Test matrices
//--------------

void TestMatrix(ostream& os)
{
    // display a headline
    os << "Matrix test\r\n===========\r\n";

    Matrix<int> A(3,3), B(3,3), C(3,3), D(3,3);

    A(0,0) =   1;
    A(0,1) =   3;
    A(0,2) =  -4;
    A(1,0) =   1;
    A(1,1) =   1;
    A(1,2) =  -2;
    A(2,0) =  -1;
    A(2,1) =  -2;
    A(2,2) =   5;

    B(0,0) =   8;
    B(0,1) =   3;
    B(0,2) =   0;
    B(1,0) =   3;
    B(1,1) =  10;
    B(1,2) =   2;
    B(2,0) =   0;
    B(2,1) =   2;
    B(2,2) =   6;

    D(0,0) =   1;
    D(0,1) =   2;
    D(0,2) =  -1;
    D(1,0) =   2;
    D(1,1) =  -1;
    D(1,2) =  -3;
    D(2,0) =   0;
    D(2,1) =  -2;
    D(2,2) =   4;

    os << "\r\nMatrix A = \r\n";
    ShowMatrix(os,A);

    os << "\r\nMatrix B = \r\n";
    ShowMatrix(os,B);

    C = A % B;
    os << "\r\nMatrix C (A % B) = \r\n";
    ShowMatrix(os,C);

    C = A + B;
    os << "\r\nMatrix C (A + B) = \r\n";
    ShowMatrix(os,C);

    C = A;
    C += B;
    os << "\r\nMatrix C (= A, += B) =\r\n";
    ShowMatrix(os,C);

    C = A + 1;
    os << "\r\nMatrix C (= A + 1) =\r\n";
    ShowMatrix(os,C);

    C += 1;
    os << "\r\nMatrix C (+= 1) =\r\n";
    ShowMatrix(os,C);

    C = A - B;
    os << "\r\nMatrix C (A - B) = \r\n";
    ShowMatrix(os,C);

    C = A;
    C -= B;
    os << "\r\nMatrix C (= A, -= B) =\r\n";
    ShowMatrix(os,C);

    C = A - 1;
    os << "\r\nMatrix C (= A - 1) =\r\n";
    ShowMatrix(os,C);

    C -= 1;
    os << "\r\nMatrix C (-= 1) =\r\n";
    ShowMatrix(os,C);

    C = A * B;
    os << "\r\nMatrix C (A * B) = \r\n";
    ShowMatrix(os,C);

    C = A;
    C *= B;
    os << "\r\nMatrix C (= A, *= B) =\r\n";
    ShowMatrix(os,C);

    C = A * 2;
    os << "\r\nMatrix C (= A * 2) =\r\n";
    ShowMatrix(os,C);

    C *= 2;
    os << "\r\nMatrix C (*= 2) =\r\n";
    ShowMatrix(os,C);

    C = B / A;
    os << "\r\nMatrix C (B / A) = \r\n";
    ShowMatrix(os,C);

    C = B;
    C /= A;
    os << "\r\nMatrix C (= B, /= A) =\r\n";
    ShowMatrix(os,C);

    C = A / 2;
    os << "\r\nMatrix C (= A / 2) =\r\n";
    ShowMatrix(os,C);

    C /= 2;
    os << "\r\nMatrix C (/= 2) =\r\n";
    ShowMatrix(os,C);

    C = -A;
    os << "\r\nMatrix C (-A) = \r\n";
    ShowMatrix(os,C);

    // test comparisons
    os << "\r\nMatrix A = \r\n";
    ShowMatrix(os,A);

    os << "\r\nMatrix D = \r\n";
    ShowMatrix(os,D);

    if (A.Equals(D))
        os << "\r\nERROR: A should not equal D";
    else
        os << "\r\nOKAY: A not equal D";

    C = A;
    if (A.Equals(C))
        os << "\r\nOKAY: A equals C\r\n";
    else
        os << "\r\nERROR: A should equal C\r\n";

    Matrix<bool> I(3,3);

    I = (A == D);
    os << "\r\nMatrix I = (A == D)\r\n";
    ShowMatrix(os,I);

    I = (A != D);
    os << "\r\nMatrix I = (A != D)\r\n";
    ShowMatrix(os,I);

    I = (A < D);
    os << "\r\nMatrix I = (A < D)\r\n";
    ShowMatrix(os,I);

    I = (A <= D);
    os << "\r\nMatrix I = (A <= D)\r\n";
    ShowMatrix(os,I);

    I = (A > D);
    os << "\r\nMatrix I = (A > D)\r\n";
    ShowMatrix(os,I);

    I = (A >= D);
    os << "\r\nMatrix I = (A >= D)\r\n";
    ShowMatrix(os,I);

    // check fill function
    C.Fill(9);
    os << "\r\nC filled with 9 =\r\n";
    ShowMatrix(os,C);

    // check Apply functions
    C = Apply(A, Times2);
    os << "\r\nC = A.Apply(Times2)\r\n";
    ShowMatrix(os,C);

    C.Apply(Times2);
    os << "\r\nApply(C,Times2)\r\n";
    ShowMatrix(os,C);

    // check row and column vector functions
    Matrix<int>   S(1,1);
    Matrix<int> r1A(3,1);
    Matrix<int> c0B(1,3);

    r1A = A.VectorRow(1);
    c0B = B.VectorCol(0);

    os << "\r\nMatrix S = \r\n";
    ShowMatrix(os,S);

    os << "\r\nMatrix R1A = \r\n";
    ShowMatrix(os,r1A);

    os << "\r\nMatrix C0B = \r\n";
    ShowMatrix(os,c0B);

    if (r1A.IsRowVector())
        os << "\r\nOKAY: R1A is row vector";
    else
        os << "\r\nERROR: R1A should be a row vector";

    if (!r1A.IsColVector())
        os << "\r\nOKAY: R1A is not a column vector";
    else
        os << "\r\nERROR: R1A should not be a column vector";

    if (!c0B.IsRowVector())
        os << "\r\nOKAY: C0B is not a row vector";
    else
        os << "\r\nERROR: C0B should not be a row vector";

    if (c0B.IsColVector())
        os << "\r\nOKAY: C0B is column vector";
    else
        os << "\r\nERROR: C0B should be a column vector";

    if (c0B.IsVector())
        os << "\r\nOKAY: C0B is a vector";
    else
        os << "\r\nERROR: C0B should be a vector";

    if (!A.IsVector())
        os << "\r\nOKAY: A is not a vector";
    else
        os << "\r\nERROR: A should not be a vector";

    if (!c0B.IsSquare())
        os << "\r\nOKAY: C0B is not square";
    else
        os << "\r\nERROR: C0B should not be square";

    if (A.IsSquare())
        os << "\r\nOKAY: A is square";
    else
        os << "\r\nERROR: A should be square";

    B.Fill(0);

    if (B.IsZero())
        os << "\r\nOKAY: B is zero";
    else
        os << "\r\nERROR: B should be zero";

    if (!A.IsZero())
        os << "\r\nOKAY: A is not zero";
    else
        os << "\r\nERROR: A should not be zero";

    // test inner product
    int ip = r1A.InnerProduct(c0B);
    os << "\r\n\r\ninner product of R1A and C0B = " << ip << "\r\n";

    // make some bigger matrices
    Matrix<int> M1(5,5), M2(5,5,3), M3(5,5), M4(5,5);

    const int junk[]  = { 1, 5, 3, 0, 1,
                          0, 2, 0, 4, 5,
                          1, 0, 0, 2, 3,
                          7, 1, 3, 0, 0,
                          2, 1, 0, 4, 6 };

    const int ident[] = { 1, 0, 0, 0, 0,
                          0, 1, 0, 0, 0,
                          0, 0, 1, 0, 0,
                          0, 0, 0, 1, 0,
                          0, 0, 0, 0, 1 };

    const int tridi[] = { 1, 1, 0, 0, 0,
                          1, 1, 1, 0, 0,
                          0, 1, 1, 1, 0,
                          0, 0, 1, 1, 1,
                          0, 0, 0, 1, 1 };

    const int utri[]  = { 1, 1, 1, 1, 1,
                          0, 1, 1, 1, 1,
                          0, 0, 1, 1, 1,
                          0, 0, 0, 1, 1,
                          0, 0, 0, 0, 1 };

    const int ltri[]  = { 1, 0, 0, 0, 0,
                          1, 1, 0, 0, 0,
                          1, 1, 1, 0, 0,
                          1, 1, 1, 1, 0,
                          1, 1, 1, 1, 1 };

    const int perm[]  = { 0, 1, 0, 0, 0,
                          1, 0, 0, 0, 0,
                          0, 0, 0, 1, 0,
                          0, 0, 0, 0, 1,
                          0, 0, 1, 0, 0 };

    const int det[]  = { 3, 5, 3, 8, 1,
                         2, 6, 3, 4, 5,
                         1, 4, 5, 2, 3,
                         7, 1, 3, 6, 8,
                         2, 4, 1, 4, 9 };
    M1 = ident;
    M3 = M1 * 2;
    M4 = junk;

    os << "\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    os << "\r\nmatrix M2 = \r\n";
    ShowMatrix(os,M2);

    os << "\r\nmatrix M3 = \r\n";
    ShowMatrix(os,M3);

    os << "\r\nmatrix M4 = \r\n";
    ShowMatrix(os,M4);

    if (M1.IsDiagonal())
        os << "\r\nOKAY: M1 is diagonal";
    else
        os << "\r\nERROR: M1 should be diagonal";

    if (M1.IsIdentity())
        os << "\r\nOKAY: M1 is an identity matrix";
    else
        os << "\r\nERROR: M1 should be an identity matrix";

    if (!M2.IsDiagonal())
        os << "\r\nOKAY: M2 is not diagonal";
    else
        os << "\r\nERROR: M2 should not be diagonal";

    if (!M2.IsIdentity())
        os << "\r\nOKAY: M2 is not an identity matrix";
    else
        os << "\r\nERROR: M2 should not be an identity matrix";

    if (M3.IsDiagonal())
        os << "\r\nOKAY: M3 is diagonal";
    else
        os << "\r\nERROR: M3 should be diagonal";

    if (!M3.IsIdentity())
        os << "\r\nOKAY: M3 is not an identity matrix";
    else
        os << "\r\nERROR: M3 should not be an identity matrix";

    if (!M4.IsDiagonal())
        os << "\r\nOKAY: M4 is not diagonal";
    else
        os << "\r\nERROR: M4 should not be diagonal";

    if (!M4.IsIdentity())
        os << "\r\nOKAY: M4 is not an identity matrix";
    else
        os << "\r\nERROR: M4 should not be an identity matrix";

    // tridiagonal tests
    M1 = tridi;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsTridiagonal())
        os << "\r\nOKAY: M1 is tridiagonal";
    else
        os << "\r\nERROR: M1 should be tridiagonal";

    if (!M4.IsTridiagonal())
        os << "\r\nOKAY: M4 is not tridiagonal";
    else
        os << "\r\nERROR: M1 should not be tridiagonal";

    // upper triangular tests
    M1 = utri;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsUpperTriangular())
        os << "\r\nOKAY: M1 is upper-triangular";
    else
        os << "\r\nERROR: M1 should be upper-triangular";

    if (!M4.IsUpperTriangular())
        os << "\r\nOKAY: M4 is not upper-triangular";
    else
        os << "\r\nERROR: M4 should not be upper-triangular";

    // lower triangular tests
    M1 = ltri;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsLowerTriangular())
        os << "\r\nOKAY: M1 is lower-triangular";
    else
        os << "\r\nERROR: M1 should be lower-triangular";

    if (!M4.IsLowerTriangular())
        os << "\r\nOKAY: M4 is not lower-triangular";
    else
        os << "\r\nERROR: M4 should not be lower-triangular";

    // permutation tests
    M1 = perm;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    M2 = ident;
    os << "\r\n\r\nmatrix M2 = \r\n";
    ShowMatrix(os,M2);

    if (M1.IsPermutation())
        os << "\r\nOKAY: M1 is permutation matrix";
    else
        os << "\r\nERROR: M1 should be permutation";

    if (M2.IsPermutation())
        os << "\r\nOKAY: M2 is permutation matrix";
    else
        os << "\r\nERROR: M2 should be permutation";

    if (!M4.IsPermutation())
        os << "\r\nOKAY: M4 is not permutation";
    else
        os << "\r\nERROR: M4 should not be permutation";

    // check singularity function
    M1(0,1) = 0;
    os << "\r\n\r\nmatrix M1 = \r\n";
    ShowMatrix(os,M1);

    if (M1.IsSingular())
        os << "\r\nOKAY: M1 is singular";
    else
        os << "\r\nERROR: M1 should be singular";

    if (!M2.IsSingular())
        os << "\r\nOKAY: M2 is not singular";
    else
        os << "\r\nERROR: M2 should not be singular";

    if (!M4.IsSingular())
        os << "\r\nOKAY: M4 is not singular";
    else
        os << "\r\nERROR: M4 should not be singular";

    // change main window heading
    os <<endl
       <<"Matrix Tests (manipulations)" <<endl
       <<"============================" <<endl;
    
    // test minors and determinants
    os << "\r\n\r\nmatrix M4 = \r\n";
    ShowMatrix(os,M4);

    os << "\r\nminor M4(1,1) = \r\n";
    ShowMatrix(os,M4.Minor(1,1));

    os << "\r\nminor M4(0,4) = \r\n";
    ShowMatrix(os,M4.Minor(0,4));

    Matrix<int> M5(2,2), M6(3,3);

    M5(0,0) = 1;
    M5(0,1) = 2;
    M5(1,0) = 3;
    M5(1,1) = 4;

    M6(0,0) = 1;
    M6(0,1) = 3;
    M6(0,2) = 2;
    M6(1,0) = 5;
    M6(1,1) = 4;
    M6(1,2) = 7;
    M6(2,0) = 6;
    M6(2,1) = 9;
    M6(2,2) = 8;

    M4 = det;
    Matrix<int> T4(5,5), T5(2,2), T6(3,3);

    T4 = M4.Transpose();
    T5 = M5.Transpose();
    T6 = M6.Transpose();

    os << "\r\nmatrix M5 = \r\n";
    ShowMatrix(os,M5);

    os << "\r\ndeterminant of M5 = "
           << M5.Determinant() << "\r\n";

    os << "\r\nmatrix T5 = \r\n";
    ShowMatrix(os,T5);

    os << "\r\ndeterminant of T5 = "
           << T5.Determinant() << "\r\n";

    os << "\r\nmatrix M6 = \r\n";
    ShowMatrix(os,M6);

    os << "\r\ndeterminant of M6 = "
           << M6.Determinant() << "\r\n";

    os << "\r\nmatrix T6 = \r\n";
    ShowMatrix(os,T6);

    os << "\r\ndeterminant of T6 = "
           << T6.Determinant() << "\r\n";

    os << "\r\nmatrix M4 = \r\n";
    ShowMatrix(os,M4);

    os << "\r\ndeterminant of M4 = "
           << M4.Determinant() << "\r\n";

    os << "\r\nmatrix T4 = \r\n";
    ShowMatrix(os,T4);

    os << "\r\ndeterminant of T4 = "
           << T4.Determinant() << "\r\n";

    Matrix<int> R;
    os << "\r\nMatrix R (def. constr.) = \r\n";
    ShowMatrix(os,R);

    R.Resize(10,10);
    os << "\r\nMatrix R (now 10x10) = \r\n";
    ShowMatrix(os,R);

    // change main window heading
    os <<endl
       <<"Matrix Tests (double)" <<endl
       <<"=====================" <<endl;
    
    // check <double> Matrix
    os << "\r\nFLOATING POINT!";

    Matrix<double> X(3,4), Y(4,3), Z(3,3);

    X(0,0) =  1.0; X(1,0) =  5.0; X(2,0) =  2.0;
    X(0,1) =  2.0; X(1,1) =  2.0; X(2,1) =  4.0;
    X(0,2) =  0.0; X(1,2) =  3.0; X(2,2) =  3.0;
    X(0,3) =  1.0; X(1,3) =  2.0; X(2,3) =  1.0;

    Y(0,0) =  0.0; Y(2,0) =  1.0;
    Y(0,1) =  1.0; Y(2,1) =  0.0;
    Y(0,2) =  2.0; Y(2,2) =  5.0;
    Y(1,0) =  1.0; Y(3,0) =  3.0;
    Y(1,1) =  3.0; Y(3,1) =  1.0;
    Y(1,2) =  2.0; Y(3,2) =  2.0;

    os << "\r\nMatrix X = \r\n";
    ShowMatrix(os,X);
    os << "\r\nMatrix Y = \r\n";
    ShowMatrix(os,Y);

    Z = X % Y;
    os << "\r\nMatrix Z (X % Y) = \r\n";
    ShowMatrix(os,Z);

    // check transposition
    Matrix<double> tX;
    tX = X.Transpose();
    os << "\r\nOriginal X =\r\n";
    ShowMatrix(os,X);
    os << "\r\nTranspose X =\r\n";
    ShowMatrix(os,tX);

    X(0,0) =  1; X(0,1) =  3; X(0,2) = -4; X(0,3) =  8;
    X(1,0) =  1; X(1,1) =  1; X(1,2) = -2; X(1,3) =  2;
    X(2,0) = -1; X(2,1) = -2; X(2,2) =  5; X(2,3) = -1;

    os << "\r\nOriginal X =\r\n";
    ShowMatrix(os,X);

    Matrix<double> lX(X.LinSolve());
    os << "\r\nX after elimination =\r\n";
    ShowMatrix(os,X);

    os << "\r\nlinear equation solution =\r\n";
    ShowMatrix(os,lX);

    X(0,0) =  1.0; X(1,0) =  3.0; X(2,0) =  5.0;
    X(0,1) =  2.0; X(1,1) =  5.0; X(2,1) =  6.0;
    X(0,2) =  0.0; X(1,2) =  4.0; X(2,2) =  3.0;
    X(0,3) =  0.1; X(1,3) = 12.5; X(2,3) = 10.3;

    os << "\r\nOriginal X =\r\n";
    ShowMatrix(os,X);

    lX = X.LinSolve();
    os << "\r\nX after elimination =\r\n";
    ShowMatrix(os,X);

    os << "\r\nlinear equation solution =\r\n";
    ShowMatrix(os,lX);

    Matrix<double> Adbl(3,3), Bdbl(3,1);

    Adbl(0,0) =  1.0; Adbl(0,1) =  2.0; Adbl(0,2) =  0.0;
    Adbl(1,0) =  3.0; Adbl(1,1) =  5.0; Adbl(1,2) =  4.0;
    Adbl(2,0) =  5.0; Adbl(2,1) =  6.0; Adbl(2,2) =  3.0;

    Bdbl(0,0) =  0.1;
    Bdbl(1,0) = 12.5;
    Bdbl(2,0) = 10.3;

    os << "\r\n\r\nmatrix Adbl = \r\n";
    ShowMatrix(os,Adbl);

    os << "\r\nmatrix Bdbl = \r\n";
    ShowMatrix(os,Bdbl);

    Matrix<double> alup(Adbl); // copy Adbl before LUP decomp
    os << "\r\nLU decomp of Adbl (before) = \r\n";
    ShowMatrix(os,alup);

    Matrix<size_t> aperm = alup.LUPDecompose();
    os << "\r\nLU decomp of Adbl (after) = \r\n";
    ShowMatrix(os,alup);

    os << "\r\nPermutation of Adbl = \r\n";
    ShowMatrix(os,aperm);

    Matrix<double> asol = alup.LUPSolve(aperm,Bdbl);
    os << "\r\nlinear solution of Adbl and Bdbl = \r\n";
    ShowMatrix(os,asol);

    Matrix<double> ainv = alup.LUPInvert(aperm);
    os << "\r\ninverse of Adbl and Bdbl = \r\n";
    ShowMatrix(os,ainv);

    Matrix<double> aid = Adbl % ainv;
    os << "\r\ninverse dot Adbl = \r\n";
    ShowMatrix(os,aid);

    Grid<size_t> iperm = ainv.LUPDecompose();
    Matrix<double> invinv = ainv.LUPInvert(iperm);
    os << "\r\ninverse of inverse =\r\n";
    ShowMatrix(os,invinv);
}

int main()
{
  TestMatrix(cout);
  return 0;
}
