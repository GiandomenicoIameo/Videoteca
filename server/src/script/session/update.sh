#!/bin/bash

filmname=$1
helpful=$2

for cart in $( ls database/cart* ); do
    line=1
    while IFS=":" read name num available; do
        if [[ $filmname == $name ]]; then
            sed -i ${line}d $cart
            echo "$filmname":$num:$helpful >> $cart
            break
        fi
        (( line = line + 1 ))
    done < $cart
done
