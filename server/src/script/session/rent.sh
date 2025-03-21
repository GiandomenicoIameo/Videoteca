#!/bin/bash

filmname=$1; number=$2

while IFS=":" read name ramount; do
    if [[ $filmname == $name ]]; then
        (( diff = ramount - number ))
        sed -i s/"$name":$ramount/"$name":$diff/ database/movies.dat
        break
    fi
done < database/movies.dat

