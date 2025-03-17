#!/bin/bash

filmname=$1
number=$2

declare -i line=1

while IFS=":" read name num; do
    if [[ $filmname == $name ]]; then
        sed -i ${line}d database/movies.dat
        break
    fi
    (( line = line + 1 ))
done < database/movies.dat

(( diff = num - number ))
echo "$filmname:$diff" >> database/movies.dat
