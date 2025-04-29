#!/bin/bash

uid=$1

declare -i count=0

if ! [[ -e database/cart$uid ]]; then
	exit $count
fi

while IFS=":" read filmname eamount date; do
	(( count = count + eamount ))
done < database/cart$uid

exit $count
