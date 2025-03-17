#!/bin/bash

filmname=$1
number=$2

declare -i line=1
declare -i status=0

IFS=":"
while read film num; do

    if [[ $filmname == $film ]]; then
      sed -i ${line}d database/movies.dat
      (( sum = number + num ))
      status=1; break
    fi

    (( line = line + 1 ))
done < database/movies.dat

if (( status == 1 )); then
    echo "$filmname:$sum" >> database/movies.dat
fi
