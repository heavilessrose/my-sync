// template function using reference parameters to compute an expression
// ���ò���, �������ò���
#include<iostream>

using namespace std;

template<class T>
T abc(const T& a, const T& b, const T& c)
{
//	a = 1; ����Ϊ�������ò���ʱ, ֵ���ܸı�
   return a + b + c;
}

int main()
{
	int x = 2, y = 3, z = 4;
   cout << "abc(x,y,z) = " << abc(x,y,z) << endl;
   cout << "x = " << x << ", " << "y = " << y << ", " << "z = " << z << endl;
   return 0;
}

