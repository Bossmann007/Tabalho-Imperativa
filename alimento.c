/* alimento.c: conversao de categorias e exibicao de um alimento. */

#include "alimento.h"
#include <stdio.h>
#include <string.h>

/* Nome mostrado para uma categoria que nao esta na tabela. */
#define TEXTO_DESCONHECIDA "Desconhecida"

/* Larguras, em letras, das colunas da descricao e da categoria. */
#define LARGURA_DESCRICAO 45
#define LARGURA_CATEGORIA 35

/* Em UTF-8, os bytes de continuacao de uma letra tem a forma 10xxxxxx. */
#define UTF8_MASCARA_CONTINUACAO 0xC0
#define UTF8_BYTE_CONTINUACAO 0x80

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

/* Procura o nome na tabela e devolve a categoria. */
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

/* Procura a categoria na tabela e devolve o nome. */
const char *categoria_para_texto(Categoria_e categoria)
{
    const char *resultado = TEXTO_DESCONHECIDA;
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
        printf("[%3d] ", alimento->numero);
        imprimir_alinhado(alimento->descricao, LARGURA_DESCRICAO);
        printf(" | ");
        imprimir_alinhado(categoria_para_texto(alimento->categoria),
                          LARGURA_CATEGORIA);
        printf(" | %6.1f kcal | %5.1f g prot\n",
               alimento->energia_kcal, alimento->proteina_g);
    }
}

/* Conta as letras do texto sem os bytes de continuacao e completa com espacos. */
void imprimir_alinhado(const char *texto, int largura)
{
    int tamanho;
    int letras = 0;
    int i;

    if (texto) {
        tamanho = (int)strlen(texto);
        for (i = 0; i < tamanho; i++) {
            if (((unsigned char)texto[i] & UTF8_MASCARA_CONTINUACAO) !=
                UTF8_BYTE_CONTINUACAO) {
                letras++;
            }
        }

        printf("%s", texto);
        for (i = 0; i < largura - letras; i++) {
            putchar(' ');
        }
    }
}
