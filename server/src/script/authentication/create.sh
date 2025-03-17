#!/bin/bash

ids=$1

username=$2
password=$3

echo "$username":"$password":$ids >> database/signed.dat
