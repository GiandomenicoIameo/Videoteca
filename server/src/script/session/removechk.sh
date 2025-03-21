#!/bin/bash

uid=$1; filmname=$2
number=$3; time=$4

while IFS=":" read name eamount ramount date; do

    if [[ "$filmname" == "$name" ]]; then
        if [[ "$date" == "$time" ]]; then
            if (( number >= 0 )) && (( number <= ramount )); then
                exit 0
            else
                exit 1
            fi
        else
            exit 2
        fi
    fi

done < database/cart$uid

exit 3
