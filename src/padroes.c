#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "padroes.h"
#include "celulas.h"
#include "uteis.h"

#define PATH_PADROES "./padroes/"
#define EXT_PADROES ".csv"

/**
 * @brief Retorna uma string contendo o caminho para o padrão escolhido.
 *
 * @param pattern Padrão escolhido
 * @return char *
 */
char *patternPath(Padroes pattern);

void lePadrao(Padroes opcao, char **ciclo, int nl, int nc, int xInic, int yInic)
{
  FILE *arquivo;
  char string[TAM], **padrao, *endereco = patternPath(opcao);
  int i, j, colCelulaPadrao, celulasNaColuna;

  assert(xInic >= -1 && xInic < nc);
  assert(yInic >= -1 && yInic < nl);

  if (xInic == -1 || yInic == -1)
  {
    xInic = (nc / 2) - 1;
    yInic = (nl / 2) - 1;
  }

  arquivo = fopen(endereco, "r");

  i = 0;

  while (fgets(string, TAM, arquivo))
  {
    padrao = split(string, ",", &celulasNaColuna);

    for (j = 0; j < celulasNaColuna; j++)
    {
      if (padrao[j][0] != '\n')
      {
        colCelulaPadrao = atoi(padrao[j]);
        int linha = yInic + i, coluna = xInic + colCelulaPadrao;

        if (linha < nl && coluna < nc)
          ciclo[linha][coluna] = ORG;
      }
    }

    i++;
    desalocaMatriz(padrao, celulasNaColuna);
  }

  free(endereco);
  fclose(arquivo);
}

char *patternPath(Padroes pattern)
{
  char *path;

  assert(pattern >= 0 && pattern <= NUM_PADROES);

  path = alocaVetor(TAM);

  strcpy(path, PATH_PADROES);

  switch (pattern)
  {
  case BLOCO:
    strcat(path, "bloco");
    break;
  case BLINKER:
    strcat(path, "blinker");
    break;
  case SAPO:
    strcat(path, "sapo");
    break;
  case GLIDER:
    strcat(path, "glider");
    break;
  case LWSS:
    strcat(path, "LWSS");
    break;
  case PULSAR:
    strcat(path, "pulsar");
    break;
  case FIREWORK:
    strcat(path, "firework");
    break;
  case FIREWORKS:
    strcat(path, "fireworks");
    break;
  case EXPLOSAO:
    strcat(path, "explosao");
    break;
  case CRESCIMENTO_INFINITO:
    strcat(path, "crescimento_infinito");
    break;
  case BIG_A:
    strcat(path, "big_a");
    break;
  }

  strcat(path, EXT_PADROES);

  return path;
}

void inicBlinker(char **m, int nL, int nC)
{
  char padrao[1][3] = {{ORG, ORG, ORG}};
  int i, j, xInic = nL / 2, yInic = nC / 2;

  for (i = 0; i < 1; i++)
    for (j = 0; j < 3; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicBloco(char **m, int nL, int nC)
{
  char padrao[2][2] = {{ORG, ORG}, {ORG, ORG}};
  int i, j, xInic = nL / 2, yInic = nC / 2;

  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicSapo(char **m, int nL, int nC)
{

  char padrao[2][4] = {{VAZ, ORG, ORG, ORG}, {ORG, ORG, ORG, VAZ}};
  int i, j, xInic = nL / 2, yInic = nC / 2;

  for (i = 0; i < 2; i++)
    for (j = 0; j < 4; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicGlider(char **m, int nL, int nC)
{
  char padrao[3][3] = {{ORG, ORG, ORG}, {ORG, VAZ, VAZ}, {VAZ, ORG, VAZ}};
  int i, j, xInic, yInic;

  xInic = nL - 4;
  yInic = nC - 4;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}

void inicLWSS(char **m, int nL, int nC)
{
  char padrao[4][5] = {{VAZ, ORG, VAZ, VAZ, ORG}, {ORG, VAZ, VAZ, VAZ, VAZ}, {ORG, VAZ, VAZ, VAZ, ORG}, {ORG, ORG, ORG, ORG, VAZ}};
  int i, j, xInic, yInic;

  xInic = nL - 5;
  yInic = nC - 6;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 5; j++)
      m[xInic + i][yInic + j] = padrao[i][j];
}
