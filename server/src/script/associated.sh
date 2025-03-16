#!/bin/bash

username=$1
password=$2

IFS=":"
while read ids user pass; do
    if [[ $username == $user ]] && [[ $password == $pass ]]; then
        exit 0
    fi
done < database/connessi.dat

exit 1
