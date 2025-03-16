#!/bin/bash

ids=$1

declare -i line=1
while IFS=":" read id user pass; do
    if [[ $ids -eq $id ]]; then
        sed -i ${line}d database/connessi.dat
    fi
    (( line = line + 1 ))
done < database/connessi.dat
