#!/bin/bash

sda=$1

if [[ -e database/cart_$sda ]]; then
    rm database/cart_$sda
fi

