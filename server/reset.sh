#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file connessi.dat in base all'identificativo
# associato alla connessione, all'username e alla password.

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
