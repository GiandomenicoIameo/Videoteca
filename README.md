# Guida ai comandi
## Creazione delle immagini dai rispettivi Dockerfile

```bash
$ cd Videoteca/client
$ docker build --tag=<nome_immagine>[:<tag>] .

$ cd Videoteca/server
$ docker build --tag=<nome_immagine>[:<tag>] .
```

## Avvio di un container in modalità interattiva
### Avvio del container del server

```bash
Il comando creare e avviare il container del server è il seguente:
$ docker run -it --name=<nome-container> --network=<nome-rete> <nome-immagine> /bin/bash

Le opzioni --name e --network consentono, rispettivamente, di
assegnare un nome al container e connettere quest'ultimo a una rete creata precedentemente
```

# N.B. Il nome del container del server deve corrispondere al valore specificato
# nella variabile server_name nel file client-tcp.py
```

## Demo
```bash
# Una volta avviato il container del server, tramite una
# sessione di terminale, è sufficiente digitare:
$ ./server-tcp.out
Server in ascolto sulla porta 8080...

# A questo punto basta spostarci nel container del client, avviato
# precedentemente mediante un'altra sessione di terminale, e digitare:
$ python3 client-tcp.py
Componi messaggio:

# Ora non dovremo far altro che digitare una stringa di caratteri e
# premere invio, dopodiché l'applicazione farà il resto.
```
