#!/bin/bash

# Il seguente script verifica se la data inviata di
# restituzione di un film inviata dal client è corretta.
# La data di restituzione non può essere inferiore alla
# data attuale.

current=$( date +%s )
input=$( date -d $1 +%s )

if (( input < current )); then
    exit 1
else
    exit 0
fi
