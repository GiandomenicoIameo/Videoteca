#!/bin/bash

filmname=$1
number=$2

declare -i line=1
declare -i status=0

IFS=":"
while read film num; do

    if [[ $filmname == $film ]]; then
        (( diff = num - number ))
        sed -i ${line}d database/movies.dat
        status=1; break
    fi

    (( line = line + 1 ))
done < database/movies.dat

if (( status == 1 )); then
    echo "$filmname:$diff" >> database/movies.dat
fi
