#!/bin/bash

# Il seguente script effettua una ricerca all'interno del file
# signed.dat ( di tutti gli utenti registrati presso il server )
# in base al nome utente e alla password. Lo script restituisce
# un exit code pari a 1 se l'username e la password corrispondo
# a un account attualmente registrato presso il server e un exit
# code pari a 0 in caso contrario

username=$1
password=$2

declare -i status=0

while read user passwd; do
    if [[ $username == $user ]] && [[ $password == $passwd ]]; then
        status=1; break
    fi
done < signed.dat

exit $status
