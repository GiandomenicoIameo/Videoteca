#!/bin/bash

username=$1
password=$2
status=0

while read user passwd; do
    if [[ $username == $user ]] && [[ $password == $passwd ]]; then
        status=1; break
    fi
done < signed.dat

exit $status
