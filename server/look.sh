#!/bin/bash

sda=$1

while read sd user pass; do
    if [[ $sda == $sd ]]; then
        exit 1
    fi
done < connessi.dat

exit 0
