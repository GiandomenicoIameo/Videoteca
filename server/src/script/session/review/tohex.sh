#!/bin/bash

file=$1

if [[ -e database/"$file".gz ]]; then
	hexdump -v -e '/1 "%02x"' database/"$file".gz
	rm database/"$file".gz
fi
