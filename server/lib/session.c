#include "session.h"
#include "search.h"

#include <string.h> // Qui è presente il prototipo della funzione strcpy
#include <stdlib.h> // Qui è stata definita la funzione system()
#include <stdio.h>  // Qui la funzione snprintf
#include <pthread.h>

// Scegliere altri nomi per le variabili.
unsigned int rdm = 0; // lettori del file movies.dat.
// semaforo per assicurare che i lettori non accedino nello stesso
// momento durante l'aggiornamento della variabile rdm.
pthread_mutex_t mtm = PTHREAD_MUTEX_INITIALIZER;
// semaforo per assicurare la mutua esclusione per i processi scrittori
// nel file movies.dat.
pthread_mutex_t wrtm = PTHREAD_MUTEX_INITIALIZER;
// Il mutex viene inizializzato una sola volta dalla libreria POSIX.

int session( int sdb, int action, char *body ) {

    // Ogni funzione nel costrutto switch poggia sull'assunto
    // che il client che desidera effettuare le operazioni di sessione
    // abbia effettuato in un precedente momento la connessione
    // all'account.

    int result = 1;

    if ( !connected( sdb ) ) {
            strcpy( body, "Non sei connesso!" );
    } else {
            switch( action ) {
                    case 0:
                            rentest( body );
                            break;
                    case 1:
                            additem( sdb, body );
                            break;
                    case 2:
                            delitem( sdb, body );
                            break;
                    case 3:
                            returned( body );
                            break;
                    case 4:
                            checkout( sdb, body );
                            break;
                    default:
                            result = -1;
                            break;
            }
    }
    return result;
}

void takeout( char *body, char *filmname, char *number, char *date ) {

     for ( ; *body != '\0'; body++, filmname++ ) {
            *filmname = *body;
     } *filmname = '\0';

    if ( number ) {
            body++;
            for ( ; *body != '\0'; body++, number++ ) {
                    *number = *body;
            } *number = '\0';
    }

    if ( date ) {
            body++;
            for ( ; *body != '\0'; body++, date++ ) {
                    *date = *body;
            } *date = '\0';
    }
}

unsigned int checkmovie( char *filmname, char *number, char *date ) {

    unsigned char status;
    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/available.sh \"%s\" %d",
              filmname, atoi( number ) );
    // Processo lettore che accede al file movies.dat.
    status = reader( command, mtm, wrtm, rdm );

    if( !status ) {
            snprintf( command, sizeof( command ),
                      "script/session/date.sh %s", date );

            if ( !WEXITSTATUS( system( command ) ) ) {
                    return 0;
            } else {
                    return 1;
            }
    } else if( status == 1 ) {
            return 2;
    } else {
            return 3;
    }
}

void rentest( char *body ) {

    void rent( char *filmname, int number );
    // Per il momento il noleggio è basato solo sulla data di restituizione,
    // nome del film e quantità disponibile.

    unsigned int res;
    char filmname[ 40 ], number[ 5 ], date[ 20 ];

    takeout( body, filmname, number, date );
    res = checkmovie( filmname, number, date );

    switch( res ) {
            case 0:
                    rent( filmname, atoi( number ) );
                    strcpy( body, "Noleggio approvato" );
                    break;
            case 1:
                    strcpy( body, "Data non valida" );
                    break;
            case 2:
                    strcpy( body, "Quantità non disponibile" );
                    break;
            default:
                    strcpy( body, "Film non trovato" );
                    break;
    }
}

void rent( char *filmname, int number ) {

    void update( char *filmname );

    extern pthread_mutex_t wrtm;
    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/rent.sh \"%s\" %d" ,
              filmname, number );
    // Processo scrittore che accede al file movies.dat.
    writer( command, wrtm );
    update( filmname );
}

void additem( int sdb, char *body ) {

    unsigned int res;
    char filmname[ 40 ], temp[ 40 ], number[ 5 ], command[ 100 ];

    takeout( body, filmname, number, NULL );

    snprintf( command, sizeof( command ), "script/session/available.sh \"%s\" %d",
              filmname, atoi( number ) );
    res = WEXITSTATUS( system( command ) );

    if ( res ) {
            strcpy( body, "Film non trovato o quantità non disponibile!" );
            return;
    }

    strcpy( temp ,filmname );
    search( temp );

    if ( strcmp( temp, "Film trovato") ) {
            strcpy( body, temp );
    } else {
            snprintf( command, sizeof( command ), "script/search/number.sh \"%s\"",
                      filmname );
            res = WEXITSTATUS( system( command ) );
            snprintf( command, sizeof( command ),
                      "script/session/addcart.sh %d \"%s\" %d %d" ,
                      sdb, filmname, atoi( number ), res );
            system( command );

            strcpy( body, "Film trovato!" );
    }
}

void delitem( int sdb, char *body ) {

    char filmname[ 40 ], temp[ 40 ], command[ 100 ];

    takeout( body, filmname, NULL, NULL );

    strcpy( temp ,filmname );
    search( temp );

    if ( strcmp( temp, "Film trovato") ) {
            strcpy( body, temp );
    } else {
            snprintf( command, sizeof( command ),
                      "script/session/fromcart.sh %d \"%s\"",
                      sdb, filmname );

            if( WEXITSTATUS( system( command ) ) ) {
                    strcpy( body, "Articolo non presente nel carrello!" );
            } else {
                    strcpy( body, "Articolo rimosso dal carrello!" );
            }
    }
}

void returned( char *body ) {

        void update( char *filmname );

        char filmname[ 40 ], temp[ 40 ], number[ 5 ], command[ 100 ];
        takeout( body, filmname, number, NULL );

        strcpy( temp, filmname );
        search( temp );

        if ( !strcmp( temp, "Film trovato") ) {
                strcpy( body, "Restituzione approvata" );
                snprintf( command, sizeof( command ),
                          "script/session/returned.sh \"%s\" %d" ,
                          filmname, atoi( number ) );
                // Processo scrittore che accede al file movies.dat.
                writer( command, wrtm );
                update( filmname );
        } else {
                strcpy( body, "Film non trovato" );
        }
}

void checkout( int sdb, char *body ) {

    unsigned int res;
    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/checkout.sh %d", sdb );
    res = WEXITSTATUS( system( command ) );

    if ( res ) {
            strcpy( body, "Checkout riuscito!" );
    } else {
            strcpy( body, "Checkout fallito!" );
    }
}

void update( char *filmname ) {

    char command[ 100 ];
    unsigned int number;

    snprintf( command, sizeof( command ),
              "script/search/number.sh \"%s\"", filmname );
    // Processo lettore che accede al file movies.dat.
    number = reader( command, mtm, wrtm, rdm );

    snprintf( command, sizeof( command ),
              "script/session/update.sh \"%s\" %d", filmname, number );
    system( command );
}
