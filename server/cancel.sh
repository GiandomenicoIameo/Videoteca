#!/bin/bash

username=$1
password=$2

counter=1

while read user passwd; do
    if [[ $username == $user ]] && [[ $password == $passwd ]]; then
        sed -i ${counter}d signed.dat
        exit 1
    fi
    (( counter = counter + 1 ))
done < signed.dat

exit 0

