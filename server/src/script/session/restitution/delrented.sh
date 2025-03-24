#!/bin/bash

uid=$1; filmname=$2
num=$3; time=$4

while IFS=":" read name number current date; do
    if [[ $filmname == $name ]] && [[ $time = $date ]]; then
        (( diff = number - num ))
        if (( diff != 0 )); then
            sed -i s/"$name":$number:"$current":"$date"/"$name":$diff:"$current":"$date"/ database/rented$uid
        else
            sed -i /"$name":$number:$current:$date/d database/rented$uid
        fi
        break
    fi
done < database/rented$uid

if (( diff == 0 )); then
    line=$( cat database/rented$uid | wc -l )

    if (( line == 0 )); then
        rm database/rented$uid
        if [[ -e database/rented$uid.gz ]]; then
            rm database/rented$uid.gz
        fi
    fi
fi
