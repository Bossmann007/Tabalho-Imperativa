/* P1.c: le alimentos.csv, grava dados.bin e confere o arquivo gravado. */

#include "alimento.h"
#include "arquivo.h"
#include "amostra.h"
#include <stdio.h>
#include <stdlib.h>

/* Maximo de alimentos que o P1 aceita do CSV. */
#define MAX_REGISTROS 600

#define ARQUIVO_CSV "alimentos.csv"
#define ARQUIVO_BIN "dados.bin"

/* Etapa B: CSV -> binario. Sao 3 passos em sequencia; cada um so roda
 * se os anteriores deram certo (codigo_retorno ainda e EXIT_SUCCESS). */
int main(void)
{
    static Alimento_t lista_original[MAX_REGISTROS];
    static Alimento_t lista_verificacao[MAX_REGISTROS];
    int codigo_retorno = EXIT_SUCCESS;
    int total_lidos;

    printf("=====================================================\n");
    printf("   PUCPR - Programacao Imperativa - Trabalho RA2     \n");
    printf("   Programa P1: Conversao CSV -> Binario (Etapa B)   \n");
    printf("=====================================================\n\n");

    /* Passo 1: le o CSV para o vetor. */
    printf("[1/3] Lendo arquivo CSV: '%s'...\n", ARQUIVO_CSV);
    total_lidos = arquivo_ler_csv(ARQUIVO_CSV, lista_original, MAX_REGISTROS);
    if (total_lidos <= 0) {
        fprintf(stderr, "ERRO: Leitura do CSV falhou! Nenhum registro "
                "lido.\n");
        codigo_retorno = EXIT_FAILURE;
    } else {
        printf("SUCESSO: %d registros lidos com sucesso do CSV!\n",
               total_lidos);
    }

    /* Passo 2: grava o vetor no arquivo binario. */
    if (codigo_retorno == EXIT_SUCCESS) {
        int total_gravados;

        printf("\n[2/3] Gravando em formato binario: '%s'...\n", ARQUIVO_BIN);
        total_gravados = arquivo_gravar_binario(ARQUIVO_BIN, lista_original,
                                                total_lidos);
        if (total_gravados != total_lidos) {
            fprintf(stderr, "ERRO: Falha ao gravar no arquivo binario! "
                    "Registros gravados: %d\n", total_gravados);
            codigo_retorno = EXIT_FAILURE;
        } else {
            printf("SUCESSO: %d registros gravados em '%s'!\n",
                   total_gravados, ARQUIVO_BIN);
        }
    }

    /* Passo 3a: o arquivo deve ter um registro de Alimento_t por
     * alimento lido. */
    if (codigo_retorno == EXIT_SUCCESS) {
        long tamanho_bytes;
        long tamanho_esperado;

        printf("\n[3/3] Conferindo integridade de '%s'...\n", ARQUIVO_BIN);
        tamanho_bytes = arquivo_obter_tamanho(ARQUIVO_BIN);
        tamanho_esperado = (long)((size_t)total_lidos * sizeof(Alimento_t));

        printf("  - sizeof(Alimento): %zu bytes\n", sizeof(Alimento_t));
        printf("  - Tamanho do arquivo no disco: %ld bytes\n", tamanho_bytes);
        printf("  - Calculo (%d x %zu):        %ld bytes\n",
               total_lidos, sizeof(Alimento_t), tamanho_esperado);

        if (tamanho_bytes != tamanho_esperado) {
            fprintf(stderr, "ERRO: Tamanho do arquivo binario diverge "
                    "do esperado!\n");
            codigo_retorno = EXIT_FAILURE;
        }
    }

    /* Passo 3b: le tudo de volta e confere a quantidade. */
    if (codigo_retorno == EXIT_SUCCESS) {
        int total_conferidos = arquivo_ler_binario(ARQUIVO_BIN,
                                                   lista_verificacao,
                                                   MAX_REGISTROS);

        if (total_conferidos != total_lidos) {
            fprintf(stderr, "ERRO: Leitura de verificacao do binario "
                    "falhou! Lidos: %d\n", total_conferidos);
            codigo_retorno = EXIT_FAILURE;
        } else {
            printf("SUCESSO: Conferencia confirmada! "
                   "%d registros validos.\n", total_conferidos);
            exibir_amostras(lista_verificacao, total_conferidos);
            printf("Status: Etapa B concluida com sucesso absoluto!\n");
        }
    }

    return codigo_retorno;
}
