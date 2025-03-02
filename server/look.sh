#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file connessi.dat ( di tutti gli utenti attualmente connessi )
# in base al codice identificativo della connessione. Lo script
# restituisce un exit code pari a 1 se l'account è connesso e un exit code
# pari a 0 in caso contrario.

sda=$1

while read sd user pass; do
    if [[ $sda == $sd ]]; then
        exit 1
    fi
done < connessi.dat

exit 0
