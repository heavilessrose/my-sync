#include <stdio.h>

int main (int argc, const char * argv[]) {
	FILE	*fp;
	int	c;
	
	fp = fopen( "../../My Data File", "r" );
	
	if ( NULL == fp ) {
        printf( "Error opening ../My Data File" );
    } else {
		while ( (c = fgetc( fp )) != EOF )
			putchar( c );
		
		fclose( fp );
	}
	
	return 0;
}