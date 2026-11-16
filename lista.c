#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

/*
 * Localiza ou cria um no de categoria, inserindo-o na lista de categorias
 * em estrita ordem alfabetica pelo nome da categoria.
 * Nao utiliza break nem continue. Possui um unico return no final.
 */
NoCategoria *lista_obter_ou_criar_categoria(NoCategoria **cabeca, Categoria cat, const char *nome) {
    NoCategoria *resultado = NULL;
    NoCategoria *atual = *cabeca;
    int encontrado = 0;

    /* 1. Procura se a categoria ja existe na lista */
    while (atual != NULL && !encontrado) {
        if (atual->categoria == cat) {
            resultado = atual;
            encontrado = 1;
        } else {
            atual = atual->proximo;
        }
    }

    /* 2. Se nao existir, cria e insere mantendo ordem alfabetica */
    if (!encontrado) {
        NoCategoria *novo = (NoCategoria *)malloc(sizeof(NoCategoria));
        if (novo != NULL) {
            novo->categoria = cat;
            strncpy(novo->nome, nome, TAM_MAX_DESCRICAO - 1);
            novo->nome[TAM_MAX_DESCRICAO - 1] = '\0';
            novo->alimentos = NULL;
            novo->arvore_energia = NULL;
            novo->arvore_proteina = NULL;
            novo->total_alimentos = 0;
            novo->proximo = NULL;

            /* Insercao ordenada na lista de categorias */
            NoCategoria *anterior_cat = NULL;
            NoCategoria *atual_cat = *cabeca;
            int posicao_encontrada = 0;

            while (atual_cat != NULL && !posicao_encontrada) {
                if (strcmp(novo->nome, atual_cat->nome) < 0) {
                    posicao_encontrada = 1;
                } else {
                    anterior_cat = atual_cat;
                    atual_cat = atual_cat->proximo;
                }
            }

            if (anterior_cat == NULL) {
                novo->proximo = *cabeca;
                *cabeca = novo;
            } else {
                novo->proximo = atual_cat;
                anterior_cat->proximo = novo;
            }

            resultado = novo;
        }
    }

    return resultado;
}

/*
 * Insere um alimento na sua categoria, mantendo a lista de alimentos
 * ordenada alfabeticamente pela descricao do alimento.
 * Tambem indexa imediatamente o alimento nas duas arvores binarias (energia e proteina).
 */
void lista_inserir_alimento(NoCategoria **cabeca_categorias, const Alimento *alimento) {
    if (cabeca_categorias != NULL && alimento != NULL) {
        const char *nome_cat = categoria_para_texto(alimento->categoria);
        NoCategoria *categoria = lista_obter_ou_criar_categoria(cabeca_categorias, alimento->categoria, nome_cat);

        if (categoria != NULL) {
            NoAlimento *novo = (NoAlimento *)malloc(sizeof(NoAlimento));
            if (novo != NULL) {
                novo->dado = *alimento;
                novo->proximo = NULL;

                /* Insercao ordenada na lista de alimentos pela descricao */
                NoAlimento *anterior = NULL;
                NoAlimento *atual = categoria->alimentos;
                int posicao_encontrada = 0;

                while (atual != NULL && !posicao_encontrada) {
                    if (strcmp(novo->dado.descricao, atual->dado.descricao) < 0) {
                        posicao_encontrada = 1;
                    } else {
                        anterior = atual;
                        atual = atual->proximo;
                    }
                }

                if (anterior == NULL) {
                    novo->proximo = categoria->alimentos;
                    categoria->alimentos = novo;
                } else {
                    novo->proximo = atual;
                    anterior->proximo = novo;
                }

                categoria->total_alimentos++;

                /* Indexacao nas arvores binarias de energia e proteina */
                categoria->arvore_energia = arvore_inserir(categoria->arvore_energia,
                                                          novo->dado.energia_kcal,
                                                          novo);
                categoria->arvore_proteina = arvore_inserir(categoria->arvore_proteina,
                                                           novo->dado.proteina_g,
                                                           novo);
            }
        }
    }
}

