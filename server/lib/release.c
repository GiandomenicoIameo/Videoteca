#include "release.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int release( int sdb, int action, char *body ) {

    // I prototipi per le funzioni cancel() e signout() sono inclusi
    // nel corpo della funzione release perché è l'unica funzione che
    // invoca cancel() e signout()

    int result = 2;

    if ( !connected( sdb ) ) {
            strcpy( body, "Non sei connesso!" );
    } else {
            switch( action ) {
                    case LOGOUT:
                            signout( sdb, body );
                            rentable();
                    break;
                    case CANCEL:
                            cancel( sdb, body );
                            rentable();
                    break;
                    default:
                            result = -1;
                    break;
            }
    }
    return result;
}

void signout( int sdb, char *body ) {

    // La funzione signout poggia sull'assunto che l'utente che desidera
    // cancellare il suo account abbia effettuato in un precedente momento
    // l'accesso a quest'ultimo.

    extern semaphore wrts;

    // L'utente non può disconnetterne un altro perché
    // la procedura si basa solo sull'dentificativo
    // associato alla connessione che ha la funzione di collegare
    // ogni connessione a un account.

    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "sed -ri 's/(.*:)%d/\\10/' database/signed.dat", sdb );
    // Processo scrittore che accede al file signed.dat
    writer( command, wrts );
    strcpy( body, "Account disconnesso!" );
}

void cancel( int sdb, char *body ) {

    // La funzione cancel poggia sull'assunto che l'utente che desidera
    // cancellare il suo account abbia effettuato in un precedente momento
    // l'accesso a quest'ultimo.id = system( command );

    extern semaphore wrts;

    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/release/cancel/cancel.sh %d", sdb );
    // Processo scrittore che accede al file signed.dat.
    writer( command, wrts );

    snprintf( command, sizeof( command ),
              "script/release/cancel/removecart.sh %d", recuid( sdb ) );
    system( command );
    strcpy( body, "Account cancellato!" );
}
