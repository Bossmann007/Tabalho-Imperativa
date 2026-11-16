#include <stdio.h>
#include <stdlib.h>
#include "alimento.h"
#include "arquivo.h"

#define MAX_REGISTROS 600
#define ESPERADO_REGISTROS 597
#define ARQUIVO_CSV "alimentos.csv"
#define ARQUIVO_BIN "dados.bin"

/*
 * Exibe amostras dos alimentos lidos para conferência visual.
 */
static void exibir_amostras(const Alimento *vetor, int quantidade) {
    if (vetor != NULL && quantidade > 0) {
        printf("\n--- AMOSTRA DOS REGISTROS ---\n");
        printf("Primeiro registro:\n");
        alimento_exibir(&vetor[0]);

        if (quantidade >= 300) {
            printf("\nRegistro intermediario (indice 299):\n");
            alimento_exibir(&vetor[299]);
        }

        printf("\nUltimo registro:\n");
        alimento_exibir(&vetor[quantidade - 1]);
        printf("-----------------------------\n\n");
    }
}

/*
 * P1.c - Programa 1 da Etapa B (Trabalho RA2 - PUCPR).
 * Responsavel por:
 *  1. Ler os dados do arquivo CSV ('alimentos.csv').
 *  2. Gravar os dados em formato binario ('dados.bin').
 *  3. Validar a integridade lendo de volta e checando o tamanho exato em bytes.
 */
int main(void) {
    int codigo_retorno = EXIT_SUCCESS;
    static Alimento lista_original[MAX_REGISTROS];
    static Alimento lista_verificacao[MAX_REGISTROS];

    printf("=====================================================\n");
    printf("   PUCPR - Programacao Imperativa - Trabalho RA2     \n");
    printf("   Programa P1: Conversao CSV -> Binario (Etapa B)   \n");
    printf("=====================================================\n\n");

    /* 1. Leitura do arquivo CSV */
    printf("[1/3] Lendo arquivo CSV: '%s'...\n", ARQUIVO_CSV);
    int total_lidos = arquivo_ler_csv(ARQUIVO_CSV, lista_original, MAX_REGISTROS);

    if (total_lidos != ESPERADO_REGISTROS) {
        fprintf(stderr, "ERRO: Leitura do CSV falhou! Registros lidos: %d (esperado: %d)\n",
                total_lidos, ESPERADO_REGISTROS);
        codigo_retorno = EXIT_FAILURE;
    } else {
        printf("SUCESSO: %d registros lidos com sucesso do CSV!\n", total_lidos);

        /* 2. Gravacao em arquivo binario */
        printf("\n[2/3] Gravando em formato binario: '%s'...\n", ARQUIVO_BIN);
        int total_gravados = arquivo_gravar_binario(ARQUIVO_BIN, lista_original, total_lidos);

        if (total_gravados != total_lidos) {
            fprintf(stderr, "ERRO: Falha ao gravar no arquivo binario! Registros gravados: %d\n",
                    total_gravados);
            codigo_retorno = EXIT_FAILURE;
        } else {
            printf("SUCESSO: %d registros gravados em '%s'!\n", total_gravados, ARQUIVO_BIN);

            /* 3. Conferencia de integridade */
            printf("\n[3/3] Conferindo integridade de '%s'...\n", ARQUIVO_BIN);
            long tamanho_bytes = arquivo_obter_tamanho(ARQUIVO_BIN);
            long tamanho_esperado = (long)(ESPERADO_REGISTROS * sizeof(Alimento));

            printf("  - sizeof(Alimento): %zu bytes\n", sizeof(Alimento));
            printf("  - Tamanho do arquivo no disco: %ld bytes\n", tamanho_bytes);
            printf("  - Calculo (597 x %zu):        %ld bytes\n", sizeof(Alimento), tamanho_esperado);

            if (tamanho_bytes != tamanho_esperado) {
                fprintf(stderr, "ERRO: Tamanho do arquivo binario diverge do esperado!\n");
                codigo_retorno = EXIT_FAILURE;
            } else {
                int total_conferidos = arquivo_ler_binario(ARQUIVO_BIN, lista_verificacao, MAX_REGISTROS);

                if (total_conferidos != ESPERADO_REGISTROS) {
                    fprintf(stderr, "ERRO: Leitura de verificacao do binario falhou! Lidos: %d\n",
                            total_conferidos);
                    codigo_retorno = EXIT_FAILURE;
                } else {
                    printf("SUCESSO: Conferencia confirmada! %d registros validos.\n", total_conferidos);
                    exibir_amostras(lista_verificacao, total_conferidos);
                    printf("Status: Etapa B concluida com sucesso absoluto!\n");
                }
            }
        }
    }

    return codigo_retorno;
}
