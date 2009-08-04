#include <stdio.h>

int main (int argc, const char * argv[]) {
	int		myInt;
	
	myInt = 5;
	printf( "myInt ---> %d\n", myInt++ );
	printf( "myInt ---> %d", ++myInt );
	
	return 0;
}