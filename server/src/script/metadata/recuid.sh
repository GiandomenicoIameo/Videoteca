#!/bin/bash

ids=$1

while IFS=":" read id user pass tid; do
	if [[ $ids -eq $tid ]]; then
		exit $id
	fi
done < database/signed.dat
