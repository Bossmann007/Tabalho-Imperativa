#include <stdio.h>
#include <string.h>
#include "alimento.h"

/*
 * Mapeamento das strings das categorias para seus respectivos valores enum.
 */
typedef struct {
    const char *texto;
    Categoria categoria;
} MapeamentoCategoria;

static const MapeamentoCategoria TABELA_CATEGORIAS[TOTAL_CATEGORIAS] = {
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
    {"Outros alimentos industrializados", CAT_OUTROS_ALIMENTOS_INDUSTRIALIZADOS},
    {"Ovos e derivados", CAT_OVOS_E_DERIVADOS},
    {"Pescados e frutos do mar", CAT_PESCADOS_E_FRUTOS_DO_MAR},
    {"Produtos açucarados", CAT_PRODUTOS_ACUCARADOS},
    {"Verduras, hortaliças e derivados", CAT_VERDURAS_HORTALICAS_E_DERIVADOS}
};

/*
 * Converte a string de categoria lida do CSV para a constante do enum.
 * Usa busca linear sem 'break' nem 'continue', com um unico return.
 */
Categoria categoria_converter(const char *texto) {
    Categoria resultado = CAT_INVALIDA;
    int encontrado = 0;
    int i = 0;

    if (texto != NULL) {
        while (i < TOTAL_CATEGORIAS && !encontrado) {
            if (strcmp(texto, TABELA_CATEGORIAS[i].texto) == 0) {
                resultado = TABELA_CATEGORIAS[i].categoria;
                encontrado = 1;
            }
            i++;
        }
    }

    return resultado;
}

/*
 * Retorna o texto correspondente a uma constante do enum Categoria.
 * Possui um unico return no final.
 */
const char *categoria_para_texto(Categoria categoria) {
    const char *resultado = "Desconhecida";
    int encontrado = 0;
    int i = 0;

    while (i < TOTAL_CATEGORIAS && !encontrado) {
        if (TABELA_CATEGORIAS[i].categoria == categoria) {
            resultado = TABELA_CATEGORIAS[i].texto;
            encontrado = 1;
        }
        i++;
    }

    return resultado;
}

/*
 * Exibe um alimento com formatacao legivel.
 */
void alimento_exibir(const Alimento *alimento) {
    if (alimento != NULL) {
        printf("[%3d] %-45s | %-35s | %6.1f kcal | %5.1f g prot\n",
               alimento->numero,
               alimento->descricao,
               categoria_para_texto(alimento->categoria),
               alimento->energia_kcal,
               alimento->proteina_g);
    }
}
