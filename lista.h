#ifndef LISTA_H
#define LISTA_H

#include "alimento.h"
#include "arvore.h"

/*
 * No da lista ligada de alimentos pertencentes a uma categoria.
 * Armazena os dados do alimento e o ponteiro para o proximo alimento.
 */
typedef struct NoAlimento {
    Alimento dado;
    struct NoAlimento *proximo;
} NoAlimento;

/*
 * No da lista ligada de categorias.
 * Cada categoria mantem:
 *  - Seu identificador enum e seu nome textual
 *  - Ponteiro para a cabeca da lista ligada de alimentos
 *  - Duas arvores binarias de indexacao (energia_kcal e proteina_g)
 *  - Quantidade total de alimentos da categoria
 *  - Ponteiro para o proximo no de categoria
 */
typedef struct NoCategoria {
    Categoria categoria;
    char nome[TAM_MAX_DESCRICAO];
    NoAlimento *alimentos;
    NoArvore *arvore_energia;
    NoArvore *arvore_proteina;
    int total_alimentos;
    struct NoCategoria *proximo;
} NoCategoria;

/*
 * Constroi as listas ligadas e as arvores binarias a partir de dados.bin.
 * Retorna o ponteiro para o primeiro no da lista de categorias.
 */
NoCategoria *lista_carregar_de_binario(const char *caminho_bin, int *total_lidos);

/*
 * Insere um alimento na categoria apropriada, mantendo ordem alfabetica
 * da lista de categorias e ordem alfabetica dos alimentos da categoria.
 */
void lista_inserir_alimento(NoCategoria **cabeca_categorias, const Alimento *alimento);

/*
 * Localiza ou cria e insere em ordem alfabetica um no de categoria na lista.
 */
NoCategoria *lista_obter_ou_criar_categoria(NoCategoria **cabeca, Categoria cat, const char *nome);

/*
 * Exibe todas as categorias cadastradas na ordem da lista (alfabetica).
 */
void lista_exibir_categorias(const NoCategoria *cabeca);

/*
 * Retorna o ponteiro para a categoria correspondente ao indice (1-baseado).
 */
NoCategoria *lista_obter_categoria_por_indice(NoCategoria *cabeca, int indice);

/*
 * Exibe todos os alimentos de uma categoria na ordem da lista (alfabetica).
 */
void lista_exibir_alimentos_categoria(const NoCategoria *categoria);

/*
 * Remove um alimento especifico (pelo numero do alimento) da categoria.
 * Reconstrói automaticamente as duas arvores binarias de indexacao.
 * Retorna 1 se removeu com sucesso, ou 0 se nao encontrou.
 */
int lista_remover_alimento(NoCategoria *categoria, int numero_alimento);

/*
 * Remove uma categoria inteira da lista, liberando seus alimentos e arvores.
 * Retorna 1 se removeu com sucesso, ou 0 se nao encontrou.
 */
int lista_remover_categoria(NoCategoria **cabeca_categorias, Categoria cat);

/*
 * Reconstrói as duas arvores binarias de indexacao da categoria a partir da lista de alimentos.
 */
void categoria_reconstruir_arvores(NoCategoria *cat);

/*
 * Grava todos os alimentos atualmente na memoria de volta para o arquivo dados.bin.
 * Retorna a quantidade total de registros gravados.
 */
int lista_salvar_binario(const NoCategoria *cabeca, const char *caminho_bin);

/*
 * Libera recursivamente toda a memoria alocada (listas e arvores).
 */
void lista_liberar_tudo(NoCategoria **cabeca);

#endif /* LISTA_H */
