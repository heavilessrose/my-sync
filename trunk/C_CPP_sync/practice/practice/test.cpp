#include <iostream>
#include <string>

using namespace std;

int main(){
	int a = 5, *p=&a;
	printf("a = %d, *p = %d, p = %d, &a = %d", a,*p,p,&a);
	getchar();
}