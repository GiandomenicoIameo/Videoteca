#!/bin/bash

id=$1

if [[ -e database/cart$id ]]; then
    rm database/cart$id
fi

