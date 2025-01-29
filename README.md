# Guida ai comandi
## Creazione delle immagini dai rispettivi Dockerfile

```bash
$ cd Videoteca/client
$ docker build --tag=<nome-immagine>[:<tag>] .

$ cd Videoteca/server
$ docker build --tag=<nome-immagine>[:<tag>] .
```

## Creazione rete di container

```bash
$ docker network create <nome-rete>
```

## Avvio di un container  modalità interattiva

```bash
# Le opzioni --name e --network consentono, rispettivamente, di
# assegnare un nome al container e connettere quest'ultimo
# a una rete creata precedentemente
$ docker run -it --name=<nome-container> --network=<nome-rete> <nome-immagine> /bin/bash
```
