#include "metadata.h"

#define AUTHENTICATION 0
#define SIGNIN 0
#define SIGNUP 1

int authentication( int sdb, int action, char *body );
void signin( int sdb, char *body );
void signup( int sdb, char *body );
void update( void );

