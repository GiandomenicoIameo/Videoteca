#include "metadata.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

unsigned int rccount = 0; // numero lettori che accedono al file connessi.dat.
unsigned int rscount = 0; // numero lettori che accedono al file signed.dat.

// semaforo per assicurare che i lettori non accedino nello stesso
// momento durante l'aggiornamento della variabile rrcount.
pthread_mutex_t cmutex = PTHREAD_MUTEX_INITIALIZER;
// semaforo per assicurare la mutua esclusione per i processi scrittori
// che accedono al file connessi.dat.
pthread_mutex_t csemwrite = PTHREAD_MUTEX_INITIALIZER;
// semaforo per assicurare che i lettori non accedino nello stesso
// momento durante l'aggiornamento della variabile rscount.
pthread_mutex_t smutex = PTHREAD_MUTEX_INITIALIZER;
// semaforo per assicurare la mutua esclusione per i processi scrittori
// che accedono al file signed.dat.
pthread_mutex_t ssemwrite = PTHREAD_MUTEX_INITIALIZER;

void writer( char *command, pthread_mutex_t mutex ) {

    pthread_mutex_lock( &mutex );
    system( command ); /* Sezione critica */
    pthread_mutex_unlock( &mutex );
}

unsigned char reader( char *command, pthread_mutex_t mutex, pthread_mutex_t write, unsigned int readers ) {
    unsigned char res;

    pthread_mutex_lock( &mutex );
    readers++;
    if ( readers == 1 )
        pthread_mutex_lock( &write );
    pthread_mutex_unlock( &mutex );

    res = WEXITSTATUS( system( command ) ); /* Sezione critica */

    pthread_mutex_lock( &mutex );
    readers--;
    if ( readers == 0 )
        pthread_mutex_unlock( &write );
    pthread_mutex_unlock( &mutex );

    return 1 - res;
}

int connected( int sdb ) {

    char command[ 100 ];
    snprintf( command, sizeof( command ), "script/connected.sh %d", sdb );
    // Processo lettore che accede al file connessi.dat
    return reader( command, cmutex, csemwrite, rccount );
}

void extract( char *body, char *username, char *password ) {

    // La procedura extract utilizza il passaggio per riferimento
    // per "restituire" al chiamante più valori modificando variabili
    // nella funzione chiamante. In questo caso vengono modificati
    // gli array nella funzione chiamante relativi all'username e
    // alla password, memorizzandovi delle stringhe.

    for( ; *body != ' '; body++, username++ )
        *username = *body;
    *username = '\0';

    body++;
    for( ; *body != '\0'; body++, password++ )
        *password = *body;
    *password = '\0';
}
