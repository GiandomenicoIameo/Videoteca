#!/bin/bash

descriptor=$1
username=$2
password=$3

line=1

while read sd user pass; do
    if [[ $descriptor == $sd ]] && [[ $username == $user ]] &&\
       [[ $password == $pass ]]; then
        sed -i ${line}d connessi.dat
        break
    fi
    (( line = line + 1 ))
done < connessi.dat



