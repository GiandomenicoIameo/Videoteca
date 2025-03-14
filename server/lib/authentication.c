#include "authentication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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

    extern pthread_mutex_t csemwrite;

    char username[ 20 ], password[ 20 ], command[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );
    // L'utente potrebbe accedere per conto di un altro, ma il seguente controllo
    // lo impedisce.
    if( find( username, password, "database/connessi.dat" ) ) {
        // Il controllo inizia con la funzione find() che verifica se l'utente
        // è attualmente connesso. La verifica viene effettuata in base
        // all'username e alla password.
        if ( !find( username, password, "database/signed.dat" ) ) {
        // Se l'utente non era connesso viene effettuato un controllo per verificare
        // le credenziali passate come input alla funzione. Se l'utente è registrato
        // presso il server l'accesso gli sarà consentito.
            snprintf( command, sizeof( command ), "echo %d \"%s\" \"%s\" >> database/connessi.dat",
                    sdb, username, password );
            // Processo lettore scrittore che accede al file connessi.dat.
            pthread_mutex_lock( &csemwrite );
            system( command ); /* Sezione critica */
            pthread_mutex_unlock( &csemwrite );

            strcpy( body, "Accesso consentito!" );
        }
        else
            strcpy( body, "Accesso negato!" );
    }
    else
        strcpy( body, "Opzione non consentita!" );
}

void signup( int sdb, char *body ) {

    extern pthread_mutex_t csemwrite;
    extern pthread_mutex_t ssemwrite;

    char username[ 20 ], password[ 20 ], command1[ 100 ], command2[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    // Un utente non può creare un nuovo account se è già connesso.
    if ( !look( sdb ) ) {
        strcpy( body, "Devi prima disconnetterti" );
        return;
    }

    if ( !find( username, password, "database/connessi.dat" ) )
        strcpy( body, "Account esistente!" );
    else {
        snprintf( command1, sizeof( command1 ), "echo %d \"%s\" \"%s\" >> database/connessi.dat",
              sdb, username, password );

        // Processo scrittore che accede al file connessi.dat
        pthread_mutex_lock( &csemwrite );
        system( command1 ); /* Sezione critica */
        pthread_mutex_unlock( &csemwrite );

        snprintf( command2, sizeof( command2 ), "echo \"%s\" \"%s\" >> database/signed.dat",
                 username, password );

        // Processo scrittore che accede al file signed.dat
        pthread_mutex_lock( &ssemwrite );
        system( command2 ); /* Sezione critica */
        pthread_mutex_unlock( &ssemwrite );

        strcpy( body, "Account creato!" );
    }
}
