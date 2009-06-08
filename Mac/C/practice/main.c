#include <stdio.h>
#include <stdlib.h>

#define AAA 5;

void sortArray(int count, int array[]){
	int tmp;
	int i;
	for(i = 0; i < count; i++){
		for (int j = i + 1; j < count; j++) {
			if(array[i] > array[j]){
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}
	
	showArray(count, array);
	printf("\n");
}

//int main (int argc, const char * argv[]) {
//	int a = AAA + AAA;
//	int arr[5];
//	for(int i = 0; i < 5; i++){
//		arr[i] = random() % 100 + 1;
//	}
//	sortArray(5, arr);
//	
//	showArray(5, arr);
//    return 0;
//}


#include "Functions.h"
// 条件编译
#define SORT

#define VARIABLE_TEST

int main(int argc, const char *argv[])
{
	
#ifdef VARIABLE_TEST
	testVariables();
#endif
	
	///// 参数传递测试
//	int array[5];
//	int i = 0;
//	for (i = 0; i < 5; i++) {
//		array[i] = 0;
//	}
//	
//	changeArray(5, array);
//	showArray(5, array);
	
	////// 结构体测试
//	Song firstSong  = make_song (210, 2004);
//	Song secondSong = make_song (256, 1992);
	
	// 结构体的另一种初始化方法
	Song firstSong = {210, 2004};
	display_song(firstSong);
	
//	Song thirdSong  = { 223, 1997 };
//	display_song ( thirdSong );
//	//////
//	
	printf("\n");
	int x = 1, y = 2;
	int *p1;
	p1= &x;
	int *p2 = &y;
	swap(&x, &y);
	printf("x = %d, y = %d \n", x, y);
	swap(p1, p2);
	printf("x = %d, y = %d \n", x, y);
	printf("\n");
	
	//////数组指针
	//arrayPointerTest();
	
	
	//////字符串
	char * testStrs[] = {"CCCCCC","BBB","AAAAAAAAAAAAAAAA"};
//	
//#ifdef SORT
//	strSort(testStrs, 3);
//#endif
//	printPointerArray(testStrs, 3);
	
	
	char **p;
	p = testStrs + 2;
	printf("%o\n", *p);
	printf("%s\n", *p);
	
	return 0;
}