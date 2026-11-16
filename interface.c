/* interface.c: menu e leitura de dados do teclado (P2) e amostra do P1. */

#include "interface.h"
#include <stdio.h>

/* Tamanho maximo de uma linha digitada pelo usuario. */
#define TAM_ENTRADA 64

/* Indice do registro do meio mostrado na amostra. */
#define INDICE_AMOSTRA_MEIO 299

/* Mostra tres alimentos do vetor para conferir a leitura. */
void exibir_amostras(const Alimento_t *vetor, int quantidade)
{
    if (vetor && quantidade > 0) {
        printf("\n--- AMOSTRA DOS REGISTROS ---\n");
        printf("Primeiro registro:\n");
        alimento_exibir(&vetor[0]);

        if (quantidade > INDICE_AMOSTRA_MEIO) {
            printf("\nRegistro intermediario (indice %d):\n",
                   INDICE_AMOSTRA_MEIO);
            alimento_exibir(&vetor[INDICE_AMOSTRA_MEIO]);
        }

        printf("\nUltimo registro:\n");
        alimento_exibir(&vetor[quantidade - 1]);
        printf("-----------------------------\n\n");
    }
}

/* Mostra as opcoes de 1 a 9. */
void exibir_menu_principal(void)
{
    printf("\n==================== MENU PRINCIPAL (P2) ====================\n");
    printf("1. Listar todas as categorias de alimentos (ordem alfabetica)\n");
    printf("2. Listar alimentos de uma categoria (ordem alfabetica)\n");
    printf("3. Listar alimentos de uma categoria por energia decrescente "
           "(arvore)\n");
    printf("4. Listar alimentos de uma categoria por proteina decrescente "
           "(arvore)\n");
    printf("5. Filtrar alimentos por faixa de energia [min, max] (arvore)\n");
    printf("6. Filtrar alimentos por faixa de proteina [min, max] (arvore)\n");
    printf("7. Remover uma categoria de alimentos\n");
    printf("8. Remover um alimento especifico\n");
    printf("9. Encerrar o programa (salvando alteracoes se houver)\n");
    printf("=============================================================\n");
    printf("Escolha uma opcao (1-9): ");
}

/* Le a linha inteira com fgets e tira o numero com sscanf.
 * No fim da entrada devolve valor_fim_entrada; no menu isso e
 * OPCAO_ENCERRAR, e o programa termina em vez de ficar em loop. */
int ler_inteiro(int valor_fim_entrada)
{
    char buffer[TAM_ENTRADA];
    int valor = 0;

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
 * No fim da entrada le 0, que e invalido. */
NoCategoria_t *selecionar_categoria(NoCategoria_t *cabeca)
{
    NoCategoria_t *selecionada = NULL;
    int indice;

    lista_exibir_categorias(cabeca);
    printf("Digite o numero da categoria desejada: ");
    indice = ler_inteiro(0);

    selecionada = lista_obter_categoria_por_indice(cabeca, indice);
    if (!selecionada) {
        printf("AVISO: Categoria invalida!\n");
    }

    return selecionada;
}
