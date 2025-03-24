#!/bin/bash

uid=$1

while IFS=":" read name eamount ramount date; do
    script/session/date.sh "$date"; var=$?
    if (( ramount < eamount )); then
        exit 1
    fi
    if (( var == 1 )); then
        exit 2
    fi
done < database/cart$uid

exit 0
