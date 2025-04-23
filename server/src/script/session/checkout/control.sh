#!/bin/bash

uid=$1
tot=$2

if ! [[ -e database/rented$uid ]]; then
	exit 0
fi

declare -i count=0
while IFS=":" read filmname eamount current date; do
	(( count = count + eamount ))
done < database/rented$uid

(( n = tot + count ))
if (( n > 20 )); then
	exit 1
else
	exit 0
fi

