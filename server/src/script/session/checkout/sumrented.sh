#!/bin/bash

uid=$1

declare -i count=0

if ! [[ -e database/rented$uid ]]; then
	exit $count
fi

while IFS=":" read filmname eamount onedate twodate; do
	(( count = count + eamount ))
done < database/rented$uid

exit $count
