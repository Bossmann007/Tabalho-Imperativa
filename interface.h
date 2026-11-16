/* interface.h: menu e leitura de dados do teclado (P2) e amostra do P1. */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "alimento.h"
#include "lista.h"

/* Opcao do menu que encerra o P2. Tambem e usada quando a entrada acaba. */
#define OPCAO_ENCERRAR 9

/* Mostra o primeiro, o do meio (indice 299) e o ultimo alimento do vetor. */
void exibir_amostras(const Alimento_t *vetor, int quantidade);

/* Mostra o menu principal do P2. */
void exibir_menu_principal(void);

/* Le uma linha e devolve o inteiro dela (0 se nao for numero).
 * Se a entrada acabou (EOF), devolve valor_fim_entrada. */
int ler_inteiro(int valor_fim_entrada);

/* Le uma linha e devolve o numero real dela (0.0 se nao for numero). */
double ler_double(void);

/* Pede o minimo e o maximo de uma grandeza, por exemplo "energia (kcal)".
 * Se o usuario digitar invertido, troca os dois. */
void ler_faixa(const char *grandeza, double *min, double *max);

/* Mostra as categorias e pede uma pelo numero.
 * Devolve a categoria escolhida, ou NULL se o numero for invalido. */
NoCategoria_t *selecionar_categoria(NoCategoria_t *cabeca);

#endif /* INTERFACE_H */
