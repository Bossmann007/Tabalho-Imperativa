/* lista.c: listas ligadas de categorias e de alimentos. */

#include "lista.h"
#include "arquivo.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Em UTF-8 as letras acentuadas usadas aqui tem 2 bytes: 0xC3 e depois
 * um byte entre 0x80 e 0xBF. Maiuscula e minuscula diferem em 0x20. */
#define UTF8_PRIMEIRO_BYTE 0xC3
#define UTF8_SEGUNDO_MIN 0x80
#define UTF8_SEGUNDO_MAX 0xBF
#define UTF8_MINUSCULA_INICIO 0xA0
#define UTF8_DIF_MAIUSCULA 0x20

/* Letra base de cada minuscula de 0xC3 0xA0 (a com crase) ate 0xC3 0xBF.
 * O ponto marca as que nao tratamos. */
static const char BASE_ACENTOS[] = "aaaaa..ceeeeiiii..ooooo..uuuu...";

static void lista_inserir_alimento(NoCategoria_t **cabeca_categorias,
                                   const Alimento_t *alimento);
static NoCategoria_t *lista_obter_ou_criar_categoria(NoCategoria_t **cabeca,
                                                     Categoria_e cat,
                                                     const char *nome);
static void categoria_reconstruir_arvores(NoCategoria_t *cat);
static void categoria_liberar(NoCategoria_t *cat);
static int letra_base(const char **texto);
static int comparar_texto(const char *a, const char *b);

/* Le dados.bin inteiro para um vetor e insere alimento por alimento. */
NoCategoria_t *lista_carregar_de_binario(const char *caminho_bin,
                                         int *total_lidos)
{
    NoCategoria_t *cabeca = NULL;
    int contador = 0;
    long tamanho = arquivo_obter_tamanho(caminho_bin);

    if (tamanho >= (long)sizeof(Alimento_t)) {
        int capacidade = (int)(tamanho / (long)sizeof(Alimento_t));
        Alimento_t *vetor = malloc((size_t)capacidade * sizeof(Alimento_t));
        int lidos;
        int i;

        if (!vetor) {
            fprintf(stderr, "ERRO FATAL: Memoria insuficiente para ler '%s'.\n",
                    caminho_bin);
            exit(EXIT_FAILURE);
        }

        lidos = arquivo_ler_binario(caminho_bin, vetor, capacidade);
        for (i = 0; i < lidos; i++) {
            lista_inserir_alimento(&cabeca, &vetor[i]);
            contador++;
        }

        free(vetor);
    }

    if (total_lidos) {
        *total_lidos = contador;
    }

    return cabeca;
}

/* Coloca o alimento na lista da sua categoria, ja na posicao certa da
 * ordem alfabetica, e tambem nas duas arvores da categoria. */
static void lista_inserir_alimento(NoCategoria_t **cabeca_categorias,
                                   const Alimento_t *alimento)
{
    if (cabeca_categorias && alimento) {
        const char *nome_cat = categoria_para_texto(alimento->categoria);
        NoCategoria_t *categoria =
            lista_obter_ou_criar_categoria(cabeca_categorias,
                                           alimento->categoria, nome_cat);

        if (categoria) {
            NoAlimento_t *novo = malloc(sizeof(NoAlimento_t));
            NoAlimento_t *anterior = NULL;
            NoAlimento_t *atual = categoria->alimentos;
            int posicao_encontrada = 0;

            if (!novo) {
                fprintf(stderr, "ERRO FATAL: Memoria insuficiente para o no "
                        "do alimento.\n");
                exit(EXIT_FAILURE);
            }
            novo->dado = *alimento;
            novo->proximo = NULL;

            /* Anda ate o primeiro alimento que vem depois do novo
             * em ordem alfabetica; o novo entra antes dele. */
            while (atual && !posicao_encontrada) {
                if (comparar_texto(novo->dado.descricao,
                                   atual->dado.descricao) < 0) {
                    posicao_encontrada = 1;
                } else {
                    anterior = atual;
                    atual = atual->proximo;
                }
            }

            if (!anterior) {
                novo->proximo = categoria->alimentos;
                categoria->alimentos = novo;
            } else {
                novo->proximo = atual;
                anterior->proximo = novo;
            }

            categoria->total_alimentos++;

            categoria->arvore_energia =
                arvore_inserir(categoria->arvore_energia,
                               novo->dado.energia_kcal, novo);
            categoria->arvore_proteina =
                arvore_inserir(categoria->arvore_proteina,
                               novo->dado.proteina_g, novo);
        }
    }
}

/* Devolve a categoria se ela ja existe. Se nao existe, cria e coloca
 * na lista em ordem alfabetica do nome. */
