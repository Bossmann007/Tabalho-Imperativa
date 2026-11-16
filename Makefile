CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: P1.exe P2.exe

P1.exe: P1.c alimento.c arquivo.c alimento.h arquivo.h
	$(CC) $(CFLAGS) P1.c alimento.c arquivo.c -o P1.exe

P2.exe: P2.c alimento.c arvore.c lista.c alimento.h arvore.h lista.h
	$(CC) $(CFLAGS) P2.c alimento.c arvore.c lista.c -o P2.exe

clean:
	rm -f P1.exe P2.exe
