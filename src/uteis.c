#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Aloca uma matriz de nl linhas e nc colunas.
 *
 * @param nl número de linhas
 * @param nc número de colunas
 */
char **alocaMatriz(int nl, int nc)
{
  int i;
  char **m;

  m = (char **)malloc(nl * sizeof(char *));

  if (m == NULL)
  {
    fprintf(stderr, "Erro ao alocar matriz.\n");
    exit(1);
  }

  for (i = 0; i < nl; i++)
  {
    m[i] = (char *)malloc(nl * sizeof(char));

    if (m[i] == NULL)
    {

      fprintf(stderr, "Erro ao alocar matriz.\n");
      exit(1);
    }
  }

  return m;
}

/**
 * Libera memória alocada pela função alocaMatriz
 *
 * @param matriz matriz a ser liberada
 * @param nl número de linhas
 */
void desalocaMatriz(char **matriz, int nl)
{
  int i;

  for (i = 0; i < nl; i++)
    free(matriz[i]);

  free(matriz);
}

/**
 * Copia a matriz anterior para matriz atual (assumindo que ambas terão o mesmo tamanho)
 *
 * @param matriz matriz a ser liberada
 * @param nl número de linhas
 * @param nc número de colunas
 */
void copiaMatriz(char **mAnt, char **mAtual, int nl, int nc)
{
    int i, j;

    for(i = 0; i < nl; i++)
        for(j = 0; j < nc; j++)
            mAtual[i][j] = mAnt[i][j];

}
