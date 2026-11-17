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

/* Opcao 3: alimentos de uma categoria por energia decrescente. */
void acao_listar_por_energia(NoCategoria_t *cabeca);

/* Opcao 4: alimentos de uma categoria por proteina decrescente. */
void acao_listar_por_proteina(NoCategoria_t *cabeca);

/* Opcao 5: alimentos com energia entre um minimo e um maximo. */
void acao_faixa_energia(NoCategoria_t *cabeca);

/* Opcao 6: alimentos com proteina entre um minimo e um maximo. */
void acao_faixa_proteina(NoCategoria_t *cabeca);

/* Opcao 7: remove uma categoria; devolve 1 se removeu. */
int acao_remover_categoria(NoCategoria_t **cabeca);

/* Opcao 8: remove um alimento; devolve 1 se removeu. */
int acao_remover_alimento(NoCategoria_t *cabeca);

/* Opcao 9: grava se houve remocao, libera a memoria e devolve o codigo. */
int acao_encerrar(NoCategoria_t **cabeca, int dados_modificados,
                  const char *caminho_bin);

#endif /* INTERFACE_H */
