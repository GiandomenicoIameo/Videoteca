#!/bin/bash

ids=$1
username=$2
password=$3

declare -i status=0
row=$( cat database/signed.dat | wc -l )

if (( row == 0 )); then
	echo 1:"$username":"$password":$ids >> database/signed.dat
else
	declare -i line=1

	while IFS=":" read uid user pass sdi; do
		if (( uid != line )); then
 			sed -i ${line}'i\'$line:"$username":"$password":$ids database/signed.dat
			status=1; break
		fi
		(( line = line + 1 ))
	done < database/signed.dat

	if (( status == 0 )); then
		echo $line:"$username":"$password":$ids >> database/signed.dat
	fi

fi
