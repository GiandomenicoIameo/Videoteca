#!/bin/bash

# Il seguente script effettua una ricerca
# all'interno del file connessi.dat e
# verifica se l'utente aveva effettuato
# in un precedente momento la procedura
# di signin.

ids=$1

while IFS=":" read id user pass sid; do
    if [[ $ids -eq $sid ]]; then
        exit 1
    fi
done < database/signed.dat

exit 0


