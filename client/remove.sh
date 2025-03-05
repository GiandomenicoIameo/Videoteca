#!/bin/bash

filmname=$1
number=$2
counter=1

IFS=":"
while read film num; do
    if [[ $filmname == $film ]]; then
        if (( $number == $num )); then
            sed -i ${counter}d carrello.dat
            echo "Rimosso"
            exit 0
        elif (( $number < $num )); then
            (( value = num - number ))
            sed -i 's/'$num'/'$value'/' carrello.dat
            echo "Rimosso"
            exit 0
        else
            echo "Quantità inesatta"
            exit 1
        fi
    fi
    (( counter = counter + 1 ))
done < carrello.dat

echo "Il film non esiste"
