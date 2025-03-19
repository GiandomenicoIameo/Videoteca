#!/bin/bash

uid=$1; filmname=$2
number=$3; date=$4

if ! [[ -e database/rented$uid ]]; then
    touch database/rented$uid
    echo "$filmname":$number:$date >> database/rented$uid
else
    declare -i status=0
    while IFS=":" read name num time; do
        if [[ $filmname == $name ]] && [[ $date == $time ]]; then
            (( sum = num + number ))
            sed -i s/"$name":$num:$time/"$name":$sum:$time/ database/rented$uid
            status=1; break
        fi
    done < database/rented$uid

    if (( status == 0 )); then
        echo "$filmname":$number:$date >> database/rented$uid
    fi
fi
