#!/bin/bash

uid=$1

if [[ -e database/cart$uid ]]; then
    rm database/cart$uid
    rm database/cart$uid.gz
fi

