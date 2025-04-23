#!/bin/bash

filmname=$1
number=$2

while IFS=":" read film num total; do
    if [[ $filmname == $film ]]; then
          (( sum = number + num ))
          sed -i s/"$film":$num:$total/"$film":$sum:$total/ database/movies.dat
          break
    fi
done < database/movies.dat
