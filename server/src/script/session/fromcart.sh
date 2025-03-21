#!/bin/bash

uid=$1; filmname=$2
number=$3

while IFS=":" read film eamount ramount date; do
    if [[ $filmname == $film ]]; then
        (( namount = eamount - number ))
        if (( namount != 0 )); then
            sed -i s/"$film":$eamount:$ramount/"$film":$namount:$ramount/ database/cart$uid
        else
            sed -i /"$film":$eamount:$ramount/d database/cart$uid
        fi
        break
    fi
done < database/cart$uid

if (( namount == 0 )); then
    line=$( cat database/cart$uid | wc -l )

    if (( line == 0 )); then
        rm database/cart$uid
    fi
fi
