#!/bin/bash

file=$1

if [[ -e database/"$file" ]]; then
	gzip -c database/"$file" > database/"$file".gz
fi
