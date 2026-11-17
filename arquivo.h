/* arquivo.h: leitura do CSV e leitura/gravacao do arquivo binario. */

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "alimento.h"

/* Le o CSV num vetor dinamico e coloca em *total quantos leu (-1 se nao abriu). */
Alimento_t *arquivo_ler_csv(const char *caminho, int *total);

/* Grava o vetor no arquivo binario e devolve quantos gravou (-1 se deu erro). */
int arquivo_gravar_binario(const char *caminho, const Alimento_t *vetor, int quantidade);

/* Le ate capacidade registros do binario e devolve quantos leu (-1 se deu erro). */
int arquivo_ler_binario(const char *caminho, Alimento_t *vetor, int capacidade);

/* Devolve o tamanho do arquivo em bytes (-1 se deu erro). */
long arquivo_obter_tamanho(const char *caminho);

#endif /* ARQUIVO_H */
