#!/bin/bash

file=$1

if [[ -e database/$file.gz ]]; then
	hexdump -v -e '/1 "%02x"' database/$file.gz
	rm database/$file.gz
else
	echo "File vuoto!" > database/$file; gzip database/$file
	hexdump -v -e '/1 "%02x"' database/$file.gz
	rm database/$file.gz
fi
