/* interface.c: menu, leitura do teclado e acoes de cada opcao (P2). */

#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tamanho maximo de uma linha digitada pelo usuario. */
#define TAM_ENTRADA 64

/* Tamanho da linha de tracos no fim das listagens por arvore. */
#define TAM_TRACOS 60

/* Valor lido quando a linha nao tem um numero. */
#define LEITURA_INVALIDA 0

/* Funcoes usadas so neste arquivo (o comentario fica na definicao). */
static int ler_inteiro(int valor_fim_entrada);
static double ler_double(void);
static void ler_faixa(const char *grandeza, double *min, double *max);
static NoCategoria_t *selecionar_categoria(NoCategoria_t *cabeca);
static void exibir_rodape(const char *texto, int total);
static void listar_decrescente(NoCategoria_t *cabeca, int por_energia);
static void listar_faixa(NoCategoria_t *cabeca, int por_energia);

/* Mostra as opcoes de 1 a 9. */
void exibir_menu_principal(void)
{
    printf("\n==================== MENU PRINCIPAL (P2) ====================\n");
    printf("%d. Listar todas as categorias de alimentos (ordem alfabetica)\n",
           OPCAO_LISTAR_CATEGORIAS);
    printf("%d. Listar alimentos de uma categoria (ordem alfabetica)\n",
           OPCAO_LISTAR_ALIMENTOS);
    printf("%d. Listar alimentos de uma categoria por energia decrescente "
           "(arvore)\n", OPCAO_LISTAR_POR_ENERGIA);
    printf("%d. Listar alimentos de uma categoria por proteina decrescente "
           "(arvore)\n", OPCAO_LISTAR_POR_PROTEINA);
    printf("%d. Filtrar alimentos por faixa de energia [min, max] "
           "(arvore)\n", OPCAO_FAIXA_ENERGIA);
    printf("%d. Filtrar alimentos por faixa de proteina [min, max] "
           "(arvore)\n", OPCAO_FAIXA_PROTEINA);
    printf("%d. Remover uma categoria de alimentos\n",
           OPCAO_REMOVER_CATEGORIA);
    printf("%d. Remover um alimento especifico\n", OPCAO_REMOVER_ALIMENTO);
    printf("%d. Encerrar o programa (salvando alteracoes se houver)\n",
           OPCAO_ENCERRAR);
    printf("=============================================================\n");
    printf("Escolha uma opcao (%d-%d): ", OPCAO_LISTAR_CATEGORIAS,
           OPCAO_ENCERRAR);
}

/* Le um inteiro e so converte para o enum se estiver entre as opcoes. */
OpcaoMenu_e ler_opcao_menu(void)
{
    OpcaoMenu_e opcao = OPCAO_INVALIDA;
    int valor = ler_inteiro(OPCAO_ENCERRAR);

    if (valor >= OPCAO_LISTAR_CATEGORIAS && valor <= OPCAO_ENCERRAR) {
        opcao = (OpcaoMenu_e)valor;
    }

    return opcao;
}

/* Le a linha inteira com fgets e tira o numero com sscanf.
 * Se a linha nao tem numero, devolve LEITURA_INVALIDA.
 * No fim da entrada devolve valor_fim_entrada; no menu isso e
 * OPCAO_ENCERRAR, e o programa termina em vez de ficar em loop. */
static int ler_inteiro(int valor_fim_entrada)
{
    char buffer[TAM_ENTRADA];
    int valor = LEITURA_INVALIDA;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        sscanf(buffer, "%d", &valor);
    } else {
        valor = valor_fim_entrada;
    }

    return valor;
}

/* Le linhas ate achar um numero. No fim da entrada para e devolve 0.0,
 * para o programa nao ficar em loop. */
static double ler_double(void)
{
    char buffer[TAM_ENTRADA];
    double valor = 0.0;
    int lido = 0;
    int fim_entrada = 0;

    while (!lido && !fim_entrada) {
        if (fgets(buffer, sizeof(buffer), stdin)) {
            lido = (sscanf(buffer, "%lf", &valor) == 1);
            if (!lido) {
                printf("AVISO: Valor invalido! Digite um numero: ");
            }
        } else {
            fim_entrada = 1;
        }
    }

    return valor;
}

