#include "search.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void search( char *body ) {

    extern unsigned int rdm;
    extern pthread_mutex_t mtm;
    extern pthread_mutex_t wrtm;

    unsigned int res;
    char name[ 40 ], command[ 100 ];

    strcpy( name, body );
    snprintf( command, sizeof( command ), "script/search/search.sh \"%s\"", name );
    // Processo lettore che accede al file movies.dat.
    pthread_mutex_lock( &mtm );
    rdm = rdm + 1;
    if ( rdm == 1 )
         pthread_mutex_lock( &wrtm );
    pthread_mutex_unlock( &mtm );

    /* Sesione critica */
    res = WEXITSTATUS( system( command ) );

    pthread_mutex_lock( &mtm );
    rdm = rdm - 1;
    if ( rdm == 0 )
         pthread_mutex_unlock( &wrtm );
    pthread_mutex_unlock( &mtm );

    if ( !res )
        strcpy( body, "Film trovato" );
    else
        strcpy( body, "Film non trovato" );
}
