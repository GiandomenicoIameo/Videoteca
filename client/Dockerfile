FROM python:latest

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

# Installo le dipendenze dal file requirements.txt
# L'opzione --no-cache-dir è un parametro che viene
# utilizzato con il comando pip install per evitare che
# pip usi la cache durante l'installazione delle dipendenze.
# Tale cache viene salvata all'interno dell'immagine
# Docker, aumentando la dimensione dell'immagine

RUN pip install --root-user-action=ignore --upgrade pip
RUN pip install --root-user-action=ignore --no-cache-dir -r requirements


