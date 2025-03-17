#!/bin/bash

# Lo script effettua una ricerca all'interno del file
# signed.dat e verifica se l'username dell'utente
# compare nel file signed.dat. In caso negativo,
# l'utente acquista il diritto di accedere all'account,
# altrimenti gli viene negato.

username=$1
filename=$2

while IFS=":" read user pass ids; do
    if [[ $username == $user ]]; then
        exit 0
    fi
done < database/signed.dat

exit 1
