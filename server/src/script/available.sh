#!/bin/bash

filmname=$1
number=$2

IFS=":"
while read film num; do

    if [[ $filmname == $film ]]; then
        if (( $number <= $num )); then
            exit 0
        else
            exit 1
        fi
    fi

done < database/movies.dat

exit 2
