#!/bin/bash

uid=$1; filmname=$2
number=$3

while IFS=":" read film num available; do
    if [[ $filmname == $film ]]; then
        (( diff = num - number ))
        if (( diff != 0 )); then
            sed -i s/"$film":$num:$available/"$film":$diff:$available/ database/cart$uid
        else
            sed -i /"$film":$num:$available/d database/cart$uid
        fi
        break
    fi
done < database/cart$uid

if (( diff == 0 )); then
    line=$( cat database/cart$uid | wc -l )

    if (( line == 0 )); then
        rm database/cart$uid
    fi
fi
