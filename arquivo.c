/* arquivo.c: leitura do CSV e leitura/gravacao do arquivo binario. */

#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tamanho maximo de uma linha do CSV. */
#define TAM_LINHA_BUFFER 512

/* Quantos alimentos cabem no vetor antes do primeiro realloc. */
#define CAPACIDADE_INICIAL 64

/* Quantos campos cada linha do CSV tem. */
#define CAMPOS_POR_LINHA 5

static int linha_para_alimento(char *linha, Alimento_t *alimento);
static void remover_quebra_linha(char *linha);
static Alimento_t *vetor_aumentar(Alimento_t *vetor, int *capacidade);

/* Le o CSV linha a linha num vetor que dobra de tamanho quando enche. */
Alimento_t *arquivo_ler_csv(const char *caminho, int *total)
{
    Alimento_t *vetor = NULL;
    FILE *arquivo = NULL;
    int total_lidos = -1;
    int capacidade = CAPACIDADE_INICIAL;
    char buffer[TAM_LINHA_BUFFER];
    Alimento_t lido;

    if (caminho) {
        arquivo = fopen(caminho, "r");
    }

    if (arquivo) {
        vetor = calloc((size_t)capacidade, sizeof(Alimento_t));
        if (!vetor) {
            fprintf(stderr, "ERRO FATAL: Memoria insuficiente para ler '%s'.\n",
                    caminho);
            exit(EXIT_FAILURE);
        }
        total_lidos = 0;

        if (fgets(buffer, sizeof(buffer), arquivo)) {
            while (fgets(buffer, sizeof(buffer), arquivo)) {
                if (linha_para_alimento(buffer, &lido)) {
                    if (total_lidos == capacidade) {
                        vetor = vetor_aumentar(vetor, &capacidade);
                    }
                    vetor[total_lidos] = lido;
                    total_lidos++;
                }
            }
        }

        fclose(arquivo);
    }

    if (total) {
        *total = total_lidos;
    }

    return vetor;
}

/* Converte uma linha numero;descricao;categoria;energia;proteina em alimento. */
static int linha_para_alimento(char *linha, Alimento_t *alimento)
{
    int convertido = 0;
    int numero = 0;
    char descricao[TAM_MAX_DESCRICAO];
    char categoria[TAM_MAX_DESCRICAO];
    double energia = 0.0;
    double proteina = 0.0;

    remover_quebra_linha(linha);

    /* NA/Tr/branco da TACO gravado como 0 */
    if (sscanf(linha, "%d;%127[^;];%127[^;];%lf;%lf", &numero, descricao,
               categoria, &energia, &proteina) == CAMPOS_POR_LINHA) {
        memset(alimento, 0, sizeof(Alimento_t));
        alimento->numero = numero;
        strncpy(alimento->descricao, descricao, TAM_MAX_DESCRICAO - 1);
        alimento->descricao[TAM_MAX_DESCRICAO - 1] = '\0';
        alimento->categoria = categoria_converter(categoria);
        alimento->energia_kcal = energia;
        alimento->proteina_g = proteina;
        convertido = 1;
    }

    return convertido;
}

/* Tira o '\n' e o '\r' do fim da linha. */
static void remover_quebra_linha(char *linha)
{
    size_t tamanho = strlen(linha);

    while (tamanho > 0 && (linha[tamanho - 1] == '\n' ||
                           linha[tamanho - 1] == '\r')) {
        linha[tamanho - 1] = '\0';
        tamanho--;
    }
}

/* Dobra a capacidade do vetor com realloc e zera a parte nova. */
static Alimento_t *vetor_aumentar(Alimento_t *vetor, int *capacidade)
{
    int nova_capacidade = *capacidade * 2;
    Alimento_t *novo = realloc(vetor, (size_t)nova_capacidade * sizeof(Alimento_t));

    if (!novo) {
        free(vetor);
        fprintf(stderr, "ERRO FATAL: Memoria insuficiente para aumentar o vetor.\n");
        exit(EXIT_FAILURE);
    }
    memset(novo + *capacidade, 0,
           (size_t)(nova_capacidade - *capacidade) * sizeof(Alimento_t));
    *capacidade = nova_capacidade;

    return novo;
}

/* Grava o vetor inteiro com fwrite; fclose com falha tambem conta como erro. */
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
            if (fclose(arquivo) == EOF) {
                total_gravados = -1;
            }
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
