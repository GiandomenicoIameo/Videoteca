#!/bin/bash

descript=$1
username=$2
password=$3

echo "$username" "$password" >> database/signed.dat
echo $descript "$username" "$password" >> database/connessi.dat
