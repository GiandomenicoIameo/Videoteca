#!/bin/bash

filmname=$1; number=$2

while IFS=":" read name ramount total; do
    if [[ $filmname == $name ]]; then
        (( diff = ramount - number ))
        sed -i s/"$name":$ramount:$total/"$name":$diff:$total/ database/movies.dat
        break
    fi
done < database/movies.dat

