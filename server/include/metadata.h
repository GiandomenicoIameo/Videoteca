#if !defined( METADATA_H )
    #define METADATA_H

    #include <pthread.h>

    void extract( char *body, char *username, char *password );
    int connected( int sdb );

    void writer( char *command, pthread_mutex_t mutex );
    unsigned char reader( char *command, pthread_mutex_t mutex,
                          pthread_mutex_t write, unsigned int readers );

#endif
