#include <stdio.h>      // Qui è definita la primitiva perror
#include <sys/socket.h> // Qui è definita la struttura sockaddr e le primitive socket, bind
                        // listen, accept e send
#include <netinet/in.h> // Qui è definita la struttura sockaddr_in e sono incluse anche le primitive
                        // htons, htonl
#include <string.h>
#include <stdlib.h>     // Qui è definita la primitiva exit
#include <unistd.h>     // Qui sono definite le primitive read e close

#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>

#define domain sin_family
#define port sin_port
#define ip sin_addr.s_addr

int lock = 1; // Variabile condivisa che assume la funzione di lucchetto
              // per l'accesso alla risorsa condivisa sda ( descrittore di file ).

sem_t semaphore;
int sum = 0;

void *runner( void *sda );
void wait( int *lock );
void signal( int *lock );

int request( char *buffer, int *type, int *action, char **body );
// char *processing( int result, int type, int action );
void response( int result, char *message, int *sdb );
// char *approve( int type, int action, char *message );
char *split( char *buffer, int *type, int *action );
void extract( char *body, char *username, char *password );

int authentication( int action, char *body );
void signup( char *body );
void signin( char *body );
int release( int action, char *body );
void cancel( char *body );
void search( char *body );
int find( char *username, char *password );

enum Type_request { AUTHENTICATION, SESSION, RELEASE, SEARCH };
enum Release_action { LOGOUT, CANCEL };
enum Authentication_action { SIGNIN, SIGNUP };

