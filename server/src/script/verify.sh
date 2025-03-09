#!/bin/bash

# Durante la fase di cancellazione dell'account, l'utente
# è chiamato a verificare la sua identità. Se l'username
# e la password sono associati all'identificativo della
# connesione, scelto dal server al momento del login, allora
# l'utente può procedere alla cancellazione dell'account.

sda=$1
username=$2
password=$3

while read sd user pass; do
    if [[ $sda == $sd ]] && [[ $username == $user ]] && [[ $password == $pass ]]; then
        exit 0
    fi
done < database/connessi.dat

exit 1
