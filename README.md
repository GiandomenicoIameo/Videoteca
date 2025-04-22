# <p align=center>Guida ai comandi</p>
*L'applicazione è stata progettata per essere eseguita su sistemi GNU/Linux. Pertanto, per tutta la durata della guida si assumerà che l'utente 
si trovi su una distribuzione GNU/Linux completamente funzionante, dotata del gestore grafico X Window System. Tuttavia, ci teniamo a precisare che la configurazione dell'ambiente di esecuzione dell'applicazione, così come i vari test sull'applicazione stessa, sono stati effettuati su un'unica distribuzione, ovvero Debian GNU/Linux 12 (Bookworm).*

*Questa guida ha come scopo quello di presentare, anzitutto, la tecnologia utilizzata per comprendere il corretto funzionamento dell'applicazione e, in secondo luogo, di descrivere i passaggi per la configurazione dell'ambiente in cui l'applicativo verrà eseguito.*

## <p align=center>Tecnologie utilizzate</p>
### Docker
*Docker è la tecnologia di virtualizzazione impiegata per questa applicazione e si basa sul concetto di ___containerizzazione___. E' uno strumento che permette in modo estremamente facile la creazione, l'utilizzo e l'esecuzione di applicazioni utilizzando i container. In questo modo l'utente focalizza l'attenzione su questioni diverse dall'installazione e configurazione dell'applicazione. 
Come prima cosa, quindi, è necessario installare il software docker. Basta digitare da terminale.*

```bash
$ sudo apt update && apt install docker
```
## <p align=center>Creazione dell'ambiente di esecuzione </p>
*Ora non ci resta che creare l'ambiente adatto per eseguire l'applicazione. Creeremo due ambienti virtuali, in cui il programma client e il programma server verranno eseguiti in modo isolato. Successivamente, questi programmi comunicheranno tramite la rete di comunicazione, sfruttando le porte e gli indirizzi dei container.*

*La creazione degli ambienti richiede dapprima la costruzione di particolari oggetti docker noti come ___immagini___. I Dockerfile, memorizzati rispettivamente nella directory ```client/``` e nella directory ```server/```, definiscono proprio come costruire queste immagini. La creazione è dettata dai seguenti comandi:*

### Creazione delle immagini dai Dockerfile

```bash
$ cd Videoteca/client
$ docker build --tag=<nome_immagine>[:<tag>] .

$ cd Videoteca/server
$ docker build --tag=<nome_immagine>[:<tag>] .
```

### Avvio dei container
#### Avvio del container del server
*Il comando per creare e avviare il container del server è il seguente:*

```bash
$ docker run --rm --name=<nome_container> -p 8080:8080 <nome_immagine> /bin/bash
```
* *L'opzione ___--name___ consente di assegnare un nome al container.*
* *L'opzione ___-p 8080:8080___  è di fondamentale importanza per la comunicazione tra i due container. Tale opzione mappa la porta del container su una porta         dell'host locale. Nel nostro caso, la porta 8080 del container ( che sarebbe anche la porta da cui ascolta il programma server ) viene esposta sulla porta 8080     del computer locale. In altre parole, senza l'opzione -p il container rimarrebbe completamente isolato, non accessibile dall'esterno.*

### Avvio del container del client
Prima di avviare il container del client è necessario concedere l'autorizzazione a quest'ultimo di accedere al server grafico X11 del sistema Linux host.

```bash
```
> E' bene notare che il comando che stiamo per menzionare consente a chiunque di connettersi al server
> grafico X11 del proprio sistema host, quindi è importante rimuovere l'autorizzazione dopo aver terminato il lavoro.

```bash
# I comandi per concedere e rimuovere l'autorizzazione ai container docker sono:

$ xhost +local:docker
$ xhost -local:docker

# Dopodiché non ci resta che avviare il container del client:

$ docker run --shm-size=512m -it --rm --name=<nome_container> -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /dev/dri:/dev/dri <nome_immagine> /bin/bash
```

## Demo

```bash
# Una volta avviato il container del server, tramite una
# sessione di terminale, è sufficiente digitare:

$ ./src/server-tcp.out
Server in ascolto sulla porta 8080...

# Una volta ciò, basta spostarci nel container del client, avviato
# precedentemente mediante un'altra sessione di terminale, e digitare:

$ python3 client-tcp.py

# A questo punto non ci resta che interagire con l'interfaccia grafica dell'
# applicativo appena apparsa sullo schermo.
```
