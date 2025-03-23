#!/bin/bash

uid=$1

while IFS=":" read filmname eamount ramount date current; do
	script/session/rent.sh "$filmname" $eamount
done < database/cart$uid
