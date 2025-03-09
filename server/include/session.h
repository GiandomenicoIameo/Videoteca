#include "metadata.h"

#define SESSION 1

int session( int sdb, int action, char *body );

void tocart( int sdb, char *body );
void fromcart( int sdb, char *body );

void takeout( char *body, char *filmname, char *number, char *date );
unsigned int checkmovie( char *filmname, char *number, char *date );
void rented( int sdb, char *body );
void returned( int sdb, char *body );
