FROM gcc:latest

# Utilizzando il seguente comando, viene creata la directory 
# di lavoro denominata app, che è una sottodirectory della 
# directory root (/) nel file system del container.
WORKDIR /app

# Mediante il seguente comando viene copiato il
# contenuto della directory corrente nella
# directory di lavoro del container: /app
COPY . /app

WORKDIR /app/src
# Compilazione del programma server-tcp.c e
# creazione dell'eseguibile
RUN make
WORKDIR /app
