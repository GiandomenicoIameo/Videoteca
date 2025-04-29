#!/bin/bash

uid=$1

if [[ -e database/cart$uid ]]; then
	while IFS=":" read filmname eamount date; do
	    # Il seguente script aggiorna il database movies.dat.
		script/session/rental/updatemovie.sh "$filmname" $eamount
		# IL seguente script aggiorna il database rented.dat dei film noleggiati.
		script/session/rental/addrent.sh $uid "$filmname" $eamount "$date"
	done < database/cart$uid
fi

rm database/cart$uid
