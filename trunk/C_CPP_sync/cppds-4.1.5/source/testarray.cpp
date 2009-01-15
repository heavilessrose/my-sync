//  TestArray.cpp
//
//  Tests the array classes.
//

// C Library
#include <math.h>

// C++ Library
#include <iomanip.h>

// Components & Algorithms
#include "intarray.h"
#include "statarray.cpp"

// template function to display array
template <class T>
ostream & operator << (ostream & buffer, StatArray<T> & a)
{
  buffer << "[" <<"0" << "," << a.RunLength() << "] ";

  for (int i = 0; i <= a.RunLength(); ++i)
    buffer << a[i] << " ";

  return buffer;
}

// function prototypes
//void ArrayTest1(ostream & buffer);
//void ArrayTest2(ostream & buffer);
void ArrayTestA(ostream & buffer);
void ArrayTest3(ostream & buffer);
void ArrayTest4(ostream & buffer);
void ArrayTest5(ostream & buffer);
void ArrayTest6(ostream & buffer);
void ArrayTest7(ostream & buffer);
//void ArrayTest8(ostream & buffer);
void ArrayTest9(ostream & buffer);
void ArrayTest10(ostream & buffer);
void ArrayTest11(ostream & buffer);
//void ArrayTest12(ostream & buffer);
void ArrayTest13(ostream & buffer);

// main test routine
void TestArray(ostream & buffer)
{
    ArrayTestA(buffer);
    ArrayTest3(buffer);
    ArrayTest4(buffer);
    ArrayTest5(buffer);
    ArrayTest6(buffer);
    ArrayTest7(buffer);    
    ArrayTest9(buffer);
    ArrayTest10(buffer);
    ArrayTest11(buffer);
    ArrayTest13(buffer);
}

/****************************************************************************/
void ArrayTestA(ostream& buffer)
{
  ifstream fin("stats.in", ios::in);
  Boolean Error_;

  cout <<endl;
  double val_ = StatArray<double>::AvgMean(fin, Error_);
  cout <<"Mean: " <<val_ <<endl;                // == 1325.18
  fin.close();
  fin.open("stats.in", ios::in);
  val_ = StatArray<double>::StdDeviation(fin, Error_);
  cout <<"Std. dev.: " <<val_ <<endl;           // == 2037.63
  fin.close();  
}

/****************************************************************************/
// basic array tests
void ArrayTest3(ostream & buffer)
{
    // banner
    buffer << "\r\nArrays: Integer Fills and Arithmetic"
              "\r\n------------------------------------\r\n";

        Range r(1, 10);
	NumericArray<int> ia(size_t(11), default_size);

	ia.AssignArg(0, ia.RunLength());
	buffer << "                  Filled with zero = " << ia << "\r\n";

	ia.FillArithmetic(2,3);
	buffer << "      Filled arithmetic (2, +3) ia = " << ia << "\r\n";

	NumericArray<int> ib(size_t(11), default_size);
	ib.FillGeometric(1,2);
	buffer << "       Filled geometric (1, *2) ib = " << ib << "\r\n";

	NumericArray<int> ic(size_t(11), default_size);
	ic = ia + ib; 
	buffer << "                 Addition: ia + ib = " << ic << "\r\n";

	ic = ia - ib;
	buffer << "              Subtraction: ia - ib = " << ic << "\r\n";

	ic = ia * ib;
	buffer << "           Multiplication: ia * ib = " << ic << "\r\n";

	ic = ia / ib;
	buffer << "                 Division: ia / ib = " << ic << "\r\n";

	ic = ia;
	ic += ib;
	buffer << "      Shorthand Addition: ia += ib = " << ic << "\r\n";

	ic = ia;
	ic -= ib;
	buffer << "   Shorthand Subtraction: ia -= ib = " << ic << "\r\n";

	ic = ia;
	ic *= ib;
	buffer << "Shorthand Multiplication: ia *= ib = " << ic << "\r\n";

	ic = ia;
	ic /= ib;
	buffer << "      Shorthand Division: ia /= ib = " << ic << "\r\n";
}

