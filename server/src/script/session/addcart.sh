#!/bin/bash

ids=$1
filmname=$2

number=$3
real=$4

if ! [[ -e database/cart$ids ]]; then
    touch database/cart$ids
fi

declare -i line=1

while IFS=":" read film num available; do
    if [[ $film == $filmname ]]; then
        sed -i ${line}d database/cart$ids
        break
    fi
done < database/cart$ids

echo "$filmname":$number:$real >> database/cart$ids
