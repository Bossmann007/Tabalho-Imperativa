#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alimento.h"
#include "arvore.h"
#include "lista.h"

#define ARQUIVO_DADOS_BIN "dados.bin"
#define TAM_ENTRADA 64

/*
 * Exibe o menu principal de opcoes do programa P2.
 */
static void exibir_menu_principal(void) {
    printf("\n==================== MENU PRINCIPAL (P2) ====================\n");
    printf("1. Listar todas as categorias de alimentos (ordem alfabetica)\n");
    printf("2. Listar alimentos de uma categoria (ordem alfabetica)\n");
    printf("3. Listar alimentos de uma categoria por energia decrescente (arvore)\n");
    printf("4. Listar alimentos de uma categoria por proteina decrescente (arvore)\n");
    printf("5. Filtrar alimentos por faixa de energia [min, max] (arvore)\n");
    printf("6. Filtrar alimentos por faixa de proteina [min, max] (arvore)\n");
    printf("7. Remover uma categoria de alimentos\n");
    printf("8. Remover um alimento especifico\n");
    printf("9. Encerrar o programa (salvando alteracoes se houver)\n");
    printf("=============================================================\n");
    printf("Escolha uma opcao (1-9): ");
}

/*
 * Le um numero inteiro da entrada padrao com seguranca.
 */
static int ler_inteiro(void) {
    char buffer[TAM_ENTRADA];
    int valor = 0;
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%d", &valor);
    }
    return valor;
}

/*
 * Le um numero real (double) da entrada padrao com seguranca.
 */
static double ler_double(void) {
    char buffer[TAM_ENTRADA];
    double valor = 0.0;
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%lf", &valor);
    }
    return valor;
}

/*
 * Solicita ao usuario que selecione uma categoria pelo indice da lista.
 */
static NoCategoria *selecionar_categoria(NoCategoria *cabeca) {
    NoCategoria *selecionada = NULL;

    lista_exibir_categorias(cabeca);
    printf("Digite o numero da categoria desejada: ");
    int indice = ler_inteiro();

    selecionada = lista_obter_categoria_por_indice(cabeca, indice);
    if (selecionada == NULL) {
        printf("AVISO: Categoria invalida!\n");
    }

    return selecionada;
}

/*
 * P2.c - Programa 2 da Etapa C (Trabalho RA2 - PUCPR).
 * Apenas contem a funcao main e funcoes auxiliares de interface local.
 */
