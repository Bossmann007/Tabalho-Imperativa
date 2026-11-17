/* arvore.c: arvore binaria de busca usada como indice de energia e proteina. */

#include "arvore.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/* Insere a chave na arvore de forma recursiva e devolve a raiz da subarvore. */
NoArvore_t *arvore_inserir(NoArvore_t *raiz, double chave,
                           struct NoAlimento *alimento)
{
    NoArvore_t *resultado = raiz;

    if (!raiz) {
        NoArvore_t *novo = malloc(sizeof(NoArvore_t));
        if (!novo) {
            fprintf(stderr,
                    "ERRO FATAL: Memoria insuficiente para o no da arvore.\n");
            exit(EXIT_FAILURE);
        }
        novo->chave = chave;
        novo->alimento = alimento;
        novo->esquerda = NULL;
        novo->direita = NULL;
        resultado = novo;
    } else if (chave < raiz->chave) {
        raiz->esquerda = arvore_inserir(raiz->esquerda, chave, alimento);
    } else {
        /* chave igual vai para a direita (>=) */
        raiz->direita = arvore_inserir(raiz->direita, chave, alimento);
    }

    return resultado;
}

/* Percorre direita, raiz e esquerda para listar da maior chave para a menor. */
void arvore_listar_decrescente(const NoArvore_t *raiz, int *contador)
{
    if (raiz) {
        arvore_listar_decrescente(raiz->direita, contador);

        if (contador) {
            (*contador)++;
        }
        if (raiz->alimento) {
            alimento_exibir(&(raiz->alimento->dado));
        }

        arvore_listar_decrescente(raiz->esquerda, contador);
    }
}

/* Lista em ordem decrescente so as chaves da faixa, sem descer onde nao ha nenhuma. */
void arvore_listar_faixa(const NoArvore_t *raiz, double min, double max,
                         int *contador)
{
    if (raiz) {
        if (raiz->chave <= max) {
            arvore_listar_faixa(raiz->direita, min, max, contador);
        }

        if (raiz->chave >= min && raiz->chave <= max) {
            if (contador) {
                (*contador)++;
            }
            if (raiz->alimento) {
                alimento_exibir(&(raiz->alimento->dado));
            }
        }

        if (raiz->chave >= min) {
            arvore_listar_faixa(raiz->esquerda, min, max, contador);
        }
    }
}

/* Libera os filhos primeiro e depois o proprio no. */
void arvore_liberar(NoArvore_t *raiz)
{
    if (raiz) {
        arvore_liberar(raiz->esquerda);
        arvore_liberar(raiz->direita);
        free(raiz);
    }
}
