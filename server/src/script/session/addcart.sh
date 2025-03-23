#!/bin/bash

uid=$1; filmname=$2
number=$3; real=$4; date=$5
current=$( date "+%Y-%m-%d" )

if ! [[ -e database/cart$uid ]]; then
    touch database/cart$uid
    echo "$filmname":$number:$real:"$current":"$date" >> database/cart$uid
else
    if grep -q ^"$filmname:.*:$date"$ database/cart$uid; then
        if (( number == 0 )); then
            sed -i /^"$filmname"':.*:'"$date"$/d database/cart$uid
        else
            sed -i s/"$filmname":'.*':$real:"$current":"$date"/"$filmname":$number:$real:"$current":"$date"/ database/cart$uid
        fi
    else
        echo "$filmname":$number:$real:"$current":"$date" >> database/cart$uid
    fi


    if (( number == 0 )); then
        line=$( cat database/cart$uid | wc -l )

        if (( line == 0 )); then
            rm database/cart$uid
            if [[ -e database/cart$uid.gz ]]; then
                rm database/cart$uid.gz
            fi
        fi
    fi

fi
