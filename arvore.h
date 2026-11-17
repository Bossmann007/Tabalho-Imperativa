/* arvore.h: arvore binaria de busca que indexa os alimentos por energia ou proteina. */

#ifndef ARVORE_H
#define ARVORE_H

/* O no da arvore aponta para o no do alimento na lista (definido em lista.h). */
struct NoAlimento;

/* Um no da arvore: a chave (energia ou proteina) e o alimento dono dela. */
typedef struct NoArvore {
    double chave;
    struct NoAlimento *alimento;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore_t;

/* Insere a chave na arvore e devolve a raiz. */
NoArvore_t *arvore_inserir(NoArvore_t *raiz, double chave,
                           struct NoAlimento *alimento);

/* Mostra os alimentos da maior chave para a menor e conta quantos mostrou. */
void arvore_listar_decrescente(const NoArvore_t *raiz, int *contador);

/* Mostra, da maior chave para a menor, so os alimentos com chave entre min e max. */
void arvore_listar_faixa(const NoArvore_t *raiz, double min, double max,
                         int *contador);

/* Libera todos os nos da arvore. */
void arvore_liberar(NoArvore_t *raiz);

#endif /* ARVORE_H */
