#!/bin/bash

# Il seguente script effettua una ricerca all'interno del
# file movies.dat ( di tutti i film ). Lo script restituisce
# un exit code pari a 1 se lo ha trovato e un exit code
# pari a 0 in caso contrario.

while read filmname available; do
    if [[ $1 == $filmname ]]; then
        exit 1
    fi
done < movies.dat

exit 0
