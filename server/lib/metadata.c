#include "metadata.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// numero lettori che accedono al file signed.dat.
unsigned int rds = 0;
// semaforo per assicurare che i lettori non accedino nello stesso
// momento durante l'aggiornamento della variabile rscount.
semaphore mts = PTHREAD_MUTEX_INITIALIZER;
// semaforo per assicurare la mutua esclusione per i processi scrittori
// che accedono al file signed.dat.
semaphore wrts = PTHREAD_MUTEX_INITIALIZER;

void writer( char *command, semaphore mutex ) {

    pthread_mutex_lock( &mutex );
    system( command ); /* Sezione critica */
    pthread_mutex_unlock( &mutex );
}

unsigned char reader( char *command, semaphore mutex, semaphore write, unsigned int readers ) {

    unsigned char res;

    pthread_mutex_lock( &mutex );
    if ( ++readers == 1 ) {
            pthread_mutex_lock( &write );
    } pthread_mutex_unlock( &mutex );

    res = WEXITSTATUS( system( command ) ); /* Sezione critica */

    pthread_mutex_lock( &mutex );
    if ( --readers == 0 ) {
            pthread_mutex_unlock( &write );
    } pthread_mutex_unlock( &mutex );

    return res;
}

int connected( int sdb ) {

    char command[ 100 ];
    snprintf( command, sizeof( command ),
              "script/metadata/connected.sh %d", sdb );
    // Processo lettore
    return reader( command, mts, wrts, rds );
}

void extract( char *body, char *username, char *password ) {

    // La procedura extract utilizza il passaggio per riferimento
    // per "restituire" al chiamante più valori modificando variabili
    // nella funzione chiamante. In questo caso vengono modificati
    // gli array nella funzione chiamante relativi all'username e
    // alla password, memorizzandovi delle stringhe.

    for( ; *body != ' '; body++, username++ ) {
            *username = *body;
    } *username = '\0';

    body++;
    for( ; *body != '\0'; body++, password++ ) {
            *password = *body;
    } *password = '\0';
}
