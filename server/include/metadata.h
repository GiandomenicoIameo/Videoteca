#if !defined( METADATA_H )
    #define METADATA_H

    #include <pthread.h>
    #define semaphore pthread_mutex_t

    void extract( char *body, char *username, char *password );
    int connected( int sdb );

    void writer( char *command, semaphore mutex );
    unsigned char reader( char *command, semaphore mutex,
                          semaphore write, unsigned int readers );

#endif
