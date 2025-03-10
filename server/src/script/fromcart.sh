#!/bin/bash

client=$1
filmname=$2

declare -i counter=1

IFS=":"
while read film number; do
    if [[ $filmname == $film ]]; then
        sed -i ${counter}d database/cart_$client
        exit 0
    fi
    (( counter = counter + 1 ))
done < database/cart_$client

exit 1
