#!/bin/bash

# Il seguente script effettua una ricerca all'interno del file
# signed.dat ( di tutti gli utenti registrati presso il server )
# in base al nome utente e alla password. Lo script restituisce
# un exit code pari a 0 se l'username e la password corrispondono
# a un account attualmente registrato presso il server e un exit
# code pari a 1 in caso contrario

username=$1
password=$2
filename=$3

if [[ $filename == "signed.dat" ]]; then
    while read user passwd; do
        if [[ $username == $user ]] && [[ $password == $passwd ]]; then
            exit 0
        fi
    done < $filename
elif [[ $filename == "connessi.dat" ]]; then
    while read sdb user passwd; do
        if [[ $username == $user ]] && [[ $password == $passwd ]]; then
            exit 0
        fi
    done < $filename
fi

exit 1
