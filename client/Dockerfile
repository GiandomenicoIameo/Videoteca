FROM python:latest

# Utilizzando il seguente comando, viene creata la directory 
# di lavoro denominata app, che è una sottodirectory della 
# directory root (/) nel file system del container.
WORKDIR /app

# Mediante il seguente comando viene copiato il
# contenuto della directory corrente nella
# directory di lavoro del container: /app
COPY . /app

RUN pip install --root-user-action=ignore --upgrade pip

# Il seguente comando consente di installare le dipendenze 
# dal file requirements. L'opzione --no-cache-dir è un 
# parametro che viene utilizzato con il comando pip install per 
# evitare che pip usi la cache durante l'installazione delle 
# dipendenze. Tale cache viene salvata all'interno dell'immagine
# Docker, aumentando la dimensione di quest'ultima.
RUN pip install --root-user-action=ignore --no-cache-dir -r requirements