void ArrayTest4(ostream & buffer)
{
    buffer << "\r\nArrays: float Fills and Arithmetic"
              "\r\n----------------------------------\r\n";

	Range r(1,10);
	NumericArray<float> fa(size_t(11), default_size), fb(size_t(11), default_size);

	fa.AssignArg(0.0F, fa.RunLength());
	buffer << "                  Filled with zero = " << fa << "\r\n";

	fa.FillArithmetic(2.0F,1.3F);
	buffer << "   Filled arithmetic (2.0, 1.3) fa = " << fa << "\r\n";

	fb.FillGeometric(1.0F,1.1F);
	buffer << "     Filled geometric (1, *1.1) fb = " << fb << "\r\n";

	NumericArray<float> fc(size_t(11), default_size);
	fc = fa + fb; 
	buffer << "                 Addition: fa + fb = " << fc << "\r\n";

	fc = fa - fb;
	buffer << "              Subtraction: fa - fb = " << fc << "\r\n";

	fc = fa * fb;
	buffer << "           Multiplication: fa * fb = " << fc << "\r\n";

	fc = fa / fb;
	buffer << "                 Division: fa / fb = " << fc << "\r\n";

	fc  = fa;
	fc += fb;
	buffer << "      Shorthand Addition: fa += fb = " << fc << "\r\n";

	fc = fa;
	fc -= fb;
	buffer << "   Shorthand Subtraction: fa -= fb = " << fc << "\r\n";

	fc = fa;
	fc *= fb;
	buffer << "Shorthand Multiplication: fa *= fb = " << fc << "\r\n";

	fc = fa;
	fc /= fb;
	buffer << "      Shorthand Division: fa /= fb = " << fc << "\r\n";

	fa.FillGeometric(1701.0F,-.25F);
	buffer << "    Geometric test: (1701.0, -.25) = " << fa << "\r\n";

	fc = Abs(fa);
	buffer << "           absolute value (friend) = " << fc << "\r\n";

	fa.Abs();
	buffer << "           absolute value (member) = " << fa << "\r\n";

	fa = -fa;
	buffer << "                    unary negation = " << fa << "\r\n";

	NumericArray<float> fd(fa); fd += fb;
	buffer << "           concatenation (fa & fb) = " << fd << "\r\n";

	static const float x[] = {1.0F,1.0F,2.0F,3.0F,5.0F,8.0F,13.0F,21.0F,34.0F,55.0F,89.0F};
	NumericArray<float> fe(x, 11);
	buffer << "           construction from array = " << fe << "\r\n";

	fc = fa;
	NumericArray<Boolean> b1 = (fc == fa);

	fc[ 3]  = 0.0F;
	NumericArray<Boolean> b2 = fc == fa;

	fc[ 3]  = fa[3];
	fc[ 1]  = 0.0F;
	NumericArray<Boolean> b3 = fc == fa;

	fc[ 1]  = fa[1];
	fc[10]  = 0.0F;
	NumericArray<Boolean> b4 = fc == fa;

	buffer << "   equality tests (should be TFFF) = "
           << b1 << " "
           << b2 << " "
           << b3 << " "
           << b4 << "\r\n";
}

void ArrayTest5(ostream & buffer)
{
    buffer << "\r\nArrays: Integer Comparison Tests"
              "\r\n--------------------------------\r\n";

    static const int MaxVal = 4;

    Range r(-5,5);
    NumericArray<int> i1(size_t(11), default_size), i2(size_t(11), default_size);
    NumericArray<Boolean> res(size_t(11), default_size);

    // fill arrays with random numbers
    for (int i = 0; i <= 10; ++i)
    {
	i1[i] = RANDOM(MaxVal);
	i2[i] = RANDOM(MaxVal);
    }

    buffer << "Comp array i1 = " << i1 << "\r\n";
    buffer << "Comp array i2 = " << i2 << "\r\n";

    res = i1 < i2;
    buffer << "      i1 < i2 = " << res << "\r\n";

    res = i1 <=i2;
    buffer << "     i1 <= i2 = " << res << "\r\n";

    res = i1 == i2;
    buffer << "     i1 == i2 = " << res << "\r\n";

    res = i1 != i2;
    buffer << "     i1 != i2 = " << res << "\r\n";

    res = i1 >= i2;
    buffer << "     i1 >= i2 = " << res << "\r\n";

    res = i1 > i2;
    buffer << "      i1 > i2 = " << res << "\r\n";
}

