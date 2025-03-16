#include "session.h"
#include "search.h"

#include <string.h> // Qui è presente il prototipo della funzione strcpy
#include <stdlib.h> // Qui è stata definita la funzione system()
#include <stdio.h>  // Qui la funzione snprintf
#include <pthread.h>

// Scegliere altri nomi per le variabili.
unsigned int rcounter = 0; // lettori del file movies.dat.
// semaforo per assicurare che i lettori non accedino nello stesso
// momento durante l'aggiornamento della variabile rcounter.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// semaforo per assicurare la mutua esclusione per i processi scrittori
// nel file movies.dat.
pthread_mutex_t semwrite = PTHREAD_MUTEX_INITIALIZER;
// Il mutex viene inizializzato una sola volta dalla libreria POSIX.

int session( int sdb, int action, char *body ) {

    int result = 1;

    switch( action ) {
        case 0:
            rented( sdb, body );
            break;
        case 1:
            additem( sdb, body );
            break;
        case 2:
            delitem( sdb, body );
            break;
        case 3:
            returned( sdb, body );
            break;
        default:
            result = -1;
            break;
    }

    return result;
}

void takeout( char *body, char *filmname, char *number, char *date ) {

     for ( ; *body != '\0'; body++, filmname++ )
        *filmname = *body;
    *filmname = '\0';

    if ( number ) {
        body++;
        for ( ; *body != '\0'; body++, number++ )
            *number = *body;
        *number = '\0';
    }

    if ( date ) {
        body++;
        for ( ; *body != '\0'; body++, date++ )
            *date = *body;
        *date = '\0';
    }
}

unsigned int checkmovie( char *filmname, char *number, char *date ) {

    unsigned char status;
    char command[ 100 ];

    snprintf( command, sizeof( command ), "script/available.sh \"%s\" %d",
                filmname, atoi( number ) );
    // Processo lettore che accede al file movies.dat.
    status = reader( command, mutex, semwrite, rcounter );

    if( !status ) {
        snprintf( command, sizeof( command ), "script/date.sh %s", date );

        if ( !WEXITSTATUS( system( command ) ) )
            return 0;
        return 1;
    }
    else if( status == 1 )
        return 2;
    return 3;
}

void rented( int sdb, char *body ) {

    // Per il momento il noleggio è basato solo sulla data di restituizione,
    // nome del film e quantità disponibile.

    int res;
    char filmname[ 40 ], number[ 5 ], date[ 20 ], command[ 100 ];

    if ( !connected( sdb ) ) {
        strcpy( body, "Non sei connesso!" ); return;
    }

    takeout( body, filmname, number, date );
    res = checkmovie( filmname, number, date );

    switch( res ) {
        case 0:
            snprintf( command, sizeof( command ),
                      "script/rent.sh \"%s\" %d" , filmname, atoi( number ) );
            // Processo scrittore che accede al file movies.dat.
            writer( command, semwrite );
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

void additem( int sdb, char *body ) {

    char filmname[ 40 ], temp[ 40 ], command[ 100 ];

    takeout( body, filmname, NULL, NULL );

    if ( !connected( sdb ) ) {
        strcpy( body, "Non sei connesso!" ); return;
    }

    strcpy( temp ,filmname );
    search( temp );

    if ( strcmp( temp, "Film trovato") ) {
        strcpy( body, temp );
        return;
    }

    snprintf( command, sizeof( command ), "script/addcart.sh %d \"%s\"" ,
                sdb, filmname );
    if( system( command ) )
        strcpy( body, "Articolo presente nel carrello" );
    else
        strcpy( body, "Articolo aggiunto al carrello" );
}

void delitem( int sdb, char *body ) {

    char filmname[ 40 ], temp[ 40 ], command[ 100 ];

    takeout( body, filmname, NULL, NULL );

    if ( !connected( sdb ) ) {
        strcpy( body, "Non sei connesso!" ); return ;
    }

    strcpy( temp ,filmname );
    search( temp );

    if ( strcmp( temp, "Film trovato") ) {
        strcpy( body, temp );
        return;
    }

    snprintf( command, sizeof( command ), "script/fromcart.sh %d \"%s\"" ,
                sdb, filmname );

    if( system( command ) )
        strcpy( body, "Articolo non presente nel carrello" );
    else
        strcpy( body, "Articolo rimosso dal carrello" );
}

void returned( int sdb, char *body ) {

    char filmname[ 40 ], temp[ 40 ], number[ 5 ], command[ 100 ];
    takeout( body, filmname, number, NULL );

    if ( !connected( sdb ) ) {
        strcpy( body, "Non sei connesso!" );
        return;
    }

    strcpy( temp, filmname );
    search( temp );

    if ( strcmp( temp, "Film trovato") )
        strcpy( body, "Film non trovato" );
    else {
        strcpy( body, "Restituzione approvata" );
        snprintf( command, sizeof( command ), "script/returned.sh \"%s\" %d" ,
                filmname, atoi( number ) );
        // Processo scrittore che accede al file movies.dat.
        writer( command, semwrite );
    }
}
