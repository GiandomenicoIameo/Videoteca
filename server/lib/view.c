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

// void search( char *body ) {
//
//     extern unsigned int rdm;
//
//     extern semaphore mtm;
//     extern semaphore wrtm;
//
//     unsigned int res;
//     char name[ 40 ], command[ 100 ];
//
//     strcpy( name, body );
//     snprintf( command, sizeof( command ), "script/search/search.sh \"%s\"", name );
//     // Processo lettore che accede al file movies.dat.
//     pthread_mutex_lock( &mtm );
//     if ( ++rdm == 1 ) {
//             pthread_mutex_lock( &wrtm );
//     } pthread_mutex_unlock( &mtm );
//
//     res = WEXITSTATUS( system( command ) );/* Sesione critica */
//
//     pthread_mutex_lock( &mtm );
//     if ( --rdm == 0 ) {
//             pthread_mutex_unlock( &wrtm );
//     } pthread_mutex_unlock( &mtm );
//
//     if ( res ) {
//             strcpy( body, "Film trovato" );
//     } else {
//         strcpy( body, "Film non trovato" );
//     }
// }
