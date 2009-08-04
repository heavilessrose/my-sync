#include <stdio.h>

#define kArraySize  10

int main (int argc, const char * argv[]) {
    int     i;
    char    s[ kArraySize ] = "Hello";
    
    printf( "i before it is initialized: %d\n\n", i );
    
    for ( i=0; i<kArraySize; i++ )
        printf( "s[ i ]: %d\n", s[ i ] );
    
}
