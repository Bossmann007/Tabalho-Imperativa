/* arquivo.c: leitura do CSV e leitura/gravacao do arquivo binario. */

#include "arquivo.h"
#include <stdio.h>
#include <string.h>

/* Tamanho maximo de uma linha do CSV. */
#define TAM_LINHA_BUFFER 512

/* Tamanho maximo do caminho alternativo "<caminho>.txt". */
#define TAM_CAMINHO 260

static void remover_quebra_linha(char *linha);

/* Le o CSV linha a linha. Se o arquivo nao abrir, tenta o mesmo nome
 * com ".txt" no fim (caso tenha sido salvo como alimentos.csv.txt). */
int arquivo_ler_csv(const char *caminho, Alimento_t *vetor, int capacidade)
{
    int total_lidos = -1;
    FILE *arquivo = NULL;

    if (caminho && vetor && capacidade > 0) {
        arquivo = fopen(caminho, "r");

        if (!arquivo && !strstr(caminho, ".txt")) {
            char caminho_alternativo[TAM_CAMINHO];
            snprintf(caminho_alternativo, sizeof(caminho_alternativo),
                     "%s.txt", caminho);
            arquivo = fopen(caminho_alternativo, "r");
        }

        if (arquivo) {
            char buffer[TAM_LINHA_BUFFER];
            int continuar = 1;
            total_lidos = 0;

            /* A primeira linha e o cabecalho e e descartada. */
            if (fgets(buffer, sizeof(buffer), arquivo)) {
                while (continuar && fgets(buffer, sizeof(buffer), arquivo)) {
                    if (total_lidos < capacidade) {
                        int numero = 0;
                        char desc[TAM_MAX_DESCRICAO];
                        char cat_texto[TAM_MAX_DESCRICAO];
                        double kcal = 0.0;
                        double prot = 0.0;

                        remover_quebra_linha(buffer);

                        /* numero;descricao;categoria;energia;proteina */
                        if (sscanf(buffer, "%d;%127[^;];%127[^;];%lf;%lf",
                                   &numero, desc, cat_texto,
                                   &kcal, &prot) == 5) {
                            Alimento_t *novo = &vetor[total_lidos];
                            novo->numero = numero;
                            strncpy(novo->descricao, desc,
                                    TAM_MAX_DESCRICAO - 1);
                            novo->descricao[TAM_MAX_DESCRICAO - 1] = '\0';
                            novo->categoria = categoria_converter(cat_texto);
                            novo->energia_kcal = kcal;
                            novo->proteina_g = prot;
                            total_lidos++;
                        }
                    } else {
                        /* O vetor encheu. */
                        continuar = 0;
                    }
                }
            }

            fclose(arquivo);
        }
    }

    return total_lidos;
}

/* Tira o '\n' e o '\r' do fim da linha. */
static void remover_quebra_linha(char *linha)
{
    if (linha) {
        size_t tamanho = strlen(linha);
        while (tamanho > 0 && (linha[tamanho - 1] == '\n' ||
                               linha[tamanho - 1] == '\r')) {
            linha[tamanho - 1] = '\0';
            tamanho--;
        }
    }
}

/* Grava o vetor inteiro de uma vez com fwrite. */
int arquivo_gravar_binario(const char *caminho, const Alimento_t *vetor,
                           int quantidade)
{
    int total_gravados = -1;
    FILE *arquivo = NULL;

    if (caminho && quantidade >= 0 && (vetor || quantidade == 0)) {
        arquivo = fopen(caminho, "wb");
        if (arquivo) {
            total_gravados = 0;
            if (quantidade > 0) {
                total_gravados = (int)fwrite(vetor, sizeof(Alimento_t),
                                             (size_t)quantidade, arquivo);
            }
            fclose(arquivo);
        }
    }

    return total_gravados;
}

/* Le os registros com fread, no maximo capacidade. */
int arquivo_ler_binario(const char *caminho, Alimento_t *vetor,
                        int capacidade)
{
    int total_lidos = -1;
    FILE *arquivo = NULL;

    if (caminho && vetor && capacidade > 0) {
        arquivo = fopen(caminho, "rb");
        if (arquivo) {
            total_lidos = (int)fread(vetor, sizeof(Alimento_t),
                                     (size_t)capacidade, arquivo);
            fclose(arquivo);
        }
    }

    return total_lidos;
}

/* Vai ao fim do arquivo e usa ftell para saber o tamanho. */
long arquivo_obter_tamanho(const char *caminho)
{
    long tamanho = -1;
    FILE *arquivo = NULL;

    if (caminho) {
        arquivo = fopen(caminho, "rb");
        if (arquivo) {
            if (fseek(arquivo, 0, SEEK_END) == 0) {
                tamanho = ftell(arquivo);
            }
            fclose(arquivo);
        }
    }

    return tamanho;
}
