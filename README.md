# Guida ai comandi
## Creazione delle immagini dai rispettivi Dockerfile

```bash
$ cd Videoteca/client
$ docker build --tag=<nome_immagine>[:<tag>] .

$ cd Videoteca/server
$ docker build --tag=<nome_immagine>[:<tag>] .
```

## Avvio dei container in modalità interattiva
### Avvio del container del server

```bash
# Il comando per creare e avviare il container del server è il seguente:

$ docker run -it --rm --name=<nome_container> -p 8080:8080 <nome_immagine> /bin/bash

# L'opzione --name consente di assegnare un nome al container.
# L'opzione -p 8080:8080  è di fondamentale importanza per la comunicazione tra due container.
# Tale opzione mappa la porta del container su una porta dell'host locale. Nel nostro caso,
# La porta 8080 del container ( che sarebbe anche la porta da cui ascolta il server ) viene
# esposta sulla porta 8080 del computer locale. In altre parole, senza l'opzione -p il container
# rimarrebbe completamente isolato, non accessibile dall'esterno.
```
### Avvio del container del client

```bash
Il comando per creare e avviare il container del client è il seguente:
$ docker run --shm-size=512m -it --rm --name=cclient -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /dev/dri:/dev/dri iclient /bin/bash
```

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

# A questo punto non ci resta che interagire con l'interfaccia grafica dell'
# applicativo appena apparsa sullo schermo.
```
