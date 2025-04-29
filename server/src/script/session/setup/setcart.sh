#!/bin/bash

uid=$1; filmname=$2
number=$3; date=$4

if ! [[ -e database/cart$uid ]]; then
    if (( number != 0 )); then
        touch database/cart$uid
        echo "$filmname":$number:"$date" >> database/cart$uid
    fi
else
    if grep -q ^"$filmname:.*:$date"$ database/cart$uid; then
        if (( number == 0 )); then
            sed -i /^"$filmname"':.*:'"$date"$/d database/cart$uid
        else
            sed -i s/"$filmname":'.*':"$date"/"$filmname":$number:"$date"/ database/cart$uid
        fi
    else
        if (( number != 0 )); then
            echo "$filmname":$number:"$date" >> database/cart$uid
        fi
    fi

    line=$( cat database/cart$uid | wc -l )

    if (( line == 0 )); then
        rm database/cart$uid
        if [[ -e database/cart$uid.gz ]]; then
            rm database/cart$uid.gz
        fi
    fi
fi
