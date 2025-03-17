#!/bin/bash

ids=$1
declare -i line=1

while IFS=":" read user pass id; do
    if [[ $ids -eq $id ]]; then
        sed -i ${line}d database/signed.dat
        echo "$user":"$pass":0 >> database/signed.dat
    fi
    (( line = line + 1 ))
done < database/signed.dat
