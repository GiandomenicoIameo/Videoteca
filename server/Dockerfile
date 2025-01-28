FROM gcc:latest

# Attraverso il seguente comando creo la directory
# di lavoro e la chiamo app.
# Tale directory è una sottodirectory della
# directory root ( / ) nel file system del
# container.
WORKDIR /app

# Mediante il seguente comando copio il
# contenuto della directory corrente nella
# directory di lavoro del container: /app
COPY . /app

# Compilo il programma server-tcp.c e
# creo l'eseguibile
RUN gcc server-tcp.c -o server-tcp.out
