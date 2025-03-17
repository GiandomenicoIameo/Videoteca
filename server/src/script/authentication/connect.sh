#!/bin/bash

ids=$1
username=$2
password=$3

declare -i line=1
while IFS=":" read user pass id; do
        if [[ $username == $user ]] && [[ $password == $pass ]]; then
                sed -i ${line}d database/signed.dat
                echo "$user":"$pass":$ids >> database/signed.dat
        fi
        (( line = line + 1 ))
done < database/signed.dat
