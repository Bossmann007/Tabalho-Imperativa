CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic-errors -Wmissing-prototypes

all: P1.exe P2.exe

P1.exe: P1.c alimento.c arquivo.c amostra.c alimento.h arquivo.h amostra.h
	$(CC) $(CFLAGS) P1.c alimento.c arquivo.c amostra.c -o P1.exe

P2.exe: P2.c alimento.c arquivo.c arvore.c lista.c interface.c alimento.h arquivo.h arvore.h lista.h interface.h
	$(CC) $(CFLAGS) P2.c alimento.c arquivo.c arvore.c lista.c interface.c -o P2.exe

clean:
	rm -f P1.exe P2.exe
