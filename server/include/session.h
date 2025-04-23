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

void returntest( int uid, char *body );

void checkout( int uid, char *body );
void checklim( int uid, char *body );
