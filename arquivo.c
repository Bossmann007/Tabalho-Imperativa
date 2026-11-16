#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

#define TAM_LINHA_BUFFER 512

/*
 * Remove os caracteres de quebra de linha ('\r' e '\n') do final de uma string.
 * Nao utiliza break nem continue.
 */
static void remover_quebra_linha(char *linha) {
    if (linha != NULL) {
        size_t tamanho = strlen(linha);
        while (tamanho > 0 && (linha[tamanho - 1] == '\n' || linha[tamanho - 1] == '\r')) {
            linha[tamanho - 1] = '\0';
            tamanho--;
        }
    }
}

/*
 * Le registros de alimentos de um arquivo CSV com separador ';'.
 * Suporta fallback automatico para extensao .txt se necessario.
 */
int arquivo_ler_csv(const char *caminho, Alimento *vetor, int capacidade) {
    int total_lidos = -1;
    FILE *arquivo = NULL;

    if (caminho != NULL && vetor != NULL && capacidade > 0) {
        arquivo = fopen(caminho, "r");

        /* Fallback: caso o arquivo tenha sido salvo como .csv.txt */
        if (arquivo == NULL && strstr(caminho, ".txt") == NULL) {
            char caminho_alternativo[260];
            snprintf(caminho_alternativo, sizeof(caminho_alternativo), "%s.txt", caminho);
            arquivo = fopen(caminho_alternativo, "r");
        }

        if (arquivo != NULL) {
            char buffer[TAM_LINHA_BUFFER];
            int continuar = 1;
            total_lidos = 0;

            /* Descarta a primeira linha (cabecalho: numero;descricao;...) */
            if (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
                while (continuar && fgets(buffer, sizeof(buffer), arquivo) != NULL) {
                    if (total_lidos < capacidade) {
                        int numero = 0;
                        char desc[TAM_MAX_DESCRICAO];
                        char cat_texto[TAM_MAX_DESCRICAO];
                        double kcal = 0.0;
                        double prot = 0.0;

                        remover_quebra_linha(buffer);

                        /* Formato: numero;descricao;categoria;energia_kcal;proteina_g */
                        if (sscanf(buffer, "%d;%127[^;];%127[^;];%lf;%lf",
                                   &numero, desc, cat_texto, &kcal, &prot) == 5) {
                            vetor[total_lidos].numero = numero;
                            strncpy(vetor[total_lidos].descricao, desc, TAM_MAX_DESCRICAO - 1);
                            vetor[total_lidos].descricao[TAM_MAX_DESCRICAO - 1] = '\0';
                            vetor[total_lidos].categoria = categoria_converter(cat_texto);
                            vetor[total_lidos].energia_kcal = kcal;
                            vetor[total_lidos].proteina_g = prot;
                            total_lidos++;
                        }
                    } else {
                        /* Capacidade maxima do vetor atingida */
                        continuar = 0;
                    }
                }
            }

            fclose(arquivo);
        }
    }

    return total_lidos;
}

/*
 * Grava o vetor de alimentos em formato binario direto.
 */
int arquivo_gravar_binario(const char *caminho, const Alimento *vetor, int quantidade) {
    int total_gravados = -1;
    FILE *arquivo = NULL;

    if (caminho != NULL && vetor != NULL && quantidade > 0) {
        arquivo = fopen(caminho, "wb");
        if (arquivo != NULL) {
            size_t gravados = fwrite(vetor, sizeof(Alimento), (size_t)quantidade, arquivo);
            total_gravados = (int)gravados;
            fclose(arquivo);
        }
    }

    return total_gravados;
}

/*
 * Le alimentos previamente gravados em formato binario.
 */
int arquivo_ler_binario(const char *caminho, Alimento *vetor, int capacidade) {
    int total_lidos = -1;
    FILE *arquivo = NULL;

    if (caminho != NULL && vetor != NULL && capacidade > 0) {
        arquivo = fopen(caminho, "rb");
        if (arquivo != NULL) {
            size_t lidos = fread(vetor, sizeof(Alimento), (size_t)capacidade, arquivo);
            total_lidos = (int)lidos;
            fclose(arquivo);
        }
    }

    return total_lidos;
}

/*
 * Retorna o tamanho total do arquivo em bytes.
 */
long arquivo_obter_tamanho(const char *caminho) {
    long tamanho = -1;
    FILE *arquivo = NULL;

    if (caminho != NULL) {
        arquivo = fopen(caminho, "rb");
        if (arquivo != NULL) {
            if (fseek(arquivo, 0, SEEK_END) == 0) {
                tamanho = ftell(arquivo);
            }
            fclose(arquivo);
        }
    }

    return tamanho;
}
