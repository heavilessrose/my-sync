#include <stdio.h>

long	factorial( long num );

int main (int argc, const char * argv[]) {
	long	num = 5L;
	
	printf( "%ld factorial is %ld.", num,
           factorial( num ) );
	
	return 0;
}


long	factorial( long num ) {
	if ( num > 1 )
		num *= factorial( num - 1 );
	
	return( num );
}