#include "release.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int release( int sdb, int action, char *body ) {

    // I prototipi per le funzioni cancel() e signout() sono inclusi
    // nel corpo della funzione release perché è l'unica funzione che
    // invoca cancel() e signout()

    int result = 2;

    switch( action ) {
        case LOGOUT:
            signout( sdb, body );
            break;
        case CANCEL:
            cancel( sdb, body );
            break;
        default:
            result = -1;
            break;
    }

    return result;
}

void signout( int sdb, char *body ) {

    // L'utente non può disconnetterne un altro perché
    // la procedura si basa solo sull'dentificativo
    // associato alla connessione che ha la funzione di collegare
    // ogni connessione a un account.

    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/logout.sh %d",
              sdb );

    if ( !WEXITSTATUS( system( command ) ) )
        strcpy( body, "Account disconnesso!" );
    else
        strcpy( body, "Account non connesso!" );
}

void cancel( int sdb, char *body ) {

    // La funzione cancel poggia sull'assunto che l'utente che desidera
    // cancellare il suo account abbia effettuato in un precedente momento
    // l'accesso a quest'ultimo.

    char username[ 20 ], password[ 20 ],
            command1[ 100 ], command2[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    snprintf( command1, sizeof( command1 ), "script/verify.sh %d \"%s\" \"%s\" ",
              sdb, username, password );

    if( WEXITSTATUS( system( command1 ) ) == 0 ) {
        // L'if verifica se l'utente era connesso prima di effettuare la
        // cancellazione dell'account.
        snprintf( command2, sizeof( command2 ), "script/cancel.sh %d \"%s\" \"%s\"",
                  sdb, username, password );
        system( command2 );

        strcpy( body, "Account cancellato!" );
    }
    else
        strcpy( body,"Cancellazione Account fallita!" );
        // Qui è necessario inserire un ciclo
}
