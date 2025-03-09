# Variabili
CC = gcc
OPTIONS = -Wall -Wextra -pedantic -fstack-protector-strong -I../include
LIB = ../lib/authentication.c ../lib/session.c ../lib/release.c \
      ../lib/search.c ../lib/metadata.c server-tcp.c
OBJ = $(LIB:.c=.o)

# Target principale
server.out:	$(OBJ)
	$(CC) $(OPTIONS) -o server.out $(OBJ)

%.o:	%.c
	$(CC) $(OPTIONS) -c $< -o $@

clean:
	rm $(OBJ) server.out