void ArrayTest6(ostream & buffer)
{
    buffer << "\r\nArrays: Long logical operations"
              "\r\n--------------------------------\r\n";

    Range r(-5,5);
    IntegerArray<long>
    la(size_t(11), default_size),
    lb(size_t(11), default_size),
    lc(size_t(11), default_size);

    for (int i = 0; i <= 10; ++i)
    {
	la[i] = RANDOM(16);
	lb[i] = RANDOM(5) + 1;
    }

    buffer << "                   long array la = " << la << "\r\n";
    buffer << "                   long array lb = " << lb << "\r\n";

    lc = la % lb;
    buffer << "       binary modulus:   la % lb = " << lc << "\r\n";

    lc = la << lb;
    buffer << "    binary shift left:  la << lb = " << lc << "\r\n";

    lc = la >> lb;
    buffer << "   binary shift right:  la >> lb = " << lc << "\r\n";

    lc = la & lb;
    buffer << "           binary AND:   la & lb = " << lc << "\r\n";

    lc = la | lb;
    buffer << "            binary OR:   la | lb = " << lc << "\r\n";

    lc = la ^ lb;
    buffer << "           binary XOR:   la ^ lb = " << lc << "\r\n";

    lc  = la;
    lc %= lb;
    buffer << "    shorthand modulus:  la %= lb = " << lc << "\r\n";

    lc  = la;
    lc <<= lb;
    buffer << "shorthand shift  left: la <<= lb = " << lc << "\r\n";

    lc  = la;
    lc >>= lb;
    buffer << "shorthand shift right: la >>= lb = " << lc << "\r\n";

    lc  = la;
    lc &= lb;
    buffer << "        shorthand AND:  la &= lb = " << lc << "\r\n";

    lc  = la;
    lc |= lb;
    buffer << "        shorthand  OR:  la |= lb = " << lc << "\r\n";

    lc  = la;
    lc ^= lb;
    buffer << "        shorthand XOR:  la ^= lb = " << lc << "\r\n";

    lc = !la;
    buffer << "                  unary NOT: !la = " << lc << "\r\n";

    lc = ~la;
    buffer << "           unary complement: ~la = " << lc << "\r\n";
}

void ArrayTest7(ostream & buffer)
{
  buffer << "\r\nArrays: Function Application & Conversion Tests"
            "\r\n-----------------------------------------------\r\n";

  Range r(0,7);
  NumericArray<double> da(size_t(8), default_size);

  for (int i = 0; i < 8; ++ i)
    da[i] = (double)i * 3.1415927;

  buffer << "   double array da = " << da << "\r\n";

  da.ApplyUnary(sqrt);
  buffer << "square roots of da = " << da << "\r\n";

  // conversion tests
  DynArray<double> ad(da);
  buffer << "   create DynArray<double> from NumericArray<double> = " << ad << "\r\n";

  buffer << "     assign DynArray<double> to NumericArray<double> = " << da << "\r\n";
  ((DynArray<double>&)da) = ad;

  NumericArray<float> fa(size_t(8), default_size);
  fa.AssignArg(1.0F, fa.RunLength());
        
  #ifdef std_cplusplus
    da = Convert<float,double>(fa);
    buffer << "assign NumericArray<float> to NumericArray<double> = " << da << "\r\n";
  #endif
}

void ArrayTest9(ostream& buffer)
{
    buffer << "\r\nArrays: Statistics Tests #1"
              "\r\n---------------------------\r\n";

    StatArray<double> dsa(15, default_size);
    size_t i;
//    Index i(r);

    for (i = 0; i < 15; ++i)
      dsa[i] = (double)RANDOM(100);

    buffer << "stat array = " << dsa << "\r\n";

    double minimum = dsa.Min();
    double maximum = dsa.Max();
    double sum     = dsa.Sum();
    double prod    = dsa.Product();
    double mean    = dsa.Mean();
    double avgdev  = dsa.AvgDeviation();
    double stddev  = dsa.StdDeviation();
    double var     = dsa.Variance();
    double skew    = dsa.Skew();
    double kurt    = dsa.Kurt();

    buffer << "  sum            = " << sum     << "\r\n";
    buffer << "  prod           = " << prod    << "\r\n\r\n";

    buffer << "\r\nSingle function stats:\r\n";
    buffer << "  minimum        = " << minimum << "\r\n";
    buffer << "  maximum        = " << maximum << "\r\n";
    buffer << "  mean           = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
    buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    dsa.MinMax(minimum,maximum);
    dsa.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "\r\nMulti-function stats:\r\n";
    buffer << "minimum        = " << minimum << "\r\n";
    buffer << "maximum        = " << maximum << "\r\n";
    buffer << "mean           = " << mean    << "\r\n";
    buffer << "avg. deviation = " << avgdev  << "\r\n";
    buffer << "std. deviation = " << stddev  << "\r\n";
    buffer << "variance       = " << var     << "\r\n";
    buffer << "skewness       = " << skew    << "\r\n";
    buffer << "kurtosis       = " << kurt    << "\r\n\r\n";
}

