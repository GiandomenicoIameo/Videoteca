#include "search.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void search( char *body ) {

    extern unsigned int rcounter;
    extern pthread_mutex_t mutex;
    extern pthread_mutex_t semwrite;

    unsigned int res;
    char name[ 40 ], command[ 100 ];

    strcpy( name, body );
    snprintf( command, sizeof( command ), "script/search.sh \"%s\"", name );

    // Processo lettore che accede al file movies.dat
    pthread_mutex_lock( &mutex );
    rcounter++;
    if ( rcounter == 1 )
         pthread_mutex_lock( &semwrite );
    pthread_mutex_unlock( &mutex );

    /* Sesione critica */
    res = WEXITSTATUS( system( command ) );

    pthread_mutex_lock( &mutex );
    rcounter--;
    if ( rcounter == 0 )
         pthread_mutex_unlock( &semwrite );
    pthread_mutex_unlock( &mutex );

    if ( !res )
        strcpy( body, "Film trovato" );
    else
        strcpy( body, "Film non trovato" );
}
