#include "authentication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int authentication( int sdb, int action, char *body ) {

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

    unsigned char enabled( char *username );
    unsigned char recorded( char *username, char *password );

    extern pthread_mutex_t csemwrite;

    char username[ 20 ], command[ 100 ],
         password[ 20 ];

    // Estrazione dell'username e della password dal corpo del messaggio.
    extract( body, username, password );
    if ( !connected( sdb ) ) {
        // Se il precedente test non fallisce allora si tratterà sicuramente
        // di un client che sta tendando l'accesso a un account per la prima
        // volta.
        if( !enabled( username ) ) {
            // Il precedente controllo verifica se l'username ricevuto dal client è associato già
            // a un account correntemente connesso presso il server.
            if ( recorded( username, password ) ) {
                // Quest'ultimo controllo verifica se esiste un account registrato presso il
                // server identificato dalla coppia ( username, password ) specificata dal client.
                snprintf( command, sizeof( command ), "echo %d:\"%s\":\"%s\" >> database/connessi.dat",
                        sdb, username, password );
                // Processo lettore scrittore che accede al file connessi.dat.
                writer( command, csemwrite );
                strcpy( body, "Accesso consentito!" );
            } else
                strcpy( body, "Accesso negato!" );
        } else
            sprintf( body, "L'utente %s risulta già connesso!", username );
    }
    else
        strcpy( body, "Sei già connesso!" );
}

void signup( int sdb, char *body ) {

    unsigned char unique( char *username );

    extern pthread_mutex_t csemwrite;
    extern pthread_mutex_t ssemwrite;

    char username[ 20 ], command[ 100 ],
         password[ 20 ];

    // Un utente connesso non può creare un nuovo account.
    if ( connected( sdb ) ) {
        strcpy( body, "Sei già connesso!" ); return;
    }
    // Estrazione dell'username e della password dal corpo del messaggio.
    extract( body, username, password );
    // Un nuovo utente deve scegliere un username univoco. Se le
    // password sono uguali, l'elemento che permette di distinguere
    // un account da un altro è il nome utente.
    if ( !unique( username ) ) {
        strcpy( body, "Username già in uso!" ); return;
    }
    snprintf( command, sizeof( command ), "echo %d:\"%s\":\"%s\" >> database/connessi.dat",
              sdb, username, password );
    // Processo scrittore che accede al file connessi.dat.
    writer( command, csemwrite );
    snprintf( command, sizeof( command ), "echo \"%s\":\"%s\" >> database/signed.dat",
                username, password );
    // Processo scrittore che accede al file signed.dat
    writer( command, ssemwrite );
    strcpy( body, "Account creato!" );
}

unsigned char enabled( char *username ) {

    extern pthread_mutex_t cmutex;
    extern pthread_mutex_t csemwrite;

    extern unsigned int rccount;

    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/enabled.sh \"%s\"",
             username );
    // Processo lettore che accede al file connessi.dat
    return reader( command, cmutex, csemwrite, rccount );
}

unsigned char recorded( char *username, char *password ) {

    extern pthread_mutex_t smutex;
    extern pthread_mutex_t ssemwrite;

    extern unsigned int rscount;

    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/recorded.sh \"%s\" \"%s\"",
             username, password );
    // Processo lettore che accede al file signed.dat.
    return reader( command, smutex, ssemwrite, rscount );
}

unsigned char unique( char *username ) {

    extern pthread_mutex_t smutex;
    extern pthread_mutex_t ssemwrite;

    extern unsigned int rscount;

    char command[ 100 ];
    snprintf( command, sizeof( command ), "script/unique.sh \"%s\"",
              username );
    // Processo lettore che accede al file signed.dat
    return reader( command, smutex, ssemwrite, rscount );
}
