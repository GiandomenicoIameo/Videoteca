#!/bin/bash

uid=$1

if [[ -e database/rented$uid ]]; then
	while IFS=":" read filmname eamount date; do
 		if { script/session/date.sh $date; }; then
			exit 1
		fi
	done < database/rented$uid
fi
