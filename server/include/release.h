#include "metadata.h"

#define RELEASE 2
#define LOGOUT  0
#define CANCEL  1

int release( int sdb, int action, char *body );
void cancel( int sdb, char *body );
void signout( int sdb, char *body );
