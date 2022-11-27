#include "shared.c"

#define NUM_PADROES 5

typedef enum
{
  BLOCO = 1,
  BLINKER = 2,
  SAPO = 3,
  GLIDER = 4,
  LWSS = 5,
} Padroes;

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