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
$ docker run --name=<nome_container> -p 8080:8080 <nome_immagine>
```
* *L'opzione ___--name___ consente di assegnare un nome al container.*
* *L'opzione ___-p 8080:8080___  è di fondamentale importanza per la comunicazione tra i due container. Tale opzione mappa la porta del container su una porta         dell'host locale. Nel nostro caso, la porta 8080 del container (che sarebbe anche la porta da cui ascolta il programma server) viene esposta sulla porta 8080     del computer locale. In altre parole, senza l'opzione -p il container rimarrebbe completamente isolato, non accessibile dall'esterno.*

### Avvio del container del client
*L'avvio del container del client richiede il completamento di due passaggi che ora descriveremo. 

*Come prima cosa è necessario concedere l'autorizzazione al container di accedere al server grafico X11 del sistema Linux host. Per impostazione predefinita, il server X limita l'accesso ai client esterni (come quelli all'interno di un container). Il comando che stiamo per menzionare modifica le regole di accesso, permettendo anche ai processi avviati nei container Docker di interagire con il server X del sistema host. Questo passaggio risulta necessario affinché l'interfaccia grafica dell'applicazione venga avviata. Il comando per concedere l'autorizzazione ai container docker è:*
 
```bash
$ xhost +local:docker
```
*E' bene notare che tale comando consente a qualsiasi processo in esecuzione su una macchina remota di connettersi al server grafico X11 del proprio sistema host. Quindi per proteggere il sistema è buona pratica revocare l'autorizzazione al termine del lavoro. Per farlo, basta digitare:*

```bash
$ xhost -local:docker
```
*Come secondo passaggio, bisogna definire la variabile d'ambiente DISPLAY. Essa indica quale server X (il gestore grafico) deve essere utilizzato per gestire le applicazioni grafiche, e quindi anche mostrarle. Senza dilungarci troppo su questa variabile digitiamo:*

```bash
$ export DISPLAY=:0
```
```:0``` *rappresenta in genere il primo server X attivo. Il comando ```export``` rende questa variabile disponibile per i processi che verranno avviati nella stessa sessione. 
Come si vedrà fra poco, tale variabile verrà esportata dall'ambiente principale (il sistema host) al container per consentire al processo grafico client di connettersi al server X del sistema host.*

*Arrivati a questo punto, non ci resta che avviare il container del client*


```bash
$ docker run --name=<nome_container> -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /dev/dri:/dev/dri <nome_immagine>
```
*Se tutti i passaggi sono stati completati con successo, l'interfaccia grafica dovrebbe presentarsi sullo schermo. In caso contrario, si prega di segnalare eventuali anomalie riscontrate durante la configurazione dell'ambiente per aiutarci a migliorare l'applicazione.*
