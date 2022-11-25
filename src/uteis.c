#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORG 'X'
#define VAZ '.'

#define TAM_LINHA 1024

/**
 * Aloca uma matriz de nl linhas e nc colunas
 *
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
char **alocaMatriz(int nl, int nc)
{
  int i;
  char **matriz;

  matriz = (char **)malloc(nl * sizeof(char *));

  if (matriz == NULL)
  {
    fprintf(stderr, "Erro ao alocar matriz.\n");
    exit(1);
  }

  for (i = 0; i < nl; i++)
  {
    matriz[i] = (char *)malloc(nl * sizeof(char));

    if (matriz[i] == NULL)
    {

      fprintf(stderr, "Erro ao alocar matriz.\n");
      exit(1);
    }
  }

  return matriz;
}

/**
 * Libera memória alocada pela função alocaMatriz
 *
 * @param matriz Matriz a ser liberada
 * @param nl Número de linhas
 */
void desalocaMatriz(char **matriz, int nl)
{
  int i;

  for (i = 0; i < nl; i++)
    free(matriz[i]);

  free(matriz);
}

/* Preenche a matriz com o valor de VAZ */
void limpaMatriz(char **matriz, int nl, int nc)
{
  int i, j;
  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
      matriz[i][j] = VAZ;
}

/**
 * Copia a matriz original para nova matriz (assumindo que ambas terão o mesmo tamanho)
 *
 * @param matrizOriginal Matriz que será copiada
 * @param novaMatriz Matriz que receberá os valores da matrizOriginal
 * @param nl Número de linhas
 * @param nc Número de colunas
 */
void copiaMatriz(char **matrizOriginal, char **novaMatriz, int nl, int nc)
{
  int i, j;

  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
      novaMatriz[i][j] = matrizOriginal[i][j];
}

/**
 * Separa uma string em um vetor de strings (na verdade é uma matriz, mas lide como se fosse um vetor)
 *
 * Ex.: uma string "Olá, meu nome é Pedro!" separada por " " retornará `{"Olá,", "meu", "nome", "é", "Pedro"}`
 *
 * @param str String a ser separada
 * @param separador O que vai separar a string
 * @param contPalavras Ponteiro para um int, que receberá o número de palavras (strings) contidas no retorno
 */
char **split(char *str, char *separador, int *contPalavras)
{
  char **palavras, *palavra;
  int i, linhas = TAM_LINHA;

  palavras = alocaMatriz(linhas, TAM_LINHA);
  palavra = strtok(str, separador);

  for (i = 0; i < linhas && palavra != NULL; i++)
  {
    strcpy(palavras[i], palavra);
    palavra = strtok(NULL, separador);
  }

  if (i < linhas)
    palavras = realloc(palavras, i * sizeof(char **));

  *contPalavras = i;

  return palavras;
}