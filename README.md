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

## Creazione container,connessione alla rete e avvio in modalità interattiva

```bash
$ docker run -it --name=<nome-container> --network=<nome-rete> <nome-immagine> /bin/bash
```
