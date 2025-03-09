#include "authentication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int authentication( int sdb, int action, char *body ) {

    // I prototipi per le funzioni signin() e signup() sono inclusi
    // nel corpo della funzione authentication perché è l'unica
    // funzione che invoca cancel() e signout().

    int result = 0;

    switch( action ) {
        case SIGNIN:
            signin( sdb, body );
            break;
        case SIGNUP:
            signup( sdb, body );
            break;
        default:
            return -1;
            break;
    }

    return result;
}

void signin( int sdb, char *body ) {

    char username[ 20 ], password[ 20 ], command[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    // L'utente potrebbe accedere per conto di un altro, ma il seguente controllo
    // lo impedisce.
    if( look( sdb ) ) {
        // Il controllo inizia con la funzione look() che verifica se l'utente
        // è attualmente connesso. La verifica viene effettuata in base all'identificativo
        // della connessione.
        if ( !find( username, password ) ) {
        // Se l'utente non era connesso viene effettuato un controllo per verificare
        // le credenziali passate come input alla funzione. Se l'utente è registrato
        // presso il server l'accesso gli sarà consentito.
            snprintf( command, sizeof( command ), "echo %d \"%s\" \"%s\" >> database/connessi.dat",
              sdb, username, password );

            system( command );
            strcpy( body, "Accesso consentito!" );
        }
        else
            strcpy( body, "Accesso negato!" );
    }
    else
        strcpy( body, "Sei già connesso!" );
}

void signup( int sdb, char *body ) {

    char username[ 20 ], password[ 20 ], command1[ 100 ], command2[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    snprintf( command1, sizeof( command1 ), "script/look.sh %d", sdb );
    // Un utente non può creare un nuovo account se è già connesso
    if ( !WEXITSTATUS( system( command1 ) ) ) {
        strcpy( body, "Devi prima disconnetterti" );
        return;
    }

    if ( !find( username, password ) )
        strcpy( body, "Account esistente!" );
    else {
        snprintf( command2, sizeof( command2 ), "script/signup.sh %d \"%s\" \"%s\"",
              sdb, username, password );
        system( command2 );
        strcpy( body, "Account creato!" );
    }
}
