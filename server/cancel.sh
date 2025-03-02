#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file signed.dat ( di tutti gli utenti registrati presso il server )
# in base all'username e alla password. Lo script elimina l'account
# associato all'username e alla password passati come argomenti allo script.
# Lo script restituisce un exit code pari a 1 se l'account è stato trovato
# e un exit code pari a 0 in caso contrario.

username=$1
password=$2

counter=1

while read user passwd; do
    if [[ $username == $user ]] && [[ $password == $passwd ]]; then
        sed -i ${counter}d signed.dat
        exit 1
    fi
    (( counter = counter + 1 ))
done < signed.dat

exit 0

