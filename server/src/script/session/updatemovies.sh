#!/bin/bash

filmname=$1
number=$2

while IFS=":" read film num; do
    if [[ $filmname == $film ]]; then
          (( sum = number + num ))
          sed -i s/"$film":$num/"$film":$sum/ database/movies.dat
          break
    fi
done < database/movies.dat