/*
 * Le registros do arquivo binario dados.bin e preenche a estrutura encadeada.
 */
NoCategoria *lista_carregar_de_binario(const char *caminho_bin, int *total_lidos) {
    NoCategoria *cabeca = NULL;
    int contador = 0;
    FILE *arquivo = NULL;

    if (caminho_bin != NULL) {
        arquivo = fopen(caminho_bin, "rb");
        if (arquivo != NULL) {
            Alimento temp;
            int lendo = 1;

            while (lendo) {
                size_t lidos = fread(&temp, sizeof(Alimento), 1, arquivo);
                if (lidos == 1) {
                    lista_inserir_alimento(&cabeca, &temp);
                    contador++;
                } else {
                    lendo = 0;
                }
            }

            fclose(arquivo);
        }
    }

    if (total_lidos != NULL) {
        *total_lidos = contador;
    }

    return cabeca;
}

/*
 * Exibe todas as categorias na ordem em que estao armazenadas na lista (alfabetica).
 */
void lista_exibir_categorias(const NoCategoria *cabeca) {
    const NoCategoria *atual = cabeca;
    int indice = 1;

    printf("\n=== LISTA DE CATEGORIAS (Ordem Alfabetica) ===\n");
    while (atual != NULL) {
        printf("  [%2d] %-40s (%d alimentos)\n",
               indice,
               atual->nome,
               atual->total_alimentos);
        atual = atual->proximo;
        indice++;
    }
    printf("==============================================\n\n");
}

/*
 * Retorna o no da categoria correspondente ao indice informado (1 a N).
 */
NoCategoria *lista_obter_categoria_por_indice(NoCategoria *cabeca, int indice) {
    NoCategoria *resultado = NULL;
    NoCategoria *atual = cabeca;
    int pos = 1;
    int encontrado = 0;

    while (atual != NULL && !encontrado) {
        if (pos == indice) {
            resultado = atual;
            encontrado = 1;
        } else {
            atual = atual->proximo;
            pos++;
        }
    }

    return resultado;
}

/*
 * Exibe todos os alimentos de uma categoria na ordem da lista (ordem alfabetica).
 */
void lista_exibir_alimentos_categoria(const NoCategoria *categoria) {
    if (categoria != NULL) {
        const NoAlimento *atual = categoria->alimentos;
        int contador = 0;

        printf("\n--- ALIMENTOS DA CATEGORIA: %s (Ordem Alfabetica) ---\n", categoria->nome);
        while (atual != NULL) {
            contador++;
            alimento_exibir(&(atual->dado));
            atual = atual->proximo;
        }
        printf("Total: %d alimentos exibidos.\n----------------------------------------------------\n\n", contador);
    }
}

/*
 * Reconstrói as duas arvores binarias de indexacao de uma categoria a partir de sua lista de alimentos.
 */
void categoria_reconstruir_arvores(NoCategoria *cat) {
    if (cat != NULL) {
        /* Libera as arvores anteriores */
        arvore_liberar(cat->arvore_energia);
        arvore_liberar(cat->arvore_proteina);
        cat->arvore_energia = NULL;
        cat->arvore_proteina = NULL;

        /* Reinsere cada alimento restante na nova arvore */
        NoAlimento *atual = cat->alimentos;
        while (atual != NULL) {
            cat->arvore_energia = arvore_inserir(cat->arvore_energia,
                                                atual->dado.energia_kcal,
                                                atual);
            cat->arvore_proteina = arvore_inserir(cat->arvore_proteina,
                                                 atual->dado.proteina_g,
                                                 atual);
            atual = atual->proximo;
        }
    }
}

/*
 * Remove um alimento especifico pelo seu numero de identificacao.
 * Apos remover da lista ligada, reconstrói as arvores de indexacao.
 */
