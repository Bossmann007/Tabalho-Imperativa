/* P1.c: le alimentos.csv, grava dados.bin e confere o arquivo gravado. */

#include "alimento.h"
#include "arquivo.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>

/* Tamanho dos vetores; precisa ser maior ou igual ao total esperado. */
#define MAX_REGISTROS 600

/* Quantos alimentos a tabela TACO tem. */
#define ESPERADO_REGISTROS 597

#define ARQUIVO_CSV "alimentos.csv"
#define ARQUIVO_BIN "dados.bin"

/* Etapa B: CSV -> binario. Cada passo so roda se o anterior deu certo. */
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

    printf("[1/3] Lendo arquivo CSV: '%s'...\n", ARQUIVO_CSV);
    total_lidos = arquivo_ler_csv(ARQUIVO_CSV, lista_original, MAX_REGISTROS);

    if (total_lidos != ESPERADO_REGISTROS) {
        fprintf(stderr, "ERRO: Leitura do CSV falhou! Registros lidos: %d "
                "(esperado: %d)\n", total_lidos, ESPERADO_REGISTROS);
        codigo_retorno = EXIT_FAILURE;
    } else {
        int total_gravados;

        printf("SUCESSO: %d registros lidos com sucesso do CSV!\n",
               total_lidos);

        printf("\n[2/3] Gravando em formato binario: '%s'...\n", ARQUIVO_BIN);
        total_gravados = arquivo_gravar_binario(ARQUIVO_BIN, lista_original,
                                                total_lidos);

        if (total_gravados != total_lidos) {
            fprintf(stderr, "ERRO: Falha ao gravar no arquivo binario! "
                    "Registros gravados: %d\n", total_gravados);
            codigo_retorno = EXIT_FAILURE;
        } else {
            long tamanho_bytes;
            long tamanho_esperado;

            printf("SUCESSO: %d registros gravados em '%s'!\n",
                   total_gravados, ARQUIVO_BIN);

            /* O arquivo deve ter exatamente 597 registros do tamanho
             * de Alimento_t; depois lemos tudo de volta. */
            printf("\n[3/3] Conferindo integridade de '%s'...\n",
                   ARQUIVO_BIN);
            tamanho_bytes = arquivo_obter_tamanho(ARQUIVO_BIN);
            tamanho_esperado =
                (long)(ESPERADO_REGISTROS * sizeof(Alimento_t));

            printf("  - sizeof(Alimento): %zu bytes\n", sizeof(Alimento_t));
            printf("  - Tamanho do arquivo no disco: %ld bytes\n",
                   tamanho_bytes);
            printf("  - Calculo (%d x %zu):        %ld bytes\n",
                   ESPERADO_REGISTROS, sizeof(Alimento_t), tamanho_esperado);

            if (tamanho_bytes != tamanho_esperado) {
                fprintf(stderr, "ERRO: Tamanho do arquivo binario diverge "
                        "do esperado!\n");
                codigo_retorno = EXIT_FAILURE;
            } else {
                int total_conferidos =
                    arquivo_ler_binario(ARQUIVO_BIN, lista_verificacao,
                                        MAX_REGISTROS);

                if (total_conferidos != ESPERADO_REGISTROS) {
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
        }
    }

    return codigo_retorno;
}
