#!/bin/bash

line=$( cat database/signed.dat | wc -l )

IFS=":"
for(( i = 0; i < $line; i++ )); do
        read username password ids < database/signed.dat
        sed -i 1d database/signed.dat
        echo "$username":"$password":0 >> database/signed.dat
done
