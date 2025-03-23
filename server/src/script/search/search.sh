#!/bin/bash

# Il seguente script effettua una ricerca all'interno del
# file movies.dat ( di tutti i film ). Lo script restituisce
# un exit code pari a 0 se lo ha trovato e un exit code
# pari a 1 in caso contrario.

filmname=$1

while IFS=":" read name available rentable; do
    if [[ $filmname == $name ]]; then
        exit 1
    fi
done < database/movies.dat

exit 0
