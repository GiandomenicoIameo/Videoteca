#!/bin/bash

# Il seguente script effettua una ricerca all'interno del file
# signed.dat e verifica se le credenziali specificate dal
# client sono associate a un account esistente.

username=$1
password=$2

IFS=":"

while read user pass; do
    if [[ $username == $user ]] && [[ $password == $pass ]]; then
        exit 0
    fi
done < database/signed.dat

exit 1
