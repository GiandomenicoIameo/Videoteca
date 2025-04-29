#!/bin/bash

file=$1

if [[ -e database/"$file".gz ]]; then
	hexdump -v -e '/1 "%02x"' database/"$file".gz
	rm database/"$file".gz
else
	# Se il database non esiste, in tal caso si tratta del file dei film noleggiati oppure del carrello ( Il database movies.dat non viene cancellato )
	# viene creato un file vuoto con nessun articolo all'interno. In questo modo, in primo luogo, nell'interfaccia grafica
	# verranno inseriti dei valori vuoti e in secondo luogo, si impedirà che venga ricevuto un file non compresso ( il client
	# si aspetta un file compresso ).

	touch database/"$file"
	gzip -c database/"$file" > database/"$file".gz

	hexdump -v -e '/1 "%02x"' database/"$file".gz
	rm database/"$file"
	rm database/"$file".gz
fi
