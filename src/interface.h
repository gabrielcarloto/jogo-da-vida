#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define RESET "\033[0m"
#define COR_AZUL "\033[94m"
#define COR_VERDE "\033[92m"
#define COR_CINZA "\033[90m"
#define COR_AMARELO "\033[93m"
#define COR_VERMELHO "\033[31m"
#define COR_PADRAO COR_VERDE

#define APAGA_LINHA "\x1b[2K"
#define COMECO_LINHA_ANT "\x1b[1F"

#define MIN_ESPACO_LATERAL 2
#define MIN_LARGURA_TELA 600
#define MIN_ALTURA_TELA 400

typedef enum
{
  AZUL,
  VERDE,
  AMARELO,
  VERMELHO
} Cores;

typedef enum
{
  LEFT,
  CENTER,
  RIGHT
} Sign_Alignment;

typedef struct
{
  Sign_Alignment alignment;
  int maxWidth;
  int maxHeight;
  int firstOptionIndex;
} Sign_Settings;

/**
 * @brief Converte uma cor do tipo Cores para uma string contendo o
 * escape code usado para imprimir a cor.
 *
 * @param cor Número correspondente à cor no enum Cores
 * @return (char *) String com o escape correspondente à cor
 */
char *corEscolhida(Cores cor);

/**
 * @brief Imprime o ciclo representando células mortas como um ponto cinza
 * e células vivas como um "O" da cor escolhida pelo usuário (por padrão, é verde)
 *
 * @param matriz Matriz do ciclo a ser impresso na tela
 * @param nl Número de linhas da matriz
 * @param nc Número de colunas da matriz
 */
void imprimeMatriz(char **matriz, int nl, int nc, char cor[]);

/**
 * Imprime uma "placa", ocupando todo o espaço disponível no terminal.
 * É necessário inserir um NULL ao final para não criar um loop infinito.
 * De preferência, use a macro imprimePlaca
 *
 * @param settings Configurações de exibição
 * @param str Strings a serem impressas (a primeira é o título)
 * @returns (int) Linha de início das opções
 */
int printSign(Sign_Settings settings, const char *str[]);

/**
 * Imprime uma "placa", ocupando todo o espaço disponível no terminal.
 *
 * @param config Configurações de exibição
 * @param str Strings a serem impressas (a primeira é o título)
 * @returns (int) Linha de início das opções
 */
#define imprimePlaca(config, ...) printSign((Sign_Settings)config, (const char *[]){__VA_ARGS__, NULL})

/**
 * @brief Apaga o terminal
 *
 * @param nl Número de linhas. Caso não o número de linhas seja desconhecido,
 * o valor 0 apaga todo o terminal.
 *
 * @deprecated Use a função ClearScreen()
 */
void apagaTela(int nl);
#endif