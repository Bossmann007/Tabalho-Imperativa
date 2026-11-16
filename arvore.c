#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "lista.h"

/*
 * Insere um novo no na arvore binaria de busca (BST).
 * Chaves duplicadas sao direcionadas para a subarvore direita,
 * garantindo estabilidade e permitindo alimentos com mesmo valor de energia/proteina.
 * Possui um unico return no final.
 */
NoArvore *arvore_inserir(NoArvore *raiz, double chave, struct NoAlimento *alimento) {
    NoArvore *resultado = raiz;

    if (raiz == NULL) {
        NoArvore *novo = (NoArvore *)malloc(sizeof(NoArvore));
        if (novo != NULL) {
            novo->chave = chave;
            novo->alimento = alimento;
            novo->esquerda = NULL;
            novo->direita = NULL;
        }
        resultado = novo;
    } else {
        if (chave < raiz->chave) {
            raiz->esquerda = arvore_inserir(raiz->esquerda, chave, alimento);
        } else {
            raiz->direita = arvore_inserir(raiz->direita, chave, alimento);
        }
        resultado = raiz;
    }

    return resultado;
}

/*
 * Percorre a arvore em percurso inverso (direita -> raiz -> esquerda)
 * para exibir os alimentos em ordem estritamente decrescente da chave.
 */
void arvore_listar_decrescente(const NoArvore *raiz, int *contador) {
    if (raiz != NULL) {
        arvore_listar_decrescente(raiz->direita, contador);

        if (contador != NULL) {
            (*contador)++;
        }
        if (raiz->alimento != NULL) {
            alimento_exibir(&(raiz->alimento->dado));
        }

        arvore_listar_decrescente(raiz->esquerda, contador);
    }
}

/*
 * Percorre a arvore binaria de busca exibindo apenas os alimentos cuja chave
 * esteja contida no intervalo [min, max], em ordem decrescente.
 * Aproveita as propriedades da BST para nao visitar subarvores que estao fora da faixa.
 */
void arvore_listar_faixa(const NoArvore *raiz, double min, double max, int *contador) {
    if (raiz != NULL) {
        /* Se a raiz for menor ou igual ao maximo, podem existir chaves validas a direita */
        if (raiz->chave <= max) {
            arvore_listar_faixa(raiz->direita, min, max, contador);
        }

        /* Se a chave da raiz estiver dentro do intervalo [min, max], exibe */
        if (raiz->chave >= min && raiz->chave <= max) {
            if (contador != NULL) {
                (*contador)++;
            }
            if (raiz->alimento != NULL) {
                alimento_exibir(&(raiz->alimento->dado));
            }
        }

        /* Se a raiz for maior ou igual ao minimo, podem existir chaves validas a esquerda */
        if (raiz->chave >= min) {
            arvore_listar_faixa(raiz->esquerda, min, max, contador);
        }
    }
}

/*
 * Libera recursivamente toda a memoria alocada para os nos da arvore.
 */
void arvore_liberar(NoArvore *raiz) {
    if (raiz != NULL) {
        arvore_liberar(raiz->esquerda);
        arvore_liberar(raiz->direita);
        free(raiz);
    }
}
