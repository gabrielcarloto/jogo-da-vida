#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "uteis.h"
#include "celulas.h"

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
    matriz[i] = (char *)malloc(nc * sizeof(char));

    if (matriz[i] == NULL)
    {

      fprintf(stderr, "Erro ao alocar matriz.\n");
      exit(1);
    }
  }

  return matriz;
}

void desalocaMatriz(char **matriz, int nl)
{
  int i;

  for (i = 0; i < nl; i++)
    free(matriz[i]);

  free(matriz);
}

void limpaMatriz(char **matriz, int nl, int nc)
{
  int i, j;
  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
      matriz[i][j] = VAZ;
}

void copiaMatriz(char **matrizOriginal, char **novaMatriz, int nl, int nc)
{
  int i, j;

  for (i = 0; i < nl; i++)
    for (j = 0; j < nc; j++)
      novaMatriz[i][j] = matrizOriginal[i][j];
}

char **split(char *str, char *separador, int *contPalavras)
{
  char **retorno;
  int i, linhas;

  if (separador == "")
  {
    linhas = strlen(str);

    char **letras = alocaMatriz(linhas, 2);

    for (i = 0; i < linhas; i++)
    {
      letras[i][0] = str[i];
      letras[i][1] = '\0';
    }

    retorno = letras;
    *contPalavras = linhas;
  }
  else
  {
    linhas = TAM;

    char **palavras = alocaMatriz(linhas, TAM);
    char *palavra = strtok(str, separador);

    for (i = 0; i < linhas && palavra != NULL; i++)
    {
      strcpy(palavras[i], palavra);
      palavra = strtok(NULL, separador);
    }

    if (i < linhas)
      palavras = realloc(palavras, i * sizeof(char **));

    retorno = palavras;
    *contPalavras = i;
  }

  return retorno;
}
