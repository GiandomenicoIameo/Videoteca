#!/bin/bash

ids=$1
filmname=$2
number=$3

declare -i counter=1
declare -i status=0

IFS=":"
while read film num available; do
    if [[ $filmname == $film ]]; then
        status=1
        sed -i ${counter}d database/cart$ids
        (( diff = num - number ))
        if (( diff != 0 )); then
            echo "$film":$diff:$available >> database/cart$ids
        fi
        break
    fi
    (( counter = counter + 1 ))
done < database/cart$ids

if (( status == 1 )); then
    line=$( cat database/cart$ids | wc -l )

    if (( line == 0 )); then
        rm database/cart$ids
    fi
fi
