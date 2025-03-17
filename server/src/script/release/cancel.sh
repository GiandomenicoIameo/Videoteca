#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file signed.dat ( di tutti gli utenti registrati presso il server )
# in base all'username e alla password. Lo script elimina l'account
# associato all'username e alla password passati come argomenti allo script.
# Di conseguenza vengono rimosse le righe corrispondenti all'username e
# alla password nel file connessi.dat e il carrello dell'utente.

ids=$1

declare -i line=1
declare -i status=0

while IFS=":" read user passwd id; do
    if [[ $ids -eq $id ]]; then
        sed -i ${line}d database/signed.dat
    fi
    (( line = line + 1 ))
done < database/signed.dat
