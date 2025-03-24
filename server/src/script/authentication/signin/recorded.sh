#!/bin/bash

# Il seguente script effettua una ricerca all'interno del file
# signed.dat e verifica se le credenziali specificate dal
# client sono associate a un account esistente.

username=$1
password=$2

while IFS=":" read uid user pass ids; do
    if [[ $username == $user ]] && [[ $password == $pass ]] && [[ $ids -eq 0 ]]; then
        exit 1
    fi
done < database/signed.dat

exit 0
