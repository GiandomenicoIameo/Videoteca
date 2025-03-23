#!/bin/bash

uid=$1

while IFS=":" read name eamount ramount current date; do
    script/session/date.sh "$date"; var=$?
    if (( ramount < eamount )) || (( var == 1 )); then
        exit 0
    fi
done < database/cart$uid

exit 1
