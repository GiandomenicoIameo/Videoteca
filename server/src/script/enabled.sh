#!/bin/bash

# Lo script effettua una ricerca all'interno del file
# connessi.dat e verifica se l'username dell'utente,
# che sta effettuando la procedura di signin, è
# associato a un account correntemente connesso
# presso il server.

username=$1

while IFS=":" read ids user pass; do
    if [[ $username == $user ]]; then
        exit 0
    fi
done < database/connessi.dat

exit 1
