#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "padroes.h"
#include "celulas.h"
#include "uteis.h"

#define PATH_PADROES "./padroes/"
#define EXT_PADROES ".csv"

void lePadrao(Padroes opcao, char **ciclo, int nl, int nc, int xInic, int yInic)
{
  FILE *arquivo;
  char string[MAX_CHARS], **padrao, endereco[MAX_CHARS] = PATH_PADROES;
  int i, j, colCelulaPadrao, celulasNaColuna;

  assert(xInic >= -1 && xInic < nc);
  assert(yInic >= -1 && yInic < nl);

  if (xInic == -1 || yInic == -1)
  {
    xInic = (nc / 2) - 1;
    yInic = (nl / 2) - 1;
  }

  switch (opcao)
  {
  case BLOCO:
    strcat(endereco, "bloco");
    break;
  case BLINKER:
    strcat(endereco, "blinker");
    break;
  case SAPO:
    strcat(endereco, "sapo");
    break;
  case GLIDER:
    strcat(endereco, "glider");
    break;
  case LWSS:
    strcat(endereco, "LWSS");
    break;
  case PULSAR:
    strcat(endereco, "pulsar");
    break;
  case FIREWORK:
    strcat(endereco, "firework");
    break;
  case FIREWORKS:
    strcat(endereco, "fireworks");
    break;
  case EXPLOSAO:
    strcat(endereco, "explosao");
    break;
  }

  strcat(endereco, EXT_PADROES);

  arquivo = fopen(endereco, "r");

  i = 0;

  while (fgets(string, MAX_CHARS, arquivo))
  {
    padrao = split(string, ",", &celulasNaColuna);

    for (j = 0; j < celulasNaColuna; j++)
    {
      colCelulaPadrao = atoi(padrao[j]);
      int linha = yInic + i, coluna = xInic + colCelulaPadrao;

      if (linha < nl && coluna < nc)
        ciclo[linha][coluna] = ORG;
    }

    i++;
    desalocaMatriz(padrao, celulasNaColuna);
  }
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
