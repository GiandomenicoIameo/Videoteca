#!/bin/bash

client=$1

if [[ -e cart_$client ]]; then
    rm cart_$client
fi

