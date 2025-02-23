#!/bin/bash

declare -i sentinel=0

IFS='\n'
while read film; do
    if [[ $1 =~ $film ]]; then
        sentinel=1; break
    fi
done < database.dat

if (( $sentinel == 1 )); then
    echo "Film trovato"
else
    echo "Film non trovato"
fi
