#!/bin/bash

sda=$1
filmname=$2

if ! [[ -e database/cart$sda ]]; then
    touch database/cart$sda
fi

IFS=":"
while read film number; do
    if [[ $film == $filmname ]]; then
        exit 1
    fi
done < database/cart$sda

echo $filmname >> database/cart$sda
exit 0
