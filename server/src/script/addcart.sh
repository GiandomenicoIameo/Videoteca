#!/bin/bash

client=$1
filmname=$2

if ! [[ -e database/cart_$client ]]; then
    touch database/cart_$client
fi

IFS=":"
while read film number; do
    if [[ $film == $filmname ]]; then
        exit 1
    fi
done < database/cart_$client

echo $filmname >> database/cart_$client
exit 0
