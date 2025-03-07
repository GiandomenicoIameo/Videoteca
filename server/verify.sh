#!/bin/bash

sda=$1
username=$2
password=$3

while read sd user pass; do
    if [[ $sda == $sd ]] && [[ $username == $user ]] && [[ $password == $pass ]]; then
        exit 0
    fi
done < connessi.dat

exit 1
