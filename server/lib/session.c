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
semaphore mtm = PTHREAD_MUTEX_INITIALIZER;
// semaforo per assicurare la mutua esclusione per i processi scrittori
// nel file movies.dat.
semaphore wrtm = PTHREAD_MUTEX_INITIALIZER;
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
                            rentest( recuid( sdb ), body );
                            break;
                    case 1:
                            preadd( recuid( sdb ), body );
                            break;
                    case 2:
                            delitem( recuid( sdb ), body );
                            break;
                    case 3:
                            returntest( recuid( sdb ), body );
                            break;
                    case 4:
                            checkout( recuid( sdb ), body );
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

unsigned int rentalchk( char *filmname, char *number, char *date ) {

    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/rentalchk.sh \"%s\" %d \"%s\"",
              filmname, atoi( number ), date );
    // Processo lettore che accede al file movies.dat.
    return reader( command, mtm, wrtm, rdm );
}

void rentest( int uid, char *body ) {

    void rent( int uid, char *filmname, int number, char *date );
    // Per il momento il noleggio è basato solo sulla data di restituizione,
    // nome del film e quantità disponibile.

    char filmname[ 40 ], number[ 5 ],
             date[ 20 ];

    takeout( body, filmname, number, date );

    // Non esiste il caso default poiché si è certi che tutte le possibili
    // opzioni siano già coperte dai vari case.
    switch( rentalchk( filmname, number, date ) ) {
            case 0:
                    rent( uid, filmname, atoi( number ), date );
                    strcpy( body, "Noleggio approvato!" );
                    break;
            case 1:
                    strcpy( body, "Data non valida!" );
                    break;
            case 2:
                    strcpy( body, "Quantità non disponibile!" );
                    break;
            case 3:
                    strcpy( body, "Film non trovato!" );
                    break;
    }
}

void rent( int uid, char *filmname, int number, char *date ) {

    void updatecart( int uid, char *filmname );

    extern semaphore wrtm;
    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/rent.sh \"%s\" %d",
              filmname, number );
    // Processo scrittore che accede al file movies.dat.
    writer( command, wrtm );

    snprintf( command, sizeof( command ),
              "script/session/addrent.sh %d \"%s\" %d \"%s\"",
              uid, filmname, number, date );
    system( command );
    updatecart( uid, filmname );
}

void preadd( int uid, char *body ) {

    void additem( int uid, char *filmname, int eamount );

    unsigned int res;
    char filmname[ 40 ], number[ 5 ], command[ 100 ];

    takeout( body, filmname, number, NULL );

    snprintf( command, sizeof( command ),
              "script/session/checkmovie.sh \"%s\" %d",
              filmname, atoi( number ) );
    res = reader( command, mtm, wrtm, rdm );

    switch( res ) {
            case 0:
                    additem( uid, filmname, atoi( number ) );
                    strcpy( body, "Articolo/i aggiunto al carrello!" );
                    break;
            case 1:
                    strcpy( body, "Quantità non disponibile!" );
                    break;
            case 2:
                    strcpy( body, "Film non trovato!" );
                    break;
    }
}

void additem( int uid, char *filmname, int eamount ) {

    char command[ 100 ];
    unsigned int ramount;

    snprintf( command, sizeof( command ), "script/search/number.sh \"%s\"",
                      filmname );
    // Processo lettore che accede al file movies.dat
    ramount = reader( command, mtm, wrtm, rdm );

    snprintf( command, sizeof( command ),
                      "script/session/addcart.sh %d \"%s\" %d %d",
                      uid, filmname, eamount, ramount );
    system( command );
}

void delitem( int uid, char *body ) {

    unsigned int res;
    char filmname[ 40 ], number[ 5 ], command[ 100 ];

    takeout( body, filmname, number, NULL );

    snprintf( command, sizeof( command ), "script/session/searchitem.sh %d \"%s\" %d",
              uid, filmname, atoi( number ) );
    res = reader( command, mtm, wrtm, rdm );

    if ( res == 2 ) {
            strcpy( body, "Film non trovato!" );
    } else if( res == 1 ) {
            strcpy( body, "Valore inserito non valido!" );
    } else {
            snprintf( command, sizeof( command ),
                      "script/session/fromcart.sh %d \"%s\" %d",
                      uid, filmname, atoi( number ) );
            // Processo che accede al proprio carrello e agisce come processo
            // lettore.
            system( command );
            strcpy( body, "Articolo/i rimosso dal carrello!" );
    }
}

void returntest( int uid, char *body ) {

    void returned( int uid, char *filmname, int number, char *date );
    void updatecart( int uid, char *filmname );

    char filmname[ 40 ], number[ 5 ],
             date[ 20 ], command[ 100 ];

    takeout( body, filmname, number, date );

    snprintf( command, sizeof( command ),
              "script/session/returnchk.sh %d \"%s\" %d \"%s\"",
              uid, filmname, atoi( number ), date );

    switch( WEXITSTATUS( system( command ) ) ) {
            case 0:
                    returned( uid, filmname, atoi( number ), date );
                    updatecart( uid, filmname );
                    strcpy( body, "Restituzione approvata!" );
                    break;
            case 1:
                    strcpy( body, "Quantità non disponibile!" );
                    break;
            case 2:
                    strcpy( body, "Data inesatta!" );
                    break;
            case 3:
                    strcpy( body, "Film non noleggiato!" );
                    break;
    }
}

void returned( int uid, char *filmname, int number, char *date ) {

    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/updatemovies.sh \"%s\" %d" ,
              filmname, number );
    // Processo scrittore che accede al file movies.dat.
    writer( command, wrtm );

    snprintf( command, sizeof( command ),
              "script/session/delrented.sh %d \"%s\" %d \"%s\"",
              uid, filmname, number, date );
    system( command );
}

void checkout( int uid, char *body ) {

    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/checkout.sh %d", uid );
    if ( WEXITSTATUS( system( command ) ) ) {
            strcpy( body, "Checkout riuscito!" );
    } else {
            strcpy( body, "Checkout fallito!" );
    }
}

void updatecart( int uid, char *filmname ) {

    char command[ 100 ];
    unsigned int number;

    snprintf( command, sizeof( command ),
              "script/search/number.sh \"%s\"", filmname );
    // Processo lettore che accede al file movies.dat.
    number = reader( command, mtm, wrtm, rdm );

    snprintf( command, sizeof( command ),
              "script/session/updatecart.sh %d \"%s\" %d",
              uid, filmname, number );
    system( command );
}
