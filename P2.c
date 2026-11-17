/* P2.c: carrega dados.bin em listas e arvores e mostra o menu (Etapa C). */

#include "interface.h"
#include <stdio.h>
#include <stdlib.h>

#define ARQUIVO_DADOS_BIN "dados.bin"

/* Carrega os dados e repete o menu ate o usuario escolher sair. */
int main(void)
{
    int codigo_retorno = EXIT_SUCCESS;
    int total_carregados = 0;
    int dados_modificados = 0;
    OpcaoMenu_e opcao = OPCAO_INVALIDA;
    NoCategoria_t *lista_categorias;

    exibir_abertura(ARQUIVO_DADOS_BIN);
    lista_categorias = lista_carregar_de_binario(ARQUIVO_DADOS_BIN,
                                                 &total_carregados);

    if (!lista_categorias || total_carregados == 0) {
        fprintf(stderr, "ERRO FATAL: Nao foi possivel carregar '%s'. "
                "Execute o P1 primeiro!\n", ARQUIVO_DADOS_BIN);
        codigo_retorno = EXIT_FAILURE;
    } else {
        printf("SUCESSO: %d registros carregados e organizados em listas "
               "e arvores!\n", total_carregados);
    }

    while (codigo_retorno == EXIT_SUCCESS && opcao != OPCAO_ENCERRAR) {
        exibir_menu_principal();
        opcao = ler_opcao_menu();

        if (opcao == OPCAO_LISTAR_CATEGORIAS) {
            lista_exibir_categorias(lista_categorias);
        } else if (opcao == OPCAO_LISTAR_ALIMENTOS) {
            acao_listar_alimentos(lista_categorias);
        } else if (opcao == OPCAO_LISTAR_POR_ENERGIA) {
            acao_listar_por_energia(lista_categorias);
        } else if (opcao == OPCAO_LISTAR_POR_PROTEINA) {
            acao_listar_por_proteina(lista_categorias);
        } else if (opcao == OPCAO_FAIXA_ENERGIA) {
            acao_faixa_energia(lista_categorias);
        } else if (opcao == OPCAO_FAIXA_PROTEINA) {
            acao_faixa_proteina(lista_categorias);
        } else if (opcao == OPCAO_REMOVER_CATEGORIA) {
            if (acao_remover_categoria(&lista_categorias)) {
                dados_modificados = 1;
            }
        } else if (opcao == OPCAO_REMOVER_ALIMENTO) {
            if (acao_remover_alimento(lista_categorias)) {
                dados_modificados = 1;
            }
        } else if (opcao == OPCAO_ENCERRAR) {
            codigo_retorno = acao_encerrar(&lista_categorias,
                                           dados_modificados,
                                           ARQUIVO_DADOS_BIN);
        } else {
            printf("Opcao invalida! Digite um valor entre %d e %d.\n",
                   OPCAO_LISTAR_CATEGORIAS, OPCAO_ENCERRAR);
        }
    }

    return codigo_retorno;
}
