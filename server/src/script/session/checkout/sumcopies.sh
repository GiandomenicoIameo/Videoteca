#!/bin/bash

uid=$1

if ! [[ -e database/cart$uid ]]; then
	exit 0
fi

declare -i count=0
while IFS=":" read filmname eamount date; do
	(( count = count + eamount ))
done < database/cart$uid

exit $count
