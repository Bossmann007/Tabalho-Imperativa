/* alimento.c: conversao de categorias e exibicao de um alimento. */

#include "alimento.h"
#include <stdio.h>
#include <string.h>

/* Liga o nome de uma categoria ao seu valor no enum. */
typedef struct {
    const char *texto;
    Categoria_e categoria;
} MapeamentoCategoria_t;

static const MapeamentoCategoria_t TABELA_CATEGORIAS[TOTAL_CATEGORIAS] = {
    {"Alimentos preparados", CAT_ALIMENTOS_PREPARADOS},
    {"Bebidas (alcoólicas e não alcoólicas)", CAT_BEBIDAS},
    {"Carnes e derivados", CAT_CARNES_E_DERIVADOS},
    {"Cereais e derivados", CAT_CEREAIS_E_DERIVADOS},
    {"Frutas e derivados", CAT_FRUTAS_E_DERIVADOS},
    {"Gorduras e óleos", CAT_GORDURAS_E_OLEOS},
    {"Leguminosas e derivados", CAT_LEGUMINOSAS_E_DERIVADOS},
    {"Leite e derivados", CAT_LEITE_E_DERIVADOS},
    {"Miscelâneas", CAT_MISCELANEAS},
    {"Nozes e sementes", CAT_NOZES_E_SEMENTES},
    {"Outros alimentos industrializados",
        CAT_OUTROS_ALIMENTOS_INDUSTRIALIZADOS},
    {"Ovos e derivados", CAT_OVOS_E_DERIVADOS},
    {"Pescados e frutos do mar", CAT_PESCADOS_E_FRUTOS_DO_MAR},
    {"Produtos açucarados", CAT_PRODUTOS_ACUCARADOS},
    {"Verduras, hortaliças e derivados", CAT_VERDURAS_HORTALICAS_E_DERIVADOS}
};

/* Procura o nome na tabela e devolve a categoria, ou CAT_INVALIDA. */
Categoria_e categoria_converter(const char *texto)
{
    Categoria_e resultado = CAT_INVALIDA;
    int encontrado = 0;
    int i;

    if (texto) {
        for (i = 0; i < TOTAL_CATEGORIAS && !encontrado; i++) {
            if (strcmp(texto, TABELA_CATEGORIAS[i].texto) == 0) {
                resultado = TABELA_CATEGORIAS[i].categoria;
                encontrado = 1;
            }
        }
    }

    return resultado;
}

/* Procura a categoria na tabela e devolve o nome, ou "Desconhecida". */
const char *categoria_para_texto(Categoria_e categoria)
{
    const char *resultado = "Desconhecida";
    int encontrado = 0;
    int i;

    for (i = 0; i < TOTAL_CATEGORIAS && !encontrado; i++) {
        if (TABELA_CATEGORIAS[i].categoria == categoria) {
            resultado = TABELA_CATEGORIAS[i].texto;
            encontrado = 1;
        }
    }

    return resultado;
}

/* Mostra numero, descricao, categoria, energia e proteina. */
void alimento_exibir(const Alimento_t *alimento)
{
    if (alimento) {
        printf("[%3d] %-45s | %-35s | %6.1f kcal | %5.1f g prot\n",
               alimento->numero,
               alimento->descricao,
               categoria_para_texto(alimento->categoria),
               alimento->energia_kcal,
               alimento->proteina_g);
    }
}
