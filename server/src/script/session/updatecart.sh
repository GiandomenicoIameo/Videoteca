#!/bin/bash

uid=$1; filmname=$2
remount=$3

if [[ -e database/cart$uid ]]; then
    sed -ri 's/('"$filmname"'.*:).*/\1'$remount'/' database/cart$uid
fi
