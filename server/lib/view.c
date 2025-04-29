#include "view.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void showmovies( char *body ) {

    FILE *fpointer;
	char buffer[ 1024 ], command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/review/compress.sh movies.dat" );
    system( command );

    snprintf( command, sizeof( command ),
              "script/session/review/tohex.sh movies.dat" );

	fpointer = popen( command, "r" );
	if ( fpointer == NULL ) {
			perror( "Errore nell'esecuzione dello script" );
            pthread_exit( ( void * )1 );
	} else {
			while ( fgets( buffer, sizeof( buffer ), fpointer ) != NULL );

			pclose( fpointer );
			strcpy( body, buffer );
	}
}
