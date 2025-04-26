#include "authentication.h"
#include "session.h"
#include "release.h"
#include "view.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h> // Qui è definita la struttura sockaddr e le primitive socket, bind
                        // listen, accept e send
#include <netinet/in.h> // Qui è definita la struttura sockaddr_in e sono incluse anche le primitive
                        // htons, htonl
#include <unistd.h>     // Qui sono definite le primitive read e close
#include <pthread.h>

#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>

#include <signal.h>

#define domain sin_family
#define port sin_port
#define ip sin_addr.s_addr

// Variabile condivisa che assume la funzione di lucchetto
// per l'accesso alla risorsa condivisa sda ( descrittore di file ).

int lock = 1;
pthread_attr_t attributes;

void *runner( void *sda );
void wait( int *lock );
void post( int *lock );

int request( int sdb, char *buffer, int *type, int *action, char **body );
void response( int result, char *message, int *sdb );
char *split( char *buffer, int *type, int *action );

void handler( int sign );

int main( void ) {

    struct sockaddr_in client, server;
    int address, listener, sda;

    pthread_t tid;

    // La creazione di una socket richiede di specificare tre
    // paramentri: dominio, tipo di comunicazione ( orientata alla connessione oppure
    // non orientata alla connessione ) e protocollo del livello di trasporto.
    listener = socket( AF_INET, SOCK_STREAM, 0 );
    // La socker creata è riferita dal descrittore di file listener.

    if( listener < 0 ) {
            perror( "creazione della socket fallita" );
            exit( EXIT_FAILURE );
    }

    // Le strutture destinate a memorizzare, rispettivamente, l'indirizzo della socket
    // del server e della socket del client vengono azzerate.
    memset( &server, 0, sizeof( server ) );
    memset( &client, 0, sizeof( client ) );

    // Le successive tre istruzioni definiscono l'indirizzo della socket.
    server.domain = AF_INET;
    // Prima di trasmettere i dati sulla rete, l'indirizzo IP e la porta
    // devono essere convertiti. Il server deve convertire sia l'indirizzo IP che
    // il numero di porta dal formato interno della macchina al formato di rete standard.
    // La rappresentazione dei dati contenuti delle intestazioni dei pacchetti di rete
    // è big-endian.

    // htons trasforma un intero senza segno di 16 bit nel formato standard di rete.
    // htonl trasforma un intero senza segno di 32 bit nel formato standard di rete.
    server.port   = htons( 8080 );
    server.ip     = htonl( INADDR_ANY );

    // La socket viene legata all'indirizzo definito nelle tre istruzioni precedenti.
    if ( bind( listener, ( const struct sockaddr * )&server , sizeof( server ) ) < 0 ) {
            perror( "Errore ricevuto dalla primitiva bind()" );
            exit( EXIT_FAILURE );
    }

    // Qui avviene quella fase denominata: Wait-for-connection e viene
    // effettuata dalle funzioni listen() e accecpt().

    // Il parametro backlog che in questo caso è pari a 3 specifica che
    // il sistema operativo è in grado di mantenere fino a 3 connessioni nella
    // coda di attesa prima che venga rifiutata una nuova connessione.
    if ( listen( listener, 3 ) < 0 ) {
            perror( "Errore ricevuto dalla primitiva listen()" );
            exit( EXIT_FAILURE );
    }

    while( 1 ) {

            signal( SIGINT, handler );
            signal( SIGTERM, handler );

            puts( "Server in ascolto sulla porta 8080..." );

            // Il thread padre esegue la primitiva accept solo dopo che il
            // il thread figlio ha eseguito signal( &lock ): dopo che ha
            // duplicato il descrittore di file sda.

            wait( &lock );

            // La funzione accept() estrae la prima richiesta di connessione e blocca
            // l'esecuzione del programma. La richiesta viene prelevata dalla coda delle
            // richieste di connessioni pendenti.
            // Una volta che la connessione è stata accettata, viene creata una nuova socket,
            // riferita dal descrittore sda, dedicata alla connessione con il client specifico.
            address = sizeof( client );
            if( ( sda = accept( listener,
            // client sarebbe la struttura che contiene l'indirizzo della socket del client. Tale
            // struttura qui viene riempita con l'indirizzo della socket del client.
                                ( struct sockaddr * )&client,
                                ( socklen_t *)&address ) ) < 0 ) {
                    perror( "Errore ricevuto dalla primitiva accept()" );
                    exit( EXIT_FAILURE );
            }

            // Prima della creazione del thread figlio, il thread principale chiama pthread_attr_setdetachehstate()
            // per configurare gli attributi di thread figlio in modo che venga creato in uno specifico stato, ovvero
            // in stato distaccato. Ciò significa che il thread principale non è più
            // responsabile della gestione del thread creato. Questo diventa indipendente e
            // quando termina, le sue risorse vengono automaticamente liberate dal sistema.
            // Al thread principale non interessa il valore di ritorno dei thread figli.

            if ( pthread_attr_setdetachstate( &attributes, PTHREAD_CREATE_DETACHED ) ) {
                    perror( "pthread_attr_setdetachstate()" ); exit( EXIT_FAILURE );
            } pthread_create( &tid, &attributes, runner, &sda );

            // La precedente chiamata crea un nuovo thread. Oltre all'ID del
            // thread e ai suoi attributi, viene passato il nome della funzione da cui il
            // nuovo thread inizierà l'esecuzione, in questo caso runner(), e l'indirizzo
            // della variabile che contiene il descrittore.

            // Qui non bisogna liberare la struttura di attributi del thread. Non è possibile
            // sapere in anticipo quanti thread verranno creati quindi non è sicuro liberarla se
            // poi nuovi thread si riferiranno ad essa.
    }
}

