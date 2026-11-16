#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "alimento.h"

/*
 * Le registros de alimentos de um arquivo CSV (com separador ';').
 * Descarta a primeira linha (cabecalho).
 * Retorna o numero de registros lidos com sucesso ou -1 em caso de falha de abertura.
 */
int arquivo_ler_csv(const char *caminho, Alimento *vetor, int capacidade);

/*
 * Grava o vetor de alimentos em formato binario direto no arquivo especificado.
 * Retorna a quantidade de registros gravados com sucesso ou -1 em caso de falha.
 */
int arquivo_gravar_binario(const char *caminho, const Alimento *vetor, int quantidade);

/*
 * Le registros de alimentos de um arquivo binario para o vetor.
 * Retorna a quantidade de registros efetivamente lidos ou -1 em caso de falha.
 */
int arquivo_ler_binario(const char *caminho, Alimento *vetor, int capacidade);

/*
 * Retorna o tamanho do arquivo em bytes (via fseek/ftell), ou -1 em caso de erro.
 */
long arquivo_obter_tamanho(const char *caminho);

#endif /* ARQUIVO_H */
