#!/bin/bash

sda=$1
line=1

while read sd user pass; do
    if [[ $sda == $sd ]]; then
        sed -i ${line}d connessi.dat
        exit 1
    fi
    (( line = line + 1 ))
done < connessi.dat

exit 0
