#!/bin/bash

# Il seguente script effettua una ricerca
# all'interno del file connessi.dat e
# verifica se l'utente aveva effettuato
# in un precedente momento la procedura
# di signin.

ids=$1

while IFS=":" read id user pass; do
    if [[ $ids -eq $id ]]; then
        exit 0
    fi
done < database/connessi.dat

exit 1