void ArrayTest10(ostream& buffer)
{
    buffer << "\r\nArrays: Statistics Tests #2"
              "\r\n---------------------------\r\n";

    StatArray<double> ds(21, default_size);

    ds[ 1] = 1.0;
    ds[ 2] = 2.0;
    
    ds[ 3] = 2.0;
//    ds[ 3] = 3.0;
    
    ds[ 4] = 3.0;
    ds[ 5] = 2.0;
    ds[ 6] = 1.0;
    ds[ 7] = 3.0;
    ds[ 8] = 4.0;
    ds[ 9] = 5.0;
    ds[10] = 7.0;
    ds[11] = 6.0;
    ds[12] = 5.0;
    ds[13] = 6.0;
    ds[14] = 4.0;
    
    ds[15] = 1.0;
//    ds[15] = 3.0;     // uncomment to test mode average
    
    ds[16] = 2.0;
    ds[17] = 3.0;
    ds[18] = 3.0;
    ds[19] = 2.0;
    ds[20] = 4.0;

    buffer << "array before: " << ds << "\r\n";

    double** Mode_ = ds.Mode();
    size_t x;

    buffer << "mode            = ";
    for (x = 0; Mode_[x]; x++)
    {
      buffer <<(*Mode_[x]);
      delete Mode_[x];
      
      if (Mode_[x+1])
        buffer <<", ";
    }

    delete[] Mode_;
    buffer << "\r\n";
    buffer << "median          = " << ds.Median() << "\r\n";
    buffer << "median (sorted) = " << ds.MedianSort() << "\r\n";
    buffer << "array  after: " << ds << "\r\n";
}

void ArrayTest11(ostream& buffer)
{
    buffer << "\r\nArrays: Correlation Tests"
              "\r\n-------------------------\r\n";

//    Range r(11);
    StatArray<double> dsa1(11, default_size), dsa2(11, default_size);
    const double rad2deg = 1.0 / 57.29577951;
    double angle1 = 45.0 * rad2deg;
    double angle2 = 45.0 * rad2deg;

//    for (int i = r.GetMin(); i < r.GetMax(); ++i)
    for (int i = 1; i < 11; ++i)
        {
        dsa1[i] = angle1;
		dsa2[i] = angle2;
		angle1 += rad2deg;
		angle2 -= rad2deg;
		}

	double cor = dsa1.Correlation(dsa2);
    buffer << "       dsa1 = " << dsa1 << "\r\n";
    buffer << "       dsa2 = " << dsa2 << "\r\n";
	buffer << "correlation = " << cor  << "\r\n\r\n";

	StatArray<double> dsa3(dsa1);
	cor = dsa1.Correlation(dsa3);
    buffer << "       dsa1 = " << dsa1 << "\r\n";
    buffer << "       dsa3 = " << dsa3 << "\r\n";
	buffer << "correlation = " << cor  << "\r\n\r\n";

	dsa1.FillRandom(  0.0, 1.0);
	dsa2.FillRandom(-10.0,10.0);
	cor = dsa1.Correlation(dsa2);

    buffer << "       dsa1 = " << dsa1 << "\r\n";
    buffer << "       dsa2 = " << dsa2 << "\r\n";
	buffer << "correlation = " << cor  << "\r\n\r\n";

	dsa1.AssignArg(1.0, dsa1.RunLength());
	dsa2.AssignArg(1.0, dsa2.RunLength());

	cor = dsa1.Correlation(dsa2);
	buffer << "correlation        (all ones) = " << cor << "\r\n";

	cor = dsa1.Correlation(dsa3);
	buffer << "correlation (ones and values) = " << cor << "\r\n";
}

