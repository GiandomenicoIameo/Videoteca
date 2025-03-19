#!/bin/bash

uid=$1; filmname=$2
number=$3; real=$4

if ! [[ -e database/cart$uid ]]; then
    touch database/cart$uid
    echo "$filmname":$number:$real >> database/cart$uid
else
    if grep -q ^"$filmname": database/cart$uid; then
        sed -i s/"$filmname":'.*':$real/"$filmname":$number:$real database/cart$uid
    fi
fi
