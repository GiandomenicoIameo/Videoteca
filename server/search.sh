#!/bin/bash

declare -i status=0

IFS='\n'
while read filmname; do
    if [[ $1 =~ $filmname ]]; then
        status=1; break
    fi
done < movies.dat

if (( $status == 1 )); then
    echo trovato
else
    echo non trovato
fi
