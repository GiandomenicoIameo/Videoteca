#!/bin/bash

uid=$1

if [[ -e database/cart$uid ]]; then
	while IFS=":" read filmname eamount total date; do
		script/session/rental/rent.sh "$filmname" $eamount
	done < database/cart$uid
fi
