#ifndef ALIMENTO_H
#define ALIMENTO_H

#define TAM_MAX_DESCRICAO 128
#define TOTAL_CATEGORIAS 15

/*
 * Enumerador representando as 15 categorias da Tabela TACO.
 * O valor CAT_INVALIDA (-1) serve de sentinela para entradas nao reconhecidas.
 */
typedef enum {
    CAT_INVALIDA = -1,
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
    CAT_VERDURAS_HORTALICAS_E_DERIVADOS
} Categoria;

/*
 * Estrutura representando um registro de alimento da TACO.
 * descricao possui tamanho fixo para permitir gravacao e leitura
 * direta em arquivo binario (sizeof(Alimento) constante).
 */
typedef struct {
    int numero;
    char descricao[TAM_MAX_DESCRICAO];
    Categoria categoria;
    double energia_kcal;
    double proteina_g;
} Alimento;

/*
 * Converte o texto da categoria lido do CSV para o respectivo valor do enum.
 * Retorna CAT_INVALIDA se o texto nao for reconhecido.
 */
Categoria categoria_converter(const char *texto);

/*
 * Converte um valor do enum Categoria para sua descricao textual legivel.
 */
const char *categoria_para_texto(Categoria categoria);

/*
 * Exibe as informacoes formatadas de um alimento na saida padrao.
 */
void alimento_exibir(const Alimento *alimento);

#endif /* ALIMENTO_H */
