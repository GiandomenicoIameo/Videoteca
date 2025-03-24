#!/bin/bash

filmname=$1
number=$2

while IFS=":" read film num rentable; do
    if [[ $filmname == $film ]]; then
          (( sum = number + num ))
          sed -i s/"$film":$num:$rentable/"$film":$sum:$rentable/ database/movies.dat
          break
    fi
done < database/movies.dat
