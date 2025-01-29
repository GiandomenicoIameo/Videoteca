# Creazione delle immagini dai rispettivi Dockerfile

```bash
$ cd Videoteca/client
$ docker build --tag=<nome_immagine> .

$ cd ../server
$ docker build --tag=<nome_immagine> .
```

# Creazione rete di container

```bash
$ docker network create <nome_rete>
```

# Creazione container,connessione alla rete e avvio in modalità interattiva

```bash
$ docker run -it --name=<nome_container> --network=<nome_rete> <nome_immagine> /bin/bash
```
