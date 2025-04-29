#!/bin/bash

uid=$1

if [[ -e database/cart$uid ]]; then
	while IFS=":" read filmname eamount date; do
		#script/session/setup/fromcart.sh $uid "$filmname" $eamount
		script/session/rental/addrent.sh $uid "$filmname" $eamount "$date"
	done < database/cart$uid
fi

rm database/cart$uid
