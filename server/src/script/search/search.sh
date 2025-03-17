#!/bin/bash

# Il seguente script effettua una ricerca all'interno del
# file movies.dat ( di tutti i film ). Lo script restituisce
# un exit code pari a 0 se lo ha trovato e un exit code
# pari a 1 in caso contrario.

IFS=":"
while read filmname available; do
    if [[ $1 == $filmname ]]; then
        exit 0
    fi
done < database/movies.dat

exit 1
