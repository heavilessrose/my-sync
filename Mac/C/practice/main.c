#include <stdio.h>
#include <stdlib.h>

#define AAA 5;

void showArray(int count, int array[]){
	for (int i = 0; i < count; i++) {
		printf(", %d", array[i]);
	}
}

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

int main (int argc, const char * argv[]) {
	int a = AAA;
	int arr[5];
	for(int i = 0; i < 5; i++){
		arr[i] = random() % 100 + 1;
	}
	sortArray(5, arr);
	
	showArray(5, arr);
    return 0;
}

