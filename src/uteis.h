#ifndef _UTEIS_H_
#define _UTEIS_H_

#define TRUE 1
#define FALSE 0

#define TAM 101

/**
 * OVERLOADING ADAPTADO DE https://stackoverflow.com/a/26408195
 */

// get number of arguments with __NARG__
#define __NARG__(...) __NARG_I_(__VA_ARGS__, __RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, N, ...) N
#define __RSEQ_N() 1, 1, 1, 0

// general definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__))(__VA_ARGS__)

#define DEBUG0()                                                         \
  printf("Nenhum erro na linha %d do arquivo %s\n", __LINE__, __FILE__); \
  Sleep(2000)

#define DEBUG1(str, ...)                                 \
  {                                                      \
    char strToPrint[TAM];                                \
    snprintf(strToPrint, TAM, str, __VA_ARGS__);         \
    printf("%s\n%s:%d", strToPrint, __FILE__, __LINE__); \
    Sleep(2000);                                         \
  }

#define DEBUG(...) VFUNC(DEBUG, __VA_ARGS__)

/**
 * @brief Aloca uma matriz de nl linhas e nc colunas
 *
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
char **alocaMatriz(int nl, int nc);

/**
 * @brief Aloca um vetor de `indices`
 *
 * @param indices Número de índices do vetor
 * @return char *
 */
char *alocaVetor(const int indices);

/**
 * @brief Libera memória alocada pela função alocaMatriz
 *
 * @param matriz Matriz a ser liberada
 * @param nl Número de linhas
 */
void desalocaMatriz(char **matriz, int nl);

/* Preenche a matriz com o valor de VAZ */
void limpaMatriz(char **matriz, int nl, int nc);

/**
 * @brief Copia a matriz original para nova matriz (assumindo que ambas terão o mesmo tamanho)
 *
 * @param matrizOriginal Matriz que será copiada
 * @param novaMatriz Matriz que receberá os valores da matrizOriginal
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
void copiaMatriz(char **matrizOriginal, char **novaMatriz, int nl, int nc);

/**
 * @brief Separa uma string em um vetor de strings (na verdade é uma matriz, mas lide como se fosse um vetor)
 *
 * Ex.: uma string "Olá, meu nome é Pedro!" separada por " " retornará `{"Olá,", "meu", "nome", "é", "Pedro"}`
 *
 * @param str String a ser separada
 * @param separador O que vai separar a string
 * @param contPalavras Ponteiro para um int, que receberá o número de palavras (strings) contidas no retorno
 */
char **split(char *str, char *separador, int *contPalavras);
#endif