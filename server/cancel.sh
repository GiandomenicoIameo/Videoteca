#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file signed.dat ( di tutti gli utenti registrati presso il server )
# in base all'username e alla password. Lo script elimina l'account
# associato all'username e alla password passati come argomenti allo script.

username=$1
password=$2

counter=1

while read user passwd; do
    if [[ $username == $user ]] && [[ $password == $passwd ]]; then
        sed -i ${counter}d signed.dat
        break
    fi
    (( counter = counter + 1 ))
done < signed.dat

