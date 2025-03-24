#!/bin/bash

uid=$1

while IFS=":" read filmname eamount ramount date; do
	script/session/rental/rent.sh "$filmname" $eamount
done < database/cart$uid
