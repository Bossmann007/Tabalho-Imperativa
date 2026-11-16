#ifndef ARVORE_H
#define ARVORE_H

#include "alimento.h"

/*
 * Declaracao adiantada para a estrutura NoAlimento (definida em lista.h).
 * Permite que o no da arvore aponte diretamente para o elemento na lista ligada,
 * conforme o requisito 8 da especificacao.
 */
struct NoAlimento;

/*
 * Estrutura do no de indexacao da arvore binaria de busca (BST).
 * Armazena a chave numerica (energia em kcal ou proteina em g) e
 * o apontador para o no do alimento correspondente na lista ligada.
 */
typedef struct NoArvore {
    double chave;
    struct NoAlimento *alimento;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

/*
 * Insere um novo no na arvore binaria de busca.
 * Chaves repetidas sao inseridas na subarvore direita.
 */
NoArvore *arvore_inserir(NoArvore *raiz, double chave, struct NoAlimento *alimento);

/*
 * Percorre e lista os alimentos em ordem decrescente da chave
 * utilizando percurso inverso (subarvore direita -> raiz -> subarvore esquerda).
 */
void arvore_listar_decrescente(const NoArvore *raiz, int *contador);

/*
 * Percorre e lista os alimentos cuja chave esteja no intervalo [min, max],
 * tambem em ordem decrescente, podando ramos fora da faixa.
 */
void arvore_listar_faixa(const NoArvore *raiz, double min, double max, int *contador);

/*
 * Libera recursivamente toda a memoria alocada para os nos da arvore.
 */
void arvore_liberar(NoArvore *raiz);

#endif /* ARVORE_H */
