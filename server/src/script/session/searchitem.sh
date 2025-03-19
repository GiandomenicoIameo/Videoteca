#!/bin/bash

uid=$1
filmname=$2
number=$3

IFS=":"
while read film num available; do

    if [[ $filmname == $film ]]; then
        if (( $number >= 0 )) && (( $number <= $num )); then
            exit 0
        else
            exit 1
        fi
    fi

done < database/cart$uid

exit 2
