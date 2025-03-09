#include "metadata.h"

#include <stdio.h>
#include <stdlib.h>

int look( int sdb ) {

    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/look.sh %d",
              sdb );
    return WEXITSTATUS( system( command ) );
}

int find( char *username, char *password ) {

    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/find.sh \"%s\" \"%s\"",
              username, password );
    return WEXITSTATUS( system( command ) );
}

void extract( char *body, char *username, char *password ) {

    // La procedura extract utilizza il passaggio per riferimento
    // per "restituire" al chiamante più valori modificando variabili
    // nella funzione chiamante. In questo caso vengono modificati
    // gli array nella funzione chiamante relativi all'username e
    // alla password, memorizzandovi delle stringhe.

    for( ; *body != ' '; body++, username++ )
        *username = *body;
    *username = '\0';

    body++;
    for( ; *body != '\0'; body++, password++ )
        *password = *body;
    *password = '\0';
}
