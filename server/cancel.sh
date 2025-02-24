#!/bin/bash

username=$1
password=$2

status=0
counter=1

while read user passwd; do
    if [ $username == $user ] && [ $password == $passwd ]; then
        status=1; break
    fi
    (( counter = counter + 1 ))
done < accounts.dat

if [ $status == 1 ]; then
    sed -i "${counter}d" accounts.dat
    exit 1
fi
exit 0

