#include "metadata.h"

#define SESSION 1

int session( int sdb, int action, char *body );

// void preadd( int uid, char *body );
// void predel( int uid, char *body );

void setout( int uid, char *body );

void takeout( char *body, char *filmname, char *number, char *date );
unsigned int rentalchk( char *filmname, char *number, char *date );
void rentest( int uid, char *body );

void showcart( int uid, char *body );
void showrented( int uid, char *body );

void checkret( int uid, char *body );

void checkout( int uid, char *body );
//void checksum( int uid, char *body );

void atomic( semaphore mutex, int uid, char *body, void ( *function )( int uid, char *body ) );
