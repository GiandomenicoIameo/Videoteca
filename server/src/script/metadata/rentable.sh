#!/bin/bash

usrc=$1

if (( usrc != 0 )); then
	while IFS=":" read filmname ramount rentable; do
		(( copies = ramount / usrc ))
		if (( copies >= 1 )); then
			sed -ri 's/('"$filmname"':.*:).*/\1'$copies'/' database/movies.dat
		else
			sed -ri 's/('"$filmname"'.*:)'$rentable'/\11/' database/movies.dat
		fi
	done < database/movies.dat
fi
