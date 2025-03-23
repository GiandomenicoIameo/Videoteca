#!/bin/bash

filmname=$1; number=$2

while IFS=":" read name ramount rentable; do
    if [[ $filmname == $name ]]; then
        (( diff = ramount - number ))
        sed -i s/"$name":$ramount:$rentable/"$name":$diff:$rentable/ database/movies.dat
        break
    fi
done < database/movies.dat