/* Pede minimo e maximo de uma grandeza, por exemplo "energia (kcal)".
 * Se o usuario digitar invertido, troca os dois: assim *min <= *max. */
static void ler_faixa(const char *grandeza, double *min, double *max)
{
    printf("Informe o valor MINIMO de %s: ", grandeza);
    *min = ler_double();
    printf("Informe o valor MAXIMO de %s: ", grandeza);
    *max = ler_double();

    if (*min > *max) {
        double temp = *min;
        *min = *max;
        *max = temp;
    }
}

/* Mostra a lista de categorias e le o numero escolhido.
 * Devolve a categoria, ou NULL se o numero for invalido
 * (no fim da entrada o numero lido e LEITURA_INVALIDA). */
static NoCategoria_t *selecionar_categoria(NoCategoria_t *cabeca)
{
    NoCategoria_t *selecionada = NULL;
    int indice;

    lista_exibir_categorias(cabeca);
    printf("Digite o numero da categoria desejada: ");
    indice = ler_inteiro(LEITURA_INVALIDA);

    selecionada = lista_obter_categoria_por_indice(cabeca, indice);
    if (!selecionada) {
        printf("AVISO: Categoria invalida!\n");
    }

    return selecionada;
}

/* Banner do P2. */
void exibir_abertura(const char *caminho_bin)
{
    printf("=====================================================\n");
    printf("   PUCPR - Programacao Imperativa - Trabalho RA2     \n");
    printf("   Programa P2: Estruturas Dinamicas (Etapa C)       \n");
    printf("=====================================================\n\n");
    printf("Carregando alimentos de '%s' para a memoria...\n", caminho_bin);
}

/* Linha de total e linha de tracos que fecham uma listagem. */
static void exibir_rodape(const char *texto, int total)
{
    int i;

    printf("Total: %d %s.\n", total, texto);
    for (i = 0; i < TAM_TRACOS; i++) {
        putchar('-');
    }
    putchar('\n');
}

/* Opcoes 3 e 4: so muda a arvore usada e o titulo.
 * por_energia = 1 usa a arvore de energia; 0 usa a de proteina. */
static void listar_decrescente(NoCategoria_t *cabeca, int por_energia)
{
    NoCategoria_t *cat = selecionar_categoria(cabeca);

    if (cat) {
        const NoArvore_t *arvore = cat->arvore_proteina;
        const char *grandeza = "PROTEINA";
        int contador = 0;

        if (por_energia) {
            arvore = cat->arvore_energia;
            grandeza = "ENERGIA";
        }
        printf("\n--- ALIMENTOS DE '%s' POR %s DECRESCENTE (ARVORE) ---\n",
               cat->nome, grandeza);
        arvore_listar_decrescente(arvore, &contador);
        exibir_rodape("alimentos exibidos", contador);
    }
}

/* Opcoes 5 e 6: le a faixa e lista pela arvore certa.
 * por_energia = 1 usa a arvore de energia; 0 usa a de proteina. */
static void listar_faixa(NoCategoria_t *cabeca, int por_energia)
{
    NoCategoria_t *cat = selecionar_categoria(cabeca);

    if (cat) {
        const NoArvore_t *arvore = cat->arvore_proteina;
        const char *pergunta = "proteina (g)";
        const char *grandeza = "PROTEINA";
        const char *unidade = "G";
        double min;
        double max;
        int contador = 0;

        if (por_energia) {
            arvore = cat->arvore_energia;
            pergunta = "energia (kcal)";
            grandeza = "ENERGIA";
            unidade = "KCAL";
        }
        ler_faixa(pergunta, &min, &max);
        printf("\n--- ALIMENTOS DE '%s' COM %s ENTRE %.1f E %.1f %s ---\n",
               cat->nome, grandeza, min, max, unidade);
        arvore_listar_faixa(arvore, min, max, &contador);
        exibir_rodape("alimentos encontrados na faixa", contador);
    }
}

