#!/bin/bash

uid=$1

while IFS=":" read name eamount ramount date; do
    script/session/date.sh $date;
    if (( ramount < eamount )) || (( $? == 1 )); then
        exit 0
    fi
done < database/cart$uid

exit 1