void ArrayTest13(ostream & buffer)
{
    buffer << "\r\nStatistics: Coyotes, Jack Rabbits, and Temperature"
              "\r\n--------------------------------------------------\r\n\r\n";

    int  oprec = buffer.precision(4);
    long oflag = buffer.flags();
    buffer.setf(ios::fixed|ios::showpoint|ios::showpos);

    Range r(1971,1990);
    Index ind(r);
    StatArray<double>
    coyote(r.GetMagnitude(), default_size),
    jackrab(r.GetMagnitude(), default_size),
    rainfall(r.GetMagnitude(), default_size);

    coyote[(ind=1971)] =  97.0; jackrab[(ind=1971)] = 200.0; rainfall[(ind=1971)] =  3.0;
    coyote[(ind=1972)] =  86.0; jackrab[(ind=1972)] = 220.0; rainfall[(ind=1972)] = 11.0;
    coyote[(ind=1973)] =  87.0; jackrab[(ind=1973)] = 225.0; rainfall[(ind=1973)] =  4.0;
    coyote[(ind=1974)] =  73.0; jackrab[(ind=1974)] = 233.0; rainfall[(ind=1974)] =  2.0;
    coyote[(ind=1975)] =  70.0; jackrab[(ind=1975)] = 261.0; rainfall[(ind=1975)] =  1.0;
    coyote[(ind=1976)] =  81.0; jackrab[(ind=1976)] = 235.0; rainfall[(ind=1976)] =  4.0;
    coyote[(ind=1977)] =  67.0; jackrab[(ind=1977)] = 257.0; rainfall[(ind=1977)] =  5.0;
    coyote[(ind=1978)] =  75.0; jackrab[(ind=1978)] = 250.0; rainfall[(ind=1978)] =  3.0;
    coyote[(ind=1979)] =  78.0; jackrab[(ind=1979)] = 223.0; rainfall[(ind=1979)] =  3.0;
    coyote[(ind=1980)] =  93.0; jackrab[(ind=1980)] = 187.0; rainfall[(ind=1980)] =  4.0;
    coyote[(ind=1981)] = 101.0; jackrab[(ind=1981)] = 171.0; rainfall[(ind=1981)] =  6.0;
    coyote[(ind=1982)] =  93.0; jackrab[(ind=1982)] = 167.0; rainfall[(ind=1982)] =  2.0;
    coyote[(ind=1983)] =  96.0; jackrab[(ind=1983)] = 154.0; rainfall[(ind=1983)] =  3.0;
    coyote[(ind=1984)] =  82.0; jackrab[(ind=1984)] = 212.0; rainfall[(ind=1984)] =  2.0;
    coyote[(ind=1985)] =  70.0; jackrab[(ind=1985)] = 210.0; rainfall[(ind=1985)] =  4.0;
    coyote[(ind=1986)] =  76.0; jackrab[(ind=1986)] = 219.0; rainfall[(ind=1986)] =  7.0;
    coyote[(ind=1987)] =  50.0; jackrab[(ind=1987)] = 308.0; rainfall[(ind=1987)] = 10.0;
    coyote[(ind=1988)] =  69.0; jackrab[(ind=1988)] = 287.0; rainfall[(ind=1988)] =  1.0;
    coyote[(ind=1989)] =  80.0; jackrab[(ind=1989)] = 244.0; rainfall[(ind=1989)] =  2.0;
    coyote[(ind=1990)] =  81.0; jackrab[(ind=1990)] = 218.0; rainfall[(ind=1990)] =  1.0;

    // general statistics
    double mean, stddev, avgdev, var, skew, kurt;

    coyote.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "coyote stats:\r\n";
    buffer << "  mean (average) = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
	 buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    jackrab.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "jack rabbit stats:\r\n";
    buffer << "  mean (average) = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
	buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    rainfall.Moment(mean,avgdev,stddev,var,skew,kurt);

    buffer << "rainfall stats:\r\n";
    buffer << "  mean (average) = " << mean    << "\r\n";
    buffer << "  avg. deviation = " << avgdev  << "\r\n";
    buffer << "  std. deviation = " << stddev  << "\r\n";
    buffer << "  variance       = " << var     << "\r\n";
    buffer << "  skewness       = " << skew    << "\r\n";
	 buffer << "  kurtosis       = " << kurt    << "\r\n\r\n";

    // correlate information
    double cor;

    cor = coyote.Correlation(jackrab);
    buffer << "correlation of  coyotes and  rabbits: " << cor << "\r\n";

    cor = jackrab.Correlation(coyote);
    buffer << "correlation of  rabbits and  coyotes: " << cor << "\r\n";

    cor = coyote.Correlation(rainfall);
    buffer << "correlation of  coyotes and rainfall: " << cor << "\r\n";

    cor = jackrab.Correlation(rainfall);
    buffer << "correlation of  rabbits and rainfall: " << cor << "\r\n\r\n";

    // looking for weird numbers
    buffer << "\r\nData dump with ZScores:\r\n"
           << "Year  Coyotes       Rabbits       Rainfall\r\n";

    int i;
    for (i = r.GetMin(); i < r.GetMax(); ++i)
        {
        ind = i;
        buffer << setw(4) << i << ": "
               << setw(4) <<   coyote[ind] << " (" << setw(6) <<   coyote.ZScore(ind) << ") "
               << setw(4) <<  jackrab[ind] << " (" << setw(6) <<  jackrab.ZScore(ind) << ") "
               << setw(4) << rainfall[ind] << " (" << setw(6) << rainfall.ZScore(ind) << ")\r\n";
        }

    buffer.flags((std::_Ios_Fmtflags)oflag);
    buffer.precision(oprec);
}

/****************************************************************************/
int main()
{
  RANDOMIZE();

  TestArray(cout);
  return 0;
}
/****************************************************************************/





