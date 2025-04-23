#!/bin/bash

uid=$1

if [[ -e database/rented$uid ]]; then
	while IFS=":" read filmname number current date; do
		script/session/restitution/delrented.sh $uid "$filmname" $number "$date"
		script/session/restitution/updatemovies.sh "$filmname" $number
	done < database/rented$uid
fi
