#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORG 'X'
#define VAZ '.'

/**
 * Aloca uma matriz de nl linhas e nc colunas.
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

/* Limpa a matriz, dado um caractere para ser colocado em todos os espaços. */
void limpaMatriz(char **matriz, char caractere, int nl, int nc)
{
  int i, j;
  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
      matriz[i][j] = caractere;
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
