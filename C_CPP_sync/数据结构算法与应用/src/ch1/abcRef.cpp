// template function using reference parameters to compute an expression
// 引用参数, 常量引用参数
#include<iostream>

using namespace std;

template<class T>
T abc(const T& a, const T& b, const T& c)
{
//	a = 1; 声明为常量引用参数时, 值不能改变
   return a + b + c;
}

int main()
{
	int x = 2, y = 3, z = 4;
   cout << "abc(x,y,z) = " << abc(x,y,z) << endl;
   cout << "x = " << x << ", " << "y = " << y << ", " << "z = " << z << endl;
   return 0;
}