void handler( int sign ) {

    extern semaphore wrts;
    char command[ 100 ];

    snprintf( command, sizeof( command ),
              "sed -ri 's/(.*:).*/\\10/' database/signed.dat" );

    // oppure anche sed -ri 's/(.*:)./\\10/' database/signed.dat" );
    // Processo scrittore che accede al file signed.dat.
    writer( command, wrts );
    puts( "\nConnessione terminata!" );

    // La struttura pthread_attr_t viene distrutta (o liberata) poiché non
    // è più necessaria.
    pthread_attr_destroy( &attributes );
    exit( EXIT_SUCCESS );

    // Il processo principale termina normalmente e ogni thread creato
    // verrà terminato automaticatimente dal sistema operativo.
    // In modo analogo, l'interruzione del processo provocherà la chiusura
    // di tutte le socket aperte associate a quel processo e di quelle associate
    // ai vari thread.
}

void wait( int *lock ) {
    while( !( *lock ) ) {
            ;
    } *lock = *lock - 1;
}

void post( int *lock ) {
    *lock = *lock + 1;
}

void *runner( void *sda ) {

    int sdb = dup( *( int * )sda );
    post( &lock );

    // Il thread figlio sblocca il lucchetto e permette al thread
    // padre di utilizzare sda per la creazione di una nuova socket.

    char *body = NULL, buffer[ 1024 ], command[ 100 ];
    int result, type, action, res;

    extern semaphore wrts;

    while( 1 ) {
            res = read( sdb, buffer, sizeof( buffer ) - 1 );

            if( res <= 0 ) {
                    if ( res < 0 ) {
                            perror( "Errore ricevuto dalla primitiva read" );
                    } else {
                           // Quando viene restituito un valore uguale a 0 significa EOF
                           // ( end-of-file ) ovvero indica che il punto terminale posto
                           // all'altro estremo della connessione ha voluto chiudere la
                           // connessione e quindi il file descriptor non potrà più
                           // essere utilizzato per leggere dati.

                           // Se l'utente era loggato prima di chiudere la connessione,
                           // viene disconnesso.
                           if ( connected( sdb ) ) {
                                   snprintf( command, sizeof( command ),
                                          "sed -ri 's/(.*:)%d/\\10/' database/signed.dat", sdb );
                                   // Processo scrittore che accede al file signed.dat.
                                   writer( command, wrts );
                            }
                     }
                     break;
            }

            // La seguente istruzione di assegnazione è fondamentale
            // per le funzioni split() ed extract() in quanto consente
            // loro di capire il punto finale del corpo del messaggio.
            buffer[ res ] = '\0';
            printf( "Client: %d\n", sdb );

            // Le funzioni request() e response() usano il passaggio di parametri per riferimento
            // per poter essere in grado di "restituire" più valori alla loro funzione
            // chiamante modificando variabili nella funzione chiamante stessa.

            result = request( sdb, buffer, &type, &action, &body );
            response( result, body, &sdb );
    }

    close( sdb );
    puts( "Connessione terminata" );

    return NULL;
}

int request( int sdb, char *buffer, int *type, int *action, char **body ) {

    int result;
    *body = split( buffer, type, action );

    printf( "Tipo di richiesta: %d%d\n", *type, *action );

    switch( *type ) {
            // Messaggi di richiesta
            case AUTHENTICATION:
                    result = authentication( sdb, *action, *body );
                    break;
            case SESSION:
                    result = session( sdb, *action, *body );
                    break;
            case RELEASE:
                    result = release( sdb, *action, *body );
                    break;
            case VIEW:
                    showmovies( *body );
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
    if ( result < 0 ) {
            strcpy( message, "404 Bad Request:\n" );
    // Il server ha compreso e accettato con successo la richiesta del client
    // e, in risposta, invia una conferma dell'avvenuta comprensione.
    } else {
            strcpy( message, "200 OK:\n" );
    }
    strcat( message, body );
    message[ strlen( message ) + 1 ] = '\0';

    if( write( *sdb, ( const char * )message, strlen( message ) ) < 0 ) {
            perror( "Errore ricevuto dalla primitiva write" );
            pthread_exit( ( void * )1 );
    }
}

char *split( char *buffer, int *type, int *action ) {

    // Spacchetta il messaggio di richiesta.
    unsigned int code[ 2 ];

    for( int i = 0; isdigit( *buffer ); buffer++, i++ ) {
            code[ i ] = ( int )*buffer - 48;
    } *type = code[ 0 ]; *action = code[ 1 ];

    // La variabile variabile buffer contiene l'indirizzo di memoria
    // del buffer, dove è memorizzato il messaggio,
    // aumentato di due indirizzi. Ciò corrisponde
    // al corpo del messaggio.

    return buffer;
}
