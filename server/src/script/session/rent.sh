#!/bin/bash

filmname=$1; number=$2

while IFS=":" read name num; do
    if [[ $filmname == $name ]]; then
        (( diff = num - number ))
        sed -i s/"$name":$num/"$name":$diff/ database/movies.dat
        break
    fi
done < database/movies.dat

