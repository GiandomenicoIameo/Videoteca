# Guida ai comandi
## Creazione delle immagini dai rispettivi Dockerfile

```bash
$ cd Videoteca/client
$ docker build --tag=<nome-immagine>[:<tag>] .

$ cd Videoteca/server
$ docker build --tag=<nome-immagine>[:<tag>] .
```

## Creazione di una rete di container

```bash
$ docker network create <nome-rete>
```

## Avvio di un container in modalità interattiva

```bash
# Le opzioni --name e --network consentono, rispettivamente, di
# assegnare un nome al container e connettere quest'ultimo
# a una rete creata precedentemente
$ docker run -it --name=<nome-container> --network=<nome-rete> <nome-immagine> /bin/bash

# N.B. Il nome del container deve corrispondere al valore specificato nella variabile
# server_name nel file client-tcp.py
```

## Demo
```bash
# Una volta avviato il container del client, tramite una sessione di terminale
# è sufficiente digitare:
$ ./server-tcp.out
Server in ascolto sulla porta 8080...

# A questo punto basta spostarci nel container del client, avviato
# precedentemente mediante un'altra sessione di terminale, e digitare:
$ python3 client-tcp.py
Componi messaggio:

# Ora non dovremo far altro che digitare una stringa di caratteri e
# premere invio, dopodiché l'applicazione farà il resto.
```
