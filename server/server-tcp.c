#include <stdio.h>      // Qui è definita la primitiva perror
#include <sys/socket.h> // Qui è definita la struttura sockaddr e le primitive socket, bind
                        // listen, accept e send
#include <netinet/in.h> // Qui è definita la struttura sockaddr_in e sono incluse anche le primitive
                        // htons, htonl
#include <string.h>
#include <stdlib.h>     // Qui è definita la primitiva exit
#include <unistd.h>     // Qui sono definite le primitive read e close

#include <string.h>

#define domain sin_family
#define port sin_port
#define ip sin_addr.s_addr

int main( void ) {

    struct sockaddr_in client, server;
    int address, sda, sdb;
    char buffer[ 1024 ] = { '\0' }, message_response[ 1024 ] = { '\0' };

    sda = socket( AF_INET, SOCK_STREAM, 0 );

    if( sda < 0 ) {
        perror( "creazione della socket fallita" );
        exit( 1 );
    }

    memset( &server, 0, sizeof( server ) );
    memset( &client, 0, sizeof( client ) );

    server.domain = AF_INET;
    server.port = htons( 8080 );
    server.ip = htonl( INADDR_ANY );

    if( bind( sda, ( const struct sockaddr * )&server , sizeof( server ) ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva bind" );
        exit( 1 );
    }

    if( listen( sda, 3 ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva listen" );
        exit( 1 );
    }

    puts( "Server in ascolto sulla porta 8080..." );
    address = sizeof( client );

    if( ( sdb = accept( sda, ( struct sockaddr * )&client, ( socklen_t *)&address ) ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva accept" );
        exit( 1 );
    }

    if( read( sdb, buffer, sizeof( buffer ) - 1 ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva read" );
        exit( 1 );
    }

    printf( "%s%s\n", "Messaggio ricevuto dal client: ", buffer );
    strcpy( message_response, "200 OK" );

    if( send( sdb, ( const char * )message_response, strlen( message_response ), 0 ) < 0 ) {
        perror( "Errore ricevuto dalla primitiva send" );
        exit( 1 );
    }

    puts( "Messaggio di risposta inviato!" );

    close( sdb );
    close( sda );

    puts( "Connessione terminata" );

    return 0;
}



