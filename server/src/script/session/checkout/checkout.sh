#!/bin/bash


# Il seguente script effettua un controllo sulle quantità che si desidera noleggiare.
# Il primo controllo verifica se il numero delle copie noleggiate sommato al numero
# delle copie che si desidera noleggiare è maggiore di 20.

# Il secondo controllo verifica se le copie che si desidera noleggiare non superi
# la quantità noleggiabile per quel film.

# Se anche un solo film nel carrello non supera il test, allora l'operazione non può
# essere completata. L'operazione di checkout è un'entità atomica. Ciò significa che
# viene eseguita completamente oppure per niente.

# Si assume che il carrello sia non vuoto. Il lato client non consentirà all'utente di
# effettuare il checkout se il carrello è vuoto. Quindi, sicuramente, se l'utente ha effettuato
# il checkout il carrello doveva essere non vuoto.

uid=$1

script/session/checkout/sumrental.sh $uid; count1=$?
script/session/checkout/sumrented.sh $uid; count2=$?

(( total = count1 + count2 ))
if (( total > 20  )); then
	exit 1
else

	declare -i sum=0
	declare -i row=0

	# Nella seguente istruzione viene estrapolata la prima riga del
	# file cart$uid

	IFS=":"
	read filmname eamount date < database/cart$uid
	previous=$filmname

	row=1

	# Se la quantità che si desidera noleggiare è maggiore
	# del numero di copie totali si ottiene un errore
	script/search/ramount.sh "$filmname"; ramount=$?
	if (( eamount > ramount )); then
		exit 2
	fi

	# Qui viene effettuato il controllo sulla data di rilascio
	# del film di nome $filmname. Non è banale perché gli articoli
	# all'interno del carrello potrebbero rimanere a lungo, quindi
	# le date potrebbero essere scadute.
	script/session/date.sh "$date"; var=$?
	if (( var == 1 )); then
		exit 3
	fi

	(( sum = sum + eamount ))

	# Il file cart$uid viene ordinato in base al nome del film
	while IFS=":" read filmname eamount date; do
		if (( row == 1 )); then
			row=2; continue
		fi

		# Qui vengono contate le quantità che si desidera noleggiare
		# relative al film di nome $filmname. Se la somma è maggiore della
		# quantità totale si ottiene un errore.
		script/search/ramount.sh "$filmname"; ramount=$?
		if [[ "$filmname" == "$previous" ]]; then
			(( sum = sum + eamount ))
			if (( sum > ramount )); then
				exit 2
			fi
		else
		# Qui viene incontrato il nome del primo film diverso dal precedente
			previous=$filmname
			sum=$eamount
			if (( eamount > ramount )); then
				exit 2
			fi
		fi
		script/session/date.sh "$date"; var=$?
		if (( var == 1 )); then
			exit 3
		fi
	done < <( cat database/cart$uid | sort -k 1 )
fi










