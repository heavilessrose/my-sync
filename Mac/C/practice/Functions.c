/*
 *  Functions.c
 *  practice
 *
 *  Created by wang luke on 4/24/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "Functions.h"

void testVariables()
{
	printf("=================\n");
	printf(" short: %d\n int: %d\n char: %d\n long: %d\n unsigned int: %d\n unsigned char: %d\n",sizeof (short), sizeof (int), sizeof (char), sizeof(long), sizeof(unsigned int), sizeof(unsigned char));
	printf("=================\n");
}

// 测试参数传递， （普通变量为值传递， 数组为引用传递）
void changeArray(int arrCount, int arr[])
{
	for (int i = 0; i < 5; i++) {
		arr[i] = 1;
	}
}

void showArray(int arrCount, int arr[])
{
	
	for (int i = 0; i < arrCount; i++) {
		printf("%d, ", arr[i]);
	}
}

// 测试结构体
Song make_song(int seconds, int year)
{
	Song newSong;
	
	newSong.lengthInSeconds = seconds;
	newSong.yearRecorded = year;
	display_song(newSong);
	
	return newSong;
}

void display_song(Song theSong)
{
	printf ("the song is %i seconds long ", theSong.lengthInSeconds);
	printf ("and was made in %i\n", theSong.yearRecorded);
}


////////////
// 指针作为参数（传引用）， 也跟值传递一样会拷贝， 但是与值传递不同的是引用传递只拷贝指针而值传递会拷贝所有内容
//（如果参数是一个很大的数组那么显然引用传递损耗更小）。
void swap(int *a, int *b)
{	
// 此种方法是改变a, b这两个指针拷贝所指向的值，即实参的值。达成目标功能。
//	int tmp = *a;
//	*a = *b;
//	*b = tmp;
//////////////////////////////////////////////////	
	// a, b实际上是实参（两个指针）的拷贝， 方法里改变了a，b两个形参的指向，不会影响实参的指向。
	// a, b, tmp都是局部变量， 方法返回后即被释放。无法完成期望的功能。 
	int *tmp;
	tmp = a;
	a = b;
	b = tmp;
}

////////////数组的指针
int arrayPointerTest()
{
//	int a[] = {1,2,3,4,5};
//	int *p = a;
//	//int *p;
//	//p = a;
//	for (int i = 0; i < 5; i++) {
//		printf("%d, ", *p++);
//	}
	
	//////////二维数组指针
	int a[3][2] = {{1,2},{3,4},{5,6}};
	//int (*p)[2] = a; // p为一个指针变量，指向包含两个元素的一维数组。p是行指针。
	
	//int (*p)[3][2] = &a;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			printf("%d, ", *(*(/*p*/a+i)+j)); // *(p+i) 为第i行地址
		}
		printf("\n");
	}
	
	/////////////字符串指针
	char *str = "Song Xiao Han Wang Ying Hua";
	
	char *str1 = "bb";
	char *str2 = "aaaaa";
	
	printf("%d", strcmp(str1,str2));
	return 0;
}

// 字符串排序
void strSort(const char *str[], int count)
{
	int k;
	char * tmp;
	for (int i = 0; i < count - 1; i++) {
		k = i;
		for (int j = i + 1; j < count; j++) 
			if(strcmp(str[k], str[j]) > 0) 
				k = j;
		if(k != i){
			tmp = str[i];
			str[i] = str[k];
			str[k] = tmp;
		}
	}
}

// 打印字符串
void printPointerArray(const char *str[], int count)
{
	for (int i = 0; i < count; i++) {
		printf("%s \n", str[i]);
	}
}


//////////// 文件



