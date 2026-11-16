/* P2.c: carrega dados.bin em listas e arvores e mostra o menu (Etapa C). */

#include "alimento.h"
#include "arvore.h"
#include "lista.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_DADOS_BIN "dados.bin"

/* Carrega os dados e repete o menu ate o usuario escolher sair.
 * Na saida grava dados.bin de novo so se algo foi removido. */
int main(void)
{
    int codigo_retorno = EXIT_SUCCESS;
    int total_carregados = 0;
    int dados_modificados = 0;
    int executando = 1;
    NoCategoria_t *lista_categorias;

    printf("=====================================================\n");
    printf("   PUCPR - Programacao Imperativa - Trabalho RA2     \n");
    printf("   Programa P2: Estruturas Dinamicas (Etapa C)       \n");
    printf("=====================================================\n\n");

    printf("Carregando alimentos de '%s' para a memoria...\n",
           ARQUIVO_DADOS_BIN);
    lista_categorias = lista_carregar_de_binario(ARQUIVO_DADOS_BIN,
                                                 &total_carregados);

    if (!lista_categorias || total_carregados == 0) {
        fprintf(stderr, "ERRO FATAL: Nao foi possivel carregar '%s'. "
                "Execute o P1 primeiro!\n", ARQUIVO_DADOS_BIN);
        codigo_retorno = EXIT_FAILURE;
    } else {
        printf("SUCESSO: %d registros carregados e organizados em listas "
               "e arvores!\n", total_carregados);

        while (executando) {
            OpcaoMenu_e opcao;

            exibir_menu_principal();
            opcao = ler_opcao_menu();

            if (opcao == OPCAO_LISTAR_CATEGORIAS) {
                lista_exibir_categorias(lista_categorias);

            } else if (opcao == OPCAO_LISTAR_ALIMENTOS) {
                NoCategoria_t *cat = selecionar_categoria(lista_categorias);
                if (cat) {
                    lista_exibir_alimentos_categoria(cat);
                }

            } else if (opcao == OPCAO_LISTAR_POR_ENERGIA) {
                NoCategoria_t *cat = selecionar_categoria(lista_categorias);
                if (cat) {
                    int cont = 0;
                    printf("\n--- ALIMENTOS DE '%s' POR ENERGIA DECRESCENTE "
                           "(ARVORE) ---\n", cat->nome);
                    arvore_listar_decrescente(cat->arvore_energia, &cont);
                    printf("Total: %d alimentos exibidos.\n"
                           "-------------------------------------------------"
                           "----------\n", cont);
                }

            } else if (opcao == OPCAO_LISTAR_POR_PROTEINA) {
                NoCategoria_t *cat = selecionar_categoria(lista_categorias);
                if (cat) {
                    int cont = 0;
                    printf("\n--- ALIMENTOS DE '%s' POR PROTEINA DECRESCENTE "
                           "(ARVORE) ---\n", cat->nome);
                    arvore_listar_decrescente(cat->arvore_proteina, &cont);
                    printf("Total: %d alimentos exibidos.\n"
                           "-------------------------------------------------"
                           "-----------\n", cont);
                }

            } else if (opcao == OPCAO_FAIXA_ENERGIA) {
                NoCategoria_t *cat = selecionar_categoria(lista_categorias);
                if (cat) {
                    double min;
                    double max;
                    int cont = 0;

                    ler_faixa("energia (kcal)", &min, &max);
                    printf("\n--- ALIMENTOS DE '%s' COM ENERGIA ENTRE %.1f "
                           "E %.1f KCAL ---\n", cat->nome, min, max);
                    arvore_listar_faixa(cat->arvore_energia, min, max, &cont);
                    printf("Total: %d alimentos encontrados na faixa.\n"
                           "-------------------------------------------------"
                           "-----------\n", cont);
                }

            } else if (opcao == OPCAO_FAIXA_PROTEINA) {
                NoCategoria_t *cat = selecionar_categoria(lista_categorias);
                if (cat) {
                    double min;
                    double max;
                    int cont = 0;

                    ler_faixa("proteina (g)", &min, &max);
                    printf("\n--- ALIMENTOS DE '%s' COM PROTEINA ENTRE %.1f "
                           "E %.1f G ---\n", cat->nome, min, max);
                    arvore_listar_faixa(cat->arvore_proteina, min, max,
                                        &cont);
                    printf("Total: %d alimentos encontrados na faixa.\n"
                           "-------------------------------------------------"
                           "----------\n", cont);
                }

            } else if (opcao == OPCAO_REMOVER_CATEGORIA) {
                NoCategoria_t *cat = selecionar_categoria(lista_categorias);
                if (cat) {
                    /* Guarda o nome antes, porque o no sera liberado. */
                    char nome_cat[TAM_MAX_DESCRICAO];
                    strncpy(nome_cat, cat->nome, sizeof(nome_cat) - 1);
                    nome_cat[sizeof(nome_cat) - 1] = '\0';

                    if (lista_remover_categoria(&lista_categorias,
                                                cat->categoria)) {
                        dados_modificados = 1;
                        printf("SUCESSO: Categoria '%s' e todos os seus "
                               "alimentos foram removidos!\n", nome_cat);
                    } else {
                        printf("ERRO: Falha ao remover categoria.\n");
                    }
                }

            } else if (opcao == OPCAO_REMOVER_ALIMENTO) {
                NoCategoria_t *cat = selecionar_categoria(lista_categorias);
                if (cat) {
                    int num_alimento;

                    lista_exibir_alimentos_categoria(cat);
                    printf("Digite o numero do alimento que deseja remover: ");
                    num_alimento = ler_inteiro(LEITURA_INVALIDA);

                    if (lista_remover_alimento(cat, num_alimento)) {
                        dados_modificados = 1;
                        printf("SUCESSO: Alimento %d removido e arvores de "
                               "'%s' reconstruidas!\n",
                               num_alimento, cat->nome);
                    } else {
                        printf("AVISO: Alimento com numero %d nao encontrado "
                               "nesta categoria.\n", num_alimento);
                    }
                }

            } else if (opcao == OPCAO_ENCERRAR) {
                if (dados_modificados) {
                    int gravados;

                    printf("\nAlteracoes detectadas. Regravando '%s' "
                           "atualizado...\n", ARQUIVO_DADOS_BIN);
                    gravados = lista_salvar_binario(lista_categorias,
                                                    ARQUIVO_DADOS_BIN);
                    if (gravados < 0) {
                        fprintf(stderr, "ERRO: Nao foi possivel gravar '%s'. "
                                "As remocoes feitas nao foram salvas.\n",
                                ARQUIVO_DADOS_BIN);
                        codigo_retorno = EXIT_FAILURE;
                    } else {
                        printf("SUCESSO: %d registros gravados na versao "
                               "atualizada de '%s'.\n",
                               gravados, ARQUIVO_DADOS_BIN);
                    }
                } else {
                    printf("\nNenhuma alteracao nos dados. O arquivo '%s' "
                           "foi mantido intacto.\n", ARQUIVO_DADOS_BIN);
                }

                printf("Liberando toda a memoria alocada (listas e "
                       "arvores)...\n");
                lista_liberar_tudo(&lista_categorias);
                printf("Memoria liberada com sucesso. Programa finalizado.\n");
                executando = 0;

            } else {
                printf("Opcao invalida! Digite um valor entre %d e %d.\n",
                       OPCAO_LISTAR_CATEGORIAS, OPCAO_ENCERRAR);
            }
        }
    }

    return codigo_retorno;
}
