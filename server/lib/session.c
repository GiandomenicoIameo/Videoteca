#include "session.h"
#include "search.h"

#include <string.h> // Qui è presente il prototipo della funzione strcpy
#include <stdlib.h> // Qui è stata definita la funzione system()
#include <stdio.h>  // Qui la funzione snprintf
#include <pthread.h>
#include <zlib.h>

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
                    case 0: // Richiesta di noleggio.
                            rentest( recuid( sdb ), body );
                            rentable();
                            break;
                    case 1: // Richiesta di aggiunta al carrello.
                            setout( recuid( sdb ), body );
                            // preadd( recuid( sdb ), body );
                            break;
                    // case 2: // Richiesta di rimozione dal carrello.
                            // predel( recuid( sdb ), body );
                            break;
                    case 2: // Richiesta di restituzione.
                            returntest( recuid( sdb ), body );
                            rentable();
                            break;
                    case 3: // Richiesta di checkout.
                            checkout( recuid( sdb ), body );
                            rentable();
                            break;
                    case 4:
                            showcart( recuid( sdb ), body );
                            break;
                    case 5: showrented( recuid( sdb ), body );
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

    // Per il momento il noleggio è basato solo sulla data di restituizione,
    // nome del film e quantità disponibile.

    void rent( int uid, char *filmname, int number, char *date );

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
                    strcpy( body, "Quantità non noleggiabile!" );
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

void setout( int uid, char *body ) {

    void setup( int uid, char* filmname, int eamount, char *date );

    char filmname[ 40 ], eamount[ 5 ],
             date[ 20 ];

    takeout( body, filmname, eamount, date );

    switch( rentalchk( filmname, eamount, date ) ) {
            case 0:
                    setup( uid, filmname, atoi( eamount ), date );
                    strcpy( body, "Articolo/i impostato" );
                    break;
            case 1:
                    strcpy( body, "Data non valida!" );
                    break;
            case 2:
                    strcpy( body, "Quantità non noleggiabile!" );
                    break;
            case 3:
                    strcpy( body, "Film non trovato!" );
                    break;
    }
}

void setup( int uid, char* filmname, int eamount, char *date ) {

    char command[ 100 ];
    unsigned int ramount;

    snprintf( command, sizeof( command ), "script/search/ramount.sh \"%s\"",
                      filmname );
    // Processo lettore che accede al file movies.dat.
    ramount = reader( command, mtm, wrtm, rdm );

    snprintf( command, sizeof( command ),
                      "script/session/addcart.sh %d \"%s\" %d %d \"%s\"",
                      uid, filmname, eamount, ramount, date );
    system( command );
}


/*
void preadd( int uid, char *body ) {

    void additem( int uid, char *filmname, int eamount, char *date );

    char filmname[ 40 ], eamount[ 5 ],
             date[ 20 ];

    takeout( body, filmname, eamount, date );

    switch( rentalchk( filmname, eamount, date ) ) {
            case 0:
                    additem( uid, filmname, atoi( eamount ), date );
                    strcpy( body, "Articolo/i aggiunto al carrello" );
                    break;
            case 1:
                    strcpy( body, "Data non valida!" );
                    break;
            case 2:
                    strcpy( body, "Quantità non noleggiabile!" );
                    break;
            case 3:
                    strcpy( body, "Film non trovato!" );
                    break;
    }
}

void additem( int uid, char *filmname, int eamount, char *date ) {

    char command[ 100 ];
    unsigned int ramount;

    snprintf( command, sizeof( command ), "script/search/ramount.sh \"%s\"",
                      filmname );
    // Processo lettore che accede al file movies.dat.
    ramount = reader( command, mtm, wrtm, rdm );

    snprintf( command, sizeof( command ),
                      "script/session/addcart.sh %d \"%s\" %d %d \"%s\"",
                      uid, filmname, eamount, ramount, date );
    system( command );
}*/

/*
void predel( int uid, char *body ) {

    void delitem( int uid, char *filmname, int number );

    unsigned int res;

    char filmname[ 40 ], number[ 5 ],
             date[ 20 ], command[ 100 ];

    takeout( body, filmname, number, date );

    snprintf( command, sizeof( command ),
              "script/session/removechk.sh %d \"%s\" %d \"%s\"",
              uid, filmname, atoi( number ), date );

    res = reader( command, mtm, wrtm, rdm );

    switch( res ) {
            case 0:
                    delitem( uid, filmname, atoi( number ) );
                    strcpy( body, "Articolo/i rimosso dal carrello!" );
                    break;
            case 1:
                    strcpy( body, "Valore inserito non valido!" );
                    break;
            case 2:
                    strcpy( body, "La data non corrisponde!" );
                    break;
            case 3:
                    strcpy( body, "Film non trovato!" );
    }
}

void delitem( int uid, char *filmname, int number ) {

    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/fromcart.sh %d \"%s\" %d",
              uid, filmname, number );
     system( command );
}*/

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

    void rentall( int uid );
    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/checkout.sh %d", uid );
    if ( WEXITSTATUS( system( command ) ) ) {
            strcpy( body, "Checkout riuscito!" );
            rentall( uid );
    } else {
            strcpy( body, "Checkout fallito!" );
    }
}

void rentall( int uid ) {

    char command[ 100 ];
    snprintf( command, sizeof( command ), "script/session/rentall.sh %d",
             uid );
    writer( command, wrtm );

    snprintf( command, sizeof( command ), "script/session/up.sh %d",
             uid );
    system( command );
}

void showcart( int uid, char *body ) {

    // Capire il significato delle seguenti istruzioni.

    FILE *fpointer;
	char buffer[ 1024 ], command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/compress.sh cart%d", uid );
    system( command );

    snprintf( command, sizeof( command ),
              "script/session/tohex.sh cart%d", uid );

	fpointer = popen( command, "r" );
	if ( fpointer == NULL ) {
			perror( "Errore nell'esecuzione dello script" );
            pthread_exit( ( void * )1 );
	} else {
			while ( fgets( buffer, sizeof( buffer ), fpointer ) != NULL );
			pclose( fpointer );
			strcpy( body, buffer );
	}
}

void updatecart( int uid, char *filmname ) {

    char command[ 100 ];
    unsigned int ramount;

    snprintf( command, sizeof( command ),
              "script/search/ramount.sh \"%s\"", filmname );
    // Processo lettore che accede al file movies.dat.
    ramount = reader( command, mtm, wrtm, rdm );

    snprintf( command, sizeof( command ),
              "script/session/updatecart.sh %d \"%s\" %d",
              uid, filmname, ramount );
    system( command );
}

void showrented( int uid, char *body ) {

    FILE *fpointer;
	char buffer[ 1024 ], command[ 100 ];

    snprintf( command, sizeof( command ),
              "script/session/compress.sh rented%d", uid );
    system( command );

    snprintf( command, sizeof( command ),
              "script/session/tohex.sh rented%d", uid );

	fpointer = popen( command, "r" );
	if ( fpointer == NULL ) {
			perror( "Errore nell'esecuzione dello script" );
            pthread_exit( ( void * )1 );
	} else {
			while ( fgets( buffer, sizeof( buffer ), fpointer ) != NULL );

			pclose( fpointer );
			strcpy( body, buffer );
	}
}
