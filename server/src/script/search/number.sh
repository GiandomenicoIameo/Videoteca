#!/bin/bash

filmname=$1

while IFS=":" read name available; do
    if [[ $filmname == $name ]]; then
        exit $available
    fi
done < database/movies.dat