int main( void ) {

    struct sockaddr_in client, server;
    int address, listener, sda;
    pthread_t tid;

    listener = socket( AF_INET, SOCK_STREAM, 0 );

    if( listener < 0 ) {
        perror( "creazione della socket fallita" );
        exit( 1 );
    }

    memset( &server, 0, sizeof( server ) );
    memset( &client, 0, sizeof( client ) );

    server.domain = AF_INET;
    server.port = htons( 8080 );
    server.ip = htonl( INADDR_ANY );

    if( bind( listener, ( const struct sockaddr * )&server , sizeof( server ) ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva bind" );
        exit( 1 );
    }

    if( listen( listener, 3 ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva listen" );
        exit( 1 );
    }

    sem_init( &semaphore, 0, 1 );

    while( 1 ) {

        puts( "Server in ascolto sulla porta 8080..." );
        address = sizeof( client );

        wait( &lock ); // Il thread padre esegue la primitiva accept solo dopo che il
                       // il thread figlio ha eseguito signal( &lock ): dopo che ha
                       // duplicato il descrittore di file sda

        if( ( sda = accept( listener, ( struct sockaddr * )&client, ( socklen_t *)&address ) ) < 0 ) {
            perror( "Errore ricevuto dalla primitiva accept" );
            exit( 1 );
        }
        pthread_create( &tid, NULL, runner, &sda );
    }

    return 0;
}

void *runner( void *sda ) {

    int sdb = dup( *( int * )sda );
    signal( &lock ); // Il thread figlio sblocca il lucchetto e permette al thread
                     // padre di utilizzare sda per la creazione di una nuova socket
    char *body = NULL, buffer[ 1024 ];
    int result, type, action, res;

    while( 1 ) {

        res = read( sdb, buffer, sizeof( buffer ) - 1 );

        if( res < 0 ) {
            perror( "Errore ricevuto dalla primitiva read" );
            pthread_exit( ( void * )1 );
        }
        else if( res == 0 ) {
            // QUando viene restituito un valore uguale a 0 significa EOF
            // ( end-of-file ) ovvero indica che il punto terminale posto
            // all'altro estremo della connessione ha voluto chiudere la
            // connessione e quindi il file descriptor non potrà più
            // essere utilizzato per leggere dati.
            break;
        }

        buffer[ res ] = '\0';
        printf( "%s\n", buffer );

        sem_wait( &semaphore );
        /* Sezione d'ingresso */
        sum = sum + 1;  /* Sezione critica */
        /* Sezione di uscita */
        sem_post( &semaphore );

        printf( "Client: %d\n", sdb );

        result = request( buffer, &type, &action, &body );
        response( result, body, &sdb );

        if ( result == RELEASE )
            break;
    }

    close( sdb );

    puts( "Connessione terminata" );
    pthread_exit( 0 );
}

void wait( int *lock ) {
    while( !( *lock ) );
    *lock = *lock - 1;
}

void signal( int *lock ) {
    *lock = *lock + 1;
}

int request( char *buffer, int *type, int *action, char **body ) {

    int result;
    *body = split( buffer, type, action );

    printf( "Tipo di richiesta: %d%d\n", *type, *action );

    switch( *type ) {
        case AUTHENTICATION:
            result = authentication( *action, *body );
            break;
        case SESSION:
            result = 1;
            break;
        case RELEASE:
            result = release( *action, *body );
            break;
        case SEARCH:
            search( *body );
            result = 3;
            break;
        default:
            result = -1;
            break;
    }

    return result;
}


void response( int result, char *body, int *sdb ) {

    char message[ 1024 ];
    // Il server non ha compreso la richiesta del client e, in risposta, invia
    // un codice corrispondende a tale evento.
    if ( result < 0 )
        strcpy( message, "404 Bad Request " );
    // Il server ha compreso e accettato con successo la richiesta del client
    // e, in risposta, invia una conferma dell'avvenuta comprensione.
    else
        strcpy( message, "200 OK: " );

    strcat( message, body );
    message[ strlen( message ) + 1 ] = '\0';

    if( write( *sdb, ( const char * )message, strlen( message ) ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva write" );
        pthread_exit( ( void * )1 );
    }
}

char *split( char *buffer, int *type, int *action ) {

    unsigned int code[ 2 ];

    for( int i = 0; isdigit( *buffer ); buffer++, i++ )
        code[ i ] = ( int )*buffer - 48;

    *type = code[ 0 ];
    *action = code[ 1 ];

    return buffer; // Tale variabile contiene l'indirizzo di memoria
                   // del buffer, dove è memorizzato il messaggio,
                   // aumentato di due indirizzi. Ciò corrisponde
                   // al corpo del messaggio
}

void extract( char *body, char *username, char *password ) {

    for( ; *body != ' '; body++, username++ )
        *username = *body;
    *username = '\0';

    body++;
    for( ; *body != '\0'; body++, password++ )
        *password = *body;
    *password = '\0';
}

int authentication( int action, char *body ) {

    int result;

    switch( action ) {
        case SIGNIN:
            signin( body );
            result = 0;
            break;
        case SIGNUP:
            signup( body );
            result = 0;
            break;
        default:
            return -1;
            break;
    }

    return result;
}

void signin( char *body ) {

    char username[ 20 ], password[ 20 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    if( find( username, password ) )
        strcpy( body, "Accesso consentito!" );
    else
        strcpy( body, "Accesso negato!" );
}

void signup( char *body ) {

    char username[ 20 ], password[ 20 ], command[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    if ( find( username, password ) )
        strcpy( body, "Account esistente!" );
    else {
        snprintf( command, sizeof( command ), "echo \"%s %s\" >> accounts.dat",
              username, password );

        system( command );
        strcpy( body, "Account creato!" );
    }
}

int release( int action, char *body ) {

    int result;

    switch( action ) {
        case LOGOUT:
            result = 2;
            break;
        case CANCEL:
            cancel( body );
            result = 2;
            break;
        default:
            result = -1;
            break;
    }

    return result;
}

void cancel( char *body ) {

    char username[ 20 ], password[ 20 ], command[ 100 ];
    // Estrazione dell'username e della password dal corpo del messaggio
    extract( body, username, password );

    snprintf( command, sizeof( command ), "./cancel.sh \"%s\" \"%s\"",
              username, password );

    if( system( command ) )
        strcpy( body, "Account cancellato!" );
    else
        strcpy( body,"Cancellazione account fallita!" );
}


void search( char *body ) {

    char name[ 40 ], command[ 100 ];
    strcpy( name, body );

    snprintf( command, sizeof( command ), "./search.sh \"%s\"", name );

    if ( system( command ) )
        strcpy( body, "Film trovato" );
    else
        strcpy( body, "Film non trovato" );
}

int find( char *username, char *password ) {

    char command[ 100 ];

    snprintf( command, sizeof( command ), "./find.sh \"%s\" \"%s\"",
              username, password );
    return system( command );
}
