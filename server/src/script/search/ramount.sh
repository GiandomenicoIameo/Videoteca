#!/bin/bash

filmname=$1

while IFS=":" read name ramount total; do
    if [[ $filmname == $name ]]; then
        exit $ramount
    fi
done < database/movies.dat

