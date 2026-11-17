/* alimento.h: tipo Alimento_t, enum das categorias e funcoes basicas. */

#ifndef ALIMENTO_H
#define ALIMENTO_H

/* Tamanho maximo da descricao de um alimento e do nome de uma categoria. */
#define TAM_MAX_DESCRICAO 128

/* Numero de categorias da tabela TACO. */
#define TOTAL_CATEGORIAS 15

/* Categorias em ordem alfabetica; CAT_INVALIDA marca um nome nao reconhecido. */
typedef enum {
    CAT_ALIMENTOS_PREPARADOS,
    CAT_BEBIDAS,
    CAT_CARNES_E_DERIVADOS,
    CAT_CEREAIS_E_DERIVADOS,
    CAT_FRUTAS_E_DERIVADOS,
    CAT_GORDURAS_E_OLEOS,
    CAT_LEGUMINOSAS_E_DERIVADOS,
    CAT_LEITE_E_DERIVADOS,
    CAT_MISCELANEAS,
    CAT_NOZES_E_SEMENTES,
    CAT_OUTROS_ALIMENTOS_INDUSTRIALIZADOS,
    CAT_OVOS_E_DERIVADOS,
    CAT_PESCADOS_E_FRUTOS_DO_MAR,
    CAT_PRODUTOS_ACUCARADOS,
    CAT_VERDURAS_HORTALICAS_E_DERIVADOS,
    CAT_INVALIDA
} Categoria_e;

/* Um alimento da tabela; a descricao tem tamanho fixo para o registro binario. */
typedef struct {
    int numero;
    char descricao[TAM_MAX_DESCRICAO];
    Categoria_e categoria;
    double energia_kcal;
    double proteina_g;
} Alimento_t;

/* Converte o nome da categoria do CSV para o enum. */
Categoria_e categoria_converter(const char *texto);

/* Devolve o nome da categoria. */
const char *categoria_para_texto(Categoria_e categoria);

/* Mostra os dados de um alimento em uma linha. */
void alimento_exibir(const Alimento_t *alimento);

/* Mostra um texto e completa com espacos ate a largura em letras. */
void imprimir_alinhado(const char *texto, int largura);

#endif /* ALIMENTO_H */
