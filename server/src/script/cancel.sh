#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file signed.dat ( di tutti gli utenti registrati presso il server )
# in base all'username e alla password. Lo script elimina l'account
# associato all'username e alla password passati come argomenti allo script.
# Di conseguenza vengono rimosse le righe corrispondenti all'username e
# alla password nel file connessi.dat e il carrello dell'utente.

sda=$1
username=$2
password=$3

counter=1

while read user passwd; do
    if [[ $username == $user ]] && [[ $password == $passwd ]]; then
        sed -i ${counter}d database/signed.dat
        script/removecart.sh $sda
        exit 0
    fi
    (( counter = counter + 1 ))
done < database/signed.dat

exit 1
