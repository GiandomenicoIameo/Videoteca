#!/bin/bash

current=$( date +%s )
input=$( date -d $1 +%s )

if (( input < current )); then
    exit 1
else
    exit 0
fi
