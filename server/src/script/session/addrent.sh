#!/bin/bash

uid=$1; filmname=$2
number=$3; date=$4
current=$( date "+%Y-%m-%d" )


if ! [[ -e database/rented$uid ]]; then
    touch database/rented$uid
    echo "$filmname":$number:"$current":"$date" >> database/rented$uid
else
    declare -i status=0
    while IFS=":" read name num ctime time; do
        if [[ $filmname == $name ]] && [[ $date == $time ]]; then
            (( sum = num + number ))
            sed -i s/"$name":$num:"$ctime":"$time"/"$name":$sum:"$ctime":"$time"/ database/rented$uid
            status=1; break
        fi
    done < database/rented$uid

    if (( status == 0 )); then
        echo "$filmname":$number:"$current":"$date" >> database/rented$uid
    fi
fi
