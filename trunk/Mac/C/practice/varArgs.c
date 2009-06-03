/*
 *  varArgs.c
 *  practice
 *
 *  Created by wang luke on 6/3/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "varArgs.h"

#include "stdio.h" 
#include "stdarg.h"
// 可变参数函数例子.打出各个参数
void simple_va_fun(int startArg, ...)
{
	// variable-argument(可变参数) list.
	// 存储参数地址的指针
    va_list arg_ptr;
    int nArgValue = startArg;
	
	// 可变参数的数目
    int nArgCout=0;
	// 以固定参数的地址为起点确定变参的内存起始地址
	// 用va_start宏初始化变量arg_ptr,这个宏的第二个参数是可变参数列表的最后一个固定参数
    va_start(arg_ptr,startArg);
    do {
		++nArgCout;
		// 输出各参数的值
        printf("the %d th arg: %d\n",nArgCout,nArgValue);
		// 得到下一个可变参数的值
		// 依次用va_arg宏使arg_ptr返回可变参数的地址,再结合参数的类型,就可以得到参数的值
        nArgValue = va_arg(arg_ptr,int);
    } while(nArgValue != -1);
	printf("==============\n");
    return;
}


int main(int argc, char* argv[])
{
    simple_va_fun(1,2,3,4,-1);
    simple_va_fun(1,2,3,-1);
    return 0;
}
