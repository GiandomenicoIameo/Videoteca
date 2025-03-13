#!/bin/bash

sda=$1
filmname=$2

declare -i counter=1
declare -i status=0

IFS=":"
while read film number; do
    if [[ $filmname == $film ]]; then
        sed -i ${counter}d database/cart$sda
        status=1
    fi
    (( counter = counter + 1 ))
done < database/cart$sda

if (( status == 1 )); then
    line=$( cat database/cart$sda | wc -l )

    if (( line == 0 )); then
        rm database/cart$sda
    fi
    exit 0
fi

exit 1
