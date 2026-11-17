/* interface.h: menu, leitura do teclado e acoes de cada opcao (P2). */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "lista.h"

/* Opcoes do menu do P2; as validas vao de 1 a 9. */
typedef enum {
    OPCAO_INVALIDA,
    OPCAO_LISTAR_CATEGORIAS,
    OPCAO_LISTAR_ALIMENTOS,
    OPCAO_LISTAR_POR_ENERGIA,
    OPCAO_LISTAR_POR_PROTEINA,
    OPCAO_FAIXA_ENERGIA,
    OPCAO_FAIXA_PROTEINA,
    OPCAO_REMOVER_CATEGORIA,
    OPCAO_REMOVER_ALIMENTO,
    OPCAO_ENCERRAR
} OpcaoMenu_e;

/* Mostra o titulo do P2 e avisa que vai carregar o arquivo. */
void exibir_abertura(const char *caminho_bin);

/* Mostra o menu principal do P2. */
void exibir_menu_principal(void);

/* Le uma opcao do menu; fora de 1 a 9 devolve OPCAO_INVALIDA. */
OpcaoMenu_e ler_opcao_menu(void);

/* Opcao 2: alimentos de uma categoria em ordem alfabetica. */
void acao_listar_alimentos(NoCategoria_t *cabeca);

/* Opcoes 3 e 4: lista uma categoria por energia ou proteina decrescente. */
void listar_decrescente(NoCategoria_t *cabeca, int por_energia);

/* Opcoes 5 e 6: lista os alimentos de uma categoria numa faixa de energia ou proteina. */
void listar_faixa(NoCategoria_t *cabeca, int por_energia);

/* Opcao 7: remove uma categoria; devolve 1 se removeu. */
int acao_remover_categoria(NoCategoria_t **cabeca);

/* Opcao 8: remove um alimento; devolve 1 se removeu. */
int acao_remover_alimento(NoCategoria_t *cabeca);

/* Opcao 9: grava se houve remocao, libera a memoria e devolve o codigo. */
int acao_encerrar(NoCategoria_t **cabeca, int dados_modificados,
                  const char *caminho_bin);

#endif /* INTERFACE_H */
