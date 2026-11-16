/* amostra.h: amostra dos registros lidos, usada so pelo P1. */

#ifndef AMOSTRA_H
#define AMOSTRA_H

#include "alimento.h"

/* Mostra o primeiro, o do meio (indice 299) e o ultimo alimento do vetor. */
void exibir_amostras(const Alimento_t *vetor, int quantidade);

#endif /* AMOSTRA_H */
