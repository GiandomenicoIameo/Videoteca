#!/bin/bash

# Il seguente script effettua una ricerca all'interno
# del file signed.dat ( di tutti gli utenti registrati presso il server )
# in base all'username e alla password. Lo script elimina l'account
# associato all'username e alla password passati come argomenti allo script.
# Di conseguenza vengono rimosse le righe corrispondenti all'username e
# alla password nel file connessi.dat e il carrello dell'utente.

ids=$1
sed -i /.'*':$ids/d database/signed.dat
