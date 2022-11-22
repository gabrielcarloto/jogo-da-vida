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