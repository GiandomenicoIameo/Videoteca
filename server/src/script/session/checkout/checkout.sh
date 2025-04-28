#!/bin/bash

# Il seguente script effettua un controllo sulle quantità che si desidera noleggiare.
# Se anche un solo film nel carrello non supera il test, allora l'operazione non può
# essere completata. L'operazione di checkout è un'entità atomica. Ciò significa che
# viene eseguita completamente oppure per niente.

uid=$1

if ! [[ -e database/cart$uid ]]; then
    exit 1
fi

declare -i sum=0
declare -i row=0

# Nella seguente istruzione viene estrapolata la prima riga del
# file cart$uid

IFS=":"
read filmname eamount ramount date < database/cart$uid
previous=$filmname

(( row = row + 1 ))


script/session/date.sh "$date"; var=$?
# Se la quantità che si desidera noleggiare è maggiore
# del numero di copie totali si ottiene un errore
if (( eamount > ramount )); then
	exit 1
fi

# Qui viene effettuato il controllo sulla data di rilascio
# del film di nome $filmname
if (( var == 1 )); then
	exit 2
fi

(( sum = sum + eamount ))

# Il file cart$uid viene ordinato in base al nome del film
while IFS=":" read filmname eamount ramount date; do
	script/session/date.sh "$date"; var=$?

	if (( row == 1 )); then
		(( row = row + 1 ))
		continue
	fi
	if [[ "$filmname" == "$previous" ]]; then
    # Qui vengono contate le quantità che si desidera noleggiare
    # relative al film di nome $filmname. Se la somma è maggiore della
    # quantità totale si ottiene un errore.
		(( sum = sum + eamount ))
		if (( sum > ramount )); then
			exit 1
		fi
	else
    # Qui viene incontrato il nome del primo film diverso dal precedente
		previous=$filmname
		sum=$eamount
		if (( eamount > ramount )); then
			exit 1
		fi
	fi

	if (( var == 1 )); then
		exit 2
	fi
done < <( cat database/cart$uid | sort -k 1 )