static NoCategoria_t *lista_obter_ou_criar_categoria(NoCategoria_t **cabeca,
                                                     Categoria_e cat,
                                                     const char *nome)
{
    NoCategoria_t *resultado = NULL;
    NoCategoria_t *atual = *cabeca;
    int encontrado = 0;

    while (atual && !encontrado) {
        if (atual->categoria == cat) {
            resultado = atual;
            encontrado = 1;
        } else {
            atual = atual->proximo;
        }
    }

    if (!encontrado) {
        NoCategoria_t *novo = malloc(sizeof(NoCategoria_t));
        NoCategoria_t *anterior_cat = NULL;
        NoCategoria_t *atual_cat = *cabeca;
        int posicao_encontrada = 0;

        if (!novo) {
            fprintf(stderr, "ERRO FATAL: Memoria insuficiente para o no "
                    "da categoria.\n");
            exit(EXIT_FAILURE);
        }
        novo->categoria = cat;
        strncpy(novo->nome, nome, TAM_MAX_DESCRICAO - 1);
        novo->nome[TAM_MAX_DESCRICAO - 1] = '\0';
        novo->alimentos = NULL;
        novo->arvore_energia = NULL;
        novo->arvore_proteina = NULL;
        novo->total_alimentos = 0;
        novo->proximo = NULL;

        /* Mesma ideia da insercao de alimentos: para no primeiro nome
         * que vem depois do novo. */
        while (atual_cat && !posicao_encontrada) {
            if (comparar_texto(novo->nome, atual_cat->nome) < 0) {
                posicao_encontrada = 1;
            } else {
                anterior_cat = atual_cat;
                atual_cat = atual_cat->proximo;
            }
        }

        if (!anterior_cat) {
            novo->proximo = *cabeca;
            *cabeca = novo;
        } else {
            novo->proximo = atual_cat;
            anterior_cat->proximo = novo;
        }

        resultado = novo;
    }

    return resultado;
}

/* Mostra cada categoria com sua posicao e quantos alimentos tem. */
void lista_exibir_categorias(const NoCategoria_t *cabeca)
{
    const NoCategoria_t *atual;
    int indice = 1;

    printf("\n=== LISTA DE CATEGORIAS (Ordem Alfabetica) ===\n");
    for (atual = cabeca; atual; atual = atual->proximo) {
        printf("  [%2d] %-40s (%d alimentos)\n",
               indice, atual->nome, atual->total_alimentos);
        indice++;
    }
    printf("==============================================\n\n");
}