int main(void) {
    int codigo_retorno = EXIT_SUCCESS;
    int total_carregados = 0;
    int dados_modificados = 0;
    int executando = 1;

    printf("=====================================================\n");
    printf("   PUCPR - Programacao Imperativa - Trabalho RA2     \n");
    printf("   Programa P2: Estruturas Dinamicas (Etapa C)       \n");
    printf("=====================================================\n\n");

    printf("Carregando alimentos de '%s' para a memoria...\n", ARQUIVO_DADOS_BIN);
    NoCategoria *lista_categorias = lista_carregar_de_binario(ARQUIVO_DADOS_BIN, &total_carregados);

    if (lista_categorias == NULL || total_carregados == 0) {
        fprintf(stderr, "ERRO FATAL: Nao foi possivel carregar '%s'. Execute o P1 primeiro!\n",
                ARQUIVO_DADOS_BIN);
        codigo_retorno = EXIT_FAILURE;
    } else {
        printf("SUCESSO: %d registros carregados e organizados em listas e arvores!\n",
               total_carregados);

        /* Loop principal controlado por variavel booleana (sem break/continue) */
        while (executando) {
            exibir_menu_principal();
            int opcao = ler_inteiro();

            if (opcao == 1) {
                /* 1. Listar todas as categorias em ordem alfabetica */
                lista_exibir_categorias(lista_categorias);

            } else if (opcao == 2) {
                /* 2. Listar alimentos de certa categoria em ordem alfabetica */
                NoCategoria *cat = selecionar_categoria(lista_categorias);
                if (cat != NULL) {
                    lista_exibir_alimentos_categoria(cat);
                }

            } else if (opcao == 3) {
                /* 3. Listar alimentos por energia decrescente via arvore binaria */
                NoCategoria *cat = selecionar_categoria(lista_categorias);
                if (cat != NULL) {
                    int cont = 0;
                    printf("\n--- ALIMENTOS DE '%s' POR ENERGIA DECRESCENTE (ARVORE) ---\n", cat->nome);
                    arvore_listar_decrescente(cat->arvore_energia, &cont);
                    printf("Total: %d alimentos exibidos.\n-----------------------------------------------------------\n", cont);
                }

            } else if (opcao == 4) {
                /* 4. Listar alimentos por proteina decrescente via arvore binaria */
                NoCategoria *cat = selecionar_categoria(lista_categorias);
                if (cat != NULL) {
                    int cont = 0;
                    printf("\n--- ALIMENTOS DE '%s' POR PROTEINA DECRESCENTE (ARVORE) ---\n", cat->nome);
                    arvore_listar_decrescente(cat->arvore_proteina, &cont);
                    printf("Total: %d alimentos exibidos.\n------------------------------------------------------------\n", cont);
                }

            } else if (opcao == 5) {
                /* 5. Listar alimentos por faixa de energia [min, max] via arvore */
                NoCategoria *cat = selecionar_categoria(lista_categorias);
                if (cat != NULL) {
                    printf("Informe o valor MINIMO de energia (kcal): ");
                    double min = ler_double();
                    printf("Informe o valor MAXIMO de energia (kcal): ");
                    double max = ler_double();

                    if (min > max) {
                        double temp = min;
                        min = max;
                        max = temp;
                    }

                    int cont = 0;
                    printf("\n--- ALIMENTOS DE '%s' COM ENERGIA ENTRE %.1f E %.1f KCAL ---\n", cat->nome, min, max);
                    arvore_listar_faixa(cat->arvore_energia, min, max, &cont);
                    printf("Total: %d alimentos encontrados na faixa.\n------------------------------------------------------------\n", cont);
                }

            } else if (opcao == 6) {
                /* 6. Listar alimentos por faixa de proteina [min, max] via arvore */
                NoCategoria *cat = selecionar_categoria(lista_categorias);
                if (cat != NULL) {
                    printf("Informe o valor MINIMO de proteina (g): ");
                    double min = ler_double();
                    printf("Informe o valor MAXIMO de proteina (g): ");
                    double max = ler_double();

                    if (min > max) {
                        double temp = min;
                        min = max;
                        max = temp;
                    }

                    int cont = 0;
                    printf("\n--- ALIMENTOS DE '%s' COM PROTEINA ENTRE %.1f E %.1f G ---\n", cat->nome, min, max);
                    arvore_listar_faixa(cat->arvore_proteina, min, max, &cont);
                    printf("Total: %d alimentos encontrados na faixa.\n-----------------------------------------------------------\n", cont);
                }

            } else if (opcao == 7) {
                /* 7. Remover uma categoria inteira */
                NoCategoria *cat = selecionar_categoria(lista_categorias);
                if (cat != NULL) {
                    Categoria cat_id = cat->categoria;
                    char nome_cat[TAM_MAX_DESCRICAO];
                    strncpy(nome_cat, cat->nome, sizeof(nome_cat) - 1);
                    nome_cat[sizeof(nome_cat) - 1] = '\0';

                    int removido = lista_remover_categoria(&lista_categorias, cat_id);
                    if (removido) {
                        dados_modificados = 1;
                        printf("SUCESSO: Categoria '%s' e todos os seus alimentos foram removidos!\n",
                               nome_cat);
                    } else {
                        printf("ERRO: Falha ao remover categoria.\n");
                    }
                }

            } else if (opcao == 8) {
                /* 8. Remover um alimento especifico (com reconstrucao das arvores) */
                NoCategoria *cat = selecionar_categoria(lista_categorias);
                if (cat != NULL) {
                    lista_exibir_alimentos_categoria(cat);
                    printf("Digite o numero do alimento que deseja remover: ");
                    int num_alimento = ler_inteiro();

                    int removido = lista_remover_alimento(cat, num_alimento);
                    if (removido) {
                        dados_modificados = 1;
                        printf("SUCESSO: Alimento %d removido e arvores de '%s' reconstruidas!\n",
                               num_alimento, cat->nome);
                    } else {
                        printf("AVISO: Alimento com numero %d nao encontrado nesta categoria.\n",
                               num_alimento);
                    }
                }

            } else if (opcao == 9) {
                /* 9. Encerramento do programa */
                if (dados_modificados) {
                    printf("\nAlteracoes detectadas. Regravando '%s' atualizado...\n", ARQUIVO_DADOS_BIN);
                    int gravados = lista_salvar_binario(lista_categorias, ARQUIVO_DADOS_BIN);
                    printf("SUCESSO: %d registros gravados na versao atualizada de '%s'.\n",
                           gravados, ARQUIVO_DADOS_BIN);
                } else {
                    printf("\nNenhuma alteracao nos dados. O arquivo '%s' foi mantido intacto.\n",
                           ARQUIVO_DADOS_BIN);
                }

                printf("Liberando toda a memoria alocada (listas e arvores)...\n");
                lista_liberar_tudo(&lista_categorias);
                printf("Memoria liberada com sucesso. Programa finalizado.\n");
                executando = 0;

            } else {
                printf("Opcao invalida! Digite um valor entre 1 e 9.\n");
            }
        }
    }

    return codigo_retorno;
}
