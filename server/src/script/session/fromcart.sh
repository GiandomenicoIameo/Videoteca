#!/bin/bash

uid=$1; filmname=$2
number=$3

while IFS=":" read film eamount ramount current date; do
    if [[ $filmname == $film ]]; then
        (( namount = eamount - number ))
        if (( namount != 0 )); then
            sed -i s/"$film":$eamount:$ramount:"$current":"$date"/"$film":$namount:$ramount:"$current":"$date"/ database/cart$uid
        else
            sed -i /"$film":$eamount:$ramount:"$current":"$date"/d database/cart$uid
        fi
        break
    fi
done < database/cart$uid

if (( namount == 0 )); then
    line=$( cat database/cart$uid | wc -l )

    if (( line == 0 )); then
        rm database/cart$uid
        if [[ -e database/cart$uid.gz ]]; then
            rm database/cart$uid.gz
        fi
    fi
fi
