#include <stdio.h>
#include <stdbool.h>  //This is to bring in the define of true and false
#include <math.h> //This is to bring in the declaration of sqrt()

int main (int argc, const char * argv[]) {
	int		candidate, isPrime, i, last;
	
	printf( "Primes from 1 to 100: 2, " );
	
	for ( candidate=3; candidate<=100; candidate+=2 ) {
		isPrime = true;
		last = sqrt( candidate );
		
		for ( i = 3; (i <= last) && isPrime; i += 2 ) {
			if ( (candidate % i) == 0 )
				isPrime = false;
		}
		
		if ( isPrime )
			printf( "%d, ", candidate );
	}
	
	return 0;
}