int lista_remover_alimento(NoCategoria *categoria, int numero_alimento) {
    int removido = 0;

    if (categoria != NULL && categoria->alimentos != NULL) {
        NoAlimento *anterior = NULL;
        NoAlimento *atual = categoria->alimentos;
        int encontrado = 0;

        while (atual != NULL && !encontrado) {
            if (atual->dado.numero == numero_alimento) {
                encontrado = 1;
            } else {
                anterior = atual;
                atual = atual->proximo;
            }
        }

        if (encontrado && atual != NULL) {
            if (anterior == NULL) {
                categoria->alimentos = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            free(atual);
            categoria->total_alimentos--;
            categoria_reconstruir_arvores(categoria);
            removido = 1;
        }
    }

    return removido;
}

/*
 * Remove uma categoria inteira, liberando todos os alimentos e arvores associados.
 */
int lista_remover_categoria(NoCategoria **cabeca_categorias, Categoria cat) {
    int removido = 0;

    if (cabeca_categorias != NULL && *cabeca_categorias != NULL) {
        NoCategoria *anterior = NULL;
        NoCategoria *atual = *cabeca_categorias;
        int encontrado = 0;

        while (atual != NULL && !encontrado) {
            if (atual->categoria == cat) {
                encontrado = 1;
            } else {
                anterior = atual;
                atual = atual->proximo;
            }
        }

        if (encontrado && atual != NULL) {
            if (anterior == NULL) {
                *cabeca_categorias = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            /* Libera as duas arvores */
            arvore_liberar(atual->arvore_energia);
            arvore_liberar(atual->arvore_proteina);

            /* Libera todos os alimentos da lista da categoria */
            NoAlimento *alimento_atual = atual->alimentos;
            while (alimento_atual != NULL) {
                NoAlimento *prox = alimento_atual->proximo;
                free(alimento_atual);
                alimento_atual = prox;
            }

            free(atual);
            removido = 1;
        }
    }

    return removido;
}

/*
 * Salva todos os alimentos atualmente em memoria no arquivo dados.bin.
 */
int lista_salvar_binario(const NoCategoria *cabeca, const char *caminho_bin) {
    int total_gravados = -1;
    FILE *arquivo = NULL;

    if (caminho_bin != NULL) {
        arquivo = fopen(caminho_bin, "wb");
        if (arquivo != NULL) {
            const NoCategoria *cat = cabeca;
            total_gravados = 0;

            while (cat != NULL) {
                const NoAlimento *alim = cat->alimentos;
                while (alim != NULL) {
                    size_t gravados = fwrite(&(alim->dado), sizeof(Alimento), 1, arquivo);
                    if (gravados == 1) {
                        total_gravados++;
                    }
                    alim = alim->proximo;
                }
                cat = cat->proximo;
            }

            fclose(arquivo);
        }
    }

    return total_gravados;
}

/*
 * Libera toda a memoria dinamicamente alocada para as categorias,
 * alimentos e arvores binarias de busca.
 */
void lista_liberar_tudo(NoCategoria **cabeca) {
    if (cabeca != NULL) {
        NoCategoria *cat_atual = *cabeca;

        while (cat_atual != NULL) {
            NoCategoria *prox_cat = cat_atual->proximo;

            /* Libera as duas arvores de busca da categoria */
            arvore_liberar(cat_atual->arvore_energia);
            arvore_liberar(cat_atual->arvore_proteina);

            /* Libera a lista ligada de alimentos da categoria */
            NoAlimento *alim_atual = cat_atual->alimentos;
            while (alim_atual != NULL) {
                NoAlimento *prox_alim = alim_atual->proximo;
                free(alim_atual);
                alim_atual = prox_alim;
            }

            /* Libera o no da categoria */
            free(cat_atual);
            cat_atual = prox_cat;
        }

        *cabeca = NULL;
    }
}
