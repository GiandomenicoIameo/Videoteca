#!/bin/bash

filmname=$1; number=$2
date=$3

while IFS=":" read film num rentable; do

    if [[ $filmname == $film ]]; then
        if (( $number >= 0 )) && (( $number <= $rentable )); then
            script/session/date.sh $date; exit $?
        else
            exit 2
        fi
    fi

done < database/movies.dat

exit 3
