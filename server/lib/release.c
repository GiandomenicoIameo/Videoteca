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

    extern pthread_mutex_t csemwrite;

    // L'utente non può disconnetterne un altro perché
    // la procedura si basa solo sull'dentificativo
    // associato alla connessione che ha la funzione di collegare
    // ogni connessione a un account.

    char command[ 100 ];

    if ( !look( sdb ) ) {
        snprintf( command, sizeof( command ), "sed -i '/^%d/d' database/connessi.dat",
                  sdb );
        // Processo scrittore che accede al file connessi.dat
        pthread_mutex_lock( &csemwrite );
        system( command ); /* Sezione critica */
        pthread_mutex_unlock( &csemwrite );

        strcpy( body, "Account disconnesso" );
    }
    else
        strcpy( body, "Account non connesso!" );
}

void cancel( int sdb, char *body ) {

    extern pthread_mutex_t ssemwrite;

    // La funzione cancel poggia sull'assunto che l'utente che desidera
    // cancellare il suo account abbia effettuato in un precedente momento
    // l'accesso a quest'ultimo.

    char username[ 20 ], password[ 20 ],
            command1[ 100 ], command2[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    if( !verify( sdb, username, password ) ) {
        // L'if verifica se l'utente era connesso prima di effettuare la
        // cancellazione dell'account.
        snprintf( command1, sizeof( command1 ), "sed -i '/%s %s/d' database/signed.dat",
                username, password );
        // Processo scrittore che accede al file signed.dat
        pthread_mutex_lock( &ssemwrite );
        system( command1 );
        pthread_mutex_unlock( &ssemwrite );

        snprintf( command2, sizeof( command2 ), "script/removecart.sh %d", sdb );
        system( command2 );

        strcpy( body, "Account cancellato!" );
    }
    else
        strcpy( body,"Cancellazione Account fallita!" );
        // Qui è necessario inserire un ciclo
}

unsigned int verify( int sdb, char *username, char *password ) {

    extern unsigned int rccount;

    extern pthread_mutex_t cmutex;
    extern pthread_mutex_t csemwrite;

    unsigned int res;
    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/verify.sh %d \"%s\" \"%s\" ",
            sdb, username, password );
    // Processo lettore che accede al file connessi.dat
    pthread_mutex_lock( &cmutex );
    rccount++;
    if ( rccount == 1 )
        pthread_mutex_lock( &csemwrite );
    pthread_mutex_unlock( &cmutex );

    res = WEXITSTATUS( system( command ) ); /* Sezione critica */

    pthread_mutex_lock( &cmutex );
    rccount--;
    if ( rccount == 0 )
        pthread_mutex_unlock( &csemwrite );
    pthread_mutex_unlock( &cmutex );

    return res;
}
