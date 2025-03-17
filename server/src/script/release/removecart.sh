#!/bin/bash

sda=$1

if [[ -e database/cart$sda ]]; then
    rm database/cart$sda
fi

