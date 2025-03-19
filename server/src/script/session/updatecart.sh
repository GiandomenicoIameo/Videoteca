#!/bin/bash

uid=$1; filmname=$2
helpful=$3

if [[ -e database/cart$uid ]]; then
    sed -ri 's/('"$filmname"'.*:).*/\1'$helpful'/' database/cart$uid
fi

#
# while IFS=":" read name num available; do
#     if [[ $filmname == $name ]]; then
#             sed -i s/$name:$num:$available/$filmname:$num:$helpful/ database/cart$uid
#             break
#     fi
# done < database/cart$uid
