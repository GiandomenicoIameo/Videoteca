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

    extern pthread_mutex_t ssemwrite;

    // L'utente non può disconnetterne un altro perché
    // la procedura si basa solo sull'dentificativo
    // associato alla connessione che ha la funzione di collegare
    // ogni connessione a un account.

    char command[ 100 ];

    if ( connected( sdb ) ) {
        snprintf( command, sizeof( command ), "script/disconnect.sh %d",
                  sdb );
        // Processo scrittore che accede al file signed.dat
        writer( command, ssemwrite );
        strcpy( body, "Account disconnesso!" );
    }
    else
        strcpy( body, "Account non connesso!" );
}

void cancel( int sdb, char *body ) {

    unsigned char associated( char *username, char *password );

    extern pthread_mutex_t ssemwrite;
    extern pthread_mutex_t csemwrite;

    // La funzione cancel poggia sull'assunto che l'utente che desidera
    // cancellare il suo account abbia effettuato in un precedente momento
    // l'accesso a quest'ultimo.

    char username[ 20 ], command[ 100 ],
         password[ 20 ];
    // Estrazione dell'username e della password dal corpo del messaggio.
    extract( body, username, password );
    if ( connected( sdb ) && associated( username, password ) ) {
        snprintf( command, sizeof( command ), "sed -i '/%s:%s/d' database/signed.dat",
                    username, password );
        // Processo scrittore che accede al file signed.dat.
        writer( command, ssemwrite );
        snprintf( command, sizeof( command ), "sed -i '/^%d/d' database/connessi.dat",
                    sdb );
        // Processo scrittore che accede al file connessi.dat.
        writer( command, csemwrite );
        snprintf( command, sizeof( command ), "script/removecart.sh %d", sdb );
        system( command );

        strcpy( body, "Account cancellato!" );
    } else
        // Se il client non è connesso oppure l'username e la password dichiarati non
        // corrispondono all'ID della sessione, la cancellazione dell'account sarà negata.
        strcpy( body, "Cancellazione Account fallita!" );
}

unsigned char associated( char *username, char *password ) {

    extern pthread_mutex_t smutex;
    extern pthread_mutex_t ssemwrite;

    extern unsigned int rscount;

    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/associated.sh \"%s\" \"%s\"",
             username, password );
    // Processo lettore che accede al file signed.da
    return reader( command, smutex, ssemwrite, rscount );
}