/* Anda pela lista ate a posicao pedida. */
NoCategoria_t *lista_obter_categoria_por_indice(NoCategoria_t *cabeca,
                                                int indice)
{
    NoCategoria_t *resultado = NULL;
    NoCategoria_t *atual = cabeca;
    int pos = 1;
    int encontrado = 0;

    while (atual && !encontrado) {
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

/* Mostra os alimentos na ordem da lista, que ja e alfabetica. */
void lista_exibir_alimentos_categoria(const NoCategoria_t *categoria)
{
    if (categoria) {
        const NoAlimento_t *atual;
        int contador = 0;

        printf("\n--- ALIMENTOS DA CATEGORIA: %s (Ordem Alfabetica) ---\n",
               categoria->nome);
        for (atual = categoria->alimentos; atual; atual = atual->proximo) {
            contador++;
            alimento_exibir(&(atual->dado));
        }
        printf("Total: %d alimentos exibidos.\n"
               "----------------------------------------------------\n\n",
               contador);
    }
}

/* Tira o alimento da lista, libera o no e monta as arvores de novo. */
int lista_remover_alimento(NoCategoria_t *categoria, int numero_alimento)
{
    int removido = 0;

    if (categoria && categoria->alimentos) {
        NoAlimento_t *anterior = NULL;
        NoAlimento_t *atual = categoria->alimentos;
        int encontrado = 0;

        while (atual && !encontrado) {
            if (atual->dado.numero == numero_alimento) {
                encontrado = 1;
            } else {
                anterior = atual;
                atual = atual->proximo;
            }
        }

        if (encontrado && atual) {
            if (!anterior) {
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

/* Apaga as duas arvores e insere de novo os alimentos que sobraram. */
static void categoria_reconstruir_arvores(NoCategoria_t *cat)
{
    if (cat) {
        NoAlimento_t *atual;

        arvore_liberar(cat->arvore_energia);
        arvore_liberar(cat->arvore_proteina);
        cat->arvore_energia = NULL;
        cat->arvore_proteina = NULL;

        for (atual = cat->alimentos; atual; atual = atual->proximo) {
            cat->arvore_energia = arvore_inserir(cat->arvore_energia,
                                                 atual->dado.energia_kcal,
                                                 atual);
            cat->arvore_proteina = arvore_inserir(cat->arvore_proteina,
                                                  atual->dado.proteina_g,
                                                  atual);
        }
    }
}

/* Tira a categoria da lista e libera tudo o que ela tem. */
int lista_remover_categoria(NoCategoria_t **cabeca_categorias,
                            Categoria_e cat)
{
    int removido = 0;

    if (cabeca_categorias && *cabeca_categorias) {
        NoCategoria_t *anterior = NULL;
        NoCategoria_t *atual = *cabeca_categorias;
        int encontrado = 0;

        while (atual && !encontrado) {
            if (atual->categoria == cat) {
                encontrado = 1;
            } else {
                anterior = atual;
                atual = atual->proximo;
            }
        }

        if (encontrado && atual) {
            if (!anterior) {
                *cabeca_categorias = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            categoria_liberar(atual);
            removido = 1;
        }
    }

    return removido;
}

/* Copia os alimentos de todas as categorias, na ordem das listas,
 * para um vetor e grava esse vetor com arquivo_gravar_binario. */
int lista_salvar_binario(const NoCategoria_t *cabeca, const char *caminho_bin)
{
    int quantidade = 0;
    int total_gravados;
    const NoCategoria_t *cat;
    Alimento_t *vetor = NULL;

    for (cat = cabeca; cat; cat = cat->proximo) {
        quantidade += cat->total_alimentos;
    }

    if (quantidade > 0) {
        int i = 0;

        vetor = malloc((size_t)quantidade * sizeof(Alimento_t));
        if (!vetor) {
            fprintf(stderr,
                    "ERRO FATAL: Memoria insuficiente para gravar '%s'.\n",
                    caminho_bin);
            exit(EXIT_FAILURE);
        }

        for (cat = cabeca; cat; cat = cat->proximo) {
            const NoAlimento_t *alim;
            for (alim = cat->alimentos; alim; alim = alim->proximo) {
                vetor[i] = alim->dado;
                i++;
            }
        }
    }

    total_gravados = arquivo_gravar_binario(caminho_bin, vetor, quantidade);
    if (total_gravados != quantidade) {
        total_gravados = -1;
    }
    free(vetor);

    return total_gravados;
}

/* Libera categoria por categoria e deixa a lista vazia. */
void lista_liberar_tudo(NoCategoria_t **cabeca)
{
    if (cabeca) {
        NoCategoria_t *cat_atual = *cabeca;

        while (cat_atual) {
            NoCategoria_t *prox_cat = cat_atual->proximo;
            categoria_liberar(cat_atual);
            cat_atual = prox_cat;
        }

        *cabeca = NULL;
    }
}

/* Libera as duas arvores, a lista de alimentos e o no da categoria.
 * Usada ao remover uma categoria e ao liberar tudo no fim. */
static void categoria_liberar(NoCategoria_t *cat)
{
    if (cat) {
        NoAlimento_t *alim_atual = cat->alimentos;

        arvore_liberar(cat->arvore_energia);
        arvore_liberar(cat->arvore_proteina);

        while (alim_atual) {
            NoAlimento_t *prox_alim = alim_atual->proximo;
            free(alim_atual);
            alim_atual = prox_alim;
        }

        free(cat);
    }
}

/* Le uma letra de *texto e avanca o ponteiro. Devolve a letra minuscula
 * e sem acento (a letra acentuada ocupa 2 bytes em UTF-8). */
static int letra_base(const char **texto)
{
    const unsigned char *p = (const unsigned char *)*texto;
    int letra = tolower(p[0]);
    int tamanho = 1;

    if (p[0] == UTF8_PRIMEIRO_BYTE && p[1] >= UTF8_SEGUNDO_MIN &&
        p[1] <= UTF8_SEGUNDO_MAX) {
        int indice = (p[1] | UTF8_DIF_MAIUSCULA) - UTF8_MINUSCULA_INICIO;

        if (BASE_ACENTOS[indice] != '.') {
            letra = BASE_ACENTOS[indice];
            tamanho = 2;
        }
    }

    *texto += tamanho;
    return letra;
}

/* Compara como o strcmp, mas ignora acento e maiuscula. */
static int comparar_texto(const char *a, const char *b)
{
    int letra_a = letra_base(&a);
    int letra_b = letra_base(&b);

    while (letra_a == letra_b && letra_a != '\0') {
        letra_a = letra_base(&a);
        letra_b = letra_base(&b);
    }

    return letra_a - letra_b;
}
