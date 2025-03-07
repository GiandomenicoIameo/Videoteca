#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS 5

int sum = 0; // Variabile condivisa
sem_t semaphore; // Semaforo

void *runner( void *num );
void riempimento( int *array, unsigned int dim );

int main( void ) {

    srand( time( NULL ) );

    pthread_t array[ NUM_THREADS ];
    int values[ NUM_THREADS ];

    riempimento( values, NUM_THREADS );
    sem_init( &semaphore, 0, 1 ); // Il semaforo viene creato dal primo thread

    for( int i = 0; i < NUM_THREADS; i++ )
        pthread_create( &array[ i ], NULL, runner, &values[ i ] );

    printf( "La somma è %d\n", sum );

    return 0;
}

void *runner( void *num ) {

    sem_wait( &semaphore );

    /* Sezione d'ingresso */

    sum = sum + *( int * )num; // Sezione critica

    /* Sezione d'uscita */

    sem_post( &semaphore );
    pthread_exit( 0 );
}

void riempimento( int *array, unsigned int dim ) {

    for ( int i = 0; i < dim; i++ )
        array[ i ] = 1 + ( rand() % 20 );
}
