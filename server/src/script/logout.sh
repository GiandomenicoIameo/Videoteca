#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file connessi.dat in base all'identificativo
# associato alla connessione.

sda=$1
line=1

while read sd user pass; do
    if [[ $sda == $sd ]]; then
        sed -i ${line}d database/connessi.dat
        exit 0
    fi
    (( line = line + 1 ))
done < database/connessi.dat

exit 1
