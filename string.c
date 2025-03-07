#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main( void ) {

    char remainderPtr[ 20 ], vett[] = "89ciao come stai?";
    unsigned int code[ 2 ];
    char *buffer = vett;

    int a;

    for( int i = 0; isdigit( *buffer ); buffer++, i++ )
        code[ i ] = ( int )*buffer - 48;

    strcpy( remainderPtr, buffer );
    puts( remainderPtr );

    return 0;
}
