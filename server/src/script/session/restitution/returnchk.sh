#!/bin/bash

uid=$1; filmname=$2; number=$3
time=$4

if ! [[ -e database/rented$uid ]]; then
    exit 1
fi

declare -i status=3

while IFS=":" read name num current date; do
    if [[ $filmname == $name ]]; then
        if [[ $time == $date ]]; then
            if (( $number >= 0 )) && (( $number <= $num )); then
                status=0; break
            else
                status=1; break
            fi
        else
            status=2
        fi
    fi
done < database/rented$uid

exit $status


