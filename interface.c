/* interface.c: menu e leitura de dados do teclado (P2). */

#include "interface.h"
#include <stdio.h>

/* Tamanho maximo de uma linha digitada pelo usuario. */
#define TAM_ENTRADA 64

/* Mostra as opcoes de 1 a 9. */
void exibir_menu_principal(void)
{
    printf("\n==================== MENU PRINCIPAL (P2) ====================\n");
    printf("%d. Listar todas as categorias de alimentos (ordem alfabetica)\n",
           OPCAO_LISTAR_CATEGORIAS);
    printf("%d. Listar alimentos de uma categoria (ordem alfabetica)\n",
           OPCAO_LISTAR_ALIMENTOS);
    printf("%d. Listar alimentos de uma categoria por energia decrescente "
           "(arvore)\n", OPCAO_LISTAR_POR_ENERGIA);
    printf("%d. Listar alimentos de uma categoria por proteina decrescente "
           "(arvore)\n", OPCAO_LISTAR_POR_PROTEINA);
    printf("%d. Filtrar alimentos por faixa de energia [min, max] "
           "(arvore)\n", OPCAO_FAIXA_ENERGIA);
    printf("%d. Filtrar alimentos por faixa de proteina [min, max] "
           "(arvore)\n", OPCAO_FAIXA_PROTEINA);
    printf("%d. Remover uma categoria de alimentos\n",
           OPCAO_REMOVER_CATEGORIA);
    printf("%d. Remover um alimento especifico\n", OPCAO_REMOVER_ALIMENTO);
    printf("%d. Encerrar o programa (salvando alteracoes se houver)\n",
           OPCAO_ENCERRAR);
    printf("=============================================================\n");
    printf("Escolha uma opcao (%d-%d): ", OPCAO_LISTAR_CATEGORIAS,
           OPCAO_ENCERRAR);
}

/* Le um inteiro e so converte para o enum se estiver entre as opcoes. */
OpcaoMenu_e ler_opcao_menu(void)
{
    OpcaoMenu_e opcao = OPCAO_INVALIDA;
    int valor = ler_inteiro(OPCAO_ENCERRAR);

    if (valor >= OPCAO_LISTAR_CATEGORIAS && valor <= OPCAO_ENCERRAR) {
        opcao = (OpcaoMenu_e)valor;
    }

    return opcao;
}

/* Le a linha inteira com fgets e tira o numero com sscanf.
 * No fim da entrada devolve valor_fim_entrada; no menu isso e
 * OPCAO_ENCERRAR, e o programa termina em vez de ficar em loop. */
int ler_inteiro(int valor_fim_entrada)
{
    char buffer[TAM_ENTRADA];
    int valor = LEITURA_INVALIDA;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        sscanf(buffer, "%d", &valor);
    } else {
        valor = valor_fim_entrada;
    }

    return valor;
}

/* Le a linha inteira com fgets e tira o numero com sscanf. */
double ler_double(void)
{
    char buffer[TAM_ENTRADA];
    double valor = 0.0;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        sscanf(buffer, "%lf", &valor);
    }

    return valor;
}

/* Pede minimo e maximo e garante que *min <= *max. */
void ler_faixa(const char *grandeza, double *min, double *max)
{
    printf("Informe o valor MINIMO de %s: ", grandeza);
    *min = ler_double();
    printf("Informe o valor MAXIMO de %s: ", grandeza);
    *max = ler_double();

    if (*min > *max) {
        double temp = *min;
        *min = *max;
        *max = temp;
    }
}

/* Mostra a lista de categorias e le o numero escolhido.
 * No fim da entrada le LEITURA_INVALIDA. */
NoCategoria_t *selecionar_categoria(NoCategoria_t *cabeca)
{
    NoCategoria_t *selecionada = NULL;
    int indice;

    lista_exibir_categorias(cabeca);
    printf("Digite o numero da categoria desejada: ");
    indice = ler_inteiro(LEITURA_INVALIDA);

    selecionada = lista_obter_categoria_por_indice(cabeca, indice);
    if (!selecionada) {
        printf("AVISO: Categoria invalida!\n");
    }

    return selecionada;
}
