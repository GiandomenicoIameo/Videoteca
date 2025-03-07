#!/bin/bash

client=$1
filmname=$2

if ! [[ -e cart_$client ]]; then
    touch cart_$client
fi

IFS=":"
while read film number; do
    if [[ $film == $filmname ]]; then
        exit 1
    fi
done < cart_$client

echo $filmname >> cart_$client
exit 0
