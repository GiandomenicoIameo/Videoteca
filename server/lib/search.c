#include "search.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void search( char *body ) {

    char name[ 40 ], command[ 100 ];
    strcpy( name, body );

    snprintf( command, sizeof( command ), "script/search.sh \"%s\"", name );

    if ( !WEXITSTATUS( system( command ) ) )
        strcpy( body, "Film trovato" );
    else
        strcpy( body, "Film non trovato" );
}
