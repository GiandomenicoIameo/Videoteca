#!/bin/bash

uid=$1

while IFS=":" read filmname eamount ramount current date; do
	script/session/fromcart.sh $uid "$filmname" $eamount
	script/session/addrent.sh $uid "$filmname" $eamount "$date"
done < database/cart$uid
