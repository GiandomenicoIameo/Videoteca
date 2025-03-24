#!/bin/bash

uid=$1; filmname=$2
ramount=$3

if [[ -e database/cart$uid ]]; then
    sed -ri 's/^('"$filmname"':.*:).*(:.*:.*)$/\1'$ramount'\2/' database/cart$uid
fi
