#include <stdio.h>

// 判断的依据：数据寻址时，用的是低位字节的地址
int main (int argc, const char * argv[]) {
	char x0,x1;
	short x=0x1234;
	x0=((char*)&x)[0]; //低地址单元
	x1=((char*)&x)[1]; //高地址单元
	
	if(x0 == 0x12) printf("Big Endian\r\n");
	else printf("Little Endian\r\n");

    return 0;
}
