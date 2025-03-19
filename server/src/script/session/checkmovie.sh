#!/bin/bash

filmname=$1
number=$2

while IFS=":" read name num; do
	if [[ $filmname == $name ]]; then
		if (( number <= num )); then
			exit 0
		else
			exit 1
		fi
	fi
done < database/movies.dat

exit 2