/* Opcao 2. */
void acao_listar_alimentos(NoCategoria_t *cabeca)
{
    NoCategoria_t *cat = selecionar_categoria(cabeca);

    if (cat) {
        lista_exibir_alimentos_categoria(cat);
    }
}

/* Opcao 3. */
void acao_listar_por_energia(NoCategoria_t *cabeca)
{
    listar_decrescente(cabeca, 1); /* 1 = energia */
}

/* Opcao 4. */
void acao_listar_por_proteina(NoCategoria_t *cabeca)
{
    listar_decrescente(cabeca, 0); /* 0 = proteina */
}

/* Opcao 5. */
void acao_faixa_energia(NoCategoria_t *cabeca)
{
    listar_faixa(cabeca, 1); /* 1 = energia */
}

/* Opcao 6. */
void acao_faixa_proteina(NoCategoria_t *cabeca)
{
    listar_faixa(cabeca, 0); /* 0 = proteina */
}

/* Opcao 7. Copia o nome antes, porque o no da categoria e liberado. */
int acao_remover_categoria(NoCategoria_t **cabeca)
{
    int removeu = 0;
    NoCategoria_t *cat = selecionar_categoria(*cabeca);

    if (cat) {
        char nome_cat[TAM_MAX_DESCRICAO];

        strncpy(nome_cat, cat->nome, sizeof(nome_cat) - 1);
        nome_cat[sizeof(nome_cat) - 1] = '\0';
        removeu = lista_remover_categoria(cabeca, cat->categoria);
        if (removeu) {
            printf("SUCESSO: Categoria '%s' e todos os seus "
                   "alimentos foram removidos!\n", nome_cat);
        } else {
            printf("ERRO: Falha ao remover categoria.\n");
        }
    }

    return removeu;
}

/* Opcao 8. Mostra os alimentos da categoria e pede o numero. */
int acao_remover_alimento(NoCategoria_t *cabeca)
{
    int removeu = 0;
    NoCategoria_t *cat = selecionar_categoria(cabeca);

    if (cat) {
        int num_alimento;

        lista_exibir_alimentos_categoria(cat);
        printf("Digite o numero do alimento que deseja remover: ");
        num_alimento = ler_inteiro(LEITURA_INVALIDA);
        removeu = lista_remover_alimento(cat, num_alimento);
        if (removeu) {
            printf("SUCESSO: Alimento %d removido e arvores de "
                   "'%s' reconstruidas!\n", num_alimento, cat->nome);
        } else {
            printf("AVISO: Alimento com numero %d nao encontrado "
                   "nesta categoria.\n", num_alimento);
        }
    }

    return removeu;
}

/* Opcao 9. So regrava o arquivo se alguma coisa foi removida. */
int acao_encerrar(NoCategoria_t **cabeca, int dados_modificados,
                  const char *caminho_bin)
{
    int codigo_retorno = EXIT_SUCCESS;

    if (dados_modificados) {
        int gravados;

        printf("\nAlteracoes detectadas. Regravando '%s' "
               "atualizado...\n", caminho_bin);
        gravados = lista_salvar_binario(*cabeca, caminho_bin);
        if (gravados < 0) {
            fprintf(stderr, "ERRO: Nao foi possivel gravar '%s'. "
                    "As remocoes feitas nao foram salvas.\n", caminho_bin);
            codigo_retorno = EXIT_FAILURE;
        } else {
            printf("SUCESSO: %d registros gravados na versao "
                   "atualizada de '%s'.\n", gravados, caminho_bin);
        }
    } else {
        printf("\nNenhuma alteracao nos dados. O arquivo '%s' "
               "foi mantido intacto.\n", caminho_bin);
    }

    printf("Liberando toda a memoria alocada (listas e arvores)...\n");
    lista_liberar_tudo(cabeca);
    printf("Memoria liberada com sucesso. Programa finalizado.\n");

    return codigo_retorno;
}
