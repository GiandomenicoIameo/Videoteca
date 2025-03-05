#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file connessi.dat in base all'identificativo
# associato alla connessione. Lo script restituisce
# un exit code pari a 0 se l'utente è stato disconnesso
# e un exit code pari a 1 se l'utente non era connesso
# prima di effettuare la disconnessione.

sda=$1
line=1

while read sd user pass; do
    if [[ $sda == $sd ]]; then
        sed -i ${line}d connessi.dat
        exit 0
    fi
    (( line = line + 1 ))
done < connessi.dat

exit 1
