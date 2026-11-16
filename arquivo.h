/* arquivo.h: leitura do CSV e leitura/gravacao do arquivo binario. */

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "alimento.h"

/* Le o CSV (separador ';'), pulando a linha de cabecalho.
 * Devolve quantos alimentos leu, ou -1 se nao abriu o arquivo. */
int arquivo_ler_csv(const char *caminho, Alimento_t *vetor, int capacidade);

/* Grava o vetor no arquivo binario. Com quantidade 0 cria o arquivo vazio.
 * Devolve quantos registros gravou, ou -1 em caso de erro. */
int arquivo_gravar_binario(const char *caminho, const Alimento_t *vetor, int quantidade);

/* Le ate capacidade registros do arquivo binario para o vetor.
 * Devolve quantos leu, ou -1 em caso de erro. */
int arquivo_ler_binario(const char *caminho, Alimento_t *vetor, int capacidade);

/* Devolve o tamanho do arquivo em bytes, ou -1 em caso de erro. */
long arquivo_obter_tamanho(const char *caminho);

#endif /* ARQUIVO_H */
