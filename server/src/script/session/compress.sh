#!/bin/bash

uid=$1

if ! [[ -e database/cart$uid.gz ]]; then
	gzip -c database/cart$uid > database/cart$uid.gz
fi
