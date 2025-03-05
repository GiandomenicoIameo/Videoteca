#!/bin/bash

filmname=$1
number=$2

while read line; do
    num=$( echo $line | awk -F : '{ print $2 }' )
    film=$( echo $line | awk -F : '{ print $1 }' )

    if [[ $filmname == $film ]]; then
        if (( $number <= $num )); then
            exit 0
        else
            exit 1
        fi
    fi

done < movies.dat

exit 2
