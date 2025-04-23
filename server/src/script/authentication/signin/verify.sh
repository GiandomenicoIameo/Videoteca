#!/bin/bash

uid=$1

if [[ -e database/rented$uid ]]; then
	while IFS=":" read filmname eamount current data; do

		var1=$( date -d "$current" +%s )
		var2=$( date -d "$data" +%s )

 		if (( var2 < var1 )); then
			exit 1
		fi
	done < database/rented$uid
fi

exit 0
