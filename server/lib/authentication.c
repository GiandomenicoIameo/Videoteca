#include "authentication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int authentication( int sdb, int action, char *body ) {

    int result = 0;

    // Se il seguente test, espresso dal costrutto if ( connected( sdb ) ),
    // non fallisce allora si tratterà sicuramente di un client che sta
    // tendando l'accesso a un account per la prima volta.

    if ( connected( sdb ) ) {
            strcpy( body, "Sei già connesso!" );
    } else {
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
    }
    return result;
}

void signin( int sdb, char *body ) {

    unsigned char recorded( char *username, char *password );

    char username[ 20 ], command[ 100 ],
         password[ 20 ];

    extern pthread_mutex_t wrts;

    // Estrazione dell'username e della password dal corpo del messaggio.
    extract( body, username, password );

    if ( recorded( username, password ) ) {
            // Quest'ultimo controllo verifica se esiste un account registrato presso il
            // server identificato dalla coppia ( username, password ) specificata dal client.
            snprintf( command, sizeof( command ), "script/authentication/connect.sh %d \"%s\" \"%s\"",
                        sdb, username, password );
            // Processo lettore scrittore che accede al file connessi.dat.
            writer( command, wrts );
            strcpy( body, "Accesso consentito!" );
    } else {
            strcpy( body, "Accesso negato!" );
    }
}

void signup( int sdb, char *body ) {

    unsigned char unique( char *username );

    char username[ 20 ], command[ 100 ],
         password[ 20 ];

    extern pthread_mutex_t wrts;

    // Estrazione dell'username e della password dal corpo del messaggio.
    extract( body, username, password );
    // Un nuovo utente deve scegliere un username univoco. Se le
    // password sono uguali, l'elemento che permette di distinguere
    // un account da un altro è il nome utente.
    if ( !unique( username ) ) {
            strcpy( body, "Username già in uso!" );
    } else {
            snprintf( command, sizeof( command ),
                      "script/authentication/create.sh %d \"%s\" \"%s\"",
                      sdb, username, password );
            // Processo scrittore che accede al file signed.dat.
            writer( command, wrts );
            strcpy( body, "Account creato!" );
    }
}

unsigned char recorded( char *username, char *password ) {

    extern unsigned int rds;

    extern pthread_mutex_t mts;
    extern pthread_mutex_t wrts;

    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/authentication/recorded.sh \"%s\" \"%s\"",
              username, password );
    // Processo lettore che accede al file signed.dat.
    return reader( command, mts, wrts, rds );
}

unsigned char unique( char *username ) {

    extern unsigned int rds;

    extern pthread_mutex_t mts;
    extern pthread_mutex_t wrts;

    char command[ 100 ];
    snprintf( command, sizeof( command ),
              "script/authentication/unique.sh \"%s\"", username );
    // Processo lettore che accede al file signed.dat
    return reader( command, mts, wrts, rds );
}
